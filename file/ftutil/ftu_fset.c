/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ftu_fset.c,v $
 * Revision 1.1.24.1  1996/10/02  17:48:38  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:38:33  damon]
 *
 * $EndLog$
 */

/*
 * (C) Copyright 1996, 1990 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

#include <sys/types.h>

#include <dcedfs/stds.h>
#include <dcedfs/bomb.h>
#include <dcedfs/common_data.h>
#include <dcedfs/syscall.h>
#include <dcedfs/vol_init.h>
#include <dcedfs/vol_errs.h>
#include <dcedfs/pipe.h>
#include <dcedfs/compat.h>		/* dfs_dceErrTxt */

#include <ftutil.h>
#include <ftu_private.h>
#include <ftu_syscalls.h>
#include <ftu_trace.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsftlmac.h>
#include <dfsftlsvc.h>
#include <dfsftlmsg.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/ftutil/ftu_fset.c,v 1.1.24.1 1996/10/02 17:48:38 damon Exp $")

/*
 * The functionality of each of the following exported routines is described
 * in the header file.  The comments below only give implementation
 * specifics, where necessary.
 */

/*
 * Private types
 */
#define MAX_KA_DESCS	2
typedef struct keepAlive {
    unsigned		count;
    ftu_fsetDesc_t	desc[MAX_KA_DESCS];

    ftu_kaProc_t	proc;
    void*		arg;
}	keepAlive_t;

/*
 * Forward declarations
 */

/*
 * DestroyFset()
 *	Destroys a fileset.  This routine takes an optional `backDesc'
 *	parameter (set to -1 if it doesn't apply) that allows routines
 *	like ftu_FsetClone() to make use of it to clean up when an error
 *	occurs.  The  clone routine will already have the victim's backing
 *	fileset open, so it passes in the descriptor.
 */
PRIVATE long DestroyFset _TAKES((
    IN ftu_fsetDesc_t	fsetDesc,
    IN ftu_fsetDesc_t	backDesc,
    IN ftu_kaProc_t	kaProc,
    IN void*		kaArg
));

/*
 * FixChain()
 */
PRIVATE long FixChain _TAKES((
    IN struct vol_status*	fsetStatusP,
    IN vol_statusDesc_t*	statusArray,
    INOUT unsigned*		numStatusDescsP
));

/*
 * GetAggrType()
 */
PRIVATE long GetAggrType _TAKES((
    IN ftu_aggrId_t	aggrId,
    OUT afs_hyper_t*	aggrTypeP
));

/*
 * KeepDescsAlive()
 */
PRIVATE void KeepDescsAlive _TAKES((
    IN keepAlive_t*	kaP
));

/*
 * RecloneIntoAdjacentFset()
 *	Reclones into the closest backing fileset.
 */
PRIVATE long RecloneIntoAdjacentFset _TAKES((
    IN ftu_aggrId_t		aggrId,
    IN ftu_fsetDesc_t		fsetDesc,
    IN struct vol_status*	fsetStatusP,
    IN ftu_fsetId_t*		backingId,
    IN ftu_kaProc_t		kaProc,
    IN void*			kaArg
));

/*
 * RecloneIntoDistantFset()
 *	Reclones into a fileset that is not the closest backing fileset.
 */
PRIVATE long RecloneIntoDistantFset _TAKES((
    IN ftu_aggrId_t	aggrId,
    IN ftu_fsetDesc_t	fsetDesc,
    IN ftu_fsetId_t*	backingId,
    IN ftu_kaProc_t	kaProc,
    IN void*		kaArg
));


/*
 * Utilities
 */
#define COMMIT_STATUS(fsetDesc)		\
    ftu_FsetSync((fsetDesc), FTU_FSET_SYNC_COMMIT_STATUS)

#define COMMIT_META(fsetDesc)		\
    ftu_FsetSync((fsetDesc), FTU_FSET_SYNC_COMMIT_META)

#define KA_ADD(ka, fsetDesc)			\
    (afsl_Assert((ka).count < MAX_KA_DESCS),	\
     (ka).desc[(ka).count++] = (fsetDesc))

#define KA_INIT(ka, fsetDesc, kaProc, kaArg)	\
    ((ka).count = 1,				\
     (ka).desc[0] = (fsetDesc),			\
     (ka).proc = (kaProc),			\
     (ka).arg = (kaArg))

#define PROBE_DESC(fsetDesc)		\
    MACRO_BEGIN				\
	if ((fsetDesc) != -1)		\
	    (void)VOL_PROBE(fsetDesc);	\
    MACRO_END

#define STAT_HIGH_ID(fsetStatus) 	HIGH_ID((fsetStatus).vol_st.volId)

#define STAT_LOW_ID(fsetStatus)		LOW_ID((fsetStatus).vol_st.volId)



/******************************************************************************
 * Fileset routines
 *****************************************************************************/

/*
 * ftu_FsetGetStatus()
 *	Just a wrapper for the system call.
 */
EXPORT long
ftu_FsetGetStatus(fsetDesc, fsetStatusP)
    IN ftu_fsetDesc_t		fsetDesc;
    OUT struct vol_status*	fsetStatusP;
{
    long	code;

    icl_Trace1(_ftu_iclSetP, FTU_TRACE_ENTER_FSET_GET_STATUS,
	       ICL_TYPE_LONG, fsetDesc);

    code = VOL_GETSTATUS(fsetDesc, fsetStatusP);

    if (code)
	ftu_svc_LogPrintf(ftl_s_ff_status_err,
			  fsetDesc, code, dfs_dceErrTxt(code));
    icl_Trace4(_ftu_iclSetP, FTU_TRACE_EXIT_FSET_GET_STATUS,
	       ICL_TYPE_LONG, code,
	       ICL_TYPE_HYPER, &fsetStatusP->vol_st.volId,
	       ICL_TYPE_HYPER, &fsetStatusP->vol_st.parentId,
	       ICL_TYPE_LONG, fsetStatusP->vol_st.accStatus);
    icl_Trace4(_ftu_iclSetP, FTU_TRACE_EXIT_FSET_GET_STATUS2,
	       ICL_TYPE_LONG, fsetStatusP->vol_st.accError,
	       ICL_TYPE_STRING, dfs_dceErrTxt(fsetStatusP->vol_st.accError),
	       ICL_TYPE_LONG, fsetStatusP->vol_st.states,
	       ICL_TYPE_STRING, fsetStatusP->vol_st.volName);
    return code;
}	/* ftu_FsetGetStatus() */


/*
 * ftu_FsetSetStatus()
 *	Just a wrapper for the system call.
 */
EXPORT long
ftu_FsetSetStatus(fsetDesc, fsetMask, fsetStatusP)
    IN ftu_fsetDesc_t		fsetDesc;
    IN u_long			fsetMask;
    IN struct vol_status*	fsetStatusP;
{
    long	code;

    icl_Trace4(_ftu_iclSetP, FTU_TRACE_ENTER_FSET_SET_STATUS,
	       ICL_TYPE_LONG, fsetDesc, ICL_TYPE_LONG, fsetMask,
	       ICL_TYPE_HYPER, &fsetStatusP->vol_st.volId,
	       ICL_TYPE_LONG, fsetStatusP->vol_st.states);

    /* 4th parameter is for more mask bits, but it is unused so far */
    code = VOL_SETSTATUS(fsetDesc, fsetMask, fsetStatusP, 0);

    if (code)
	ftu_svc_LogPrintf(ftl_s_ff_set_status_err,
			  fsetDesc, code, dfs_dceErrTxt(code));
    icl_Trace1(_ftu_iclSetP, FTU_TRACE_EXIT_FSET_SET_STATUS,
	       ICL_TYPE_LONG, code);
    return code;
}	/* ftu_FsetSetStatus() */


/*
 * ftu_FsetBulkSetStatus()
 *	Just a wrapper for the system call.
 */
EXPORT long
ftu_FsetBulkSetStatus(numStatusDescs, statusDescArray)
    IN unsigned			numStatusDescs;
    IN vol_statusDesc_t*	statusDescArray;
{
    long	code;
    int		i;

    icl_Trace1(_ftu_iclSetP, FTU_TRACE_ENTER_FSET_BULK_SET_STATUS,
	       ICL_TYPE_LONG, numStatusDescs);
    for (i = 0; i < numStatusDescs; i++)
	icl_Trace3(_ftu_iclSetP, FTU_TRACE_ENTER_FSET_BULK_SET_STATUS2,
		   ICL_TYPE_LONG, statusDescArray[i].vsd_volDesc,
		   ICL_TYPE_LONG, statusDescArray[i].vsd_mask,
		   ICL_TYPE_HYPER,
		   &statusDescArray[i].vsd_status.vol_st.volId);
    icl_Trace0(_ftu_iclSetP, FTU_TRACE_ENTER_FSET_BULK_SET_STATUS3);

    code = VOL_BULKSETSTATUS(numStatusDescs, statusDescArray);

    if (code)
	ftu_svc_LogPrintf(ftl_s_ff_set_status_fset,
			  numStatusDescs, code, dfs_dceErrTxt(code));
    icl_Trace1(_ftu_iclSetP, FTU_TRACE_EXIT_FSET_BULK_SET_STATUS,
	       ICL_TYPE_LONG, code);
    return code;
}	/* ftu_FsetBulkSetStatus() */


/*
 * ftu_FsetSwapIds()
 *	Locate any neighbors in the low-level chains and fix things up
 *	in one fell swoop with a bulk set status.
 */
