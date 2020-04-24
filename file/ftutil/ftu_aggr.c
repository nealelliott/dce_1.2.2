/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ftu_aggr.c,v $
 * Revision 1.1.19.1  1996/10/02  17:48:27  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:38:23  damon]
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
#include <dcedfs/common_data.h>
#include <dcedfs/syscall.h>
#include <dcedfs/vol_init.h>
#include <dcedfs/vol_errs.h>
#include <dcedfs/compat.h>		/* dfs_dceErrTxt */

#include <ftutil.h>
#include <ftu_private.h>
#include <ftu_trace.h>
#include <ftu_syscalls.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsftlmac.h>
#include <dfsftlsvc.h>
#include <dfsftlmsg.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/ftutil/ftu_aggr.c,v 1.1.19.1 1996/10/02 17:48:27 damon Exp $")

/*
 * Constants
 */
#define NUM_FSETS	16

/*
 * The functionality of each of the following exported routines is described
 * in the header file.  The comments below only give implementation
 * specifics, where necessary.
 */


/******************************************************************************
 * Aggregate routines
 *****************************************************************************/

/*
 * ftu_AggrGetInfo()
 *	Mostly a wrapper for the system call except for remapping ENOENT to
 *	`FTU_E_NOT_ATTACHED' and some kludgery whenever `aggrInfo' is null.
 */
EXPORT long
ftu_AggrGetInfo(aggrId, aggrInfoP)
    IN ftu_aggrId_t		aggrId;
    OUT struct aggrInfo*	aggrInfoP;
{
    long		code= 0;
    struct aggrInfo	dummyInfo;
    struct aggrInfo*	infoPtrP = aggrInfoP;
    int numfree;

    icl_Trace1(_ftu_iclSetP, FTU_TRACE_ENTER_AGGR_GET_INFO,
	       ICL_TYPE_LONG, aggrId);

    /* Caller doesn't want the info, but wants to know if this is a valid ID */
    if (infoPtrP == (struct aggrInfo*)0)
	infoPtrP = &dummyInfo;

    /* Remap ENOENT to be consistent with the other lookup-type routines */
    if ((code = AG_INFO(aggrId, infoPtrP)) && code == ENOENT)
	code = FTU_E_NOT_ATTACHED;

    if (code)
	ftu_svc_LogPrintf(ftl_s_fa_aggr_info_err,
			  aggrId, code, dfs_dceErrTxt(code));
    icl_Trace4(_ftu_iclSetP, FTU_TRACE_EXIT_AGGR_GET_INFO,
	       ICL_TYPE_LONG, code,
	       ICL_TYPE_STRING, infoPtrP->name,
	       ICL_TYPE_STRING, infoPtrP->devName,
	       ICL_TYPE_LONG, infoPtrP->type);
    /* Hack to prevent problems with cpp */
    numfree = infoPtrP->realFree;

    icl_Trace3(_ftu_iclSetP, FTU_TRACE_EXIT_AGGR_GET_INFO2,
	       ICL_TYPE_LONG, infoPtrP->totalUsable,
	       ICL_TYPE_LONG, numfree,
	       ICL_TYPE_LONG, infoPtrP->minFree);
    return code;
}	/* ftu_AggrGetInfo() */


/*
 * ftu_AggrSync()
 *	Just a wrapper for the system call.
 */
EXPORT long
ftu_AggrSync(aggrId, syncOp)
    IN ftu_aggrId_t	aggrId;
    IN int		syncOp;
{
    long	code = 0;

    icl_Trace2(_ftu_iclSetP, FTU_TRACE_ENTER_AGGR_SYNC,
	       ICL_TYPE_LONG, aggrId, ICL_TYPE_LONG, syncOp);

    code = AG_SYNC(aggrId, syncOp);

    if (code)
	ftu_svc_LogPrintf(ftl_s_fa_sync_aggr_err,
			  aggrId, code, dfs_dceErrTxt(code));
    icl_Trace1(_ftu_iclSetP, FTU_TRACE_EXIT_AGGR_SYNC,
	       ICL_TYPE_LONG, code);
    return code;
}	/* ftu_AggrSync() */


/*
 * ftu_AggrCreateFset()
 *	Cobbles together a vol_status structure, and calls
 *	ftu_AggrCreateFsetWithStatus().  Many of this routine's documented
 *	error codes are propagated from that function.
 */
