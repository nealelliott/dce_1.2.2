/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1996, 1990 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>			/* Should be always first */
#include <cm.h>				/* Cm-based standard headers */
#include <cm_vcache.h>
#include <cm_stats.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/cm/RCS/cm_vcache.c,v 4.20 1996/03/07 20:16:53 cfe Exp $")

/*
 * This file contains the virtual cache operations; these are not vnode-like in 
 * that the functions to call are embedded here. Enough is enough, as they say. We
 * should virtualize them sometime, perhaps, if there really are a lot of different
 * types of cache files that people want to insinuate into AFS 4.
 */

/* generic cache type info */
static int cm_maxCacheType=0;
int cm_diskless = 0;
static int cm_cacheType = -1;
static struct cm_cacheOps *cm_cacheTypes[CM_MAXCACHETYPES];

int cm_RegisterCacheType(aops)
    register struct cm_cacheOps *aops; 
{
    register int slot;

    if (cm_maxCacheType >= CM_MAXCACHETYPES-1)
	return -1;
    cm_cacheTypes[slot = (cm_maxCacheType++)] = aops;
    return slot;
}


void cm_SetCacheType(aslot)
    register int aslot; 
{
    if (aslot >= 0 && aslot < cm_maxCacheType) 
	cm_cacheType = aslot;
    else 
	panic("bogus setcachetype");
}

#ifdef AFS_VFSCACHE
char *cm_CFileOpen(handle)
struct fid * handle;
{
    /* return a generic file structure */
    return (*(cm_cacheTypes[cm_cacheType]->open))(handle);
}
#else
char *cm_CFileOpen(ainode)
    register long ainode; 
{
    return (*(cm_cacheTypes[cm_cacheType]->open))(ainode);
}
#endif /* AFS_VFSCACHE */


int cm_CFileTruncate(afile, asize)
    register long asize;
    register char *afile; 
{
    return (*(cm_cacheTypes[cm_cacheType]->truncate))(afile, asize);
}


int cm_CFileClose(afile)
    register char *afile; 
{
    return (*(cm_cacheTypes[cm_cacheType]->close))(afile);
}


int cm_CFileRW(afile, arw, abase, aoffset, alen, acredp)
    char *afile;
    enum uio_rw arw;
    char *abase;
    long aoffset;
    long alen;
    osi_cred_t *acredp; 
{
    struct uio tuio;
    struct iovec tiovec;

    /* 
     * Setup base UIO structure pointing to one io vector 
     */
    osi_InitUIO(&tuio);
    tuio.osi_uio_iov = &tiovec;
    tuio.osi_uio_iovcnt = 1;
    tuio.osi_uio_offset = aoffset;
    tuio.osi_uio_resid = alen;
    tuio.osi_uio_seg = OSI_UIOSYS;	/* kernel space for this one */
#ifndef AFS_OSF_ENV
    tuio.osi_uio_fmode = (arw == UIO_READ ? FREAD : FWRITE);
#endif
    tiovec.iov_base = abase;
    tiovec.iov_len = alen;

    if (cm_CFileRDWR(afile, arw, &tuio, acredp))
	return -1;	/* something went wrong */

#ifdef CM_ENABLE_COUNTERS
    if (arw == UIO_READ)
	CM_BUMP_COUNTER_BY(dataCacheBytesReadFromCache, (alen - tuio.osi_uio_resid));
    else
	CM_BUMP_COUNTER_BY(dataCacheBytesWrittenToCache, (alen - tuio.osi_uio_resid));
#endif /* CM_ENABLE_COUNTERS */

    /* 	
     * Otherwise, return the # of bytes actually read or written 
     */

    return (alen - tuio.osi_uio_resid);
}


/* 
 * arw is one of UIO_READ or UIO_WRITE, auio is a uio structure, acredp is the
 * required credential structure, and afile is the generic file object.
 */
int cm_CFileRDWR(afile, arw, auio, acredp)
    char *afile;
    enum uio_rw arw;
    struct uio *auio;
    osi_cred_t *acredp; 
{
    return (*(cm_cacheTypes[cm_cacheType]->rdwr))(afile, arw, auio, acredp);
}