EXPORT long
ftu_FsetSwapIds(fsetDesc1, fsetDesc2)
    IN ftu_fsetDesc_t	fsetDesc1;
    IN ftu_fsetDesc_t	fsetDesc2;
{
    long		code;
    unsigned		i;
    unsigned		numStatusDescs = 2;
    struct vol_status*	status1P;
    struct vol_status*	status2P;
    vol_statusDesc_t	statusArray[6];
    ftu_fsetId_t	tmpId;
    char		tmpName[VOLNAMESIZE + 1];

    icl_Trace2(_ftu_iclSetP, FTU_TRACE_ENTER_FSET_SWAP_IDS,
	       ICL_TYPE_LONG, fsetDesc1, ICL_TYPE_LONG, fsetDesc2);

    status1P = &statusArray[0].vsd_status;
    status2P = &statusArray[1].vsd_status;

    if (code = ftu_FsetGetStatus(fsetDesc1, status1P))
	goto done;

    if (code = ftu_FsetGetStatus(fsetDesc2, status2P))
	goto done;

    statusArray[0].vsd_volDesc = fsetDesc1;
    statusArray[0].vsd_mask = VOL_STAT_VOLID|VOL_STAT_VOLNAME;
    statusArray[1].vsd_volDesc = fsetDesc2;
    statusArray[1].vsd_mask = VOL_STAT_VOLID|VOL_STAT_VOLNAME;

    tmpId = status1P->vol_st.volId;
    status1P->vol_st.volId = status2P->vol_st.volId;
    status2P->vol_st.volId = tmpId;

    bcopy(status1P->vol_st.volName, tmpName, sizeof tmpName);
    tmpName[sizeof tmpName - 1] = '\0';
    bcopy(status2P->vol_st.volName, status1P->vol_st.volName,
	  sizeof status1P->vol_st.volName);
    status1P->vol_st.volName[sizeof status1P->vol_st.volName - 1] = '\0';
    bcopy(tmpName, status2P->vol_st.volName, sizeof status2P->vol_st.volName);
    status2P->vol_st.volName[sizeof status2P->vol_st.volName - 1] = '\0';

    if (code = FixChain(status1P, statusArray, &numStatusDescs))
	goto done;

    if (code = FixChain(status2P, statusArray, &numStatusDescs))
	goto done;

    code = ftu_FsetBulkSetStatus(numStatusDescs, statusArray);

  done:
    for (i = 2; i < numStatusDescs; i++)
	if (!code)
	    code = ftu_AggrCloseFset(statusArray[i].vsd_volDesc);
        else
	    ftu_AggrAbortFsetOpen(statusArray[i].vsd_volDesc);

    if (code)
	ftu_svc_LogPrintf(ftl_s_ro_vols_restore_swap_failed,
			  fsetDesc1, fsetDesc2, code, dfs_dceErrTxt(code));
    icl_Trace1(_ftu_iclSetP, FTU_TRACE_EXIT_FSET_SWAP_IDS,
	       ICL_TYPE_LONG, code);
    return code;
}	/* ftu_FsetSwapIds() */


/*
 * FixChain()
 */
PRIVATE long
FixChain(fsetStatusP, statusArray, numStatusDescsP)
    IN struct vol_status*	fsetStatusP;
    IN vol_statusDesc_t*	statusArray;
    INOUT unsigned*		numStatusDescsP;
{
    long		code;
    vol_statusDesc_t*	curDescP;
    unsigned		i;

    if (!AFS_hiszero(fsetStatusP->vol_dy.llBackId)) {
	i = (*numStatusDescsP)++;
	curDescP = &statusArray[i];

	code = ftu_AggrOpenFset(fsetStatusP->vol_dy.aggrId,
				&fsetStatusP->vol_dy.llBackId,
				VOL_SYS_SETSTATUS, VOLERR_TRANS_SETSTATUS,
				&curDescP->vsd_volDesc);
	if (code)
	    return code;

	curDescP->vsd_mask = VOL_STAT_LLFWDID;
	curDescP->vsd_status.vol_dy.llFwdId = fsetStatusP->vol_st.volId;
    }

    if (!AFS_hiszero(fsetStatusP->vol_dy.llFwdId)) {
	i = (*numStatusDescsP)++;
	curDescP = &statusArray[i];

	code = ftu_AggrOpenFset(fsetStatusP->vol_dy.aggrId,
				&fsetStatusP->vol_dy.llFwdId,
				VOL_SYS_SETSTATUS, VOLERR_TRANS_SETSTATUS,
				&curDescP->vsd_volDesc);
	if (code)
	    return code;

	curDescP->vsd_mask = VOL_STAT_LLBACKID;
	curDescP->vsd_status.vol_dy.llBackId = fsetStatusP->vol_st.volId;
    }
}	/* FixChain() */


/*
 * ftu_FsetPushStatus()
 *	Just a wrapper for the system call.
 */
EXPORT long
ftu_FsetPushStatus(fsetDesc)
    IN ftu_fsetDesc_t	fsetDesc;
{
    long	code;

    icl_Trace1(_ftu_iclSetP, FTU_TRACE_ENTER_FSET_PUSH_STATUS,
	       ICL_TYPE_LONG, fsetDesc);

    code = VOL_PUSHSTATUS(fsetDesc);

    if (code)
	ftu_svc_LogPrintf(ftl_s_ff_push_status_err,
			  fsetDesc, code, dfs_dceErrTxt(code));
    icl_Trace1(_ftu_iclSetP, FTU_TRACE_EXIT_FSET_PUSH_STATUS,
	       ICL_TYPE_LONG, code);
    return code;
}	/* ftu_FsetPushStatus() */


/*
 * ftu_FsetSync()
 *	Just a wrapper for the system call.
 */
EXPORT long
ftu_FsetSync(fsetDesc, syncOp)
    IN ftu_fsetDesc_t	fsetDesc;
    IN int		syncOp;
{
    long	code;

    icl_Trace2(_ftu_iclSetP, FTU_TRACE_ENTER_FSET_SYNC,
	       ICL_TYPE_LONG, fsetDesc, ICL_TYPE_LONG, syncOp);

    code = VOL_SYNC(fsetDesc, syncOp);

    if (code)
	ftu_svc_LogPrintf(ftl_s_ff_sync_err,
			  fsetDesc, code, dfs_dceErrTxt(code));
    icl_Trace1(_ftu_iclSetP, FTU_TRACE_EXIT_FSET_SYNC,
	       ICL_TYPE_LONG, code);
    return code;
}	/* ftu_FsetSync() */


/*
 * ftu_FsetClone()
 *	Just a wrapper for ftu_FsetCloneWithKA()
 */
EXPORT long
ftu_FsetClone(srcDesc, destIdP, destName, destStates)
    IN ftu_fsetDesc_t	srcDesc;
    IN ftu_fsetId_t*	destIdP;
    IN char*		destName;
    IN u_long		destStates;
{
    return ftu_FsetCloneWithKA(srcDesc, destIdP, destName, destStates, 0, 0);
}	/* ftu_FsetClone() */


/*
 * ftu_FsetCloneWithKA()
 *	1. Create new fileset with status initialized from the original
 *	2. Perform an insertion into the doubly linked list formed
 *	   by the forward and backward ID fields in the fileset status area.
 *	3. Clone each file
 *	4. Swap the identities of the filesets so that the original ID
 *	   still names the read-write copy.
 */
