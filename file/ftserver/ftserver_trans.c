/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ftserver_trans.c,v $
 * $EndLog$
 */
/* Copyright (C) 1996, 1990 Transarc Corporation - All rights reserved. */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <fcntl.h>

#include <dcedfs/compat.h>

#include <dce/rpc.h>
#include <pthread.h>

#include <ftserver_proc.h>
#include <ftserver_data.h>

#include <dcedfs/queue.h>

#include <dcedfs/volume.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/basicincludes.h>
#include <dcedfs/sock_pipe.h>
#include <ftserver.h>
#include <ftserver_trans.h>
#include <ftserver_trace.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfstsvmac.h>
#include <dfstsvsvc.h>
#include <dfstsvmsg.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/ftserver/RCS/ftserver_trans.c,v 4.50 1996/10/12 18:10:32 madhuc Exp $")

/*
 * Module's variables
 */
struct ftserver_trans *ftserver_trans = 0;	/* Root of trans linked list */
static osi_dlock_t ftserver_translock;		/* allocation lock for trans */
static long ftserver_transCounter = 1;		/* Unique transaction Id */
static int ftserver_transGC = 0;		/* number of GCed trans */


/*
 * Initializes the trans module
 */
trans_Init()
{
    static int trans_initiated = 0;

    if (trans_initiated)
	return;
    trans_initiated = 1;
    lock_Init(&ftserver_translock);
}


/*
 * Create a new transaction, returning ptr to same with high ref count
 */
struct ftserver_trans *ftserver_NewTrans(volIdp, aggrId, inCode, codep)
    afs_hyper_t *volIdp;
    long aggrId;
    long inCode;
    long *codep;
{
    register struct ftserver_trans *transp;

    icl_Trace2(iclSetP, FTSERVER_TRACE_ENTER_FTSERVER_NEW_TRANS,
	       ICL_TYPE_HYPER, volIdp, ICL_TYPE_LONG, aggrId);

    /* should never be zero; think of this as an assertion */
    if (!inCode) {
	    Log(tsv_s_trans_log_new_trans, AFS_HGETBOTH(*volIdp), aggrId);
	    exit(1);
    }

    lock_ObtainWrite(&ftserver_translock);

    for (transp = ftserver_trans; transp; transp = transp->t_next) {
	if (AFS_hsame(transp->t_volId, *volIdp) &&
	    (transp->t_aggrId == aggrId)){
	    *codep = transp->t_accError;
	    if (!*codep)
		*codep = EINVAL;
	    lock_ReleaseWrite(&ftserver_translock);
	    icl_Trace4(iclSetP, FTSERVER_TRACE_EXIT_FTSERVER_NEW_TRANS,
		       ICL_TYPE_LONG, 1, ICL_TYPE_POINTER, 0,
		       ICL_TYPE_LONG, 0, ICL_TYPE_LONG, *codep);
	    return (struct ftserver_trans *) 0;
	}
    }
    transp = (struct ftserver_trans *) osi_Alloc(sizeof(struct ftserver_trans));
    if (!transp) {
	*codep = ENOMEM;
	lock_ReleaseWrite(&ftserver_translock);
	icl_Trace4(iclSetP, FTSERVER_TRACE_EXIT_FTSERVER_NEW_TRANS,
		   ICL_TYPE_LONG, 2, ICL_TYPE_POINTER, 0,
		   ICL_TYPE_LONG, 0, ICL_TYPE_LONG, *codep);
	return (struct ftserver_trans *) 0;
    }
    bzero((char *)transp, sizeof(struct ftserver_trans));
    transp->t_volId = *volIdp;
    transp->t_aggrId = aggrId;
    transp->t_refCount = 1;
    transp->t_vDesc = -1;
    transp->t_transId = ftserver_transCounter++;
    transp->t_accError = inCode;

#if !defined(OSF_NO_SOCKET_DUMP)
    transp->t_srvrSockP = (void *)0;
    transp->t_clntSockP = (void *)0;
#endif /* OSF_NO_SOCKET_DUMP */

    transp->t_next = ftserver_trans;
    ftserver_trans = transp;
    osi_GetTime((struct timeval *)&transp->t_creationTime);
    transp->t_lastTime = transp->t_creationTime.sec;
    if (verbose > 1)
	dce_svc_printf(TSV_S_TRANS_NEWTRANS_MSG, AFS_HGETBOTH(*volIdp), aggrId,
		       transp->t_transId, transp->t_refCount);
    icl_Trace4(iclSetP, FTSERVER_TRACE_EXIT_FTSERVER_NEW_TRANS,
	       ICL_TYPE_LONG, 3, ICL_TYPE_POINTER, transp,
	       ICL_TYPE_LONG, transp->t_transId, ICL_TYPE_LONG, *codep);
    lock_ReleaseWrite(&ftserver_translock);
    return transp;
}


