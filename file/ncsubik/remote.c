/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: remote.c,v $
 * Revision 1.1.46.1  1996/10/02  17:55:47  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:42:42  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1991, 1996 Transarc Corporation - All rights reserved */
/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987, 1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */

#include <sys/types.h>
#include <sys/file.h>
#include <sys/time.h>
#include <netinet/in.h>

#include <dce/rpc.h>
#include <pthread.h>
#include <dcedfs/icl.h>
#include <dcedfs/osi.h>
#include <dcedfs/compat.h>
#include <dcedfs/dfsauth.h>

#define UBIK_INTERNALS
#include <utrace.h>
#include <ubikdisk_proc.h>
#include <ubikvote_proc.h>
#include <ubik.h>

/* routines for handling requests remotely-submitted by the sync site.  These 
   are only write transactions (we don't propagate read trans), and there is 
   at most one write transaction extant at any one time.
*/

struct ubik_trans *ubik_currentTrans = 0;
/* definitions of event set structures for tracing and setup function */
extern struct icl_set *ubikdisk_iclSetp;
extern void ubik_hold(), ubik_rele(), ubik_versionLock(), ubik_versionUnlock();
extern long ubik_CheckBadHost();

long ubik_CheckAuth(acall)
     handle_t acall;
{
  return(dfsauth_server_CheckAuthorization(&acall, (dacl_permset_t *)NULL, 
					   (char *)NULL, (uuid_t *)NULL));
}


error_status_t
SUBIKDISK_GetServerInterfaces (h, serverInterfacesP)
     handle_t h;
     dfs_interfaceList *serverInterfacesP;
{
    error_status_t st;
    int cancelState, cancelErr;

    DFS_DISABLE_CANCEL(&cancelState, cancelErr);
    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_GET_SERVER_INTERFACES, 
	       ICL_TYPE_POINTER, h);
    dfs_GetServerInterfaces (UBIKDISK_v4_0_s_ifspec, serverInterfacesP, &st);
    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_GET_SERVER_INTERFACES_EXIT,
	       ICL_TYPE_POINTER, h, ICL_TYPE_LONG, st);
    DFS_ENABLE_CANCEL(cancelState, cancelErr);
    return st;
}


/* the rest of these guys handle remote execution of write transactions: this 
   is the code executed on the other servers when a sync site is executing a 
   write transaction.
*/
SUBIKDISK_Begin(rxcall, atid)
    handle_t rxcall;
    struct ubik_tid * volatile atid;
{
    volatile long code;
    volatile int cancelState, cancelErr;
    char *excNameP, excString[20];
    static char rtnName[] = "SUBIKDISK_Begin";
    extern char *ExceptionToString();

    DFS_DISABLE_CANCEL(&cancelState, cancelErr);
    if (atid)
      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_BEGIN, 
		 ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG,
		 atid->epoch, ICL_TYPE_LONG, atid->counter);
    if (code = ubik_CheckAuth(rxcall)) {
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return code;
    }
    ubik_hold();
    if (code = ubik_CheckBadHost(rxcall)){
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
        return(code);
    }
    ubik_versionLock(ubik_dbase);
    TRY {
      icl_Trace1(ubikdisk_iclSetp, UK_TRACE_CALL_CHECKTID,
		 ICL_TYPE_STRING, rtnName);
      urecovery_CheckTid(atid);
      if (ubik_currentTrans) {
	udisk_end(ubik_currentTrans);	/* aborts it if it wasn't already 
					   committed */
	ubik_currentTrans = (struct ubik_trans *) 0;
      }
      code = udisk_begin(ubik_dbase, UBIK_WRITETRANS, &ubik_currentTrans);
      if (code == 0 && ubik_currentTrans) {
	/* label this trans with the right trans id */
	ubik_currentTrans->tid.epoch = atid->epoch;
	ubik_currentTrans->tid.counter = atid->counter;
      }
    } CATCH_ALL {
      excNameP = ExceptionToString(THIS_CATCH);
      if (excNameP == NULL) {
	sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	excNameP = excString;
      }
      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
		 ICL_TYPE_STRING,
		 excNameP, ICL_TYPE_STRING, __FILE__, 
		 ICL_TYPE_LONG,  __LINE__);
      code = UEXCEPTION;
    } ENDTRY    
    ubik_versionUnlock(ubik_dbase);
    ubik_rele();
    if (atid)
      icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_BEGIN_EXIT, 
		 ICL_TYPE_POINTER, rxcall,
		 ICL_TYPE_LONG, code);
    DFS_ENABLE_CANCEL(cancelState, cancelErr);
    return code;
}

