/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ftserver_vprocs.c,v $
 * $EndLog$
 */
/*
 * Copyright (C) 1996, 1989 Transarc Corporation
 * All rights reserved
 */

#include <dcedfs/param.h>
#include <fcntl.h>
#include <dce/rpc.h>
#include <dcedfs/queue.h> 
#include <dcedfs/volume.h>
#include <dcedfs/sysincludes.h>

#include <dcedfs/dfsauth.h>
#include <dcedfs/ftutil.h>  
#include <dcedfs/osi_dfserrors.h>

#include <ftserver_proc.h>
#include <ftserver_data.h>

#include <dcedfs/pipe.h>
#include <dcedfs/sock_pipe.h>
#include <dcedfs/ncs_pipe.h>
#include <dcedfs/compat.h>
#include <dcedfs/flserver.h>	/* XXX */
#include <dcedfs/syscall.h>
#include <dcedfs/vol_errs.h>
#include <dcedfs/ftutil.h>
#include <dcedfs/ftu_syscalls.h>

#include <ftserver.h>
#include <ftserver_trans.h>
#include <ftserver_trace.h>

#include <pthread.h>

#ifdef AFS_OSF_ENV
#include <assert.h>
#endif /* AFS_OSF_ENV */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfstsvmac.h>
#include <dfstsvsvc.h>
#include <dfstsvmsg.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/ftserver/RCS/ftserver_vprocs.c,v 4.290 1996/10/16 20:15:18 gait Exp $")

/* array of ftserver ops values, each value is an 'or' of all volops
 * invoked by this particular ftserver op 
 */
unsigned long ftserver_ops_values[] = {
    FTU_SYS_DESTROY,			/* FTSERVER_OP_DELETE */
    FTU_SYS_DUMP,			/* FTSERVER_OP_DUMP */
    FTU_SYS_RESTORE | FTU_SYS_SYNC,	/* FTSERVER_OP_RESTORE */
    FTU_SYS_CLONE,			/* FTSERVER_OP_CLONE */
    FTU_SYS_RECLONE_INTO,		/* FTSERVER_OP_RECLONE */
    FTU_SYS_GET_STATUS,			/* FTSERVER_OP_GETFLAGS */
    FTU_SYS_SET_STATUS | FTU_SYS_SYNC,	/* FTSERVER_OP_SETFLAGS */
    FTU_SYS_GET_STATUS,			/* FTSERVER_OP_GETSTATUS */
    FTU_SYS_SET_STATUS | FTU_SYS_SYNC,	/* FTSERVER_OP_SETSTATUS */
    VOL_SYS_SWAPVOLIDS,			/* FTSERVER_OP_SETSTATUS_ID */
    FTU_SYS_SWAP_IDS | FTU_SYS_SYNC,	/* FTSERVER_OP_SWAPIDS */
    VOL_SYS_NOACCESS			/* FTSERVER_OP_NOACCESS */
};

/* 
 * a verification macro to ensure that any ftserver op that is invoked has the
 * corresponding ftserver op bit set in the ftserver ops bit vector. This
 * vector is stored in the t_states field of the transaction.
 */
#define FTSCHECK(transp, ftsOp, code) { \
        code = 0; \
        if ( ((ftsOp) & (transp->t_accStatus)) != (ftsOp) ) { \
	   if (verbose) \
		dce_svc_printf(TSV_S_VPROCS_OP_NOT_SET_MSG, \
   (ftsOp), (transp)->t_accStatus, __FILE__, __LINE__); \
	   (code) = VOLERR_PERS_BADFTSOPSVECTOR; \
	   } \
}

/* The following two procedures handle disabling and enabling cancels. */

static void vols_CancelsBlock(rtnName, stateP)
char *rtnName;
int *stateP;
{
    int cancelCode;

    icl_Trace1(iclSetP, FTSERVER_TRACE_ENTER_LOCK,
		ICL_TYPE_STRING, rtnName);
    errno=0;
    cancelCode = 0;
    DFS_DISABLE_CANCEL(stateP, cancelCode);
    if (cancelCode)
	Log(tsv_s_vprocs_log_DFS_DISABLE_CANCEL, rtnName, errno);
    icl_Trace3(iclSetP, FTSERVER_TRACE_ENTER_LOCK_2,
		ICL_TYPE_STRING, rtnName, ICL_TYPE_LONG, *stateP,
		ICL_TYPE_LONG, cancelCode);
}

static long vols_CancelsRestore(rtnName, state, errorcode)
char *rtnName;
int state;
long errorcode;
{
    int cancelCode;

    icl_Trace2(iclSetP, FTSERVER_TRACE_ENTER_UNLOCK,
		ICL_TYPE_STRING, rtnName, ICL_TYPE_LONG, state);
    errno=0;
    cancelCode = 0;
    DFS_ENABLE_CANCEL(state, cancelCode);
    if (cancelCode)
	Log(tsv_s_vprocs_log_DFS_ENABLE_CANCEL, rtnName, errno);
    icl_Trace2(iclSetP, FTSERVER_TRACE_ENTER_UNLOCK_2,
		ICL_TYPE_STRING, rtnName, ICL_TYPE_LONG, cancelCode);
    return osi_errEncode(errorcode);
}

struct transarray {
    int count;
    struct ftserver_trans *transes[4];
};

static void vols_AliveTranses(argp)
void *argp;
{
    register int ix;
    register struct transarray *tarp = (struct transarray *)argp;
    register unsigned long Now;

    Now = osi_Time();
    for (ix = 0; ix < tarp->count; ++ix)
	ftserver_KeepTransAlive(tarp->transes[ix], Now);
}

/* forward declarations */
static void ftops_to_volops();
static long vols_FillBaseType();
static long DeleteTrans();


static long getOrigStatus(transp)
struct ftserver_trans *transp;
{/* If we'll be restoring a fileset, get its status before we start. */
/* This is because the act of opening for restoration will set the VOL_DELONSALVAGE bit. */
    struct vol_AgOpen agopen;
    struct vol_status volStatus;
    long code, errCode;
    int vDesc;

    icl_Trace4(iclSetP, FTSERVER_TRACE_ENTER_GET_ORIG_STATUS,
	       ICL_TYPE_LONG, transp->t_transId,
	       ICL_TYPE_LONG, transp->t_aggrId,
	       ICL_TYPE_HYPER, &transp->t_volId,
	       ICL_TYPE_LONG, transp->t_vDesc);

    bzero((char *)&agopen, sizeof(struct vol_AgOpen));
    agopen.volId = transp->t_volId;
    agopen.agId = transp->t_aggrId;
    agopen.accStat = VOL_SYS_GETSTATUS;
    agopen.accErr = VOLERR_TRANS_GETSTATUS;
    if (code = VOL_AGOPEN(&agopen, &vDesc, &errCode)) {
	if (code == EBUSY) code = errCode;
	if (transp->t_returnCode == 0) transp->t_returnCode = code;
	icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_GET_ORIG_STATUS,
		   ICL_TYPE_LONG, 1, ICL_TYPE_LONG, code);
	ftserver_KeepTransAlive(transp, 0);
	return code;
    }
    ftserver_KeepTransAlive(transp, 0);
    code = VOL_GETSTATUS(vDesc, &volStatus);
    if (code) {
	if (transp->t_returnCode == 0) transp->t_returnCode = code;
	ftu_AggrAbortFsetOpen(vDesc);
	icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_GET_ORIG_STATUS,
		   ICL_TYPE_LONG, 2, ICL_TYPE_LONG, code);
	return code;
    }
    transp->t_states = volStatus.vol_st.states;
#if 0
    /* BOGUS because just-created filesets are marked INCONSISTENT
      just because they have no root dir. */
    if ((transp->t_states & VOL_DELONSALVAGE) != 0)
	transp->t_flags |= FTSERVER_FLAGS_WASINCONSISTENT;
#endif /* 0 */

    ftu_AggrAbortFsetOpen(vDesc);
    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_GET_ORIG_STATUS,
	       ICL_TYPE_LONG, 3, ICL_TYPE_LONG, code);
    return code;
}

/* Convert between the on-the-wire format, ftserver_Date, and the */
/* ftutil-supplied ftu_dumpCriteria_t */
static void dumpDateToDumpCriteria(dumpDateP, dumpCriteriaP)
    ftserver_Date *dumpDateP;
    ftu_dumpCriteria_t *dumpCriteriaP;
{
    bzero((char*)dumpCriteriaP, sizeof *dumpCriteriaP);
    
    switch (dumpDateP->mask) {
      case 0:
      default:
	dumpCriteriaP->dc_type = FTU_DUMP_TYPE_FULL;
	break;
      case 1:
	dumpCriteriaP->dc_type = FTU_DUMP_TYPE_DATE_WITH_DIRS;
	dumpCriteriaP->dc_date = dumpDateP->fromDate;
	break;
      case 2:
	dumpCriteriaP->dc_type = FTU_DUMP_TYPE_VERS_WITH_DIRS;
	dumpCriteriaP->dc_version = dumpDateP->fromVersion;
	break;
      case 3:
	dumpCriteriaP->dc_type = FTU_DUMP_TYPE_DATE;
	dumpCriteriaP->dc_date = dumpDateP->fromDate;
	break;
      case 4:
	dumpCriteriaP->dc_type = FTU_DUMP_TYPE_VERS;
	dumpCriteriaP->dc_version = dumpDateP->fromVersion;
	break;
    };
}
    
