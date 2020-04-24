/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bos_userint.c,v $
 * Revision 1.1.537.1  1996/10/02  21:06:18  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:50:20  damon]
 *
 * Revision 1.1.529.4  1994/08/24  17:57:52  sasala
 * 	Implement partial S12Y for DFS - bos
 * 	[1994/08/24  17:43:07  sasala]
 * 
 * Revision 1.1.529.3  1994/07/13  22:23:07  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  20:53:49  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:03:59  mbs]
 * 
 * Revision 1.1.529.2  1994/06/09  14:24:00  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:35:56  annie]
 * 
 * Revision 1.1.529.1  1994/02/04  20:34:35  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:19:51  devsrc]
 * 
 * Revision 1.1.527.1  1993/12/07  17:37:01  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  17:48:18  jaffe]
 * 
 * Revision 1.1.4.8  1993/01/21  16:28:45  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  18:22:02  cjd]
 * 
 * Revision 1.1.4.7  1992/11/24  19:50:37  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:35:36  bolinger]
 * 
 * Revision 1.1.4.6  1992/11/18  20:58:41  jaffe
 * 	Transarc delta: comer-ot5785-remove-valid-from-argument 1.1
 * 	  Selected comments:
 * 	    Removed _valid from -kvno parameter description for addkey and genkey.
 * 	    See above.
 * 	[1992/11/17  22:30:47  jaffe]
 * 
 * Revision 1.1.4.5  1992/10/28  21:45:55  jaffe
 * 	Fixed RCSID
 * 	[1992/10/28  21:18:20  jaffe]
 * 
 * Revision 1.1.4.4  1992/10/27  21:52:14  jaffe
 * 	Transarc delta: bab-ot5471-bbos-uninstall-all 1.2
 * 	  Selected comments:
 * 	    The bos command and bosserver will now support a -all flag for
 * 	    removing all versions of the specified file.
 * 	    ot 5471
 * 	    Added -all flag to the user interface.
 * 	    Cosmetic cleanup.
 * 	Transarc delta: bab-ot5473-bos-zero-kvno-string 1.1
 * 	  Selected comments:
 * 	    Changed help messages for -kvno in addkey and genkey to indicate that "+"
 * 	    is aaceptable to specify that the system should select an appropriate kvno.
 * 	    Also installed validation routines to ensure that no number bigger than
 * 	    is accepted.
 * 	    ot 5473
 * 	    See above.
 * 	Transarc delta: bab-ot5475-bbos-dacl-clearlist 1.2
 * 	  Selected comments:
 * 	    Added support for a -clearlist option to the bos command, without
 * 	    actually activating it inthe user interface.
 * 	    ot 5475
 * 	    Added support for -clearlist option to rmadmin.
 * 	    Needed to handle cases in which clearlist may have been specified
 * 	    alone or with more than one user or group.
 * 	[1992/10/27  15:28:44  jaffe]
 * 
 * Revision 1.1.4.3  1992/09/25  19:43:54  jaffe
 * 	Transarc delta: bab-ot5231-bos-accept-only-numbers-for-kvno 1.1
 * 	  Selected comments:
 * 	    The bos commands were accepting non-numeric data as key version number
 * 	    specification.
 * 	    ot 5231
 * 	    Add CMD_PARM_NUMERIC flags to creation of all kvno params.
 * 	[1992/09/23  19:57:50  jaffe]
 * 
 * Revision 1.1.4.2  1992/08/31  21:53:07  jaffe
 * 	Replace missing RCS ids
 * 	[1992/08/31  16:10:18  jaffe]
 * 
 * 	Transarc delta: bab-ot4755-bbos-remove-exec 1.1
 * 	  Selected comments:
 * 	    Removed bosserver exec funcitionality and the bos command that invokes it.
 * 	    ot 4755
 * 	    See above.
 * 	Transarc delta: jaffe-ot3211-fix-prototypes 1.1
 * 	  Selected comments:
 * 	    Remove ANSI-C declaration files which contained prototypes for standard
 * 	    functions.  We expect these prototypes to be defined by the system include
 * 	    files.
 * 	    use stdio instead of stdioDecls.h
 * 	[1992/08/30  13:33:57  jaffe]
 * 
 * Revision 1.1.2.3  1992/05/20  20:18:47  mason
 * 	Transarc delta: bab-ot3012-bos-noauth-localauth 1.1
 * 	  Files modified:
 * 	    userInt/bos: bos_userint.c
 * 	  Selected comments:
 * 	    The "-noauth" and "-localauth" optiosn to the bos commands are
 * 	    mutually exclusive, but the syntax spec didn't reflect that.
 * 	    This delta makes them mutually exclusive.
 * 	    Make "-noauth" and "-localauth" mutually exclusive.
 * 	[1992/05/20  16:46:12  mason]
 * 
 * Revision 1.1.2.2  1992/04/14  04:11:33  mason
 * 	Transarc delta: bab-ot2194-bbos-salvage-setdebug 1.1
 * 	  Files modified:
 * 	    bosserver: Makefile, bbos_ncs_interface.acf
 * 	    bosserver: bbos_ncs_interface.idl, bossvr_ncs_procs.c
 * 	    userInt/bos: Makefile, bos_main.c, bos_userint.c
 * 	    userInt/bos: bos_userint.h
 * 	  Selected comments:
 * 	    Removed references to bos salvage in bos and the bosserver,
 * 	    put references to bos setdebug in appropriate ifdefs (but
 * 	    it is always a part of the bosserver interface, it just will
 * 	    return BZNOTIMPL if it hasn't been activated) and some general
 * 	    code cleanup.
 * 	    Remove dead code and cover implementation of bos setdebug with
 * 	    appropriate ifdefs.
 * 	[1992/04/13  18:07:07  mason]
 * 
 * Revision 1.1  1992/01/19  02:50:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  bos_userint.c -- routines that set up the syntax descriptors for the user
 * interface for the bos command suite
 *
 *
 *	Copyright (C) 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>


RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/userInt/bos/bos_userint.c,v 1.1.537.1 1996/10/02 21:06:18 damon Exp $")

#include <dcedfs/cmd.h>
#include <dcedfs/cmdAux.h>

#include <stdio.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbosmac.h>
#include <dfsbossvc.h>
#include <dfsbosmsg.h>

#include <bos_userint.h>

  unsigned char   *cmd_explanation;
  error_status_t   st;
/*
 * the validation routines for key version numbers
 */
int ValidateKvnoString(theStringP, theParamDescP, allowZero)
     char *			theStringP;
     struct cmd_parmdesc *	theParamDescP;
     int			allowZero;
{
  int		rtnVal = 0;	/* assume success */
  int		kvno = 0;
  
  if (strcmp(theStringP, "+") == 0) {
    if (!allowZero) {
      rtnVal = 1;
    }
  }
  else {
    rtnVal = cmd_param_CheckStringNumeric(theStringP, 0);
    if (rtnVal == 0) {
      kvno = atoi(theStringP);
      if (kvno > 255) {
	rtnVal = 1;
      }
      else {
	if ((kvno < 0) || ((!allowZero) && (kvno == 0))) {
	  rtnVal = 1;
	}
      }
    }
  }
  
  return rtnVal;
}

int ValidateKvnoStringWithZero(theStringP, theParamDescP)
     char *			theStringP;
     struct cmd_parmdesc *	theParamDescP;
{
  return ValidateKvnoString(theStringP, theParamDescP, 1);
}

int ValidateKvnoStringWithoutZero(theStringP, theParamDescP)
     char *			theStringP;
     struct cmd_parmdesc *	theParamDescP;
{
  return ValidateKvnoString(theStringP, theParamDescP, 0);
}

/* the user interface setup routines */
void bos_ui_SetUpNormalDefaultArgs(aSyntax)
     struct cmd_syndesc *	aSyntax;
{



  cmd_explanation = dce_msg_get_msg(bos_s_add_server, &st);
  (void)cmd_IndexedAddParm(aSyntax, "-server", CMD_SINGLE, CMD_REQUIRED, 
		(char *) cmd_explanation, BOS_SERVER_INDEX);
  free(cmd_explanation);


  (void)cmd_Seek(aSyntax, BOS_NORMAL_OFFSET);
/*

  cmd_explanation = dce_msg_get_msg(bos_s_add_cell, &st);
  (void)cmd_AddParm(aSyntax, "-cell", CMD_SINGLE, CMD_OPTIONAL, 
  (char *) cmd_explanation);
  free(cmd_explanation);

*/
}

void bos_ui_SetUpNormalDefaultFlags(aSyntax)
     struct cmd_syndesc *	aSyntax;
{
  (void)cmd_Seek(aSyntax, BOS_NOAUTH_INDEX);
  

  cmd_explanation = dce_msg_get_msg(bos_s_add_noauth, &st);
  (void)cmd_AddParm(aSyntax, "-noauth", CMD_FLAG, CMD_OPTIONAL, 
  (char *) cmd_explanation);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bos_s_add_localauth, &st);
  (void)cmd_AddParm(aSyntax, "-localauth", CMD_FLAG, CMD_OPTIONAL, 
  (char *) cmd_explanation);
  free(cmd_explanation);

  (void)cmd_MakeAlternate(aSyntax, "-noauth", "-localauth");
}