EXPORT long
ftu_FsetCloneWithKA(srcDesc, destIdP, destName, destStates, kaProc, kaArg)
    IN ftu_fsetDesc_t	srcDesc;
    IN ftu_fsetId_t*	destIdP;
    IN char*		destName;
    IN u_long		destStates;
    IN ftu_kaProc_t	kaProc;
    IN void*		kaArg;
{
    ftu_aggrId_t	aggrId;
#ifdef DFS_BOMB
    volatile
#endif /* DFS_BOMB */
      ftu_fsetDesc_t	backingDesc = -1;
    struct vol_status*	backingStatusP;
    long		code = 0;
#ifdef DFS_BOMB
    volatile
#endif /* DFS_BOMB */
      ftu_fsetDesc_t	destDesc;
    struct vol_status*	destStatusP;
    int			didFirstSplice = 0;
    int			didSecondSpliceAndSwap = 0;
    keepAlive_t		ka;
    unsigned		numStatusDescs = 2;
    struct vol_status*	srcStatusP;
    vol_statusDesc_t	statusArray[3];
#define SRC_IDX		0
#define DEST_IDX	1
#define BACK_IDX	2
#define FN		"ftu_FsetCloneWithKA"

    icl_Trace4(_ftu_iclSetP, FTU_TRACE_ENTER_FSET_CLONE,
	       ICL_TYPE_LONG, srcDesc, ICL_TYPE_HYPER, destIdP,
	       ICL_TYPE_STRING, destName, ICL_TYPE_LONG, destStates);

    afsl_PAssertTruth(destStates & VOL_READONLY, ("Clone must be readonly"));

    srcStatusP = &statusArray[SRC_IDX].vsd_status;
    statusArray[SRC_IDX].vsd_mask = 0;
    destStatusP = &statusArray[DEST_IDX].vsd_status;
    backingStatusP = &statusArray[BACK_IDX].vsd_status;

    if (code = BOMB_EXEC(FN ".1", ftu_FsetGetStatus(srcDesc, srcStatusP)))
	goto done;

    if (!AFS_hiszero(srcStatusP->vol_dy.llFwdId)) {
	code = FTU_E_NOT_HEAD_FSET;
	goto done;
    }
    
    CALL_KA_PROC(kaProc, kaArg);

    aggrId = srcStatusP->vol_dy.aggrId;

    ftu_svc_LogPrintf(ftl_s_ff_cloning, aggrId, srcStatusP->vol_st.volName,
		  STAT_HIGH_ID(*srcStatusP), STAT_LOW_ID(*srcStatusP),
		  destName, HIGH_ID(*destIdP), LOW_ID(*destIdP));

    /*
     * Copy source status to destination status, and then change some
     * destination fields.
     */
    *destStatusP = *srcStatusP;
    destStatusP->vol_st.volId = *destIdP;
    destStatusP->vol_st.parentId = srcStatusP->vol_st.volId;
    strncpy(destStatusP->vol_st.volName, destName,
	    sizeof destStatusP->vol_st.volName - 1);
    destStatusP->vol_st.volName[sizeof destStatusP->vol_st.volName - 1] = '\0';
    destStatusP->vol_st.type = VOL_READONLY;
    destStatusP->vol_st.states &= ~(VOL_RW|VOL_IS_COMPLETE|VOL_TYPEFIELD|
				    VOL_LCLMOUNT|VOL_MOVE_TARGET|
				    VOL_MOVE_SOURCE|VOL_OFFLINE|
				    VOL_OUTOFSERVICE);
    destStatusP->vol_st.states |= VOL_ZAPME|destStates;
    destStatusP->vol_st.accError = 0;
    destStatusP->vol_st.accStatus = 0;
    destStatusP->vol_st.volMoveTimeout = 0;
    AFS_hzero(destStatusP->vol_dy.llBackId);
    AFS_hzero(destStatusP->vol_dy.llFwdId);

    /* Allow the clone to complete even if the source is over its limits */
    if (AFS_hcmp(srcStatusP->vol_dy.allocLimit,
	     srcStatusP->vol_dy.allocUsage) == -1) {
	/* Limit is less than usage, so bump the limit in the destination */

	ftu_svc_LogPrintf(ftl_s_ff_up_clone_alloc_limit, HIGH_ID(*destIdP), LOW_ID(*destIdP));
	icl_Trace4(_ftu_iclSetP, FTU_TRACE_FSET_CLONE_BUMP_LIMIT,
		   ICL_TYPE_STRING, "alloc.", ICL_TYPE_HYPER, destIdP,
		   ICL_TYPE_HYPER, &srcStatusP->vol_dy.allocUsage,
		   ICL_TYPE_HYPER, &destStatusP->vol_dy.allocLimit);

	destStatusP->vol_dy.allocLimit = srcStatusP->vol_dy.allocUsage;
    }
    if (AFS_hcmp(srcStatusP->vol_dy.visQuotaLimit,
	     srcStatusP->vol_dy.visQuotaUsage) == -1) {
	/* Limit is less than usage, so bump the limit in the destination */

	ftu_svc_LogPrintf(ftl_s_ff_up_clone_vis_limit, HIGH_ID(*destIdP), LOW_ID(*destIdP));
	icl_Trace4(_ftu_iclSetP, FTU_TRACE_FSET_CLONE_BUMP_LIMIT,
		   ICL_TYPE_STRING, "vis.", ICL_TYPE_HYPER, destIdP,
		   ICL_TYPE_HYPER, &srcStatusP->vol_dy.visQuotaUsage,
		   ICL_TYPE_HYPER, &destStatusP->vol_dy.visQuotaLimit);

	destStatusP->vol_dy.visQuotaLimit = srcStatusP->vol_dy.visQuotaUsage;
    }

    osi_GetTime(((struct timeval *)&destStatusP->vol_st.cloneTime));

    /*
     * Create and open the new fileset
     */
    code = BOMB_EXEC(FN ".2",
		     ftu_AggrCreateFsetWithStatus(aggrId, destIdP,
						  destStatusP, 0));
    if (code)
	goto done;

    code = BOMB_EXEC(FN ".3",
		     ftu_AggrOpenFset(aggrId, destIdP,
				      FTU_SYS_SYNC|VOL_SYS_DESTROY
				      |VOL_SYS_SETSTATUS|VOL_SYS_SWAPVOLIDS
				      |FTU_SYS_PUSH_STATUS|FTU_SYS_DESTROY
				      |VOL_SYS_CLONE,
				      VOLERR_TRANS_CLONE,
				      (ftu_fsetDesc_t*)&destDesc));
    if (code) {
	ftu_svc_LogPrintf(ftl_s_ff_open_failed, destStatusP->vol_st.volName,
		      code, dfs_dceErrTxt(code));
	icl_Trace2(_ftu_iclSetP, FTU_TRACE_FSET_CLONE_OPEN_FAILED,
		   ICL_TYPE_STRING, destStatusP->vol_st.volName,
		   ICL_TYPE_HYPER, &destStatusP->vol_st.volId);
	goto done;
    }

    /*
     * Some of the tests for this code simulate crashes by causing
     * exceptions to be raised at the following bomb points.  To allow these
     * tests to work without waiting for the fileset descriptor to time out,
     * we make sure that the affected filesets are closed properly, but we
     * don't do any other cleanup since our intention is to simulate a crash.
     */
#ifdef DFS_BOMB
    TRY {
#endif
	CALL_KA_PROC(kaProc, kaArg);
	PROBE_DESC(srcDesc);

	if (code = BOMB_EXEC(FN ".4", COMMIT_META(destDesc)))
	    goto destroy_dest;

	CALL_KA_PROC(kaProc, kaArg);
	PROBE_DESC(srcDesc);
	
	/*
	 * Open the original backing volume, if any, to change its
	 * forwarding link
	 */
	if (!AFS_hiszero(srcStatusP->vol_dy.llBackId)) {
	    code = BOMB_EXEC(FN ".5",
			     ftu_AggrOpenFset(aggrId,
					      &srcStatusP->vol_dy.llBackId,
					      VOL_SYS_SETSTATUS|VOL_SYS_SYNC,
					      VOLERR_TRANS_CLONE,
					      (ftu_fsetDesc_t*)&backingDesc));
	    if (code) {
		if (code == FTU_E_NO_SUCH_FSET || code == EXDEV) {
		    /* OK, but we shouldn't have a backing pointer. */
		    
		    ftu_svc_LogPrintf(ftl_s_ff_zero_bogus_id, STAT_HIGH_ID(*srcStatusP),
				  STAT_LOW_ID(*srcStatusP),
				  HIGH_ID(srcStatusP->vol_dy.llBackId),
				  LOW_ID(srcStatusP->vol_dy.llBackId));
		    icl_Trace2(_ftu_iclSetP,
			       FTU_TRACE_FSET_CLONE_BOGUS_BACKING_ID,
			       ICL_TYPE_HYPER, &srcStatusP->vol_st.volId,
			       ICL_TYPE_HYPER, &srcStatusP->vol_dy.llBackId);
		    
		    AFS_hzero(srcStatusP->vol_dy.llBackId);
		    statusArray[SRC_IDX].vsd_mask |= VOL_STAT_LLBACKID;
		    backingDesc = -1;
		    /* fall through */
		} else {
		    goto destroy_dest;
		}
	    }
	    
	    CALL_KA_PROC(kaProc, kaArg);
	    PROBE_DESC(srcDesc);
	    PROBE_DESC(destDesc);
	}
	
	/* Splice new fileset onto the end of the chain */
	statusArray[SRC_IDX].vsd_volDesc = srcDesc;
	statusArray[SRC_IDX].vsd_mask |= VOL_STAT_STATES|VOL_STAT_LLFWDID;
	srcStatusP->vol_st.states |= VOL_CLONEINPROG;
	srcStatusP->vol_dy.llFwdId = *destIdP;
	
	statusArray[DEST_IDX].vsd_volDesc = destDesc;
	statusArray[DEST_IDX].vsd_mask = VOL_STAT_LLBACKID;
	destStatusP->vol_dy.llBackId = srcStatusP->vol_st.volId;
	
	if (code = BOMB_EXEC(FN ".6", ftu_FsetBulkSetStatus(2, statusArray)))
	    goto close_backing;
	
	CALL_KA_PROC(kaProc, kaArg);
	PROBE_DESC(backingDesc);
	
	/* Remember that we spliced new fileset onto the end of the chain */
	didFirstSplice = 1;
	
	if ((code = BOMB_EXEC(FN ".7", ftu_FsetPushStatus(srcDesc))) ||
	    (code = BOMB_EXEC(FN ".8", ftu_FsetPushStatus(destDesc))))
	    goto close_backing;
	
	CALL_KA_PROC(kaProc, kaArg);
	PROBE_DESC(backingDesc);
	
	/* Call helper to do actual clone of every file */
	KA_INIT(ka, backingDesc, kaProc, kaArg);
	code = BOMB_EXEC(FN ".9",
			 ftutil_VolCloneAux(srcDesc, destDesc, NULL,
					    KeepDescsAlive, &ka));
	if (code)
	    goto close_backing;
	
	if ((code = BOMB_EXEC(FN ".10", COMMIT_META(srcDesc))) ||
	    (code = BOMB_EXEC(FN ".11", COMMIT_META(destDesc))))
	    goto close_backing;
	
	CALL_KA_PROC(kaProc, kaArg);
	PROBE_DESC(backingDesc);
	
	/*
	 * Ready for this?  We have to switch the identities of the
	 * two filesets.  The copy we started out with (which was r/w)
	 * must now become the r/o copy and the clone we created (which was
	 * r/o) must become the r/w copy.
	 *
	 * Why?  The clone operation sets the high bit in the original
	 * data block ref. in the clone.  If we then modify the original
	 * version of the data, the clone will continue to point to
	 * the new data.  Not what we want.  If we instead modify the cloned
	 * version of the data, the filesystem detects that the copy-on-write
	 * bit is set and allocates a new block - in the clone.
	 *
	 * These are exactly the semantics we want for our main r/w
	 * volume to have.  Thus, by switching identities, future
	 * operations "do the right thing."
	 *
	 * We switch the ID's here, rather than in the fldb because
	 * by switching them here we retain any network-based references
	 * to these filesets.
	 */
	
	/*
	 * srcStatusP contains the status of the destination fileset
	 * at this point; simliarly, destStatusP contains the status
	 * of the source fileset at this point.
	 *
	 * We can fix this situation by swapping the fileset ids
	 * and storing srcStatusP into the source fileset & vice-versa
	 * for destStatusP.
	 */
	statusArray[SRC_IDX].vsd_volDesc = destDesc;
	statusArray[SRC_IDX].vsd_mask = ~0;
	srcStatusP->vol_st.states &= ~VOL_CLONEINPROG;
	
	statusArray[DEST_IDX].vsd_volDesc = srcDesc;
	statusArray[DEST_IDX].vsd_mask = ~0;
	destStatusP->vol_st.states &= ~VOL_ZAPME;
	/* Set the limits back to their proper values */
	destStatusP->vol_dy.allocLimit = srcStatusP->vol_dy.allocLimit;
	destStatusP->vol_dy.visQuotaLimit = srcStatusP->vol_dy.visQuotaLimit;
	
	srcStatusP->vol_dy.llFwdId = destStatusP->vol_dy.llFwdId;
	destStatusP->vol_dy.llFwdId = srcStatusP->vol_st.volId;
	
	destStatusP->vol_dy.llBackId = srcStatusP->vol_dy.llBackId;
	srcStatusP->vol_dy.llBackId = *destIdP;
	
	if (backingDesc != -1) {
	    /*
	     * Set ONLY the llFwdId of the old backing guy to point to the
	     * new volume
	     */
	    statusArray[BACK_IDX].vsd_volDesc = backingDesc;
	    statusArray[BACK_IDX].vsd_mask = VOL_STAT_LLFWDID;
	    backingStatusP->vol_dy.llFwdId = *destIdP;
	    numStatusDescs = 3;
	}
	
	/* Do the swap */
	code = BOMB_EXEC(FN ".12",
			 ftu_FsetBulkSetStatus(numStatusDescs, statusArray));
	if (code)
	    goto close_backing;
	
	CALL_KA_PROC(kaProc, kaArg);
	
	/* Remember that we spliced clone into the middle and did the swap */
	didSecondSpliceAndSwap = 1;
	
	/*
	 * Since the ID swap and backing chain update has been accomplished, we
	 * no longer destroy the destination fileset if we encounter an error.
	 */
	
	/* Sync the result of the swap */
	if ((code = BOMB_EXEC(FN ".13", COMMIT_STATUS(srcDesc))) ||
	    (code = BOMB_EXEC(FN ".14", COMMIT_STATUS(destDesc))))
	    goto close_backing;
	
	if (backingDesc != -1 &&
	    (code = BOMB_EXEC(FN ".15", COMMIT_STATUS(backingDesc))))
	    goto close_backing;
	
	CALL_KA_PROC(kaProc, kaArg);

#ifdef DFS_BOMB
    } CATCH_ALL {
	if (backingDesc != -1)
	    ftu_AggrAbortFsetOpen(backingDesc);
    
	ftu_AggrAbortFsetOpen(destDesc);

	RERAISE;
    } ENDTRY
#endif	/* DFS_BOMB */

  close_backing:
    if (backingDesc != -1)
	if (!code)
	    code = ftu_AggrCloseFset(backingDesc);
	else
	    ftu_AggrAbortFsetOpen(backingDesc);

  destroy_dest:
    if (code) {
	if (!didFirstSplice) {
	    /*
	     * We never created any files in the destination, so we can
	     * simply destroy the empty fileset.
	     */
	    (void)BOMB_EXEC(FN ".16", VOL_DESTROY(destDesc));

	    CALL_KA_PROC(kaProc, kaArg);
	    PROBE_DESC(srcDesc);
	} else if (!didSecondSpliceAndSwap) {
	    /*
	     * Once the new fileset has been spliced onto the head of the
	     * backing chain, and before we've done the second splice, we have
	     * to call a destroy routine that knows how to clean up the
	     * modifications to the backing chain and turn off the CLONEINPROG
	     * bit.  If we've already done the second splice we leave the new
	     * clone intact.
	     */
	    (void)DestroyFset(destDesc, srcDesc, kaProc, kaArg);
	}
	ftu_AggrAbortFsetOpen(destDesc);
    } else {
	code = ftu_AggrCloseFset(destDesc);
    }

  done:
    if (code)
	ftu_svc_LogPrintf(ftl_s_ff_clone_failed, code, dfs_dceErrTxt(code));
    icl_Trace1(_ftu_iclSetP, FTU_TRACE_EXIT_FSET_CLONE,
	       ICL_TYPE_LONG, code);
    return code;
}	/* ftu_FsetCloneWithKA() */


