/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* volume_table.c -- contains procedures which manage volume tables.  Primarily
 *     this is used by the volume module but the AVL is also a volume table of
 *     sorts so the aggregate module needs these facilities to allocate new
 *     volumes. */

/* Copyright (C) 1996, 1990 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <dcedfs/episode/logbuf.h>
#include <anode.h>
#include <tlock.h>
#include <anode_block.h>
#include <fixed_anode.h>
#include <block_alloc.h>
#include <volume_table.h>

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/anode/RCS/volume_table.c,v 4.70 1996/03/04 19:02:23 ota Exp $")

#define DEBUG_THIS_FILE EPIA_DEBUG_VOLUME_TABLE

/* Locking Note: Some of these fields are not protected by the lock since they
 * are effectively static, namely: bsize and anodesPerBlock.  The handleLock
 * protects the allocation of new handles as well as the handle refCount.
 *
 * The volTable lock is *NOT* a tlock, we depend on aggregate or volume tlocks
 * to accomplish the needed ECMerge when doing volume or anode deletes,
 * respectively. */

#define MAX_ANODES_PER_VOLUME 10000000	/* is this reasonable? */

/* For size restrictions during truncate we have to allow for indirect blocks
 * to be freed unexpectedly during a truncate.  Conservatively, we assume one
 * page in any group could free all the indirect blocks at the edge to the
 * highest possible tree crossing.  Given the above limit on anodes per volume
 * and assuming worst case of a 1K block size, we might need 2.5M vt pages.
 * From basic.test we see that freeing the first page in tree 2 and the last
 * page in tree 1 could free 7 blocks, 5 of which are indirect. */
#define MAX_INDIRECTBLOCKS_PER_TRUNCATE 5

static osi_dlock_t handleLock;

#define MAPENTYPE unsigned short
#define MAPUNKNOWN 0xffff		/* don't know about this page */

struct volTable {
    epi_mutex_t lock;			/* not a tlock! */
    u_long firstIndex;			/* first legal index to used */
    u_long lastIndex;			/* index of highest used anode */
    u_long nFree;			/* number of unused anodes */
    u_long nUsed;			/* number used */
    /* if pages==0 then volume is deleted or never created. */
    u_long pages;			/* number of pages in table */
    u_long volIndex;			/* index of volume in AVL */
    long   magic;			/* magic number for anode blocks */
    u_long bsize;			/* bytes per page */
    u_int  anodesPerBlock;		/* max value of any byte in map */
    long mruPage;			/* page last modified */
    long mruBlock;			/* fs block associated with mruPage */
    u_long mapSize;			/* entries in map */
    MAPENTYPE *map;			/* free anodes in each page */
};

/* set vt to good looking volume table handle from volC or zero. */

#define GetOKVT(vt,volC) \
    (((volC) && (vt = (struct volTable *)epia_GetVolumeRock(volC)) && \
      (vt->volIndex == epia_GetIndex(volC)) && \
      vt->bsize && vt->anodesPerBlock && \
      (vt->magic == ANODE_BLOCK_AVL_MAGIC || \
       vt->magic == ANODE_BLOCK_VOLUME_MAGIC)) \
     ? vt : (vt = 0))

#define OKVT(vt,volC) \
    {   struct epia_info info; \
	long length; \
	if (code = epia_GetInfo (volC, &info)) return code; \
	if (!epia_GetVolumeRock(volC)) return EPI_E_NOINIT; \
	if (!GetOKVT (vt, volC)) return EPI_E_NOTAVOLUME; \
	length = AFS_hgetlo(info.length); \
	if (!AFS_hfitsinu32(info.length) || (length % vt->bsize)) \
	    return EPI_E_NOTAVOLUME; \
	if (vt->pages == 0) return EPI_E_NOTAVOLUME; \
	if (vt->map == 0) { \
	    long code; \
	    code = CheckVolumeTablePages(vt, volC); \
	    if (code) return code; \
	} \
    }

static void Cleanup(epi_anode_t volC)
{
    struct volTable *vt;
    epix_EnterCodeBlock(17);
    GetOKVT(vt, volC);
    if (vt) {
	afsl_Assert (!epi_mutex_held(&vt->lock)); /* better not be lockers */
	if (vt->map)
	    osi_Free_r (vt->map, vt->mapSize * sizeof(*(vt->map)));
	epi_mutex_destroy (&vt->lock);
	osi_Free_r (vt, sizeof(*vt));
    }
    epia_SetVolumeRock (volC, 0, 0);
}

#ifndef KERNEL
/* Reschedule -- see file.c */

PRIVATE void Reschedule ()
{
    if (afsdb_episode_anode & (1<<(EPIA_DEBUG_ADDPREEMPTS-1))) {
	osi_Reschedule ();
    }
}
#else
#define Reschedule()
#endif

/* epit_Init -- Initializes the fileset table module.  It is called only once
 *     during system initialization by \fcname{epig_Init}, see which. */

/* EXPORT */
long epit_Init ()
{
    static int inited = 0;
    if (inited) return 0;
    inited++;
    afsl_SetFileNumber();
    lock_InitDesc (&handleLock, "volume table handles");

#ifdef AFS_DEBUG
    /* CodeBlock: epia */
    epia_codeBlock[DEBUG_THIS_FILE][0] = 26;
#endif

    return 0;
}

/* CheckVolumeTablePages -- examines every page of a volume table, checks it
 *     for consistency and counts the free and allocated anodes, filling in the
 *     vt->map structure in the process. */

