/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ubikclient.c,v $
 * Revision 1.1.111.1  1996/10/02  17:56:07  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:42:55  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1996 Transarc Corporation, All rights reserved. */
/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987, 1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */
#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>

#include <dcedfs/compat.h>

#include <dce/rpc.h>
#include <pthread.h>
#include <dce/secsts.h>
#include <dce/rgybase.h>
#include <dce/binding.h>
#include <dcedfs/dfsauth.h>
#include <dcedfs/bomb.h>

#define DFS_SELF_SUFFIX		"/self"
#define UBIK_INTERNALS
#define UBIK_MAXCHASE		20	/* max times to chase sync site */
#define UBIK_MAXPASS		2	/* max times try the servers */
#define UBIK_CHASEDELAY		2	/* delay when chasing sync site  */
#define UBIK_QUORUMDELAY	2	/* delay when waiting for quorum */

#include <ubik.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/ncsubik/ubikclient.c,v 1.1.111.1 1996/10/02 17:56:07 damon Exp $")

extern void ubik_hold(), ubik_rele();
/* macro to print debug messages; define as "#define dfprintf(x) printf x" */
#define dfprintf(x)
/* splice for principal name */
#define	DFS_PSPLICE		"/dfs-server"
/* The following definition obviously has to track rpc error code changes */

/* ubik_get_pname
 *	get principal name, and/or it's components, from a binding.
 * entry:
 *	srvrBinding - from which to get the CDS name
 * exit
 *	pNamePP - contains (complete) principal name. This consists of
 *		cellname, dfs splice, tail
 *	cellNamePP - contains cellname component of principal name
 *	tailPP - contains suffix for principal name
 *	pUuidP - contains principal uuid (if pointer not NULL)
 *	cellUuidP - contains cell uuid (if pointer not NULL)
 *	stP - return status
 *
 * This is a version of this function called ubik_get_pname that is
 * identical except that it doesn't have the two uuid return parameters.
 */

/* SHARED */
void ubik_get_pname_and_uuid(srvrBinding, rgyHandle, fullPrinNamePP,
			   pNamePP, cellNamePP, pUuidP, cellUuidP, stP)
     IN  rpc_binding_handle_t srvrBinding;
     IN  sec_rgy_handle_t rgyHandle;
     OUT char **fullPrinNamePP, **pNamePP;
     OUT char **cellNamePP;
     OUT uuid_t *pUuidP;
     OUT uuid_t *cellUuidP;
     OUT error_status_t *stP;
{
    sec_rgy_name_t	cellName;
    sec_rgy_name_t	prinName;
    unsigned_char_t    *entryNameP = 0;
    long		code = 0;
    unsigned int        slen;
    unsigned_char_t	*parseName = 0;
    unsigned32 st;

    /* extract server's CDS path from binding */
    rpc_ns_binding_inq_entry_name(srvrBinding, DFS_ENTRY_SYNTAX,
				  &entryNameP, stP);
    if (*stP != rpc_s_ok)
	goto exit;
    slen = strlen((char *)entryNameP) + strlen(DFS_PSPLICE) + 3;
    parseName = (unsigned_char_t *) malloc(slen);
    if (parseName == 0) {
      *stP = UNOMEM;
      rpc_string_free(&entryNameP, &st);
      goto exit;
    }
    code = compat_MakeDfsName((char *)entryNameP, (char *)parseName,
			      slen, compat_dfs_name_principal);
    if (code) {
      *stP = code;
      rpc_string_free(&entryNameP, &st);
      free(parseName);
      goto exit;
    }

    if (fullPrinNamePP)
	*fullPrinNamePP = (char *)parseName;

    /* split it into components */
    sec_id_parse_name(rgyHandle, parseName, cellName, cellUuidP,
		      prinName, pUuidP, stP);
    if (*stP != error_status_ok) {
      /*
       * We probably have to handle foreign cell referral errors here
       */
      if (*stP == sec_login_s_no_current_context) { /* try unauthenticated */
	/*
	 * well, this shouldn't really happen because we took care of this
	 * earlier.
	 */
	sec_rgy_bind_auth_info_t bindingAuthInfo;
	sec_rgy_handle_t secdHandle;
	sec_rgy_name_t cellDirName;

	/* We are using our local cell here to resolve the 'parseName'. We
	 * might get foreign cell referral errors here. Not clear at present
	 * how to handle this.
	 */
	bindingAuthInfo.info_type = sec_rgy_bind_auth_none;
	(void) strcpy((char *)cellDirName, CELL_DIR_ROOT);
	sec_rgy_site_bind(cellDirName, &bindingAuthInfo, &secdHandle, stP);
	if (*stP != error_status_ok) {
	  rpc_string_free(&entryNameP, &st);
	  free(parseName);
	  *fullPrinNamePP = (char *)0;
	  goto exit;
	}
	sec_id_parse_name(secdHandle, parseName, cellName, cellUuidP, prinName,
			  pUuidP, stP);
	if (*stP != error_status_ok) { /* we give up now */
	  rpc_string_free(&entryNameP, &st);
	  free(parseName);
	  *fullPrinNamePP = (char *)0;
	  goto exit;
	}
      } else { /* some other error occurred */
	  rpc_string_free(&entryNameP, &st);
	  free(parseName);
	  *fullPrinNamePP = (char *)0;
	  goto exit;
      }
    }

    /* save the requested items */
    if (cellNamePP) {
	/* copy out cell name */
	*cellNamePP = (char *) malloc(strlen((char *)cellName)+1);
	if (*cellNamePP == 0) {
	  *stP = UNOMEM;
	  rpc_string_free(&entryNameP, &st);
	  free(parseName);
	  *fullPrinNamePP = (char *)0;
	  goto exit;
	}
	strcpy(*cellNamePP,(char *)cellName);
    }

    if (pNamePP) {
	*pNamePP = (char *)malloc(strlen((char *)prinName)+1);
	if (*pNamePP == 0) {
	  *stP = UNOMEM;
	  rpc_string_free(&entryNameP, &st);
	  free(parseName);
	  free(*cellNamePP);
	  *fullPrinNamePP = *cellNamePP = (char *)0;
	  goto exit;
	}
	strcpy(*pNamePP, (char *)prinName);
    }
    rpc_string_free(&entryNameP, &st);

exit:
    return;
}

