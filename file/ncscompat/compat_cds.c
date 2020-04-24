/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright (c) 1995 Transarc Corporation
 * All rights reserved
 */
/*
 * HISTORY
 * $Log: compat_cds.c,v $
 * Revision 1.1.71.1  1996/10/02  17:54:15  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:42:04  damon]
 *
 * $EndLog$
 */
#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <arpa/inet.h>

#include <dce/rpc.h>

/*
 * We need the following header file for rpc_ns_set_authn prototype, until
 * they fix access to the name space when there is no default context
 */
#include <dce/rpcpvt.h>

/* We need the following header file for rpcd if_spec */
#include <dce/ep.h>
#include <pthread.h>

#include <dcedfs/dfsauth.h>

#include <compat.h>

#include <dcedfs/osi_net.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsncsmac.h>
#include <dfsncssvc.h>
#include <dfsncsmsg.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/ncscompat/compat_cds.c,v 1.1.71.1 1996/10/02 17:54:15 damon Exp $")

extern int  compat_svc_must_init;
extern void compat_svc_init();

#define INIT_SVC if (compat_svc_must_init) compat_svc_init()

#define IS_COMM_ERR(s)  ((s) == rpc_s_ok ? 0 : 1)
#define CHK_RPC_ERR(st) 			\
    MACRO_BEGIN					\
    if (st != rpc_s_ok)				\
       goto exit;				\
    MACRO_END

#define STRIP_EP	1

/* notes
 *	CDS location	/.:/hosts/<hostname>/self
 *	principal	/.:/hosts/<hostname>/dfs-server
 */

/* we don't seem to get arg type-checking without a separate prototype, so here it is */
static void dfs_register_rpc_server_cds_core _TAKES((
					rpc_if_handle_t	if_handle,
					rpc_mgr_epv_t	manager_epv,
					uuid_t *	obj_uuidP,
					uuid_t *	type_uuidP,
					int		maxcalls,
					char *		annotationP,
					error_status_t *	stp
						    ));

/* ------------------
 * routines for rendezvous via RPCD - the scheme du jour
 * ------------------
 */

#define	DFS_PROTSEQ			"ncadg_ip_udp"
#define	DFS_DEFAULT_PREFIX		"/.:/hosts/"
#define	DFS_DEFAULT_POSTFIX		"/dfs-server"
#define DFS_SELF_SUFFIX			"/self"
#define	DFS_ENTRY_SYNTAX		rpc_c_ns_syntax_dce


/* dfs_register_rpc_server_cds_core
 *      Register a server with the rpc runtime - only the cds portion
 * entry:
 *      if_handle - interface spec
 *	mgr_epv - manager entry point vector
 *      entry_nameP - name service entry with which bindings will be registered
 *	adminPathP - path to admin list file
 *      maxcalls - max # calls
 * exit:
 *	st - returned status. rpc_s_ok indicates success.
 * notes:
 *	currently we only use udp.
 *
 * 26 July 1991 (bab) - separated out the cds core so that a version of
 * rpc_register_dfs_server that used no authentication could be cleanly written
 * without requiring interface changes.
 *
 */