struct ftserver_trans *ftserver_FindTrans(transId)
    register long transId;
{
    register struct ftserver_trans *transp;
    afs_hyper_t zeroHyper;

    icl_Trace1(iclSetP, FTSERVER_TRACE_ENTER_FTSERVER_FIND_TRANS,
	       ICL_TYPE_LONG, transId);
    AFS_hzero(zeroHyper);
    lock_ObtainWrite(&ftserver_translock);
    for (transp = ftserver_trans; transp; transp = transp->t_next) {
	if (transp->t_transId == transId) {
	    transp->t_lastTime = osi_Time();
	    transp->t_refCount++;
	    if (verbose > 1) dce_svc_printf(TSV_S_TRANS_FINDTRANS_MSG, transId,
				AFS_HGETBOTH(transp->t_volId),
				transp->t_aggrId,
				transp->t_refCount);
	    icl_Trace4(iclSetP, FTSERVER_TRACE_EXIT_FTSERVER_FIND_TRANS,
		       ICL_TYPE_LONG, 1, ICL_TYPE_POINTER, transp,
		       ICL_TYPE_LONG, transp->t_aggrId,
		       ICL_TYPE_HYPER, &transp->t_volId);
	    lock_ReleaseWrite(&ftserver_translock);
	    return transp;
	}
    }
    lock_ReleaseWrite(&ftserver_translock);
    icl_Trace4(iclSetP, FTSERVER_TRACE_EXIT_FTSERVER_FIND_TRANS,
	       ICL_TYPE_LONG, 2, ICL_TYPE_POINTER, 0,
	       ICL_TYPE_LONG, 0, ICL_TYPE_HYPER, &zeroHyper);
    if (verbose > 1) dce_svc_printf(TSV_S_TRANS_FINDTRANS_NULL_MSG, transId);
    return (struct ftserver_trans *) 0;
}


struct ftserver_trans *ftserver_FindTransById(volIdp)
    afs_hyper_t *volIdp;
{
    register struct ftserver_trans *transp;

    lock_ObtainWrite(&ftserver_translock);
    for (transp = ftserver_trans; transp; transp = transp->t_next) {
	if (AFS_hsame(transp->t_volId, *volIdp)) {
	    transp->t_lastTime = osi_Time();
	    transp->t_refCount++;
	    lock_ReleaseWrite(&ftserver_translock);
	    return transp;
	}
    }
    lock_ReleaseWrite(&ftserver_translock);
    return (struct ftserver_trans *) 0;
}


struct ftserver_trans *ftserver_FindTransByDesc(vDesc)
    long vDesc;
{
    register struct ftserver_trans *transp;

    lock_ObtainWrite(&ftserver_translock);
    for (transp = ftserver_trans; transp; transp = transp->t_next) {
	if (transp->t_vDesc == vDesc) {
	    transp->t_lastTime = osi_Time();
	    transp->t_refCount++;
	    lock_ReleaseWrite(&ftserver_translock);
	    return transp;
	}
    }
    lock_ReleaseWrite(&ftserver_translock);
    return (struct ftserver_trans *) 0;
}


/*
 * delete transaction if refcount == 1, otherwise queue delete for later.
 *
 * If locked is set it must have called under a locked ftserver_translock.
 */