static CheckVolumeTablePages(
  IN struct volTable *vt,
  IN epi_anode_t volC)
{
    long code = 0;

    BEGIN_MUTEX (&vt->lock) {
	long index;
	long p;				/* current page number */
	struct elbb_buffer *buf;

	if (vt->map) continue;		/* already is one... */
	epix_EnterCodeBlock(7);

	vt->mruPage = -1;
	vt->mruBlock = EPIX_ANODE_BLOCKEMPTY;

	for (vt->mapSize=16; vt->pages>=vt->mapSize; vt->mapSize += 16);
	vt->map = osi_Alloc_r (vt->mapSize * sizeof(*(vt->map)));
	for (p=0; p<vt->mapSize; p++) vt->map[p] = MAPUNKNOWN;

	index = 0;
	for (p=0; p<vt->pages; p++) {
	    BEGIN_CRIT (code = epix_GetBuffer (volC, p, &buf),
			code = elbb_Release (buf, elbb_lazy)) {
		char *page = elbb_BufferData (buf);
		long  o;		/* offset in page of anode */

		vt->map[p] = 0;
		afsl_AssertCond(epic_AnodeBlockOkay(buf, 0, vt->volIndex));
		index = p*vt->anodesPerBlock;
		for (o = epic_FirstOffset(); epic_LegalOffset(o, vt->bsize);
		     o = epic_NextOffset(o)) {
		    if ((index >= vt->firstIndex) &&
			(*(int32 *)(page + o) == 0)) {
			vt->map[p]++;
			vt->nFree++;
		    }
		    else {
			vt->nUsed++;
			vt->lastIndex = index;
		    }
		    index++;
		}
		if (vt->map[p] && ((vt->mruPage == -1) ||
				   (vt->map[p] > vt->map[vt->mruPage]))) {
		    vt->mruPage = p;
		    vt->mruBlock = elbb_BufferBlock(buf);
		}
	    } END_CRIT;
	    if (code == EPI_E_NOBLOCK) {
		/* just pass on unallocated pages */
		epix_EnterCodeBlock(21);
		vt->map[p] = MAPUNKNOWN;
		vt->nFree += vt->anodesPerBlock;
		if (p == 0) {		/* first page can't be unallocated */
		    code = EPI_E_NOTAVOLUME;
		    break;
		} else code = 0;
	    }
	    else if (code) break;
	}
	if (code) continue;
	afsl_Assert (vt->pages * vt->anodesPerBlock == vt->nFree + vt->nUsed);
    } END_LOCK;
    return code;
}

/* epit_InitVolumeTable -- Sets up a fileset anode table handle given the
 *     container \arg{volC} and \arg{reservedIndexes}, the number of
 *     anode indexes to reserve. This must be called before most of the
 *     routines in this module, such as \fcname{epit_Allocate},
 *     \fcname{epit_Deflate}, or \fcname{epit_EnumerateVolumeTable}.
 *
 *     It does not examine the contents of the VT at all; particularly it
 *     allows the length to be zero(0).  This is important since this routine
 *     is called from epit_Create to setup the vt handle.  Looking at each page
 *     is done on demand by CheckVolumeTablePages (above).
 */

/* EXPORT */
long epit_InitVolumeTable (volC, reservedIndexes)
  IN epi_anode_t volC;
  IN u_int reservedIndexes;
{
    long code;
    struct epia_info info;
    struct volTable *vt;
    u_int logbsize;
    u_long bsize;
    int type;
    u_long length;

    epix_EnterCodeBlock(1);
    code = epia_GetInfo (volC, &info);
    if (code) return code;
    if (epia_GetVolumeRock(volC)) return 0; /* already done? */
    if (!AFS_hfitsinu32(info.length)) return EPI_E_NOTAVOLUME;
    length = AFS_hgetlo(info.length);
    logbsize = epia_AnodeDevice(volC)->logBlkSize;
    bsize = epia_AnodeDevice(volC)->blkSize;
    if (length & (bsize-1)) return EPI_E_NOTAVOLUME;
    type = epia_GetType (volC);
    if ((type != EPIA_CFLAGS_AVL) && (type != EPIA_CFLAGS_VOLUME))
	return EPI_E_NOTAVOLUME;

    lock_ObtainWrite_r (&handleLock);
    if (epia_GetVolumeRock(volC)) { /* Check for race... */
	epix_EnterCodeBlock(2);
	lock_ReleaseWrite_r (&handleLock);
	return 0;
    }

    vt = osi_Alloc_r(sizeof(*vt));
    bzero((caddr_t)vt, sizeof (*vt));
    epi_mutex_init (&vt->lock, "volume table");
    BEGIN_MUTEX (&vt->lock) {
	epia_SetVolumeRock (volC, vt, Cleanup);
	lock_ReleaseWrite_r (&handleLock);

	vt->volIndex = info.index;
	if (type == EPIA_CFLAGS_AVL) vt->magic = ANODE_BLOCK_AVL_MAGIC;
	else vt->magic = ANODE_BLOCK_VOLUME_MAGIC;
	vt->bsize = bsize;
	vt->pages = length >> logbsize;
	vt->anodesPerBlock = epic_anodesPerBlock[logbsize-10];
	vt->firstIndex = reservedIndexes;
	vt->lastIndex = 0;
	vt->nFree = vt->nUsed = 0;
    } END_LOCK;

    GetOKVT(vt, volC);
    afsl_Assert(vt);
    return 0;
}

/* FindLastIndex -- recalculates the last used index in a volume table.  Calls
 *     truncate to flush unused pages at end of container.
 *
 * LOCKS -- Must be called with the volTable write locked.
 *
 * CAUTIONS -- Due to restrictions on the max size of transactions, and on
 *     transactions that free blocks in particular, this operation can only
 *     reduce the size of a volume table by a restricted amount on a single
 *     call.  Consequently, it may leave the container much larger than it
 *     should be.
 */

