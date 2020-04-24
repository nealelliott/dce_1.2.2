/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: lock.c,v $
 * Revision 1.1.44.1  1996/10/02  17:55:35  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:42:39  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1990, 1996 Transarc Corporation - All rights reserved */
/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987, 1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */

#include <sys/types.h>
#include <sys/file.h>
#include <sys/errno.h>
#include <pthread.h>
#include <dcedfs/icl.h>
#include <dcedfs/compat.h>
#define UBIK_INTERNALS 1
#include <ubik.h>
#include <utrace.h>
#include <ubik_data.h>

extern struct icl_set *ubikdisk_iclSetp;
/*
 * Locking routines for mutual exclusion. The version lock is held to prevent
 * the database from changing while read/write transactions are in progrss.
 * The low level lock is a simple mutex for cma. The locking order is, always
 * grab the version lock without holding any other locks.
 */

static osi_dlock_t ubik_llock;	/* the base definition */

/* SHARED  */
void ubik_initLocks(dbase)
IN struct ubik_dbase *dbase;
{
  lock_Init(&((dbase)->versionLock)); 
  lock_Init(&ubik_llock);
}

/*
 * ubik_hold
 *	A simple routine to hold the low level lock. This is held when handling
 *	any shared data structures in ubik, but is released when making an RPC
 *	or when sleeping. Never hold this when obtaining the version lock.
 */
/* SHARED  */
void ubik_hold()
{
/*  icl_Trace0(ubikdisk_iclSetp, UK_TRACE_LOCK_HOLD); */
  lock_ObtainWrite(&ubik_llock); 
/*  icl_Trace0(ubikdisk_iclSetp, UK_TRACE_LOCK_HOLD_EXIT);*/
}

/*
 * ubik_hold_no_block
 *	A simple try lock mechanism for grabbing the low level lock. This can
 *   	be used by routines that don't want to eat up a server thread waiting 
 *	for the lock. This can prevent a number of udebug requests from 
 *	starving ubik out of threads.
 */
SHARED int ubik_hold_no_block()
{
  return (lock_ObtainWriteNoBlock(&ubik_llock));
}


/*
 * ubik_rele
 *	A simple routine to release the ubik low level lock.
 */
/* SHARED  */
void ubik_rele()
{
/*  icl_Trace0(ubikdisk_iclSetp, UK_TRACE_LOCK_RELE); */
  lock_ReleaseWrite(&ubik_llock);
/*  icl_Trace0(ubikdisk_iclSetp, UK_TRACE_LOCK_RELE_EXIT); */
}

/*
 * ubik_versionLock
 *	Grab the version lock after releasing the low level lock, and then
 *	reobtain the low level lock.
 *	This routine called with low level lock held.
 */
/* SHARED  */
void ubik_versionLock(dbase)
IN  struct ubik_dbase *dbase;
{
  icl_Trace0(ubikdisk_iclSetp, UK_TRACE_LOCK_VLHOLD);
  ubik_rele();
  ubik_dprint("ubik_versionLock\n"); 
  lock_ObtainWrite(&((dbase)->versionLock));
  ubik_hold();
  icl_Trace0(ubikdisk_iclSetp, UK_TRACE_LOCK_VLHOLD_EXIT);
}

/* Get both locks, assuming neither is held. */
/* SHARED */
void ubik_versionLockBoth(dbase)
IN  struct ubik_dbase *dbase;
{
  icl_Trace0(ubikdisk_iclSetp, UK_TRACE_LOCK_VLHOLD);
  ubik_dprint("ubik_versionLockBoth\n"); 
  lock_ObtainWrite(&((dbase)->versionLock));
  ubik_hold();
  icl_Trace0(ubikdisk_iclSetp, UK_TRACE_LOCK_VLHOLD_EXIT);
}

/*
 * ubik_versionUnlock
 *	A simple routine to release the version lock.
 */
/* SHARED  */
void ubik_versionUnlock(dbase)
IN  struct ubik_dbase *dbase;
{
  icl_Trace0(ubikdisk_iclSetp, UK_TRACE_LOCK_VLRELE);
  lock_ReleaseWrite(&((dbase)->versionLock));
  ubik_dprint("ubik_versionUnlock\n");
  icl_Trace0(ubikdisk_iclSetp, UK_TRACE_LOCK_VLRELE_EXIT);
}

/* called with both llock and dbase version lock held.  Must
 * release both.
 */
