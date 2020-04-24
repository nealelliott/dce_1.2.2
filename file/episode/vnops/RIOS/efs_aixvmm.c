/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1994, 1990 Transarc Corporation
   All rights reserved.
 */

/*
 * Functions to integrate Episode with the AIX VMM
 */

#include <dcedfs/param.h>

#include <dcedfs/sysincludes.h>
#include <dcedfs/osi_uio.h>
#include <dcedfs/osi_user.h>
#include <dcedfs/osi_buf.h>
#include <dcedfs/osi_intr.h>
#include <sys/vmuser.h>
#include <sys/m_except.h>
#include <dcedfs/stds.h>
#include <dcedfs/xvfs_vnode.h>
#include <dcedfs/aggr.h>
#include <dcedfs/volume.h>
#include <dcedfs/debug.h>
#include <dcedfs/queue.h>
#include <dcedfs/lock.h>

#include <dcedfs/episode/file.h>

#include <efs_evnode.h>
#include <efs_misc.h>			/* vnm_GetFileH */
#include <efs_vnodeops.h>		/* efs_getlength, etc */
#include <efs_volvnops.h>

#include <sys/sleep.h>

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RIOS/RCS/efs_aixvmm.c,v 1.121 1995/04/20 14:03:29 ota Exp $")

/*
 * Event definition for using the e_post() and e_wait() services.
 */
#define EVENT_EFSPAGE 0x10000000

/*
 * Definitions for pager status word.
 */
#define EFS_PAGER_WAITING	0x1
    
/*
 * Defines used for grouping buffers together which are contiguous
 * in memory.
 */
#define MAXMERGE (0x20000)	/* 128K */

#define EFS_COMPARABLE(x,y) \
	(((x)->b_vp == (y)->b_vp) \
	 && ((x)->b_flags == (y)->b_flags) \
	 && ((x)->b_xmemd.subspace_id == (y)->b_xmemd.subspace_id) \
	 && !((x)->b_flags & B_PFPROT) \
	 && !(((x)->b_flags & B_READ) && ((x)->b_flags & B_PFSTORE))) \

/*
 * The following locks protect the 4 lists that the process level
 * daemons are using.
 */

osi_dlock_t  efs_pageInQLock, efs_pageUnprotectQLock, efs_pageOutQLock;

#if !LATER
static osi_dlock_t blockedBufLock;
static struct buf *blockedBufs = NULL;
#endif

struct buf *efs_pageInQ, *efs_pageUnprotectQ, *efs_pageOutQ;

struct buf *efs_pageInSQ, *efs_pageInSLast, *efs_pageUnprotectSQ, 
	   *efs_pageUnprotectSLast, *efs_pageOutSQ, *efs_pageOutSLast;

static int efs_pageInRunning= 0, efs_pageOutRunning = 0, 
    efs_pageUnprotectRunning = 0;	

static pid_t efs_pageInID, efs_pageOutID, efs_pageUnprotectID;
static int efs_pageInStatus, efs_pageOutStatus, efs_pageUnprotectStatus;

/*
 * Buffers are given to us in lists connected by the av_forw field 
 * passed to our efs_vmstrategy() routine. We then need to break
 * up these lists into groups of buffs that represent data physically
 * contiguous on disk and call epia_Strategy for each of those lists.
 * In order to avoid getting locks, looking at anode's etc. at interupt
 * time we make efs_vmstrategy() simply add the buffs that it gets to 
 * a list (connected via the buff's b_work field) that is then processed 
 * by separate daemons that run at process level.
 *
 */

/*
 * Signal a pager to process pages.
 * This is called at INTPAGER.
 */
static void
efs_signalPager(pid_t pagerId, int *pagerStatus)
{
    if (*pagerStatus & EFS_PAGER_WAITING)
	e_post(EVENT_EFSPAGE, pagerId);
}

/*
 * Wait for page fault requests to come down from VMM.
 * Should be called at INTPAGER or higher.
 */ 
static void
efs_waitForPages(int *pagerStatus)
{
    long rc;

    *pagerStatus |= EFS_PAGER_WAITING;
    do {
	rc = e_wait(EVENT_EFSPAGE, EVENT_EFSPAGE, EVENT_SIGRET);
    } while ((rc != EVENT_EFSPAGE) && (rc != EVENT_SIG));
    *pagerStatus &= ~EFS_PAGER_WAITING;
}

