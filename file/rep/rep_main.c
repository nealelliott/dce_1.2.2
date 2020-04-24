/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rep_main.c,v $
 * $EndLog$
 */
/*
 * (C) Copyright 1996, 1990 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 *
 * rep_main.c
 *	The replication service.
 *	Manages secondary replicas.
 */

#define AFSDEBUG_DECLARE 1		/* Declare AFS-related debug code */

#include <dcedfs/param.h>		/* General system parameters */
#include <dcedfs/osi.h>
#include <dcedfs/basicincludes.h>	/* Another basic include file set */
#include <dcedfs/cmd.h>
#include <dcedfs/osi.h>
#include <dcedfs/lock.h>
#include <dcedfs/tkm_tokens.h>		/* token types */
#include <dcedfs/nubik.h>
#include <dcedfs/afs4int.h>		/* afsFid */
#include <dcedfs/tkn4int.h>		/* tkn-revocation interface */
#include <dcedfs/fldb_data.h>
#include <dcedfs/fldb_proc.h>
#include <dcedfs/flserver.h>		/* flserver interface */
#include <dcedfs/volume.h>
#include <dcedfs/vol_init.h>		/* volser's syscall */
#include <dcedfs/ag_init.h>		/* aggregate syscall */
#include <dcedfs/vol_errs.h>
#include <dcedfs/ftserver.h>
#include <dcedfs/ftserver_trans.h>
#include <dcedfs/ftserver_data.h>
#include <dcedfs/ftserver_proc.h>
#include <dcedfs/xvfs_vnode.h>
#include <dcedfs/compat.h>
#include <dcedfs/ftutil.h>
#include <dcedfs/ftu_syscalls.h>
#include <dcedfs/osi_dfserrors.h>

/* SAMS support */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsrpsmac.h>
#include <dfsrpssvc.h>
#include <dfsrpsmsg.h>

#include <pthread.h>
#include <dcedfs/ktime.h>
#if defined(AFS_HPUX_ENV)
#include <fcntl.h>
#endif /* defined(AFS_HPUX_ENV) */
#include <dce/rpc.h>
#include <dcedfs/osi_dfserrors.h>
#include <dcedfs/fshs_errs.h>
#include <dcedfs/hs_errs.h>
#include <dcedfs/dfsauth.h>
#include <dcedfs/ftutil.h>
#include <dcedfs/rep_errs.h>		/* error code values */
#include <dcedfs/rep_data.h>	/* repser RPC structures and interface */
#include <dcedfs/rep_proc.h>

#include <rep_host.h>
#include <rep_queue.h>			/* concurrent priority queue */
#include <repser.h>			/* internal structures */
#include <rep_trace.h>

#ifdef	_EPISODE_LIC_MGMT_
#include <lic/lic.h>
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/rep/RCS/rep_main.c,v 10.96 1996/10/18 13:09:52 gait Exp $")

/* the thread routines */
static pthread_addr_t BackgroundProcessThread _TAKES((pthread_addr_t));
static pthread_addr_t RepThread _TAKES((pthread_addr_t));
static pthread_addr_t ReaperThread _TAKES((pthread_addr_t));

/* Statistics-keeping */
static u_int32 NumAttns;
static u_int32 NumOversizeAttns;
static u_int32 TotOversizeAttnSizes;
static afs_hyper_t TotOversizeAttnSizeSquares;
static u_int32 NumKAs;
static u_int32 NumOversizeKAs;
static u_int32 TotOversizeKASizes;
static afs_hyper_t TotOversizeKASizeSquares;

/* Periodic calls. */
static u_int32 NextForceKeepAlives;

osi_dlock_t RepTableLock;
struct replica *RepRoot;
int numReplicas;

static rep_queue_t RepQ;
static rep_queue_t DeathQ;

/* XXX guess the appropriate WVT types */
#define MY_WVT_TYPES (TKN_DATA_READ | TKN_STATUS_READ)

#define	ALL_BITS_ON_IN(Zbits, Zvbl) (((Zbits) & (~(Zvbl))) == 0)
#define HAVE_MY_WVT(Xvbl) (ALL_BITS_ON_IN(MY_WVT_TYPES,(Xvbl)))
static afs_hyper_t allTokenTypes;

const char WhoIAm[] = "repserver";

char ErrBuff[250];

static afs_hyper_t zeroDum;

/* init SAMS tables */

DCE_SVC_DEFINE_HANDLE(rps_svc_handle,
                      rps_svc_table,
                      "rps")

void dfs_define_rps_message_tables()
{
      error_status_t st ;

      dce_msg_define_msg_table(rps__table,
                               sizeof rps__table / sizeof rps__table[0],
                               &st);
      if(st != svc_s_ok)
         fprintf(stderr,
	       "Cannot register rps incore message table 0x%lx\n", st);

}

dce_svc_handle_t   rps_svc_handle;

