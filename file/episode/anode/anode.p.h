/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1996, 1990 Transarc Corporation - All rights reserved */

/* This file contains structure declarations for the fixed anode module and
 * some structures used by the dynamic anode module.  In particular, this file
 * describes the on-disk anode structure and the anode handle used throughout
 * Episode. */

/* $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/anode/RCS/anode.p.h,v 4.109 1996/03/04 18:58:10 ota Exp $ */

#ifndef TRANSARC_EPI_ANODE_H
#define TRANSARC_EPI_ANODE_H

#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_uio.h>
#include <dcedfs/episode/async.h>
#include <dcedfs/episode/fifo.h>
#include <dcedfs/episode/logbuf.h>

#include <dcedfs/episode/tlock.h>

/* Debug mask bits for anode modules */
#define EPIA_DEBUG_ANODE_BLOCK	1
#define EPIA_DEBUG_FIXED_ANODE	2
#define EPIA_DEBUG_BLOCK_ALLOC	3
#define EPIA_DEBUG_ANODE	4
#define EPIA_DEBUG_VOLUME_TABLE	5
#define EPIA_DEBUG_VOLUME	6
#define EPIA_DEBUG_FILE		7
#define EPIA_DEBUG_AGGREGATE	8
#define EPIA_DEBUG_STRATEGY	9
#define EPIA_DEBUG_NFILES      10	/* max index == 9 */

#define EPIA_DEBUG_SLEEP	32	/* all calls to sleep */
#define EPIA_DEBUG_CODEBLOCKS	31	/* call EnterCodeBlock */
#define EPIA_DEBUG_UNIQUETIME	30	/* test UniqueTime */
#define EPIA_DEBUG_ADDPREEMPTS	29	/* insert preemtions */
#define EPIA_DEBUG_DEFERTRANS   28	/* defer ending trans */

extern int afsdb_episode_anode;

extern u_int epia_maxExtentBlks;	/* max number of blocks in an extent */
extern u_int epia_maxIOs;		/* max ioData structs to allocate */

#ifdef AFS_DEBUG
#define EPIA_MAX_FILE_CODE_BLOCKS 150
extern int epia_codeBlock[EPIA_DEBUG_NFILES][EPIA_MAX_FILE_CODE_BLOCKS];
extern char *epia_debugFilename[EPIA_DEBUG_NFILES];
#endif

/* Anode error conversion trace macro */
#define EFS_TRACE_ANODE_ERROR_MAPPING(olderror, newerror) \
    icl_Trace4(epi_iclSet, TR_CONVERT_ERROR, \
	       ICL_TYPE_LONG, (olderror), \
	       ICL_TYPE_LONG, (newerror), \
	       ICL_TYPE_STRING, __FILE__, \
	       ICL_TYPE_LONG, __LINE__)

/* file storage methods */
enum epia_ws_method { WS_EMPTY, WS_INLINE, WS_FRAGMENTED, WS_BLOCKED };

/* Exported interface objects */

struct epia_anode;

struct epia_anodeRock {
    opaque rock;			/* generic pointer */
    void (*cleanup)(struct epia_anode *); /* proc called when GCing handle */
};
typedef struct epia_anodeRock epia_anodeRock_t;


/* Quota structure */

/* epia_quota_t -- is sometimes used as a delta quota and sometimes as an
 *     absolute quota.  This results in some fuzziness about the high bit of
 *     the (presumed) 32 bit number.  We will just cast the long to u_long as
 *     necessary; not to worry. */

typedef struct epia_quota {	/* argument to quota proc */
    long a, v;			/* adj. for allocated and visible */
} epia_quota_t;

typedef long (*epia_qproc_t)(
    elbt_TranRec_t, struct epia_anode *, int, epia_quota_t);

/* The handle for open anodes. */

struct epia_anode {
    struct epia_anode *next;		/* next same hash */

    /* hash on the next three words */
    async_device_t *device;		/* disk device containing anode */
    daddr_t block;			/* block containing disk anode */
    u_long offset;			/* anode's offset in block */