/*
 * Episode VM strategy function
 * Put buffer abp at the end of the appropriate strategy list.
 *
 * This function is called at interrupt level and so must not cause a page
 * fault.
 */
efs_vmstrategy (abp)
    register struct buf *abp;
{
    struct buf **strQ, **strLast, **laststrQ, *nbp;
    pid_t pagerID, lastPagerID;
    int *pagerStatus, *lastPagerStatus;

    strQ = NULL;
    pagerStatus = NULL;
    pagerID = 0;

    while (abp) {
	laststrQ = strQ;
	lastPagerID = pagerID;
	lastPagerStatus = pagerStatus;

	/*
	 * Verify that fields we are going to overload are not
	 * set by the VMM.
	 */
	afsl_AssertTruth(abp->av_back == NULL);
	afsl_AssertTruth(abp->b_start.tv_sec == 0);

	if ((abp->b_flags & (B_PFSTORE|B_PFPROT)) == (B_PFSTORE|B_PFPROT)) {
	    /*
	     * This is a protection fault of some type which may require
	     * require allocating disk space to back the page.
	     */
	    strQ = &efs_pageUnprotectSQ;
	    strLast = &efs_pageUnprotectSLast;
	    pagerStatus = &efs_pageUnprotectStatus;
	    pagerID = efs_pageUnprotectID;
	} else if (abp->b_flags & B_READ) {
	    /*
	     * A read or read for store request.
	     */
	    strQ = &efs_pageInSQ;
	    strLast = &efs_pageInSLast;
	    pagerStatus = &efs_pageInStatus;
	    pagerID = efs_pageInID;
	} else {
	    /*
	     * Pageout.
	     */
	    strQ = &efs_pageOutSQ;
	    strLast = &efs_pageOutSLast;
	    pagerStatus = &efs_pageOutStatus;
	    pagerID = efs_pageOutID;
	}
	/* 
	 * If switching to a new queue, wakeup up the handler for the
	 * last queue.
	 */
	if (laststrQ && (laststrQ != strQ))
	    efs_signalPager(lastPagerID, lastPagerStatus);

	nbp = abp->av_forw;
	abp->b_work = 0;
	abp->av_forw = NULL;

	if (*strQ != NULL) {
	    osi_assert((*strLast) != NULL && (*strLast)->b_work == 0);
	    (*strLast)->b_work = (int) abp;
	    *strLast = abp;
	} else {
	    *strQ = *strLast = abp;
	}
	abp = nbp;	/* go to the next buffer */
    } 			/* end while */
    efs_signalPager(pagerID, pagerStatus);
    return 0;
}

/*
 * move any busy buffers back to the main queue, and wake up the
 * background daemon.
 */
void efs_WakeupBlockedPageIns(struct volume *volp)
{
    struct buf *tbp, *nbp, *rbufs = NULL, *ubufs = NULL;
    struct buf *abp;

#ifdef LATER
    /* XXX -- preemption? */
    lock_ObtainWrite(&volp->v_lock);
    if (VTOEVLD(volp) == NULL) {
	lock_ReleaseWrite(&volp->v_lock);
	return;
    }
    tbp = VTOEVLD(volp)->sideRail;
    VTOEVLD(volp)->sideRail = NULL;
    lock_ReleaseWrite(&volp->v_lock);
#else /*LATER*/
    lock_ObtainWrite_r(&blockedBufLock);
    tbp = blockedBufs;
    blockedBufs = NULL;
    lock_ReleaseWrite_r(&blockedBufLock);
#endif /*LATER*/
    for (abp = tbp; abp != NULL; abp = nbp) {
	nbp = abp->av_forw;
	afsl_Assert (abp->b_flags & B_READ);
	if ((abp->b_flags & (B_PFSTORE | B_PFPROT)) == (B_PFSTORE | B_PFPROT)){
	    abp->av_forw = ubufs;
	    ubufs = abp;
	} else {
	    abp->av_forw = rbufs;
	    rbufs = abp;
	}
    }
    /* 
     * at this point all the buffers that where on the busylist are threaded
     * to one of the 2 lists. Add the 2 lists on their request queues and
     * wakeup the appriate daemon(s)
     */
    if (rbufs != NULL) {
	int opri;

	lock_ObtainWrite_r(&efs_pageInQLock);
	if (efs_pageInQ == NULL){
	    efs_pageInQ = rbufs;
	    opri = i_disable(INTPAGER);
	    efs_signalPager(efs_pageInID, &efs_pageInStatus);
	    i_enable(opri);
	} else {	
	    for (tbp = efs_pageInQ; tbp->av_forw != NULL; tbp = tbp->av_forw);
	    afsl_Assert(tbp != NULL);
	    tbp->av_forw = rbufs;
	}
	lock_ReleaseWrite_r(&efs_pageInQLock);
    }
    if (ubufs != NULL) {
	int opri;

	lock_ObtainWrite_r(&efs_pageUnprotectQLock);
	if (efs_pageUnprotectQ == NULL) {
	    efs_pageUnprotectQ = rbufs;
	    opri = i_disable(INTPAGER);
	    efs_signalPager(efs_pageUnprotectID, &efs_pageUnprotectStatus);
	    i_enable(opri);
	} else {	
	    for (tbp = efs_pageUnprotectQ; 
		 tbp->av_forw != NULL; tbp = tbp->av_forw);
	    afsl_Assert(tbp != NULL);
	    tbp->av_forw = ubufs;
	}
	lock_ReleaseWrite_r(&efs_pageUnprotectQLock);
    }
    return;
}

