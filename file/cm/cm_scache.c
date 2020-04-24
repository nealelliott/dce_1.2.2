/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cm_scache.c,v $
 * Revision 1.1.24.1  1996/10/02  17:12:29  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:05:53  damon]
 *
 * $EndLog$
 */
/*
 *      Copyright (C) 1996, 1990 Transarc Corporation
 *      All rights reserved.
 */


#include <dcedfs/param.h>			/* Should be always first */
#include <dcedfs/osi_cred.h>
#include <dcedfs/afs4int.h>
#include <cm.h>				/* Cm-based standard headers */
#include <cm_volume.h>
#include <cm_conn.h>
#include <cm_stats.h>
#include <cm_aclent.h>
#include <cm_dnamehash.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/cm/cm_scache.c,v 1.1.24.1 1996/10/02 17:12:29 damon Exp $")

/* This comment describes the locking hierarchy used in the cache manager.
 * Please keep this information up-to-date!
 *
 * fetch/store flags in scache entries
 *
 * fetch/store flags in dcache entries
 *
 * scache llocks (lock in vnode order if lock more than one at once)
 *
 * dcache llocks
 *
 * nh_lock (dnamehash lock) (locks cm_scachelock, called with locked vnodes)
 *
 * cm_scachelock (locked before cm_dcachelock by FlushSCache and cm_GetDownD)
 *
 * cm_getdcachelock (locked before cm_dcachelock in
 *     cm_GetDCache, cm_GetNDCaches)
 *
 * cm_dcachelock
 *
 * volume lock
 *
 * cm_volumelock
 *
 * server lock
 *
 * cm_serverlock
 *
 * conn lock
 *
 * cm_connlock
 *
 * cm_tgtlock, cm_racinglock, cm_qtknlock, cm_memCacheLock, cm_initStateLock
 *
 * Note that the only locks that can be held over a primary interface
 * RPC are the fetching flags for scache and dcache entries (not
 * the storing flags!).  Holding any other lock over an RPC to the
 * primary interface will result in terrible global deadlocks.
 *
 * Also note that the fetch/store scache/dcache locks can only be
 * changed while holding the corresponding scache or dcache entry
 * locks in write mode.  This means that you can prevent a lock from
 * being acquired by holding the appropriate llock.  Also note that
 * this requires that the llocks to below the fetch/store flags in the
 * locking hierarchy, so that we can obtain the llocks before clearing
 * the flags.
 *
 * Note that no RPCs, to either the primary or secondary interface,
 * can be made while holding locks at or below the cm_volumelock
 * level, since locks below that level are required for the basic
 * RPC connection management machinery.
 *
 * Here's the definition of the locking semantics of the status and
 * data storing and fetching flags, as well as the storeCount field
 * in the scache entry:
 *
 * The status and data fetching flags are set whenever the status
 * (for a file) or data (for a dcache entry) is being fetched by the
 * server.
 *
 * The status storing flag is set whenever status info is being
 * sent back to the server.  This includes operations that truncate
 * or extend the file.  The scache storeCount field is incremented
 * everytime any data cache entry is stored back, even if it isn't
 * storing any status information back.  Typically, either the storeCount
 * field is non-zero (indicating a commutative store of some chunk,
 * protected by the dcache storing flag), or the scache store flag is
 * set, indicating that we're actually updating global file information,
 * like the file length.
 *
 * The data storing flag indicates that a particular chunk is being
 * stored back to the server.  Whenever this is happening, either
 * the vnode's storeCount is bumped or the vnode's status storing flag
 * is set.
 */
   

/*
 * Cache module related globals
 */
struct lock_data cm_scachelock;		/* lock: alloc new stat cache entries */
struct cm_scache *freeSCList = 0;	/* free list for stat cache entries */
struct cm_scache *Initial_freeSCList;	/* Ptr to orig malloced free list */
struct squeue SLRU;                     /* LRU queue for stat entries */
/*static*/ long cm_scount=0;		/* Runaway scache struct alloc cntr */
struct cm_scache *cm_shashTable[SC_HASHSIZE]; /* stat cache hash table */

static void GetDownS _TAKES((int));
static int FlushSCache _TAKES((struct cm_scache *, int));

/* holds bulkstat statistics */

struct sc_stats sc_stats;

/* 
 * This routine is responsible for allocating a new cache entry from the free 
 * list. It formats the cache entry and inserts it into the appropriate hash 
 * tables.  It must be called with cm_scachelock write-locked so as to prevent
 * several processes  from trying to create a new cache entry simultaneously.
 *    
 * param fidp is the file id of the file whose cache entry is being  created.
 */
struct cm_scache *cm_NewSCache(fidp, avolp)
  register afsFid *fidp;
  struct cm_volume *avolp;
{

    struct cm_scache *scp;
    register long i;
#ifdef	AFS_AIX_ENV
    struct gnode *gnodepnt;
#endif

    /* 
     * Pull out a free cache entry 
     */
    if (!freeSCList) {
	GetDownS(5);
	/*
	 * The cm_scachelock may have been dropped to recycle scache
	 * entries, so recheck to see if another thread already created
	 * an entry.
	 */
	if (scp = cm_FindSCache(fidp)) {
	    return scp;
	}
    }
    if (!freeSCList) {
	/* 
	 * none free, making one is better than a panic 
	 */
	cm_scount++;			/* count in case we have a leak */
	scp = (struct cm_scache *) osi_Alloc(sizeof (struct cm_scache));
	CM_BUMP_COUNTER(statusCacheEntries);
    } else {
	scp = freeSCList;  		/* take from free list */
	freeSCList = (struct cm_scache *) (scp->lruq.next);
    }
    bzero((char *) scp, sizeof(struct cm_scache)); /* clear whole enchilada */
    scp->fid = *fidp;
    CM_SETRC(scp, 1);
    scp->next = cm_shashTable[i = SC_HASH(fidp)];
    cm_shashTable[i] = scp;
    scp->ds.scanChunk = -1;	/* invalid */
    QInit(&scp->lockList);
    QInit(&scp->tokenList);
    QAdd(&SLRU, &scp->lruq);			/* put in lruq */
    lock_Init(&scp->llock);
#ifdef AFS_AIX31_VM
    lock_Init(&scp->pvmlock);
#endif
#ifdef AFS_SUNOS5_ENV
    rw_init(&scp->rwlock, "DFS vnode rwlock", RW_DEFAULT, DEFAULT_WT);
#endif
    scp->randomACL = (struct cm_aclent *) 0;
    /* 
     * Initialize vnode data
     */
#ifdef	AFS_AIX_ENV
    /* 
     * Don't forget to free the gnode space 
     */
    SCTOV(scp)->v_gnode = gnodepnt = (struct gnode*)osi_Alloc(sizeof(struct gnode));
    bzero((char *)gnodepnt, sizeof(struct gnode));
#endif
#ifndef SCACHE_PUBLIC_POOL
    osi_SetVnodeOps(scp, cm_GetAFSOps());
#endif
    scp->volp = avolp;
    if (avolp) {
	cm_HoldVolume(avolp);
	/* check if this is the volume root; mark it as such */
	if (!FidCmp(&scp->fid, &avolp->rootFid)) {
	    scp->states |= SC_VOLROOT;
	    /* and copy out ".." information, if present */
	    if (avolp->dotdot.Vnode != 0) {
		if (!scp->rootDotDot)
		    scp->rootDotDot = (afsFid *)osi_Alloc(sizeof (afsFid));
		*scp->rootDotDot = avolp->dotdot;
	    }
	}
	/* and propagate out read-only flag */
	if (avolp->states & VL_RO)
	    scp->states |= SC_RO;
    }
    if (cm_globalVFS == (struct osi_vfs *) 0)
	panic("NewSCache: afs globalvfs");
#ifndef SCACHE_PUBLIC_POOL
    osi_vSetVfsp(SCTOV(scp), cm_globalVFS);
    SET_CONVERTED(SCTOV(scp));
#else
    scp->v.v_vnode = NULLVP;
#endif /* SCACHE_PUBLIC_POOL */
    cm_vSetType(scp, VREG);
#ifdef	AFS_HPUX_ENV
    scp->v.v_fstype = VDFS;
#endif	/* AFS_HPUX_ENV */
#ifdef	AFS_AIX_ENV
    scp->v.v_vfsnext = cm_globalVFS->vfs_vnodes;  
    scp->v.v_vfsprev = NULL;
    cm_globalVFS->vfs_vnodes = &scp->v;
    if (scp->v.v_vfsnext != NULL)
	scp->v.v_vfsnext->v_vfsprev = &scp->v;
    scp->v.v_next = gnodepnt->gn_vnode;		/* Single vnode per gnode */
    gnodepnt->gn_vnode = &scp->v;
#endif
    /* now do some periodic work that may be triggered by creating a
     * new scache entry.
     */
    if (cm_needServerFlush) {
	lock_ReleaseWrite(&cm_scachelock);
	cm_ServerTokenMgt();
	lock_ObtainWrite(&cm_scachelock);
    }
    return scp;
}