static long FindLastIndex(
  IN elbb_tranRec_t trans,
  IN struct volTable *vt,
  IN epi_anode_t volC)
{
    long code;
    struct elbb_buffer *buf;
    u_long index;			/* index of first anode on last page */
    int nPagesRemoved;			/* number of pages truncated so far */

    epix_EnterCodeBlock(10);
    afsl_Assert (epi_mutex_held(&vt->lock) &&
		 (vt->pages > 0));
    nPagesRemoved = 0;
    while ((vt->map[vt->pages-1] == MAPUNKNOWN) ||
	   (vt->map[vt->pages-1] == vt->anodesPerBlock)) {
	/* truncate last page */
	/* Assume tlock on volume header is write-locked when an anode is
         * deleted.  This assures that recent transactions that modified them
         * get equivalenced to this one so they all get rolled back together in
         * a crash. */
        afs_hyper_t length;

	/* Only a limited number of pages can be removed at a time. */
	if (nPagesRemoved >=
	    EPIB_MAX_FREEDBLOCKS_PER_TRANS-MAX_INDIRECTBLOCKS_PER_TRUNCATE) {
	    epix_EnterCodeBlock(18);
	    break;
	}
        epix_EnterCodeBlock(11);
        AFS_hset64(length, 0, (vt->pages-1)*vt->bsize);
        code = epia_Truncate (trans, volC, 0, length);
        if (code) return code;
        vt->pages--;
        vt->nFree -= vt->anodesPerBlock;
	if (vt->mruPage == vt->pages) {
	    vt->mruPage = -1;		/* invalidate mruBlock */
	    vt->mruBlock = EPIX_ANODE_BLOCKEMPTY;
	}
        nPagesRemoved++;
    }
    afsl_Assert (vt->pages > 0);
    index = (vt->pages-1)*vt->anodesPerBlock;
    if ((vt->map[vt->pages-1] == MAPUNKNOWN) ||
	(vt->map[vt->pages-1] == vt->anodesPerBlock)) {
	vt->lastIndex = index - 1;
	code = 0;
    } else {
	/* now search last page */
	BEGIN_CRIT (code = epix_GetBuffer (volC, vt->pages-1, &buf),
		    code = elbb_Release (buf, elbb_lazy)) {
	    char *page = elbb_BufferData(buf);
	    long o;			/* offset in page of anode */
	    int i;			/* index within page */

	    afsl_AssertCond(epic_AnodeBlockOkay(buf, 0, vt->volIndex));
	    for (i=0, o = epic_FirstOffset(); epic_LegalOffset(o, vt->bsize);
		 i++, o = epic_NextOffset(o)) {
		if (index+i < vt->firstIndex || *(int32 *)(page + o) != 0) {
		    vt->lastIndex = index+i; /* in-use anode */
		}
	    }
	} END_CRIT;
    }
    return code;
}

/* NewPage -- allocates and initializes a new page of a volume table.  When
 *     this routine returns successfully it is always true that (vt->pages >
 *     p).  The withhold argument may be set to the RWFLAGS which allow the
 *     block allocator to use withheld blocks.
 *
 * LOCKS USED -- Must be called with the volTable write locked. */

static long NewPage(
  IN elbb_tranRec_t trans,
  IN epi_anode_t volC,			/* volume table to add page to */
  IN struct volTable *vt,
  IN long p,				/* index of needed page */
  IN int withhold)			/* use withholding */
{
    long code;
    struct uio uio;
    struct elbb_buffer *buf;

    if ((p < vt->pages) && (vt->map[p] != MAPUNKNOWN))
	return 0;			/* already exitst */

    epix_EnterCodeBlock(3);
    afsl_Assert (epi_mutex_held(&vt->lock) &&
		 (p >= 0) && ((p >= vt->pages) || (vt->map[p] == MAPUNKNOWN)));

    /* Allocate a new block using epia_Write then call epic_FormatAnodeBlock to
     * initialize it.  Once it's allocated (and the quota checks are passed)
     * there is no turning back. */

    epia_InitUio (&uio, 0, p*vt->bsize, vt->bsize, /*buffer*/0);
    code = epia_Write (trans, volC, withhold, &uio);
    if (code) return code;
    Reschedule();

    BEGIN_CRIT (code = epix_GetBuffer (volC, p, &buf),
		code = elbb_Release (buf, elbb_lazy)) {
	code = epic_FormatAnodeBlock (trans, &buf, vt->volIndex, vt->magic);
	if (code) continue;
    } END_CRIT;
    afsl_MBZ (code);
    Reschedule();

    if (p >= vt->mapSize) {		/* grow map if necessary */
	/* Note that only the elements (0<=p<vt->pages) are valid. */
	long newMapSize = p+16;
	long newBytes = newMapSize * sizeof(*(vt->map));
	MAPENTYPE *newMap = osi_Alloc_r(newBytes);
	long oldBytes = vt->mapSize*sizeof(*(vt->map));
	int i;
	epix_EnterCodeBlock(4);
	bcopy((caddr_t)vt->map, (caddr_t)newMap, oldBytes);
	osi_Free_r (vt->map, oldBytes);
	for (i=vt->mapSize; i<newMapSize; i++) newMap[i] = MAPUNKNOWN;
	vt->map = newMap;
	vt->mapSize = newMapSize;
    }
    vt->map[p] = vt->anodesPerBlock;	/* fully empty */
    vt->nFree += vt->anodesPerBlock;
    if (p >= vt->pages) {
	int i;
	/* zap intervening pages */
	for (i=vt->pages; i<p; i++) {
	    vt->map[i] = MAPUNKNOWN;
	    vt->nFree += vt->anodesPerBlock;
	}
	vt->pages = p+1;
    }
    return 0;
}

/* epit_Allocate -- Allocate an index in the fileset anode table \arg{volC}
 *     as close to \arg{nearest}, if it is non-zero, as possible.  The new
 *     index is returned in \arg{indexP}.  The companion function is
 *     \fcname{epit_Free}.
 *
 * PARAMETERS -- flags is used to specify the desired type of the index,
 *     whether for a file or auxilary container or don't care.  It also
 *     specifies whether the newly allocated blocks should consume the block
 *     allocator's withhold reserve. */

