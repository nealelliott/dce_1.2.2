/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cm_userint.c,v $
 * Revision 1.1.437.1  1996/10/02  21:09:50  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:50:25  damon]
 *
 * Revision 1.1.432.3  1994/07/13  22:23:13  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  20:53:51  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:04:02  mbs]
 * 
 * Revision 1.1.432.2  1994/06/09  14:24:11  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:36:08  annie]
 * 
 * Revision 1.1.432.1  1994/02/04  20:34:41  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:19:54  devsrc]
 * 
 * Revision 1.1.430.1  1993/12/07  17:37:07  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  17:49:14  jaffe]
 * 
 * Revision 1.1.3.5  1993/01/21  16:29:12  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  18:22:33  cjd]
 * 
 * Revision 1.1.3.4  1993/01/13  19:28:48  shl
 * 	Transarc delta: comer-ot5805-remove-debug-option-from-cm-command 1.1
 * 	  Selected comments:
 * 	    With ICL tracing, there is no longer any need for the debug options
 * 	    under the cm command.  This delta removes these options.  It also gets
 * 	    rid of the DFSLog file and all calls to the logging functions from
 * 	    dfsd.  The remaining AFSLOG calls in the kernel should be changed to
 * 	    use ICL.  If they are enabled now, they will turn into kernel printfs.
 * 	    Removed SetUpDebug().
 * 	Transarc delta: kazar-ot6038-zap-cm-debug-command 1.1
 * 	  Selected comments:
 * 	    get rid of "cm debug" command, as it is obsolete.
 * 	    remove "cm debug" from syntax table.
 * 	[1993/01/12  22:20:33  shl]
 * 
 * Revision 1.1.3.3  1992/11/24  20:40:27  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:36:03  bolinger]
 * 
 * Revision 1.1.3.2  1992/08/31  21:53:47  jaffe
 * 	Replace missing RCS ids
 * 	[1992/08/31  16:10:25  jaffe]
 * 
 * 	Transarc delta: kazar-ot4460-remove-exportdfs-option 1.1
 * 	  Selected comments:
 * 	    request made to ifdef out "cm exportdfs" command until NFS exporter ready
 * 	    ifdef exportdfs command out of syntax table
 * 	[1992/08/30  13:37:00  jaffe]
 * 
 * Revision 1.1  1992/01/19  02:50:14  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *	cm_userint.c -- routines that set-up and manipulate the cm commamd's
 * user interface
 *
 *	Copyright (C) 1995, 1991 Transarc Corporation
 *	All rights reserved.
 *
 */

#include <stdio.h>

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include <dcedfs/cmd.h>
#include <dcedfs/cmdAux.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/userInt/cm/cm_userint.c,v 1.1.437.1 1996/10/02 21:09:50 damon Exp $")

#include <cm_userint.h>

void SetUpCheckFilesets()
{
  struct cmd_syndesc *	checkFilesetsSyntax;

  checkFilesetsSyntax = cmd_CreateSyntax("checkfilesets", DoCheckFilesets, (char *)NULL,
					 "check fileset/name mappings");
}

void SetUpFlush()
{
  struct cmd_syndesc *	flushSyntax;

  flushSyntax = cmd_CreateSyntax("flush", DoFlush, (char *)NULL,
				 "flush file data and status information from cache");
  
  SetUpPathSwitch(flushSyntax, CMD_LIST, CMD_OPTIONAL);
}

void SetUpFlushFileset()
{
  struct cmd_syndesc *	flushFilesetSyntax;

  flushFilesetSyntax = cmd_CreateSyntax("flushfileset", DoFlushFileset, (char *)NULL,
				"flush all data in fileset (leaving status information)");
  
  SetUpPathSwitch(flushFilesetSyntax, CMD_LIST, CMD_OPTIONAL);
}

void SetUpGetCacheInfo()
{
  struct cmd_syndesc *	getCacheInfoSyntax;

  getCacheInfoSyntax = cmd_CreateSyntax("getcachesize", DoGetCacheInfo, (char *)NULL,
					"get cache usage information");
  
}

void SetUpGetDevOk()
{
  struct cmd_syndesc *	getDevOkSyntax;

  getDevOkSyntax = cmd_CreateSyntax("getdevok", DoGetDevOk, (char *)NULL,
				    "get fileset device file status");
  
  SetUpPathSwitch(getDevOkSyntax, CMD_LIST, CMD_OPTIONAL);
}