/* 
 * This function takes a file id and request structure, and is responsible
 * for fetching the status information associated with the file.
 * 
 * The return value is a UNIX error code, which may be 0 or ESTALE.  The
 * caller is responsible for retrying if ESTALE is returned.
 *
 * The cache entry is passed back with an increased vrefCount field. The entry 
 * must be discarded by calling cm_PutSCache when you are through using the 
 * pointer to the cache entry.
 *
 * Since the scp is not returned locked, the only guarantee of token state
 * associated with the scp is OPEN_PRESERVE (due to the bumped vrefCount) and
 * therefore the immutable fields in the status information can be relied upon
 * without obtaining additional tokens.
 *
 * You should not hold any locks when calling this function, except locks on 
 * other scache entries. If you lock more than one scache entry simultaneously,
 * you should lock them in this order:
 *
 *    1.  Lock all files first, then directories.
 * 
 *    2.  Within a particular type, lock entries in Fid.Vnode order.
 *  
 * This locking hierarchy is convenient because it allows locking of a parent 
 * dir cache entry, given a file (to check its access control list).  It also 
 * allows renames to be handled easily by locking directories in a constant 
 * order.
 */
int cm_GetSCache(fidp, scpp, rreqp)
    register afsFid *fidp;
    struct cm_scache **scpp;
    struct cm_rrequest *rreqp; 
{
    register long code;
    register struct cm_conn *connp;
    register struct cm_scache *scp;
    afsFetchStatus OutStatus;
    afs_token_t Token;
    u_long startTime, flags = 0;
    struct cm_server *tserverp;
    afsVolSync tsync;
    struct cm_volume *volp;
    long srvix;
    afs_hyper_t *vvp;
    error_status_t st;
    int reallyOK;

    *scpp = (struct cm_scache *) 0;
    volp = cm_GetVolume(fidp, rreqp);
    if (!volp)
       return ENODEV;
    lock_ObtainWrite(&cm_scachelock);
    scp = cm_FindSCache(fidp);
    if (!scp)
	/* 
	 * No cache entry, better grab one 
	 */
	scp = cm_NewSCache(fidp, volp);
    lock_ReleaseWrite(&cm_scachelock);

    icl_Trace3(cm_iclSetp, CM_TRACE_GETSCACHE, ICL_TYPE_POINTER, scp,
	       ICL_TYPE_LONG, AFS_hgetlo(fidp->Volume),
	       ICL_TYPE_LONG, fidp->Vnode);

    lock_ObtainRead(&scp->llock);

    for (;;) {
	/* 
	 * Check if we're all done 
	 */
	if ((scp->states & SC_STATD) &&
	    cm_HaveTokens(scp, TKN_OPEN_PRESERVE)) {

	    lock_ReleaseRead(&scp->llock);
	    if (volp) 
		cm_PutVolume(volp);

	    *scpp = scp;
	    return 0;
	}

	lock_ReleaseRead(&scp->llock);
	lock_ObtainWrite(&scp->llock);
	if (scp->states & SC_FETCHING) {
	    /* someone else already fetching status for this file */
	    scp->states |= SC_WAITING;
	    osi_SleepW(&scp->states, &scp->llock);
	    lock_ObtainRead(&scp->llock);
	    continue;	/* from the top requires read lock*/
	}
	/* if we get here, we've committed to doing a stat RPC call,
	 * and we have a write lock on the scache entry.  We've set the
	 * status fetching flag, and so others will wait until we're done.
	 */
	scp->states |= SC_FETCHING;
	break;
    } /* for (;;) */

    /* 
     * Stat the file 
     */
    AFS_hzero(Token.beginRange);
    Token.endRange = osi_hMaxint64;
    AFS_hset64(Token.type, 0, TKN_STATUS_READ | TKN_OPEN_PRESERVE);
    if (cm_HaveTokensRange(scp, &Token, 0, &reallyOK))
	flags = 0;
    else if (reallyOK)
	/* Don't request tokens if we already have them. */
	flags = 0;
    else
	flags = AFS_FLAG_RETURNTOKEN;
    lock_ReleaseWrite(&scp->llock);
    cm_StartTokenGrantingCall();

    /* Normally cm_StartVolumeRPC() is called by cm_Conn, but since
     * in this case volp will not be NULL it won't so we call it here
     * to note the fact that we start an RPC for this volume.
     */
    if (volp) 
	cm_StartVolumeRPC(volp);
    do {
	tserverp = 0;
	if (connp = cm_Conn(fidp, MAIN_SERVICE(SRT_FX),
			    rreqp, &volp, &srvix)) {
	    if (volp && ((volp->states & VL_LAZYREP) != 0))
		vvp = &volp->latestVVSeen;
	    else
		vvp = &osi_hZero;
	    icl_Trace0(cm_iclSetp, CM_TRACE_SCACHESTART);
	    startTime = osi_Time();
	    st = BOMB_EXEC
		("COMM_FAILURE",
		 (osi_RestorePreemption(0),
		  st = AFS_FetchStatus(connp->connp, fidp, vvp, flags,
				       &OutStatus, &Token, &tsync),
		  osi_PreemptionOff(),
		  st));
	    code = osi_errDecode(st);
	    tserverp = connp->serverp;
	    cm_SetReqServer(rreqp, tserverp);
	    icl_Trace1(cm_iclSetp, CM_TRACE_SCACHEEND, ICL_TYPE_LONG, code);
	} else 
	    code = -1;
	if (code == 0) {
	    /* Loop back if this call went backwards in VV-time */
	    code = cm_CheckVolSync(fidp, &tsync, volp, startTime, srvix, rreqp);
	}
    } while (cm_Analyze(connp, code, fidp, rreqp, volp, srvix, startTime));
    lock_ObtainWrite(&scp->llock);

    /* wakeup others waiting for new status info */
    scp->states &= ~SC_FETCHING;
    if (scp->states & SC_WAITING) {
	scp->states &= ~SC_WAITING;
	osi_Wakeup(&scp->states);
    }

