/*-
 * @TRANSARC_COPYRIGHT@
 *
 * $TALog: interactive_test.h,v $
 * Revision 1.1  1994/07/08  21:53:57  bab
 * Adding program to test the interactive interface, based on dceback.  This
 * is a check-point version of this delta during merge checking/debugging.
 *
 * Declarations/definitions used in interactive demo/test program.
 * [added by delta bab-tools-cmd-add-interactive, r1.2]
 *
 * $EndTALog$
 */

/*
 *	interactive_test.h -- header file containing constants
 *
 *
 *	Copyright (C) 1994 Transarc Corporation
 *	All rights reserved.
 *
 */

#include <stdio.h>

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include <dcedfs/cmd.h>
#include <dcedfs/cmdAux.h>

/* the offset declarations */

/* we can use a common set of indices for this simple suite */
/* we leave source file & destfile distinct, just in case..... */
#define HOST_INDEX		0
#define SOURCEFILE_INDEX	1
#define DESTFILE_INDEX		2
#define STDIN_INDEX		3
#define STDOUT_INDEX		4
#define COMP_INDEX		5
#define DESTDIR_INDEX		6
#define ALLOWNOENCRYPT_INDEX	7

#define DFSTYPE_ADMIN_STRING	"admin"
#define DFSTYPE_BAKDB_STRING	"bakdb"
#define DFSTYPE_CONFIG_STRING	"config"
#define DFSTYPE_DFSTAB_STRING	"dfstab"
#define DFSTYPE_FLDB_STRING	"fldb"

#define SECTYPE_COMMON_STRING	"common"
#define SECTYPE_MASTER_STRING	"master"

/* declaration of the set up routine */
IMPORT void dceback_ui_SetUpSyntax _TAKES((void));

/* declarations of the routines that do the real work */
IMPORT int dceback_DoBackupCDS _TAKES((
				    struct cmd_syndesc *	aSyntaxP,
				    char *			aRockP
				  ));

IMPORT int dceback_DoBackupDFS _TAKES((
				    struct cmd_syndesc *	aSyntaxP,
				    char *			aRockP
				  ));
IMPORT int dceback_DoBackupMisc _TAKES((
				    struct cmd_syndesc *	aSyntaxP,
				    char *			aRockP
				  ));
IMPORT int dceback_DoBackupSecurity _TAKES((
				    struct cmd_syndesc *	aSyntaxP,
				    char *			aRockP
				  ));
IMPORT int dceback_DoRestoreCDS _TAKES((
				    struct cmd_syndesc *	aSyntaxP,
				    char *			aRockP
				  ));
IMPORT int dceback_DoRestoreDFS _TAKES((
				    struct cmd_syndesc *	aSyntaxP,
				    char *			aRockP
				  ));
IMPORT int dceback_DoRestoreMisc _TAKES((
				    struct cmd_syndesc *	aSyntaxP,
				    char *			aRockP
				  ));
IMPORT int dceback_DoRestoreSecurity _TAKES((
				    struct cmd_syndesc *	aSyntaxP,
				    char *			aRockP
				  ));
