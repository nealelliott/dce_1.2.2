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
 * Revision 1.1.626.1  1996/10/02  18:10:10  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:44:44  damon]
 *
 * Revision 1.1.621.2  1994/06/09  14:16:46  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:28:57  annie]
 * 
 * Revision 1.1.621.1  1994/02/04  20:26:02  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:16:13  devsrc]
 * 
 * Revision 1.1.619.1  1993/12/07  17:30:45  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/06  13:43:27  jaffe]
 * 
 * $EndLog$
 */

/* Copyright (C) 1993, 1990, 1989 Transarc Corporation - All rights reserved */

#define DEBUG_THIS_FILE OSI_DEBUG_LOCK

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/debug.h>
#include <dcedfs/osi.h>
#include <dcedfs/lock.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/osi/lock.c,v 1.1.626.1 1996/10/02 18:10:10 damon Exp $")

int afsdb_osi = 0;

/*
 * Generic routines common to all lock package implementations.
 */

/*
 * obtain the specified type of lock
 */
void
lock_Obtain(osi_dlock_t *lock, int lockType)
{
    switch (lockType) {
      case READ_LOCK:
	lock_ObtainRead(lock);
	break;
      case WRITE_LOCK:
	lock_ObtainWrite(lock);
	break;
      case SHARED_LOCK:
	lock_ObtainShared(lock);
	break;
      case BOOSTED_LOCK:
	lock_UpgradeSToW(lock);
	break;
      default:
	panic("lock_Obtain");
    }
}

/*
 * release a lock, taking the type as a parameter.
 */
void
lock_Release(osi_dlock_t *lockp, int type)
{
    switch(type) {
      case READ_LOCK:
	lock_ReleaseRead(lockp);
	break;
      case WRITE_LOCK:
	lock_ReleaseWrite(lockp);
	break;
      case SHARED_LOCK:
	lock_ReleaseShared(lockp);
	break;
      default:
	panic("lock_Release");
    }
}

/*
 * obtain the specified type of lock without blocking on the lock
 */
int
lock_ObtainNoBlock(osi_dlock_t *lock, int lockType)
{
    switch (lockType) {
      case READ_LOCK:
	return lock_ObtainReadNoBlock(lock);
      case WRITE_LOCK:
	return lock_ObtainWriteNoBlock(lock);
      case SHARED_LOCK:
	return lock_ObtainSharedNoBlock(lock);
      case BOOSTED_LOCK:
	return lock_UpgradeSToWNoBlock(lock);
      default:
	panic("lock_ObtainNoBlock");
	/* NOTREACHED */
    }
}
