/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfsauth_server.c,v $
 * Revision 1.1.813.1  1996/10/02  20:59:12  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:47:30  damon]
 *
 * $EndLog$
 */
/*
 *	dfsauth_server.c -- server-side routines for the DFS authentication package
 *
 * Copyright (C) 1996, 1991 Transarc Corporation
 * All rights reserved.
 */


#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>

#if !defined(KERNEL)
#include <dcedfs/compat.h>		/* dfs_dceErrTxt */
#endif /* !defined(KERNEL) */

#if !defined(KERNEL)
#include <dce/dce_cf_const.h>
#endif /* !defined(KERNEL) */
#include <dce/binding.h>
#include <dce/nbase.h>
#include <dce/id_base.h>
#include <dce/sec_cred.h>
#include <dce/sec_login.h>
#include <dce/secidmap.h>
#include <dce/rpc.h>

#include <dcedfs/dacl.h>

#include <dfsauth.h>

#include <../dfssec_errorStrings.h>
#include <dfsauth_debug.h>
#include <dfsauth_pathnames.h>

#include <../exception_print.h>		/* and pthread stuff */

#include <dcedfs/icl.h>

#include <dfsauth_trace.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/security/dfsauth/dfsauth_server.c,v 1.1.813.1 1996/10/02 20:59:12 damon Exp $")

IMPORT struct icl_set *dfsauth_iclSetp;

IMPORT long dacl_CheckAccessPac _TAKES((
					dacl_t *		aclP,
					u_int32 *		permBitsP,
					dacl_permset_t *	accessRequestedP,
					sec_id_pac_t *		pacP,
					epi_principal_id_t *	userObjP,
					epi_principal_id_t *	groupObjP
				      ));

IMPORT int			dfsauth_client_initialized;
IMPORT int			dfsauth_client_localauthset;

PRIVATE int			dfsauth_server_initialized = 0;
PRIVATE char *			savedListFilenameP = (char *)NULL;
PRIVATE int			errorTablesInitialized = 0;

PRIVATE int			dfsauth_server_initializedClientLocalAuth = 0;
PRIVATE int			dfsauth_server_clientWasNoauth = 0;

PRIVATE pthread_mutexattr_t	dfsauth_server_mutexattr;
PRIVATE pthread_mutex_t		dfsauth_server_mutex;
PRIVATE pthread_once_t		dfsauth_server_lockInitOnce = pthread_once_init;

/*
 * We have to use exceptions here, since this can only be a void operation, because
 * it will be called through pthread_once().
 */
PRIVATE void dfsauth_server_LockInit()
#if defined(dce_54d)
     pthread_addr_t	ignoredArgP;
#endif /* defined(dce_54d) */
{
  static char	routineName[] = "dfsauth_server_LockInit";
  
  /* apparently, PTHREAD_MUTEXATTR_DEFAULT is undefined, despite the documentation */
  TRY
    pthread_mutexattr_create(&dfsauth_server_mutexattr);
  CATCH_ALL
    PrintException(THIS_CATCH, routineName, "pthread_mutexattr_create",
		   __FILE__, __LINE__);
    RERAISE;
  ENDTRY;

  TRY
    pthread_mutex_init(&dfsauth_server_mutex, dfsauth_server_mutexattr);
  CATCH_ALL
    PrintException(THIS_CATCH, routineName, "pthread_mutex_init", __FILE__, __LINE__);
    RERAISE;
  ENDTRY;

  TRY
    pthread_mutexattr_delete(&dfsauth_server_mutexattr);
  CATCH_ALL
    PrintException(THIS_CATCH, routineName, "pthread_mutexattr_delete",
		   __FILE__, __LINE__);
    RERAISE;
  ENDTRY;
}

/*
 * The following routine tries to grab the server package lock, initializing it if
 * necessary.  We have to use exceptions here or move this routine into another file,
 * due to the pre-processor implementation of the pthreads routines.
 */
PRIVATE long dfsauth_server_GrabLock()
{
  volatile long	rtnVal = 0;
  static char	routineName[] = "dfsauth_server_GrabLock";

  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_TRACE_0 );
  
  TRY
    pthread_once(&dfsauth_server_lockInitOnce, dfsauth_server_LockInit);
  CATCH_ALL
    PrintException(THIS_CATCH, routineName, "pthread_once", __FILE__, __LINE__);
    rtnVal = DAUT_ERROR_THREAD_ERROR;
  ENDTRY;

  if (rtnVal == 0) {
    TRY
      pthread_mutex_lock(&dfsauth_server_mutex);
    CATCH_ALL
      PrintException(THIS_CATCH, routineName, "pthread_mutex_unlock", __FILE__, __LINE__);
      rtnVal = DAUT_ERROR_THREAD_ERROR;
    ENDTRY;
  }
  
#if !defined(KERNEL)
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %s", routineName, dfs_dceErrTxt(rtnVal)));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_TRACE_1 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %lu", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_TRACE_2 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */
  
  return rtnVal;
}

PRIVATE long dfsauth_server_ReleaseLock()
{
  volatile long	rtnVal = 0;
  static char	routineName[] = "dfsauth_server_ReleaseLock";
  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_TRACE_3 );
  
  TRY
    pthread_mutex_unlock(&dfsauth_server_mutex);
  CATCH_ALL
    PrintException(THIS_CATCH, routineName, "pthread_mutex_unlock", __FILE__, __LINE__);
    rtnVal = DAUT_ERROR_THREAD_ERROR;
  ENDTRY;
  
