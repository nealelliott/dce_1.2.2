/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */

/*
 * Copyright (C) 1996, 1990 Transarc Corporation
 * All rights reserved.
 */
/* $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RCS/efs_evnode.h,v 4.185 1996/03/04 19:03:01 ota Exp $ */

#ifndef TRANSARC_EFS_EVNODE_H
#define TRANSARC_EFS_EVNODE_H

#include <sys/vnode.h>
#include <dcedfs/queue.h>		/* struct squeue */
#include <dcedfs/dacl.h>		/* dacl_t */
#include <dcedfs/episode/critical.h>	/* CRIT macros */
#include <dcedfs/episode/anode.h>	/* epi_anode_t */
#include <dcedfs/episode/tlock.h>	/* struct tlock_data */
#include <dcedfs/episode/fid.h>		/* epid_volFileId_t */
#include <dcedfs/episode/volume.h>	/* epi_volume_t */
#include <dcedfs/xvfs_vnode.h>
#include <dcedfs/osi_vmm.h>
#include <dcedfs/osi_buf.h>

/*
 * The Episode vnode structure.
 */

/*
 * enum where -- is used to label vnodes with interesting states they have been
 *     through for debugging purposes.  I pack these enum values into 4 four
 *     bits fields so that the debugger (at least on Solaris) can show the
 *     history with these symbolic names.  Operationally I just shift a long
 *     and add using the HERE macro (below).
 */
enum where {
    w_init=0,
    w_norecycle,
    w_recycle,
    w_found,
    w_ostale,
    w_ofound,
    w_nosetid,
    w_ofail,
    w_onew,
    w_silookup,
    w_delete,
    w_istale,
    w_idelete,
    w_sustale,
    w_stopuse
};
#define HERE(evp, h) \
    ((evp)->vd_testUnion.shift = ((evp)->vd_testUnion.shift << 4) + (int)h)

#define EFS_CACHE_LEN	16u	/* Maximum length of name to be cached */
#define ATIME_INTERVAL 100	/* Time interval(in secs) to modify atime */

struct evnode {
    struct vnode vn;			/* vnode; must be first for VTOEV */

    /* Protected by global vntableLock */

    struct evnode *vd_hashNext;
    struct volume *vd_volp;		/* receptacle of related volume info */
    afs_hyper_t vd_volid;			/* volume identifier */
    epi_volFileId_t vd_fid;		/* file identifier */
    struct fifo_Thread vd_lru;		/* lru links */
    long vd_label;			/* for vnode iteration ; see StopUse */
    unsigned vd_onHash:1;		/* present on the hash table */
    unsigned vd_onLRU:1;		/* present on the LRU list */
    unsigned vd_avoidRecycle:1;		/* on AIX: not safe to recycle */

    struct {
	osi_dlock_t lock;		/* mutex protecting cached info */

	unsigned noChange:1;		/* atime updates ignored */
	unsigned noStatus:1;		/* status is invalid */
	unsigned noDirty:1;		/* no unwritten status modifications */
	unsigned newAtime:1;		/* atime is dirty */
	unsigned newMtime:1;		/* mtime is dirty */
	unsigned newCtime:1;		/* ctime is dirty */
	unsigned newLen:1;		/* vd_len is dirty */
	unsigned lookupValid:1;		/* name/fid cache is valid */

	struct timeval lastAtime;	/* Last access time */
	struct timeval lastMtime;	/* Last mod time */
	struct timeval lastCtime;	/* Last change time */
	u_long dataVersion;		/* current DV */

	/* The following fields save information about the most recent
         * directory lookup.  They are only used for directories. */
	char lookupName[EFS_CACHE_LEN];	/* entry name */
	epi_volFileId_t lookupFid;	/* FID */
	int lookupOffset;		/* offset */
    } vd_cache;				/* updates maybe dirty in vnode */

    struct {
	osi_dlock_t lock;		/* protects anode and related info */
	unsigned noAnode:1;		/* anode handle must stay closed */
	unsigned noDelete:1;		/* deletions ignored */
	unsigned readonly:1;		/* file is in a R/O volume */
	unsigned anode:1;		/* anode handle is open */
	unsigned anodeRO:1;		/* unwritable because (copies>0) */
	epi_anode_t ap;			/* anode handle representing fid */
    } vd_file;