/*
 * Advisory age value to protect against starving old requests.
 * The counter is maintained w/o serialization.
 */
long bufAge=0;

/*
 * The request list's structure is as follows : buffers that 
 * refer to the same vnode and consecutive logical offsets are
 * linked together via the av_forw field. All others are connected
 * via the b_work field.  The av_back field is overloaded to hold
 * to total byte count of all linked bufs.  The bstart.tv_sec
 * field is used to store the age value.
 */
void efs_MergeToRequestList(struct buf *bP, 
			    struct buf **rList, osi_dlock_t *lock)
{
    struct buf *tbP, *nextbP, *prevbP, *lbP, *abovebP;
    int checkMerge;

    lock_ObtainWrite_r(lock);
    while (bP != NULL) {
	afsl_AssertTruth(bP->av_forw == NULL);
	nextbP = (struct buf *)bP->b_work;
	bP->b_work = 0;
	bP->b_start.tv_sec = bufAge++;
	bP->av_back = (struct buf *)bP->b_bcount;

	tbP = *rList;
	abovebP = NULL;
        checkMerge = 1;
	while (tbP != NULL) {
	    if (checkMerge && EFS_COMPARABLE(tbP, bP)) {
		unsigned long off0, off1, end0, end1;

                off0 = dbtob(bP->b_blkno);
                end0 = off0 + bP->b_bcount;
                off1 = dbtob(tbP->b_blkno);
                end1 = off1 + (long)tbP->av_back;

		if (end0 < off1) {
		    checkMerge = 0;
		    bP->b_work = (int)tbP;
		    if (abovebP != NULL) {
			abovebP->b_work = (int)bP;
		    } else {
			afsl_Assert(tbP == *rList);
			*rList = bP;
		    }
		} else if (end0 == off1) {
		    /* We can put it at the front of this list */
		    checkMerge = 0;
		    bP->av_back = (struct buf *)
			(bP->b_bcount + (long)tbP->av_back);
		    bP->b_start.tv_sec = 
			MIN(bP->b_start.tv_sec, tbP->b_start.tv_sec);
		    bP->av_forw = tbP;
		    bP->b_work = tbP->b_work;
		    tbP->b_work = tbP->b_start.tv_sec = 0;
		    tbP->av_back = NULL;

		    if (abovebP != NULL) {
			abovebP->b_work = (int)bP;
		    } else {
			afsl_Assert(tbP == *rList);
			*rList = bP;
		    }
		} else if (off0 == end1) {
		    /* We can put it at the back of this list */
		    checkMerge = 0;
		    prevbP = tbP;
		    for (lbP = tbP->av_forw; lbP != NULL; lbP = lbP->av_forw) {
			afsl_Assert(lbP->b_blkno > prevbP->b_blkno);
			prevbP = lbP;
		    }
		    prevbP->av_forw = bP;
		    bP->av_forw = NULL;
		    tbP->b_start.tv_sec = 
			MIN(tbP->b_start.tv_sec, bP->b_start.tv_sec);
		    tbP->av_back = (struct buf *)
			(bP->b_bcount + (long)tbP->av_back);
		    bP->b_start.tv_sec = 0;
		    bP->av_back = NULL;
		} else if (off0 > end1) {
		    struct buf *nbufp = (struct buf *)tbP->b_work;
		    /* 
		     * See if the next group is before this buffer or
		     * if we should keep going.
		     */
		    if ((nbufp == NULL) || !EFS_COMPARABLE(bP, nbufp) ||
			(end0 < dbtob(nbufp->b_blkno))) { 
			checkMerge = 0;
			bP->b_work = (int)tbP->b_work;
			tbP->b_work = (int)bP;
		    }
	 	}
	    }	/* end of if COMPARABLE */

	    /* 
	     * Can't be put in this chain try the next one.
	     */
	    abovebP = tbP;
	    tbP = (struct buf *) tbP->b_work;

	    /*
	     * Check for very old buffers that have probably been
	     * starved out by sorting.  If we find one, move it
	     * to the front of the list.  We check here after tbP is
	     * bumped to avoid checking the first tbP which is rList and
	     * to make the check before this next tbP gets a chance for
	     * the compatability checks.
	     */
	    if ((tbP) && ((bufAge - tbP->b_start.tv_sec) > 500)) {
		struct buf *nbufp;

		afsl_AssertTruth(tbP != *rList);
		afsl_AssertTruth(tbP->b_work != (int)bP);
		nbufp = (struct buf *)tbP->b_work;
		if (abovebP != NULL) abovebP->b_work = (int)nbufp;
		tbP->b_work = (int)*rList;
		*rList = tbP;
		tbP = nbufp;
		if (tbP == NULL)
		    break;	/* At end of list now */
	    }
	}	/* end while(tbP) */

	if (tbP == NULL && checkMerge) {
	    /* we weren't able to put bP into any chain start a new one */
	    if (abovebP != NULL)
		abovebP->b_work = (int)bP;
	    else {
		afsl_Assert(*rList == NULL);
		*rList = bP;
	    }
	}
	bP = nextbP;
    }
    lock_ReleaseWrite_r(lock);
}