    if (code) {
	cm_EndTokenGrantingCall((afs_token_t *) 0, (struct cm_server *) 0,
				(struct cm_volume *) 0, startTime);
	/* now, if we're still within hardMaxTotalLatency, we're fine
	 * anyway.  So, if this is a readonly and we have stat info,
	 * and either we have no hard max latency, or it hasn't been
	 * exceeded, return success anyway.
	 */
	if ((reallyOK & CM_REALLYOK_REPRECHECK) && (scp->states & SC_STATD)) {
	    lock_ReleaseWrite(&scp->llock);
	    if (volp) 	
		cm_EndVolumeRPC(volp);		/* done with it */
	    *scpp = scp;
	    return 0;
	}
	else {
	    /* failed, and we can't use what we've got */
	    lock_ReleaseWrite(&scp->llock);
	    lock_ObtainWrite(&cm_scachelock);
	    CM_RELE(scp);
	    lock_ReleaseWrite(&cm_scachelock);
	    if (volp) 	
		cm_EndVolumeRPC(volp);		/* done with it */
	    return code;
	}
    }
    cm_EndTokenGrantingCall(&Token, tserverp, scp->volp, startTime);
    tokenStat.FetchStatus++;
    cm_MergeStatus(scp, &OutStatus, &Token, 0, rreqp);
    /* Check for race with deletion */
    code = cm_GetTokens(scp, TKN_OPEN_PRESERVE, rreqp, WRITE_LOCK);
    lock_ReleaseWrite(&scp->llock);
    if (volp) 	
	cm_EndVolumeRPC(volp);		/* done with it */
    if (code) {
	cm_PutSCache(scp);
	return code;
    }
    *scpp = scp;
    return 0;
}


/* 
 * Find a scache entry.  Must be called with the cm_scachelock write locked.
 * (For CM_HOLD to work on an MP).
 */
struct cm_scache *cm_FindSCache(fidp)
    register afsFid *fidp; 
{
    register struct cm_scache *scp;

    for (scp = cm_shashTable[SC_HASH(fidp)]; scp; scp = scp->next) {
        if (!FidCmp(&scp->fid, fidp) && scp->asyncStatus == 0) {
	    break;
        }
    }

    if (scp) {

	/* installed by bulkstat? */

	if(scp->installStates & BULKSTAT_INSTALLEDBY_BULKSTAT)
	{
		scp->installStates |= BULKSTAT_SEEN;
        	sc_stats.statusCacheBulkstatSeen++;
	}

	CM_BUMP_COUNTER(statusCacheHits);
        CM_HOLD(scp);
        QRemove(&scp->lruq);
        QAdd(&SLRU, &scp->lruq);                /* Move to lruq head */
    }
    else {
	CM_BUMP_COUNTER(statusCacheMisses);
    }
    return scp;
}


#if CM_MAXFIDSZ<24

/* 
 * Find an scache entry for NFS (cm_vget).  Identical to cm_FindSCache (above),
 * but does a partial match, since some bits of the FID are not known.
 * There may be ambiguity (more than one match), in which case we fail.
 */
struct cm_scache *cm_NFSFindSCache(fidp)
    register afsFid *fidp; 
{
    register struct cm_scache *scp;
    struct cm_scache *found_scp = NULL;

    for (scp = cm_shashTable[SC_HASH(fidp)]; scp; scp = scp->next) {

#if CM_MAXFIDSZ>=20

	/* Ignore high 32 bits of Volume */
	if (scp->fid.Unique == fidp->Unique
	    && scp->fid.Vnode == fidp->Vnode
	    && AFS_hgetlo(scp->fid.Volume) == AFS_hgetlo(fidp->Volume)
	    && AFS_hsame(scp->fid.Cell, fidp->Cell)
	    && scp->asyncStatus == 0) {
	    if (found_scp)
		return NULL;
	    else found_scp = scp;
	}

#else

	/* Ignore high 48 bits of Volume and high 16 bits of Vnode */
	if (scp->fid.Unique == fidp->Unique
	    && (scp->fid.Vnode & 0xfffff) == (fidp->Vnode & 0xfffff)
	    && ((AFS_hgetlo(scp->fid.Volume) & 0xfffff) ==
		(AFS_hgetlo(fidp->Volume) & 0xfffff))
	    && AFS_hsame(scp->fid.Cell, fidp->Cell)
	    && scp->asyncStatus == 0) {
	    if (found_scp)
		return NULL;
	    else found_scp = scp;
        }

#endif /* CM_MAXFIDSZ>=20 */

    }

    scp = found_scp;
    if (scp != NULL) {
	CM_BUMP_COUNTER(statusCacheHits);
        CM_HOLD(scp);
        QRemove(&scp->lruq);
        QAdd(&SLRU, &scp->lruq);                /* Move to lruq head */
    }
    else {
	CM_BUMP_COUNTER(statusCacheMisses);
    }
    return scp;
}

#endif /* CM_MAXFIDSZ<24 */


/*
 * Mark access, modify and change times to be userTime. 0 (or current time if 
 * userTime is equal to zero.
 *
 * Requires write lock on llock.
 */
int cm_MarkTime(scp, userTime, credp, flag, doLock)
  osi_cred_t *credp;
  register struct cm_scache *scp;
  long userTime;
  long flag;			/* Which time to modify */
  int doLock;
{
    register long code;
    struct cm_rrequest treq;
    
    if (userTime == 0)   /* user not providing the time */
        userTime = osi_Time();

    if (flag & ~CM_MODACCESS) {
	icl_Trace3(cm_iclSetp, CM_TRACE_MARKTIME, ICL_TYPE_POINTER, scp,
		   ICL_TYPE_LONG, userTime, ICL_TYPE_LONG, flag);

	/* if we're doing more than an access time update (for which we
	 * do not want to bother getting an update token), and if we
	 * have a credential, then get the appropriate token.
	 */
	if (doLock) {
	    cm_InitReq(&treq, credp);
	    if (code = cm_GetTokens(scp, TKN_UPDATESTAT, &treq, WRITE_LOCK))
		return cm_CheckError(code, &treq);
	}
    }
    if (flag & CM_MODACCESS) {
        scp->m.ModFlags |= CM_MODACCESS;
	scp->m.AccessTime.sec = userTime;
	scp->m.AccessTime.usec = 0;
    }
    if (flag & CM_MODMOD) {
        scp->m.ModFlags |= CM_MODMOD;
	/* utimes, which is the only call that needs to set the mtime
	 * to an exact value (and should thus disable the server code
	 * that forces the mtime to be a good version # and monotonically
	 * increase), doesn't use this path, so we always turn off
	 * MODMODEXACT here.
	 */
	scp->m.ModFlags &= ~CM_MODMODEXACT;	/* not from a utimes */
	scp->m.ModTime.sec = userTime;
	scp->m.ModTime.usec = 0;
    }
    if (flag & CM_MODCHANGE) {
        scp->m.ModFlags |= CM_MODCHANGE;
	scp->m.ChangeTime.sec = userTime;
	scp->m.ChangeTime.usec = 0;
    }
    return 0;
}


/*
 * This function is called to decrement the reference count on a cache entry. 
 */
void cm_PutSCache(scp)
    register struct cm_scache *scp; 
{
    lock_ObtainWrite(&cm_scachelock);
    CM_RELE(scp);
    lock_ReleaseWrite(&cm_scachelock);
}


/* called with at least a read-locked scp.
 * tells whether we need to store the status from this file or not.
 */
