/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: authtest_main.c,v $
 * Revision 1.1.14.1  1996/10/02  20:58:39  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:47:11  damon]
 *
 * Revision 1.1.9.2  1994/07/13  22:26:43  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  21:14:07  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:01:36  mbs]
 * 
 * Revision 1.1.9.1  1994/06/09  14:19:15  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:31:13  annie]
 * 
 * Revision 1.1.5.4  1993/01/21  15:18:41  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  15:56:17  cjd]
 * 
 * Revision 1.1.5.3  1992/11/24  18:31:07  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:23:00  bolinger]
 * 
 * Revision 1.1.5.2  1992/08/31  21:12:55  jaffe
 * 	Replace Missing RCS ids
 * 	[1992/08/31  15:48:43  jaffe]
 * 
 * Revision 1.1.3.2  1992/05/20  20:04:27  mason
 * 	Transarc delta: dstokes-ot2171-remove-cma-calls 1.1
 * 	  Files modified:
 * 	    bosserver: bossvr_main.c, bossvr_thread_childWatch.c
 * 	    flserver: flc.c, flclient.c, flserver.c
 * 	    ncscompat: compat_serverdup.c
 * 	    ncsubik: ubikclient.c, ubikcmd.c, utst_client.c
 * 	    ncsubik: utst_server.c; rep: rep_main.c
 * 	    security/dacl: testacl.c
 * 	    security/dfsauth: auth_client_main.c, authtest_main.c
 * 	    security/dfsauth: dfsauth_client.c, dfsauth_server.c
 * 	    tkm: tkm_tokens.c; update: client.c
 * 	    userInt/fts: volc_vldbint.c
 * 	  Selected comments:
 * 	    Remove explicit CMA calls to allow other threading package styles.
 * 	    Dawn Stokes says this works fine for her.
 * 	    Remove cma_init() call.
 * 	[1992/05/20  11:58:47  mason]
 * 
 * Revision 1.1  1992/01/19  02:52:22  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  authtest_main.c -- main routines for auth rpc test execution
 *
 *
 *	Copyright (C) 1991 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/debug.h>
#include <dcedfs/osi.h>

#include <dcedfs/cmd.h>
#include <dcedfs/cmdAux.h>

#include <dcedfs/compat.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/security/dfsauth/authtest_main.c,v 1.1.14.1 1996/10/02 20:58:39 damon Exp $")

#include <stdio.h>

#include <dce/cma.h>
#include <pthread.h>

#include <dce/rpc.h>

#include <authtest.h>
/*
#include <authtest2.h>
*/
#include <dfsauth.h>
#include <dfsauth_debug.h>

/* useful routine for NCS error printing */
IMPORT char * RgySec_ErrorString _TAKES((
					 error_status_t	* error
				       ));


char		programName[BUFSIZ];
uuid_t		authGroupUuid;
int		isLocalAuthPresent = 0;

char		authfilename[BUFSIZ];

long		auth_debug_flag = 0xf;
#define AUTH_DEBUG_MASK(bit)		(1 << (bit - 1))
#define AUTH_DEBUG_BIT_RPC_TRACE	1
#define AUTH_DEBUG_BIT_BINDING		2
#define AUTH_DEBUG_BIT_DCE_TRACE	3
#define AUTH_DEBUG_BIT_DCE_AUTH_TRACE	4

char *	listFilenameP = "./AclFile";