#if !defined(KERNEL)
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %s", routineName, dfs_dceErrTxt(rtnVal)));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_TRACE_4 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %lu", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_TRACE_5 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */
  
  return rtnVal;
}

/*
 * The following routine is only to be used within the dfsauth package.  Since it has
 * no parameters, we won't bother putting a prototype in dfsauth.p.h
 */
EXPORT long dfsauth_CheckServerNoAuth()
{
  long		rtnVal = DAUT_ERROR_ACCESS_DENIED;	/* assume no success */
  int		statusRtnVal = 1;
  int		status;
  static char	routineName[] = "dfsauth_CheckServerNoAuth";

  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s: server is checking noauth status", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_TRACE_6 );
  
#if !defined(FORCE_NOAUTH)
  statusRtnVal = dfsauth_server_GetNoAuthStatus(&status);
  if (statusRtnVal == 0) {
    if (status != 0) {	/* if noauth is in effect */
      rtnVal = 0;	/* any user is authorized */
    }
  }
#else /* !defined(FORCE_NOAUTH) */
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s: server is assuming noauth mode", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_TRACE_7 );
  rtnVal = 0;	/* all the servers run in noauth mode for now */
#endif /* !defined(FORCE_NOAUTH) */

  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s: server is%s running in noauth mode",
	    routineName, (rtnVal == 0) ? "" : " not"));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_TRACE_8,
	     ICL_TYPE_STRING, ((rtnVal==0) ? "" : " not"));
  
  return rtnVal;
}

/*
 * main initialization routine for a server
 * ONLY the bosserver should call this routine without going through the
 * covering routine dfsauth_server_InitAuthentication!!!
 *
 * parameters:
 *	principalNameP: the principal name under which the server will run.  This
 * parameter is REQUIRED in kernel space, but can be defaulted to the dfs principal name
 * for the local host in user-space
 *	listFilenameP: the name of the file containing the admin list against which
 * authorization checks will be made.  This parameter is REQUIRED in user-space,
 * and must be NULL in kernel-space.
 *	serverExtensionP: this parameter is only used when principalNameP is NULL and
 * the server's principal is obtained from a local file.  If the extension is non-NULL,
 * it is expected to be the string representation of a uuid representing one of the
 * pre-defined types of servers (see the files ./dfsauth_localprincipal.c and
 * dfsauth_uuidmap.h).  The uuid map will be used to determine a suffix for the
 * principal to be used by the server.
 *	This feature is currently unused by DFS code; should the decision 
 * be made to use it, the uuid maps will be extended.  Until then, NULL should
 * be passed for this parameter.
 *	noAuth:	an integer indicating whether or not the server is to run without
 * authorization checking.  If the value of noAuth is non-zero, the server will refrain
 * from doing any of the rpc_server initialization calls.
 */
