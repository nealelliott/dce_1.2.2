/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: compat_serverdup.c,v $
 * Revision 1.1.18.1  1996/10/02  17:54:42  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:42:21  damon]
 *
 * $EndLog$
 */
/*
 *	compat.c -- routines for killing off duplicate servers
 *		    and for unregistering self
 *
 * Copyright (C) 1991, 1995 Transarc Corporation
 * All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>

#include <dce/rpc.h>
#include <dce/dce_error.h>

#include <pthread.h>

#include <compat.h>
#include <compat_debug.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsncsmac.h>
#include <dfsncssvc.h>
#include <dfsncsmsg.h>

extern int  compat_svc_must_init;
extern void compat_svc_init();

#define INIT_SVC if (compat_svc_must_init) compat_svc_init()

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/ncscompat/compat_serverdup.c,v 1.1.18.1 1996/10/02 17:54:42 damon Exp $")
 
IMPORT uuid_t uuid_nil;
     
/*
 * the dfsauth routines we will use (we don't use dfsauth.h, since it depends
 * on compat)
 */
IMPORT boolean32 dfsauth_rpc_mgmt_authz_check _TAKES((
				      IN  rpc_binding_handle_t	client_binding,
				      IN  unsigned32 requested_mgmt_operation,
				      OUT unsigned32 *statusP
						      ));
IMPORT long dfsauth_client_InitAuthentication _TAKES((
			   IN rpc_binding_handle_t *serverBindingHandleP,
			   IN char *		serverLocationP,
			   IN int		useNoAuth,
			   IN int		useLocalAuth
					    ));
IMPORT long dfsauth_client_CleanupAuthentication _TAKES((void));


IMPORT long compat_ShutdownDuplicateServer _TAKES((
					       rpc_if_handle_t	if_handle,
					       uuid_t *	serverInstanceUuidP,
					       int	retainLocalAuthContext
						     ));
IMPORT long compat_UnregisterServer _TAKES((
					    rpc_if_handle_t	if_handle,
					    uuid_t *	serverInstanceUuidP
					  ));
IMPORT int compat_DetectDuplicateServer _TAKES((
			       rpc_if_handle_t	if_handle,
			       uuid_t *	serverInstanceUuidP,
			       rpc_binding_handle_t *returnBinding,
			       error_status_t *errorStatus
				     ));

static void osi_dp_unexpected_error();
 