long SFTSERVER_CreateTrans(callp, volIdp, aggrId, attFlags, transIdp)
rpc_binding_handle_t callp;
afs_hyper_t *volIdp;
unsigned long aggrId;
unsigned long attFlags;
unsigned long *transIdp;
{
    register struct ftserver_trans *transp;
    long code;
    int cancelState, vDesc;
    unsigned long accError, accStat;
    unsigned long openFlags;
    static char	routineName[] = "SFTSERVER_CreateTrans";

    vols_CancelsBlock(routineName, &cancelState);
    icl_Trace4(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_CREATE_TRANS,
	       ICL_TYPE_POINTER, callp, ICL_TYPE_HYPER, volIdp,
	       ICL_TYPE_LONG, aggrId, ICL_TYPE_LONG, attFlags);
    if (verbose > 1)
	dce_svc_printf(TSV_S_VPROCS_FTSERVER_CREATETRANS_MSG,
		       AFS_HGETBOTH(*volIdp), aggrId);

    if ((code = dfsauth_server_CheckAuthorization(&callp, (dacl_permset_t *)NULL,
						   (char *)NULL,
						   (uuid_t *)NULL)) == 0) {
	/* 
	 * decode the flags parameter to extract the ftserver ops bit vector 
	 * and the VOLERR error code
	 */
	FLAGS_DECODE(attFlags, accStat, accError);
	if (verbose)
	    dce_svc_printf(TSV_S_VPROCS_CREATETRANS_FLAGS_DECODE_MSG, attFlags, accStat, accError);

	if (!(transp = ftserver_NewTrans(volIdp, aggrId, accError, &code))) {
	    icl_Trace3(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_CREATE_TRANS,
		       ICL_TYPE_LONG, 1, ICL_TYPE_LONG, code,
		       ICL_TYPE_LONG, 0);
	    if (verbose > 1) 
		dce_svc_printf(TSV_S_VPROCS_CREATETRANS_ETURNING_CODE_MSG, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}

	code = vols_FillBaseType(transp);
	if (code) {
	    if (transp->t_returnCode == 0) transp->t_returnCode = code;
	    ftserver_DeleteTrans(transp, 0);
	    icl_Trace3(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_CREATE_TRANS,
		       ICL_TYPE_LONG, 2, ICL_TYPE_LONG, code,
		       ICL_TYPE_LONG, 0);
	    if (verbose > 1) 
		dce_svc_printf(TSV_S_VPROCS_FTSERVER_CREATETRANS_3_MSG, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}
	if (accStat & FTSERVER_OP_RESTORE) {
	    code = getOrigStatus(transp);
	    if (code) {
		if (transp->t_returnCode == 0) transp->t_returnCode = code;
		ftserver_DeleteTrans(transp, 0);
		icl_Trace3(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_CREATE_TRANS,
			   ICL_TYPE_LONG, 3, ICL_TYPE_LONG, code,
			   ICL_TYPE_LONG, 0);
		if (verbose > 1) 
		    dce_svc_printf(TSV_S_VPROCS_FTSERVER_CREATETRANS_4_MSG, code);
		return vols_CancelsRestore(routineName, cancelState, code);
	    }
	}
	/* 
	 * accStat contains the ftserver ops bit vector; convert this to a 
	 * volops bit vector
	 */
	ftops_to_volops (accStat, &openFlags);
	
	code = ftu_AggrOpenFset(aggrId, &transp->t_volId, openFlags, accError,
				&vDesc);
	if (code) {
	    /*
	     * FTSERVER_ERR_NOVOL is an architectural error code--fts checks
	     * for it after some RPCs to this routine, so we have to remap
	     * the ftutil layer's code.
	     */
	    if (code == FTU_E_NO_SUCH_FSET)
		code = FTSERVER_ERR_NOVOL;
	    if (transp->t_returnCode == 0) transp->t_returnCode = code;
	    ftserver_DeleteTrans(transp, 0);
	    icl_Trace3(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_CREATE_TRANS,
		       ICL_TYPE_LONG, 4, ICL_TYPE_LONG, code,
		       ICL_TYPE_LONG, 0);
	    if (verbose > 1) 
		dce_svc_printf(TSV_S_VPROCS_FTSERVER_CREATETRANS_2_MSG, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}
	transp->t_vDesc = vDesc;
	ftserver_KeepTransAlive(transp, 0);
	/* 
	 * The new t_accStatus field is used to store the ftserver ops vector.
	 * This can be used to make the FTSCHECK calls to verify if the 
	 * bit corresponding to the FTSERVER call made is set prior to it.
	 */
	transp->t_states = 0; /* we don't need attFlags parameter anymore */ 
	transp->t_accStatus = accStat;
	*transIdp = transp->t_transId;
	ftserver_PutTrans(transp);
    }
    else {
	/* don't log simple access denials */
	if (code != DAUT_ERROR_ACCESS_DENIED) {
	    Log(tsv_s_vprocs_CheckAuthorization_failed,
		routineName, dfs_dceErrTxt(code));
	}
	icl_Trace2(iclSetP, FTSERVER_TRACE_AUTHFAIL,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, code);
	*transIdp = 0;
    }

    icl_Trace3(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_CREATE_TRANS,
	       ICL_TYPE_LONG, 5, ICL_TYPE_LONG, code,
	       ICL_TYPE_LONG, *transIdp);
    if (verbose > 1)
	dce_svc_printf(TSV_S_VPROCS_FTSERVER_CREATETRANS_RETURNING_MSG, *transIdp, code);
    return vols_CancelsRestore(routineName, cancelState, code);
}


static long DeleteTrans(callp, transId, isAbort)
    rpc_binding_handle_t callp;
    unsigned long transId;
    int isAbort;
{
    register struct ftserver_trans *transp;
    long code;
    static char	routineName[] = "DeleteTrans";

    icl_Trace3(iclSetP, FTSERVER_TRACE_ENTER_DELETE_TRANS,
	       ICL_TYPE_POINTER, callp, ICL_TYPE_LONG, transId,
	       ICL_TYPE_LONG, isAbort);
    if ((code = dfsauth_server_CheckAuthorization(&callp, 
						  (dacl_permset_t *)NULL,
						   (char *)NULL, 
						  (uuid_t *)NULL)) == 0) {
	if (verbose > 1) 
	    dce_svc_printf(TSV_S_VPROCS_SFTSERVER_DELETETRANS_MSG, transId, isAbort);
	if (!(transp = ftserver_FindTrans(transId))) {
	    code = FTSERVER_ERR_NOTRANS;
	    icl_Trace2(iclSetP, FTSERVER_TRACE_ENTER_DELETE_TRANS,
		       ICL_TYPE_LONG, 1, ICL_TYPE_LONG, code);
	    if (verbose > 1) 
		dce_svc_printf(TSV_S_VPROCS_SFTSERVER_DELETETRANS_NO_SUCH_MSG, transId);
	    return code;
	}

	if (verbose > 1) 
	  dce_svc_printf(TSV_S_VPROCS_SFTSERVER_DELETETRANS_ON_AGGR_MSG,
		 		isAbort ? "aborting" : "deleting",
				transp->t_transId,
				AFS_HGETBOTH(transp->t_volId),
				transp->t_aggrId);

	if (isAbort ||(transp->t_flags & FTSERVER_FLAGS_WASINCONSISTENT) != 0)
	    ftu_AggrAbortFsetOpen(transp->t_vDesc);
	else
	    code = ftu_AggrCloseFset(transp->t_vDesc);
	if (transp->t_returnCode == 0) transp->t_returnCode = code;
	ftserver_DeleteTrans(transp, 0);
    }
    else {
	/* don't log simple access denials */
	if (code != DAUT_ERROR_ACCESS_DENIED) {
	    Log(tsv_s_vprocs_CheckAuthorization_failed,
		routineName, dfs_dceErrTxt(code));
	}
	icl_Trace2(iclSetP, FTSERVER_TRACE_AUTHFAIL,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, code);
    }

    icl_Trace2(iclSetP, FTSERVER_TRACE_ENTER_DELETE_TRANS,
	       ICL_TYPE_LONG, 2, ICL_TYPE_LONG, code);
    return code;
}


long SFTSERVER_AbortTrans(callp, transId)
    rpc_binding_handle_t callp;
    unsigned long transId;
{
    long code;
    int cancelState;
    static char	routineName[] = "SFTSERVER_AbortTrans";

    vols_CancelsBlock(routineName, &cancelState);
    icl_Trace2(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_ABORT_TRANS,
	       ICL_TYPE_POINTER, callp, ICL_TYPE_LONG, transId);
    code = DeleteTrans(callp, transId, 1);
    icl_Trace1(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_ABORT_TRANS,
	       ICL_TYPE_LONG, code);
    return vols_CancelsRestore(routineName, cancelState, code);
}


long SFTSERVER_DeleteTrans(callp, transId)
    rpc_binding_handle_t callp;
    unsigned long transId;
{
    long code;
    int cancelState;
    static char	routineName[] = "SFTSERVER_DeleteTrans";

    vols_CancelsBlock(routineName, &cancelState);
    icl_Trace2(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_DELETE_TRANS,
	       ICL_TYPE_POINTER, callp, ICL_TYPE_LONG, transId);
    code = DeleteTrans(callp, transId, 0);
    icl_Trace1(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_DELETE_TRANS,
	       ICL_TYPE_LONG, code);
    return vols_CancelsRestore(routineName, cancelState, code);
}


static unsigned long typeToTypeBits(typeVal)
unsigned long typeVal;
{/* Convert a type value to the typefield that describes it, if any. */
    register unsigned long typeBits;

    typeBits = 0;
    switch (typeVal) {
	case VOLTIX_TO_VOLTYPE(RWVOL):
	    typeBits = VOL_TYPE_RW;
	    break;
	case VOLTIX_TO_VOLTYPE(ROVOL):
	    typeBits = VOL_TYPE_RO;
	    break;
	case VOLTIX_TO_VOLTYPE(BACKVOL):
	    typeBits = VOL_TYPE_BK;
	    break;
	case VOLTIX_TO_VOLTYPE(3):
	    typeBits = VOL_TYPE_TEMP;
	    break;
    }
    return typeBits;
}

/*
 * Create a volume, given the partition, name, type, etc.  Return a new
 * transaction ID attached to the new transaction.
 */
long SFTSERVER_CreateVolume(callp, aggrId, volNamep, volType, attFlags, parVolIdp, volIdp, transIdp)
    rpc_binding_handle_t callp;
    unsigned long aggrId;
    char *volNamep;		/* XXXXXXXX */
    unsigned long volType;
    unsigned long attFlags;
    afs_hyper_t *parVolIdp;
    afs_hyper_t *volIdp;
    unsigned long *transIdp; 
{
    register struct ftserver_trans *transp;
    long code, accErr;
    int vDesc;
    unsigned long accStat;
    int cancelState;
    long type;
    unsigned long openFlags;
    static char	routineName[] = "SFTSERVER_CreateVolume";

    vols_CancelsBlock(routineName, &cancelState);
    icl_Trace4(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_CREATE_VOLUME,
	       ICL_TYPE_POINTER, callp, ICL_TYPE_LONG, aggrId,
	       ICL_TYPE_STRING, volNamep, ICL_TYPE_LONG, volType);
    icl_Trace3(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_CREATE_VOLUME2,
	       ICL_TYPE_LONG, attFlags, ICL_TYPE_HYPER, parVolIdp,
	       ICL_TYPE_HYPER, volIdp);
    if ((code = dfsauth_server_CheckAuthorization(&callp, (dacl_permset_t *)NULL,
						   (char *)NULL, (uuid_t *)NULL)) == 0) {
	if (verbose > 1)
	    dce_svc_printf(TSV_S_VPROCS_FTSERVER_CREATEVOLUME_CREATING_MSG,
			   volNamep, AFS_HGETBOTH(*volIdp), aggrId);
	/* decode the flags parameter to extract the ftserver ops bit vector 
	 * and the VOLERR error code
	 */
	FLAGS_DECODE(attFlags, accStat, accErr);
	if (verbose)
	    dce_svc_printf(TSV_S_VPROCS_FTSERVER_CREATETRANS_FLAGS_DECODE_MSG, attFlags, accStat, accErr);
	/* create a transaction for this volume */
	if (!(transp = ftserver_NewTrans(volIdp, aggrId, accErr, &code))) {
	    icl_Trace3(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_CREATE_VOLUME,
		       ICL_TYPE_LONG, 1, ICL_TYPE_LONG, code,
		       ICL_TYPE_LONG, 0);
	    if (verbose > 1)
		dce_svc_printf(TSV_S_VPROCS_FTSERVER_CREATEVOLUME_ALREADY_MSG,
			       AFS_HGETBOTH(*volIdp), aggrId, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}

	code = vols_FillBaseType(transp);
	if (code) {
	    if (transp->t_returnCode == 0) transp->t_returnCode = code;
	    ftserver_DeleteTrans(transp, 0);
	    icl_Trace3(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_CREATE_VOLUME,
		       ICL_TYPE_LONG, 2, ICL_TYPE_LONG, code,
		       ICL_TYPE_LONG, 0);
	    if (verbose > 1) 
		dce_svc_printf(TSV_S_VPROCS_FTSERVER_CREATEVOLUME_BASE_TYPE_MSG, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}
/* XXX: need to get this from cell profile */
#ifndef DEFAULT_MAX_QUOTA
#define DEFAULT_MAX_QUOTA 5000
#endif /* DEFAULT_MAX_QUOTA */

	/* create the actual volume */
	type = (volType == VOLTIX_TO_VOLTYPE(RWVOL) ? VOL_RW : VOL_READONLY),
	code = ftu_AggrCreateFset(aggrId, volIdp, volNamep, parVolIdp,
				  type|typeToTypeBits(volType),
				  DEFAULT_MAX_QUOTA, AGGR_CREATE_ROOT);
	if (code) {
	    if (code == ENOENT) code = FTSERVER_ERR_BADAGGREGATE;
	    if (transp->t_returnCode == 0) transp->t_returnCode = code;
	    ftserver_DeleteTrans(transp, 0);
	    icl_Trace3(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_CREATE_VOLUME,
		       ICL_TYPE_LONG, 3, ICL_TYPE_LONG, code,
		       ICL_TYPE_LONG, 0);
	    if (verbose > 1)
		dce_svc_printf(TSV_S_VPROCS_FTSERVER_CREATEVOLUME_FTU_AGGRCREATEFSET_MSG, AFS_HGETBOTH(*volIdp), aggrId, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}
	ftserver_KeepTransAlive(transp, 0);

	/* next, take the volume offline and remember it */

	/* 
	 * accStat contains the ftserver ops bit vector; convert this to a 
	 * volops bit vector
	 */
	ftops_to_volops(accStat, &openFlags);

	/* We're going to do a sync for free! */
	openFlags |= FTU_SYS_SYNC;
	code = ftu_AggrOpenFset(aggrId, &transp->t_volId,
				openFlags, accErr, &vDesc);
	if (code) {
	    /* can't open the newly-created volume: return an error */
	    if (transp->t_returnCode == 0) transp->t_returnCode = code;
	    ftserver_DeleteTrans(transp, 0);
	    icl_Trace3(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_CREATE_VOLUME,
		       ICL_TYPE_LONG, 4, ICL_TYPE_LONG, code,
		       ICL_TYPE_LONG, 0);
	    if (code == EBUSY) code = accErr;
	    if (verbose > 1)
		dce_svc_printf(TSV_S_VPROCS_FTSERVER_CREATEVOLUME_FTU_AGGROPENFSET_MSG, AFS_HGETBOTH(*volIdp), aggrId, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}
	transp->t_vDesc = vDesc;
	transp->t_states = 0;
	transp->t_accStatus = accStat;
	*transIdp = transp->t_transId;
	ftserver_KeepTransAlive(transp, 0);
	code = ftu_FsetSync(vDesc, FTU_FSET_SYNC_COMMIT_META);
	ftserver_KeepTransAlive(transp, 0);
	if (code != 0) {
	    /* Can't sync it to disk! */
	    if (transp->t_returnCode == 0) transp->t_returnCode = code;
	    ftserver_DeleteTrans(transp, 0);
	    icl_Trace3(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_CREATE_VOLUME,
		       ICL_TYPE_LONG, 5, ICL_TYPE_LONG, code,
		       ICL_TYPE_LONG, 0);
	    if (verbose > 1)
		dce_svc_printf(TSV_S_VPROCS_FTSERVER_CREATEVOLUME_FTU_FSETSYNC_MSG, AFS_HGETBOTH(*volIdp), aggrId, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}
	if (verbose > 1)
	    dce_svc_printf(TSV_S_VPROCS_FTSERVER_CREATEVOLUME_RETURNS_MSG,
			   AFS_HGETBOTH(*volIdp), aggrId, transp->t_transId);
	ftserver_PutTrans(transp);
	Log(tsv_s_vprocs_log_ftserver_CreateVolume, volNamep,
	    AFS_HGETBOTH(*volIdp), aggrId);
    }
    else {
	/* don't log simple access denials */
	if (code != DAUT_ERROR_ACCESS_DENIED) {
	    Log(tsv_s_vprocs_CheckAuthorization_failed,
		routineName, dfs_dceErrTxt(code));
	}
	icl_Trace2(iclSetP, FTSERVER_TRACE_AUTHFAIL,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, code);
	*transIdp = 0;
    }
    icl_Trace3(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_CREATE_VOLUME,
	       ICL_TYPE_LONG, 6, ICL_TYPE_LONG, code,
	       ICL_TYPE_LONG, *transIdp);
    return vols_CancelsRestore(routineName, cancelState, code);
}


/*
 * DeleteVolume.  Find and destroy a transaction attached to an already-existing
 * volume.
 */
long SFTSERVER_DeleteVolume(callp, transId)
    rpc_binding_handle_t callp;
    unsigned long transId; 
{
    register struct ftserver_trans *transp;
    register long code;
    static char	routineName[] = "SFTSERVER_DeleteVolume";
    int cancelState;
    long ftserver_DeleteVolume();
    struct transarray tar;

    vols_CancelsBlock(routineName, &cancelState);
    icl_Trace2(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_DELETE_VOLUME,
	       ICL_TYPE_POINTER, callp, ICL_TYPE_LONG, transId);
    if ((code = dfsauth_server_CheckAuthorization(&callp, 
						  (dacl_permset_t *)NULL,
						   (char *)NULL, 
						  (uuid_t *)NULL)) == 0) {
	if (!(transp = ftserver_FindTrans(transId))) {
	    code = FTSERVER_ERR_NOTRANS;
	    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_DELETE_VOLUME,
		       ICL_TYPE_LONG, 1, ICL_TYPE_LONG, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}
	/* Make sure ftserver ops bit vector is complete */
	FTSCHECK(transp, FTSERVER_OP_DELETE, code);
	if (code) {
	    if (transp->t_returnCode == 0) transp->t_returnCode = code;
	    ftserver_PutTrans(transp);
	    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_DELETE_VOLUME,
		       ICL_TYPE_LONG, 2, ICL_TYPE_LONG, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}

	if (transp->t_states & VOL_DEADMEAT) {
	    if (transp->t_returnCode == 0) transp->t_returnCode = VOLERR_PERS_DELETED;
	    code = VOLERR_PERS_DELETED;
	    ftserver_PutTrans(transp);
	    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_DELETE_VOLUME,
		       ICL_TYPE_LONG, 3, ICL_TYPE_LONG, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}

	tar.count = 1;
	tar.transes[0] = transp;
	code = ftu_FsetDestroyWithKA(transp->t_vDesc,
				     vols_AliveTranses, (void *)&tar);
	if (transp->t_returnCode == 0) transp->t_returnCode = code;
	ftserver_PutTrans(transp);
    }
    else {
	/* don't log simple access denials */
	if (code != DAUT_ERROR_ACCESS_DENIED) {
	    Log(tsv_s_vprocs_CheckAuthorization_failed,
		routineName, dfs_dceErrTxt(code));
	}
	icl_Trace2(iclSetP, FTSERVER_TRACE_AUTHFAIL,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, code);
    }
    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_DELETE_VOLUME,
	       ICL_TYPE_LONG, 4, ICL_TYPE_LONG, code);
    return vols_CancelsRestore(routineName, cancelState, code);
}

#if !defined(OSF_NO_SOCKET_DUMP)
/*
 * Establish a socket connection with a client
 */
long SFTSERVER_ConnectSock(
    rpc_binding_handle_t callp,
    unsigned32 transId,
    ftserver_addr_t *clntAddr,
    ftserver_addr_t *srvrAddr)
{
    struct ftserver_trans * volatile transp;
    volatile long code = 0;
    volatile int cancelState;
    char *excNameP;
    char excBuff[50];
    static char routineName[] = "SFTSERVER_Connect";
    extern char * ExceptionToString();
    transp = NULL;
    TRY {
	if (!(transp = ftserver_FindTrans(transId))) {
	    code = FTSERVER_ERR_NOTRANS;
	} else if (transp->t_clntSockP || transp->t_srvrSockP) {
	    code = EISCONN;
	} else {
            vols_CancelsBlock(routineName, &cancelState);
            icl_Trace4(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_CONNECT_SOCK,
	               ICL_TYPE_POINTER, callp, ICL_TYPE_LONG, transId,
		       ICL_TYPE_POINTER, clntAddr, ICL_TYPE_POINTER, srvrAddr);
	    /* attempt to establish the connection to the client */
	    switch (clntAddr->protocol) {
	      case FTSERVER_UNIX:
	        srvrAddr->protocol = FTSERVER_UNIX;
	        icl_Trace2(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_CONNECT_UNIX,
		           ICL_TYPE_LONG, clntAddr->protocol,
		           ICL_TYPE_STRING, clntAddr->address.unix_addr);
	        code = connectUnixSock((char *)clntAddr->address.unix_addr,
				       (char *)srvrAddr->address.unix_addr,
				       (srvrSock_t **)&transp->t_srvrSockP);
	        if (!code) {
		    icl_Trace2(iclSetP,
			       FTSERVER_TRACE_EXIT_SFTSERVER_CONNECT_UNIX,
			       ICL_TYPE_LONG, srvrAddr->protocol,
			       ICL_TYPE_STRING, srvrAddr->address.unix_addr);
	        } else {
		    transp->t_srvrSockP = NULL;
		}
	        break;
	      case FTSERVER_TCP:
	        srvrAddr->protocol = FTSERVER_TCP;
	        icl_Trace3(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_CONNECT_TCP,
		           ICL_TYPE_LONG, clntAddr->protocol,
		           ICL_TYPE_LONG, clntAddr->address.tcp_addr.tcpPort,
		           ICL_TYPE_LONG, clntAddr->address.tcp_addr.inetAddr);
	        code = connectTcpSock(clntAddr->address.tcp_addr.tcpPort,
	    			      clntAddr->address.tcp_addr.inetAddr,
	    			      &srvrAddr->address.tcp_addr.tcpPort,
	    			      &srvrAddr->address.tcp_addr.inetAddr,
				      (srvrSock_t **)&transp->t_srvrSockP);
	        if (!code) {
		    icl_Trace3(iclSetP,
			       FTSERVER_TRACE_EXIT_SFTSERVER_CONNECT_TCP,
			       ICL_TYPE_LONG, srvrAddr->protocol,
			       ICL_TYPE_LONG,
			       srvrAddr->address.tcp_addr.tcpPort,
			       ICL_TYPE_LONG,
			       srvrAddr->address.tcp_addr.inetAddr);
	        } else {
		    transp->t_srvrSockP = NULL;
		}
	        break;
	      default:
	        icl_Trace1(iclSetP,
			   FTSERVER_TRACE_ENTER_SFTSERVER_CONNECT_UNSPEC,
		           ICL_TYPE_LONG, clntAddr->protocol);
	        code = FTSERVER_ERR_BAD_PROTOCOL;
	    }
	}
    } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	    sprintf(excBuff, "[[unk: %#lx]]", THIS_CATCH);
	    excNameP = excBuff;
	}
	Log(tsv_s_vprocs_log_dumping_exception, routineName, excNameP);
	icl_Trace3(iclSetP, FTSERVER_TRACE_EXCEPTION,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, code,
		   ICL_TYPE_STRING, excNameP);
	code = EINTR;
    } ENDTRY
    if (code) {
        srvrAddr->protocol = FTSERVER_UNSPEC;
    }
    if (transp != NULL) {
	ftserver_PutTrans(transp);
    }
    icl_Trace1(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_CONNECT_SOCK,
	       ICL_TYPE_LONG, code);
    return vols_CancelsRestore(routineName, cancelState, code);
}

/*
 * Terminate a socket connection with a client
 */
long SFTSERVER_DisconnectSock(
    rpc_binding_handle_t callp,
    unsigned32 transId)
{
    struct ftserver_trans * volatile transp;
    volatile long code;
    volatile int cancelState;
    char *excNameP;
    char excBuff[50];
    static char routineName[] = "SFTSERVER_Disconnect";
    extern char * ExceptionToString();
    transp = NULL;
    TRY {
	if (!(transp = ftserver_FindTrans(transId))) {
	        code = FTSERVER_ERR_NOTRANS;
	} else {
            vols_CancelsBlock(routineName, &cancelState);
            icl_Trace2(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_DISCONNECT_SOCK,
	               ICL_TYPE_POINTER, callp, ICL_TYPE_LONG, transId);
	    if (transp->t_srvrSockP) {
		code = termSrvrSock((srvrSock_t *)transp->t_srvrSockP);
		transp->t_srvrSockP = NULL;
	    } else if (transp->t_clntSockP) {
		code = termClntSock((clntSock_t *)transp->t_clntSockP);
		transp->t_clntSockP = NULL;
	    } else {
		code = ENOTCONN;
	    }
	}
    } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	    sprintf(excBuff, "[[unk: %#lx]]", THIS_CATCH);
	    excNameP = excBuff;
	}
	Log(tsv_s_vprocs_log_dumping_exception, routineName, excNameP);
	icl_Trace3(iclSetP, FTSERVER_TRACE_EXCEPTION,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, code,
		   ICL_TYPE_STRING, excNameP);
	code = EINTR;
    } ENDTRY
    if (transp != NULL)
	ftserver_PutTrans(transp);
    icl_Trace1(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_DISCONNECT_SOCK,
	       ICL_TYPE_LONG, code);
    return vols_CancelsRestore(routineName, cancelState, code);
}

/*
 * Listen for a socket connection from a client
 */
long SFTSERVER_ListenSock(
    rpc_binding_handle_t callp,
    unsigned32 transId,
    ftserver_addr_t *srvrAddr)
{
    struct ftserver_trans * volatile transp;
    volatile long code = 0;
    volatile int cancelState;
    char *excNameP;
    char excBuff[50];
    static char routineName[] = "SFTSERVER_Listen";
    extern char * ExceptionToString();
    transp = NULL;
    TRY {
	if (!(transp = ftserver_FindTrans(transId))) {
	    code = FTSERVER_ERR_NOTRANS;
	} else if (transp->t_clntSockP || transp->t_srvrSockP) {
	    code = EISCONN;
	} else {
            vols_CancelsBlock(routineName, &cancelState);
            icl_Trace3(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_LISTEN_SOCK,
	               ICL_TYPE_POINTER, callp, ICL_TYPE_LONG, transId,
		       ICL_TYPE_POINTER, srvrAddr);
	    /* attempt to establish the connection to the client */
	    code = lstnTcpSock(SOCK_CLNT_RECV, NULL,
			       &srvrAddr->address.tcp_addr.tcpPort,
			       &srvrAddr->address.tcp_addr.inetAddr,
			       (clntSock_t **)&transp->t_clntSockP);
	    if (!code) {
	        srvrAddr->protocol = FTSERVER_TCP;
	        icl_Trace3(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_LISTEN_TCP,
		           ICL_TYPE_LONG, srvrAddr->protocol,
		           ICL_TYPE_LONG, srvrAddr->address.tcp_addr.tcpPort,
		           ICL_TYPE_LONG, srvrAddr->address.tcp_addr.inetAddr);
	    }
	}
    } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	    sprintf(excBuff, "[[unk: %#lx]]", THIS_CATCH);
	    excNameP = excBuff;
	}
	Log(tsv_s_vprocs_log_dumping_exception, routineName, excNameP);
	icl_Trace3(iclSetP, FTSERVER_TRACE_EXCEPTION,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, code,
		   ICL_TYPE_STRING, excNameP);
	code = EINTR;
    } ENDTRY
    if (code) {
	srvrAddr->protocol = FTSERVER_UNSPEC;
    }
    if (transp != NULL) {
	ftserver_PutTrans(transp);
    }
    icl_Trace1(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_LISTEN_SOCK,
	       ICL_TYPE_LONG, code);
    return vols_CancelsRestore(routineName, cancelState, code);
}

/*
 * Accept a socket connection from a client
 */
long SFTSERVER_AcceptSock(
    rpc_binding_handle_t callp,
    unsigned32 transId,
    ftserver_addr_t *clntAddr)
{
    struct ftserver_trans * volatile transp;
    volatile long code = 0;
    volatile int cancelState;
    char *excNameP;
    char excBuff[50];
    static char routineName[] = "SFTSERVER_Accept";
    extern char * ExceptionToString();
    transp = NULL;
    TRY {
	if (!(transp = ftserver_FindTrans(transId))) {
	    code = FTSERVER_ERR_NOTRANS;
	} else if (!transp->t_clntSockP) {
	    code = ENOTCONN;
	} else if (clntAddr->protocol != FTSERVER_TCP) {
	    code = FTSERVER_ERR_BAD_PROTOCOL;
	    termClntSock((clntSock_t *)transp->t_clntSockP);
	    transp->t_clntSockP = NULL;
	} else {
            vols_CancelsBlock(routineName, &cancelState);
            icl_Trace3(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_ACCEPT_SOCK,
	               ICL_TYPE_POINTER, callp, ICL_TYPE_LONG, transId,
		       ICL_TYPE_POINTER, clntAddr);
	    icl_Trace3(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_ACCEPT_TCP,
		       ICL_TYPE_LONG, clntAddr->protocol,
		       ICL_TYPE_LONG, clntAddr->address.tcp_addr.tcpPort,
		       ICL_TYPE_LONG, clntAddr->address.tcp_addr.inetAddr);
	    /* attempt to establish the connection to the client */
	    code = acceptTcpSock((clntSock_t *)transp->t_clntSockP,
			         clntAddr->address.tcp_addr.tcpPort,
			         clntAddr->address.tcp_addr.inetAddr);
	    if (code) {
	        transp->t_clntSockP = NULL;
	    } else {
		/* convert the client connection to a server connection */
		code = srvrSockFromClnt((clntSock_t *)transp->t_clntSockP,
					(srvrSock_t **)&transp->t_srvrSockP);
		if (code) {
	            transp->t_srvrSockP = NULL;
		}
	        transp->t_clntSockP = NULL;
	    }
	}
    } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	    sprintf(excBuff, "[[unk: %#lx]]", THIS_CATCH);
	    excNameP = excBuff;
	}
	Log(tsv_s_vprocs_log_dumping_exception, routineName, excNameP);
	icl_Trace3(iclSetP, FTSERVER_TRACE_EXCEPTION,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, code,
		   ICL_TYPE_STRING, excNameP);
	code = EINTR;
    } ENDTRY
    if (transp != NULL) {
	ftserver_PutTrans(transp);
    }
    icl_Trace1(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_ACCEPT_SOCK,
	       ICL_TYPE_LONG, code);
    return vols_CancelsRestore(routineName, cancelState, code);
}
#endif /* OSF_NO_SOCKET_DUMP */