/*
 * KeepDescsAlive()
 */
PRIVATE void
KeepDescsAlive(kaP)
    IN keepAlive_t*	kaP;
{
    unsigned	i;

    for (i = 0; i < kaP->count; i++)
	PROBE_DESC(kaP->desc[i]);

    CALL_KA_PROC(kaP->proc, kaP->arg);
}	/* KeepDescsAlive() */


/*
 * ftu_FsetReclone()
 *	Find the backing fileset, if any, and let RecloneIntoAdjacentFset()
 *	do the real work.
 */
EXPORT long
ftu_FsetReclone(fsetDesc)
    IN ftu_fsetDesc_t	fsetDesc;
{
    ftu_aggrId_t	aggrId;
    ftu_fsetDesc_t	backingDesc;
    long	 	code = 0;
    struct vol_status	fsetStatus;

    icl_Trace1(_ftu_iclSetP, FTU_TRACE_ENTER_FSET_RECLONE,
	       ICL_TYPE_LONG, fsetDesc);

    if (code = ftu_FsetGetStatus(fsetDesc, &fsetStatus))
	goto done;

    aggrId = fsetStatus.vol_dy.aggrId;

    if (AFS_hiszero(fsetStatus.vol_dy.llBackId)) {
	code = FTU_E_NO_BACKING_FSET;
	goto done;
    }

    ftu_svc_LogPrintf(ftl_s_ff_recloning, aggrId, fsetStatus.vol_st.volName,
		  STAT_HIGH_ID(fsetStatus), STAT_LOW_ID(fsetStatus),
		  HIGH_ID(fsetStatus.vol_dy.llBackId),
		  LOW_ID(fsetStatus.vol_dy.llBackId));

    code = RecloneIntoAdjacentFset(aggrId, fsetDesc, &fsetStatus,
				   &fsetStatus.vol_dy.llBackId, 0, 0);

  done:
    if (code)
	ftu_svc_LogPrintf(ftl_s_ff_recloning_err,
			  STAT_HIGH_ID(fsetStatus), STAT_LOW_ID(fsetStatus),
			  code, dfs_dceErrTxt(code));
    icl_Trace1(_ftu_iclSetP, FTU_TRACE_EXIT_FSET_RECLONE,
	       ICL_TYPE_LONG, code);
    return code;
}	/* ftu_Reclone() */


/*
 * ftu_FsetRecloneInto()
 *	Just a wrapper for ftu_FsetRecloneIntoWithKA.
 */ 
EXPORT long
ftu_FsetRecloneInto(fsetDesc, backingIdP)
    IN ftu_fsetDesc_t	fsetDesc;
    IN ftu_fsetId_t*	backingIdP;
{
    return ftu_FsetRecloneIntoWithKA(fsetDesc, backingIdP, 0, 0);
}	/* ftu_FsetRecloneInto() */


/*
 * ftu_FsetRecloneIntoWithKA()
 *	Just get the status and let helper routines do the real	work.
 */
EXPORT long
ftu_FsetRecloneIntoWithKA(fsetDesc, backingIdP, kaProc, kaArg)
    IN ftu_fsetDesc_t	fsetDesc;
    IN ftu_fsetId_t*	backingIdP;
    IN ftu_kaProc_t	kaProc;
    IN void*		kaArg;
{
    ftu_aggrId_t	aggrId;
    long		code = 0;
    struct vol_status	fsetStatus;

    icl_Trace2(_ftu_iclSetP, FTU_TRACE_ENTER_FSET_RECLONE_INTO,
	       ICL_TYPE_LONG, fsetDesc, ICL_TYPE_HYPER, backingIdP);

    if (code = ftu_FsetGetStatus(fsetDesc, &fsetStatus))
	goto done;

    CALL_KA_PROC(kaProc, kaArg);

    aggrId = fsetStatus.vol_dy.aggrId;

    ftu_svc_LogPrintf(ftl_s_ff_recloning_into, aggrId, fsetStatus.vol_st.volName,
		  STAT_HIGH_ID(fsetStatus), STAT_LOW_ID(fsetStatus),
		  HIGH_ID(*backingIdP), LOW_ID(*backingIdP));

    if (AFS_hsame(*backingIdP, fsetStatus.vol_dy.llBackId))
	/* Reclone into our immediate predecessor */
	code = RecloneIntoAdjacentFset(aggrId, fsetDesc, &fsetStatus,
				       backingIdP, kaProc, kaArg);
    else
	/* Reclone into a more distant ancestor */
	code = RecloneIntoDistantFset(aggrId, fsetDesc, backingIdP,
				      kaProc, kaArg);

  done:
    if (code)
	ftu_svc_LogPrintf(ftl_s_ff_reclone_failed,
			  STAT_HIGH_ID(fsetStatus), STAT_LOW_ID(fsetStatus),
			  code, dfs_dceErrTxt(code));
    icl_Trace1(_ftu_iclSetP, FTU_TRACE_EXIT_FSET_RECLONE_INTO,
	       ICL_TYPE_LONG, code);
    return code;
}	/* ftu_FsetRecloneIntoWithKA() */


