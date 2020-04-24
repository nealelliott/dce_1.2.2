/*
 * @OSF_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: rep_host.c,v $
 * Revision 1.1.5.1  1996/10/02  18:14:06  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:45:59  damon]
 *
 * $EndLog$
 */

/*
 * (C) Copyright 1996, 1994 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_dfserrors.h>
#include <dcedfs/fshs_errs.h>
#include <dcedfs/dfsauth.h>
#include <dcedfs/rep_data.h>

#include "repser.h"
#include "rep_host.h"
#include "rep_trace.h"

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/rep/rep_host.c,v 1.1.5.1 1996/10/02 18:14:06 damon Exp $")

#define REP_HOST_LIFE_GUARANTEE		30
#define REP_HOST_LEAD_TIME		20
#define REP_HOST_SET_CONTEXT_WAIT	10

static long SetContext(rep_host_t*, handle_t);
static long SetNewConn(rep_host_t*, rep_host_conn_t**, rep_host_conn_type_t);
static void LockMutex(pthread_mutex_t*);
static void UnlockMutex(pthread_mutex_t*);

static char*		cellName = NULL;
static rep_host_t*	hostRoot = NULL;
static osi_dlock_t	hostTableLock;
static struct icl_set*	iclSetP;
static unsigned		numHosts = 0;
static unsigned32	revokeEpoch;
static afsNetData	revokeCallbackAddr;


void
reph_Init(const char* cn, unsigned32 re, const afsNetData* rca,
	  struct icl_set* isP)
{
    cellName = osi_Alloc(strlen(cn) + 1);
    osi_assert(cellName != NULL);
    (void)strcpy(cellName, cn);

    lock_Init(&hostTableLock);

    revokeEpoch = re;
    revokeCallbackAddr = *rca;

    iclSetP = isP;
}	/* reph_Init() */


rep_host_t*
reph_GetHost(struct afsNetAddr* addrs, unsigned numAddrs, char* hostName)
{
    int			code;
    rep_host_t*		hostP;
    unsigned		i, j;

    icl_Trace3(iclSetP, REP_TRACE_REPH_GET_HOST,
	       ICL_TYPE_STRING,
	       inet_ntoa(((struct sockaddr_in*)&addrs[0])->sin_addr),
	       ICL_TYPE_LONG, numAddrs, ICL_TYPE_STRING, hostName);

    osi_assert(addrs != NULL && numAddrs > 0 && hostName != NULL);

    lock_ObtainShared(&hostTableLock);

    for (hostP = hostRoot; hostP != NULL; hostP = hostP->rh_next) {
	if (hostP->rh_numAddrs == numAddrs &&
	    SameAddresses(hostP->rh_addrs, addrs, numAddrs) &&
	    strcmp(hostP->rh_name, hostName) == 0) {

	    /* We have a match */
	    ++hostP->rh_refCount;
	    lock_ReleaseShared(&hostTableLock);
	    icl_Trace1(iclSetP, REP_TRACE_LEAVE_REPH_GET_HOST,
		       ICL_TYPE_POINTER, hostP);
	    return hostP;
	}
    }

    /*
     * Since we're allocating a new host structure, a stronger lock on the
     * host table isn't necessary until we want to thread the new host onto
     * the table.
     */

    if ((hostP = osi_Alloc(sizeof *hostP)) == NULL){
	lock_ReleaseShared(&hostTableLock);
	return NULL;
    }

    bzero(hostP, sizeof *hostP);	/* Overkill */

    hostP->rh_refCount = 2;		/* 1 for host table and 1 for caller */

    /* Initialize the address section */
    bcopy(addrs, hostP->rh_addrs, numAddrs * sizeof *hostP->rh_addrs);
    hostP->rh_numAddrs = numAddrs;
    strncpy(hostP->rh_name, hostName, sizeof hostP->rh_name);
    /* strncpy does not ensure that the string will be terminated */
    hostP->rh_name[(sizeof hostP->rh_name) - 1] = '\0';

    /* Initialialize the fileserver section */
    code = pthread_mutex_init(&hostP->rh_fsMutex, pthread_mutexattr_default);
    if (code == -1) {
	osi_Free(hostP, sizeof *hostP);
	lock_ReleaseShared(&hostTableLock);
	return NULL;
    }
    code = pthread_cond_init(&hostP->rh_fsSetContextDone,
			     pthread_condattr_default);
    if (code == -1) {
	(void)pthread_mutex_destroy(&hostP->rh_fsMutex);
	osi_Free(hostP, sizeof *hostP);
	lock_ReleaseShared(&hostTableLock);
	return NULL;
    }
    hostP->rh_fsFlags = 0;
    hostP->rh_fsConnP = NULL;
    hostP->rh_fsLastSuccess = 0;
    hostP->rh_fsHostLifeGuarantee = REP_HOST_LIFE_GUARANTEE;
    hostP->rh_fsHostLeadTime = REP_HOST_LEAD_TIME;
    code = dfsuuid_Create(&hostP->rh_fsUUID, /* primary server */ 0);
    osi_assert(code == 0);		/* Should never fail */

    /* Initialize the ftserver section */
    code = pthread_mutex_init(&hostP->rh_ftMutex, pthread_mutexattr_default);
    if (code == -1) {
	(void)pthread_cond_destroy(&hostP->rh_fsSetContextDone);
	(void)pthread_mutex_destroy(&hostP->rh_fsMutex);
	osi_Free(hostP, sizeof *hostP);
	lock_ReleaseShared(&hostTableLock);
	return NULL;
    }
    hostP->rh_ftConnP = NULL;

    /* Thread new host onto the table */
    lock_UpgradeSToW(&hostTableLock);

    hostP->rh_next = hostRoot;
    hostRoot = hostP;
    ++numHosts;

    lock_ReleaseWrite(&hostTableLock);

    icl_Trace1(iclSetP, REP_TRACE_LEAVE_REPH_GET_HOST,
	       ICL_TYPE_POINTER, hostP);
    return hostP;
}	/* reph_GetHost() */