int cm_NeedStatusStore(scp)
  register struct cm_scache *scp;
{
    /* cm_NeedStatusStore MUST NOT be more aggressive than what cm_ScanStatus 
     * can consume. Else, the loop in RevokeStatusToken cannot make sense.
     */
     if ((scp->m.ModFlags & (CM_MODTRUNCPOS | CM_MODLENGTH | CM_MODMOD
			      | CM_MODOWNER | CM_MODGROUP | CM_MODMODE)) ||
	(((scp->states & SC_RO) == 0) && (scp->m.ModFlags & CM_MODACCESS)
	 && scp->m.AccessTime.sec > scp->m.serverAccessTime + CM_ACCESSSKEW))
	return 1;
    else
	return 0;
}


/*
 * This function is called to increment the reference count on a cache entry. 
 */
void cm_HoldSCache(scp)
    register struct cm_scache *scp; 
{
    lock_ObtainWrite(&cm_scachelock);
    CM_HOLD(scp);
    lock_ReleaseWrite(&cm_scachelock);
}


/* 
 * This function takes a fid and a cm_rrequest structure, and is responsible 
 * for storing the status information back to the server.  This routine 
 * must be called with the llock write-locked and uses the X file service.
 * This function drops the llock while storing the updates.
 *
 * The storeStatusp field describes a set of changes to be made
 * to the vnode only if the RPC succeeds.  The caller should merge
 * those changes in with those from cm_ScanStatus before calling
 * this function, or pass in a null, and we'll do the scan status.
 *
 */
cm_StoreSCache(scp, storeStatusp, fsyncFlag, rreqp)
    register struct cm_scache *scp;
    register afsStoreStatus *storeStatusp;
    int fsyncFlag;
    struct cm_rrequest *rreqp; 
{
    register long code;
    register struct cm_conn *connp;
    struct lclHeap {
	afsFetchStatus OutStatus;
	afsStoreStatus tstatus;
	afsVolSync tsync;
    };
#ifdef AFS_SMALLSTACK_ENV
    struct lclHeap *lhp = NULL;
#else /* AFS_SMALLSTACK_ENV */
    struct lclHeap lh;
    struct lclHeap *lhp = &lh;
#endif /* AFS_SMALLSTACK_ENV */
    struct cm_volume *volp = (struct cm_volume *)0;
    long serverFlags;
    long flags;
    long srvix;
    u_long startTime;
    error_status_t st;

    if (code = scp->asyncStatus) {
       return code;
    }

#ifdef AFS_SMALLSTACK_ENV
    /* Keep stack frame small by doing heap-allocation */
    lhp = (struct lclHeap *)osi_AllocBufferSpace();
    /* CONSTCOND */
    osi_assert(sizeof(struct lclHeap) <= osi_BUFFERSIZE);
#endif /* AFS_SMALLSTACK_ENV */
    if (!storeStatusp) {
	/* stabilize the cache entry and mark it as being stored.
	 * don't StabilizeSCache if storeStatusp passed in, as scache
	 * already stabilized, and we don't want more
	 * dirty bits being added before we get SC_STORING set.
	 */
	cm_StabilizeSCache(scp);
	lhp->tstatus.mask = 0;	/* init */
	storeStatusp = &lhp->tstatus;
	cm_ScanStatus(scp, storeStatusp);	/* add local status */
    }

    if (storeStatusp->mask == 0 && !fsyncFlag) {
#ifdef AFS_SMALLSTACK_ENV
	osi_FreeBufferSpace((struct osi_buffer *)lhp);
#endif /* AFS_SMALLSTACK_ENV */
	return 0;	/* nothing to do */
    }

    /* otherwise, commit to storing */
    scp->states |= SC_STORING;
    scp->states &= ~SC_STOREINVAL;

    if (fsyncFlag)
	serverFlags = AFS_FLAG_SKIPTOKEN | AFS_FLAG_SYNC;
    else
	serverFlags = AFS_FLAG_SKIPTOKEN;

    /* release the lock to make the RPC */
    lock_ReleaseWrite(&scp->llock);
    do {
	if (connp = cm_Conn(&scp->fid,  SEC_SERVICE(SRT_FX),
			    rreqp, &volp, &srvix)) {
	    /* Check whether TSR trashed this store. */
	    code = 0;
	    lock_ObtainWrite(&scp->llock);
	    if ((scp->states & SC_STOREINVAL) || scp->asyncStatus) {
		scp->states &= ~SC_STOREINVAL;
		if (!code) code = scp->asyncStatus;
		if (!code) code = ESTALE;	/* anything but 0 */
		icl_Trace3(cm_iclSetp, CM_TRACE_STORESCACHECONNINVALID,
			   ICL_TYPE_POINTER, scp,
			   ICL_TYPE_FID, &scp->fid,
			   ICL_TYPE_LONG, code);
	    }
	    lock_ReleaseWrite(&scp->llock);
	    if (!code) {
		icl_Trace0(cm_iclSetp, CM_TRACE_STORESTATUSSTART);
		startTime = osi_Time();
		st = BOMB_EXEC("COMM_FAILURE",
			       (osi_RestorePreemption(0),
				st = AFS_StoreStatus(connp->connp, &scp->fid, 
						     storeStatusp, &osi_hZero,
						     serverFlags, &lhp->OutStatus, 
						     &lhp->tsync),
				osi_PreemptionOff(),
				st));
		code = osi_errDecode(st);
		icl_Trace1(cm_iclSetp, CM_TRACE_STORESTATUSEND, ICL_TYPE_LONG, code);
	    }
	} else 
	    code = -1;
    } while (cm_Analyze(connp, code, &scp->fid, rreqp, volp, srvix, startTime));
    lock_ObtainWrite(&scp->llock);
    if (code == 0) {
        /* Success; do the changes locally */
	flags = CM_MERGE_STORING;	/* means we did a ScanStatus already */
	if (storeStatusp->mask & (AFS_SETMODE | AFS_SETOWNER | AFS_SETGROUP))
	    flags |= CM_MERGE_CHACL;
	cm_MergeStatus(scp, &lhp->OutStatus, (afs_token_t *) 0, flags, rreqp);
	(void) cm_CheckVolSync(&scp->fid, &lhp->tsync, volp, startTime, srvix, rreqp);
    }
    /* nothing to do if fail, since we want to leave
     * already performed vnode changes around for bkg daemon to do
     */

    /* clear storing flag, and wakeup anyone waiting */
    scp->states &= ~SC_STORING;
    if ((scp->states & SC_STOREINVAL) || scp->asyncStatus) {
	scp->states &= ~SC_STOREINVAL;
	if (!code) code = scp->asyncStatus;
	if (!code) code = ESTALE;	/* anything but 0 */
	icl_Trace3(cm_iclSetp, CM_TRACE_STORESCACHEINVALID,
		   ICL_TYPE_POINTER, scp,
		   ICL_TYPE_FID, &scp->fid,
		   ICL_TYPE_LONG, code);
    }
    if (scp->states & SC_WAITING) {
	scp->states &= ~SC_WAITING;
	osi_Wakeup(&scp->states);
    }
#ifdef AFS_SMALLSTACK_ENV
    osi_FreeBufferSpace((struct osi_buffer *)lhp);
#endif /* AFS_SMALLSTACK_ENV */
    if (volp) 
	cm_EndVolumeRPC(volp);
    return code;
}


/*
 * This function stores back all of the modified status information for this 
 * file. It is called without any locks.
 */
