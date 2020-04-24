/*
 * @OSF_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: rep_host.h,v $
 * Revision 1.1.5.1  1996/10/02  18:14:08  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:46:00  damon]
 *
 * $EndLog$
 */

#ifndef	_REP_HOST_H_
#define	_REP_HOST_H_

/*
 * (C) Copyright 1994 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 *
 * $Header: /u0/rcs_trees/dce/rcs/file/rep/rep_host.h,v 1.1.5.1 1996/10/02 18:14:08 damon Exp $
 */

#include <dce/rpc.h>
#include <dcedfs/common_data.h>		/* afsUUID */
#include <dcedfs/fldb_data.h>		/* ADDRSINSITE & MAXKPRINCIPALLEN */
#include <dcedfs/icl.h>

#include <pthread.h>

/* Connection types */
typedef enum rep_host_conn_type {
    REPH_CONN_TYPE_FS,			/* Fileserver connection type */
    REPH_CONN_TYPE_FT			/* Ftserver connection type */
}	rep_host_conn_type_t;

/* A server connection */
typedef struct rep_host_conn {
    /* The referenced count is protected by rhc_mutex */
    pthread_mutex_t		rhc_mutex;
    int				rhc_refCount;

    /*
     * These members are not really protected by any lock, but they are
     * guaranteed not to change as long as there is a reference to this
     * structure.
     */
    rep_host_conn_type_t	rhc_type;
    handle_t			rhc_handle;
}	rep_host_conn_t;

/* A host--caches server connections */
typedef struct rep_host {
    /* The following members are protected by rep_host.c:repTableLock */
    struct rep_host	*rh_next;
    unsigned		rh_refCount;

    /*
     * The following members are not really protected by any lock, but
     * they are guaranteed not to change as long as there is a reference to
     * this structure.  Note that the host table itself holds a reference.
     */
    struct afsNetAddr	rh_addrs[ADDRSINSITE];
    unsigned		rh_numAddrs;
    char		rh_name[MAXKPRINCIPALLEN];

    /* The following members are protected by rh_fsMutex */
    pthread_mutex_t	rh_fsMutex;
    unsigned		rh_fsFlags;
    pthread_cond_t	rh_fsSetContextDone;
    rep_host_conn_t*	rh_fsConnP;
    time_t		rh_fsLastSuccess;
    unsigned		rh_fsHostLifeGuarantee;
    unsigned		rh_fsHostLeadTime;
    afsUUID 		rh_fsUUID;
    
    /* The following members are protected by rh_ftMutex */
    pthread_mutex_t	rh_ftMutex;
    rep_host_conn_t*	rh_ftConnP;
}	rep_host_t;

/* Flag bits for rh_fsFlags */
#define REPH_FS_FLAG_DOING_SET_CONTEXT		0x1
#define REPH_FS_FLAG_AWAITING_SET_CONTEXT	0x2


/*
 * reph_Init -- Initialize the rep host subsystem.
 */
extern void reph_Init(const char* cellName,
		      unsigned32 revokeEpoch,
		      const afsNetData* revokeCallbackAddr,
		      struct icl_set*);

/*
 * reph_GetHost -- Get a host structure for the host with the given
 *	addresses.  This function may return NULL if there is no memory
 *	available for the new host structure.
 */
extern rep_host_t* reph_GetHost(struct afsNetAddr* addrs,
				unsigned numAddrs,
				char* hostName);

/*
 * reph_PutHost -- Drop a reference to a host that was returned by
 *	reph_GetHost().
 */
extern void reph_PutHost(rep_host_t* hostP);

/*
 * reph_GetConn -- Get a connection to the given host.  The type argument
 *	specifies the type of server to get a connection to.  The current
 *	choices are fileserver and ftserver connections.  If an error
 *	occurs while trying to set up a connection, NULL will be returned
 *	in *connPP and the return code will be set to a non-zero value.
 *	After receiving a non-NULL connection, the caller should make RPCs
 *	using the rhc_handle member.  When finished with the connection, the
 *	caller should release it by calling reph_PutConn().
 */
extern long reph_GetConn(rep_host_t* hostP, rep_host_conn_type_t connType,
			 rep_host_conn_t** connPP);

/*
 * reph_ResetConn -- If an error occurs while using a connection, the user
 *	can attempt to reset the connection with this routine.
 *
 *	Summary of return values:
 *	0: connError indicates a condition which may be correctable with
 *		a reset, and the reset succeeded.  The user can retry the
 *		operation that failed.  When the user is finished with
 *		the connection, they should release it by calling
 *		reph_PutConn().
 *	REP_ERR_RESETFAILED: connError indicates a condition which
 *		may be correctable with a reset, but the reset failed.
 *		*connPP may be set to NULL; if so, don't call reph_PutConn on
 *		it.  Currently, the only case where REP_ERR_RESETFAILED is
 *		returned without setting *connPP to NULL is when connError
 *		is rpc_s_auth_tkt_expired and the attempt to refresh the
 *		local auth context failed.
 *	connError: connError indicates a condition which is not correctable
 *		by resetting the connection, so that same error is returned.
 *		In this case, it is still necessary to release the connection
 *		by calling reph_PutConn().
 */
extern long reph_ResetConn(rep_host_t* hostP,
			   rep_host_conn_t** connPP,
			   long connError);

/*
 * reph_PutConn -- Drop a reference to a host that was returned by
 *	reph_GetConn().
 */
extern void reph_PutConn(rep_host_conn_t* connP);

/*
 * reph_MergeSuccess() -- After a successful call to a fileserver, its
 *	"last successful call" time should be updated with this routine.
 *	The specified time is only used if the time that is already recorded
 *	for the last success is less recent.
 */
extern void reph_MergeSuccess(rep_host_t*, time_t);

/*
 * reph_GetNumHosts() -- Return the number of hosts contacted so far.
 */
extern unsigned reph_GetNumHosts(void);


#endif	/* _REP_HOST_H_ */
