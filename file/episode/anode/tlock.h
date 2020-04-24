/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/anode/RCS/tlock.h,v 4.34 1995/04/19 20:33:02 ota Exp $ */

/* Copyright (C) 1995, 1990 Transarc Corporation - All rights reserved */

#ifndef TRANSARC_EPI_TLOCK_H
#define	TRANSARC_EPI_TLOCK_H    1

#include <dcedfs/lock.h>
#include <dcedfs/episode/logbuf.h>

/*
 * Transaction lock, or tlock.  If two uncommitted transactions both use
 * the same lock, they must be put in the same equivalence class.  To minimize
 * merging of transactions into large equivalence classes, try to call
 * EquivTran when the lock is released, in preference to when it is obtained.
 */
struct tlock_data {
    osi_dlock_t lock;			/* lock upon which tlock is based */
    elbb_tranRec_t trans;		/* trans ID of last lock holder */
};

struct tmutex_data {
    epi_mutex_t lock;			/* lock upon which tlock is based */
    elbb_tranRec_t trans;		/* trans ID of last lock holder */
};

typedef struct tlock_data tlock_data_t;

#define tlock_GetTrans(tlock) ((tlock)->trans)
#define tlock_InitTrans(tlock, tran) ((tlock)->trans = (tran))

#define TMUTEX_REALLY	0x20		/* really a tmutex */
#define TLOCK_BEFORE	0x10		/* operate on lock first */
#define TLOCK_ASSIGN	0x8		/* update tlock tran id */
#define TLOCK_OPERATION	0x7		/* low bits encode lock operation */

#define TLOCK_EQUIV		(			        0)
#define TLOCK_EQUIV_ASSIGN	(		TLOCK_ASSIGN  | 0)

#define TLOCK_OBTAIN_READ       (TLOCK_BEFORE		      | 1)
#define TLOCK_RELEASE_READ	(			        2)
#define TLOCK_OBTAIN_WRITE	(TLOCK_BEFORE | TLOCK_ASSIGN  | 3)
#define TLOCK_RELEASE_WRITE	(	        TLOCK_ASSIGN  | 4)

#define TMUTEX_OBTAIN		(TLOCK_OBTAIN_WRITE |TMUTEX_REALLY)
#define TMUTEX_RELEASE		(TLOCK_RELEASE_WRITE|TMUTEX_REALLY)
#define TMUTEX_EQUIV		(TLOCK_EQUIV|TMUTEX_REALLY)
#define TMUTEX_EQUIV_ASSIGN	(TLOCK_EQUIV_ASSIGN|TMUTEX_REALLY)

/* Naming: Since read locks typically will not need to specify a transaction,
 * that is the default.  Specify a "T" suffix if a transaction is being
 * specified.  For write locks a transaction is usually required to that is the
 * default.  Specify a "NT" suffix if no transaction is used. */

#define tlock_ObtainReadT(trans, tlock) \
    tlock_Equiv (TLOCK_OBTAIN_READ, tlock, trans)

#define tlock_ObtainRead(tlock) \
    lock_ObtainRead_r(&(tlock)->lock)

#define tlock_ObtainReadNoBlock(tlock) \
    lock_ObtainReadNoBlock(&(tlock)->lock)

#define tlock_ObtainWrite(trans, tlock) \
    tlock_Equiv (TLOCK_OBTAIN_WRITE, tlock, trans)

#define tlock_ObtainWriteNT(tlock) \
    lock_ObtainWrite_r(&(tlock)->lock)

#define tlock_ObtainWriteNoBlockNT(tlock) \
    lock_ObtainWriteNoBlock(&(tlock)->lock)

#define tlock_ConvertWToR(tlock) \
	lock_ConvertWToR(&(tlock)->lock);

#define tlock_ReleaseReadT(trans, tlock) \
    tlock_Equiv (TLOCK_RELEASE_READ, tlock, trans)