/*
 * Dump a volume. 
 */
long SFTSERVER_Dump(callp, transId, dumpDatep, dataPipeP)
     rpc_binding_handle_t callp;
     unsigned long transId;
     struct ftserver_Date *dumpDatep;
     pipe_t *dataPipeP;
{
    struct ftserver_trans * volatile transp;
    volatile long code;
    afsPipe_t afsPipe;

#if !defined(OSF_NO_SOCKET_DUMP)
    srvrSock_t *srvrSockP;
#endif /* OSF_NO_SOCKET_DUMP */

    volatile int cancelState, needTerminator;
    char *excNameP;
    char excBuff[50];
    static char routineName[] = "SFTSERVER_Dump";
    char dum;		/* see if the push routine really needs an address for 0 bytes */
    extern char * ExceptionToString();

    transp = NULL;
    needTerminator = 0;
    TRY {
    vols_CancelsBlock(routineName, &cancelState);
    icl_Trace2(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_DUMP,
	       ICL_TYPE_POINTER, callp, ICL_TYPE_LONG, transId);
    icl_Trace4(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_DUMP2,
	       ICL_TYPE_LONG, dumpDatep->mask,
	       ICL_TYPE_LONG, dumpDatep->fromDate.sec,
	       ICL_TYPE_LONG, dumpDatep->fromDate.usec,
	       ICL_TYPE_HYPER, &dumpDatep->fromVersion);
    icl_Trace1(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_DUMP3,
	       ICL_TYPE_POINTER, dataPipeP);
    if ((code = dfsauth_server_CheckAuthorization(&callp, (dacl_permset_t *)NULL,
						   (char *)NULL,
						   (uuid_t *)NULL)) == 0) {
	if (!(transp = ftserver_FindTrans(transId))) {
	    code = FTSERVER_ERR_NOTRANS;
	} else {
	    FTSCHECK(transp, FTSERVER_OP_DUMP, code);
	    if (code) {
		if (transp->t_returnCode == 0) transp->t_returnCode = code;
	    } else {
		/*
		 * Seek to the beginning of the volume, and start processing the dump 
		 */
		/* we create a pipe structure for use with dump */
		code = CreateNCSPipe(dataPipeP, &afsPipe, PFL_WRITE);

#if !defined(OSF_NO_SOCKET_DUMP)
		if (!code && transp->t_srvrSockP) {
		    /* if we have a socket, forget the pipe */
		    POP_RELE(&afsPipe);
		    srvrSockP = (srvrSock_t *)(transp->t_srvrSockP);
		    transp->t_srvrSockP = NULL;
		    code = createSockPipe(srvrSockP, &afsPipe, PFL_WRITE);
		}
#endif /* OSF_NO_SOCKET_DUMP */

		if (code) {
		    if (transp->t_returnCode == 0) transp->t_returnCode = code;
		} else {
		    ftu_dumpCriteria_t	dumpCriteria;
		    struct transarray	tar;

		    needTerminator = 1;
		    dumpDateToDumpCriteria(dumpDatep, &dumpCriteria);
		    tar.count = 1;
		    tar.transes[0] = (struct ftserver_trans *)transp;
		    code = ftu_FsetDumpToPipeWithKA(transp->t_vDesc,
						    &dumpCriteria, &afsPipe,
						    vols_AliveTranses,
						    (void *)&tar);
		    if (transp->t_returnCode == 0) transp->t_returnCode = code;
		}
	    }
	}
    }
    else {
	/* don't log simple access denials */
	if (code != DAUT_ERROR_ACCESS_DENIED) {
	    Log(tsv_s_vprocs_CheckAuthorization_failed,
		routineName, dfs_dceErrTxt(code));
	}
	icl_Trace2(iclSetP, FTSERVER_TRACE_AUTHFAIL,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, code);
    }
    if (needTerminator) {
	/* terminate the output pipe */
	if (POP_CLOSE(&afsPipe, PFL_WRITE) == -1)
	    code = EPIPE;
	POP_RELE(&afsPipe);
    }
    } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	    sprintf(excBuff, "[[unk: %#lx]]", THIS_CATCH);
	    excNameP = excBuff;
	}
	Log(tsv_s_vprocs_log_dumping_exception, routineName, excNameP);
	icl_Trace3(iclSetP, FTSERVER_TRACE_EXCEPTION,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, code,
		   ICL_TYPE_STRING, excNameP);
	code = EPIPE;
    } ENDTRY
    if (transp != NULL)
	ftserver_PutTrans(transp);
    icl_Trace1(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_DUMP, ICL_TYPE_LONG, code);
    return vols_CancelsRestore(routineName, cancelState, code);
}

/*
 * Code to drain an NCS pipe to avoid exceptions on errors
 */