long NCS_Init(maxCalls, serverPrincipalName, listFilenameP)
     int	maxCalls;
     char *	serverPrincipalName;
     char *	listFilenameP;
{
  error_status_t		ncs_error_status;
  boolean32			validProtSeq;
  static ndr_char		protSeq[] = "ip";
  rpc_binding_vector_t *	bvec;
  unsigned_char_t *		stringBinding;
  int				i;
  char				calleeName[BUFSIZ];
  static char			routineName[] = "NCS_Init";
  long				rtnVal;
  
  uuid_t			serverTypeUuid =
    { /* 2d6f80e4-2cac-11ca-8b78-02608c2ebaac */
      0x2d6f80e4,
      0x2cac,
      0x11ca,
      0x8b,
      0x78,
      {0x02, 0x60, 0x8c, 0x2e, 0xba, 0xac}
    };
  uuid_t			serverInstanceUuid;
  uuid_t			staticServerInstanceUuid =
    { /* cc2c5b22-2caa-11ca-9dc3-02608c2ebaac */
      0xcc2c5b22,
      0x2caa,
      0x11ca,
      0x9d,
      0xc3,
      {0x02, 0x60, 0x8c, 0x2e, 0xba, 0xac}
    };

  char				serverInstanceUuidStr[] =
    "cc2c5b22-2caa-11ca-9dc3-02608c2ebaac";
  uuid_vector_t			serverObjUuidVector;
uuid_t nil_uuid;
char * uuidString;
  
  /* relevant externals */
  IMPORT uuid_t uuid_nil;


  /*
   * Tell the RPC runtime to listen for the remote procedure calls using the
   * specified protocol sequence.
   */
#ifdef use_useallprotseq
  (void)strcpy(calleeName, "rpc_server_use_all_protseqs");
  dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_DCE_TRACE,
	   ("%s: %s about to call %s", programName, routineName, calleeName));
  rpc_server_use_all_protseqs(maxCalls, &ncs_error_status);
#else /* use_useallprotseq */

  (void)strcpy(calleeName, "rpc_network_is_protseq_valid");
  if (rpc_network_is_protseq_valid(protSeq, &ncs_error_status) == true) {
    dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_DCE_AUTH_TRACE,
	     ("%s: %s returned from %s with protseq %s",
	      programName, routineName, calleeName, protSeq));

    (void)strcpy(calleeName, "rpc_server_use_protseq");
    dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_DCE_AUTH_TRACE,
	     ("%s: %s about to call %s with protseq %s",
	      programName, routineName, calleeName, protSeq));
    rpc_server_use_protseq(protSeq, maxCalls, &ncs_error_status);
  }
  else {
    fprintf(stderr, "%s: protocol sequence unsupported: %s\n", programName, protSeq);
    return 1;
  }

#endif /* use_useallprotseq */

  dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_DCE_AUTH_TRACE,
	   ("%s: %s returned from %s", programName, routineName, calleeName));
  if (ncs_error_status != error_status_ok)  {
    fprintf(stderr, "%s: unable to use protocol sequence: %s\n",
	    programName, RgySec_ErrorString(&ncs_error_status));
    return 1;
  }

  uuid_from_string(serverInstanceUuidStr, &serverInstanceUuid, &ncs_error_status);
  if (ncs_error_status != error_status_ok) {
    fprintf(stderr, "%s: unable to map uuid string to uuid: %s\n",
	    routineName, RgySec_ErrorString(&ncs_error_status));
    return 1;
  }
  
  if ((uuid_equal(&serverInstanceUuid, &staticServerInstanceUuid,
		  &ncs_error_status) == false) ||
      (ncs_error_status != error_status_ok)) {
    fprintf(stderr, "%s: error transcribing server instance uuid: %s\n",
	    routineName, RgySec_ErrorString(&ncs_error_status));

    uuid_to_string(&staticServerInstanceUuid, &uuidString, &ncs_error_status);
    if (ncs_error_status == error_status_ok) {
      fprintf(stderr, "static uuid string: %s\ntranslated uuid: %s\n",
	      uuidString, serverInstanceUuidStr);
    }
    else {
      fprintf(stderr, "unable to transcribe static uuid into string\n");
    }
  }
  
  rpc_object_set_type(&serverInstanceUuid, &serverTypeUuid, &ncs_error_status);
  if (ncs_error_status != error_status_ok) {
    fprintf(stderr, "%s: unable to set instance uuid type: %d\n",
	    routineName, RgySec_ErrorString(&ncs_error_status));
    return 1;
  }

  uuid_create_nil (&nil_uuid, &ncs_error_status);
  if (ncs_error_status != error_status_ok) {
    fprintf(stderr, "%s: unable to create NULL uuid: %s\n",
	    routineName, RgySec_ErrorString(&ncs_error_status));
    return 1;
  }
  
  /* register the fact that we are exporting the  auth server interface */
  (void)strcpy(calleeName, "rpc_server_register_if");
  dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_DCE_TRACE,
	   ("%s: %s about to call %s", programName, routineName, calleeName));
  rpc_server_register_if(AUTH_v0_0_s_ifspec,
#ifdef notdef
			 &uuid_nil,
#endif /* notdef */
			 &serverTypeUuid,
			 (rpc_mgr_epv_t)NULL,
			 &ncs_error_status);
  dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_DCE_TRACE,
	   ("%s: %s returned from %s", programName, routineName, calleeName));
  if (ncs_error_status != error_status_ok) {
    fprintf(stderr, "%s: unable to register interface: %s\n",
	    programName, RgySec_ErrorString(&ncs_error_status));
    return 1;
  }