EXPORT long compat_ShutdownDuplicateServer(if_handle, serverInstanceUuidP,
					   retainLocalAuthContext)
     rpc_if_handle_t	if_handle;
     uuid_t *		serverInstanceUuidP;
     int		retainLocalAuthContext;
{
  long				rtnVal = 0;
  long				dummyRtnVal = 0;
  char				calleeName[1024];
  rpc_binding_vector_t *	bvec;
  error_status_t		ncs_error_status;
  int				i;
  unsigned_char_t *		stringBinding;
  rpc_ep_inq_handle_t		epInqContext;
  rpc_if_id_t			ifid;
  rpc_binding_handle_t		otherServerBinding;
  struct timespec		delay;
  int				savedErrno;
  char				errorBuffer[dce_c_error_string_len];
  static char			routineName[] = "compat_ShutdownDuplicateServer";

  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_TRACE,
	   ("%s entered", routineName));

  if (serverInstanceUuidP == (uuid_t *)NULL) {
    serverInstanceUuidP = &uuid_nil;
  }
  
  /* establish routine to check who can do revocations */
  (void)strcpy(calleeName, "rpc_mgmt_set_authorization_fn");
  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
	   ("%s: about to call %s", routineName, calleeName));
  rpc_mgmt_set_authorization_fn(dfsauth_rpc_mgmt_authz_check, &ncs_error_status);
  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
	   ("%s: returned from %s", routineName, calleeName));
  if (ncs_error_status == rpc_s_ok) {
    /* init the interface id */
    (void)strcpy(calleeName, "rpc_if_inq_id");
    dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
	     ("%s: about to call %s", routineName, calleeName));
    rpc_if_inq_id(if_handle, &ifid, &ncs_error_status);
    dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
	     ("%s: returned from call to %s", routineName, calleeName));
    if (ncs_error_status == rpc_s_ok) {
      (void)strcpy(calleeName, "rpc_mgmt_ep_elt_inq_begin");
      dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
	       ("%s: about to call %s", routineName, calleeName));
      rpc_mgmt_ep_elt_inq_begin((rpc_binding_handle_t)NULL,
				rpc_c_ep_match_by_both,
				&ifid,
				rpc_c_vers_exact,
				serverInstanceUuidP,
				&epInqContext,
				&ncs_error_status);
      dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
	       ("%s: returned from call to %s", routineName, calleeName));
      
      if (ncs_error_status == rpc_s_ok) {
	(void)strcpy(calleeName, "rpc_mgmt_ep_elt_inq_next");
	dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
		 ("%s: about to call %s", routineName, calleeName));
	rpc_mgmt_ep_elt_inq_next(epInqContext,
				 &ifid,	/* we can overwrite this */
				 &otherServerBinding,
				 (uuid_t *)NULL,
				 (unsigned_char_t **)NULL,
				 &ncs_error_status);
	dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
		 ("%s: returned from call to %s", routineName, calleeName));
	while (ncs_error_status == rpc_s_ok) {
	  /*
	   * there is already a server operating with this binding
	   */
	  (void)strcpy(calleeName, "dfsauth_client_InitAuthentication");
	  if ((rtnVal = dfsauth_client_InitAuthentication(&otherServerBinding,
							  (char *)NULL,
							  0 /* => not noauth */,
							  1 /* => use localauth */)) == 0) {
	    /* if we do have a listening server, we have to kill it */
	    (void)strcpy(calleeName, "rpc_mgmt_stop_server_listening");
	    dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
		     ("%s: about to call %s", routineName, calleeName));
	    rpc_mgmt_stop_server_listening(otherServerBinding, &ncs_error_status);
	    dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
		     ("%s: returned from call to %s", routineName, calleeName));
	      
	    if (ncs_error_status == rpc_s_ok) {
	      /* give the exiting process a chance to exit (hopefully) */
	      delay.tv_sec = 3;
	      delay.tv_nsec = 0;
	      (void)strcpy(calleeName, "pthread_delay_np");
	      if (pthread_delay_np(&delay) == -1) {
		savedErrno = errno;
		osi_dp_unexpected_error(routineName, calleeName,
					strerror (savedErrno));
	      }
	    }
	    else {
	      /* rpc_mgmt_stop_server_listening */
	      dfs_copyDceErrTxt(ncs_error_status, errorBuffer, sizeof(errorBuffer));
	      osi_dp_unexpected_error(routineName, calleeName, errorBuffer);

	      rtnVal = ncs_error_status; /* propagate the failure to the caller */
	    }
	      
	    if (!retainLocalAuthContext) {
	      (void)strcpy(calleeName, "dfsauth_client_CleanupAuthentication");
	      dummyRtnVal = dfsauth_client_CleanupAuthentication();
	      if (dummyRtnVal) {
		/* dfsauth_client_CleanupAuthentication */
		osi_dp_unexpected_error(routineName, calleeName,
					dfs_dceErrTxt(dummyRtnVal));
		if (!rtnVal) {
		  /* don't cover previously flagged errors with this one */
		  rtnVal = dummyRtnVal;
		}
	      }
	    }
	  }
	  else {
	    /* dfsauth_client_InitAuthentication */
	    osi_dp_unexpected_error(routineName, calleeName,
				    dfs_dceErrTxt(rtnVal));
	  }

	  /* free the returned data */
	  (void)strcpy(calleeName, "rpc_binding_free");
	  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
		   ("%s: about to call %s", routineName, calleeName));
	  rpc_binding_free(&otherServerBinding, &ncs_error_status);
	  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
		   ("%s: returned from call to %s", routineName, calleeName));
	  if (ncs_error_status != rpc_s_ok) {
	    /* rpc_binding_free */
	    dfs_copyDceErrTxt(ncs_error_status, errorBuffer, sizeof(errorBuffer));
	    osi_dp_unexpected_error(routineName, calleeName, errorBuffer);
	  }
	  
	  (void)strcpy(calleeName, "rpc_mgmt_ep_elt_inq_next");
	  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
		   ("%s: about to call %s", routineName, calleeName));
	  rpc_mgmt_ep_elt_inq_next(epInqContext,
				   &ifid, /* we really don't care about this either */
				   &otherServerBinding,
				   (uuid_t *)NULL,
				   (unsigned_char_t **)NULL,
				   &ncs_error_status);
	  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
		   ("%s: returned from call to %s", routineName, calleeName));
	}
	if (ncs_error_status != rpc_s_no_more_elements) {
	  /* rpc_mgmt_ep_elt_inq_next */
	  dfs_copyDceErrTxt(ncs_error_status, errorBuffer, sizeof(errorBuffer));
	  osi_dp_unexpected_error(routineName, calleeName, errorBuffer);
	}
	
	(void)strcpy(calleeName, "rpc_mgmt_ep_elt_inq_done");
	dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
		 ("%s: about to call %s", routineName, calleeName));
	rpc_mgmt_ep_elt_inq_done(&epInqContext, &ncs_error_status);
	dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
		 ("%s: returned from call to %s", routineName, calleeName));
	if (ncs_error_status != rpc_s_ok) {
	  /* rpc_mgmt_ep_elt_inq_done */
	  dfs_copyDceErrTxt(ncs_error_status, errorBuffer, sizeof(errorBuffer));
	  osi_dp_unexpected_error(routineName, calleeName, errorBuffer);
	}
      }
      else {
	/* rpc_mgmt_ep_elt_inq_begin */
	dfs_copyDceErrTxt(ncs_error_status, errorBuffer, sizeof(errorBuffer));
	osi_dp_unexpected_error(routineName, calleeName, errorBuffer);
      }
    }
    else {
      /* rpc_if_inq_id */
      dfs_copyDceErrTxt(ncs_error_status, errorBuffer, sizeof(errorBuffer));
      osi_dp_unexpected_error(routineName, calleeName, errorBuffer);
    }
  }
  else {
    /* rpc_mgmt_set_authorization_fn */
    dfs_copyDceErrTxt(ncs_error_status, errorBuffer, sizeof(errorBuffer));
    osi_dp_unexpected_error(routineName, calleeName, errorBuffer);
    rtnVal = ncs_error_status;
  }
  
  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_TRACE,
	   ("%s exiting; returning %ld", routineName, rtnVal));
  return rtnVal;
}