    /* These fields are protected by the handleLock not the handle's lock */
    u_int  refCount;			/* number active users */
    int	   GConClose;			/* GC immediately when refCount <- 0 */
    struct fifo_Thread thread;		/* on Free or Inuse list */

    /* the handle's lock protects the following fields */
    struct tlock_data lock;
    opaque disk;			/* ptr to copy of disk anode */
    afs_hyper_t length;			/* in-memory afs_hyper_t */
    long   flags;			/* handle flags: dirty */
    u_long volIndex;			/* AVL index of VT of this anode */
    struct epia_anode *backingAnode;	/* backing anode, eval on demand */

    /* for allocate-on-write containers */
    opaque allocationHandle;		/* handle for block allocator */
    epia_qproc_t quotaProc;		/* procedure to check quota */

    /* ptrs to storage for other modules, and corresponding cleanup procs */
    struct epia_anodeRock
	allocation,			/* for the block allocator */
	volume,				/* volume table manager */
	file;				/* if this is a regular file */
    opaque firstIBCache;		/* NYI: block addr cache */
};
typedef struct epia_anode *epi_anode_t; /* XXX Should be object, not ptr */

/* Handle flags: most defined fixed_anode.c */
#define EPIA_HFLAGS_DEVICEUNLOGGED 0x10	/* device hasn't had logging inited */

/* These define flag bits for the Create call.  Note that the type values are
 * also used in the diskAnode flags word to type those structures.  Zero is an
 * invalid value.  These values may not change. */
#define EPIA_CFLAGS_TYPE          0xf   /* mask for specifying anode type */
#define EPIA_CFLAGS_AVL		    1	/* aggregate volume list */
#define EPIA_CFLAGS_VOLUME          2
#define EPIA_CFLAGS_BITMAP          3	/* allocation bitmap */
#define EPIA_CFLAGS_LOG	            4	/* recovery log */
#define EPIA_CFLAGS_BADBLOCKS	    5	/* blocks not in allocation pool */
#define EPIA_CFLAGS_REGULAR       0xf	/* other */

/* TODO -- Expand REGULAR from 1111(base2) to 11xx(base2).  Low bits are usable
 *     by higher level modules.
 *
 *     For file module try these assignments:
 *     00 - file
 *     01 - acl
 *     10 - plist
 *     11 - magic in first word of header.
 */

/* These flags are also used for the epia_Create call as input parameters but
 * are also returned by epia_GetInfo.  They are used by fixed anode in
 * diskAnode flags. */
#define EPIA_CFLAGS_LOGGED	0x0010	/* anode updates should be logged */
#define EPIA_CFLAGS_COPYONWRITE 0x0020	/* allocate new blocks on write */
#define EPIA_CFLAGS_PREALLOCATED 0x040	/* block allocation fixed by newaggr */

/* This flag is used for by epia_Create and epia_GetInfo but is not used by
 * fixed anode in diskAnode flags. */
#define EPIA_CFLAGS_BACKING     0x1000  /* anode backs existing anode */

/* These are output-only flags from GetInfo and are used by fixed anode. */
#define EPIA_CFLAGS_INLINE	0x0080	/* contents stored inline */
#define EPIA_CFLAGS_FRAGMENTED	0x0100	/* contents stored as fragments */
#define EPIA_CFLAGS_UNSAFE	0x0200	/* damaged during salvage */

/* This is an unused input-only flag to Create that is not used by
 * anode. */
#define EPIA_CFLAGS_ESTLENGTH	0x0400	/* estLength parameter is valid */

/* This is an output-only flag that is not used by fixed anode. */
#define EPIA_CFLAGS_BLOCKED	0x0800	/* contents stored in blocks */

#define EPIA_CFLAGS		0xffff

/* Anode disk flags: mostly defined in fixed_anode.c */
#define EPIA_AFLAGS_STATUSSIZE	(0xff)	/* size of status area */
#define EPIA_AFLAGS_TYPE_SHIFT	8
#define EPIA_AFLAGS_BITS_SHIFT	12
#define EPIA_AFLAGS_BITS_MASK	0x3f0