long cm_SyncSCache(scp)
  register struct cm_scache *scp;
{
    register long code = 0;
    struct cm_rrequest treq;

    icl_Trace1(cm_iclSetp, CM_TRACE_SYNCSCACHE, ICL_TYPE_POINTER, scp);
    lock_ObtainWrite(&scp->llock);
    if (scp->m.ModFlags & CM_MODMASK) {
	/* figure out what credential we need to use, based on opener.
	 */
	if (scp->writerCredp)
	    cm_InitReq(&treq, scp->writerCredp);
	else
	    cm_InitReq(&treq, osi_credp);

	/* finally, do the work */
	code = cm_StoreSCache(scp, (afsStoreStatus *) 0, 0, &treq);
	if ((code = cm_CheckError(code, &treq)) == 0)
	    goto out;
	if (CM_ERROR_TRANSIENT(code)) {
	    if (!(scp->states & SC_STOREFAILED)) {
		scp->storeFailTime = osi_Time();
		scp->storeFailStatus = code;
		scp->states |= SC_STOREFAILED;
	    }
	    icl_Trace1(cm_iclSetp, CM_TRACE_SYNCSCACHEFAIL, ICL_TYPE_POINTER, 
		       scp);
	    goto out;
	}
	cm_MarkBadSCache(scp, code);
    }

out:
    lock_ReleaseWrite(&scp->llock);
    return code;
}

/*
 * Copy in random modified status information to the store status block.
 * Vnode llock must be write-locked.
 */
void cm_ScanStatus(scp, storeStatusp)
    struct cm_scache *scp;
    afsStoreStatus *storeStatusp; 
{
    /* cm_NeedStatusStore MUST NOT be more aggressive than what cm_ScanStatus can consume. */
    /* Else, the loop in RevokeStatusToken cannot make sense. */
    icl_Trace2(cm_iclSetp, CM_TRACE_SCANSTATUS, ICL_TYPE_POINTER, scp,
	       ICL_TYPE_LONG, scp->m.ModFlags);
    if (scp->m.ModFlags & CM_MODTRUNCPOS) {
	storeStatusp->mask |= AFS_SETTRUNCLENGTH;
	AFS_hset64(storeStatusp->truncLength, 0, scp->truncPos);
    }
    if (scp->m.ModFlags & CM_MODLENGTH) {
	storeStatusp->mask |= AFS_SETLENGTH;
	AFS_hset64(storeStatusp->length, 0, scp->m.Length);
    }
    if (scp->m.ModFlags & CM_MODMOD) {
	storeStatusp->mask |= AFS_SETMODTIME;
	if (scp->m.ModFlags & CM_MODMODEXACT)
	    storeStatusp->mask |= AFS_SETMODEXACT;
	storeStatusp->modTime = scp->m.ModTime;
    }
#if 0
    if (scp->m.ModFlags & CM_MODCHANGE) {
	/* 	
	 * don't do this one, it isn't handled by the server 
	 */
    }
#endif /* 0 */
    if (scp->m.ModFlags & CM_MODOWNER) {
	storeStatusp->mask |= AFS_SETOWNER;
	storeStatusp->owner = scp->m.Owner;
    }
    if (scp->m.ModFlags & CM_MODGROUP) {
	storeStatusp->mask |= AFS_SETGROUP;
	storeStatusp->group = scp->m.Group;
    }
    if (scp->m.ModFlags & CM_MODMODE) {
	storeStatusp->mask |= AFS_SETMODE;
	storeStatusp->mode = scp->m.Mode;
    }
    /* This is where we put code to skip setting access time too often.
     * If atime is within an hour, or we're making the call anyway,
     * then the access time at the server is good enough for us.
     * Explicit setting via utimes marks CM_MODACCESSEXACT, which
     * is covered by ~CM_MODACCESS.
     */
    if ((scp->m.ModFlags & CM_MODACCESS)
	&& ((scp->m.AccessTime.sec > scp->m.serverAccessTime + CM_ACCESSSKEW)
	    || (scp->m.ModFlags & ~CM_MODACCESS))) {
	storeStatusp->mask |= AFS_SETACCESSTIME;
	storeStatusp->accessTime = scp->m.AccessTime;
    }
}

/* 
 * Clear those bits that triggered the last scan status; used after a 
 * successful call to cm_ScanStatus to prevent storing the same info again.
 * The vnode llock must be write locked.
 */
void cm_ClearScan(scp)
  register struct cm_scache *scp; 
{
    scp->m.ModFlags &= ~(CM_MODLENGTH | CM_MODMOD | CM_MODCHANGE | 
		 CM_MODACCESS | CM_MODOWNER | CM_MODGROUP | CM_MODMODE |
		 CM_MODMODEXACT | CM_MODTRUNCPOS | CM_MODACCESSEXACT);
}


/*
 * Reset access associated with all cached files for a
 * particular user.
 */
void cm_ResetAccessCache(apag)
     register long apag;
{
    register int i;
    register struct cm_scache *scp;

    lock_ObtainWrite(&cm_scachelock);
    for (i = 0; i < SC_HASHSIZE; i++) {
	for (scp = cm_shashTable[i]; scp; scp = scp->next) {
	    CM_HOLD(scp);
	    lock_ReleaseWrite(&cm_scachelock);
	    lock_ObtainWrite(&scp->llock);
	    cm_InvalidateACLUser(scp, apag);
	    lock_ReleaseWrite(&scp->llock);
	    lock_ObtainWrite(&cm_scachelock);
	    CM_RELE(scp);
	}
    }
    lock_ReleaseWrite(&cm_scachelock);
}

/* 
 * This routine is responsible for moving at least one, (up to number) entries 
 * from the LRU queue to the free queue.  Number is just a hint, but this 
 * routine  must (if possible) move at least one entry, or its caller will 
 * panic.  This routine must be called with cm_scachelock write-locked.
 */
static void GetDownS(number)
    int number; 
{
    register struct squeue *tq;
    struct squeue *uq;
    register struct cm_scache *scp;

    if (lock_Check(&cm_scachelock) != -1) 
	panic("getdownS lock");
    for (tq = SLRU.prev; tq != &SLRU; tq = uq) {
	scp = SC_QTOS(tq);
	if (CM_RC(scp) == 0) {		/* if open, ref count is fine */
	    if (!FlushSCache(scp, 0))
		break;
	}
	/* do this after the FlushSCache, since it releases the
	 * scachelock, and uq might be bad.  However, tq isn't bad,
	 * since FlushSCache holds the vnode when the scachelock is
	 * released.  Note that if FlushSCache actually frees the vnode,
	 * we break out above.
	 */
	uq = QPrev(tq);
    }
}


/* 
 * This routine must be called with the cm_scachelock lock held for writing,
 * which prevents people from changing the vrefCount field. 
 */