static void dfs_register_rpc_server_cds_core(if_handle, manager_epv,
					      obj_uuidP, type_uuidP, maxcalls,
					      annotationP, stp)
     IN  rpc_if_handle_t if_handle;
     IN  rpc_mgr_epv_t manager_epv;
     IN  uuid_t *obj_uuidP;
     IN  uuid_t *type_uuidP;
     IN  int maxcalls;
     IN  char *	annotationP;
     OUT error_status_t *stp;
{
    rpc_binding_vector_t *binding_vectorP=(rpc_binding_vector_t *)NULL;
    uuid_vector_t *object_uuid_vectorP=(uuid_vector_t *)NULL;
    error_status_t altSt;

    *stp = rpc_s_ok;

    /* If the type UUID is non-nil, the object UUID should not be nil.
     * Is the vice versa true?
     */
    if (!uuid_is_nil(type_uuidP, stp) && *stp == uuid_s_ok)
      if (uuid_is_nil(obj_uuidP, stp) && *stp == uuid_s_ok) {
	*stp = COMPAT_ERROR_BAD_UUID;
	goto exit;
      }

    /* register interface and manager epv with rpc runtime, with a
     * null object uuid
     */
    rpc_server_register_if(if_handle, type_uuidP, manager_epv, stp);
    CHK_RPC_ERR(*stp);

    /* register the object uuid and its type with the rpc runtime */
    if (!uuid_is_nil(type_uuidP, stp) && *stp == uuid_s_ok)
      rpc_object_set_type(obj_uuidP, type_uuidP, stp);
    CHK_RPC_ERR(*stp);

    /* for the present, use only udp */
    rpc_server_use_protseq((unsigned char *)DFS_PROTSEQ, maxcalls, stp);
    CHK_RPC_ERR(*stp);

    /* get our own bindings */
    rpc_server_inq_bindings(&binding_vectorP, stp);
    CHK_RPC_ERR(*stp);

#ifdef DEBUG
    compat_print_binding_vector(binding_vectorP);
#endif /* DEBUG */

    if (!uuid_is_nil(obj_uuidP, (unsigned32 *)stp) && *stp == uuid_s_ok) {
      object_uuid_vectorP = (uuid_vector_t *) osi_Alloc(sizeof(uuid_vector_t));
      if ( object_uuid_vectorP == 0 ) {
	  *stp = COMPAT_ERROR_NOMEM;
	  goto exit;
	}
      /* set the object uuid in the vector */
      object_uuid_vectorP->count = 1;
      object_uuid_vectorP->uuid[0] = obj_uuidP;
    }
    else
      object_uuid_vectorP = (uuid_vector_t *)NULL;

    /* now we can register the bindings. First with the end point mapper
     * i.e. rpcd.
     */
    rpc_ep_register(if_handle, binding_vectorP, object_uuid_vectorP,
		    (unsigned_char_t *)annotationP, stp);
    CHK_RPC_ERR(*stp);

exit:
    if (object_uuid_vectorP != (uuid_vector_t *)NULL)
      (void)osi_Free(object_uuid_vectorP, sizeof(uuid_vector_t));
    if (binding_vectorP != (rpc_binding_vector_t *)NULL)
      rpc_binding_vector_free(&binding_vectorP, &altSt);
    return;
}

/* dfs_register_rpc_server
 *      Register a server with the rpc runtime
 * entry:
 *      if_handle - interface spec
 *	mgr_epv - manager entry point vector
 *      entry_nameP - name service entry with which bindings will be registered
 *	adminPathP - path to admin list file
 *      maxcalls - max # calls
 *	annotationP - annotation to be entered into rpc ep database
 * exit:
 *	st - returned status. rpc_s_ok indicates success.
 * notes:
 *	currently we only use udp.
 *
 * 26 July 1991 (bab) - separated out the cds core so that a version of
 * rpc_register_dfs_server that used no authentication could be cleanly written
 * without requiring interface changes.
 *
 */

/* EXPORT */
void dfs_register_rpc_server(if_handle, manager_epv, obj_uuidP,
				    type_uuidP, maxcalls, adminPathP,
				    annotationP, stp)
     IN  rpc_if_handle_t if_handle;
     IN  rpc_mgr_epv_t manager_epv;
     IN  uuid_t *obj_uuidP;
     IN  uuid_t *type_uuidP;
     IN  int maxcalls;
     IN  char *adminPathP;
     IN  char *	annotationP;
     OUT error_status_t *stp;
{
  long		code = 0;
  static char	routineName[] = "dfs_register_rpc_server";

  INIT_SVC;

  *stp = rpc_s_ok;

  dfs_register_rpc_server_cds_core(if_handle, manager_epv, obj_uuidP,
				   type_uuidP, maxcalls, annotationP, stp);

  if (*stp == rpc_s_ok) {
    code = dfsauth_server_InitAuthentication((char *)NULL, adminPathP,
					     (char *)NULL);
    if ( code ) {
      dce_svc_printf(NCS_S_CC_AUTHENTICATION_FAILED_MSG,
		     routineName, dfs_dceErrTxt(code));
      *stp = code;
    }
  }

}

/*
 * The following function is called only from the fts client when it registers
 * a server for token revocation callbacks.
 */