/* Flags to control epia_Read, epia_Truncate and epia_Write */
#define	EPIA_RWFLAGS_UNBUFFERED	1	/* bypass buffer system */
#define	EPIA_RWFLAGS_NOWITHHOLD	2	/* okay to consume withholding */

/* Advisory about max inline file size */
#define EPIA_INLINE_MAX_LEN (96)

#define epia_WriteLocked(ap)	tlock_HasOwner(&(ap)->lock)

/* Data returned by GetInfo */
struct epia_info {
    long flags;				/* miscellaneous CFLAGS & type */
    afs_hyper_t length;			/* high water mark of all writes */
    u_long allocated;			/* fragments allocated by this anode */
    u_long visible;			/* fragments readable in anode */
    u_int  statusSize;			/* size of status data */
    u_long index;			/* index in anode container */
    u_short copies;			/* non-zero if a backing anode */
    short spareShort;

    /* The volId field is no longer used.  The backing info is valid iff
     * container is type REGULAR. */
    afs_hyper_t volId;			/* obs: if fileset anode table */
    u_long backingIndex;		/* index of anode in backing vol */
    u_long backingVolIndex;		/* avl index of volume of same */

    short sspare[2];
    long lspare[4];
};

typedef struct epia_info epia_info_t;

/*
 * XXX the following wewre moved here from fixed_anode.h since they are
 * needed in vnops/efs_{newalloc,tmpcopy}
 */

#define ANODE_DIRECTBLOCKS 8 /* direct blocks per anode */
#define ANODE_INDIRECT_TREES 4 /* top level indirect blocks per anode */

typedef unsigned long baddr_t;		/* container's logical block address */

struct epix_blockLocation {		/* to specify logical block */
    int tree;				/* which indirect block tree,
					   or -1
					   or EPIX_DUMMY_BLOCK_LOCATION */
    /* ANODE_DIRECTBLOCKS == 8 */
    int directBlock;			/* which direct, if (tree == -1) */
    baddr_t base;			/* block number of base of tree */
    u_long span;			/* sep of leaves of cur indirect blk */
    u_int depth;			/* number of indirect blks used */
    daddr_t blocks[ANODE_INDIRECT_TREES];	/* disk addr of indirect blk */
    u_int indexes[ANODE_INDIRECT_TREES];	/* index in each indirect blk */
    long visibleAdjust;			/* visible size of IB being replaced */
};

typedef struct epix_blockLocation epix_blockLoc_t;

#define epix_IsLocDirect(loc) ((loc)->tree == -1)
#define EPIX_IsLocIndirectArray(loc) \
    (((loc)->tree >= 0) && ((loc)->depth > 0))
#define EPIX_IsLocTerminalIndirect(loc) \
    (((loc)->tree >= 0) && ((loc)->depth > 0) && ((loc)->span == 1))
#define EPIX_IsLocNonTerminalIndirect(loc) \
    (((loc)->tree >= 0) && (((loc)->depth == 0) || ((loc)->span > 1)))
#define EPIX_MakeDirectBlockLoc(loc, block) \
    (afsl_Assert((block) < ANODE_DIRECTBLOCKS), \
     (loc)->tree = -1, (loc)->directBlock = (block), \
     (loc)->base = (loc)->span = (loc)->depth = 0)


/* Also used in on-disk representation of an anode, so use explicit sizes */
struct epix_fragDescription {		/* description of fragment storage */
    daddr_t block;			/* block containing fragments */
    u_int16 first;			/* first fragment in block */
    u_int16 length;			/* number of fragments */
};

typedef struct epix_fragDescription epix_fragDescr_t;

/* anode special block address values */
#define EPIX_ANODE_BLOCKEMPTY ((daddr_t) 0xffffffff) /* unallocated */
#define EFS_HOLE EPIX_ANODE_BLOCKEMPTY
#define EPIX_ANODE_BLOCKEMPTY_BYTE 0xff	/* for use with buffer_Fill */
#define EPIX_ANODE_BLOCKBACKING 0x80000000 /* allocated in backing anode */
#define EPIX_ANODE_BLOCKINBACKING 0xfffffffe /* see backing anode (obs) */
#define EPIX_ANODE_BLOCKRESERVED2 0xfffffffd
#define EPIX_ANODE_BLOCKRESERVED3 0xfffffffc