EXPORT long dfsauth_server_InitAuth(principalNameP, listFilenameP, serverExtensionP,
				    noAuth)
     char *		principalNameP;
     char *		listFilenameP;
     char *		serverExtensionP;
     int		noAuth;
{
  volatile long		rtnVal = 0;
  long			lockStatus = 0;
  volatile int		exceptionRaised = 0;
  char			serverPrincipalName[BUFSIZ];
  char			calleeName[BUFSIZ];
  char			fullListPathname[BUFSIZ];
  int			statRtn;
  int			savedErrno;
  struct stat		statBuf;
  sec_rgy_name_t	theGlobalName;
  uuid_t		realmUuid;
  sec_rgy_bind_auth_info_t	bindingAuthInfo;
  sec_rgy_handle_t		secdHandle;
  volatile error_status_t	ncs_error_status;
  static char		routineName[] = "dfsauth_server_InitAuth";
  
#if !defined(FORCE_NOSECURITY)

  if ((rtnVal = dfsauth_server_GrabLock()) == 0) {
    if (dfsauth_server_initialized == 0) {
      if (errorTablesInitialized == 0) {
#if !defined(KERNEL)
	initialize_dau_error_table();
	initialize_dcl_error_table();
#endif /* !defined(KERNEL) */
	
	errorTablesInitialized = 1;
      }

      /* initialize the tracing package */
      if (dfsauth_InitTracingPackage() != 0) {
      osi_printf("%s: error initializing DFS trace package\t%s, %d\n",
		 routineName, __FILE__, __LINE__);
      }

      /* stash away the names of the authorization files to use */
#if !defined(KERNEL)
      if (listFilenameP != (char *)NULL) {
	rtnVal = dfsauth_GenerateAdminListPath(listFilenameP,
					       fullListPathname, sizeof(fullListPathname));
	if (rtnVal == 0) {
	  savedListFilenameP = osi_Alloc(strlen(fullListPathname) + 1);
	  (void)strcpy(savedListFilenameP, fullListPathname);
	}
	else if (rtnVal == DAUT_ERROR_BUFFER_TOO_SMALL) {
	  rtnVal = DAUT_ERROR_PATHNAME_TOO_LONG;
	}
      }
      else {
	if (noAuth != 0) {
	  rtnVal = dfsauth_InitPathnames(); /* We need to get the noauth pathname set up.
					     * This is done automatically by
					     * dfsauth_GenerateAdminListPath when it is used.
					     */
	  
	}
	else {
	  dfsauth_LogParamError(routineName, "listFilenameP");
	  rtnVal = DAUT_ERROR_PARAMETER_ERROR;
	}
      }
#endif /* !defined(KERNEL) */
      
      if (rtnVal == 0) {
#if !defined(KERNEL)
	/* make sure there is an ACL on disk where it's expected (if it's expected) */
	/* we create it, if requested, even if running noauth */
	if (listFilenameP != (char *)NULL) {
	  /* avoid the overhead of having to setup localauth, if we can */
	  statRtn = stat(fullListPathname, &statBuf);
	  savedErrno = errno;
	  
	  if ((statRtn == -1) && (savedErrno == ENOENT)) {
	    if ((dfsauth_client_initialized == 0) || (dfsauth_client_localauthset == 0)) {
#ifdef notdef
	      dfsauth_server_clientWasNoauth = (dfsauth_client_initialized &&
						(dfsauth_client_localauthset == 0));
	      rtnVal = dfsauth_client_InitAuthentication((rpc_binding_handle_t *)NULL,
							 (char *)NULL,
							 0 /* => not noauth */,
							 1 /* => use localauth */);
	      if (rtnVal == 0) {
		dfsauth_server_initializedClientLocalAuth = 1;
	      }
#endif /* notdef */
	    }
	    
	    if (rtnVal == 0) {
	      bindingAuthInfo.info_type = sec_rgy_bind_auth_none;
	      (void)strcpy((char *)theGlobalName, CELL_DIR_ROOT);
	      sec_rgy_site_bind(theGlobalName,
				&bindingAuthInfo,
				&secdHandle,
				(error_status_t *)&ncs_error_status);
	      
	      if (ncs_error_status == error_status_ok) {
		/* get the local realm uuid from the security server */
		(void)sprintf((char *)theGlobalName, "%s%s", CELL_DIR_ROOT, "root");
		TRY
		  exceptionRaised = 0;
	          sec_id_parse_name(secdHandle,
				    theGlobalName,
				    NULL,
				    &realmUuid,
				    NULL,
				    (uuid_t *)NULL,
				    (error_status_t *)&ncs_error_status);
		CATCH_ALL
		  PrintException(THIS_CATCH, routineName, "sec_id_parse_name",
				 __FILE__, __LINE__);
	          exceptionRaised = 1;
		ENDTRY;
	      
		if ((ncs_error_status == error_status_ok) && !exceptionRaised) {
		  rtnVal = dacl_CreateAclOnDisk(savedListFilenameP, &bosserverAclMgmtUuid,
						(epi_uuid_t *)&realmUuid,
						(dacl_permset_t *)NULL, (dacl_permset_t *)NULL,
						(dacl_permset_t *)NULL,
						0 /* => not an Episode file */, 0);
		  if (rtnVal == DACL_ERROR_ACL_FILE_EXISTS) {
		    rtnVal = 0;	/* if the file's already there, we're still all right */
		  }
		}
		else {
		  if (exceptionRaised) {
		    dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
			    ("%s: sec_id_parse_name raised an exception; see stdout",
			     routineName));
		    icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_NONE_0 );
		    rtnVal = DAUT_ERROR_SEC_RGY_PGO_ERROR;
		  }
		  else {
		    dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
			    ("%s: unable to parse global name: %s (%s)",
			     routineName, theGlobalName,
			     RgySec_ErrorString((error_status_t *)&ncs_error_status)));
		    icl_Trace2(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_NONE_1,
			    ICL_TYPE_STRING, theGlobalName, ICL_TYPE_STRING,
			    RgySec_ErrorString(
			    (error_status_t *)&ncs_error_status));
		    rtnVal = (long)ncs_error_status;
		  }
		}

		if (dfsauth_server_initializedClientLocalAuth) {
		  rtnVal = dfsauth_client_CleanupAuthentication();
		  
		  if (dfsauth_server_clientWasNoauth) {
		    rtnVal = dfsauth_client_InitAuthentication((rpc_binding_handle_t *)NULL,
							       (char *)NULL,
							       1 /* => use noauth */,
							       0 /* => not localauth */);
		    
		  }
		}
	      }
	      else {
		dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		    ("%s: unable to bind to a registry site: %s (%s)",
		    routineName, theGlobalName,
		    RgySec_ErrorString((error_status_t *)&ncs_error_status)));
		icl_Trace2(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_NONE_2,
		    ICL_TYPE_STRING, theGlobalName, ICL_TYPE_STRING,
		    RgySec_ErrorString((error_status_t *)&ncs_error_status));
	      }
	    }	/* end if we tried to create the acl */
	  }	/* end if we had to create the acl */
	}	/* end if listFilenameP is not NULL */