/*
 * RecloneIntoAdjacentFset()
 *	Private helper function for the two reclone routines above.
 *	First do the reclone, and then update the backing fileset's status
 *	to match the read/write's
 */
PRIVATE long
RecloneIntoAdjacentFset(aggrId, fsetDesc, fsetStatusP, backingIdP,
			kaProc, kaArg)
    IN ftu_aggrId_t		aggrId;
    IN ftu_fsetDesc_t		fsetDesc;
    IN struct vol_status*	fsetStatusP;
    IN ftu_fsetId_t*		backingIdP;
    IN ftu_kaProc_t		kaProc;
    IN void*			kaArg;
{
    ftu_fsetDesc_t	backingDesc;
    struct vol_status	backingStatus;
    long		code;
    keepAlive_t		ka;
    u_long		mask = 0;
    afs_hyper_t	newAllocLimit;
    afs_hyper_t	newVisLimit;
    afsTimeval		origCloneTime;
#undef FN
#define FN "RecloneIntoAdjacentFset"

    code = BOMB_EXEC(FN ".1",
		     ftu_AggrOpenFset(aggrId, backingIdP,
				      FTU_SYS_GET_STATUS|VOL_SYS_RECLONE
				      |FTU_SYS_SYNC|FTU_SYS_SET_STATUS,
				      VOLERR_TRANS_RECLONE,
				      &backingDesc));
    if (code)
	return code;

    CALL_KA_PROC(kaProc, kaArg);
    PROBE_DESC(fsetDesc);

    code = BOMB_EXEC(FN ".2", ftu_FsetGetStatus(backingDesc, &backingStatus));
    if (code) {
	ftu_AggrAbortFsetOpen(backingDesc);
	return code;
    }

    CALL_KA_PROC(kaProc, kaArg);
    PROBE_DESC(fsetDesc);

    /*
     * Set the limits so that reclone never runs out of space.  These will be
     * reset to their proper values by the set-status below.
     */
    newAllocLimit = backingStatus.vol_dy.allocUsage;
    AFS_hadd(newAllocLimit, fsetStatusP->vol_dy.allocUsage);
    if (AFS_hcmp(backingStatus.vol_dy.allocLimit, newAllocLimit) == -1) {
	/* Limit is less than potential usage, so bump the backing limit */

	ftu_svc_LogPrintf(ftl_s_ff_up_reclone_alloc_limit, HIGH_ID(*backingIdP), LOW_ID(*backingIdP));
	icl_Trace4(_ftu_iclSetP,
		   FTU_TRACE_RECLONE_INTO_ADJACENT_FSET_BUMP_LIMIT,
		   ICL_TYPE_STRING, "alloc.", ICL_TYPE_HYPER, backingIdP,
		   ICL_TYPE_HYPER, &newAllocLimit,
		   ICL_TYPE_HYPER, &backingStatus.vol_dy.allocLimit);

	backingStatus.vol_dy.allocLimit = newAllocLimit;
	mask |= VOL_STAT_ALLOCLIMIT;
    }

    newVisLimit = backingStatus.vol_dy.visQuotaUsage;
    AFS_hadd(newVisLimit, fsetStatusP->vol_dy.visQuotaUsage);
    if (AFS_hcmp(backingStatus.vol_dy.visQuotaLimit, newVisLimit) == -1) {
	/* Limit is less than potential usage, so bump the backing limit */

	ftu_svc_LogPrintf(ftl_s_ff_up_reclone_vis_limit, HIGH_ID(*backingIdP), LOW_ID(*backingIdP));
	icl_Trace4(_ftu_iclSetP,
		   FTU_TRACE_RECLONE_INTO_ADJACENT_FSET_BUMP_LIMIT,
		   ICL_TYPE_STRING, "vis.", ICL_TYPE_HYPER, backingIdP,
		   ICL_TYPE_HYPER, &newVisLimit,
		   ICL_TYPE_HYPER, &backingStatus.vol_dy.visQuotaLimit);

	backingStatus.vol_dy.visQuotaLimit = newVisLimit;
	mask |= VOL_STAT_VISLIMIT;
    }

    if (mask) {
	code = BOMB_EXEC(FN ".3",
			 ftu_FsetSetStatus(backingDesc, mask, &backingStatus));
	if (code) {
	    ftu_AggrAbortFsetOpen(backingDesc);
	    return code;
	}

	CALL_KA_PROC(kaProc, kaArg);
	PROBE_DESC(fsetDesc);
    }

    code = BOMB_EXEC(FN ".4",
		     ftutil_VolReCloneAux(fsetDesc, backingDesc, NULL,
					  kaProc, kaArg));
    if (code) {
	ftu_AggrAbortFsetOpen(backingDesc);
	return code;
    }

    if ((code = BOMB_EXEC(FN ".5", COMMIT_META(fsetDesc))) ||
	(code = BOMB_EXEC(FN ".6", COMMIT_META(backingDesc)))) {
	ftu_AggrAbortFsetOpen(backingDesc);
	return code;
    }

    CALL_KA_PROC(kaProc, kaArg);

    /*
     * Create a new status out of the old one:
     * Copy selected fields from the (former) source volume status and
     * set them in the dest.
     * We tuck away the cloneTime and restore it to minimize unexpected
     * side-effects.
     */
    origCloneTime = fsetStatusP->vol_st.cloneTime;
    osi_GetTime(((struct timeval *)&fsetStatusP->vol_st.cloneTime));
    code = BOMB_EXEC(FN ".7",
		     ftu_FsetSetStatus(backingDesc,
				       VOL_STAT_OWNER|VOL_STAT_UPDATEDATE
				       |VOL_STAT_ACCESSDATE|VOL_STAT_CLONETIME
				       |VOL_STAT_ALLOCLIMIT|VOL_STAT_VISLIMIT
				       |VOL_STAT_MINQUOTA| VOL_STAT_INDEX
				       |VOL_STAT_VVCURRTIME
				       |VOL_STAT_VVPINGCURRTIME,
				       fsetStatusP));
    fsetStatusP->vol_st.cloneTime = origCloneTime;
    if (code) {
	ftu_AggrAbortFsetOpen(backingDesc);
	return code;
    }

    CALL_KA_PROC(kaProc, kaArg);
    PROBE_DESC(fsetDesc);

    if (code = BOMB_EXEC(FN ".8", COMMIT_STATUS(backingDesc))) {
	ftu_AggrAbortFsetOpen(backingDesc);
	return code;
    }

    CALL_KA_PROC(kaProc, kaArg);
    PROBE_DESC(fsetDesc);

    if (code = ftu_AggrCloseFset(backingDesc))
	return code;

    CALL_KA_PROC(kaProc, kaArg);
    PROBE_DESC(fsetDesc);

    return 0;
}	/* RecloneIntoAdjacentFset() */


/*
 * RecloneIntoDistantFset()
 *	Private helper function for ftu_FsetRecloneInto().  This routine
 * 	accomplishes a "reclone" into a distant backing fileset by deleting
 *	the backing fileset and doing a fresh clone.
 *
 *	We don't worry about syncing out changes here since ftu_FsetDestroy()
 *	and ftu_FsetClone() both do their own syncing.
 */
PRIVATE long
RecloneIntoDistantFset(aggrId, fsetDesc, backingIdP, kaProc, kaArg)
    IN ftu_aggrId_t	aggrId;
    IN ftu_fsetDesc_t	fsetDesc;
    IN ftu_fsetId_t*	backingIdP;
    IN ftu_kaProc_t	kaProc;
    IN void*		kaArg;
{
    ftu_fsetDesc_t	backingDesc;
    u_long		backingStates;
    struct vol_status	backingStatus;
    long		code;
    keepAlive_t		ka;

    code = ftu_AggrOpenFset(aggrId, backingIdP,
			    FTU_SYS_GET_STATUS|FTU_SYS_DESTROY,
			    VOLERR_TRANS_RECLONE,
			    &backingDesc);
    if (code)
	return code;

    KA_INIT(ka, fsetDesc, kaProc, kaArg);
    KeepDescsAlive(&ka);

    /* Save the original status for use with the new clone */
    if (code = ftu_FsetGetStatus(backingDesc, &backingStatus)) {
	ftu_AggrAbortFsetOpen(backingDesc);
	return code;
    }

    KeepDescsAlive(&ka);

    /* Destroy the backing fileset */
    code = ftu_FsetDestroyWithKA(backingDesc,
				 (ftu_kaProc_t)KeepDescsAlive, &ka);
    if (code) {
	ftu_AggrAbortFsetOpen(backingDesc);
	return code;
    }

    KeepDescsAlive(&ka);

    if (code = ftu_AggrCloseFset(backingDesc))
	return code;

    KeepDescsAlive(&ka);

    /* Re-use the part of the states field */
    backingStates = backingStatus.vol_st.states & (VOL_TYPEFIELD|VOL_TYPEBITS
						   |VOL_REPSERVER_MGD|VOL_REPFIELD
						   |VOL_NOEXPORT);

    /* Make a fresh clone */
    code = ftu_FsetCloneWithKA(fsetDesc, backingIdP,
			       backingStatus.vol_st.volName, backingStates,
			       kaProc, kaArg);
    if (code)
	return code;

    return 0;
}	/* RecloneIntoDistantFset() */


/*
 * ftu_FsetDestroy()
 *	Just a wrapper for ftu_FsetDestroyWithKA().
 */
