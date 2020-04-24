/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dbs_dump.c,v $
 * Revision 1.1.92.1  1996/10/02  16:59:59  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:19  damon]
 *
 * $EndLog$
 */
/*
*/
/*
 * Copyright (C) 1991, 1996 Transarc Corporation
 * All Rights Reserved
 */
/*
 * dbs_dump.c -- database dump code
 * 
 * 
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */
RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/bakserver/dbs_dump.c,v 1.1.92.1 1996/10/02 16:59:59 damon Exp $")

#include <sys/types.h>
#include <dcedfs/stds.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/file.h>
#include <dcedfs/param.h>
#include <stdio.h>
#include <dcedfs/compat.h>
#include <dcedfs/nubik.h>
#include <dcedfs/lock.h>
#include <dce/rpc.h>
#include <pthread.h>

#include <netinet/in.h>
#include <string.h>
#include <budb_errs.h>
#include <database.h>
#include <error_macros.h>
#include <globals.h>

/* interlocking for database dump */
dumpSyncT       dumpSync;
dumpSyncP       dumpSyncPtr = &dumpSync;

/* dump ubik database - interface routines */

/* badEntry
 *	no checking for now.
 */

long
badEntry(dbAddr)
     u_long dbAddr;
{
    /* return entry ok */
    return(0);
}

setupDbDump(writeFid)
     int writeFid;
{
    long code = 0;
    int cancelState, cancelCode;
    char *rtn_Name="setupDbDump";

    /* This thread is cancellable. However, we cannot be cancelled while
     * holding the lock. This would lead to a deadlock. So we protect 
     * ourselves while we hold the lock.
     */
    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      Log("%s: DFS_DISABLE_CANCEL failed with errno %lu\n",
	  rtn_Name, errno);
    lock_ObtainWrite(&(dumpSyncPtr->ds_lock));
    code = InitRPC(&(dumpSyncPtr->ut), LOCKREAD, (int *) 1 /*this_op*/);
    if ( code ) {
        lock_ReleaseWrite(&(dumpSyncPtr->ds_lock));
	goto error_exit;
    }
    lock_ReleaseWrite(&(dumpSyncPtr->ds_lock));
    /* We could get cancelled during the enable cancel, but that's ok because 
     * we are not holding on to any resources.
     */
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    if (cancelCode)
      Log("%s: DFS_ENABLE_CANCEL failed with errno %lu\n",
	  rtn_Name, errno);
    code = writeDatabase(dumpSyncPtr->ut, writeFid);
    if ( code )
	LogError(code, "writeDatabase failed\n");
    code = close(writeFid);
    if ( code )
	LogError(code, "pipe writer close failed\n");

    LogDebug(5, "writeDatabase complete\n");

error_exit:
    if ( dumpSyncPtr->ut )
	ubik_EndTrans(dumpSyncPtr->ut);
    return(code);
}