/* dfs_register_rpc_server_noauth
 *      Register a server with the rpc runtime (without using the auth subsystem)
 *
 * entry:
 *      if_handle - interface spec
 *	mgr_epv - manager entry point vector
 *      entry_nameP - name service entry with which bindings will be registered
 *	adminPathP - path to admin list file
 *      maxcalls - max # calls
 *	annotationP - annotation to be entered into rpc ep database
 * exit:
 *	st - returned status. rpc_s_ok indicates success.
 * notes:
 *	currently we only use udp.
 *
 * 26 July 1991 (bab) - separated out the cds core so that a version of
 * rpc_register_dfs_server that used no authentication could be cleanly written
 * without requiring interface changes.
 *
 */

/* EXPORT */
void dfs_register_rpc_server_noauth(if_handle, manager_epv,
					   obj_uuidP, type_uuidP, maxcalls,
					   annotationP, stp)
     IN  rpc_if_handle_t if_handle;
     IN  rpc_mgr_epv_t manager_epv;
     IN  uuid_t *obj_uuidP;
     IN  uuid_t *type_uuidP;
     IN  int maxcalls;
     IN  char *	annotationP;
     OUT error_status_t *stp;
{
  long		code = 0;
  static char	routineName[] = "dfs_register_rpc_server_noauth";

  *stp = rpc_s_ok;

  dfs_register_rpc_server_cds_core(if_handle, manager_epv, obj_uuidP,
				   type_uuidP, maxcalls,
				   annotationP, stp);

#if 0
  /* Omit this for now since its only caller (fts) cannot use it. */
  /* Might be OK, except that dfsauth_server_InitAuth ignores the noAuth flag. */
  if (*stp == rpc_s_ok) {
    code = dfsauth_server_InitAuth((char *)NULL, (char *)NULL, (char *)NULL,
				   1 /* => noauth */);
    if ( code ) {
      dce_svc_printf(NCS_S_CC_AUTHENTICATION_FAILED_MSG,
		     routineName, dfs_dceErrTxt(code));
      *stp = code;
    }
  }
#endif /* 0 */

}

/* dfs_locate_rpc_server
 *	Return the bindings associated with a selected server.
 * entry:
 *	entry_nameP - hostname or CDS entry name
 * 	bindingP - ptr to a binding
 * 	localAuthFlag - true if local authentication
 *	noAuthFlag - true if we want no authentication
 * exit:
 *	bindingP - returned binding
 *	st - returned status. If == rpc_s_ok, then succeeded.
 */