#define efs_SortRequestList(list, lock) /* do nothing for now */

struct buf *efs_GetNextBuffer(struct buf **sList, 
			      int *sListStatus,
			      struct buf **rList,
			      osi_dlock_t *rLock)
{
    struct buf *gotFromStrategy;
    struct buf *ret;
    int oldPriority;
    int didSleep;

    while (1) {
	/*
	 * Disable interrupts from the strategy function, and save the 
	 * prior priority level. We use this instead of a lock for the
	 * efs_strategyList
	 */
	didSleep = 0;
	lock_ObtainRead_r(rLock);
	oldPriority = i_disable(INTPAGER);
	/* get the entire current list */
	gotFromStrategy = *sList;
	*sList = NULL;

	if ((gotFromStrategy == NULL) && (*rList == NULL)) {
	    lock_ReleaseRead_r(rLock);
	    efs_waitForPages(sListStatus);
	    didSleep = 1;
	}
	i_enable (oldPriority);
	if (didSleep == 0) lock_ReleaseRead_r(rLock);

	if (gotFromStrategy != NULL) {
	    efs_MergeToRequestList(gotFromStrategy, rList, rLock);
	    efs_SortRequestList(rList, rLock);
	}
	/* 
	 * Make a quick test to see if there may be more
	 * work on the strategy Q.  If so, collect some more
	 * buffers in hopes of batching them together.  Note
	 * the check is made without disabling interrupts, but
	 * thats ok, the outcome is harmless.
	 */
	if (*sList) continue;

	lock_ObtainWrite_r(rLock);
	ret = *rList;
	if (ret != NULL) {
	    *rList = (struct buf *) ret->b_work;
	    ret->b_work = 0;
	    ret->av_back = NULL;	/* reset to initial value */
	    ret->b_start.tv_sec = 0;	/* reset to initial value */
	}
	lock_ReleaseWrite_r(rLock);
	if (ret != NULL) return(ret);
    }
}

int efs_SkipThisBuf(
  struct buf *bp)
{
    struct vnode *vp = ((struct gnode *) bp->b_vp)->gn_vnode;
    struct evnode *evp = VTOEV(vp);
    struct volume *volp;
    int code;
    extern void efs_FinishVMIO();