#endif /* !defined(KERNEL) */
	
	if (rtnVal == 0) {
	  if (principalNameP != (char *)NULL) {
	    (void)strncpy(serverPrincipalName, principalNameP,
			  sizeof(serverPrincipalName));
	  }
	  else {
#if defined(KERNEL)
	    rtnVal = DAUT_ERROR_PRINCIPAL_NAME_REQD_IN_KERNEL;
#else /* defined(KERNEL) */
	    rtnVal = dfsauth_PrinName_GetName((char *)NULL, serverExtensionP,
					      serverPrincipalName,
					      sizeof(serverPrincipalName));
#endif /* defined(KERNEL) */
	  }
	}
	
	if (rtnVal == 0) {
	  /* if noAuth is spec'd here, it's probably because we are bootstrapping */
	  /*
	   * Because of the way the RPC works, we can't really bootstrap via any
	   * of our servers.  In particular, the bosserver can't be used to bootstrap
	   * the key file.
	   */
	  (void)strcpy(calleeName, "rpc_server_register_auth_info");
	  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_DCE_TRACE,
		   ("%s: about to call %s with principal name %s",
		    routineName, calleeName, serverPrincipalName));
	  icl_Trace2(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_TRACE_9,ICL_TYPE_STRING, calleeName, ICL_TYPE_STRING, serverPrincipalName);
	  
	  TRY
	    exceptionRaised = 0;
	    rpc_server_register_auth_info((unsigned char *)serverPrincipalName,
					  rpc_c_authn_dce_private,
					  (rpc_auth_key_retrieval_fn_t)NULL,
					  (void *)NULL,
					  (error_status_t *)&ncs_error_status);
	  CATCH_ALL
	    PrintException(THIS_CATCH, routineName,
			   "rpc_server_register_auth_info",
			   __FILE__, __LINE__);
	    exceptionRaised = 1;
	  ENDTRY;
	  
	  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_DCE_TRACE,
		   ("%s: returned from %s", routineName, calleeName));
	  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_TRACE_10 , ICL_TYPE_STRING, calleeName);
	  
	  if ((ncs_error_status != error_status_ok) || exceptionRaised) {
	    if (exceptionRaised) {
	      dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		      ("%s: rpc_server_register_auth_info raised an exception; see stdout",
		       routineName));
	      icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_NONE_3 );
	      rtnVal = DAUT_ERROR_RPC_ERROR;
	    }
	    else {
	      dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		  ("%s: unable to register authn info: %s",
		   routineName,
		   RgySec_ErrorString((error_status_t *)&ncs_error_status)));
	      icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_NONE_4,
		   ICL_TYPE_STRING,
		   RgySec_ErrorString((error_status_t *)&ncs_error_status));
	      rtnVal = (long)ncs_error_status;
	    }
	  }
	}
	else {
#if defined(KERNEL)
	  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
		   ("%s: dfsauth_PrinName_GetName returned error: %lu",
		    routineName, rtnVal));
	  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_WARNINGS_11 , ICL_TYPE_LONG, rtnVal);
#else /* defined(KERNEL) */
	  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
		   ("%s: dfsauth_PrinName_GetName returned error: %s", routineName,
		    dfs_dceErrTxt(rtnVal)));
	  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_WARNINGS_12 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#endif /* defined(KERNEL) */
	}
	
	dfsauth_server_initialized = 1;
      }
    }

    if (lockStatus = dfsauth_server_ReleaseLock()) {
      osi_printf("%s: error releasing dfsauth server package lock\t%s, %d\n",
		 routineName, __FILE__, __LINE__);
      if (rtnVal == 0) {
	rtnVal = lockStatus;	/* don't mask any other errors */
      }
    }
  }
  else {
    osi_printf("%s: error obtaining dfsauth server package lock\t%s, %d\n",
	       routineName, __FILE__, __LINE__);
  }
  
#endif /* !defined(FORCE_NOSECURITY) */
  return rtnVal;
}

/*
 * the cover routine for dfsauth_server_InitAuth
 * Any server that is not the bosserver should call this routine, rather than
 * calling dfsauth_server_InitAuth directly.
 */
EXPORT long dfsauth_server_InitAuthentication(principalNameP, listFilenameP,
					      serverExtensionP)
     char *		principalNameP;
     char *		listFilenameP;
     char *		serverExtensionP;
{
  return dfsauth_server_InitAuth(principalNameP, listFilenameP, serverExtensionP,
				 0 /* no noAuth */);
}

/* if a group is specified, any member of that group can do anything */
PRIVATE int dfsauth_CheckServerPacGroupAuth(userPacP, authGroupP)
     sec_id_pac_t *	userPacP;
     uuid_t *		authGroupP;
{
  int			rtnVal = 1;	/* assume not authz */
  error_status_t	uuidError;
  unsigned int		i;
  
  /* first check the login group */
  if ((uuid_equal(&(userPacP->group.uuid), authGroupP, &uuidError) == true) &&
      (uuidError == uuid_s_ok)) {
    rtnVal = 0;
  }
  
  /* now, check the local groups, if necessary */
  for (i = 0; (rtnVal == 1) && (i < userPacP->num_groups); i++) {
    if ((uuid_equal(&(userPacP->groups[i].uuid), authGroupP, &uuidError) == true) &&
	(uuidError == uuid_s_ok)) {
      rtnVal = 0;
    }
  }

  /* now, check the foreign groups, if necessary */
  /* XXX - we have to be able to get the local realm's uuid here */
  for (i = 0; (rtnVal == 1) && (i < userPacP->num_foreign_groups); i++) {
    if ((uuid_equal(&(userPacP->foreign_groups[i].id.uuid), authGroupP, &uuidError)
	 == true) &&
	(uuidError == uuid_s_ok)) {
      rtnVal = 0;;
    }
  }
  
  return rtnVal;
}