static int FlushSCache(scp, shuttingDown)
    register struct cm_scache *scp;
    int shuttingDown;
{
    register long i;
    register struct cm_scache **uscp, *wscp;

    if (CM_RC(scp) != 0) {
	if (!shuttingDown) {
	    return EBUSY;
	}
    }

    /*
     * Flush VM for this file.  On Solaris, this also checks whether the
     * open count is high.
     */
    if (cm_VMFlushSCache(scp) != 0)
	return EBUSY;

    /* Other generic checks: don't recycle a vnode that has dirty
     * data, since even if unused, we need to keep token around
     * until data sent back to server.  Same applies to modified
     * status fields.  Use cm_NeedStatusStore since it knows what
     * atime values we'd really bother to send back to the server.
     *
     * Note that open or lock tokens require an open file descriptor,
     * and so we don't have to worry about a 0 reference count file
     * requiring those tokens in order to be processed properly.
     *
     * Don't recycle a vnode if the bkg daemon hasn't yet returned
     * its TKN_OPEN_PRESERVE token.
     */
    if (scp->modChunks > 0 || cm_NeedStatusStore(scp)) return EBUSY;
    if (scp->states & SC_RETURNREF) return EBUSY;

    icl_Trace1(cm_iclSetp, CM_TRACE_FLUSHSCACHE, ICL_TYPE_POINTER, scp);
    CM_BUMP_COUNTER(statusCacheFlushes);

    /* installed by bulkstat and never referrenced? */

    if((scp->installStates & BULKSTAT_INSTALLEDBY_BULKSTAT) &&
	(!(scp->installStates & BULKSTAT_SEEN)))
            sc_stats.statusCacheBulkstatNotseen++;

    /* 
     * pull the entry out of the lruq and put it on the free list 
     */
    QRemove(&scp->lruq);
    /* 
     * Remove entry from the hash chain 
     */
    i = SC_HASH(&scp->fid);
    uscp = &cm_shashTable[i];
    for (wscp = *uscp; wscp; uscp = &wscp->next, wscp = *uscp) {
	if (scp == wscp) {
	    *uscp = scp->next;
	    break;
	}
    }
    if (!wscp) 
	panic("flushscache");	/* not in correct hash bucket */
    if (scp->volp) {
	cm_PutVolume(scp->volp);
	scp->volp = (struct cm_volume *) 0;
    }
    if (scp->rootDotDot) 
	osi_Free((opaque)(scp->rootDotDot), sizeof(afsFid));
#ifdef	AFS_AIX_ENV
    /* 
     * Free the alloced gnode that was accompanying the scache's vnode 
     */
    aix_gnode_rele(SCTOV(scp));
#endif
    cm_QuickQueueTokens(scp);		/* queue tokens 4 respective servers */
    cm_FreeAllTokens(&scp->tokenList);	/* free any left, just in case */
    cm_ClearOnlineState(scp);
    cm_FreeAllACLEnts(scp);
    cm_FreeAllCookies(scp);
    if (scp->states & SC_VDIR) cm_FreeAllVDirs(scp);
    if (scp->linkDatap) 
	osi_Free(scp->linkDatap, strlen(scp->linkDatap)+1);
    /* 
     * Put the entry in the free list and free the callback 
     */
    scp->lruq.next = (struct squeue *) freeSCList;
    freeSCList = scp;
    if (scp->writerCredp) {
	crfree(scp->writerCredp);
	scp->writerCredp = (osi_cred_t *) 0;
    }
#ifdef AFS_SUNOS5_ENV
    rw_destroy(&scp->rwlock);
    if (scp->NFSCredp) {
	crfree(scp->NFSCredp);
	scp->NFSCredp = (osi_cred_t *) 0;
    }
#endif
    return 0;
}

/* 
 * Return the scache (held) for the fileset root.
 */
struct cm_scache *cm_GetRootSCache(fidp, rreqp)
     afsFid *fidp;
     struct cm_rrequest *rreqp;
{
    register struct lclHeap {
	afsFetchStatus OutFidStatus;
	afs_token_t Token;
	afsFid outFid;
	afs_hyper_t savedCellID;
	afsVolSync tsync;
    };
#ifdef AFS_SMALLSTACK_ENV
    struct lclHeap *lhp = NULL;
#else /* AFS_SMALLSTACK_ENV */
    struct lclHeap lh;
    struct lclHeap *lhp = &lh;
#endif /* AFS_SMALLSTACK_ENV */
    struct cm_scache *rootscp = NULL;
    struct cm_server *tserverp;
    struct cm_volume *volp;
    u_long tokenFlag;
    register struct cm_conn *connp;
    long srvix;
    u_long startTime;
    afs_hyper_t *vvp;
    error_status_t st;
    long code;

    volp = cm_GetVolume(fidp, rreqp);
    if (!volp)
       return (struct cm_scache *) 0;

#ifdef AFS_SMALLSTACK_ENV
    /* Keep stack frame small by doing heap-allocation */
    lhp = (struct lclHeap *)osi_AllocBufferSpace();
    /* CONSTCOND */
    osi_assert(sizeof(struct lclHeap) <= osi_BUFFERSIZE);
#endif /* AFS_SMALLSTACK_ENV */

    /* 
     * save the cell id for later use after the rpc call. 
     */
    lhp->savedCellID = fidp->Cell;
    if (volp->states & VL_RO)
	tokenFlag = AFS_FLAG_SKIPTOKEN;
    else {
	tokenFlag = AFS_FLAG_RETURNTOKEN;
	AFS_hzero(lhp->Token.beginRange);
	lhp->Token.endRange = osi_hMaxint64;
	AFS_hset64(lhp->Token.type, 0, TKN_STATUS_READ | TKN_OPEN_PRESERVE);
    }
    cm_StartTokenGrantingCall();
    /* Normally cm_StartVolumeRPC() is called by cm_Conn, but since
     * in this case volp will not be NULL it won't so we call it here
     * to note the fact that we start an RPC for this volume.
     */
    if (volp) 
	cm_StartVolumeRPC(volp);

    do {
	tserverp = 0;
	if (connp = cm_Conn(fidp, MAIN_SERVICE(SRT_FX), rreqp, &volp, &srvix)){
	    if (volp && ((volp->states & VL_LAZYREP) != 0))
		vvp = &volp->latestVVSeen;
	    else
		vvp = &osi_hZero;
	    icl_Trace0(cm_iclSetp, CM_TRACE_LOOKUPROOTSTART);
	    startTime = osi_Time();
	    st = BOMB_EXEC("COMM_FAILURE",
			   (osi_RestorePreemption(0),
			    st = AFS_LookupRoot(connp->connp, fidp, vvp,
				tokenFlag, &lhp->outFid, &lhp->OutFidStatus,
				&lhp->Token, &lhp->tsync),
			    osi_PreemptionOff(),
			    st));
	    code = osi_errDecode(st);
	    tserverp = connp->serverp;
	    cm_SetReqServer(rreqp, tserverp);
	    icl_Trace1(cm_iclSetp, CM_TRACE_LOOKUPROOTEND, ICL_TYPE_LONG,code);
	} else 
	    code = -1;
	if (code == 0) {
	    /* Loop if we went backward in VV-time */
	    code = cm_CheckVolSync(fidp, &lhp->tsync, volp, startTime, srvix,
				   rreqp);
	}
    } while (cm_Analyze(connp, code, fidp, rreqp, volp, srvix, startTime));
    if (code != 0) {
	cm_EndTokenGrantingCall((afs_token_t *) 0, (struct cm_server *) 0,
				(struct cm_volume *) 0, startTime);
	cm_EndVolumeRPC(volp);
#ifdef AFS_SMALLSTACK_ENV
	osi_FreeBufferSpace((struct osi_buffer *)lhp);
#endif /* AFS_SMALLSTACK_ENV */
	return NULL;
    }
    /*
     * Now process the new info for the fileset root. Note, OutFid contains
     * an appropriate vnode info, ie, (vnodeId, vnodeUnique) provided by FX.
     */
    lock_ObtainWrite(&cm_scachelock);

    /* Adjust the cell ID, in case it has been changed by FX servers */