    /* do volume processing, so that we don't handle a page fault for
     * a volume that is offline.  Can't check before now, because
     * we're at interrupt level until now.  Basic idea is that we
     * allow writes (but not reads) once the volume goes busy, so that
     * we can finish cleaning the VM pages (writes), but can't create new
     * dirty pages.  Once we're back from the VOL_OPEN call, we want to
     * block all new operations, although there really shouldn't be
     * any other operations happening anyway, since there are no pages
     * left to dirty.
     */
    if (!(bp->b_flags & (B_READ|B_PFPROT|B_PFSTORE)))
	return (0);

    BEGIN_LCRIT (struct volume *volp,
		 code = efs_GetVolume(vp, &volp),
		 VOL_RELE(volp)) {
	BEGIN_LOCK_WRITE (&volp->v_lock) {
	    if (VTOEVLD(volp) == NULL) {
		code = ESTALE;
	    } else {
		int openbits = VTOEVLD(volp)->vld_openbits;
		if ((openbits & STOPUSE_NO_VM) ||
		    ((openbits & STOPUSE_NO_DIRTYVM) &&
		     ((bp->b_flags & B_PFSTORE) &&
		      (bp->b_flags & B_PFPROT)))) {
#ifdef LATER
		    bp->av_forw = VTOEVLD(volp)->sideRail;
		    VTOEVLD(volp)->sideRail =  bp;
#else /* LATER */
		    lock_ObtainWrite_r(&blockedBufLock);
		    bp->av_forw = blockedBufs;
		    blockedBufs = bp;
		    lock_ReleaseWrite_r(&blockedBufLock);
#endif /* LATER */
		}
	    }
	} END_LOCK;
    } END_CRIT;
    if (code) {
	efs_FinishVMIO(bp, code);
	return 1;
    } else {
	return 0;
    }
}


/*
 * Path to process reads and read for store requests.
 */
efs_PageInDaemon ()
{
    struct buf *bp;
    caddr_t tmpaddr;
    struct vnode *vp;
    long code;

    if (!efs_pageInRunning) {
	efs_pageInRunning = 1;
	/* Initialize lists and locks */
	lock_Init (&efs_pageInQLock);
	pin(&efs_pageInQ, sizeof(efs_pageInSQ));
	pin(&efs_pageInSLast, sizeof(efs_pageInSLast));
	pin(&efs_pageInQLock, sizeof(efs_pageInQLock));
	pin(&efs_pageInID, sizeof(efs_pageInID));
	pin(&efs_pageInStatus, sizeof(efs_pageInStatus));
	efs_pageInQ = NULL;
	efs_pageInStatus = 0;
	efs_pageInID = osi_GetPid();
    } else
	return(EEXIST);

    while (1) {
	bp = efs_GetNextBuffer(&efs_pageInSQ, &efs_pageInStatus,
				&efs_pageInQ, &efs_pageInQLock);
	afsl_Assert(bp != NULL);

	icl_Trace4 (efs_iclSet, EFS_TRACE_PAGEIN_REQ_START,
		    ICL_TYPE_POINTER, bp,
		    ICL_TYPE_LONG, bp->b_blkno,
		    ICL_TYPE_LONG, bp->b_bcount,
		    ICL_TYPE_LONG, bp->b_flags);
	
	if (efs_SkipThisBuf(bp))
	    continue;

	/* 
	 * when we get here, if the vnode has an identity, the corresponding 
	 * volume handle is held and has activeVnode count bumped.  If 
	 * code == 0, it is set.
	 */

	icl_Trace2 (efs_iclSet, EFS_TRACE_PAGEIN_REQ_STRATEGY,
		    ICL_TYPE_POINTER, bp,
		    ICL_TYPE_POINTER, bp->b_vp);

	(void)efs_pagein(bp);
	
	/* Trace end for the record, but it isn't safe to reference through bp
         * anymore. */

	icl_Trace1 (efs_iclSet, EFS_TRACE_PAGEIN_REQ_END,
		    ICL_TYPE_POINTER, bp);
    } 
} 

/*
 *
 */