static void drainInputPipe(routineName, code, dataPipeP)
     char * volatile routineName;
     volatile long code;
     pipe_t * volatile dataPipeP;
{
    unsigned long itemCount;
    unsigned long itemTotal;
    unsigned long callCount;
    idl_byte localBuffer[2048];
    char *excNameP;
    char excBuff[50];
    extern char * ExceptionToString();

    TRY {
	/* drain the pipe, but throw info away. */
	icl_Trace2(iclSetP, FTSERVER_TRACE_NEED_DRAIN,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, code);
	itemTotal = 0;
	callCount = 0;
	if (dataPipeP->pull) {
	    do {
		(*(dataPipeP->pull))(dataPipeP->state, localBuffer,
				     sizeof(localBuffer), &itemCount);
	        itemTotal += itemCount;
	        ++callCount;
	    } while (itemCount > 0);
	}
	icl_Trace3(iclSetP, FTSERVER_TRACE_DRAIN_DONE,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, itemTotal,
		   ICL_TYPE_LONG, callCount);
    } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	    sprintf(excBuff, "[[unk: %#lx]]", THIS_CATCH);
	    excNameP = excBuff;
	}
	Log(tsv_s_vprocs_log_exception_terminating, routineName, excNameP);
	icl_Trace3(iclSetP, FTSERVER_TRACE_EXCEPTION,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, code,
		   ICL_TYPE_STRING, excNameP);
    } ENDTRY
    return;
}

/*
 * Restore a volume
 */
long SFTSERVER_Restore(callp, transId, flags, dataPipeP)
     rpc_binding_handle_t callp;
     unsigned long transId, flags;
     pipe_t *dataPipeP;
{
    struct ftserver_trans * volatile transp;
    volatile long	code;
#if !defined(OSF_NO_SOCKET_DUMP)
    srvrSock_t *srvrSockP;
#endif /* OSF_NO_SOCKET_DUMP */
    afsPipe_t	afsPipe;
    char *excNameP;
    char excBuff[50];
    static char	routineName[] = "SFTSERVER_Restore";
    int		needDrain;
    volatile int cancelState;
    long		code2;
    u_long		ftuFlags;
    extern char * ExceptionToString();
    struct transarray tar;

    transp = NULL;
    needDrain = 1;
    TRY {
    vols_CancelsBlock(routineName, &cancelState);
    icl_Trace4(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_RESTORE,
	       ICL_TYPE_POINTER, callp, ICL_TYPE_LONG, transId,
	       ICL_TYPE_LONG, flags, ICL_TYPE_POINTER, dataPipeP);
    if ((code = dfsauth_server_CheckAuthorization(&callp, (dacl_permset_t *)NULL,
						   (char *)NULL,
						   (uuid_t *)NULL)) == 0) {
	if (!(transp = ftserver_FindTrans(transId))) {
	    code = FTSERVER_ERR_NOTRANS;
	} else {
	    FTSCHECK(transp, FTSERVER_OP_RESTORE, code);
	    if (code) {
		if (transp->t_returnCode == 0) transp->t_returnCode = code;
	    } else {
		/* create a pipe structure */
		code = CreateNCSPipe(dataPipeP, &afsPipe, PFL_READ);
#if !defined(OSF_NO_SOCKET_DUMP)
		if (!code && transp->t_srvrSockP) {
		    /* if we have a socket, forget the pipe */
		    needDrain = 0;
		    POP_RELE(&afsPipe);
		    srvrSockP = (srvrSock_t *)(transp->t_srvrSockP);
		    transp->t_srvrSockP = NULL;
		    code = createSockPipe(srvrSockP, &afsPipe, PFL_READ);
		}
#endif /* OSF_NO_SOCKET_DUMP */
		if (code == 0) {
		    Log(tsv_s_vprocs_log_restoring_fileset,
			AFS_HGETBOTH(transp->t_volId), transp->t_aggrId);
		    needDrain = 0;	/* regardless of return code */
		    /* Convert on-the-wire flags to the ftutil values */
		    ftuFlags = 0;
		    if (flags & FTSERVER_RESTORE_FORCE_PRESERVE)
			ftuFlags |= FTU_RESTORE_FORCE_PRESERVE;
		    if (flags & FTSERVER_RESTORE_DISJOINT_OK)
			ftuFlags |= FTU_RESTORE_DISJOINT_OK;
		    tar.count = 1;
		    tar.transes[0] = (struct ftserver_trans *)transp;
		    code = ftu_FsetRestoreFromPipeWithKA(transp->t_vDesc,
							 &afsPipe, ftuFlags,
							 vols_AliveTranses,
							 (void *)&tar);
		    if (code == FTU_E_DISJOINT_DATE)
			code = FTSERVER_ERR_DISJOINT_DATE;
		    else if (code == FTU_E_DISJOINT_VERSION)
			code = FTSERVER_ERR_DISJOINT_VV;
		    else if (code == FTU_E_RETROGRADE_DATE)
			code = FTSERVER_ERR_DISJOINT2_DATE;
		    else if (code == FTU_E_RETROGRADE_VERSION)
			code = FTSERVER_ERR_DISJOINT2_VV;
		    else if (code == FTU_E_RESTORING_BAD_ACL)
			code = FTSERVER_ERR_BAD_ACL_RESTORE;
		    else if (code == FTU_E_RESTORING_BAD_ACL_ENTRYTYPE)
			code = FTSERVER_ERR_BAD_ACL_ENTRYTYPE_RESTORE;
		    Log(tsv_s_vprocs_log_restored_fileset,
			AFS_HGETBOTH(transp->t_volId), transp->t_aggrId,
			code);
		    code2 = ftu_FsetSync(transp->t_vDesc,
					 FTU_FSET_SYNC_COMMIT_ALL);
		    ftserver_KeepTransAlive((struct ftserver_trans *)transp, 0);
		    if (code == 0)
			code = code2;
		}
		if (transp->t_returnCode == 0) transp->t_returnCode = code;
	    }
	}
    }
    else {
	/* don't log simple access denials */
	if (code != DAUT_ERROR_ACCESS_DENIED) {
	    Log(tsv_s_vprocs_CheckAuthorization_failed,
		routineName, dfs_dceErrTxt(code));
	}
	icl_Trace2(iclSetP, FTSERVER_TRACE_AUTHFAIL,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, code);
    }
    if (needDrain) {
	/* drain the pipe, but throw info away. */
	drainInputPipe(routineName, code, dataPipeP);
    }
    } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	    sprintf(excBuff, "[[unk: %#lx]]", THIS_CATCH);
	    excNameP = excBuff;
	}
	Log(tsv_s_vprocs_log_exception_terminating, routineName, excNameP);
	icl_Trace3(iclSetP, FTSERVER_TRACE_EXCEPTION,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, code,
		   ICL_TYPE_STRING, excNameP);
	code = EPIPE;
    } ENDTRY
    if (transp != NULL)
	ftserver_PutTrans(transp);
    icl_Trace1(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_RESTORE,
		ICL_TYPE_LONG, code);
    return vols_CancelsRestore(routineName, cancelState, code);
}


/*
 * Forward Volume; There are two filesets referenced here. The fileset corresponding to fromtransId will be dumped. So it should be opened with accStat similar to SFTSERVER_DumpVolume. The fileset corresponding to totransId will be restored at the destination (destp). So it should be opened with accStat similar to SFTSERVER_RestoreVolume. Both the filesets should be opened with accErr set to VOLERR_TRANS_FORWARD
 */
long SFTSERVER_Forward(callp, fromtransId, dumpDatep, destp, restoreFlags, totransId)
    rpc_binding_handle_t callp;
    unsigned long fromtransId;
    struct ftserver_Date *dumpDatep;
    struct ftserver_dest *destp;
    unsigned long restoreFlags;
    unsigned long totransId;
{
    register struct ftserver_trans *fromtransp;
    register long code;
    int cancelState;
    static char	routineName[] = "SFTSERVER_Forward";

    vols_CancelsBlock(routineName, &cancelState);
    icl_Trace2(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_FORWARD,
	       ICL_TYPE_POINTER, callp, ICL_TYPE_LONG, fromtransId);
    icl_Trace4(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_FORWARD2,
	       ICL_TYPE_LONG, dumpDatep->mask,
	       ICL_TYPE_LONG, dumpDatep->fromDate.sec,
	       ICL_TYPE_LONG, dumpDatep->fromDate.usec,
	       ICL_TYPE_HYPER, &dumpDatep->fromVersion);
    icl_Trace3(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_FORWARD3,
	       ICL_TYPE_POINTER, destp, ICL_TYPE_LONG, restoreFlags,
	       ICL_TYPE_LONG, totransId);
    if ((code = dfsauth_server_CheckAuthorization(&callp, 
						  (dacl_permset_t *)NULL,
						  (char *)NULL,
						  (uuid_t *)NULL)) == 0) {
	if (!(fromtransp = ftserver_FindTrans(fromtransId))) {
	    return vols_CancelsRestore(routineName, cancelState,
				       FTSERVER_ERR_NOTRANS);
	}
	
	code = vols_Forward(fromtransp, callp, fromtransId, dumpDatep,
			    destp, restoreFlags, totransId);
	if (fromtransp->t_returnCode == 0) fromtransp->t_returnCode = code;

	ftserver_PutTrans(fromtransp);
    }
    else {
	/* don't log simple access denials */
	if (code != DAUT_ERROR_ACCESS_DENIED) {
	    Log(tsv_s_vprocs_CheckAuthorization_failed,
		routineName, dfs_dceErrTxt(code));
	}
	icl_Trace2(iclSetP, FTSERVER_TRACE_AUTHFAIL,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, code);
    }
    icl_Trace1(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_FORWARD,
	       ICL_TYPE_LONG, code);
    return vols_CancelsRestore(routineName, cancelState, code);
}


/*
 * Clone volume - This function deals with three filesets.
 */
long SFTSERVER_Clone(callP, transId, newType, newNamep, newVolIdp)
    rpc_binding_handle_t callP;
    unsigned long transId;
    unsigned long newType;
    char *newNamep;
    afs_hyper_t *newVolIdp;
{
    struct ftserver_trans *transp;
    long code;
    afs_hyper_t oldFSId, newFSId;
    int cancelState;
    static char	routineName[] = "SFTSERVER_Clone";
    struct transarray tar;

    vols_CancelsBlock(routineName, &cancelState);
    icl_Trace4(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_CLONE,
	       ICL_TYPE_POINTER, callP, ICL_TYPE_LONG, transId,
	       ICL_TYPE_LONG, newType, ICL_TYPE_STRING, newNamep);
    icl_Trace1(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_CLONE2,
	       ICL_TYPE_HYPER, newVolIdp);
    if ((code = dfsauth_server_CheckAuthorization(&callP, (dacl_permset_t *)NULL,
						   (char *)NULL,
						   (uuid_t *)NULL)) == 0) {
	if (!(transp = ftserver_FindTrans(transId))) {
	    code = FTSERVER_ERR_NOTRANS;
	    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_CLONE,
		       ICL_TYPE_LONG, 1, ICL_TYPE_LONG, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}
	FTSCHECK(transp, FTSERVER_OP_CLONE, code);
	if (code) {
	    if (transp->t_returnCode == 0) transp->t_returnCode = code;
	    ftserver_PutTrans(transp);
	    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_CLONE,
		       ICL_TYPE_LONG, 2, ICL_TYPE_LONG, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}

	tar.count = 1;
	tar.transes[0] = transp;
	code = ftu_FsetCloneWithKA(transp->t_vDesc, newVolIdp, newNamep,
				   typeToTypeBits(newType)|VOL_READONLY,
				   vols_AliveTranses, (void *)&tar);

	if (transp->t_returnCode == 0) transp->t_returnCode = code;
	ftserver_PutTrans(transp);
    }
    else {
	/* don't log simple access denials */
	if (code != DAUT_ERROR_ACCESS_DENIED) {
	    Log(tsv_s_vprocs_CheckAuthorization_failed,
		routineName, dfs_dceErrTxt(code));
	}
	icl_Trace2(iclSetP, FTSERVER_TRACE_AUTHFAIL,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, code);
	bzero((char *)newVolIdp, sizeof(newVolIdp));
    }
    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_CLONE,
	       ICL_TYPE_LONG, 3, ICL_TYPE_LONG, code);
    return vols_CancelsRestore(routineName, cancelState, code);
}

/*
 * ReClone volume.
 */
long SFTSERVER_ReClone(callp, transId, newVolIdp)
    rpc_binding_handle_t callp;
    unsigned long transId;
    afs_hyper_t *newVolIdp;
{
    register struct ftserver_trans *trans1p;
    register long code;
    int cancelState;
    afs_hyper_t newAllocLimit;
    afs_hyper_t newVisLimit;
    u_long mask = 0;
    static char routineName[] = "SFTSERVER_ReClone";
    struct transarray tar;

    vols_CancelsBlock(routineName, &cancelState);
    if (verbose) dce_svc_printf(TSV_S_VPROCS_RECLONE_TRANSID_MSG, transId,
				AFS_HGETBOTH(*newVolIdp));
    icl_Trace3(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_RECLONE,
	       ICL_TYPE_POINTER, callp, ICL_TYPE_LONG, transId,
	       ICL_TYPE_HYPER, newVolIdp);
    if ((code = dfsauth_server_CheckAuthorization(&callp, (dacl_permset_t *)NULL,
						   (char *)NULL, (uuid_t *)NULL)) == 0) {
	if (!(trans1p = ftserver_FindTrans(transId))) {
	    code = FTSERVER_ERR_NOTRANS;
	    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_RECLONE,
		       ICL_TYPE_LONG, 1, ICL_TYPE_LONG, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}
	FTSCHECK(trans1p, FTSERVER_OP_RECLONE, code);
	if (code) {
	    if (trans1p->t_returnCode == 0) trans1p->t_returnCode = code;
	    ftserver_PutTrans(trans1p);
	    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_RECLONE,
		       ICL_TYPE_LONG, 2, ICL_TYPE_LONG, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}

	tar.count = 1;
	tar.transes[0] = trans1p;
	code = ftu_FsetRecloneIntoWithKA(trans1p->t_vDesc, newVolIdp,
					 vols_AliveTranses, (void *)&tar);
	if (trans1p->t_returnCode == 0) trans1p->t_returnCode = code;
	ftserver_PutTrans(trans1p);
    } else {
	/* don't log simple access denials */
	if (code != DAUT_ERROR_ACCESS_DENIED) {
	    Log(tsv_s_vprocs_CheckAuthorization_failed,
		routineName, dfs_dceErrTxt(code));
	}
	icl_Trace2(iclSetP, FTSERVER_TRACE_AUTHFAIL,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, code);
    }
    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_RECLONE,
	       ICL_TYPE_LONG, 9, ICL_TYPE_LONG, code);
    if (verbose) dce_svc_printf(TSV_S_VPROCS_RECLONE_LEAVING_MSG,code);
    return vols_CancelsRestore(routineName, cancelState, code);
}


/*
 * GetFlags.  */