SUBIKDISK_Commit(rxcall, atid)
    handle_t rxcall;
    struct ubik_tid *atid;
{
    volatile long code;
    struct ubik_dbase * volatile dbase;
    char *excNameP, excString[20];
    static char rtnName[] = "SUBIKDISK_Commit";
    extern char *ExceptionToString();
    volatile int cancelState, cancelErr;

    DFS_DISABLE_CANCEL(&cancelState, cancelErr);
    if (atid)
      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_COMMIT, 
		 ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, atid->epoch,
		 ICL_TYPE_LONG, atid->counter);
    if (code = ubik_CheckAuth(rxcall)) {
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return code;
    }
    ubik_hold();
    if (code = ubik_CheckBadHost(rxcall)){
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
        return(code);
    }
    if (!ubik_currentTrans) {
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_COMMIT_EXIT, 
		   ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, 
		   USYNC);
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return USYNC;
    }
    /*
     * sanity check to make sure only write trans appear here 
     */
    if (ubik_currentTrans->type != UBIK_WRITETRANS) {
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_COMMIT_EXIT, 
		   ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, 
		   UBADTYPE);
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return UBADTYPE;
    }
    dbase = ubik_currentTrans->dbase;
    ubik_versionLock(dbase);
    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_CALL_CHECKTID,
	       ICL_TYPE_STRING, rtnName);
    urecovery_CheckTid(atid);
    if (!ubik_currentTrans) {
	ubik_versionUnlock(dbase);
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_COMMIT_EXIT, 
		   ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, 
		   USYNC);
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return USYNC;
    }
    TRY {
      code = udisk_commit(ubik_currentTrans);
      if (code == 0) {
	/* sync site should now match */
	ubik_dbVersion = ubik_dbase->version;
      }
    } CATCH_ALL {
      excNameP = ExceptionToString(THIS_CATCH);
      if (excNameP == NULL) {
	sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	excNameP = excString;
      }
      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
		 ICL_TYPE_STRING,
		 excNameP, ICL_TYPE_STRING, __FILE__, 
		 ICL_TYPE_LONG,  __LINE__);
      code = UEXCEPTION;
    } ENDTRY
    ubik_versionUnlock(dbase);
    ubik_rele();
    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_COMMIT_EXIT, 
	       ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, 
	       code);
    DFS_ENABLE_CANCEL(cancelState, cancelErr);
    return code;
}

SUBIKDISK_ReleaseLocks(rxcall, atid)
    handle_t rxcall;
    struct ubik_tid *atid;
{
    struct ubik_dbase * volatile dbase;
    volatile long code;
    volatile int cancelState, cancelErr;
    static char rtnName[] = "SUBIKDISK_ReleaseLocks";
    char *excNameP, excString[20];
    extern char *ExceptionToString();

    DFS_DISABLE_CANCEL(&cancelState, cancelErr);

    if (atid)
      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_RELEASE_LOCKS,
		 ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, atid->epoch,
		 ICL_TYPE_LONG, atid->counter);
    if (code = ubik_CheckAuth(rxcall)) {
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_RELEASE_LOCKS_EXIT,
		   ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, code);
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return code;
    }
    ubik_hold();
    if (code = ubik_CheckBadHost(rxcall)){
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
        return(code);
    }
    if (!ubik_currentTrans) {
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_RELEASE_LOCKS_EXIT,
		   ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, USYNC);
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return USYNC;
    }
    /*
     * sanity check to make sure only write trans appear here 
     */
    if (ubik_currentTrans->type != UBIK_WRITETRANS) {
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_RELEASE_LOCKS_EXIT, 
		   ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, 
		   UBADTYPE);
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return UBADTYPE;
    }
    dbase = ubik_currentTrans->dbase;
    ubik_versionLock(dbase);
    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_CALL_CHECKTID,
	       ICL_TYPE_STRING, rtnName);
    urecovery_CheckTid(atid);
    if (!ubik_currentTrans) {
	ubik_versionUnlock(dbase);
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_RELEASE_LOCKS_EXIT,
		   ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, USYNC);
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return USYNC;
    }
    TRY {
      ulock_ReleaseAllLocks(ubik_currentTrans);
      udisk_end(ubik_currentTrans);
      ubik_currentTrans = (struct ubik_trans *) 0;
    } CATCH_ALL {
      excNameP = ExceptionToString(THIS_CATCH);
      if (excNameP == NULL) {
	sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	excNameP = excString;
      }
      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
		 ICL_TYPE_STRING,
		 excNameP, ICL_TYPE_STRING, __FILE__, 
		 ICL_TYPE_LONG,  __LINE__);
      code = UEXCEPTION;
    } ENDTRY
    ubik_versionUnlock(dbase);
    ubik_rele();
    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_RELEASE_LOCKS_EXIT,
	       ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, 0);
    DFS_ENABLE_CANCEL(cancelState, cancelErr);
    return code;
}