/* SHARED */
void ubik_get_pname(srvrBinding, rgyHandle, fullPrinNamePP,
			   pNamePP, cellNamePP, stP)
     IN  rpc_binding_handle_t srvrBinding;
     IN  sec_rgy_handle_t rgyHandle;
     OUT char **fullPrinNamePP, **pNamePP;
     OUT char **cellNamePP;
     OUT error_status_t *stP;
{
    ubik_get_pname_and_uuid(srvrBinding, rgyHandle, fullPrinNamePP,
			    pNamePP, cellNamePP, /*pUuidP*/NULL,
			    /*cellUuidP*/NULL, stP);
}

/*
 * shuffle the clients bindings to move the binding specified by
 * offset to the front of the list so it will be tried first from
 * now on.
 */

SHARED void ubik_shuffleClientBindings (clientP, server, offset)
     IN struct ubik_client *clientP;
     IN int server;
     IN int offset;
{
  int i;
  rpc_binding_handle_t tempHandle;
  unsigned long tempSockAddr;
  char tempFlag;
  
  if (!clientP || offset < 1 || offset >= UBIK_MAXBINDINGS ||
      server < 0 || server >= clientP->nServers )
    return; /* don't bother reporting error */

  tempHandle = clientP->conns[server][offset];
  tempSockAddr = clientP->addrs[server][offset];
  tempFlag = clientP->authflag[server][offset];
  for ( i = offset ; i > 0 ; i-- ) {
    clientP->conns[server][i] = clientP->conns[server][i-1];
    clientP->addrs[server][i] = clientP->addrs[server][i-1];
    clientP->authflag[server][i] = clientP->authflag[server][i-1];
  }
  clientP->conns[server][0] = tempHandle;
  clientP->addrs[server][0] = tempSockAddr;
  clientP->authflag[server][0] = tempFlag;
}

/* ubik_auth_binding
 *	 do the necessary authentication for a ubik binding
 */

/* SHARED */
void ubik_auth_binding(bindingP, rgyHandle, cellNameP, fullpNameP,
			      pNameP, secGroupP, stP)
     IN  rpc_binding_handle_t bindingP;
     IN  sec_rgy_handle_t rgyHandle;
     IN  unsigned_char_t *cellNameP;
     IN  unsigned_char_t *fullpNameP;
     IN  unsigned_char_t *pNameP;
     IN  unsigned_char_t *secGroupP;
     OUT error_status_t *stP;
{
    long ismember;

#ifdef DEBUG
    compat_print_binding("Ubik conn: ", bindingP);
#endif /* DEBUG */

    /* authenticate the binding */
    rpc_binding_set_auth_info(bindingP, fullpNameP,
			      rpc_c_authn_level_default,
			      rpc_c_authn_dce_private,
			      (rpc_auth_identity_handle_t)NULL,
			      rpc_c_authz_dce,
			      stP);
    CHK_RPC_ERR(*stP);

#ifdef DEBUG
    compat_print_binding("Ubik conn: ", bindingP);
#endif /* DEBUG */
    if (secGroupP) {
      /* have specified a group to check against */
      ismember = sec_rgy_pgo_is_member(rgyHandle,
				       sec_rgy_domain_group,
				       secGroupP,
				       pNameP,
				       stP);
      CHK_RPC_ERR(*stP);
      if (ismember == false)
	*stP = UNOTMEMBER;
    } else
      *stP = UBADSECGRP;

exit:
    return;
}

/* ubik_auth_server_bindings
 *	Authenticate the bindings for the selected server. The bindings
 *	are authenticated in parallel to reduce startup latency when there
 *      is more than one binding for a server. We authicate the bindings
 *      all at once, then use any binding that works.
 */

typedef struct ubik_auth_server_arg {
    struct ubik_client *clientP;	/* client parameters */
    long server;			/* server index */
    int binding;			/* binding index */
} ubik_auth_server_arg;

/* thread to perform the authentication */
pthread_addr_t ubik_auth_server_thread(parm)
    IN  pthread_addr_t parm;
{
    ubik_auth_server_arg *autharg;
    struct ubik_client *clientP;
    long server;
    int binding;
    error_status_t st;

#ifdef DEBUG
    compat_print_binding("Ubik conn: ", clientP->conns[server][binding]);
#endif /* DEBUG */

    /*
     * copy the parameters
     */
    autharg = (ubik_auth_server_arg *)parm;
    clientP = autharg->clientP;
    server = autharg->server;
    binding = autharg->binding;

    /*
     * try to authenticate the binding
     */
    ubik_auth_binding(clientP->conns[server][binding], clientP->rgyHandle,
		      clientP->cellName, clientP->fullname[server],
		      clientP->principals[server], clientP->secGroup, &st);

    /*
     * return the status code
     */

    return((pthread_addr_t)st);
}

