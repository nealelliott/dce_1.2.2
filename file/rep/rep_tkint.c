/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rep_tkint.c,v $
 * $EndLog$
 */
/*
 * (C) Copyright 1990, 1995 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

/* rep_tkint.c: handling some token revocation work. */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/basicincludes.h>
#include <dcedfs/osi.h>
#include <dcedfs/compat.h>
#include <dcedfs/afs4int.h>
#include <dcedfs/rep_data.h>
#include <dcedfs/tkm_tokens.h>
#include <dcedfs/tkn4int.h>

/* SAMS support */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsrpsmac.h>
#include <dfsrpssvc.h>
#include <dfsrpsmsg.h>

#include <pthread.h>
#include <dcedfs/icl.h>

#include <repser.h>
#include <rep_trace.h>


struct afsNetData revocationNetData;
unsigned long revocationEpoch;

extern int parm_tokenProcs;

/* This is the thread that spawns the RPC listener. */
static pthread_addr_t rpcThread(arg)
pthread_addr_t arg;
{
    unsigned long st;

    /* Listen to the network for incoming calls */
    icl_Trace0(rep_iclSetp, REP_TR_LISTEN_CALLS);
    rpc_mgmt_set_server_com_timeout(rpc_c_binding_default_timeout+2, &st);
    rpc_server_listen(parm_tokenProcs, &st);
    sprintf(ErrBuff, "Returned from rpc_server_listen: %s", dfs_dceErrTxt(st));
    Log("%s", ErrBuff);
    if (st != error_status_ok && st != rpc_s_already_listening) {
	sprintf(ErrBuff, "RPC no longer listening to network: %s",
		dfs_dceErrTxt(st));
	Log("%s", ErrBuff);
	return ((pthread_addr_t) 1);
    }
    return ((pthread_addr_t) 0);
}

