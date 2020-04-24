/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: efts_userint.h,v $
 * Revision 1.1.63.1  1996/10/02  17:27:48  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:34:10  damon]
 *
 * Revision 1.1.58.2  1994/06/09  14:03:16  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:38:18  annie]
 * 
 * Revision 1.1.58.1  1994/02/04  20:15:15  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:11:19  devsrc]
 * 
 * Revision 1.1.56.1  1993/12/07  17:20:50  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  14:58:03  jaffe]
 * 
 * Revision 1.1.4.4  1993/01/21  18:23:19  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  14:13:08  zeliff]
 * 
 * Revision 1.1.4.3  1992/11/24  16:56:40  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:26:16  bolinger]
 * 
 * Revision 1.1.4.2  1992/08/31  20:20:10  jaffe
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
 * 	    Added/renamed constants for argument offsets.
 * 	    Rearrange link lines in utils/Makefile.
 * 	    Fix compilation problems on OSF/1.
 * 	    Add lsquota command to efts.
 * 	    Add LSQUOTA_* defines.
 * 	[1992/08/30  02:48:28  jaffe]
 * 
 * Revision 1.1.2.3  1992/05/13  22:00:20  jdp
 * 	Transarc delta: jdp-efts-fix-setquota-2683 1.1
 * 	  Files modified:
 * 	    sautils: efts_userint.c, efts_userint.h, newft.c
 * 	  Selected comments:
 * 	    This delta fixes the "setquota" subcommand of efts.
 * 	    Fixed declaration of efts_SetQuota().
 * 	[1992/05/13  21:27:14  jdp]
 * 
 * Revision 1.1.2.2  1992/05/05  17:43:23  jdp
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	[1992/05/04  23:01:05  jdp]
 * 
 * Revision 1.1  1992/01/19  02:40:23  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * efts_userint.h - constants used by the efts user interface.
 *
 *
 * (C) Copyright 1991 Transarc Corporation
 * All Rights Reserved
 */

#ifndef TRANSARC_EFTS_USERINT_H
#define TRANSARC_EFTS_USERINT_H

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/cmd.h>
#include <dcedfs/icl.h>

#define COMMON_OFFSET 12
#define VERBOSE_OFFSET (COMMON_OFFSET+0)
#define QUIET_OFFSET (COMMON_OFFSET+1)
#define DUMPTRACELOG_OFFSET (COMMON_OFFSET+2)

/* param offsets by command */

/**** aggrinfo ****/
#define AGGRINFO_AGGREGATE_OFFSET 0

/**** attach ****/
#define ATTACH_DEVICE_OFFSET 0
#define ATTACH_NAME_OFFSET 1
#define ATTACH_ID_OFFSET 2

/**** clone ****/
#define CLONE_AGGREGATE_OFFSET 0
#define CLONE_SRC_ID_OFFSET 1
#define CLONE_SRC_NAME_OFFSET 2
#define CLONE_DEST_ID_OFFSET 3
#define CLONE_DEST_NAME_OFFSET 4

/**** clonesys ****/
#define CLONESYS_AGGREGATE_OFFSET 0
#define CLONESYS_PREFIX_OFFSET 1

/**** createft ****/
#define CREATEFT_AGGREGATE_OFFSET 0
#define CREATEFT_ID_OFFSET 1
#define CREATEFT_NAME_OFFSET 2
#define CREATEFT_QUOTA_OFFSET 3

/**** setstatusft ****/
#define SETSTATUS_AGGREGATE_OFFSET 0
#define SETSTATUS_ID_OFFSET 1
#define SETSTATUS_TYPE_OFFSET 2
#define SETSTATUS_FLAG_OFFSET 3

/**** getstatusft ****/
#define GETSTATUS_AGGREGATE_OFFSET 0
#define GETSTATUS_ID_OFFSET 1
#define GETSTATUS_NAME_OFFSET 2

/**** deleteft ****/
#define DELETEFT_AGGREGATE_OFFSET 0
#define DELETEFT_ID_OFFSET 1
#define DELETEFT_NAME_OFFSET 2

/**** detach ****/
#define DETACH_AGGREGATE_OFFSET 0

/**** dump ****/
#define DUMP_AGGREGATE_OFFSET 0
#define DUMP_ID_OFFSET 1
#define DUMP_NAME_OFFSET 2
#define DUMP_TIME_OFFSET 3
#define DUMP_VERSION_OFFSET 4
#define DUMP_FILE_OFFSET 5

/**** lsaggr ****/
/* NO arguments */

/**** lsft ****/
#define LSFT_AGGREGATE_OFFSET 0

/**** lsquota ****/
#define LSQUOTA_AGGREGATE_OFFSET 0
#define LSQUOTA_ID_OFFSET 1
#define LSQUOTA_NAME_OFFSET 2

/**** reclone ****/
#define RECLONE_AGGREGATE_OFFSET 0
#define RECLONE_ID_OFFSET 1
#define RECLONE_NAME_OFFSET 2

/**** renameft ****/
#define RENAMEFT_AGGREGATE_OFFSET 0
#define RENAMEFT_OLD_FTID_OFFSET 1
#define RENAMEFT_OLD_FTNAME_OFFSET 2
#define RENAMEFT_NEW_FTID_OFFSET 3
#define RENAMEFT_NEW_FTNAME_OFFSET 3

/**** restore ****/
#define RESTORE_AGGREGATE_OFFSET 0
#define RESTORE_ID_OFFSET 1
#define RESTORE_NAME_OFFSET 2
#define RESTORE_FILE_OFFSET 3

/**** setquota ****/
#define SETQUOTA_AGGREGATE_OFFSET 0
#define SETQUOTA_ID_OFFSET 1
#define SETQUOTA_NAME_OFFSET 2
#define SETQUOTA_QUOTA_OFFSET 3


/*
 * Declarations of the command functions
 */
IMPORT int efts_Aggrinfo _TAKES((struct cmd_syndesc *, char *));
IMPORT int efts_Attach _TAKES((struct cmd_syndesc *, char *));
IMPORT int efts_Clone _TAKES((struct cmd_syndesc *, char *));
IMPORT int efts_Clonesys _TAKES((struct cmd_syndesc *, char *));
IMPORT int efts_CreateFt _TAKES((struct cmd_syndesc *, char *));
IMPORT int efts_DeleteFt _TAKES((struct cmd_syndesc *, char *));
IMPORT int efts_Detach _TAKES((struct cmd_syndesc *, char *));
IMPORT int efts_Dump _TAKES((struct cmd_syndesc *, char *));
IMPORT int efts_GetStatusFt _TAKES((struct cmd_syndesc *, char *));
IMPORT int efts_Lsaggr _TAKES((struct cmd_syndesc *, char *));
IMPORT int efts_Lsft _TAKES((struct cmd_syndesc *, char *));
IMPORT int efts_Lsquota _TAKES((struct cmd_syndesc *, char *));
IMPORT int efts_Reclone _TAKES((struct cmd_syndesc *, char *));
IMPORT int efts_Renameft _TAKES((struct cmd_syndesc *, char *));
IMPORT int efts_Restore _TAKES((struct cmd_syndesc *, char *));
IMPORT int efts_SetQuota _TAKES((struct cmd_syndesc *, char *));
IMPORT int efts_SetStatusFt _TAKES((struct cmd_syndesc *, char *));

EXPORT void efts_ui_SetupSyntax _TAKES((void));

#endif /* TRANSARC_EFTS_USERINT_H */