error_status_t
BUDB_DumpDB(h, maxLength, done, charListPtr)
  handle_t h;
  long maxLength;
  long *done;
  charListT	*charListPtr;
{
    register int readSize=0;     /* data we got on each read from pipe */
    register long readCounter=0; /* keep track of how much data we got */
    long code = 0;
    char *rtn_Name="BUDB_DumpDB";
    extern dumpWatcher();
    int cancelState, cancelCode;

    bak_Lock(rtn_Name, &cancelState, &cancelCode, BAK_CANCELS);
    if (cancelCode)
      return(cancelCode);

    if (!callPermitted(h)) {
      bak_Unlock(rtn_Name, cancelState, &cancelCode, BAK_CANCELS);
      return BUDB_NOTPERMITTED;
    }
    LogDebug(2, "Entering BUDB_DumpDB: \n");
    LogDebug(1, "database dump requesting data length %ld\n", maxLength);
    *done=0;
    charListPtr->charListT_len = 0;
    /* We grab the lock just to synchronize with the writer */
    lock_ObtainWrite(&(dumpSyncPtr->ds_lock));
    if ( dumpSyncPtr->statusFlags == 0 )
    {
	/* mark dump in progress */
	dumpSyncPtr->statusFlags = 1;
	/* reset the clock on dump timeout */
	dumpSyncPtr->timeToLive = time(0) + DUMP_TTL_INC;

	code = pipe(dumpSyncPtr->pipeFid);
	if ( code ) {
	  lock_ReleaseWrite(&(dumpSyncPtr->ds_lock));
	  bak_Unlock(rtn_Name, cancelState, &cancelCode, BAK_CANCELS);
	  ERROR(BUDB_DUMPFAILED);
	}
	LogDebug(1, "starting dump, setting up pipe with descriptors %d, %d\n",
		 dumpSyncPtr->pipeFid[0], dumpSyncPtr->pipeFid[1]);

	code = pthread_create ( &dumpSyncPtr->dumper, 
			        pthread_attr_default,
			        (pthread_startroutine_t)setupDbDump, 
			        (pthread_addr_t)dumpSyncPtr->pipeFid[1]
			       );
	if (IS_PTHR_ERROR(code)) {
	    lock_ReleaseWrite(&(dumpSyncPtr->ds_lock));
	    bak_Unlock(rtn_Name, cancelState, &cancelCode, BAK_CANCELS);
	    LogError(code, "BUDB_DumpDB: Create thread failed, %s\n",
		     dfs_dceErrTxt(code));
	    ERROR(code);
	}
	code = pthread_create ( &dumpSyncPtr->watcher,
			        pthread_attr_default,
			        (pthread_startroutine_t)dumpWatcher, 
			        (pthread_addr_t)0
			       );
	if (IS_PTHR_ERROR(code)) {
	    lock_ReleaseWrite(&(dumpSyncPtr->ds_lock));
	    bak_Unlock(rtn_Name, cancelState, &cancelCode, BAK_CANCELS);
	    LogError(code, "BUDB_DumpDB: Create thread failed, %s\n",
		     dfs_dceErrTxt(code));
	    ERROR(code);
	}
	LogDebug(1, "created dump thread %ld and watcher thread %ld\n",
		 (long)&dumpSyncPtr->dumper, (long)&dumpSyncPtr->watcher);
    }
    lock_ReleaseWrite(&(dumpSyncPtr->ds_lock));
    *done=0; readCounter=0;
    /* now read the database and feed it to the rpc connection */
    LogDebug(1, "read the data from pipe and send it back over the RPC\n");
    while (readCounter < maxLength) {
      readSize = read(dumpSyncPtr->pipeFid[0], 
		      (char *)charListPtr->charListT_val+readCounter,
		      maxLength-readCounter);
      if (readSize > 0) {
	readCounter += readSize;
	LogDebug(1, "read data of size %ld, so far read %ld", 
		 readSize, readCounter);
	/* reset the clock on dump timeout */
	lock_ObtainWrite(&(dumpSyncPtr->ds_lock));
	dumpSyncPtr->timeToLive = time(0) + DUMP_TTL_INC;
	lock_ReleaseWrite(&(dumpSyncPtr->ds_lock));
      }
      else if (readSize == 0){
	LogDebug(1, "no more data, we are done\n");
	*done = 1;
	code = 0;
	break;
      }
      else { /* some error happened, we have to abort */
	LogDebug(1, "error reading from pipe, return failure\n");
	*done = 1;
	code = BUDB_DUMPFAILED;
	break;
      }
    } /* while */

    charListPtr->charListT_len = readCounter;
    lock_ObtainWrite(&(dumpSyncPtr->ds_lock));
    if ( *done ) {				/* last chunk */
	close(dumpSyncPtr->pipeFid[0]);
	dumpSyncPtr->statusFlags = 0;
	dumpSyncPtr->timeToLive = time(0) + DUMP_TTL_INC;
    }
    else
      dumpSyncPtr->timeToLive = time(0) + 10; /* kill threads soon */
    LogDebug(1, "total read %ld bytes\n", readCounter);
    LogDebug(2, "Leaving BUDB_DumpDB, code %ld\n", code);
    lock_ReleaseWrite(&(dumpSyncPtr->ds_lock));
    bak_Unlock(rtn_Name, cancelState, &cancelCode, BAK_CANCELS);
error_exit:
    return(code);
}

error_status_t
BUDB_RestoreDbHeader(h, header)
  handle_t h;
  struct DbHeader *header;
{
    struct ubik_trans *ut = 0;
    long code = 0;
    char *rtn_Name="BUDB_RestoreDbHeader";
    extern struct memoryDB db;
    int cancelState, cancelCode;

    bak_Lock(rtn_Name, &cancelState, &cancelCode, BAK_CANCELS);
    if (cancelCode)
      return(cancelCode);