/* EXPORT */
void dfs_locate_rpc_server(entry_nameP, bindingP, obj_uuidP,
				  localAuthFlag, noAuthFlag, stp)
     IN  unsigned_char_t *entry_nameP;
     IN  rpc_binding_handle_t *bindingP;
     IN  uuid_t *obj_uuidP;
     IN  int localAuthFlag, noAuthFlag;
     OUT error_status_t *stp;
{
    rpc_ns_lookup_handle_t lookup_contextP;
    rpc_binding_vector_t *binding_vectorP = 0;
    char *principalP;
    unsigned32 maxbindings = 10;
    rpc_binding_handle_t *handlePtr;
    int nbindings;
    long size;
    error_status_t unusedError;
    unsigned_char_t *binding_locP=0;
    char	*serverPrincipalName;
    int		dfsauthForcedNoauth = 0;
    int		shortNameNoauth = 0;
    boolean32	current_ns_authn;
    boolean32	ignored_ns_authn;
    int		set_ns_noauth = 0;
    error_status_t	rpc_ns_error;
    long code = 0;
    static char	routineName[] = "dfs_locate_rpc_server";

    INIT_SVC;

    /* If the client isn't authenticated, but we may try to get into the name space,
     * anyway, make sure the rpc_ns routines do not try to use authenticated bindings.
     */
    if ((noAuthFlag != 0) ||
	((localAuthFlag == 0) && (dfsauth_client_IsClientAuthenticated() == 0))) {
      /*
       * The rpc_ns_set_authn routine is not really an exported part of the rpc
       * library, but, until they fix the case in which there is no default binding,
       * and go ahead and try the name space access, we're sort of stuck with
       * having to use the private routine.
       */
      set_ns_noauth = 1;
      rpc_ns_set_authn(false, &current_ns_authn, stp);
      if (*stp != rpc_s_ok) {
	dce_svc_printf(NCS_S_CC_CANT_DISABLE_AUTHENTICATION_MSG, routineName);
	goto exit;
      }
    }

    if ( *entry_nameP == '/' )
    {
	code = dfsauth_client_InitAuthentication((rpc_binding_handle_t *)NULL,
						 (char *)NULL,
						 noAuthFlag,
						 localAuthFlag);
	if ( code )
	{
	  if (code == DAUT_ERROR_NO_CREDENTIALS_FOUND) {
	    *stp = (error_status_t)COMPAT_ERROR_NO_CREDENTIALS;
	  }
	  else {
	    dce_svc_printf(NCS_S_CC_CLIENT_AUTHENTICATION_FAILED_MSG,
			   dfs_dceErrTxt(code));
	    *stp = (error_status_t)code;
	  }

	  goto exit;
	}

	binding_locP = (unsigned char *) malloc (strlen((char *)entry_nameP) +
						 strlen(DFS_SELF_SUFFIX) + 1);
	if (binding_locP == (unsigned char *)NULL) {
	    *stp = (error_status_t)COMPAT_ERROR_NOMEM;
	    return;
	}

	code = compat_MakeDfsName ((char *)entry_nameP, (char *)binding_locP,
				   strlen((char *)entry_nameP)+strlen(DFS_SELF_SUFFIX) + 1,
				   compat_dfs_name_binding);
	if (code != 0) {
	    *stp = (error_status_t)code;
	    goto exit;
	}
	/* CDS path */
	rpc_ns_binding_lookup_begin(DFS_ENTRY_SYNTAX,
				    binding_locP,
				    NULL,
				    NULL,
				    maxbindings,
				    &lookup_contextP,
				    stp);
	if (*stp != rpc_s_ok) {
	    free( binding_locP );
	    goto exit;
        }


	rpc_ns_binding_lookup_next(lookup_contextP,
				   &binding_vectorP,
				   stp);
	if (*stp != rpc_s_ok) {
	    free ( binding_locP );
	    rpc_ns_binding_lookup_done(&lookup_contextP, &unusedError);
	    goto exit;
	}

	rpc_ns_binding_lookup_done(&lookup_contextP, stp);
	if (*stp != rpc_s_ok) {
	    free ( binding_locP );
	    rpc_binding_vector_free(&binding_vectorP, &unusedError);
	    goto exit;
	}

#ifdef DEBUG
	compat_print_binding_vector(binding_vectorP);
#endif /* DEBUG */

	/* remove protocols that DFS does not use (non-udp) */
	(void)compat_filter_binding_vector(binding_vectorP, stp);
	if (*stp != rpc_s_ok) {
	    free ( binding_locP );
	    rpc_binding_vector_free(&binding_vectorP, &unusedError);
	    goto exit;
	}

#ifdef DEBUG
	compat_print_binding_vector(binding_vectorP);
#endif /* DEBUG */

	code = compat_binding_vector_size(binding_vectorP, &size);
	if (code) {
	    *stp = (error_status_t)code;
	    free ( binding_locP );
	    rpc_binding_vector_free(&binding_vectorP, &unusedError);
	    goto exit;
	}

	/* XXX -
	   This routine does not handle the fact that there could be multiple
	   bindings associated with a host, and a binding vector should be
	   returned by this function and not a single binding. This affects the
	   ubik functionality because ubik servers could have multiple host
	   bindings, and clients should be able to make RPCs using any of these
	   bindings, and implementing their own recovery mechanisms upon
	   failure using any of these bindings. Changing this routine to return
	   binding vectors has large implications which changes required in
	   fts, bos, update, bak, scout and ubik. This should be handled some
	   time in the future, and this comment would then be removed. For now
	   I just comment out the check so that things would not break
	 */
#ifdef notdef
	/* there should be a single if spec */
	if ( size != 1 )
	{
	    dce_svc_printf(NCS_S_CC_MULTIPLE_INTERFACES_MSG, routineName, entry_nameP);
	    *stp = (error_status_t)rpc_s_max_descs_exceeded;
	    free ( binding_locP );
	    rpc_binding_vector_free(&binding_vectorP, &unusedError);
	    goto exit;
	}
#endif /* notdef */

	/* now extract the binding */
	nbindings = binding_vectorP->count;		/* no. slots */
	handlePtr = &binding_vectorP->binding_h[0];

#ifdef DEBUG
	compat_print_binding("test 1", *handlePtr);
#endif /* DEBUG */

	while ( nbindings )
	{
	    if ( *handlePtr != NULL )
		break;
	    handlePtr++;
	    nbindings--;
	}

	if ( *handlePtr == (rpc_binding_handle_t)NULL )
	{
	    /* failed to find the binding */
	    *stp = (error_status_t)COMPAT_ERROR_NOBINDINGS;
	    free ( binding_locP );
	    rpc_binding_vector_free(&binding_vectorP, &unusedError);
	    goto exit;
	}
#ifdef DEBUG
	compat_print_binding("test 2", *handlePtr);
#endif /* DEBUG */

	rpc_binding_copy(*handlePtr, bindingP, stp);
	if (*stp != rpc_s_ok) {
	    free ( binding_locP );
	    rpc_binding_vector_free(&binding_vectorP, &unusedError);
	    goto exit;
	}

	/* now fix up the binding we obtained */
	rpc_binding_reset(*bindingP, stp);
	if (*stp != rpc_s_ok) {
	    rpc_binding_free(bindingP, &unusedError);
	    free ( binding_locP );
	    rpc_binding_vector_free(&binding_vectorP, &unusedError);
	    goto exit;
	}

	/*
	 * The dfsauth_client_InitBindingAuth call takes a principal name,
	 * rather than server location, so we do that translation here.
	 */
	serverPrincipalName = (char *)malloc(strlen((char *)entry_nameP) +
					     strlen(DFS_DEFAULT_POSTFIX) + 1);
	if (serverPrincipalName == (char *)NULL) {
	    *stp = (error_status_t)COMPAT_ERROR_NOMEM;
	    rpc_binding_free(bindingP, &unusedError);
	    free ( binding_locP );
	    rpc_binding_vector_free(&binding_vectorP, &unusedError);
	    goto exit;
	}

	code = compat_MakeDfsName ((char *)entry_nameP,
				   serverPrincipalName,
				   strlen((char *)entry_nameP) + strlen(DFS_DEFAULT_POSTFIX) + 1,
				   compat_dfs_name_principal);
	if (code != 0) {
	    *stp = (error_status_t)code;
	    free(serverPrincipalName);
	    rpc_binding_free(bindingP, &unusedError);
	    free ( binding_locP );
	    rpc_binding_vector_free(&binding_vectorP, &unusedError);
	    goto exit;
	}

	code = dfsauth_client_InitBindingAuth(bindingP,
					      noAuthFlag,
					      localAuthFlag,
					      serverPrincipalName);
	if ( code )
	{
	  if (code == DAUT_ERROR_NO_CREDENTIALS_FOUND) {
	    *stp = (error_status_t)COMPAT_ERROR_NO_CREDENTIALS;
	  }
	  else {
	    dce_svc_printf(NCS_S_CC_CLIENT_BINDING_AUTH_FAILED_MSG,
			   dfs_dceErrTxt(code));
	    *stp = (error_status_t)code;
	  }

	  free(serverPrincipalName);
	  rpc_binding_free(bindingP, &unusedError);
	  free ( binding_locP );
	  rpc_binding_vector_free(&binding_vectorP, &unusedError);
	  goto exit;
	}

	if (!uuid_is_nil(obj_uuidP, (unsigned32 *)stp) && *stp == uuid_s_ok){
	  rpc_binding_set_object(*bindingP, obj_uuidP, stp);
	  if (*stp != rpc_s_ok) {
	    free(serverPrincipalName);
	    rpc_binding_free(bindingP, &unusedError);
	    free ( binding_locP );
	    rpc_binding_vector_free(&binding_vectorP, &unusedError);
	    goto exit;
	  }
	}
	else if (*stp != uuid_s_ok) {
	  free(serverPrincipalName);
	  rpc_binding_free(bindingP, &unusedError);
	  free ( binding_locP );
	  rpc_binding_vector_free(&binding_vectorP, &unusedError);
	  goto exit;
	}

	/* We are done, free all resources */
	free(serverPrincipalName);
	free ( binding_locP );
	rpc_binding_vector_free(&binding_vectorP, &unusedError);
    }
    else  {
	unsigned_char_t *stringBindingP = 0;
	struct hostent *hostP;
	struct in_addr inetAddr;
	char *inAddrString;
	char **ptr;

	/* hostname */
	hostP = gethostbyname((char *)entry_nameP);
	if ( hostP == 0 )
	{
	    *stp = (error_status_t)COMPAT_ERROR_MALFORMED_HOSTNAME;
	    goto exit;
	}

	ptr = hostP->h_addr_list;
	inetAddr = *((struct in_addr *) *ptr);
	inAddrString = inet_ntoa(inetAddr);
	rpc_string_binding_compose(NULL, 		/* obj uuid */
	       (unsigned char *)   DFS_PROTSEQ,		/* prot seq */
	       (unsigned char *)   inAddrString,	/* addr */
				   NULL,		/* endpoint */
				   NULL,		/* options */
				   &stringBindingP,
				   stp);
	if (*stp != rpc_s_ok)
	    goto exit;

	/* turn it into a real binding */
	rpc_binding_from_string_binding(stringBindingP, bindingP, stp);
	if (*stp != rpc_s_ok) {
	    rpc_string_free(&stringBindingP, &unusedError);
	    goto exit;
	}

	/*
	 * In this case, we have no way to form a principal name (yet?)
	 * so we have to go noauth
	 */
	if (noAuthFlag == 0) {
	  /* only do warning if caller hasn't explicitly requested noauth already */
	  shortNameNoauth = 1;
	}
	code = dfsauth_client_InitAuthentication(bindingP,
				          (char *) entry_nameP,
						 1 /* noAuthFlag */,
						 0 /* localAuthFlag */);

	if ( code )
	{
	    dce_svc_printf(NCS_S_CC_CLIENT_AUTH_FAILED_FOR_MSG,
			   routineName, entry_nameP, dfs_dceErrTxt(code));
	    *stp = (error_status_t)code;
	    rpc_string_free(&stringBindingP, &unusedError);
	    rpc_binding_free(bindingP, &unusedError);
	    goto exit;
	}
	if (!uuid_is_nil(obj_uuidP, (unsigned32 *)stp) && *stp == uuid_s_ok) {
	  rpc_binding_set_object(*bindingP, obj_uuidP, stp);
	  if (*stp != rpc_s_ok) {
	    rpc_string_free(&stringBindingP, &unusedError);
	    rpc_binding_free(bindingP, &unusedError);
	    goto exit;
	  }
	}
	else if (*stp != uuid_s_ok) {
	  rpc_string_free(&stringBindingP, &unusedError);
	  rpc_binding_free(bindingP, &unusedError);
	  goto exit;
	}
      }

exit:
    if (set_ns_noauth != 0) {
      /* in this case, we had disabled name space authn, so restore it before return */
      /* see comments on previous use of rpc_ns_set_authn, above */
      rpc_ns_set_authn(current_ns_authn, &ignored_ns_authn, &rpc_ns_error);
      if (rpc_ns_error != rpc_s_ok) {
	dce_svc_printf(NCS_S_CC_UNABLE_TO_RESET_AUTH_MSG, routineName, rpc_ns_error);
	/* only send this one back if it doesn't mask others */
	if (*stp == 0) {
	  *stp = (error_status_t)rpc_ns_error;
	}
      }
    }

    if ((*stp == error_status_ok) && (shortNameNoauth != 0)) {
      *stp = (error_status_t)COMPAT_ERROR_SHORTNAME_NOAUTH;
    }

    return;
}