void SetUpGetSetuid()
{
  struct cmd_syndesc *	getSetuidSyntax;

  getSetuidSyntax = cmd_CreateSyntax("getsetuid", DoGetSetuid, (char *)NULL,
				     "get fileset setuid status");
  
  SetUpPathSwitch(getSetuidSyntax, CMD_LIST, CMD_OPTIONAL);
}

void SetUpLsCells()
{
  struct cmd_syndesc *	lsCellsSyntax;

  lsCellsSyntax = cmd_CreateSyntax("lscellinfo", DoLsCells, (char *)NULL,
				   "list cells contacted since workstation reboot");
}


void SetUpSetCacheSize()
{
  struct cmd_syndesc *	setCacheSizeSyntax;

  setCacheSizeSyntax = cmd_CreateSyntax("setcachesize", DoSetCacheSize, (char *)NULL,
					 "set cache size");

  (void)cmd_IndexedAddParm(setCacheSizeSyntax, "-size", CMD_SINGLE,
			   (CMD_REQUIRED | CMD_PARM_NUMERIC),
			   "kbytes", SETCACHESIZE_SIZE_INDEX);
  (void)cmd_IndexedAddParm(setCacheSizeSyntax, "-reset", CMD_FLAG, CMD_REQUIRED, "",
			   SETCACHESIZE_RESET_INDEX);
  (void)cmd_MakeAlternate(setCacheSizeSyntax, "-size", "-reset");
}

void SetUpSetDevOk()
{
  struct cmd_syndesc *	setDevOkSyntaxP;

  setDevOkSyntaxP = cmd_CreateSyntax("setdevok", DoSetDevOk, (char *)NULL,
				     "set device file status");

  SetUpPathSwitch(setDevOkSyntaxP, CMD_LIST, CMD_OPTIONAL);

  (void)cmd_IndexedAddParm(setDevOkSyntaxP, "-state", CMD_SINGLE,
		    (CMD_OPTIONAL | CMD_PARM_LITERAL), "on", SETDEVOK_STATE_INDEX);
  (void)cmd_AddParamHelp(setDevOkSyntaxP, "-state", "off", CMD_PARM_LITERAL);
}

void SetUpSetSetuid()
{
  struct cmd_syndesc *	setSetuidSyntax;

  setSetuidSyntax = cmd_CreateSyntax("setsetuid", DoSetSetuid, (char *)NULL,
				     "set fileset setuid status");
  
  SetUpPathSwitch(setSetuidSyntax, CMD_LIST, CMD_OPTIONAL);

  (void)cmd_IndexedAddParm(setSetuidSyntax, "-state", CMD_SINGLE,
		    (CMD_OPTIONAL | CMD_PARM_LITERAL), "on", SETSETUID_STATE_INDEX);
  (void)cmd_AddParamHelp(setSetuidSyntax, "-state", "off", CMD_PARM_LITERAL);
}

void SetUpStatServers()
{
  struct cmd_syndesc *	statServersSyntax;

  statServersSyntax = cmd_CreateSyntax("statservers", DoStatServers, (char *)NULL,
				       "check functioning servers");
  
  (void)cmd_IndexedAddParm(statServersSyntax, "-cell", CMD_SINGLE, CMD_OPTIONAL, "cellname",
			   STATSERVERS_CELLNAME_INDEX);
  (void)cmd_IndexedAddParm(statServersSyntax, "-all", CMD_FLAG, CMD_OPTIONAL, "",
			   STATSERVERS_ALL_INDEX);
  (void)cmd_MakeAlternate(statServersSyntax, "-cell", "-all");

  (void)cmd_IndexedAddParm(statServersSyntax, "-fast", CMD_FLAG, CMD_OPTIONAL, "",
			   STATSERVERS_FAST_INDEX);
}

void SetUpSysname()
{
  struct cmd_syndesc *	sysnameSyntax;

  sysnameSyntax = cmd_CreateSyntax("sysname", DoSysname, (char *)NULL,
				   "get/set sysname (i.e., @sys) value");

  (void)cmd_IndexedAddParm(sysnameSyntax, "-newsys", CMD_SINGLE, CMD_OPTIONAL, "sysname",
			   SYSNAME_NEWSYS_INDEX);
}