    struct {
	osi_dlock_t lock;		/* protects virtual memory cache */
	unsigned noReadable:1;		/* block all PageIns */
	unsigned noWritable:1;		/* block PIs requiring writable pgs */
	unsigned readonly:1;		/* fail PIs requiring writable pages */
	unsigned valid:1;		/* valid pages may exist */
	unsigned dirty:1;		/* modified pages may exist */
	unsigned seg:1;			/* on AIX: has VM segment */
	off_t seqReadOff;		/* offset of next sequential read */
	off_t readAheadOff;		/* offset of next readahead cluster */
	off_t lastReadAheadOff;
    } vd_vm;

    osi_dlock_t vd_idLock;		/* protects vnode's id from changing */

    /* The vnode operation lock; the highest lock in the vnode hierarchy.
     * Updates to directories must use this as a tlock, for other file types
     * just a plain rw lock. */
    struct tlock_data vd_tlock;

    int vd_flags;			/* miscellaneous flags -- see below */
    afs_hyper_t vd_len;			/* length */

    dacl_t *vd_acl;			/* ACL */

    /*
     * Access cache. Valid if cached cred is non null.
     * Covered by tlock and cache lock. The cache lock is just being used as
     * an expedient convenience. Ideally we would have a separate
     * synchronization object to maximise concurrency because the cache lock
     * is currently too heavyweight being held across calls to the anode layer
     * that can involve disk I/O.
     */
    struct {
	osi_cred_t *cred;	        /* credentials for cached access */
	dacl_permset_t access;		/* cached access */
    } vd_accessCache;
#define vd_cred vd_accessCache.cred
#define vd_access vd_accessCache.access

    /* for debugging: vnode history */
    union {
#ifndef AFS_AIX_ENV
	/* can't pack enums with AIX compiler */
	struct {
	    enum where a8:4;
	    enum where a7:4;
	    enum where a6:4;
	    enum where a5:4;
	    enum where a4:4;
	    enum where a3:4;
	    enum where a2:4;
	    enum where a1:4;
	} test;
#endif
	long shift;
    } vd_testUnion;

    /* file locks */
    struct squeue vd_flocks;		/* list of locks */

    /* Disk allocation hint */
    daddr_t vd_lastDblk;		/* last allocated disk block */
    off_t vd_dblkOff;			/* offset of lastDblk */

    /* system-dependent */

#if !defined(KERNEL) || defined(AFS_AIX_ENV)
    osi_dlock_t refCountMutex;		/* Protects v_count updates */
#endif

#ifdef AFS_AIX_ENV
    struct gnode vd_aixgnode;		/* vnodes 1-to-1 with gnodes */
    unsigned long pagefaults;
#endif /* AFS_AIX_ENV */

#if defined(AFS_AIX_ENV)
    /*
     * Top half seralization for operations which affect or use file length
     * and must drop locks to make VMM calls. This is at the top of the
     * locking hierarchy.
     */
    osi_dlock_t vmmLenLock;
#endif /* AFS_AIX_ENV */

#ifdef AFS_SUNOS5_ENV
    u_int vd_delayOff;			/* delayed write offset */
    u_int vd_delayLen;			/* delayed write len */
#endif /* AFS_SUNOS5_ENV */

#ifdef AFS_OSF_ENV
    dev_t vd_rdev;			/* device number */
#endif /* AFS_OSF_ENV */
};
typedef struct evnode *efs_vnode_t;	/* XXX */
typedef struct evnode efs_evnode_t;

/*
 * efs_lockvp -- acquires a mutex that protects the vnode reference count
 *     (v_vcount) and flags (v_flags); efs_unlockvp does the corresponding
 *     release.  They are used by VOP_INACTIVE to avoid races while putting a
 *     vnode to bed, and by other threads to test safely whether or not a vnode
 *     is active.  On SunOS, this mutex is part of the standard vnode
 *     structure; on AIX, we roll our own.
 */