/* ------------------
 * misc. utilities
 * ------------------
 */

/* compat_get_group_object_uuid
 * entry:
 *	groupNameP - group to look at
 * exit:
 *	objectUuidP - returned object uuid
 * return value:
 *	0 - ok
 *	n - error
 */

/* EXPORT */
void compat_get_group_object_uuid(groupNameP, object_uuidP, stP)
     IN unsigned_char_t *groupNameP;
     OUT uuid_t *object_uuidP;
     OUT error_status_t *stP;
{
    rpc_ns_handle_t inquiry_context;
    unsigned32 unusedSt;                   /* returned status */
    int numObjects = 0;

    /* get the objects exported by the name service entry referenced
     * by group_nameP
     */
    rpc_ns_entry_object_inq_begin(DFS_ENTRY_SYNTAX, groupNameP,
                                  &inquiry_context, stP);
    if (*stP != rpc_s_ok)
      goto exit;

    /*
     * We would like to keep track of any changes made to the object UUID
     * before start. By pass the local cache and read directly from the server.
     */
    rpc_ns_mgmt_handle_set_exp_age(inquiry_context, (unsigned32)0, stP);
    /* don't bother with failures */

    /* there should be a single object uuid attached to the entry selected */
    while ( 1 ) {
	rpc_ns_entry_object_inq_next(inquiry_context, object_uuidP, stP);
	if (*stP != rpc_s_ok) {
	  if (*stP == rpc_s_no_more_members)
	    break;
	  else {
	    rpc_ns_entry_object_inq_done(&inquiry_context, &unusedSt);
	    goto exit;
	  }
	}
	numObjects++;
    }

    /* should be a single object, and have terminated normally */
    if (numObjects > 1) {
      *stP = COMPAT_ERROR_TOO_MANY_OBJECTS;
      goto exit;
    }

    rpc_ns_entry_object_inq_done(&inquiry_context, stP);
    if (*stP != rpc_s_ok)
      goto exit;

exit:
    return;
}

