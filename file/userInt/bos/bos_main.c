/*-
 * @TRANSARC_COPYRIGHT@
 */

#if !defined(_NOIDENT) && !defined(lint)
static char *TARCSId = "$Id: bos_main.c,v 4.60 1996/04/19 13:23:44 gait Exp $";
#endif

/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 *  bos_main.c -- routines that do most of the work for the implementation of
 * the bos command suite
 *
 *
 *	Copyright (C) 1995, 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>

#include <dcedfs/ktime.h>
/*
#include <dcedfs/common_data.h>
 */
#include <com_err.h>
#include <dcedfs/cmd.h>
#include <dcedfs/cmdAux.h>
#include <dcedfs/dfsauth.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbosmac.h>
#include <dfsbossvc.h>
#include <dfsbosmsg.h>

#ifdef use_cds_compat
#include <dcedfs/compat_errs.h>
/*
#include <dcedfs/compat_cds.h>
 */
#endif /* use_cds_compat */

#include <dce/rpc.h>

#include <bos_hostutils.h>

#include <dcedfs/bbos_util.h>
#include <dcedfs/bbos_ncs_interface.h>
#include <dcedfs/bbos_pathnames.h>
#include <dcedfs/bbos_bnode.h>

#include <bos_pipeFile.h>
#include <bos_userint.h>

#include <dce/rpcexc.h>


dce_svc_handle_t   bos_svc_handle;