#if !defined(KERNEL) || defined(AFS_AIX_ENV)
#define efs_lockvp(evp)		lock_ObtainWrite_r(&evp->refCountMutex)
#define efs_unlockvp(evp)	lock_ReleaseWrite_r(&evp->refCountMutex)
#elif defined(AFS_SUNOS5_ENV)
#define efs_lockvp(evp)		osi_lockvp_r(EVTOV(evp))
#define efs_unlockvp(evp)	osi_unlockvp_r(EVTOV(evp))
#else
#define efs_lockvp(evp)		/* XXX */
#define efs_unlockvp(evp)	/* XXX */
#define efs_lockvp_r(evp)	/* XXX */
#define efs_unlockvp_r(evp)	/* XXX */
#endif

#if defined(AFS_AIX_ENV)
#define EFS_VMLEN_WLOCK(evp)	lock_ObtainWrite_r(&evp->vmmLenLock)
#define EFS_VMLEN_WUNLOCK(evp)	lock_ReleaseWrite_r(&evp->vmmLenLock)
#define EFS_VMLEN_RLOCK(evp)	lock_ObtainRead_r(&evp->vmmLenLock)
#define EFS_VMLEN_RUNLOCK(evp)	lock_ReleaseRead_r(&evp->vmmLenLock)
#else
#define EFS_VMLEN_WLOCK(evp)
#define EFS_VMLEN_WUNLOCK(evp)
#define EFS_VMLEN_RLOCK(evp)
#define EFS_VMLEN_RUNLOCK(evp)
#endif

#define vd_seg vd_aixgnode.gn_seg	/* segment ID for AIX 3.1 VM */

#ifndef AFS_OSF_ENV
#define vd_rdev vn.v_rdev		/* device number */
#endif  /* !AFS_OSF_ENV */

/* Flags */

#define VD_ROOT		0x2		/* root of FS */
#define VD_LENVALID	0x10		/* vd_len valid */
#define	VD_DIRDELETED	0x40		/* directory link count is zero */

#define EV_ISDIRTYTIMES(evp) \
    ((evp)->vd_cache.newAtime || \
     (evp)->vd_cache.newMtime || \
     (evp)->vd_cache.newCtime)
#define EV_ISDIRTY(evp) (EV_ISDIRTYTIMES(evp) || (evp)->vd_cache.newLen)

extern epi_anode_t vnm_GetFileH(efs_evnode_t *evp);
#define EVTOA(evp) vnm_GetFileH(evp)	/* get anode pointer of vnode. */

#if 0
/*#if defined(AFS_SUNOS5_ENV) && defined(KERNEL)*/
#define	 ASSERT_VMBITS_CONSISTENT(evp) \
    afsl_Assert(evp->vd_vm.valid || (EVTOV(evp)->v_pages == NULL))
#else
#define	 ASSERT_VMBITS_CONSISTENT(evp)
#endif


/*
 * Macros to convert between evnode and vnode; they "cheat" based on
 * the fact that the vnode is the first member of the evnode.
 */
#define EVTOV(evp) (&(evp)->vn)
#define VTOEV(vp) ((struct evnode *)(vp))

#define EVTOVFS(evp) VTOVFS((evp)->vd_volp) /* get vfs pointer */
#define EVTOVH(evp) VTOVH((evp)->vd_volp)

#ifdef AFS_AIX_ENV
#define EVTOGP(evp) (&(evp)->vd_aixgnode) /* get gnode under AIX */
#define BPTOVP(bp) (((struct gnode*)((bp)->b_vp))->gn_vnode)
#endif /* AFS_AIX_ENV */

/* Slightly different handling of null VFS under OSF-1 */
#if	defined(AFS_OSF_ENV) && defined(_KERNEL)
#define EPI_ISNULLVFSP(vfsp) ((vfsp) == NULLMOUNT || (vfsp) == DEADMOUNT)
#else
#define EPI_ISNULLVFSP(vfsp) ((vfsp) == (struct osi_vfs *) 0)
#endif

#ifdef AFS_OSF_ENV
#define EPI_VFS_READONLY(vfsp) (((vfsp)->m_flag & M_RDONLY) != 0)
#else
#define EPI_VFS_READONLY(vfsp) (((vfsp)->vfs_flag & VFS_RDONLY) != 0)
#endif