/* compat_filter_binding_vector
 *	remove all bindings from the binding vector that do not use
 * 	udp. XX Is this necessary any more?
 */

/* SHARED */
void compat_filter_binding_vector(binding_vectorP, stP)
     INOUT rpc_binding_vector_t *binding_vectorP;
     OUT error_status_t *stP;
{
    register rpc_binding_handle_t *srcHandlePtr, *destHandlePtr;
    unsigned_char_t *stringBindingP;
    unsigned_char_t *protseq;
    int nbindings;
    error_status_t unusedSt;

    *stP = rpc_s_ok;
    nbindings = binding_vectorP->count;
    srcHandlePtr = destHandlePtr = &binding_vectorP->binding_h[0];

    while ( nbindings-- )
    {
	if ( *srcHandlePtr == 0 )
	{
	    /* empty slot */
	    ++srcHandlePtr;
	    binding_vectorP->count--;
	    continue;
	}

	/* convert the binding to a string binding */
	rpc_binding_to_string_binding(*srcHandlePtr, &stringBindingP, stP);
	if (*stP != rpc_s_ok)
	  goto exit;

	/* break apart the string binding */
	rpc_string_binding_parse(stringBindingP,
				 NULL, &protseq,
				 NULL, NULL, NULL, stP);
	if (*stP != rpc_s_ok) {
	    rpc_string_free(&stringBindingP, &unusedSt);
	    goto exit;
	}

	/* check the protseq, the only one supported */
	if ( (strcmp("ncadg_ip_udp", (char *)protseq) != 0)
	&&   (strcmp("ip", (char *)protseq) != 0)
	   )
	{
	    /* free this binding */
	    rpc_binding_free(srcHandlePtr, &unusedSt);
	    *srcHandlePtr = 0;
	    binding_vectorP->count--;
	} else {
	    /* avoid any possible pointer-aliasing problems */
	    if (destHandlePtr != srcHandlePtr)
		*destHandlePtr = *srcHandlePtr;
	    ++destHandlePtr;
	}

	++srcHandlePtr;

	/* Now free allocated resources for the next iteration */
	rpc_string_free(&stringBindingP, &unusedSt);
	rpc_string_free(&protseq, &unusedSt);
    }

exit:
    return;
}