void bos_ui_SetUpAddKey()
{
  struct cmd_syndesc *	addKeySyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_addkey, &st);
  addKeySyntax = cmd_CreateSyntax("addkey", bos_DoAddKey, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);



  cmd_explanation = dce_msg_get_msg(bos_s_addkey_kvno, &st);
  (void)cmd_IndexedAddParm(addKeySyntax, "-kvno", CMD_SINGLE,
			   CMD_REQUIRED,
  (char *) cmd_explanation , ADDKEY_KVNO_INDEX);
  free(cmd_explanation);

  cmd_AddValidationRoutine(addKeySyntax, "-kvno", ValidateKvnoStringWithZero);
  

  cmd_explanation = dce_msg_get_msg(bos_s_addkey_password, &st);
  (void)cmd_IndexedAddParm(addKeySyntax, "-password", CMD_SINGLE, CMD_REQUIRED, 
		(char *) cmd_explanation, ADDKEY_PASSWORD_INDEX);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bos_s_addkey_principal, &st);
  (void)cmd_IndexedAddParm(addKeySyntax, "-principal", CMD_SINGLE, CMD_OPTIONAL,
  (char *) cmd_explanation , ADDKEY_PRINCIPAL_INDEX);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bos_s_addkey_localonly, &st);
  (void)cmd_IndexedAddParm(addKeySyntax, "-localonly", CMD_FLAG, CMD_OPTIONAL, 
		(char *) cmd_explanation, ADDKEY_LOCALONLY_INDEX);
  free(cmd_explanation);


  bos_ui_SetUpNormalDefaultArgs(addKeySyntax);
  bos_ui_SetUpNormalDefaultFlags(addKeySyntax);
}