void initialize_svc()
{
  error_status_t st;

  bos_svc_handle = dce_svc_register(bos_svc_table,  (idl_char *)"bos",  &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot register svc 0x%lx\n", st);
  dce_svc_routing((unsigned char *) "NOTICE:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route NOTICE messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "WARNING:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route WARNING messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "ERROR:STDERR:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route ERROR messages  0x%lx\n", st);

  dfs_define_bos_message_tables();

}



#define BOSSVR_CELLGIVEN(theSyntax)	cmd_IsParamPresent(theSyntax, BOS_CELL_INDEX)

#define DEFAULT_DEST_DIR	afs_bin_dir

#define MAX_PARAMS	6

IMPORT char * dfs_dceErrTxt _TAKES((
				 unsigned long st
				 ));

IMPORT void bos_ui_SetUpSyntax _TAKES((
				       void
				     ));

IMPORT int h_errno;

IMPORT long volutil_GetPartitionID _TAKES((
					    char * aname
					  ));

IMPORT char * volutil_PartitionName _TAKES((
						long avalue
					      ));
#ifdef	AFS_AIX_ENV
IMPORT int atoi _TAKES((
			const char *	intString
		      ));
#endif /* AFS_AIX_ENV */

#ifdef compileEgs
int interactiveDepth = 0;
#endif /* compileEgs */

char			programName[BUFSIZ];
rpc_binding_handle_t	theBosserverBinding;

/* debugging */
PRIVATE long	bos_debug_flag = 0;
#define BOS_DEBUG_BIT_CMD	1
#define BOS_DEBUG_BIT_DCE	2


/* get a binding handle to be used for communication with the bosserver */
rpc_binding_handle_t * GetBosserverBinding(netAddrStr, endPointStr)
     unsigned char *	netAddrStr;
     unsigned char *	endPointStr;
{
  rpc_binding_handle_t *	rtnVal = &theBosserverBinding;
  static unsigned_char_t	emptyStr[] = "";
  static unsigned_char_t	protSeqStr[] = "ncadg_ip_udp";

  unsigned_char_t *		theBosserverString;
  error_status_t		rpc_error = error_status_ok;

  /* make up a string representation of the desired binding */
  /* we don't have a particular object to access */
  rpc_string_binding_compose(/* uuid = */ emptyStr,
			     protSeqStr,
			     netAddrStr,
			     /* endpoint = */ endPointStr,
			     /* options = */ emptyStr,
			     &theBosserverString,
			     &rpc_error);
  if (rpc_error != error_status_ok) {
    dce_svc_printf(BOS_S_NO_STRING_BIND_MSG, programName, dfs_dceErrTxt(rpc_error));
    rtnVal = (rpc_binding_handle_t *)NULL;
  }
  else {
    /* get a binding handle for the desired bosserver */
    rpc_binding_from_string_binding(theBosserverString, &theBosserverBinding, &rpc_error);
    if (rpc_error != error_status_ok) {
      dce_svc_printf(BOS_S_NO_BINDING_OBTAINED_MSG, programName, dfs_dceErrTxt(rpc_error));
      rtnVal = (rpc_binding_handle_t *)NULL;
    }

    dmprintf(bos_debug_flag, BOS_DEBUG_BIT_DCE,
	     ("%s: binding achieved: %s", programName, theBosserverString));

    rpc_string_free(&theBosserverString, &rpc_error);
  }

  return rtnVal;
}

rpc_binding_handle_t * GetBindingFromSynDesc(aSyntax)
     struct cmd_syndesc *	aSyntax;
{
  rpc_binding_handle_t *	rtnVal = (rpc_binding_handle_t *)NULL;
  char *			hostStr;
  char *			netAddrStr;
  int				useNoauth = cmd_IsParamPresent(aSyntax, BOS_NOAUTH_INDEX);
  int				useLocalauth = cmd_IsParamPresent(aSyntax, BOS_LOCAL_INDEX);
  error_status_t		cdsError;
  uuid_t			nil_uuid;

  hostStr = ((cmd_IsParamPresent(aSyntax, BOS_SERVER_INDEX)) ?
	     aSyntax->parms[BOS_SERVER_INDEX].items->data :
	     (char *)NULL);
#ifdef use_cds_compat
  uuid_create_nil(&nil_uuid, (unsigned32 *)&cdsError);
  if (cdsError == uuid_s_ok) {
  dfs_locate_rpc_server(aSyntax->parms[BOS_SERVER_INDEX].items->data,
			&theBosserverBinding, &nil_uuid,
			useLocalauth, useNoauth,
			&cdsError);
  if ((cdsError == error_status_ok) ||
      (cdsError == COMPAT_ERROR_SHORTNAME_NOAUTH)) {
    rtnVal = &theBosserverBinding;

    /* print warning if noauth mode forced */
    if (cdsError == COMPAT_ERROR_SHORTNAME_NOAUTH) {
      dce_svc_printf(BOS_S_SHORT_NAME_USED_MSG, programName);
    }
  }
  else {
    /*
     * It only does any good to retry noauth if we were trying to use the caller's
     * default creds.
     */
    if ((cdsError == COMPAT_ERROR_NO_CREDENTIALS) &&
	(useNoauth == 0) && (useLocalauth == 0)) {
      /* try noauth mode */
      dfs_locate_rpc_server(aSyntax->parms[BOS_SERVER_INDEX].items->data,
			    &theBosserverBinding, &nil_uuid,
			    0 /* try noLocalAuth */, 1 /* try useNoAuth */,
			    &cdsError);

      if (cdsError == error_status_ok) {
	rtnVal = &theBosserverBinding;
	dce_svc_printf(BOS_S_NO_AUTH_INFO_SENT_MSG, programName);
      }
    }

    if (cdsError != error_status_ok) {
      dce_svc_printf(BOS_S_NO_SERVER_FOUND_MSG, programName, dfs_dceErrTxt(cdsError));
    }
  }
 }
 else
   dce_svc_printf(BOS_S_NO_NIL_UUID_MSG, programName, dfs_dceErrTxt(cdsError));
#else /* use_cds_compat */
  netAddrStr = hostutil_NetAddrStr(hostStr);

  if (netAddrStr != (char *)NULL) {
    rtnVal = GetBosserverBinding(netAddrStr, (char *)NULL);
  }
#endif /* use_cds_compat */

  return rtnVal;
}


/* routines that do the real work go here */

/* initialization and termination code */
int InitProc(aSyntaxP, aRock)
     struct cmd_syndesc *	aSyntaxP;
     char *			aRock;
{
  /* if this routine returns anything other than 0, nothing further will be done */

  int				rtnVal = 0;
  char *			serverLocationP = (char *)NULL;
  char				serverBuffer[512];
  rpc_binding_handle_t *	theBosserverBindingP;
  int				useNoauth = cmd_IsParamPresent(aSyntaxP,
							       BOS_NOAUTH_INDEX);
  int				useLocalauth = cmd_IsParamPresent(aSyntaxP,
								  BOS_LOCAL_INDEX);


  if ((strcmp(aSyntaxP->name, "help") != 0) && (strcmp(aSyntaxP->name, "-help") != 0) &&
      (strcmp(aSyntaxP->name, "apropos") != 0)) {
#ifdef notdef
  /*
   * All the real bos commands now require a server parameter, so, if that param is
   * not present, this is probably a cmd package command.
   */
  if (cmd_IsParamPresent(aSyntaxP, BOS_SERVER_INDEX)) {

    /*
     * This "fix" didn't do the right thing if there were addl parameters to the
     * help or apropos commands.
     */
#endif /* notdef */

    theBosserverBindingP = GetBindingFromSynDesc(aSyntaxP);

    if (theBosserverBindingP != (rpc_binding_handle_t *)NULL) {
#ifndef use_cds_compat
/* the cds routine will also set up the auth info */
      if (cmd_IsParamPresent(aSyntaxP, BOS_SERVER_INDEX)) {
	serverLocationP = cmd_FirstData(aSyntaxP, BOS_SERVER_INDEX);
	sprintf(serverBufer, "hosts/%s", serverLocationP);
      }

      rtnVal =
	dfsauth_client_InitAuthentication(theBosserverBindingP,
					  serverBuffer,
					  useNoauth, useLocalauth);
      if (rtnVal != 0) {
	/*
	 *  It only makes sense to retry noauth if we had been depending on the caller's
	 * default credentials.
	 */
	if ((rtnVal == DAUT_ERROR_NO_CREDENTIALS_FOUND) &&
	    (useNoauth == 0) && (useLocalauth == 0)) {
	  rtnVal =
	    dfsauth_client_InitAuthentication(theBosserverBindingP,
					      serverBuffer,
					      1 /* try useNoauth */,
					      0 /* try noLocalauth */);

	  if (rtnVal == 0) {
	    dce_svc_printf(BOS_S_NOAUTH_MODE_FORCED_MSG);
	  }
	}

	if (rtnVal != 0) {
	  com_err(programName, rtnVal, "(authentication initialization)");
	}
      }
#endif /* use_cds_compat */
    }
    else {
      dce_fprintf(stderr, bos_s_no_bossrvr_binding, programName);
      if (h_errno != 0) {
	dce_fprintf(stderr, bos_s_no_bossrvr_binding2, HostErrorString(h_errno));
      }
      dce_fprintf(stderr, bos_s_no_bossrvr_binding3);
      rtnVal = 1;
    }
  }
  return rtnVal;
}

int TermProc(aSyntaxP, aRockP)
     struct cmd_syndesc *	aSyntaxP;
     char *			aRockP;
{
  long		dauthRtnVal;
  static char	routineName[] = "TermProc";

  /*
   * this routine is executed as long as the command routine did not exit the pgm;
   * and its return value is completely ignored
   */

  if ((strcmp(aSyntaxP->name, "help") != 0) && (strcmp(aSyntaxP->name, "-help") != 0) &&
      (strcmp(aSyntaxP->name, "apropos") != 0)) {
#ifdef notdef
  /*
   * All the real bos commands now require a server parameter, so, if that param is
   * not present, this is probably a cmd package command.
   */
  if (cmd_IsParamPresent(aSyntaxP, BOS_SERVER_INDEX)) {
    /*
     * This "fix" didn't do the right thing if there were addl parameters to the
     * help or apropos commands.
     */
#endif /* notdef */
    dauthRtnVal = dfsauth_client_CleanupAuthentication();
    if (dauthRtnVal != 0) {
      com_err(programName, dauthRtnVal, routineName);
    }
  }

  return (0);
}

/*
 * these routines determine the value returned by cmd_Dispatch (as long as one of
 * the routines is run - see comment in the init routine, above)
 */
int bos_DoAddKey(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  long			bossvr_error = 0;
  error_status_t	rpc_error = error_status_ok;
  char *		errorStringP = "";
  struct bossvr_key	newKey;
  long			newKvno;
  char			cellBuffer[256];
  char *		tcell;
  char			principalName [BUFSIZ];
  int			kvno = 0;
  static char		routineName[] = "bos_DoAddKey";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));

  if (cmd_IsParamPresent(aSyntax, ADDKEY_PRINCIPAL_INDEX)) {
    (void)strncpy(principalName, cmd_FirstData(aSyntax, ADDKEY_PRINCIPAL_INDEX),
		  sizeof(principalName) - 1);
  }
  else {
    principalName[0] = '\0';
  }

  /* the validation routine will only let a single "+" or a valid kvno though */
  /* rely on kvno initialized to 0 */
  if (strcmp(cmd_FirstData(aSyntax, ADDKEY_KVNO_INDEX), "+") != 0) {
    kvno = atoi(cmd_FirstData(aSyntax, ADDKEY_KVNO_INDEX));
  }

  bossvr_error = dfsauth_client_SetFullEncryption(&theBosserverBinding);
  if ((bossvr_error == 0) ||
      (bossvr_error == DAUT_ERROR_PROT_LEVEL_UNSUPPORTED) ||
      ((bossvr_error == DAUT_ERROR_CLIENT_SERVICE_NOT_SUPP_NOAUTH) &&
       (cmd_IsParamPresent(aSyntax, BOS_NOAUTH_INDEX)))) {
    if (bossvr_error == DAUT_ERROR_PROT_LEVEL_UNSUPPORTED)
	dce_svc_printf(BOS_S_NO_ENCRYPT_BY_RPC_MSG);

    bossvr_error = BOSSVR_AddKey(theBosserverBinding,
				 (unsigned char *)principalName,
				 kvno,
				 (unsigned char *)
				    cmd_FirstData(aSyntax,
					ADDKEY_PASSWORD_INDEX),
				 cmd_IsParamPresent(aSyntax,
				    ADDKEY_LOCALONLY_INDEX),
				 &rpc_error);

    if (rpc_error == rpc_s_unsupported_protect_level
	|| rpc_error == rpc_s_unknown_reject) {
	dce_svc_printf(BOS_S_NO_DES_MSG);

	bossvr_error = dfsauth_client_ResetDefaultEncryption(&theBosserverBinding);
	if (bossvr_error == 0) {
	    bossvr_error = BOSSVR_AddKey(theBosserverBinding,
					 (unsigned char *)principalName,
					 kvno,
					 (unsigned char *)
					    cmd_FirstData(aSyntax,
						ADDKEY_PASSWORD_INDEX),
					 cmd_IsParamPresent(aSyntax,
					    ADDKEY_LOCALONLY_INDEX),
					 &rpc_error);
	}
    }
  }

  if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
    errorStringP = ((rpc_error != error_status_ok) ?
		    dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
    dce_svc_printf(BOS_S_NO_KEY_SET_MSG, programName, cmd_FirstData(aSyntax,
		    ADDKEY_KVNO_INDEX), errorStringP);
    rtnVal = 1;
  }

  return rtnVal;
}

int bos_DoAddUser(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int				everFailed = 0;
  long				bossvr_error = 0;
  error_status_t		rpc_error = error_status_ok;
  char *			filenameP;
  char *			errorStringP = "";
  ulong				flags = 0;
  register struct cmd_item *	thisItemP;
  bossvr_out_string             cellnameBuffer;
  unsigned char                 *typeStr;
  static char		routineName[] = "bos_DoAddUser";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));


  cellnameBuffer.strLen = 0;
  cellnameBuffer.theString = (unsigned char *)NULL;
  bossvr_error = BOSSVR_GetCellName(theBosserverBinding, &cellnameBuffer, &rpc_error);
  if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
    errorStringP = ((rpc_error != error_status_ok) ?
                    dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
    dce_fprintf(stderr, bos_s_cell_name, programName, errorStringP);
    return 1;
  }

  filenameP = cmd_FirstData(aSyntax, ADDUSER_FILENAME_INDEX);
  if (cmd_IsParamPresent(aSyntax, ADDUSER_CREATEFLAG_INDEX)) {
    flags |= BOSSVR_ADDADMIN_CREATELIST;
  }

  if ((cmd_IsParamPresent(aSyntax, ADDUSER_USERS_INDEX)) ||
      (cmd_IsParamPresent(aSyntax, ADDUSER_GROUPS_INDEX))) {
    /* first, add the new users */
    for (thisItemP = aSyntax->parms[ADDUSER_USERS_INDEX].items;
	 thisItemP != (struct cmd_item *)NULL;
	 thisItemP = thisItemP->next) {

         if ((strncmp(thisItemP->data,"/",1) != 0) || (strncmp((char *)cellnameBuffer.theString,thisItemP->data,cellnameBuffer.strLen) == 0)) 
            typeStr=(unsigned char *)"user";  /* user must be in the local cell */
         else  
            typeStr=(unsigned char *)"foreign_user"; /* user is in foreign cell */ 

      bossvr_error = BOSSVR_AddSUser(theBosserverBinding,
				     (unsigned char *)filenameP,
				     typeStr, 
				     (unsigned char *)thisItemP->data,
				     (unsigned char *)/*NULL,*/ "",
				     flags,
				     &rpc_error);

	if (bossvr_error == BZ_ADMIN_FILE_NOT_FOUND) {
	  dce_svc_printf(BOS_S_ADMIN_FILE_NOT_FOUND_MSG, programName, filenameP);
	  break;
	}
	else {
	  if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
	    errorStringP = ((rpc_error != error_status_ok) ?
			    dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
	    dce_svc_printf(BOS_S_NO_USER_ADDED_MSG, programName, thisItemP->data,
			    errorStringP);
	    everFailed = 1;
	  }
      }
    }

    if (bossvr_error != BZ_ADMIN_FILE_NOT_FOUND) {
      /* now, add the new groups */
      for (thisItemP = aSyntax->parms[ADDUSER_GROUPS_INDEX].items;
	   thisItemP != (struct cmd_item *)NULL;
	   thisItemP = thisItemP->next) {
         if ((strncmp(thisItemP->data,"/",1) != 0) || (strncmp((char *)cellnameBuffer.theString,thisItemP->data,cellnameBuffer.strLen) == 0)) 
            typeStr=(unsigned char *)"group"; /* group must be in local cell */
         else  
            typeStr=(unsigned char *)"foreign_group"; /* group is in foreign cell */ 

	bossvr_error = BOSSVR_AddSUser(theBosserverBinding,
				       (unsigned char *)filenameP,
				       typeStr, 
				       (unsigned char *)thisItemP->data,
				       (unsigned char *)/*NULL,*/ "",
				       flags,
				       &rpc_error);

	if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
	  if (bossvr_error == BZ_ADMIN_FILE_NOT_FOUND) {
	    dce_svc_printf(BOS_S_ADMIN_FILE_NOT_FOUND2_MSG, programName, filenameP);
	    break;
	  }
	  else {
	    errorStringP = ((rpc_error != error_status_ok) ?
			    dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
	    dce_svc_printf(BOS_S_NO_GROUP_ADDED_MSG, programName, thisItemP->data,
			    errorStringP);
	    everFailed = 1;
	  }
	}
      }
    }
  }
  else {
    if ((flags & BOSSVR_ADDADMIN_CREATELIST) != 0) {
      bossvr_error = BOSSVR_AddSUser(theBosserverBinding,
				     (unsigned char *)filenameP,
				     BBOS_NO_TYPE,
				     BBOS_NO_NAME,
				     (unsigned char *)/*NULL,*/ "",
				     flags,
				     &rpc_error);
      if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
	errorStringP = ((rpc_error != error_status_ok) ?
			dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
	dce_svc_printf(BOS_S_NO_FILE_CREATED_MSG, programName, filenameP, errorStringP);
	everFailed = 1;
      }
    }
    else {
      dce_svc_printf(BOS_S_NO_ACTION_GIVEN_MSG, programName);
      everFailed = 1;
    }
  }

  return ((everFailed || bossvr_error) ? 1 : 0);
}