/* EXPORT */
long epit_Allocate (trans, volC, flags, nearest, indexP)
  IN elbb_tranRec_t trans;
  IN epi_anode_t volC;
  IN int flags;
  IN u_long  nearest;			/* nearby index, if possible */
  OUT u_long *indexP;
{
    long code = 0;
    struct volTable *vt;
    long index;				/* anode index */
    long p;				/* best page w/ free anodes */
    daddr_t block;			/* fs block corresponding to "p" */
    int pDelta;				/* stride for page search */
    int pOdd;				/* low bit of page number */
    long freest;			/* page w/ most free anode */
    int firsti;				/* best index in page to check */
    int type;				/* locate index for file or aux */
    int withhold;			/* okay to use withholding */

    epix_EnterCodeBlock(5);
    afsl_Trace (AFSL_TR_ENTRY, ("allocate volC=%x type=%d nearest=%u",
				volC, type, nearest));
    *indexP = 0;
    OKVT (vt, volC);

    type = flags & (EPIT_TYPE_FILE|EPIT_TYPE_AUX);
    if (epit_useOldFidIndex)
	type = EPIT_TYPE_DONTCARE;
    withhold = ((flags & EPIT_DONT_WITHHOLD) ? EPIA_RWFLAGS_NOWITHHOLD : 0);

    switch (type) {
      case EPIT_TYPE_DONTCARE: pDelta = 1; pOdd = 0; break;
      case EPIT_TYPE_FILE:     pDelta = 2; pOdd = 0; break;
      case EPIT_TYPE_AUX:      pDelta = 2; pOdd = 1; break;
      default:
	return EPI_E_BADARGS;
    }
#define MAKE_GOOD_PAGE(p) ((p&~(pDelta-1)) | pOdd)

    BEGIN_MUTEX (&vt->lock) {
	if (vt->pages == 0) { ABORT_CRIT (EPI_E_NOTAVOLUME) }
	/* use hint if possible */
	if (nearest != 0) {
	    /* it would be nice if we could validate the index, but failing
             * that we'll just grow the volume to be big enough. */
	    epix_EnterCodeBlock(6);
	    if (nearest > MAX_ANODES_PER_VOLUME)
		{ ABORT_CRIT (EPI_E_BADARGS) }
	    p = nearest / vt->anodesPerBlock;
	    afsl_Assert (p == MAKE_GOOD_PAGE(p));
	    code = NewPage (trans, volC, vt, p, withhold);
	    if (code) continue;
	    if (vt->map[p]) {
		firsti = nearest % vt->anodesPerBlock;
		goto found_page;
	    }
	    epix_EnterCodeBlock(22);
	    nearest = 0;
	    firsti = 0;
	}

	/* No hint we can use so pick most recently used page or a fairly free
         * page or make a new page */

	p = vt->mruPage;
	if ((p != -1) && (p < vt->pages) && (p == MAKE_GOOD_PAGE(p)) &&
	    (vt->map[p] != MAPUNKNOWN) && (vt->map[p] > 0))
	    goto found_page;
	    
	freest = -1;
	for (p=MAKE_GOOD_PAGE(0); p<vt->pages; p+=pDelta) {
	    if ((vt->map[p] != MAPUNKNOWN) &&
		(vt->map[p] > (u_short)((freest == -1) ? 0 : vt->map[freest])))
		freest=p;
	}
	if (freest == -1) {
	    /* no existing pages w/ space, so check for unallocated page */
	    epix_EnterCodeBlock(23);
	    for (p=MAKE_GOOD_PAGE(0); p<vt->pages; p+=pDelta) {
		if (vt->map[p] == MAPUNKNOWN) freest=p;
	    }
	}
	if (freest == -1) {
	    /* otherwise extend VT */
	    epix_EnterCodeBlock(24);
	    freest = vt->pages;
	    while (freest != MAKE_GOOD_PAGE(freest)) freest++;
	}
	p = freest;
	code = NewPage (trans, volC, vt, p, withhold);
	if (code) continue;
	firsti = 0;

found_page:
	if ((p == vt->mruPage) && (vt->mruBlock != EPIX_ANODE_BLOCKEMPTY))
	    block = vt->mruBlock;
	else {
	    code = epia_Map (volC, p, &block, NULL);
	    if (code) continue;
	    vt->mruPage = p;
	    vt->mruBlock = block;
	}
	index = p*vt->anodesPerBlock;
	BEGIN_LCRIT (struct elbb_buffer *buf,
		     code = elbb_Read (epia_AnodeDevice(volC), block,
				       elbb_bufLogged, &buf),
		     code = elbb_Release (buf, elbb_lazy)) {
	    int   i;			/* index in this page */
	    char *page = elbb_BufferData(buf);
	    long  o;			/* offset in page of anode */
	    afsl_AssertCond(epic_AnodeBlockOkay(buf, 0, vt->volIndex));

	    if (nearest != 0) {		/* first, try best choice */
		o = epic_IndexToOffset(firsti);
		if (*(int32 *)(page + o) == 0) {
		    epix_EnterCodeBlock(8);
		    index += firsti;
		    goto found_index;
		}
	    }

	    for (i=0, o = epic_FirstOffset();
		 epic_LegalOffset (o, vt->bsize);
		 index++, i++, o = epic_NextOffset(o)) {
		if (index < vt->firstIndex) continue;
		if (*(int32 *)(page + o)  == 0) {
		    int32 flags;
		    epix_EnterCodeBlock(9);
found_index:
		    flags = ANODE_MAGIC;
		    code = elbb_Modify (buf, o, (caddr_t)&flags, sizeof(flags),
					trans, !elbb_IsZeroTrans(trans), &buf);
		    if (code) break;
		    *indexP = index;
		    vt->map[p]--;
		    vt->nFree--;
		    vt->nUsed++;
		    if (index > vt->lastIndex) vt->lastIndex = index;
		    break;
		}
	    }
	    afsl_PAssert (*indexP,
			  ("In volC %#x page %d claims %d free but is full",
			   volC, p, vt->map[p]));
	} END_CRIT;
	if (code) continue;
	if (vt->map[vt->pages-1] == vt->anodesPerBlock)
	    /* maybe shrink volTable */
	    code = FindLastIndex (trans, vt, volC);
    } END_LOCK; /* lock vt */
    if (code) return code;
    afsl_Trace (AFSL_TR_EXIT, ("allocated index %u", *indexP));
    return 0;
}