void SetUpWhereis()
{
  struct cmd_syndesc *	whereisSyntax;

  whereisSyntax = cmd_CreateSyntax("whereis", DoWhereis, (char *)NULL,
				   "list cell, server, and fileset on which file resides");

  SetUpPathSwitch(whereisSyntax, CMD_LIST, CMD_OPTIONAL);
}

void SetUpListStores()
{
    struct cmd_syndesc *tscp;

    tscp = cmd_CreateSyntax("lsstores", DoListStores, (char *) 0,
			    "list retrying stores");
}

void SetUpResetStores()
{
    struct cmd_syndesc *tscp;

    tscp = cmd_CreateSyntax("resetstores", DoResetStores, (char *)0,
			    "reset retrying stores");
}

void SetUpGetPrefs()
{
    struct cmd_syndesc *tscp;

    tscp = cmd_CreateSyntax("getpreferences", DoGetPrefs, (char *)NULL,
			    "get server preferences");

    (void)cmd_IndexedAddParm(tscp, "-path", CMD_SINGLE, CMD_OPTIONAL,
			     "filename", GPREFS_PATH_INDEX);
    (void)cmd_IndexedAddParm(tscp, "-numeric", CMD_FLAG, CMD_OPTIONAL,
			     "addresses only", GPREFS_NUMERIC_INDEX);
    (void)cmd_IndexedAddParm(tscp, "-fldb", CMD_FLAG, CMD_OPTIONAL,
			     "fldb servers only", GPREFS_FLDB_INDEX);
}

void SetUpSetPrefs()
{
    struct cmd_syndesc *tscp;

    tscp = cmd_CreateSyntax("setpreferences", DoSetPrefs, (char *)NULL,
			    "set server preferences");

    (void)cmd_IndexedAddParm(tscp, "-server", CMD_LIST, CMD_OPTIONAL,
			   "machine rank", SPREFS_SERVER_INDEX);
    (void)cmd_IndexedAddParm(tscp, "-path", CMD_SINGLE, CMD_OPTIONAL,
			     "filename", SPREFS_PATH_INDEX);
    (void)cmd_IndexedAddParm(tscp, "-stdin", CMD_FLAG, CMD_OPTIONAL,
			     "read machine/rank pairs from stdin",
			     SPREFS_STDIN_INDEX);
    (void)cmd_IndexedAddParm(tscp, "-fldb", CMD_FLAG, CMD_OPTIONAL,
			     "fldb servers only", SPREFS_FLDB_INDEX);
}

void SetUpGetProtectLevels()
{
    struct cmd_syndesc *tscp;

    tscp = cmd_CreateSyntax("getprotectlevels", DoGetProtectLevels, (char *)NULL,
			    "get protection levels");
}

void SetUpSetProtectLevels()
{
    struct cmd_syndesc *tscp;

    tscp = cmd_CreateSyntax("setprotectlevels", DoSetProtectLevels, (char *)NULL,
			    "set protection levels");

    (void)cmd_IndexedAddParm(tscp, "-initiallocalprotectlevel", CMD_SINGLE,
			     CMD_OPTIONAL, "level",
			     SETPROT_INITLOCAL_INDEX);
    (void)cmd_IndexedAddParm(tscp, "-minlocalprotectlevel", CMD_SINGLE,
			     CMD_OPTIONAL, "level",
			     SETPROT_MINLOCAL_INDEX);
    (void)cmd_IndexedAddParm(tscp, "-initialremoteprotectlevel", CMD_SINGLE,
			     CMD_OPTIONAL, "level",
			     SETPROT_INITREMOTE_INDEX);
    (void)cmd_IndexedAddParm(tscp, "-minremoteprotectlevel", CMD_SINGLE,
			     CMD_OPTIONAL, "level",
			     SETPROT_MINREMOTE_INDEX);
}

void SetUpSyntax()
{
  SetUpCheckFilesets();
  SetUpFlush();
  SetUpFlushFileset();
  SetUpGetCacheInfo();
  SetUpGetDevOk();
  SetUpGetSetuid();
  SetUpLsCells();
  SetUpSetCacheSize();
  SetUpSetDevOk();
  SetUpSetSetuid();
  SetUpStatServers();
  SetUpSysname();
  SetUpWhereis();
  SetUpResetStores();
  SetUpListStores();
  SetUpGetPrefs();
  SetUpSetPrefs();
  SetUpGetProtectLevels();
  SetUpSetProtectLevels();
}