/* SHARED */
void ubik_auth_server_bindings(clientP, server)
     IN  struct ubik_client *clientP;
     IN  long server;
{
    int i;
    long code;
    error_status_t st;
    ubik_auth_server_arg authargs[UBIK_MAXADDRS];
    pthread_t threads[UBIK_MAXADDRS];
    int running[UBIK_MAXADDRS];

    if ( clientP->states[server] & CFAuthDone ) return;

    /*
     * start a thread to authenticate each binding for this server
     */
    for ( i = 0 ; i < UBIK_MAXADDRS ; i++ ) {
	if ( clientP->conns[server][i] ) {
	    authargs[i].clientP = clientP;
	    authargs[i].server = server;
	    authargs[i].binding = i;
	    code = pthread_create(&threads[i],pthread_attr_default,
				  ubik_auth_server_thread,
				  (pthread_addr_t)(&authargs[i]));
	    if (!code) running[i] = 1;
	    else running[i] = 0;
	}
    }

    /*
     * Wait for each thread to complete
     */
    for ( i = 0 ; i < UBIK_MAXADDRS ; i++ ) {
	if ( clientP->conns[server][i] ) {
	    if ( running[i] ) {
	        code = pthread_join(threads[i],(pthread_addr_t *)&st);
	        pthread_detach(&threads[i]);
	    } else code = 0;
	    if ( (code || !running[i]) ) {
	        /* problem with thread, authenticate serially */
	        st = (error_status_t)
		        ubik_auth_server_thread((pthread_addr_t)(&authargs[i]));
	    }
	    if ( !st ) {
	        clientP->authflag[server][i] |= BFAuthPass;
		clientP->states[server] |= CFAuthBinding;
	    }
	}
    }
    clientP->nAuthHosts++;
    clientP->states[server] |= CFAuthDone;

    return;
}


/* ubik_ClientInit
 *	Initialize a ubik client. The number of server bindings actually
 *	present is implicit.
 */