EXPORT long
ftu_AggrCreateFset(aggrId, fsetIdP, fsetName, parentIdP, fsetStates,
		   fsetMaxQuota, fsetFlags)
    IN ftu_aggrId_t	aggrId;
    IN ftu_fsetId_t*	fsetIdP;
    IN char*		fsetName;
    IN ftu_fsetId_t*	parentIdP;
    IN u_long		fsetStates;
    IN long		fsetMaxQuota;
    IN u_long		fsetFlags;
{
    long		code;
    struct vol_status	fsetStatus;

    bzero((char *)&fsetStatus, sizeof fsetStatus);

    fsetStatus.vol_st.volId = *fsetIdP;
    if (parentIdP != (ftu_fsetId_t*)0)
	fsetStatus.vol_st.parentId = *parentIdP;

    if (strlen(fsetName) > (size_t)FTU_MAX_FSET_NAME)
	return FTU_E_NAME_TOO_LONG;
    strncpy(fsetStatus.vol_st.volName, fsetName, FTU_MAX_FSET_NAME);
    fsetStatus.vol_st.volName[FTU_MAX_FSET_NAME] = '\0';
					/* strncpy may leave unterminated */

    fsetStatus.vol_st.type = fsetStates & VOL_TYPEBITS;
    fsetStatus.vol_st.states = fsetStates;
    /* convert visible quota from Kb to bytes */
    AFS_hset64(fsetStatus.vol_dy.visQuotaLimit, 0, fsetMaxQuota);
    AFS_hleftshift(fsetStatus.vol_dy.visQuotaLimit, 10);
    fsetStatus.vol_dy.allocLimit = ftu_unlimitedQuota;

    code = ftu_AggrCreateFsetWithStatus(aggrId, fsetIdP, &fsetStatus,
					fsetFlags);
    if (code)
	return code;

    return 0;
}	/* ftu_AggrCreateFset() */


/*
 * ftu_AggrCreateFsetWithStatus()
 *	Perform various consistency checks and then call the system call.
 */
EXPORT long
ftu_AggrCreateFsetWithStatus(aggrId, fsetIdP, fsetStatusP, fsetFlags)
    IN ftu_aggrId_t		aggrId;
    IN ftu_fsetId_t*		fsetIdP;
    IN struct vol_status*	fsetStatusP;
    IN u_long			fsetFlags;
{
    long		code = 0;
    ftu_fsetDesc_t	dummyDesc;

    icl_Trace4(_ftu_iclSetP, FTU_TRACE_ENTER_AGGR_CREATE_FSET_WITH_STATUS,
	       ICL_TYPE_LONG, aggrId, ICL_TYPE_HYPER, fsetIdP,
	       ICL_TYPE_HYPER, &fsetStatusP->vol_st.parentId,
	       ICL_TYPE_LONG, fsetStatusP->vol_st.states);
    icl_Trace2(_ftu_iclSetP, FTU_TRACE_ENTER_AGGR_CREATE_FSET_WITH_STATUS2,
	       ICL_TYPE_STRING, fsetStatusP->vol_st.volName,
	       ICL_TYPE_LONG, fsetFlags);
    ftu_svc_LogPrintf(ftl_s_fa_create_fileset, fsetStatusP->vol_st.volName,
		  HIGH_ID(*fsetIdP), LOW_ID(*fsetIdP),
		  aggrId);

    /* Make sure aggregate is attached */
    if (code = ftu_AggrGetInfo(aggrId, (struct aggrInfo*)0))
	goto done;

    /* Make sure that the fileset ID isn't in use */
    code = ftu_AggrOpenFset(aggrId, fsetIdP,
			    FTU_SYS_GET_STATUS, VOLERR_TRANS_GETSTATUS,
			    &dummyDesc);
    if (code == FTU_E_NO_SUCH_FSET) {
	code = 0;
    } else {
	if (code == 0) {
	    code = ftu_AggrCloseFset(dummyDesc);
	    if (code == 0)
		code = FTU_E_ID_CLASH;
	}
	goto done;
    }

#if 0
    /* oops--this can happen, until something cleans up junk, leftover filesets first */
    /* Make sure the fileset name isn't in use */
    code = ftu_AggrLookupFset(aggrId, fsetStatusP->vol_st.volName,
			      (ftu_fsetId_t*)0);
    if (code == FTU_E_NO_SUCH_FSET) {
	code = 0;
    } else {
	if (code == 0)
	    code = FTU_E_NAME_CLASH;
	goto done;
    }
#endif /* 0 */

    /* Do the create */
    code = AG_VOLCREATE(aggrId, fsetIdP, fsetStatusP, fsetFlags);
	
  done:
    if (code)
	ftu_svc_LogPrintf(ftl_s_fa_create_failed, fsetStatusP->vol_st.volName,
		      HIGH_ID(*fsetIdP), LOW_ID(*fsetIdP),
		      code, dfs_dceErrTxt(code));
    icl_Trace1(_ftu_iclSetP, FTU_TRACE_EXIT_AGGR_CREATE_FSET_WITH_STATUS,
	       ICL_TYPE_LONG, code);
    return code;
}	/* ftu_AggrCreateFsetWithStatus() */