#if !defined(KERNEL)
EXPORT void * dfsauth_server_CheckAdminListAccessBegin(listFileNameOverrideP)
     IN char *			listFileNameOverrideP;
{
  long			code = 0;
  static char		routineName[] = "dfsauth_server_CheckAdminListAccess";
  char			overrideFullPathname[BUFSIZ];
  dacl_t *		theAclP;
  char *		listFilenameP = (char *)NULL;
  void *		rtnVal = (void *)0;

  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s: entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_CHECKADMINLISTACCESS);

  code = dfsauth_server_GrabLock();
  if (code == 0) {
    if (dfsauth_server_initialized != 0) {
      /* figure out whether to use default filenames or not, but only outside the kernel */
      if (listFileNameOverrideP == (char *)NULL) {
	listFilenameP = savedListFilenameP;
      }
      else {
	code = dfsauth_GenerateAdminListPath(listFileNameOverrideP,
					     overrideFullPathname,
					     sizeof(overrideFullPathname));
	listFilenameP = overrideFullPathname;
      }
	  
      if ((code == 0) && (listFilenameP != (char *)NULL)) {
	theAclP = (dacl_t *)osi_Alloc(sizeof(dacl_t));
	if (theAclP != (dacl_t *)0) {
	  code = dacl_ReadFromDisk(theAclP, listFilenameP, &bosserverAclMgmtUuid);
	  if (code == 0) {
	    rtnVal = (void *)theAclP;
	  }
	  else {
	    osi_Free(theAclP, sizeof(dacl_t));
	  }
        }
      }
    }
    code = dfsauth_server_ReleaseLock();
    if (code) {
      osi_printf("%s: error releasing dfsauth server package lock\t%s, %d\n",
		 routineName, __FILE__, __LINE__);
    }
  }
  else {
    osi_printf("%s: error obtaining dfsauth server package lock\t%s, %d\n",
	       routineName, __FILE__, __LINE__);
  }

  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s: exiting, returning %x", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_CHECKADMINLISTACCESS_EXIT, 
	     ICL_TYPE_LONG, rtnVal);
  
  return rtnVal;
}

EXPORT void dfsauth_server_CheckAdminListAccessEnd(cookie)
     IN void *			cookie;
{
  dacl_FreeAclEntries((dacl_t *)cookie);
  osi_Free(cookie, sizeof(dacl_t));
}

EXPORT int dfsauth_server_CheckAdminListAccessNext(cookie, realmUuidp, princUuidp)
     IN void *			cookie;
     IN uuid_t *		realmUuidp;
     IN uuid_t *		princUuidp;
{
     dacl_t *			aclP = (dacl_t *)cookie;
     u_int32 *			permBitsP;
     dacl_permset_t 		accessRequested = (dacl_permset_t) ~0L;
     dacl_permset_t		accessAllowed = 0L;	/* assume nothing granted */

  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_CHECKADMINLIST);
  dacl_DetermineAccessAllowed(aclP, NULL, &accessRequested, (epi_uuid_t *)realmUuidp,
			      (epi_uuid_t *)princUuidp, NULL, NULL, 0, NULL, 0, 
			      NULL, NULL, /*isAuthn*/1, &accessAllowed);

  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_CHECKADMINLIST_EXIT, 
	     ICL_TYPE_LONG, accessAllowed);
  return ((int)accessAllowed);
}

EXPORT int dfsauth_server_CheckAdminListGroupAccess(cookie, rgyHandle, princName, fullPrincName)
      IN void *			cookie;
      IN sec_rgy_handle_t	rgyHandle;
      IN char *			princName;
      IN char *			fullPrincName;
{
    int 			i;
    int				isMember = 0;
    dacl_t *			aclP = (dacl_t *)cookie;
    dacl_entry_t *		entryListP;
    unsigned int		numEntries;
    unsigned32			st;
    sec_rgy_name_t		groupName;
    sec_rgy_name_t		principal;
    sec_rgy_handle_t		foreignRgyHandle;

    /* check local group access first */
    entryListP = aclP->complex_entries[(int)dacl_complex_entry_type_group].complex_entries;
    numEntries = aclP->complex_entries[(int)dacl_complex_entry_type_group].num_entries;

    (void) strcpy((char *)principal, princName);
    for(i = 0; i < numEntries; i++, entryListP++)
    {
	/* turn group entry into a group name */
	sec_id_gen_group(rgyHandle, (uuid_t *)&aclP->default_realm,
			 (uuid_t *)&entryListP->entry_info.id, NULL, NULL,
			 groupName, &st);

	if (st == error_status_ok) {
	    /* we have the name, now do the check */
	    if (sec_rgy_pgo_is_member(rgyHandle, sec_rgy_domain_group, groupName, 
				      principal, &st)) {
		isMember = 1;
		break;
	    }
	}
    }

    if (!isMember) {
	/* 
	 * haven't found it yet, start the painful process of looking at
	 * foreign groups.
	 */
	entryListP = aclP->complex_entries[(int)dacl_complex_entry_type_other].complex_entries;
	numEntries = aclP->complex_entries[(int)dacl_complex_entry_type_other].num_entries;

	(void) strcpy((char *)principal, fullPrincName);
	for(i = 0; (i < numEntries) && !isMember; i++, entryListP++)
	{
	    sec_rgy_name_t	cellname;

	    if (entryListP->entry_type != dacl_entry_type_foreign_group) 
		continue;

	    /* turn foreign group entry into a foreign group name */
	    sec_id_gen_group(rgyHandle, (uuid_t *)&entryListP->entry_info.foreign_id.realm,
			     (uuid_t *)&entryListP->entry_info.id, NULL, cellname,
			     groupName, &st);
	    /* bind to foreign registry */
	    sec_rgy_site_open((unsigned char *)cellname, &foreignRgyHandle, &st);
	    if (st == error_status_ok) {
		if (st == error_status_ok) {
		    /* we have the name, now do the check */
		    if (sec_rgy_pgo_is_member(foreignRgyHandle, sec_rgy_domain_group, groupName, 
					      principal, &st)) {
			isMember = 1;
		    }
		}
		sec_rgy_site_close(foreignRgyHandle, &st);
	    }
	}
    }	

    return isMember;
}
#endif /* !KERNEL */