EXPORT long compat_UnregisterServer(if_handle, serverInstanceUuidP)
     rpc_if_handle_t	if_handle;
     uuid_t *		serverInstanceUuidP;
{
  long				rtnVal = 0;
  rpc_binding_vector_t *	bvec;
  uuid_vector_t			serverObjUuidVector;
  error_status_t		ncs_error_status;
  char				calleeName[1024];
  char				errorBuffer[dce_c_error_string_len];
  static char			routineName[] = "compat_UnregisterServer";
  
  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_TRACE,
	   ("%s entered", routineName));

  if (serverInstanceUuidP == (uuid_t *)NULL) {
    serverInstanceUuidP = &uuid_nil;
  }

  /* get the vector of binding handles to unregister */
  (void)strcpy(calleeName, "rpc_server_inq_bindings");
  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
	   ("%s: about to call %s", routineName, calleeName));
  rpc_server_inq_bindings(&bvec, &ncs_error_status);
  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
	   ("%s: returned from %s", routineName, calleeName));
  if (ncs_error_status == rpc_s_ok)  {
    serverObjUuidVector.count = 1;
    serverObjUuidVector.uuid[0] = serverInstanceUuidP;
    
    (void)strcpy(calleeName, "rpc_ep_unregister");
    dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
	     ("%s: about to call %s", routineName, calleeName));
    rpc_ep_unregister(if_handle,
		      bvec,
		      &serverObjUuidVector,
		      &ncs_error_status);
    dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
	     ("%s: returned from %s", routineName, calleeName));
    if (ncs_error_status != rpc_s_ok) {
      /* rpc_ep_unregister */
      dfs_copyDceErrTxt(ncs_error_status, errorBuffer, sizeof(errorBuffer));
      osi_dp_unexpected_error(routineName, calleeName, errorBuffer);
      rtnVal = ncs_error_status;
    }

    (void)strcpy(calleeName, "rpc_binding_vector_free");
    dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
	     ("%s: about to call %s", routineName, calleeName));
    rpc_binding_vector_free(&bvec, &ncs_error_status);
    dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
	     ("%s: returned from %s", routineName, calleeName));
    if (ncs_error_status != rpc_s_ok) {
      /* rpc_binding_vector_free */
      dfs_copyDceErrTxt(ncs_error_status, errorBuffer, sizeof(errorBuffer));
      osi_dp_unexpected_error(routineName, calleeName, errorBuffer);
    }
  }
  else {
    /* rpc_server_inq_bindings */
    dfs_copyDceErrTxt(ncs_error_status, errorBuffer, sizeof(errorBuffer));
    osi_dp_unexpected_error(routineName, calleeName, errorBuffer);
    return 1;
  }

  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_TRACE,
	   ("%s exiting; returning %ld", routineName, rtnVal));

  return rtnVal;
}
 
