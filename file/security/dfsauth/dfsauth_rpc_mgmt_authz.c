/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfsauth_rpc_mgmt_authz.c,v $
 * Revision 1.1.711.1  1996/10/02  20:59:07  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:47:27  damon]
 *
 * Revision 1.1.706.3  1994/07/13  22:26:37  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  21:14:02  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:01:47  mbs]
 * 
 * Revision 1.1.706.2  1994/06/09  14:19:39  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:31:30  annie]
 * 
 * Revision 1.1.706.1  1994/02/04  20:29:45  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:17:55  devsrc]
 * 
 * Revision 1.1.704.1  1993/12/07  17:33:24  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  16:55:47  jaffe]
 * 
 * Revision 1.1.6.4  1993/01/21  15:19:30  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  15:57:53  cjd]
 * 
 * Revision 1.1.6.3  1992/11/24  18:32:53  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:24:25  bolinger]
 * 
 * Revision 1.1.6.2  1992/08/31  21:13:30  jaffe
 * 	Replace Missing RCS ids
 * 	[1992/08/31  15:48:50  jaffe]
 * 
 * 	Transarc delta: bab-ot4321-security-dfsauth-dce-error-trace 1.1
 * 	  Selected comments:
 * 	    dfsauth code that interacts with the DCE security service was
 * 	    not making appropriate use of DFSAUTH_DEBUG_BIT_DCE_ERRORS &
 * 	    DFSAUTH_DEBUG_BIT_DCE_TRACE.  This use was cleaned up to make it
 * 	    easier to set the dfsauth_debug_flag to an appropriate value to
 * 	    get more information out of the dfsauth package.
 * 	    ot 4321
 * 	    See above.
 * 	[1992/08/30  12:23:20  jaffe]
 * 
 * Revision 1.1.3.5  1992/04/01  22:53:23  mason
 * 	change required in security/dfsauth/dfsauth_rpc_mgmt_authz.c to return
 * 	appropriate value through the status parameter when the authorization
 * 	check fails.  the status parameter is set inappropriately due to a
 * 	misinterpretation of a misleading man page.
 * 	[1992/04/01  22:52:47  mason]
 * 
 * Revision 1.1.3.4  1992/01/29  22:33:25  carl
 * 	replacing proper version of this file, the previous one was an old
 * 	version which must be replaced (for the merge to osf1)
 * 	[1992/01/29  22:32:51  carl]
 * 
 * Revision 1.1.3.3  1992/01/24  03:48:33  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:19:18  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  20:23:43  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:39:35  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:21:48  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
 */
/*
 *	dfsauth_rpc_mgmt_authz.c -- routine to check authorization for execution
 *  of rpc_mgmt operations
 *
 *	Copyright (C) 1991 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/compat.h>		/* dfs_dceErrTxt */

#include <dce/uuid.h>
#include <dce/id_base.h>
#include <dce/rpc.h>
#include <dce/pgo.h>

#include <dfsauth.h>
#include <dfsauth_debug.h>
#include <dfsauth_trace.h>

#include <../dfssec_errorStrings.h>
#include <../exception_print.h>		/* and pthread stuff */

#include <dcedfs/icl.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/security/dfsauth/dfsauth_rpc_mgmt_authz.c,v 1.1.711.1 1996/10/02 20:59:07 damon Exp $")

IMPORT struct icl_set *dfsauth_iclSetp;
IMPORT long dfsauth_CheckServerNoAuth _TAKES((void));

/* these routines will not work at all in the kernel */