/* epit_Free -- Deallocates the anode in fileset anode table \arg{volC} at
 *     \arg{index}.
 *
 * LOCKS USED -- We assume that a tlock is held by the caller at a granularity
 *     large enough to encompass the whole page containing each anode. */

/* EXPORT */
long epit_Free (trans, volC, index)
  IN elbb_tranRec_t trans;
  IN epi_anode_t volC;
  IN u_long index;
{
    long code;
    struct elbb_buffer *buf;
    struct volTable *vt;
    long o;				/* offset in page of anode */
    long p;				/* page containing index */

    epix_EnterCodeBlock(12);
    OKVT (vt, volC);

    BEGIN_MUTEX (&vt->lock) {
	if (vt->pages == 0) { ABORT_CRIT (EPI_E_NOTAVOLUME) }
	if ((index < vt->firstIndex) || (index > vt->lastIndex))
	    { ABORT_CRIT (EPI_E_BADARGS) }
	p = index / vt->anodesPerBlock;
	afsl_Assert ((p < vt->pages) && (vt->map[p] < vt->anodesPerBlock));
	o = epic_IndexToOffset (index % vt->anodesPerBlock);

	BEGIN_CRIT (code = epix_GetBuffer (volC, p, &buf),
		    code = elbb_Release (buf, elbb_lazy)) {
	    char *page = elbb_BufferData(buf);
	    int32 flags;		/* flags from anode being freed */

	    afsl_AssertCond (epic_AnodeBlockOkay (buf, 0, vt->volIndex));
	    flags = *(int32 *)(page + o);
	    if (flags == 0)
		code = EPI_E_NOENT;
	    else if (!epic_LegalAnode(page+o))
		code = EPI_E_NOTAVOLUME; /* something's wrong... */
	    if (code) continue;
	    afsl_PAssert ((flags == ANODE_MAGIC),
			  ("Freeing inuse anode index %d in volTable#%d",
			   index, epia_GetIndex(volC)));
	    flags = 0;			/* free it */
	    code = elbb_Modify (buf, o, (caddr_t)&flags, sizeof(flags),
				trans, !elbb_IsZeroTrans(trans), &buf);
	    if (code) continue;
	    vt->mruPage = p;
	    vt->mruBlock = elbb_BufferBlock(buf);
	} END_CRIT;
	if (code) continue;
	vt->map[p]++;
	vt->nFree++;
	vt->nUsed--;
	if ((index == vt->lastIndex) ||
	    (vt->map[vt->pages-1] == vt->anodesPerBlock))
	    /* maybe shrink volTable */
	    code = FindLastIndex (trans, vt, volC);
    } END_LOCK; /* lock vt */
    return code;
}

/* epit_EnumerateVolumeTable -- Iterates through the fileset table and calls
 *     the procedure \arg{proc} for each allocated anode.  It is called
 *     with three arguments: the specifed \arg{rock}, the \arg{volC},
 *     and the epia-style index of the anode.  If it returns a non-zero result
 *     the iteration stops and this value is returned to the caller.
 *
 * LOCKS USED -- The volume table is read locked during this call including
 *     each call to proc.  This prevents modifications to the volume table. */

/* EXPORT */
long epit_EnumerateVolumeTable (volC, proc, rock)
  IN epi_anode_t volC;
  IN long (*proc)();			/* call for each entry */
  IN char *rock;			/* pass through parameter */
{
    long code = 0;
    struct volTable *vt;
    struct elbb_buffer *buf;
    long p;				/* current page number */
    int nFreePage;			/* number free on current page */
    u_long nFree;			/* number free in whole volume table */
    u_long nUsed;			/* number used in whole volume table */
    u_long index;
    long result;			/* value returned by proc */

    epix_EnterCodeBlock(13);
    OKVT (vt, volC);

    nFree = nUsed = 0;
    result = 0;
    index = 0;
    BEGIN_MUTEX (&vt->lock) {
	for (p=0; p<vt->pages; p++) {
	    if (vt->map[p] == MAPUNKNOWN) {
		nFree += vt->anodesPerBlock;
		continue;
	    }
	    nFreePage = 0;
	    BEGIN_CRIT (code = epix_GetBuffer (volC, p, &buf),
			code = elbb_Release (buf, elbb_lazy)) {
		char *page = elbb_BufferData (buf);
		long  o;		/* offset in page of anode */

		afsl_AssertCond (epic_AnodeBlockOkay (buf, 0, vt->volIndex));
		for (o = epic_FirstOffset(); epic_LegalOffset(o, vt->bsize);
		     o = epic_NextOffset(o)) {
		    if (index < vt->firstIndex) {
			nUsed++;
		    }
		    else if (*(int32 *)(page + o) == 0) nFreePage++;
		    else {
			result = (*proc)(rock, volC, index);
			if (result) break;
			nUsed++;
		    }
		    index++;
		}
	    } END_CRIT;
	    if (code || result) break;
	    afsl_Assert (vt->map[p] == nFreePage);
	    nFree += nFreePage;
	} /* for each page */
    } END_LOCK;
    if (code) return code;
    afsl_Assert (result || (vt->nFree == nFree && vt->nUsed == nUsed &&
			    vt->pages*vt->anodesPerBlock == nFree+nUsed));
    return result;
}