ftserver_DeleteTrans(atransp, locked)
    register struct ftserver_trans *atransp;
    long locked;
{
    register struct ftserver_trans *transp, **transpp;
    long error;

    icl_Trace4(iclSetP, FTSERVER_TRACE_ENTER_FTSERVER_DELETE_TRANS,
	       ICL_TYPE_LONG, atransp->t_transId,
	       ICL_TYPE_LONG, atransp->t_aggrId,
	       ICL_TYPE_HYPER, &atransp->t_volId,
	       ICL_TYPE_LONG, atransp->t_vDesc);
    icl_Trace1(iclSetP, FTSERVER_TRACE_ENTER_FTSERVER_DELETE_TRANS2,
	       ICL_TYPE_LONG, locked);
    if (!locked)
	lock_ObtainWrite(&ftserver_translock);	
    if (atransp->t_refCount > 1) {
	/*
	 * Someone else is using it now; do a delayed delete.
	 */
	atransp->t_refCount--;
	atransp->t_flags |= FTSERVER_FLAGS_DELETED;
	if (verbose > 1) dce_svc_printf(TSV_S_TRANS_DELTRANS_MSG, atransp->t_transId, atransp->t_refCount);
	if (!locked)
	    lock_ReleaseWrite(&ftserver_translock);
	icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_FTSERVER_DELETE_TRANS,
		   ICL_TYPE_LONG, 1, ICL_TYPE_LONG, 0);
	return 0;
    }
    transpp = &ftserver_trans;
    for (transp = *transpp; transp; transpp = &transp->t_next, transp = *transpp) {
	if (transp == atransp) {
	    if (verbose > 1) dce_svc_printf(TSV_S_TRANS_DELTRANS_DELETING_MSG, atransp->t_transId);
	    *transpp = transp->t_next;

#if !defined(OSF_NO_SOCKET_DUMP)
	    if (transp->t_srvrSockP) {
		termSrvrSock((srvrSock_t *)transp->t_srvrSockP);
	    }
	    if (transp->t_clntSockP) {
		termClntSock((clntSock_t *)transp->t_clntSockP);
	    }
#endif /* OSF_NO_SOCKET_DUMP */

	    osi_Free(transp, sizeof(struct ftserver_trans));
	    if (!locked)
		lock_ReleaseWrite(&ftserver_translock);
	    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_FTSERVER_DELETE_TRANS,
		       ICL_TYPE_LONG, 2, ICL_TYPE_LONG, 0);
	    return 0;
	}
    }
    if (verbose > 1) dce_svc_printf(TSV_S_TRANS_DELTRANS_FAILED_MSG, atransp->t_transId, atransp->t_refCount);
    if (!locked)
	lock_ReleaseWrite(&ftserver_translock);
    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_FTSERVER_DELETE_TRANS,
	       ICL_TYPE_LONG, 3, ICL_TYPE_LONG, -1);
    return -1;
}


/*
 * Decrement reference count for trans and do delayed deletes, if necessary.
 */
ftserver_PutTrans(transp)
    register struct ftserver_trans *transp;
{
    int code;

    icl_Trace4(iclSetP, FTSERVER_TRACE_ENTER_FTSERVER_PUT_TRANS,
	       ICL_TYPE_LONG, transp->t_transId,
	       ICL_TYPE_LONG, transp->t_aggrId,
	       ICL_TYPE_HYPER, &transp->t_volId,
	       ICL_TYPE_LONG, transp->t_vDesc);

    lock_ObtainWrite(&ftserver_translock);	
    if (transp->t_refCount == 0){ 
	lock_ReleaseWrite(&ftserver_translock);	
	code = FTSERVER_ERR_TRELE;
	icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_FTSERVER_PUT_TRANS,
		   ICL_TYPE_LONG, 1, ICL_TYPE_LONG, code);
	return code;
    }
    transp->t_lastTime = osi_Time();
    if ((transp->t_refCount == 1) && (transp->t_flags & FTSERVER_FLAGS_DELETED)) {
	if (verbose > 1) dce_svc_printf(TSV_S_TRANS_PUTTRANS_MSG, transp->t_transId, transp->t_refCount);
	ftserver_DeleteTrans(transp, 1);
	lock_ReleaseWrite(&ftserver_translock);	
	return 0;
    }
    transp->t_refCount--;
    if (verbose > 1) dce_svc_printf(TSV_S_TRANS_PUTTRANS_COUNT_MSG, transp->t_transId, transp->t_refCount);
    lock_ReleaseWrite(&ftserver_translock);	
    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_FTSERVER_PUT_TRANS,
	       ICL_TYPE_LONG, 2, ICL_TYPE_LONG, 0);
    return 0;
}


#define	OLDTRANSTIME	    (5*60)	    /* seconds */
#define	OLDTRANSWARN	    (3*60)     /* seconds */

/*
 * look for old transactions and delete them
 */