long SFTSERVER_GetFlags(callp, transId, OutFlagsp)
    rpc_binding_handle_t callp;
    unsigned long transId;
    unsigned long *OutFlagsp;
{
    register struct ftserver_trans *transp;
    register long code;
    int cancelState;
    struct vol_status status;
    static char	routineName[] = "SFTSERVER_GetFlags";

    vols_CancelsBlock(routineName, &cancelState);
    icl_Trace2(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_GET_FLAGS,
	       ICL_TYPE_POINTER, callp, ICL_TYPE_LONG, transId);
    if ((code = dfsauth_server_CheckAuthorization(&callp, (dacl_permset_t *)NULL,
						   (char *)NULL, (uuid_t *)NULL)) == 0) {
	if (!(transp = ftserver_FindTrans(transId))) {
	    code = FTSERVER_ERR_NOTRANS;
	    icl_Trace3(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_GET_FLAGS,
		       ICL_TYPE_LONG, 1, ICL_TYPE_LONG, code,
		       ICL_TYPE_LONG, 0);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}
	FTSCHECK(transp, FTSERVER_OP_GETFLAGS, code);
	if (code) {
	    if (transp->t_returnCode == 0) transp->t_returnCode = code;
	    ftserver_PutTrans(transp);
	    icl_Trace3(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_GET_FLAGS,
		       ICL_TYPE_LONG, 2, ICL_TYPE_LONG, code,
		       ICL_TYPE_LONG, 0);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}

	code = ftu_FsetGetStatus(transp->t_vDesc, &status);
	if (!code)
	    *OutFlagsp = status.vol_st.states;

	ftserver_PutTrans(transp);
    }
    else {
	/* don't log simple access denials */
	if (code != DAUT_ERROR_ACCESS_DENIED) {
	    Log(tsv_s_vprocs_CheckAuthorization_failed,
		routineName, dfs_dceErrTxt(code));
	}
	icl_Trace2(iclSetP, FTSERVER_TRACE_AUTHFAIL,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, code);
	*OutFlagsp = 0;
    }
    icl_Trace3(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_GET_FLAGS,
	       ICL_TYPE_LONG, 3, ICL_TYPE_LONG, code,
	       ICL_TYPE_LONG, *OutFlagsp);
    return vols_CancelsRestore(routineName, cancelState, code);
}


/*
 * SetFlags.
 */
long SFTSERVER_SetFlags(callp, transId, flags)
    rpc_binding_handle_t callp;
    unsigned long transId;
    unsigned long flags;
{
    register struct ftserver_trans *transp;
    struct vol_status volStatus;
    register long code;
    int cancelState;
    static char	routineName[] = "SFTSERVER_SetFlags";

    vols_CancelsBlock(routineName, &cancelState);
    icl_Trace3(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_SET_FLAGS,
	       ICL_TYPE_POINTER, callp, ICL_TYPE_LONG, transId,
	       ICL_TYPE_LONG, flags);
    if ((code = dfsauth_server_CheckAuthorization(&callp, (dacl_permset_t *)NULL,
						   (char *)NULL, (uuid_t *)NULL)) == 0) {
	if (!(transp = ftserver_FindTrans(transId))) {
	    code = FTSERVER_ERR_NOTRANS;
	    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_SET_FLAGS,
		       ICL_TYPE_LONG, 1, ICL_TYPE_LONG, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}
	FTSCHECK(transp, FTSERVER_OP_SETFLAGS, code);
	if (code) {
	    if (transp->t_returnCode == 0) transp->t_returnCode = code;
	    ftserver_PutTrans(transp);
	    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_SET_FLAGS,
		       ICL_TYPE_LONG, 2, ICL_TYPE_LONG, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}
		   
	bzero((char *)&volStatus, sizeof(struct vol_status));
	volStatus.vol_st.states = flags;
	code = ftu_FsetSetStatus(transp->t_vDesc, VOL_STAT_STATES, &volStatus);
	if (!code) {
	    ftserver_KeepTransAlive(transp, 0);
	    transp->t_states = flags;
	    code = ftu_FsetSync(transp->t_vDesc, FTU_FSET_SYNC_COMMIT_STATUS);
	}
	if (transp->t_returnCode == 0) transp->t_returnCode = code;
	ftserver_PutTrans(transp);
    }
    else {
	/* don't log simple access denials */
	if (code != DAUT_ERROR_ACCESS_DENIED) {
	    Log(tsv_s_vprocs_CheckAuthorization_failed,
		routineName, dfs_dceErrTxt(code));
	}
	icl_Trace2(iclSetP, FTSERVER_TRACE_AUTHFAIL,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, code);
    }
    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_SET_FLAGS,
	       ICL_TYPE_LONG, 3, ICL_TYPE_LONG, code);
    return vols_CancelsRestore(routineName, cancelState, code);
}

/* forward declaration */
static void status_lclToNet();

/*
 * GetStatus.
 */
long SFTSERVER_GetStatus(callp, transId, statusp)
    rpc_binding_handle_t callp;
    unsigned long transId;
    ftserver_status *statusp;
{
    register struct ftserver_trans *transp;
    struct vol_status volStatus;
    register long code;
    int cancelState;
    static char	routineName[] = "SFTSERVER_GetStatus";

    vols_CancelsBlock(routineName, &cancelState);
    icl_Trace3(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_GET_STATUS,
	       ICL_TYPE_POINTER, callp, ICL_TYPE_LONG, transId,
	       ICL_TYPE_POINTER, statusp);
    if ((code = dfsauth_server_CheckAuthorization(&callp, 
						  (dacl_permset_t *)NULL,
						   (char *)NULL, 
						  (uuid_t *)NULL)) == 0) {
	if (!(transp = ftserver_FindTrans(transId))) {
	    code = FTSERVER_ERR_NOTRANS;
	    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_GET_STATUS,
		       ICL_TYPE_LONG, 1, ICL_TYPE_LONG, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}
	FTSCHECK(transp, FTSERVER_OP_GETSTATUS, code);
	if (code) {
	    if (transp->t_returnCode == 0) transp->t_returnCode = code;
	    ftserver_PutTrans(transp);
	    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_GET_STATUS,
		       ICL_TYPE_LONG, 2, ICL_TYPE_LONG, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}

	code = ftu_FsetGetStatus(transp->t_vDesc, &volStatus);
	if (!code) {
	    status_lclToNet(&volStatus, statusp);
	    transp->t_states = volStatus.vol_st.states;
	    statusp->vss.accError = 0;	/* ensure that it doesn't look busy. */
	}
	transp->t_states = volStatus.vol_st.states;
	if (transp->t_returnCode == 0) transp->t_returnCode = code;
	ftserver_PutTrans(transp);
    }
    else {
	/* don't log simple access denials */
	if (code != DAUT_ERROR_ACCESS_DENIED) {
	    Log(tsv_s_vprocs_CheckAuthorization_failed,
		routineName, dfs_dceErrTxt(code));
	}
	icl_Trace2(iclSetP, FTSERVER_TRACE_AUTHFAIL,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, code);
	bzero((char *)statusp, sizeof(ftserver_status));
    }
    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_GET_STATUS,
	       ICL_TYPE_LONG, 3, ICL_TYPE_LONG, code);
    return vols_CancelsRestore(routineName, cancelState, code);
}


/*
 * SetStatus.
 */
long SFTSERVER_SetStatus(callp, transId, mask, statusp, spare1)
    rpc_binding_handle_t callp;
    unsigned long transId;
    unsigned long mask;
    struct ftserver_status *statusp;
    unsigned long spare1;
{
    register struct ftserver_trans *transp;
    struct vol_status status;
    register long code;
    register unsigned long setmask;
    int cancelState;
    static char	routineName[] = "SFTSERVER_SetStatus";

    vols_CancelsBlock(routineName, &cancelState);
    icl_Trace4(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_SET_STATUS,
	       ICL_TYPE_POINTER, callp, ICL_TYPE_LONG, transId,
	       ICL_TYPE_LONG, mask, ICL_TYPE_POINTER, statusp);
    if ((code = dfsauth_server_CheckAuthorization(&callp, (dacl_permset_t *)NULL,
						   (char *)NULL,
						   (uuid_t *)NULL)) == 0) {
	if (!(transp = ftserver_FindTrans(transId)))   {
	    code = FTSERVER_ERR_NOTRANS;
	    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_SET_STATUS,
		       ICL_TYPE_LONG, 1, ICL_TYPE_LONG, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}
	FTSCHECK(transp, FTSERVER_OP_SETSTATUS, code);
	if (code) {
	    if (transp->t_returnCode == 0) transp->t_returnCode = code;
	    ftserver_PutTrans(transp);
	    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_SET_STATUS,
		       ICL_TYPE_LONG, 2, ICL_TYPE_LONG, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}

	/*
	 * if the mask is zero, we assume all fields are to be set
	     */
	bzero((char *)&status, sizeof(struct vol_status));
	if (mask == 0)
	    mask = (long)(-1);

	/* Have to translate from one structure to another. */

	setmask = 0;

	if (mask & VOL_STAT_VOLNAME) {
	    strcpy(status.vol_st.volName, (char*)statusp->vss.volName);
	    setmask |= VOL_STAT_VOLNAME;
	}

	if (mask & VOL_STAT_VOLID) {
	    status.vol_st.volId = statusp->vss.volId;
	    setmask |= VOL_STAT_VOLID;
	    /* special protection for setting of the volume-ID */
	    FTSCHECK(transp, FTSERVER_OP_SETSTATUS_ID, code);
	    if (code) {
		if (transp->t_returnCode == 0) transp->t_returnCode = code;
		ftserver_PutTrans(transp);
		icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_SET_STATUS,
			   ICL_TYPE_LONG, 3, ICL_TYPE_LONG, code);
		return vols_CancelsRestore(routineName, cancelState, code);
	    }
	}

	if (mask & VOL_STAT_VERSION) {
	    status.vol_dy.volversion = statusp->vsd.volversion;
	    setmask |= VOL_STAT_VERSION;
	}

	if (mask & VOL_STAT_UNIQUE) {
	    status.vol_dy.unique = statusp->vsd.unique;
	    setmask |= VOL_STAT_UNIQUE;
	}

	if (mask & VOL_STAT_OWNER) {
	    status.vol_dy.owner = statusp->vsd.owner;
	    setmask |= VOL_STAT_OWNER;
	}

	if (mask & VOL_STAT_TYPE) {
	    status.vol_st.type = statusp->vss.type;
	    setmask |= VOL_STAT_TYPE;
	}

	if (mask & VOL_STAT_STATES) {
	    status.vol_st.states = statusp->vss.states;
	    setmask |= VOL_STAT_STATES;
	}

	if (mask & VOL_STAT_BACKUPID) {
	    status.vol_dy.backupId = statusp->vsd.backupId;
	    setmask |= VOL_STAT_BACKUPID;
	}

	if (mask & VOL_STAT_PARENTID) {
	    status.vol_st.parentId = statusp->vss.parentId;
	    setmask |= VOL_STAT_PARENTID;
	}

	if (mask & VOL_STAT_CLONEID) {
	    status.vol_dy.cloneId = statusp->vsd.cloneId;
	    setmask |= VOL_STAT_CLONEID;
	}

	if (mask & VOL_STAT_CREATEDATE) {
	    status.vol_dy.creationDate = statusp->vsd.creationDate;
	    setmask |= VOL_STAT_CREATEDATE;
	}

	if (mask & VOL_STAT_UPDATEDATE) {
	    status.vol_dy.updateDate = statusp->vsd.updateDate;
	    setmask |= VOL_STAT_UPDATEDATE;
	}

	if (mask & VOL_STAT_ACCESSDATE) {
	    status.vol_dy.accessDate = statusp->vsd.accessDate;
	    setmask |= VOL_STAT_ACCESSDATE;
	}

	if (mask & VOL_STAT_COPYDATE) {
	    status.vol_dy.copyDate = statusp->vsd.copyDate;
	    setmask |= VOL_STAT_COPYDATE;
	}

	if (mask & VOL_STAT_ALLOCLIMIT) {
	    status.vol_dy.allocLimit = statusp->vsd.allocLimit;
	    setmask |= VOL_STAT_ALLOCLIMIT;
	}

	if (mask & VOL_STAT_VISLIMIT) {
	    status.vol_dy.visQuotaLimit = statusp->vsd.visQuotaLimit;
	    setmask |= VOL_STAT_VISLIMIT;
	}

	if (mask & VOL_STAT_MINQUOTA) {
	    status.vol_dy.minQuota = statusp->vsd.minQuota;
	    setmask |= VOL_STAT_MINQUOTA;
	}

	if (mask & VOL_STAT_BACKVOLINDEX) {
	    status.vol_dy.backupIndex = statusp->vsd.backupIndex;
	    setmask |= VOL_STAT_BACKVOLINDEX;
	}

	if (mask & VOL_STAT_STATUSMSG) {
	    bcopy((char *)statusp->vsd.statusMsg, status.vol_dy.statusMsg,
		  sizeof(statusp->vsd.statusMsg));
	    setmask |= VOL_STAT_STATUSMSG;
	}

	if (mask & VOL_STAT_VVCURRTIME) {
	    status.vol_st.vvCurrentTime = statusp->vss.vvCurrentTime;
	    setmask |= VOL_STAT_VVCURRTIME;
	}

	if (mask & VOL_STAT_VVPINGCURRTIME) {
	    status.vol_st.vvPingCurrentTime = statusp->vss.vvPingCurrentTime;
	    setmask |= VOL_STAT_VVPINGCURRTIME;
	}

	if (mask & VOL_STAT_BACKUPDATE) {
	    status.vol_dy.backupDate = statusp->vsd.backupDate;
	    setmask |= VOL_STAT_BACKUPDATE;
	}

	if (mask & VOL_STAT_RECLAIMDALLY) {
	    status.vol_st.reclaimDally = statusp->vss.reclaimDally;
	    setmask |= VOL_STAT_RECLAIMDALLY;
	}

	if (mask & VOL_STAT_LLBACKID) {
	    status.vol_dy.llBackId = statusp->vsd.llBackId;
	    setmask |= VOL_STAT_LLBACKID;
	}

	if (mask & VOL_STAT_LLFWDID) {
	    status.vol_dy.llFwdId = statusp->vsd.llFwdId;
	    setmask |= VOL_STAT_LLFWDID;
	}

	if (mask & VOL_STAT_VOLMOVETIMEOUT) {
	    status.vol_st.volMoveTimeout = statusp->vss.volMoveTimeout;
	    setmask |= VOL_STAT_VOLMOVETIMEOUT;
	}

	if (mask & VOL_STAT_NODEMAX) {
	    status.vol_dy.nodeMax = statusp->vsd.nodeMax;
	    setmask |= VOL_STAT_NODEMAX;
	}

	code = ftu_FsetSetStatus(transp->t_vDesc, setmask, &status);
	if (!code) {
	    ftserver_KeepTransAlive(transp, 0);
	    if (setmask & VOL_STAT_STATES) {
		transp->t_states = status.vol_st.states;
	    }
	    code = ftu_FsetSync(transp->t_vDesc, FTU_FSET_SYNC_COMMIT_STATUS);
	}
	if (transp->t_returnCode == 0) transp->t_returnCode = code;

	ftserver_PutTrans(transp);
    }
    else {
	/* don't log simple access denials */
	if (code != DAUT_ERROR_ACCESS_DENIED) {
	    Log(tsv_s_vprocs_CheckAuthorization_failed,
		routineName, dfs_dceErrTxt(code));
	}
	icl_Trace2(iclSetP, FTSERVER_TRACE_AUTHFAIL,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, code);
    }
    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_SET_STATUS,
	       ICL_TYPE_LONG, 4, ICL_TYPE_LONG, code);
    return vols_CancelsRestore(routineName, cancelState, code);
}


/*
 * ListVolumes: List all specified attached volumes on the server. Doesn't take a transaction id, so don't have to worry about fileset opens.
 */