PRIVATE long dfsauth_server_CheckAuthorizationAclOptional(rpcBindingP, permsetP,
							  listFileNameOverrideP,
							  authGroupUuidP,
							  checkAclFile)
     IN rpc_binding_handle_t *	rpcBindingP;
     IN dacl_permset_t *	permsetP;
     IN char *			listFileNameOverrideP;
     IN uuid_t *		authGroupUuidP;
     IN int			checkAclFile;
{
  /* assume user in not authorized */
  volatile long		rtnVal = DAUT_ERROR_ACCESS_DENIED;
  long			lockStatus = 0;
  volatile long		tempRtnVal = 0;
  volatile int		exceptionRaised = 0;
  static char		routineName[] = "dfsauth_server_CheckAuthorizationAclOptional";
  char			calleeName[BUFSIZ];
  char			overrideFullPathname[BUFSIZ];
  rpc_authz_handle_t	clientId;
  sec_id_pac_t *	clientDCEIdP;
  uuid_t *		clientPrinUuidP;
  unsigned32		authnLevel;
  unsigned32		authnSvc;
  unsigned32		authzSvc;
  volatile error_status_t	extractError;
  dacl_permset_t	defaultUserListPerms = DACL_USERLIST_PERMS;
  dacl_permset_t *	permsRequestedP;
  dacl_t		theAcl;
  char *		listFilenameP = (char *)NULL;

  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s: entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_TRACE_13 );

#if defined(FORCE_NOSECURITY)
  rtnVal = 0;