EXPORT long
ftu_FsetDestroy(fsetDesc)
    IN ftu_fsetDesc_t	fsetDesc;
{
    return ftu_FsetDestroyWithKA(fsetDesc, 0, 0);
}	/* ftu_FsetDestroy() */

/*
 * ftu_FsetDestroyWithKA()
 *	Just a wrapper for DestroyFset().
 */
EXPORT long
ftu_FsetDestroyWithKA(fsetDesc, kaProc, kaArg)
    IN ftu_fsetDesc_t	fsetDesc;
    IN ftu_kaProc_t	kaProc;
    IN void*		kaArg;
{
    return DestroyFset(fsetDesc, -1, kaProc, kaArg);
}	/* ftu_FsetDestroyWithKA() */


/*
 * DestroyFset()
 *	Helper used by ftu_FsetDestroy() and ftu_FsetClone().
 *
 *	1. If there is a forward fileset, unclone the victim into it.
 *	2. Splice the victim out of the chain, but leave its forward and
 *	   backward pointers intact:
 *
 *                   /----<-------<---------<----\
 *         _______  |            _______          |  _______
 *         |     |<--            |\   /|          ---|     |
 *         |     |               | \ / |------>----->|     |
 *         |     |               |  /  |             |     |
 *         |     |<-------<------| / \ |             |     |
 *         |     |---            |/   \|          -->|     |
 *         -------  |            -------          |  -------
 *                   \---->------->--------->----/
 *
 *	   This connectivity allows us to restart this operation by
 *	   allowing us to always find the backward neighbor in step 4.
 *	   If instead, we wait until the victim is destroyed to splice
 *	   it out, we would have an intermediate state where the neighboring
 *	   filesets pointed to a non-existent fileset, a clearly bogus
 *	   situation.
 *
 *	3. Destroy the victim.
 *	4. Unset the CLONEINPROG bit on the backward neighbor.
 */