void
reph_PutHost(rep_host_t* hostP)
{
    icl_Trace2(iclSetP, REP_TRACE_REPH_PUT_HOST,
	       ICL_TYPE_POINTER, hostP, ICL_TYPE_STRING, hostP->rh_name);

    lock_ObtainWrite(&hostTableLock);
    osi_assert(hostP->rh_refCount > 0);	/* Table still has a reference */
    --hostP->rh_refCount;
    lock_ReleaseWrite(&hostTableLock);
}	/* reph_PutHost() */


long
reph_GetConn(rep_host_t* hostP, rep_host_conn_type_t connType,
	     rep_host_conn_t** outConnPP)
{
    long		code;
    rep_host_conn_t*	currentConnP;
    rep_host_conn_t**	currentConnPP;
    pthread_mutex_t*	mutexP;

    icl_Trace3(iclSetP, REP_TRACE_REPH_GET_CONN,
	       ICL_TYPE_POINTER, hostP, ICL_TYPE_STRING, hostP->rh_name,
	       ICL_TYPE_LONG, connType);

    switch (connType) {
      case REPH_CONN_TYPE_FS:
	currentConnPP = &hostP->rh_fsConnP;
	mutexP = &hostP->rh_fsMutex;
	break;
      case REPH_CONN_TYPE_FT:
	currentConnPP = &hostP->rh_ftConnP;
	mutexP = &hostP->rh_ftMutex;
	break;
      default:
	osi_assert(0);			/* Internal error */
	break;
    }

    LockMutex(mutexP);

    currentConnP = *currentConnPP;	/* For convenience */

    if (currentConnP != NULL) {
	/*
	 * If there is a non-NULL fileserver connection, we shouldn't be
	 * in the middle of a set context.
	 */
	osi_assert(connType != REPH_CONN_TYPE_FS ||
		   !(hostP->rh_fsFlags & REPH_FS_FLAG_DOING_SET_CONTEXT));

	/* We have a good connection; bump its refcount and return it. */
	LockMutex(&currentConnP->rhc_mutex);
	++currentConnP->rhc_refCount;	/* A reference for our caller */
	UnlockMutex(&currentConnP->rhc_mutex);

	*outConnPP = currentConnP;
	code = 0;
    } else {
	/*
	 * We don't already have a connection--get a new one and cache it in
	 * the host structure.
	 */
	code = SetNewConn(hostP, currentConnPP, connType);
	*outConnPP = *currentConnPP;
    }

    UnlockMutex(mutexP);

    icl_Trace2(iclSetP, REP_TRACE_LEAVE_REPH_GET_CONN,
	       ICL_TYPE_LONG, code, ICL_TYPE_POINTER, *outConnPP);

    return code;
}	/* reph_GetConn() */