/*
 * compat_DetectDuplicateServer
 *	procedure that determines if a particular server is already running
 * on a server, and if so returns a boolean true, and if so desired, also 
 * returns a binding handle to the already running server. This routine would
 * identify only the first duplicate server.
 *
 * if returnBinding is non-NULL, then the duplicate server binding is 
 * returned. This binding has to be freed by the caller using the rpc_binding_
 * free routine.
 */
EXPORT compat_DetectDuplicateServer(if_handle, serverInstanceUuidP,
					 returnBinding, errorStatus)
     rpc_if_handle_t	if_handle;
     uuid_t *		serverInstanceUuidP;
     rpc_binding_handle_t *returnBinding;
     error_status_t 	*errorStatus;
{
  char				calleeName[1024];
  error_status_t		ncs_error_status;
  int				i;
  rpc_ep_inq_handle_t		epInqContext;
  rpc_if_id_t			ifid;
  rpc_binding_handle_t		otherServerBinding;
  char				errorBuffer[dce_c_error_string_len];
  int				foundDup=0;
  static char			*routineName="compat_DetectDuplicateServer";

  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_TRACE,
	   ("%s entered", routineName));
  *errorStatus = 0;
  if (serverInstanceUuidP == (uuid_t *)NULL) {
    serverInstanceUuidP = &uuid_nil;
  }
  
  /* establish routine to check who can do revocations */
  (void)strcpy(calleeName, "rpc_mgmt_set_authorization_fn");
  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
	   ("%s: about to call %s", routineName, calleeName));
  rpc_mgmt_set_authorization_fn(dfsauth_rpc_mgmt_authz_check,
				&ncs_error_status);
  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
	   ("%s: returned from %s", routineName, calleeName));
  if (ncs_error_status == rpc_s_ok) {
    /* init the interface id */
    (void)strcpy(calleeName, "rpc_if_inq_id");
    dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
	     ("%s: about to call %s", routineName, calleeName));
    rpc_if_inq_id(if_handle, &ifid, &ncs_error_status);
    dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
	     ("%s: returned from call to %s", routineName, calleeName));
    if (ncs_error_status == rpc_s_ok) {
      (void)strcpy(calleeName, "rpc_mgmt_ep_elt_inq_begin");
      dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
	       ("%s: about to call %s", routineName, calleeName));
      rpc_mgmt_ep_elt_inq_begin((rpc_binding_handle_t)NULL,
				rpc_c_ep_match_by_both,
				&ifid,
				rpc_c_vers_exact,
				serverInstanceUuidP,
				&epInqContext,
				&ncs_error_status);
      dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
	       ("%s: returned from call to %s", routineName, calleeName));
      
      if (ncs_error_status == rpc_s_ok) {
	(void)strcpy(calleeName, "rpc_mgmt_ep_elt_inq_next");
	dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
		 ("%s: about to call %s", routineName, calleeName));
	rpc_mgmt_ep_elt_inq_next(epInqContext,
				 &ifid,
				 &otherServerBinding,
				 (uuid_t *)NULL,
				 (unsigned_char_t **)NULL,
				 &ncs_error_status);
	dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
		 ("%s: returned from call to %s", routineName, calleeName));
	while (ncs_error_status == rpc_s_ok) {
	  /*
	   * determine whether or not there is already a server operating with
	   * this binding
	   */
	  (void)strcpy(calleeName, "rpc_mgmt_is_server_listening");
	  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
		   ("%s: about to call %s", routineName, calleeName));
	  if ((rpc_mgmt_is_server_listening(otherServerBinding, 
					    &ncs_error_status) == true) &&
	      (ncs_error_status == rpc_s_ok)) {
	    dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
		     ("%s: returned from call to %s", 
		      routineName, calleeName));
	    break;
	  }
	  else {
	    dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
		     ("%s: returned from call to %s", routineName,calleeName));
	    /* rpc_mgmt_is_server_listening */
#if 0
	    if (ncs_error_status != rpc_s_ok) {
	      dfs_copyDceErrTxt(ncs_error_status, errorBuffer, 
				sizeof(errorBuffer));
	      osi_dp_unexpected_error(routineName, calleeName, errorBuffer);
	    }