#define use_rpcauth 1
#ifdef use_rpcauth
  rtnVal = dfsauth_server_InitAuthentication(serverPrincipalName, listFilenameP,
					     (char *)NULL);
  if (rtnVal != 0)	return rtnVal;
#endif /* use_rpcauth */				

  /*
   * Inquire all protocol sequences supported by both the RPC runtime and
   * the underlying operating system.
   */
  (void)strcpy(calleeName, "rpc_server_inq_bindings");
  dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_DCE_TRACE,
	   ("%s: %s about to call %s", programName, routineName, calleeName));
  rpc_server_inq_bindings(&bvec, &ncs_error_status);
  dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_DCE_TRACE,
	   ("%s: %s returned from %s", programName, routineName, calleeName));
  if (ncs_error_status != error_status_ok)  {
    printf("%s: can't obtain bindings: %s\n",
	   programName, RgySec_ErrorString(&ncs_error_status));
    return 1;
  }

  (void)strcpy(calleeName, "compat_ShutdownDuplicateServer");
  dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_DCE_TRACE,
	   ("%s: %s about to call %s", programName, routineName, calleeName));
  if (compat_ShutdownDuplicateServer(AUTH_v0_0_s_ifspec, &serverInstanceUuid, 0)) {
/*
  if (compat_ShutdownDuplicateServer(AUTH_v0_0_s_ifspec, &uuid_nil, 0)) {
*/
    fprintf(stderr, "%s: couldn't remove duplicate server\n",
	    routineName);
    return 1;
  }

  /*
   * now register the end point with rpcd
   */

  serverObjUuidVector.count = 1;
  serverObjUuidVector.uuid[0] = &serverInstanceUuid;

  /*
   * Inquire all protocol sequences supported by both the RPC runtime and
   * the underlying operating system.
   */
  (void)strcpy(calleeName, "rpc_server_inq_bindings");
  dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_DCE_TRACE,
	   ("%s: about to call %s", routineName, calleeName));
  rpc_server_inq_bindings(&bvec, &ncs_error_status);
  dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_DCE_TRACE,
	   ("%s: returned from call to %s", routineName, calleeName));
  if (ncs_error_status == rpc_s_ok)  {
    (void)strcpy(calleeName, "rpc_ep_register");
    dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_DCE_TRACE,
	     ("%s: about to call %s", routineName, calleeName));
    rpc_ep_register(AUTH_v0_0_s_ifspec,
		    bvec,
		    &serverObjUuidVector,
/*
		    (uuid_vector_t *)NULL,
*/
		    "DFS authtest server",
		    &ncs_error_status);
    dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_DCE_TRACE,
	     ("%s: returned from call to %s", routineName, calleeName));
    if (ncs_error_status != rpc_s_ok) {
      /* rpc_ep_register */
      osi_printf("%s: unexpected error from %s: %s",
		 routineName, calleeName, RgySec_ErrorString(&ncs_error_status));
      rtnVal = 1;
    }
  }
  else {
    /* rpc_server_inq_bindings */
    osi_printf("%s: unexpected error from %s: %s",
	       routineName, calleeName, RgySec_ErrorString(&ncs_error_status));
    rtnVal = 1;
  }

  /*
   * Now we print out all binding information.
   */

  if (auth_debug_flag & AUTH_DEBUG_MASK(AUTH_DEBUG_BIT_BINDING)) {
    osi_printf ("%s: the %d binding(s) under which we operate:\n",
		programName, bvec->count);
    (void)strcpy(calleeName, "rpc_binding_to_string_binding");
    for (i = 0; i < bvec->count; i++)  {
      dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_DCE_TRACE,
	       ("%s: %s about to call %s", programName, routineName, calleeName));
      rpc_binding_to_string_binding(bvec->binding_h[i],
				    &stringBinding,
				    &ncs_error_status);
      dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_DCE_TRACE,
	       ("%s: %s returned from %s", programName, routineName, calleeName));
      if (ncs_error_status != error_status_ok)  {
	osi_printf("%s: can't get string binding: %s\n",
		   programName, RgySec_ErrorString(&ncs_error_status));
	return 1;
      }
      osi_printf("%s\n", stringBinding);
      rpc_string_free(&stringBinding, &ncs_error_status);
    }
    osi_printf("\n");
  }
  