/* SHARED  */
void ubik_sleep(dbase, addr)
     IN  struct ubik_dbase *dbase;
     IN  char *addr;
{
    ubik_versionUnlock(dbase);
    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_LOCK_SLEEP,
	       ICL_TYPE_LONG, dbase->flags);
    osi_SleepW(addr, &ubik_llock);
    ubik_hold();
}

/*
 * called with ubik_llock held. Routine release ubik_llock on return.
 * timeout, if set indicates that the sleep timed out instead of woken up.
 */
SHARED void ubik_timedSleep(addr, absWaitTime, timeout)
     IN long addr;
     IN struct timespec *absWaitTime;
     OUT int *timeout;
{
  osi_TimedSleepW((opaque)addr, (osi_dlock_t *)&ubik_llock, 
		  absWaitTime, timeout);
}

/* Locks hang off of each transaction, with all the transaction hanging off of
 * the appropriate dbase.  This package expects to be used in a two-phase locking
 * protocol, so it doesn't provide a way to release anything but all of the locks in the
 * transaction.
 *
 * At present, it doesn't support the setting of more than one byte-position lock at a time, that is
 * the length field must be 1.  This doesn't mean that a single transaction can't set more than
 * one lock, however.
 *
 * It is the responsibility of the user to avoid deadlock by setting locks in a partial order.
 */

/* debugging hooks */
ulock_Debug(aparm)
    ubik_debug *aparm; {
    struct ubik_lock *tl;
    struct ubik_trans *tt;
    aparm->anyReadLocks = 0;
    aparm->anyWriteLocks = 0;

    for(tt=ubik_dbase->activeTrans; tt; tt=tt->next) {
	for(tl=tt->activeLocks; tl; tl=tl->next) {
	    if (tl) {
		if (tl->type == LOCKREAD) aparm->anyReadLocks = 1;
		else if (tl->type == LOCKWRITE) aparm->anyWriteLocks = 1;
	    }
	}
    }
}

/* set a transaction lock.  Atype is LOCKREAD or LOCKWRITE, await is true if you
    want to wait for the lock instead of returning EWOULDLBOCK.  alen must always
    be 1 for now.
*/
ulock_SetLock(atrans, apos, alen, atype, await)
    struct ubik_trans *atrans;
    long apos, alen;
    int atype, await; {
    struct ubik_lock *tl;
    struct ubik_trans *tt;
    struct ubik_dbase *dbase;
    int lockOK;

    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_LOCK_SETLOCK, ICL_TYPE_POINTER, 
	       atrans);
    icl_Trace4(ubikdisk_iclSetp, UK_TRACE_LOCK_SETLOCK_PARAMS, ICL_TYPE_LONG, 
	       apos, ICL_TYPE_LONG, alen, ICL_TYPE_LONG, atype, ICL_TYPE_LONG, 
	       await);
    if (atrans->flags & TRDONE) return UDONE;
    if (alen != 1) return EINVAL;
    dbase = atrans->dbase;
retry:
    lockOK = 1;
    for(tt=dbase->activeTrans; tt; tt=tt->next) {
	for(tl=tt->activeLocks; tl; tl=tl->next) {
	    if (tl->start == apos) {
		if (atype == LOCKREAD) {
		    if (tl->type == LOCKWRITE) lockOK=0;
		}
		else if (atype == LOCKWRITE) {
		    /* write lock */
		    lockOK=0;
		}
		else return EINVAL;
		if (!lockOK) {
		    if (await==0) return EWOULDBLOCK;
		    /* otherwise wait for lock release */
		    tl->flags |= LWANT;
		    ubik_versionUnlock(dbase);
		    osi_SleepW((opaque) &tl->flags, &ubik_llock);
		    ubik_versionLockBoth(dbase);
		    goto retry;
		}
	    }
	}
    }
    /* if we get here, create new lock record and add to spec'd transaction */
    tl = (struct ubik_lock *) malloc(sizeof(struct ubik_lock));
    tl->next = atrans->activeLocks;
    atrans->activeLocks = tl;
    tl->type = atype;
    tl->start = apos;
    tl->length = 1;
    tl->flags = 0;
    return 0;
}

/* release all transaction locks */
ulock_ReleaseAllLocks(atrans)
    struct ubik_trans *atrans; {
    struct ubik_lock *tl, *nl;
    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_LOCK_RELEASE, ICL_TYPE_POINTER,
	       atrans);
    for(tl = atrans->activeLocks; tl; tl=nl) {
	nl = tl->next;
	if (tl->flags & LWANT)
	    osi_Wakeup((opaque) &tl->flags);
	free(tl);
    }
    atrans->activeLocks = (struct ubik_lock *) 0;
    return 0;
}