void bos_ui_SetUpAddUser()
{
  struct cmd_syndesc *	addUserSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_addadmin, &st);
  addUserSyntax = cmd_CreateSyntax("addadmin", bos_DoAddUser, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);



  cmd_explanation = dce_msg_get_msg(bos_s_add_adminlist, &st);
  (void)cmd_IndexedAddParm(addUserSyntax, "-adminlist", CMD_SINGLE, CMD_REQUIRED,
  (char *) cmd_explanation , ADDUSER_FILENAME_INDEX);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bos_s_adduser_principal, &st);
  (void)cmd_IndexedAddParm(addUserSyntax, "-principal", CMD_LIST, CMD_OPTIONAL, 
		(char *) cmd_explanation, ADDUSER_USERS_INDEX);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bos_s_adduser_group, &st);
  (void)cmd_IndexedAddParm(addUserSyntax, "-group", CMD_LIST, CMD_OPTIONAL, 
		(char *) cmd_explanation, ADDUSER_GROUPS_INDEX);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bos_s_adduser_createlist, &st);
  (void)cmd_IndexedAddParm(addUserSyntax, "-createlist", CMD_FLAG, CMD_OPTIONAL, 
		(char *) cmd_explanation, ADDUSER_CREATEFLAG_INDEX);
  free(cmd_explanation);


  bos_ui_SetUpNormalDefaultArgs(addUserSyntax);
  bos_ui_SetUpNormalDefaultFlags(addUserSyntax);
}

void bos_ui_SetUpCreate()
{
  struct cmd_syndesc *	createSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_create, &st);
  createSyntax = cmd_CreateSyntax("create", bos_DoCreate, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);



  cmd_explanation = dce_msg_get_msg(bos_s_add_create_process, &st);
  (void)cmd_IndexedAddParm(createSyntax, "-process", CMD_SINGLE, CMD_REQUIRED,
  (char *) cmd_explanation , CREATE_PROCESS_INDEX);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bos_s_add_create_type, &st);
  (void)cmd_IndexedAddParm(createSyntax, "-type", CMD_SINGLE, CMD_REQUIRED,
  (char *) cmd_explanation , CREATE_TYPE_INDEX);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bos_s_add_create_cmd, &st);
  (void)cmd_IndexedAddParm(createSyntax, "-cmd", CMD_LIST, CMD_REQUIRED,
  (char *) cmd_explanation , CREATE_CMD_INDEX);
  free(cmd_explanation);

  bos_ui_SetUpNormalDefaultArgs(createSyntax);
  bos_ui_SetUpNormalDefaultFlags(createSyntax);
}

void bos_ui_SetUpDelete()
{
  struct cmd_syndesc *	deleteSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_delete, &st);
  deleteSyntax = cmd_CreateSyntax("delete", bos_DoDelete, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);

  

  cmd_explanation = dce_msg_get_msg(bos_s_add_del_process, &st);
  (void)cmd_IndexedAddParm(deleteSyntax, "-process", CMD_LIST, CMD_REQUIRED,
  (char *) cmd_explanation , DELETE_PROCESS_INDEX);
  free(cmd_explanation);

  bos_ui_SetUpNormalDefaultArgs(deleteSyntax);
  bos_ui_SetUpNormalDefaultFlags(deleteSyntax);
}