int bos_DoCreate(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  long			bossvr_error = 0;
  error_status_t	rpc_error = error_status_ok;
  char *		errorStringP = "";
  unsigned char *	type;
  unsigned char *	name;
  unsigned char *	params[MAX_PARAMS];
  int			i;
  struct cmd_item *	thisItemP;
  static char		routineName[] = "bos_DoCreate";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));

  for (i = 0, thisItemP = aSyntax->parms[CREATE_CMD_INDEX].items;
       (i < MAX_PARAMS) && (thisItemP != (struct cmd_item *)NULL);
       i++, thisItemP = thisItemP->next) {
    params[i] = (unsigned char *)thisItemP->data;
  }

  if (i < MAX_PARAMS) {
    for (; i < MAX_PARAMS; i++) {
      params[i] = (unsigned char *)"";
    }
  }
  else {
    if (thisItemP != (struct cmd_item *)NULL) {
      dce_svc_printf(BOS_S_EXCESS_PARMS_MSG, programName, params[MAX_PARAMS - 1]);
    }
  }

  name = (unsigned char *)aSyntax->parms[CREATE_PROCESS_INDEX].items->data;
  type = (unsigned char *)aSyntax->parms[CREATE_TYPE_INDEX].items->data;
  bossvr_error = BOSSVR_CreateBnode(theBosserverBinding, type, name,
				params[0],
				params[1],
				params[2],
				params[3],
				params[4],
				params[5],
				&rpc_error);

  if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
    errorStringP = ((rpc_error != error_status_ok) ?
		    dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
    dce_svc_printf(BOS_S_NO_SERVER_INSTANCE_MSG, programName, name, type, errorStringP);
    rtnVal = 1;
  }

  return rtnVal;
}

int bos_DoDelete(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  long			bossvr_error = 0;
  error_status_t	rpc_error = error_status_ok;
  char *		errorStringP = "";
  struct cmd_item *	thisItemP;
  static char		routineName[] = "bos_DoDelete";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));

  for (thisItemP = aSyntax->parms[DELETE_PROCESS_INDEX].items;
       thisItemP != (struct cmd_item *)NULL;
       thisItemP = thisItemP->next) {
    bossvr_error = BOSSVR_DeleteBnode(theBosserverBinding, (unsigned char *)thisItemP->data, &rpc_error);

    if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
      if (bossvr_error == BZBUSY) {
	dce_svc_printf(BOS_S_INSTANCE_NOT_DELETED_MSG, programName, thisItemP->data);
      }
      else {
	errorStringP = ((rpc_error != error_status_ok) ?
			dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
	dce_svc_printf(BOS_S_INSTANCE_NOT_DELETED2_MSG, programName, thisItemP->data,
			 errorStringP);
      }
      rtnVal = 1;
    }
  }

  return rtnVal;
}

int bos_DoGcKeys(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  long			bossvr_error = 0;
  error_status_t	rpc_error = error_status_ok;
  char *		errorStringP = "";
  char			principalName [BUFSIZ];

  if (cmd_IsParamPresent(aSyntax, GCKEYS_PRINCIPAL_INDEX)) {
    (void)strncpy(principalName, cmd_FirstData(aSyntax, GCKEYS_PRINCIPAL_INDEX),
		  sizeof(principalName) - 1);
  }
  else {
    principalName[0] = '\0';
  }

  bossvr_error = dfsauth_client_SetFullEncryption(&theBosserverBinding);
  if ((bossvr_error == 0) ||
      (bossvr_error == DAUT_ERROR_PROT_LEVEL_UNSUPPORTED) ||
      ((bossvr_error == DAUT_ERROR_CLIENT_SERVICE_NOT_SUPP_NOAUTH) &&
       (cmd_IsParamPresent(aSyntax, BOS_NOAUTH_INDEX)))) {
    if (bossvr_error == DAUT_ERROR_PROT_LEVEL_UNSUPPORTED)
	dce_svc_printf(BOS_S_NO_ENCRYPT_BY_RPC2_MSG);

    bossvr_error = BOSSVR_GarbageCollectKeys(theBosserverBinding,
					     (unsigned char *)principalName,
					     &rpc_error);

    if (rpc_error == rpc_s_unsupported_protect_level
	|| rpc_error == rpc_s_unknown_reject) {
	dce_svc_printf(BOS_S_NO_DES_MSG);

	bossvr_error = dfsauth_client_ResetDefaultEncryption(&theBosserverBinding);
	if (bossvr_error == 0) {
	    bossvr_error = BOSSVR_GarbageCollectKeys(theBosserverBinding,
						     (unsigned char *)principalName,
						     &rpc_error);
	}
    }
  }

  if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
    errorStringP = ((rpc_error != error_status_ok) ?
		    dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
    dce_svc_printf(BOS_S_NO_KEYS_COLLECTED_MSG, programName,
	    (cmd_IsParamPresent(aSyntax, GCKEYS_PRINCIPAL_INDEX) ?
	     cmd_FirstData(aSyntax, GCKEYS_PRINCIPAL_INDEX) : "server"),
	    errorStringP);
    rtnVal = 1;
  }

  return rtnVal;
}

int bos_DoGenKey(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  long			bossvr_error = 0;
  error_status_t	rpc_error = error_status_ok;
  int			kvno = 0;
  char *		errorStringP = "";
  char			principalName [BUFSIZ];

  if (cmd_IsParamPresent(aSyntax, GENKEY_PRINCIPAL_INDEX)) {
    (void)strncpy(principalName, cmd_FirstData(aSyntax, GENKEY_PRINCIPAL_INDEX),
		  sizeof(principalName) - 1);
  }
  else {
    principalName[0] = '\0';
  }

  /* the validation routine will only let a single "+" or a valid kvno though */
  /* rely on kvno initialized to 0 */
  if (strcmp(cmd_FirstData(aSyntax, GENKEY_KVNO_INDEX), "+") != 0) {
    kvno = atoi(cmd_FirstData(aSyntax, GENKEY_KVNO_INDEX));
  }

  bossvr_error = dfsauth_client_SetFullEncryption(&theBosserverBinding);
  if ((bossvr_error == 0) ||
      (bossvr_error == DAUT_ERROR_PROT_LEVEL_UNSUPPORTED) ||
      ((bossvr_error == DAUT_ERROR_CLIENT_SERVICE_NOT_SUPP_NOAUTH) &&
       (cmd_IsParamPresent(aSyntax, BOS_NOAUTH_INDEX)))) {
    if (bossvr_error == DAUT_ERROR_PROT_LEVEL_UNSUPPORTED)
	dce_svc_printf(BOS_S_NO_ENCRYPT_BY_RPC3_MSG);

    bossvr_error = BOSSVR_GenerateKey(theBosserverBinding,
				      (unsigned char *)principalName,
				      kvno, &rpc_error);

    if (rpc_error == rpc_s_unsupported_protect_level
	|| rpc_error == rpc_s_unknown_reject) {
	dce_svc_printf(BOS_S_NO_DES_MSG);

	bossvr_error = dfsauth_client_ResetDefaultEncryption(&theBosserverBinding);
	if (bossvr_error == 0) {
	    bossvr_error = BOSSVR_GenerateKey(theBosserverBinding,
					      (unsigned char *)principalName,
					      kvno, &rpc_error);
	}
    }
  }

  if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
    errorStringP = ((rpc_error != error_status_ok) ?
		    dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
    dce_svc_printf(BOS_S_NO_KEY_VERSION_MSG, programName,
	    atol(cmd_FirstData(aSyntax, GENKEY_KVNO_INDEX)),
	    (cmd_IsParamPresent(aSyntax, GENKEY_PRINCIPAL_INDEX) ?
	     cmd_FirstData(aSyntax, GENKEY_PRINCIPAL_INDEX) : "server"),
	    errorStringP);
    rtnVal = 1;
  }

  return rtnVal;
}


int bos_DoGetDates(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int				rtnVal = 0;
  error_status_t		rpc_error = error_status_ok;
  char *			errorStringP = "";
  long				bossvr_error = 0;
  struct cmd_item *		nextFileItemP;
  char				destDir[BUFSIZ];
  char				fullPathname[BUFSIZ];

  long				fileTime;
  long				bakFileTime;
  long				oldFileTime;
  char *			timeStringP;
  static char			routineName[] = "bos_DoGetDates";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));

  if (cmd_IsParamPresent(aSyntax, GETDATES_DESTDIR_INDEX)) {
    (void)strcpy(destDir, aSyntax->parms[GETDATES_DESTDIR_INDEX].items->data);
  }
  else {
    (void)strcpy(destDir, DEFAULT_DEST_DIR);
  }

  nextFileItemP = aSyntax->parms[GETDATES_FILE_INDEX].items;
  while ((nextFileItemP != (struct cmd_item *)NULL) && (rtnVal == 0)) {

    bosutil_ComputeDestDir(nextFileItemP->data, destDir,
			   fullPathname, sizeof(fullPathname));

    bossvr_error = BOSSVR_GetDates(theBosserverBinding,
			       (unsigned char *)fullPathname, &fileTime, &bakFileTime, &oldFileTime,
			       &rpc_error);

    if ((rpc_error == error_status_ok) && (bossvr_error == 0)) {
      dce_fprintf(stdout, bos_s_file_date_info1, fullPathname);
      if (fileTime == 0) {
	dce_fprintf(stdout, bos_s_file_date_info2a);
	rtnVal = 1;
      }
      else {
	dce_fprintf(stdout, bos_s_file_date_info2b, bosutil_DateOf(fileTime));
      }

      if (bakFileTime == 0) {
	dce_fprintf(stdout, bos_s_file_date_info3a);
	rtnVal &= 1;
      }
      else {
	dce_fprintf(stdout, bos_s_file_date_info3b, bosutil_DateOf(bakFileTime));
      }

      if (oldFileTime == 0) {
	dce_fprintf(stdout, bos_s_file_date_info4a);
	rtnVal &= 1;
      }
      else {
	dce_fprintf(stdout, bos_s_file_date_info4b, bosutil_DateOf(oldFileTime));
      }

      dce_fprintf(stdout, bos_s_file_date_info5);
      fflush(stdout);
    }
    else {
      errorStringP = ((rpc_error != error_status_ok) ?
		      dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
      dce_svc_printf(BOS_S_NO_DATE_CHECKED_MSG, programName, nextFileItemP->data,
		      errorStringP);
      rtnVal = 1;
    }
    nextFileItemP = nextFileItemP->next;
  }	/* end while */

  return rtnVal;
}