long
reph_ResetConn(rep_host_t* hostP, rep_host_conn_t** callersConnPP,
	       long connCode)
{
    rep_host_conn_t*		callersConnP = *callersConnPP; /* Convenience*/
    long			code;
    rep_host_conn_type_t	connType;
    rep_host_conn_t*		currentConnP;
    rep_host_conn_t**		currentConnPP;
    pthread_mutex_t*		mutexP;
    rep_host_conn_t*		newConnP;

    /* You shouldn't be trying to reset a connection that you never had. */
    osi_assert(callersConnPP != NULL && callersConnP != NULL);

    /* You shouldn't be trying to reset a connection without a real problem. */
    osi_assert(connCode != 0);

    icl_Trace4(iclSetP, REP_TRACE_REPH_RESET_CONN,
	       ICL_TYPE_POINTER, hostP, ICL_TYPE_STRING, hostP->rh_name,
	       ICL_TYPE_POINTER, *callersConnPP, ICL_TYPE_LONG, connCode);

    connType = callersConnP->rhc_type;

    if (connCode == rpc_s_auth_tkt_expired) {
	/*
	 * If the local auth context expired, there is no need to get
	 * a new connection.  We merely refresh the auth context and leave
	 * the caller's existing connection unchanged.  If the refresh fails,
	 * however, we signal the failure by returning REP_ERR_RESETFAILED.
	 */
	if (code = dfsauth_client_RefreshLocalAuthContext()) {
	    icl_Trace1(iclSetP, REP_TRACE_REPH_REFRESH_AUTH_FAILED,
		       ICL_TYPE_LONG, code);
	    code = REP_ERR_RESETFAILED;
	}

	icl_Trace2(iclSetP, REP_TRACE_LEAVE_REPH_RESET_CONN,
		   ICL_TYPE_LONG, code, ICL_TYPE_POINTER, *callersConnPP);
	return code;
    }

    if (!(connCode == FSHS_ERR_STALEHOST || connCode == FSHS_ERR_FATALCONN ||
	  (connCode >= rpc_s_mod && connCode <= rpc_s_mod + 4096))) {
	/*
	 * If we don't recognize the error as something that a connection
	 * reset will help us recover from, just return the original error
	 * code, leaving the caller's connection unchanged.
	 */
	icl_Trace2(iclSetP, REP_TRACE_LEAVE_REPH_RESET_CONN,
		   ICL_TYPE_LONG, connCode, ICL_TYPE_POINTER, *callersConnPP);
	return connCode;
    }
	
    switch (connType) {
      case REPH_CONN_TYPE_FS:
	currentConnPP = &hostP->rh_fsConnP;
	mutexP = &hostP->rh_fsMutex;
	break;
      case REPH_CONN_TYPE_FT:
	currentConnPP = &hostP->rh_ftConnP;
	mutexP = &hostP->rh_ftMutex;
	break;
      default:
	osi_assert(0);			/* Internal error */
	break;
    }
    
    LockMutex(mutexP);
    
    currentConnP = *currentConnPP;	/* For convenience */

    /*
     * We are going to attempt to reset the caller's connecton, so
     * we lose the reference now.
     */
    reph_PutConn(callersConnP);

    if (currentConnP != callersConnP && currentConnP != NULL) {
	/*
	 * The caller's connection does not match the current connection
	 * in the host structure.  This must mean that some other thread
	 * experienced a problem with the connection and has already
	 * attempted to reset it.  If they were successful, the
	 * current connection will be non-NULL, and we just return it.
	 * If they were unsuccessful, the current connection will be NULL,
	 * and we fall through to the code that attempts a reset.
	 *
	 * If there is a non-NULL fileserver connection, we shouldn't be
	 * in the middle of a set context.
	 */
	osi_assert(connType != REPH_CONN_TYPE_FS ||
		   !(hostP->rh_fsFlags & REPH_FS_FLAG_DOING_SET_CONTEXT));

	LockMutex(&currentConnP->rhc_mutex);
	++currentConnP->rhc_refCount;
	UnlockMutex(&currentConnP->rhc_mutex);

	*callersConnPP = currentConnP;	/* Return the current connection */
	code = 0;
    } else {
	/*
	 * If we've reached this point, either: the caller's connection *does*
	 * match the current connection in the host structure; or, they don't
	 * match because the current connection in the host structure is
	 * NULL, meaning a previous reset attempt failed.  In either case,
	 * we try a reset now.
	 */
	if (currentConnP != NULL) {
	    reph_PutConn(currentConnP);	/* Lose the current connection */
	    *currentConnPP = NULL;
	}

	code = SetNewConn(hostP, currentConnPP, connType);
    
	/* Change caller's connection whether or not the reset failed */
	*callersConnPP = *currentConnPP;
    }

    UnlockMutex(mutexP);

    if (code != 0) {
	icl_Trace1(iclSetP, REP_TRACE_REPH_RESET_CONN_FAILED,
		   ICL_TYPE_LONG, code);
	code = REP_ERR_RESETFAILED;	/* Reset failed */
    }

    icl_Trace2(iclSetP, REP_TRACE_LEAVE_REPH_RESET_CONN,
	       ICL_TYPE_LONG, code, ICL_TYPE_POINTER, *callersConnPP);
    return code;
}	/* reph_ResetConn() */


