/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: db_lock.c,v $
 * Revision 1.1.89.1  1996/10/02  16:59:55  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:17  damon]
 *
 * $EndLog$
 */
/*
 * Copyright (C) 1991, 1996 Transarc Corporation
 * All Rights Reserved
 */
/*
 * db_lock.c -- Lock code for bakserver
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */
RCSID("$Header: /u0/rcs_trees/dce/rcs/file/bakserver/db_lock.c,v 1.1.89.1 1996/10/02 16:59:55 damon Exp $")

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <dcedfs/param.h>
#include <dcedfs/compat.h>
#include <dcedfs/nubik.h>
#include <dcedfs/lock.h>
#include <budb_errs.h>
#include <database.h>
#include <error_macros.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

#include <globals.h>

/* The bakserver database defined in database.c */
extern struct memoryDB db;
#define	DBH_POS(ptr)		( (char *) (ptr) - (char *) &db.h )

error_status_t
BUDB_FreeAllLocks(h, instanceId)
  handle_t h;
     u_long instanceId;
{
    db_lockP    startPtr, endPtr;
    struct ubik_trans *ut;
    long code;
    char *rtn_Name = "BUDB_FreeAllLocks";
    int cancelState, cancelCode;

    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);

    if (!callPermitted(h)) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return BUDB_NOTPERMITTED;
    }

    code = InitRPC(&ut, LOCKWRITE, (int *) 1);
    if ( code ) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(code);
    }
    LogDebug(2, "Entering BUDB_FreeAllLocks: \n");
    LogDebug(2, "input parameters: instance Id is %lu\n", instanceId);
    LogDebug(1, "freeing all locks\n");
    lock_ObtainRead(&(db.lock));
    startPtr = &db.h.textLocks[0];
    endPtr = &db.h.textLocks[TB_NUM-1];
    lock_ReleaseRead(&(db.lock));
    while ( startPtr <= endPtr )
    {
	if ( (ntohl(startPtr->lockState) == 1)
        &&   (ntohl(startPtr->instanceId) == instanceId)
           )
	{
	    LogDebug(1, "found lock type %ld, state %ld, time %ld, expires %ld, instanceId %ld, host %ld\n",
		     startPtr->type, startPtr->lockTime, startPtr->expires,
		     startPtr->instanceId, startPtr->lockHost);
	    /* release the lock */
	    startPtr->lockState = 0;			/* unlock it */
	    startPtr->lockTime = 0;
	    startPtr->expires = 0;
	    startPtr->instanceId = 0;
	    LogDebug(1, "unlocking this lock\n");
	    dbwrite(ut, DBH_POS(startPtr), (char *) startPtr,
		    sizeof(db_lockT));
	}
        startPtr++;
    }
    code = ubik_EndTrans(ut);
    LogDebug(2, "Leaving BUDB_FreeAllLocks, code %ld\n", code);
    if (code)
      LogDebug(1, "failed to free all locks, code=%ld\n", code);
    else
      LogDebug(1, "successfully freed all locks\n");
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return(code);
}
error_status_t
BUDB_FreeLock(h, lockHandle)
  handle_t h;
     u_long lockHandle;
{
    db_lockP	lockPtr = 0;
    struct ubik_trans *ut;
    long code;
    char *rtn_Name="BUDB_FreeLock";
    int cancelState, cancelCode;

    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);

    if (!callPermitted(h)) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return BUDB_NOTPERMITTED;
    }
    code = InitRPC(&ut, LOCKWRITE, (int *) 1);
    if ( code ) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(code);
    }
    LogDebug(2, "Entering BUDB_FreeLock: \n");
    LogDebug(2, "input parameters: lock handle %lu\n", lockHandle);
    LogDebug(1, "freeing lock %lu\n", lockHandle);
    lock_ObtainRead(&(db.lock));
    if ( checkLockHandle(ut, lockHandle) == 0 ) {
      lock_ReleaseRead(&(db.lock));
      ABORT(BUDB_BADARGUMENT);
    }
    lockPtr = (db_lockP) lockHandle;
    lockPtr->lockState = 0;                             /* unlock it */
    lockPtr->lockTime = 0;
    lockPtr->expires = 0;
    lockPtr->instanceId = 0;
    LogDebug(1, "unlocking %lu\n", lockHandle);
    dbwrite(ut, DBH_POS(lockPtr), (char *) lockPtr, sizeof(db_lockT));
    lock_ReleaseRead(&(db.lock));
    code = ubik_EndTrans(ut);
    LogDebug(2, "Leaving BUDB_FreeLock, code %ld\n", code);
    if (code)
      LogDebug(1, "failed to free lock, code=%ld\n", code);
    else
      LogDebug(1, "successfully freed lock\n");
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return(code);