void bos_ui_SetUpGcKeys()
{
  struct cmd_syndesc *	gcKeysSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_gckeys, &st);
  gcKeysSyntax = cmd_CreateSyntax("gckeys", bos_DoGcKeys, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);

  

  cmd_explanation = dce_msg_get_msg(bos_s_add_gckeys_principal, &st);
  (void)cmd_IndexedAddParm(gcKeysSyntax, "-principal", CMD_SINGLE, CMD_OPTIONAL,
  (char *) cmd_explanation , GCKEYS_PRINCIPAL_INDEX);
  free(cmd_explanation);


  bos_ui_SetUpNormalDefaultArgs(gcKeysSyntax);
  bos_ui_SetUpNormalDefaultFlags(gcKeysSyntax);
}

void bos_ui_SetUpGenKey()
{
  struct cmd_syndesc *	genKeySyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_genkey, &st);
  genKeySyntax = cmd_CreateSyntax("genkey", bos_DoGenKey, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);

  

  cmd_explanation = dce_msg_get_msg(bos_s_add_genkey_kvno, &st);
  (void)cmd_IndexedAddParm(genKeySyntax, "-kvno", CMD_SINGLE, 
			   CMD_REQUIRED,
  (char *) cmd_explanation , GENKEY_KVNO_INDEX);
  free(cmd_explanation);

  cmd_AddValidationRoutine(genKeySyntax, "-kvno", ValidateKvnoStringWithZero);


  cmd_explanation = dce_msg_get_msg(bos_s_add_genkey_princ, &st);
  (void)cmd_IndexedAddParm(genKeySyntax, "-principal", CMD_SINGLE, CMD_OPTIONAL,
  (char *) cmd_explanation , GENKEY_PRINCIPAL_INDEX);
  free(cmd_explanation);


  bos_ui_SetUpNormalDefaultArgs(genKeySyntax);
  bos_ui_SetUpNormalDefaultFlags(genKeySyntax);
}

void bos_ui_SetUpGetDates()
{
  struct cmd_syndesc *	getDatesSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_getdates, &st);
  getDatesSyntax = cmd_CreateSyntax("getdates", bos_DoGetDates, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);

  

  cmd_explanation = dce_msg_get_msg(bos_s_add_getdates_file, &st);
  (void)cmd_IndexedAddParm(getDatesSyntax, "-file", CMD_LIST, CMD_REQUIRED,
  (char *) cmd_explanation , GETDATES_FILE_INDEX);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bos_s_add_getdates_dir, &st);
  (void)cmd_IndexedAddParm(getDatesSyntax, "-dir", CMD_SINGLE, CMD_OPTIONAL,
  (char *) cmd_explanation , GETDATES_DESTDIR_INDEX);
  free(cmd_explanation);

  bos_ui_SetUpNormalDefaultArgs(getDatesSyntax);
  bos_ui_SetUpNormalDefaultFlags(getDatesSyntax);
}

void bos_ui_SetUpGetLog()
{
  struct cmd_syndesc *	getLogSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_getlog, &st);
  getLogSyntax = cmd_CreateSyntax("getlog", bos_DoGetLog, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);

  

  cmd_explanation = dce_msg_get_msg(bos_s_add_getlog_file, &st);
  (void)cmd_IndexedAddParm(getLogSyntax, "-file", CMD_SINGLE, CMD_REQUIRED, 
		(char *) cmd_explanation, GETLOG_FILE_INDEX);
  free(cmd_explanation);

  bos_ui_SetUpNormalDefaultArgs(getLogSyntax);
  bos_ui_SetUpNormalDefaultFlags(getLogSyntax);
}

void bos_ui_SetUpGetRestart()
{
  struct cmd_syndesc *	getRestartSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_getrestart, &st);
  getRestartSyntax = cmd_CreateSyntax("getrestart", bos_DoGetRestart, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);


  bos_ui_SetUpNormalDefaultArgs(getRestartSyntax);
  bos_ui_SetUpNormalDefaultFlags(getRestartSyntax);
}