void
reph_PutConn(rep_host_conn_t* connP)
{
    int			code;
    error_status_t	st;

    osi_assert(connP != NULL);

    icl_Trace1(iclSetP, REP_TRACE_REPH_PUT_CONN, ICL_TYPE_POINTER, connP);

    LockMutex(&connP->rhc_mutex);

    if (--connP->rhc_refCount > 0) {
	/* There are still more references */
	UnlockMutex(&connP->rhc_mutex);
	return;
    }

    /*
     * The last reference has been dropped--free the connection.
     */
    icl_Trace0(iclSetP, REP_TRACE_REPH_PUT_CONN_INTO_GRAVE);

    UnlockMutex(&connP->rhc_mutex);
    code = pthread_mutex_destroy(&connP->rhc_mutex);
    osi_assert(code == 0);

    rpc_binding_free(&connP->rhc_handle, &st);
    osi_assert(st == error_status_ok);

    osi_Free(connP, sizeof *connP);
}	/* reph_PutConn() */


void
reph_MergeSuccess(rep_host_t* hostP, time_t successTime)
{
    LockMutex(&hostP->rh_fsMutex);
    hostP->rh_fsLastSuccess = MAX(hostP->rh_fsLastSuccess, successTime);
    icl_Trace1(iclSetP, REP_TRACE_REPH_MERGE_SUCCESS,
	       ICL_TYPE_LONG, hostP->rh_fsLastSuccess);
    UnlockMutex(&hostP->rh_fsMutex);
}	/* reph_MergeSuccess() */

unsigned
reph_GetNumHosts(void)
{
    unsigned	n;

    lock_ObtainRead(&hostTableLock);
    n = numHosts;
    lock_ReleaseRead(&hostTableLock);

    return n;
}	/* reph_GetNumHosts() */