/* compat_binding_vector_size
 *	Determine the (actual) number of bindings in a binding vector
 * entry:
 *	binding_vectorP - ptr to binding vector
 * exit:
 *	size - set to number of actual bindings present
 */

/* EXPORT */
int compat_binding_vector_size(binding_vectorP, size)
     IN  rpc_binding_vector_t *binding_vectorP;
     OUT long *size;
{
    rpc_binding_handle_t binding;
    unsigned32 i;

    *size = 0;
    if (!binding_vectorP) return -1;
    for ( i = 0; i < binding_vectorP->count; i++ )
    {
        if(binding_vectorP->binding_h[i])
	    (*size)++;
    }
    return(0);
}


/* ------------
 * Debug support
 * ------------
 */

/* EXPORT */
void compat_print_binding(infoStP, binding)
     char *infoStP;
     rpc_binding_handle_t binding;
{
    unsigned_char_t *string_bindingP;
    unsigned32 st;

    INIT_SVC;

    rpc_binding_to_string_binding(binding, &string_bindingP, &st);
    if ( st != rpc_s_ok )
	return;
    dce_svc_printf(NCS_S_CC_BINDING_IS_MSG, infoStP, string_bindingP);
    rpc_string_free(&string_bindingP, &st);
}

/* EXPORT */
void compat_print_binding_vector(bindingVectorP)
     rpc_binding_vector_t *bindingVectorP;
{
    rpc_binding_handle_t *handlePtr;
    int nbindings;

    INIT_SVC;

    nbindings = bindingVectorP->count;
    handlePtr = &bindingVectorP->binding_h[0];

    dce_svc_printf(NCS_S_CC_BINDING_VECTOR_SIZE_MSG, nbindings);
    while ( nbindings-- )
    {
	if ( *handlePtr != NULL )
	{
	    compat_print_binding("", *handlePtr);
	}
	handlePtr++;
    }
}