#else /* defined(FORCE_NOSECURITY) */
  if (rpcBindingP != (rpc_binding_handle_t *)NULL) {
    lockStatus = dfsauth_server_GrabLock();

    if (lockStatus == 0) {
      if (dfsauth_server_initialized != 0) {
	rtnVal = dfsauth_CheckServerNoAuth();
	if (rtnVal != 0) {
	  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
		   ("%s: server is not running no auth, will check id", routineName));
	  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_TRACE_14 );
#if !defined(KERNEL)
	  /* figure out whether to use default filenames or not, but only outside the kernel */
	  if (listFileNameOverrideP == (char *)NULL) {
	    listFilenameP = savedListFilenameP;
	  }
	  else {
	    tempRtnVal = dfsauth_GenerateAdminListPath(listFileNameOverrideP,
						       overrideFullPathname,
						       sizeof(overrideFullPathname));
	    listFilenameP = overrideFullPathname;
	  }
#endif /* !defined(KERNEL) */
	  
	  if (tempRtnVal == 0) {
	    (void)strcpy(calleeName, "rpc_binding_inq_auth_client");
	    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_DCE_TRACE,
		     ("%s: about to call %s", routineName, calleeName));
	    icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_TRACE_15 , ICL_TYPE_STRING, calleeName);
	    
	    TRY
	      exceptionRaised = 0;
	      rpc_binding_inq_auth_client(*rpcBindingP,
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
	    
	    if ((extractError == error_status_ok) && ! exceptionRaised) {
	      dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
		       ("%s: found authn svc: %s", routineName,
			Rpc_Authn_Service_Strings(authnSvc)));
	      icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_WARNINGS_16 , ICL_TYPE_STRING, Rpc_Authn_Service_Strings(authnSvc));
	      dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
		       ("%s: found authn level: %d", routineName,
			authnLevel));
	      icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_WARNINGS_17 , ICL_TYPE_LONG, authnLevel);
	      dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
		       ("%s: found authz svc: %s", routineName,
			Rpc_Authz_Service_Strings(authzSvc)));
	      icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_WARNINGS_18 , ICL_TYPE_STRING, Rpc_Authz_Service_Strings(authzSvc));
	      
	      if (authzSvc == rpc_c_authz_none) {		/* -noauth */
		rtnVal = dfsauth_CheckServerNoAuth();
	      }
	      else if (authzSvc == rpc_c_authz_name) {
		dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
			 ("%s: found name authzn; name = %s",
			  routineName, (unsigned_char_t *)clientId));
		icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_WARNINGS_19 , ICL_TYPE_STRING, (unsigned_char_t*)clientId);
		rtnVal = dfsauth_CheckServerNoAuth();
	      }
	      else if (authzSvc == rpc_c_authz_dce) {
		clientDCEIdP = (sec_id_pac_t *)clientId;
		if (clientDCEIdP->authenticated == true) {  
		  if (authnLevel == rpc_c_authn_level_none) {	/* try for noauth */
		    rtnVal = dfsauth_CheckServerNoAuth();
		  }
		  else {
		    /* first, see if the ACL allows the access (if there is an ACL) */
		    if ((checkAclFile != 0) && (listFilenameP != (char *)NULL)) {
		      rtnVal = dacl_ReadFromDisk(&theAcl, listFilenameP, &bosserverAclMgmtUuid);
		      
		      if (rtnVal == 0) {
			if (permsetP == (dacl_permset_t *)NULL) {
			  /* the most common case, for the moment */
			  permsRequestedP = &defaultUserListPerms;
			}
			else {
			  permsRequestedP = permsetP;
			}
			rtnVal = dacl_CheckAccessPac(&theAcl, (u_int32 *)NULL,
				permsRequestedP, clientDCEIdP,
				(epi_principal_id_t *)NULL,
				(epi_principal_id_t *)NULL);
			dacl_FreeAclEntries(&theAcl);
		      }
		      else {
#if defined(KERNEL)
			dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
				 ("%s: error reading acl from disk: %lu", routineName, rtnVal));
			icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_WARNINGS_20 , ICL_TYPE_LONG, rtnVal);
#else /* defined(KERNEL) */
			dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
				 ("%s: error reading acl from disk: %s", routineName,
				  dfs_dceErrTxt(rtnVal)));
			icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_WARNINGS_21 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#endif /* defined(KERNEL) */
		      }
		    }
		    
		    /*
		     * check the group, if it's there and we're still (implicitly) denied
		     * access
		     */
		    if ((rtnVal != 0) && (rtnVal != DACL_ERROR_ACCESS_EXPLICITLY_DENIED) &&
			(authGroupUuidP != (uuid_t *)NULL)) {
		      rtnVal = dfsauth_CheckServerPacGroupAuth(clientDCEIdP, authGroupUuidP);
		    }
		    
		  }
		}	/* end if the PAC is really authenticated */
		else {
		  /* if there is no authentication, try noauth */
		  rtnVal = dfsauth_CheckServerNoAuth();
		}
		
	      }	/* end if dce authentication used */
	      
	      else {
		dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
			 ("%s: %s unrecognized authzn svc found in binding: %#x",
			  routineName, calleeName, authzSvc));
		icl_Trace2(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_WARNINGS_22 , ICL_TYPE_STRING, calleeName, ICL_TYPE_LONG, authzSvc);
	      }
	    }	/* end if we were able to extract the PAC from the binding handle */
	    else {
	      if (exceptionRaised) {
		dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
			("%s: rpc_binding_inq_auth_client raised an exception; see stdout",
			 routineName));
		icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_NONE_5 );
		rtnVal = DAUT_ERROR_RPC_ERROR;
	      }
	      else {
		if (extractError == rpc_s_binding_has_no_auth) {
		  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
			   ("%s: binding has no auth info, checking noauth", routineName));
		  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_WARNINGS_23 );
		  rtnVal = dfsauth_CheckServerNoAuth();
		}
		else {
		  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
			   ("%s: %s error: %s",
			    routineName, calleeName,
			    RgySec_ErrorString((error_status_t *)&extractError)));
		  icl_Trace2(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_WARNINGS_24,
			ICL_TYPE_STRING, calleeName, ICL_TYPE_STRING, 
			RgySec_ErrorString((error_status_t *)&extractError));
		}
	      }
	    }	/* end else (not able to extract PAC from binding handle) */
	    
	    if (rtnVal != 0) {
	      rtnVal = DAUT_ERROR_ACCESS_DENIED;	/* fold all the access errors into one */
	    }
	  }
	  else {
	    if (tempRtnVal == DAUT_ERROR_BUFFER_TOO_SMALL) {
	      rtnVal = DAUT_ERROR_PATHNAME_TOO_LONG;
	    }
	    else {
	      rtnVal = tempRtnVal;
	    }
	  }
	}
      }
      else {
	rtnVal = DAUT_ERROR_SERVER_NOT_INITIALIZED;
      }

      lockStatus = dfsauth_server_ReleaseLock();
      if (lockStatus) {
	osi_printf("%s: error releasing dfsauth server package lock\t%s, %d\n",
		   routineName, __FILE__, __LINE__);
      }
    }
    else {
      osi_printf("%s: error obtaining dfsauth server package lock\t%s, %d\n",
		 routineName, __FILE__, __LINE__);
    }
  }
  else {
    dfsauth_LogParamError(routineName, "rpcBindingP");
    rtnVal = DAUT_ERROR_PARAMETER_ERROR;
  }

#endif /* defined(FORCE_NOSECURITY) */

  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s: exiting, returning %d", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_TRACE_25 , ICL_TYPE_LONG, rtnVal);
  
  return rtnVal;
}