static long
SetNewConn(rep_host_t* hostP, rep_host_conn_t** connPP,
	   rep_host_conn_type_t connType)
{
    long		code;
    rep_host_conn_t*	connP = NULL;
    handle_t		handle;
    unsigned		i;
    long		ptCode;
    char		pNameBuf[MAXKPRINCIPALLEN];

    /*
     * The connection in the host structure should have never existed (if
     * called from reph_GetConn) or should have been thrown away (if called
     * from reph_ResetConn).
     */
    osi_assert(connPP != NULL && *connPP == NULL);

    icl_Trace4(iclSetP, REP_TRACE_REPH_SET_NEW_CONN,
	       ICL_TYPE_POINTER, hostP, ICL_TYPE_STRING, hostP->rh_name,
	       ICL_TYPE_POINTER, *connPP, ICL_TYPE_LONG, connType);

    /*
     * Before we do any real work, make sure that no one else is trying
     * to set this connection.  Normally, the fact that we hold the 
     * mutex for this connection would prevent races, but when 
     * establishing fileserver connections we are forced to unlock the mutex
     * in order to make the AFS_SetContext() RPC.
     */
    if (connType == REPH_CONN_TYPE_FS) {
	osi_assert(connPP == &hostP->rh_fsConnP);

	if (hostP->rh_fsFlags & REPH_FS_FLAG_DOING_SET_CONTEXT) {
	    struct timespec	wakeupTime;

	    /*
	     * Another thread is trying to set this fileserver connection--
	     * wait for it to complete, but don't let this thread get hungup
	     * forever.  If the other thread does not complete in
	     * REP_HOST_SET_CONTEXT_WAIT seconds, we return anyway.
	     */
	    wakeupTime.tv_sec = osi_Time() + REP_HOST_SET_CONTEXT_WAIT;
	    wakeupTime.tv_nsec = 0;

	    do {
		icl_Trace1(iclSetP, REP_TRACE_REPH_AWAITING_SET_CONTEXT,
			   ICL_TYPE_LONG, wakeupTime.tv_sec);
		hostP->rh_fsFlags |= REPH_FS_FLAG_AWAITING_SET_CONTEXT;
		errno = 0;
		ptCode = pthread_cond_timedwait(&hostP->rh_fsSetContextDone,
						&hostP->rh_fsMutex,
						&wakeupTime);
		osi_assert(ptCode != -1 || errno == EAGAIN);
	    } while (hostP->rh_fsFlags & REPH_FS_FLAG_DOING_SET_CONTEXT &&
		     ptCode != -1);

	    /*
	     * Either the other thread has completed its attempt to set this
	     * fileserver connection or we've watited as long as we're
	     * willing to.  In any case, we return the current connection,
	     * null or not.
	     */
	    connP = *connPP;		/* For convenience */
	    if (connP == NULL) {
		code = REP_ERR_PREVSETFAILED;
	    } else {
		LockMutex(&connP->rhc_mutex);
		++connP->rhc_refCount;	/* A reference for our caller */
		UnlockMutex(&connP->rhc_mutex);
		code = 0;
	    }
	    icl_Trace2(iclSetP, REP_TRACE_LEAVE_REPH_SET_NEW_CONN,
		       ICL_TYPE_LONG, code, ICL_TYPE_POINTER, *connPP);
	    return code;
	}
    } else {
	osi_assert(connPP == &hostP->rh_ftConnP);
    }

    /* Initialize the principal name */
    (void)sprintf(pNameBuf, "%s/%s/dfs-server", cellName, hostP->rh_name);

    /*
     * Try each of the host's addresses until we've either tried them all
     * or were successful.
     */
    for (i = 0; i < hostP->rh_numAddrs; i++) {
	error_status_t	st;

	/* Try to create a binding handle for this address */
	code = rpcx_binding_from_sockaddr(&hostP->rh_addrs[i], &handle);
	if (code != error_status_ok) {
	    icl_Trace2(iclSetP, REP_TRACE_REPH_TRY_ANOTHER_ADDRESS,
		       ICL_TYPE_LONG, 1, ICL_TYPE_LONG, code);
	    continue;			/* Try another address */
	}

	/* Try to initialize auth info on this binding handle */
	code = dfsauth_client_InitBindingAuth(&handle, 0, 1, pNameBuf);
	if (code != error_status_ok) {
	    rpc_binding_free(&handle, &st);
	    osi_assert(st == error_status_ok);
	    icl_Trace2(iclSetP, REP_TRACE_REPH_TRY_ANOTHER_ADDRESS,
		       ICL_TYPE_LONG, 2, ICL_TYPE_LONG, code);
	    continue;			/* Try another address */
	}

	/*
	 * There is nothing more to do if we're not connecting to a file
	 * server.
	 */
	if (connType != REPH_CONN_TYPE_FS)
	    break;

	/*
	 * We must be attempting to set up a connection to a file server.
	 */

	/* Try to set object uuid with the one we've been using for this host*/
	rpc_binding_set_object(handle, &hostP->rh_fsUUID, &st);
	if (code != error_status_ok) {
	    icl_Trace2(iclSetP, REP_TRACE_REPH_TRY_ANOTHER_ADDRESS,
		       ICL_TYPE_LONG, 3, ICL_TYPE_LONG, st);
	    rpc_binding_free(&handle, &st);
	    osi_assert(st == error_status_ok);
	    continue;			/* Try another address */
	}

	/* Try to set a file server context */
	code = SetContext(hostP, handle);

	if (code == rpc_s_auth_tkt_expired) {
	    /*
	     * Our login context expired--refresh and try again.
	     */
	    code = dfsauth_client_RefreshLocalAuthContext();
	    if (code == error_status_ok) {
		code = SetContext(hostP, handle);
	    }
	}

	if (code == 0)
	    break;			/* Success */

	/* Set context failed or local auth context cannot be refreshed */
	icl_Trace2(iclSetP, REP_TRACE_REPH_TRY_ANOTHER_ADDRESS,
		   ICL_TYPE_LONG, 4, ICL_TYPE_LONG, code);
	rpc_binding_free(&handle, &st);
	osi_assert(st == error_status_ok);

	/* Try another address */
    }

    if (code == 0) {
	/* Allocate a new connection structure */
	if ((connP = osi_Alloc(sizeof *connP)) != NULL) {
	    ptCode = pthread_mutex_init(&connP->rhc_mutex,
					pthread_mutexattr_default);
	    if (ptCode == 0) {
		connP->rhc_refCount = 2; /* 1 for hostP and 1 for caller */
		connP->rhc_type = connType;
		connP->rhc_handle = handle;
	    } else {
		osi_Free(connP, sizeof *connP);
		connP = NULL;
		code = REP_ERR_NOSTORAGE;
	    }
	} else {
	    code = REP_ERR_NOSTORAGE;
	}
    }

    *connPP = connP;			/* Reset connection in host struct */

    if (connType == REPH_CONN_TYPE_FS &&
	(hostP->rh_fsFlags & REPH_FS_FLAG_AWAITING_SET_CONTEXT)) {
	/*
	 * Other threads are waiting to set this connection--wake them up
	 * now.
	 */
	hostP->rh_fsFlags &= ~REPH_FS_FLAG_AWAITING_SET_CONTEXT;
	ptCode = pthread_cond_broadcast(&hostP->rh_fsSetContextDone);
	osi_assert(ptCode == 0);
    }

    icl_Trace2(iclSetP, REP_TRACE_LEAVE_REPH_SET_NEW_CONN,
	       ICL_TYPE_LONG, code, ICL_TYPE_POINTER, *connPP);
    return code;
}	/* SetNewConn() */