/* EXPORT */
int ubik_ClientInit(ifspec, nsGroupP, securityGroupP, noAuth, localAuth,
			   initAuth, aclient)
     IN  rpc_if_handle_t ifspec;
     IN  unsigned_char_t *nsGroupP;
     IN  unsigned_char_t *securityGroupP;
     IN  int noAuth, localAuth;
     IN  int initAuth;
     OUT struct ubik_client **aclient;
{
    int i, j, k;
    int offset, authhosts = 0;
    struct ubik_client *tc = (struct ubik_client *)0;
    struct sockaddr_in serverAddrs[UBIK_MAXSERVERS];
    struct sockaddr_in tempSockAddr;
    unsigned long tempAddr;
    ubik_binding_vector_t *bindingVectorP = NULL;
    rpc_binding_handle_t *selectedBinding;
    char *cellNameP = 0;
    char *fullPrinNameP = 0;
    char *prinNameP = 0;
    long maxBindings = UBIK_MAXSERVERS*3;
    unsigned32 st, unusedSt;
    sec_rgy_name_t nsCellName;
    sec_rgy_bind_auth_info_t ubik_sec_auth_info;
    uuid_t princUuid;
    uuid_t realmUuid;
    long code = 0;

    /* Initialize ICL ubik tracing, ignore return code */
    code = setupICL();

    ubik_hold();
    tc = (struct ubik_client *) osi_Alloc(sizeof(struct ubik_client));
    if (tc == (struct ubik_client *)NULL) {
	code = UNOMEM;
	goto error_exit;
    }
    bzero((char *)tc, sizeof(*tc));

    /*
     * copy the security group name into the client structure
     */
    tc->secGroup = (char *)osi_Alloc(strlen((char *)securityGroupP)+1);
    if ( tc->secGroup == NULL ) {
	code = UNOMEM;
	goto error_exit;
    }
    (void)strcpy(tc->secGroup, (char *)securityGroupP);

    ubik_GetServerList(nsGroupP, ifspec, (char)0, &bindingVectorP, &st);
    if (st != rpc_s_ok) {
	code = st;
	goto error_exit;
    }

    if (bindingVectorP->count > UBIK_MAXSERVERS) {
	code = UNHOSTS;			/* too many servers */
	goto error_exit;
    }

    if (initAuth) { /* do this if not done already by the client */
      /* establish authentication for ourselves */
      code = dfsauth_client_InitAuthentication((rpc_binding_handle_t *)NULL,
					       (char *)NULL,
					       noAuth, localAuth);
      if (code) {
	fprintf(stderr,
		"ubik_ClientInit: can't init auth (%d, %d): code %lu: %s\n",
		noAuth, localAuth, code, dfs_dceErrTxt(code));
	goto error_exit;
      }
    }

    /* Generate a registry handle for the appropriate site */
    /* First use our cell to find out the name of the other cell */
    bzero((char *)& ubik_sec_auth_info, sizeof(sec_rgy_bind_auth_info_t));
    ubik_sec_auth_info.info_type = sec_rgy_bind_auth_none;
    (void) strcpy((char *)nsCellName, CELL_DIR_ROOT);
    sec_rgy_site_bind(nsCellName, &ubik_sec_auth_info, &tc->rgyHandle, &st);
    if (st != error_status_ok) {
	code = st;
	goto error_exit;
    }
    sec_id_parse_name(tc->rgyHandle, nsGroupP, nsCellName, (uuid_t *)0,
		      NULL, (uuid_t *)0, &st);
    sec_rgy_site_close(tc->rgyHandle, &unusedSt);
    if (st != error_status_ok) {
	code = st;
	goto error_exit;
    }
    /* Now that we have cell, get handle with authentication to that cell */
    ubik_sec_auth_info.info_type = sec_rgy_bind_auth_dce;
    ubik_sec_auth_info.tagged_union.dce_info.authn_level = \
      rpc_c_authn_level_default;
    ubik_sec_auth_info.tagged_union.dce_info.authn_svc = \
      rpc_c_authn_dce_private;
    ubik_sec_auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    ubik_sec_auth_info.tagged_union.dce_info.identity = NULL;

    sec_rgy_site_bind (nsCellName, &ubik_sec_auth_info, &tc->rgyHandle, &st);
    if (st != error_status_ok) {
	if (st == sec_login_s_no_current_context || st == rpc_s_auth_tkt_expired
	    || st == sec_rgy_server_unavailable) {
	/*
	 * workaround implemented to obtain an unauthenticated connection
	 * to the ubik server, if the user does not have any valid
	 * credentials.
	 */
	noAuth = 1;
	bzero((char *)&ubik_sec_auth_info, sizeof(sec_rgy_bind_auth_info_t));
	ubik_sec_auth_info.info_type = sec_rgy_bind_auth_none;
	sec_rgy_site_bind(nsCellName, &ubik_sec_auth_info, &tc->rgyHandle, &st);
	if (st != error_status_ok) {
	    code = st;
	    goto error_exit;
	}
      } else {
	  code = st;
	  goto error_exit;
      }
    }

    /* Save a copy of the bindings. This is an "unnecessary" copy, but
     * conveniently serves to randomize the bindings and to drop
     * any null bindings that may be in the vector
     */
    tc->rgyValid = 1;
    tc->nServers = bindingVectorP->count;
    tc->nAuthHosts = 0;

    for (i=0; i<bindingVectorP->count; i++) {
	char *entry_nameP;

	selectedBinding = (rpc_binding_handle_t *)&(bindingVectorP->s[i].binding_h[0]);
	for (j = 0; j < bindingVectorP->s[i].count && j < UBIK_MAXADDRS; j++) {
	    rpc_binding_copy(selectedBinding[j], &(tc->conns[i][j]), &st);
	    if (st != rpc_s_ok) {
		code = st;
		goto error_exit;
	    }

	    /*
	     * Save the original IP address from each binding handle, the
	     * RPC runtime may change the address in the binding handle.
	     * If we can't get an IP address out of the binding, don't
	     * panic, it probably isn't an IP binding handle.
	     */
	    if (!rpcx_binding_to_sockaddr (selectedBinding[j],
					   &tempSockAddr)) {
	        if (!rpcx_ipaddr_from_sockaddr (&tempSockAddr,
						(char *)&tempAddr)) {
		    tc->addrs[i][j] = ntohl(tempAddr);
		} else {
		    tc->addrs[i][j] = 0xFFFFFFFF;
		}
	    } else {
		tc->addrs[i][j] = 0xFFFFFFFF;
	    }
	    
	    ubik_get_pname_and_uuid(selectedBinding[j], tc->rgyHandle,
				    &fullPrinNameP, &prinNameP, &cellNameP,
				    &princUuid, &realmUuid, &st);
	    if (st != rpc_s_ok) {
		code = st;
		goto error_exit;
	    }
#ifdef DEBUG
	    compat_print_binding("Ubik conn: ", selectedBinding[j]);
#endif /* DEBUG */

	    /* now check the cell name component and save it if reqd */
	    if (tc->cellName == 0) {
		tc->cellName = (char *)osi_Alloc(strlen(cellNameP)+1);
		if (!tc->cellName) {
		    code = UNOMEM;
		    goto error_exit;
		}
		strcpy(tc->cellName, cellNameP);
		free(cellNameP); cellNameP = 0;
	    } else {
		/* do a consistency check - all servers must be in same cell */
		if (strcmp(tc->cellName, cellNameP) != 0) {
		    code = UBADCELL;
		    goto error_exit;
		}
		free(cellNameP); cellNameP = 0;
	    }

	    /* do a consistency check between the cell name from the bindings and
	     * the cell name from the server group path name.
	     * This could be a foreign cell, so can't use the default handle.
	     */

	    if (strcmp(tc->cellName, (char *)nsCellName) != 0) {
		code = UBADCELL;
		goto error_exit;
	    }

	    /* save variant portion of principal */
	    if (tc->principals[i] == 0) {
		tc->principals[i] = (char *)osi_Alloc(strlen(prinNameP)+1);
		if (!tc->principals[i]) {
		    code = UNOMEM;
		    goto error_exit;
		}
		(void)strcpy(tc->principals[i], prinNameP);
	    } else {
		if (strcmp(tc->principals[i], prinNameP) != 0) {
		    code = UBADPRINNAME;
		    goto error_exit;
		}
	    }

	    /* save the full name of principal and the principals identity */
	    if (tc->fullname[i] == 0) {
		tc->fullname[i] = (char *)osi_Alloc(strlen(fullPrinNameP)+1);
		if (!tc->fullname[i]) {
		    code = UNOMEM;
		    goto error_exit;
		}
		(void)strcpy(tc->fullname[i], fullPrinNameP);
		tc->princUuid[i] = princUuid;
		tc->realmUuid[i] = realmUuid;
	    } else {
		if (strcmp(tc->fullname[i], fullPrinNameP) != 0 ||
		    !uuid_equal(&tc->princUuid[i], &princUuid, &unusedSt) ||
		    !uuid_equal(&tc->realmUuid[i], &realmUuid, &unusedSt)) {
		    code = UBADPRINNAME;
		    goto error_exit;
		}
	    }

	    /*
	     * save the identity of the principal
	     */


	    free(fullPrinNameP); fullPrinNameP = 0;
	    free(prinNameP); prinNameP = 0;
#ifdef DEBUG
	    compat_print_binding("Ubik conn: ", tc->conns[i][j]);
#endif /* DEBUG */
	} /* for(j=0...) */
    } /* for (i=0...) */

    /* free the ubik_binding_vector here */
    ubik_free_bindings(&bindingVectorP, &unusedSt);

    /*
     * We need at least one server
     */

    if ( tc->nServers == 0 ) {
	code = UNOSERVERS;
	goto error_exit;
    }

    /*
     * initialize the server's provider versions to very large value.
     * we will adjust this down if an RPC call fails with a range error
     */

    for ( i = 0 ; i < tc->nServers ; i++ ) tc->pver[i] = INT_MAX;

    /*
     * now that we have initialized the client structure, we need to
     * authenticate a binding for one of the servers. Pick the first
     * server at random and loop through until we find one that is
     * configured correctly.
     */

    tc->selectedServer = time(NULL) % tc->nServers;
    if ( !noAuth ) {
        for ( i = 0 ; i < tc->nServers ; i++ )
	    tc->states[i] |= CFLastFailed;
	/*
	 * loop through the servers trying to authenticate bindings
	 * until we find a good binding. ubik_auth_server_bindings will
	 * reset the CFLastFailed flag when it successfully authenticates
	 * a binding.
	 */
	while ( tc->nAuthHosts < tc->nServers ) {
	    ubik_auth_server_bindings(tc,tc->selectedServer);
	    if ( tc->states[tc->selectedServer]&CFAuthBinding ) {
		tc->states[tc->selectedServer] &= ~CFLastFailed;
		authhosts = 1;
		break;
	    }
	    tc->selectedServer = (tc->selectedServer+1) % tc->nServers;
	}
    } else {
	/*
 	 * running unauthenticated,
	 */

	authhosts = 1;
	tc->nAuthHosts = tc->nServers;
	for ( i = 0 ; i < tc->nServers ; i++ ) {
	    tc->states[i] |= CFAuthBinding;
	    tc->states[i] |= CFAuthDone;
	    for ( j = 0 ; j < UBIK_MAXADDRS ; j++ ) {
		if ( tc->conns[i][j] ) tc->authflag[i][j] |= BFAuthPass;
	    }
	}
    }

    /*
     * Error if no server was located with a valid binding
     */

    if ( !authhosts ) {
	code = UNOSERVERS;
	goto error_exit;
    }
    
exit:
    /* success */
    *aclient = tc;
    ubik_rele();
    return 0;

error_exit:
    /* cleanup */
    ubik_rele();
    if (cellNameP) free(cellNameP);
    if (fullPrinNameP) free(fullPrinNameP);
    if (prinNameP) free(prinNameP);
    ubik_free_bindings(&bindingVectorP, &unusedSt);
    if (tc) ubik_ClientFree(tc);
    *aclient = NULL;
    return(code);
}