ftserver_GCTrans()
{
    register struct ftserver_trans *transp, *ntransp;
    long now;

    icl_Trace0(iclSetP, FTSERVER_TRACE_ENTER_FTSERVER_GC_TRANS);
    lock_ObtainWrite(&ftserver_translock);
    now = osi_Time();
    for (transp = ftserver_trans; transp; transp = ntransp) {
	ntransp = transp->t_next;
	if (transp->t_lastTime + OLDTRANSWARN < now) {
	    Log(tsv_s_trans_log_trans_seconds, transp, transp->t_transId,
		AFS_HGETBOTH(transp->t_volId), transp->t_aggrId,
		now - transp->t_lastTime, transp->t_refCount);
	    Log(tsv_s_trans_log_trans_desc, transp, transp->t_vDesc,
		transp->t_lastTime, transp->t_creationTime.sec,
		transp->t_states, transp->t_accStatus, transp->t_accError);
	}
	if (transp->t_refCount > 0)
	    continue;
	if (transp->t_lastTime + OLDTRANSTIME < now) {
	    Log(tsv_s_trans_log_trans_id, transp, transp->t_transId,
		AFS_HGETBOTH(transp->t_volId), transp->t_aggrId,
		now - transp->t_lastTime, transp->t_refCount);
	    transp->t_refCount++;
	    ftserver_DeleteTrans(transp, 1);
	    ftserver_transGC++;
	}
    }
    lock_ReleaseWrite(&ftserver_translock);
    icl_Trace1(iclSetP, FTSERVER_TRACE_EXIT_FTSERVER_GC_TRANS,
	       ICL_TYPE_LONG, 0);
    return 0;
}

ftserver_SwapTransStates(transAp, transBp)
    register struct ftserver_trans *transAp;
    register struct ftserver_trans *transBp;
{
/* Swap transaction state after swapping IDs/identities on the disk. */
    unsigned long tempULong;
    afs_hyper_t tempHyper;
    void *tempPtr;

    icl_Trace4(iclSetP, FTSERVER_TRACE_ENTER_FTSERVER_SWAP_TRANS_STATES,
	       ICL_TYPE_LONG, transAp->t_transId,
	       ICL_TYPE_LONG, transAp->t_aggrId,
	       ICL_TYPE_HYPER, &transAp->t_volId,
	       ICL_TYPE_LONG, transAp->t_vDesc);
    icl_Trace4(iclSetP, FTSERVER_TRACE_ENTER_FTSERVER_SWAP_TRANS_STATES2,
	       ICL_TYPE_LONG, transBp->t_transId,
	       ICL_TYPE_LONG, transBp->t_aggrId,
	       ICL_TYPE_HYPER, &transBp->t_volId,
	       ICL_TYPE_LONG, transBp->t_vDesc);
    lock_ObtainWrite(&ftserver_translock);
    tempULong = transAp->t_aggrId;
    transAp->t_aggrId = transBp->t_aggrId;
    transBp->t_aggrId = tempULong;
    tempULong = transAp->t_states;
    transAp->t_states = transBp->t_states;
    transBp->t_states = tempULong;
    tempULong = transAp->t_accStatus;
    transAp->t_accStatus = transBp->t_accStatus;
    transBp->t_accStatus = tempULong;
    tempULong = transAp->t_accError;
    transAp->t_accError = transBp->t_accError;
    transBp->t_accError = tempULong;
    tempULong = transAp->t_flags;
    transAp->t_flags = transBp->t_flags;
    transBp->t_flags = tempULong;
    tempHyper = transAp->t_baseType;
    transAp->t_baseType = transBp->t_baseType;
    transBp->t_baseType = tempHyper;

#if !defined(OSF_NO_SOCKET_DUMP)
    tempPtr = transAp->t_srvrSockP;
    transAp->t_srvrSockP = transBp->t_srvrSockP;
    transBp->t_srvrSockP = tempPtr;
    tempPtr = transAp->t_clntSockP;
    transAp->t_clntSockP = transBp->t_clntSockP;
    transBp->t_clntSockP = tempPtr;
#endif /* OSF_NO_SOCKET_DUMP */

    lock_ReleaseWrite(&ftserver_translock);
    icl_Trace1(iclSetP, FTSERVER_TRACE_EXIT_FTSERVER_SWAP_TRANS_STATES,
	       ICL_TYPE_LONG, 0);
    return 0;
}

void ftserver_KeepTransAlive(transp, Now)
    register struct ftserver_trans *transp;
    register unsigned long Now;
{/* Note activity for a transaction. */
    /* Mostly this exists to reduce garbage being logged. */
    lock_ObtainWrite(&ftserver_translock);
    if (Now == 0) Now = osi_Time();
    transp->t_lastTime = Now;
    lock_ReleaseWrite(&ftserver_translock);
    /* Yield on this server thread to prevent starvation. */
    ftutil_ThreadYield();
}