void initialize_svc()
{
  error_status_t st;
  static char programName[] = "repserver";

  rps_svc_handle = dce_svc_register(rps_svc_table,  (idl_char *)"rps",
&st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot register svc 0x%lx\n", st);
  dce_svc_set_progname(programName, &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot set program name 0x%lx\n", st);
  dce_svc_routing((unsigned char *) "NOTICE:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route NOTICE messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "WARNING:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route WARNING messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "ERROR:STDERR:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route ERROR messages  0x%lx\n", st);

  dfs_define_rps_message_tables();

}


/* define event set structure for tracing and setup function */

struct icl_set *rep_iclSetp = 0;

static void printBinding(buf, hdl)
char *buf;
handle_t hdl;
{
    unsigned_char_t *stringBinding;
    unsigned_char_t *uuidstr, *protseq, *netaddr, *endpoint, *options;
    unsigned long st;
    char tmpbuf[512];

    icl_Trace1(rep_iclSetp, REP_BINDING_PRINT,
	       ICL_TYPE_STRING, buf);
    rpc_binding_to_string_binding(hdl, &stringBinding, &st);
    if (st != error_status_ok)  {
	dce_svc_printf(RPS_S_BAD_BINDING_MSG,
		dfs_dceErrTxt(st));
	return;
      }
    rpc_string_binding_parse(stringBinding, &uuidstr, &protseq,
			     &netaddr, &endpoint, &options, &st);
    if (st != error_status_ok) {
	dce_svc_printf(RPS_S_PARSE_ERR_MSG,stringBinding,
		dfs_dceErrTxt(st));
    } else {
	sprintf(tmpbuf, "%s: u '%s', p '%s', n '%s', e '%s', o '%s'",
	       stringBinding, uuidstr, protseq, netaddr, endpoint, options);
	icl_Trace1(rep_iclSetp, REP_BINDING_PRINT, ICL_TYPE_STRING, tmpbuf);
	rpc_string_free(&uuidstr, &st);
	rpc_string_free(&protseq, &st);
	rpc_string_free(&netaddr, &st);
	rpc_string_free(&endpoint, &st);
	rpc_string_free(&options, &st);
    }
    rpc_string_free(&stringBinding, &st);
}

static void printAllBindings(hdr)
char *hdr;
{
    error_status_t		st;
    rpc_binding_vector_t *	bvec;
    int			i;
    char			buff[50];

    rpc_server_inq_bindings(&bvec, &st);
    if (st != error_status_ok)  {
	dce_svc_printf(RPS_S_NO_BINDINGS_MSG,hdr,
		dfs_dceErrTxt(st));
	return;
    }
    icl_Trace2(rep_iclSetp, REP_ALLBINDING_PRINT,
		ICL_TYPE_STRING, hdr,
		ICL_TYPE_LONG, bvec->count);
    for (i = 0; i < bvec->count; i++)  {
	sprintf(buff, "%s [%d]", hdr, i);
	printBinding(buff, bvec->binding_h[i]);
    }
}

/* Arguments */
static int parm_mainProcs = 4;
int parm_tokenProcs = 4;

static char hypermsg[] = "%lu,,%lu: %s";
static char msgonly[] = "%s";
static char msgmsgnl[] = "%s: %s\n";

static void LogPrint(rp)
struct replica *rp;
{
    Log(hypermsg, AFS_HGETBOTH(rp->v.VolIDOfSecondary), rp->RepMsg);
    icl_Trace2(rep_iclSetp, REP_LOGPRINT_VOLMSG,
	       ICL_TYPE_HYPER, &rp->v.VolIDOfSecondary,
	       ICL_TYPE_STRING, rp->RepMsg);
}

static void DebugPrint(rp)
struct replica *rp;
{
    icl_Trace2(rep_iclSetp, REP_LOGPRINT_VOLMSG,
	       ICL_TYPE_HYPER, &rp->v.VolIDOfSecondary,
	       ICL_TYPE_STRING, rp->RepMsg);
}

static void LogErrPrint()
{
    dce_svc_printf(RPS_S_LOG_ERR_MSG,WhoIAm,ErrBuff);
    Log(msgonly, ErrBuff);
}

void RepHold(struct replica* rp)
{
    lock_ObtainWrite(&rp->refLock);
    ++rp->RefCount;
    lock_ReleaseWrite(&rp->refLock);
}	/* RepHold() */

void RepRele(struct replica* rp)
{
    lock_ObtainWrite(&rp->refLock);
    if (rp->RefCount-- <= 1) {
	lock_ReleaseWrite(&rp->refLock);
	(void)repq_Enter(&DeathQ, rp, 0);
    } else {
	lock_ReleaseWrite(&rp->refLock);
    }
}	/* RepRele() */

struct ubik_client *VLDBcstruct;
afs_hyper_t localCellID;
char localCellName[MAXCELLCHARS];
static char localHostName[MAXCELLCHARS];

/* synchronization objects */
static pthread_mutex_t	repserverMutex;
pthread_cond_t	actNowCond;

#if defined(AFS_OSF_ENV)
#define REP_DEFAULT_STACK_SIZE	128000
#else /* AFS_OSF_ENV */
/* use the pthread default stack size for now */
#define REP_DEFAULT_STACK_SIZE	(60 * 512)
#endif /* AFS_OSF_ENV */

#ifdef	_EPISODE_LIC_MGMT_
/*
 * License call back functions.
 */
void EpisodeLicenseError(type, str)
int 	type;
char	*str;
{
    switch (type) {
      case LICENSE_FAILURE: 
	Log("repserver LICENSE FAILURE: %s\n", str);
	break;

      case LICENSE_EXPIRES_SOON:
	Log("repserver LICENSE EXPIRY: %s\n", str);
	break;

      case LICENSE_CHECKIN_NO_LICENSE:
	Log("repserver LICENSE CHECKIN: %s\n", str);
	break;

      case LICENSE_IN_QUEUE:
	Log("repserver LICENSE QUEUE: %s\n", str);
	break;

      default:
	Log("repserver LICENSE EVENT: %s\n", str);
	break;
    }
}

/*
 * This function is only called if a license could not be reobtained
 * after check-in interval elapsed.  This indicates that there is no way
 * to get a license for now.  We log an error and exit.
 */
void callbackOnCheckinFailed(lic_product_t productname, char *version)
{
    osi_assert((productname == LICENSE_DCE_EPISODE));

    Log("%s: Failed to maintain license for %s, exiting.\n",
	WhoIAm, lic_GetProductNameString(productname));
    exit(1);
}

/*
 * Handle various types of events from the license server by
 * causing appropriate message to be logged.
 * These events are not total license failures.  We are called in the license
 * package's thread, so we must return at end.
 */
void callbackOnEvent(lic_status_t 	status,
		     lic_product_t 	productname,
		     char		*version,
		     void 		*arg)
{
    char	logbuf[256];

    switch (status) {
      case LICENSE_EXPIRES_SOON:
	sprintf(logbuf,"license for %s [%s] expires at %s",
		lic_GetProductNameString(productname), version, arg);
	EpisodeLicenseError(LICENSE_EXPIRES_SOON, logbuf);
	break;

      case LICENSE_CHECKIN_NO_LICENSE:
	sprintf(logbuf,"license check-in failed for %s [%s]",
		lic_GetProductNameString(productname), version);
	EpisodeLicenseError(LICENSE_CHECKIN_NO_LICENSE, logbuf);
	break;

      case LICENSE_IN_QUEUE:
	sprintf(logbuf,"unexpected LICENSE_IN_QUEUE event for %s [%s]",
		lic_GetProductNameString(productname), version);
	EpisodeLicenseError(LICENSE_IN_QUEUE, logbuf);
	break;

      default:
	sprintf(logbuf,"unexpected %d event for %s [%s]",
		status, lic_GetProductNameString(productname), version);
	EpisodeLicenseError(status, logbuf);
	break;
    }
    return;
}
#endif	/*_EPISODE_LIC_MGMT_*/

/* sameCell -- tests whether two cell IDs match.  Returns a boolean result. */

int sameCell(
  afs_hyper_t *cell1p,
  afs_hyper_t *cell2p)
{
    if (AFS_hiszero(*cell1p) || AFS_hcmp64(*cell1p, 0,1) == 0)
	cell1p = &localCellID;
    if (AFS_hiszero(*cell2p) || AFS_hcmp64(*cell2p, 0,1) == 0)
	cell2p = &localCellID;
    return AFS_hsame(*cell1p, *cell2p);
}

static int initClients()
{
    int32 code, i;
    unsigned long st;
    char *cellNameP = 0;
    char *hostNameP = 0;
    pthread_mutexattr_t	mutexAttr;

    /* first, get everybody's attribute values set up */

    /* create & init attr values needed by the mutex */
    if (pthread_mutexattr_create(&mutexAttr) != 0) {
	sprintf(ErrBuff, "error creating mutex attribute during init; errno = %d", errno);
	LogErrPrint();
	exit(2);
    }
    /* the global mutex will probably have to be a friendly mutex */
    if (pthread_mutexattr_setkind_np(&mutexAttr, MUTEX_RECURSIVE_NP) != 0) {
	sprintf(ErrBuff, "error setting MUTEX_RECURSIVE_NP during init; errno = %d", errno);
	LogErrPrint();
	exit(2);
    }

    /* now, actually create the synchronization & pthread objects */
    if (pthread_mutex_init(&repserverMutex, mutexAttr) != 0) {
	sprintf(ErrBuff, "%s: error initializing repserver global mutex; errno = %d", errno);
	LogErrPrint();
	exit(2);
    }
    if (pthread_cond_init(&actNowCond, pthread_condattr_default) != 0) {
	sprintf(ErrBuff, "error initializing new restart time condition variable; errno = %d",
		errno);
	LogErrPrint();
	exit(2);
    }

    /* hold the global repserver mutex until other initialization is complete. */
    if (pthread_mutex_lock(&repserverMutex) != 0) {
	sprintf(ErrBuff, "error locking repserver global mutex during init; errno = %d", errno);
	LogErrPrint();
	exit(2);
    }
    icl_Trace0(rep_iclSetp, REP_TRACE_UNINIT_MUTEX);
    /* now reclaim the space used by the prototype attributes objects */
    if (pthread_mutexattr_delete(&mutexAttr) != 0) {
	sprintf(ErrBuff, "error deleting mutex attribute during init; errno = %d", errno);
	LogErrPrint();
	exit(2);
    }

    /* Ensure we'll be able to do volops without deadlocks. */
    ftutil_ImagePrep();

    dce_cf_get_cell_name(&cellNameP, &st);
    if ((st != error_status_ok) || (cellNameP == 0) || (cellNameP[0] != '/')) {
	sprintf(ErrBuff, "Can't determine the local cell name: %s.  Exiting.",
		dfs_dceErrTxt(st));
	LogErrPrint();
	exit(2);
    } else {
	/* successfully got cell name */
	strcpy(localCellName, cellNameP);
    }
    if (cellNameP != 0) free(cellNameP);
    dce_cf_get_host_name(&hostNameP, &st);
    if ((st != error_status_ok) || (hostNameP == 0) || (hostNameP[0] == '\0')) {
	sprintf(ErrBuff, "Can't determine the local host name: %s.  Exiting.",
		dfs_dceErrTxt(st));
	LogErrPrint();
	exit(2);
    } else {
	/* successfully got cell name */
	strcpy(localHostName, hostNameP);
    }
    if (hostNameP != 0) free(hostNameP);
    return 0;
}

/* forward references */
globalref   REP_v4_0_epv_t		REP_v4_0_manager_epv;
globalref   TKN4Int_v4_0_epv_t	TKN4Int_v4_0_manager_epv;

static int initServers()
{
    error_status_t st;
    static unsigned_char_t protSeq[] = "ncadg_ip_udp";
    boolean32 validProtSeq;
    pthread_attr_t threadAttr;
    static char routineName[] = "initServers";
    unsigned_char_t *stringBinding;
    register int i;
    uuid_t nil_uuid;
    pthread_t dummyTid;

    /* Advertise our own two exported RPC services. */

    /* create & init attr values needed by the threads */
    if (pthread_attr_create(&threadAttr) != 0) {
	sprintf(ErrBuff, "error creating thread attribute during init; errno = %d", errno);
	LogErrPrint();
	exit(2);
    }
    if (pthread_attr_setstacksize(&threadAttr, REP_DEFAULT_STACK_SIZE) != 0) {
	sprintf(ErrBuff,
		"error setting stack size to %lu in thread attribute during init; errno = %d",
		REP_DEFAULT_STACK_SIZE, errno);
	LogErrPrint();
	exit(2);
    }

    /* spawn all the non-RPC repserver threads */
    for (i = 0; i < parm_mainProcs; ++i) {
	if (pthread_create(&dummyTid, threadAttr, RepThread, NULL) != 0){
	    sprintf(ErrBuff,
		    "error creating replica thread %d of %d; errno = %d",
		    i+1, parm_mainProcs, errno);
	    LogErrPrint();
	    exit(2);
	}
    }

    if (pthread_create(&dummyTid, threadAttr,
		       BackgroundProcessThread, (pthread_addr_t)NULL) != 0) {
	sprintf(ErrBuff, "error creating background thread; errno = %d",errno);
	LogErrPrint();
	exit(2);
    }

    if (pthread_create(&dummyTid, threadAttr,
		       ReaperThread, (pthread_addr_t)NULL) != 0) {
	sprintf(ErrBuff, "error creating reaper thread; errno = %d",errno);
	LogErrPrint();
	exit(2);
    }

    if (pthread_attr_delete(&threadAttr) != 0) {
	sprintf(ErrBuff, "error deleting thread attribute during init; errno = %d", errno);
	LogErrPrint();
	exit(2);
    }

    rpc_mgmt_set_server_stack_size(REP_DEFAULT_STACK_SIZE, &st);
    if (st != rpc_s_ok){
	sprintf(ErrBuff, "repserver: can't set RPC server stack size to %lu: %lu",
		REP_DEFAULT_STACK_SIZE, st);
	LogErrPrint();
	exit(2);
    }

    dfs_installInterfaceDescription(REP_v4_0_s_ifspec, REP_v4_0_s_ifspec,
				     1 /* provider_version number */,
				     (unsigned_char_t *)"Transarc RepServer Interface", &st);
    if (st != 0) {
	sprintf(ErrBuff, "repserver: can't install interface description: %lu", st);
	LogErrPrint();
	exit(2);
    }

    /* register the fact that we are exporting the token manager interface */
    icl_Trace2(rep_iclSetp, REP_TRACE_DFS_REG_BEGIN,
		  ICL_TYPE_STRING, WhoIAm,
		  ICL_TYPE_STRING, routineName);
    uuid_create_nil(&nil_uuid, (unsigned32 *)&st);
    if (st != uuid_s_ok) {
      sprintf(ErrBuff, "unable to create a nil UUID: %s", dfs_dceErrTxt(st));
      LogErrPrint();
      return 1;
    }
    dfs_register_rpc_server(TKN4Int_v4_0_s_ifspec,
			    (rpc_mgr_epv_t) &TKN4Int_v4_0_manager_epv,
			    &nil_uuid, &nil_uuid,
			    parm_tokenProcs,
			    "admin.rep",
			    "DFS repserver tkn4",
			    &st);
    icl_Trace2(rep_iclSetp, REP_TRACE_DFS_REG_END, 
		ICL_TYPE_STRING, WhoIAm,
		ICL_TYPE_STRING, routineName);
    if (st != error_status_ok) {
	sprintf(ErrBuff, "unable to register interface: %s", dfs_dceErrTxt(st));
	LogErrPrint();
	return 1;
    }

    rep_initTokens();	/* gets the local port for token revocations */
    reph_Init(localCellName, revocationEpoch, &revocationNetData, rep_iclSetp);

    /* register the fact that we are exporting the repserver interface */
    icl_Trace2(rep_iclSetp, REP_TRACE_REP_REG_BEGIN, 
		ICL_TYPE_STRING, WhoIAm,
		ICL_TYPE_STRING, routineName);
    dfs_register_rpc_server(REP_v4_0_s_ifspec,
			    (rpc_mgr_epv_t) &REP_v4_0_manager_epv,
			    &nil_uuid, &nil_uuid,
			    parm_tokenProcs,
			    "admin.rep",
			    "DFS repserver rep",
			    &st);
    icl_Trace2(rep_iclSetp, REP_TRACE_REP_REG_END, 
		ICL_TYPE_STRING, WhoIAm,
		ICL_TYPE_STRING, routineName);
    if (st != error_status_ok) {
	sprintf(ErrBuff, "unable to register interface: %s", dfs_dceErrTxt(st));
	LogErrPrint();
	return 1;
    }

    printAllBindings("after reg TKN and REP");

    rep_startTokens();	/* spawns listener for token revocations */
    icl_Trace0(rep_iclSetp, REP_TRACE_MUTEX_REL);

    /* release the global repserver mutex */
    if (pthread_mutex_unlock(&repserverMutex) != 0) {
	sprintf(ErrBuff, "error unlocking repserver global mutex during init; errno = %d", errno);
	LogErrPrint();
	exit(2);
    }

    while (1) {
	select(0, 0, 0, 0, 0);
    }
    /* never reached: */
    /* return 0; */
}

/*VARARGS3*/
static u_long setNextTime(rp, intrv, format, s1)
register struct replica *rp;
u_long intrv;
char *format;
{/* Do some of the bookkeeping associated with recording a failure condition. */
    struct timeval atv;

    osi_GetTime(&atv);
    atv.tv_sec += intrv;
    rp->nextLookTime = atv;
    if (format != NULL) {
	sprintf(rp->RepMsg, format, s1);
	icl_Trace3(rep_iclSetp, REP_TRACE_DELAY, 
		   ICL_TYPE_HYPER, &rp->v.VolIDOfSecondary,
		   ICL_TYPE_LONG, intrv, 
		   ICL_TYPE_STRING, rp->RepMsg);
    }
    return (atv.tv_sec);
}

/*VARARGS4*/
static u_long setTokenTime(rp, intrv, timep, format, s1)
register struct replica *rp;
u_long intrv;
register struct timeval *timep;
char *format;
{/* Do some of the bookkeeping associated with recording a failure condition. */
    u_long When, expCeiling;

    When = timep->tv_sec + intrv;
    expCeiling = rp->TimeOfWVTExpiration.tv_sec - 2;
    if (When > expCeiling) When = expCeiling;
    if (rp->hostP != NULL) {
	pthread_mutex_lock(&rp->hostP->rh_fsMutex);
	expCeiling = rp->hostP->rh_fsLastSuccess
	  + rp->hostP->rh_fsHostLifeGuarantee
	  - rp->hostP->rh_fsHostLeadTime;
	pthread_mutex_unlock(&rp->hostP->rh_fsMutex);
	if (When > expCeiling) When = expCeiling;
    }
    if (format != NULL) {
	sprintf(rp->RepMsg, format, s1);
	icl_Trace3(rep_iclSetp, REP_TRACE_RENEW, 
		   ICL_TYPE_HYPER, &rp->v.VolIDOfSecondary,
		   ICL_TYPE_LONG, intrv, 
		   ICL_TYPE_STRING, rp->RepMsg);
    }
  return (When);
}

struct replica *NewReplicaStruct()
{/* Allocate, initialize, and return a new ``struct replica''. */
    register struct replica *rp;

    rp = (struct replica *) osi_Alloc(sizeof(struct replica));
    if (rp == NULL) return NULL;
    bzero((char *)rp, sizeof(struct replica));
    rp->Next = NULL;
    lock_Init(&rp->refLock);
    rp->RefCount = 0;
    lock_Init(&(rp->lockObject));
    rp->v.Reps = NULL;
    osi_GetTime(&rp->TimeOfLatestKnownCurrent);
    rp->TimeOfLatestKnownCurrent.tv_sec -= (90*24*60*60);
    rp->remoteBusyWait = 5*60;	/* default */
    rp->kaLeadTime = 10*60;
    rp->tokenLeadTime = 10*60;
    rp->memoryWait = 3*60;
    rp->localDiskWait = 2*60;
    rp->CellId = localCellID;
    return (rp);
}

static void NoticeIfDeleted(rp, lv, code)
register struct replica *rp;
register struct localvol *lv;
register long code;
{/* React to a class of VOL_AGOPEN failures.  If this replica's ``lv'' is
  * gone, arrange to construct a new one in the future.
  */
    if (code == ENODEV || code == EXDEV || code == FTU_E_NO_SUCH_FSET) {
	/* Fileset zapped or maybe aggregate off-line */
	lv->flags |= lvf_Deleted;
	/* Account for the side-effects of a detach/attach cycle */
	lv->itsStates &= ~VOL_HAS_TOKEN;
	lv->diskCurr.usec = 0;
	lv->diskPingCurr.sec = 0;
	lv->diskPingCurr.usec = 0;
	lv->diskTknExp.sec = 0;
	lv->diskTknExp.usec = 0;
	/* Now get the scheduler to start me from the beginning soon. */
	repq_ResetDeadline(&RepQ, rp->qCookie, 0);
    }
}

static void SetLatestCurrentTime(rp, lvp, lossTimeP)
register struct replica *rp;
register struct localvol *lvp;
struct timeval *lossTimeP;
{/*
 * Set the TimeOfLatestKnownCurrent time to now if we currently have what
 * amounts to knowledge that the source VV is the same as the local VV.  Do this
 * because we're about to do something that may or may not clear that knowledge.
 * We don't want to advance the stored TimeOfLatestKnownCurrent unless we know we
 * currently have that knowledge.
 */
    if (lvp == NULL) {
	/* Try to get the correct LV, to update its status. */
	for (lvp = rp->AllLocals; lvp != NULL; lvp = lvp->Next) {
	    if (AFS_hsame(rp->v.VolIDOfSecondary, lvp->volId)) break;
	}
    }
    if (lvp != NULL
	&& AFS_hsame(rp->v.VolIDOfSecondary, lvp->volId)
	&& HAVE_MY_WVT(AFS_hgetlo(rp->WVT_TypesLeft))
	&& !AFS_hiszero(rp->VVOnSource)
	&& (lvp->flags & lvf_GotStatus)
	&& AFS_hsame(rp->VVOnSource, lvp->volVersion)) {
	if (lossTimeP)
	    rp->TimeOfLatestKnownCurrent = *lossTimeP;
	else
	    osi_GetTime(&rp->TimeOfLatestKnownCurrent);
    }
}

void CopyLVStatus(lvp, volstp)
register struct localvol *lvp;
register struct vol_status *volstp;
{/* Copy the relevant information from vol_status to localvol. */
    lvp->itsStates = volstp->vol_st.states;
    lvp->volVersion = volstp->vol_dy.volversion;
    lvp->llBackId = volstp->vol_dy.llBackId;
    lvp->llFwdId = volstp->vol_dy.llFwdId;
    lvp->diskCurr = volstp->vol_st.vvCurrentTime;
    lvp->flags |= lvf_GotStatus;
}

static int32 GetLVStatus(rp, lv, volstp)
register struct replica *rp;
register struct localvol *lv;
register struct vol_status *volstp;
{/* Get the status for the local volume described by *lv. */
    long code;
    struct vol_status volstat;

    icl_Trace0(rep_iclSetp, REP_TRACE_GETLV_AGOPEN);
    code = ftu_AggrGetFsetStatus(lv->aggr, &lv->volId, &volstat);
    if (code != 0) {
	sprintf(rp->RepMsg, "GetLVStatus: can't get status for local vol %lu,,%lu: %s",
		AFS_HGETBOTH(lv->volId), dfs_dceErrTxt(code));
	LogPrint(rp);
    } else {
	CopyLVStatus(lv, &volstat);
	if (volstp != NULL) *volstp = volstat;
	sprintf(rp->RepMsg, "GetLVStatus: Got states for LV %lu,,%lu: %#lx; returning.",
			      AFS_HGETBOTH(lv->volId), lv->itsStates);
	DebugPrint(rp);
    }
    return (code);
}

osi_dlock_t FreeIDLock;
static int FreeIDInitialized = 0;
static struct freeVolID *freeIDs;
static long numAllocVolIDs = 0;
static long numUsedVolIDs = 0;
static long numReusedVolIDs = 0;

static struct freeVolID *getFreeVolID(rp)
register struct replica *rp;
{/* Allocate an unused volume ID. */
    register struct freeVolID *fvp;
    long code;

    if (FreeIDInitialized == 0) {
	lock_Init(&FreeIDLock);
	freeIDs = NULL;
	FreeIDInitialized = 1;
    }
    lock_ObtainWrite(&FreeIDLock);
    fvp = freeIDs;
    if (fvp != NULL) {
	freeIDs = fvp->Next;
	fvp->wasReused = 1;
	++numReusedVolIDs;
    } else {
	fvp = (struct freeVolID *) osi_Alloc(sizeof(struct freeVolID));
	if (fvp != NULL) {
	    code = ubik_Call(VL_GetNewVolumeId, VLDBcstruct, 0, 1,
			     &myNetAddrs[goodMyNetAddrs], &fvp->ID);
	    if (code != 0) {
		sprintf(rp->RepMsg, "getFreeVolID: could not get a new vol ID: %s/%d",
			dfs_dceErrTxt(code), code);
		Log(hypermsg,
		    AFS_HGETBOTH(rp->v.VolIDOfSecondary), rp->RepMsg);
		/* fprintf(stderr, msgmsgnl, WhoIAm, rp->RepMsg); */
		osi_Free((opaque) fvp, sizeof(struct freeVolID));
		fvp = NULL;
	    } else {
		fvp->wasReused = 0;
		++numAllocVolIDs;
		++numUsedVolIDs;
	    }
	} else {
	    strcpy(rp->RepMsg, "getFreeVolID: no mem");
	    LogPrint(rp);
	}
    }
    lock_ReleaseWrite(&FreeIDLock);
    return fvp;
}

static void freeFreeVolID(fvp)
struct freeVolID *fvp;
{/* Free a volume ID for local re-use. */
    lock_ObtainWrite(&FreeIDLock);
    fvp->Next = freeIDs;
    freeIDs = fvp;
    if (fvp->wasReused)
	--numReusedVolIDs;
    else
	--numUsedVolIDs;
    lock_ReleaseWrite(&FreeIDLock);
}

static void claimFreeVolID(idp)
register afs_hyper_t *idp;
{/* Add a freed ID for local use. */
    register struct freeVolID *fvp;
    long code;

    if (FreeIDInitialized == 0) {
	lock_Init(&FreeIDLock);
	freeIDs = NULL;
	FreeIDInitialized = 1;
    }
    lock_ObtainWrite(&FreeIDLock);
    /* sanity check */
    for (fvp = freeIDs; fvp != NULL; fvp = fvp->Next) {
	if (AFS_hsame(*idp, fvp->ID)) {
	    lock_ReleaseWrite(&FreeIDLock);
	    return;
	}
    }
    fvp = (struct freeVolID *) osi_Alloc(sizeof(struct freeVolID));
    if (fvp == NULL) {
	lock_ReleaseWrite(&FreeIDLock);
	return;
    }
    fvp->ID = *idp;
    fvp->Next = freeIDs;
    freeIDs = fvp;
    ++numAllocVolIDs;	/* a little bogus */
    lock_ReleaseWrite(&FreeIDLock);
}

int32 AddVLDBEntry(vlentp, vldbP, mySiteP)
register struct vldbentry *vlentp;
register struct vldbRepInfo *vldbP;
int32 *mySiteP;
{/* Use the raw VLDB information at *vlentp.  Write it in caller's vldbRepInfo struct. */
/* Return 0 for all-OK, >0 for no such entry, <0 for transient failures. */
    int32 MySite;
    register int32 Ix, Ix2;
    register int32 RepCount, cnt;
    int roIx, rwIx, rwSiteIx, stageSiteIx, anyRO;
    u_int32 roFlag, rwFlag;
    register int siteCount;
    int siteIx[MAXNSERVERS], addrCount[MAXNSERVERS];

    bzero((char *)vldbP, sizeof(struct vldbRepInfo));
    strncpy((char *)vldbP->mainVolName, (char *)vlentp->name,
	     (MAXFTNAMELEN > VOLNAMESIZE ? VOLNAMESIZE : MAXFTNAMELEN));
    roIx = rwIx = -1;
    for (Ix = 0; Ix < MAXVOLTYPES && !AFS_hiszero(vlentp->VolIDs[Ix]); ++Ix) {
	if (vlentp->VolTypes[Ix] == VOLTIX_TO_VOLTYPE(RWVOL)) {
	    vldbP->VolIDOfPrimary = vlentp->VolIDs[Ix];
	    rwIx = Ix;
	} else if (vlentp->VolTypes[Ix] == VOLTIX_TO_VOLTYPE(ROVOL)) {
	    vldbP->VolIDOfSecondary = vlentp->VolIDs[Ix];
	    roIx = Ix;
	} else if (vlentp->VolTypes[Ix] == VOLTIX_TO_VOLTYPE(BACKVOL)) {
	    vldbP->VolIDOfBackup = vlentp->VolIDs[Ix];
	}
    }
    vldbP->vlflags = vlentp->flags;
    if (!(vlentp->flags & VLF_RWEXISTS)) return REP_ERR_NOTREPLICA;
    if (rwIx < 0 || roIx < 0) return REP_ERR_NOTREPLICA;
    if ((vlentp->flags & VLF_LAZYREP) != VLF_LAZYREP)
	return REP_ERR_UNKNOWNREPLICA;
    roFlag = ((unsigned)VLSF_ZEROIXHERE >> roIx);
    rwFlag = ((unsigned)VLSF_ZEROIXHERE >> rwIx);
    vldbP->maxTotalLatency = vlentp->maxTotalLatency;
    vldbP->hardMaxTotalLatency = vlentp->hardMaxTotalLatency;
    if (vlentp->flags & VLF_LAZYRELE) {
	vldbP->minimumPounceDally = 0;
    } else {
	vldbP->minimumPounceDally = vlentp->minimumPounceDally;
    }
    vldbP->reclaimDally = vlentp->reclaimDally;
    /* Find out where the site boundaries are. */
    siteCount = 0;
    bzero((char *)siteIx, sizeof(siteIx));
    bzero((char *)addrCount, sizeof(addrCount));
    for (Ix = 0; Ix < vlentp->nServers; ++Ix) {
	if (vlentp->siteFlags[Ix] & VLSF_DEFINED) {
	    if (vlentp->siteFlags[Ix] & VLSF_SAMEASPREV && Ix > 0) {
		++addrCount[siteCount-1];
	    } else {
		siteIx[siteCount] = Ix;
		addrCount[siteCount] = 1;
		++siteCount;
	    }
	}
    }
    /* Find the R/W site. */
    rwSiteIx = -1;
    for (Ix = 0; Ix < siteCount; ++Ix) {
	if ((vlentp->siteFlags[siteIx[Ix]] & (rwFlag | VLSF_DEFINED))
	    == (rwFlag | VLSF_DEFINED)) {
	    RepCount = addrCount[Ix];
	    if (RepCount > ADDRSPERSITE) RepCount = ADDRSPERSITE;
	    for (Ix2 = 0; Ix2 < RepCount; ++Ix2) {
		vldbP->PrimaryAddrs[Ix2] = vlentp->siteAddr[siteIx[Ix]+Ix2];
	    }
	    vldbP->NumPrimaryAddrs = RepCount;
	    vldbP->PrimaryAggregate = vlentp->sitePartition[siteIx[Ix]];
	    strncpy((char *)&vldbP->hName[0], 
		    (char *)&vlentp->sitePrincipal[siteIx[Ix]].text[0],
		    (unsigned int)sizeof(vldbP->hName));
	    rwSiteIx = Ix;
	    break;
	}
    }
    /* Complain if there's no R/W site */
    if (rwSiteIx < 0) {
	Log("No R/W site for fileset %s!", vlentp->name);
	icl_Trace2(rep_iclSetp, REP_TRACE_NO_RW_ID, 
		   ICL_TYPE_STRING, WhoIAm,
		   ICL_TYPE_STRING, vlentp->name);
	return REP_ERR_NOTREPLICA;
    }
    /* Look for the staging replica, if this is release-style replication. */
    /* This assumes that any given site is listed with its first server address always first. */
    stageSiteIx = -1;
    anyRO = 0;
    vldbP->StageAggregate = (u_int32)-1;
    if (vlentp->flags & VLF_LAZYRELE) {
	if ((vlentp->siteFlags[siteIx[rwSiteIx]] & (roFlag | VLSF_DEFINED))
	    == (roFlag | VLSF_DEFINED)) {
	    stageSiteIx = rwSiteIx;	/* Actually the same FL server index */
	} else {
	    /* Maybe another server index on same site */
	    for (Ix = 0; Ix < siteCount; ++Ix) {
		if (((vlentp->siteFlags[siteIx[Ix]] & (roFlag | VLSF_DEFINED))
		     == (roFlag | VLSF_DEFINED))
		    && (vlentp->sitePartition[siteIx[Ix]] == vldbP->PrimaryAggregate)
		    && SameAddress(&vlentp->siteAddr[siteIx[Ix]],
				   &vldbP->PrimaryAddrs[0])) {
		    stageSiteIx = Ix;	/* on same (server, aggr) as R/W */
		    break;
		}
	    }
	}
	if (stageSiteIx < 0) {	/* not found yet on same site */
	    for (Ix = 0; Ix < siteCount; ++Ix) {
		if (((vlentp->siteFlags[siteIx[Ix]] & (roFlag | VLSF_DEFINED))
		     == (roFlag | VLSF_DEFINED))
		    && SameAddress(&vlentp->siteAddr[siteIx[Ix]],
				   &vldbP->PrimaryAddrs[0])) {
		    stageSiteIx = Ix;	/* on same server, diff aggr as R/W */
		    break;
		}
	    }
	}
	/* Complain if this is a release-replication volume and there's no staging replica */
	if (stageSiteIx < 0) {
	    anyRO = 0;
	    for (Ix = 0; Ix < siteCount; ++Ix) {
		if ((vlentp->siteFlags[siteIx[Ix]] & (roFlag | VLSF_DEFINED))
		     == (roFlag | VLSF_DEFINED)) {
		    anyRO = 1;
		    break;
		}
	    }
	    if (anyRO != 0) {/* Don't complain unless there are indeed R/O sites */
		Log("Release-style replicated fileset %s, rw=%lu,,%lu, ro=%lu,,%lu, with no R/O on primary!",
		    vlentp->name,
		    AFS_HGETBOTH(vldbP->VolIDOfPrimary),
		    AFS_HGETBOTH(vldbP->VolIDOfSecondary));
		icl_Trace3(rep_iclSetp, REP_TRACE_NO_RO_PRIMARY_RELEASE,
			   ICL_TYPE_STRING, vlentp->name,
			   ICL_TYPE_HYPER, &vldbP->VolIDOfPrimary,
			   ICL_TYPE_HYPER, &vldbP->VolIDOfSecondary);
	    }
	} else {
	    vldbP->StageAggregate = vlentp->sitePartition[siteIx[Ix]];
	}
    }
    vldbP->Reps = (struct RepSite *) osi_Alloc(siteCount * sizeof(struct RepSite));
    if (vldbP->Reps == (struct RepSite *) 0) return REP_ERR_NOSTORAGE;
    vldbP->MaxReps = siteCount;
    RepCount = 0;
    /* Now fill in the addresses of all the replicas, not using the staging replica, if any. */
    for (Ix = 0; Ix < siteCount; ++Ix) {
	if (Ix != stageSiteIx
	    && ((vlentp->siteFlags[siteIx[Ix]] & (roFlag | VLSF_DEFINED))
		== (roFlag | VLSF_DEFINED))) {
	    cnt = addrCount[Ix];
	    if (cnt > ADDRSPERSITE) cnt = ADDRSPERSITE;
	    for (Ix2 = 0; Ix2 < cnt; ++Ix2) {
		vldbP->Reps[RepCount].AddrInfo[Ix2] =
		    vlentp->siteAddr[siteIx[Ix]+Ix2];
	    }
	    if (vlentp->flags & VLF_LAZYRELE) {
		vldbP->Reps[RepCount].ReplicaLatency = vldbP->maxTotalLatency/4;
	    } else {
		vldbP->Reps[RepCount].ReplicaLatency =
		  vlentp->sitemaxReplicaLatency[siteIx[Ix]];
	    }
	    vldbP->Reps[RepCount].ReplicaAggregate = vlentp->sitePartition[siteIx[Ix]];
	    vldbP->Reps[RepCount].NumAddrs = cnt;
	    ++RepCount;
	}
    }
    vldbP->NumReps = RepCount;	/* might have shrunk. */
    MySite = -1;	/* leave -1 if this is primary or we're not in the replica list. */
    for (Ix = 0; Ix < RepCount; ++Ix) {
	if (OnThisMachine(&(vldbP->Reps[Ix].AddrInfo[0]), vldbP->Reps[Ix].NumAddrs)) {
	    MySite = Ix; break;
	}
    }
    *mySiteP = MySite;
    icl_Trace4(rep_iclSetp, REP_TRACE_VLDB_ENTRY_DONE, 
	       ICL_TYPE_HYPER, &vlentp->VolIDs[roIx],
	       ICL_TYPE_STRING, vlentp->name,
	       ICL_TYPE_LONG, RepCount, 
	       ICL_TYPE_LONG, MySite);
    return (0);
}

static int32 VLDBLookup(cellP, volidP, vldbP, MySiteP, codep)
afs_hyper_t *cellP, *volidP;
register struct vldbRepInfo *vldbP;
int32 *MySiteP;
long *codep;
{/* Obtain VLDB information for the given FidP.  Write it in caller's vldbRepInfo struct. */
/* Return 0 for all-OK, >0 for no such entry, <0 for transient failures. */
    int32 Ret;
    struct vldbentry VLDB;

    *codep = 0;
    bzero((char *)vldbP, sizeof(struct vldbRepInfo));
    if (!sameCell(cellP, &localCellID)) return -2;
    Ret = ubik_Call(VL_GetEntryByID, VLDBcstruct, 0, volidP,
		     VOLTIX_TO_VOLTYPE(ROVOL),
		     &VLDB);
    /* check for entry-really-not-there */
    if (Ret == VL_NOENT) {*codep = Ret; return 1;}
    if (Ret != error_status_ok) {*codep = Ret; return -1;}
    Ret = AddVLDBEntry(&VLDB, vldbP, MySiteP);
    return Ret;
}

static struct localvol *newLocalVol()
{/* Allocate, initialize, and return a new ``struct localvol'' chunk. */
    struct localvol *lv;

    lv = (struct localvol *) osi_Alloc(sizeof(struct localvol));
    if (lv != NULL) bzero((char *)lv, sizeof(struct localvol));
    return lv;
}

static int NeedSetVol(rp, volIdp, lvp, vvp)
register struct replica *rp;
afs_hyper_t *volIdp;
register struct localvol *lvp;
register struct vol_SetVV *vvp;
{/* Figure the corrent vol_SetVV to use and return non-zero if it might need to be set. */
    long code;
    int needCall;
    afsTimeval TV;
    u_long newStatus;

    icl_Trace0(rep_iclSetp, REP_TRACE_NSVOL_VV);
    /* First, complete setting up the vol_SetVV structure. */
    vvp->vv_Mask |= VOL_HAS_TOKEN;
    if (lvp != NULL
	&& HAVE_MY_WVT(AFS_hgetlo(rp->WVT_TypesLeft))
	&& !AFS_hiszero(rp->VVOnSource)
	&& (lvp->flags & lvf_GotStatus)
	&& AFS_hsame(rp->VVOnSource, lvp->volVersion)) {
	vvp->vv_Flags |= VOL_HAS_TOKEN;
	osi_GetTime(&rp->TimeOfLatestKnownCurrent);
    } else {
	vvp->vv_Flags &= ~VOL_HAS_TOKEN;
    }
    /* vv_TknExp should be the earlier of the host expiration or the token expiration. */
    vvp->vv_TknExp = *((afsTimeval *) &rp->TimeOfWVTExpiration);
    if (rp->hostP != NULL) {
	bzero((char *)&TV, sizeof(TV));
	pthread_mutex_lock(&rp->hostP->rh_fsMutex);
	TV.sec = rp->hostP->rh_fsLastSuccess
	  + rp->hostP->rh_fsHostLifeGuarantee
	  - rp->hostP->rh_fsHostLeadTime;
	pthread_mutex_unlock(&rp->hostP->rh_fsMutex);
	if (TV.sec <= rp->TimeOfWVTExpiration.tv_sec) {
	    vvp->vv_TknExp = TV;
	}
    }
    vvp->vv_Curr = *((afsTimeval *) &rp->TimeOfLatestKnownCurrent);
    vvp->vv_PingCurr = *((afsTimeval *) &rp->TimeOfLastPing);

    /* Now, compare the vol_SetVV request and compare it with what we know is already set. */
    if (lvp == NULL) {
	/* Don't know that the given volIdp is the given lvp */
	icl_Trace0(rep_iclSetp, REP_TRACE_NSVOL_NO_LVP);
	needCall = 1;
    } else if (!AFS_hsame(lvp->volId, *volIdp)) {
	/* Know that the given volIdp is NOT the given lvp */
	icl_Trace2(rep_iclSetp,  REP_TRACE_NSVOL_LVP, 
		   ICL_TYPE_HYPER, &lvp->volId, 
		   ICL_TYPE_HYPER, volIdp);
	needCall = 1;
    } else {
	/* Same lvp.  Maybe no change. */
	newStatus = lvp->itsStates & ~vvp->vv_Mask;
	newStatus |= (vvp->vv_Flags & vvp->vv_Mask);
	/* Setting any new flags values? */
	if ((lvp->flags & lvf_GotStatus) == 0) {
	    icl_Trace0(rep_iclSetp, REP_TRACE_NSVOL_NOSTATE);
	    needCall = 1;
	} else if ((vvp->vv_Flags & vvp->vv_Mask) != (lvp->itsStates & vvp->vv_Mask)) {
	    icl_Trace2(rep_iclSetp, REP_TRACE_NSVOL_SETBITS, 
		       ICL_TYPE_LONG, lvp->itsStates & vvp->vv_Mask,
		       ICL_TYPE_LONG, vvp->vv_Flags & vvp->vv_Mask);
	    needCall = 1;
	    /* Now test the various timestamps. */
	} else if (((newStatus & (VOL_HAS_TOKEN | VOL_IS_COMPLETE))
		    == (VOL_HAS_TOKEN | VOL_IS_COMPLETE))
		   && bcmp((char *)&vvp->vv_TknExp,
			   (char *)&lvp->diskTknExp,
			   sizeof(afsTimeval)) != 0) {
	    /* Changes to vv_TknExp matter only if both
	       HAS_TOKEN and IS_COMPLETE are (to be) set. */
	    icl_Trace4(rep_iclSetp, REP_TRACE_NSVOL_TKNEXP,
		       ICL_TYPE_LONG, lvp->diskTknExp.sec,
		       ICL_TYPE_LONG, lvp->diskTknExp.usec,
		       ICL_TYPE_LONG, vvp->vv_TknExp.sec,
		       ICL_TYPE_LONG, vvp->vv_TknExp.usec);
	    needCall = 1;
	} else if ((newStatus & VOL_IS_COMPLETE)
		   && bcmp((char *)&vvp->vv_Curr,
			   (char *)&lvp->diskCurr,
			   sizeof(afsTimeval)) != 0) {
	    /* Changes to vv_Curr matter only if VOL_IS_COMPLETE is set */
	    icl_Trace4(rep_iclSetp, REP_TRACE_NSVOL_NEW_CURTIME,
		       ICL_TYPE_LONG, lvp->diskCurr.sec,
		       ICL_TYPE_LONG,lvp->diskCurr.usec,
		       ICL_TYPE_LONG, vvp->vv_Curr.sec,
		       ICL_TYPE_LONG, vvp->vv_Curr.usec);
	    needCall = 1;
	} else if ((newStatus & VOL_IS_COMPLETE)
		   && bcmp((char *)&vvp->vv_PingCurr,
			   (char *)&lvp->diskPingCurr,
			   sizeof(afsTimeval)) != 0) {
	    /* Changes to vv_PingCurr matter only if VOL_IS_COMPLETE is set */
	    icl_Trace4(rep_iclSetp, REP_TRACE_NSVOL_NEW_PINGCUR, 
		       ICL_TYPE_LONG, lvp->diskPingCurr.sec,
		       ICL_TYPE_LONG, lvp->diskPingCurr.usec,
		       ICL_TYPE_LONG, vvp->vv_PingCurr.sec,
		       ICL_TYPE_LONG, vvp->vv_PingCurr.usec);
	    needCall = 1;
	} else {
	    icl_Trace0(rep_iclSetp, REP_TRACE_NSVOL_NOCHANGE);
	    needCall = 0;
	}
    }
    return (needCall);
}

static long SetOpenVol(rp, volIdp, lvp, voldesc, vvp)
register struct replica *rp;
afs_hyper_t *volIdp;
register struct localvol *lvp;
long voldesc;
register struct vol_SetVV *vvp;
{/* Set the local volume identified as ``volIdp'' to have the vol_SetVV structure vvp. */
    long code;
    int needCall;
    struct timeval TV;
    struct vol_status volstat;
    char buf[256];

    icl_Trace0(rep_iclSetp,REP_TRACE_SETOPENVOL_SETVV);
    code = VOL_SETVV(voldesc, vvp);
    if (code != 0) {
	sprintf(rp->RepMsg, "SetOpenVol: can't setvv for local vol %lu,,%lu: %s",
		AFS_HGETBOTH(*volIdp), dfs_dceErrTxt(code));
	LogPrint(rp);
	return code;
    }
    /* Shouldn't need to SYNC these changes, since we're not changing stable data */
    sprintf(buf, "flags %lx, mask %lx, curr %lu.%06lu, pc %lu.%06lu, TO %lu.%06lu",
	    vvp->vv_Flags, vvp->vv_Mask,
	    vvp->vv_Curr.sec, vvp->vv_Curr.usec,
	    vvp->vv_PingCurr.sec, vvp->vv_PingCurr.usec,
	    vvp->vv_TknExp.sec, vvp->vv_TknExp.usec);
    icl_Trace2(rep_iclSetp, REP_SETOPENVOL_SETVV_INFO,
	       ICL_TYPE_HYPER, volIdp, 
	       ICL_TYPE_STRING, buf);
    /* Update the lvp cached info */
    if (lvp && AFS_hsame(lvp->volId, *volIdp)) {
	lvp->diskTknExp = vvp->vv_TknExp;
	lvp->diskCurr = vvp->vv_Curr;
	lvp->diskPingCurr = vvp->vv_PingCurr;
	if (rp->hostP) {
	    pthread_mutex_lock(&rp->hostP->rh_fsMutex);
	    lvp->lastCallTime = rp->hostP->rh_fsLastSuccess;
	    pthread_mutex_unlock(&rp->hostP->rh_fsMutex);
	}
	if (lvp->flags & lvf_GotStatus) {
	    lvp->itsStates &= ~vvp->vv_Mask;
	    lvp->itsStates |= (vvp->vv_Mask & vvp->vv_Flags);
	} else {
	    icl_Trace0(rep_iclSetp, REP_TRACE_SETOPENVOL_GETSTAT);
	    code = ftu_FsetGetStatus(voldesc, &volstat);
	    if (code != 0) {
		sprintf(rp->RepMsg,
			"SetOpenVol: can't get status for local vol %lu,,%lu: %s",
			AFS_HGETBOTH(lvp->volId), dfs_dceErrTxt(code));
		LogPrint(rp);
		return code;
	    }
	    CopyLVStatus(lvp, &volstat);
	}
    }
    icl_Trace1(rep_iclSetp, REP_TRACE_SETOPENVOL_RET, 
		ICL_TYPE_LONG, code);
    return 0;
}

static int32 CheckSetOpenVol(rp, volIdp, lvp, volDesc)
register struct replica *rp;
afs_hyper_t *volIdp;
register struct localvol *lvp;
long volDesc;
{/* Set the VV info of this open volume if necessary. */
    /* volDesc must have been opened with FTU_SYS_GET_STATUS | VOL_SYS_SETVV. */
    struct vol_SetVV vv;
    long code;
    int needCall;

    if (lvp == NULL) {
	/* Try to get the correct LV, to update its status. */
	for (lvp = rp->AllLocals; lvp != NULL; lvp = lvp->Next) {
	    if (AFS_hsame(*volIdp, lvp->volId)) break;
	}
    }
    bzero((char *)&vv, sizeof(vv));
    vv.vv_Mask = 0;
    if (lvp != NULL && AFS_hsame(lvp->volId, *volIdp)) {
	vv.vv_Mask = VOL_IS_COMPLETE;
	vv.vv_Flags = (lvp->itsStates & VOL_IS_COMPLETE);
    }
    needCall = NeedSetVol(rp, volIdp, lvp, &vv);
    if (needCall == 0) return 0;
    icl_Trace0(rep_iclSetp, REP_CSETOPEN_SETOPENVOL);
    code = SetOpenVol(rp, volIdp, lvp, volDesc, &vv);
    if (code != 0) {
	return code;
    }
    icl_Trace1(rep_iclSetp, REP_CSETOPEN_RET,
	       ICL_TYPE_LONG, code);
    return (code);
}

static int32 UpdateLocal(rp, volIdp, lvp, aggid)
struct replica *rp;
afs_hyper_t *volIdp;
register struct localvol *lvp;
unsigned long aggid;
{/* Update the local state according to the state of *rp. */
    struct vol_SetVV vv;
    long code;
    int needCall, voldesc;

    if (lvp == NULL) {
	/* Try to get the correct LV, to update its status. */
	for (lvp = rp->AllLocals; lvp != NULL; lvp = lvp->Next) {
	    if (AFS_hsame(*volIdp, lvp->volId)) break;
	}
    }
    bzero((char *)&vv, sizeof(vv));
    vv.vv_Mask = 0;
    if (lvp != NULL && AFS_hsame(lvp->volId, *volIdp)) {
	vv.vv_Mask = VOL_IS_COMPLETE;
	vv.vv_Flags = (lvp->itsStates & VOL_IS_COMPLETE);
    }
    needCall = NeedSetVol(rp, volIdp, lvp, &vv);
    if (needCall == 0) return 0;
    icl_Trace0(rep_iclSetp, REP_TRACE_ULOCAL_AGOPEN);
    code = ftu_AggrOpenFset(aggid, volIdp,
			    FTU_SYS_GET_STATUS | VOL_SYS_SETVV | FTU_SYS_SYNC,
			    VOLERR_TRANS_REPSETFILESETVERSION, &voldesc);
    if (code != 0) {
	sprintf(rp->RepMsg, "UpdateLocal: can't open local vol %lu,,%lu: %s",
		AFS_HGETBOTH(*volIdp), dfs_dceErrTxt(code));
	if (lvp != NULL || (code != ENODEV && code != ENOENT
			    && code != EXDEV && code != FTU_E_NO_SUCH_FSET))
	    LogPrint(rp);
	else
	    DebugPrint(rp);
	if (lvp != NULL)
	    NoticeIfDeleted(rp, lvp, code);
	return code;
    }
    icl_Trace0(rep_iclSetp, REP_TRACE_ULOCAL_SOPENVOL);
    code = SetOpenVol(rp, volIdp, lvp, voldesc, &vv);
    if (code != 0) {
	(void) ftu_AggrAbortFsetOpen(voldesc);
	return code;
    }
    icl_Trace0(rep_iclSetp, REP_TRACE_ULOCAL_CLOSE);
    code = ftu_AggrCloseFset(voldesc);
    if (code != 0) {
	sprintf(rp->RepMsg, "UpdateLocal: can't close local vol %lu,,%lu: %s",
		AFS_HGETBOTH(*volIdp), dfs_dceErrTxt(code));
	LogPrint(rp);
    }
    icl_Trace1(rep_iclSetp, REP_TRACE_ULOCAL_RET,
		ICL_TYPE_LONG, code);
    return (code);
}

static int32 SetLVStatus(rp, lv, desiredStatus)
register struct replica *rp;
register struct localvol *lv;
u_long desiredStatus;
{/* Set the status for the local volume described by *lv. */
    long code, code2;
    struct vol_status volstat;
    int volD;

    icl_Trace0(rep_iclSetp, REP_TRACE_SLVSTAT_AGOPEN);
    code = ftu_AggrOpenFset(lv->aggr, &lv->volId,
			    FTU_SYS_SET_STATUS | FTU_SYS_SYNC
			    | FTU_SYS_GET_STATUS | VOL_SYS_SETVV,
			    VOLERR_TRANS_REPSETSTATUS, &volD);
    if (code != 0) {
	sprintf(rp->RepMsg, "SetLVStatus: can't open local vol %lu,,%lu: %s",
		AFS_HGETBOTH(lv->volId), dfs_dceErrTxt(code));
	LogPrint(rp);
	NoticeIfDeleted(rp, lv, code);
	return code;
    }
    bzero((char *)&volstat, sizeof(volstat));
    volstat.vol_st.states = desiredStatus;
    icl_Trace1(rep_iclSetp, REP_TRACE_SLVSTAT_SETSTAT, 
		ICL_TYPE_LONG, desiredStatus);
    code = ftu_FsetSetStatus(volD, VOL_STAT_STATES, &volstat);
    if (code != 0) {
	sprintf(rp->RepMsg, "SetLVStatus: can't set status for local vol %lu,,%lu: %s",
		AFS_HGETBOTH(lv->volId), dfs_dceErrTxt(code));
	(void) ftu_AggrAbortFsetOpen(volD);
	LogPrint(rp);
	return code;
    }
    lv->itsStates = desiredStatus;
    icl_Trace0(rep_iclSetp, REP_TRACE_SLVSTAT_CHECK_SETOPENVOL);
    code = CheckSetOpenVol(rp, &lv->volId, lv, volD);
    icl_Trace0(rep_iclSetp, REP_TRACE_SLVSTAT_SYNC);
    code2 = ftu_FsetSync(volD, FTU_FSET_SYNC_COMMIT_STATUS);
    if (code == 0) code = code2;
    if (code != 0) {
	sprintf(rp->RepMsg, "SetLVStatus: can't %s local vol %lu,,%lu: %s",
		(code2 == 0 ? "setvv" : "sync"),
		AFS_HGETBOTH(lv->volId), dfs_dceErrTxt(code));
	(void) ftu_AggrAbortFsetOpen(volD);
	LogPrint(rp);
	return code;
    }
    icl_Trace2(rep_iclSetp, REP_TRACE_SLVSTAT_SET_STATES, 
	       ICL_TYPE_HYPER, &lv->volId,
	       ICL_TYPE_LONG, lv->itsStates);
    code = ftu_AggrCloseFset(volD);
    if (code != 0) {
	sprintf(rp->RepMsg, "SetLVStatus: can't close local vol %lu,,%lu: %s",
		AFS_HGETBOTH(lv->volId), dfs_dceErrTxt(code));
	LogPrint(rp);
    }
    icl_Trace1(rep_iclSetp, REP_TRACE_SLVSTAT_RET,
		ICL_TYPE_LONG, code);
    return (code);
}

static int32 DestroyVolume(rp, lv, checkstat)
struct replica *rp;
register struct localvol *lv;
int checkstat;
{/* Destroy the volume sitting under *lv. */
    long code;
    struct vol_status volstat;
    int volD;

    icl_Trace1(rep_iclSetp, REP_DESTROY_VOL_BEG, 
	       ICL_TYPE_HYPER, &lv->volId);
    code = ftu_AggrOpenFset(lv->aggr, &lv->volId,
			    FTU_SYS_DESTROY | FTU_SYS_GET_STATUS,
			    VOLERR_PERS_REPDESTROY, &volD);
    if (code != 0) {
	sprintf(rp->RepMsg, "DestroyVolume: can't open local vol %lu,,%lu: %s",
		AFS_HGETBOTH(lv->volId), dfs_dceErrTxt(code));
	LogPrint(rp);
	return code;
    }
    if (checkstat) {
	icl_Trace0(rep_iclSetp, REP_DESTROY_VOL_GETSTAT);
	bzero((char *)&volstat, sizeof(volstat));
	code = ftu_FsetGetStatus(volD, &volstat);
	if (code != 0) {
	    (void) ftu_AggrAbortFsetOpen(volD);
	    sprintf(rp->RepMsg,
		    "DestroyVolume: can't get status for local vol %lu,,%lu: %s",
		    AFS_HGETBOTH(lv->volId), dfs_dceErrTxt(code));
	    LogPrint(rp);
	    return code;
	}
	CopyLVStatus(lv, &volstat);
	/* Check whether the states are such that we own this fileset. */
	if (((lv->itsStates & VOL_REPSERVER_MGD) == 0)
	    || (((lv->itsStates & VOL_REPFIELD) != VOL_REP_RELEASE)
		&& ((lv->itsStates & VOL_REPFIELD) != VOL_REP_LAZY))
	    || (((lv->itsStates & VOL_TYPEFIELD) != VOL_TYPE_RO)
		&& ((lv->itsStates & VOL_TYPEFIELD) != VOL_TYPE_TEMP))) {
	    (void) ftu_AggrAbortFsetOpen(volD);
	    sprintf(rp->RepMsg,
		    "DestroyVolume: unmanaged status for local vol %lu,,%lu: %#lx",
		    AFS_HGETBOTH(lv->volId), lv->itsStates);
	    (void) ftu_AggrCloseFset(volD);
	    return -1;
	}
    }
    code = ftu_FsetDestroy(volD);
    if (code != 0) {
	sprintf(rp->RepMsg, "DestroyVolume: can't destroy local vol %lu,,%lu: %s",
		AFS_HGETBOTH(lv->volId), dfs_dceErrTxt(code));
	(void) ftu_AggrAbortFsetOpen(volD);
	LogPrint(rp);
	return code;
    }
    lv->flags |= lvf_Deleted;
    icl_Trace0(rep_iclSetp, REP_DESTROY_VOL_CLOSE);
    code = ftu_AggrCloseFset(volD);
    if (code != 0) {
	sprintf(rp->RepMsg, "DestroyVolume: can't close local destroyed vol %lu,,%lu: %s",
		AFS_HGETBOTH(lv->volId), dfs_dceErrTxt(code));
	LogPrint(rp);
    }
    icl_Trace1(rep_iclSetp, REP_DESTROY_VOL_RET,
		 ICL_TYPE_LONG, code);
    return (code);
}

static long LoseWVT(rp, lossTimeP, typesLostP, initTime)
struct replica *rp;
struct timeval *lossTimeP;
afs_hyper_t *typesLostP;
int initTime;
{/* Do what's necessary to mark that we've lost the WVT for replica *rp. */
    u_int32 MaxDally, Noise;
    afs_hyper_t typesLeft;
    long code, code2;
    struct localvol *lvp;

    /* This routine is called with rp->lockObject held write-locked. */
    rp->TimeOfLastPing = *lossTimeP;
    if (HAVE_MY_WVT(AFS_hgetlo(rp->WVT_TypesLeft))) {
	typesLeft = rp->WVT_TypesLeft;
	AFS_HOP(typesLeft, &~, *typesLostP);
	if (HAVE_MY_WVT(AFS_hgetlo(typesLeft))) {
	    rp->WVT_TypesLeft = typesLeft;
	    return 0;		  /* we still have what we want--ignore loss */
	}
	SetLatestCurrentTime(rp, NULL, lossTimeP);
	AFS_hzero(rp->VVOnSource);
	rp->WVT_TypesLeft = typesLeft;	/* mark big WVT loss */
	rp->TimeOfReadTokenLoss = *lossTimeP;
	sprintf(rp->RepMsg, "LoseWVT: Lost WVT at %lu: types %#lx remain",
		lossTimeP->tv_sec, AFS_hgetlo(typesLeft));
	if (rp->MymaxReplicaLatency >= 2*rp->v.minimumPounceDally) {
	    MaxDally = (rp->MymaxReplicaLatency/2) - rp->v.minimumPounceDally;
	    if (MaxDally < rp->v.minimumPounceDally) MaxDally = rp->v.minimumPounceDally;
	} else if (rp->MymaxReplicaLatency >= rp->v.minimumPounceDally) {
	    MaxDally = rp->MymaxReplicaLatency - rp->v.minimumPounceDally;
	} else MaxDally = 0;
	/* Make some number between 0 and 4095 */
	Noise = ((lossTimeP->tv_sec ^ lossTimeP->tv_usec) & 0xfff);
	rp->nextLookTime = *lossTimeP;
	code = rp->v.minimumPounceDally + ((MaxDally * Noise) >> 12);
	if (initTime) {
	    /* Lost the token because of an InitTokenState: try again soon. */
	    if (code > 10) code = 10;	/* max seconds for retry in this case. */
	}
	rp->nextLookTime.tv_sec += code;
	DebugPrint(rp);
	/* Tell the kernel's volume structure that we don't have the token any more. */
	code = UpdateLocal(rp, &(rp->v.VolIDOfSecondary), NULL,
			   rp->MyReplicaAggregate);
	for (lvp = rp->AllLocals; lvp != NULL; lvp = lvp->Next) {
	    if (((lvp->flags & lvf_GotStatus) == 0)
		|| (lvp->itsStates & VOL_HAS_TOKEN)) {
		code2 = UpdateLocal(rp, &(rp->v.VolIDOfSecondary), lvp,
				   rp->MyReplicaAggregate);
		if (code == 0) code = code2;
	    }
	}
	if ((code == ENOENT || code == ENODEV || code == FTU_E_NO_SUCH_FSET)
	    && rp->PublishedP == NULL) code = 0;
    } else {
	AFS_hzero(rp->VVOnSource);
	if ((rp->Flags & repflag_GrabbingWVTRead) != 0) {
	    typesLeft = rp->WVT_TypesRevoked;
	    AFS_HOP(rp->WVT_TypesRevoked, |, *typesLostP);
	    sprintf(rp->RepMsg, "LoseWVT: Racing revocation at %lu: %#lx -> %#lx",
		    lossTimeP->tv_sec, AFS_hgetlo(typesLeft),
		    AFS_hgetlo(rp->WVT_TypesRevoked));
	    DebugPrint(rp);
	} else if (!AFS_hiszero(rp->WVT_TypesLeft)) {
	    typesLeft = rp->WVT_TypesLeft;
	    AFS_HOP(typesLeft, &~, *typesLostP);
	    sprintf(rp->RepMsg, "LoseWVT: More token types revoked at %lu: %#lx -> %#lx",
		    lossTimeP->tv_sec, AFS_hgetlo(rp->WVT_TypesLeft),
		    AFS_hgetlo(typesLeft));
	    DebugPrint(rp);
	    rp->WVT_TypesLeft = typesLeft;
	}
	code = 0;
    }
    return code;
}

int32 VLDBCopy(oldVLDBp, rp, siteIx, errbuf, styleChanged)
struct vldbRepInfo *oldVLDBp;
struct replica *rp;
int32 siteIx;
char *errbuf;
int styleChanged;
{/* Copy old to replica slot.  Allocate a new copy of the site data. */
    struct RepSite *newreps;
    int repsize;

    repsize = oldVLDBp->NumReps * sizeof(struct RepSite);
    newreps = (struct RepSite *) osi_Alloc(repsize);
    if (newreps == NULL) {
	sprintf(errbuf, "%s: VLDBCopy: no memory for replica addresses", WhoIAm);
	return ENOMEM;
    }
    bcopy((char *)oldVLDBp->Reps, (char *)newreps, repsize);
    rp->v = *oldVLDBp;
    rp->v.Reps = newreps;
    rp->v.MaxReps = oldVLDBp->NumReps;	/* record the allocation */
    rp->MymaxReplicaLatency = oldVLDBp->Reps[siteIx].ReplicaLatency;
    rp->MyReplicaAggregate = oldVLDBp->Reps[siteIx].ReplicaAggregate;
    rp->mySiteIx = siteIx;
    rp->Flags |= repflag_KnowVLDB;
    if (oldVLDBp->vlflags & VLF_LAZYRELE) {
	rp->Flags |= repflag_ReleaseOnly;
    } else {
	rp->Flags &= ~repflag_ReleaseOnly;
    }
#if !defined(OSF_NO_SOCKET_DUMP)
    if (oldVLDBp->vlflags & VLF_SOCKFLAG) {
	rp->Flags |= repflag_UseSocket;
    } else {
	rp->Flags &= ~repflag_UseSocket;
    }
#endif /* OSF_NO_SOCKET_DUMP */
    rp->remoteBusyWait = rp->MymaxReplicaLatency / 10;
    if (rp->remoteBusyWait < rp->v.minimumPounceDally)
	rp->remoteBusyWait = rp->v.minimumPounceDally;
    if (rp->remoteBusyWait > 15*60) rp->remoteBusyWait = 15*60;
    rp->kaLeadTime = rp->v.reclaimDally / 4;
    if (rp->kaLeadTime < 5*60) rp->kaLeadTime = 5*60;
    rp->memoryWait = 3*60;
    if (rp->memoryWait > rp->remoteBusyWait) rp->memoryWait = rp->remoteBusyWait;
    rp->localDiskWait = 2*60;
    if (rp->localDiskWait > rp->remoteBusyWait) rp->localDiskWait = rp->remoteBusyWait;
    if (rp->v.maxTotalLatency == 0
	 || rp->v.hardMaxTotalLatency < rp->v.maxTotalLatency
	 || rp->v.minimumPounceDally > rp->MymaxReplicaLatency
	 || rp->v.reclaimDally == 0) {
	sprintf(errbuf, "VLDBCopy: Invalid replication parameters in VLDB for replica %lu,,%lu",
		AFS_HGETBOTH(rp->v.VolIDOfSecondary));
	return EINVAL;
    }
    if (styleChanged) {
	/* Must flush some attributes that release-replication lies about. */
	struct timeval currTV;
	osi_GetTime(&currTV);
	LoseWVT(rp, &currTV, &rp->WVT_TypesLeft, 1);
    }
    return 0;
}

/* RPC procedures to satisfy the token-revocation requests */

static u_long STKN_Probe(rpcCallp)
handle_t rpcCallp;
{/* Just be alive. */
    icl_Trace1(rep_iclSetp, REP_STKN_PROBE, 
		ICL_TYPE_STRING, WhoIAm);
    return 0;
}

static error_status_t STKN_InitTokenState(
  handle_t rpcCallp,
  unsigned32 Flags,
  unsigned32 hostLifeGuarantee,
  unsigned32 hostRPCGuarantee,
  unsigned32 deadServerTimeout,
  unsigned32 serverRestartEpoch,
  unsigned32 serverSizesAttrs,
  unsigned32 spare2,
  unsigned32 spare3,
  unsigned32 *spare4,
  unsigned32 *spare5,
  unsigned32 *spare6)
{/* Clear all token state that was granted from the calling host. */
    struct replica *rp;
    struct timeval rcvtv;
    int Ix;
    error_status_t st;
    afsUUID connId;
    int cancelState;

    cancelState = pthread_setcancel(CANCEL_OFF);
    if (cancelState == -1) return COMPAT_ERROR_DISABLE_CANCEL;

    osi_GetTime(&rcvtv);
    icl_Trace1(rep_iclSetp, REP_INIT_TOKEN, 
		ICL_TYPE_STRING, WhoIAm);
    rpc_binding_inq_object(rpcCallp, &connId, &st);
    if (st == rpc_s_ok) {
	if (uuid_is_nil(&connId, (unsigned32 *)&st)) {
	    Log("STKN_InitTokenState: got nil server UUID");
	    (void) pthread_setcancel(cancelState);
	    return EINVAL;
	}
    } else {
	Log("STKN_InitTokenState: could not read UUID from binding: %s",
		dfs_dceErrTxt(st));
	(void) pthread_setcancel(cancelState);
	return st;
    }
    icl_Trace1(rep_iclSetp,REP_INIT_TOKEN_GLOCK, 
		ICL_TYPE_STRING, WhoIAm);
    lock_ObtainRead(&RepTableLock);
    icl_Trace1(rep_iclSetp,REP_INIT_TOKEN_GOT_GLOCK, 
		ICL_TYPE_STRING, WhoIAm);
    for (rp = RepRoot; rp != NULL; rp = rp->Next) {
	lock_ObtainShared(&(rp->lockObject));
	if (rp->hostP != NULL
	    && uuid_equal(&connId, &rp->hostP->rh_fsUUID, &st)) {
	    /* this one's revoked! */
	    lock_UpgradeSToW(&(rp->lockObject));
	    rp->hostLifeGuarantee = hostLifeGuarantee;
	    pthread_mutex_lock(&rp->hostP->rh_fsMutex);
	    rp->hostP->rh_fsHostLifeGuarantee = hostLifeGuarantee;
	    rp->hostP->rh_fsHostLeadTime = hostLifeGuarantee / 3;
	    /* ignore serverSizesAttrs */
	    pthread_mutex_unlock(&rp->hostP->rh_fsMutex);
	    (void) LoseWVT(rp, &rcvtv, &allTokenTypes, 1);
	    repq_ResetDeadline(&RepQ, rp->qCookie, rp->nextLookTime.tv_sec);
	    lock_ReleaseWrite(&(rp->lockObject));
	} else {
	    lock_ReleaseShared(&(rp->lockObject));
	}
    }
    lock_ReleaseRead(&RepTableLock);
    icl_Trace1(rep_iclSetp,REP_INIT_TOKEN_REL_GLOCK, 
		ICL_TYPE_STRING, WhoIAm);

    (void) pthread_setcancel(cancelState);
    return 0;
}

static u_long STKN_TokenRevoke(rpcCallp, revokeDescp)
handle_t rpcCallp;
register afsRevokes *revokeDescp;
{/* Revoke the named tokens. */
    long numRevokes;
    register struct afsRevokeDesc *rdp;
    long code, retVal;
    int Ix;
    afsUUID connId;
    error_status_t st;
    register struct replica *rp;
    afs_hyper_t typesLeft;
    struct timeval rcvtv;
    int cancelState;

    cancelState = pthread_setcancel(CANCEL_OFF);
    if (cancelState == -1) return COMPAT_ERROR_DISABLE_CANCEL;

    osi_GetTime(&rcvtv);
    rpc_binding_inq_object(rpcCallp, &connId, &st);
    if (st == rpc_s_ok) {
	if (uuid_is_nil(&connId, (unsigned32 *)&st)) {
	    Log("STKN_TokenRevoke: got nil server UUID");
	    (void) pthread_setcancel(cancelState);
	    return EINVAL;
	}
    } else {
	Log("STKN_TokenRevoke: could not read UUID from binding: %s",
		dfs_dceErrTxt(st));
	(void) pthread_setcancel(cancelState);
	return st;
    }
    numRevokes = revokeDescp->afsRevokes_len;
    icl_Trace2(rep_iclSetp, REP_TOKEN_REVOKE, 
		ICL_TYPE_STRING, WhoIAm, 
		ICL_TYPE_LONG, numRevokes);
    retVal = 0;
    rdp = &revokeDescp->afsRevokes_val[0];
    for (Ix = 0; Ix < numRevokes; ++Ix, ++rdp) {
	rdp->flags = 0;
	AFS_hzero(rdp->errorIDs);	/* assume revoke will be OK */
	lock_ObtainRead(&RepTableLock);
	for (rp = RepRoot; rp != NULL; rp = rp->Next) {
	    if (AFS_hsame(rdp->fid.Volume, rp->v.VolIDOfPrimary)) {
		lock_ObtainShared(&(rp->lockObject));
		if (rp->hostP != NULL
		    && AFS_hsame(rp->WVT_ReadTokenID, rdp->tokenID)
		    && uuid_equal(&connId, &rp->hostP->rh_fsUUID, &st)) {
		    lock_UpgradeSToW(&(rp->lockObject));
		    icl_Trace3(rep_iclSetp, REP_TOKEN_REVOKE_LOSEWVT_CALL,
			       ICL_TYPE_FID, &rdp->fid,
			       ICL_TYPE_HYPER, &rdp->tokenID,
			       ICL_TYPE_LONG, AFS_hgetlo(rdp->type));
		    code = LoseWVT(rp, &rcvtv, &rdp->type, 0);
#if 0
		    if (code != 0) {
			/* Found the token-holder, but could not set the
			   local disk to lose the token. */
			rdp->errorIDs = rp->WVT_TypesLeft;
			retVal = HS_ERR_PARTIAL;
		    }
#endif /* 0 */
		    repq_ResetDeadline(&RepQ, rp->qCookie,
				       rp->nextLookTime.tv_sec);
		    lock_ReleaseWrite(&(rp->lockObject));
		    break;
		} else {
		    lock_ReleaseShared(&rp->lockObject);
		}
	    }
	}
	lock_ReleaseRead(&RepTableLock);
    }
    (void) pthread_setcancel(cancelState);
    return retVal;
}

static u_long STKN_GetCellName(rpcCallp, cellP, cellNamep)
handle_t rpcCallp;
afs_hyper_t *cellP;
unsigned char *cellNamep;
{/* apparently does id-to-name translation somehow.  This does the trivial case. */
    int cancelState;

    cancelState = pthread_setcancel(CANCEL_OFF);
    if (cancelState == -1) return COMPAT_ERROR_DISABLE_CANCEL;

    icl_Trace1(rep_iclSetp, REP_GET_CELLNAME_CALL, 
		ICL_TYPE_STRING, WhoIAm);
    if (sameCell(cellP, &localCellID) && localCellName[0] != '\0') {
	strcpy((char *)cellNamep, localCellName);
	icl_Trace2(rep_iclSetp, REP_GET_CELLNAME_RET, 
		   ICL_TYPE_STRING, WhoIAm, 
		   ICL_TYPE_STRING, cellNamep);
	(void) pthread_setcancel(cancelState);
	return 0;
    }
    icl_Trace1(rep_iclSetp, REP_GET_CELLNAME_RET_NONAME, 
		ICL_TYPE_STRING, WhoIAm);
    *cellNamep = '\0';
    (void) pthread_setcancel(cancelState);
    return ESRCH;
}

/* DEBUGGING ONLY --may go away */
static u_long STKN_GetLock(rpcCallp, index, lockp)
handle_t rpcCallp;
u_long index;
struct afsDBLock *lockp;
{/* We don't have the CM's internal lock structure */
    icl_Trace0(rep_iclSetp, REP_GET_LOCK);
    return ESRCH;
}

static u_long STKN_GetCE(rpcCallp, index, cep)
handle_t rpcCallp;
u_long index;
struct afsDBCacheEntry *cep;
{/* We don't have the CM's internal structure at all. */
    icl_Trace1(rep_iclSetp, REP_GET_CE,
		ICL_TYPE_STRING, WhoIAm);
    return ESRCH;
}

static u_long STKN_GetServerInterfaces(h, serverInterfacesP)
    handle_t  h;
    dfs_interfaceList *serverInterfacesP;
{
    register struct dfs_interfaceDescription *idp;
    rpc_if_id_t if_id;
    unsigned long st;
    int cancelState;

    cancelState = pthread_setcancel(CANCEL_OFF);
    if (cancelState == -1) return COMPAT_ERROR_DISABLE_CANCEL;

    /* only one interface defined so far */
    /* New interfaces or extensions will add to this list (not replacing the original). */
    serverInterfacesP->dfs_interfaceList_len = 0;
    rpc_if_inq_id (TKN4Int_v4_0_s_ifspec, &if_id, &st);
    if (st != error_status_ok) {
	(void) pthread_setcancel(cancelState);
	return st;
    }
    serverInterfacesP->dfs_interfaceList_len = 1;
    idp = &serverInterfacesP->dfs_interfaceList_val[0]; /* fill this in */

    bzero((char *)idp, sizeof(struct dfs_interfaceDescription));
    idp->interface_uuid = if_id.uuid;
    idp->vers_major = if_id.vers_major;
    idp->vers_minor = if_id.vers_minor;
    idp->vers_provider = 1;	/* provider_version(1) from tkn4int.idl file */
    strncpy((char *)&idp->spareText[0], 
	    (char *)"Transarc TKN repserver revocation service",
	     (unsigned int)MAXSPARETEXT);
    (void) pthread_setcancel(cancelState);
    return error_status_ok;
}

static u_long STKN_SetParams(h, Flags, paramsP)
    handle_t h;
    unsigned long Flags;
    afsConnParams *paramsP;
{/* NOT IMPLEMENTED YET */
    return EINVAL;
}

static rep_host_conn_t *GetConn(struct replica *rp, 
				 rep_host_conn_type_t connType,
				 rep_host_t **hostPP,
				 char *where)
{
    long		code;
    rep_host_conn_t*	connP = NULL;
    rep_host_t*		hostP;
    int			lockType = LOCK_NOLOCK;

    /* Remember the type of lock held on the replica structure */
    if (osi_rwlock_write_held(&rp->lockObject))
	lockType = WRITE_LOCK;
    else if (osi_rwlock_read_held(&rp->lockObject))
	lockType = READ_LOCK;

    /*
     * If we have a null hostP we must have a write lock, so we can set the
     * hostP; otherwise, we will not be changing the replica, so we can just
     * have a read lock. 
     */
    osi_assert(lockType == READ_LOCK && rp->hostP != NULL ||
	       lockType == WRITE_LOCK);

    do {
	if (connP != NULL) {
	    /*
	     * If this isn't the first time through this loop we need to give
	     * up the connection that we just got.
	     */
	    reph_PutConn(connP);
	    connP = NULL;
	}

	if (rp->hostP == NULL) {
	    rp->hostP = reph_GetHost(rp->v.PrimaryAddrs, rp->v.NumPrimaryAddrs,
				     rp->v.hName);
	    if (rp->hostP == NULL) {
		sprintf(rp->RepMsg,
		      "%s/GetConn: Can't get host structure for %s, no memory",
			where, rp->v.hName);
		LogPrint(rp);
		return NULL;
	    }
	}
	
	/*
	 * We may have to make an RPC to initialize a new connection, so we
	 * drop the replica lock.
	 */
	hostP = rp->hostP;		/* Remember hostP in local storage */
	lock_Release(&rp->lockObject, lockType);
	
	code = reph_GetConn(hostP, connType, &connP);
	
	/* Reobtain the replica lock */
	lock_Obtain(&rp->lockObject, lockType);

	/*
	 * The primary address information may have changed (most likely
	 * because the primary was moved) after we dropped the lock.
	 * This will have caused the old hostP to be thrown away, so we
	 * try again.  See CheckVLDBRelationship and CheckWithVLDB.
	 */
    } while (code == 0 && rp->hostP == NULL);

    if (code == 0) {
	osi_assert(rp->hostP != NULL && connP != NULL);
	if (hostPP != NULL)
	    *hostPP = rp->hostP;
    } else {
	osi_assert(connP == NULL);
	sprintf(rp->RepMsg, "%s/GetConn: Can't get connection to host %s: %s",
		where, hostP->rh_name, dfs_dceErrTxt(code));
	DebugPrint(rp);
    }

    return connP;
}	/* GetConn() */

#define GET_FS_CONN(rp, hostPP, where)	\
    GetConn((rp), REPH_CONN_TYPE_FS, (hostPP), (where))
#define GET_FT_CONN(rp, where)	\
    GetConn((rp), REPH_CONN_TYPE_FT, NULL, (where))

static long ResetConn(struct replica *rp,
		       rep_host_t *origHostP,
		       rep_host_conn_t **connPP,
		       long connCode, char *where)
{
    long	code;
    rep_host_t*	hostP;
    int		lockType = LOCK_NOLOCK;

    osi_assert(*connPP != NULL);

    /* Remember the type of lock held on the replica structure */
    if (osi_rwlock_write_held(&rp->lockObject))
	lockType = WRITE_LOCK;
    else if (osi_rwlock_read_held(&rp->lockObject))
	lockType = READ_LOCK;

    /* We must have either a write lock or a read lock. */
    osi_assert(lockType == WRITE_LOCK || lockType == READ_LOCK);

    if (rp->hostP == NULL || origHostP != NULL && rp->hostP != origHostP) {
	/*
	 * The primary host has changed, making this connection useless,
	 * so we fail the reset.
	 */
	sprintf(rp->RepMsg,
 "%s/ResetConn: %lu,,%lu: Primary host has changed--no reset attempted for %s",
		where,
		AFS_HGETBOTH(rp->v.VolIDOfSecondary),
		dfs_dceErrTxt(code));
	LogPrint(rp);
	
	reph_PutConn(*connPP);
	*connPP = NULL;
	return REP_ERR_RESETFAILED;
    }

    /*
     * We may have to make an RPC to initialize a new connection, so we
     * drop the replica lock.
     */
    hostP = rp->hostP;			/* Remember hostP in local storage */
    lock_Release(&rp->lockObject, lockType);

    code = reph_ResetConn(hostP, connPP, connCode);

    /* Reobtain the replica lock */
    lock_Obtain(&rp->lockObject, lockType);

    if (*connPP == NULL)
	setNextTime(rp, rp->remoteBusyWait, NULL, 0);

    if (code == 0) {
	osi_assert(*connPP != NULL);
    } else {
	sprintf(rp->RepMsg,
		"%s/ResetConn: Can't reset connection to host %s: %s",
		where, hostP->rh_name, dfs_dceErrTxt(code));
	DebugPrint(rp);
    }

    return code;
}	/* ResetConn() */

static void MergeSuccess(struct replica *rp,
			  rep_host_t *origHostP,
			  time_t successTime)
{
    osi_assert(lock_Check(&rp->lockObject) != 0);

    /*
     * Don't update success time if we succeeded on a different host.
     */
    if (rp->hostP != NULL || rp->hostP == origHostP)
	reph_MergeSuccess(rp->hostP, successTime);
}      	/* MergeSuccess() */

static u_int32 LoseAllWVTRead(rp, nowp)
struct replica *rp;
struct timeval *nowp;
{/* Attempt to lose the entire WVT-Read token */
/* Called with lockObject write-locked. */
    struct afsReturns toks;	/* even so, we'll be doing only one at a time */
    long code;
    int retryOK, retryCount;
    error_status_t st;
    rep_host_conn_t *connP;
    rep_host_t *hostP;

    AFS_HOP(rp->WVT_TypesLeft, &~, rp->WVT_TypesRevoked);
    AFS_hzero(rp->WVT_TypesRevoked);
    if (!AFS_hiszero(rp->WVT_TypesLeft)
	 && rp->TimeOfWVTExpiration.tv_sec >= nowp->tv_sec) {	/* not expired yet */
	toks.afsReturns_len = 1;
	bzero((char *)&toks.afsReturns_val[0], sizeof(toks.afsReturns_val[0]));
	toks.afsReturns_val[0].fid.Cell = rp->CellId;
	toks.afsReturns_val[0].fid.Volume = rp->v.VolIDOfPrimary;
	toks.afsReturns_val[0].fid.Vnode = ~(1L);	/* whole-volume token */
	toks.afsReturns_val[0].fid.Unique = 0;	/* whole-volume token */
	toks.afsReturns_val[0].tokenID = rp->WVT_ReadTokenID;
	toks.afsReturns_val[0].type = rp->WVT_TypesLeft;
	/* toks.afsReturns_val[0].flags = 0; */

	/*
	 * We consider the token to be gone even if the attempt to release it
	 * fails.  This simplifies the requirements on callers who can
	 * now assume that no tokens are held (for this replica) after calling
	 * this routine.  Furthermore, it is more or less harmless since the
	 * token revocation code will freely give up tokens that we don't have
	 * any memory of.
	 */
	AFS_hzero(rp->WVT_TypesLeft);

	/* Get a connection to the primary's file server */
	if ((connP = GET_FS_CONN(rp, &hostP, "LoseAllWVTRead-1")) == NULL) {
	    return (setNextTime(rp, rp->remoteBusyWait, NULL, 0));
	}
	/* we have a connection to the server where the primary is stored. */
	sprintf(rp->RepMsg,
		"Attempting to lose token (%#lx) for %lu,,%lu on primary",
		AFS_hgetlo(toks.afsReturns_val[0].type),
		AFS_HGETBOTH(rp->v.VolIDOfSecondary));
	DebugPrint(rp);
	retryCount = 0;
	for (retryOK = 1; retryOK;) {
	    retryOK = 0;
	    ++retryCount;
	    lock_ReleaseWrite(&(rp->lockObject));
	    st = (unsigned long) osi_Time();
	    code = AFS_ReleaseTokens(connP->rhc_handle, &toks, 0);
	    code = osi_errDecode(code);
	    lock_ObtainWrite(&(rp->lockObject));
	    if (code != 0 && retryCount < 3) {
		code = ResetConn(rp, hostP, &connP, code, "LoseAllWVTRead-2");
		if (code == REP_ERR_RESETFAILED) {
		    if (connP != NULL)
			reph_PutConn(connP);
		    return rp->nextLookTime.tv_sec;
		} else if (code == 0) retryOK = 1;
	    }
	}
	reph_PutConn(connP);
	sprintf(rp->RepMsg, "ReleaseTokens result: %ld", code);
	DebugPrint(rp);
	if (code == TKM_ERROR_NOENTRY) {
	    sprintf(rp->RepMsg, "LoseAllWVTRead: **TKM doesn't know token %lu,,%lu (ft %lu,,%lu, types %lu,,%lu)",
		    AFS_HGETBOTH(rp->WVT_ReadTokenID),
		    AFS_HGETBOTH(rp->v.VolIDOfPrimary),
		    AFS_HGETBOTH(rp->WVT_TypesLeft));
	    LogPrint(rp);
	}
	if (code != 0 && code != TKM_ERROR_NOENTRY) {
	    return (setNextTime(rp, rp->remoteBusyWait,
				"LoseAllWVTRead: %s", dfs_dceErrTxt(code)));
	}
	MergeSuccess(rp, hostP, st);
    } else {
	AFS_hzero(rp->WVT_TypesLeft);
    }
    rp->nextLookTime.tv_sec = 0;
    strcpy(rp->RepMsg, "LoseAllWVTRead: WVT successfully given up.");
    DebugPrint(rp);
    return (osi_Time() + 10*60*60);
}

static u_int32 TryForWVTRead(rp)
struct replica *rp;
{/* Attempt to get a read-class whole-volume-token for the primary volume. */
/* Called with rp->lockObject write-locked. */
    afsFid fid;
    struct timeval starttv;
    struct afsFetchStatus OutStatus;
    struct afsVolSync tsync;
    afs_token_t minToken, realToken;
    afs_recordLock_t rLock;
    long code;
    u_long delay, hexptime, texptime, interval;
    int retryOK, retryCount;
    error_status_t st;
    rep_host_conn_t *connP;
    rep_host_t *hostP;

    /* Describe the kind of token we want */
    bzero((char *)&minToken, sizeof(afs_token_t));
    AFS_hset64(minToken.type, 0, MY_WVT_TYPES);
    AFS_hzero(minToken.beginRange);
    minToken.endRange = osi_hMaxint64;
    /* Now say what object we want a token for (the whole primary volume) */
    fid.Cell = rp->CellId;
    fid.Volume = rp->v.VolIDOfPrimary;
    fid.Vnode = ~((unsigned long) 1);	/* signal whole-volume token */
    fid.Unique = 0;
    /* Ensure we have a connection to the primary's file server */
    if ((connP = GET_FS_CONN(rp, &hostP, "TryForWVTRead-1")) == NULL)
	return (setNextTime(rp, rp->remoteBusyWait, NULL, 0)); /* whoops: no live conn */
    /* No real tokens on a R/O server. */
    osi_GetTime(&starttv);
    if (rp->Flags & repflag_ReleaseOnly) {
	rp->TimeOfLastPing = starttv;
	AFS_hset64(rp->WVT_TypesLeft, 0, MY_WVT_TYPES);
	AFS_hzero(rp->WVT_TypesRevoked);
	/*
	 * Expire the fake token in 1/4 of the maxage interval, so we'll try to
	 * keep the disk's stored vvCurrentTime relatively fresh/recent.
	 */
	osi_GetTime(&rp->TimeOfWVTExpiration);
	rp->TimeOfWVTExpiration.tv_sec += rp->MymaxReplicaLatency;
	/*
	 * Get called again either myMaxRep/4 or 60 seconds, whichever is
	 * smaller, before this simulated token is due to expire.
	 */
	rp->tokenLeadTime = rp->MymaxReplicaLatency/4;
	if (rp->tokenLeadTime > 60)
	    rp->tokenLeadTime = 60;
	rp->Flags &= ~repflag_GrabbingWVTRead;
	(void) UpdateLocal(rp, &rp->v.VolIDOfSecondary, NULL,
			   rp->MyReplicaAggregate);
	rp->nextLookTime.tv_sec = 0;
	return (rp->TimeOfWVTExpiration.tv_sec - rp->tokenLeadTime);
    }
    /* we now have a connection to the file server where the primary is stored. */
    sprintf(rp->RepMsg, "Attempting to get token (%#lx) for %lu,,%lu on primary",
	     MY_WVT_TYPES,
	     AFS_HGETBOTH(rp->v.VolIDOfPrimary));
    DebugPrint(rp);
    rp->TimeOfLastPing = starttv;
    rp->Flags |= repflag_GrabbingWVTRead;   /* deal with race */
    AFS_hzero(rp->WVT_TypesRevoked);
    retryCount = 0;
    for (retryOK = 1; retryOK;) {
	retryOK = 0;
	++retryCount;
	lock_ReleaseWrite(&(rp->lockObject));
	st = (unsigned long) osi_Time();
	code = AFS_GetToken(connP->rhc_handle, &fid, &minToken, &zeroDum,
			    AFS_FLAG_TOKENJUMPQUEUE | AFS_FLAG_FORCEVOLQUIESCE,
			    &realToken, &rLock, &OutStatus, &tsync);
	code = osi_errDecode(code);
	icl_Trace1(rep_iclSetp, REP_GET_TOKEN,
		   ICL_TYPE_LONG, code);
	lock_ObtainWrite(&(rp->lockObject));
	if (code != 0 && retryCount < 3) {
	    code = ResetConn(rp, hostP, &connP, code, "TryForWVTRead-2");
	    if (code == REP_ERR_RESETFAILED) {
		if (connP != NULL)
		    reph_PutConn(connP);
		return rp->nextLookTime.tv_sec;
	    } else if (code == 0)
		retryOK = 1;
	}
    }
    reph_PutConn(connP);
    rp->Flags &= ~repflag_GrabbingWVTRead;
    /* Nothing in the OutStatus information that we want. */
    if (code == 0) {
	MergeSuccess(rp, hostP, st);
	rp->WVT_TypesLeft = realToken.type;
	rp->WVT_ReadTokenID = realToken.tokenID;
	sprintf(rp->RepMsg, "GetToken returned %#lx token, ID %lu,,%lu.",
		AFS_hgetlo(rp->WVT_TypesLeft),
		AFS_HGETBOTH(rp->WVT_ReadTokenID));
	DebugPrint(rp);
	/* Check for token-revocation race. */
	if (!AFS_hiszero(rp->WVT_TypesRevoked)) {
	    AFS_HOP(rp->WVT_TypesLeft, &~, rp->WVT_TypesRevoked);
	    sprintf(rp->RepMsg, "TryForWVTRead: types %#lx already revoked, leaving %#lx",
		    AFS_hgetlo(rp->WVT_TypesRevoked),
		    AFS_hgetlo(rp->WVT_TypesLeft));
	    DebugPrint(rp);
	}
	rp->tokenLeadTime = realToken.expirationTime / 3;
	if (rp->tokenLeadTime < 20 && realToken.expirationTime > 30)
	    rp->tokenLeadTime = 20;
    } else if (code == TKM_ERROR_TOKENCONFLICT) {
	sprintf(rp->RepMsg,
	       "TryForWVTRead: %lu,,%lu: token conflict: a file may be write locked on the primary",
	       AFS_HGETBOTH(rp->v.VolIDOfPrimary));
	DebugPrint(rp);
    }
    if (code != 0 || !HAVE_MY_WVT(AFS_hgetlo(rp->WVT_TypesLeft))) {
	(void) UpdateLocal(rp, &rp->v.VolIDOfSecondary, NULL, rp->MyReplicaAggregate);
	if (code == 0) {
	    /* Better to make the retry interval a fraction of
	     * hostLifeGuarantee (if we know that information) to
	     * prevent recurring stale host problems the next time we try again.
	     */
	    if (rp->hostP != NULL) {
		interval = ((rp->remoteBusyWait < rp->hostP->rh_fsHostLifeGuarantee/4)?
			    rp->remoteBusyWait: rp->hostP->rh_fsHostLifeGuarantee/4);
	    } else
		interval = 15;
	} else
	    interval = rp->remoteBusyWait;
	return (setNextTime(rp,
			    interval,
			    "TryForWVTRead: %s",
			    (code == 0
			     ? "interleaved revoke"
			     : (char *) dfs_dceErrTxt(code))));
    }
    rp->TimeOfWVTExpiration = starttv;
    rp->TimeOfWVTExpiration.tv_sec += realToken.expirationTime;
    SetLatestCurrentTime(rp, NULL, NULL);
    rp->VVOnSource = tsync.VV;
    (void) UpdateLocal(rp, &rp->v.VolIDOfSecondary, NULL, rp->MyReplicaAggregate);
    rp->nextLookTime.tv_sec = 0;
    texptime = rp->TimeOfWVTExpiration.tv_sec - rp->tokenLeadTime;
    pthread_mutex_lock(&rp->hostP->rh_fsMutex);
    hexptime = rp->hostP->rh_fsLastSuccess
	  + rp->hostP->rh_fsHostLifeGuarantee
	  - rp->hostP->rh_fsHostLeadTime;
    pthread_mutex_unlock(&rp->hostP->rh_fsMutex);
    if (hexptime < texptime) texptime = hexptime;
    return (texptime);
}

static int32 CloneAVolume(rp, lvp, newlvpp, timep)
struct replica *rp;
struct localvol *lvp, **newlvpp;
u_int32 *timep;
{/* Make and return a volume that's a clone of *lvp. */
    struct localvol *lv;
    struct freeVolID *fvid;
    struct vol_status newStat;
    int volSrc, volDest;
    long code;
    afs_hyper_t bigUsage;
    vol_statusDesc_t statusArray[2];

    bzero((char *)&newStat, sizeof(newStat));
    /* Get old volume's status. */
    code = GetLVStatus(rp, lvp, &newStat);
    if (code != 0) {*timep = setNextTime(rp, rp->localDiskWait, NULL, 0); return code;}
    icl_Trace0(rep_iclSetp, REP_CLONEVOL_CALL);
    lv = newLocalVol();
    if (lv == NULL) {
	*timep = setNextTime(rp, rp->memoryWait, "CloneAVolume: no memory for a new localvol", 0);
	return ENOMEM;
    }
    fvid = getFreeVolID(rp);
    if (fvid == NULL) {
	osi_Free((opaque) lv, sizeof(struct localvol));
	*timep = setNextTime(rp, rp->memoryWait, NULL, 0);
	return ENOMEM;
    }
    lv->volId = fvid->ID;
    lv->aggr = rp->MyReplicaAggregate;
    sprintf(rp->RepMsg, "Trying to clone volume %lu,,%lu as free ID %lu,,%lu",
	    AFS_HGETBOTH(lvp->volId), AFS_HGETBOTH(fvid->ID));
    DebugPrint(rp);
    /* Overwrite key fields with new information. */
    newStat.vol_st.volId = lv->volId;
    newStat.vol_st.parentId = rp->v.VolIDOfPrimary;
    newStat.vol_dy.cloneId = rp->v.VolIDOfSecondary;
    newStat.vol_dy.backupId = rp->v.VolIDOfBackup;
    /* we'll adjust these links as we clone/unclone */
    AFS_hzero(newStat.vol_dy.llBackId);
    AFS_hzero(newStat.vol_dy.llFwdId);
    strncpy(newStat.vol_st.volName,
	    (char *)rp->v.mainVolName,
	    sizeof(newStat.vol_st.volName)-10);
    strcat(newStat.vol_st.volName, ".clone");
    newStat.vol_st.type = VOL_READONLY;
    newStat.vol_st.states = VOL_READONLY | VOL_ZAPME | VOL_TYPE_TEMP
      | VOL_REPSERVER_MGD;
    newStat.vol_st.states |=
	((rp->Flags & repflag_ReleaseOnly) ? VOL_REP_RELEASE : VOL_REP_LAZY);
    newStat.vol_st.accError = 0;
    newStat.vol_st.accStatus = 0;
    AFS_hset64(bigUsage, 0, 100000000);
    if (AFS_hcmp(bigUsage, newStat.vol_dy.allocLimit) < 0)
	bigUsage = newStat.vol_dy.allocLimit;
    if (AFS_hcmp(bigUsage, newStat.vol_dy.allocUsage) < 0)
	bigUsage = newStat.vol_dy.allocUsage;
    if (AFS_hcmp(bigUsage, newStat.vol_dy.visQuotaLimit) < 0)
	bigUsage = newStat.vol_dy.visQuotaLimit;
    if (AFS_hcmp(bigUsage, newStat.vol_dy.visQuotaUsage) < 0)
	bigUsage = newStat.vol_dy.visQuotaUsage;
    newStat.vol_dy.allocLimit = ftu_unlimitedQuota;
    newStat.vol_dy.visQuotaLimit = bigUsage;
    osi_GetTime(((struct timeval *) &newStat.vol_st.cloneTime));
    /* Need to come up with quota/owner/.... information from somewhere, no? XXX */
    code = AG_VOLCREATE(lv->aggr, &lv->volId, &newStat, 0);
    icl_Trace2(rep_iclSetp, REP_CLONEVOL_CREATE, 
	       ICL_TYPE_LONG, code, 
	       ICL_TYPE_LONG, errno);
    if (code != 0) {
	if (code == ENOENT) code = FTSERVER_ERR_BADAGGREGATE;
	sprintf(rp->RepMsg, "CloneAVolume: can't create new replica volume: %s",
		dfs_dceErrTxt(code));
	/* Maybe it existed already.  Try to destroy it. */
	(void) DestroyVolume(rp, lv, 1);
	osi_Free((opaque) lv, sizeof(struct localvol));
	freeFreeVolID(fvid);
	LogPrint(rp);
	*timep = setNextTime(rp, rp->localDiskWait, NULL, 0);
	return code;
    }
    code = GetLVStatus(rp, lv, 0);
    if (code != 0) {
	DestroyVolume(rp, lv, 0);
	osi_Free((opaque) lv, sizeof(struct localvol));
	freeFreeVolID(fvid);
	*timep = setNextTime(rp, rp->localDiskWait, NULL, 0);
	return code;
    }

    /* Here's where we should clone all the files. */
    icl_Trace1(rep_iclSetp, REP_CLONEVOL_AGOPEN,
		ICL_TYPE_LONG, 1);
    code = ftu_AggrOpenFset(lvp->aggr, &lvp->volId,
			    FTU_SYS_SET_STATUS | VOL_SYS_CLONE | FTU_SYS_SYNC |
			    FTU_SYS_PUSH_STATUS,
			    VOLERR_TRANS_REPCLONE, &volSrc);
    if (code != 0) {
	sprintf(rp->RepMsg, "CloneAVolume: can't open source volume: %s",
		dfs_dceErrTxt(code));
	DestroyVolume(rp, lv, 0);
	osi_Free((opaque) lv, sizeof(struct localvol));
	freeFreeVolID(fvid);
	LogPrint(rp);
	NoticeIfDeleted(rp, lvp, code);
	*timep = setNextTime(rp, rp->localDiskWait, NULL, 0);
	return code;
    }
    icl_Trace1(rep_iclSetp, REP_CLONEVOL_AGOPEN,
		ICL_TYPE_LONG, 2);
    code = ftu_AggrOpenFset(lv->aggr, &lv->volId,
			    FTU_SYS_SET_STATUS | VOL_SYS_CLONE | FTU_SYS_SYNC
			    | FTU_SYS_PUSH_STATUS,
			    VOLERR_TRANS_REPCLONE, &volDest);
    if (code != 0) {
	sprintf(rp->RepMsg, "CloneAVolume: can't open new destination volume: %s",
		dfs_dceErrTxt(code));
	(void) ftu_AggrAbortFsetOpen(volSrc);
	DestroyVolume(rp, lv, 0);
	osi_Free((opaque) lv, sizeof(struct localvol));
	freeFreeVolID(fvid);
	LogPrint(rp);
	*timep = setNextTime(rp, rp->localDiskWait, NULL, 0);
	return code;
    }
    /* Set the low-level links in both filesets before we clone anything. */
    icl_Trace0(rep_iclSetp, REP_CLONEVOL_SETSTAT);
    statusArray[0].vsd_volDesc = volSrc;
    statusArray[0].vsd_mask = VOL_STAT_LLFWDID;
    statusArray[0].vsd_status.vol_dy.llFwdId = lv->volId;
    statusArray[1].vsd_volDesc = volDest;
    statusArray[1].vsd_mask = VOL_STAT_LLBACKID;
    statusArray[1].vsd_status.vol_dy.llBackId = lvp->volId;
    code = ftu_FsetBulkSetStatus(2, statusArray);
    if (code != 0) {
	sprintf(rp->RepMsg, "CloneAVolume: can't set low-level IDs: %s",
		dfs_dceErrTxt(code));
    } else {
	icl_Trace0(rep_iclSetp, REP_CLONEVOL_PUSHSTAT);
	code = ftu_FsetPushStatus(volSrc);
	if (code != 0) {
	    sprintf(rp->RepMsg, "CloneAVolume: can't pre-sync old volume: %s",
		dfs_dceErrTxt(code));
	}
	code = ftu_FsetPushStatus(volDest);
	if (code != 0) {
	    sprintf(rp->RepMsg, "CloneAVolume: can't pre-sync new volume: %s",
		dfs_dceErrTxt(code));
	}
    }
    icl_Trace0(rep_iclSetp, REP_CLONEVOL_VCLONE);
    if (code == 0) {
	code = ftutil_VolClone(volSrc, volDest, rp->RepMsg);
    }
    if (code != 0) {
	(void) ftu_AggrAbortFsetOpen(volSrc);
	(void) ftu_AggrAbortFsetOpen(volDest);
	DestroyVolume(rp, lv, 0);
	osi_Free((opaque) lv, sizeof(struct localvol));
	freeFreeVolID(fvid);
	LogPrint(rp); /* ftutil_VolClone() composes the message */
	*timep = setNextTime(rp, rp->localDiskWait, NULL, 0);
	return code;
    }
    icl_Trace0(rep_iclSetp, REP_CLONEVOL_SYNC);
    code = ftu_FsetSync(volSrc, FTU_FSET_SYNC_COMMIT_META);
    if (code != 0) {
	sprintf(rp->RepMsg, "CloneAVolume: can't sync source volume %lu,,%lu: %s",
		AFS_HGETBOTH(lv->volId), dfs_dceErrTxt(code));
	(void) ftu_AggrAbortFsetOpen(volSrc);
	LogPrint(rp);
	return code;
    }
    icl_Trace0(rep_iclSetp, REP_CLONEVOL_CLOSE);
    /* done cloning the source fileset */
    code = ftu_AggrCloseFset(volSrc);
    if (code != 0) {
	sprintf(rp->RepMsg, "CloneAVolume: can't close source volume: %s",
		dfs_dceErrTxt(code));
	(void) ftu_AggrAbortFsetOpen(volDest);
	DestroyVolume(rp, lv, 0);
	osi_Free((opaque) lv, sizeof(struct localvol));
	freeFreeVolID(fvid);
	LogPrint(rp);
	*timep = setNextTime(rp, rp->localDiskWait, NULL, 0);
	return code;
    }
    icl_Trace0(rep_iclSetp, REP_CLONEVOL_SYNC_2);
    code = ftu_FsetSync(volDest, FTU_FSET_SYNC_COMMIT_META);
    if (code != 0) {
	sprintf(rp->RepMsg, "CloneAVolume: can't sync destination volume %lu,,%lu: %s",
		AFS_HGETBOTH(lv->volId), dfs_dceErrTxt(code));
	(void) ftu_AggrAbortFsetOpen(volDest);
	LogPrint(rp);
	return code;
    }
    icl_Trace0(rep_iclSetp, REP_CLONEVOL_CLOSE_2);
    code = ftu_AggrCloseFset(volDest);
    if (code != 0) {
	sprintf(rp->RepMsg, "CloneAVolume: can't close destination volume: %s",
		dfs_dceErrTxt(code));
	DestroyVolume(rp, lv, 0);
	osi_Free((opaque) lv, sizeof(struct localvol));
	freeFreeVolID(fvid);
	LogPrint(rp);
	*timep = setNextTime(rp, rp->localDiskWait, NULL, 0);
	return code;
    }
    lv->flags |= lvf_FreeID;
    lv->Next = rp->AllLocals;
    rp->AllLocals = lv;
    *newlvpp = lv;
    rp->fvptr = fvid;
    strcpy(rp->RepMsg, "All OK with making the clone.");
    DebugPrint(rp);
    rp->nextLookTime.tv_sec = 0;
    *timep = osi_Time() + 10*60*60;
    return 0;
}

static int32 NewVolume(rp, newlvpp, timep)
struct replica *rp;
struct localvol **newlvpp;
u_int32 *timep;
{/* Make and return a new, empty volume for rp.  VolID is rovol's, name is foo.T. */
    struct localvol *lv;
    struct vol_status newStat;
    int code;

    for (;;) {
	lv = newLocalVol();
	if (lv == NULL) {
	    *timep = setNextTime(rp, rp->memoryWait, "NewVolume: no memory for a new localvol", 0);
	    return ENOMEM;
	}
	lv->volId = rp->v.VolIDOfSecondary;
	lv->aggr = rp->MyReplicaAggregate;
	bzero((char *)&newStat, sizeof(newStat));
	sprintf(rp->RepMsg, "Making a brand-new secondary %lu,,%lu",
		AFS_HGETBOTH(lv->volId));
	DebugPrint(rp);
	newStat.vol_st.volId = lv->volId;
	newStat.vol_st.parentId = rp->v.VolIDOfPrimary;
	newStat.vol_dy.cloneId = rp->v.VolIDOfSecondary;
	newStat.vol_dy.backupId = rp->v.VolIDOfBackup;
	/* AFS_hzero(newStat.vol_dy.llBackId); */
	/* AFS_hzero(newStat.vol_dy.llFwdId); */
	strncpy(newStat.vol_st.volName,
		(char *)rp->v.mainVolName,
		sizeof(newStat.vol_st.volName)-10);
	strcat(newStat.vol_st.volName, ".readonly");
	newStat.vol_st.type = VOL_READONLY;
	newStat.vol_st.states = VOL_READONLY | VOL_ZAPME | VOL_TYPE_RO
	  | VOL_REPSERVER_MGD;
	newStat.vol_st.states |=
	    ((rp->Flags & repflag_ReleaseOnly) ? VOL_REP_RELEASE : VOL_REP_LAZY);
	/* The fileset restoration will set the quota information correctly. */
	newStat.vol_dy.allocLimit = ftu_unlimitedQuota;
	AFS_hset64(newStat.vol_dy.visQuotaLimit, 0, 0x10000000);
	osi_GetTime(((struct timeval *) &newStat.vol_dy.creationDate));
	newStat.vol_st.cloneTime = newStat.vol_dy.creationDate;
	code = AG_VOLCREATE(lv->aggr, &lv->volId, &newStat, 0);
	if (code == 0)
	    break;
	if (code == ENOENT) code = FTSERVER_ERR_BADAGGREGATE;
	if (code != EEXIST) {
	    osi_Free((opaque) lv, sizeof(struct localvol));
	    *timep = setNextTime(rp, rp->localDiskWait, "NewVolume: can't create new replica volume: %s",
				 dfs_dceErrTxt(code));
	    return code;
	}
	/* The fileset exists already.  Maybe destroy it. */
	code = DestroyVolume(rp, lv, 1);
	osi_Free((opaque) lv, sizeof(struct localvol));
	/* >0 means ordinary errors.  -1 means not locally managed. */
	if (code != 0) {
	    *timep = setNextTime(rp, rp->localDiskWait, NULL, 0);
	    return code;
	}
	/* Else loop back to try the creation again. */
    }
    code = GetLVStatus(rp, lv, NULL);
    if (code != 0) {
	DestroyVolume(rp, lv, 0);
	osi_Free((opaque) lv, sizeof(struct localvol));
	*timep = setNextTime(rp, rp->localDiskWait, NULL, 0);
	return code;
    }
    lv->Next = rp->AllLocals;
    rp->AllLocals = lv;
    *newlvpp = lv;
    strcpy(rp->RepMsg, "Brand-new volume made just fine.");
    DebugPrint(rp);
    rp->nextLookTime.tv_sec = 0;
    *timep = osi_Time() + 10*60*60;
    return 0;
}

static int32 CloneLocalReplica(rp, timep)
struct replica *rp;
u_int32 *timep;
{/* Clone or re-clone the R/W into the local published volume.
 * If there's already a clone, it's ``rp->PublishedP''.
  * Return 0 if all done, InProgress if not done yet, and anything else on
  * error.
  */
    struct localvol *rwlv, *newlvp;
    u_int32 newStates;
    struct vol_status rwStatus;
    afs_hyper_t idTemp;
    long code;
    int rwVolDesc;
    char newVolName[VOLNAMESIZE];
    char *reason;

    osi_assert(HAVE_MY_WVT(AFS_hgetlo(rp->WVT_TypesLeft)));

    osi_GetTime(&rp->TimeOfLastRetrievalTry);
    /* set a default timeout */
    *timep = rp->TimeOfLastRetrievalTry.tv_sec + 10*60*60;

    if (rp->PublishedP != NULL &&
	AFS_hiszero(rp->PublishedP->llFwdId)) {
	/* Looks like this isn't a clone!  Gotta get rid of it before starting. */
	sprintf(rp->RepMsg, "CloneLocalReplica: Published isn't a clone; destroying it.");
	LogPrint(rp);
	newlvp = rp->PublishedP;
	rp->PublishedP = NULL;	/* condemn PublishedP/newlvp */
	code = DestroyVolume(rp, newlvp, 0);
	if (code != ENODEV && code != FTU_E_NO_SUCH_FSET) {
	    sprintf(rp->RepMsg, "CloneLocalReplica: can't destroy non-cloned RO %lu,,%lu: %s",
		    AFS_HGETBOTH(newlvp->volId), dfs_dceErrTxt(code));
	    LogPrint(rp);
	    *timep = setNextTime(rp, rp->localDiskWait, NULL, 0);
	    return code;
	}
	newlvp->flags |= lvf_Deleted;
	/* Recovery accomplished! */
    }

    rwlv = newLocalVol();
    if (rwlv == NULL) {
	*timep = setNextTime(rp, rp->memoryWait, "CloneLocalReplica: no memory for a new localvol", 0);
	return ENOMEM;
    }
    rwlv->aggr = rp->v.PrimaryAggregate;
    rwlv->volId = rp->v.VolIDOfPrimary;
    sprintf(rp->RepMsg, "Trying to clone R/W volume %lu,,%lu",
	    AFS_HGETBOTH(rwlv->volId));
    DebugPrint(rp);
    /* Keep the rp->lockObject locked, blocking revocations, while we open: */
    code = ftu_AggrOpenFset(rwlv->aggr, &rwlv->volId,
			    FTU_SYS_GET_STATUS | FTU_SYS_SET_STATUS
			    | FTU_SYS_CLONE | FTU_SYS_RECLONE | FTU_SYS_SYNC
			    | FTU_SYS_PUSH_STATUS,
			    VOLERR_TRANS_REPCLONE, &rwVolDesc);
    if (code != 0) {
	sprintf(rp->RepMsg, "CloneLocalReplica: can't open source R/W volume: %s",
		dfs_dceErrTxt(code));
	LogPrint(rp);
	osi_Free((opaque) rwlv, sizeof(struct localvol));
	*timep = setNextTime(rp, rp->localDiskWait, NULL, 0);
	return code;
    }
    lock_ReleaseWrite(&rp->lockObject);

    icl_Trace2(rep_iclSetp, REP_TRACE_CLONELOCAL_RWOPENED,
	       ICL_TYPE_HYPER, &rwlv->volId,
	       ICL_TYPE_LONG, rwVolDesc);
    /* Check whether the R/W is in a suitable state for copying. */
    code = ftu_FsetGetStatus(rwVolDesc, &rwStatus);
    if (code != 0) {
	lock_ObtainWrite(&rp->lockObject);
	sprintf(rp->RepMsg, "CloneLocalReplica: can't get status of R/W volume: %s",
		dfs_dceErrTxt(code));
	LogPrint(rp);
	(void) ftu_AggrCloseFset(rwVolDesc);
	osi_Free((opaque) rwlv, sizeof(struct localvol));
	*timep = setNextTime(rp, rp->localDiskWait, NULL, 0);
	return code;
    }

    lock_ObtainWrite(&rp->lockObject);
    CopyLVStatus(rwlv, &rwStatus);
    code = (rwlv->itsStates
	    & (VOL_DELONSALVAGE | VOL_MOVE_SOURCE | VOL_ZAPME));
    icl_Trace2(rep_iclSetp, REP_TRACE_CLONELOCAL_RWGOTSTAT,
	       ICL_TYPE_HYPER, &rwlv->volId,
	       ICL_TYPE_LONG, code);
    if (code) {
	(void) ftu_AggrCloseFset(rwVolDesc);
	if (code & VOL_DELONSALVAGE)
	    reason = "inconsistent";
	else if (code & VOL_MOVE_SOURCE)
	    reason = "being moved elsewhere";
	else
	    reason = "being deleted";
	sprintf(rp->RepMsg, "CloneLocalReplica: R/W volume is %s; states %#lx",
		reason, rwlv->itsStates);
	LogPrint(rp);
	osi_Free((opaque) rwlv, sizeof(struct localvol));
	*timep = setNextTime(rp, rp->localDiskWait, NULL, 0);
	return EINVAL;
    }
    icl_Trace2(rep_iclSetp, REP_TRACE_CLONELOCAL_GOING,
	       ICL_TYPE_HYPER, &rwlv->volId,
	       ICL_TYPE_POINTER, rp->PublishedP);
    idTemp = rp->v.VolIDOfSecondary;
    if (rp->PublishedP == NULL) {
	strncpy(newVolName,
		(char *)rp->v.mainVolName,
		sizeof(newVolName)-10);
	strcat(newVolName, ".readonly");
	lock_ReleaseWrite(&rp->lockObject);
	code = ftu_FsetClone(rwVolDesc, &idTemp, newVolName,
			     VOL_TYPE_RO | VOL_READONLY | VOL_REPSERVER_MGD
			     | VOL_REP_LAZY);
    } else {
	lock_ReleaseWrite(&rp->lockObject);
	code = ftu_FsetRecloneInto(rwVolDesc, &idTemp);
    }
    lock_ObtainWrite(&rp->lockObject);
    if (rp->PublishedP) {
	/* no matter what happened, we no longer know this vol's status */
	rp->PublishedP->flags &= ~lvf_GotStatus;
    }
    if (code) {
	(void) ftu_AggrAbortFsetOpen(rwVolDesc);
	sprintf(rp->RepMsg, "CloneLocalReplica: can't %s R/W %lu,,%lu: %s",
		(rp->PublishedP == NULL ? "clone" : "reclone"),
		AFS_HGETBOTH(rwlv->volId), dfs_dceErrTxt(code));
	LogPrint(rp);
	osi_Free((opaque) rwlv, sizeof(struct localvol));
	*timep = setNextTime(rp, rp->localDiskWait, NULL, 0);
	return code;
    }
    code = ftu_AggrCloseFset(rwVolDesc);
    if (code != 0) {
	sprintf(rp->RepMsg, "CloneLocalReplica: can't close R/W vol %lu,,%lu: %s",
		AFS_HGETBOTH(rwlv->volId), dfs_dceErrTxt(code));
	LogPrint(rp);
	osi_Free((opaque) rwlv, sizeof(struct localvol));
	*timep = setNextTime(rp, rp->localDiskWait, NULL, 0);
	return code;
    }
    osi_Free((opaque) rwlv, sizeof(struct localvol));
    rwlv = NULL;

    icl_Trace2(rep_iclSetp, REP_TRACE_CLONELOCAL_GONE,
	       ICL_TYPE_HYPER, &rp->v.VolIDOfPrimary,
	       ICL_TYPE_HYPER, &rp->v.VolIDOfSecondary);
    /* Now finish the situation with the published volume */
    newlvp = rp->PublishedP;
    if (newlvp == NULL) {
	newlvp = newLocalVol();
	if (newlvp == NULL) {
	    *timep = setNextTime(rp, rp->memoryWait, "CloneLocalReplica: no memory for a new localvol", 0);
	    return ENOMEM;
	}
    }
    newlvp->aggr = rp->MyReplicaAggregate;
    newlvp->volId = rp->v.VolIDOfSecondary;
    code = GetLVStatus(rp, newlvp, NULL);
    if (code != 0) {
	sprintf(rp->RepMsg,
		"CloneLocalReplica: can't get status on freshly-cloned volume %u,,%u: %s",
		AFS_HGETBOTH(newlvp->volId), dfs_dceErrTxt(code));
	LogPrint(rp);
	if (rp->PublishedP == NULL) {
	    DestroyVolume(rp, newlvp, 1);
	    osi_Free((opaque) newlvp, sizeof(struct localvol));
	}
	*timep = setNextTime(rp, rp->localDiskWait, NULL, 0);
	return code;
    }
    newStates = newlvp->itsStates;
    newStates &= ~(VOL_TYPEFIELD | VOL_REPFIELD | VOL_ZAPME | VOL_OUTOFSERVICE
		   | VOL_OFFLINE | VOL_RW | VOL_CLONEINPROG);
    newStates |= (VOL_IS_COMPLETE | VOL_REPSERVER_MGD | VOL_TYPE_RO
		  | VOL_READONLY);
    newStates |= ((rp->Flags & repflag_ReleaseOnly) ?
		  VOL_REP_RELEASE
		  : VOL_REP_LAZY);
    icl_Trace3(rep_iclSetp, REP_TRACE_CLONELOCAL_FIXED,
	       ICL_TYPE_HYPER, &newlvp->volId,
	       ICL_TYPE_LONG, newlvp->itsStates,
	       ICL_TYPE_LONG, newStates);
    code = SetLVStatus(rp, newlvp, newStates);
    if (code != 0) {
	sprintf(rp->RepMsg,
		"CloneLocalReplica: can't set status on freshly-cloned volume %u,,%u: %s",
		AFS_HGETBOTH(newlvp->volId), dfs_dceErrTxt(code));
	LogPrint(rp);
	if (rp->PublishedP == NULL) {
	    DestroyVolume(rp, newlvp, 1);
	    osi_Free((opaque) newlvp, sizeof(struct localvol));
	}
	*timep = setNextTime(rp, rp->localDiskWait, NULL, 0);
	return code;
    }
    SetLatestCurrentTime(rp, newlvp, NULL);
    if (rp->PublishedP == NULL) {
	/* we allocated newlvp: quick, this is the published one! */
	newlvp->Next = rp->AllLocals;
	rp->AllLocals = newlvp;
	rp->PublishedP = newlvp;
    }
    osi_GetTime(&rp->LastPublishTime);
    if (HAVE_MY_WVT(AFS_hgetlo(rp->WVT_TypesLeft))) {
	strcpy(rp->RepMsg, "All OK with making a published clone.");
	DebugPrint(rp);
	rp->nextLookTime.tv_sec = 0;
    } else {
	strcpy(rp->RepMsg, "Lost our token while making a published clone.");
	DebugPrint(rp);
	*timep = setNextTime(rp, rp->remoteBusyWait, NULL, 0);
	return ELOOP;
    }
    return 0;
}

static int32 GetIncrementalDump(rp, lv, versp, timep)
struct replica *rp;
struct localvol *lv;
afs_hyper_t *versp;
u_int32 *timep;
{/* Get an incremental dump from the primary for *rp and apply it to lv.
  * The incremental should go back to version vers.
  * Return 0 if all done, InProgress if not done yet, and anything else on
  * error.
  */
    int code, Ix;
    int secCode = 0;
    u_long primTr, secTr, newStates, delay;
    rep_host_t *localHostP;
    rep_host_conn_t *localConnP;
    char pNameBuf[1024];
    u_long st;
    struct ftserver_Date dumpDate;
    struct ftserver_dest dumpDest;
    struct ftserver_status ftstat;
    extern char *ExceptionToString();
    char *excNameP;
    char excBuff[50];
    rep_host_conn_t *connP;
    rep_host_t *hostP;
    struct timeval Now;
#if !defined(OSF_NO_SOCKET_DUMP)
    ftserver_addr_t primAddr;
    ftserver_addr_t secAddr;
#endif /* OSF_NO_SOCKET_DUMP */

    osi_assert(HAVE_MY_WVT(AFS_hgetlo(rp->WVT_TypesLeft)));

    icl_Trace1(rep_iclSetp, REP_INCRDUMP_CALL, 
	       ICL_TYPE_HYPER, &lv->volId);
    if (versp != 0) 
	icl_Trace1(rep_iclSetp, REP_INCRDUMP_CALL, 
		   ICL_TYPE_HYPER, versp);

    osi_GetTime(&rp->TimeOfLastRetrievalTry);

    /* Ensure we have a connection to the primary's volume server */
    if ((connP = GET_FT_CONN(rp, "GetIncrementalDump-1")) == NULL) {
	*timep = setNextTime(rp, rp->memoryWait, NULL, 0);
	return -1;
    }

    /* we now have a connection to volume server where primary is stored. */

    if (!HAVE_MY_WVT(AFS_hgetlo(rp->WVT_TypesLeft))) {
	reph_PutConn(connP);
	*timep = setNextTime(rp, rp->remoteBusyWait,
			     "GetIncrementalDump: aborted: lost WVT", 0);
	return -1;	/* whoops: lost token */
    }

    printBinding("rmt ftsrv conn before call", connP->rhc_handle);
    if (rp->Flags & repflag_ReleaseOnly) {
	lock_ReleaseWrite(&rp->lockObject);
	code = FTSERVER_CreateTrans(connP->rhc_handle,
				    &rp->v.VolIDOfSecondary,
				    rp->v.StageAggregate,
				    FLAGS_ENCODE(FTSERVER_OP_DUMP
						 |FTSERVER_OP_GETSTATUS,
						 VOLERR_TRANS_REPFORWARD),
				    &primTr);
    } else {
	lock_ReleaseWrite(&rp->lockObject);
	code = FTSERVER_CreateTrans(connP->rhc_handle,
				    &rp->v.VolIDOfPrimary,
				    rp->v.PrimaryAggregate,
				    FLAGS_ENCODE(FTSERVER_OP_DUMP
						 |FTSERVER_OP_GETSTATUS,
						 VOLERR_TRANS_REPFORWARD),
				    &primTr);
    }
    code = osi_errDecode(code);
    printBinding("rmt ftsrv conn after call", connP->rhc_handle);

    lock_ObtainWrite(&rp->lockObject);

    if (code != 0) {
	long code2 = ResetConn(rp, NULL, &connP, code, "GetIncrementalDump-2");

	if (connP != NULL)
	    reph_PutConn(connP);

	delay = rp->remoteBusyWait;
	if (code == rpc_s_auth_tkt_expired) {
	    /* ResetConn took care of auth context refresh */
	    if (code2 == 0 && delay > 5) delay = 5;	/* quick turn-around */
	}
	*timep = setNextTime(rp, delay,
			 "GetIncrementalDump: can't open remote source ft: %s",
			     dfs_dceErrTxt(code));
	return code;
    }

    if (!HAVE_MY_WVT(AFS_hgetlo(rp->WVT_TypesLeft))) {
	lock_ReleaseWrite(&rp->lockObject);
	(void) FTSERVER_DeleteTrans(connP->rhc_handle, primTr);
	lock_ObtainWrite(&rp->lockObject);

	reph_PutConn(connP);

	*timep = setNextTime(rp, rp->remoteBusyWait,
			     "GetIncrementalDump: aborted: lost WVT", 0);
	return -1;	/* whoops: lost token */
    }

    lock_ReleaseWrite(&rp->lockObject);

    code = FTSERVER_GetStatus(connP->rhc_handle, primTr, &ftstat);
    code = osi_errDecode(code);

    lock_ObtainWrite(&rp->lockObject);

    if (code != 0) {
	long code2 = ResetConn(rp, NULL, &connP, code, "GetIncrementalDump-3");

	if (connP != NULL) {
	    lock_ReleaseWrite(&rp->lockObject);
	    (void) FTSERVER_DeleteTrans(connP->rhc_handle, primTr);
	    lock_ObtainWrite(&rp->lockObject);

	    reph_PutConn(connP);
	}

	delay = rp->remoteBusyWait;
	if (code == rpc_s_auth_tkt_expired && code2 == 0 && delay > 5)
	    delay = 5;			/* quick turn-around */
	*timep = setNextTime(rp, delay,
		      "GetIncrementalDump: can't get ft status on primary: %s",
			     dfs_dceErrTxt(code));
	return code;
    }

    if (!HAVE_MY_WVT(AFS_hgetlo(rp->WVT_TypesLeft))) {
	lock_ReleaseWrite(&rp->lockObject);
	(void) FTSERVER_DeleteTrans(connP->rhc_handle, primTr);
	lock_ObtainWrite(&rp->lockObject);

	reph_PutConn(connP);

	*timep = setNextTime(rp, rp->remoteBusyWait,
			     "GetIncrementalDump: aborted: lost WVT", 0);
	return -1;	/* whoops: lost token */
    }

    if (ftstat.vss.states & VOL_DELONSALVAGE) {
	lock_ReleaseWrite(&rp->lockObject);
	(void) FTSERVER_AbortTrans(connP->rhc_handle, primTr);
	lock_ObtainWrite(&rp->lockObject);

	reph_PutConn(connP);

	*timep = setNextTime(rp, rp->localDiskWait,
			   "GetIncrementalDump: source is inconsistent (%#lx)",
			     ftstat.vss.states);
	return FTS_ERR_INCONSISTENT;
    }

    localHostP = reph_GetHost(myNetAddrs, numMyNetAddrs, localHostName);
    if (localHostP == NULL) {
	lock_ReleaseWrite(&rp->lockObject);
	(void) FTSERVER_AbortTrans(connP->rhc_handle, primTr);
	lock_ObtainWrite(&rp->lockObject);

	reph_PutConn(connP);

	*timep = setNextTime(rp, rp->memoryWait,
			     "GetIncrementalDump: out of memory");
	return REP_ERR_NOSTORAGE;
    }

    lock_ReleaseWrite(&rp->lockObject);

    code = reph_GetConn(localHostP, REPH_CONN_TYPE_FT, &localConnP);

    lock_ObtainWrite(&rp->lockObject);

    if (code != 0) {
	lock_ReleaseWrite(&rp->lockObject);
	(void) FTSERVER_DeleteTrans(connP->rhc_handle, primTr);
	lock_ObtainWrite(&rp->lockObject);

	reph_PutConn(connP);
	reph_PutHost(localHostP);

	*timep = setNextTime(rp, rp->memoryWait, NULL, 0);
	return -1;
    }
    
    lock_ReleaseWrite(&rp->lockObject);

    printBinding("lcl ftsrv conn before call", localConnP->rhc_handle);
    code = FTSERVER_CreateTrans(localConnP->rhc_handle, &lv->volId, lv->aggr,
				FLAGS_ENCODE(FTSERVER_OP_RESTORE,
					     VOLERR_TRANS_REPFORWARD),
				&secTr);
    code = osi_errDecode(code);
    printBinding("lcl ftsrv conn after call", localConnP->rhc_handle);

    lock_ObtainWrite(&rp->lockObject);

    if (code != 0) {
	long code2;

	lock_ReleaseWrite(&rp->lockObject);
	code2 = reph_ResetConn(localHostP, &localConnP, code);
	(void) FTSERVER_DeleteTrans(connP->rhc_handle, primTr);
	lock_ObtainWrite(&(rp->lockObject));

	reph_PutConn(connP);
	if (localConnP != NULL)
	    reph_PutConn(localConnP);
	reph_PutHost(localHostP);

	delay = rp->remoteBusyWait;
	if (code == rpc_s_auth_tkt_expired && code2 == 0 && delay > 5)
	    delay = 5;			/* quick turn-around */
	*timep = setNextTime(rp, delay,
			    "GetIncrementalDump: can't open local dest ft: %s",
			     dfs_dceErrTxt(code));
	return code;
    }

    if (!HAVE_MY_WVT(AFS_hgetlo(rp->WVT_TypesLeft))) {
	lock_ReleaseWrite(&rp->lockObject);
	(void) FTSERVER_DeleteTrans(connP->rhc_handle, primTr);
	(void) FTSERVER_DeleteTrans(localConnP->rhc_handle, secTr);
	lock_ObtainWrite(&rp->lockObject);

	reph_PutConn(connP);
	reph_PutConn(localConnP);
	reph_PutHost(localHostP);

	*timep = setNextTime(rp, rp->remoteBusyWait,
			     "GetIncrementalDump: aborted: lost WVT", 0);
	return -1;	/* whoops: lost token */
    }

    bzero((char *)&dumpDate, sizeof(dumpDate));
    if (versp == NULL) {	/* get entire volume */
	dumpDate.mask = 0;
    } else {			/* get only stuff since the given version */
	dumpDate.mask = 4;
	dumpDate.fromVersion = *versp;
    }
    bzero((char *)&dumpDest, sizeof(dumpDest));
    dumpDest.destSSID = 1;
    dumpDest.address = rp->v.Reps[rp->mySiteIx].AddrInfo[0];
    strncpy((char *)&dumpDest.prinName[0], (char *)localHostName,
	    sizeof(dumpDest.prinName));

    strcpy(rp->RepMsg, "Calling FTSERVER_Forward() on primary");
    DebugPrint(rp);

    lock_ReleaseWrite(&rp->lockObject);

#if !defined(OSF_NO_SOCKET_DUMP)
    /* establish a third party connection between the servers */
    if (rp->Flags & repflag_UseSocket) {
	/* put the destination server in listen mode */
	secCode = FTSERVER_ListenSock(localConnP->rhc_handle, secTr, &primAddr);
	if (!secCode) {
	    /* connect the source server to the destination server */
	    code = FTSERVER_ConnectSock(connP->rhc_handle, primTr,
					&primAddr, &secAddr);
	    if (!code) {
		/* complete the connection between the servers */
		secCode = FTSERVER_AcceptSock(localConnP->rhc_handle,
					      secTr, &secAddr);
		if (secCode) {
		    /* connection refused indicates interference */
		    if (secCode == ECONNREFUSED) {
			secCode = 0;
		    }
		    /* accept failed, use RPC pipes for data transfer */
		    code = FTSERVER_DisconnectSock(connP->rhc_handle, primTr);
		}
	    } else {
		/* RPC range or protocol error indicates down level server */
		if (code == rpc_s_op_rng_error ||
		    code == FTSERVER_ERR_BAD_PROTOCOL) {
		    code = 0;
		}
		/* connect failed, use RPC pipes for data transfer */
		secCode = FTSERVER_DisconnectSock(localConnP->rhc_handle,
						  secTr);
	    }
	/* RPC range error indicates down level server */
	}  else if (secCode == rpc_s_op_rng_error) {
	    secCode = 0;
	}
    }
#endif /* OSF_NO_SOCKET_DUMP */
    if (!code && !secCode) {
        code = FTSERVER_Forward(connP->rhc_handle, primTr, &dumpDate,
				&dumpDest, 0, secTr);
    }

    code = osi_errDecode(code);
    secCode = osi_errDecode(secCode);

    lock_ObtainWrite(&rp->lockObject);

    if (code != 0 || secCode != 0) {
	long code2;

	if (code) {
	    code2 = ResetConn(rp, NULL, &connP, code, "GetIncrementalDump-4");
	}

	if (connP != NULL) {
	    lock_ReleaseWrite(&rp->lockObject);
	    (void) FTSERVER_DeleteTrans(connP->rhc_handle, primTr);
	    lock_ObtainWrite(&rp->lockObject);

	    reph_PutConn(connP);
	}

	delay = rp->remoteBusyWait;
	if ((code == 0 || (code == rpc_s_auth_tkt_expired && code2 == 0))
	    && delay > 5) {
	    delay = 5;			/* quick turn-around */
	}

	if (secCode) {
	    code2 = ResetConn(rp, NULL, &localConnP, secCode,
			      "GetIncrementalDump-4a");
	}

	if (localConnP != NULL) {
	    lock_ReleaseWrite(&rp->lockObject);
	    if (code == FTSERVER_ERR_DISJOINT_DATE
	        || code == FTSERVER_ERR_DISJOINT_VV
	        || code == FTSERVER_ERR_DISJOINT2_DATE
	        || code == FTSERVER_ERR_DISJOINT2_VV) {
	        (void) FTSERVER_DeleteTrans(localConnP->rhc_handle, secTr);
	    } else {
	        (void) FTSERVER_AbortTrans(localConnP->rhc_handle, secTr);
	    }
	    lock_ObtainWrite(&rp->lockObject);

	    reph_PutConn(localConnP);
	}
	reph_PutHost(localHostP);

	*timep = setNextTime(rp, delay,
			     "GetIncrementalDump: can't retrieve ft: %s",
			     dfs_dceErrTxt(code?code:secCode));

	return code?code:secCode;
    }

    lock_ReleaseWrite(&rp->lockObject);

    code = FTSERVER_DeleteTrans(connP->rhc_handle, primTr);
    code = osi_errDecode(code);

    lock_ObtainWrite(&rp->lockObject);

    if (code != 0) {
	long code2 = ResetConn(rp, NULL, &connP, code, "GetIncrementalDump-5");

	if (connP != NULL)
	    reph_PutConn(connP);

	lock_ReleaseWrite(&rp->lockObject);
	(void) FTSERVER_DeleteTrans(localConnP->rhc_handle, secTr);
	lock_ObtainWrite(&rp->lockObject);

	reph_PutConn(localConnP);
	reph_PutHost(localHostP);

	delay = rp->remoteBusyWait;
	if (code == rpc_s_auth_tkt_expired && code2 == 0 && delay > 5)
	    delay = 5;	/* quick turn-around */
	*timep = setNextTime(rp, delay,
			"GetIncrementalDump: can't delete src transaction: %s",
			     dfs_dceErrTxt(code));
	return code;
    }

    reph_PutConn(connP);	/* We're done with this for good now */

    lock_ReleaseWrite(&rp->lockObject);

    code = FTSERVER_DeleteTrans(localConnP->rhc_handle, secTr);
    code = osi_errDecode(code);

    lock_ObtainWrite(&rp->lockObject);
    
    if (code != 0) {
	long code2;

	lock_ReleaseWrite(&rp->lockObject);
	code2 = reph_ResetConn(localHostP, &localConnP, code);
	lock_ObtainWrite(&rp->lockObject);

	if (localConnP != NULL)
	    reph_PutConn(localConnP);
	reph_PutHost(localHostP);

	delay = rp->remoteBusyWait;
	if (code == rpc_s_auth_tkt_expired && code2 == 0 && delay > 5)
	    delay = 5;			/* quick turn-around */
	*timep = setNextTime(rp, delay,
		       "GetIncrementalDump: can't delete dest transaction: %s",
			     dfs_dceErrTxt(code));
	return code;
    }

    reph_PutConn(localConnP);
    reph_PutHost(localHostP);

    /* See what the restoration set the local status to. */
    code = GetLVStatus(rp, lv, NULL);
    if (code != 0) {
	*timep = setNextTime(rp, rp->localDiskWait, NULL, 0);
	return code;
    }

    /*
     * If we got this far, we don't care whether or not we lost the WVT.
     * We'll publish the new replica, and then start another
     * update attempt, if necessary.
     */

    newStates = lv->itsStates;
    newStates &= ~(VOL_TYPEFIELD | VOL_REPFIELD | VOL_BUSY | VOL_ZAPME
		   | VOL_OUTOFSERVICE | VOL_OFFLINE);
    newStates |= (VOL_IS_COMPLETE | VOL_REPSERVER_MGD | VOL_TYPE_RO);
    newStates |= ((rp->Flags & repflag_ReleaseOnly) ?
		  VOL_REP_RELEASE
		  : VOL_REP_LAZY);
    code = SetLVStatus(rp, lv, newStates);
    strcpy(rp->RepMsg, "Dump is done.");
    DebugPrint(rp);
    if (code != 0) {
	*timep = setNextTime(rp, rp->localDiskWait, NULL, 0);
	return code;
    }
    rp->nextLookTime.tv_sec = 0;
    *timep = osi_Time() + 10*60*60;
    return 0;
}

static int32 CompleteNewVol(rp, lv)
struct replica *rp;
struct localvol *lv;
{/* Correct the registry for volume *lv so that it now has the published ID and so it's listed as such in the volume registry.
     Do it atomically, if possible. */
    /* Need to change the name from foo.T to just foo, and to bring the volume back on-line. */
    int volD;
    long code, code2;
    struct vol_status volStat;

    sprintf(rp->RepMsg, "Marking new volume %lu,,%lu as complete (and changing its name).",
	     AFS_HGETBOTH(lv->volId));
    DebugPrint(rp);
    bzero((char *)&volStat, sizeof(volStat));
    code = ftu_AggrOpenFset(lv->aggr, &lv->volId,
			    FTU_SYS_GET_STATUS | FTU_SYS_SET_STATUS
			    | FTU_SYS_SYNC | VOL_SYS_SETVV,
			    VOLERR_TRANS_REPEDITSTATUS, &volD);
    if (code != 0) {
	sprintf(rp->RepMsg, "CompleteNewVol: can't open local vol %lu,,%lu: %s",
		AFS_HGETBOTH(lv->volId), dfs_dceErrTxt(code));
	LogPrint(rp);
	NoticeIfDeleted(rp, lv, code);
	return code;
    }
    icl_Trace0(rep_iclSetp, REP_COMPNEWVOL_GETSTAT);
    code = ftu_FsetGetStatus(volD, &volStat);
    if (code != 0) {
	sprintf(rp->RepMsg, "CompleteNewVol: can't get status for local vol %lu,,%lu: %s",
		AFS_HGETBOTH(lv->volId), dfs_dceErrTxt(code));
	(void) ftu_AggrAbortFsetOpen(volD);
	LogPrint(rp);
	return code;
    }
    strncpy(volStat.vol_st.volName,
	    (char *)rp->v.mainVolName,
	    sizeof(volStat.vol_st.volName)-9);
    strcat(volStat.vol_st.volName, ".readonly");
    volStat.vol_st.states &= ~(VOL_ZAPME | VOL_TYPEFIELD | VOL_RW | VOL_OFFLINE
				| VOL_OUTOFSERVICE | VOL_CLONEINPROG);
    volStat.vol_st.states |= (VOL_IS_COMPLETE | VOL_TYPE_RO | VOL_READONLY
			       | VOL_REPSERVER_MGD);
    icl_Trace0(rep_iclSetp, REP_COMPNEWVOL_SETSTAT);
    osi_GetTime(((struct timeval *) &volStat.vol_st.cloneTime));
    code = ftu_FsetSetStatus(volD, 
			     VOL_STAT_VOLNAME | VOL_STAT_STATES
			     | VOL_STAT_CLONETIME, &volStat);
    if (code != 0) {
	sprintf(rp->RepMsg, "CompleteNewVol: can't set status for local vol %lu,,%lu: %s",
		AFS_HGETBOTH(lv->volId), dfs_dceErrTxt(code));
	(void) ftu_AggrAbortFsetOpen(volD);
	LogPrint(rp);
	return code;
    }
    icl_Trace0(rep_iclSetp, REP_COMPNEWVOL_CSETOPENVOL);
    code = CheckSetOpenVol(rp, &lv->volId, lv, volD);
    icl_Trace0(rep_iclSetp, REP_COMPNEWVOL_SYNC);
    code2 = ftu_FsetSync(volD, FTU_FSET_SYNC_COMMIT_STATUS);
    if (code == 0) code = code2;
    if (code != 0) {
	sprintf(rp->RepMsg, "CompleteNewVol: can't %s new status for local vol %lu,,%lu: %s",
		(code2 ? "sync" : "setvv"),
		AFS_HGETBOTH(lv->volId), dfs_dceErrTxt(code));
	(void) ftu_AggrAbortFsetOpen(volD);
	LogPrint(rp);
	return code;
    }
    CopyLVStatus(lv, &volStat);
    icl_Trace0(rep_iclSetp, REP_COMPNEWVOL_CLOSE);
    code = ftu_AggrCloseFset(volD);
    if (code != 0) {
	sprintf(rp->RepMsg, "CompleteNewVol: can't close local vol %lu,,%lu: %s",
		AFS_HGETBOTH(lv->volId), dfs_dceErrTxt(code));
	LogPrint(rp);
	return code;
    }
    SetLatestCurrentTime(rp, lv, NULL);
    strcpy(rp->RepMsg, "CompleteNewVol: Volume now marked as complete");
    DebugPrint(rp);
    return 0;
}


static int32 EliminateJunkVolume(rp)
struct replica *rp;
{/* For any copy-on-write pointers in rp->PublishedP, overwrite them with
   pointers from rp->JunkP, wiping out whatever rp->JunkP bpointed to.
When done, trash the old rp->JunkP volume.
Basically, need to un-clone the clone, giving the newly-written volume the ID
   and name of the old one, and deleting the old one.
We have allocated the rp->fvptr structure, and need to free it when we're done
   with newlv.
 */
    long code;
    struct localvol *J, *P;
    struct localvol *lv, *lastlv;
    afs_hyper_t junkId;

    J = rp->JunkP;
    if (J == NULL) return 0;	/* nothing to do */
    P = rp->PublishedP;
    if (P == NULL) abort();

    sprintf(rp->RepMsg, "Uncloning and eliminating the old junk husk %lu,,%lu",
	     AFS_HGETBOTH(J->volId));
    DebugPrint(rp);
    junkId = J->volId;
    /* Truncate the old data in the old volume, moving the pointers up to
       the corresponding new location. */
    code = ftu_AggrDestroyFset(J->aggr, &junkId);
    if (code != 0) {
	sprintf(rp->RepMsg, "EliminateJunkVolume: can't eliminate junk volume: %s",
		dfs_dceErrTxt(code));
	LogPrint(rp);
	return code;
    }
    J->flags |= lvf_Deleted;
    rp->JunkP = NULL;
    if (rp->fvptr != NULL) {
	freeFreeVolID(rp->fvptr);
    } else {
	claimFreeVolID(&junkId);
    }
    rp->fvptr = NULL;
    for (lastlv = NULL, lv = rp->AllLocals; lv != NULL; lv = lv->Next) {
	if (lv == J) {
	    if (lastlv == NULL) {
		rp->AllLocals = lv->Next;
	    } else {
		lastlv->Next = lv->Next;
	    }
	    osi_Free((opaque) lv, sizeof(*lv));
	    break;
	} else {
	    lastlv = lv;
	}
    }
    code = GetLVStatus(rp, P, NULL);
    if (code != 0) return code;

    strcpy(rp->RepMsg, "Unclone successful.");
    DebugPrint(rp);
    return 0;
}

static int32 CompleteAndSwitch(rp)
struct replica *rp;
{/* Lock out accesses to *oldlv.
    For any copy-on-write pointers in *newlv, overwrite them with pointers from
       *oldlv, wiping out whatever *oldlv pointed to.
    Re-register the volume name and ID so that the new one gets found.
    Basically, need to un-clone the clone, giving the newly-written volume the
       ID and name of the old one, and deleting the old one.
    We have allocated the rp->fvptr structure, and need to free it when we're
       done with newlv.
 */
    int volSrc, volDest;
    long code, xcode;
    afs_hyper_t idTemp;
    struct vol_SetVV vv;
    struct timeval TV;
    struct vol_status volstat;
    register struct localvol *oldlv, *newlv;
    int needVVCall;
    vol_statusDesc_t statusArray[2];

    oldlv = rp->PublishedP;
    newlv = rp->BuildingP;

    /* Correct the on-disk status. */
    code = CompleteNewVol(rp, newlv);
    if (code != 0) return code;

    sprintf(rp->RepMsg, "Attempting to switch publication of %lu,,%lu to the new volume %lu,,%lu",
	     AFS_HGETBOTH(oldlv->volId),
	     AFS_HGETBOTH(newlv->volId));
    DebugPrint(rp);
    code = ftu_AggrOpenFset(oldlv->aggr, &oldlv->volId,
			    FTU_SYS_BULK_SET_STATUS | FTU_SYS_SYNC
			    | FTU_SYS_SET_STATUS,
			    VOLERR_TRANS_REPSWITCHFILESETS, &volSrc);
    if (code != 0) {
	sprintf(rp->RepMsg, "CompleteAndSwitch: can't open old volume to swap: %s",
		dfs_dceErrTxt(code));
	LogPrint(rp);
	NoticeIfDeleted(rp, oldlv, code);
	return code;
    }
    icl_Trace0(rep_iclSetp, REP_CSWITCH_AGOPEN);
    code = ftu_AggrOpenFset(newlv->aggr, &newlv->volId,
			    FTU_SYS_GET_STATUS | VOL_SYS_SETVV
			    | FTU_SYS_BULK_SET_STATUS | FTU_SYS_SYNC
			    | FTU_SYS_SET_STATUS,
			    VOLERR_TRANS_REPSWITCHFILESETS, &volDest);
    if (code != 0) {
	sprintf(rp->RepMsg, "CompleteAndSwitch: can't open new volume to swap: %s",
		dfs_dceErrTxt(code));
	(void) ftu_AggrAbortFsetOpen(volSrc);
	LogPrint(rp);
	NoticeIfDeleted(rp, newlv, code);
	return code;
    }
    bzero((char *)&vv, sizeof(vv));
    needVVCall = NeedSetVol(rp, &newlv->volId, newlv, &vv);
    if (needVVCall) {
	icl_Trace0(rep_iclSetp, REP_CSWITCH_SOPENVOL);
	code = SetOpenVol(rp, &newlv->volId, newlv, volDest, &vv);
	if (code != 0) {
	    (void) ftu_AggrAbortFsetOpen(volDest);
	    (void) ftu_AggrAbortFsetOpen(volSrc);
	    return code;
	}
    }

    icl_Trace0(rep_iclSetp, REP_CSWITCH_SWAPVOLIDS);
    /* Swap the IDs of the two volumes. */
    statusArray[0].vsd_volDesc = volSrc;
    statusArray[0].vsd_mask = VOL_STAT_VOLID|VOL_STAT_LLFWDID;
    statusArray[0].vsd_status.vol_st.volId = newlv->volId;
    statusArray[0].vsd_status.vol_dy.llFwdId = oldlv->volId;
    statusArray[1].vsd_volDesc = volDest;
    statusArray[1].vsd_mask = VOL_STAT_VOLID|VOL_STAT_LLBACKID;
    statusArray[1].vsd_status.vol_st.volId = oldlv->volId;
    statusArray[1].vsd_status.vol_dy.llBackId = newlv->volId;
    code = ftu_FsetBulkSetStatus(2, statusArray);
    if (code != 0) {
	sprintf(rp->RepMsg, "CompleteAndSwitch: can't swap vids; old %lu,,%lu, new %lu,,%lu: %s",
		AFS_HGETBOTH(oldlv->volId), AFS_HGETBOTH(newlv->volId),
		dfs_dceErrTxt(code));
    } else {
	/* Mirror the registry state */
	idTemp = oldlv->volId;
	oldlv->volId = newlv->volId;
	newlv->volId = idTemp;
	rp->JunkP = rp->PublishedP;
	rp->PublishedP = rp->BuildingP;
	rp->BuildingP = NULL;
	icl_Trace0(rep_iclSetp, REP_CSWITCH_GETSTATUS);
	code = ftu_FsetGetStatus(volSrc, &volstat);
	if (code != 0) {
	    sprintf(rp->RepMsg, "CompleteAndSwitch: can't get status for local vol %lu,,%lu: %s",
		    AFS_HGETBOTH(newlv->volId), dfs_dceErrTxt(code));
	} else {
	    CopyLVStatus(newlv, &volstat);
	    icl_Trace2(rep_iclSetp, REP_CSWITCH_STATES,
		       ICL_TYPE_HYPER, &newlv->volId, 
		       ICL_TYPE_LONG, newlv->itsStates);
	}

	osi_GetTime(&rp->LastPublishTime);
	icl_Trace0(rep_iclSetp, REP_CSWITCH_SYNC);
	xcode = ftu_FsetSync(volSrc, FTU_FSET_SYNC_COMMIT_STATUS);
	if (xcode != 0) {
	    if (code == 0) {
		code = xcode;
		sprintf(rp->RepMsg,
			"CompleteAndSwitch: can't sync new fileset; old %lu,,%lu, new %lu,,%lu: %s",
			AFS_HGETBOTH(oldlv->volId), AFS_HGETBOTH(newlv->volId),
			dfs_dceErrTxt(code));
	    }
	}
	icl_Trace0(rep_iclSetp, REP_CSWITCH_SYNC_2);
	xcode = ftu_FsetSync(volDest, FTU_FSET_SYNC_COMMIT_STATUS);
	if (xcode != 0) {
	    if (code == 0) {
		code = xcode;
		sprintf(rp->RepMsg,
			"CompleteAndSwitch: can't sync old fileset; old %lu,,%lu, new %lu,,%lu: %s",
			AFS_HGETBOTH(oldlv->volId), AFS_HGETBOTH(newlv->volId),
			dfs_dceErrTxt(code));
	    }
	}
    }
    if (code != 0) {
	(void) ftu_AggrAbortFsetOpen(volSrc);
	(void) ftu_AggrAbortFsetOpen(volDest);
	LogPrint(rp);
	return code;
    }
    icl_Trace0(rep_iclSetp, REP_CSWITCH_CLOSE);
    code = ftu_AggrCloseFset(volSrc);
    if (code != 0) {
	sprintf(rp->RepMsg, "CompleteAndSwitch: can't close new vol after ID swap: %s",
		dfs_dceErrTxt(code));
	(void) ftu_AggrAbortFsetOpen(volDest);
	LogPrint(rp);
    }
    icl_Trace0(rep_iclSetp, REP_CSWITCH_CLOSE_2);
    xcode = ftu_AggrCloseFset(volDest);
    if (xcode != 0) {
	sprintf(rp->RepMsg, "CompleteAndSwitch: can't close old vol after ID swap: %s",
		dfs_dceErrTxt(xcode));
	LogPrint(rp);
    }
    if (code == 0) code = xcode;
    strcpy(rp->RepMsg, "IDs all switched.");
    DebugPrint(rp);
    return (code);
}

static u_int32 StartVolumeRetrieval(rp)
struct replica *rp;
{/* Worker for ReplicaWantsAdvance. */
    register struct localvol *lv;
    struct localvol *newlv = NULL;
    int Status;
    struct timeval Now;
    u_int32 nextTime, tempTime;

    lv = rp->PublishedP;
    if (lv == NULL) {
	strcpy(rp->RepMsg, "StartVolumeRetrieval(rp, NULL)");
    } else {
	sprintf(rp->RepMsg, "StartVolumeRetrieval(rp, %lu,,%lu)",
		AFS_HGETBOTH(lv->volId));
    }
    DebugPrint(rp);
    osi_GetTime(&Now);
    nextTime = Now.tv_sec + 10*60*60;	/* default a large time */
    if (rp->nextLookTime.tv_sec != 0) {
	tempTime = rp->nextLookTime.tv_sec;
	if (tempTime > Now.tv_sec) {	/* don't try too frequently */
	    if (tempTime < nextTime) nextTime = tempTime;
	    return nextTime;
	}
    }
    if (rp->hostP != NULL && lv != NULL) {
	int doIt = 0;
	pthread_mutex_lock(&rp->hostP->rh_fsMutex);
	if (rp->hostP->rh_fsLastSuccess != lv->lastCallTime)
	    doIt = 1;
	pthread_mutex_unlock(&rp->hostP->rh_fsMutex);
	if (doIt)
	    UpdateLocal(rp, &lv->volId, lv, lv->aggr);
    }
    if (!HAVE_MY_WVT(AFS_hgetlo(rp->WVT_TypesLeft))) {
	if (!AFS_hiszero(rp->WVT_TypesLeft)) {
	    tempTime = LoseAllWVTRead(rp, &Now);
	    if (tempTime < nextTime) nextTime = tempTime;
	}
	tempTime = TryForWVTRead(rp);
	if (tempTime < nextTime) nextTime = tempTime;
	if (!HAVE_MY_WVT(AFS_hgetlo(rp->WVT_TypesLeft)))
	    return nextTime;
    }
    if ((rp->Flags & repflag_ReleaseOnly) == 0
	&& rp->BuildingP == NULL && rp->JunkP == NULL
	&& rp->v.PrimaryAggregate == rp->MyReplicaAggregate
	&& OnThisMachine(&(rp->v.PrimaryAddrs[0]), rp->v.NumPrimaryAddrs)) {
	Status = CloneLocalReplica(rp, &tempTime);
	if (tempTime < nextTime) nextTime = tempTime;
	if (Status == 0) {
	    tempTime = setNextTime(rp, 0, "StartVolumeRetrieval: Published newly-cloned volume", 0);
	    if (tempTime < nextTime) nextTime = tempTime;
	    /* XXX maybe want to record VVOnSource, or assume that the Get routine will. */
	}
    } else {
	if (lv != NULL) {
	    Status = CloneAVolume(rp, lv, &newlv, &tempTime);
	    if (tempTime < nextTime) nextTime = tempTime;
	} else {
	    Status = NewVolume(rp, &newlv, &tempTime);
	    if (tempTime < nextTime) nextTime = tempTime;
	}
	if (Status == 0) {
	    /* rp->BuildingP = newlv; */
	    Status = GetIncrementalDump(rp, newlv,
					(lv == NULL ? NULL : &lv->volVersion),
					&tempTime);
	    if (tempTime < nextTime) nextTime = tempTime;
	}
	if (Status == 0) {
	    /* success implies that we have a new nextLookTime */
	    rp->BuildingP = newlv;
	    GetLVStatus(rp, newlv, NULL);
	    tempTime = setNextTime(rp, 0, "StartVolumeRetrieval: About to publish newly-built volume", 0);
	    if (tempTime < nextTime) nextTime = tempTime;
	    /* XXX maybe want to record VVOnSource, or assume that the Get routine will. */
	} else {
	    if (rp->fvptr != NULL) {
		freeFreeVolID(rp->fvptr);
		rp->fvptr = NULL;
	    }
	    if (newlv != NULL) {
		/* only destroy if CloneAVolume() or NewVolume() worked */
		newlv->flags |= lvf_Deleted;
		DestroyVolume(rp, newlv, 0);
	    }
	}
    }
    return nextTime;
}

static int32 DeleteAllHangingState(rp)
struct replica *rp;
{/* Delete all local volumes associated with *rp */
    struct localvol *lv, *nlv;
    struct volChanged *vc, *nvc;
    long code;

    sprintf(rp->RepMsg, "Nuking hanging state for rp with sec=%lu,,%lu",
	     AFS_HGETBOTH(rp->v.VolIDOfSecondary));
    DebugPrint(rp);

    /*
     * If we fail to destroy one of the local volumes, we return the reason
     * for the failure and expect our caller to try again later.
     */
    for (lv = rp->AllLocals; lv != NULL; lv = lv->Next) {
	if (!(lv->flags & lvf_Deleted)) {
	    code = DestroyVolume(rp, lv, 0);
	    if (code != ENODEV && code != FTU_E_NO_SUCH_FSET)
		return code;
	    lv->flags |= lvf_Deleted;
	}
    }

    /* From this point on we can't fail */

    lock_Destroy(&rp->lockObject);

    if (rp->v.Reps != NULL)
	osi_Free(rp->v.Reps, rp->v.MaxReps * sizeof *rp->v.Reps);

    for (lv = rp->AllLocals; lv != NULL; lv = nlv) {
	osi_assert(lv->flags & lvf_Deleted);
	nlv = lv->Next;
	osi_Free(lv, sizeof *lv);
    }

    if (rp->fvptr != NULL)
	freeFreeVolID(rp->fvptr);

    if (rp->hostP != NULL)
	reph_PutHost(rp->hostP);

    if (rp->KA != NULL)
	osi_Free(rp->KA, rp->MaxKAs * sizeof *rp->KA);

    for (vc = rp->Changes; vc != NULL; vc = nvc) {
	nvc = vc->Another;
	if (vc->maxFids > 0)
	    osi_Free(vc->fids, vc->maxFids * sizeof *vc->fids);
	osi_Free(vc, sizeof *vc);
    }

    return 0;
}

static int32 CheckWithVLDB(rp, flags, delp)
struct replica *rp;
u_int32 flags;
int *delp;
{/* Return 0 for all-OK, >0 for persistent failures, <0 for transients. */
    struct vldbRepInfo VInfo;
    struct replica *nrp;
    int32 ret, Code;
    int32 MySiteIx, ShouldExistHere;
    char *oldR, *newR;
    static char rel[] = "release";
    static char lazy[] = "lazy";
    struct timeval Now;

    ShouldExistHere = 0;	/* 0 unk, <0 no, >0 yes */
    *delp = 0;
    bzero((char *)&VInfo, sizeof(VInfo));
    VInfo.Reps = NULL;
    ret = VLDBLookup(&localCellID, &rp->v.VolIDOfSecondary, &VInfo, &MySiteIx, &Code);
    if (ret < 0) {
	setNextTime(rp, rp->remoteBusyWait,
		    "CheckWithVLDB: Can't read VLDB: %s",
		    dfs_dceErrTxt(Code != 0 ? Code : ret));
	return (ret);	/* cannot tell just now */
    } else if (ret > 0) {
	ShouldExistHere = -1;	/* volume doesn't exist in VLDB */
    } else {	/* (ret == 0) volume exists, data all present */
	if (MySiteIx >= 0) ShouldExistHere = 1; /* our site is in the list of secs */
	else ShouldExistHere = -1;	/* our site is NOT in the list of secs */
    }
    if (ShouldExistHere < 0) {	/* nope--delete all traces */
	rp->Flags |= repflag_KillMe;
	*delp = 1;
	strcpy(rp->RepMsg, "CheckWithVLDB: Replica is moribund via CheckWithVLDB.");
	LogPrint(rp);
	return 0;
    }
    /* The volume should exist on this site. */
    sprintf(rp->RepMsg, "Refreshing VLDB information for sec=%lu,,%lu",
	     AFS_HGETBOTH(rp->v.VolIDOfSecondary));
    DebugPrint(rp);
    Code = 0;	/* flag any change */
    if ((rp->Flags & repflag_ReleaseOnly) != 0) {
	if ((VInfo.vlflags & VLF_LAZYRELE) == 0) {
	    Code = 1;
	    oldR = rel;
	    newR = lazy;
	}
    } else {
	if ((VInfo.vlflags & VLF_LAZYRELE) != 0) {
	    Code = 1;
	    oldR = lazy;
	    newR = rel;
	}
    }
    if (Code) {
	Log("%lu,,%lu CHANGED from %s-style repl to %s-style",
	    AFS_HGETBOTH(rp->v.VolIDOfSecondary),
	    oldR, newR);
    }
    if (rp->v.Reps != NULL) {	/* get a fresh copy */
	osi_Free((opaque) rp->v.Reps, rp->v.MaxReps * sizeof(struct RepSite));
	rp->v.Reps = NULL;
    }
    if (VInfo.NumPrimaryAddrs != rp->v.NumPrimaryAddrs ||
	! SameAddresses(VInfo.PrimaryAddrs, rp->v.PrimaryAddrs,
			VInfo.NumPrimaryAddrs) ||
	strcmp(VInfo.hName, rp->v.hName) != 0)
    {
	/* The address information for the primary has changed */
	Log("CheckWithVLDB: %lu,,%lu: Primary address info has changed",
	    AFS_HGETBOTH(rp->v.VolIDOfSecondary));
	osi_GetTime(&Now);
	LoseWVT(rp, &Now, &allTokenTypes, 0);
	if (rp->hostP != NULL) {
	    /* Lose the out of date host info.  The next call to
	       GetConn() will reestablish it. */
	    reph_PutHost(rp->hostP);
	    rp->hostP = NULL;
	}
    }	
    Code = VLDBCopy(&VInfo, rp, MySiteIx, rp->RepMsg, Code);
    if (VInfo.Reps != NULL)
	osi_Free((opaque) VInfo.Reps, VInfo.MaxReps * sizeof(struct RepSite));
    if (Code != 0) {
	LogPrint(rp);
	if (Code == ENOMEM) return -1;
	else return Code;
    }
    return 0;
}

static long FixUpLocals()
{/* Close any windows, handling changes that might have occurred
   between scanning the VLDB and scanning the set of local volumes. */
    struct replica *rp, *nrp;
    int del;

    lock_ObtainWrite(&RepTableLock);
    for (rp = RepRoot; rp != NULL; rp = nrp) {
	RepHold(rp);
	nrp = rp->Next;
	if ((rp->Flags & repflag_KnowVLDB) == 0) {
	    (void) CheckWithVLDB(rp, 0, &del);
	}
	RepRele(rp);
    }
    lock_ReleaseWrite(&RepTableLock);
    return 0;
}

static int32 RationalizeVolumeLists(Starting)
int Starting;
{/* Filter through the stuff for all the known volumes and get rid of the garbage. */
    struct replica *rp, *nrp;
    struct localvol *lv, *llv, *nlv, *tmplv;
    int DeletedAny, del;
    long code;
    u_long bits, bits2, mask;

    /* RW=off, READONLY=on, REPSERVER_MGD=on */
    mask = VOL_RW | VOL_READONLY | VOL_REPSERVER_MGD;
    bits = VOL_READONLY | VOL_REPSERVER_MGD;
    /* ZAPME=off, CLONEINPROG=off, OUTOFSERVICE=off, OFFLINE=off */
    mask |= (VOL_ZAPME | VOL_CLONEINPROG | VOL_OUTOFSERVICE | VOL_OFFLINE);
    if (Starting) {
	/* DEL=off,  COMPLETE=on */
	mask |= VOL_DELONSALVAGE | VOL_IS_COMPLETE;
	bits |= VOL_IS_COMPLETE;
    }
    /* REPFIELD=LAZY or REPFIELD=RELEASE */
    mask |= VOL_REPFIELD;
    bits2 = bits;
    bits |= VOL_REP_LAZY;
    bits2 |= VOL_REP_RELEASE;

    lock_ObtainWrite(&RepTableLock);
    for (rp = RepRoot; rp != NULL; rp = nrp) {
	nrp = rp->Next;
	if ((rp->Flags & repflag_KnowVLDB) == 0) {
	    code = CheckWithVLDB(rp, 0, &del);
	    if (del != 0) continue;	/* was marked for deletion */
	}
	if ((rp->Flags & repflag_KnowVLDB) == 0) continue;
	if (rp->PublishedP != NULL) {
	    for (lv = rp->AllLocals; lv != NULL; lv = lv->Next) {
		if (lv == rp->PublishedP) break;
	    }
	    if (lv == NULL) rp->PublishedP = NULL;	/* must be in list */
	}
	if (rp->BuildingP != NULL) {
	    for (lv = rp->AllLocals; lv != NULL; lv = lv->Next) {
		if (lv == rp->BuildingP) break;
	    }
	    if (lv == NULL) rp->BuildingP = NULL;	/* must be in list */
	}
	if (rp->JunkP != NULL) {
	    for (lv = rp->AllLocals; lv != NULL; lv = lv->Next) {
		if (lv == rp->JunkP) break;
	    }
	    if (lv == NULL) rp->JunkP = NULL;	/* must be in list */
	}
	DeletedAny = 0;
	for (lv = rp->AllLocals; lv != NULL; lv = tmplv) {
	    tmplv = lv->Next;
	    if ((lv->flags & lvf_GotStatus) == 0) GetLVStatus(rp, lv, NULL);
	    if ((lv->flags & lvf_GotStatus) == 0) break;
	    if ((((lv->itsStates & mask) != bits) && ((lv->itsStates & mask) != bits2))
		|| (Starting && !AFS_hsame(rp->v.VolIDOfSecondary, lv->volId)
			     && !AFS_hsame(rp->v.VolIDOfSecondary, lv->llBackId)
			     && !AFS_hsame(rp->v.VolIDOfSecondary, lv->llFwdId))) {
		icl_Trace1(rep_iclSetp, REP_INCOMP_VOL,
			   ICL_TYPE_HYPER, &lv->volId);
		code = DestroyVolume(rp, lv, 0);
		if (code == 0) {
		    DeletedAny = 1;
		}
	    }
	}
	if (DeletedAny) {
	    for (llv = NULL, lv = rp->AllLocals; lv != NULL; lv = nlv) {
		nlv = lv->Next;
		if ((lv->flags & lvf_Deleted) != 0) {
		    if (llv == NULL) {
			rp->AllLocals = lv->Next;
		    } else {
			llv->Next = lv->Next;
		    }
		    if (lv == rp->PublishedP) rp->PublishedP = NULL;
		    if (lv == rp->BuildingP) rp->BuildingP = NULL;
		    if (lv == rp->JunkP) rp->JunkP = NULL;
		    osi_Free((opaque) lv, sizeof(*lv));
		} else {
		    llv = lv;
		}
	    }
	}
	if (Starting) {
	    /* Make nlv point to the latest (complete) instance. */
	    /* Make llv point to the previous one. */
	    nlv = llv = NULL;
	    for (lv = rp->AllLocals; lv != NULL; lv = lv->Next) {
		if (nlv == NULL) nlv = lv;
		else {
		    if (AFS_hcmp(nlv->volVersion, lv->volVersion) < 0)
			nlv = lv;
		}
	    }
	    for (lv = rp->AllLocals; lv != NULL; lv = lv->Next) {
		if (lv != nlv) {
		    if (llv == NULL) llv = lv;
		    else {
			if (AFS_hcmp(llv->volVersion, lv->volVersion) < 0)
			    llv = lv;
		    }
		}
	    }
	    if (nlv != NULL) {	/* delete all the rest */
		if (llv != NULL
		    && !AFS_hiszero(nlv->llBackId)
		    && AFS_hsame(nlv->llBackId, llv->volId)) {
		    /* two volumes, one the clone of another */
		    rp->PublishedP = nlv;
		    rp->JunkP = llv;
		    rp->BuildingP = NULL;
		} else {
		    /* just keep the one volume */
		    rp->PublishedP = nlv;
		    rp->JunkP = NULL;
		    rp->BuildingP = NULL;
		    llv = NULL;
		}
		for (lv = rp->AllLocals; lv != NULL; lv = tmplv) {
		    tmplv = lv->Next;
		    if (lv != nlv && lv != llv) {
			code = DestroyVolume(rp, lv, 0);
			if (code != 0) {
			    return code;
			} else {
			    if (lv == rp->PublishedP) rp->PublishedP = NULL;
			    if (lv == rp->BuildingP) rp->BuildingP = NULL;
			    if (lv == rp->JunkP) rp->JunkP = NULL;
			    osi_Free((opaque) lv, sizeof(*lv));
			}
		    }
		}
	    }
	    if (rp->PublishedP != NULL &&
		(rp->PublishedP->flags & lvf_GotStatus)) {
		/*
		 * If we're starting up after a crash, we must make some
		 * conservative assumptions about the age of this replica.
		 *
		 * Since we write the vvCurrentTime to disk with every
		 * VOL_SETVV, we know the replica is at least as current
		 * the time recorded in vvCurrentTime (cached in diskCurr).
		 * To be conservative, we assume that we lost the token
		 * at the last known vvCurrentTime.
		 */
		rp->TimeOfLatestKnownCurrent =
		    *((struct timeval *) &rp->PublishedP->diskCurr);
	    }
	}
    }
    lock_ReleaseWrite(&RepTableLock);
    return 0;
}

static void QueueReplicas()
{
    struct replica*	rp;

    lock_ObtainRead(&RepTableLock);
    for (rp = RepRoot; rp != NULL; rp = rp->Next) {
	lock_ObtainWrite(&rp->lockObject);
	rp->qCookie = repq_Enter(&RepQ, rp, 0);
	RepHold(rp);
	lock_ReleaseWrite(&rp->lockObject);
    }
    lock_ReleaseRead(&RepTableLock);
}	/* QueueReplicas() */

static u_int32 ReplicaWantsAdvance(rp, needAdvP, delp)
struct replica *rp;
int *needAdvP;
int *delp;
{/* Called with rp write-locked.  Do any individual processing.
   Return when we need to be called again for this volume.
   Return in *needAdvP whether bulk processing should follow. */

/* OK.  The replica might have a PublishedVol or not, and a BuildingVol or not.
      The BuildingVol might be marked in-progress in the replica structure.
      Either volume might be marked ``completed'' on the disk.  Any volume
      versions present might have any relationship.

  Here are our steps.
  (A) Discard incomplete PublishedVols.
  (B) Discard incomplete BuildingVols not marked in-progress.
  (C) Promote, with warning, complete, in-progress Building vols to be not in progress.
  (D) When BuildingVols are built successfully, mark them as complete and not in progress.
  (F) Switch completed, not-in-progress BuildingVol to a PublishedVol.
  (G) Determine VV for this volume.
  (H) If lose VV, switch to adjacent relevant state.
  (I) If have P but not B, and P is out of date, create a new B vol, mark
	in progress, not complete, and initiate an update relative to the VV of P.
  (J) If have no P or B, create new B vol, mark in progress, not complete, and
	initiate a complete new volume copy.
  (K) When B update is completed, mark as such. (like D)
  (M) Promote a complete B to be P; destroy old P; and null out the old B slot. (like F)
 
Step (G) is done in bulk by the ProcessBundle call, below.
 */
    struct localvol *P, *B;
    struct localvol *lv, *llv, *nlv;
    long code;
    struct timeval Now;
    int RC;
    u_int32 nextTime, tempTime;

    *delp = 0;
    *needAdvP = FALSE;
    osi_GetTime(&Now);
    nextTime = Now.tv_sec + 10*60*60;

    icl_Trace2(rep_iclSetp, REP_WANTSADV_BEG,
	       ICL_TYPE_LONG, rp->nextLookTime.tv_sec,
	       ICL_TYPE_LONG, Now.tv_sec);

    if (rp->nextLookTime.tv_sec != 0) {
	tempTime = rp->nextLookTime.tv_sec;
	if (tempTime > Now.tv_sec) {	/* don't try too frequently */
	    if (tempTime < nextTime) nextTime = tempTime;
	    return nextTime;
	}
    }
    if ((rp->Flags & repflag_KnowVLDB) == 0) {
	code = CheckWithVLDB(rp, 0, delp);
	if (*delp != 0) return 0;	/* Delete at first opportunity */
	if ((rp->Flags & repflag_KnowVLDB) == 0) return rp->nextLookTime.tv_sec;
    }

    icl_Trace0(rep_iclSetp, REP_WANTSADV_KNOWVLDB);

    /* Eliminate all the junk or deleted or zapped volumes right away. */
    llv = NULL;
    for (lv = rp->AllLocals; lv != NULL; lv = nlv) {
	nlv = lv->Next;
	if (((lv->flags & lvf_Deleted) != 0) ||
	    (lv != rp->PublishedP && lv != rp->BuildingP && lv != rp->JunkP)) {
	    /* deleted or not referenced; eliminate it */
	    if (llv == NULL) {
		rp->AllLocals = lv->Next;
	    } else {
		llv->Next = lv->Next;
	    }
	    if (lv == rp->PublishedP)
		rp->PublishedP = NULL;
	    if (lv == rp->BuildingP)
		rp->BuildingP = NULL;
	    if (lv == rp->JunkP)
		rp->JunkP = NULL;
	    if ((lv->flags & lvf_Deleted) == 0) (void) DestroyVolume(rp, lv, 0);
	    osi_Free((opaque) lv, sizeof(struct localvol));
	} else {	/* referenced; remember pointer source */
	    llv = lv;
	}
    }

    icl_Trace0(rep_iclSetp, REP_WANTSADV_JDZ);

    /* Only now expect the rp-> pointers to contain volumes that are present */
    if (rp->JunkP != NULL) {
	if (EliminateJunkVolume(rp) != 0) {
	    return (setNextTime(rp, rp->localDiskWait, NULL, 0));
	}
    }

    icl_Trace0(rep_iclSetp, REP_WANTSADV_ELIMJUNK);

    P = rp->PublishedP;			/* (A) */

    icl_Trace1(rep_iclSetp, REP_WANTSADV_A,
	       ICL_TYPE_POINTER, P);

    if (P != NULL) {
	if ((P->flags & lvf_GotStatus) == 0) {
	    RC = GetLVStatus(rp, P, NULL);
	    if (RC != 0) return (setNextTime(rp, rp->localDiskWait, NULL, 0));
	}
	if (((P->flags & lvf_GotStatus) != 0) && (P->itsStates & VOL_IS_COMPLETE) == 0) {
	    if ((P->flags & lvf_Deleted) == 0) (void) DestroyVolume(rp, P, 0);
	    rp->PublishedP = NULL;
	    P = NULL;
	}
    }
    B = rp->BuildingP;
    if (B != NULL) {
	if ((B->flags & lvf_GotStatus) == 0) {
	    RC = GetLVStatus(rp, B, NULL);
	    if (RC != 0) return (setNextTime(rp, rp->localDiskWait, NULL, 0));
	}

	if (((B->flags & lvf_GotStatus) != 0) && (B->itsStates & VOL_IS_COMPLETE) == 0) { /* (B) */

	    icl_Trace1(rep_iclSetp, REP_WANTSADV_B,
		       ICL_TYPE_POINTER, B);

	    if ((B->flags & lvf_Deleted) == 0) (void) DestroyVolume(rp, B, 0);
	    rp->BuildingP = NULL;
	    B = NULL;
	}
	if (B != NULL && (B->flags & lvf_GotStatus) != 0 && (B->itsStates & VOL_IS_COMPLETE) != 0) {	/* (F) (M) */

	    P = rp->PublishedP;

	    icl_Trace1(rep_iclSetp, REP_WANTSADV_FM,
		       ICL_TYPE_POINTER, P);

	    if (P != NULL) {
		if (CompleteAndSwitch(rp) == 0) {	/* move pointers to the good stuff */
		    if (EliminateJunkVolume(rp) == 0) {
			/* OK */
			rp->Flags &= ~repflag_UserWantsUpdate;
			setNextTime(rp, 10*60*60, "ReplicaWantsAdvance: all OK at %lu", osi_Time());
		    } else {
			/* XXX what to do?  Leave state so PromoteFiles can be tried again? */
			tempTime = setNextTime(rp, rp->localDiskWait, NULL, 0);
			if (tempTime < nextTime) nextTime = tempTime;
		    }
		} else {
		    /* XXX what to do?  Leave state so PromoteFiles can be tried again? */
		    tempTime = setNextTime(rp, rp->localDiskWait, NULL, 0);
		    if (tempTime < nextTime) nextTime = tempTime;
		}
	    } else {	/* wasn't an old volume */
		if (CompleteNewVol(rp, B) == 0) {
		    rp->PublishedP = B;
		    rp->BuildingP = NULL;
		    code = UpdateLocal(rp, &rp->PublishedP->volId, rp->PublishedP,
				       rp->PublishedP->aggr);
		    if (code != 0) {
			tempTime = setNextTime(rp, rp->localDiskWait, NULL, 0);
			if (tempTime < nextTime) nextTime = tempTime;
		    }
		} else {
		    /* XXX what to do? */
		    tempTime = setNextTime(rp, rp->localDiskWait, NULL, 0);
		    if (tempTime < nextTime) nextTime = tempTime;
		}
	    }
	}
    }
    osi_GetTime(&Now);		    /* (I) (J) */

    icl_Trace3(rep_iclSetp, REP_WANTSADV_IJ,
	       ICL_TYPE_HYPER, &rp->WVT_TypesLeft,
	       ICL_TYPE_LONG, Now.tv_sec,
	       ICL_TYPE_LONG, rp->nextLookTime.tv_sec);

    if (!HAVE_MY_WVT(AFS_hgetlo(rp->WVT_TypesLeft))) {
	if (!AFS_hiszero(rp->WVT_TypesLeft)) {
	    tempTime = LoseAllWVTRead(rp, &Now);
	    if (tempTime < nextTime) nextTime = tempTime;
	}
	tempTime = TryForWVTRead(rp);
	if (tempTime < nextTime) nextTime = tempTime;
	if (!HAVE_MY_WVT(AFS_hgetlo(rp->WVT_TypesLeft)))
	    return nextTime;
    }
    if (HAVE_MY_WVT(AFS_hgetlo(rp->WVT_TypesLeft))
	 && (rp->nextLookTime.tv_sec == 0
	     || rp->nextLookTime.tv_sec <= Now.tv_sec)) { /* OK, start. */

	P = rp->PublishedP;
	B = rp->BuildingP;

	icl_Trace2(rep_iclSetp, REP_WANTSADV_HAVE_WVT_OK_START,
		   ICL_TYPE_POINTER, P,
		   ICL_TYPE_POINTER, B);

	if (rp->hostP != NULL && P != NULL) {
	    int doIt = 0;
	
	    pthread_mutex_lock(&rp->hostP->rh_fsMutex);
	    if (rp->hostP->rh_fsLastSuccess != P->lastCallTime)
		doIt = 1;
	    pthread_mutex_unlock(&rp->hostP->rh_fsMutex);
	    if (doIt)
		UpdateLocal(rp, &P->volId, P, P->aggr);
	}
	if (B == NULL
	    && (P == NULL || AFS_hiszero(P->volVersion)
		|| (!AFS_hiszero(rp->VVOnSource) &&
		    !AFS_hsame(P->volVersion, rp->VVOnSource)))) {
	    tempTime = StartVolumeRetrieval(rp);
	    if (tempTime < nextTime) nextTime = tempTime;
	} else if (P != NULL && AFS_hiszero(rp->VVOnSource)) {
	    *needAdvP = TRUE;	/* trigger (G) */

	    tempTime = setNextTime(rp, 0, "ReplicaWantsAdvance: Trying to get VV from primary", 0);
	    if (tempTime < nextTime) nextTime = tempTime;

	    icl_Trace1(rep_iclSetp, REP_WANTSADV_G,
		       ICL_TYPE_LONG, nextTime);

	    return nextTime;
	} else if (B == NULL && P != NULL && AFS_hsame(P->volVersion, rp->VVOnSource)) {
	    /* regained token for this version--set local exporter state again */
	    code = UpdateLocal(rp, &P->volId, P, P->aggr);
	    if (code != 0) {
		tempTime = setNextTime(rp, rp->localDiskWait, NULL, 0);
		if (tempTime < nextTime) nextTime = tempTime;
	    }
	}
    } else {

	icl_Trace2(rep_iclSetp, REP_WANTSADV_MAYBE_NO_WVT,
		   ICL_TYPE_LONG, rp->nextLookTime.tv_sec,
		   ICL_TYPE_LONG, nextTime);

	if (!HAVE_MY_WVT(AFS_hgetlo(rp->WVT_TypesLeft))) {
	    if (rp->nextLookTime.tv_sec != 0 &&
		rp->nextLookTime.tv_sec < nextTime)
		nextTime = rp->nextLookTime.tv_sec;
	}
    }

    icl_Trace1(rep_iclSetp, REP_WANTSADV_BOTTOM,
	       ICL_TYPE_LONG, nextTime);

    return nextTime;
}

static u_int32 StartImporting(rp)
    struct replica	*rp;
{
    long			code;
    rep_host_conn_t		*connP;
    struct afsBulkVolIDs	IDs;
    time_t			minNextTime, tempTime;
    struct timeval		Now;
    long			resetCode;
    int				retryCount = 0;
    struct afsBulkVVs		VVs;
    struct afsVolSync		*vsP;
    int				wantAdv, del;
    rep_host_t			*hostP;
    afs_hyper_t			volIDOfSource;

    minNextTime = osi_Time() + 24*60*60;

    icl_Trace3(rep_iclSetp, REP_STARTIMPORT_BEG,
	       ICL_TYPE_POINTER, rp,
	       ICL_TYPE_LONG, rp->nextLookTime.tv_sec,
	       ICL_TYPE_LONG, rp->Flags);

    if ((rp->Flags & repflag_KillMe) != 0)
	return minNextTime;

    tempTime = ReplicaWantsAdvance(rp, &wantAdv, &del);
    if (del != 0) 
	return minNextTime;	      /* volume was deleted after VLDB check */

    minNextTime = MIN(minNextTime, tempTime);

    if (wantAdv == 0)
	return minNextTime;

    osi_GetTime(&Now);
    tempTime = Now.tv_sec + 10*60*60;
    minNextTime = MIN(minNextTime, tempTime);

    if ((connP = GET_FS_CONN(rp, &hostP, "StartImporting-1")) == NULL) {
	tempTime = setNextTime(rp, rp->remoteBusyWait, NULL, 0);
	return MIN(minNextTime, tempTime);
    }

    osi_assert(rp->hostP != NULL);

    if ((rp->Flags & repflag_ReleaseOnly) == 0)
	volIDOfSource = rp->v.VolIDOfPrimary;
    else
	volIDOfSource = rp->v.VolIDOfSecondary;

    IDs.afsBulkVolIDs_val[0] = volIDOfSource;
    IDs.afsBulkVolIDs_len = 1;
    bzero((char *)&VVs, sizeof VVs);

    do {
	u_int32	junk;

	lock_ReleaseWrite(&(rp->lockObject));
	
	osi_GetTime(&Now);
	code = AFS_BulkFetchVV(connP->rhc_handle, &rp->CellId, &IDs,
			       1, 0, 0, 0, &VVs, &junk);

	lock_ObtainWrite(&(rp->lockObject));
	
	code = osi_errDecode(code);
	if (code != 0) {
	    icl_Trace1(rep_iclSetp, REP_BFETCH_VV, ICL_TYPE_LONG, code);
	    
	    resetCode = ResetConn(rp, hostP, &connP, code, "StartImporting-2");
	    if (resetCode == REP_ERR_RESETFAILED) {
		if (connP != NULL)
		    reph_PutConn(connP);
		return rp->nextLookTime.tv_sec;
	    }
	}
    } while (code != 0 && resetCode == 0 && ++retryCount < 3);
    
    reph_PutConn(connP);

    if (code != 0) {
	tempTime = setNextTime(rp, rp->remoteBusyWait,
			       "StartImporting: Cannot get VV: %s",
			       dfs_dceErrTxt(code));
	return MIN(minNextTime, tempTime);
    }

    MergeSuccess(rp, hostP, Now.tv_sec);

    vsP = &VVs.afsBulkVVs_val[0];

    if (VVs.afsBulkVVs_len == 1 && !AFS_hiszero(vsP->VV)
	&& AFS_hsame(vsP->VolID, volIDOfSource)) {
	SetLatestCurrentTime(rp, NULL, NULL);
	rp->VVOnSource = vsP->VV;
	rp->TimeOfLastPing = Now;
	strcpy(rp->RepMsg, "StartImporting: Got VV ok");
    } else {
	strcpy(rp->RepMsg, "StartImporting: Unknown VV on primary");
	DebugPrint(rp);
    }

    return minNextTime;
}	/* StartImporting() */

static u_int32 RenewTokens(struct replica *rp)
{
    long		code;
    rep_host_conn_t*	connP;
    time_t		hostLifeDeadline;
    time_t		hostLifeRefreshTime;
    rep_host_t*		hostP;
    time_t		minNextTime, tempTime;
    struct timeval	Now;
    long		resetCode;

    osi_GetTime(&Now);
    minNextTime = Now.tv_sec + 24*60*60;

    /* Ignore this replica if it is marked for deletion */
    if ((rp->Flags & repflag_KillMe) != 0)
	return minNextTime;

    /* Ignore this replica if we don't have a token to renew */
    if (!HAVE_MY_WVT(AFS_hgetlo(rp->WVT_TypesLeft)))
	return minNextTime;

    osi_GetTime(&Now);
    if (rp->TimeOfWVTExpiration.tv_sec <= (Now.tv_sec+1)) {
	/* Token has expired */
	LoseWVT(rp, &Now, &allTokenTypes, 0);
	tempTime = rp->nextLookTime.tv_sec;
	return MIN(minNextTime, tempTime);
    }
    
    /* Token has not expired yet */
    tempTime = rp->TimeOfWVTExpiration.tv_sec - rp->tokenLeadTime;
    minNextTime = MIN(minNextTime, tempTime);

    if ((connP = GET_FS_CONN(rp, &hostP, "RenewTokens-1")) == NULL) {
	osi_GetTime(&Now);
	tempTime = setTokenTime(rp, rp->remoteBusyWait, &Now, NULL, 0);
	return MIN(minNextTime, tempTime);
    }

    pthread_mutex_lock(&hostP->rh_fsMutex);
    hostLifeDeadline = hostP->rh_fsLastSuccess
	+ hostP->rh_fsHostLifeGuarantee;
    hostLifeRefreshTime = hostLifeDeadline - hostP->rh_fsHostLeadTime;
    pthread_mutex_unlock(&hostP->rh_fsMutex);

    osi_GetTime(&Now);
    if (hostLifeDeadline <= Now.tv_sec) {
	/* host expired: token might have expired */
	LoseWVT(rp, &Now, &allTokenTypes, 0);
	tempTime = rp->nextLookTime.tv_sec;
	minNextTime = MIN(minNextTime, tempTime);
    } else if (hostLifeRefreshTime <= Now.tv_sec) {
	int	retryCount = 0;
	u_long	secs, usecs, dist, disper;
	time_t	successTime;

	/*
	 * We're within the host refresh lead time, so make an AFS_GetTime
	 * call to refresh host
	 */

	icl_Trace1(rep_iclSetp, REP_RFRSH_LASTCALL,
		   ICL_TYPE_HYPER, &rp->v.VolIDOfSecondary);

	do {
	    lock_ReleaseWrite(&(rp->lockObject));

	    successTime = osi_Time();
	    code = AFS_GetTime(connP->rhc_handle,
			       &secs, &usecs, &dist, &disper);

	    lock_ObtainWrite(&(rp->lockObject));

	    code = osi_errDecode(code);
	    if (code != 0) {
		icl_Trace1(rep_iclSetp, REP_GETTIME, ICL_TYPE_LONG, code);
		
		resetCode = ResetConn(rp, hostP, &connP, code,
				      "RenewTokens-2");
		if (resetCode == REP_ERR_RESETFAILED) {
		    if (connP != NULL)
			reph_PutConn(connP);
		    return rp->nextLookTime.tv_sec;
		}
	    }
	} while (code != 0 && resetCode == 0 && ++retryCount < 3);

	if (code != 0) {
	    reph_PutConn(connP);
	    tempTime = setNextTime(rp, rp->remoteBusyWait,
				   "RenewTokens/AFS_GetTime: %s",
				   dfs_dceErrTxt(code));
	    return MIN(minNextTime, tempTime);
	}

	MergeSuccess(rp, hostP, successTime);
	pthread_mutex_lock(&hostP->rh_fsMutex);
	tempTime = hostP->rh_fsLastSuccess +
	    hostP->rh_fsHostLifeGuarantee - hostP->rh_fsHostLeadTime;
	pthread_mutex_unlock(&hostP->rh_fsMutex);
	minNextTime = MIN(minNextTime, tempTime);

	/* Take care of at least the first guy! */
	if (rp->PublishedP)
	    UpdateLocal(rp, &rp->PublishedP->volId,
			rp->PublishedP, rp->PublishedP->aggr);
    } else {
	/*
	 * Just wait for the time-for-a-call to come around
	 * tempTime already set correctly
	 */
	minNextTime = MIN(minNextTime, hostLifeRefreshTime);
    }

    reph_PutConn(connP);

    /* Check whether it's time to renew token itself */
    tempTime = rp->TimeOfWVTExpiration.tv_sec - rp->tokenLeadTime;
    if (tempTime <= Now.tv_sec)
	tempTime = TryForWVTRead(rp);

    return MIN(minNextTime, tempTime);
}	/* RenewTokens() */

long CheckVLDBRelationship(cellIdentP, volIdentP, flags)
afs_hyper_t *cellIdentP;
afs_hyper_t *volIdentP;
u_long flags;
{/* Check whether we're doing the right thing by this volume.
    flags might include REP_FLAG_QUICKDESTROY or REP_FLAG_AWAIT . */
/* Return 0 for all-OK else an errno value or a DCE error value. */
    struct vldbRepInfo VInfo;
    struct replica *rp, *lastrp;
    int32 ret, Code;
    int32 MySiteIx, ShouldExistHere;
    char *oldR, *newR;
    static char rel[] = "release";
    static char lazy[] = "lazy";
    struct timeval Now;

    ShouldExistHere = 0;	/* 0 unk, <0 no, >0 yes */
    VInfo.Reps = NULL;
    ret = VLDBLookup(cellIdentP, volIdentP, &VInfo, &MySiteIx, &Code);
    if (ret < 0) {
	return (Code ? Code : ret);	/* cannot tell just now */
    } else if (ret > 0) {
	ShouldExistHere = -1;	/* volume doesn't exist */
	VInfo.VolIDOfSecondary = *volIdentP;	/* for search below */
    } else {	/* (ret == 0) volume exists, data all present */
	if (MySiteIx >= 0) ShouldExistHere = 1; /* our site is in the list of secs */
	else ShouldExistHere = -1;	/* our site is NOT in the list of secs */
    }
    lock_ObtainWrite(&RepTableLock);
    lastrp = NULL;
    for (rp = RepRoot; rp != NULL; rp = rp->Next) {
	if (AFS_hsame(rp->v.VolIDOfSecondary, VInfo.VolIDOfSecondary)) {
	    RepHold(rp);
	    lock_ObtainWrite(&rp->lockObject);
	    break;
	}
	lastrp = rp;
    }

    Code = 0;
    if (ShouldExistHere < 0 && rp != NULL) {
	/* need to delete it */
	rp->Flags |= repflag_KillMe;
	repq_ResetDeadline(&RepQ, rp->qCookie, 0);
	strcpy(rp->RepMsg, "CheckVLDBRelationship: Replica is made moribund.");
	LogPrint(rp);
	lock_ReleaseWrite(&rp->lockObject);
	RepRele(rp);
	rp = NULL;
    } else if (ShouldExistHere > 0) {
	lastrp = rp;
	if (rp != NULL) {
	    Code = 0;	/* flag any change */
	    if ((rp->Flags & repflag_ReleaseOnly) != 0) {
		if ((VInfo.vlflags & VLF_LAZYRELE) == 0) {
		    Code = 1;
		    oldR = rel;
		    newR = lazy;
		}
	    } else {
		if ((VInfo.vlflags & VLF_LAZYRELE) != 0) {
		    Code = 1;
		    oldR = lazy;
		    newR = rel;
		}
	    }
	    if (Code) {
		Log("CheckVLDBRelationship: %lu,,%lu CHANGED from %s-style repl to %s-style",
		    AFS_HGETBOTH(rp->v.VolIDOfSecondary), oldR, newR);
	    }
	    if (rp->v.Reps != NULL) {	/* get a fresh copy */
		osi_Free((opaque) rp->v.Reps, rp->v.MaxReps * sizeof(struct RepSite));
		rp->v.Reps = NULL;
	    }
	    if (VInfo.NumPrimaryAddrs != rp->v.NumPrimaryAddrs ||
		! SameAddresses(VInfo.PrimaryAddrs, rp->v.PrimaryAddrs,
				VInfo.NumPrimaryAddrs) ||
		strcmp(VInfo.hName, rp->v.hName) != 0)
	    {
		/* The address information for the primary has changed */
		Log("CheckVLDBRelationship: %lu,,%lu: Primary address info has changed",
		    AFS_HGETBOTH(rp->v.VolIDOfSecondary));
		osi_GetTime(&Now);
		LoseWVT(rp, &Now, &allTokenTypes, 0);
		if (rp->hostP != NULL) {
		    /* Lose the out of date host info.  The next call to
		       GetConn() will reestablish it. */
		    reph_PutHost(rp->hostP);
		    rp->hostP = NULL;
		}
	    }	
	} else {
	    /* need to make it */
	    rp = NewReplicaStruct();
	    if (rp == NULL) {
		lock_ReleaseWrite(&RepTableLock);
		if (VInfo.Reps != NULL)
		    osi_Free((opaque) VInfo.Reps, VInfo.MaxReps * sizeof(struct RepSite));
		return REP_ERR_NOSTORAGE;
	    }
	    Code = 0;
	}
	Code = VLDBCopy(&VInfo, rp, MySiteIx, rp->RepMsg, Code);
	if (VInfo.Reps != NULL) {
	    osi_Free((opaque) VInfo.Reps, VInfo.MaxReps * sizeof(struct RepSite));
	    VInfo.Reps = NULL;
	}
	if (Code != 0) {
	    if (lastrp != NULL) {
		/* We had found it in the table, so unlock and release it */
		lock_ReleaseWrite(&rp->lockObject);
		RepRele(rp);
	    }
	    lock_ReleaseWrite(&RepTableLock);
	    LogPrint(rp);
	    if (Code == ENOMEM) Code = REP_ERR_NOSTORAGE;
	    return Code;
	}
	if (lastrp == NULL) {
	    /*
	     * We hadn't found it in the table, so enter it in the dispatch
	     * queue and the rep table.
	     */
	    rp->RefCount = 3;		/* 1 for RepQ, 1 for RepRoot, */
					/* and 1 for this reference */
	    /*
	     * Obtain the object lock to hold up any worker threads, because
	     * they can get their hands on this replica as soon as it is 
	     * queued.
	     */
	    lock_ObtainWrite(&rp->lockObject);
	    rp->qCookie = repq_Enter(&RepQ, rp, 0);
	    rp->Next = RepRoot;
	    RepRoot = rp;
	    ++numReplicas;
	} else {
	    repq_ResetDeadline(&RepQ, rp->qCookie, 0);
	}
    }
    if (rp != NULL) {
	lock_ReleaseWrite(&rp->lockObject);
	RepRele(rp);
    }
    lock_ReleaseWrite(&RepTableLock);
    if (VInfo.Reps != NULL) osi_Free((opaque) VInfo.Reps, VInfo.MaxReps * sizeof(struct RepSite));
    return Code;
}

static void recordCaller(rpcCallp, callName)
handle_t rpcCallp;
char *callName;
{
    unsigned32 authnLevel, authnSvc, authzSvc, st;
    rpc_authz_cred_handle_t privs;
    unsigned char *serverPrincP;

    serverPrincP = NULL;
    rpc_binding_inq_auth_caller(rpcCallp, &privs, &serverPrincP,
				&authnLevel, &authnSvc, &authzSvc, &st);
    if (st) {
	icl_Trace2(rep_iclSetp, REP_TRACE_RECORDCALLER_FAIL,
		   ICL_TYPE_STRING, callName,
		   ICL_TYPE_LONG, st);
    } else {
	icl_Trace4(rep_iclSetp, REP_TRACE_RECORDCALLER_OK,
		   ICL_TYPE_STRING, callName,
		   ICL_TYPE_STRING, (serverPrincP ? serverPrincP :
				     (u_char*)"*NONE*"),
		   ICL_TYPE_LONG, authnSvc,
		   ICL_TYPE_LONG, authnLevel);
	if (serverPrincP)
	    rpc_string_free(&serverPrincP, &st);
    }
}

static u_long SREP_Probe(rpcCallp)
handle_t rpcCallp;
{
    recordCaller(rpcCallp, "Probe");
    icl_Trace1(rep_iclSetp, REP_SREP_PROBE, 
		ICL_TYPE_STRING, WhoIAm);
    return 0;
}

/*------------------------------------------------------------------------
 * REP_CheckReplicationConfig
 *
 * Description:
 *	Verify that this site's relationship with a volume's replication parameters is correct.
 *
 * Arguments:
 *	cellIdent: cell identifier.
 *	volID: identifier for the volume being checked.
 *	flags: any flags for the operation.
 *
 * Returns:
 *	REP_ERR_SUCCESS: everything went well.
 *	REP_ERR_NOTREPLICA: the volume is not a replica; service it as normal.
 *	REP_ERR_UNKNOWNREPLICA: the volume is not a recognizable kind of replica.
 *
 * Environment:
 *	Not yet specified.
 *
 * Locks:
 *	Not yet specified.
 *
 * Side Effects:
 *	Not yet specified.
 *------------------------------------------------------------------------*/
static u_long SREP_CheckReplicationConfig(rpcCallp, cellIdP, volIdP, flags,
					 spare1, spare2, spare3, spare4, spare5)
handle_t rpcCallp;
afs_hyper_t *cellIdP, *volIdP;
u_long flags;
u_long spare1, spare2, spare3;
u_long *spare4, *spare5;
{/* Check replication configuration for the given volume. */
    long ret;
    int cancelState;
    struct replica *rp;

    cancelState = pthread_setcancel(CANCEL_OFF);
    recordCaller(rpcCallp, "CheckReplicationConfig");
    if (cancelState == -1) return COMPAT_ERROR_DISABLE_CANCEL;
    icl_Trace1(rep_iclSetp, REP_CHECK_REP, 
	       ICL_TYPE_STRING, WhoIAm);

    /* unset KnowVLDB flag if replica is extant; CheckVLDBRelationship() will
     * reset the flag if replica state is successfully updated.
     */

    lock_ObtainRead(&RepTableLock);

    for (rp = RepRoot; rp != NULL; rp = rp->Next) {
	if (AFS_hsame(rp->v.VolIDOfSecondary, *volIdP)) {
	    lock_ObtainWrite(&rp->lockObject);
	    rp->Flags &= ~repflag_KnowVLDB;
	    lock_ReleaseWrite(&rp->lockObject);
	    break;
	}
    }

    lock_ReleaseRead(&RepTableLock);

    ret = CheckVLDBRelationship(cellIdP, volIdP, flags);
    (void) pthread_setcancel(cancelState);
    return osi_errEncode(ret);
}

/*------------------------------------------------------------------------
 * REP_AllCheckReplicationConfig
 *
 * Description:
 *	Verify that everybody's relationship with the volume's replication parameters is correct.
 *
 * Arguments:
 *	cellIdent: cell identifier.
 *	volID: identifier for the volume being checked.
 *	flags: any flags for the operation.  REP_FLAG_QUICKDESTROY causes any resulting vol-destroy
		operations not to attempt to obtain a whole-volume TKM_OPEN_DELETE token first.
 *	TheseGuysToo: a list of other servers to contact, in addition to any listed in the VLDB.
 *
 * Returns:
 *	REP_ERR_SUCCESS: everything went well.
 *	REP_ERR_NOTREPLICA: the volume is not a replica.
 *	REP_ERR_UNKNOWNREPLICA: the volume is not a recognizable kind of replica.
 *	REP_ERR_NOSTORAGE: some problem; can't store the set of calls that must be made.
 *
 * Environment:
 *	Not yet specified.
 *
 * Locks:
 *	Not yet specified.
 *
 * Side Effects:
 *	Not yet specified.
 *------------------------------------------------------------------------*/
static u_long SREP_AllCheckReplicationConfig(rpcCallp, cellIdP, volIdP, howManyGuys, TheseGuysToo, flags,
					    spare1, spare2, spare3, spare4, spare5)
handle_t rpcCallp;
afs_hyper_t *cellIdP, *volIdP;
u_long howManyGuys;
afsNetAddrs *TheseGuysToo;
u_long flags;
u_long spare1, spare2, spare3;
u_long *spare4, *spare5;
{/* Promise to get everybody to check the replication configuration for the given volume. */
/* Keep the initial time of call, the volume descriptor, and the set of other guys. */
    u_long ret;
    int cancelState;

    cancelState = pthread_setcancel(CANCEL_OFF);
    recordCaller(rpcCallp, "AllCheckReplicationConfig");
    if (cancelState == -1) return COMPAT_ERROR_DISABLE_CANCEL;
    icl_Trace1(rep_iclSetp, REP_ALLCHECK_REP, 
		ICL_TYPE_STRING, WhoIAm);
    ret = handleWillCall(cellIdP, volIdP, howManyGuys, TheseGuysToo, flags);
    (void) pthread_setcancel(cancelState);
    return osi_errEncode(ret);
}

/*------------------------------------------------------------------------
 * REP_GetServerInterfaces
 *
 * Description:
 *	Return interfaces that this server exports.
 *
 *------------------------------------------------------------------------*/
static u_long SREP_GetServerInterfaces(rpcCallp, serverInterfacesP)
handle_t rpcCallp;
dfs_interfaceList *serverInterfacesP;
{
    error_status_t st;
    int cancelState;

    cancelState = pthread_setcancel(CANCEL_OFF);
    recordCaller(rpcCallp, "GetServerInterfaces");
    if (cancelState == -1) return COMPAT_ERROR_DISABLE_CANCEL;
    icl_Trace1(rep_iclSetp, REP_GETSERVER_INT,
		ICL_TYPE_STRING, WhoIAm);
    dfs_GetServerInterfaces(REP_v4_0_s_ifspec, serverInterfacesP, &st);
    (void) pthread_setcancel(cancelState);
    return osi_errEncode(st);
}

struct RepKA {struct replica *rep; struct afsNetAddr primhost;};

static u_int32 BundleKeepAlives(KAs)
struct RepKA *KAs;
{/* all ThisPass ones are write-locked; all others, shared-locked.  Table is also locked. */
    /* Send keep-alive message for all files on the given server. */
    register int Ix, XIx;
    int ThisCount, Executing, TotThisPass, PassB, PassE;
    register struct replica *rp, *xrp;
    struct replica *anyOneRP;
    register afsFidExp *FEXBeg, *FEXEnd;
    u_int32 x, nextTime, tempTime, minRemoteBusy, timeout, realDally;
    struct afsBulkFEX bulkfex;
    struct timeval Now;
    long code;
    int retryOK, retryCount;
    error_status_t st;
    rep_host_conn_t *connP;
    unsigned_char_t *strBinding;
    rep_host_t *hostP;

    osi_GetTime(&Now);
    nextTime = Now.tv_sec + 24*60*60;
    minRemoteBusy = 24*60*60;
/* First, the non-executing FIDs */
    TotThisPass = 0;
    for (rp = RepRoot; rp != NULL; rp = rp->Next) {
	if ((rp->Flags & repflag_KAThisPass) != 0) {	/* add this replica's ones */
	    TotThisPass += rp->NumKAs;
	}
    }
    if (TotThisPass <= 0) return nextTime;
    icl_Trace1(rep_iclSetp, REP_BKEEP_ALIVE, 
		 ICL_TYPE_LONG, TotThisPass);
    rp = RepRoot;	/* Funky interleaved loops! */
    Ix = 0;		/* The RepRoot/NumKAs loop intertwines with PassE/TotThisPass. */
    for (PassE = 0; PassE < TotThisPass; ) {
	PassB = PassE;
	ThisCount = TotThisPass - PassE;
	if (ThisCount > AFS_BULKMAX) ThisCount = AFS_BULKMAX;
	PassE += ThisCount;
	xrp = rp;
	XIx = Ix;
	FEXBeg = &bulkfex.afsBulkFEX_val[0];
	FEXEnd = &(FEXBeg[ThisCount]);
	Executing = 0;
	x = PassB;		/* counter for the buffer. */
	/* ``rp'' was initialized above... */
	for (; rp != NULL; rp = rp->Next, Ix = 0) {
	    if ((rp->Flags & repflag_KAThisPass) != 0) {	/* add this replica's ones */
		anyOneRP = rp;
		if (rp->remoteBusyWait < minRemoteBusy)
		    minRemoteBusy = rp->remoteBusyWait;
		/* ``Ix'' was initialized outside the big loop and at the end of each ``rp'' pass... */
		for (; Ix < rp->NumKAs && x < PassE; ++Ix, ++x) {
		    if ((rp->KA[Ix].Flags & KA_EXECUTING) != 0) {
			FEXBeg->fid.Cell = rp->CellId;
			FEXBeg->fid.Volume = rp->v.VolIDOfPrimary;
			FEXBeg->fid.Vnode = rp->KA[Ix].VnodeIx;
			FEXBeg->fid.Unique = rp->KA[Ix].Uniq;
			FEXBeg->keepAliveTime = rp->v.reclaimDally;
			icl_Trace3(rep_iclSetp, REP_BKEEP_ADDING,
				   ICL_TYPE_FID, &FEXBeg->fid,
				   ICL_TYPE_LONG, FEXBeg->keepAliveTime,
				   ICL_TYPE_LONG, 1);
			++FEXBeg;
			++Executing;
		    } else {
			--FEXEnd;
			FEXEnd->fid.Cell = rp->CellId;
			FEXEnd->fid.Volume = rp->v.VolIDOfPrimary;
			FEXEnd->fid.Vnode = rp->KA[Ix].VnodeIx;
			FEXEnd->fid.Unique = rp->KA[Ix].Uniq;
			FEXEnd->keepAliveTime = rp->v.reclaimDally;
			icl_Trace3(rep_iclSetp, REP_BKEEP_ADDING,
				   ICL_TYPE_FID, &FEXEnd->fid,
				   ICL_TYPE_LONG, FEXEnd->keepAliveTime,
				   ICL_TYPE_LONG, 0);
		    }
		}
		if (x >= PassE) break;
	    }
	}
	if (FEXBeg != FEXEnd) abort();
	bulkfex.afsBulkFEX_len = ThisCount;
	connP = GET_FS_CONN(anyOneRP, &hostP, "BundleKeepAlives-1");
	if (connP == NULL) {
	    tempTime = Now.tv_sec + minRemoteBusy;
	    if (tempTime < nextTime) nextTime = tempTime;
	    return nextTime;
	}
	rpc_binding_to_string_binding(connP->rhc_handle, &strBinding, &st);
	if (st != error_status_ok)
	    strBinding = (unsigned_char_t*)dfs_dceErrTxt(code);
	icl_Trace4(rep_iclSetp, REP_BKEEP_ALIVE_CALL, 
		   ICL_TYPE_HYPER, &anyOneRP->v.VolIDOfSecondary,
		   ICL_TYPE_STRING, strBinding,
		   ICL_TYPE_LONG, ThisCount, 
		   ICL_TYPE_LONG, Executing);
	if (st == error_status_ok)
	    rpc_string_free(&strBinding, &st);
	osi_GetTime(&Now);
	retryCount = 0;
	for (retryOK = 1; retryOK;) {
	    retryOK = 0;
	    ++retryCount;
	    /* &&& should unlock over the RPC call */
	    st = (unsigned long) osi_Time();
	    code = AFS_BulkKeepAlive(connP->rhc_handle,
				     &bulkfex, Executing, 0, 0, 0, &x);
	    code = osi_errDecode(code);
	    icl_Trace1(rep_iclSetp, REP_BKEEP_ALIVE_RET,
		       ICL_TYPE_LONG, code);
	    if (code != 0 && retryCount < 3) {
		code = ResetConn(anyOneRP, hostP, &connP, code,
				     "BundleKeepAlives-2");
		if (code == REP_ERR_RESETFAILED) {
		    if (connP != NULL)
			reph_PutConn(connP);
		    return rp->nextLookTime.tv_sec;
		} else if (code == 0)
		    retryOK = 1;
	    }
	}
	reph_PutConn(connP);
	if (code == 0) {
	    MergeSuccess(anyOneRP, hostP, st);
	    x = PassB;
	    for (; xrp != NULL; xrp = xrp->Next, XIx = 0) {
		if ((xrp->Flags & repflag_KAThisPass) != 0) {	/* add this replica's ones */
		    for (; XIx < xrp->NumKAs && x < PassE; ++XIx, ++x) {
			xrp->KA[XIx].SentWhen = Now.tv_sec;
			xrp->KA[XIx].Flags &= ~(KA_SOON);
			icl_Trace4(rep_iclSetp, REP_BKEEP_SENTTRACK,
				   ICL_TYPE_HYPER, &xrp->v.VolIDOfSecondary,
				   ICL_TYPE_LONG, xrp->KA[XIx].VnodeIx,
				   ICL_TYPE_LONG, xrp->KA[XIx].Uniq,
				   ICL_TYPE_LONG, xrp->KA[XIx].Flags);
		    }
		    if (x >= PassE) break;
		}
	    }
	}
    }
    for (rp = RepRoot; rp != NULL; rp = rp->Next) {
	if ((rp->Flags & repflag_KAThisPass) != 0) {
	    timeout = Now.tv_sec - rp->v.reclaimDally;
	    realDally = rp->v.reclaimDally - rp->kaLeadTime;
	    for (Ix = 0; Ix < rp->NumKAs; ++Ix) {
		if (rp->KA[Ix].RecvWhen >= timeout) {	/* not expired yet */
		    /* compute when this will start getting late */
		    tempTime = rp->KA[Ix].SentWhen + realDally;
		    if (tempTime <= Now.tv_sec) { /* already getting late for this one */
			tempTime = Now.tv_sec + rp->remoteBusyWait;
			icl_Trace4(rep_iclSetp, REP_BKEEP_RECALL,
				   ICL_TYPE_HYPER, &rp->v.VolIDOfSecondary,
				   ICL_TYPE_LONG, rp->KA[Ix].VnodeIx,
				   ICL_TYPE_LONG, rp->KA[Ix].Uniq,
				   ICL_TYPE_LONG, tempTime);
			icl_Trace3(rep_iclSetp, REP_BKEEP_RECALL_2,
				   ICL_TYPE_LONG, rp->KA[Ix].RecvWhen,
				   ICL_TYPE_LONG, rp->v.reclaimDally,
				   ICL_TYPE_LONG, rp->KA[Ix].SentWhen);
		    }
		    if (nextTime > tempTime) nextTime = tempTime;
		}
	    }
	}
    }
    return nextTime;
}

static int PruneAndCheckKA(rp, Secs, BackLimit)
struct replica *rp;
u_int32 Secs;
{/* Prune unreferenced entries from a KA set for a replica. */
 /* Also note if any referenced entry needs to have something sent. */
    struct FileKA *Src, *Dest;
    int NewCount, Ix, AnyNeed;
    u_int32 Timeout, oldNumKAs;

    AnyNeed = 0;
    Timeout = Secs - rp->v.reclaimDally;
    Src = rp->KA;
    Dest = Src;
    NewCount = 0;
    oldNumKAs = rp->NumKAs;
    for (Ix = 0; Ix < oldNumKAs; ++Ix) {
	if (Src->RecvWhen >= Timeout) {
	    if (Src->SentWhen == 0 || Src->SentWhen <= BackLimit || (Src->Flags & KA_SOON) != 0)
		AnyNeed = 1;
	    if (Src != Dest) *Dest = *Src;
	    ++Dest;
	    ++NewCount;
	}
	++Src;
    }
    rp->NumKAs = NewCount;
    icl_Trace3(rep_iclSetp, REP_PRUNE_AND_CHECK_KALIVE,
		ICL_TYPE_HYPER, &rp->v.VolIDOfSecondary,
		ICL_TYPE_LONG, oldNumKAs,
		ICL_TYPE_LONG, NewCount);
    return AnyNeed;
}

#define LOTSA_KAS 20

static u_int32 ForceKeepAlives()
{/* Force out any new keep-alive information. */
    u_int32 Secs, BackLimit, MinBackLimit, uDum;
    register struct replica *rp, *xrp, *rootscan;
    int ReplicasWantingKA;
    struct RepKA SomeKAs[LOTSA_KAS], *KABase, *KA;
    int KASize;
    struct timeval Now;

    icl_Trace1(rep_iclSetp, REP_FKEEP_ALIVE_CALL,
		ICL_TYPE_STRING, WhoIAm);
    osi_GetTime(&Now);
    Secs = Now.tv_sec;
    ReplicasWantingKA = 0;
    MinBackLimit = 24*60*60;
    lock_ObtainRead(&RepTableLock);
    for (rp = RepRoot; rp != NULL; rp = rp->Next) {
	/* decide if a replica needs any KA processing. */
	lock_ObtainWrite(&(rp->lockObject));
	if ((rp->Flags & repflag_KillMe) == 0) {
	    rp->Flags &= ~(repflag_KANow | repflag_KAThisPass);
	    if (rp->NumKAs > 0) {
		BackLimit = rp->kaLeadTime;
		if (BackLimit < MinBackLimit) MinBackLimit = BackLimit;
		if (PruneAndCheckKA(rp, Secs, Secs - BackLimit) != 0) {
		    rp->Flags |= repflag_KANow;
		    rp->LastKASweep = Now;
		    ++ReplicasWantingKA;
		}
	    }
	}
	lock_ReleaseWrite(&(rp->lockObject));
    }
    NextForceKeepAlives = Secs + MinBackLimit;	/* Schedule next call */
    if (ReplicasWantingKA > 0) {
	if (ReplicasWantingKA > LOTSA_KAS) {
	    KASize = ReplicasWantingKA * sizeof(struct RepKA);
	    KABase = (struct RepKA *) osi_Alloc(KASize);
	    if (KABase == NULL) {
		lock_ReleaseRead(&RepTableLock);
		return NextForceKeepAlives;
	    }
	    ++NumOversizeKAs;
	    TotOversizeKASizes += ReplicasWantingKA;
	    uDum = ReplicasWantingKA * ReplicasWantingKA;
	    AFS_hadd32(TotOversizeKASizeSquares, uDum);
	} else {
	    KABase = SomeKAs;
	    KASize = 0;
	}
	++NumKAs;
	KA = KABase;
	for (rootscan = RepRoot; rootscan != NULL; rootscan = rootscan->Next) {
	    /* give KA attention to them in bundles. */
	    xrp = NULL;
	    /* Gather up a bundle. */
	    for (rp = rootscan; rp != NULL; rp = rp->Next) {
		lock_ObtainShared(&(rp->lockObject));
		rp->Flags &= ~repflag_KAThisPass;
		if ((rp->Flags & repflag_KANow) != 0) {
		    if (xrp == NULL) xrp = rp;	/* xrp is start of bundle */
		    if (xrp == rp
			|| (SameAddress(&(rp->v.PrimaryAddrs[0]),
					&(xrp->v.PrimaryAddrs[0]))
			    && sameCell(&rp->CellId, &xrp->CellId))) {
			/* another one for this bundle? */
			KA->rep = rp;
			KA->primhost = rp->v.PrimaryAddrs[0];
			++KA;
			rp->Flags |= repflag_KAThisPass;
			lock_UpgradeSToW(&(rp->lockObject));
		    }
		}
	    }
	    /* Bundle is in KABase array. */
	    BackLimit = BundleKeepAlives(KABase);	/* all ThisPass ones are write-locked; all others, shared-locked. */
	    if (NextForceKeepAlives > BackLimit) NextForceKeepAlives = BackLimit;
	    /* Mark bundle as having been processed. */
	    for (rp = rootscan; rp != NULL; rp = rp->Next) {
		if ((rp->Flags & repflag_KAThisPass) != 0) {
		    rp->Flags &= ~(repflag_KANow | repflag_KAThisPass);
		    lock_ReleaseWrite(&(rp->lockObject));
		} else {
		    lock_ReleaseShared(&(rp->lockObject));
		}
	    }
	}
	if (KASize != 0) {			/* free allocated memory */
	    osi_Free((opaque) KABase, KASize);
	    KABase = NULL;
	}
    }
    lock_ReleaseRead(&RepTableLock);
    return NextForceKeepAlives;
}

static time_t ExpireVolChanges(struct replica *rp)
{/* Do background processing necessary to punt old volChanged structures. */
    time_t Now, nextTime, tempNext, maxStamp;
    struct volChanged *vcp, *lastvcp, *nextvcp;

    Now = osi_Time();
    nextTime = Now + 24*60*60;

    lastvcp = NULL;
    for (vcp = rp->Changes; vcp != NULL; vcp = nextvcp) {
	nextvcp = vcp->Another;
	maxStamp = vcp->whenReferenced;
	if (vcp->whenCreated > maxStamp) maxStamp = vcp->whenCreated;
	if (((Now - maxStamp) > (rp->v.maxTotalLatency<<1))) {
	    /*
	     * Expired--delete it
	     */
	    icl_Trace4(rep_iclSetp, REP_FREE_VOL_CHANGED,
		       ICL_TYPE_HYPER, &rp->v.VolIDOfSecondary,
		       ICL_TYPE_HYPER, &vcp->fromVV,
		       ICL_TYPE_HYPER, &vcp->toVV,
		       ICL_TYPE_LONG, vcp->numFids);
	    if (lastvcp == NULL) {
		rp->Changes = vcp->Another;
	    } else {
		lastvcp->Another = vcp->Another;
	    }
	    osi_Free((opaque) vcp->fids, vcp->maxFids*sizeof(struct fidInVol));
	    osi_Free((opaque) vcp, sizeof(struct volChanged));
	} else {
	    /*
	     * Not expired yet--give it three extra seconds to keep from
	     * spinning.
	     */
	    tempNext = maxStamp + (rp->v.maxTotalLatency<<1) + 3;
	    nextTime = MIN(nextTime, tempNext);
	    lastvcp = vcp;
	}
    }

    return nextTime;
}

static pthread_addr_t BackgroundProcessThread(arg)
pthread_addr_t arg;
{/* Do the background processing necessary to keep life in the replicated lane
    moving. */
    u_int32 nextAction, tempNextAction, Now;
    int32 sleeptime;
    int code;
    struct timespec wakeupTime;
    char buf[128];

    icl_Trace0(rep_iclSetp, REP_BKGD_THREAD_INITMUTEX);
    /* hold the global repserver mutex--synchronize with the init process. */
    if (pthread_mutex_lock(&repserverMutex) != 0) {
	sprintf(ErrBuff,
		"error locking global mutex in background; errno = %d",
		errno);
	LogErrPrint();
	pthread_exit((pthread_addr_t)NULL);
	/* execution never reaches here */
    }
    /* immediately release the global repserver mutex (and proceed to do
       useful work). */
    if (pthread_mutex_unlock(&repserverMutex) != 0) {
	sprintf(ErrBuff,
		"error unlocking global mutex in background; errno = %d",
		errno);
	LogErrPrint();
	pthread_exit((pthread_addr_t)NULL);
	/* execution never reaches here */
    }

    icl_Trace0(rep_iclSetp, REP_BKGD_THREAD_INITMUTEX_LOOP);
    Now = osi_Time();
    nextAction = Now + 24*60*60;
    for (;;) {
	tempNextAction = ForceKeepAlives();
	if (tempNextAction < nextAction) {
	    icl_Trace1(rep_iclSetp, REP_FKEEP_ALIVE_SLEEP,
		       ICL_TYPE_LONG, tempNextAction - Now);
	    nextAction = tempNextAction;
	}
	tempNextAction = (unsigned long) DoWillCalls();
	if (tempNextAction < nextAction) {
	    icl_Trace1(rep_iclSetp, REP_DOWILL_CALLS_SLEEP,
		       ICL_TYPE_LONG, tempNextAction - Now);
	    nextAction = tempNextAction;
	}
	/* hold the global repserver mutex */
	if (pthread_mutex_lock(&repserverMutex) != 0) {
	    sprintf(ErrBuff, "error locking repserver global mutex in background; errno = %d",
		    errno);
	    LogErrPrint();
	    pthread_exit((pthread_addr_t)NULL);
	    /* execution never reaches here */
	}
	Now = osi_Time();
	sleeptime = nextAction - Now;
	if (sleeptime > 0) {
	    icl_Trace1(rep_iclSetp, REP_SLEEP_ACTNOW,
		       ICL_TYPE_LONG, sleeptime);
	    wakeupTime.tv_nsec = 0;
	    wakeupTime.tv_sec = nextAction;
	    errno = 0;
	    code = pthread_cond_timedwait(&actNowCond,
					  &repserverMutex,
					  &wakeupTime);
	    if (code == -1 && errno != 0) code = errno;
	    if (code && code != EAGAIN) {
		sprintf(ErrBuff, "error in pthread_cond_timedwait: code = %d", code);
		Log(msgonly, ErrBuff);
		/* LogErrPrint(); */
		/* pthread_exit((pthread_addr_t)NULL); */
		/* execution never reaches here */
		sleep(1);		/* just in case we're spinning */
	    }
	    Now = osi_Time();
	    sleeptime = nextAction - Now;


	    if (sleeptime == 0) sprintf(buf, "on time.");
	    else if (sleeptime < 0) sprintf(buf, "%ld secs late.", -sleeptime);
	    else sprintf(buf, "%ld secs early.", sleeptime);
	    icl_Trace4(rep_iclSetp, REP_TR_BKG,
		       ICL_TYPE_LONG, Now,
		       ICL_TYPE_LONG, nextAction,
		       ICL_TYPE_STRING, (code == EAGAIN ? "timed out" : "signaled"),
		       ICL_TYPE_STRING, buf);
	}
	/* release the global repserver mutex */
	if (pthread_mutex_unlock(&repserverMutex) != 0) {
	    sprintf(ErrBuff,
		    "error unlocking repserver global mutex in background; errno = %d",
		    errno);
	    LogErrPrint();
	    pthread_exit((pthread_addr_t)NULL);
	    /* execution never reaches here */
	}
	nextAction = Now + 24*60*60;
    }
}

static pthread_addr_t RepThread(arg)
    pthread_addr_t arg;
{
    time_t		nextAction, tempNextAction;
    rep_queue_cookie_t	qCookie;
    struct replica	*rp;

    /*
     * Hold the global repserver mutex and release it immediately to
     * synchronize with the main thread.
     */
    if (pthread_mutex_lock(&repserverMutex) != 0 ||
	pthread_mutex_unlock(&repserverMutex) != 0) {
	sprintf(ErrBuff,
		"error (un)locking global mutex in background; errno = %d",
		errno);
	LogErrPrint();
	pthread_exit((pthread_addr_t)NULL);
	/* execution never reaches here */
    }

    for (;;) {
	rp = repq_Get(&RepQ, &qCookie);

	lock_ObtainWrite(&rp->lockObject);

	osi_assert(rp->qCookie == qCookie);

	if (rp->Flags & repflag_KillMe) {
	    lock_ReleaseWrite(&rp->lockObject);

	    repq_Delete(&RepQ, qCookie);
	    
	    lock_ObtainWrite(&RepTableLock);

	    if (rp == RepRoot) {
		RepRoot = rp->Next;
	    } else {
		struct replica	*prp;

		for (prp = RepRoot; prp != NULL; prp = prp->Next) {
		    if (prp->Next == rp)
			break;
		}
		osi_assert(prp != NULL);
		prp->Next = rp->Next;
	    }
	    --numReplicas;

	    lock_ReleaseWrite(&RepTableLock);

	    RepRele(rp);		/* Once for RepQ */
	    RepRele(rp);		/* Once for RepRoot */
	} else {
	    nextAction = osi_Time() + 24*60*60;

	    tempNextAction = RenewTokens(rp);
	    if (tempNextAction < nextAction) {
		icl_Trace1(rep_iclSetp, REP_RENEW_TKNS_SLEEP,
			   ICL_TYPE_LONG, tempNextAction - osi_Time());
		nextAction = tempNextAction;
	    }

	    tempNextAction = StartImporting(rp);
	    if (tempNextAction < nextAction) {
		icl_Trace1(rep_iclSetp, REP_START_IMPORT_SLEEP,
			   ICL_TYPE_LONG, tempNextAction - osi_Time());
		nextAction = tempNextAction;
	    }

	    tempNextAction = ExpireVolChanges(rp);
	    if (tempNextAction < nextAction) {
		icl_Trace1(rep_iclSetp, REP_EXPVOL_CHANGES_SLEEP,
			   ICL_TYPE_LONG, tempNextAction - osi_Time());
		nextAction = tempNextAction;
	    }

	    lock_ReleaseWrite(&rp->lockObject);
	}

	/*
	 * If the replica was deleted the next action time is irrelevant,
	 * since this call will just free up the queue item.
	 */
	repq_Put(&RepQ, qCookie, nextAction);
    }
}	/* RepThread() */

static pthread_addr_t ReaperThread(arg)
    pthread_addr_t arg;
{
    long 		code;
    struct replica	*prp;
    rep_queue_cookie_t	qCookie;
    struct replica	*rp;
    time_t		retryTime;
    
    for (;;) {
	rp = repq_Get(&DeathQ, &qCookie);
	
	/*
	 * We don't grab any locks here, since this replica is no longer
	 * on any lists.  No other thread can find it.
	 */
	osi_assert((rp->Flags & repflag_KillMe) && rp->RefCount == 0);

	sprintf(rp->RepMsg, "Destroying replica");
	LogPrint(rp);

	if ((code = DeleteAllHangingState(rp)) == 0) {
	    osi_Free(rp, sizeof *rp);
	    repq_Delete(&DeathQ, qCookie);
	} else {
	    /* If we the delete failed, set a time at which we'll try again */
	    retryTime = osi_Time() + rp->localDiskWait;

	    sprintf(rp->RepMsg,
		    "Destroy failed: %s -- retrying in %lu seconds",
		    dfs_dceErrTxt(code), (unsigned long)rp->localDiskWait);
	    DebugPrint(rp);
	}

	/*
	 * If we just did a repq_Delete(), the deadline is irrelevant, since
	 * this call will just free up the queue item.  If
	 * DeleteAllHangingState() failed, we try again later.
	 */
	repq_Put(&DeathQ, qCookie, retryTime);
    }
}	/* ReaperThread() */

/*------------------------------------------------------------------------
 * REP_KeepFilesAlive
 *
 * Description:
 *	Keep a file from being deleted on the primary.
 *
 * Arguments:
 *	VolP: Pointer to the volume in which this file sits.
 *	FidP: Descriptor of file being kept alive.
 *	Flags: Flag bits for the operation.  Bits include REP_FLAG_EXECUTING for
 *		files that are not only being read but also being executed from.
 *
 * Returns:
 *	REP_ERR_SUCCESS: everything went well.
 *	REP_ERR_NOTREPLICA: the volume is not a replica, or the VolP doesn't match the FidP.
 *	REP_ERR_UNKNOWNREPLICA: the volume is not a recognizable kind of replica.
 *
 * Environment:
 *	Not yet specified.
 *
 * Locks:
 *	Not yet specified.
 *
 * Side Effects:
 *	Not yet specified.
 *------------------------------------------------------------------------*/
static int cmpFids(fid1p, fid2p)
const void  *fid1p, *fid2p;
{
    register unsigned long *lp1, *lp2;
    register int count;

    lp1 = (unsigned long *) fid1p;
    lp2 = (unsigned long *) fid2p;
    for (count = (sizeof(afsFid) / sizeof(unsigned long)); count > 0; --count) {
	if (*lp1 < *lp2) return -1;
	else if (*lp1 > *lp2) return 1;
	++lp1; ++lp2;
    }
    return 0;
}

static u_long SREP_KeepFilesAlive(rpcCallp, files, howManyFiles, flags, spare1, spare2, spare3, spare4, spare5)
handle_t rpcCallp;
afsFids *files;
u_long howManyFiles;
u_long flags;
u_long spare1, spare2, spare3;
u_long *spare4, *spare5;
{/* Keep the named files alive (not deleted) on the source. */
    register struct replica *rp;
    register struct FileKA *ka;
    struct timeval Now;
    register u_int32 Ix, Finger, kaIx, fdIx, missing;
    u_int32 NewMax;
    long BigErr;
    int cancelState, needWakeup;

    cancelState = pthread_setcancel(CANCEL_OFF);
    recordCaller(rpcCallp, "KeepFilesAlive");
    if (cancelState == -1) return COMPAT_ERROR_DISABLE_CANCEL;
    icl_Trace1(rep_iclSetp, REP_KEEP_FALIVE_CALL, 
		ICL_TYPE_STRING, WhoIAm);
    BigErr = 0;
    osi_GetTime(&Now);
    *spare4 = 0;
    *spare5 = 0;
    needWakeup = 0;
    if (files->afsFids_len < howManyFiles) howManyFiles = files->afsFids_len;
    icl_Trace2(rep_iclSetp, REP_KEEP_FALIVE_RET,
		ICL_TYPE_LONG, howManyFiles,
		ICL_TYPE_LONG, flags);
    qsort(&files->afsFids_val[0], howManyFiles, sizeof(afsFid), cmpFids);
    for (Ix = 0; Ix < howManyFiles; Ix = Finger) {
	for (Finger = Ix+1; Finger < howManyFiles; ++Finger) {	/* gather all in a volume */
	    if (!AFS_hsame(files->afsFids_val[Ix].Volume, files->afsFids_val[Finger].Volume)
		|| !sameCell(&files->afsFids_val[Ix].Cell, &files->afsFids_val[Finger].Cell)) break;
	}
	icl_Trace4(rep_iclSetp, REP_KEEP_FALIVE_HUNK,
		   ICL_TYPE_HYPER, &files->afsFids_val[Ix].Cell,
		   ICL_TYPE_HYPER, &files->afsFids_val[Ix].Volume,
		   ICL_TYPE_LONG, Ix,
		   ICL_TYPE_LONG, Finger);
	/* The fids in [Ix..Finger) are in the same cell&volume; find the rp. */
	lock_ObtainRead(&RepTableLock);
	for (rp = RepRoot; rp != NULL; rp = rp->Next) {
	    if (AFS_hsame(rp->v.VolIDOfSecondary, files->afsFids_val[Ix].Volume)
		&& (sameCell(&rp->CellId, &files->afsFids_val[Ix].Cell))) {
		RepHold(rp);
		lock_ObtainWrite(&(rp->lockObject));
		break;
	    }
	}
	lock_ReleaseRead(&RepTableLock);
	if (rp != NULL) {	/* found a match */
	    icl_Trace1(rep_iclSetp, REP_KEEP_FALIVE_HUNKRP,
		       ICL_TYPE_POINTER, rp);
	    missing = 0;
	    for (fdIx = Ix; fdIx < Finger; ++fdIx) {
		for (kaIx = 0; kaIx < rp->NumKAs; ++kaIx) {
		    if ((rp->KA[kaIx].VnodeIx == files->afsFids_val[fdIx].Vnode)
			&& rp->KA[kaIx].Uniq == files->afsFids_val[fdIx].Unique) {
			if (rp->KA[kaIx].RecvWhen < Now.tv_sec) {
			    rp->KA[kaIx].RecvWhen = Now.tv_sec;
			    needWakeup = 1;
			}
			files->afsFids_val[fdIx].Vnode = 0;
			BigErr = 0;
			break;
		    }
		}
		if (kaIx >= rp->NumKAs) {
		    kaIx = rp->NumKAs;
		    if (kaIx < rp->MaxKAs) {
			rp->KA[kaIx].VnodeIx = files->afsFids_val[fdIx].Vnode;
			rp->KA[kaIx].Uniq = files->afsFids_val[fdIx].Unique;
			rp->KA[kaIx].RecvWhen = Now.tv_sec;
			rp->KA[kaIx].SentWhen = 0;
			rp->KA[kaIx].Flags = (flags & REP_FLAG_EXECUTING) ?
			  KA_EXECUTING : 0;
			if ((flags & REP_FLAG_IMMEDIATE) != 0)
			    rp->KA[kaIx].Flags |= KA_SOON;
			files->afsFids_val[fdIx].Vnode = 0;
			++(rp->NumKAs);
			needWakeup = 1;
		    } else {
			++missing;
		    }
		}
	    }
	    if (missing > 0) {
		NewMax = rp->NumKAs + MAX(8, missing);
		ka = (struct FileKA *) osi_Alloc(NewMax * sizeof(struct FileKA));
		if (ka == NULL) {
		    BigErr = REP_ERR_NOSTORAGE;
		} else {
		    icl_Trace4(rep_iclSetp, REP_TR_KA_INFO,
			       ICL_TYPE_HYPER, &rp->v.VolIDOfSecondary,
			       ICL_TYPE_LONG, rp->NumKAs+missing, 
			       ICL_TYPE_LONG, NewMax,
			       ICL_TYPE_LONG, rp->MaxKAs);
		    if (rp->KA != NULL) {
			bcopy((char *)rp->KA, (char *)ka,
			      (rp->NumKAs * sizeof(struct FileKA)));
			osi_Free((opaque) rp->KA,
				 (rp->MaxKAs * sizeof(struct FileKA)));
		    }
		    rp->KA = ka;
		    rp->MaxKAs = NewMax;
		    kaIx = rp->NumKAs;
		    for (fdIx = Ix; fdIx < Finger; ++fdIx) {
			if (files->afsFids_val[fdIx].Vnode != 0) {
			    osi_assert(kaIx < rp->MaxKAs);
			    rp->KA[kaIx].VnodeIx = files->afsFids_val[fdIx].Vnode;
			    rp->KA[kaIx].Uniq = files->afsFids_val[fdIx].Unique;
			    rp->KA[kaIx].RecvWhen = Now.tv_sec;
			    rp->KA[kaIx].SentWhen = 0;
			    rp->KA[kaIx].Flags = (flags & REP_FLAG_EXECUTING) ?
			      KA_EXECUTING : 0;
			    if ((flags & REP_FLAG_IMMEDIATE) != 0)
				rp->KA[kaIx].Flags |= KA_SOON;
			    ++kaIx;
			    rp->NumKAs = kaIx;
			    needWakeup = 1;
			}
		    }
		}
	    }
	    lock_ReleaseWrite(&(rp->lockObject));
	    RepRele(rp);
	} else BigErr = REP_ERR_NOTREPLICA;
    }
    if (needWakeup) {
	fdIx = pthread_cond_signal(&actNowCond);
	osi_assert(fdIx == 0);
    }
    (void) pthread_setcancel(cancelState);
    return osi_errEncode(BigErr);
}

static long getVolChanged(rp, fromVVp, toVVp, vcpp)
struct replica *rp;
afs_hyper_t *fromVVp, *toVVp;
struct volChanged **vcpp;
{/* Obtain a volChanged structure giving the changes needed to go from *fromVVp
   to at least *toVVp, link it in, and return it. */
    struct volChanged *vcp;
    struct fidInVol *fids;
    u_int32 maxFids, newMaxFids;
    struct fidInVol *newFids;
    int volDesc;
    struct localvol *lv, *blv, *nlv;
    long srcIx, code, code2, scancode, attrcode, isDeleted;
    register long ix;
    struct xvfs_attr attBuf;

    *vcpp = NULL;
    nlv = blv = NULL;
    for (lv = rp->AllLocals; lv != NULL; lv = lv->Next) {
	/* best is an exact match on VVs */
	if (AFS_hsame(*toVVp, lv->volVersion)) {blv = lv; break;}
	/* next is the nearest one higher than *toVVp */
	else if (AFS_hcmp(*toVVp, lv->volVersion) < 0) {
	    if (nlv == NULL) nlv = lv;
	    else if (AFS_hcmp(nlv->volVersion, lv->volVersion) > 0) nlv = lv;
	}
    }
    lv = (blv ? blv : nlv);
    /* any volume meet the criteria? */
    if (lv == NULL) {
	icl_Trace2(rep_iclSetp, REP_TR_NOVOL,
		   ICL_TYPE_HYPER, &rp->v.VolIDOfSecondary, 
		   ICL_TYPE_HYPER, toVVp);		   
	return REP_ERR_SHORTVVLOG;
    }
    icl_Trace3(rep_iclSetp, REP_TR_MAKE_VCHANGE,
		ICL_TYPE_HYPER, &rp->v.VolIDOfSecondary,
		ICL_TYPE_HYPER, fromVVp,
		ICL_TYPE_HYPER, &lv->volVersion);
    icl_Trace0(rep_iclSetp, REP_GETVOLCHG_AGOPEN);
    code = ftu_AggrOpenFset(lv->aggr, &lv->volId,
			    VOL_SYS_SCAN | VOL_SYS_GETATTR,
			    VOLERR_TRANS_REPGETFILESETCHANGES, &volDesc);
    if (code != 0) {
	sprintf(rp->RepMsg, "Can't open local vol %lu,,%lu/%lu to make volChanged: %s",
		AFS_HGETBOTH(lv->volId), lv->aggr, dfs_dceErrTxt(code));
	DebugPrint(rp);
	NoticeIfDeleted(rp, lv, code);
	return code;
    }
    icl_Trace0(rep_iclSetp, REP_GETVOLCHG_SCAN);    
    maxFids = 20;
    fids = (struct fidInVol *) osi_Alloc(maxFids * sizeof(struct fidInVol));
    if (fids == NULL) return ENOMEM;
    ix = 0;
    for (srcIx = VOL_ROOTINO; ; ++srcIx) {
	isDeleted = 0;
	errno = 0;
	scancode = VOL_SCAN(volDesc, srcIx);
	if (scancode != 0) {
	    if (scancode == VOL_ERR_EOF || scancode == VOL_ERR_EOW) {
		code = 0;
		break;
	    } else if (scancode != VOL_ERR_DELETED) {
		code = scancode;
		sprintf(rp->RepMsg,"Can't scan ix %ld in local vol %lu,,%lu/%lu for volChanged: %s",
			srcIx, AFS_HGETBOTH(lv->volId), lv->aggr,
			dfs_dceErrTxt(code));
		DebugPrint(rp);
		break;
	    }
	}
	/* Protect against overloaded VOL_ERR_DELETED result code.
	 * Also keep in mind that we may be talking to an "old" server
	 * that does not support getting attributes of deleted files.
	 */
	AFS_hset64(attBuf.xvattr.fileID, -1, -1);
	errno = 0;
	attrcode = VOL_GETATTR(volDesc, &attBuf);
	if (attrcode != 0) {
	    if (scancode == VOL_ERR_DELETED) {
		/* we attempted to get attributes of a deleted file.
		 * if succeeded, note it; otherwise, ignore error code
		 * and continue scanning.
		 */
		if (attrcode == VOL_ERR_DELETED &&
		    AFS_hgethi(attBuf.xvattr.fileID) != -1) {
		    isDeleted = 1;
		} else {
		    continue;
		}
	    } else {
		/* we attempted to get attributes of an extant file. */
		code = attrcode;
		sprintf(rp->RepMsg, "Can't getAttr ix %ld in local vol %lu,,%lu/%lu for volChanged: %s",
			srcIx,
			AFS_HGETBOTH(lv->volId), lv->aggr,
			dfs_dceErrTxt(code));
		DebugPrint(rp);
		break;
	    }
	}
	if (isDeleted || (AFS_hcmp(attBuf.xvattr.volVersion, *fromVVp) > 0)) {
	    if (ix >= maxFids) {
		newMaxFids = (ix * 2) + 3;
		newFids = (struct fidInVol *) osi_Alloc(newMaxFids * sizeof(struct fidInVol));
		if (newFids == NULL) {
		    sprintf(rp->RepMsg, "No mem for %ld fidinVols",
			    newMaxFids);
		    DebugPrint(rp);
		    code = ENOMEM;
		    break;
		}
		bcopy((char *)fids, (char *)newFids, ix * sizeof(struct fidInVol));
		osi_Free((opaque) fids, maxFids * sizeof(struct fidInVol));
		fids = newFids;
		maxFids = newMaxFids;
	    }
	    fids[ix].Vnode = AFS_hgethi(attBuf.xvattr.fileID);
	    fids[ix].Unique = AFS_hgetlo(attBuf.xvattr.fileID);
	    ++ix;
	}
    }
    if (code != 0) {
	(void) ftu_AggrAbortFsetOpen(volDesc);
	osi_Free((opaque) fids, maxFids * sizeof(struct fidInVol));
	return code;
    }
    icl_Trace0(rep_iclSetp, REP_GETVOLCHG_CLOSE);
    /* got EOF on the new volume */
    code = ftu_AggrCloseFset(volDesc);
    if (code != 0) {
	sprintf(rp->RepMsg, "Can't close volChanged vol: %s",
		dfs_dceErrTxt(code));
	DebugPrint(rp);
	osi_Free((opaque) fids, maxFids * sizeof(struct fidInVol));
	return code;
    }
    icl_Trace0(rep_iclSetp, REP_GETVOLCHG_CLOSE_FIN);
    vcp = (struct volChanged *) osi_Alloc(sizeof(struct volChanged));
    if (vcp == NULL) {
	osi_Free((opaque) fids, maxFids * sizeof(struct fidInVol));
	return ENOMEM;
    }
    vcp->whenCreated = osi_Time();
    vcp->whenReferenced = vcp->whenCreated;
    vcp->fromVV = *fromVVp;
    vcp->toVV = lv->volVersion;
    vcp->numFids = ix;
    vcp->maxFids = maxFids;
    vcp->fids = fids;
    sprintf(rp->RepMsg, "New volChanged object (%lu,,%lu -> %lu,,%lu) has %d fids.",
	     AFS_HGETBOTH(*fromVVp), AFS_HGETBOTH(lv->volVersion), ix);
    DebugPrint(rp);
    lock_UpgradeSToW(&(rp->lockObject));
    vcp->Another = rp->Changes;
    rp->Changes = vcp;
    lock_ConvertWToS(&(rp->lockObject));
    *vcpp = vcp;
    return 0;
}

/*------------------------------------------------------------------------
 * REP_GetVolChangedFiles
 *
 * Description:
 *	Fetch IDs for files that were new since a given VV.
 *
 * Arguments:
 *	volumeID: identifier for the volume being queried.
 *	initialVV: volume version that the caller has.
 *	finalVV: volume version to which the caller wishes to advance.
 *	reallyFinalVV: volume version to which the result advances.
 *	fileIDs: IDs for a superset of the files changed between initialVV and reallyFinalVV.
 *
 * Returns:
 *	REP_ERR_SUCCESS: everything went well.
 *	REP_ERR_SHORTVVLOG: server does not remember enough to answer the question
 *	REP_ERR_NOTREPLICA: this is not a replicated volume
 *	REP_ERR_NOVOLUME: no such volume exists on this server
 *
 * Environment:
 *	This call will only work if the local server kept enough state.
 *
 * Locks:
 *	Not yet specified;
 *
 * Side Effects:
 *	Not yet specified.
 *------------------------------------------------------------------------*/
static u_long SREP_GetVolChangedFiles(rpcCallp, cellIdP, volIdP, initialVV,
	finalVV, cookie, spare1, spare2, spare3, reallyFinalVV, nextCookieP,
	fidPerVolP, numFidsInVolP, flagsP, spare4, spare5)
handle_t rpcCallp;
afs_hyper_t *cellIdP, *volIdP, *initialVV, *finalVV;
u_long cookie;
u_long spare1, spare2, spare3;
afs_hyper_t *reallyFinalVV;
u_long *nextCookieP;
struct fidsInVol *fidPerVolP;
u_long *numFidsInVolP;
u_long *flagsP;
u_long *spare4, *spare5;
{/* Get the ids of files in the given volume that have changed between the given VVs. */
    register struct replica *rp;
    register struct volChanged *vcp, *vc2, *vc3;
    struct volChanged *vcx;
    afs_hyper_t fromVV;
    u_long nowTime;
    int Progress, AtEnd;
    long code, numRet, shadowNum, toBuild, toStore, ix, ixLen;
    int cancelState;

    cancelState = pthread_setcancel(CANCEL_OFF);
    recordCaller(rpcCallp, "GetVolChangedFiles");
    if (cancelState == -1) return COMPAT_ERROR_DISABLE_CANCEL;
    icl_Trace1(rep_iclSetp, REP_GETVOLCHG_FILES,
		ICL_TYPE_STRING, WhoIAm);
    fidPerVolP->fidsInVol_len = 0;		/* RPC insurance */
    AFS_hzero(*reallyFinalVV);
    *nextCookieP = *numFidsInVolP = 0;
    *flagsP = *spare4 = *spare5 = 0;
    code = AFS_hcmp(*initialVV, *finalVV);
    if (code > 0) {
	(void) pthread_setcancel(cancelState);
	return osi_errEncode(EINVAL);
    }
    if (code == 0) {
	*reallyFinalVV = *finalVV;
	*flagsP = 1;
	(void) pthread_setcancel(cancelState);
	return 0;
    }
    lock_ObtainRead(&RepTableLock);
    for (rp = RepRoot; rp != NULL; rp = rp->Next) {
	if (AFS_hsame(rp->v.VolIDOfSecondary, *volIdP) && sameCell(&rp->CellId, cellIdP)) {
	    RepHold(rp);
	    break;
	}
    }
    lock_ReleaseRead(&RepTableLock);
    if (rp == NULL) {
	(void) pthread_setcancel(cancelState);
	return REP_ERR_NOTREPLICA;
    }
    lock_ObtainShared(&(rp->lockObject));
    nowTime = osi_Time();
    fromVV = *initialVV;	/* goal: advance fromVV past finalVV */
    Progress = 1;
    numRet = 0;
    while ((AFS_hcmp(fromVV, *finalVV) < 0) && Progress) {
	Progress = 0;
	vc2 = vc3 = NULL;
	for (vcp = rp->Changes; vcp != NULL; vcp = vcp->Another) {
	    /* get in vc2 an exact match for fromVV */
	    if (AFS_hsame(fromVV, vcp->fromVV)) {
		vc2 = vcp;
		break;
	    }
	    /* but take any one that we can use to advance (spanning fromVV) */
	    if (AFS_hcmp(vcp->fromVV, fromVV) < 0 &&
		AFS_hcmp(fromVV, vcp->toVV) < 0) {
		vc3 = vcp;
		break;
	    }
	}
	vcp = (vc2 ? vc2 : vc3);
	if (vcp != NULL) {	/* find something? */
	    numRet += vcp->numFids;
	    fromVV = vcp->toVV;
	    vcp->whenReferenced = nowTime;
	    Progress = 1;
	}
    }
    /* couldn't get there with what we had. */
    if (AFS_hcmp(fromVV, *finalVV) < 0) {
	code = getVolChanged(rp, &fromVV, finalVV, &vcx);
	if (code != 0 || vcx == NULL) {	/* couldn't make something to use */
	    lock_ReleaseShared(&(rp->lockObject));
	    RepRele(rp);
	    (void) pthread_setcancel(cancelState);
	    if (code)
		code = osi_errEncode(code);
	    else
		code = REP_ERR_SHORTVVLOG;
	    return code;
	}
	vcp = vcx;	/* into a register */
	numRet += vcp->numFids;
	nowTime = osi_Time();
    }
    toBuild = numRet - cookie;
    if (toBuild > REP_FIDBULKMAX) toBuild = REP_FIDBULKMAX;
    if (toBuild == 0) {
	lock_ReleaseShared(&(rp->lockObject));
	RepRele(rp);
	(void) pthread_setcancel(cancelState);
	return 0;
    }
    bzero((char *)&fidPerVolP->fidsInVol_val[0], toBuild * sizeof(struct fidInVol));
    toStore = 0;
    shadowNum = 0;
    fromVV = *initialVV;	/* goal: advance 'fromVV' past finalVV */
    Progress = 1;
    AtEnd = 0;
    while ((AFS_hcmp(fromVV, *finalVV) < 0) &&
	    Progress &&
	    (shadowNum < (cookie + toBuild - toStore)) &&
	    (toStore < toBuild)) {
	/* assert(cookie >= shadowNum); */
	Progress = 0;
	vc2 = vc3 = NULL;
	for (vcp = rp->Changes; vcp != NULL; vcp = vcp->Another) {
	    /* get in vc2 an exact match for fromVV */
	    if (AFS_hsame(fromVV, vcp->fromVV)) {
		vc2 = vcp;
		break;
	    }
	    /* but take any one that we can use to advance (spanning fromVV) */
	    if (AFS_hcmp(vcp->fromVV, fromVV) < 0 &&
		AFS_hcmp(fromVV, vcp->toVV) < 0) {
		vc3 = vcp;
		break;
	    }
	}
	vcp = (vc2 ? vc2 : vc3);
	AtEnd = 0;	/* whether at end of VC */
	if (vcp != NULL) {	/* find something? */
	    AtEnd = 1;
	    if ((shadowNum + vcp->numFids) <= cookie ) {
		/* skipping this VC chunk entirely */
		shadowNum += vcp->numFids;
	    } else {
		/* copying out some part of this VC */
		ix = (cookie - shadowNum);	/* where to start copying in VC */
		shadowNum = cookie;	/* advance over skipped parts */
		ixLen = vcp->numFids - ix;	/* how much to copy of this VC */
		if (ixLen > (toBuild - toStore)) {	/* bounded by space remaining in buf */
		    ixLen = (toBuild - toStore);
		    AtEnd = 0;	/* no longer at end of VC */
		}
		bcopy((char *)&vcp->fids[ix], (char *)&fidPerVolP->fidsInVol_val[toStore],
		      (ixLen * sizeof(struct fidInVol)));
		cookie += ixLen;		/* advance over copied part */
		shadowNum += ixLen;
		toStore += ixLen;
	    }
	    fromVV = vcp->toVV;
	    vcp->whenReferenced = nowTime;
	    Progress = 1;
	}
    }
    lock_ReleaseShared(&(rp->lockObject));
    RepRele(rp);
    *reallyFinalVV = fromVV;
    fidPerVolP->fidsInVol_len = toStore;
    if (AtEnd && AFS_hcmp(fromVV, *finalVV) >= 0)
	*flagsP = 1;			/* flag last call needed */
    *nextCookieP = cookie;
    *numFidsInVolP = toStore;
    (void) pthread_setcancel(cancelState);
    return 0;
}

static void copyRepStat(rp, rsp)
register struct replica *rp;
register struct repStatus *rsp;
{/* Copy the replica status from the ``struct replica'' to the repStatus structure. */
    bzero((char *)rsp, sizeof(struct repStatus));
    rsp->volId = rp->v.VolIDOfSecondary;
    if (rp->Flags & repflag_ReleaseOnly) {
	rsp->srcVolId = rp->v.VolIDOfSecondary;
	rsp->srcAggr = rp->v.StageAggregate;
    } else {
	rsp->srcVolId = rp->v.VolIDOfPrimary;
	rsp->srcAggr = rp->v.PrimaryAggregate;
    }
    rsp->curAggr = rp->MyReplicaAggregate;
    if (rp->PublishedP != NULL) {
	rsp->curVV = rp->PublishedP->volVersion;
	rsp->volStates = rp->PublishedP->itsStates;
    }
    rsp->srcVV = rp->VVOnSource;
    rsp->srcAddr = rp->v.PrimaryAddrs[0];
    rsp->CellId = rp->CellId;
    rsp->WVT_ID = rp->WVT_ReadTokenID;
    rsp->tokenLossTime = *((afsTimeval *) &rp->TimeOfReadTokenLoss);
    rsp->tokenExpireTime = *((afsTimeval *) &rp->TimeOfWVTExpiration);
    rsp->lastReplicaPublish = *((afsTimeval *) &rp->LastPublishTime);
    rsp->vvCurr = *((afsTimeval *) &rp->TimeOfLatestKnownCurrent);
    rsp->vvPingCurr = *((afsTimeval *) &rp->TimeOfLastPing);
    rsp->whenTried = *((afsTimeval *) &rp->TimeOfLastRetrievalTry);
    rsp->whenToTryAgain = *((afsTimeval *) &rp->nextLookTime);
    rsp->lastKASweep = *((afsTimeval *) &rp->LastKASweep);
    rsp->flags = rp->Flags;
    rsp->numKAs = rp->NumKAs;
    strncpy((char *)rsp->volName, (char *)rp->v.mainVolName, sizeof(rsp->volName));
    strncpy((char *)rsp->RepMsg, (char *)rp->RepMsg, sizeof(rsp->RepMsg));
}

static u_long SREP_GetRepStatus(rpcCallp, cookie, spare1, spare2, spare3,
				 nextCookieP, StatusesP, howManyStatusesP, flagsP,
				 spare4, spare5)
handle_t rpcCallp;
u_long cookie;
u_long spare1, spare2, spare3;
u_long *nextCookieP;
struct repStatuses *StatusesP;
u_long *howManyStatusesP, *flagsP;
u_long *spare4, *spare5;
{/* Get status information for how the replication maintenance is going, per-replica. */
    long thisCook;
    register struct replica *rp;
    long Ix, NumToUse;
    register struct repStatus *rsp;
    int cancelState;

    cancelState = pthread_setcancel(CANCEL_OFF);
    recordCaller(rpcCallp, "GetRepStatus");
    if (cancelState == -1) return COMPAT_ERROR_DISABLE_CANCEL;
    icl_Trace2(rep_iclSetp,  REP_GETREPSTAT_CALL,
		ICL_TYPE_STRING, WhoIAm,
		ICL_TYPE_LONG, cookie);
    NumToUse = 0;
    StatusesP->repStatuses_len = 0;
    lock_ObtainRead(&RepTableLock);
    for (thisCook = 0, rp = RepRoot; rp != NULL; ++thisCook, rp = rp->Next) {
	if (thisCook >= cookie) ++NumToUse;
    }
    if (NumToUse > REP_STATBULKMAX) NumToUse = REP_STATBULKMAX;
    Ix = 0;
    for (thisCook = 0, rp = RepRoot; rp != NULL; ++thisCook, rp = rp->Next) {
	if (thisCook >= cookie) {
	    if (Ix < NumToUse) {
		rsp = &StatusesP->repStatuses_val[Ix++];
		/*
		 * No need to hold rp.  We hold the RepTableLock, so we know
		 * there is at least one reference.
		 */
		lock_ObtainRead(&rp->lockObject);
		copyRepStat(rp, rsp);
		lock_ReleaseRead(&rp->lockObject);
	    }
	}
    }
    lock_ReleaseRead(&RepTableLock);
    *howManyStatusesP = StatusesP->repStatuses_len = Ix;
    *spare4 = 0;
    *spare5 = 0;
    if (rp != NULL) {
	/* not EOF */
	*flagsP = 0;
	*nextCookieP = thisCook + 1;
	(void) pthread_setcancel(cancelState);
	return 0;
    } else {
	/* EOF */
	*flagsP = 1;
	*nextCookieP = thisCook + 1;
	(void) pthread_setcancel(cancelState);
	return (Ix == 0 ? REP_ERR_EOF : 0);
    }
}

static u_long SREP_GetOneRepStatus(rpcCallp, cellIdP, volIdP, spare1, spare2, spare3,
	StatusP, flagsP, spare4, spare5)
handle_t rpcCallp;
afs_hyper_t *cellIdP, *volIdP;
u_long spare1, spare2, spare3;
repStatus *StatusP;
u_long *flagsP;
u_long *spare4, *spare5;
{/* Get the status of one particular replica. */
    register struct replica *rp;
    long code;
    int cancelState;

    cancelState = pthread_setcancel(CANCEL_OFF);
    recordCaller(rpcCallp, "GetOneRepStatus");
    if (cancelState == -1) return COMPAT_ERROR_DISABLE_CANCEL;
    icl_Trace1(rep_iclSetp, REP_GETONE_REPSTAT_CALL,
		ICL_TYPE_STRING, WhoIAm);
    *flagsP = *spare4 = *spare5 = 0;
    lock_ObtainRead(&RepTableLock);
    for (rp = RepRoot; rp != NULL; rp = rp->Next) {
	if (AFS_hsame(rp->v.VolIDOfSecondary, *volIdP) && sameCell(&rp->CellId, cellIdP)) {
	    RepHold(rp);
	    break;
	}
    }
    lock_ReleaseRead(&RepTableLock);
    if (rp == NULL) {
	(void) pthread_setcancel(cancelState);
	return REP_ERR_NOTREPLICA;
    }
    lock_ObtainRead(&(rp->lockObject));
    copyRepStat(rp, StatusP);
    lock_ReleaseRead(&(rp->lockObject));
    RepRele(rp);
    (void) pthread_setcancel(cancelState);
    return 0;
}

static u_long SREP_GetRepServerStatus(rpcCallp, spare1, spare2, spare3, serverStatusP, spare4, spare5)
handle_t rpcCallp;
u_long spare1, spare2, spare3;
struct repserverStatus *serverStatusP;
u_long *spare4, *spare5;
{/* Get status information for how the replication maintenance is going, server-wide. */
    register struct repserverStatus *rssp = serverStatusP;
    int cancelState;

    cancelState = pthread_setcancel(CANCEL_OFF);
    recordCaller(rpcCallp, "GetRepServerStatus");
    if (cancelState == -1) return COMPAT_ERROR_DISABLE_CANCEL;
    icl_Trace1(rep_iclSetp, REP_GETREP_SERVSTAT_CALL,
		ICL_TYPE_STRING, WhoIAm);
    bzero((char *)rssp, sizeof(struct repserverStatus));
    rssp->Attns.Count = NumAttns;
    rssp->Attns.OverCount = NumOversizeAttns;
    rssp->Attns.SizeOverCount = TotOversizeAttnSizes;
    rssp->Attns.SizeOverCountSq = TotOversizeAttnSizeSquares;
    rssp->KAs.Count = NumKAs;
    rssp->KAs.OverCount = NumOversizeKAs;
    rssp->KAs.SizeOverCount = TotOversizeKASizes;
    rssp->KAs.SizeOverCountSq = TotOversizeKASizeSquares;
    rssp->nextForceKA = NextForceKeepAlives;
    lock_ObtainRead(&RepTableLock);
    rssp->numReplicas = numReplicas;
    lock_ReleaseRead(&RepTableLock);
    rssp->numHosts = reph_GetNumHosts();
    rssp->numAllocVIDs = numAllocVolIDs;
    rssp->numUsedVIDs = numUsedVolIDs;
    rssp->numReusedVIDs = numReusedVolIDs;
    *spare4 = 0;
    *spare5 = 0;
    (void) pthread_setcancel(cancelState);
    return 0;
}

/*------------------------------------------------------------------------
 * REP_UpdateSelf
 *
 * Description:
 *	Remote request (a hint) to update a specific replica.
 *
 * Arguments:
 *	cellID: cell identifier
 *	volumeID: identifier of volume for which update is requested.
 *
 * Returns:
 *	REP_ERR_SUCCESS: everything went well.
 *	REP_ERR_TOOEARLY: replication server cannot yet begin the update process, because the minimumPounceDally} interval
                           has not yet expired.
 *	REP_ERR_NOWAYJOSE: replication server chooses to ignore the call.
 *	REP_ERR_NOTREPLICA: this is not a replicated volume
 *	REP_ERR_NOVOLUME: no such volume exists on this server
 *
 * Environment:
 *	Not yet specified.
 *
 * Locks:
 *	Not yet specified.
 *
 * Side Effects:
 *	Not yet specified.
 *------------------------------------------------------------------------*/
long rep_doUpdate(rp)
struct replica *rp;
{/* Cause an update to happen soon. */
    lock_ObtainWrite(&(rp->lockObject));
    osi_GetTime(&rp->nextLookTime);	/* clobber this value */
    rp->Flags |= repflag_UserWantsUpdate;
    if (rp->Flags & repflag_ReleaseOnly) {
	SetLatestCurrentTime(rp, NULL, NULL);
	AFS_hzero(rp->WVT_TypesLeft);
	AFS_hzero(rp->WVT_TypesRevoked);
    }
    AFS_hzero(rp->VVOnSource);
    repq_ResetDeadline(&RepQ, rp->qCookie, 0);
    lock_ReleaseWrite(&(rp->lockObject));
    return 0;
}

static u_long SREP_UpdateSelf(rpcCallp, cellIdP, volIdP, flags, spare1, spare2, spare3, spare4, spare5)
handle_t rpcCallp;
afs_hyper_t *cellIdP, *volIdP;
u_long flags;
u_long spare1, spare2, spare3;
u_long *spare4, *spare5;
{/* Request an update to the given volume. */
    register struct replica *rp;
    long ErrCode;
    int cancelState;

    cancelState = pthread_setcancel(CANCEL_OFF);
    recordCaller(rpcCallp, "UpdateSelf");
    if (cancelState == -1) return COMPAT_ERROR_DISABLE_CANCEL;
    icl_Trace1(rep_iclSetp, REP_UPDSELF_CALL, 
		ICL_TYPE_STRING, WhoIAm);
    ErrCode = 0;
    lock_ObtainRead(&RepTableLock);
    for (rp = RepRoot; rp != NULL; rp = rp->Next) {
	if (AFS_hsame(*volIdP, rp->v.VolIDOfSecondary) &&
	    sameCell(cellIdP, &rp->CellId)) {
	    RepHold(rp);
	    break;
	}
    }
    lock_ReleaseRead(&RepTableLock);
    if (rp == NULL) {
	ErrCode = REP_ERR_NOVOLUME;
    } else {
	rep_doUpdate(rp);
	RepRele(rp);
	ErrCode = REP_ERR_SUCCESS;
    }
    (void) pthread_setcancel(cancelState);
    return ErrCode;
}
static void AbortDumpHandler(sig)
int sig;
{
    dce_svc_printf(RPS_S_ABORT_DUMP_MSG);
    icl_DumpUser();
    abort();
}


static void SetupICL()
{
    struct icl_log *logp;
    long code, code2;
    static int Initted = 0;

    if (Initted)
	return;
    Initted = 1;

    code = icl_ExportIF("repserver");
    if (code)
	(void) fprintf(stderr, "could not export ICL interface, code=%d\n", code);

    code = icl_StartCatcher(0);
    if (code < 0)
	(void) fprintf(stderr, "could not start ICL signal watcher, code=%d, errno=%d\n", code, errno);	
    else
    {
	code = icl_CreateLog("common", 30*1024, &logp);
	if (code == 0) {
	    code = icl_CreateSet("repserver", logp, (struct icl_log *) 0, &rep_iclSetp);
	    code2 = ftu_CreateIclSet(logp);
	    if (!code) code = code2;
	}
	if (code)
	    fprintf(stderr, "repserver: warning: can't initialize ICL tracing; code %d\n", code);
#if defined(AFS_DEBUG) && defined(AFS_AIX_ENV)
	else
	{
#ifdef AFS_SIGACTION_ENV
#define SIGFUNC sigaction
	    struct sigaction	sv;

	    /* set up sigaction stuff now since CMA won't let us catch these */
	    bzero((char *)&sv, sizeof(sv));
	    sv.sa_handler = AbortDumpHandler;
#else
#define SIGFUNC sigvec
	    struct sigvec	sv;

	    /* set up sigaction stuff now since CMA won't let us catch these */
	    bzero((char *)&sv, sizeof(sv));
	    sv.sv_handler = AbortDumpHandler;
#if 0
	    sv.sv_flags = SA_OLDSTYLE;
#endif /* 0 */
#endif /* AFS_SIGACTION_ENV */

	    if (SIGFUNC(SIGBUS, &sv, NULL) < 0) {
		dce_svc_printf(RPS_S_CATCH_SIG_MSG,"SIGBUS",errno);
	    }
	    if (SIGFUNC(SIGSEGV, &sv, NULL) < 0) {
		dce_svc_printf(RPS_S_CATCH_SIG_MSG,"SIGSEGV",errno);
	    }
	    if (SIGFUNC(SIGILL, &sv, NULL) < 0) {
		dce_svc_printf(RPS_S_CATCH_SIG_MSG,"SIGILL",errno);
	    }
	}
#endif	/* AFS_DEBUG && AFS_AIX_ENV */
     }
}


/*------------------------------------------------------------------------
 * static rep_Initialize
 *
 * Description:
 *	Initialize the Replication Service.
 *
 * Arguments:
 *	None.
 *
 * Returns:
 *	REP_ERR_SUCCESS if everything went well.
 *
 * Environment:
 *	This call will only perform the module initialization the first
 *	time it is called.  Subsequent calls will be no-ops.  There is a
 *	potential race condition should two parties call this routine when
 *	the module has yet to be initialized; this really cannot be
 *	resolved with locks, since locks themselves must be initialized.
 *
 * Locks:
 *	No locks are held across invocations.  Internally, RepTableLock
 *	is held for the entire routine.
 *
 * Side Effects:
 *	Sets up internal data structures; initializes locks.
 *------------------------------------------------------------------------*/

static int32 rep_Initialize()
{ /* rep_Initialize */
    static int IAmInitialized = 0;

/* If we're already initialized, bug out right now. */
    if (IAmInitialized != 0) return REP_ERR_SUCCESS;

/* We seem to be the first thread here. */
    lock_Init(&RepTableLock);
    lock_ObtainWrite(&RepTableLock);
    ++IAmInitialized;
    if (IAmInitialized != 1) panic("Double initialization of replication package");

    /* Do the nitty-gritty of initialization */
    initRepLog(WhoIAm);
    RepRoot = NULL;
    numReplicas = 0;
    repq_InitICL(rep_iclSetp);
    repq_Init(&RepQ);
    repq_Init(&DeathQ);
    AFS_hzero(zeroDum);
    NumAttns = NumOversizeAttns = TotOversizeAttnSizes = 0;
    AFS_hzero(TotOversizeAttnSizeSquares);
    NumKAs = NumOversizeKAs = TotOversizeKASizes = 0;
    AFS_hzero(TotOversizeKASizeSquares);

    AFS_hzero(localCellID);
    localCellName[0] = 0;
    AFS_hset64(allTokenTypes,  (~((unsigned long) 0)), (~((unsigned long) 0)));

    NextForceKeepAlives = 0;
    ErrBuff[0] = '\0';
    FreeIDInitialized = 0;
    numAllocVolIDs = numUsedVolIDs = numReusedVolIDs = 0;

    (void) initPeers();
    initialize_rep_error_table();

    /* Initialize the error-mapping table used for decoding the codes from RPC calls. */
    osi_initDecodeTable();
    /* Initialize the encoding table as well. */
    osi_initEncodeTable();

    lock_ReleaseWrite(&RepTableLock);
    return REP_ERR_SUCCESS;
}

static int RepDriver (as, arock)
  IN struct cmd_syndesc *as;
  IN char *arock;
{
    /* Call rx_Init(); pass it one of the ports that will be exported.
      Build a collection of credentials.
      For each RPC service to be exported, call rx_NewService().
        Pass it the xxxxExecuteRequest procedure address and the credentials.
    To start all the RPC services, call rx_StartServer().
      (Decide if that process is to be donated.)

    To import an RPC service, use rx_NewConnection().
      Can use the same connection for many simultaneous calls.
	If need to use a split call, have to do rx_NewCall and separately call
	  the Start and End procedures for the call.
	Pass rx_NewConnection() the credentials to use.
 */
    long code;
    int again;
    sigset_t		signalBlockSet;

    /*
     * Process arguments for consistency before we try any initialization
     */
    parm_mainProcs = 4;
    parm_tokenProcs = 4;
    if (as->parms[0].items)
	parm_mainProcs = atoi(as->parms[0].items->data);
    if (as->parms[1].items)
	parm_tokenProcs = atoi(as->parms[1].items->data);
/*    if (as->parms[2].items)
       Debugging = 1;*/

    if (parm_mainProcs <= 0) {
	dce_svc_printf(RPS_S_INVALID_ARG_MSG,WhoIAm,"-mainprocs");
	exit(1);
    }

    if (parm_tokenProcs <= 0) {
	dce_svc_printf(RPS_S_INVALID_ARG_MSG,WhoIAm,"-tokenprocs");
	exit(1);
    }

  /* 
   * block SIGUSR1 for all threads. The ICL thread that wiil be
   * waiting for it will clear its mask by using sigwait()
   */
  (void)sigemptyset(&signalBlockSet);
  (void)sigaddset(&signalBlockSet, SIGUSR1);
  (void)sigprocmask(SIG_BLOCK, &signalBlockSet, (sigset_t *)NULL);  

    SetupICL();

    /* temp code to disable private datagram sockets to work around 6731 */
#ifndef AFS_AIX_ENV
    rpc__dg_disa_priv_sockets();
#endif /* AFS_AIX_ENV */


    code = rep_Initialize();


    if (code != 0) {
      Log("%s: %s from rep_Initialize().  Exiting.",
          WhoIAm, dfs_dceErrTxt(code));
      exit(1);
    }


#ifdef	_EPISODE_LIC_MGMT_
    /*
     * Get an LFS license.  This call starts a thread which checks-in the
     * license every so often.  Note that DCE_EPISODE licenses are created
     * using the "same node" policy, so if dfsbind is running (and wants an
     * episode license) we will share with it.
     */
    {
	lic_status_t lcode;
	const char EPL_LIC_VERSION[] = "1.0";
	const int EPL_CHECK_PERIOD = 30*60;	/* 30 minutes */

	lcode = lic_GetTieredLicense(LICENSE_DCE_EPISODE, EPL_LIC_VERSION,
				     callbackOnCheckinFailed, callbackOnEvent,
				     EPL_CHECK_PERIOD, 1, 0);
	if (lcode != LICENSE_SUCCESS) {
	    fprintf(stderr,
		"%s: Failed to obtain license for %s (code=%d).  Exiting.\n",
		WhoIAm, lic_GetProductNameString(LICENSE_DCE_EPISODE), lcode);
	    Log("%s: Failed to obtain license for %s (code=%d).  Exiting.",
		WhoIAm, lic_GetProductNameString(LICENSE_DCE_EPISODE), lcode);
	    exit(1);
	}
    }
#endif

    /* spawn a thread to keep our identity up to date */
    code = dfsauth_client_AutoRefreshLocalAuthContext();
    if (code != 0) {
	Log("%s: %s from dfs_client_AutoRefreshLocalAuthContext().  Exiting.",
	    WhoIAm, dfs_dceErrTxt(code));
	exit(1);
    }

    Log("Replication server started.  Mainprocs=%d; tokenprocs=%d.",
	 parm_mainProcs, parm_tokenProcs);

    numMyNetAddrs = -1;
    if (getMyNetAddrs() <= 0) {
	dce_svc_printf(RPS_S_BAD_NW_ADDRESS_MSG,
		WhoIAm);
	exit(1);
    }
    /* initClients() sets up imported RPC interfaces */
    /* also, sets up all the threads */
    if (initClients() != 0) exit(1);
    /* Get information and start ourselves going */
    for (;;) {
	code = EnumerateVLDB(&again);
	if (code) {
	    if (again) {
		Log("%s: %s from EnumerateVLDB().  Retrying.",
		    WhoIAm, dfs_dceErrTxt(code));
		sleep(30);
	    } else {
		Log("%s: %s from EnumerateVLDB().  Exiting.",
		    WhoIAm, dfs_dceErrTxt(code));
		exit(1);
	    }
	} else break;
    }
    code = EnumerateLocalVolumes();
    if (code) {
	Log("%s: %s from EnumerateLocalVolumes().  Exiting.",
	    WhoIAm, dfs_dceErrTxt(code));
	exit(1);
    }
    code = FixUpLocals();
    if (code) {
	Log("%s: %s from FixUpLocals().  Exiting.",
	    WhoIAm, dfs_dceErrTxt(code));
	exit(1);
    }
    code = RationalizeVolumeLists(1);
    if (code) {
	Log("%s: %s from RationalizeVolumeLists().  Exiting.",
	    WhoIAm, dfs_dceErrTxt(code));
	exit(1);
    }

    QueueReplicas();

  /* set up everything NCS needs to run and listen for calls */
    /* initServers() sets up exported RPC interfaces */
    code = initServers();
    /* ``never'' returns, but if it does... */
    if (code) {
	dce_svc_printf(RPS_S_BAD_INIT_SERVERS_MSG,WhoIAm,
		dfs_dceErrTxt(code));
	exit(3);
    }
    dce_svc_printf(RPS_S_INIT_SERVERS_RETURNED_MSG,WhoIAm);
    exit(4);
}

/* Crank everything up. */
main(argc, argv)
int argc; char **argv;
{
    struct cmd_syndesc *ts;

    osi_setlocale(LC_ALL, "");
    ErrBuff[0] = '\0';

    initialize_svc();

    ts = cmd_CreateSyntax((char *)0, RepDriver, (char *)0, "Replication service" );
    cmd_AddParm(ts, "-mainprocs", CMD_SINGLE, CMD_OPTIONAL,
			"number_of_background_daemons");
    cmd_AddParm(ts, "-tokenprocs", CMD_SINGLE, CMD_OPTIONAL,
			"number_of_token_daemons");

    return cmd_Dispatch(argc, argv);
}

/* Entry-point vectors for the two RPC interfaces we export */
globaldef TKN4Int_v4_0_epv_t TKN4Int_v4_0_manager_epv = {
    STKN_Probe,
    STKN_InitTokenState,
    STKN_TokenRevoke,
    STKN_GetCellName,
    STKN_GetLock,
    STKN_GetCE,
    STKN_GetServerInterfaces,
    STKN_SetParams
};

globaldef REP_v4_0_epv_t REP_v4_0_manager_epv = {
    SREP_CheckReplicationConfig,
    SREP_AllCheckReplicationConfig,
    SREP_KeepFilesAlive,
    SREP_GetVolChangedFiles,
    SREP_GetRepStatus,
    SREP_GetRepServerStatus,
    SREP_UpdateSelf,
    SREP_Probe,
    SREP_GetOneRepStatus,
    SREP_GetServerInterfaces
};