void rep_initTokens()
{
    rpc_binding_vector_t *bvecP;
    rpc_binding_handle_t *bindingP;
    register int ix;
    unsigned long st;
    unsigned_char_t *stringBinding, *strUuid, *strProtseq, *strNetaddr, *strEndpoint;
    static int inited = 0;
    int foundSelf;
    struct sockaddr_in myRevoke;
    char *cellNamep, *hostNamep, *princNamep;

    if (inited != 0)
	return;

    revocationEpoch = osi_Time();
    bzero((char *)&revocationNetData, sizeof(revocationNetData));

    /*
      * Inquire all protocol sequences supported by both the RPC runtime and
      * the underlying operating system.
      */
    rpc_server_inq_bindings(&bvecP, &st);
    if (st != error_status_ok) {
	sprintf(ErrBuff, "Cannot list RPC bindings: %s.  Exiting.",
		dfs_dceErrTxt(st));
	dce_svc_printf(RPS_S_PRINT_ERR_MSG, WhoIAm, ErrBuff);
	Log("%s", ErrBuff);
	exit(1);
    }
    foundSelf = 0;
    for (ix = 0, bindingP = &bvecP->binding_h[0]; ix < bvecP->count; ++ix, ++bindingP) {
	if (*bindingP == NULL)
	    continue;
	rpc_binding_to_string_binding(*bindingP, &stringBinding, &st);
	if (st != error_status_ok) {
	    sprintf(ErrBuff, "Cannot get string binding number %d: %s.  Exiting.",
		    ix, dfs_dceErrTxt(st));
	    dce_svc_printf(RPS_S_PRINT_ERR_MSG, WhoIAm, ErrBuff);
	    Log("%s", ErrBuff);
	    exit(1);
	}
	rpc_string_binding_parse(stringBinding, &strUuid, &strProtseq,
				 &strNetaddr, &strEndpoint, NULL, &st);
	if (st != error_status_ok) {
	    sprintf(ErrBuff, "Cannot parse string binding number %d, ``%s'': %s.  Exiting.",
		    ix, stringBinding, dfs_dceErrTxt(st));
	    dce_svc_printf(RPS_S_PRINT_ERR_MSG, WhoIAm, ErrBuff);
	    Log("%s", ErrBuff);
	    exit(1);
	}
	if (strcmp("ncadg_ip_udp", (char *)strProtseq) == 0
	    || strcmp("ip", (char *)strProtseq) == 0) {
	    if (strEndpoint != NULL && strcmp((char *)strEndpoint, "") != 0) {
		foundSelf = 1;
		bzero((char *)&myRevoke, sizeof(myRevoke));
		myRevoke.sin_family = AF_INET;
		myRevoke.sin_addr.s_addr = inet_addr(strNetaddr);
		myRevoke.sin_port = (unsigned short)
		  htons((unsigned short)atoi((char *)strEndpoint));
		revocationNetData.sockAddr = *((afsNetAddr *) &myRevoke);
		icl_Trace2(rep_iclSetp, REP_TR_REVOKE_SOCKET,
			   ICL_TYPE_STRING, inet_ntoa(myRevoke.sin_addr),
			   ICL_TYPE_LONG, ntohs(myRevoke.sin_port));
	    }
	}
	if (stringBinding) rpc_string_free(&stringBinding, &st);
	if (strUuid) rpc_string_free(&strUuid, &st);
	if (strProtseq) rpc_string_free(&strProtseq, &st);
	if (strNetaddr) rpc_string_free(&strNetaddr, &st);
	if (strEndpoint) rpc_string_free(&strEndpoint, &st);

	/* Quit if we're done. */
	if (foundSelf != 0) break;
    }
    if (foundSelf == 0) {
	strcpy(ErrBuff, "Could not find ncadg_ip_udp binding for myself!  Exiting.");
	dce_svc_printf(RPS_S_PRINT_ERR_MSG, WhoIAm, ErrBuff);
	Log("%s", ErrBuff);
	exit(1);
    }
    rpc_binding_vector_free(&bvecP, &st);
    if (st != error_status_ok) {
	sprintf(ErrBuff, "Cannot free the vector of RPC bindings: %s.  Exiting.",
		dfs_dceErrTxt(st));
	dce_svc_printf(RPS_S_PRINT_ERR_MSG, WhoIAm, ErrBuff);
	Log("%s", ErrBuff);
	exit(1);
    }

    /* Figure the local principal name for fx's revocation calls. */
    dce_cf_get_cell_name(&cellNamep, &st);
    if (st) {
	sprintf(ErrBuff, "Cannot get the local cell name: %s.  Exiting.",
		dfs_dceErrTxt(st));
	dce_svc_printf(RPS_S_PRINT_ERR_MSG, WhoIAm, ErrBuff);
	Log("%s", ErrBuff);
	exit(1);
    }
    /* Get the name of the machine principal, too */
    dce_cf_get_host_name(&hostNamep, &st);
    if (st) {
	sprintf(ErrBuff, "Cannot get the local host name: %s.  Exiting.",
		dfs_dceErrTxt(st));
	dce_svc_printf(RPS_S_PRINT_ERR_MSG, WhoIAm, ErrBuff);
	Log("%s", ErrBuff);
	exit(1);
    }
    if ((strlen(cellNamep)+strlen(hostNamep)+sizeof"//dfs-server") >=
	sizeof(revocationNetData.principalName)) {
	sprintf(ErrBuff, "Local principal name (%s/%s/dfs-server) too long.  Continuing.",
		cellNamep, hostNamep);
	dce_svc_printf(RPS_S_PRINT_ERR_MSG, WhoIAm, ErrBuff);
	Log("%s", ErrBuff);
    } else {
	sprintf((char*)&revocationNetData.principalName[0], "%s/%s/dfs-server",
		cellNamep, hostNamep);
	sprintf(ErrBuff, "Using '%s' as the local principal name.",
		&revocationNetData.principalName[0]);
	Log("%s", ErrBuff);
    }
    free(cellNamep);
    free(hostNamep);

    rpc_server_register_auth_info (revocationNetData.principalName,
				   rpc_c_authn_default,
				   (rpc_auth_key_retrieval_fn_t)NULL,
				   (void *)NULL,
				   &st);
    if (st != error_status_ok) {
	sprintf(ErrBuff, "Cannot register server authentication: %s.  Continuing unauthenticated.",
		dfs_dceErrTxt(st));
	dce_svc_printf(RPS_S_PRINT_ERR_MSG, WhoIAm, ErrBuff);
	Log("%s", ErrBuff);
	revocationNetData.principalName[0] = '\0';
    }

    inited = 1;
}

void rep_startTokens()
{
    pthread_t rpcListenerThreadPtr;
    pthread_attr_t subthreadAttr;
    long code;
    static int inited = 0;

    if (inited != 0)
	return;

    /* Create the thread attributes. */
    pthread_attr_create(&subthreadAttr);
    pthread_attr_setstacksize(&subthreadAttr, 10000);

    /* Create the thread that will take the RPC requests. */
    icl_Trace0(rep_iclSetp, REP_TR_LISTEN_THR_CREATE);
    code = pthread_create(&rpcListenerThreadPtr, subthreadAttr, rpcThread,
			  (pthread_addr_t)0);
    if (code) { /* thread creation failed */
	sprintf(ErrBuff, "Cannot create the RPC listener thread: errno %d; %s.  Exiting.",
		errno, dfs_dceErrTxt(code));
	dce_svc_printf(RPS_S_PRINT_ERR_MSG, WhoIAm, ErrBuff);
	Log("%s", ErrBuff);
	exit(1);
    }

    inited = 1;
}