static long
SetContext(rep_host_t *hostP, handle_t handle)
{
    long	code;
    afsUUID	dummyUuid;
    time_t	successTime;

    icl_Trace3(iclSetP, REP_TRACE_REPH_SET_CONTEXT,
	       ICL_TYPE_POINTER, hostP, ICL_TYPE_STRING, hostP->rh_name,
	       ICL_TYPE_POINTER, handle);

    /*
     * We have to unlock the mutex before calling AFS_SetContext(),
     * because the fileserver will call us back (via TKN_InitTokenState)
     * before the set context call returns.  TKN_InitTokenState()
     * will want to write to the host structure with rh_fsMutex locked,
     * so we would have a deadlock if we didn't unlock the mutex.
     * Furthermore, we shouldn't be holding a mutex accross an
     * RPC anyway.
     *
     * We let any racing threads know that we're in the middle of setting
     * the connection by turning on REPH_FS_FLAG_DOING_SET_CONTEXT.  See
     * the beginning of SetNewConn() to see how they're handled.
     */
    hostP->rh_fsFlags |= REPH_FS_FLAG_DOING_SET_CONTEXT;
    UnlockMutex(&hostP->rh_fsMutex);
    
    successTime = osi_Time();		/* Conservatively record success time*/
    code = AFS_SetContext(handle, revokeEpoch, &revokeCallbackAddr, 0,
			  &dummyUuid, OSI_MAX_FILE_PARM_CLIENT, 0);
    code = osi_errDecode(code);
    
    LockMutex(&hostP->rh_fsMutex);
    hostP->rh_fsFlags &= ~REPH_FS_FLAG_DOING_SET_CONTEXT;
    
    if (code == 0)
	hostP->rh_fsLastSuccess = successTime;

    icl_Trace1(iclSetP, REP_TRACE_LEAVE_REPH_SET_CONTEXT, ICL_TYPE_LONG, code);

    return code;
}	/* SetContext() */


static void
LockMutex(pthread_mutex_t* mutexP)
{
    int	code;

    code = pthread_mutex_lock(mutexP);
    osi_assert(code == 0);
}	/* Lock() */


static void
UnlockMutex(pthread_mutex_t* mutexP)
{
    int	code;

    code = pthread_mutex_unlock(mutexP);
    osi_assert(code == 0);
}