/* ubik_ClientFree
 *	Free resources allocated for a ubik client.
 */

/* EXPORT */
int ubik_ClientFree(aclient)
     IN struct ubik_client *aclient;
{
    int i, j;
    struct ubik_client *tc = (struct ubik_client *)0;
    unsigned32 unusedSt;

    if (!aclient) return UNULLPTR;
    ubik_hold();
    if (aclient->rgyValid)
	sec_rgy_site_close(aclient->rgyHandle, &unusedSt);
    if (aclient->cellName)
	osi_Free(aclient->cellName,strlen(aclient->cellName)+1);
    if (aclient->secGroup)
	osi_Free(aclient->secGroup,strlen(aclient->secGroup)+1);
    for (i=0; i<aclient->nServers; i++) {
        for (j=0; j < UBIK_MAXADDRS; j++)
    	if (aclient->conns[i][j]) {
    	    rpc_binding_free(&(aclient->conns[i][j]), &unusedSt);
    	} else break;
        if (aclient->principals[i])
    	    osi_Free (aclient->principals[i],strlen(aclient->principals[i])+1);
        if (aclient->fullname[i])
    	    osi_Free (aclient->fullname[i],strlen(aclient->fullname[i])+1);
    }
    osi_Free(aclient,sizeof(struct ubik_client));
    ubik_rele();
    return(0);
}

/*
 *  ubik_ClientCleanup() --
 *    Cleanup function for ubik clients.  Basically it is supposed to
 *    'undo' what the initialization function did.
 */
/* EXPORT */
long ubik_ClientCleanup()
{
    long status;

    /*
     *  Hide the client not initialized error from the dfs auth package.
     */
    if (((status = dfsauth_client_CleanupAuthentication()) != 0) &&
	(status == DAUT_ERROR_CLIENT_NOT_INITIALIZED))
	status = 0;

    return(status);
}