void bos_ui_SetUpInstall()
{
  struct cmd_syndesc *	installSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_install, &st);
  installSyntax = cmd_CreateSyntax("install", bos_DoInstall, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);



  cmd_explanation = dce_msg_get_msg(bos_s_add_install_file, &st);
  (void)cmd_IndexedAddParm(installSyntax, "-file", CMD_LIST, CMD_REQUIRED, 
		(char *) cmd_explanation, INSTALL_FILE_INDEX);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bos_s_add_install_dir, &st);
  (void)cmd_IndexedAddParm(installSyntax, "-dir", CMD_SINGLE, CMD_OPTIONAL,
  (char *) cmd_explanation , INSTALL_DESTDIR_INDEX);
  free(cmd_explanation);

  bos_ui_SetUpNormalDefaultArgs(installSyntax);
  bos_ui_SetUpNormalDefaultFlags(installSyntax);
}

#ifdef notdef
void bos_ui_SetUpKill()
{
  struct cmd_syndesc *	killSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_kill_pun, &st);
  killSyntax = cmd_CreateSyntax("kill", bos_DoKill, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);


  bos_ui_SetUpNormalDefaultArgs(killSyntax);
  bos_ui_SetUpNormalDefaultFlags(killSyntax);
}
#endif /* notdef */

void bos_ui_SetUpLsCell()
{
  struct cmd_syndesc *	lsCellSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_lscell, &st);
  lsCellSyntax = cmd_CreateSyntax("lscell", bos_DoLsCell, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);


  bos_ui_SetUpNormalDefaultArgs(lsCellSyntax);
  bos_ui_SetUpNormalDefaultFlags(lsCellSyntax);
}

#ifdef notdef
void bos_ui_SetUpLsHosts()
{
  struct cmd_syndesc *	lsHostsSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_lshosts, &st);
  lsHostsSyntax = cmd_CreateSyntax("lshosts", bos_DoLsHosts, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);


  bos_ui_SetUpNormalDefaultArgs(lsHostsSyntax);
  bos_ui_SetUpNormalDefaultFlags(lsHostsSyntax);
}
#endif /* notdef */

void bos_ui_SetUpLsKeys()
{
  struct cmd_syndesc *	lsKeysSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_lskeys, &st);
  lsKeysSyntax = cmd_CreateSyntax("lskeys", bos_DoLsKeys, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);



  cmd_explanation = dce_msg_get_msg(bos_s_add_lskeys_princ, &st);
  (void)cmd_IndexedAddParm(lsKeysSyntax, "-principal", CMD_SINGLE, CMD_OPTIONAL,
  (char *) cmd_explanation , LSKEYS_PRINCIPAL_INDEX);
  free(cmd_explanation);


  bos_ui_SetUpNormalDefaultArgs(lsKeysSyntax);
  bos_ui_SetUpNormalDefaultFlags(lsKeysSyntax);
}

void bos_ui_SetUpLsUsers()
{
  struct cmd_syndesc *	lsUsersSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_lsadmin, &st);
  lsUsersSyntax = cmd_CreateSyntax("lsadmin", bos_DoLsUsers, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);



  cmd_explanation = dce_msg_get_msg(bos_s_add_lsusers_adminlist, &st);
  (void)cmd_IndexedAddParm(lsUsersSyntax, "-adminlist", CMD_SINGLE, CMD_REQUIRED,
  (char *) cmd_explanation , LSUSERS_FILENAME_INDEX);
  free(cmd_explanation);

  bos_ui_SetUpNormalDefaultArgs(lsUsersSyntax);
  bos_ui_SetUpNormalDefaultFlags(lsUsersSyntax);
}

void bos_ui_SetUpPrune()
{
  struct cmd_syndesc *	pruneSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_prune_now, &st);
  pruneSyntax = cmd_CreateSyntax("prune", bos_DoPrune, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);



  cmd_explanation = dce_msg_get_msg(bos_s_add_prune_bak, &st);
  (void)cmd_IndexedAddParm(pruneSyntax, "-bak", CMD_FLAG, CMD_OPTIONAL, 
		(char *) cmd_explanation, PRUNE_BAK_INDEX);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bos_s_add_purne_old, &st);
  (void)cmd_IndexedAddParm(pruneSyntax, "-old", CMD_FLAG, CMD_OPTIONAL, 
		(char *) cmd_explanation, PRUNE_OLD_INDEX);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bos_s_add_prune_core, &st);
  (void)cmd_IndexedAddParm(pruneSyntax, "-core", CMD_FLAG, CMD_OPTIONAL, 
		(char *) cmd_explanation, PRUNE_CORE_INDEX);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bos_s_add_prune_all, &st);
  (void)cmd_IndexedAddParm(pruneSyntax, "-all", CMD_FLAG, CMD_OPTIONAL, 
		(char *) cmd_explanation, PRUNE_ALL_INDEX);
  free(cmd_explanation);

  bos_ui_SetUpNormalDefaultArgs(pruneSyntax);
  bos_ui_SetUpNormalDefaultFlags(pruneSyntax);
}