#define tlock_ReleaseRead(tlock) \
    lock_ReleaseRead_r (&(tlock)->lock);

#define tlock_ReleaseWrite(trans, tlock) \
    tlock_Equiv (TLOCK_RELEASE_WRITE, tlock, trans)

#define tlock_ReleaseWriteNT(tlock) \
    lock_ReleaseWrite_r (&(tlock)->lock);

#define tlock_HasOwner(tlock)	lock_HasOwner(&(tlock)->lock)
#define tlock_Check(tlock)	lock_Check(&(tlock)->lock)

#define tlock_EquivAssign(trans, tlock) \
    tlock_Equiv (TLOCK_EQUIV_ASSIGN, tlock, trans)
#define tlock_EquivNoAssign(trans, tlock) \
    tlock_Equiv (TLOCK_EQUIV, tlock, trans)

extern void tlock_Init (struct tlock_data *tlock, const char *name);
extern void tlock_Destroy (struct tlock_data *tlock);

extern void tlock_Equiv _TAKES((
  IN int op,
  IN struct tlock_data *tlock,
  IN buffer_tranRec_t trans));
extern void tlock_ObtainDoubleWriteNT _TAKES((
  IN struct tlock_data *tlock1,
  IN struct tlock_data *tlock2));
extern void tlock_Recycle (struct tlock_data *tlock);

#define osi_TSleepW(addr, tlock) osi_SleepW_r (addr, &(tlock)->lock)

#define AssertTWriteLocked(l) afsl_Assert(tlock_Check(l) == -1)
#define AssertTReadLocked(l) afsl_Assert(tlock_Check(l) > 0)
#define AssertTUnlocked(l) afsl_Assert(tlock_Check(l) == 0)
#define AssertTLocked(l) afsl_Assert(tlock_Check(l) != 0)

/* The following depend on ../tools/critical.h.  The header
 * <dcedfs/episode/critical.h> should included if these macros are used.
 * XXX Why don't we include it here?
 */

#define BEGIN_TLOCK_READ(tlock)			\
    BEGIN_LOCK (tlock_ObtainRead (tlock),	\
		tlock_ReleaseRead (tlock))

#define BEGIN_TLOCK_WRITE(trans,tlock)		\
    BEGIN_LOCK (tlock_ObtainWriteNT (tlock),	\
		tlock_ReleaseWrite (trans, tlock))

#define BEGIN_TLOCK_WRITE_NT(tlock)		\
    BEGIN_LOCK (tlock_ObtainWriteNT (tlock),	\
		tlock_ReleaseWriteNT (tlock))

extern void tmutex_Init (struct tmutex_data *tlock, const char *name);
extern void tmutex_Destroy (struct tmutex_data *tlock);
#define tmutex_GetTrans(tlock) ((tlock)->trans)
#define tmutex_held(tlock) epi_mutex_held(&(tlock)->lock)
#define tmutex_cond_wait(cv,tlock) epi_cond_wait (cv, &(tlock)->lock)
#define tmutex_HasOwner(tlock) epi_mutex_hasowner(&(tlock)->lock)

#define BEGIN_TMUTEX(trans,tlock)		\
    BEGIN_LOCK (epi_mutex_lock (&(tlock)->lock),\
		tlock_Equiv (TMUTEX_RELEASE,\
			     (struct tlock_data *)(tlock), trans))

#define BEGIN_TMUTEX_NT(tlock)			\
    BEGIN_LOCK (epi_mutex_lock (&(tlock)->lock),\
		epi_mutex_unlock (&(tlock)->lock))

#define tmutex_EquivAssign(trans, tlock) \
    tlock_Equiv (TMUTEX_EQUIV_ASSIGN, (struct tlock_data *)(tlock), trans)
#define tmutex_EquivNoAssign(trans, tlock) \
    tlock_Equiv (TMUTEX_EQUIV, (struct tlock_data *)(tlock), trans)

#endif /* !TRANSARC_EPI_TLOCK_H */