PRIVATE long dfsauth_get_local_host_and_cell_uuid(cellUuidP, hostUuidP)
     uuid_t *	cellUuidP;
     uuid_t *	hostUuidP;
{
  volatile long		rtnVal = 0;
  sec_rgy_name_t	localPrincipalName;
  volatile int		exceptionRaised = 0;
  volatile error_status_t	secRgyError;
  static char		routineName[] = "dfsauth_get_local_host_uuid";

  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_AUTHZ_TRACE_0 );

  rtnVal = dfsauth_PrinName_GetName((char *)NULL, (uuid_t *)NULL,
				    localPrincipalName, sizeof(localPrincipalName));
  if (rtnVal == 0) {
    TRY
      exceptionRaised = 0;
      sec_id_parse_name(sec_rgy_default_handle,
			localPrincipalName,
			NULL,
			cellUuidP,
			NULL,
			hostUuidP,
			(error_status_t *)&secRgyError);
    CATCH_ALL
      PrintException(THIS_CATCH, routineName, "sec_id_parse_name",
		     __FILE__, __LINE__);
      exceptionRaised = 1;
    ENDTRY;

    if ((secRgyError != error_status_ok) || exceptionRaised) {
      if (exceptionRaised) {
	dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		("%s: rpc_binding_inq_auth_client raised an exception; see stdout",
		 routineName));
	icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_AUTHZ_NONE_0 );
	rtnVal = DAUT_ERROR_SEC_RGY_PGO_ERROR;
      }
      else {
	dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		("%s: unable to get local principal uuid: %s",
		 routineName,
		 RgySec_ErrorString((error_status_t *)&secRgyError)));
	icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_AUTHZ_NONE_1,
		ICL_TYPE_STRING,
		RgySec_ErrorString((error_status_t *)&secRgyError));
	rtnVal = (long)secRgyError;
      }
    }
  }
  else {
    /* couldn't get local principal name */
    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_PRINCIPAL,
	     ("%s: unable to get local principal name: %s",
	      routineName,
	      RgySec_ErrorString((error_status_t *)&secRgyError)));
    icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_AUTHZ_PRINCIPAL_1,
	      ICL_TYPE_STRING,
	      RgySec_ErrorString((error_status_t *)&secRgyError));
  }
  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %s", routineName, dfs_dceErrTxt(rtnVal)));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_AUTHZ_TRACE_2 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));

  return rtnVal;
}