PRIVATE long
DestroyFset(fsetDesc, backDesc, kaProc, kaArg)
    IN ftu_fsetDesc_t	fsetDesc;
    IN ftu_fsetDesc_t	backDesc;
    IN ftu_kaProc_t	kaProc;
    IN void*		kaArg;
{
    ftu_aggrId_t	aggrId;
    ftu_fsetId_t	backId;
    long		code = 0;
    struct vol_status	fsetStatus;
    ftu_fsetDesc_t	fwdDesc = -1;
    ftu_fsetId_t	fwdId;
    ftu_fsetDesc_t	localBackDesc = -1;
    keepAlive_t		ka;
    unsigned		nextIdx = 0;
    vol_statusDesc_t	statusArray[2];
#undef FN
#define FN	"DestroyFset"

    icl_Trace2(_ftu_iclSetP, FTU_TRACE_ENTER_DESTROY_FSET,
	       ICL_TYPE_LONG, fsetDesc, ICL_TYPE_LONG, backDesc);

    /*
     * This volume might be the backing volume for another volume.
     * If this is true, we have to unclone that other volume before we
     * can proceed.
     */
    if (code = BOMB_EXEC(FN ".1", ftu_FsetGetStatus(fsetDesc, &fsetStatus)))
	goto done;

    CALL_KA_PROC(kaProc, kaArg);
    PROBE_DESC(backDesc);

    aggrId = fsetStatus.vol_dy.aggrId;

    ftu_svc_LogPrintf(ftl_s_ff_destroying, aggrId, fsetStatus.vol_st.volName,
		  STAT_HIGH_ID(fsetStatus), STAT_LOW_ID(fsetStatus));

    /* Refuse to destroy a locally-mounted fileset */
    if (fsetStatus.vol_st.states & VOL_LCLMOUNT) {
	code = FTU_E_LOCAL_MOUNT;
	goto done;
    }

    fwdId = fsetStatus.vol_dy.llFwdId;
    backId = fsetStatus.vol_dy.llBackId;

    /* Ensure that there is a backId if a valid backDesc was passed in */
    afsl_PAssertTruth(backDesc == -1 || !AFS_hiszero(backId),
		      ("There is no evidence of a backing fileset"));

    /* Open backward-linked fileset, if any */
    if (!AFS_hiszero(backId)) {
	struct vol_status	backStatus;

	if (backDesc != -1) {
	    /*
	     * If our caller passed in a backDesc, make sure that it matches
	     * the victim's backId.
	     */
	    code = BOMB_EXEC(FN ".2",
			     ftu_FsetGetStatus(backDesc, &backStatus));
	    if (code)
		goto done;
	
	    CALL_KA_PROC(kaProc, kaArg);
	    PROBE_DESC(fsetDesc);

	    afsl_PAssertTruth(AFS_hsame(backStatus.vol_st.volId, backId),
			      ("Backing fileset IDs don't match (%ld != %ld)",
			       backStatus.vol_st.volId, backId));

	    /* Everything matches, use the backDesc that was passed in */
	    localBackDesc = backDesc;
	} else if (!AFS_hsame(backId, fsetStatus.vol_st.volId)) {
	    /* No backDesc was passed in, we'll open the fileset ourselves */
	    code = BOMB_EXEC(FN ".3",
			     ftu_AggrOpenFset(aggrId, &backId,
					      VOL_SYS_SETSTATUS
					      |FTU_SYS_PUSH_STATUS
					      |FTU_SYS_GET_STATUS
					      |FTU_SYS_SET_STATUS
					      |FTU_SYS_SYNC,
					      VOLERR_TRANS_DELETE,
					      &localBackDesc));
	    if (code == FTU_E_NO_SUCH_FSET || code == EXDEV) {
		/*
		 * Don't worry if this can't find a backing fileset--
		 * just go ahead.
		 */
		ftu_svc_LogPrintf(ftl_s_ff_ignore_bogus_id, STAT_HIGH_ID(fsetStatus),
			      STAT_LOW_ID(fsetStatus),
			      HIGH_ID(backId), LOW_ID(backId));
		icl_Trace2(_ftu_iclSetP,
			   FTU_TRACE_DESTROY_FSET_BOGUS_BACKING_ID,
			   ICL_TYPE_HYPER, &fsetStatus.vol_st.volId,
			   ICL_TYPE_HYPER, &backId);

		AFS_hzero(backId);
		localBackDesc = -1;
	    } else if (code) {
		goto done;
	    }

	    CALL_KA_PROC(kaProc, kaArg);
	    PROBE_DESC(fsetDesc);
	} else {
	    /*
	     * We recognize (and ignore) the special case where the backId
	     * points to the fileset that we're destroying.  The repserver
	     * can leave filesets in that state.
	     */
	    ftu_svc_LogPrintf(ftl_s_ro_vols_restore_ignore_bogus_back_id,
			  STAT_HIGH_ID(fsetStatus),
			  STAT_LOW_ID(fsetStatus),
			  HIGH_ID(backId), LOW_ID(backId));
	    icl_Trace2(_ftu_iclSetP,
		       FTU_TRACE_DESTROY_FSET_BOGUS_BACKING_ID,
		       ICL_TYPE_HYPER, &fsetStatus.vol_st.volId,
		       ICL_TYPE_HYPER, &backId);

	    AFS_hzero(backId);
	}
    }

    /* Open forward-linked fileset, if any */
    if (!AFS_hiszero(fwdId)) {
	if (!AFS_hsame(fwdId, fsetStatus.vol_st.volId)) {
	    code = BOMB_EXEC(FN ".4",
			     ftu_AggrOpenFset(aggrId, &fwdId,
					      FTU_SYS_GET_STATUS
					      |VOL_SYS_UNCLONE
					      |FTU_SYS_SYNC|VOL_SYS_SETSTATUS
					      |FTU_SYS_PUSH_STATUS,
					      VOLERR_TRANS_DELETE,
					      &fwdDesc));
	    if (code == FTU_E_NO_SUCH_FSET || code == EXDEV) {
		/*
		 * Don't worry if this can't find a forward fileset--
		 * just go ahead.
		 */
		ftu_svc_LogPrintf(ftl_s_ff_ignore_bogus_fwd,
				  STAT_HIGH_ID(fsetStatus),
				  STAT_LOW_ID(fsetStatus),
				  HIGH_ID(fwdId), LOW_ID(fwdId));
		icl_Trace2(_ftu_iclSetP,
			   FTU_TRACE_DESTROY_FSET_BOGUS_FWD_ID,
			   ICL_TYPE_HYPER, &fsetStatus.vol_st.volId,
			   ICL_TYPE_HYPER, &fwdId);

		AFS_hzero(fwdId);
		fwdDesc = -1;
	    } else if (code) {
		goto close_back;
	    }
	    
	    CALL_KA_PROC(kaProc, kaArg);
	    PROBE_DESC(fsetDesc);
	    PROBE_DESC(localBackDesc);
	} else {
	    /*
	     * We recognize (and ignore) the special case where the fwdId
	     * points to the fileset that we're destroying.  The repserver
	     * can leave filesets in that state.
	     */
	    ftu_svc_LogPrintf(ftl_s_ro_vols_restore_ignore_bogus_fwd_id,
			  STAT_HIGH_ID(fsetStatus),
			  STAT_LOW_ID(fsetStatus),
			  HIGH_ID(fwdId), LOW_ID(fwdId));
	    icl_Trace2(_ftu_iclSetP,
		       FTU_TRACE_DESTROY_FSET_BOGUS_FWD_ID,
		       ICL_TYPE_HYPER, &fsetStatus.vol_st.volId,
		       ICL_TYPE_HYPER, &fwdId);
	    
	    AFS_hzero(fwdId);
	}
    }

    /* Mark the target as an unusable wreck of a fileset. */
    fsetStatus.vol_st.states |= VOL_ZAPME;
    code = BOMB_EXEC(FN ".5",
		     ftu_FsetSetStatus(fsetDesc,
				       VOL_STAT_STATES, &fsetStatus));
    if (code)
	goto close_fwd;

    CALL_KA_PROC(kaProc, kaArg);
    PROBE_DESC(localBackDesc);
    PROBE_DESC(fwdDesc);

    /* Serialize update to fileset status */
    if (code = BOMB_EXEC(FN ".6", ftu_FsetPushStatus(fsetDesc)))
	goto close_fwd;

    CALL_KA_PROC(kaProc, kaArg);
    PROBE_DESC(localBackDesc);
    PROBE_DESC(fwdDesc);

    if (localBackDesc != -1) {
	/* Point back.fwd to fwdId */
	statusArray[nextIdx].vsd_volDesc = localBackDesc;
	statusArray[nextIdx].vsd_mask = VOL_STAT_LLFWDID;
	statusArray[nextIdx].vsd_status.vol_dy.llFwdId = fwdId;
	nextIdx++;
    }

    if (fwdDesc != -1) {	/* Unclone the forward-linked fileset */
	struct vol_status	fwdStatus;
	afs_hyper_t		newAllocLimit;
	afs_hyper_t		savedAllocLimit;
	afs_hyper_t		savedVisLimit;
	u_long			mask = 0;

	if (code = BOMB_EXEC(FN ".7", ftu_FsetGetStatus(fwdDesc, &fwdStatus)))
	    goto close_fwd;

	CALL_KA_PROC(kaProc, kaArg);
	PROBE_DESC(fsetDesc);
	PROBE_DESC(localBackDesc);

	/* Set the limits so that unclone never runs out of space */
	newAllocLimit = fwdStatus.vol_dy.allocUsage;
	AFS_hadd(newAllocLimit, fsetStatus.vol_dy.allocUsage);
	if (AFS_hcmp(fwdStatus.vol_dy.allocLimit, newAllocLimit) == -1) {
	    /* Limit is less than potential usage, so bump the forward limit */

	    ftu_svc_LogPrintf(ftl_s_ff_up_unclone_alloc_limit, HIGH_ID(fwdId), LOW_ID(fwdId));
	    icl_Trace4(_ftu_iclSetP,
		   FTU_TRACE_DESTROY_FSET_BUMP_LIMIT,
		   ICL_TYPE_STRING, "alloc.", ICL_TYPE_HYPER, &fwdId,
		   ICL_TYPE_HYPER, &newAllocLimit,
		   ICL_TYPE_HYPER, &fwdStatus.vol_dy.allocLimit);

	    savedAllocLimit = fwdStatus.vol_dy.allocLimit;
	    fwdStatus.vol_dy.allocLimit = newAllocLimit;
	    mask |= VOL_STAT_ALLOCLIMIT;
	}

	if (AFS_hcmp(fwdStatus.vol_dy.visQuotaLimit,
		 fwdStatus.vol_dy.visQuotaUsage) == -1) {
	    /* Limit is less than usage, so bump the limit */

	    ftu_svc_LogPrintf(ftl_s_ff_up_unclone_vis_limit, HIGH_ID(fwdId), LOW_ID(fwdId));
	    icl_Trace4(_ftu_iclSetP,
		   FTU_TRACE_DESTROY_FSET_BUMP_LIMIT,
		   ICL_TYPE_STRING, "vis.", ICL_TYPE_HYPER, &fwdId,
		   ICL_TYPE_HYPER, &fwdStatus.vol_dy.visQuotaUsage,
		   ICL_TYPE_HYPER, &fwdStatus.vol_dy.visQuotaLimit);

	    savedVisLimit = fwdStatus.vol_dy.visQuotaLimit;
	    fwdStatus.vol_dy.visQuotaLimit = fwdStatus.vol_dy.visQuotaUsage;
	    mask |= VOL_STAT_VISLIMIT;
	}

	if (mask != 0) {
	    code = BOMB_EXEC(FN ".8",
			     ftu_FsetSetStatus(fwdDesc, mask, &fwdStatus));
	    if (code)
		goto close_fwd;

	    CALL_KA_PROC(kaProc, kaArg);
	    PROBE_DESC(fsetDesc);
	    PROBE_DESC(localBackDesc);
	}

	KA_INIT(ka, backDesc, kaProc, kaArg);
	code = BOMB_EXEC(FN ".9",
			 ftutil_VolUnCloneAux(fwdDesc, fsetDesc, NULL,
					      KeepDescsAlive, &ka));
	if (code)
	    goto close_fwd;

	if (mask != 0) {
	    /* Set the limits back */
	    fwdStatus.vol_dy.allocLimit = savedAllocLimit;
	    fwdStatus.vol_dy.visQuotaLimit = savedVisLimit;

	    code = BOMB_EXEC(FN ".10",
			     ftu_FsetSetStatus(fwdDesc, mask, &fwdStatus));
	    if (code)
		goto close_fwd;

	    CALL_KA_PROC(kaProc, kaArg);
	    PROBE_DESC(fsetDesc);
	    PROBE_DESC(localBackDesc);
	}

	/*
	 * Commit meta-data updates to the target fileset and the
	 * forward-linked fileset
	 */
	if ((code = BOMB_EXEC(FN ".11", COMMIT_META(fsetDesc))) ||
	    (code = BOMB_EXEC(FN ".12", COMMIT_META(fwdDesc))))
	    goto close_fwd;

	CALL_KA_PROC(kaProc, kaArg);
	PROBE_DESC(localBackDesc);

	/* Point fwd.back to backId */
	statusArray[nextIdx].vsd_volDesc = fwdDesc;
	statusArray[nextIdx].vsd_mask = VOL_STAT_LLBACKID;
	statusArray[nextIdx].vsd_status.vol_dy.llBackId = backId;
	nextIdx++;
    }

    if (nextIdx > 0) {
	/* Splice the deleted fileset out of the chain */
	code = BOMB_EXEC(FN ".13",
			 ftu_FsetBulkSetStatus(nextIdx, statusArray));
	if (code)
	    goto close_fwd;

	CALL_KA_PROC(kaProc, kaArg);

	/* Serialize update to backward fileset status */
	if (localBackDesc != -1 &&
	    (code = BOMB_EXEC(FN ".14", ftu_FsetPushStatus(localBackDesc))))
	    goto close_fwd;

	CALL_KA_PROC(kaProc, kaArg);
	PROBE_DESC(fsetDesc);
	PROBE_DESC(fwdDesc);

	/* Serialize update to forward fileset status */
	if (fwdDesc != -1 &&
	    (code = BOMB_EXEC(FN ".15", ftu_FsetPushStatus(fwdDesc))))
	    goto close_fwd;

	CALL_KA_PROC(kaProc, kaArg);
	PROBE_DESC(fsetDesc);
	PROBE_DESC(localBackDesc);
    }

    /* Destroy the fileset */
    KA_INIT(ka, localBackDesc, kaProc, kaArg);
    KA_ADD(ka, fwdDesc);
    code = BOMB_EXEC(FN ".16",
		     ftutil_VolDepleteAux(fsetDesc, NULL,
					  KeepDescsAlive, &ka));
    if (code)
	goto close_fwd;

    /* Commit the deplete */
    if (code = BOMB_EXEC(FN ".17", COMMIT_META(fsetDesc)))
	goto close_fwd;

    CALL_KA_PROC(kaProc, kaArg);
    PROBE_DESC(fsetDesc);
    PROBE_DESC(localBackDesc);
    PROBE_DESC(fwdDesc);

    /* Turn off CLONEINPROG flag if we're cleaning up after a failed clone */
    if (localBackDesc != -1) {
	code = BOMB_EXEC(FN ".18",
			 ftu_FsetGetStatus(localBackDesc, &fsetStatus));
	if (code)
	    goto close_fwd;
	
	CALL_KA_PROC(kaProc, kaArg);
	PROBE_DESC(fsetDesc);
	PROBE_DESC(fwdDesc);

	if (fsetStatus.vol_st.states & VOL_CLONEINPROG) {
	    fsetStatus.vol_st.states &= ~VOL_CLONEINPROG;
	    code = BOMB_EXEC(FN ".19",
			     ftu_FsetSetStatus(localBackDesc, VOL_STAT_STATES,
					       &fsetStatus));
	    if (code)
		goto close_fwd;

	    CALL_KA_PROC(kaProc, kaArg);
	    PROBE_DESC(fsetDesc);
	    PROBE_DESC(fwdDesc);

	    /* Commit status update */
	    if (code = BOMB_EXEC(FN ".20", COMMIT_STATUS(localBackDesc)))
		goto close_fwd;

	    CALL_KA_PROC(kaProc, kaArg);
	    PROBE_DESC(fsetDesc);
	    PROBE_DESC(fwdDesc);
	}
    }

    /* Destroy the husk */
    if (code = BOMB_EXEC(FN ".21", VOL_DESTROY(fsetDesc)))
	goto close_fwd;

    /* Commit the destroy by syncing the aggregate */
    code = BOMB_EXEC(FN ".22",
		     ftu_AggrSync(aggrId, FTU_AGGR_SYNC_COMMIT_META));
    if (code)
	goto close_fwd;

  close_fwd:
    if (fwdDesc != -1)
	if (!code)
	    code = ftu_AggrCloseFset(fwdDesc);
	else
	    ftu_AggrAbortFsetOpen(fwdDesc);
    CALL_KA_PROC(kaProc, kaArg);
    PROBE_DESC(fsetDesc);
    PROBE_DESC(localBackDesc);

  close_back:
    /*
     * Only close the backing fileset if we opened it (ie. if our caller didn't
     * pass a backDesc in).
     */
    if (localBackDesc != -1 && backDesc == -1)
	if (!code)
	    code = ftu_AggrCloseFset(localBackDesc);
	else
	    ftu_AggrAbortFsetOpen(localBackDesc);
    CALL_KA_PROC(kaProc, kaArg);
    PROBE_DESC(fsetDesc);

  done:
    if (code)
	ftu_svc_LogPrintf(ftl_s_ff_destroy_err,
			  STAT_HIGH_ID(fsetStatus), STAT_LOW_ID(fsetStatus),
			  code, dfs_dceErrTxt(code));
    icl_Trace1(_ftu_iclSetP, FTU_TRACE_EXIT_DESTROY_FSET,
	       ICL_TYPE_LONG, code);
    return code;
}	/* DestroyFset() */