/*
 * ftu_AggrOpenFset()
 *	Populate an agopen structure and call the system call, remapping
 *	ENODEV to `FTU_E_NO_SUCH_FSET'.
 */
EXPORT long
ftu_AggrOpenFset(aggrId, fsetIdP, fsetFlags, fsetErrorIn, fsetDescP)
    IN ftu_aggrId_t	aggrId;
    IN ftu_fsetId_t*	fsetIdP;
    IN u_long		fsetFlags;
    IN u_long		fsetErrorIn;
    OUT ftu_fsetDesc_t*	fsetDescP;
{
    struct vol_AgOpen	agopen;
    long		code = 0;
    u_long		errorOut = 0;

    icl_Trace3(_ftu_iclSetP, FTU_TRACE_ENTER_AGGR_OPEN_FSET,
	       ICL_TYPE_LONG, aggrId, ICL_TYPE_HYPER, fsetIdP,
	       ICL_TYPE_LONG, fsetFlags);
    icl_Trace2(_ftu_iclSetP, FTU_TRACE_ENTER_AGGR_OPEN_FSET2,
	       ICL_TYPE_LONG, fsetErrorIn,
	       ICL_TYPE_STRING, dfs_dceErrTxt(fsetErrorIn));

    /* Make sure aggregate is attached */
    if (code = ftu_AggrGetInfo(aggrId, (struct aggrInfo*)0))
	goto done;

    bzero((char *)&agopen, sizeof agopen);
    agopen.agId = aggrId;
    agopen.volId = *fsetIdP;
    agopen.accStat = fsetFlags;
    agopen.accErr = fsetErrorIn;

    if (code = VOL_AGOPEN(&agopen, fsetDescP, &errorOut))
	if (code == EBUSY && errorOut != 0)
	    code = errorOut;		/* Use the extended error code */
        else if (code == ENODEV)
	    code = FTU_E_NO_SUCH_FSET;	/* Fileset doesn't exist */

  done:
    if (code)
	ftu_svc_LogPrintf(ftl_s_fa_open_failed,
			  aggrId, HIGH_ID(*fsetIdP), LOW_ID(*fsetIdP),
			  code, dfs_dceErrTxt(code));
    icl_Trace2(_ftu_iclSetP, FTU_TRACE_EXIT_AGGR_OPEN_FSET,
	       ICL_TYPE_LONG, code, ICL_TYPE_LONG, *fsetDescP);
    return code;
}	/* ftu_AggrOpenFset() */


/*
 * ftu_AggrCloseFset()
 *	Just a wrapper for the system call.
 */
EXPORT long
ftu_AggrCloseFset(fsetDesc)
    IN ftu_fsetDesc_t	fsetDesc;
{
    long	code = 0;

    icl_Trace1(_ftu_iclSetP, FTU_TRACE_ENTER_AGGR_CLOSE_FSET,
	       ICL_TYPE_LONG, fsetDesc);

    /* Pass a 0 to indicate a normal close */
    code = VOL_CLOSE(fsetDesc, 0);

    if (code)
	ftu_svc_LogPrintf(ftl_s_fa_close_descriptor_err,
			  fsetDesc, code, dfs_dceErrTxt(code));
    icl_Trace1(_ftu_iclSetP, FTU_TRACE_EXIT_AGGR_CLOSE_FSET,
	       ICL_TYPE_LONG, code);
    return code;
}	/* ftu_AggrCloseFset() */


/*
 * ftu_AggrAbortFsetOpen
 *	Just a wrapper for the system call.
 */
EXPORT void
ftu_AggrAbortFsetOpen(fsetDesc)
    IN ftu_fsetDesc_t	fsetDesc;
{
    long	code = 0;

    icl_Trace1(_ftu_iclSetP, FTU_TRACE_ENTER_AGGR_ABORT_FSET_OPEN,
	       ICL_TYPE_LONG, fsetDesc);

    /* Pass a 1 to indicate an abort */
    code = VOL_CLOSE(fsetDesc, 1);

    if (code)
	ftu_svc_LogPrintf(ftl_s_fa_abort_err,
			  fsetDesc, code, dfs_dceErrTxt(code));
    icl_Trace1(_ftu_iclSetP, FTU_TRACE_EXIT_AGGR_ABORT_FSET_OPEN,
	       ICL_TYPE_LONG, code);
}	/* ftu_AggrAbortFsetOpen() */