/* EV_ISREADONLY -- tests whether we reject attempts to modify a vnode.  We
 *     explicitly check the vfs R/O bit because we can not yet set this
 *     atomically for an entire volume.  This logic is similar to that for
 *     computing noDirty. */

#define EV_ISREADONLY(evp) \
    ((evp)->vd_file.readonly || (evp)->vd_file.anodeRO || \
     (!EPI_ISNULLVFSP(EVTOVFS(evp)) && EPI_VFS_READONLY(EVTOVFS(evp))))

/* Miscellaneous hack -- bypass ops vector under AIX */

#define	FAST_VN_HOLD(evp) \
    MACRO_BEGIN \
	efs_lockvp(evp); \
	EVTOV(evp)->v_count++; \
	efs_unlockvp(evp); \
    MACRO_END

#define VN_DECR(vp) ((vp)->v_count--)

/*
 * Instead of a NOIDENTITY bit we just look at the fid, protected by the
 * vntableLock, of course.
 */
#define NoIdentity(evp) epid_IsInvalid(&evp->vd_fid)

/*
 * EV_CHECKSTALE -- these macros introduce most vnode operations.  They are
 *     necessary because some serious volume operations (e.g.  vol_destroy)
 *     can make held vnodes stale.  All vnode operations except VOP_INACTIVE
 *     return ESTALE when passed a stale vnode.  This includes VM ops such as
 *     getpage and putpage.
 *
 *     This check is usually coupled with an assertion that the anode pointer
 *     is valid.  Vnode ops that do not access the anode can use
 *     EV_CHECKSTALE_NO_ANODE to bypass the assertion.  This is primarily
 *     of historical significance, since the anode pointer will be valid in
 *     the absence of serious volume operations.
 *
 * LOCK USED -- none.  Nearly all vnode operations take held vnodes and cannot
 *     run concurrently with the above mentioned serious volume operations.
 *     This means that the identity of the vnode passed to these vnode
 *     operations is stable and checking for NoIdentity is safe without locks.
 *     Exceptions to this rule include efs_inactive and efs_getvolume, q.v.
 */
#define EV_CHECKSTALE(evp) \
    (NoIdentity(evp) ? \
	(icl_Trace1(efs_iclSet, EFS_TRACE_STALE, ICL_TYPE_POINTER, (evp)), \
		ESTALE) : \
	(afsl_Assert((evp)->vd_file.anode), 0))

#define EV_CHECKSTALE_NO_ANODE(evp) \
    (NoIdentity(evp) ? \
	(icl_Trace1(efs_iclSet, EFS_TRACE_STALE, ICL_TYPE_POINTER, (evp)), \
		ESTALE) : \
	0)

/*
 * Episode VnodeOps Statistics  -- for dir check
 */
struct efsdircache {
     u_long enters;			/* number of times of caching data */
     u_long checks;			/* total number of checks */
     /* lookup counters */
     u_long lDot;			/* special cases of "." */
     u_long lHits;			/* number of hits */
     u_long lHitNoEnt;			/* number of hits, of failed lookup */
     u_long lMisses;			/* number of misses */
     u_long lMissNoEnt;			/* number of failed lookups. */
     /* check counters */
     u_long cDots;			/* special cases of "." or ".." */
     u_long cHits;			/* number of hits */
     u_long cHitNoEnt;			/* number of hits, of failed lookup */
     u_long cMisses;			/* number of misses */
     u_long cMissNoEnt;			/* number of failed lookups. */
};

/*
 * EVnode type determination.
 * XXX Why the weird casts?
 */
#define ISSPECTYPE(t) ((short)(t) == (short) VBLK || (short)(t) == (short) VCHR)
#define ISFIFOTYPE(t) ((short)(t) == (short) VFIFO)

#define EV_ISFIFO(evp) OSI_ISFIFO(EVTOV(evp))
#define EV_ISVDEV(evp) OSI_ISVDEV(EVTOV(evp))
#define EV_ISDIR(evp) OSI_ISDIR(EVTOV(evp))
#define EV_ISREG(evp) OSI_ISREG(EVTOV(evp))
#define EV_ISLNK(evp) OSI_ISLNK(EVTOV(evp))