    if (!callPermitted(h)) {
      bak_Unlock(rtn_Name, cancelState, &cancelCode, BAK_CANCELS);
      return BUDB_NOTPERMITTED;
    }
    code = InitRPC(&ut, LOCKWRITE, (int *) 1 /*this_op*/);
    if ( code ) {
      bak_Unlock(rtn_Name, cancelState, &cancelCode, BAK_CANCELS);
      return (code);
    }
    LogDebug(2, "Entering BUDB_RestoreDbHeader: \n");
    LogDebug(1, "restoring database header\n");
    lock_ObtainShared(&(db.lock));
    if ( header->dbversion != ntohl(db.h.version) ) {
        lock_ReleaseShared(&(db.lock));
	ERROR(BUDB_VERSIONMISMATCH);
    }
    lock_UpgradeSToW(&(db.lock));
    /* merge rather than replace the header information */
    if ( db.h.lastDumpId < htonl(header->lastDumpId))
	db.h.lastDumpId = htonl(header->lastDumpId);

    if ( db.h.lastTapeId < htonl(header->lastTapeId))
	db.h.lastTapeId = htonl(header->lastTapeId);

    if ( db.h.lastInstanceId < htonl(header->lastInstanceId))
	db.h.lastInstanceId = htonl(header->lastInstanceId);
    LogDebug(1, "merging database header, last dumpid %ld, last tapeid %ld, last instance id %ld\n", 
	     ntohl(db.h.lastDumpId), ntohl(db.h.lastTapeId), 
	     ntohl(db.h.lastInstanceId));
    code = dbwrite(ut, 0, (char *) &db.h, sizeof(db.h));
    if ( code )
	code = BUDB_IO;

    lock_ReleaseWrite(&(db.lock));
    ubik_EndTrans(ut);
    LogDebug(2, "Leaving BUDB_RestoreDbHeader, code %ld\n", code);
    if (code)
      LogDebug(1, "failed to restore database header, code=%ld\n");
    else
      LogDebug(1, "successfully completed restoring database header\n");
    bak_Unlock(rtn_Name, cancelState, &cancelCode, BAK_CANCELS);
    return(code);

error_exit:
    ubik_AbortTrans(ut);
    LogDebug(2, "Leaving BUDB_RestoreDbHeader, code %ld\n", code);
    LogDebug(1, "failed to restore database header, code=%ld\n");
    bak_Unlock(rtn_Name, cancelState, &cancelCode, BAK_CANCELS);
    return(code);
}

/* dumpWatcher
 *	monitors the state of a database dump. If the dump call do not 
 *	reset the time to live value, the dump times out. In that case,
 *	we kill the database traversal thread and clean up all the other 
 *	state. Most importantly, the database is unlocked so that other
 *	transactions can proceed.
 */

dumpWatcher()
{
    long code=0;
    struct timespec delayTime;
    int cancelState, cancelCode;
    char *rtn_Name="dumpWatcher";

    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    while ( 1 ) { /*w*/
        lock_ObtainShared(&(dumpSyncPtr->ds_lock));
	if ( dumpSyncPtr->statusFlags == 0 ) {
	  /* dump has finished */
	  lock_ReleaseShared(&(dumpSyncPtr->ds_lock));
	  goto exit;
	}

	/* check time to live */
	if ( time(0) > dumpSyncPtr->timeToLive ) { /*if*/
	    /* dump has exceeded the allocated time - terminate it */
	    Log("%s: Terminating database dump after timeout exceeded: %s\n",
		rtn_Name, ctime(&dumpSyncPtr->timeToLive));
	    LogError(0, "Database dump timeout exceeded: %s\n",
		     ctime(&dumpSyncPtr->timeToLive));
	    LogError(0, "Terminating database dump\n");
	    LogDebug(1,"%s: terminating database dump, timeout exceeded: %s\n",
		     rtn_Name, ctime(&dumpSyncPtr->timeToLive));
	    lock_UpgradeSToW(&(dumpSyncPtr->ds_lock));
	    /* The pipe closures would probably raise SIGPIPE in writeDatabase.
	     * It will have to register a handler for it.
	     */
	    close(dumpSyncPtr->pipeFid[0]);
	    close(dumpSyncPtr->pipeFid[1]);
	    code = pthread_cancel (dumpSyncPtr->dumper);
	    if (IS_PTHR_ERROR(code))
		LogError(code, "dumpWatcher: failed to kill dump thread, %s\n",
			 dfs_dceErrTxt(code));
	    if ( dumpSyncPtr->ut ) {
		code = ubik_AbortTrans(dumpSyncPtr->ut);
		if ( code )
		    LogError(code, "Aborting dump transaction\n");
	    }
	    lock_ReleaseWrite(&(dumpSyncPtr->ds_lock));
	    goto exit;
	} /*if*/

	lock_ReleaseShared(&(dumpSyncPtr->ds_lock));
        delayTime.tv_sec = 5;
        delayTime.tv_nsec = 0;
        (void)pthread_delay_np (&delayTime);
	pthread_yield();
    } /*w*/
exit:
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return(code);
}