/* print_registered_interfaces
 *	print all the interfaces registered
 * entry:
 *	NULL - interfaces registered by this server
 *	non-NULL - interfaces registered by entity whose binding is passed in
 */

static void print_registered_interfaces(binding)
     rpc_binding_handle_t binding;
{
    rpc_if_id_vector_t	*if_vectorP = 0;
    rpc_if_id_t		*if_idP;
    unsigned_char_t	*string_uuidP;
    unsigned32 count;
    int i;
    unsigned32 st;

    INIT_SVC;

    rpc_mgmt_inq_if_ids(binding, &if_vectorP, &st);
    CHK_RPC_ERR(st);

    count = if_vectorP->count;
    i = 0;

    pthread_lock_global_np();
    dce_svc_printf(NCS_S_CC_REGISTERED_INTERFACE_COUNT_MSG, count);
    pthread_unlock_global_np();

    while ( count-- )
    {
	if_idP = if_vectorP->if_id[i++];
	if ( if_idP == NULL )
	    continue;

	uuid_to_string(&if_idP->uuid, &string_uuidP, &st);
	CHK_RPC_ERR(st);

	pthread_lock_global_np();
	dce_svc_printf(NCS_S_CC_INTERFACE_MSG, if_idP, string_uuidP, if_idP->vers_major, if_idP->vers_minor);
	pthread_unlock_global_np();
	rpc_string_free(&string_uuidP, &st);
    }

exit:
    if ( if_vectorP )
	rpc_if_id_vector_free(&if_vectorP, &st);
}

/* print_if_handle
 *	take an interface handle (i.e. interface spec) and print out
 *	the interface uuid and version numbers
 *
 */

static int print_if_handle(if_handle)
     rpc_if_handle_t	if_handle;
{
    rpc_if_id_t	if_id;
    unsigned_char_t	*string_uuidP = NULL;
    unsigned32 st;

    INIT_SVC;

    rpc_if_inq_id(if_handle, &if_id, &st);
    CHK_RPC_ERR(st);

    uuid_to_string(&if_id.uuid, &string_uuidP, &st);
    CHK_RPC_ERR(st);

    dce_svc_printf(NCS_S_CC_IF_HANDLE_MSG, string_uuidP, if_id.vers_major, if_id.vers_minor);

exit:
    if ( string_uuidP != NULL )
	rpc_string_free(&string_uuidP, &st);
    return(0);
}

/* EXPORT */
int compat_print_uuid(uuid)
  uuid_t uuid;
{
    unsigned_char_t *uuidStr;
    unsigned32 st;

    INIT_SVC;

    uuid_to_string(&uuid, &uuidStr, &st);
    CHK_RPC_ERR(st);
    dce_svc_printf(NCS_S_CC_UUID_MSG, uuidStr);
    if (uuidStr)
	rpc_string_free(&uuidStr, &st);
 exit:
    return(0);
}

/* EXPORT */
char *compat_print_hostname_from_binding (binding)
  rpc_binding_handle_t binding;

{
    unsigned_char_t *string_binding, *network_addr, *prot_seq;
    struct hostent *he;
    unsigned32 code;
    unsigned long addr;

    if (!binding) return (char *)0;

    rpc_binding_to_string_binding (binding,
				   &string_binding,
				   &code);
    if (code) return (char *)0;

    rpc_string_binding_parse (string_binding,
			      0,
			      &prot_seq,
			      &network_addr,
			      0,
			      0,
			      &code);
    if (code) return (char *)0;
    addr = inet_addr((char *)network_addr);
    he = gethostbyaddr((char *)&addr, sizeof(addr), AF_INET);
    rpc_string_free(&string_binding, &code);
    rpc_string_free(&network_addr, &code);
    if (he)
	return (he->h_name);
    else
	return (char *)0;
}