void bos_ui_SetUpRmKey()
{
  struct cmd_syndesc *	rmKeySyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_rmkey, &st);
  rmKeySyntax = cmd_CreateSyntax("rmkey", bos_DoRmKey, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);



  cmd_explanation = dce_msg_get_msg(bos_s_add_rmkey_kvno, &st);
  (void)cmd_IndexedAddParm(rmKeySyntax, "-kvno", CMD_LIST, CMD_REQUIRED,
		(char *) cmd_explanation, RMKEY_KVNO_INDEX);
  free(cmd_explanation);

  cmd_AddValidationRoutine(rmKeySyntax, "-kvno", ValidateKvnoStringWithoutZero);


  cmd_explanation = dce_msg_get_msg(bos_s_add_rmkey_princ, &st);
  (void)cmd_IndexedAddParm(rmKeySyntax, "-principal", CMD_SINGLE, CMD_OPTIONAL,
  (char *) cmd_explanation , RMKEY_PRINCIPAL_INDEX);
  free(cmd_explanation);

  bos_ui_SetUpNormalDefaultArgs(rmKeySyntax);
  bos_ui_SetUpNormalDefaultFlags(rmKeySyntax);
}

void bos_ui_SetUpRmUser()
{
  struct cmd_syndesc *	rmUserSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_creat_rmadmin, &st);
  rmUserSyntax = cmd_CreateSyntax("rmadmin", bos_DoRmUser, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);



  cmd_explanation = dce_msg_get_msg(bos_s_add_rmuser_adminlist, &st);
  (void)cmd_IndexedAddParm(rmUserSyntax, "-adminlist", CMD_SINGLE, CMD_REQUIRED,
  (char *) cmd_explanation , RMUSER_FILENAME_INDEX);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bos_s_add_rmuser_princ, &st);
  (void)cmd_IndexedAddParm(rmUserSyntax, "-principal", CMD_LIST, CMD_OPTIONAL, 
		(char *) cmd_explanation, RMUSER_USER_INDEX);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bos_s_add_rmuser_group, &st);
  (void)cmd_IndexedAddParm(rmUserSyntax, "-group", CMD_LIST, CMD_OPTIONAL, 
		(char *) cmd_explanation, RMUSER_GROUP_INDEX);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bos_s_add_rmuser_rmlist, &st);
  (void)cmd_IndexedAddParm(rmUserSyntax, "-removelist", CMD_FLAG, CMD_OPTIONAL, 
		(char *) cmd_explanation, RMUSER_REMOVEFLAG_INDEX);
  free(cmd_explanation);


#if defined(BOS_ACTIVATE_RMADMIN_CLEARLIST)

  cmd_explanation = dce_msg_get_msg(bos_s_add_rmuser_clrlist, &st);
  (void)cmd_IndexedAddParm(rmUserSyntax, "-clearlist", CMD_FLAG, CMD_OPTIONAL, 
		(char *) cmd_explanation, RMUSER_CLEARLIST_INDEX);
  free(cmd_explanation);

#endif /* defined(BOS_ACTIVATE_RMADMIN_CLEARLIST) */

  bos_ui_SetUpNormalDefaultArgs(rmUserSyntax);
  bos_ui_SetUpNormalDefaultFlags(rmUserSyntax);
}

void bos_ui_SetUpRestart()
{
  struct cmd_syndesc *	restartSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_restart, &st);
  restartSyntax = cmd_CreateSyntax("restart", bos_DoRestart, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);



  cmd_explanation = dce_msg_get_msg(bos_s_add_rest_bosserver, &st);
  (void)cmd_IndexedAddParm(restartSyntax, "-bosserver", CMD_FLAG, CMD_OPTIONAL, 
		(char *) cmd_explanation, RESTART_BOS_INDEX);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bos_s_add_rest_process, &st);
  (void)cmd_IndexedAddParm(restartSyntax, "-process", CMD_LIST, CMD_OPTIONAL,
  (char *) cmd_explanation , RESTART_PROCESS_INDEX);
  free(cmd_explanation);

  (void)cmd_MakeAlternate(restartSyntax, "-bosserver", "-process");

  bos_ui_SetUpNormalDefaultArgs(restartSyntax);
  bos_ui_SetUpNormalDefaultFlags(restartSyntax);
}

