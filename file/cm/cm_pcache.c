/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1990, 1994 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>		/* Should be always first */
#include <cm.h>				/* Cm-based standard headers */
#include <cm_dcache.h>
#include <cm_vcache.h>
#include <cm_bkg.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/cm/RCS/cm_pcache.c,v 10.4 1995/03/10 23:58:00 andi Exp $")

/*
 * This file contains the real procedures behind the virtual ones listed in 
 * cm_vcache.c. It contains them for both UFS- and memory-based caching systems.
 */

/* 
 * There is an array of memory cache elems pointed to by memCache, which are treated 
 * as if they were raw inodes. They contain a size field and a pointer to some data.
 * When we open a memory cache file, we return a pointer to one of these structures.
 */
struct memCacheEntry {
    int size;  		    		/* # of valid bytes in this entry */
    int dataSize;  			/* size of allocated data area */
    char *data;    			/* bytes */
};
static struct lock_data cm_memCacheLock;
static int memCacheSize;		/* always the same as afiles? */
static struct memCacheEntry *memCache;	/* the cache itself */
static int memCacheBlockSize;		/* the chunk size */

#ifdef AFS_VFSCACHE
static char *cm_UFSCacheOpen _TAKES((struct fid *));
static char *cm_MemCacheOpen _TAKES((struct fid *));
#else
static char *cm_UFSCacheOpen _TAKES((long));
static char *cm_MemCacheOpen _TAKES((long));
#endif /* AFS_VFSCACHE */
static int cm_UFSCacheClose _TAKES((char *));
static int cm_UFSCacheTruncate _TAKES((char *, long));
static int cm_UFSCacheRDWR _TAKES((char *, enum uio_rw, struct uio *,
				   osi_cred_t *));
static int cm_MemCacheClose _TAKES((char *));
static int cm_MemCacheTruncate _TAKES((char *, long));
static int cm_MemCacheRDWR _TAKES((char *, enum uio_rw, struct uio *,
				   osi_cred_t *));


/*
 * Here are the basic UFS functions 
 */
struct cm_cacheOps cm_UFSCacheOps = {
    cm_UFSCacheOpen,
    cm_UFSCacheTruncate,
    cm_UFSCacheRDWR,
    cm_UFSCacheClose,
};

#ifdef AFS_VFSCACHE
static char *cm_UFSCacheOpen(handle)
struct fid *handle;
{
    return (char *) osi_Open(cm_cache_mp, handle);
}
#else
static char *cm_UFSCacheOpen(ainode)
    register long ainode; 
{
    return (char *) osi_Open(&cacheDev, ainode);
}
#endif /* AFS_VFSCACHE */


static int cm_UFSCacheTruncate(afile, asize)
    char *afile;
    long asize; 
{
    struct osi_stat tostat;
    int code;

    /* cm_CFileTruncate only shrinks files, and most systems
     * have very slow truncates, even when the file is already
     * small enough.  Check now and save some time.
     */
    code = osi_Stat((struct osi_file *)afile, &tostat);
    if (code) return code;
    if (tostat.size <= asize) return 0;

    return osi_Truncate((struct osi_file *)afile, asize);

}


static int cm_UFSCacheRDWR(afile, arw, auio, acredp)
    char *afile;
    enum uio_rw arw;
    struct uio *auio;
    osi_cred_t *acredp; 
{
    register long code;

    code = osi_RDWR((struct osi_file *)afile, arw, auio);
    if (code) {
	cm_printf("dfs: cache I/O failed, code = %d\n", code);
    }
    return code;
}


static int cm_UFSCacheClose(afile)
    char *afile; 
{
    return osi_Close((struct osi_file *)afile);
}


/*
 * Here are the basic memcache versions of the CFile functions 
 */
struct cm_cacheOps cm_MemCacheOps = {
    cm_MemCacheOpen,
    cm_MemCacheTruncate,
    cm_MemCacheRDWR,
    cm_MemCacheClose,
};
long cm_cacheDirty, cm_maxCacheDirty;


#ifdef AFS_VFSCACHE
static char *cm_MemCacheOpen(handle)
struct fid * handle;
{
    int entnum;

    entnum = (int)handle->fid_len;
    if (entnum < 0 || entnum >= memCacheSize)
	panic("bad memcache index");
    return (char *) &(memCache[entnum]);
}
#else
static char *cm_MemCacheOpen(ainode)
    register long ainode; 
{
    if (ainode < 0 || ainode >= memCacheSize)
	panic("bad memcache index");
    return (char *) &(memCache[ainode]);
}
#endif /* AFS_VFSCACHE */