SUBIKDISK_Abort(rxcall, atid)
    handle_t rxcall;
    struct ubik_tid *atid;
{
    long code;
    struct ubik_dbase *dbase;
    static char rtnName[] = "SUBIKDISK_Abort";
    int cancelState, cancelErr;

    DFS_DISABLE_CANCEL(&cancelState, cancelErr);
    if (atid)
      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_ABORT, 
		 ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, atid->epoch,
		 ICL_TYPE_LONG, atid->counter);
    if (code = ubik_CheckAuth(rxcall)) {
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return code;
    }
    ubik_hold();
    if (code = ubik_CheckBadHost(rxcall)){
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
        return(code);
    }
    if (!ubik_currentTrans) {
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_ABORT_EXIT, 
		   ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, USYNC);
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return USYNC;
    }
    dbase = ubik_currentTrans->dbase;
    ubik_versionLock(dbase);
    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_CALL_CHECKTID,
	       ICL_TYPE_STRING, rtnName);
    urecovery_CheckTid(atid);
    if (!ubik_currentTrans) {
	ubik_versionUnlock(dbase);
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_ABORT_EXIT, 
		   ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, USYNC);
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return USYNC;
    }
    /*
     * sanity check to make sure only write trans appear here 
     */
    if (ubik_currentTrans->type != UBIK_WRITETRANS) {
        ubik_versionUnlock(dbase);
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_ABORT_EXIT, 
		   ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, 
		   UBADTYPE);
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return UBADTYPE;
    }
    code = udisk_abort(ubik_currentTrans);
    udisk_end(ubik_currentTrans);
    ubik_currentTrans = (struct ubik_trans *) 0;
    ubik_versionUnlock(dbase);
    ubik_rele();
    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_ABORT_EXIT, 
	       ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, code);
    DFS_ENABLE_CANCEL(cancelState, cancelErr);
    return code;
}

SUBIKDISK_Lock(rxcall, atid, afile, apos, alen, atype)
    handle_t rxcall;
    struct ubik_tid *atid;
    long afile, apos, alen, atype;
{
    long code;
    struct ubik_dbase *dbase;
    static char rtnName[] = "SUBIKDISK_Lock";
    int cancelState, cancelErr;

    DFS_DISABLE_CANCEL(&cancelState, cancelErr);
    if (atid)
      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_LOCK, ICL_TYPE_POINTER, 
		 rxcall, ICL_TYPE_LONG, atid->epoch, ICL_TYPE_LONG, 
		 atid->counter);
    icl_Trace4(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_LOCK_PARAMS, ICL_TYPE_LONG,
	       afile, ICL_TYPE_LONG, apos, ICL_TYPE_LONG, alen, ICL_TYPE_LONG,
	       atype);
    if (code = ubik_CheckAuth(rxcall)) {
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return code;
    }
    ubik_hold();
    if (code = ubik_CheckBadHost(rxcall)){
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
        return(code);
    }
    if (!ubik_currentTrans) {
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_LOCK_EXIT, 
		   ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, USYNC);
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return USYNC;
    }
    /*
     * sanity check to make sure only write trans appear here 
     */
    if (ubik_currentTrans->type != UBIK_WRITETRANS) {
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_LOCK_EXIT, 
		   ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, 
		   UBADTYPE);
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return UBADTYPE;
    }
    if (alen != 1) {
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_LOCK_EXIT, 
		   ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, UBADLOCK);
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return UBADLOCK;
    }
    dbase = ubik_currentTrans->dbase;
    ubik_versionLock(dbase);
    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_CALL_CHECKTID,
	       ICL_TYPE_STRING, rtnName);
    urecovery_CheckTid(atid);
    if (!ubik_currentTrans) {
	ubik_versionUnlock(dbase);
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_LOCK_EXIT, 
		   ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, USYNC);
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return USYNC;
    }
    code = ulock_SetLock(ubik_currentTrans, apos, alen, atype, 1);
    ubik_versionUnlock(dbase);
    ubik_rele();
    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_LOCK_EXIT, 
	       ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, code);
    DFS_ENABLE_CANCEL(cancelState, cancelErr);
    return code;
}