EXPORT boolean32 dfsauth_rpc_mgmt_authz_check(client_binding, requested_mgmt_operation,
					      statusP)
     IN  rpc_binding_handle_t	client_binding;
     IN  unsigned32		requested_mgmt_operation;
     OUT unsigned32 *		statusP;
{
  volatile boolean32	rtnVal = false;
  volatile uuid_t		localCellUuid;
  volatile uuid_t	localHostPrincipalUuid;
  uuid_t * volatile	clientCellUuidP = (uuid_t *)NULL;
  uuid_t * volatile	clientPrincipalUuidP = (uuid_t *)NULL;
  rpc_authz_handle_t	clientId;
  sec_id_pac_t *	clientDCEIdP;
  unsigned32		authnLevel;
  unsigned32		authnSvc;
  unsigned32		authzSvc;
  volatile error_status_t	extractError;
  volatile unsigned32	uuidError;
  volatile int		exceptionRaised = 0;
  long			localHostRtn;
  static char		routineName[] = "dfsauth_rpc_mgmt_authz_check";
  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_AUTHZ_TRACE_3 );
  if (requested_mgmt_operation == rpc_c_mgmt_inq_if_ids) {
    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	     ("%s: requested operation is %s",
	      routineName, "rpc_mgmt_inq_if_ids"));
    icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_AUTHZ_TRACE_4 , ICL_TYPE_STRING, "rpc_mgmt_inq_if_ids");
  } else if (requested_mgmt_operation == rpc_c_mgmt_inq_princ_name) {
    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	     ("%s: requested operation is %s",
	      routineName, "rpc_mgmt_inq_server_princ_name"));
    icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_AUTHZ_TRACE_5 , ICL_TYPE_STRING, "rpc_mgmt_inq_server_princ_name");
  } else if (requested_mgmt_operation == rpc_c_mgmt_inq_stats) {
    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	     ("%s: requested operation is %s",
	      routineName, "rpc_mgmt_inq_stats"));
    icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_AUTHZ_TRACE_6 , ICL_TYPE_STRING, "rpc_mgmt_inq_stats");
  } else if (requested_mgmt_operation == rpc_c_mgmt_is_server_listen) {
    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	     ("%s: requested operation is %s",
	      routineName, "rpc_mgmt_is_server_listening"));
    icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_AUTHZ_TRACE_7 , ICL_TYPE_STRING, "rpc_mgmt_is_server_listening");
  } else if (requested_mgmt_operation == rpc_c_mgmt_stop_server_listen) {
    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	     ("%s: requested operation is %s",
	      routineName, "rpc_mgmt_stop_server_listening"));
    icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_AUTHZ_TRACE_8 , ICL_TYPE_STRING, "rpc_mgmt_stop_server_listening");
  }
  
  if (requested_mgmt_operation == rpc_c_mgmt_stop_server_listen) {
#if defined(DFSAUTH_RPC_USE_ADMIN_LIST)
    if (dfsauth_server_CheckAuthorization(&client_binding, (dacl_permset_t *)NULL,
					  (char *)NULL, (uuid_t *)NULL) == 0) {
      rtnVal = true;
    }
#else /* defined(DFSAUTH_RPC_USE_ADMIN_LIST) */
    localHostRtn = dfsauth_get_local_host_and_cell_uuid(&localCellUuid,
							&localHostPrincipalUuid);

    if (localHostRtn == 0) {
      /* extract the user uuid from the binding */
      TRY
	exceptionRaised = 0;
        rpc_binding_inq_auth_client(client_binding,
				    &clientId,
				    (unsigned_char_p_t *)NULL,
				    &authnLevel,
				    &authnSvc,
				    &authzSvc,
				    (error_status_t *)&extractError);
      CATCH_ALL
	PrintException(THIS_CATCH, routineName, "rpc_binding_inq_auth_client",
		       __FILE__, __LINE__);
        exceptionRaised = 1;
      ENDTRY;
      
      if ((extractError == error_status_ok) && !exceptionRaised) {
	if (authzSvc == rpc_c_authz_none) {		/* -noauth */
	  if (dfsauth_CheckServerNoAuth() == 0) {
	    rtnVal = true;
	  }
	}
	else if (authzSvc == rpc_c_authz_name) {
	  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
		   ("%s: found name authzn; name = %s",
		    routineName, (unsigned_char_t *)clientId));
	  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_AUTHZ_WARNINGS_9 , ICL_TYPE_STRING, (unsigned_char_t*)clientId);
	  if (dfsauth_CheckServerNoAuth() == 0) {
	    rtnVal = true;
	  }
	}
	else if (authzSvc == rpc_c_authz_dce) {
	  clientDCEIdP = (sec_id_pac_t *)clientId;
	  if (clientDCEIdP->authenticated == true) {  
	    if (authnLevel == rpc_c_authn_level_none) {	/* try for noauth */
	      if (dfsauth_CheckServerNoAuth() == 0) {
		rtnVal = true;
	      }
	    }
	    else {
	      clientCellUuidP = &(clientDCEIdP->realm.uuid);
	      clientPrincipalUuidP = &(clientDCEIdP->principal.uuid);
	    }
	  }
	}
      }
      else {
	if (exceptionRaised) {
	  dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		  ("%s: rpc_binding_inq_auth_client raised an exception; see stdout",
		   routineName));
	  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_AUTHZ_NONE_2 );
	}
	else {
	  if (extractError == rpc_s_binding_has_no_auth) {
	    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
		     ("%s: binding has no auth info, checking noauth", routineName));
	    icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_AUTHZ_WARNINGS_10 );
	    if (dfsauth_CheckServerNoAuth() == 0) {
	      rtnVal = true;
	    }
	  }
	  else {
	    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
		     ("%s: rpc_binding_inq_auth_client error: %s",
		      routineName,
		      RgySec_ErrorString((error_status_t *)&extractError)));
	    icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_AUTHZ_WARNINGS_11,
		     ICL_TYPE_STRING,
		     RgySec_ErrorString((error_status_t *)&extractError));
	  }
	}
      }

      if (clientPrincipalUuidP != (uuid_t *)NULL) {
	/*
	 * Note that uuidError may change during the left-to-right evaluation of
	 * the following condition.
	 */
	if (((uuid_equal((uuid_t *)&localCellUuid, (uuid_t *)clientCellUuidP,
			 (error_status_t *)&uuidError) == true) &&
	     (uuidError == uuid_s_ok)) &&
	    ((uuid_equal((uuid_t *)&localHostPrincipalUuid, 
		(uuid_t *)clientPrincipalUuidP, 
		(error_status_t *)&uuidError) == true) &&
	     (uuidError == uuid_s_ok))) {
	  rtnVal = true;
	}
      }
      else {
	dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
		 ("%s: Error: unable to get client principal uuid", routineName));
	icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_AUTHZ_WARNINGS_12 );
      }
    }
    else {
      dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
	       ("%s: Error: unable to get local host principal uuid: %s",
		routineName, dfs_dceErrTxt(localHostRtn)));
      icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_AUTHZ_WARNINGS_13 , ICL_TYPE_STRING, dfs_dceErrTxt(localHostRtn));
    }
    
#endif /* defined(USE_ADMIN_LIST) */
  }
  else {
    /* for everything else, we will take the default */
    rtnVal = true;
  }

  *statusP = ((rtnVal == true) ? error_status_ok : rpc_s_mgmt_op_disallowed);
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %s", routineName, (rtnVal == true) ? "true": "false"));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_AUTHZ_TRACE_14,
	     ICL_TYPE_STRING, ((rtnVal==true) ? "true" : "false"));

  return rtnVal;
}