#define epix_IsBlockSpecial(b)	(((b) & 0x7ffffffc) == 0x7ffffffc)
#define epix_IsBlockReadable(b)	(!epix_IsBlockSpecial(b))
#define epix_IsBlockAllocated(b) (((u_long)(b)) < 0x7ffffffc)
#define epix_IsBlockWritable(b)	epix_IsBlockAllocated(b)
#define EPIX_IsBlockInBacking(b) \
    (((b) & EPIX_ANODE_BLOCKBACKING) && !epix_IsBlockSpecial(b))
#define epix_IsBlockEmpty(b) \
    ((((u_long)b)|EPIX_ANODE_BLOCKBACKING) == EPIX_ANODE_BLOCKEMPTY)
#define epix_IsBlockViaBacking(b) \
    (((b)|EPIX_ANODE_BLOCKBACKING) == EPIX_ANODE_BLOCKINBACKING)
#define EPIX_IsBlockBacking(b) \
    (EPIX_IsBlockInBacking(b) || epix_IsBlockViaBacking(b))
#define EPIX_IsBlockAccessible(b, r) \
    (epix_IsBlockWritable(b) || ((r) && epix_IsBlockReadable(b)))
#define epix_BlockAddressSpecial(b) \
    (!epix_IsBlockWritable(b))		/* for backwards compatibility */

#define epix_MapBlockToBacking(b) (((u_long)b) & ~EPIX_ANODE_BLOCKBACKING)

#define EPIA_INLINE(ap)		(epia_GetFlags(ap) & EPIA_CFLAGS_INLINE)
#define EPIA_FRAGMENTED(ap)	(epia_GetFlags(ap) & EPIA_CFLAGS_FRAGMENTED)
#define EPIA_BLOCKED(ap) \
    ((epia_GetFlags(ap) & (EPIA_CFLAGS_INLINE | EPIA_CFLAGS_FRAGMENTED)) == 0)

#define EFS_FRAG_DADDR(dev, fr) \
    (abtodb((dev), (fr)->block) + btodb((fr)->first << (dev)->logFragSize))
#define EFS_FRAG_LENGTH(dev, fr) ((fr)->length << (dev)->logFragSize)

extern int epix_CheckBlockAllocation(
  epi_anode_t ap,
  off_t off,
  daddr_t *fsbP,
  long *alenP);

extern long epix_GetIBSize(epi_anode_t h);

extern long epix_FindBlocks(
  epi_anode_t handle,
  baddr_t block,
  u_int nBlocks,
  int forRead,
  daddr_t *pBlocks,
  int *nFoundP,				/* signed for compat. w/ insert */
  u_long *nSkipP,
  struct epix_blockLocation *loc);	/* in indirect block tree */

extern long epix_FsyncForce(epi_anode_t h);

extern long epix_Insert(
  elbb_tranRec_t trans,
  epi_anode_t handle,
  int nBlocks,				/* signed */
  daddr_t *pBlock,			/* physical block address to insert */
  struct epix_blockLocation *loc);	/* position to insert block */

extern long epix_InsertFragments(
  elbb_tranRec_t trans,
  epi_anode_t h,
  epix_fragDescr_t *frags); /* allocated frags */

extern long epix_ReadInline(
  epi_anode_t h,
  u_long offset,
  u_long length,
  caddr_t buffer);

extern long epix_WriteInline(
  elbb_tranRec_t trans,
  epi_anode_t h,
  u_long offset,
  u_long length,
  caddr_t buffer);

extern long epix_InsertInline(
  elbb_tranRec_t trans,
  epi_anode_t h,
  u_int offset,
  u_int length,
  char *buffer);

extern long epix_GetFragInfo(epi_anode_t h, epix_fragDescr_t *desc);

extern long epix_GetInline(
  epi_anode_t h,
  u_int offset,
  u_int length,
  osi_uio_seg_t iospace,
  char *buffer);