int bos_DoGetLog(aSyntax, aRock)
     struct cmd_syndesc * volatile	aSyntax;
     char *			aRock;
{
  volatile int		rtnVal = 0;
  volatile long	bossvr_error = 0;
  volatile error_status_t	rpc_error = error_status_ok;
  char *		errorStringP = "";
  pipe_t * volatile	thePipeP;
  static char		routineName[] = "bos_DoGetLog";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));


  thePipeP = (pipe_t *)osi_Alloc(sizeof(pipe_t));
  if (thePipeP != (pipe_t *)NULL) {
    thePipeP->alloc = FilePipeAlloc;
    thePipeP->push = FilePipePush;
    thePipeP->state = stdout;

    dce_svc_printf(BOS_S_FETCHING_LOG_FILE_MSG,
		 aSyntax->parms[GETLOG_FILE_INDEX].items->data);

    TRY
      bossvr_error = BOSSVR_GetLog(theBosserverBinding,
	(unsigned char *)aSyntax->parms[GETLOG_FILE_INDEX].items->data,
	(pipe_t *)thePipeP, (error_status_t *)&rpc_error);
    CATCH (rpc_x_ss_pipe_closed)
      dce_svc_printf(BOS_S_UNEX_PIPE_EX1_MSG, programName,
		 aSyntax->parms[GETLOG_FILE_INDEX].items->data,
                 "rpc_x_ss_pipe_closed");
      rtnVal = 1;
    CATCH (rpc_x_ss_pipe_comm_error)
      dce_svc_printf(BOS_S_UNEX_PIPE_EX2_MSG, programName,
		 aSyntax->parms[GETLOG_FILE_INDEX].items->data,
                 "rpc_x_ss_pipe_comm_error");
      rtnVal = 1;
    CATCH (rpc_x_ss_pipe_discipline_error)
      dce_svc_printf(BOS_S_UNEX_PIPE_EX3_MSG, programName,
		 aSyntax->parms[GETLOG_FILE_INDEX].items->data,
                 "rpc_x_ss_pipe_discipline_error");
      rtnVal = 1;
    CATCH (rpc_x_ss_pipe_empty)
      dce_svc_printf(BOS_S_UNEX_PIPE_EX4_MSG, programName,
		 aSyntax->parms[GETLOG_FILE_INDEX].items->data,
                 "rpc_x_ss_pipe_empty");
      rtnVal = 1;
    CATCH (rpc_x_ss_pipe_memory)
      dce_svc_printf(BOS_S_UNEX_PIPE_EX5_MSG, programName,
		 aSyntax->parms[GETLOG_FILE_INDEX].items->data,
                 "rpc_x_ss_pipe_memory");
      rtnVal = 1;
    CATCH (rpc_x_ss_pipe_order)
      dce_svc_printf(BOS_S_UNEX_PIPE_EX6_MSG, programName,
		 aSyntax->parms[GETLOG_FILE_INDEX].items->data,
                 "rpc_x_ss_pipe_order");
      rtnVal = 1;
    CATCH_ALL
      dce_svc_printf(BOS_S_UNEX_EXCP_MSG, programName,
		 aSyntax->parms[GETLOG_FILE_INDEX].items->data);
      rtnVal = 1;
    ENDTRY;

    if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
      errorStringP = ((rpc_error != error_status_ok) ?
		      dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
      dce_svc_printf(BOS_S_GET_LOG_FILE_FAIL_MSG, programName,
		 aSyntax->parms[GETLOG_FILE_INDEX].items->data, errorStringP);
      rtnVal = 1;
    }
  }
  else {
    dce_svc_printf(BOS_S_MEM_ALLOC_ERR_MSG, programName);
    rtnVal = 1;
  }

  return rtnVal;
}

int bos_DoGetRestart(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int				rtnVal = 0;
  long				bossvr_error = 0;
  error_status_t		rpc_error = error_status_ok;
  char *			errorStringP = "";
  char				hostNameBuffer[BUFSIZ];
  char *			hostName;
  struct bossvr_netKTime	generalTime;
  struct bossvr_netKTime	newBinaryTime;
  long				ktimeCode;
  char				messageBuffer[BUFSIZ];
  static char			routineName[] = "bos_DoGetRestart";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));

  if (cmd_IsParamPresent(aSyntax, BOS_SERVER_INDEX)) {
    hostName = aSyntax->parms[BOS_SERVER_INDEX].items->data;
  }
  else {
    hostNameBuffer[0] = '\0';
    if (gethostname(hostNameBuffer, BUFSIZ) == -1) {
      dce_fprintf(stderr, bos_s_bad_gethostname, strerror(errno));
    }
    hostName = hostNameBuffer;
  }


  bossvr_error = BOSSVR_GetRestartTime(theBosserverBinding, 1,
				   &generalTime, &rpc_error);
  if ((rpc_error == error_status_ok) && (bossvr_error == 0)) {
    bossvr_error = BOSSVR_GetRestartTime(theBosserverBinding, 2,
				     &newBinaryTime, &rpc_error);
    if ((rpc_error == error_status_ok) && (bossvr_error == 0)) {
      ktimeCode = ktime_DisplayString((struct ktime *) (&generalTime),
				      messageBuffer);
      if (ktimeCode == 0) {
	dce_svc_printf(BOS_S_SERVER_RESTARTS_MSG, hostName, messageBuffer);
	ktimeCode = ktime_DisplayString((struct ktime *)(&newBinaryTime),
					messageBuffer);
	if (ktimeCode == 0) {
	  dce_svc_printf(BOS_S_SERVER_RESTARTS_NEWBIN_MSG, hostName, messageBuffer);
	}
      }
    }
  }

  /* check for error conditions and print messages, as appropriate */
  if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
    errorStringP = ((rpc_error != error_status_ok) ?
		    dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
    dce_svc_printf(BOS_S_NO_RESTART_INFO_MSG, programName, errorStringP);
    rtnVal = ((bossvr_error == 0) ? 1 : bossvr_error);	/* take a non-zero guess */
  }
  else {
    if (ktimeCode != 0) {
      dce_svc_printf(BOS_S_NO_RESTART_TIME_MSG, programName, bosutil_em(ktimeCode));
      rtnVal = ktimeCode;
    }
  }

  return rtnVal;
}

int bos_DoInstall(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  long			bossvr_error = 0;
  error_status_t	rpc_error = error_status_ok;
  char *		errorStringP = "";
  pipe_t *		thePipeP;
  char			destDir[BUFSIZ];
  char			fullPathname[BUFSIZ];
  struct cmd_item *	thisItemP;
  struct stat		statBuf;
  int			statCode;
  static char		routineName[] = "bos_DoInstall";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));

  thePipeP = (pipe_t *)osi_Alloc(sizeof(pipe_t));
  thePipeP->alloc = FilePipeAlloc;
  thePipeP->pull = FilePipePull;

  if (thePipeP != (pipe_t *)NULL) {
    if (cmd_IsParamPresent(aSyntax, INSTALL_DESTDIR_INDEX)) {
      (void)strcpy(destDir, aSyntax->parms[INSTALL_DESTDIR_INDEX].items->data);
    }
    else {
      (void)strcpy(destDir, DEFAULT_DEST_DIR);
    }

    for (thisItemP = aSyntax->parms[INSTALL_FILE_INDEX].items;
	 (thisItemP != (struct cmd_item *)NULL) && (rtnVal == 0);
	 thisItemP = thisItemP->next) {

      thePipeP->state = fopen(thisItemP->data, "r");
      if (thePipeP->state != (char *)NULL) {
	statCode = stat(thisItemP->data, &statBuf);
	if (statCode == 0) {
	  bosutil_ComputeDestDir(thisItemP->data, destDir,
				 fullPathname, sizeof(fullPathname));
	  bossvr_error = BOSSVR_Install(theBosserverBinding, (unsigned char *)fullPathname, statBuf.st_size,
				    /* flags = */ 0, statBuf.st_mtime,
				    thePipeP, &rpc_error);

	  if ((rpc_error == error_status_ok) && (bossvr_error == 0)) {
	    dce_svc_printf(BOS_S_FILE_INSTALLED_MSG, programName, thisItemP->data);
	  }
	  else {
	    errorStringP = ((rpc_error != error_status_ok) ?
			    dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
	    dce_svc_printf(BOS_S_FILE_NOT_INSTALLED_MSG, programName,
			    thisItemP->data, errorStringP);
	    rtnVal = 1;
	  }
	}
	else {
	  dce_svc_printf(BOS_S_NO_STAT_FILE_MSG, programName, thisItemP->data);
	  rtnVal = 1;
	}

	fclose(thePipeP->state);
      }
      else {
	dce_svc_printf(BOS_S_FILE_NOT_OPENED_MSG, programName, thisItemP->data);
	rtnVal = 1;
      }

    }	/* end for (looping through all file names) */
  }	/* end if we were able to allocate a pipe structure */
  else {
    rtnVal = 1;
  }

  return rtnVal;
}

int bos_DoKill(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  long			bossvr_error = 0;
  error_status_t	rpc_error = error_status_ok;
  char *		errorStringP = "";
  struct cmd_item *	thisItemP;
  static char		routineName[] = "bos_DoKill";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));

/*
  bossvr_error = BOSSVR_ReBossvr(theBosserverBinding, 0, &rpc_error);
 */
  bossvr_error = BOSSVR_ShutdownAll(theBosserverBinding, &rpc_error);
  if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
    errorStringP = ((rpc_error != error_status_ok) ?
		    dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
    dce_svc_printf(BOS_S_BOSSRVR_KILL_FAILED_MSG, programName, errorStringP);
    rtnVal = 1;
  }
}

int bos_DoLsCell(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  long			bossvr_error = 0;
  error_status_t	rpc_error = error_status_ok;
  char *		errorStringP = "";
  bossvr_out_string	cellnameBuffer;
  bossvr_out_string	hostnameBuffer;
  int			i = 0;
  static char		routineName[] = "bos_DoLsCell";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));

  cellnameBuffer.strLen = 0;
  cellnameBuffer.theString = (unsigned char *)NULL;
  bossvr_error = BOSSVR_GetCellName(theBosserverBinding, &cellnameBuffer, &rpc_error);

  if ((rpc_error == error_status_ok) && (bossvr_error == 0)) {
    dce_svc_printf(BOS_S_CELL_NAME_MSG, cellnameBuffer.theString);
  }
  else {
    errorStringP = ((rpc_error != error_status_ok) ?
		    dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
    dce_svc_printf(BOS_S_NO_CELL_NAME_MSG, programName, errorStringP);
    rtnVal = 1;
  }

  return rtnVal;
}