    lhp->outFid.Cell = lhp->savedCellID;
    /*
     * Use NULL for the old-server values, since we do not know that
     * we have the HERE token for this fileset.
	 */
    rootscp = cm_FindSCache(&lhp->outFid);
    if (rootscp) { /* race condition */
       /* 
	* well, someone else already got the root scache for this fileset.
	* Just merge the new status. 
	*/
       lock_ReleaseWrite(&cm_scachelock);
       lock_ObtainWrite(&rootscp->llock);
       cm_EndTokenGrantingCall(&lhp->Token, tserverp, rootscp->volp, startTime);
       cm_MergeStatus(rootscp, &lhp->OutFidStatus, &lhp->Token, 0, rreqp);
       lock_ReleaseWrite(&rootscp->llock);
    } else  {
       rootscp = cm_NewSCache(&lhp->outFid, volp);
       lock_ReleaseWrite(&cm_scachelock);
       lock_ObtainWrite(&rootscp->llock);
       rootscp->states |= SC_VOLROOT;
       cm_EndTokenGrantingCall(&lhp->Token, tserverp, rootscp->volp, startTime);
       cm_MergeStatus(rootscp, &lhp->OutFidStatus, &lhp->Token, 0, rreqp);
       lock_ReleaseWrite(&rootscp->llock);
    }
    cm_EndVolumeRPC(volp);
#ifdef AFS_SMALLSTACK_ENV
    osi_FreeBufferSpace((struct osi_buffer *)lhp);
#endif /* AFS_SMALLSTACK_ENV */
    return rootscp;
}

/*
 * This is called to mark this scache as "BAD" after the CM decides that it
 * could not reuse this scache. So, subsequent operations to this file will
 * be denied.
 *
 * Must be called with scp write-locked.
 */
void cm_MarkBadSCache(scp, asyncCode)
    struct cm_scache *scp;
    long asyncCode;
{
    /* watch for recursive calls to this function, in case something
     * goes wrong with cm_ForceReturnToken (not too unlikely).
     */
    if (scp->states & SC_MARKINGBAD) 
        return;

    icl_Trace2(cm_iclSetp, CM_TRACE_MARKBADSCACHE,
	       ICL_TYPE_POINTER, (long) scp,
	       ICL_TYPE_LONG, asyncCode);

    scp->states |= SC_MARKINGBAD;

    /* now we want to stabilize the scache entry against all
     * stores (we can't do fetches since this is called from
     * callback procedures, but cm_InvalidateAllSegments now knows
     * how to handle a chunk being fetched, so that it will be
     * invalidated after the fetch is over, before FETCHING is cleared.
     *
     * This next call can block and release the scp lock.
     */
    /* cm_StabilizeSCache(scp); */
    /* Nope, we don't want to stabilize this entry now.  The problem is that
     * we might be being called because TSR failed for a file, underneath
     * cm_Conn(), and cm_Conn() might be being called by cm_StoreDCache() or
     * any of the other procedures that will turn off SC_STORING only when
     * they exit.
     * 
     * Instead, cm_InvalidateAllSegments will have to tolerate non-stabilized
     * scache entries and set SC_STOREINVAL and/or DC_DFSTOREINVAL; when a
     * caller goes to turn off SC_STORING or DC_DFSTORING, they'll notice
     * those xxxSTOREINVAL bits and propagate their failure.
     */

    /* if it is a file, mark vnode as bad, so that all references to
     * this vnode (from already open files, esp.) will get asyncCode
     * as an error.
     */
    if (cm_vType(scp) != VDIR) {
	lock_ObtainWrite(&cm_scachelock);
	if (!scp->asyncStatus)
	    scp->asyncStatus = asyncCode;
	lock_ReleaseWrite(&cm_scachelock);
    }

    /* Turn off all dirty status bits.  We've already returned any
     * tokens we have that cover dirty data, and invalidated caches.
     * Now, turn off all status stuff.
     */
    cm_ClearScan(scp);
    scp->m.ModFlags &= ~CM_MODTRUNCPOS;

    /* discard all chunks, dirty or otherwise */
    cm_InvalidateAllSegments(scp,
		/* don't stabilize! */ CM_INVAL_SEGS_MARKINGBAD);

    scp->states &= ~SC_STOREFAILED;	/* no longer trying */

    /* discard caches supported by all data and status tokens; this
     * call releases the scp lock, so do it after above so we don't
     * allow vnode to be de-stabilized again.
     */
#ifdef AFS_SUNOS5_ENV
    /*
     * On Solaris, cm_ForceReturnToken may cause calls to cm_putpage, which
     * will wait for SC_PPLOCKED to clear.  The process that sets and clears
     * that flag may, however, be waiting for cm_Conn, which may be waiting
     * for TSR to complete, which may be waiting for us.  To avoid this
     * deadlock, let that process call cm_ForceReturnToken.
     */
    if (!(scp->states & SC_PPLOCKED))
	cm_ForceReturnToken(scp, TKN_UPDATE, 0);
#else
    cm_ForceReturnToken(scp, TKN_UPDATE, 0);
#endif

    icl_Trace2(cm_iclSetp, CM_TRACE_MARKBADSCACHE_2,
	       ICL_TYPE_POINTER, (long) scp,
	       ICL_TYPE_LONG, asyncCode);

    /* finally, mark that we're out of this function */
    scp->states &= ~SC_MARKINGBAD;
}

#if defined(SCACHE_PUBLIC_POOL) && defined(AFS_OSF_ENV)
cm_reclaim(vp)
register struct vnode *vp;
{
	struct cm_scache *scp;
	struct cm_scache **tscp;
	long code;

	scp = VTOSC(vp);
	/* assumes that vp and scp point at each other */
	lock_ObtainWrite(&cm_scachelock);
	tscp = (struct cm_scache **)vp->v_data;
	*tscp = (struct cm_scache *)NULL;
	scp->v.v_vnode = NULLVP;
	if (scp->v.v_count-- == 0) panic("cm_reclaim v_count");
	lock_ReleaseWrite(&cm_scachelock);
	return(0);
}

/* Function to get a corresponding public pool vnode from the system,
 * given the appropriate scache entry.  On private pool systems, there
 * is only one structure, and this function turns into a macro that
 * just casts the pointer to the appropriate type.
 *
 * Decrements the refCount on scp, and increment it on returned
 * vp.  Note that we also adjust cm refcount based on vp->v_data
 * so that if we add a new vnode reference, we bump the ref count,
 * and if we remove a reference, we decrement the refcount.
 *
 * If we fail, we don't decrement the refcount on scp, and we return
 * a null pointer from this function.
 *
 * Invariant: if scache points to a vnode, the vnode points back
 * to the same scache entry (if the scachelock is held).  The invariant
 * in the other direction also applies: if vnode points to scache entry,
 * the scache entry points back to the same vnode (under scachelock).
 */