long SFTSERVER_ListVolumes(callp, aggrId, inCookieP, outCookieP, statEntries)
    rpc_binding_handle_t callp;
    unsigned long aggrId;
    ftserver_iterator *inCookieP, *outCookieP;
    ftserver_statEntries *statEntries;
{
    long code, inlen, outlen, code1, code2, bigcode, vDesc;
    struct io_rwDesc iodesc;
    register int ix;
    struct vol_status vsArray[FTSERVER_MAXSTATENTRIES];
    register struct vol_status *vsp;
    register struct ftserver_status *fsp;
    struct vol_AgOpen agopen;
    int cancelState;
    static char	routineName[] = "SFTSERVER_ListVolumes";

    vols_CancelsBlock(routineName, &cancelState);
    icl_Trace3(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_LIST_VOLUMES,
	       ICL_TYPE_POINTER, callp, ICL_TYPE_LONG, aggrId,
	       ICL_TYPE_LONG, inCookieP->index);
    statEntries->ftserver_status_len = 0;
    if ((code = dfsauth_server_CheckAuthorization(&callp,
						   (dacl_permset_t *)NULL,
						   (char *)NULL,
						   (uuid_t *)NULL)) == 0) {
	inlen = sizeof(vsArray);

	iodesc.descId = aggrId;
	iodesc.position = inCookieP->index;
	iodesc.length = inlen;
	iodesc.bufferp = (char *) &vsArray[0];
	code = AG_VOLENUMERATE(FTSERVER_MAXSTATENTRIES, &iodesc,
			       &outCookieP->index, &outlen);
	if (code == ENODEV) code = FTSERVER_ERR_BADAGGREGATE;
	if (code == 0 || code == VOL_ERR_EOF) {
	    statEntries->ftserver_status_len = (outlen / sizeof(struct vol_status));
	    bigcode = 0;
	    for (ix = 0; ix < statEntries->ftserver_status_len; ++ix) {
		vsp = &vsArray[ix];
		fsp = &statEntries->ftserver_status_val[ix];
		bzero((char *)&agopen, sizeof(struct vol_AgOpen));
		agopen.volId = vsp->vol_st.volId;
		agopen.agId = aggrId;
		agopen.accStat = VOL_SYS_GETSTATUS;
		agopen.accErr = VOLERR_TRANS_LIST; /* list filesets */
		code1 = VOL_AGOPEN(&agopen, &vDesc, &code2);
		if (code1 != 0) {
		    /* get basic info out */
		    status_lclToNet(vsp, fsp);
		    if (code1 == EBUSY && code2 != 0) code1 = code2;	/* wide code */
		    Log(tsv_s_vprocs_log_listVolumes,
			AFS_HGETBOTH(vsp->vol_st.volId), code1);
		    if (verbose)
			dce_svc_printf(TSV_S_VPROCS_LOG_LISTVOLUMES_MSG,
				       AFS_HGETBOTH(vsp->vol_st.volId), code1);
		    /* ensure that the caller sees the problem */
		    fsp->vss.accError = osi_errEncode(code1);	/* guaranteed non-zero */
		    code1 = 0;	/* go on to the next one */
		} else {
		    ftutil_ThreadYield();
		    code1 = VOL_GETSTATUS(vDesc, vsp);
		    status_lclToNet(vsp, fsp);
		    fsp->vss.accError = 0;	/* ensure it doesn't look busy */
		    if (code1 || (vsp->vol_st.states & VOL_DELONSALVAGE) != 0)
			ftu_AggrAbortFsetOpen(vDesc);
		    else
			code1 = ftu_AggrCloseFset(vDesc);
		}
		if (code1 != 0) bigcode = code1;	/* pick up any error */
		ftutil_ThreadYield();
	    }
	    if (bigcode != 0)
		code = bigcode;	/* propagate any error back */
	}
    } else {
	/* don't log simple access denials */
	if (code != DAUT_ERROR_ACCESS_DENIED) {
	    Log(tsv_s_vprocs_CheckAuthorization_failed,
		routineName, dfs_dceErrTxt(code));
	}
	icl_Trace2(iclSetP, FTSERVER_TRACE_AUTHFAIL,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, code);
	bzero((char *)outCookieP, sizeof(ftserver_iterator));
	bzero((char *)statEntries, sizeof(ftserver_statEntries));
    }
    icl_Trace3(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_LIST_VOLUMES,
	       ICL_TYPE_LONG, code, ICL_TYPE_LONG, outCookieP->index,
	       ICL_TYPE_LONG, statEntries->ftserver_status_len);
    return vols_CancelsRestore(routineName, cancelState, code);
}

/*
 * ListAggregates: List all attached aggregates on the server.
 */
long SFTSERVER_ListAggregates(callp, inCookieP, outCookieP, aggrList)
     rpc_binding_handle_t callp;
     ftserver_iterator *inCookieP, *outCookieP;
     ftserver_aggrEntries *aggrList;
{
    long code, inlen, outlen, bytesUsed, maxAggr;
    static char	routineName[] = "SFTSERVER_ListAggregates";
    struct aggrList *listp;
    register int Ix, Ctr;
    int cancelState;

    vols_CancelsBlock(routineName, &cancelState);
    icl_Trace2(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_LIST_AGGREGATES,
	       ICL_TYPE_POINTER, callp, ICL_TYPE_LONG, inCookieP->index);

    bzero((char *)outCookieP, sizeof(*outCookieP));
    aggrList->ftserver_aggrList_len = 0;
    /* First get the number of attached aggregates */
    /* xaggr/ag_init.c stores the requisite space as an out parameter */
    code = AG_ENUMERATE(sizeof(bytesUsed), &bytesUsed, &outlen);
    if (code == 0) {
	goto enable;	/* nothing to return */
    } else if (errno != E2BIG) {
	code = errno;	/* save it */
	Log(tsv_s_vprocs_log_ag_enumerate_failed,
	    routineName, dfs_dceErrTxt(code));
	goto enable;
    }
    for (;;) {
	/* Now allocate (at least) the necessary storage */
	maxAggr = (bytesUsed / sizeof(struct aggrList)) + 5;	/* allow for creation race */
	inlen = maxAggr * sizeof(struct aggrList);
	listp = (struct aggrList *) osi_Alloc(inlen);
	if (listp == 0) {
	  code = ENOMEM;
	  goto enable;
	}
	code = AG_ENUMERATE(inlen, listp, &bytesUsed);
	if (code == 0) break;	/* Got a good list! */
	if (errno == E2BIG) {
	    /* Lost the race.  Try again. */
	    bytesUsed = *((long *) listp);
	    osi_Free((opaque) listp, inlen);
	} else {
	    /* Unexpected failure; return it */
	    code = errno;
	    osi_Free((opaque) listp, inlen);
	    goto enable;
	}
    }
    maxAggr = bytesUsed / sizeof(struct aggrList);
    /* We now have a list of all the maxAggr aggregates on the server in listp */
    /* Return some slice of them */
    for (Ix = inCookieP->index, Ctr = 0;
	  Ix < maxAggr && Ctr < FTSERVER_MAXAGGR;
	  ++Ix, ++Ctr) {
	/* copy from aggr_syscall structure to RPC structure */
	bcopy(listp[Ix].name,
	      (char *)aggrList->ftserver_aggrEntries_val[Ctr].name,
	      sizeof(aggrList->ftserver_aggrEntries_val[Ctr].name));
	bcopy(listp[Ix].dvname,
	      (char *)aggrList->ftserver_aggrEntries_val[Ctr].dvname,
	      sizeof(aggrList->ftserver_aggrEntries_val[Ctr].dvname));
	aggrList->ftserver_aggrEntries_val[Ctr].Id = listp[Ix].Id;
	aggrList->ftserver_aggrEntries_val[Ctr].type = listp[Ix].type;
	aggrList->ftserver_aggrEntries_val[Ctr].spare1 = listp[Ix].spare1;
	aggrList->ftserver_aggrEntries_val[Ctr].spare2 = listp[Ix].spare2;
	aggrList->ftserver_aggrEntries_val[Ctr].spare3 = listp[Ix].spare3;
	aggrList->ftserver_aggrEntries_val[Ctr].spare4 = listp[Ix].spare4;
    }
    aggrList->ftserver_aggrList_len = Ctr;
    outCookieP->index = Ix;
    osi_Free((opaque) listp, inlen);

  enable:
    icl_Trace3(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_LIST_AGGREGATES,
	       ICL_TYPE_LONG, code, ICL_TYPE_LONG, outCookieP->index,
	       ICL_TYPE_LONG, aggrList->ftserver_aggrList_len);
    return vols_CancelsRestore(routineName, cancelState, code);
}


/*
 * AggregateInfo: List info about particular aggregate.
 */
long SFTSERVER_AggregateInfo(callp, aggrId, aggrInfop)
    rpc_binding_handle_t callp;
    unsigned long aggrId;
    struct ftserver_aggrInfo *aggrInfop;
{
    register long code;
    static char	routineName[] = "SFTSERVER_AggregateInfo";
    int cancelState;

    vols_CancelsBlock(routineName, &cancelState);
    icl_Trace2(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_AGGREGATE_INFO,
	       ICL_TYPE_POINTER, callp, ICL_TYPE_LONG, aggrId);

    code =  ftu_AggrGetInfo(aggrId, (struct aggrInfo *)aggrInfop);
    if (code == ENOENT) code = FTSERVER_ERR_BADAGGREGATE;

    icl_Trace4(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_AGGREGATE_INFO,
	       ICL_TYPE_LONG, code, ICL_TYPE_STRING, aggrInfop->name,
	       ICL_TYPE_STRING, aggrInfop->devName,
	       ICL_TYPE_LONG, aggrInfop->type);
    icl_Trace3(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_AGGREGATE_INFO2,
	       ICL_TYPE_LONG, aggrInfop->totalUsable,
	       ICL_TYPE_LONG, aggrInfop->curFree,
	       ICL_TYPE_LONG, aggrInfop->minFree);
    return vols_CancelsRestore(routineName, cancelState, code);
}


/*
 * Volser Monitor
 */
long SFTSERVER_Monitor(callp, entriesp, numberEntries)
     rpc_binding_handle_t callp;
     ftserver_transEntries *entriesp;
{
    ftserver_transStatus *tstatusp;
    struct ftserver_trans *transp;
    register long code = 0;
    int cancelState;
    static char	routineName[] = "SFTSERVER_Monitor";

    vols_CancelsBlock(routineName, &cancelState);
    icl_Trace1(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_MONITOR, 
	       ICL_TYPE_POINTER, callp);
    if ((code = dfsauth_server_CheckAuthorization(&callp, (dacl_permset_t *)NULL,
						   (char *)NULL, (uuid_t *)NULL)) == 0) {
	tstatusp = entriesp->ftserver_transStatus_val;
	entriesp->ftserver_tranStatus_len = 0;
	for (transp = ftserver_trans; transp; transp = transp->t_next) {
	    bzero((char *)tstatusp, sizeof(ftserver_transStatus));
	    tstatusp->transId = transp->t_transId;
	    tstatusp->aggrId = transp->t_aggrId;
	    tstatusp->volId = transp->t_volId;
	    tstatusp->vDesc = transp->t_vDesc;	
	    tstatusp->lastTime = transp->t_lastTime;
	    tstatusp->creationTime = transp->t_creationTime;
	    tstatusp->returnCode = transp->t_returnCode;
	    tstatusp->states = transp->t_states;
	    tstatusp->refCount = transp->t_refCount;
	    tstatusp->accStatus = transp->t_accStatus;
	    tstatusp->accError = transp->t_accError;
	    tstatusp->baseType = transp->t_baseType;
	    tstatusp->flags = transp->t_flags;
	    entriesp->ftserver_tranStatus_len++;
	    if (FTSERVER_MAXTRANSENTRIES <= entriesp->ftserver_tranStatus_len)
		break;
	    tstatusp++;
	}
    }
    else {
	/* don't log simple access denials */
	if (code != DAUT_ERROR_ACCESS_DENIED) {
	    Log(tsv_s_vprocs_CheckAuthorization_failed,
		routineName, dfs_dceErrTxt(code));
	}
	icl_Trace2(iclSetP, FTSERVER_TRACE_AUTHFAIL,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, code);
	bzero((char *)entriesp, sizeof(ftserver_transEntries));
    }
    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_MONITOR,
	       ICL_TYPE_LONG, code,
	       ICL_TYPE_LONG, entriesp->ftserver_tranStatus_len);
    return vols_CancelsRestore(routineName, cancelState, code);
}


/*
 * Get one volume status, without opening a transaction.
 */
long SFTSERVER_GetOneVolStatus(callp, volIdp, aggrId, spare1, statusP)
    rpc_binding_handle_t callp;
    afs_hyper_t *volIdp;
    u_long aggrId;
    u_long spare1;
    ftserver_status *statusP;
{
    long code, code2, vDesc;
    struct vol_status volStatus;
    struct vol_AgOpen agopen;
    static char	routineName[] = "SFTSERVER_GetOneVolStatus";
    int cancelState;

    vols_CancelsBlock(routineName, &cancelState);
    icl_Trace4(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_GET_ONE_VOL_STATUS,
	       ICL_TYPE_POINTER, callp, ICL_TYPE_HYPER, volIdp,
	       ICL_TYPE_LONG, aggrId, ICL_TYPE_POINTER, statusP);

    code = ftu_AggrGetFsetStatus(aggrId, volIdp, &volStatus);
    if (code) {
	if (verbose > 1)
	    dce_svc_printf(TSV_S_VPROCS_FTU_AGGRGETFSETSTATUS_CODE_MSG , code);
	icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_GET_ONE_VOL_STATUS,
		   ICL_TYPE_LONG, 1, ICL_TYPE_LONG, code);
	ftutil_ThreadYield();
	return vols_CancelsRestore(routineName, cancelState, code);
    }
    ftutil_ThreadYield();
    status_lclToNet(&volStatus, statusP);
    statusP->vss.accError = 0;	/* ensure it doesn't look busy */

    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_GET_ONE_VOL_STATUS,
	       ICL_TYPE_LONG, 2, ICL_TYPE_LONG, code);
    return vols_CancelsRestore(routineName, cancelState, code);
}


/*
 * Get interfaces supported by this server.
 */
long SFTSERVER_GetServerInterfaces(callp, serverInterfacesP)
rpc_binding_handle_t callp;
dfs_interfaceList *serverInterfacesP;
{
    error_status_t st;
    static char	routineName[] = "SFTSERVER_GetServerInterfaces";
    int cancelState;

    vols_CancelsBlock(routineName, &cancelState);
    icl_Trace1(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_GET_SERVER_INTERFACES,
	       ICL_TYPE_POINTER, callp);
    dfs_GetServerInterfaces(FTSERVER_v4_0_s_ifspec, serverInterfacesP, &st);
    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_GET_SERVER_INTERFACES,
	       ICL_TYPE_LONG, st,
	       ICL_TYPE_LONG, serverInterfacesP->dfs_interfaceList_len);
    return vols_CancelsRestore(routineName, cancelState, st);
}


/*
 * FixChain()
 */