#ifdef notdef
  /* register the fact that we are exporting the  auth server interface */
  (void)strcpy(calleeName, "rpc_server_register_if");
  dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_DCE_TRACE,
	   ("%s: %s about to call %s", programName, routineName, calleeName));
  rpc_server_register_if(AUTH2_v0_0_s_ifspec,
			 &uuid_nil,
/*
			 &serverTypeUuid,
*/
			 (rpc_mgr_epv_t)NULL,
			 &ncs_error_status);
  dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_DCE_TRACE,
	   ("%s: %s returned from %s", programName, routineName, calleeName));
  if (ncs_error_status != error_status_ok) {
    fprintf(stderr, "%s: unable to register interface: %s\n",
	    programName, RgySec_ErrorString(&ncs_error_status));
    return 1;
  }

  /*
   * Inquire all protocol sequences supported by both the RPC runtime and
   * the underlying operating system.
   */
  (void)strcpy(calleeName, "rpc_server_inq_bindings");
  dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_DCE_TRACE,
	   ("%s: %s about to call %s", programName, routineName, calleeName));
  rpc_server_inq_bindings(&bvec, &ncs_error_status);
  dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_DCE_TRACE,
	   ("%s: %s returned from %s", programName, routineName, calleeName));
  if (ncs_error_status != error_status_ok)  {
    printf("%s: can't obtain bindings: %s\n",
	   programName, RgySec_ErrorString(&ncs_error_status));
    return 1;
  }

  /*
   * now register the end point with rpcd
   */
  (void)strcpy(calleeName, "rpc_server_ep_register");
  dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_DCE_TRACE,
	   ("%s: %s about to call %s", programName, routineName, calleeName));
  rpc_ep_register(AUTH2_v0_0_s_ifspec,
		  bvec,
		  (uuid_vector_t *)NULL,
		  (unsigned_char_t *)NULL,
		  &ncs_error_status
		  );
  dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_DCE_TRACE,
	   ("%s: %s returned from %s", programName, routineName, calleeName));
  if (ncs_error_status != error_status_ok) {
    fprintf(stderr, "%s: unable to register end point: %s\n",
	    programName, RgySec_ErrorString(&ncs_error_status));
    return 1;    
  }
#endif /* notdef */

  (void)strcpy(calleeName, "rpc_server_listen");
  dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_BINDING,
	   ("%s: about to listen for network requests.", programName));
  dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_DCE_TRACE,
	   ("%s: %s about to call %s with maxCalls = %d",
	    programName, routineName, calleeName, maxCalls));

  /* now, listen to the network for incoming calls */
  rpc_server_listen(maxCalls, &ncs_error_status);

  /* if we get to here, we have, somehow been kicked out of the server_listen procedure */
  dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_DCE_TRACE,
	   ("%s: %s returned from %s", programName, routineName, calleeName));
  fprintf(stderr, "%s: no longer listening to network: %s\n",
	  programName, RgySec_ErrorString(&ncs_error_status));

  rpc_mgmt_stop_server_listening((rpc_binding_handle_t)NULL, &ncs_error_status);
  if (ncs_error_status == rpc_s_ok) {
    fprintf(stderr, "%s: server re-stopped\n", routineName);
  }
  else {
    fprintf(stderr, "%s: %s unable to re-stop server from listening: %s\n",
	    programName, routineName, RgySec_ErrorString(&ncs_error_status));
  }
  

  if (compat_UnregisterServer(AUTH_v0_0_s_ifspec, &uuid_nil)) {
    fprintf(stderr, "%s: couldn't unregister end point\n",
	    routineName);
    return 1;
  }
  
  /* any shutdown processing goes here */
  return 0;	/* this is probably OK */
}