abort_exit:
    ubik_AbortTrans (ut);
    LogDebug(2, "Leaving BUDB_FreeLock, code %ld\n", code);
    LogDebug(1, "failed to free lock, code=%ld\n", code);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return(code);
}

error_status_t
BUDB_GetInstanceId(h, instanceId)
  handle_t h;
     u_long *instanceId;
{
    struct ubik_trans *ut;
    long code;
    long instanceValue;
    char *rtn_Name="BUDB_GetInstanceId";
    int cancelState, cancelCode;

    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);

    if (!callPermitted(h)) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return BUDB_NOTPERMITTED;
    }

    code = InitRPC(&ut, LOCKWRITE, (int *) 1);
    if ( code ) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(code);
    }
    LogDebug(2, "Entering BUDB_GetInstanceId: \n");
    lock_ObtainRead(&(db.lock));
    instanceValue = ntohl(db.h.lastInstanceId) + 1;
    lock_ReleaseRead(&(db.lock));
    set_header_word(ut, lastInstanceId, htonl(instanceValue));
    *instanceId = instanceValue;
    LogDebug(2, "output parameters: instance id %lu\n", *instanceId);

    code = ubik_EndTrans(ut);
    LogDebug(2, "Leaving BUDB_GetInstanceId, code %ld\n", code);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return(code);
}

error_status_t
BUDB_GetLock(h, instanceId, lockName, expiration, lockHandle)
  handle_t h;
     u_long instanceId;
     long lockName;
     long expiration;
     u_long *lockHandle;
{
    struct timeval tv;
    db_lockP	lockPtr;
    struct ubik_trans *ut;
    char *rtn_Name="BUDB_GetLock";
    long code;
    int cancelState, cancelCode;

    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);

    if (!callPermitted(h)) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return BUDB_NOTPERMITTED;
    }
    if ( (lockName < 0) || (lockName >= TB_NUM) ) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_BADARGUMENT);
    }
    /* get the current time */
    osi_gettimeofday(&tv, 0);

    code = InitRPC(&ut, LOCKWRITE, (int *) 1);
    if ( code ) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(code);
    }
    LogDebug(2, "Entering BUDB_GetLock: \n");
    LogDebug(2,"input parameters: instance id %lu, name %ld, expires %ld\n",
	     instanceId, lockName, expiration);
    LogDebug(1, "getting lock %ld\n", lockName);
    lock_ObtainRead(&(db.lock));
    lockPtr = &db.h.textLocks[lockName];

    if ( (ntohl(lockPtr->lockState) != 0)		/* lock set */
    &&   (ntohl(lockPtr->expires) > tv.tv_sec)		/* not expired */
       )
    {
        LogDebug(1, "requested lock already locked ");
	if ( ntohl(lockPtr->instanceId) == instanceId ) {
	  LogDebug(1, "by me\n");
	  code = BUDB_SELFLOCKED;
	}
	else {
	  LogDebug(1, "by someone else\n");
	  code = BUDB_LOCKED;
	}
	lock_ReleaseRead(&(db.lock));
	goto abort_exit;
    }
    LogDebug(1, "trying to acquire lock; ");
    lockPtr->lockState = htonl(1);			/* lock it */
    lockPtr->lockTime = htonl(tv.tv_sec);		/* when locked */
    lockPtr->expires = htonl(tv.tv_sec + expiration);
    lockPtr->instanceId = htonl(instanceId);
    code = dbwrite(ut, DBH_POS(lockPtr), (char *) lockPtr, sizeof(db_lockT));
    if ( code ) {
      lock_ReleaseRead(&(db.lock));
      ABORT(code);
    }
    *lockHandle = (u_long) lockPtr;
    LogDebug(1, "successfully obtained, lock handle %ld\n", *lockHandle);
    lock_ReleaseRead(&(db.lock));
    code = ubik_EndTrans(ut);
    LogDebug(2, "Leaving BUDB_GetLock, code %ld\n", code);
    if (code)
      LogDebug(1, "failed to get lock, code=%ld\n", code);
    else
      LogDebug(1, "successfully obtained lock\n");
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return(code);

abort_exit:
    ubik_AbortTrans (ut);
    LogDebug(2, "Leaving BUDB_GetLock, code %ld\n", code);
    LogDebug(1, "failed to get lock, code=%ld\n", code);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return(code);
}


/* checkLockHandle
 * exit:
 *	0 - if invalid handle
 *	1 - if handle is valid
 */

checkLockHandle(ut, lockHandle)
     struct ubik_trans *ut;
     u_long lockHandle;
{
    db_lockP    startPtr, endPtr;

    /* sanity check the arguments */
    startPtr = &db.h.textLocks[0];
    endPtr = &db.h.textLocks[TB_NUM-1];
    while ( startPtr <= endPtr )
    {
        if ( startPtr == (db_lockP) lockHandle )
	    return(1);

        startPtr++;
    }
    return(0);
}