/* epit_Create -- Creates a (normal) fileset anode table.  Use
 *     \fcname{epit_CreateAVL} to create an AVL anode table.  It takes an empty
 *     container in \arg{volC} and formats it appropriately for use as a
 *     fileset, reserving \arg{reservedIndexes} anode slots at the
 *     beginning for the fileset header (in practice this number is always two
 *     (2) for a fileset).
 *
 * LOCKS USED -- This operation is protected by a write lock on the aggregate
 *     which is held during volume creation, of which this is part.  Although
 *     how this fact is used is not clear. */

/* EXPORT */
long epit_Create (trans, volC, reservedIndexes)
  IN elbb_tranRec_t trans;
  IN epi_anode_t volC;
  IN u_long reservedIndexes;
{
    long code;
    struct volTable *vt;

    epix_EnterCodeBlock(14);
    code = epit_InitVolumeTable (volC, reservedIndexes);
    if (code) return code;
    GetOKVT(vt, volC);
    afsl_AssertTruth(vt);
    code = CheckVolumeTablePages (vt, volC);
    if (code) return code;

    if (reservedIndexes == 0) return 0;
    if (reservedIndexes > vt->anodesPerBlock) return EPI_E_BADARGS;

    /* otherwise preallocate indexes used by volume header */

    BEGIN_MUTEX (&vt->lock) {
	code = NewPage (trans, volC, vt, 0L, EPIA_RWFLAGS_NOWITHHOLD);
	if (code) continue;
	vt->firstIndex = reservedIndexes;
	vt->lastIndex = reservedIndexes-1;
	vt->map[0] -= reservedIndexes;
	vt->nFree -= reservedIndexes;
	vt->nUsed += reservedIndexes;
    } END_LOCK;
    return code;
}

/* epit_Delete -- Deletes a (normal) fileset table in \arg{volC}.  The
 *     fileset must be empty; if the fileset occupies more than one page the
 *     function \fcn{epit_Deflate} must be called first.  The parameter
 *     \arg{reservedIndexes} is provided to initialize the fileset, if
 *     necessary; the caller does not need to do this first (although this is
 *     probably a useless feature since the caller always has to call
 *     \fcn{epit_Deflate} first anyway and that routine requires that the
 *     fileset be initialized).
 *
 * LOCKS USED -- This operation is protected by a write lock on the aggregate
 *     which is held during volume deletion, of which this is part.  Although
 *     how this fact is used is not clear. */

/* EXPORT */
long epit_Delete (trans, volC, reservedIndexes)
  IN elbb_tranRec_t trans;
  IN epi_anode_t volC;
  IN u_int reservedIndexes;
{
    long code;
    struct volTable *vt;

    epix_EnterCodeBlock(19);
    code = epit_InitVolumeTable (volC, reservedIndexes);
    if (code) return code;
    OKVT (vt, volC);
    BEGIN_MUTEX (&vt->lock) {
	afs_hyper_t length;
	if (vt->pages == 0) { ABORT_CRIT (EPI_E_NOTAVOLUME) }
	if ((vt->pages > 1) || (vt->nUsed > vt->firstIndex))
	    { ABORT_CRIT (EPI_E_NOTEMPTY) }
	AFS_hzero (length);
	code = epia_Truncate (trans, volC, 0, length);
	if (code) continue;
	vt->pages = 0;			/* (vt->pages == 0) => vol deleted */
    } END_LOCK;
    return code;
}

/* epit_CreateAVL -- Creates the aggregate volume list.  It takes the
 *     \arg{device} and \arg{avlAnodeBlock}, the superblock block address.  The
 *     number of reserved anodes at the beginning of the AVL for use as the
 *     aggregate header (a.k.a. the superblock) is also specified, in
 *     \arg{reservedIndexes}.  This is also the anode index of the AVL anode
 *     itself (in practice this is always one (1)).  That anode is created at
 *     this index during the call to \fcname{epix_CreateAVL} which does much of
 *     the work of this procedure.
 *
 *     The anode handle for the AVL, initialized as an anode table, is return
 *     in \arg{avlP}.
 *
 *     To create a fileset anode table call \fcname{epit_Create} instead.
 *
 * LOCKS USED -- This operation is protected by the global aggregate creation
 *     lock. */

/* EXPORT */
long epit_CreateAVL (device, avlAnodeBlock, reservedIndexes, avlP)
  IN struct async_device *device;
  IN daddr_t avlAnodeBlock;
  IN u_long reservedIndexes;
  OUT epi_anode_t *avlP;
{
    long code;
    long bsize = device->blkSize;
    u_long avlIndex = reservedIndexes;	/* index to use for AVL */
    struct volTable *vt;
    afs_hyper_t length;

    epix_EnterCodeBlock(15);

    /* This also formats the first page of the AVL */
    code = epix_CreateAVL (device, avlAnodeBlock, avlIndex, avlP);
    if (code) return code;
    AFS_hset64(length, 0, bsize);
    code = epix_SetLength (elbb_zeroTrans, *avlP, length, 0);
    if (code) return code;
    code = epit_InitVolumeTable (*avlP, reservedIndexes);
    if (code) return code;
    GetOKVT(vt, *avlP);
    code = CheckVolumeTablePages (vt, *avlP);
    if (code) return code;

    afsl_Assert (vt && (vt->pages == 1) && (vt->lastIndex == avlIndex));
    return 0;
}

/* epit_LastIndex -- Sets \arg{indexP} to be the value of the last index
 *     used within the fileset table \arg{volC}. */

/* EXPORT */
long epit_LastIndex (volC, indexP)
  IN epi_anode_t volC;
  OUT u_long *indexP;
{
    long code;
    struct volTable *vt;

    epix_EnterCodeBlock(16);
    OKVT (vt, volC);

    *indexP = vt->lastIndex;
    return 0;
}