int bos_DoLsKeys(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  register long		code;
  register int		i = 0;
  struct bossvr_key	tkey;
  struct bossvr_keyInfo	keyInfo;
  long			kvno;
  long			bossvr_error = 0;
  error_status_t	rpc_error = error_status_ok;
  char *		errorStringP = "";
  char			principalName[BUFSIZ];
  struct bossvr_key	zeroKey;
  struct bossvr_keyInfo	zeroKeyInfo;
  u_int32		deskey[2];
  long int              savedModifyDate = 0;
  static char		routineName[] = "bos_DoLsKeys";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));

  if (cmd_IsParamPresent(aSyntax, LSKEYS_PRINCIPAL_INDEX)) {
    (void)strncpy(principalName, cmd_FirstData(aSyntax, LSKEYS_PRINCIPAL_INDEX),
		  sizeof(principalName) - 1);
  }
  else {
    principalName[0] = '\0';
  }

  bzero((char *)&zeroKey, sizeof(struct bossvr_key));
  bzero((char *)&zeroKeyInfo, sizeof(struct bossvr_keyInfo));

  bossvr_error = dfsauth_client_SetFullEncryption(&theBosserverBinding);
  if ((bossvr_error == 0) ||
      (bossvr_error == DAUT_ERROR_PROT_LEVEL_UNSUPPORTED) ||
      ((bossvr_error == DAUT_ERROR_CLIENT_SERVICE_NOT_SUPP_NOAUTH) &&
       (cmd_IsParamPresent(aSyntax, BOS_NOAUTH_INDEX)))) {
    if (bossvr_error == DAUT_ERROR_PROT_LEVEL_UNSUPPORTED)
	dce_svc_printf(BOS_S_NO_DATA_ENCRYPTN_MSG);

    bossvr_error = 0;
    while ((rpc_error == error_status_ok) && (bossvr_error == 0)) {
      bossvr_error = BOSSVR_ListKeys(theBosserverBinding,
				     (unsigned char *)principalName,
				     i, &kvno, &tkey, &keyInfo,
				     &rpc_error);

      if (rpc_error == rpc_s_unsupported_protect_level
	|| rpc_error == rpc_s_unknown_reject) {
	dce_svc_printf(BOS_S_NO_DES_MSG);

	bossvr_error = dfsauth_client_ResetDefaultEncryption(&theBosserverBinding);
	if (bossvr_error == 0) {
	  bossvr_error = BOSSVR_ListKeys(theBosserverBinding,
					 (unsigned char *)principalName,
					 i, &kvno, &tkey, &keyInfo, &rpc_error);
	}
      }

      if ((rpc_error == error_status_ok) && (bossvr_error == 0)) {
        savedModifyDate = keyInfo.mod_sec;
        /* first check if key is returned */
	if (bcmp ((char *)&tkey, (char *)&zeroKey, sizeof(tkey)) == 0) {
	  if (bcmp ((char *)&keyInfo, (char *)&zeroKeyInfo, sizeof(struct bossvr_keyInfo))
	      == 0) /* shouldn't happen */
	    dce_svc_printf(BOS_S_KEY_VERSION_MSG, kvno);
	  else
	    dce_svc_printf(BOS_S_KEY_W_CKSUM_MSG, kvno, keyInfo.keyCheckSum);
	}
	else {
	  dce_printf(bos_s_key_is, kvno);
	  /* this is a DES key coming back; such a key will print as 2 longs */
	  bcopy((char *)(tkey.data), (char *)deskey, sizeof(deskey));
	  dce_printf(bos_s_key_is_2, deskey[0], deskey[1]);
	  dce_printf(bos_s_key_is_3);
	}
	i++;
      }
    }
  }

  if (savedModifyDate) {
    /* this feature may not currently work */
      dce_svc_printf(BOS_S_KEYS_LAST_CHANGED_MSG, bosutil_DateOf(savedModifyDate));
  }
  if ((rpc_error == error_status_ok) && (bossvr_error == BZDOM)) {
    dce_svc_printf(BOS_S_ALL_DONE_MSG);
  }
  else {
    errorStringP = ((rpc_error != error_status_ok) ?
		    dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
    dce_svc_printf(BOS_S_ERROR_LISTING_KEYS_MSG, programName, errorStringP);
    rtnVal = 1;
  }

  return rtnVal;
}

#define MAX_CHARS_PER_LINE	79
int bos_DoLsUsers(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  long			bossvr_error = 0;
  error_status_t	rpc_error = error_status_ok;
  char *		errorStringP = "";
  int			printHeading = 1;	/* no heading printed if no calls succ */
  int			charsSoFarOnLine = 0;
  char *		filenameP;
  bossvr_out_string	usernameBuffer;
  bossvr_out_string	typeBuffer;
  bossvr_out_string	permsBuffer;
  char			entryString[BUFSIZ];
  static char		headingString[] = "Admin Users are: ";
  int			i = 0;
  char			separator[2] = "";
  static char		routineName[] = "bos_DoLsUsers";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));

  filenameP = cmd_FirstData(aSyntax, LSUSERS_FILENAME_INDEX);

  rpc_error = error_status_ok;
  while ((rpc_error == error_status_ok) && (bossvr_error == 0)) {
    bossvr_error = BOSSVR_ListSUsers(theBosserverBinding,
				 (unsigned char *)filenameP,
				 i,
				 &usernameBuffer,
				 &typeBuffer,
				 &permsBuffer,
				 &rpc_error);

    if ((rpc_error == error_status_ok) && (bossvr_error == 0)) {
      (void)sprintf(entryString, "%s: %s",
		    typeBuffer.theString,
		    usernameBuffer.theString);

      if (printHeading) {
        error_status_t  st;
        unsigned char   *cmd_explanation;
        cmd_explanation = dce_msg_get_msg(bos_s_admin_users_heading1, &st);
	fprintf(stdout, cmd_explanation);
	charsSoFarOnLine = strlen(cmd_explanation);
        free(cmd_explanation);
	/* can't reset printHeading here, because we need it a little later */
      }

      if ((charsSoFarOnLine + strlen(entryString) + 1) > (unsigned)MAX_CHARS_PER_LINE) {
	/* allow for potential comma after this entry */
	dce_fprintf(stdout, bos_s_admin_users_heading2a, separator);
	charsSoFarOnLine = 0;
      }
      else {
	dce_fprintf(stdout, bos_s_admin_users_heading2b, separator);
	charsSoFarOnLine += strlen(separator);
      }

      dce_fprintf(stdout, bos_s_admin_users_heading3, entryString);
      charsSoFarOnLine += strlen(entryString);

      if (printHeading) {
	{
        char *dcesptr;
	dcesptr = dce_sprintf(bos_s_admin_users_heading4);
	strcpy(separator , dcesptr);
	free(dcesptr);
	}
	printHeading = 0;
      }
    }

    i++;
  }

  if (bossvr_error == BZNOENT) {
    dce_fprintf(stdout, bos_s_admin_users_heading5a);
  }
  else {	/* we broke for a real error */
    errorStringP = ((rpc_error != error_status_ok) ?
		    dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
    dce_fprintf(stderr, bos_s_admin_users_heading5b, programName, errorStringP);
    rtnVal = 1;
  }

  return rtnVal;
}

int bos_DoPrune(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  long			bossvr_error = 0;
  error_status_t	rpc_error = error_status_ok;
  char *		errorStringP = "";
  unsigned32		flags = 0;
  static char		routineName[] = "bos_DoPrune";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));

  if (cmd_IsParamPresent(aSyntax, PRUNE_BAK_INDEX)) flags |= BOSSVR_PRUNEBAK;
  if (cmd_IsParamPresent(aSyntax, PRUNE_OLD_INDEX)) flags |= BOSSVR_PRUNEOLD;
  if (cmd_IsParamPresent(aSyntax, PRUNE_CORE_INDEX)) flags |= BOSSVR_PRUNECORE;
  if (cmd_IsParamPresent(aSyntax, PRUNE_ALL_INDEX)) flags |= 0xff;

  bossvr_error = BOSSVR_Prune(theBosserverBinding, flags, &rpc_error);

  if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
    errorStringP = ((rpc_error != error_status_ok) ?
		    dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
    dce_svc_printf(BOS_S_PRUNE_FAILURE_MSG, programName, errorStringP);
    rtnVal = 1;
  }

  return rtnVal;
}

int bos_DoRmKey(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  long			bossvr_error = 0;
  char			principalName [BUFSIZ];
  error_status_t	rpc_error = error_status_ok;
  char *		errorStringP = "";
  long			theKey;
  struct cmd_item *	thisItemP;
  static char		routineName[] = "bos_DoRmKey";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));

  if (cmd_IsParamPresent(aSyntax, RMKEY_PRINCIPAL_INDEX)) {
    (void)strncpy(principalName, cmd_FirstData(aSyntax, RMKEY_PRINCIPAL_INDEX),
		  sizeof(principalName) - 1);
  }
  else {
    principalName[0] = '\0';
  }

  bossvr_error = dfsauth_client_SetFullEncryption(&theBosserverBinding);
  if ((bossvr_error == 0) ||
      (bossvr_error == DAUT_ERROR_PROT_LEVEL_UNSUPPORTED) ||
      ((bossvr_error == DAUT_ERROR_CLIENT_SERVICE_NOT_SUPP_NOAUTH) &&
       (cmd_IsParamPresent(aSyntax, BOS_NOAUTH_INDEX)))) {
    if (bossvr_error == DAUT_ERROR_PROT_LEVEL_UNSUPPORTED)
	dce_svc_printf(BOS_S_DATA_ENCRYPT_NOT_SUP_MSG);

    for (thisItemP = aSyntax->parms[RMKEY_KVNO_INDEX].items;
	 (thisItemP != (struct cmd_item *)NULL) && (rtnVal == 0);
	 thisItemP = thisItemP->next) {
      theKey = atol(thisItemP->data);
      bossvr_error = BOSSVR_DeleteKey(theBosserverBinding,
				      (unsigned char *)principalName,
				      theKey, &rpc_error);

      if (rpc_error == rpc_s_unsupported_protect_level
	|| rpc_error == rpc_s_unknown_reject) {
	dce_svc_printf(BOS_S_NO_DES_MSG);

	bossvr_error = dfsauth_client_ResetDefaultEncryption(&theBosserverBinding);
	if (bossvr_error == 0) {
	  bossvr_error = BOSSVR_DeleteKey(theBosserverBinding,
					 (unsigned char *)principalName,
					 theKey, &rpc_error);
	}
      }

      if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
	errorStringP = ((rpc_error != error_status_ok) ?
			dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
	dce_svc_printf(BOS_S_KEY_DELETE_FAILURE_MSG, programName, theKey, errorStringP);
	rtnVal = 1;
      }
    }
  }
  else {
    errorStringP = bosutil_em(bossvr_error);
    dce_svc_printf(BOS_S_KEY_DELETE_FAILURE2_MSG, programName, theKey, errorStringP);
    rtnVal = 1;
  }

  return rtnVal;
}

