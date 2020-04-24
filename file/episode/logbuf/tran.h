/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: tran.h,v $
 * Revision 1.1.63.1  1996/10/02  17:25:46  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:32:55  damon]
 *
 * $EndLog$
 */
/* tran.h */

/*
 * (C) Copyright 1995, 1992 Transarc Corporation
 * All Rights Reserved
 */

/*
 * This package describes the data structures and interfaces into the
 * transaction management system.
 *
 * $Header: /u0/rcs_trees/dce/rcs/file/episode/logbuf/tran.h,v 1.1.63.1 1996/10/02 17:25:46 damon Exp $
 */

#if !defined(TRANSARC_EPISODE_TRAN_H)
#define TRANSARC_EPISODE_TRAN_H

#include <dcedfs/hash.h>
#include <dcedfs/episode/logbuf.h>

/*
 * transaction status
 */
#define ELBT_TRANSTAT_FREE (0x0)
#define ELBT_TRANSTAT_ACTIVE (0xFE4378)
#define ELBT_TRANSTAT_PENDING (0x84A3C0DA)
#define ELBT_TRANSTAT_COMPLETE (0xE39804)

/*
 * Macros
 */

#define elbt_TranLock(tP) lock_ObtainWrite_r(&(tP)->tranLock)
#define elbt_TranUnlock(tP) \
    do { elbt_AssertTranLocked(tP); \
	 lock_ReleaseWrite_r(&(tP)->tranLock); } while (0)
#define elbt_TranUnlockAndSleep(tP,addr) \
    do { elbt_AssertTranLocked(tP); \
	 osi_SleepW_r((opaque)addr, &(tP)->tranLock); } while (0)
#define elbt_TranWriteLocked(tP) (lock_Check(&(tP)->tranLock) == -1)
#define elbt_AssertTranLocked(tP) afsl_AssertWriteLocked(&(tP)->tranLock)
#define elbt_TranLockNoBlock(tP) lock_ObtainWriteNoBlock(&(tP)->tranLock)

#define elbt_ReleaseTran(tP) elbt_TranUnlock(tP)

/* SHARED prototypes */
extern void elbt_Init _TAKES((u_int32));
extern struct elbt_TranRec *elbt_GetTran(elbt_TranRec_t tid, int active);
extern void elbt_StartTran _TAKES((const caddr_t, elbt_TranRec_t,
				   struct async_device *, elbt_TranRec_t *));
extern void elbt_Shutdown _TAKES((struct async_device *,
				  void (*const)(struct buffer *, int)));
extern void elbt_TranstatWait _TAKES((struct elbt_TranRec *));
extern void elbt_GCTran _TAKES((
  IN struct async_device *dP,
  IN int force));
extern void elbt_Quiesce _TAKES((
  IN struct async_device *dP,
  IN int consistency,			/* 0x1 quiesce/commit | 0x2 reg/undo */
  IN int arg_pass,			/* These are really u_int16s */
  IN int arg_off));

#endif /* !defined(TRANSARC_EPISODE_TRAN_H) */