#define SIMPLE_PRINCIPAL_INDEX		0
#define SIMPLE_ACL_FILENAME_INDEX	1
#define SIMPLE_MAXCALLS_INDEX		2
#define SIMPLE_DEBUG_INDEX		3
#define SIMPLE_RPCDEBUG_INDEX		4
int DoSimple(aSyntaxP, aRockP)
     struct cmd_syndesc *	aSyntaxP;
     char *			aRockP;
{
  int			rtnVal = 0;
  int			maxCalls = 4;
  error_status_t	rpcauthErr;
  char *		principalNameP = (char *)NULL;
  static char		routineName[] = "DoSimple";

  if (cmd_IsParamPresent(aSyntaxP, SIMPLE_DEBUG_INDEX)) {
    (void)ParseDebugFlag(cmd_FirstData(aSyntaxP, SIMPLE_DEBUG_INDEX),
			 &dfsauth_debug_flag);
  }

  if (cmd_IsParamPresent(aSyntaxP, SIMPLE_RPCDEBUG_INDEX)) {
    rpc__dbg_set_switches(cmd_FirstData(aSyntaxP, SIMPLE_RPCDEBUG_INDEX), &rpcauthErr);
    if (rpcauthErr != error_status_ok) {
      fprintf(stderr, "%s: rpc debug flag setting failed: %s\n",
	      routineName, RgySec_ErrorString(&rpcauthErr));
    }
  }

  if (cmd_IsParamPresent(aSyntaxP, SIMPLE_PRINCIPAL_INDEX)) {
    principalNameP = cmd_FirstData(aSyntaxP, SIMPLE_PRINCIPAL_INDEX);
  }
  if (cmd_IsParamPresent(aSyntaxP, SIMPLE_MAXCALLS_INDEX)) {
    maxCalls = atoi(cmd_FirstData(aSyntaxP, SIMPLE_MAXCALLS_INDEX));
  }
  
  if (cmd_IsParamPresent(aSyntaxP, SIMPLE_ACL_FILENAME_INDEX)) {
    listFilenameP = cmd_FirstData(aSyntaxP, SIMPLE_ACL_FILENAME_INDEX);
  }

  /* set up everything NCS needs to run and listen for calls */
  rtnVal = NCS_Init(maxCalls, principalNameP, listFilenameP);

  /* we should never get to here unless something went wrong */
}

void SetUpSimpleSyntax()
{
  struct cmd_syndesc *	simpleSyntaxP;

  simpleSyntaxP = cmd_CreateSyntax("simple", DoSimple,
				   (char *)NULL, "make simple RPC");
  cmd_AddParm(simpleSyntaxP, "-principal", CMD_SINGLE, CMD_OPTIONAL,
	      "principal name");
  cmd_AddParm(simpleSyntaxP, "-listfilename", CMD_SINGLE, CMD_OPTIONAL,
	      "user list acl file name");
  cmd_AddParm(simpleSyntaxP, "-maxcalls", CMD_SINGLE, CMD_OPTIONAL, "number rpcs");
  cmd_AddParm(simpleSyntaxP, "-debug", CMD_SINGLE, CMD_OPTIONAL, "debug flag value");
  cmd_AddParm(simpleSyntaxP, "-rpcdebug", CMD_SINGLE, CMD_OPTIONAL,
	      "rpc debug flag value");
}

void SetUpSyntax()
{
  SetUpSimpleSyntax();
}


int main(argc, argv)
     int	argc;
     char *	argv[];
{
  int			rtnVal = 0;	/* as a good value as any for now */
  int			i;

  (void)strcpy(programName, argv[0]);

  initialize_dau_error_table();
  
  SetUpSyntax();
  rtnVal = cmd_Dispatch(argc, argv);

  return rtnVal;
}

     