SUBIKDISK_Write(rxcall, atid, afile, apos, alen, adata)
    handle_t rxcall;
    struct ubik_tid *atid;
    long afile, apos, alen;
    bulkdata *adata; 
{
    long code;
    struct ubik_dbase *dbase;
    static char rtnName[] = "SUBIKDISK_Write";
    int cancelState, cancelErr;

    DFS_DISABLE_CANCEL(&cancelState, cancelErr);
    if (atid)
      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_WRITE, ICL_TYPE_POINTER, 
		 rxcall, ICL_TYPE_LONG, atid->epoch, ICL_TYPE_LONG,
		 atid->counter);
    icl_Trace4(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_WRITE_PARAMS,ICL_TYPE_LONG,
	       afile, ICL_TYPE_LONG, apos, ICL_TYPE_LONG, alen, 
	       ICL_TYPE_POINTER, adata);
    if (code = ubik_CheckAuth(rxcall)) {
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return code;
    }
    ubik_hold();
    if (code = ubik_CheckBadHost(rxcall)){
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
        return(code);
    }
    if (!ubik_currentTrans) {
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_WRITE_EXIT, 
		   ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, USYNC);
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return USYNC;
    }
    /*
     * sanity check to make sure only write trans appear here 
     */
    if (ubik_currentTrans->type != UBIK_WRITETRANS) {
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_WRITE_EXIT, 
		   ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, 
		   UBADTYPE);
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return UBADTYPE;
    }
    dbase = ubik_currentTrans->dbase;
    ubik_versionLock(dbase);
    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_CALL_CHECKTID,
	       ICL_TYPE_STRING, rtnName);
    urecovery_CheckTid(atid);
    if (!ubik_currentTrans) {
	ubik_versionUnlock(dbase);
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_WRITE_EXIT, 
		   ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, USYNC);
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return USYNC;
    }
    code = udisk_write(ubik_currentTrans, afile, adata, apos, alen);
    ubik_versionUnlock(dbase);
    ubik_rele();
    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_WRITE_EXIT, 
	       ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, code);
    DFS_ENABLE_CANCEL(cancelState, cancelErr);
    return code;
}

SUBIKDISK_BulkUpdate(rxcall, atid, acnt, abulk)
    handle_t rxcall;
    struct ubik_tid *atid;
    long acnt;
    ubik_bulk_update abulk[]; 
{
    volatile long code;
    volatile int cancelState, cancelErr;
    struct ubik_dbase *dbase;
    char *excNameP, excString[20];
    static char rtnName[] = "SUBIKDISK_BulkUpdate";
    extern char *ExceptionToString();
    volatile int nbulk;

    DFS_DISABLE_CANCEL(&cancelState, cancelErr);
    if (atid)
      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_BULKUPDATE,
		 ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, atid->epoch,
		 ICL_TYPE_LONG, atid->counter);
    for ( nbulk = 0 ; nbulk < acnt ; nbulk++ ) {
        icl_Trace4(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_BULKUPDATE_PARAMS_1,
		   ICL_TYPE_LONG, abulk[nbulk].op, ICL_TYPE_LONG,
		   abulk[nbulk].arg1, ICL_TYPE_LONG, abulk[nbulk].arg2,
		   ICL_TYPE_LONG, abulk[nbulk].arg3);
        icl_Trace3(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_BULKUPDATE_PARAMS_2,
		   ICL_TYPE_LONG, abulk[nbulk].arg4, ICL_TYPE_LONG,
		   abulk[nbulk].length, ICL_TYPE_POINTER, abulk[nbulk].data);
    }
    if (code = ubik_CheckAuth(rxcall)) {
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return code;
    }
    ubik_hold();
    if (code = ubik_CheckBadHost(rxcall)){
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
        return(code);
    }
    /*
     * If the first entry in the request is a BeginTrans, then start the new
     * transaction before processing the rest of the updates in the list.
     */
    if ( abulk[0].op == UBULK_BEGIN ) {
        ubik_versionLock(ubik_dbase);
        TRY {
          icl_Trace1(ubikdisk_iclSetp, UK_TRACE_CALL_CHECKTID,
		     ICL_TYPE_STRING, rtnName);
          urecovery_CheckTid(atid);
          if (ubik_currentTrans) {
	    /* aborts it if it wasn't already committed */
	    udisk_end(ubik_currentTrans);
	    ubik_currentTrans = (struct ubik_trans *) 0;
          }
          code = udisk_begin(ubik_dbase, UBIK_WRITETRANS, &ubik_currentTrans);
          if (code == 0 && ubik_currentTrans) {
	    /* label this trans with the right trans id */
	    ubik_currentTrans->tid.epoch = atid->epoch;
	    ubik_currentTrans->tid.counter = atid->counter;
          }
        } CATCH_ALL {
          excNameP = ExceptionToString(THIS_CATCH);
          if (excNameP == NULL) {
	    sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	    excNameP = excString;
          }
          icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
		     ICL_TYPE_STRING,
		     excNameP, ICL_TYPE_STRING, __FILE__, 
		     ICL_TYPE_LONG,  __LINE__);
          code = UEXCEPTION;
        } ENDTRY    
        ubik_versionUnlock(ubik_dbase);
	nbulk = 1;
    } else nbulk = 0;
    if (!ubik_currentTrans) {
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_BULKUPDATE_EXIT, 
		   ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, USYNC);
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return USYNC;
    }
    /*
     * sanity check to make sure only write trans appear here 
     */
    if (ubik_currentTrans->type != UBIK_WRITETRANS) {
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_BULKUPDATE_EXIT, 
		   ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, 
		   UBADTYPE);
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return UBADTYPE;
    }
    dbase = ubik_currentTrans->dbase;
    ubik_versionLock(dbase);
    if ( nbulk == 0 )
    {
        icl_Trace1(ubikdisk_iclSetp, UK_TRACE_CALL_CHECKTID,
	           ICL_TYPE_STRING, rtnName);
        urecovery_CheckTid(atid);
        if (!ubik_currentTrans) {
	    ubik_versionUnlock(dbase);
	    ubik_rele();
	    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_BULKUPDATE_EXIT, 
		       ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, USYNC);
	    DFS_ENABLE_CANCEL(cancelState, cancelErr);
	    return USYNC;
        }
    }
    for ( code = 0 ; code == 0 && nbulk < acnt ; nbulk++ ) {
	switch ( abulk[nbulk].op ) {
	  case UBULK_LOCK:
	    /* file parameter (arg1) isn't passed to ulock_Setlock */
	    code = ulock_SetLock(ubik_currentTrans, abulk[nbulk].arg2,
				 abulk[nbulk].arg3, abulk[nbulk].arg4, 1);
	    break;
	  case UBULK_TRUNC:
	    code = udisk_truncate(ubik_currentTrans, abulk[nbulk].arg1,
				  abulk[nbulk].arg2);
	    break;
	  case UBULK_WRITE:
            code = udisk_write(ubik_currentTrans, abulk[nbulk].arg1,
			       abulk[nbulk].data, abulk[nbulk].arg2,
			       abulk[nbulk].length);
	    break;
	  default:
	    code = UBADBULK;
	}
    }
    ubik_versionUnlock(dbase);
    ubik_rele();
    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_BULKUPDATE_EXIT, 
	       ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, code);
    DFS_ENABLE_CANCEL(cancelState, cancelErr);
    return code;
}

