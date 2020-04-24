/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cm_userint.h,v $
 * Revision 1.1.9.1  1996/10/02  21:09:52  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:50:26  damon]
 *
 * Revision 1.1.4.1  1994/06/09  14:24:12  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:36:09  annie]
 * 
 * Revision 1.1.2.5  1993/01/28  18:36:19  shl
 * 	Transarc delta: jaffe-ot6442-cleanup-ioctls 1.5
 * 	  Selected comments:
 * 	    Cleanup the ioctl and pioctl interface to dfs.  Removed many obsolete
 * 	    interfaces.
 * 	    More cleanup, remove dead code and unused pioctls.
 * 	    After importing dfsbind changes, now have a complete change set.
 * 	[1993/01/27  21:34:27  shl]
 * 
 * Revision 1.1.2.4  1993/01/21  16:29:15  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  18:22:38  cjd]
 * 
 * Revision 1.1.2.3  1993/01/13  19:28:52  shl
 * 	Transarc delta: comer-ot5805-remove-debug-option-from-cm-command 1.1
 * 	  Selected comments:
 * 	    With ICL tracing, there is no longer any need for the debug options
 * 	    under the cm command.  This delta removes these options.  It also gets
 * 	    rid of the DFSLog file and all calls to the logging functions from
 * 	    dfsd.  The remaining AFSLOG calls in the kernel should be changed to
 * 	    use ICL.  If they are enabled now, they will turn into kernel printfs.
 * 	    Removed proto for DoDebug() and #defines used by SetUpDebug().
 * 	[1993/01/12  22:21:02  shl]
 * 
 * Revision 1.1.2.2  1992/11/24  20:40:37  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:36:09  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:50:11  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *	cm_userint.h -- header file containing constants and routine 
 * prototypes shared between cm.c and cm_userint.c
 *
 *
 *	Copyright (C) 1995, 1991 Transarc Corporation
 *	All rights reserved.
 *
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include <dcedfs/cmd.h>

/*** the offsets for the parameters to the operations ***/
/*** checkfilesets ***/
/* no args */

/*** exportdfs ***/
#define EXPORTDFS_EXPORTER_INDEX	0
#define EXPORTDFS_STATE_INDEX		1

/*** flush ***/
#define FLUSH_PATH_INDEX	0

/*** flushfileset ***/
#define FLUSHVOL_PATH_INDEX	0

/*** getcacheinfo ***/
/* no args */

/*** getdevok ***/
#define GETDEVOK_PATH_INDEX		0

/*** getsetuid ***/
#define GETSETUID_PATH_INDEX		0

/*** lscellinfo ***/
/* no args */

/*** setcachesize ***/
#define SETCACHESIZE_SIZE_INDEX		0
#define SETCACHESIZE_RESET_INDEX	1

/*** setdevok ***/
#define SETDEVOK_PATH_INDEX		0
#define SETDEVOK_STATE_INDEX		1

/*** setsetuid ***/
#define SETSETUID_PATH_INDEX		0
#define SETSETUID_STATE_INDEX		1

/*** statservers ***/
#define STATSERVERS_CELLNAME_INDEX	0
#define STATSERVERS_ALL_INDEX		1
#define STATSERVERS_FAST_INDEX		2

/*** sysname ***/
#define SYSNAME_NEWSYS_INDEX	0

/*** whereis ***/
#define WHEREIS_PATH_INDEX	0

/*** liststores ***/
/* no args */

/*** resetstores ***/
/* no args */

/*** getprefs ***/
#define GPREFS_PATH_INDEX	0
#define GPREFS_NUMERIC_INDEX	1
#define GPREFS_FLDB_INDEX	2

/*** setprefs ***/
#define SPREFS_SERVER_INDEX	0
#define SPREFS_PATH_INDEX	1
#define SPREFS_STDIN_INDEX	2
#define SPREFS_FLDB_INDEX	3

/*** getprotectlevels ***/
/* no args */

/*** setprotectlevels ***/
#define SETPROT_INITLOCAL_INDEX 0
#define SETPROT_MINLOCAL_INDEX 1
#define SETPROT_INITREMOTE_INDEX 2
#define SETPROT_MINREMOTE_INDEX 3

/* the operation that main needs to call to get things set up */
IMPORT void SetUpSyntax _TAKES((void));

/* the operations to which the setup routines need to refer */
IMPORT int DoCheckFilesets _TAKES((
				   struct cmd_syndesc *	aSyntax,
				   char *		aRock
				 ));
IMPORT int DoFlush _TAKES((
			   struct cmd_syndesc *	aSyntax,
			   char *		aRock
			 ));
IMPORT int DoResetStores _TAKES((
				 struct cmd_syndesc *as,
				 char *arock
			       ));
IMPORT int DoListStores _TAKES((
				struct cmd_syndesc *	as,
				char *			arock
			      ));
IMPORT int DoFlushFileset _TAKES((
				  struct cmd_syndesc *	aSyntax,
				  char *		aRock
				));
IMPORT int DoGetCacheInfo _TAKES((
				  struct cmd_syndesc *	aSyntax,
				  char *			aRock
				));
IMPORT int DoGetDevOk _TAKES((
			       struct cmd_syndesc *	aSyntax,
			       char *			aRock
			     ));
IMPORT int DoGetSetuid _TAKES((
			       struct cmd_syndesc *	aSyntax,
			       char *			aRock
			     ));
IMPORT int DoLsCells _TAKES((
			     struct cmd_syndesc *	aSyntax,
			     char *			aRock
			   ));
IMPORT int DoSetCacheSize _TAKES((
				  struct cmd_syndesc *	aSyntax,
				  char *			aRock
				));
IMPORT int DoSetDevOk _TAKES((
			      struct cmd_syndesc *	aSyntax,
			      char *			aRock
			    ));
IMPORT int DoSetSetuid _TAKES((
			       struct cmd_syndesc *	aSyntax,
			       char *			aRock
			     ));
IMPORT int DoStatServers _TAKES((
				 struct cmd_syndesc *	aSyntax,
				 char *			aRock
			       ));
IMPORT int DoSysname _TAKES((
			     struct cmd_syndesc *	aSyntax,
			     char *			aRock
			   ));
IMPORT int DoWhereis _TAKES((
			     struct cmd_syndesc *	aSyntax,
			     char *			aRock
			   ));
IMPORT int DoGetPrefs _TAKES((
				    struct cmd_syndesc *	aSyntax,
				    char *			aRock
				    ));
IMPORT int DoSetPrefs _TAKES((
				    struct cmd_syndesc *	aSyntax,
				    char *			aRock
				    ));
IMPORT int DoGetProtectLevels _TAKES((
				      struct cmd_syndesc *	aSyntax,
				      char *			aRock
				      ));
IMPORT int DoSetProtectLevels _TAKES((
				      struct cmd_syndesc *	aSyntax,
				      char *			aRock
				      ));