int bos_DoRmUser(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  long			bossvr_error = 0;
  error_status_t	rpc_error = error_status_ok;
  char *		filenameP;
  char *		errorStringP = "";
  ulong			flags = 0;
  struct cmd_item *	thisItemP;
  int			done = 0;
  bossvr_out_string             cellnameBuffer;
  unsigned char                 *typeStr;
  static char		routineName[] = "bos_DoRmUser";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));

  cellnameBuffer.strLen = 0;
  cellnameBuffer.theString = (unsigned char *)NULL;
  bossvr_error = BOSSVR_GetCellName(theBosserverBinding, &cellnameBuffer, &rpc_error);
  if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
    errorStringP = ((rpc_error != error_status_ok) ?
                    dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
    dce_fprintf(stderr, bos_s_cell_name, programName, errorStringP);
    return 1;
  }

  filenameP = cmd_FirstData(aSyntax, RMUSER_FILENAME_INDEX);
  if (cmd_IsParamPresent(aSyntax, RMUSER_REMOVEFLAG_INDEX)) {
    flags |= BOSSVR_RMADMIN_REMOVELIST;
  }

  if (cmd_IsParamPresent(aSyntax, RMUSER_CLEARLIST_INDEX)) {
    flags |= BOSSVR_RMADMIN_CLEARLIST;
  }

  if ((cmd_IsParamPresent(aSyntax, RMUSER_USER_INDEX)) ||
      (cmd_IsParamPresent(aSyntax, RMUSER_GROUP_INDEX))) {
    /* first remove the users given */
    for (thisItemP = aSyntax->parms[RMUSER_USER_INDEX].items;
	 (thisItemP != (struct cmd_item *)NULL) && (done == 0);
	 thisItemP = thisItemP->next) {

         if ((strncmp(thisItemP->data,"/",1) != 0) || (strncmp((char *)cellnameBuffer.theString,thisItemP->data,cellnameBuffer.strLen) == 0)) 
            typeStr=(unsigned char *)"user";  /* user must be in the local cell */
         else  
            typeStr=(unsigned char *)"foreign_user"; /* user is in foreign cell */ 
      bossvr_error = BOSSVR_DeleteSUser(theBosserverBinding,
					(unsigned char *)filenameP,
					typeStr, 
					(unsigned char *)thisItemP->data,
					flags,
					&rpc_error);

      if ((rpc_error == error_status_ok) && (bossvr_error == 0)) {
	/* everything that could be done, will have been done if clearlist was set */
	if ((flags & BOSSVR_RMADMIN_CLEARLIST) != 0) {
	  done = 1;
	}
      }
      else {
	errorStringP = ((rpc_error != error_status_ok) ?
			dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
	dce_svc_printf(BOS_S_USER_NOT_DELETED_MSG, programName, thisItemP->data);
	if (bossvr_error == BZNOENT) {
	  dce_svc_printf(BOS_S_NO_SUCH_USER_MSG);
	}
	else {
	  dce_svc_printf(BOS_S_NAME_OF_NOT_DELETED_MSG, errorStringP);
	}

	rtnVal = 1;
      }
    }

    /* now, remove the groups given */
    for (thisItemP = aSyntax->parms[RMUSER_GROUP_INDEX].items;
	 (thisItemP != (struct cmd_item *)NULL) && (done == 0);
	 thisItemP = thisItemP->next) {
         if ((strncmp(thisItemP->data,"/",1) != 0) || (strncmp((char *)cellnameBuffer.theString,thisItemP->data,cellnameBuffer.strLen) == 0)) 
            typeStr=(unsigned char *)"group"; /* group must be in local cell */
         else  
            typeStr=(unsigned char *)"foreign_group"; /* group is in foreign cell */ 
      bossvr_error = BOSSVR_DeleteSUser(theBosserverBinding,
					(unsigned char *)filenameP,
					typeStr, 
					(unsigned char *)thisItemP->data,
					flags,
					&rpc_error);

      if ((rpc_error == error_status_ok) && (bossvr_error == 0)) {
	/* everything that could be done, will have been done if clearlist was set */
	if ((flags & BOSSVR_RMADMIN_CLEARLIST) != 0) {
	  done = 1;
	}
      }
      else {
	errorStringP = ((rpc_error != error_status_ok) ?
			dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
	dce_svc_printf(BOS_S_GROUP_DELETE_FAILED_MSG, programName, thisItemP->data);
	if (bossvr_error == BZNOENT) {
	  dce_svc_printf(BOS_S_NO_SUCH_GROUP_MSG);
	}
	else {
	  dce_svc_printf(BOS_S_NAME_OF_NOT_DEL_GROUP_MSG, errorStringP);
	}

	rtnVal = 1;
      }
    }
  }
  else {
    if (((flags & BOSSVR_RMADMIN_REMOVELIST) != 0) ||
	((flags & BOSSVR_RMADMIN_CLEARLIST) != 0)) {
      bossvr_error = BOSSVR_DeleteSUser(theBosserverBinding,
					(unsigned char *)filenameP,
					BBOS_NO_TYPE,
					BBOS_NO_NAME,
					flags,
					&rpc_error);
      if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
	errorStringP = ((rpc_error != error_status_ok) ?
			dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
	dce_svc_printf(BOS_S_REMOVE_FILE_FAILED_MSG, programName, filenameP, errorStringP);
	rtnVal = 1;
      }
    }
    else {
      dce_svc_printf(BOS_S_NOTHING_SPECD_ERROR_MSG, programName);
      rtnVal = 1;
    }
  }

  return rtnVal;
}


int bos_DoRestart(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  long			bossvr_error = 0;
  error_status_t	rpc_error = error_status_ok;
  char *		errorStringP = "";
  struct cmd_item *	thisItemP;
  static char		routineName[] = "bos_DoRestart";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));

  if (cmd_IsParamPresent(aSyntax, RESTART_BOS_INDEX)) {
    /* the user wants to restart the entire bosserver in this case */
    bossvr_error = BOSSVR_ReBossvr(theBosserverBinding, &rpc_error);

    if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
      errorStringP = ((rpc_error != error_status_ok) ?
		      dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
      dce_svc_printf(BOS_S_RESTART_BOSSRVR_FAILURE_MSG, programName, errorStringP);
      rtnVal = 1;
    }
  }
  else {
    if (cmd_IsParamPresent(aSyntax, RESTART_PROCESS_INDEX)) {
      /* restart only the named processes */
      for (thisItemP = aSyntax->parms[RESTART_PROCESS_INDEX].items;
	   thisItemP != (struct cmd_item *)NULL;
	   thisItemP = thisItemP->next) {
	bossvr_error = BOSSVR_Restart(theBosserverBinding, (unsigned char *)thisItemP->data, &rpc_error);

	if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
	  errorStringP = ((rpc_error != error_status_ok) ?
			  dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
	  dce_svc_printf(BOS_S_INSTANCE_RESTART_FAILURE_MSG, programName,
			  thisItemP->data, errorStringP);
	  rtnVal = 1;
	}
      }	/* end for */
    }
    else {
      /* restart everybody */
      bossvr_error = BOSSVR_RestartAll(theBosserverBinding, &rpc_error);

      if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
	errorStringP = ((rpc_error != error_status_ok) ?
			dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
	dce_svc_printf(BOS_S_SERVERS_RESTART_FAILURE_MSG, programName, errorStringP);
	rtnVal = 1;
      }
    }

  }

  return rtnVal;
}

int bos_DoSetAuth(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  long			bossvr_error = 0;
  error_status_t	rpc_error = error_status_ok;
  char *		errorStringP = "";
  long			flag;
  char *		authString;
  static char		routineName[] = "bos_DoSetAuth";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));

  authString = aSyntax->parms[SETAUTH_AUTHSTRING_INDEX].items->data;
  if (strcmp(authString, "on") == 0) {
    flag = 0;	/* auth req.: noauthflag is false */
  }
  else if (strcmp(authString, "off") == 0) {
    flag = 1;
  }
  else {
    dce_svc_printf(BOS_S_ILLEGAL_AUTH_SPEC_MSG, programName, authString);
    rtnVal = 1;
  }

  if (rtnVal == 0) {
    bossvr_error = BOSSVR_SetNoAuthFlag(theBosserverBinding, flag, &rpc_error);

    if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
      errorStringP = ((rpc_error != error_status_ok) ?
		      dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
      dce_svc_printf(BOS_S_AUTH_FLAG_NOT_SET_MSG, programName, errorStringP);
      rtnVal = 1;
    }
  }

  return rtnVal;
}

#if defined(DFS_BOS_DEBUG_BOSSERVER)
PRIVATE int bos_ParseDebugFlag(flagString, flagValP)
     char *	flagString;
     long *	flagValP;
{
  int		rtnVal = 1;	/* assume success */
  char		secondCharacter = *(flagString + 1);

  if (*flagString == '0') {
    if ((secondCharacter == 'x') || (secondCharacter == 'X')) {
      /* we have a hex number */
      rtnVal = sscanf(flagString, "%lx", flagValP);
    }
    else {
      /* we have an octal number */
      rtnVal = sscanf(flagString, "%lo", flagValP);
    }
  }
  else {
    /* we have a decimal value */
    *flagValP = atol(flagString);
  }

  return (rtnVal);
}

int bos_DoSetDebug(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  long			bossvr_error = 0;
  error_status_t	rpc_error = error_status_ok;
  char *		errorStringP = "";
  long			newDebugVal;
  long *		newDebugPtrP = (long *)NULL;
  int			conversionFlag = 1;	/* assume success */
  long			oldDebugVal;
  static char		routineName[] = "bos_DoSetDebug";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));

  if (cmd_IsParamPresent(aSyntax, SETDEBUG_FLAG_INDEX)) {
    conversionFlag = bos_ParseDebugFlag(aSyntax->parms[SETDEBUG_FLAG_INDEX].items->data,
					&newDebugVal);
    newDebugPtrP = &newDebugVal;
  }

  if (conversionFlag == 1) {
    bossvr_error = BOSSVR_SetDebug(theBosserverBinding, newDebugPtrP, &oldDebugVal,
			       &rpc_error);

    if ((rpc_error == error_status_ok) && (bossvr_error == 0)) {
      dce_svc_printf(BOS_S_BOSSRVR_DBGFLG_VALUE_MSG, programName, (newDebugPtrP == (long *)NULL) ? "current" : "previous",
	      oldDebugVal);
    }
    else {
      errorStringP = ((rpc_error != error_status_ok) ?
		      dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
      dce_svc_printf(BOS_S_BOSSRVR_SET_DBGFLG_FAIL_MSG, programName, errorStringP);
      rtnVal = 1;
    }
  }
  else {
    dce_svc_printf(BOS_S_FLAG_VALUE_BAD_MSG, programName,
		      aSyntax->parms[SETDEBUG_FLAG_INDEX].items->data);
  }

  return rtnVal;
}
#endif /* defined(DFS_BOS_DEBUG_BOSSERVER) */