extern u_int epix_GetMaxInlineData (epi_anode_t h);

extern long epix_SetLength(
  elbb_tranRec_t trans,
  epi_anode_t h,
  afs_hyper_t length,				/* desired length of container */
  int max);				/* max length into current length */

/* Exported macros */

#define epia_AnodeDevice(h) ((h)->device)
#define epia_FragmentSize(h) ((h)->device->fragSize)
#define epia_BlockSize(h) ((h)->device->blkSize)
#define epia_DeviceLogged(h) (!((h)->flags & EPIA_HFLAGS_DEVICEUNLOGGED))
#define epia_SetDeviceLogged(h) ((h)->flags &= ~EPIA_HFLAGS_DEVICEUNLOGGED)

#define epia_SetAllocationHandle(h, value) \
    ((h)->allocationHandle = (opaque)value)
#define epia_GetAllocationHandle(h) ((h)->allocationHandle)
#define epia_SetQuotaProc(h, proc) ((h)->quotaProc = proc)
#define epia_GetQuotaProc(h) ((h)->quotaProc)

#define epia_GetAllocationRock(h) ((h)->allocation.rock)
#define epia_SetAllocationRock(h, value, proc) \
    ((h)->allocation.rock = (opaque)(value), \
     (h)->allocation.cleanup = (proc))

#define epia_GetVolumeRock(h) ((h)->volume.rock)
#define epia_SetVolumeRock(h, value, proc) \
    ((h)->volume.rock = (opaque)(value), \
     (h)->volume.cleanup = (proc))

#define epia_GetFileRock(h) ((h)->file.rock)
#define epia_SetFileRock(h, value, proc) \
    ((h)->file.rock = (opaque)(value), \
     (h)->file.cleanup = (proc))

/* Note: this is the AVL index of the volume in which h resides.  The volume
 * index of h is obtained using epia_GetIndex (see below). */
#define epia_GetVolIndex(h) ((h)->volIndex)

/* Some macros that "cheat" and access anode disk structures.  They are much
 * faster than calling GetInfo, but do no locking. */
 /* XXX These hard-wired offsets are the sad result of hiding the
  * disk anode structure in fixed_anode.c XXX */

#define EPIA_DISKANODE_FLAGS		0
#define EPIA_DISKANODE_VOLDATA		(1*sizeof(int32))
#define EPIA_DISKANODE_INDEX		(3*sizeof(int32))
#define EPIA_DISKANODE_ALLOCATED	(4*sizeof(int32))
#define EPIA_DISKANODE_VISIBLE		(11*sizeof(int32))
#define EPIA_DISKANODE_COPIES		(18*sizeof(int16))

#define epia_GetStatusSize(h) \
    ((*(u_int32 *)((char *)((h)->disk) + EPIA_DISKANODE_FLAGS)) & \
     EPIA_AFLAGS_STATUSSIZE)
#define epia_GetType(h) \
    (((*(u_int32 *)((char *)((h)->disk) + EPIA_DISKANODE_FLAGS)) >> \
      EPIA_AFLAGS_TYPE_SHIFT) & EPIA_CFLAGS_TYPE)
#define epia_GetIndex(h) \
    (*(u_int32 *)((char *)((h)->disk) + EPIA_DISKANODE_INDEX))
#define epia_GetAllocated(h) \
    (*(u_int32 *)((char *)((h)->disk) + EPIA_DISKANODE_ALLOCATED))
#define epia_GetVisible(h) \
    (*(u_int32 *)((char *)((h)->disk) + EPIA_DISKANODE_VISIBLE))
#define epia_GetFlags(h) \
    (((*(u_int32 *)((char *)((h)->disk) + EPIA_DISKANODE_FLAGS)) >> \
      EPIA_AFLAGS_BITS_SHIFT) & EPIA_AFLAGS_BITS_MASK)
#define epia_GetCopies(h) \
    (*(int16 *)((char *)((h)->disk) + EPIA_DISKANODE_COPIES))
#define epia_GetLength(h) ((h)->length)

/* XXX -- As a temporary hack until new afs_hyper_t handling macros are deployed,
 *     just fetch the low half of the afs_hyper_t. */