SUBIKDISK_Truncate(rxcall, atid, afile, alen)
    handle_t rxcall;
    struct ubik_tid *atid;
    long afile;
    long alen; 
{
    long code;
    struct ubik_dbase *dbase;
    static char rtnName[] = "SUBIKDISK_Truncate";
    int cancelState, cancelErr;

    DFS_DISABLE_CANCEL(&cancelState, cancelErr);
    if (atid)
      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_TRUNCATE, 
		 ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, atid->epoch,
		 ICL_TYPE_LONG, atid->counter);
    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_TRUNCATE_PARAMS,
	       ICL_TYPE_LONG, atid->epoch, ICL_TYPE_LONG, atid->counter);
    if (code = ubik_CheckAuth(rxcall)) {
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return code;
    }
    ubik_hold();
    if (code = ubik_CheckBadHost(rxcall)){
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
        return(code);
    }
    if (!ubik_currentTrans) {
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_TRUNCATE_EXIT, 
		   ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, USYNC);
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return USYNC;
    }
    dbase = ubik_currentTrans->dbase;
    ubik_versionLock(dbase);
    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_CALL_CHECKTID,
	       ICL_TYPE_STRING, rtnName);
    urecovery_CheckTid(atid);
    if (!ubik_currentTrans) {
	ubik_versionUnlock(dbase);
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_TRUNCATE_EXIT, 
		   ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, USYNC);
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return USYNC;
    }
    /*
     * sanity check to make sure only write trans appear here 
     */
    if (ubik_currentTrans->type != UBIK_WRITETRANS) {
        ubik_versionUnlock(dbase);
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_TRUNCATE_EXIT, 
		   ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, 
		   UBADTYPE);
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return UBADTYPE;
    }
    code = udisk_truncate(ubik_currentTrans, afile, alen);
    ubik_versionUnlock(dbase);
    ubik_rele();
    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_TRUNCATE_EXIT, 
	       ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, code);
    DFS_ENABLE_CANCEL(cancelState, cancelErr);
    return code;
}

SUBIKDISK_GetVersion(rxcall, aversion)
    handle_t rxcall;
    struct ubik_version *aversion; 
{
    long code;
    int cancelState, cancelErr;