/*
 * ftu_AggrEnumerateFsets()
 *	Fill in an io_rwDesc structure and call the system call.
 */
EXPORT long
ftu_AggrEnumerateFsets(aggrId, indexP, fsetBuf, numFsets, numFsetsReturnedP)
    IN ftu_aggrId_t		aggrId;
    INOUT unsigned*		indexP;
    IN struct vol_status*	fsetBuf;
    IN unsigned			numFsets;
    OUT unsigned*		numFsetsReturnedP;
{
    long		code;
    struct io_rwDesc	iodesc;
    unsigned		outlen;

    icl_Trace4(_ftu_iclSetP, FTU_TRACE_ENTER_AGGR_ENUMERATE_FSETS,
	       ICL_TYPE_LONG, aggrId, ICL_TYPE_LONG, *indexP,
	       ICL_TYPE_LONG, fsetBuf, ICL_TYPE_LONG, numFsets);

    /* Make sure aggregate is attached */
    if (code = ftu_AggrGetInfo(aggrId, (struct aggrInfo*)0))
	goto done;

    iodesc.descId = aggrId;
    iodesc.position = *indexP;
    iodesc.bufferp = (char *)fsetBuf;
    iodesc.length = numFsets * sizeof(struct vol_status);

    /* XXX The system call should not really be returning `VOL_ERR_EOF'. (It */
    /* XXX is currently defined as 1!) */
    if ((code = AG_VOLENUMERATE(numFsets, &iodesc, indexP, &outlen)) &&
	code != VOL_ERR_EOF)
	goto done;

    code = 0;
    *numFsetsReturnedP = outlen / sizeof(struct vol_status);

  done:
    if (code)
	ftu_svc_LogPrintf(ftl_s_fa_enumerate_err,
			  aggrId, code, dfs_dceErrTxt(code));
    icl_Trace3(_ftu_iclSetP, FTU_TRACE_EXIT_AGGR_ENUMERATE_FSETS,
	       ICL_TYPE_LONG, code,
	       ICL_TYPE_LONG, *indexP, ICL_TYPE_LONG, *numFsetsReturnedP);
    return code;
}	/* ftu_AggrEnumerateFsets() */


/*
 * ftu_AggrLookupFset()
 *	Enumerate filesets, looking for a match in each batch.
 *	ftu_AggrEnumerateFsets() ensures that `aggrId' is valid.
 */
EXPORT long
ftu_AggrLookupFset(aggrId, fsetName, fsetIdP)
    IN ftu_aggrId_t	aggrId;
    IN char*		fsetName;
    OUT ftu_fsetId_t*	fsetIdP;
{
    long			code;
    struct vol_status		fsetBuf[NUM_FSETS];
    register struct vol_status*	fsetBufP;
    ftu_fsetId_t		oneFsetId;
    unsigned			fsetIndex = 0;
    unsigned			fsetsReturned;
    register unsigned		i;

    icl_Trace2(_ftu_iclSetP, FTU_TRACE_ENTER_AGGR_LOOKUP_FSET,
	       ICL_TYPE_LONG, aggrId, ICL_TYPE_STRING, fsetName);

    do {
	code = ftu_AggrEnumerateFsets(aggrId, &fsetIndex, fsetBuf, NUM_FSETS,
				      &fsetsReturned);
	if (code)
	    break;

	code = FTU_E_NO_SUCH_FSET;

	for (i = 0, fsetBufP = fsetBuf; i < fsetsReturned; i++, fsetBufP++)
	    if (strncmp(fsetName, fsetBufP->vol_st.volName,
			sizeof fsetBufP->vol_st.volName) == 0) {
		if (fsetIdP)
		    *fsetIdP = fsetBufP->vol_st.volId;
		oneFsetId = fsetBufP->vol_st.volId;
		code = 0;
		break;
	    }
    } while (!(fsetsReturned < NUM_FSETS));

    if (code) {
	ftu_svc_LogPrintf(ftl_s_fa_lookup_err,
			  fsetName, aggrId, code, dfs_dceErrTxt(code));
	icl_Trace1(_ftu_iclSetP,
		   FTU_TRACE_EXIT_AGGR_LOOKUP_FSET_WITHOUT_FSET_ID,
		   ICL_TYPE_LONG, code);
    } else {
	icl_Trace2(_ftu_iclSetP, FTU_TRACE_EXIT_AGGR_LOOKUP_FSET,
		   ICL_TYPE_LONG, code, ICL_TYPE_HYPER, &oneFsetId);
    }
    return code;
}	/* ftu_AggrLookupFset() */