struct vnode *
cm_get_vnode(struct cm_scache *scp)
{
    struct vnode *vp;
    struct cm_scache **cptr;

    /* fast path: see if back pointer can be validated */
    lock_ObtainWrite(&cm_scachelock);
    if (vp = scp->v.v_vnode) {
	VN_LOCK(vp);
	if (vget_nowait(vp) == 0) {
	    VN_UNLOCK(vp);
	    cptr = (struct cm_scache **)&vp->v_data;
	    /* back and forward pointer only change under scachelock */
	    osi_assert(*cptr == scp);
	    /* decrement as per cm_get_vnode's description */
	    if (scp->v.v_count-- == 0)
		panic("cm_get_vnode v_count");
	    lock_ReleaseWrite(&cm_scachelock);
	    return(vp);
	}
	VN_UNLOCK(vp);
    }
    lock_ReleaseWrite(&cm_scachelock);

    /* otherwise, we must get a new vnode and point it at our scache entry */
    for (;;) {
	/* used to just lock cm_scachelock, and VREF the pointer found in
	 * the scache entry.  But we could be in the middle of a reclaim,
	 * and VREF would bump the ref count of a doomed vnode.  We must
	 * use vget_nowait (normal vget could wait for VXLOCK (and thus
	 * for a reclaim to finish), which would violated locking hierarchy
	 * established by cm_reclaim (lock vxlock first, then cm_scachelock))
	 */

	/* wait for reclaim to finish (probably why we're here in 1st place)
	 * otherwise first vget_nowait below will fail, too.
	 */
	if (vp = scp->v.v_vnode) {
	    if (vget(vp) == 0)
		vrele(vp);
	}

	/* start by getting a vnode to install */
	if (getnewvnode(VT_AFS,(struct osi_vnodeops *)cm_GetAFSOps(), &vp)) {
	    return(NULLVP);
	}

	lock_ObtainWrite(&cm_scachelock);
	if (scp->v.v_vnode) { /* did someone already allocate it ? */
	    vp->v_type = VBAD;	/* throw away newly allocated vnode */
	    vfree(vp);
	    vp = scp->v.v_vnode;	/* use existing vnode */
	    VN_LOCK(vp);
	    if (vget_nowait(vp) == 0) {
		VN_UNLOCK(vp);
		/* only keep 1 ref for osf/1 vnode */
		if (scp->v.v_count-- == 0) panic("cm_get_vnode v_count2");
		lock_ReleaseWrite(&cm_scachelock);
		return(vp);
	    }
	    VN_UNLOCK(vp);
	    /* otherwise we lost the race; the vnode is probably vxlocked
	     * and we have to wait for it before we try again.  Probably
	     * most reasonable (in terms of OSF/1 dependencies reflected
	     * here) is to just do a vget w/o the cm_scachelock held.
	     */
	    lock_ReleaseWrite(&cm_scachelock);
	    icl_Trace1(cm_iclSetp, CM_TRACE_RECLAIMLOST, ICL_TYPE_POINTER, scp);
	    continue;
	}	/* if there's a back ptr from scache to vnode */

	/* otherwise, we found no back pointer, so we install our
	 * new vnode in here, and set its back pointer.
	 */
	osi_vSetType(vp, scp->v.v_type);
	scp->v.v_vnode = vp;
	cptr = (struct cm_scache **)vp->v_data;
	*cptr = scp;
	lock_ReleaseWrite(&cm_scachelock);
	osi_vSetVfsp(vp, cm_globalVFS);
	SET_CONVERTED(vp);
	return vp;
    } /* for (;;) */
    /* NOTREACHED */
}

#else
cm_reclaim(scp)
register struct cm_scache *scp;
{
	return(0);
}
#endif /* SCACHE_PUBLIC_POOL && AFS_OSF_ENV */


/*
 * Attempt to idle the scache pool by flushing all entries which do
 * not have a reference count.  If any entries have references or
 * are unable to be flushed, then EBUSY is returned.  The idle is
 * performed in a multi pass loop.  The first passes check for and flushe
 * all unreferenced entries which are not virtual dirs or mountpoints for
 * cell roots.  The last pass then frees the virtual dirs and cell
 * mountpoints if the earlier loops were able to free the other scache entries.
 * This is done to avoid removing an vdir or cell root entry which is
 * a parent of a referenced entry. An entry can be considered to have 
 * no references if the following conditions exist:
 *  1) it has a refcount of 0
 *  2) it is a virtual directory or the root of a cell fileset
 *     and has a refcount of 1.  Note that normal fileset mountpoints
 *     cannot have a refcount and be considered free.
 *  3) it is the root (VROOT) virtual directory for a cell and has
 *     a refcount of 2 or less.
 *
 * This function  should not be called with any scp locks or 
 * the cm_scachelock lock held.
 */
#ifdef AFS_SUNOS5_ENV
#define	AFS_ROOT_REFCOUNT		1
#else /* AFS_SUNOS5_ENV */
#define	AFS_ROOT_REFCOUNT		2
#endif /* AFS_SUNOS5_ENV */

int cm_IdleSCaches()
{
    register struct squeue *tq;
    struct squeue *uq;
    register struct cm_scache *scp;
    int rc = 0;
    int final_flush = 0;
    /* infinite loop protection: */
    int max_retry = 20000+20*(cm_cacheScaches+cm_scount);
    int retry;
#ifdef AFS_SUNOS5_ENV
    int okcount;
#endif /* AFS_SUNOS5_ENV */
    extern struct cm_scache *cm_rootVnode;

    lock_ObtainWrite(&cm_scachelock);
    for (;;) {
	retry = 0;
	for (tq = SLRU.prev; tq != &SLRU; tq = uq) {
	    scp = SC_QTOS(tq);
	    uq = QPrev(tq);

	    if (CM_RC(scp) && (cm_vType(scp) == VDIR)) {
		/* 
		 * Try to clean up references in the name cache.
		 * But must make sure the scp is not one of the
		 * special cases where recounts can exists when
		 * the scp is really free.
	 	 */
		if (!(((CM_RC(scp) == 1) && 
		    (scp->states & (SC_VDIR | SC_VOLROOT)) && 
		    (scp->parentVDIR)) ||
		    ((CM_RC(scp) == AFS_ROOT_REFCOUNT) && 
		    ((SCTOV(scp))->v_flag & VROOT)))) {
		    lock_ReleaseWrite(&cm_scachelock);
		    nh_delete_dvp(scp);
		    lock_ObtainWrite(&cm_scachelock);
		    if (--max_retry >= 0)
			retry = 1;
		    else
			rc = EBUSY;
		    break;	/* dropped lock, start over */
		}
	    }

#ifdef AFS_SUNOS5_ENV
	    if (!final_flush) {
		okcount = -1;
		if (CM_RC(scp) == 0) {
		    okcount = 0;
		} else if ((CM_RC(scp) == 1) && 
			   (scp->states & (SC_VDIR | SC_VOLROOT)) &&
			   (scp->parentVDIR)) {
		    okcount = 1;
		} else if (((SCTOV(scp))->v_flag & VROOT) && 
			   (CM_RC(scp) == AFS_ROOT_REFCOUNT)) {
		    okcount = AFS_ROOT_REFCOUNT;
		}
		/* The reference to scp->opens is just for use as a hint, so
		 * holding the wrong lock should be OK.
		 */
		if (okcount >= 0 && scp->opens != 0) {
		    CM_RAISERC(scp);
		    cm_CheckOpens(scp, okcount+1);
		    CM_LOWERRC(scp);
		}
	    }
#endif /* AFS_SUNOS5_ENV */
	    if (CM_RC(scp) == 0) {
		if (FlushSCache(scp, 0)) {
		    rc = EBUSY;
		    break;
		}
		/* Retry, since more ref counts might have gone to zero */
		if (--max_retry >= 0)
		    retry = 1;
		else
		    rc = EBUSY;
	    } else if ((CM_RC(scp) == 1) && 
		(scp->states & (SC_VDIR | SC_VOLROOT)) &&
		(scp->parentVDIR)) {
		if (final_flush) {
		    (void) FlushSCache(scp, 1);
		}
	    } else if (((SCTOV(scp))->v_flag & VROOT) && 
		(CM_RC(scp) == AFS_ROOT_REFCOUNT)) {
		if (final_flush) {
		    (void) FlushSCache(scp, 1);
		    cm_rootVnode = 0;
		    CM_SETRC(scp, 0);
		}
	    } else {
		rc = EBUSY;
		break;
	    }
	}	/* end for */

	if (rc || final_flush)
	    break; 	/* done */
	else if (retry == 0)
	    final_flush = 1;
    }	/* end for (;;) */
    lock_ReleaseWrite(&cm_scachelock);
    return rc;
}