extern enum vtype vnm_stat_type_to_vtype[];
extern int vnm_vtype_to_stat_type[];

#define EV_MODE_TO_VTYPE(mode) (vnm_stat_type_to_vtype [(mode)>>12])
#define EV_VTYPE_TO_MODE(vtype) (vnm_vtype_to_stat_type [(int) (vtype)])

/* convert between squeue and vnode */

#define VTOHQ(v) (&v->vd_hash)
#define VTOLQ(v) (&v->vd_lru)
#define HQTOV(q) ((struct evnode *) \
		      ((char *) (q) - offsetof(struct evnode, vd_hash)))
#define LQTOV(q) ((struct evnode *) \
		      ((char *) (q) -  offsetof(struct evnode, vd_lru)))

/* Miscellaneous */
#define EFS_EVNUM_TO_NET(evnum)	((evnum) - 2)
#define EFS_NET_TO_EVNUM(net)	((net) + 2)

/* These work for any combination afs_hyper_t and afs_hyper_t. */
#define VOLID_EQ(v1, v2) AFS_hsame(v1, v2)
#define VOLID_SET(v1, v2) ((v1) = (v2))

/*
 * vndh_SetDirHandle -- sets a dirhandle to point to a directory vnode.
 *
 *     This is not a sensible prefix.
 *     Ceci n'est pas une pipe.
 */
#define vndh_SetDirHandle(evp, edh) \
    edh_SetDirHandle (edh, (evp)->vd_file.ap, VTOVH((evp)->vd_volp), \
		      &(evp)->vd_fid)

#if !defined(AFS_SUNOS5_ENV) && !defined(AFS_AIX_ENV)

/* Support for efs_bread and efs_brelse */

struct efs_buf {
    struct buf buf;			/* must be first */
    struct buffer *epbuf;		/* back ptr to Episode buffer */
};

#define EBTOB(ebp) ((struct buf *) (ebp))
#define BTOEB(bp) ((struct efs_buf *) (bp))

#endif /* AFS_SUNOS5_ENV, AFS_AIX_ENV */

/* Compare with device vnode -- perhaps should be in OSI */

#ifdef AFS_OSF_ENV
#ifdef _KERNEL
#define EQUAL_DEV(bdevvp, dev) ((bdevvp)->v_specinfo->si_rdev == (dev))
#else
#define EQUAL_DEV(bdevvp, dev) 1
#endif /* _KERNEL */
#else
#define EQUAL_DEV(bdevvp, dev) ((bdevvp)->v_rdev == (dev))
#endif /* AFS_OSF_ENV */

/* Openbits for StopUse */

#define STOPUSE_NO_CHANGE	(1<<0)	/* fileset is open */
#define STOPUSE_NO_ANODE	(1<<1)	/* anode handle must be closed */
#define STOPUSE_NO_STATUS	(1<<2)	/* clean and invalidate status */
#define STOPUSE_NO_DIRTY	(1<<3)	/* write thru status updates */
#define STOPUSE_NO_VM		(1<<4)	/* clean and invalidate VM */
#define STOPUSE_NO_DIRTYVM	(1<<5)	/* write thru dirty VM pages */

/* Flags for vnm_Inuse */

#define INUSE_VFSVALID	1
#define INUSE_ALLOWROOT	2

/* Flags for efs_rdwr_subr */

#define EFS_RWFLAGS_LOCK	1	/* don't assume locks set */
#define	EFS_RWFLAGS_UNBUFFERED	2	/* use unbuffered I/O */

/* VM support */