/* parse list for clients */
/* EXPORT */
int ubik_ParseClientList(argc, argv, aothers, aserverCount)
    IN  int argc;
    IN  char **argv;
    IN  struct sockaddr_in *aothers;
    OUT long *aserverCount;
{
    long i;
    char *tp;
    struct hostent *th;
    long counter;
    int inServer;

    inServer = 0;	/* haven't seen -servers yet */
    counter = 0;
    for(i=1; i<argc; i++) {
	/* look for -servers argument */
	tp = argv[i];

	if (inServer) {
	    if (*tp == '-') break;  /* done */
	    /* otherwise this is a new host name */
	    th = gethostbyname(tp);
	    if (!th) return UBADHOST;
	    rpcx_sockaddr_from_ipaddr(th->h_addr, aothers++);
	    if (counter++ >= UBIK_MAXSERVERS) return UNHOSTS;
	} else {
	    /* haven't seen a -server yet */
	    if (!strcmp(tp, "-servers")) {
		inServer = 1;
	    }
	}
    }
    if (!inServer) {
	/* never saw a -server */
	return UNOENT;
    }

    *aserverCount = counter;
    return 0;
}

/*
 * routine to reset the bindings for one of the servers
 */

long ubik_ResetBinding(aclient,server)
    struct ubik_client *aclient;
    long server;
{
    int i;
    error_status_t code;

    for( i = 0 ; i < UBIK_MAXADDRS ; i++ ) {
        if ( aclient->conns[server][i] ) {
            rpc_binding_reset((rpc_binding_handle_t)aclient->conns[server][i],
                              (error_status_t *)&code);
	}
    }

    return(0);
}



/*
 * routine to make an RPC call inside of a TRY block.
 */

long ubik_TryCall(aproc,aclient,aserver,apver,
		  p1,p2,p3,p4,p5,p6,p7,p8,
		  p9,p10,p11,p12,p13,p14,p15,p16)
    int (* volatile aproc)();
    struct ubik_client * volatile aclient;
    volatile long aserver;
    volatile long apver;		/* required provider version */
    volatile long p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16;
{
    volatile error_status_t code;
    volatile int i;
    char *excNameP, excString[20];
    extern char *ExceptionToString();

    ubik_rele();
    for (i = 0 ; i < UBIK_MAXADDRS ; i++ ) {
        if ( !aclient->authflag[aserver][i] ) continue;
	if ( aclient->conns[aserver][i] == (rpc_binding_handle_t)0 ) continue;
	if ( aclient->pver[aserver] >= apver ) {
            TRY {
#ifdef DEBUG
                compat_print_binding("making call to: ",
				     aclient->conns[aserver][i]);
#endif /* DEBUG */
                code = (*aproc)(aclient->conns[aserver][i],
			     p1,p2,p3,p4,p5,p6,p7,p8,
		             p9,p10,p11,p12,p13,p14,p15,p16);
            } CATCH_ALL {
                excNameP = ExceptionToString(THIS_CATCH);
                if (excNameP == NULL) {
                     sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
                     excNameP = excString;
                }
                code = UEXCEPTION;
            } ENDTRY

	    /*
	     * if we get a range error, then this server is at a
	     * provider version level no higher than apver-1.
	     */

	    if ( code == rpc_s_op_rng_error ) {
		if ( apver > 1 ) aclient->pver[aserver] = apver - 1;
		code = UDOWNLVL;
	    }
	}
	else code = UDOWNLVL;
	if (code != rpc_s_comm_failure) break;
    }
    ubik_hold();

    /*
     * if the first binding for this server failed, then shuffle
     * them so we use the good binding first next time
     */

    if ( code == 0 && i > 0 ) {
	ubik_shuffleClientBindings(aclient,aserver,i);
    }

    return(code);
}

/* notes
 *      1) This call is just a front end for ubik_PVCall().
 *	2) first call with SINGLESERVER set, record the server to be used.
 *	3) subsequent calls use that server. If a failure is encountered,
 *	   the state is cleaned up and the error returned back to the caller.
 *	4) upon completion, the user must make a dummy call with
 *	   END_SINGLESERVER set, to clean up state.
 *	5) The ubik_PVCall routine no longer requires END_SINGLESERVER.
 *	   The first call without SINGLESERVER set also cleans up the state.
 */

/* ubik_Call
 *	This is the original routine for compability with existing clients.
 */
/* EXPORT */
long ubik_Call(aproc, aclient, aflags, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16)
    int (* aproc)();
    struct ubik_client * aclient;
    long aflags;
    long p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16;
{
    error_status_t code;
    long ubik_PVCall();

    code = ubik_PVCall(aproc,aclient,aflags,1,
			p1,p2,p3,p4,p5,p6,p7,p8,
			p9,p10,p11,p12,p13,p14,p15,p16);

    return(code);
}

/* ubik_PVCall
 *      Call this instead of the stub and to be sure to contact a server.
 *      This is the new interface for making ubik calls to servers at
 *      provider versions higher than 1. The compability routine is used
 *      to communicate with servers that cannot support the new call.
 * notes:
 *	- the first null binding signals the end of the binding list.
 *      - this call no longer requires a dummy call with END_SINGLESERVER
 *        set, the first call without SINGLESERVER set clears the state.
 *      - this call allows the caller to specify whether the request
 *        must be handled by the sync site with the UF_SYNC_SITE flag.
 *      - if the caller does not specify the UF_SYNC_SITE flag, then we
 *        anticipate that the client is going to be doing more updates, and 
 *        we start sending all requests to the sync site.
 */