/*
 * These two routines had to be separated because of the way the flserver authzn
 * checking code is structured.
 */
EXPORT long dfsauth_server_CheckAuthorization(rpcBindingP, permsetP, listFileNameOverrideP,
					      authGroupUuidP)
     IN rpc_binding_handle_t *	rpcBindingP;
     IN dacl_permset_t *	permsetP;
     IN char *			listFileNameOverrideP;
     IN uuid_t *		authGroupUuidP;
{
  return dfsauth_server_CheckAuthorizationAclOptional(rpcBindingP, permsetP,
						      listFileNameOverrideP,
						      authGroupUuidP,
						      1 /* => check ACL file, too */);
}

EXPORT long dfsauth_server_CheckAuthorizationGroup(rpcBindingP, authGroupUuidP)
     IN rpc_binding_handle_t *	rpcBindingP;
     IN uuid_t *		authGroupUuidP;
{
  long		rtnVal = 0;
  static char	routineName[] = "dfsauth_server_CheckAuthorizationGroup";
  
#if !defined(FORCE_NOSECURITY)
  if ((rpcBindingP != (rpc_binding_handle_t *)NULL) &&
      (authGroupUuidP != (uuid_t *)NULL)) {
    /*
     * rpcBindingHandleP will be checked in the following routine, but, if any problem
     * is logged here, the error message will be a bit more informative.
     */
    rtnVal = dfsauth_server_CheckAuthorizationAclOptional(rpcBindingP,
							  (dacl_permset_t *)NULL,
							  (char *)NULL,
							  authGroupUuidP,
							  0 /* => check group only */);
  }
  else {
    if (rpcBindingP == (rpc_binding_handle_t *)NULL) {
      dfsauth_LogParamError(routineName, "rpcBindingP");
    }
    if (authGroupUuidP == (uuid_t *)NULL) {
      dfsauth_LogParamError(routineName, "authGroupUuidP");
    }
    
    rtnVal = DAUT_ERROR_PARAMETER_ERROR;
  }
#endif /* !defined(FORCE_NOSECURITY) */

  return rtnVal;
}

#if !defined(KERNEL)
/*
 * A server calling this function (and running as superuser) will
 * be sure to be running under the DCE self identity.
 */
EXPORT void dfsauth_server_RunAsSelf()
{
    sec_login_handle_t      context;
    error_status_t          st;

    sec_login_get_current_context(&context, &st);
    if (st == error_status_ok)
        sec_login_release_context(&context, &st);
}

/*
 * A server calls this function to get the realm and principal UUIDs from
 * a client's binding handle.
 */

EXPORT long dfsauth_server_GetClientIdentity(bindingHndl,
					     realmUuidP,
					     princUuidP)
    IN  rpc_binding_handle_t	bindingHndl;
    OUT uuid_t			*realmUuidP;
    OUT uuid_t			*princUuidP;
{
    long			code = 0;
    unsigned32			st = rpc_s_ok, st2;
    rpc_authz_cred_handle_t	credHndl;
    sec_cred_pa_handle_t	paHndl = NULL;
    sec_id_pa_t			*paData;
    sec_id_pac_t		*pacData;
    unsigned32			authzSvc;

    icl_Trace3(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_GETCLIENTIDENTITY,
	       ICL_TYPE_POINTER, bindingHndl, ICL_TYPE_POINTER, realmUuidP,
	       ICL_TYPE_POINTER, princUuidP);
 
    /* get a handle to the caller's identity */
    rpc_binding_inq_auth_caller(bindingHndl, &credHndl, NULL, NULL, NULL,
				&authzSvc, &st);
    if (st != rpc_s_ok || authzSvc != rpc_c_authz_dce) {
	code = DAUT_ERROR_BAD_CLIENT_IDENTITY;
	goto exit;
    }

    /* get a handle to the caller's credentials */
    paHndl = sec_cred_get_initiator(credHndl, &st);
    if (st != rpc_s_ok) {
	code = DAUT_ERROR_BAD_CLIENT_IDENTITY;
	goto exit;
    }

    /*
     * Get the client's priviledged attributes data.
     * We need to support two styles of attributes, PACs and EPACs.
     * Pre DCE 1.1 clients use PACs. Post DCE 1.1 clients use EPACs.
     */
    paData = sec_cred_get_pa_data(paHndl, &st);
    if (st != rpc_s_ok) {
	pacData = sec_cred_get_v1_pac(paHndl, &st2);
	if (st2 != rpc_s_ok || !pacData->authenticated) {
	    code = DAUT_ERROR_BAD_CLIENT_IDENTITY;
	    goto exit;
	} else {
	    st = rpc_s_ok;
	    *realmUuidP = pacData->realm.uuid;
	    *princUuidP =  pacData->principal.uuid;
	}
    } else {
        *realmUuidP = paData->realm.uuid;
        *princUuidP = paData->principal.uuid;
    }

exit:
    if (paHndl != NULL) {
        sec_cred_free_pa_handle(&paHndl, &st2);
    }
    icl_Trace2(dfsauth_iclSetp, DFSAUTH_ICL_SERVER_GETCLIENTIDENTITY_EXIT,
	       ICL_TYPE_LONG, code, ICL_TYPE_LONG, st);
    return code;
}
#endif /* !defined(KERNEL) */