    DFS_DISABLE_CANCEL(&cancelState, cancelErr);
    if (aversion)
      icl_Trace1(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_GETVERSION, 
		 ICL_TYPE_POINTER, rxcall);
    aversion->epoch = 0;
    aversion->counter = 0;
    /*
     * If we are the sync site, recovery shouldn't be running on any other
     * site. We shouldn't be getting this RPC as long as we are the sync site.
     * To prevent any unforseen activity, we should reject this RPC until
     * we have recognized that we are not the sync site anymore, and/or if
     * we have any pending WRITE transactions that have to complete. This
     * way we can be assured that this RPC would not block any pending 
     * transactions that should either fail or pass. If we have recognized the
     * fact that we are not the sync site any more, all write transactions
     * would fail with UNOQUORUM anyway.
     */
    if (code = ubik_CheckAuth(rxcall)) {
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return code;
    }
    ubik_hold();
    if (code = ubik_CheckBadHost(rxcall)){
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
        return(code);
    }
    if (ubeacon_AmSyncSite()) {
      ubik_rele();
      icl_Trace4(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_GETVERSION_EXIT, 
		 ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, aversion->epoch,
		 ICL_TYPE_LONG, aversion->counter, ICL_TYPE_LONG, UDEADLOCK);
      DFS_ENABLE_CANCEL(cancelState, cancelErr);
      return UDEADLOCK;
    }
    ubik_versionLock(ubik_dbase);
    code = (*ubik_dbase->getlabel) (ubik_dbase, 0, aversion);
    ubik_versionUnlock(ubik_dbase);
    if (code) {
	/* tell other side there's no dbase */
	aversion->epoch = 0;
	aversion->counter = 0;
    }
    ubik_rele();
    icl_Trace4(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_GETVERSION_EXIT, 
	       ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, aversion->epoch,
	       ICL_TYPE_LONG, aversion->counter, ICL_TYPE_LONG, 0);
    DFS_ENABLE_CANCEL(cancelState, cancelErr);
    return 0;
}

SUBIKDISK_GetFile(rxcall, file, result, version)
    volatile handle_t rxcall;
    long file;
    struct ubik_version * volatile version;
    pipe_t *result;
{
    volatile long code, excCode;
    struct ubik_dbase * volatile dbase;
    volatile long offset;
    struct ubik_stat ubikstat;
    unsigned char tbuffer[256];
    volatile unsigned long tlen;
    volatile long length;
    volatile int cancelState, cancelErr;
    char *excNameP, excString[20];
    extern char *ExceptionToString();

    DFS_DISABLE_CANCEL(&cancelState, cancelErr);
    if (version)
      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_GETFILE_EXIT,
		 ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, file,
		 ICL_TYPE_POINTER, result);
    version->epoch = version->counter = 0;
    if (code = ubik_CheckAuth(rxcall)) {
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return code;
    }
    ubik_hold();
    if (code = ubik_CheckBadHost(rxcall)){
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
        return(code);
    }
    if (ubeacon_AmSyncSite()) {
      ubik_rele();
      icl_Trace4(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_GETFILE_EXIT, 
		 ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, version->epoch,
		 ICL_TYPE_LONG, version->counter, ICL_TYPE_LONG, UDEADLOCK);
      DFS_ENABLE_CANCEL(cancelState, cancelErr);
      return UDEADLOCK;
    }
    dbase = ubik_dbase;
    ubik_versionLock(dbase);
    code = (*dbase->stat) (dbase, file, &ubikstat);
    if (code) {
	ubik_versionUnlock(dbase);
	ubik_rele();
	icl_Trace4(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_GETFILE_EXIT,
		   ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, version->epoch,
		   ICL_TYPE_LONG, version->counter, ICL_TYPE_LONG, code);
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return code;
    }
    length = ubikstat.size;
    offset = 0;
    excCode = 0;
    while (!excCode && length > 0) {
      tlen = (length > sizeof(tbuffer) ? sizeof(tbuffer) : length);
      code = (*dbase->read)(dbase, file, tbuffer, offset, tlen);
      if (code) 
	  break;    /* we still have to signal EOF in pipe */

      ubik_rele();
      TRY {
	(*result->push)(result->state, tbuffer, (long)tlen);/* no error return */
      } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	  sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	  excNameP = excString;
	}
	icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, ICL_TYPE_STRING,
		   excNameP, ICL_TYPE_STRING, __FILE__, ICL_TYPE_LONG,
		   __LINE__);
	excCode = UEXCEPTION;
      } ENDTRY
      ubik_hold();
      length -= tlen;
      offset += tlen;
    } /* while */

    if (!excCode) {
      ubik_rele();
      TRY {
	(*result->push)(result->state,  tbuffer, 0);/* signal end of pipe */
      } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	  sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	  excNameP = excString;
	}
	icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, ICL_TYPE_STRING,
		   excNameP, ICL_TYPE_STRING, __FILE__, ICL_TYPE_LONG,
		   __LINE__);
	excCode = UEXCEPTION;
      } ENDTRY
      ubik_hold();
    }
    if (code || excCode) {
      ubik_versionUnlock(dbase);
      ubik_rele();
      icl_Trace4(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_GETFILE_EXIT, 
		 ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, version->epoch,
		 ICL_TYPE_LONG, version->counter, ICL_TYPE_LONG, code);
      DFS_ENABLE_CANCEL(cancelState, cancelErr);
      return (code?code:excCode);
    }

    TRY {
      code = (*dbase->getlabel)(dbase, file, version);/*return the dbase,too */
    } CATCH_ALL {
      excNameP = ExceptionToString(THIS_CATCH);
      if (excNameP == NULL) {
	sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	excNameP = excString;
      }
      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, ICL_TYPE_STRING,
		 excNameP, ICL_TYPE_STRING, __FILE__, ICL_TYPE_LONG,
		 __LINE__);
      code = UEXCEPTION;
    } ENDTRY
    ubik_versionUnlock(dbase);
    ubik_rele();
    icl_Trace4(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_GETFILE_EXIT, 
	       ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, version->epoch,
	       ICL_TYPE_LONG, version->counter, ICL_TYPE_LONG, code);
    DFS_ENABLE_CANCEL(cancelState, cancelErr);
    return code;
}