PRIVATE long
FixChain(fsetStatusP, statusArray, numStatusDescsP)
    IN struct vol_status*	fsetStatusP;
    IN vol_statusDesc_t*	statusArray;
    INOUT unsigned*		numStatusDescsP;
{
    long			code = 0;
    vol_statusDesc_t*		curDescP;
    unsigned			i;
    struct ftserver_trans	*tmpTransp;

    if (!AFS_hiszero(fsetStatusP->vol_dy.llBackId)) {
	i = (*numStatusDescsP)++;
	curDescP = &statusArray[i];

	tmpTransp = ftserver_FindTransById(&fsetStatusP->vol_dy.llBackId);
	if (tmpTransp)
	    curDescP->vsd_volDesc = tmpTransp->t_vDesc;
	else {
	    code = ftu_AggrOpenFset(fsetStatusP->vol_dy.aggrId,
				    &fsetStatusP->vol_dy.llBackId,
				    VOL_SYS_SETSTATUS, VOLERR_TRANS_SETSTATUS,
				    &curDescP->vsd_volDesc);
	    if (code)
		return code;
	}

	curDescP->vsd_mask = VOL_STAT_LLFWDID;
	curDescP->vsd_status.vol_dy.llFwdId = fsetStatusP->vol_st.volId;
    }

    if (!AFS_hiszero(fsetStatusP->vol_dy.llFwdId)) {
	i = (*numStatusDescsP)++;
	curDescP = &statusArray[i];

	tmpTransp = ftserver_FindTransById(&fsetStatusP->vol_dy.llFwdId);
	if (tmpTransp)
	    curDescP->vsd_volDesc = tmpTransp->t_vDesc;
	else {
	    code = ftu_AggrOpenFset(fsetStatusP->vol_dy.aggrId,
				    &fsetStatusP->vol_dy.llFwdId,
				    VOL_SYS_SETSTATUS, VOLERR_TRANS_SETSTATUS,
				    &curDescP->vsd_volDesc);
	    if (code)
		return code;
	}

	curDescP->vsd_mask = VOL_STAT_LLBACKID;
	curDescP->vsd_status.vol_dy.llBackId = fsetStatusP->vol_st.volId;
    }

    return code;
}	/* FixChain() */

/*
 * Swap IDs of two volumes.
 */
long SFTSERVER_SwapIDs(callp, transIdA, transIdB, spare1, spare2p)
    rpc_binding_handle_t callp;
    unsigned long transIdA;
    unsigned long transIdB;
    unsigned long spare1;
    unsigned long *spare2p;
{
    register struct ftserver_trans *trans1p, *trans2p;
    register long code;
    int cancelState;
    static char routineName[] = "SFTSERVER_SwapIDs";
    unsigned		i;
    unsigned		numStatusDescs = 2;
    struct vol_status*	status1P;
    struct vol_status*	status2P;
    vol_statusDesc_t	statusArray[6];
    ftu_fsetId_t	tmpId;
    char		tmpName[VOLNAMESIZE + 1];

    vols_CancelsBlock(routineName, &cancelState);
    *spare2p = 0;
    if (verbose) dce_svc_printf(TSV_S_VPROCS_SWAPIDS_MSG, transIdA, transIdB);
    icl_Trace3(iclSetP, FTSERVER_TRACE_ENTER_SFTSERVER_SWAP_IDS,
	       ICL_TYPE_POINTER, callp, ICL_TYPE_LONG, transIdA,
	       ICL_TYPE_LONG, transIdB);
    if ((code = dfsauth_server_CheckAuthorization(&callp, (dacl_permset_t *)NULL,
						   (char *)NULL, (uuid_t *)NULL)) == 0) {
	if (!(trans1p = ftserver_FindTrans(transIdA))) {
	    code = FTSERVER_ERR_NOTRANS;
	    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_SWAP_IDS,
		       ICL_TYPE_LONG, 1, ICL_TYPE_LONG, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}
	FTSCHECK(trans1p, FTSERVER_OP_SWAPIDS, code);
	if (code) {
	    if (trans1p->t_returnCode == 0) trans1p->t_returnCode = code;
	    ftserver_PutTrans(trans1p);
	    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_SWAP_IDS,
		       ICL_TYPE_LONG, 2, ICL_TYPE_LONG, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}
	if (!(trans2p = ftserver_FindTrans(transIdB))) {
	    ftserver_PutTrans(trans1p);
	    code = FTSERVER_ERR_NOTRANS;
	    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_SWAP_IDS,
		       ICL_TYPE_LONG, 3, ICL_TYPE_LONG, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}
	FTSCHECK(trans2p, FTSERVER_OP_SWAPIDS, code);
	if (code) {
	    if (trans2p->t_returnCode == 0) trans2p->t_returnCode = code;
	    ftserver_PutTrans(trans1p);
	    ftserver_PutTrans(trans2p);
	    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_SWAP_IDS,
		       ICL_TYPE_LONG, 4, ICL_TYPE_LONG, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}

	status1P = &statusArray[0].vsd_status;
	status2P = &statusArray[1].vsd_status;

	code = ftu_FsetGetStatus(trans1p->t_vDesc, status1P);

	if (!code)
	    code = ftu_FsetGetStatus(trans2p->t_vDesc, status2P);

	if (!code) {
	    statusArray[0].vsd_volDesc = trans1p->t_vDesc;
	    statusArray[0].vsd_mask = VOL_STAT_VOLID|VOL_STAT_VOLNAME;
	    statusArray[1].vsd_volDesc = trans2p->t_vDesc;
	    statusArray[1].vsd_mask = VOL_STAT_VOLID|VOL_STAT_VOLNAME;
	    
	    tmpId = status1P->vol_st.volId;
	    status1P->vol_st.volId = status2P->vol_st.volId;
	    status2P->vol_st.volId = tmpId;
	    
	    bcopy(status1P->vol_st.volName, tmpName, sizeof tmpName);
	    tmpName[sizeof tmpName - 1] = '\0';
	    bcopy(status2P->vol_st.volName, status1P->vol_st.volName,
		  sizeof status1P->vol_st.volName);
	    status1P->vol_st.volName[sizeof status1P->vol_st.volName-1] = '\0';
	    bcopy(tmpName, status2P->vol_st.volName,
		  sizeof status2P->vol_st.volName);
	    status2P->vol_st.volName[sizeof status2P->vol_st.volName-1] = '\0';
	    
	    code = FixChain(status1P, statusArray, &numStatusDescs);
	}

	if (!code)
	    code = FixChain(status2P, statusArray, &numStatusDescs);

	if (!code)
	    code = ftu_FsetBulkSetStatus(numStatusDescs, statusArray);

	for (i = 2; i < numStatusDescs; i++) {
	    struct ftserver_trans *tmpTransp;

	    tmpTransp = ftserver_FindTransByDesc(statusArray[i].vsd_volDesc);

	    if (tmpTransp) {
		ftserver_PutTrans(tmpTransp);
		ftserver_PutTrans(tmpTransp);
	    } else {
		if (!code)
		    code = ftu_AggrCloseFset(statusArray[i].vsd_volDesc);
		else
		    ftu_AggrAbortFsetOpen(statusArray[i].vsd_volDesc);
	    }
	}

	if (code) {
	    if (trans1p->t_returnCode == 0) trans1p->t_returnCode = code;
	    if (trans2p->t_returnCode == 0) trans2p->t_returnCode = code;
	    ftserver_PutTrans(trans1p);
	    ftserver_PutTrans(trans2p);
	    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_SWAP_IDS,
		       ICL_TYPE_LONG, 5, ICL_TYPE_LONG, code);
	    return vols_CancelsRestore(routineName, cancelState, code);
	}

	/* The t_volId/t_vDesc fields are still in synch. */
	ftserver_SwapTransStates(trans1p, trans2p);

	/* Try to sync the changes back to disk */
	VOL_SYNC(trans1p->t_vDesc, VOL_SYNC_COMMITSTATUS);
	VOL_SYNC(trans2p->t_vDesc, VOL_SYNC_COMMITSTATUS);

	ftserver_PutTrans(trans1p);
	ftserver_PutTrans(trans2p);
    }
    else {
	/* don't log simple access denials */
	if (code != DAUT_ERROR_ACCESS_DENIED) {
	    Log(tsv_s_vprocs_CheckAuthorization_failed,
		routineName, dfs_dceErrTxt(code));
	}
	icl_Trace2(iclSetP, FTSERVER_TRACE_AUTHFAIL,
		   ICL_TYPE_STRING, routineName,
		   ICL_TYPE_LONG, code);
    }
    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_SFTSERVER_SWAP_IDS,
	       ICL_TYPE_LONG, 6, ICL_TYPE_LONG, code);
    if (verbose) dce_svc_printf(TSV_S_VPROCS_SWAPID_LEAVING_MSG,code);
    return vols_CancelsRestore(routineName, cancelState, code);
}


/*
 * LOCAL FUNCTIONS
 */

/* called by the thread that is handling the getting of data for the
    remote SFTSERVER_Restore */

void forwardRestoreAlloc(state, bsize, bufP, outSizeP)
    void *state;
    idl_ulong_int bsize;
    idl_byte **bufP;
    idl_ulong_int *outSizeP;
{
    localPipeAlloc(state, bsize, bufP, outSizeP);
}

void forwardPull(state, buf, inSize, outSizeP)
    void *state;
    idl_byte *buf;
    idl_ulong_int inSize;
    idl_ulong_int *outSizeP;
{
    afsPipe_t *pipeP = (afsPipe_t *)state;

    icl_Trace3(iclSetP, FTSERVER_TRACE_ENTER_FORWARD_PULL,
	       ICL_TYPE_POINTER, state, ICL_TYPE_POINTER, buf,
	       ICL_TYPE_LONG, inSize);
    localPipePull(state, buf, inSize, outSizeP);
    if (verbose) dce_svc_printf(TSV_S_VPROCS_FOWARDPULL_READ_MSG, *outSizeP);
    icl_Trace1(iclSetP, FTSERVER_TRACE_EXIT_FORWARD_PULL,
	       ICL_TYPE_LONG, *outSizeP);
    return;
}

/* called by thread that is doing dump for vols_Forward */

struct forwardWriterArgs {
    afsPipe_t *pipeP;
    struct ftserver_Date *dumpDatep;
    struct ftserver_trans *fromTransp;
};

pthread_startroutine_t forwardWriter(argsP)
     struct forwardWriterArgs *argsP;
{
      volatile long code;
      register afsPipe_t *pipeP;
      register struct ftserver_Date *dumpDateP;
      register struct ftserver_trans *fromTransP;
      ftu_dumpCriteria_t	dumpCriteria;
      volatile int cancelState;
      char *excNameP;
      char excBuff[50];
      static char routineName[] = "forwardWriter";
      struct transarray tar;
      extern char * ExceptionToString();
      
      vols_CancelsBlock(routineName, &cancelState);
      if (!argsP) {
	  icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_FORWARD_WRITER,
		     ICL_TYPE_LONG, 1, ICL_TYPE_LONG, 0);
	  (void) vols_CancelsRestore(routineName, cancelState, 0);
	  pthread_exit((pthread_addr_t)0);
      }

      pipeP = argsP->pipeP;
      dumpDateP = argsP->dumpDatep;
      fromTransP = argsP->fromTransp;

      icl_Trace1(iclSetP, FTSERVER_TRACE_ENTER_FORWARD_WRITER,
		 ICL_TYPE_POINTER, pipeP);
      icl_Trace4(iclSetP, FTSERVER_TRACE_ENTER_FORWARD_WRITER2,
		 ICL_TYPE_LONG, dumpDateP->mask,
		 ICL_TYPE_LONG, dumpDateP->fromDate.sec,
		 ICL_TYPE_LONG, dumpDateP->fromDate.usec,
		 ICL_TYPE_HYPER, &dumpDateP->fromVersion);
      icl_Trace4(iclSetP, FTSERVER_TRACE_ENTER_FORWARD_WRITER3,
		 ICL_TYPE_LONG, fromTransP->t_transId,
		 ICL_TYPE_LONG, fromTransP->t_aggrId,
		 ICL_TYPE_HYPER, &fromTransP->t_volId,
		 ICL_TYPE_LONG, fromTransP->t_vDesc);

      /* child */
      if (verbose) dce_svc_printf(TSV_S_VPROCS_CHILD_IN_FORWARD_MSG);
      TRY {
	  dumpDateToDumpCriteria(dumpDateP, &dumpCriteria);
	  tar.count = 1;
	  tar.transes[0] = fromTransP;
	  code = ftu_FsetDumpToPipeWithKA(fromTransP->t_vDesc,
					  &dumpCriteria, pipeP,
					  vols_AliveTranses, (void *)&tar);
	  POP_CLOSE(pipeP, PFL_WRITE);/* close this end of pipe */
      } CATCH_ALL {
	    excNameP = ExceptionToString(THIS_CATCH);
	    if (excNameP == NULL) {
		sprintf(excBuff, "[[unk: %#lx]]", THIS_CATCH);
		excNameP = excBuff;
	    }
	    Log(tsv_s_vprocs_log_exception_while_dumping,
		routineName, excNameP);
	    icl_Trace3(iclSetP, FTSERVER_TRACE_EXCEPTION,
		       ICL_TYPE_STRING, routineName,
		       ICL_TYPE_LONG, code,
		       ICL_TYPE_STRING, excNameP);
	    code = EPIPE;
      } ENDTRY
      code = vols_CancelsRestore(routineName, cancelState, code);
      pthread_exit((pthread_addr_t)code);
}


vols_Forward(fromtransp, callp, fromtransId, dumpDatep, destp, restoreFlags, totransId)
     struct ftserver_trans * volatile fromtransp;
     rpc_binding_handle_t callp;
     long fromtransId;
     struct ftserver_Date *dumpDatep;
     struct ftserver_dest *destp;
     volatile unsigned long restoreFlags;
     volatile long totransId;
{
    volatile long	code;
    long	code2;
    long	dfsauthCode;
    volatile pipe_t restoreDataPipe;
    volatile afsPipe_t localPipe;
    volatile afsPipe_t sockPipe;
    volatile long sockFlag;
    ncsPipeData_t ncsOutPipe;
    volatile pthread_t dumperThread;
    pthread_attr_t dumperAttr;
    struct forwardWriterArgs * volatile dumperArgsP = NULL;
    volatile rpc_binding_handle_t newCall;
#if !defined(OSF_NO_SOCKET_DUMP)
    srvrSock_t *srvrSockP;
#endif /* OSF_NO_SOCKET_DUMP */
    char *excNameP;
    char excBuff[50];
    error_status_t st;
    volatile int retryOK, retryCount;
    char pNameBuf[1024];
    extern char localCellName[];
    extern char * ExceptionToString();
    static char	routineName[] = "vols_Forward";

    icl_Trace4(iclSetP, FTSERVER_TRACE_ENTER_VOLS_FORWARD,
	       ICL_TYPE_LONG, fromtransp->t_transId,
	       ICL_TYPE_LONG, fromtransp->t_aggrId,
	       ICL_TYPE_HYPER, &fromtransp->t_volId,
	       ICL_TYPE_LONG, fromtransp->t_vDesc);
    icl_Trace2(iclSetP, FTSERVER_TRACE_ENTER_VOLS_FORWARD2,
	       ICL_TYPE_POINTER, callp, ICL_TYPE_LONG, fromtransId);
    icl_Trace4(iclSetP, FTSERVER_TRACE_ENTER_VOLS_FORWARD3,
	       ICL_TYPE_LONG, dumpDatep->mask,
	       ICL_TYPE_LONG, dumpDatep->fromDate.sec,
	       ICL_TYPE_LONG, dumpDatep->fromDate.usec,
	       ICL_TYPE_HYPER, &dumpDatep->fromVersion);
    icl_Trace3(iclSetP, FTSERVER_TRACE_ENTER_VOLS_FORWARD4,
	       ICL_TYPE_POINTER, destp, ICL_TYPE_LONG, restoreFlags,
	       ICL_TYPE_LONG, totransId);

    /* check to see if the passed-in address is valid */
    if (destp->destSSID != 1) {
	icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_VOLS_FORWARD,
		   ICL_TYPE_LONG, 1, ICL_TYPE_LONG, EINVAL);
	return EINVAL;
    }

    if (code = CreateLocalPipe(&localPipe, PFL_WRITE)) {
	icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_VOLS_FORWARD,
		   ICL_TYPE_LONG, 2, ICL_TYPE_LONG, code);
	return code;
    }