#define epia_GetLen32(h) AFS_hgetlo((h)->length)

/* The field these reference is obsolete and provided only for compatibility
 * with older disk formats. */
#define epia_GetVolDataHi(h) \
    (*(int32 *)((char *)((h)->disk) + EPIA_DISKANODE_VOLDATA))
#define epia_GetVolDataLo(h) \
    (*(u_int32 *)((char *)((h)->disk) + EPIA_DISKANODE_VOLDATA+sizeof(int32)))

/* Exported functions */

extern long epia_Init(u_long maxHandles);
extern void epia_InitStrategy(void);

extern long epia_Open(
  epi_anode_t anodeC,
  u_long index,
  opaque allocationHandle,
  epia_qproc_t quotaProc,
  epi_anode_t *handleP
);

#define epia_InitUio(uio, iov, off, len, buf) \
    ((((uio)->osi_uio_iov = (iov)) ? \
	((uio)->osi_uio_iovcnt = 1, \
	 ((struct iovec *)(iov))->iov_base = (caddr_t)(buf), \
	 ((struct iovec *)(iov))->iov_len = (len)) : \
	((uio)->osi_uio_iovcnt = 0)), \
     (uio)->osi_uio_offset = (off), \
     (uio)->osi_uio_seg = OSI_UIOSYS, \
     (uio)->osi_uio_resid = (len))

extern long epia_StartTran(
  char *serviceName,
  epi_anode_t anode,
  elbt_TranRec_t *transP
);

extern long epia_EndTran(epi_anode_t anode, elbt_TranRec_t trans);

extern long epia_Read(epi_anode_t h, long flags, struct uio *uio);

extern long epia_Write(
  elbt_TranRec_t trans,
  epi_anode_t h,
  long flags,
  struct uio *uio
);

extern long epia_Close(epi_anode_t handle);

extern long epia_Create(
  elbt_TranRec_t trans,
  epi_anode_t volC,
  u_long index,
  long flags,
  u_int statusSize,
  u_long estLength,
  afs_hyper_t volId,				/* obs */
  epi_anode_t backingAnodeC,
  u_long backingIndex,
  epi_anode_t *handleP
);

extern long epia_SetStatus(
  elbt_TranRec_t trans,
  epi_anode_t h,
  u_int offset,
  u_int length,
  char *buffer
);

extern long epia_GetStatus(
  epi_anode_t h,
  u_int bufLength,
  char *buffer,
  u_int *statusSizeP
);

extern long epia_GetInfo(epi_anode_t h, struct epia_info *info);

extern long epia_Map(
  epi_anode_t handle,
  u_long block,
  daddr_t *pBlockP,
  async_device_t **deviceP
);

extern long epia_Delete(elbt_TranRec_t trans, epi_anode_t anodeC);

extern long epia_Truncate(
  elbt_TranRec_t trans,
  epi_anode_t handle,
  long flags,
  afs_hyper_t length
);

extern long epia_Clone(
  elbt_TranRec_t trans,
  epi_anode_t h,			/* copy-on-write container */
  epi_anode_t b,			/* backing container */
  long flags				/* to reclone or to unclone */
);

extern long epia_Fsync(epi_anode_t h);

extern osi_iodone_t epia_Iodone(struct buf *bp);
extern long epia_Strategy(struct buf *bp, epi_anode_t ap);
extern int epia_BioWait(struct buf *bp);
extern void epia_BufError(struct buf *bp);
extern void epia_MarkInTransit(struct buf *bp);
extern void epia_ClearInTransit(struct buf *bp);
extern void epia_StartAsyncDaemon(void);
extern long epia_UseQuota(
  epi_anode_t h,
  elbb_tranRec_t trans,
  epia_quota_t delta
);

extern int epia_HasHoles(epi_anode_t ap, long len);

#if defined(AFS_SUNOS5_ENV) && defined(KERNEL)
extern void epia_StartAsyncDaemon(void);
#endif /* AFS_SUNOS5_ENV && KERNEL */

#endif /* TRANSARC_EPI_ANODE_H */