/*
 * ftu_FsetDump()
 *	Create a file pipe and let ftu_FsetDumpToPipe() do the work.
 */
EXPORT long ftu_FsetDump(fsetDesc, dumpCriteriaP, fileStream)
    IN ftu_fsetDesc_t		fsetDesc;
    IN ftu_dumpCriteria_t*	dumpCriteriaP;
    IN FILE*			fileStream;
{
    long	code;
    afsPipe_t	filePipe;

    if (CreateFilePipe(fileStream, &filePipe, PFL_WRITE) == -1)
	return EIO;			/* Make something up */

    code = ftu_FsetDumpToPipe(fsetDesc, dumpCriteriaP, &filePipe);

    POP_RELE(&filePipe);

    return code;
}	/* ftu_FsetDump() */


/*
 * ftu_FsetDumpToPipe()
 *	Just a wrapper for ftu_FsetDumpToPipeWithKA().
 */
EXPORT long ftu_FsetDumpToPipe(fsetDesc, dumpCriteriaP, pipeP)
    IN ftu_fsetDesc_t		fsetDesc;
    IN ftu_dumpCriteria_t*	dumpCriteriaP;
    IN afsPipe_t*		pipeP;
{
    return ftu_FsetDumpToPipeWithKA(fsetDesc, dumpCriteriaP, pipeP, 0, 0);
}	/* ftu_FsetDumpToPipe() */

/*
 * ftu_FsetDumpToPipeWithKA()
 *	Determine the aggregate's type so that we can pass it on to
 *	vols_DumpVolume(), a helper routine that does the real work.
 */
EXPORT long ftu_FsetDumpToPipeWithKA(fsetDesc, dumpCriteriaP, pipeP,
				     kaProc, kaArg)
    IN ftu_fsetDesc_t		fsetDesc;
    IN ftu_dumpCriteria_t*	dumpCriteriaP;
    IN afsPipe_t*		pipeP;
    IN ftu_kaProc_t		kaProc;
    IN void*			kaArg;
{
    ftu_aggrId_t	aggrId;
    afs_hyper_t		aggrType;
    long		code;
    afsPipe_t		filePipe;
    struct vol_status	fsetStatus;

    icl_Trace1(_ftu_iclSetP, FTU_TRACE_ENTER_FSET_DUMP_TO_PIPE,
	       ICL_TYPE_LONG, fsetDesc);
    icl_Trace4(_ftu_iclSetP, FTU_TRACE_ENTER_FSET_DUMP_TO_PIPE2,
	       ICL_TYPE_LONG, dumpCriteriaP->dc_type,
	       ICL_TYPE_LONG, dumpCriteriaP->dc_date.sec,
	       ICL_TYPE_LONG, dumpCriteriaP->dc_date.usec,
	       ICL_TYPE_HYPER, &dumpCriteriaP->dc_version);
    icl_Trace1(_ftu_iclSetP, FTU_TRACE_ENTER_FSET_DUMP_TO_PIPE3,
	       ICL_TYPE_POINTER, pipeP);

    if (code = ftu_FsetGetStatus(fsetDesc, &fsetStatus))
	goto done;

    CALL_KA_PROC(kaProc, kaArg);

    aggrId = fsetStatus.vol_dy.aggrId;

    ftu_svc_LogPrintf(ftl_s_ff_dumping, aggrId, fsetStatus.vol_st.volName,
		  STAT_HIGH_ID(fsetStatus), STAT_LOW_ID(fsetStatus));

    if (code = GetAggrType(aggrId, &aggrType))
	goto done;

    code = vols_DumpVolume(fsetDesc, dumpCriteriaP, pipeP, &aggrType,
			   kaProc, kaArg);

  done:
    if (code)
	ftu_svc_LogPrintf(ftl_s_ff_dumping_failed,
			  STAT_HIGH_ID(fsetStatus), STAT_LOW_ID(fsetStatus),
			  code, dfs_dceErrTxt(code));
    icl_Trace1(_ftu_iclSetP, FTU_TRACE_EXIT_FSET_DUMP_TO_PIPE,
	       ICL_TYPE_LONG, code);
    return code;
}	/* ftu_FsetDumpToPipeWithKA() */


/*
 * ftu_FsetRestore()
 *	Create a file pipe and let ftu_FsetRestoreFromPipe() do the work.
 */
EXPORT long
ftu_FsetRestore(fsetDesc, fileStream, flags)
    IN ftu_fsetDesc_t	fsetDesc;
    IN FILE*		fileStream;
    IN u_long		flags;
{
    long	code;
    afsPipe_t	filePipe;

    if (CreateFilePipe(fileStream, &filePipe, PFL_READ) == -1)
	return EIO;

    code = ftu_FsetRestoreFromPipe(fsetDesc, &filePipe, flags);

    POP_RELE(&filePipe);

    return code;
}	/* ftu_FsetRestore() */


/*
 * ftu_FsetRestoreFromPipe()
 *	Just a wrapper for ftu_FsetRestoreFromPipeWithKA().
 */
EXPORT long
ftu_FsetRestoreFromPipe(fsetDesc, pipeP, flags)
    IN ftu_fsetDesc_t	fsetDesc;
    IN afsPipe_t*	pipeP;
    IN u_long		flags;
{
    return ftu_FsetRestoreFromPipeWithKA(fsetDesc, pipeP, flags, 0, 0);
}	/* ftu_FsetRestoreFromPipe() */

/*
 * ftu_FsetRestoreFromPipeWithKA()
 *	Determine the aggregate's type so that we can pass it on to
 *	vols_RestoreVolume(), a helper routine that does the real work.
 */
EXPORT long
ftu_FsetRestoreFromPipeWithKA(fsetDesc, pipeP, flags, kaProc, kaArg)
    IN ftu_fsetDesc_t	fsetDesc;
    IN afsPipe_t*	pipeP;
    IN u_long		flags;
    IN ftu_kaProc_t	kaProc;
    IN void*		kaArg;
{
    ftu_aggrId_t	aggrId;
    afs_hyper_t		aggrType;
    long		code;
    struct vol_status	fsetStatus;

    icl_Trace3(_ftu_iclSetP, FTU_TRACE_ENTER_FSET_RESTORE_FROM_PIPE,
	       ICL_TYPE_LONG, fsetDesc, ICL_TYPE_POINTER, pipeP,
	       ICL_TYPE_LONG, flags);

    if (code = ftu_FsetGetStatus(fsetDesc, &fsetStatus))
	goto done;

    CALL_KA_PROC(kaProc, kaArg);

    aggrId = fsetStatus.vol_dy.aggrId;

    ftu_svc_LogPrintf(ftl_s_ff_restoring, aggrId, fsetStatus.vol_st.volName,
		  STAT_HIGH_ID(fsetStatus), STAT_LOW_ID(fsetStatus));

    if (code = GetAggrType(aggrId, &aggrType))
	goto done;

    code = vols_RestoreVolume(fsetDesc, &aggrType, pipeP, flags,
			      kaProc, kaArg);

  done:
    if (code == FTU_E_RESTORED_INCONSISTENT_FSET)
	ftu_svc_LogPrintf(ftl_s_ff_restoring_inconsistent, STAT_HIGH_ID(fsetStatus), STAT_LOW_ID(fsetStatus));
    else if (code != 0)
	ftu_svc_LogPrintf(ftl_s_ff_restoring_failed,
			  STAT_HIGH_ID(fsetStatus), STAT_LOW_ID(fsetStatus),
			  code, dfs_dceErrTxt(code));
    icl_Trace1(_ftu_iclSetP, FTU_TRACE_EXIT_FSET_RESTORE_FROM_PIPE,
	       ICL_TYPE_LONG, code);
    return code;
}	/* ftu_FsetRestoreFromPipeWithKA() */


/*
 * GetAggrType()
 */
PRIVATE long
GetAggrType(aggrId, aggrTypeP)
    IN ftu_aggrId_t	aggrId;
    OUT afs_hyper_t*	aggrTypeP;
{
    struct aggrInfo	aggrInfo;
    long		code;
    u_long		subType;

    if (code = ftu_AggrGetInfo(aggrId, &aggrInfo))
	return code;

    switch (aggrInfo.type) {
      case AG_TYPE_EPI:
      case AG_TYPE_VXFS:
      case AG_TYPE_DMEPI:
	subType = 1;
	break;

      case AG_TYPE_UFS:
#ifdef AFS_AIX31_ENV
	subType = 1;
#else
#ifdef AFS_OSF_ENV
	subType = 2;
#else
	subType = 3;
#endif	/* AFS_OSF_ENV */
#endif	/* AFS_AIX31_ENV */
	break;

      default:
	subType = 0;
    }

    AFS_hset64(*aggrTypeP, aggrInfo.type, subType);

    return 0;
}	/* GetAggrType() */