efs_PageUnprotectDaemon ()
{
    struct buf *bp;

    if (!efs_pageUnprotectRunning) {
	efs_pageUnprotectRunning = 1;
	/* Initialize lists and locks */
	lock_Init (&efs_pageUnprotectQLock);
	pin(&efs_pageUnprotectSQ, sizeof(efs_pageUnprotectSQ));
	pin(&efs_pageUnprotectSLast, sizeof(efs_pageUnprotectSLast));
	pin(&efs_pageUnprotectQLock, sizeof(efs_pageUnprotectQLock));
	pin(&efs_pageUnprotectID, sizeof(efs_pageUnprotectID));
	pin(&efs_pageUnprotectStatus, sizeof(efs_pageUnprotectStatus));
	efs_pageUnprotectQ = NULL;
	efs_pageUnprotectStatus = 0;
	efs_pageUnprotectID = osi_GetPid();
    } else
	return(EEXIST);

    while (1) {
	bp = efs_GetNextBuffer(&efs_pageUnprotectSQ, &efs_pageUnprotectStatus,
			&efs_pageUnprotectQ, &efs_pageUnprotectQLock);

	afsl_Assert(bp != NULL);

	icl_Trace4 (efs_iclSet, EFS_TRACE_PAGEUNPROTECT_REQ_START,
		    ICL_TYPE_POINTER, bp,
		    ICL_TYPE_LONG, bp->b_blkno,
		    ICL_TYPE_LONG, bp->b_bcount,
		    ICL_TYPE_LONG, bp->b_flags);
	
	if (efs_SkipThisBuf(bp))
	    continue;

	icl_Trace2 (efs_iclSet, EFS_TRACE_PAGEUNPROTECT_REQ_STRATEGY,
		    ICL_TYPE_POINTER, bp,
		    ICL_TYPE_POINTER, bp->b_vp);

	efs_pageunprotect(bp);
	
	/* Trace end for the record, but it isn't safe to reference through bp
         * anymore.
	 */
	icl_Trace1 (efs_iclSet, EFS_TRACE_PAGEUNPROTECT_REQ_END,
		    ICL_TYPE_POINTER, bp);
    } 
} 

/*
 * Path which handles page outs (ie. writes to disk).
 */
efs_PageOutDaemon ()
{
    struct buf *bp;
    long code;

    if (!efs_pageOutRunning) {
	efs_pageOutRunning = 1;
	/* Initialize lists and locks */
	lock_Init (&efs_pageOutQLock);
	pin(&efs_pageOutSQ, sizeof(efs_pageOutSQ));
	pin(&efs_pageOutSLast, sizeof(efs_pageOutSLast));
	pin(&efs_pageOutQLock, sizeof(efs_pageOutQLock));
	pin(&efs_pageOutID, sizeof(efs_pageOutID));
	pin(&efs_pageOutStatus, sizeof(efs_pageOutStatus));
	efs_pageOutQ = NULL;
	efs_pageOutStatus = 0;
	efs_pageOutID = osi_GetPid();
    } else
	return(EEXIST);

    while (1) {
	bp = efs_GetNextBuffer(&efs_pageOutSQ, &efs_pageOutStatus, 
				&efs_pageOutQ, &efs_pageOutQLock);
	afsl_Assert(bp != NULL);

	icl_Trace4 (efs_iclSet, EFS_TRACE_PAGEOUT_REQ_START,
		    ICL_TYPE_POINTER, bp,
		    ICL_TYPE_LONG, bp->b_blkno,
		    ICL_TYPE_LONG, bp->b_bcount,
		    ICL_TYPE_LONG, bp->b_flags);
	
	if (efs_SkipThisBuf(bp))
	    continue;

	icl_Trace2 (efs_iclSet, EFS_TRACE_PAGEOUT_REQ_STRATEGY,
		    ICL_TYPE_POINTER, bp,
		    ICL_TYPE_POINTER, bp->b_vp);

	efs_pageout(bp);
	
	/* Trace end for the record, but it isn't safe to reference through bp
         * anymore. */

	icl_Trace1 (efs_iclSet, EFS_TRACE_PAGEOUT_REQ_END,
		    ICL_TYPE_POINTER, bp);
    } 
} 

/*
 * efs_getvmh -- create AIX 3.1 VM segment if necessary
 */
int efs_getvmh (vp)
  struct vnode *vp;
{
    long code = 0;
    efs_vnode_t evp = VTOEV(vp);

    BEGIN_LOCK_WRITE (&evp->vd_vm.lock) {
	afsl_Assert (!evp->vd_vm.seg == !vp->v_gnode->gn_seg);
	if (!vp->v_gnode->gn_seg) {
	    long len;
	    efs_getlength (vp, &len, NULL);
	    code = vms_create (&vp->v_gnode->gn_seg, V_CLIENT, vp->v_gnode,
				   len, 0, 0);
	    if (code)
		EXIT_CRIT;
	    evp->vd_vm.seg = 1;
	}
    } END_LOCK;
    return code;
}