void bos_ui_SetUpSetAuth()
{
  struct cmd_syndesc *	setAuthSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_setauth, &st);
  setAuthSyntax = cmd_CreateSyntax("setauth", bos_DoSetAuth, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);

  

  cmd_explanation = dce_msg_get_msg(bos_s_add_sa_authchk, &st);
  (void)cmd_IndexedAddParm(setAuthSyntax, "-authchecking", CMD_SINGLE,
  (CMD_REQUIRED | CMD_PARM_LITERAL),
  (char *) cmd_explanation , SETAUTH_AUTHSTRING_INDEX);
  free(cmd_explanation);

  cmd_explanation = dce_msg_get_msg(bos_s_addhelp_sa_authchk, &st);
  (void)cmd_AddParamHelp(setAuthSyntax, "-authchecking",
  (char *) cmd_explanation , CMD_PARM_LITERAL);

  bos_ui_SetUpNormalDefaultArgs(setAuthSyntax);
  bos_ui_SetUpNormalDefaultFlags(setAuthSyntax);
}

#if defined(DFS_BOS_DEBUG_BOSSERVER)
void bos_ui_SetUpSetDebug()
{
  struct cmd_syndesc *	setDebugSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_setdebug, &st);
  setDebugSyntax = cmd_CreateSyntax("setdebug", bos_DoSetDebug, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);



  cmd_explanation = dce_msg_get_msg(bos_s_add_sdbg_flag, &st);
  (void)cmd_IndexedAddParm(setDebugSyntax, "-flag", CMD_SINGLE, CMD_OPTIONAL,
  (char *) cmd_explanation , SETDEBUG_FLAG_INDEX);
  free(cmd_explanation);

  bos_ui_SetUpNormalDefaultArgs(setDebugSyntax);
  bos_ui_SetUpNormalDefaultFlags(setDebugSyntax);
}
#endif /* defined(DFS_BOS_DEBUG_BOSSERVER) */

void bos_ui_SetUpSetRestart()
{
  struct cmd_syndesc *	setRestartSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_setrestart, &st);
  setRestartSyntax = cmd_CreateSyntax("setrestart", bos_DoSetRestart, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);

  

  cmd_explanation = dce_msg_get_msg(bos_s_add_setr_genl, &st);
  (void)cmd_IndexedAddParm(setRestartSyntax, "-general", CMD_SINGLE, CMD_REQUIRED, 
		(char *) cmd_explanation, SETRESTART_ALL_INDEX);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bos_s_add_setr_newbin, &st);
  (void)cmd_IndexedAddParm(setRestartSyntax, "-newbinary", CMD_SINGLE, CMD_REQUIRED,
  (char *) cmd_explanation , SETRESTART_BINARY_INDEX);
  free(cmd_explanation);

  (void)cmd_MakeAlternate(setRestartSyntax, "-general", "-newbinary");
  bos_ui_SetUpNormalDefaultArgs(setRestartSyntax);
  bos_ui_SetUpNormalDefaultFlags(setRestartSyntax);
}

void bos_ui_SetUpShutdown()
{
  struct cmd_syndesc *	shutdownSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_shutdown, &st);
  shutdownSyntax = cmd_CreateSyntax("shutdown", bos_DoShutdown, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);

  

  cmd_explanation = dce_msg_get_msg(bos_s_add_shutd_process, &st);
  (void)cmd_IndexedAddParm(shutdownSyntax, "-process", CMD_LIST, CMD_OPTIONAL,
  (char *) cmd_explanation , SHUTDOWN_PROCESS_INDEX);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bos_s_add_shutd_wait, &st);
  (void)cmd_IndexedAddParm(shutdownSyntax, "-wait", CMD_FLAG, CMD_OPTIONAL, 
		(char *) cmd_explanation, SHUTDOWN_WAIT_INDEX);
  free(cmd_explanation);


  bos_ui_SetUpNormalDefaultArgs(shutdownSyntax);
  bos_ui_SetUpNormalDefaultFlags(shutdownSyntax);
}

void bos_ui_SetUpStart()
{
  struct cmd_syndesc *	startSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_start, &st);
  startSyntax = cmd_CreateSyntax("start", bos_DoStart, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);



  cmd_explanation = dce_msg_get_msg(bos_s_add_start_process, &st);
  (void)cmd_IndexedAddParm(startSyntax, "-process", CMD_LIST, CMD_REQUIRED,
  (char *) cmd_explanation , START_PROCESS_INDEX);
  free(cmd_explanation);

  bos_ui_SetUpNormalDefaultArgs(startSyntax);
  bos_ui_SetUpNormalDefaultFlags(startSyntax);
}

void bos_ui_SetUpStartup()
{
  struct cmd_syndesc *	startupSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_startup, &st);
  startupSyntax = cmd_CreateSyntax("startup", bos_DoStartup, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);

  

  cmd_explanation = dce_msg_get_msg(bos_s_add_stup_process, &st);
  (void)cmd_IndexedAddParm(startupSyntax, "-process", CMD_LIST, CMD_OPTIONAL,
  (char *) cmd_explanation , STARTUP_PROCESS_INDEX);
  free(cmd_explanation);

  bos_ui_SetUpNormalDefaultArgs(startupSyntax);
  bos_ui_SetUpNormalDefaultFlags(startupSyntax);
}

