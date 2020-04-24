/*
 *	Copyright (C) 1992 1993 1994 Transarc Corporation
 *	All rights reserved.
 */
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 *	tkm_userint.c -- routines that set-up and manipulate the tkm control
 *	commamd's user interface.
 *
 */

#include <stdio.h>

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include <dcedfs/cmd.h>
#include <dcedfs/cmdAux.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/tkm/RCS/tkm_userint.c,v 1.9 1994/11/07 17:13:55 cfe Exp $")

#include <tkm_userint.h>

void tkm_SetUpGetExp()
{
  struct cmd_syndesc *	s;

  s = cmd_CreateSyntax("getexpiration", tkm_DoGetExpirationInterval,
		       (char *)NULL,
		       "get current system token expiration interval");
}

void tkm_SetUpSetExp()
{
  struct cmd_syndesc *	s;

  s = cmd_CreateSyntax("setexpiration", tkm_DoSetExpirationInterval,
		       (char *)NULL,
		       "set current system token expiration interval");
  (void)cmd_IndexedAddParm(s, "-expiration", CMD_SINGLE, CMD_REQUIRED,
			   "seconds", SET_EXP_NEW_EXP_INDEX);
}

void tkm_SetUpGetQuota()
{
  struct cmd_syndesc *	s;

  s = cmd_CreateSyntax("getquota", tkm_DoGetTokenQuota, (char *)NULL,
				"get current system token quota");
}

void tkm_SetUpSetQuota()
{
  struct cmd_syndesc *	s;

  s = cmd_CreateSyntax("setquota", tkm_DoSetTokenQuota, (char *)NULL,
				"set current system token quota");
  (void)cmd_IndexedAddParm(s, "-quota", CMD_SINGLE, CMD_REQUIRED,
			   "number tokens", SET_MAXTOKENS_INDEX);
}

void tkm_SetUpGetMinTokens()
{
  struct cmd_syndesc *	s;

  s = cmd_CreateSyntax("getmintokens", tkm_DoGetMinTokens, (char *)NULL,
				"get minimum number of free tokens");
}

void tkm_SetUpSetMinTokens()
{
  struct cmd_syndesc *	s;

  s = cmd_CreateSyntax("setmintokens", tkm_DoSetMinTokens, (char *)NULL,
				"set minimum number of free tokens");
  (void)cmd_IndexedAddParm(s, "-min", CMD_SINGLE, CMD_REQUIRED,
			   "number tokens", SET_MINTOKENS_INDEX);
}
void tkm_SetUpGetMaxFiles()
{
  struct cmd_syndesc *	s;

  s = cmd_CreateSyntax("getmaxfiles", tkm_DoGetMaxFiles, (char *)NULL,
				"get maximum number of active files");
}

void tkm_SetUpSetMaxFiles()
{
  struct cmd_syndesc *	s;

  s = cmd_CreateSyntax("setmaxfiles", tkm_DoSetMaxFiles, (char *)NULL,
				"set maximum number of active files");
  (void)cmd_IndexedAddParm(s, "-max", CMD_SINGLE, CMD_REQUIRED,
			   "number files", SET_MAXFILES_INDEX);
}

void tkm_SetUpGetMaxVols()
{
  struct cmd_syndesc *	s;

  s = cmd_CreateSyntax("getmaxvols", tkm_DoGetMaxVols, (char *)NULL,
				"get maximum number of active filesets");
}

void tkm_SetUpSetMaxVols()
{
  struct cmd_syndesc *	s;

  s = cmd_CreateSyntax("setmaxvols", tkm_DoSetMaxVols, (char *)NULL,
				"set maximum number of active filesets");
  (void)cmd_IndexedAddParm(s, "-max", CMD_SINGLE, CMD_REQUIRED,
			   "number filesets", SET_MAXVOLS_INDEX);
}

void tkm_SetUpSyntax()
{
  tkm_SetUpGetExp();
  tkm_SetUpSetExp();
  tkm_SetUpGetQuota();
  tkm_SetUpSetQuota();
  tkm_SetUpGetMinTokens();
  tkm_SetUpSetMinTokens();
  tkm_SetUpGetMaxFiles();
  tkm_SetUpSetMaxFiles();
  tkm_SetUpGetMaxVols();
  tkm_SetUpSetMaxVols();
}