int bos_DoSetRestart(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  long			bossvr_error = 0;
  error_status_t	rpc_error = error_status_ok;
  char *		errorStringP = "";
  long			type;
  struct bossvr_netKTime		restartTime;
  long			ktimeCode;
  char *		timeToParse;
  static char		routineName[] = "bos_DoSetRestart";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));

  if (cmd_IsParamPresent(aSyntax, SETRESTART_ALL_INDEX)) {
    type = 1;
    timeToParse = aSyntax->parms[SETRESTART_ALL_INDEX].items->data;
  }

  if (cmd_IsParamPresent(aSyntax, SETRESTART_BINARY_INDEX)) {
    type = 2;
    timeToParse = aSyntax->parms[SETRESTART_BINARY_INDEX].items->data;
  }

  ktimeCode = ktime_ParsePeriodic(timeToParse, (struct ktime *)(&restartTime));
  if (ktimeCode == 0) {
    bossvr_error = BOSSVR_SetRestartTime(theBosserverBinding, type,
				     &restartTime, &rpc_error);

    if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
      errorStringP = ((rpc_error != error_status_ok) ?
		      dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
      dce_svc_printf(BOS_S_BOSSVR_ERROR_MSG, programName, errorStringP);
      rtnVal = 1;
    }
  }
  else {
/* the ktime code doesn't really give an informatice error code, anyway
    dce_svc_printf(BOS_S_FAILED_TO_PARSE_TIME_MSG, programName, timeToParse,
		      bosutil_em(ktimeCode));
 */
    dce_svc_printf(BOS_S_FAILED_TO_PARSE_TIME1_MSG, programName, timeToParse);
    rtnVal = ktimeCode;
  }

  return rtnVal;
}

int bos_DoShutdown(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  long			bossvr_error = 0;
  error_status_t	rpc_error = error_status_ok;
  char *		errorStringP = "";
  struct cmd_item *	thisItemP;
  static char		routineName[] = "bos_DoShutdown";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));

  if (!cmd_IsParamPresent(aSyntax, SHUTDOWN_PROCESS_INDEX)) {
    bossvr_error = BOSSVR_ShutdownAll(theBosserverBinding, &rpc_error);

    if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
      errorStringP = ((rpc_error != error_status_ok) ?
		      dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
      dce_svc_printf(BOS_S_SHUTD_SERVERS_FAILED_MSG, programName, errorStringP);
      rtnVal = 1;
    }
  }
  else {
    for (thisItemP = aSyntax->parms[SHUTDOWN_PROCESS_INDEX].items;
	(rpc_error == error_status_ok) && (thisItemP != (struct cmd_item *)NULL);
	 thisItemP = thisItemP->next) {
      bossvr_error = BOSSVR_SetTStatus(theBosserverBinding,
				   (unsigned char *)thisItemP->data, BSTAT_SHUTDOWN, &rpc_error);
      if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
	errorStringP = ((rpc_error != error_status_ok) ?
			dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
	dce_svc_printf(BOS_S_SHUTD_INSTANCE_FAILED_MSG, programName,
			 thisItemP->data, errorStringP);
	rtnVal = 1;
      }
    }	/* end for */
  }

  if ((rpc_error == error_status_ok) &&
      (cmd_IsParamPresent(aSyntax, SHUTDOWN_WAIT_INDEX))) {
    bossvr_error = BOSSVR_WaitAll(theBosserverBinding, &rpc_error);

    if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
      errorStringP = ((rpc_error != error_status_ok) ?
		      dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
      dce_svc_printf(BOS_S_SHUTD_WITH_PROCESSORS_ACTIVE_MSG, programName, errorStringP);
      rtnVal = 1;
    }
  }

  return rtnVal;
}

int bos_DoStart(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  long			bossvr_error = 0;
  error_status_t	rpc_error = error_status_ok;
  char *		errorStringP = "";
  struct cmd_item *	thisItemP;
  static char		routineName[] = "bos_DoStart";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));

  for (thisItemP = aSyntax->parms[START_PROCESS_INDEX].items;
       thisItemP != (struct cmd_item *)NULL;
       thisItemP = thisItemP->next) {
    bossvr_error = BOSSVR_SetStatus(theBosserverBinding,
				(unsigned char *)thisItemP->data, BSTAT_NORMAL, &rpc_error);

    if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
      errorStringP = ((rpc_error != error_status_ok) ?
		      dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
      dce_svc_printf(BOS_S_INSTANCE_START_FAILURE_MSG, programName,
		      thisItemP->data, errorStringP);
      rtnVal = 1;
    }
  }

  return rtnVal;
}

int bos_DoStartup(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  long			bossvr_error = 0;
  error_status_t	rpc_error = error_status_ok;
  char *		errorStringP = "";
  struct cmd_item *	thisItemP;
  static char		routineName[] = "bos_DoStartup";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));

  thisItemP = aSyntax->parms[STARTUP_PROCESS_INDEX].items;
  if (thisItemP == (struct cmd_item *)NULL) {
    bossvr_error = BOSSVR_StartupAll(theBosserverBinding, &rpc_error);

    if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
      errorStringP = ((rpc_error != error_status_ok) ?
		      dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
      dce_svc_printf(BOS_S_STARTUP_SERVERS_FAIL_MSG, programName, errorStringP);
      rtnVal = 1;
    }
  }
  else {
    for (/* init'd above */;
	 thisItemP != (struct cmd_item *)NULL;
	 thisItemP = thisItemP->next) {
      bossvr_error = BOSSVR_SetTStatus(theBosserverBinding,
				   (unsigned char *)thisItemP->data, BSTAT_NORMAL, &rpc_error);

      if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
	errorStringP = ((rpc_error != error_status_ok) ?
			dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
	dce_svc_printf(BOS_S_INSTANCE_START_BAD_MSG, programName, thisItemP->data,
			errorStringP);
	rtnVal = 1;
      }
    }
  }

  return rtnVal;
}

PRIVATE void bos_PrintAllStatusInfo(name, type, statusP, processStatus, auxStatus,
				    paramBuffers, longDisplay)
     char *			name;
     char *			type;
     struct bossvr_status *	statusP;
     long			processStatus;
     char *			auxStatus;
     bossvr_out_string		paramBuffers[MAX_PARAMS];
     int			longDisplay;
{
  int i;

  dce_fprintf(stdout, bos_s_all_bos_status_info1, name);
  if (longDisplay) {
    dce_fprintf(stdout, bos_s_bos_status_info2, type);
  }

  if (statusP->fileGoal == statusP->goal) {
    if (!statusP->goal) {
      dce_fprintf(stdout, bos_s_bos_status_info3);
    }
  }
  else {
    if (statusP->fileGoal) {
      dce_fprintf(stdout, bos_s_bos_status_info4a);
    }
    else {
      dce_fprintf(stdout, bos_s_bos_status_info4b);
    }
  }

  if (statusP->flags & BOSSVR_ERRORSTOP) {
    dce_fprintf(stdout, bos_s_bos_status_info5);
  }

  if (statusP->flags & BOSSVR_HASCORE) {
    dce_fprintf(stdout, bos_s_bos_status_info6);
  }

  /* print out the status information */
  dce_fprintf(stdout, bos_s_bos_status_info7, name);
  if (processStatus == BSTAT_NORMAL) dce_fprintf(stdout, bos_s_bos_status_info8a);
  else if (processStatus == BSTAT_SHUTDOWN) dce_fprintf(stdout, bos_s_bos_status_info8b);
  else if (processStatus == BSTAT_STARTINGUP) dce_fprintf(stdout, bos_s_bos_status_info8c);
  else if (processStatus == BSTAT_SHUTTINGDOWN) dce_fprintf(stdout, bos_s_bos_status_info8d);
  if (auxStatus[0] != '\0') {
    dce_svc_printf(BOS_S_AUX_STATUS_INFO_MSG, auxStatus);
  }

  if (longDisplay) {
    /* more instance information */
    if (statusP->procStartTime) {
      dce_svc_printf(BOS_S_PROCESS_LAST_START_TIME_MSG,
	 bosutil_DateOf(statusP->procStartTime), statusP->procStarts);
    }
    if (statusP->lastAnyExit) {
      dce_svc_printf(BOS_S_LAST_EXIT_TIME_MSG, bosutil_DateOf(statusP->lastAnyExit));
    }
    if (statusP->lastErrorExit) {
      dce_fprintf(stderr, bos_s_last_error_exit1,
              bosutil_DateOf(statusP->lastErrorExit));
      if (statusP->errorSignal) {
	if (statusP->errorSignal == SIGTERM) {
	  dce_fprintf(stdout, bos_s_last_error_exit2a);
	}
	else {
	  dce_fprintf(stdout, bos_s_last_error_exit2b, statusP->errorSignal);
	}
      }
      else {
	dce_fprintf(stdout, bos_s_last_error_exit2c, statusP->errorCode);
      }
    }

    /* and, finally, print parameter info */
    for (i = 0; (i < MAX_PARAMS) && ((paramBuffers[i].theString)[0] != '\0'); i++) {
      dce_fprintf(stdout, bos_s_parameter_info, i + 1, paramBuffers[i].theString);
    }
    dce_fprintf(stdout, bos_s_extra_newline);
  }

}