void bos_ui_SetUpStatus()
{
  struct cmd_syndesc *	statusSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_status, &st);
  statusSyntax = cmd_CreateSyntax("status", bos_DoStatus, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);

  

  cmd_explanation = dce_msg_get_msg(bos_s_add_status_proc, &st);
  (void)cmd_IndexedAddParm(statusSyntax, "-process", CMD_LIST, CMD_OPTIONAL,
  (char *) cmd_explanation , STATUS_PROCESS_INDEX);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bos_s_add_status_long, &st);
  (void)cmd_IndexedAddParm(statusSyntax, "-long", CMD_FLAG, CMD_OPTIONAL, 
		(char *) cmd_explanation, STATUS_LONG_INDEX);
  free(cmd_explanation);

  bos_ui_SetUpNormalDefaultArgs(statusSyntax);
  bos_ui_SetUpNormalDefaultFlags(statusSyntax);
}

void bos_ui_SetUpStop()
{
  struct cmd_syndesc *	stopSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_stop, &st);
  stopSyntax = cmd_CreateSyntax("stop", bos_DoStop, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);

  

  cmd_explanation = dce_msg_get_msg(bos_s_add_stop_proc, &st);
  (void)cmd_IndexedAddParm(stopSyntax, "-process", CMD_LIST, CMD_REQUIRED,
  (char *) cmd_explanation , STOP_PROCESS_INDEX);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bos_s_add_stop_wait, &st);
  (void)cmd_IndexedAddParm(stopSyntax, "-wait", CMD_FLAG, CMD_OPTIONAL, 
		(char *) cmd_explanation, STOP_WAIT_INDEX);
  free(cmd_explanation);

  bos_ui_SetUpNormalDefaultArgs(stopSyntax);
  bos_ui_SetUpNormalDefaultFlags(stopSyntax);
}

void bos_ui_SetUpUnInstall()
{
  struct cmd_syndesc *	unInstallSyntax;


  cmd_explanation = dce_msg_get_msg(bos_s_create_uninstall, &st);
  unInstallSyntax = cmd_CreateSyntax("uninstall", bos_DoUnInstall, (char *)NULL,
  (char *) cmd_explanation );
  free(cmd_explanation);



  cmd_explanation = dce_msg_get_msg(bos_s_add_un_file, &st);
  (void)cmd_IndexedAddParm(unInstallSyntax, "-file", CMD_LIST, CMD_REQUIRED,
  (char *) cmd_explanation , UNINSTALL_FILE_INDEX);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bos_s_add_un_dir, &st);
  (void)cmd_IndexedAddParm(unInstallSyntax, "-dir", CMD_SINGLE, CMD_OPTIONAL,
  (char *) cmd_explanation , UNINSTALL_DESTDIR_INDEX);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bos_s_add_un_all, &st);
  (void)cmd_IndexedAddParm(unInstallSyntax, "-all", CMD_FLAG, CMD_OPTIONAL,
  (char *) cmd_explanation , UNINSTALL_ALL_INDEX);
  free(cmd_explanation);


  bos_ui_SetUpNormalDefaultArgs(unInstallSyntax);
  bos_ui_SetUpNormalDefaultFlags(unInstallSyntax);
}


void bos_ui_SetUpSyntax()
{
  bos_ui_SetUpAddKey();
  bos_ui_SetUpAddUser();
  bos_ui_SetUpCreate();
  bos_ui_SetUpDelete();
  bos_ui_SetUpGcKeys();
  bos_ui_SetUpGenKey();
  bos_ui_SetUpGetDates();
  bos_ui_SetUpGetLog();
  bos_ui_SetUpGetRestart();
  bos_ui_SetUpInstall();
  bos_ui_SetUpLsCell();
  bos_ui_SetUpLsKeys();
  bos_ui_SetUpLsUsers();
  bos_ui_SetUpPrune();
  bos_ui_SetUpRmKey();
  bos_ui_SetUpRmUser();
  bos_ui_SetUpRestart();
  bos_ui_SetUpSetAuth();
#if defined(DFS_BOS_DEBUG_BOSSERVER)
  bos_ui_SetUpSetDebug();
#endif /* defined(DFS_BOS_DEBUG_BOSSERVER) */
  bos_ui_SetUpSetRestart();
  bos_ui_SetUpShutdown();
  bos_ui_SetUpStart();
  bos_ui_SetUpStartup();
  bos_ui_SetUpStatus();
  bos_ui_SetUpStop();
  bos_ui_SetUpUnInstall();
}