SUBIKDISK_SendFile(rxcall, file, length, avers, apipe)
    handle_t rxcall;
    volatile long file;
    volatile long length;
    struct ubik_version * volatile avers;
    pipe_t * volatile apipe;
{
    volatile long code;
    struct ubik_dbase * volatile dbase;
    unsigned char tbuffer[256];
    long offset;
    struct ubik_version tversion;
    volatile unsigned long tlen;
    /* send the file back to the requester */
    volatile int cancelState, cancelErr;
    char *excNameP, excString[20];
    extern char *ExceptionToString();

    DFS_DISABLE_CANCEL(&cancelState, cancelErr);
    icl_Trace4(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_SENDFILE, 
	       ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, file,
	       ICL_TYPE_LONG, length, ICL_TYPE_POINTER, apipe);
    if (avers)
      icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_SENDFILE_PARAMS,
		 ICL_TYPE_LONG, avers->epoch, ICL_TYPE_LONG, avers->counter);
    
    if (code = ubik_CheckAuth(rxcall)) {
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
	return code;
    }
    ubik_hold();
    if (code = ubik_CheckBadHost(rxcall)){
	DFS_ENABLE_CANCEL(cancelState, cancelErr);
        return(code);
    }
    if (ubeacon_AmSyncSite()) {
      ubik_rele();
      icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_SENDFILE, 
		 ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, UDEADLOCK);
      DFS_ENABLE_CANCEL(cancelState, cancelErr);
      return UDEADLOCK;
    }
    dbase = ubik_dbase;
    ubik_versionLock(dbase);

    /* abort any active trans that may scribble over the database */
    urecovery_AbortAll(dbase);

    offset = 0;
    tversion.epoch = 0;	/* start off by labelling in-transit db as invalid */
    tversion.counter = 0;
    (*dbase->setlabel) (dbase, file, &tversion);      /* setlabel does sync */
    while (length > 0) {
      tlen = (length > sizeof(tbuffer) ? sizeof(tbuffer) : length);
      ubik_rele();
      code=0;

      TRY {
	(*apipe->pull)(apipe->state, tbuffer, 
				sizeof(tbuffer), (unsigned long *)&tlen);
      } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) { 
	  sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	  excNameP = excString;
	}
	icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, ICL_TYPE_STRING,
		   excNameP, ICL_TYPE_STRING, __FILE__, ICL_TYPE_LONG,
		   __LINE__); 
	code = UEXCEPTION;
      } ENDTRY 

      ubik_hold();
      if (code || (tlen == 0)) {
	  urecovery_AbortAll(dbase);
	  ubik_versionUnlock(dbase);
	  ubik_rele();
	  icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_SENDFILE_EXIT,
		     ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, code);
	  DFS_ENABLE_CANCEL(cancelState, cancelErr);
	  if (tlen == 0) {
	     /* 
	      * length should normally be 0 before this happens, so this is a
	      * premature EOF. Note: the real error is kept in pipe's state 
	      * XXX
	      */
	     code = 0;
	  }
	  return code;
      }
      
      TRY {
	code = (*dbase->write)(dbase, file, tbuffer, offset, tlen);
      } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	  sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	  excNameP = excString;
	}
	icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, ICL_TYPE_STRING,
		   excNameP, ICL_TYPE_STRING, __FILE__, ICL_TYPE_LONG,
		   __LINE__);
	code = UEXCEPTION;
      } ENDTRY

      if (code) {
	 volatile int st = 0;
	 /* We have to drain the pipe, abort transaction, and return error */
	 ubik_rele();
	 do {
	  TRY {
	    (*apipe->pull)(apipe->state, tbuffer,
				sizeof(tbuffer), (unsigned long *)&tlen);
	  } CATCH_ALL {
	    excNameP = ExceptionToString(THIS_CATCH);
	    if (excNameP == NULL) {
	      sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	      excNameP = excString;
	    }
	    icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, ICL_TYPE_STRING,
		       excNameP, ICL_TYPE_STRING, __FILE__, ICL_TYPE_LONG,
		       __LINE__);
	    st = UEXCEPTION;
	  } ENDTRY

	} while(!st && tlen);
	ubik_hold();
	urecovery_AbortAll(dbase);
	ubik_versionUnlock(dbase);
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_SENDFILE_EXIT,
		   ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, code);
	DFS_ENABLE_CANCEL(cancelState, cancelErr);

	return code;	/* preserve the error from the original write */
      }

      offset += tlen;
      length -= tlen;
    } /* while */

    /* we call pull here again, since otherwise it looks like the RPC package
     * raises a "pipe discipline error".
     */
    ubik_rele();
    code = 0;

    TRY {
      (*apipe->pull)(apipe->state, tbuffer,
			sizeof(tbuffer), (unsigned long *)&tlen);
    } CATCH_ALL {
      excNameP = ExceptionToString(THIS_CATCH);
      if (excNameP == NULL) {
	sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	excNameP = excString;
      }
      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, ICL_TYPE_STRING,
		 excNameP, ICL_TYPE_STRING, __FILE__, ICL_TYPE_LONG,
		 __LINE__);
      code = UEXCEPTION;
    } ENDTRY

    ubik_hold();
    if (code) { /* if an exception */
      urecovery_AbortAll(dbase);
      ubik_versionUnlock(dbase);
      ubik_rele();
      icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_SENDFILE_EXIT,
		 ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, code);
      DFS_ENABLE_CANCEL(cancelState, cancelErr);
      return code;
    }
    if (tlen != 0) {
      /* We have to drain the pipe, abort transaction, and return error */
      code=0;
      ubik_rele();
      do  {
	
	TRY {
	  (*apipe->pull)(apipe->state, tbuffer,
			sizeof(tbuffer), (unsigned long *)&tlen);
	} CATCH_ALL {
	  excNameP = ExceptionToString(THIS_CATCH);
	  if (excNameP == NULL) {
	    sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	    excNameP = excString;
	  }
	  icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, ICL_TYPE_STRING,
		     excNameP, ICL_TYPE_STRING, __FILE__, ICL_TYPE_LONG,
		     __LINE__);
	  code = UEXCEPTION;
	} ENDTRY

      } while(!code && tlen);
      ubik_hold();
      urecovery_AbortAll(dbase);
      ubik_versionUnlock(dbase);
      ubik_rele();
      icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_SENDFILE_EXIT,
		 ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, UPIPE);
      DFS_ENABLE_CANCEL(cancelState, cancelErr);
      return UPIPE;
    }
    /* sync data first, then write label and resync (resync done by setlabel 
       call). This way, good label is only on good database. */
    TRY {
      (*ubik_dbase->sync)(dbase, file);
      code = (*ubik_dbase->setlabel)(dbase, file, avers);
      ubik_dbase->version = *avers;
      udisk_Invalidate(dbase, file); /* new dbase, flush disk buffers */
    } CATCH_ALL {
      excNameP = ExceptionToString(THIS_CATCH);
      if (excNameP == NULL) {
	sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	excNameP = excString;
      }
      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, ICL_TYPE_STRING,
		 excNameP, ICL_TYPE_STRING, __FILE__, ICL_TYPE_LONG,
		 __LINE__);
      code = UEXCEPTION;
    } ENDTRY
    osi_Wakeup((opaque) &dbase->version);
    ubik_versionUnlock(dbase);
    ubik_rele();
    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_SENDFILE_EXIT,
	       ICL_TYPE_POINTER, rxcall, ICL_TYPE_LONG, code);
    DFS_ENABLE_CANCEL(cancelState, cancelErr);
    return code;
}


SUBIKDISK_Probe(rxcall)
     handle_t rxcall;
{
  /* probably can handle this sans locking */
  /*
   * We make this permission check because the calling server may not be in
   * our admin list. In this case, we don't want the calling server to think
   * that it can make RPCs to us and expect it to go through. In the case of
   * pipe transfer this may result in unwanted exceptions to be raised when
   * the caller makes a GetFile or SendFile RPC. Returning an error here would
   * mark us as down in the caller server and so, we wouldn't be getting such
   * RPCs until we add the calling server to our admin list.
   */
    int cancelState, cancelErr;
    long rc;

    DFS_DISABLE_CANCEL(&cancelState, cancelErr);
    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_PROBE, ICL_TYPE_POINTER, 
	     rxcall);
    rc=ubik_CheckAuth(rxcall);
    if ( !rc ){
        ubik_hold();
        rc = ubik_CheckBadHost(rxcall);
        ubik_rele();
      }
    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_SUBIKDISK_PROBE_EXIT, ICL_TYPE_LONG, rc);
    DFS_ENABLE_CANCEL(cancelState, cancelErr);
    return(rc);
}
