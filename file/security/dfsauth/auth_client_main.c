/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 *  auth_client_main.c -- routines that try to call the auth test server
 *
 *
 *	Copyright (C) 1995, 1991 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>
#include <dcedfs/debug.h>

#include <dcedfs/cmd.h>
#include <dcedfs/cmdAux.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/security/dfsauth/RCS/auth_client_main.c,v 4.14 1995/07/26 16:09:17 blake Exp $")

#include <dce/cma.h>
#include <dce/rpc.h>
#include <dce/nbase.h>
#include <dce/id_base.h>
#include <dce/sec_login.h>
#include <dcedfs/compat.h>		/* dfs_dceErrTxt */

#include <authtest.h>
/*
#include <authtest2.h>
*/

#include <dfsauth.h>
#include <dfsauth_debug.h>
#include <../dfssec_errorStrings.h>

#include <../exception_print.h>

extern char * hostutil_NetAddrStr _TAKES((
					  char *	hostString
					));

char			programName[BUFSIZ];
rpc_binding_handle_t	theServerBinding;
rpc_binding_handle_t	theServer2Binding;

#define	AUTH_SERVER_PORT	"7007"


/* get a binding handle to be used for communication with the bosserver */
rpc_binding_handle_t * GetServerBinding(netAddrStr, endPointStr)
     char *	netAddrStr;
     char *	endPointStr;
{
  rpc_binding_handle_t *	rtnVal = &theServerBinding;
  static unsigned_char_t	emptyStr[] = "";
  static unsigned_char_t	protSeqStr[] = "ncadg_ip_udp";

  unsigned_char_t *		theServerString;
  error_status_t		rpc_error;
  static char			routineName[] = "GetServerBinding";

  /* make up a string representation of the desired binding */
  /* we don't have a particular object to access */
  rpc_string_binding_compose("cc2c5b22-2caa-11ca-9dc3-02608c2ebaac",
#ifdef notdef
  rpc_string_binding_compose(/* uuid = */ emptyStr,
#endif /* notdef */
			     protSeqStr,
			     netAddrStr,
			     /* endpoint = */ (char *)NULL,
			     /* options = */ emptyStr,
			     &theServerString,
			     &rpc_error);
  if (rpc_error != error_status_ok) {
    fprintf(stderr, "%s: unable to compose a string to repr the binding: %s\n",
	    programName, RgySec_ErrorString(&rpc_error));
    rtnVal = (rpc_binding_handle_t *)NULL;
  }
  else {
    /* get a binding handle for the desired bosserver */
    rpc_binding_from_string_binding(theServerString, &theServerBinding, &rpc_error);
    if (rpc_error != error_status_ok) {
      fprintf(stderr, "%s: unable to obtain binding from string: %s\n",
	      programName, RgySec_ErrorString(&rpc_error));
      rtnVal = (rpc_binding_handle_t *)NULL;
    }

    /* get another binding handle for the desired bosserver */
    if (rtnVal != (rpc_binding_handle_t *)NULL) {
      rpc_binding_from_string_binding(theServerString, &theServer2Binding, &rpc_error);
      if (rpc_error != error_status_ok) {
	fprintf(stderr, "%s: unable to obtain second binding from string: %s\n",
		programName, RgySec_ErrorString(&rpc_error));
	rtnVal = (rpc_binding_handle_t *)NULL;
      }
    }

#if defined(BOZO_PRINT_TRACE)
    fprintf(stderr, "%s: binding achieved: %s\n", programName, theBosserverString);
#endif /* defined(BOZO_PRINT_TRACE) */

    rpc_string_free(&theServerString, &rpc_error);
  }

  return rtnVal;
}

rpc_binding_handle_t * GetBindingFromServerString(hostString)
     char *	hostString;
{
  rpc_binding_handle_t *	rtnVal = (rpc_binding_handle_t *)NULL;
  char *			netAddrStr;

  netAddrStr = hostutil_NetAddrStr(hostString);

  if (netAddrStr != (char *)NULL) {
    rtnVal = GetServerBinding(netAddrStr, AUTH_SERVER_PORT);
  }

  return rtnVal;
}


#define DEFAULT_MACHINE_INDEX		12
#define DEFAULT_NOAUTH_INDEX		13
#define DEFAULT_LOCALAUTH_INDEX		14
#define DEFAULT_NULLAUTH_INDEX		15
#define DEFAULT_DEBUG_INDEX		16
#define DEFAULT_RPCDEBUG_INDEX		17
#define DEFAULT_RESOLVE_INDEX		18
/* initialization and termination code */
int InitProc(aSyntaxP, aRockP)
     struct cmd_syndesc *	aSyntaxP;
     char *			aRockP;
{
  int				rtnVal = 0;
  rpc_binding_handle_t *	theServerBindingP;
  char *			serverLocationP = (char *)NULL;
  unsigned_char_t *		serverPrinNameP;
  error_status_t		rpcauthErr;
  static char			routineName[] = "InitProc";
rpc_binding_handle_t theNewBinding;

  initialize_dau_error_table();

  if ((strcmp(aSyntaxP->name, "help") != 0) && (strcmp(aSyntaxP->name, "apropos") != 0)) {
    if (cmd_IsParamPresent(aSyntaxP, DEFAULT_DEBUG_INDEX)) {
      (void)ParseDebugFlag(cmd_FirstData(aSyntaxP, DEFAULT_DEBUG_INDEX),
			   &dfsauth_debug_flag);
    }

    if (cmd_IsParamPresent(aSyntaxP, DEFAULT_RPCDEBUG_INDEX)) {
      rpc__dbg_set_switches(cmd_FirstData(aSyntaxP, DEFAULT_RPCDEBUG_INDEX), &rpcauthErr);
      if (rpcauthErr != error_status_ok) {
	fprintf(stderr, "%s: rpc debug flag setting failed: %s\n",
		routineName, RgySec_ErrorString(&rpcauthErr));
      }
    }

    theServerBindingP = GetBindingFromServerString((char *)NULL);
    if (theServerBindingP != (rpc_binding_handle_t *)NULL) {
      if (cmd_IsParamPresent(aSyntaxP, DEFAULT_RESOLVE_INDEX)) {
#if defined(rpc5b)
	fprintf(stderr, "%s: resolve flag ignored for this version of RPC (5b)",
		programName);
#else /* defined(rpc5b) */
	fprintf(stdout, "%s: about to call rpc_ep_resolve_binding\n", routineName);
	fflush(stdout);
	rpc_ep_resolve_binding(theServerBinding,
			       AUTH_v0_0_c_ifspec,
			       &rpcauthErr);
	fprintf(stdout, "%s: returned from call to rpc_ep_resolve_binding\n",
		routineName);
	fflush(stdout);
	if (rpcauthErr) {
	  fprintf(stderr, "%s: rpc_ep_resolve_binding returned error %s\n",
		  routineName, RgySec_ErrorString(&rpcauthErr));
	}
#endif /* defined(rpc5b) */
      }

      if (cmd_IsParamPresent(aSyntaxP, DEFAULT_MACHINE_INDEX)) {
	serverLocationP = cmd_FirstData(aSyntaxP, DEFAULT_MACHINE_INDEX);
      }

      if (cmd_IsParamPresent(aSyntaxP, DEFAULT_NULLAUTH_INDEX)) {
	fprintf(stderr, "%s: performing NULL prin authn setup\n", routineName);
	rtnVal =
	  dfsauth_client_InitNullAuthentication(theServerBindingP,
						serverLocationP,
						cmd_IsParamPresent(aSyntaxP,
								   DEFAULT_NOAUTH_INDEX),
						cmd_IsParamPresent(aSyntaxP,
							   DEFAULT_LOCALAUTH_INDEX));
      }
      else {
	fprintf(stderr, "%s: performing regular prin authn setup\n", routineName);
	rtnVal =
	  dfsauth_client_InitAuthentication(theServerBindingP,
					    serverLocationP,
					    cmd_IsParamPresent(aSyntaxP,
							       DEFAULT_NOAUTH_INDEX),
					    cmd_IsParamPresent(aSyntaxP,
							       DEFAULT_LOCALAUTH_INDEX));
#ifdef notdef
	{
	  error_status_t		addlAuthError;
	  dce_error_string_t		authErrorString;

	  rpc_binding_inq_auth_info(*theServerBindingP,
				    (unsigned_char_p_t *)NULL,
				    (unsigned32 *)NULL,
				    (unsigned32 *)NULL,
				    (rpc_auth_identity_handle_t *)NULL,
				    (unsigned32 *)NULL,
				    &addlAuthError);
	  if (addlAuthError == error_status_ok) {
	    fprintf(stdout, "source binding has auth info\n");
	  }
	  else {
	    error_status_t	otherAddlAuthError;

	    if (addlAuthError == rpc_s_binding_has_no_auth) {
	      fprintf(stdout, "source binding has no info\n");
	    }
	    else {
	      dce_error_inq_text(addlAuthError, authErrorString, &otherAddlAuthError);
	      if (otherAddlAuthError == error_status_ok) {
		fprintf(stdout, "error getting auth info from source binding (%ld): %s\n",
			addlAuthError, authErrorString);
	      }
	      else {
		fprintf(stdout,
			"%s: %d: error translating error code (%ld) into string: %ld\n",
			__FILE__, __LINE__, addlAuthError, otherAddlAuthError);
	      }
	    }
	  }
	}

	{
	  error_status_t		addlAuthError;
	  dce_error_string_t		authErrorString;

	  rpc_binding_copy(*theServerBindingP, &theNewBinding, &addlAuthError);
	  if (addlAuthError != error_status_ok) {
	    error_status_t otherAddlAuthError;

	    dce_error_inq_text(addlAuthError, authErrorString, &otherAddlAuthError);
	    if (otherAddlAuthError == error_status_ok) {
	      fprintf(stdout, "error copying binding (%ld): %s\n",
		      addlAuthError, authErrorString);
	    }
	    else {
	      fprintf(stdout,
		      "%s: %d: error translating error code (%ld) into string: %ld\n",
		      __FILE__, __LINE__, addlAuthError, otherAddlAuthError);
	    }
	  }
	}

	{
	  error_status_t		addlAuthError;
	  dce_error_string_t		authErrorString;

	  rpc_binding_inq_auth_info(theNewBinding,
				    (unsigned_char_p_t *)NULL,
				    (unsigned32 *)NULL,
				    (unsigned32 *)NULL,
				    (rpc_auth_identity_handle_t *)NULL,
				    (unsigned32 *)NULL,
				    &addlAuthError);
	  if (addlAuthError == error_status_ok) {
	    fprintf(stdout, "dest binding has auth info\n");
	  }
	  else {
	    error_status_t	otherAddlAuthError;

	    if (addlAuthError == rpc_s_binding_has_no_auth) {
	      fprintf(stdout, "dest binding has no info\n");
	    }
	    else {
	      dce_error_inq_text(addlAuthError, authErrorString, &otherAddlAuthError);

	      if (otherAddlAuthError == error_status_ok) {
		fprintf(stdout, "error getting auth info from dest binding (%ld): %s\n",
			addlAuthError, authErrorString);
	      }
	      else {
		fprintf(stdout,
			"%s: %d: error translating error code (%ld) into string: %ld\n",
			__FILE__, __LINE__, addlAuthError, otherAddlAuthError);
	      }
	    }
	  }
	}
#endif /* notdef */
      }

      if (rtnVal == 0) {
	if (cmd_IsParamPresent(aSyntaxP, DEFAULT_NULLAUTH_INDEX)) {
	  rpc_binding_inq_auth_info(*theServerBindingP,
				    &serverPrinNameP,
				    (unsigned32 *)NULL,
				    (unsigned32 *)NULL,
				    (rpc_auth_identity_handle_t *)NULL,
				    (unsigned32 *)NULL,
				    &rpcauthErr);
	  if (rpcauthErr == error_status_ok) {
	    fprintf(stderr, "%s: server name is %s\n", routineName, serverPrinNameP);
	  }
	  else {
	    fprintf(stderr, "%s: error obtaining server prin name: %s\n",
		    routineName, RgySec_ErrorString(&rpcauthErr));
	  }
	}
      }
    }
    else {
      fprintf(stderr, "%s: unable to obtain server binding", programName);
      if (h_errno != 0) {
	fprintf(stderr, ": %d", h_errno);
      }
      fprintf(stderr, ".\n");
      rtnVal = 1;
    }
  }

  return rtnVal;
}

int TermProc(aSyntaxP, aRockP)
     struct cmd_syndesc *	aSyntaxP;
     char *			aRockP;
{
  /* the return value from this routine is completely ignored */
  long				rtn = 0;
  static char	routineName[] = "TermProc";

  if ((strcmp(aSyntaxP->name, "help") != 0) && (strcmp(aSyntaxP->name, "apropos") != 0)) {
/*
    rtn = dfsauth_client_CleanupAuthentication(cmd_IsParamPresent(aSyntaxP,
							  DEFAULT_LOCALAUTH_INDEX));
*/
    if (rtn != 0) {
      com_err(programName, rtn, "(%s)", routineName);
    }
  }
}

int DoSimple(aSyntaxP, aRockP)
     struct cmd_syndesc *	aSyntaxP;
     char *			aRockP;
{
  volatile int		rtnVal = 0;
  long			rpcRtnVal = 0;
  error_status_t	rpcCommStatus;
  static char		routineName[] = "DoSimple";

  TRY
    rpcRtnVal = AUTH_ServerPlaceHolder(theServerBinding, &rpcCommStatus);
    if (rpcCommStatus == error_status_ok) {
      fprintf(stderr, "%s: %s: returned: %s\n", routineName, "AUTH_ServerPlaceHolder",
	      dfs_dceErrTxt(rpcRtnVal));
    }
    else {
      fprintf(stderr, "%s: %s: error: %s\n", routineName, "AUTH_ServerPlaceHolder",
	      RgySec_ErrorString(&rpcCommStatus));
      rtnVal = 1;
    }
  CATCH_ALL
    PrintException(THIS_CATCH, routineName, "AUTH_ServerPlaceHolder", __FILE__, __LINE__);
    rtnVal = 2;
  ENDTRY;

  return rtnVal;
}

int DoKill(aSyntaxP, aRockP)
     struct cmd_syndesc *	aSyntaxP;
     char *			aRockP;
{
  volatile int		rtnVal = 0;
  long			rpcRtnVal = 0;
  error_status_t	rpcCommStatus;
  static char		routineName[] = "DoKill";

  TRY
    rpcRtnVal = AUTH_ServerKill(theServerBinding, &rpcCommStatus);
    if (rpcCommStatus == error_status_ok) {
      fprintf(stderr, "%s: %s: returned: %s\n", routineName, "AUTH_ServerKill",
	      dfs_dceErrTxt(rpcRtnVal));
    }
    else {
      fprintf(stderr, "%s: %s: error: %s\n", routineName, "AUTH_ServerKill",
	      RgySec_ErrorString(&rpcCommStatus));
      rtnVal = 1;
    }
  CATCH_ALL
    PrintException(THIS_CATCH, routineName, "AUTH_ServerKill", __FILE__, __LINE__);
    rtnVal = 2;
  ENDTRY;

  return rtnVal;
}

int DoGetDebug(aSyntaxP, aRockP)
     struct cmd_syndesc *	aSyntaxP;
     char *			aRockP;
{
  volatile int		rtnVal = 0;
  long			rpcRtnVal = 0;
  long			serverDebugVal;
  unsigned_char_t *	stringBinding;
  error_status_t	rpcCommStatus;
  static char		routineName[] = "DoGetDebug";

  TRY
    rpcRtnVal = AUTH_ServerGetDebug(theServerBinding, &serverDebugVal, &rpcCommStatus);
    if (rpcCommStatus == error_status_ok) {
      if (rpcRtnVal == 0) {
	fprintf(stdout, "%s: server debug flag is %#x\n", routineName, serverDebugVal);
      }
      else {
	fprintf(stderr, "%s: %s: returned: %s\n", routineName, "AUTH_ServerGetDebug",
		RgySec_ErrorString((error_status_t *)&rpcRtnVal));
      }
    }
    else {
      fprintf(stderr, "%s: %s: error: %s\n", routineName, "AUTH_ServerGetDebug",
	      RgySec_ErrorString(&rpcCommStatus));
      rtnVal = 1;
    }
  CATCH_ALL
    PrintException(THIS_CATCH, routineName, "AUTH_ServerGetDebug", __FILE__, __LINE__);
    rtnVal = 2;
  ENDTRY;


#ifdef notdef
  rpc_binding_to_string_binding(theServerBinding, &stringBinding, &rpcCommStatus);
  if (rpcCommStatus == error_status_ok) {
    fprintf(stdout, "%s: the original string binding: %s\n", routineName, stringBinding);
  }
  else {
    fprintf(stderr, "%s: %s: error: %s\n", routineName, "rpc_binding_to_string_binding",
	    RgySec_ErrorString(&rpcCommStatus));
    rtnVal = 1;
  }

  if (rtnVal == 0) {
    TRY
      rpcRtnVal = AUTH2_ServerGetDebug(theServer2Binding, &serverDebugVal, &rpcCommStatus);
      if (rpcCommStatus == error_status_ok) {
	if (rpcRtnVal == 0) {
	  fprintf(stdout, "%s: 2nd server debug flag is %#x\n", routineName, serverDebugVal);
	}
	else {
	  fprintf(stderr, "%s: %s: returned: %s\n", routineName, "AUTH2_ServerGetDebug",
		  RgySec_ErrorString((error_status_t *)&rpcRtnVal));
	}
      }
      else {
	fprintf(stderr, "%s: %s: error: %s\n", routineName, "AUTH2_ServerGetDebug",
		RgySec_ErrorString(&rpcCommStatus));
	rtnVal = 1;
      }
  CATCH_ALL
    PrintException(THIS_CATCH, routineName, "AUTH2_ServerGetDebug", __FILE__, __LINE__);
    rtnVal = 2;
  ENDTRY;

    rpc_binding_to_string_binding(theServer2Binding, &stringBinding, &rpcCommStatus);
    if (rpcCommStatus == error_status_ok) {
      fprintf(stdout, "%s: the second string binding: %s\n", routineName, stringBinding);
    }
    else {
      fprintf(stderr, "%s: %s: error: %s\n", routineName, "rpc_binding_to_string_binding",
	      RgySec_ErrorString(&rpcCommStatus));
      rtnVal = 1;
    }
  }
#endif /* notdef */

  return rtnVal;
}

#define SETDEBUG_NEWVAL_INDEX	0
int DoSetDebug(aSyntaxP, aRockP)
     struct cmd_syndesc *	aSyntaxP;
     char *			aRockP;
{
  volatile int		rtnVal = 0;
  long			rpcRtnVal = 0;
  long			newServerDebugVal = 0;
  long			oldServerDebugVal = 0;
  error_status_t	rpcCommStatus;
  static char		routineName[] = "DoSetDebug";

  (void)ParseDebugFlag(cmd_FirstData(aSyntaxP, SETDEBUG_NEWVAL_INDEX),
		       &newServerDebugVal);

  TRY
    rpcRtnVal = AUTH_ServerSetDebug(theServerBinding,
				    newServerDebugVal, &oldServerDebugVal, &rpcCommStatus);
    if (rpcCommStatus == error_status_ok) {
      if (rpcRtnVal == 0) {
	fprintf(stdout, "%s: server debug flag was %#x\n", routineName,
		oldServerDebugVal);
      }
      else {
	fprintf(stderr, "%s: %s: returned: %s\n", routineName, "AUTH_ServerSetDebug",
		dfs_dceErrTxt(rpcRtnVal));
      }
    }
    else {
      fprintf(stderr, "%s: %s: error: %s\n", routineName, "AUTH_ServerSetDebug",
	      RgySec_ErrorString(&rpcCommStatus));
      rtnVal = 1;
    }
  CATCH_ALL
    PrintException(THIS_CATCH, routineName, "AUTH_ServerSetDebug", __FILE__, __LINE__);
    rtnVal = 2;
  ENDTRY;

  return rtnVal;
}

void SetUpDefaults(aSyntaxP)
     struct cmd_syndesc * aSyntaxP;
{
  cmd_IndexedAddParm(aSyntaxP, "-machine", CMD_SINGLE, CMD_OPTIONAL,
	      "machine name", DEFAULT_MACHINE_INDEX);
  cmd_IndexedAddParm(aSyntaxP, "-noauth", CMD_FLAG, CMD_OPTIONAL, "",
		     DEFAULT_NOAUTH_INDEX);
  cmd_IndexedAddParm(aSyntaxP, "-localauth", CMD_FLAG, CMD_OPTIONAL, "",
		     DEFAULT_LOCALAUTH_INDEX);
  cmd_IndexedAddParm(aSyntaxP, "-nullauth", CMD_FLAG, CMD_OPTIONAL,
		     "", DEFAULT_NULLAUTH_INDEX);
  cmd_IndexedAddParm(aSyntaxP, "-debug", CMD_SINGLE, CMD_OPTIONAL, "debug flag value",
		     DEFAULT_DEBUG_INDEX);
  cmd_IndexedAddParm(aSyntaxP, "-rpcdebug", CMD_SINGLE, CMD_OPTIONAL,
		     "rpc debug flag value", DEFAULT_RPCDEBUG_INDEX);
  cmd_IndexedAddParm(aSyntaxP, "-resolve", CMD_FLAG, CMD_OPTIONAL,
		     "", DEFAULT_RESOLVE_INDEX);
}

void SetUpSimpleCall()
{
  struct cmd_syndesc *	simpleSyntaxP;

  simpleSyntaxP = cmd_CreateSyntax("simple", DoSimple,
				   (char *)NULL, "make simple RPC");
  SetUpDefaults(simpleSyntaxP);
}

void SetUpKill()
{
  struct cmd_syndesc * killSyntaxP;

  killSyntaxP = cmd_CreateSyntax("kill", DoKill,
				 (char *)NULL, "kill server");
  SetUpDefaults(killSyntaxP);
}

void SetUpGetDebug()
{
  struct cmd_syndesc * getDebugSyntaxP;

  getDebugSyntaxP = cmd_CreateSyntax("getdebug", DoGetDebug,
				     (char *)NULL, "get server flag value");
  SetUpDefaults(getDebugSyntaxP);
}

void SetUpSetDebug()
{
  struct cmd_syndesc * setDebugSyntaxP;

  setDebugSyntaxP = cmd_CreateSyntax("setdebug", DoSetDebug,
				     (char *)NULL, "set server flag value");
  cmd_AddParm(setDebugSyntaxP, "-serverdebug", CMD_SINGLE, CMD_REQUIRED,
	       "new server debug flag value");
  SetUpDefaults(setDebugSyntaxP);
}

SetUpSyntax()
{
  SetUpSimpleCall();
  SetUpKill();
  SetUpGetDebug();
  SetUpSetDebug();
}


int main(argc, argv)
     int	argc;
     char *	argv[];
{
  int			rtnVal = 0;

  (void)strcpy(programName, argv[0]);

  SetUpSyntax();
  (void)cmd_SetBeforeProc(InitProc, (char *)NULL);
  (void)cmd_SetAfterProc(TermProc, (char *)NULL);

  rtnVal = cmd_Dispatch(argc, argv);

  return rtnVal;
}