/* EXPORT */
long ubik_PVCall(aproc, aclient, aflags, apver,
	         p1, p2, p3, p4, p5, p6, p7, p8,
	         p9, p10, p11, p12, p13, p14, p15, p16)
    int (* aproc)();
    struct ubik_client * aclient;
    long aflags;
    long apver;		/* required provider version */
    long p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16;
{
    error_status_t code;
    error_status_t someCode;
    unsigned long newHost;
    int i;
    int j;
    long server;
    long firstServer;
    int chaseCount;
    int newSyncSite;
    int gotSync;
    int pass;
    unsigned long startTime;
    char *excNameP, excString[20];
    extern char *ExceptionToString();
    long UBIKVOTE_GetSyncSiteIdentity();
    long UBIKVOTE_GetSyncSite();
    unsigned32 st, unusedSt;
    struct timespec tv;
    uuid_t syncRealm;
    uuid_t syncPrinc;

    if ( aclient == NULL ) return(UNULLPTR);

    ubik_hold();

    /*
     * we need at least one server configured before we can run
     */

    if ( aclient->nServers == 0 ) {
	ubik_rele();
	return(UNOSERVERS);
    }

    /*
     * the END_SINGLESERVER request resets the single server state
     */

    if ( aflags & UF_END_SINGLESERVER ) {
	aclient->flags &= ~UF_SINGLESERVER;
	ubik_rele();
	return(0);
    }

    /*
     * requests without the SINGLESERVER bit set clear the single
     * server state.
     */

    if ( !(aflags & UF_SINGLESERVER ) ) {
	aclient->flags &= ~UF_SINGLESERVER;
    }

    /*
     * loop through the servers trying the request. If the client has
     * requested that we contact the sync site, then send the request to
     * the last known sync site first, otherwise send the requests round
     * robin to distribute the load on the servers.
     *
     * If none of the hosts we know are up can process the request, then
     * start trying hosts that we think are down. We authenticate connections
     * as we go, since the client initialization only authenticates the
     * one connection to check whether any servers are configured correctly.
     */

    someCode = UNOSERVERS;
    chaseCount = 0;
    pass = 0;
    startTime = osi_Time();
    if ( (aflags & UF_SYNC_SITE) && (aclient->flags & UF_SYNC_SITE) ) {
	firstServer = aclient->syncSite;
    } else {
	firstServer = aclient->selectedServer;
    }
    server = firstServer;
    while( 1 ) {
	/*
	 * try to contact a functioning server. By default, we use the
	 * same server for all requests. We select another server whenever
	 * we get an RPC error.
	 *
	 * on the first pass, only try to contact servers that have
	 * been successfully contacted previously.
	 *
	 * on the second pass, also try servers that have authenticated
	 * bindings, even if the last call to that server returned an error.
	 *
	 * after the first two passes, authenticate one binding for
	 * each server on each pass. Once all bindings have been 
	 * authenticated, we give up if no servers are listening.
	 *
	 * If we have been told that this server is the sync site, then be
	 * persistent about authenticating its bindings, since at least
	 * one binding must be valid or the server could never been elected.
	 */

	if ( (pass > 1 || chaseCount > 0 ) &&
	    !(aclient->states[server]&CFAuthDone) ) {
	    /* authenticate bindings for this server */
	    ubik_auth_server_bindings(aclient,server);
	}

	if ( (pass > 0 || chaseCount > 0) &&
	     (aclient->states[server]&CFAuthBinding) ) {
	    aclient->states[server] &= ~CFLastFailed;
	}

	if ( !(aclient->states[server]&CFLastFailed) &&
	     (aclient->states[server]&CFAuthBinding) ) {

	    code = ubik_TryCall(aproc,aclient,server,apver,
			        p1,p2,p3,p4,p5,p6,p7,p8,
			        p9,p10,p11,p12,p13,p14,p15,p16);


	    /*
	     * we cannot attempt a retry if this a single server request,
	     * but we still need to reset the bindings on RPC errors.
	     */

	    if ( code && (aclient->flags & UF_SINGLESERVER) ) {
		aclient->flags &= ~UF_SINGLESERVER;
	        if ( (code >= rpc_s_mod) && (code <= (rpc_s_mod+4096)) ) {
		    /*
		     * RPC error, mark the server down and reset the binding
		     */

	            aclient->pver[server] = INT_MAX;
                    aclient->states[server] |= CFLastFailed;
		    if ( server == aclient->syncSite )
			aclient->flags &= ~UF_SYNC_SITE;
	            ubik_ResetBinding(aclient,server);
	        }
		ubik_rele();
		return(code);
	    }

	    /* note that getting a UNOTSYNC error code back does *not*
	     * guarantee that there is a sync site yet elected.  However,
	     * if there is a sync site out there somewhere, and you're
	     * trying an operation that requires a sync site, ubik will
	     * return UNOTSYNC, indicating the operation won't work until
	     * you find a sync site
	     */

	    if (code == UNOTSYNC) {
		/*
		 * at least one server is up, reset the pass count
		 */
		pass = 0;
	        someCode = UNOTSYNC;

		/*
		 * if we already know the sync site, try there next
		 */ 
		if ( (aclient->flags & UF_SYNC_SITE) &&
		     server != aclient->syncSite ) {
		    server = aclient->syncSite;
		    continue;
		}

		/*
		 * If we are already chasing the sync site, then break
		 * out of the loop if the servers are sending us around
		 * in circles.
		 */
		if (chaseCount >= UBIK_MAXCHASE) {
		    ubik_rele();
		    return(UNOTSYNC);
		}

		/*
		 * We need to ask who the sync site is, if we thought we
		 * knew which server was the sync site, then the sync site
		 * has moved.
		 */
		aclient->flags &= ~UF_SYNC_SITE;
		gotSync = 0;
		newSyncSite = -1;

		/*
		 * ask server for the location of the sync site. Try for
		 * the DCE identity first. If the server is down level
		 * then ask for an IP address instead.
		 */
	        code = ubik_TryCall(UBIKVOTE_GetSyncSiteIdentity,
				    aclient,server,2,&syncRealm,&syncPrinc);
		if (!code) {
		    gotSync = 1;
		    /*
		     * look for a server whose identity matches the
		     * identity of the sync site.
		     */
		    for (i = 0 ; i < aclient->nServers ; i++) {
			if (uuid_equal(&syncPrinc, &aclient->princUuid[i],
				       &unusedSt) &&
			    uuid_equal(&syncRealm, &aclient->realmUuid[i],
				       &unusedSt)) {
				newSyncSite = i;
				break;
			}
		    }
		} else if (code == UDOWNLVL) {
	            code = ubik_TryCall(UBIKVOTE_GetSyncSite,
				        aclient,server,1,&newHost);
	            if ( !code && newHost ) {
			gotSync = 1;
		        /*
		         * Look for a server with an IP binding handle
			 * that matches the IP address of the sync site.
		         */
		        newHost = (unsigned long)htonl(newHost);
		        for (i = 0 ; i < aclient->nServers &&
				     newSyncSite < 0 ; i++) {
		            for (j = 0 ; j < UBIK_MAXADDRS ; j++) {
			        if (!aclient->conns[i][j]) break;
				if (aclient->addrs[i][j] == newHost) {
			            newSyncSite = i;
				    break;
			        }
		            }
			}
		    }
		}

		/*
		 * If we received a response to our request for the
		 * new sync site, then update the client so we will
		 * retry the request on the sync site. If we received
		 * a response that didn't map to a known server, we
		 * will return UNOTSYNC.
		 */
		if (gotSync) {
		    if (newSyncSite >= 0) {
		        chaseCount++;
		        aclient->flags |= UF_SYNC_SITE;
		        aclient->syncSite = newSyncSite;
		        server = newSyncSite;
		    } else {
			/*
			 * we have never heard of the sync site server,
			 * so we cannot try to contact it.
			 */
		        ubik_rele();
		        return(UNOTSYNC);
		    }

		    /*
		     * if the we are chasing the sync site, delay a little
		     * to give the server's a chance to get back in sync.
		     */
		    if ( chaseCount > 1 ) {
			ubik_rele();
			tv.tv_sec = UBIK_CHASEDELAY;
			tv.tv_nsec = 0;
			pthread_delay_np(&tv);
			ubik_hold();
		    }
		    /* reset the quorum timeout */
		    startTime = osi_Time();
		    continue;
		}
	    } else if ( code == UNOQUORUM ) {
		/*
		 * this server is still recovering, wait for the servers
		 * to establish quorum.
		 */
		pass = 0;
	        someCode = UNOQUORUM;
	    } else if ( (code >= rpc_s_mod) && (code <= (rpc_s_mod+4096)) ) {
		/*
		 * RPC error, reset the binding
		 */
		someCode = code;
		ubik_ResetBinding(aclient,server);
		aclient->pver[server] = INT_MAX;
		aclient->states[server] |= CFLastFailed;
		if ( server == aclient->syncSite ) {
		    aclient->flags &= ~UF_SYNC_SITE;
		}
	    } else if ( code ) {
	        /*
		 * miscellaneous ubik error or application error,
		 * the server is up. Use this server for all future
		 * requests unless the client set the UF_SYNC_SITE
		 * bit.
		 */
	        aclient->states[server] &= ~CFLastFailed;
		if ( !(aflags & UF_SYNC_SITE) )
		    aclient->selectedServer = server;
	        ubik_rele();
	        return code;
	    } else {
		/*
		 * Success, the server is up. Use this server
		 * for all future requests unless the client set
		 * the UF_SYNC_SITE bit.
		 */
	        aclient->states[server] &= ~CFLastFailed;
		if ( !(aflags & UF_SYNC_SITE) )
		    aclient->selectedServer = server;
		if ( aflags & UF_SINGLESERVER )
		    aclient->flags |= UF_SINGLESERVER;
		ubik_rele();
		return(0);
	    }
	}

	server++;
	if ( server >= aclient->nServers ) server = 0;

	if ( server == firstServer ) {
	    /*
	     * we just finished a loop through the servers
	     */

	    if ( someCode == UNOQUORUM ) {
		/*
		 * wait for the server to establish quorum
		 */
		ubik_rele();
		if ( (osi_Time()-startTime) > (2*BIGTIME)) {
		    return(UNOQUORUM);
		}
		tv.tv_sec = UBIK_QUORUMDELAY;
		tv.tv_nsec = 0;
		pthread_delay_np(&tv);	/* don't just spin-loop */
		ubik_hold();
	    } else {
		pass++;

		/*
		 * if we have tried each host at least once, and there
		 * are no more bindings left to authenticate then
		 * return the error.
		 */

		if ( pass > UBIK_MAXPASS &&
		     aclient->nAuthHosts == aclient->nServers ) {
		    ubik_rele();
		    return(someCode);
		}
	    }
	}
    }
}