#if defined(KERNEL) && (defined(AFS_AIX_ENV) || defined(AFS_SUNOS5_ENV))
extern int vnvm_fsync(struct vnode *vp, osi_cred_t *credp);
extern int vnvm_schedule(struct vnode *vp, osi_cred_t *credp, int syncflag);
extern int vnvm_delete(struct vnode *vp, osi_cred_t *credp);
extern int vnvm_recycle(struct vnode *vp, osi_cred_t *credp);
extern int vnvm_StopUse(struct vnode *vp, osi_cred_t *credp);
extern int vnvm_Clean(struct vnode *vp, osi_cred_t *credp);
#else
#define vnvm_fsync(vp, credp) 0
#define vnvm_schedule(vp, credp, syncflag) 0
#define vnvm_delete(vp, credp) 0
#define vnvm_recycle(vp, credp) 0
#define vnvm_StopUse(vp, credp) 0
#define vnvm_Clean(vp, credp) 0
#endif /* KERNEL && (AFS_AIX_ENV || AFS_SUNOS5_ENV) */

#if defined(KERNEL)
extern struct buf *vnvm_PrepareUserBuffer(
    caddr_t uaddr,
    size_t length,
    int is_write
);
extern void vnvm_ReleaseUserBuffer(
    struct buf *bp,
    caddr_t uaddr,
    size_t length
);
#else
#define vnvm_PrepareUserBuffer(uaddr, length, is_write) 0
#define vnvm_ReleaseUserBuffer(bp, uaddr, length)
#endif

extern u_long efs_GetLength(efs_vnode_t evp);
extern int efs_setlength(struct vnode *vp, long len, osi_cred_t *credp);
extern long efs_GetBlockSize(struct vnode *vp);
extern struct xvfs_vnodeops *epii_GetEPIOps(void);

/* Definitions and macros for tracing */

#define EFS_DEBUG_VNOPS 1
#define EFS_DEBUG_VOLOPS 2
#define EFS_DEBUG_AGOPS 3
#define EFS_DEBUG_DHOPS 4
#define EFS_DEBUG_VATTR 5
#define EFS_DEBUG_MISC 6
#define EFS_DEBUG_VFSOPS 7
#define EFS_DEBUG_ACCESS 8
#define EFS_DEBUG_INIT 9
#define EFS_DEBUG_VOLVNOPS 10
#define EFS_DEBUG_VNODE 11
#define EFS_DEBUG_NEWALLOC 12
#define EFS_DEBUG_PROMOTE 13
#define EFS_DEBUG_PAGEIO 14

extern long afsdb_episode_vnops;

#define EFS_TRACE_VNOPS(whichbit, epiError, unixError)  \
    MACRO_BEGIN						\
	dmprintf(afsdb_episode_vnops, (whichbit),	\
		 ("Mapping episode error: %u to unix errno: %u\n", \
		  (epiError), (unixError)));		\
	icl_Trace4(efs_iclSet, EFS_TRACE_CONVERT_ERROR,	\
		   ICL_TYPE_LONG, (epiError),		\
		   ICL_TYPE_LONG, (unixError),		\
		   ICL_TYPE_STRING, __FILE__,		\
		   ICL_TYPE_LONG, __LINE__);		\
    MACRO_END

/* Added ICL tracing declarations.  This include file is not installed.
 * It is used privately by the vnops layer. -ota 930622 */

#ifdef KERNEL

#include <dcedfs/icl.h>			/* icl_Trace */
#include <dcedfs/episode/efs_trace.h>

/*
 * ICL sets declared in efs_init.c
 * Use efs_iclSet for only the critical traces and efs_iclSet2 for others
 * efs_iclSetalloc is for traces of routines that manipulate the disk
 * allocation
 */
extern struct icl_set *efs_iclSet;
extern struct icl_set *efs_iclSet2;
extern struct icl_set *efs_iclSetalloc;
#else /* KERNEL */

/*
 * Undefine the macros defined in <dcedfs/icl.h>
 */
#undef icl_Trace0
#undef icl_Trace1
#undef icl_Trace2
#undef icl_Trace3
#undef icl_Trace4
#define icl_Trace0(a, b) 0
#define icl_Trace1(a, b, c1, d1) 0
#define icl_Trace2(a, b, c1, d1, c2, d2) 0
#define icl_Trace3(a, b, c1, d1, c2, d2, c3, d3) 0
#define icl_Trace4(a, b, c1, d1, c2, d2, c3, d3, c4, d4) 0

#endif /* KERNEL */

#define Assert(x) afsl_Assert(x)

#endif /* TRANSARC_EFS_EVNODE_H */