#endif
	  }
	  
	  /* free the returned data */
	  (void)strcpy(calleeName, "rpc_binding_free");
	  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
		   ("%s: about to call %s", routineName, calleeName));
	  rpc_binding_free(&otherServerBinding, &ncs_error_status);
	  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
		   ("%s: returned from call to %s", routineName, calleeName));
	  if (ncs_error_status != rpc_s_ok) {
	    /* rpc_binding_free */
	    dfs_copyDceErrTxt(ncs_error_status, errorBuffer,
			      sizeof(errorBuffer));
	    osi_dp_unexpected_error(routineName, calleeName, errorBuffer);
	  }
	  
	  (void)strcpy(calleeName, "rpc_mgmt_ep_elt_inq_next");
	  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
		   ("%s: about to call %s", routineName, calleeName));
	  rpc_mgmt_ep_elt_inq_next(epInqContext,
				   &ifid,
				   &otherServerBinding,
				   (uuid_t *)NULL,
				   (unsigned_char_t **)NULL,
				   &ncs_error_status);
	  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
		   ("%s: returned from call to %s", routineName, calleeName));
	}
	if (ncs_error_status == rpc_s_ok) {
	  /* found duplicate server */
	  foundDup = 1;
	  if (returnBinding)
	    *returnBinding = otherServerBinding;
	  else {
	    /* free the returned data */
	    (void)strcpy(calleeName, "rpc_binding_free");
	    dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
		     ("%s: about to call %s", routineName, calleeName));
	    rpc_binding_free(&otherServerBinding, &ncs_error_status);
	    dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
		     ("%s: returned from call to %s", routineName,calleeName));
	    if (ncs_error_status != rpc_s_ok) {
	      /* rpc_binding_free */
	      dfs_copyDceErrTxt(ncs_error_status, errorBuffer,
				sizeof(errorBuffer));
	      osi_dp_unexpected_error(routineName, calleeName, errorBuffer);
	    }
	  }
	}
	else if (ncs_error_status != rpc_s_no_more_elements) {
	  /* rpc_mgmt_ep_elt_inq_next */
	  dfs_copyDceErrTxt(ncs_error_status, errorBuffer,sizeof(errorBuffer));
	  osi_dp_unexpected_error(routineName, calleeName, errorBuffer);
	  *errorStatus = ncs_error_status;
	}
	
	(void)strcpy(calleeName, "rpc_mgmt_ep_elt_inq_done");
	dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
		 ("%s: about to call %s", routineName, calleeName));
	rpc_mgmt_ep_elt_inq_done(&epInqContext, &ncs_error_status);
	dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_DCE_TRACE,
		 ("%s: returned from call to %s", routineName, calleeName));
	if (ncs_error_status != rpc_s_ok) {
	  /* rpc_mgmt_ep_elt_inq_done */
	  dfs_copyDceErrTxt(ncs_error_status, errorBuffer,sizeof(errorBuffer));
	  osi_dp_unexpected_error(routineName, calleeName, errorBuffer);
	  *errorStatus = ncs_error_status;
	}
      }
      else {
	/* rpc_mgmt_ep_elt_inq_begin */
	dfs_copyDceErrTxt(ncs_error_status, errorBuffer, sizeof(errorBuffer));
	osi_dp_unexpected_error( routineName, calleeName, errorBuffer);
	*errorStatus = ncs_error_status;
      }
    }
    else {
      /* rpc_if_inq_id */
      dfs_copyDceErrTxt(ncs_error_status, errorBuffer, sizeof(errorBuffer));
      osi_dp_unexpected_error( routineName, calleeName, errorBuffer);
      *errorStatus = ncs_error_status;
    }
  }
  else {
    /* rpc_mgmt_set_authorization_fn */
    dfs_copyDceErrTxt(ncs_error_status, errorBuffer, sizeof(errorBuffer));
    osi_dp_unexpected_error( routineName, calleeName, errorBuffer);
    *errorStatus = ncs_error_status;
  }
  
  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_TRACE,
	   ("%s exiting; returning %ld", routineName, *errorStatus));
  return foundDup;
}



/* 
  There are no servicability calls in the kernel. If you look in osi.h, you'll
  see that outside the kernel, osi_dp is replaced with a "printf".
*/

static void osi_dp_unexpected_error(routine, callee, err)

  char                 *routine,
                       *callee,
                       *err;

{
#ifdef KERNEL

  osi_dp("%s: unexpected error from %s: %s\n", 
	 routine,
	 callee,
	 err);

#else
  
  INIT_SVC;
  
  dce_svc_printf(NCS_S_CS_UNEXPECTED_ERROR_MSG, routine,
	 callee,
	 err);

#endif /* KERNEL */
} 