#if !defined(OSF_NO_SOCKET_DUMP)
    /* check for a socket to use for the data transfer */
    if (fromtransp->t_srvrSockP) {
	/* shut down the local pipe */
	POP_CLOSE((afsPipe_t *)&localPipe, PFL_WRITE);
	/* set up a socket pipe */
	sockFlag = 1;
	srvrSockP = (srvrSock_t *)(fromtransp->t_srvrSockP);
	fromtransp->t_srvrSockP = NULL;
	if (code = createSockPipe(srvrSockP, (afsPipe_t *)&sockPipe,
			          PFL_WRITE)) {
	    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_VOLS_FORWARD,
		       ICL_TYPE_LONG, 3, ICL_TYPE_LONG, code);
	    return(code);
	}
    } else {
	sockFlag = 0;
    }
#else /* OSF_NO_SOCKET_DUMP */
    sockFlag = 0;
#endif /* OSF_NO_SOCKET_DUMP */

    /* now, create the new helper thread */
    dumperArgsP = (struct forwardWriterArgs *)
	osi_Alloc(sizeof(struct forwardWriterArgs));
    if (dumperArgsP == NULL) {
	code = ENOMEM;

#if !defined(OSF_NO_SOCKET_DUMP)
	if (sockFlag) {
	    POP_RELE((afsPipe_t*)&sockPipe);
	}
#endif /* OSF_NO_SOCKET_DUMP */

	POP_RELE((afsPipe_t*)&localPipe);
	icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_VOLS_FORWARD,
		   ICL_TYPE_LONG, 4, ICL_TYPE_LONG, code);
	return code;
    }
#if !defined(OSF_NO_SOCKET_DUMP)
    if (sockFlag) {
	dumperArgsP->pipeP = (afsPipe_t*)&sockPipe;
    } else {
	dumperArgsP->pipeP = (afsPipe_t*)&localPipe;
    }
#else /* OSF_NO_SOCKET_DUMP */
    dumperArgsP->pipeP = (afsPipe_t*)&localPipe;
#endif /* OSF_NO_SOCKET_DUMP */
    dumperArgsP->dumpDatep = dumpDatep;
    dumperArgsP->fromTransp = (struct ftserver_trans *)fromtransp;

    pthread_attr_create(&dumperAttr);
    pthread_attr_setstacksize(&dumperAttr, 10000);
    code = pthread_create((pthread_t *)&dumperThread, dumperAttr, 
	(pthread_startroutine_t)forwardWriter, (pthread_addr_t)dumperArgsP);
    pthread_attr_delete(&dumperAttr);
    if (code != 0) { /* thread creation failed, errno holds the reason */
	code = errno;
	code2 = 0;
	goto done2;
    }

    if (verbose) dce_svc_printf(TSV_S_VPROCS_PARENT_IN_FORWARD_MSG);
    restoreDataPipe.alloc = forwardRestoreAlloc;
    restoreDataPipe.state = (void *)&localPipe;
    restoreDataPipe.push = 0;
    restoreDataPipe.pull = forwardPull;

    /*
     * We must create a new connection to the target server!
     *
     * the destination is passed to us, though!
     */

    rpcx_binding_from_sockaddr((struct sockaddr *)&destp->address, 
		(rpc_binding_handle_t *)&newCall);

    /* set local auth */
    sprintf(pNameBuf, "%s/%s/dfs-server", localCellName, destp->prinName);
    code = dfsauth_client_InitBindingAuth((rpc_binding_handle_t *)&newCall,
						0, 1, pNameBuf);
    if (code) {
	rpc_binding_free((rpc_binding_handle_t *)&newCall, &st);
	goto done;
    }
    rpc_mgmt_set_com_timeout(newCall, rpc_c_binding_default_timeout+3, &st);
    if (st != error_status_ok) {
	Log(tsv_s_vprocs_log_cannot_set_timeout,
	    routineName, dfs_dceErrTxt(st));
	rpc_binding_free((rpc_binding_handle_t *)&newCall, &st);
	code = (long) st;
	goto done;
    }

    for (retryCount = 0, retryOK = 1; retryOK; ++retryCount) {
	retryOK = 0;
	TRY {
	    code = FTSERVER_Restore((rpc_binding_handle_t)newCall, 
			totransId, restoreFlags, (pipe_t *)&restoreDataPipe);
	} CATCH_ALL {
	    excNameP = ExceptionToString(THIS_CATCH);
	    if (excNameP == NULL) {
		sprintf(excBuff, "[[unk: %#lx]]", THIS_CATCH);
		excNameP = excBuff;
	    }
	    Log(tsv_s_vprocs_log_exception_while_restoring,
		routineName, excNameP);
	    icl_Trace3(iclSetP, FTSERVER_TRACE_EXCEPTION,
		       ICL_TYPE_STRING, routineName,
		       ICL_TYPE_LONG, code,
		       ICL_TYPE_STRING, excNameP);
	    code = EPIPE;
	} ENDTRY
	if (code == rpc_s_auth_tkt_expired && retryCount < 3) {
	      code = dfsauth_client_RefreshLocalAuthContext();
	      if (code == 0) retryOK = 1;	/* retry the routine */
	}
    }

    rpc_binding_free((rpc_binding_handle_t *)&newCall, &st);

 done: /* cleanup */
    if (code) {
	/* Ensure that the pipe process doesn't hang up trying to write data
	 that will never be consumed. */
#if !defined(OSF_NO_SOCKET_DUMP)
	if (sockFlag) {
	    POP_CRASH((afsPipe_t*)&sockPipe, code);
	} else {
	    POP_CRASH((afsPipe_t*)&localPipe, code);
	}
#else /* OSF_NO_SOCKET_DUMP */
        POP_CRASH((afsPipe_t*)&localPipe, code);
#endif /* OSF_NO_SOCKET_DUMP */
    }
    code2 = 0;
    if (pthread_join(dumperThread, (pthread_addr_t *)&code2) != 0) {
	Log(tsv_s_vprocs_log_no_dumpe_thread, routineName, errno);
	if (code == 0) code = errno;
    }
    if (pthread_detach((pthread_t *)&dumperThread) != 0) {
	Log(tsv_s_vprocs_log_detach_dumper, routineName, errno);
	if (code == 0) code = errno;
    }
 done2:
    /* Get error code from both dump and restore procedures */
    if (code2 != 0) {
	if (code == 0
	    || code == FTSERVER_ERR_DISJOINT_DATE
	    || code == FTSERVER_ERR_DISJOINT_VV
	    || code == FTSERVER_ERR_DISJOINT2_DATE
	    || code == FTSERVER_ERR_DISJOINT2_VV)
	    code = code2;
	else
	    Log(tsv_s_vprocs_log_dumper_errors,
		AFS_HGETBOTH(fromtransp->t_volId), fromtransp->t_aggrId,
		code2, code);
    }
    POP_RELE((afsPipe_t*)&localPipe);
#if !defined(OSF_NO_SOCKET_DUMP)
    if (sockFlag) {
        POP_RELE((afsPipe_t*)&sockPipe);
    }
#endif /* OSF_NO_SOCKET_DUMP */
    if (dumperArgsP != NULL) osi_Free(dumperArgsP, sizeof(struct forwardWriterArgs));
    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_VOLS_FORWARD,
	       ICL_TYPE_LONG, 5, ICL_TYPE_LONG, code);
    return code;
}

/* Copy status information to network form. */
static void status_lclToNet(volstatp, ftstatp)
register struct vol_status *volstatp;
register struct ftserver_status *ftstatp;
{
    /*
      * XXX Boy, wouldn't be nice if we would just do a bcopy of the whole thing
      * instead of individual (slow) assignments... XXX
      */
    bzero((char *)ftstatp, sizeof(struct ftserver_status));
    ftstatp->vss.volId = volstatp->vol_st.volId;
    ftstatp->vss.parentId = volstatp->vol_st.parentId;
    ftstatp->vss.cloneTime = volstatp->vol_st.cloneTime;
    ftstatp->vss.vvCurrentTime = volstatp->vol_st.vvCurrentTime;
    ftstatp->vss.vvPingCurrentTime = volstatp->vol_st.vvPingCurrentTime;
    ftstatp->vss.type = volstatp->vol_st.type;
    ftstatp->vss.accStatus = volstatp->vol_st.accStatus;
    ftstatp->vss.accError = volstatp->vol_st.accError;
    ftstatp->vss.states = volstatp->vol_st.states;
    ftstatp->vss.reclaimDally = volstatp->vol_st.reclaimDally;
    ftstatp->vss.tokenTimeout = volstatp->vol_st.tokenTimeout;
    ftstatp->vss.activeVnops = volstatp->vol_st.activeVnops;
    ftstatp->vss.volMoveTimeout = volstatp->vol_st.volMoveTimeout;
    ftstatp->vss.static_spare4 = volstatp->vol_st.procID;
    ftstatp->vss.static_spare5 = volstatp->vol_st.spare5;
    ftstatp->vss.static_spare6 = volstatp->vol_st.spare6;
    ftstatp->vss.static_spare7 = volstatp->vol_st.spare7;
    ftstatp->vss.static_spare8 = volstatp->vol_st.spare8;
    ftstatp->vss.static_spare9 = volstatp->vol_st.spare9;
    ftstatp->vss.concurrency = volstatp->vol_st.concurrency;
    bcopy(&volstatp->vol_st.cspares[0],
	  (char *)&ftstatp->vss.static_cspares[0],
	  sizeof(volstatp->vol_st.cspares));
    strncpy((char*)ftstatp->vss.volName,
	    volstatp->vol_st.volName,
	    sizeof(ftstatp->vss.volName));
    ftstatp->vsd.creationDate = volstatp->vol_dy.creationDate;
    ftstatp->vsd.updateDate = volstatp->vol_dy.updateDate;
    ftstatp->vsd.accessDate = volstatp->vol_dy.accessDate;
    ftstatp->vsd.backupDate = volstatp->vol_dy.backupDate;
    ftstatp->vsd.copyDate = volstatp->vol_dy.copyDate;
    ftstatp->vsd.volversion = volstatp->vol_dy.volversion;
    ftstatp->vsd.backupId = volstatp->vol_dy.backupId;
    ftstatp->vsd.cloneId = volstatp->vol_dy.cloneId;
    ftstatp->vsd.llFwdId = volstatp->vol_dy.llFwdId;
    ftstatp->vsd.llBackId = volstatp->vol_dy.llBackId;
    ftstatp->vsd.fileCount = volstatp->vol_dy.fileCount;
    ftstatp->vsd.allocLimit = volstatp->vol_dy.allocLimit;
    ftstatp->vsd.allocUsage = volstatp->vol_dy.allocUsage;
    ftstatp->vsd.visQuotaLimit = volstatp->vol_dy.visQuotaLimit;
    ftstatp->vsd.visQuotaUsage = volstatp->vol_dy.visQuotaUsage;
    ftstatp->vsd.minQuota = volstatp->vol_dy.minQuota;
    ftstatp->vsd.owner = volstatp->vol_dy.owner;
    ftstatp->vsd.unique = volstatp->vol_dy.unique;
    ftstatp->vsd.index = volstatp->vol_dy.index;
    ftstatp->vsd.rwindex = volstatp->vol_dy.rwIndex;
    ftstatp->vsd.backupIndex = volstatp->vol_dy.backupIndex;
    ftstatp->vsd.parentIndex = volstatp->vol_dy.parentIndex;
    ftstatp->vsd.cloneIndex = volstatp->vol_dy.cloneIndex;
    ftstatp->vsd.nodeMax = volstatp->vol_dy.nodeMax;
    ftstatp->vsd.dynamic_spare1 = volstatp->vol_dy.aggrId;
    ftstatp->vsd.dynamic_spare2 = volstatp->vol_dy.spare2;
    ftstatp->vsd.dynamic_spare3 = volstatp->vol_dy.spare3;
    ftstatp->vsd.dynamic_spare4 = volstatp->vol_dy.spare4;
    ftstatp->vsd.dynamic_spare5 = volstatp->vol_dy.spare5;
    ftstatp->vsd.dynamic_spare6 = volstatp->vol_dy.spare6;
    ftstatp->vsd.tag = volstatp->vol_dy.tag;
    ftstatp->vsd.msgLen = volstatp->vol_dy.msgLen;
    bcopy(&volstatp->vol_dy.cspares[0],
	  (char *)&ftstatp->vsd.dynamic_cspares[0],
	  sizeof(volstatp->vol_dy.cspares));
    strncpy((char*)ftstatp->vsd.statusMsg,
	  volstatp->vol_dy.statusMsg,
	  sizeof(ftstatp->vsd.statusMsg));
}

/* Get the base type of a fileset.
 * This becomes a type identifier in the dump/restore stuff. */
static long vols_FillBaseType(transp)
struct ftserver_trans *transp;
{
    long code;
    struct aggrInfo agInfo;
    unsigned long subtype;

    icl_Trace4(iclSetP, FTSERVER_TRACE_ENTER_VOLS_FILL_BASE_TYPE,
	       ICL_TYPE_LONG, transp->t_transId,
	       ICL_TYPE_LONG, transp->t_aggrId,
	       ICL_TYPE_HYPER, &transp->t_volId,
	       ICL_TYPE_LONG, transp->t_vDesc);

    code = ftu_AggrGetInfo(transp->t_aggrId, &agInfo);
    if (code != 0) {
	if (code == ENOENT) code = FTSERVER_ERR_BADAGGREGATE;
	icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_VOLS_FILL_BASE_TYPE,
		   ICL_TYPE_LONG, 1, ICL_TYPE_LONG, code);
	return code;
    }
    subtype = 0;
    switch (agInfo.type) {
	case AG_TYPE_EPI:
	case AG_TYPE_VXFS:
	case AG_TYPE_DMEPI:
	    subtype = 1;
	    break;
	case AG_TYPE_UFS:
#ifdef AFS_AIX31_ENV
	    subtype = 1;
#else /* AFS_AIX31_ENV */
#ifdef AFS_OSF_ENV
	    subtype = 2;
#else /* AFS_OSF_ENV */
	    subtype = 3;	/* for now */
#endif /* AFS_OSF_ENV */
#endif /* AFS_AIX31_ENV */
	    break;
    }
    AFS_hset64(transp->t_baseType, agInfo.type, subtype);
    icl_Trace2(iclSetP, FTSERVER_TRACE_EXIT_VOLS_FILL_BASE_TYPE,
	       ICL_TYPE_LONG, 1, ICL_TYPE_LONG, 0);
    return 0;
}

/* Convert the ftserver ops bit vector to a volops bit vector */
static void ftops_to_volops (ftsOps, volOpsP)
unsigned long ftsOps, *volOpsP;
{
    int i, j;

    *volOpsP = 0;
    for (j = 0, i = (sizeof(ftserver_ops_values) / sizeof(ftserver_ops_values[0]));
	  i > 0;
	  --i, ++j) {
	if (ftsOps & 0x1)
	    *volOpsP |= ftserver_ops_values[j];
	ftsOps >>= 1;
    }
    if (verbose) dce_svc_printf(TSV_S_VPROCS_FTOPS_TO_VOLOPS_MSG, *volOpsP);
}