PRIVATE int bos_StatOneProcess(name, longDisplay)
     char *	name;
     int	longDisplay;
{
  int			rtnVal = 0;
  long			bossvr_error = 0;
  error_status_t	rpc_error = error_status_ok;
  char *		errorStringP = "";
  bossvr_out_string	typeBuffer;
  long			processStatus;
  bossvr_out_string	auxStatusBuffer;
  bossvr_out_string	paramBuffers[MAX_PARAMS];
  char			cmdLine[BUFSIZ];
  struct bossvr_status	theStatus;
  int			i = 0;

  /* initialize arrays */
  for (i = 0; i < MAX_PARAMS; i++) {
    paramBuffers[i].strLen = 0;
    paramBuffers[i].theString = (unsigned char *)NULL;
  }
  i = 0;
  typeBuffer.strLen = 0;
  typeBuffer.theString = (unsigned char *)NULL;
  auxStatusBuffer.strLen = 0;
  auxStatusBuffer.theString = (unsigned char *)NULL;

  /* get instance info */
  bossvr_error = BOSSVR_GetInstanceInfo(theBosserverBinding,
				    (unsigned char *)name, &typeBuffer, &theStatus, &rpc_error);

  if ((rpc_error == error_status_ok) && (bossvr_error == 0)) {
    /* get status info */
    bossvr_error = BOSSVR_GetStatus(theBosserverBinding,
				(unsigned char *)name, &processStatus, &auxStatusBuffer, &rpc_error);

    if ((rpc_error == error_status_ok) && (bossvr_error == 0)) {
      if (longDisplay) {
	while ((bossvr_error == 0) && (i < MAX_PARAMS)) {
	  bossvr_error = BOSSVR_GetInstanceParm(theBosserverBinding,
					    (unsigned char *)name, i, &(paramBuffers[i]), &rpc_error);
	  if ((rpc_error != error_status_ok) ||
	      ((bossvr_error != 0) && (bossvr_error != BZDOM))) {
	    errorStringP = ((rpc_error != error_status_ok) ?
			    dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
	    dce_svc_printf(BOS_S_PARM_FAILURE_MSG, programName, i, name, errorStringP);
	    rtnVal = 1;
	  }

	  i++;
	}
      }
      bos_PrintAllStatusInfo(name, typeBuffer.theString, &theStatus,
			     processStatus, auxStatusBuffer.theString, paramBuffers,
			     longDisplay);
    }
    else {
      errorStringP = ((rpc_error != error_status_ok) ?
		      dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
      dce_svc_printf(BOS_S_STATUS_FAILURE_AGAIN_MSG, programName, name, errorStringP);
      rtnVal = 1;
    }
  }
  else {
    errorStringP = ((rpc_error != error_status_ok) ?
		    dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
    dce_svc_printf(BOS_S_NO_INSTANCE_INFO_MSG, programName, name, errorStringP);
    rtnVal = 1;
  }

  return rtnVal;

}

PRIVATE int bos_StatNamedProcesses(aSyntax)
     struct cmd_syndesc *	aSyntax;
{
  int			everFailed = 0;
  int			longDisplay;
  struct cmd_item *	thisItemP;

  longDisplay = cmd_IsParamPresent(aSyntax, STATUS_LONG_INDEX);
  for(thisItemP = aSyntax->parms[STATUS_PROCESS_INDEX].items;
      thisItemP != (struct cmd_item *)NULL;
      thisItemP = thisItemP->next) {
    if (bos_StatOneProcess(thisItemP->data, longDisplay) != 0) {
      everFailed = 1;
    }
  }

  return everFailed;
}

int bos_DoStatus(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  long			bossvr_error = 0;
  error_status_t	rpc_error = error_status_ok;
  char *		errorStringP = "";
  int			i;
  bossvr_out_string	instanceName;
  int			longDisplay = 0;
  ulong			serverStatus = 0;
  static char		routineName[] = "bos_DoStatus";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));

  longDisplay = cmd_IsParamPresent(aSyntax, STATUS_LONG_INDEX);

  /* first, check the overall status of the machine */
  bossvr_error = BOSSVR_GetServerStatus(theBosserverBinding, &serverStatus, &rpc_error);
  if ((rpc_error == error_status_ok) && (bossvr_error == 0)) {
    if ((serverStatus & BOSSVR_SERVER_NOAUTH) != 0) {
      dce_svc_printf(BOS_S_BOSSERVER_REPORT_MSG);
    }

    if (longDisplay && ((serverStatus & BOSSVR_BADDIRACCESS) != 0)) {
      dce_svc_printf(BOS_S_INAPPROP_SRVR_DIR_ACCESS_MSG);
    }
  }
  else {
    errorStringP = ((rpc_error != error_status_ok) ?
		    dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
    dce_svc_printf(BOS_S_NO_HOST_BOSSRVR_CONTACT_MSG, programName, errorStringP);
    rtnVal = 1;
  }	/* end else (error condition) */


  /* now, check the status of each of the individual bnodes */
  if (rtnVal == 0) {
    /* if processes are named, the user wants to know only about those processes */
    if (cmd_IsParamPresent(aSyntax, STATUS_PROCESS_INDEX)) {
      rtnVal = bos_StatNamedProcesses(aSyntax);
    }
    /* otherwise, the user wants to know about all processes managed by the server */
    else {
      for (i = 0; (bossvr_error == 0) && (rtnVal == 0); i++) {
	instanceName.strLen = 0;
	instanceName.theString = (unsigned char *)NULL;
	bossvr_error = BOSSVR_EnumerateInstance(theBosserverBinding,
						i, &instanceName, &rpc_error);

	if ((rpc_error == error_status_ok) && (bossvr_error == 0)) {
	  bos_StatOneProcess(instanceName.theString, longDisplay, (i == 0));
	}
	else {
	  if (bossvr_error != BZDOM) {
	    errorStringP = ((rpc_error != error_status_ok) ?
			    dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
	    dce_svc_printf(BOS_S_NO_HOST_BOSSRVR_CONTACT2_MSG, programName, errorStringP);
	    rtnVal = 1;
	  }
	}	/* end else (error condition) */
      }	/* end for (instance iteration) */
    }	/* end else (if must iterate over all instances) */
  }

  return rtnVal;
}

int bos_DoStop(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  long			bossvr_error = 0;
  error_status_t	rpc_error = error_status_ok;
  char *		errorStringP = "";
  struct cmd_item *	thisItemP;
  static char		routineName[] = "bos_DoStop";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));

  for (thisItemP = aSyntax->parms[STOP_PROCESS_INDEX].items;
       (rpc_error == error_status_ok) && (thisItemP != (struct cmd_item *)NULL);
       thisItemP = thisItemP->next) {
    bossvr_error = BOSSVR_SetStatus(theBosserverBinding,
				    (unsigned char *)thisItemP->data, BSTAT_SHUTDOWN, &rpc_error);

    if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
      errorStringP = ((rpc_error != error_status_ok) ?
		      dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
      dce_svc_printf(BOS_S_INSTANCE_NOT_STOPPED_MSG, programName, thisItemP->data,
		      errorStringP);
      rtnVal = 1;
    }
  }

  if ((rpc_error == error_status_ok) && (cmd_IsParamPresent(aSyntax, STOP_WAIT_INDEX))) {
    bossvr_error = BOSSVR_WaitAll(theBosserverBinding, &rpc_error);
    if ((rpc_error != error_status_ok) || (bossvr_error != 0)) {
      errorStringP = ((rpc_error != error_status_ok) ?
		      dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
      dce_svc_printf(BOS_S_NOT_WAITING_PROC_SHUTD_MSG, programName, errorStringP);
      rtnVal = 1;
    }
  }

  return rtnVal;
}

int bos_DoUnInstall(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  long			bossvr_error = 0;
  error_status_t	rpc_error = error_status_ok;
  char *		errorStringP = "";
  struct cmd_item *	thisItemP;
  char			destDir[BUFSIZ];
  char			fullPathname[BUFSIZ];
  int			allUsed = 0;
  u_long		flags = 0;
  char			noEntryError[] = "there is no earlier version present to reinstall";
  static char		routineName[] = "bos_DoUnInstall";

  dmprintf(bos_debug_flag, BOS_DEBUG_BIT_CMD,
	   ("%s entered.", routineName));

  if (cmd_IsParamPresent(aSyntax, UNINSTALL_DESTDIR_INDEX)) {
    (void)strcpy(destDir, aSyntax->parms[UNINSTALL_DESTDIR_INDEX].items->data);
  }
  else {
    (void)strcpy(destDir, DEFAULT_DEST_DIR);
  }

  if (cmd_IsParamPresent(aSyntax, UNINSTALL_ALL_INDEX)) {
    flags |= BOSSVR_UNINSTALL_ALL;
  }

  for (thisItemP = aSyntax->parms[UNINSTALL_FILE_INDEX].items;
       ((thisItemP != (struct cmd_item *)NULL) && (rtnVal == 0));
       thisItemP = thisItemP->next) {
    bosutil_ComputeDestDir(thisItemP->data, (char *)destDir, (char *)fullPathname, sizeof(fullPathname));
    bossvr_error = BOSSVR_UnInstall(theBosserverBinding, (unsigned char *)fullPathname,
				    flags, &rpc_error);

    if ((rpc_error == error_status_ok) && (bossvr_error == 0)) {
      dce_svc_printf(BOS_S_FILE_NOT_INSTALLED2_MSG, programName, thisItemP->data);
    }
    else {
      if (bossvr_error == BZNOENT) {
	errorStringP = noEntryError;
      }
      else {
	errorStringP = ((rpc_error != error_status_ok) ?
			dfs_dceErrTxt(rpc_error) : bosutil_em(bossvr_error));
      }

      dce_svc_printf(BOS_S_UNINSTAL_ITEM_FAILED_MSG, programName, thisItemP->data,
		        errorStringP);
      rtnVal = 1;
    }
  }

  return rtnVal;
}



int main(argc, argv)
     int	argc;
     char *	argv[];
{
  int		rtnVal = 0;
  long		cmdCode = 0;

  osi_setlocale(LC_ALL, "");


    initialize_svc();


  /* initialize variables containing useful path names */
  InitPaths();

  /* intialization and clean up code */
  (void)cmd_SetBeforeProc(InitProc, (char *)NULL);
  (void)cmd_SetAfterProc(TermProc, (char *)NULL);

  (void)strcpy(programName, argv[0]);

  bos_ui_SetUpSyntax();

  /* now, let the cmd package figure out what needs to be done */
  cmdCode = cmd_Dispatch(argc, argv);
  if (cmdCode) {
    if ( cmd_IsCmdError(cmdCode) ) {
/*
      com_err(argv[0], cmdCode, "command dispatch failed");
*/
    }
    rtnVal = 1;
  }

  return rtnVal;
}
