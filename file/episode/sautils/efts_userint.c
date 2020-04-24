/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: efts_userint.c,v $
 * Revision 1.1.62.1  1996/10/02  17:27:46  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:34:09  damon]
 *
 * Revision 1.1.57.3  1994/07/13  22:22:32  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  20:50:47  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:57:21  mbs]
 * 
 * Revision 1.1.57.2  1994/06/09  14:03:14  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:38:16  annie]
 * 
 * Revision 1.1.57.1  1994/02/04  20:15:13  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:11:18  devsrc]
 * 
 * Revision 1.1.55.1  1993/12/07  17:20:49  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  14:57:38  jaffe]
 * 
 * Revision 1.1.4.4  1993/01/21  18:23:16  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  14:13:04  zeliff]
 * 
 * Revision 1.1.4.3  1992/11/24  16:56:36  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:26:10  bolinger]
 * 
 * Revision 1.1.4.2  1992/08/31  20:19:51  jaffe
 * 	Fixed RCSID
 * 	[1992/08/30  14:23:41  jaffe]
 * 
 * 	Transarc delta: jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	  Selected comments:
 * 	    This revision of this delta contains changes made to episode/async by the OSF
 * 	    between dce1.0 and dce1.0.1b9.  Subsequent revisions will contain merges
 * 	    for other episode subdirectories.
 * 	    This revision of this delta contains changes made to episode/dir by the OSF
 * 	    This revision of this delta contains changes made to episode/lobguf by the OSF
 * 	    This revision of this delta contains changes made to episode/salvage by the OSF
 * 	    This revision of this delta contains changes made to episode/sautils by the OSF
 * 	    Merged in changes made by the OSF between dce1.0 and dce1.0.1b9.
 * 	    This revision of this delta contains changes made to episode/tools by the OSF
 * 	    This revision of this delta contains changes made to episode/utils by the OSF
 * 	    This revision of this delta contains changes made to episode/vnops by the OSF
 * 	    between dce1.0 and dce1.0.1b9.
 * 	    This revision fixes some problems in the logbuf layer that occurred after
 * 	    the merge.
 * 	    This delta adds anode/epi_errs.msf.
 * 	    This delta fixes problems in the logbuf layer.
 * 	    This revision fixes some problems in the vnode layer.
 * 	    Fix more vnode layer problems.
 * 	    Add files that were introduced after DCE 1.0 to the .klib Makefiles.
 * 	    This delta fixes various problems that caused undefined references
 * 	    while building the AIX kernel extension.
 * 	    This revision fixes random problems encountered under OSF/1.
 * 	    This revision fixes some problems encountered when compiling test_vnodeops
 * 	    on OSF/1.
 * 	    This revision includes the OSF's changes to epimount.
 * 	Transarc delta: jdp-ftutils 1.11
 * 	  Selected comments:
 * 	    Checkpoint to upgrade to a new release.
 * 	    Checkpoint to work on another delta.
 * 	    Checkpoint to fix vol_efsSwapIDs() (see OT #4671).
 * 	    Change arguments to the clone subcommand.
 * 	    Another checkpoint.
 * 	    Cleaned up setup routines.
 * 	    Rearrange link lines in utils/Makefile.
 * 	    Fix compilation problems on OSF/1.
 * 	    Add lsquota command to efts.
 * 	    Add efts_ui_SetupLsquota().
 * 	[1992/08/30  02:48:06  jaffe]
 * 
 * Revision 1.1.2.3  1992/05/13  22:00:15  jdp
 * 	Transarc delta: jdp-efts-fix-setquota-2683 1.1
 * 	  Files modified:
 * 	    sautils: efts_userint.c, efts_userint.h, newft.c
 * 	  Selected comments:
 * 	    This delta fixes the "setquota" subcommand of efts.
 * 	    Change "setquota" to dispatch efts_SetQuota() instead of efts_Clone()!
 * 	[1992/05/13  21:26:54  jdp]
 * 
 * Revision 1.1.2.2  1992/05/05  17:43:15  jdp
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	[1992/05/04  22:59:53  jdp]
 * 
 * Revision 1.1  1992/01/19  02:40:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * efts_userint.c -- user interface routines for fts
 *
 *
 * (C) Copyright 1991 Transarc Corporation
 * All Rights Reserved.
 */

#include <stdio.h>
#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/episode/sautils/efts_userint.c,v 1.1.62.1 1996/10/02 17:27:46 damon Exp $")

#include <dcedfs/cmdAux.h>
#include <efts_userint.h>

PRIVATE void efts_ui_SetUpNormalDefaults(aSyntax)
    struct cmd_syndesc *aSyntax;
{
    (void)cmd_Seek(aSyntax, COMMON_OFFSET);

    (void)cmd_AddParm(aSyntax, "-verbose", CMD_FLAG, CMD_OPTIONAL,"");
    (void)cmd_AddParm(aSyntax, "-quiet", CMD_FLAG, CMD_OPTIONAL, "");
    (void)cmd_AddParm(aSyntax, "-trace", CMD_FLAG, CMD_OPTIONAL, "");
}

PRIVATE void efts_ui_SetupAggrinfo()
{
    struct cmd_syndesc *syntax;
      
    syntax = cmd_CreateSyntax("aggrinfo", efts_Aggrinfo, 0,
			      "Print information about an aggregate");
    (void)cmd_IndexedAddParm(syntax, "-aggregate", CMD_SINGLE, CMD_REQUIRED,
			     "aggregate name",
			     AGGRINFO_AGGREGATE_OFFSET);

    efts_ui_SetUpNormalDefaults(syntax);
}

PRIVATE void efts_ui_SetupAttach()
{
    struct cmd_syndesc *syntax;

    syntax = cmd_CreateSyntax("attach", efts_Attach, 0,
			      "Add an aggregate to the registry");
    (void)cmd_IndexedAddParm(syntax, "-device", CMD_SINGLE, CMD_REQUIRED,
			     "device name",
			     ATTACH_DEVICE_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-name", CMD_SINGLE, CMD_OPTIONAL,
			     "aggregate name", ATTACH_NAME_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-id", CMD_SINGLE, CMD_OPTIONAL,
			     "aggregate id", ATTACH_ID_OFFSET);

    efts_ui_SetUpNormalDefaults(syntax);
}

PRIVATE void efts_ui_SetupClone()
{
    struct cmd_syndesc *syntax;
      
    syntax = cmd_CreateSyntax("clone", efts_Clone, 0,
			      "Clone a fileset");
    (void)cmd_IndexedAddParm(syntax, "-aggregate", CMD_SINGLE, CMD_REQUIRED,
			     "aggregate name", CLONE_AGGREGATE_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-srcid", CMD_SINGLE, CMD_OPTIONAL,
			     "source fileset id",			     
			     CLONE_SRC_ID_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-srcname",  CMD_SINGLE, CMD_OPTIONAL,
			     "source fileset name",
			     CLONE_SRC_NAME_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-destid", CMD_SINGLE, CMD_OPTIONAL,
			     "destination fileset id",
			     CLONE_DEST_ID_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-destname", CMD_SINGLE, CMD_OPTIONAL,
			     "destination fileset name",
			     CLONE_DEST_NAME_OFFSET);

    efts_ui_SetUpNormalDefaults(syntax);
}

PRIVATE void efts_ui_SetupClonesys()
{
    struct cmd_syndesc *syntax;
      
    syntax = cmd_CreateSyntax("clonesys", efts_Clonesys, 0,
			      "Clone a group of filesets");
    (void)cmd_IndexedAddParm(syntax, "-aggregate", CMD_SINGLE, 0,
			     "aggregate name",
			     CLONESYS_AGGREGATE_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-prefix", CMD_SINGLE, 0,
			     "string",
			     CLONESYS_PREFIX_OFFSET);

    efts_ui_SetUpNormalDefaults(syntax);
}

PRIVATE void efts_ui_SetupCreateft()
{
    struct cmd_syndesc *syntax;

    syntax = cmd_CreateSyntax("createft", efts_CreateFt, 0,
			      "Create a new fileset");
    (void)cmd_IndexedAddParm(syntax, "-aggregate", CMD_SINGLE, CMD_REQUIRED,
			     "aggregate name",
			     CREATEFT_AGGREGATE_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-id", CMD_SINGLE, CMD_OPTIONAL,
			     "fileset id",
			     CREATEFT_ID_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-name", CMD_SINGLE, CMD_OPTIONAL,
			     "fileset name",
			     CREATEFT_NAME_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-quota", CMD_SINGLE, CMD_OPTIONAL,
			     "quota in 1k blocks",
			     CREATEFT_QUOTA_OFFSET);
    efts_ui_SetUpNormalDefaults(syntax);
}

PRIVATE void efts_ui_SetupSetStatusft()
{
    struct cmd_syndesc *syntax;

    syntax = cmd_CreateSyntax("setstatusft", efts_SetStatusFt, 0,
			      "Set fileset status");
    (void)cmd_IndexedAddParm(syntax, "-aggregate", CMD_SINGLE, CMD_REQUIRED,
			     "aggregate name",
			     SETSTATUS_AGGREGATE_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-id", CMD_SINGLE, CMD_REQUIRED,
			     "fileset id",
			     SETSTATUS_ID_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-type", CMD_SINGLE, CMD_OPTIONAL,
			     "fileset type (RW, RO, or BK)",
			     SETSTATUS_TYPE_OFFSET);

    (void)cmd_IndexedAddParm(syntax, "-flag", CMD_SINGLE, CMD_OPTIONAL,
			     "flags ({no,}export, {on,off}line,delonsalvage,busy",
			     SETSTATUS_FLAG_OFFSET);

    efts_ui_SetUpNormalDefaults(syntax);
}

PRIVATE void efts_ui_SetupGetStatusft()
{
    struct cmd_syndesc *syntax;

    syntax = cmd_CreateSyntax("getstatusft", efts_GetStatusFt, 0,
			     "Get fileset status");
    (void)cmd_IndexedAddParm(syntax, "-aggregate", CMD_SINGLE, CMD_REQUIRED,
			     "aggregate name",
			     GETSTATUS_AGGREGATE_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-id", CMD_SINGLE, CMD_OPTIONAL,
			     "fileset id",
			     GETSTATUS_ID_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-name", CMD_SINGLE, CMD_OPTIONAL,
			     "fileset name",
			     GETSTATUS_NAME_OFFSET);
    efts_ui_SetUpNormalDefaults(syntax);
}

PRIVATE void efts_ui_SetupDeleteft()
{
    struct cmd_syndesc *syntax;

    syntax = cmd_CreateSyntax("deleteft", efts_DeleteFt, 0,
			      "Delete an existing fileset");
    (void)cmd_IndexedAddParm(syntax, "-aggregate", CMD_SINGLE, CMD_REQUIRED,
			     "aggregate name",
			     DELETEFT_AGGREGATE_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-id", CMD_SINGLE, CMD_OPTIONAL,
			     "fileset id",
			     DELETEFT_ID_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-name", CMD_SINGLE, CMD_OPTIONAL,
			     "fileset name",
			     DELETEFT_NAME_OFFSET);

    efts_ui_SetUpNormalDefaults(syntax);
}

PRIVATE void efts_ui_SetupDetach()
{
    struct cmd_syndesc *syntax;

    syntax = cmd_CreateSyntax("detach", efts_Detach, 0,
			      "Remove an aggregate from the registry");
    (void)cmd_IndexedAddParm(syntax, "-aggregate", CMD_SINGLE, CMD_REQUIRED,
			     "aggregate name",
			     DETACH_AGGREGATE_OFFSET);

    efts_ui_SetUpNormalDefaults(syntax);
}

PRIVATE void efts_ui_SetupDump()
{
    struct cmd_syndesc *syntax;

    syntax = cmd_CreateSyntax("dump", efts_Dump, 0,
			      "Dump contents of fileset");
    (void)cmd_IndexedAddParm(syntax, "-aggregate", CMD_SINGLE, CMD_REQUIRED,
			     "aggregate name",
			     DUMP_AGGREGATE_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-id", CMD_SINGLE, CMD_OPTIONAL,
			     "fileset id",
			     DUMP_ID_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-name", CMD_SINGLE, CMD_OPTIONAL,
			     "fileset name",
			     DUMP_NAME_OFFSET);

    (void)cmd_IndexedAddParm(syntax, "-time", CMD_SINGLE, CMD_REQUIRED,
			     "date",
			     DUMP_TIME_OFFSET);
    (void)cmd_AddParamHelp(syntax, "-time", "0", CMD_PARM_LITERAL);
    (void)cmd_IndexedAddParm(syntax, "-version", CMD_SINGLE, CMD_REQUIRED,
			     "number",
			     DUMP_VERSION_OFFSET);
    (void)cmd_MakeAlternate(syntax, "-time", "-version");
    (void)cmd_IndexedAddParm(syntax, "-file", CMD_SINGLE, CMD_OPTIONAL,
			      "filename",
			      DUMP_FILE_OFFSET);

    efts_ui_SetUpNormalDefaults(syntax);
}

PRIVATE void efts_ui_SetupLsaggr()
{
    struct cmd_syndesc *lsaggrSyntax;

    lsaggrSyntax = cmd_CreateSyntax("lsaggr", efts_Lsaggr, 0,
				    "List aggregates");

    efts_ui_SetUpNormalDefaults(lsaggrSyntax);
}

PRIVATE void efts_ui_SetupLsft()
{
    struct cmd_syndesc *syntax;

    syntax = cmd_CreateSyntax("lsft", efts_Lsft, 0,
			      "List filesets on an aggregate");
    (void)cmd_IndexedAddParm(syntax, "-aggregate", CMD_SINGLE, CMD_REQUIRED,
			     "aggregate name",
			     LSFT_AGGREGATE_OFFSET);

    efts_ui_SetUpNormalDefaults(syntax);
}

PRIVATE void efts_ui_SetupLsquota()
{
    struct cmd_syndesc *syntax;
      
    syntax = cmd_CreateSyntax("lsquota", efts_Lsquota, 0,
			      "List fileset quota");
    (void)cmd_IndexedAddParm(syntax, "-aggregate", CMD_SINGLE, CMD_REQUIRED,
			     "aggregate name",
			     LSQUOTA_AGGREGATE_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-id", CMD_SINGLE, CMD_OPTIONAL,
			     "fileset id",
			     LSQUOTA_ID_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-name", CMD_SINGLE, CMD_OPTIONAL,
			     "fileset name",
			     LSQUOTA_NAME_OFFSET);

    efts_ui_SetUpNormalDefaults(syntax);
}

PRIVATE void efts_ui_SetupReclone()
{
    struct cmd_syndesc *syntax;
      
    syntax = cmd_CreateSyntax("reclone", efts_Reclone, 0,
			      "Reclone a fileset");
    (void)cmd_IndexedAddParm(syntax, "-aggregate", CMD_SINGLE, CMD_REQUIRED,
			     "aggregate name",
			     RECLONE_AGGREGATE_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-id", CMD_SINGLE, CMD_OPTIONAL,
			     "fileset id",
			     RECLONE_ID_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-name", CMD_SINGLE, CMD_OPTIONAL,
			     "fileset name",
			     RECLONE_NAME_OFFSET);
    efts_ui_SetUpNormalDefaults(syntax);
}

PRIVATE void efts_ui_SetupRenameft()
{
    register struct cmd_syndesc *syntax;
      
    syntax = cmd_CreateSyntax("renameft", efts_Renameft, 0,
				"Rename a fileset");
    (void)cmd_IndexedAddParm(syntax, "-aggregate", CMD_SINGLE, 0,
			     "aggregate name",
			     RENAMEFT_AGGREGATE_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-oldfsid", CMD_SINGLE, CMD_OPTIONAL,
			     "old fileset id",
			     RENAMEFT_OLD_FTID_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-oldfsname", CMD_SINGLE, CMD_OPTIONAL,
			     "old fileset name",
			     RENAMEFT_OLD_FTNAME_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-newfsid", CMD_SINGLE, CMD_OPTIONAL,
			     "new fileset id",
			     RENAMEFT_NEW_FTID_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-newfsname", CMD_SINGLE,
			     CMD_OPTIONAL, "new fileset name",
			     RENAMEFT_NEW_FTNAME_OFFSET);

    efts_ui_SetUpNormalDefaults(syntax);
}

PRIVATE void efts_ui_SetupRestore()
{
    struct cmd_syndesc *syntax;

    syntax = cmd_CreateSyntax("restore", efts_Restore, 0,
			      "Restore a fileset");
    (void)cmd_IndexedAddParm(syntax, "-aggregate", CMD_SINGLE, CMD_REQUIRED,
			     "aggregate name",
			     RESTORE_AGGREGATE_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-id", CMD_SINGLE, CMD_OPTIONAL,
			     "fileset id",
			     RESTORE_ID_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-name", CMD_SINGLE, CMD_OPTIONAL,
			     "fileset name",
			     RESTORE_NAME_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-file", CMD_SINGLE, CMD_OPTIONAL,
			      "filename",
			      RESTORE_FILE_OFFSET);
    efts_ui_SetUpNormalDefaults(syntax);
}

PRIVATE void efts_ui_SetupSetquota()
{
    struct cmd_syndesc *syntax;
      
    syntax = cmd_CreateSyntax("setquota", efts_SetQuota, 0,
			      "Set fileset quota");
    (void)cmd_IndexedAddParm(syntax, "-aggregate", CMD_SINGLE, CMD_REQUIRED,
			     "aggregate name",
			     SETQUOTA_AGGREGATE_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-id", CMD_SINGLE, CMD_OPTIONAL,
			     "fileset id",
			     SETQUOTA_ID_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-fsname", CMD_SINGLE, CMD_OPTIONAL,
			     "fileset name",
			     SETQUOTA_NAME_OFFSET);
    (void)cmd_IndexedAddParm(syntax, "-quota",CMD_SINGLE, CMD_REQUIRED,
			     "quota in 1K blocks",
			     SETQUOTA_QUOTA_OFFSET);

    efts_ui_SetUpNormalDefaults(syntax);
}

void efts_ui_SetUpSyntax()
{
    efts_ui_SetupAggrinfo();
    efts_ui_SetupAttach();
    efts_ui_SetupClone();
    efts_ui_SetupClonesys();
    efts_ui_SetupCreateft();
    efts_ui_SetupDeleteft();
    efts_ui_SetupDetach();
    efts_ui_SetupDump();
    efts_ui_SetupGetStatusft();
    efts_ui_SetupLsaggr();
    efts_ui_SetupLsft();
    efts_ui_SetupLsquota();
    efts_ui_SetupReclone();
    efts_ui_SetupRenameft();
    efts_ui_SetupRestore();
    efts_ui_SetupSetquota();
    efts_ui_SetupSetStatusft();
}