static int cm_MemCacheTruncate(afile, asize)
    char *afile;
    register long asize; 
{
    register struct memCacheEntry *mcep = (struct memCacheEntry *) afile;

    /* 
     * Used to be that a cache file could have been more than memCacheBlockSize
     * in size, but that can't happen any more (it was in AFS 3 only). It occurred
     * only because we did whole-file transfers of directories.
     */
    lock_ObtainWrite(&cm_memCacheLock);
    if (asize < mcep->size) {
	bzero(mcep->data + asize, mcep->size - asize);
	mcep->size = asize;
    }
    lock_ReleaseWrite(&cm_memCacheLock);
    return 0;
}


static int cm_MemCacheRDWR(afile, arw, auio, acredp)
    char *afile;
    enum uio_rw arw;
    register struct uio *auio;
    osi_cred_t *acredp; 
{
    register struct memCacheEntry *mcp = (struct memCacheEntry *)afile;
    register long bytesMoved;
    register long offset, xfrSize;
    long code = 0;

    if ((offset = auio->osi_uio_offset) < 0) 
	return 0;

    xfrSize = auio->osi_uio_resid;
    if (arw == UIO_READ) {
	lock_ObtainRead(&cm_memCacheLock);
	/* use min of bytes in buffer or requested size */
	bytesMoved =
	    (xfrSize < mcp->size - offset) ? xfrSize : mcp->size-offset;
	if (bytesMoved > 0)
	    code = osi_uiomove(mcp->data + offset, bytesMoved, UIO_READ, auio);
	lock_ReleaseRead(&cm_memCacheLock);
    } else {			/* a write, we may extend the block */
	lock_ObtainWrite(&cm_memCacheLock);
	if (xfrSize + offset > memCacheBlockSize) 
	    panic("big memcache write");
	else 
	    code = osi_uiomove(mcp->data+auio->osi_uio_offset, xfrSize,
			       UIO_WRITE, auio);
	if (offset+xfrSize > mcp->size)
	    mcp->size = offset+xfrSize;
	lock_ReleaseWrite(&cm_memCacheLock);
    }
    return code;
}


static int cm_MemCacheClose(afile)
    char *afile; 
{
    return 0;
}


/* 
 * afiles is # of files in cache (# of memory blocks), and asize
 * is the desired size (the chunk size, essentially).
 */
void cm_InitMemCache(afiles, asize)
    long afiles;
    long asize; 
{
    register struct memCacheEntry *tmp;
    register long i;
    register struct cm_dcache *tdc;

    lock_Init(&cm_memCacheLock);

    tmp = (struct memCacheEntry *) osi_Alloc(afiles * sizeof(struct memCacheEntry));
    bzero((char *) tmp, afiles*sizeof(struct memCacheEntry));
    i = cm_RegisterCacheType(&cm_MemCacheOps);
    if (i<0) 
	panic("register cacheops");
    cm_SetCacheType(i);

    memCache = tmp;
    memCacheSize = afiles;
    memCacheBlockSize = asize;

    for (i = 0; i < afiles; i++, tmp++) {
	tmp->dataSize = asize;
	tmp->data = osi_Alloc(asize);
    }

    /* now initialize all the dcache entries properly */
    lock_ObtainWrite(&cm_dcachelock);
    for (i = 0; i < afiles; i++) {
	tdc = cm_GetDSlot(i, 0);
	if (!tdc) 
	    panic("memcache dcache init");
#ifdef AFS_VFSCACHE
	tdc->f.handle.fid_len = i;	/* really memcache slot # */
#else
	tdc->f.inode = i;	/* really memcache slot # */
#endif /* AFS_VFSCACHE */
	tdc->f.fid.Vnode = 0;		/* not in the hash table */
	tdc->f.hvNextp = freeDCList;	/* put entry in free cache slot list */
	freeDCList = i;
	freeDCCount++;
	cm_indexFlags[i] |= DC_IFFREE;
	tdc->f.states &= ~DC_DWRITING;
	cm_indexFlags[i] &= ~DC_IFENTRYMOD;
	tdc->refCount--;		/* we're done now */
	cm_cacheCounter++;
    }
    lock_ReleaseWrite(&cm_dcachelock);
}