/* epit_Deflate -- Truncates empty pages from the end of a fileset anode table
 *     in \arg{volC}.  This must be called, perhaps repeatedly, on a
 *     fileset before it can be deleted.  It takes a transaction and does some
 *     amount of work.  If the fileset anode table is not completely deflated
 *     it will return EPI_E_NOTDONE.  When this code is returned the caller
 *     should end current transaction, start a new one and make the call again.
 *     This routine is called from \fcname{epiv_Deflate}. */

/* EXPORT */
long epit_Deflate (trans, volC)
  IN elbb_tranRec_t trans;
  IN epi_anode_t volC;
{
    long code = 0;
    struct volTable *vt;

    epix_EnterCodeBlock(20);
    OKVT (vt, volC);

    BEGIN_MUTEX (&vt->lock) {
	if ((vt->map[vt->pages-1] == MAPUNKNOWN) ||
	    (vt->map[vt->pages-1] == vt->anodesPerBlock)) {
	    /* shrink volTable */
	    code = FindLastIndex (trans, vt, volC);
	    if (!code && ((vt->map[vt->pages-1] == MAPUNKNOWN) ||
			  (vt->map[vt->pages-1] == vt->anodesPerBlock)))
		code = EPI_E_NOTDONE;
	} else code = 0;
    } END_LOCK;
    return code;
}

#ifndef KERNEL
#include <verify.h>
#include <com_err.h>

/* epit_VerifyVolumeTable -- iterates through the volume table and calls the
 *     specified procedure for each allocated entry.  It is basically like the
 *     enumerate function, above, but is much more tolerant of errors.
 *
 * PARAMETERS -- The function "proc" is called with three arguments: the rock,
 *     the volC, and the index of the entry.  If this routine returns a
 *     non-zero value the iteration stops and that value is returned by this
 *     function.
 *
 * LOCKS USED -- don't bother during salvage.
 *
 * TODO -- do we need to have a routine to delete failing entries?
 */

/* SHARED */
long epit_VerifyVolumeTable (ss, volC, proc, rock)
  IN struct epiz_salvageState *ss;
  IN epi_anode_t volC;
  IN long (*proc)();			/* call for each entry */
  IN char *rock;			/* pass through parameter */
{
    long code;
    struct volTable *vt;
    struct elbb_buffer *buf;
    long p;				/* current page number */
    int nFreePage;			/* number free on current page */
    u_long nFree;			/* number free in whole volume table */
    u_long nUsed;			/* number used in whole volume table */
    u_long index;
    long result;			/* value returned by proc */
    long magic;
    u_long volIndex;			/* index of this volume table in AVL */
    char id[21];			/* msg identifying volume table */
    epi_anode_t *anodeP;		/* reset current anode handle */
    u_long *indexP;			/* track current index */

    epix_EnterCodeBlock(25);
    if (!GetOKVT (vt, volC)) return EPI_E_NOINIT;
    if (vt->pages == 0) return EPI_E_NOTAVOLUME;

    volIndex = epia_GetIndex (volC);
    if (volC == ss->aggrAnode) {
	magic = ANODE_BLOCK_AVL_MAGIC;
	strcpy (id, "AVL");
	anodeP = &ss->vol;
	indexP = &ss->volIx;
    } else {
	magic = ANODE_BLOCK_VOLUME_MAGIC;
	id[sizeof(id)-1] = 0;
	sprintf (id, "volume @ %d", volIndex);
	afsl_MBZ ((long)(id[sizeof(id)-1]));
	ss->vol = volC;
	anodeP = &ss->anode;
	indexP = &ss->anodeIx;
    }

    *indexP = 0;
    *anodeP = 0;
    nFree = nUsed = 0;
    result = 0;
    index = 0;
    for (p=0; p<vt->pages; p++) {
	nFreePage = 0;
	BEGIN_CRIT (code = epix_GetBuffer (volC, p, &buf),
		    code = elbb_Release (buf, elbb_lazy)) {
	    char *page = elbb_BufferData (buf);
	    long  o;			/* offset in page of anode */

	    ss->loc.index = p;
	    ss->loc.block = elbb_BufferBlock(buf);
	    if (ss->pass == EPIZ_SS_SCANPASS) ss->nAnodePages++;

	    if (!epic_AnodeBlockOkay (buf, magic, vt->volIndex)) {

		/* This page is apparently no good. */

		epiz_ReportError (ss, 0, EPIZ_E_BADANODEBLOCK, p);
		if (ss->flags & EPIZ_SS_SALVAGE) {
		    code = epic_FormatAnodeBlock (elbb_NullTranId,
						  &buf, volIndex, magic);
		    if (code) continue;
		    afsl_Assert (epic_AnodeBlockOkay (buf, magic,
						      vt->volIndex));
		} else {
		    /* not much we can do here, so punt this page. */
		    ABORT_CRIT (EPI_E_NOBLOCK);
		}
	    }
	    index = p*vt->anodesPerBlock;
	    for (o = epic_FirstOffset(); epic_LegalOffset(o, vt->bsize);
		 o = epic_NextOffset(o)) {

		*indexP = index;
		if (index < vt->firstIndex) {
		    nUsed++;
		}
		else if (*(int32 *)(page + o) == 0) nFreePage++;
		else {
		    nUsed++;
		    /* If this routine returns a non-zero error code we report
                     * an error, and, if salvaging, delete the anode. */
		    result = (*proc)(rock, volC, index);
		    if (result) {
			epiz_ReportError (ss, result, EPIZ_E_DEADANODE);
			if (ss->flags & EPIZ_SS_SALVAGE) {
			    int32 flags = 0; /* 0 means it's free */
			    code = elbb_Modify (buf, o, (caddr_t)&flags,
						sizeof(flags),
						elbb_NullTranId, 0, &buf);
			    if (code) continue;
			    nFreePage++;
			    nUsed--;
			}
		    }
		    *anodeP = 0;
		}
		index++;
	    }
	} END_CRIT;
	if (code == EPI_E_NOBLOCK) {
	    epix_EnterCodeBlock(26);
	    if (p == 0) {		/* first page can't be unallocated */
		code = EPI_E_NOTAVOLUME;
		break;
	    } else code = 0;
	    nFreePage = vt->anodesPerBlock;
	}
	else if (code) break;
	nFree += nFreePage;
    }					/* for each page */
    if (code) return code;
    afsl_Assert (vt->pages*vt->anodesPerBlock == nFree+nUsed);
    *indexP = 0;
    return 0;
}

#ifdef SCAN_SALVAGER
/* Procedure for guessing whether a given anode block looks like a block of a volume. */
/* EXPORT */
long epit_BlockScoreInt(recon, ix, data, typeP, scoreP, Recurse, AVLIx)
struct epiz_ReconState *recon;
u_long ix;
opaque data;
long *typeP, *scoreP;
int Recurse;
u_int32 AVLIx;
{/* evaluate whether the given block looks like a block of a volume. */
    long score, type, code, FirstBlockScore, possScore;
    long subScore, subType;
    long offs;				/* offset in anodeBlock */
    struct diskVolumeHeader *VolHdr;
    int IsFirstBlock, CharIx;
    int NumAnodes, TotScore, VolFirstPage, SawLog, SawBitmap, CurrIx;
    int32 anodeType, anodeIndex;
    char *NamePtr;

    FirstBlockScore = 0;
/* This could legally be either the first page of the AVL (the superblock) or some other page. */
/* Does it look like an aggr header? */
    VolHdr = (struct diskAggregateHeader *) (((char *)data) + epic_FirstOffset());
    if (VolHdr->magic == VOLUME_MAGIC) ++FirstBlockScore;
    else --FirstBlockScore;
    if (VolHdr->name[sizeof(VolHdr->name)-1] == '\0') {
	/* assert that the VolHdr->name is null-terminated and all the rest of the name is null. */
	CharIx = strlen(VolHdr->name);	/* index of name's first NUL */
	for (; CharIx < sizeof(VolHdr->name); ++CharIx) {
	    if (VolHdr->name[CharIx] != '\0') break;
	}
	if (CharIx >= sizeof(VolHdr->name)) ++FirstBlockScore;
	else --FirstBlockScore;
    }
    if (AVLIx != 0) {
	if (VolHdr->index == AVLIx) ++FirstBlockScore;
	else --FirstBlockScore;
    }
    if (!AFS_hiszero(VolHdr->id)) ++FirstBlockScore;
    else --FirstBlockScore;
    if ( (VolHdr->modTime - VolHdr->createTime) > 0 && (VolHdr->accessTime - VolHdr->createTime) > 0) ++FirstBlockScore;
    else --FirstBlockScore;
    if ((VolHdr->quota.usage * 2) <= VolHdr->quota.limit) ++FirstBlockScore;
    else --FirstBlockScore;
    if ((VolHdr->quota.usage < (recon->nBlocks * (recon->BlockSize / recon->FragmentSize)))
	 && (VolHdr->quota.limit < (recon->nBlocks * (recon->BlockSize / recon->FragmentSize)))) ++FirstBlockScore;
    else --FirstBlockScore;

    if (FirstBlockScore >= 6) {
	IsFirstBlock = 1;
    } else {
	IsFirstBlock = 0;
    }

/* Look through all the purported anodes in the block. */
    NumAnodes = TotScore = VolFirstPage = 0;
    score = possScore = 0;
    for (offs = epic_IndexToOffset((IsFirstBlock ? 1 : 0));
	  epic_LegalOffset(offs, bsize);
	  offs = epic_NextOffset(offs)) {
	code = epix_AnodeScore(recon, ix, ((char *)data + offs), &subScore, Recurse, offs, &anodeType, &anodeIndex, TYPEDATA);
	if (code != 0) continue;
	if (anodeType != 0) {
	    CurrIx = epic_OffsetToIndex(offs);
	    if (anodeIndex == CurrIx) {	/* first page of AVL if these agree */
		VolFirstPage = 1;
	    }
	    /* Test if this anodeIndex value is reasonable. */
	    if ((anodeIndex % epic_AnodesPerBlock(recon->BlockSize)) == CurrIx)
		++score;
	    else --score;
	    ++possScore;
	}
	++NumAnodes;
	TotScore += subScore;
    }
    if (IsFirstBlock == VolFirstPage) score += 2;
    else score -= 2;
    possScore += 2;
    if (IsFirstBlock && VolFirstPage) {
	type = TYPEANODEBLOCK | SECTIONHEAD | LISTTYPEVOL;
	score += FirstBlockScore;
    } else if (IsFirstBlock == 0 && VolFirstPage == 0) {
	type = TYPEANODEBLOCK | SECTIONMIDDLE | LISTTYPEVOL;
	score -= FirstBlockScore;
    }
    possScore += 7;
    score = EvidenceConst(score, possScore);
    if (NumAnodes > 0) score = (3 * score + TotScore) / (3 + NumAnodes);
    if (score > EvidenceConst(1,4) && Recurse) {
	recon->State[ix].NativeGuess = type;
	recon->State[ix].NativeEvidence = score + EVIDENCE_OFFSET;
    }
    *scoreP = score;
    *typeP = type;
    return 0;
}

/* EXPORT */
long epit_BlockScore(recon, ix, data, typeP, scoreP, Recurse)
struct epiz_ReconState *recon;
u_long ix;
opaque data;
long *typeP, *scoreP;
int Recurse;
{/* Compat with table-driven version. */
    return epit_BlockScoreInt(recon, ix, data, typeP, scoreP, Recurse, 0);
}
#endif /* SCAN_SALVAGER */
#endif /* not KERNEL */
