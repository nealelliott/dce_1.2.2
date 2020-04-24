/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bos_userint.h,v $
 * Revision 1.1.12.1  1996/10/02  21:06:20  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:50:21  damon]
 *
 * Revision 1.1.7.1  1994/06/09  14:24:02  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:35:58  annie]
 * 
 * Revision 1.1.4.5  1993/01/21  16:28:53  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  18:22:07  cjd]
 * 
 * Revision 1.1.4.4  1993/01/13  19:28:35  shl
 * 	Transarc delta: jaffe-cleanup-2.3-compiler-warnings 1.8
 * 	  Selected comments:
 * 	    Fix compiler warnings found when moving to OSF/1 1.1.1.
 * 	    add * / to avoid an embeded / * in comment.
 * 	    Remove the function dacl_SetOwnerControl.  It is not used.
 * 	[1993/01/12  22:18:55  shl]
 * 
 * Revision 1.1.4.3  1992/10/27  21:52:18  jaffe
 * 	Transarc delta: bab-ot5471-bbos-uninstall-all 1.2
 * 	  Selected comments:
 * 	    The bos command and bosserver will now support a -all flag for
 * 	    removing all versions of the specified file.
 * 	    ot 5471
 * 	    Added -all flag to the user interface.
 * 	    Cosmetic cleanup.
 * 	Transarc delta: bab-ot5475-bbos-dacl-clearlist 1.2
 * 	  Selected comments:
 * 	    Added support for a -clearlist option to the bos command, without
 * 	    actually activating it inthe user interface.
 * 	    ot 5475
 * 	    Added support for -clearlist option to rmadmin.
 * 	    Needed to handle cases in which clearlist may have been specified
 * 	    alone or with more than one user or group.
 * 	[1992/10/27  15:29:14  jaffe]
 * 
 * Revision 1.1.4.2  1992/08/31  21:53:18  jaffe
 * 	Transarc delta: bab-ot4755-bbos-remove-exec 1.1
 * 	  Selected comments:
 * 	    Removed bosserver exec funcitionality and the bos command that invokes it.
 * 	    ot 4755
 * 	    See above.
 * 	[1992/08/30  13:34:15  jaffe]
 * 
 * Revision 1.1.2.2  1992/04/14  04:11:42  mason
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
 * 	[1992/04/13  18:07:32  mason]
 * 
 * Revision 1.1  1992/01/19  02:50:01  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *	bos_userint.h -- header file containing constants and routine prototypes shared
 * between bos.c and bos_userint.c
 *
 *
 *	Copyright (C) 1991 Transarc Corporation
 *	All rights reserved.
 *
 */

/* a generally useful pair of macros */
#define BOSSVR_BITS_PER_CHAR	8
#define BOSSVR_BITSOF(type)	(sizeof(type) * BOSSVR_BITS_PER_CHAR)


/* first, the offsets for the common parameters */
#define BOS_SERVER_INDEX	0
#define BOS_NORMAL_OFFSET	12
#define BOS_CELL_INDEX		(BOS_NORMAL_OFFSET + 0)
#define BOS_LONG_INDEX		(BOS_NORMAL_OFFSET + 1)
#define BOS_NOAUTH_INDEX	(BOS_NORMAL_OFFSET + 2)
#define BOS_LOCAL_INDEX		(BOS_NORMAL_OFFSET + 3)

/**** and, now, the offsets on a per-routine basis ****/

/**** addkey ****/
#define ADDKEY_KVNO_INDEX	1
#define ADDKEY_PASSWORD_INDEX	2
#define ADDKEY_PRINCIPAL_INDEX	3
#define ADDKEY_LOCALONLY_INDEX	4

/**** adduser ****/
#define ADDUSER_FILENAME_INDEX		1
#define ADDUSER_USERS_INDEX		2
#define ADDUSER_GROUPS_INDEX		3
#define ADDUSER_CREATEFLAG_INDEX	4

/**** create ****/
#define CREATE_PROCESS_INDEX	1
#define CREATE_TYPE_INDEX	2
#define CREATE_CMD_INDEX	3

/**** delete ****/
#define DELETE_PROCESS_INDEX	1

/**** gckeys ****/
#define GCKEYS_PRINCIPAL_INDEX	1

/**** genkey ****/
#define GENKEY_KVNO_INDEX	1
#define GENKEY_PRINCIPAL_INDEX	2

/**** getdates ****/
#define GETDATES_FILE_INDEX	1
#define GETDATES_DESTDIR_INDEX	2

/**** getlog ****/
#define GETLOG_FILE_INDEX	1

/**** getrestart ****/
/* (only uses the default parameters) */

/**** install ****/
#define INSTALL_FILE_INDEX	1
#define INSTALL_DESTDIR_INDEX	2

/**** kill ****/
/* (only uses the default parameters) */

/**** lskeys ****/
#define LSKEYS_PRINCIPAL_INDEX	1

/**** lsusers ****/
#define LSUSERS_FILENAME_INDEX	1

/**** prune ****/
#define PRUNE_BAK_INDEX		1
#define PRUNE_OLD_INDEX		2
#define PRUNE_CORE_INDEX	3
#define PRUNE_ALL_INDEX		4

/**** rmkey ****/
#define RMKEY_KVNO_INDEX	1
#define RMKEY_PRINCIPAL_INDEX	2

/**** rmuser ****/
#define RMUSER_FILENAME_INDEX	1
#define RMUSER_USER_INDEX	2
#define RMUSER_GROUP_INDEX	3
#define RMUSER_REMOVEFLAG_INDEX	4
#define RMUSER_CLEARLIST_INDEX	5

/**** restart ****/
#define RESTART_BOS_INDEX	1
#define RESTART_PROCESS_INDEX	2

/**** setauth ****/
#define SETAUTH_AUTHSTRING_INDEX	1

/**** setdebug ****/
#if defined(DFS_BOS_DEBUG_BOSSERVER)
#define SETDEBUG_FLAG_INDEX	1
#endif /* defined(DFS_BOS_DEBUG_BOSSERVER) */

/**** setrestart ****/
#define SETRESTART_ALL_INDEX	1
#define SETRESTART_BINARY_INDEX	2

/**** shutdown ****/
#define SHUTDOWN_PROCESS_INDEX	1
#define SHUTDOWN_WAIT_INDEX	2

/**** start ****/
#define START_PROCESS_INDEX	1

/**** startup ****/
#define STARTUP_PROCESS_INDEX	1

/**** status ****/
#define STATUS_PROCESS_INDEX	1
#define STATUS_LONG_INDEX	2

/**** stop ****/
#define STOP_PROCESS_INDEX	1
#define STOP_WAIT_INDEX		2

/**** uninstall ****/
#define UNINSTALL_FILE_INDEX	1
#define UNINSTALL_DESTDIR_INDEX	2
#define UNINSTALL_ALL_INDEX	3


/* declarations of routines that do the actual work */
IMPORT int bos_DoAddKey _TAKES((
				struct cmd_syndesc *	aSyntax,
				char *			aRock
			      ));
IMPORT int bos_DoAddUser _TAKES((
				 struct cmd_syndesc *	aSyntax,
				 char *			aRock
			       ));
IMPORT int bos_DoCreate _TAKES((
				struct cmd_syndesc *	aSyntax,
				char *			aRock
			      ));
IMPORT int bos_DoDelete _TAKES((
				struct cmd_syndesc *	aSyntax,
				char *			aRock
			      ));
IMPORT int bos_DoGcKeys _TAKES((
				struct cmd_syndesc *	aSyntax,
				char *			aRock
			      ));
IMPORT int bos_DoGenKey _TAKES((
				struct cmd_syndesc *	aSyntax,
				char *			aRock
			      ));
IMPORT int bos_DoGetDates _TAKES((
				  struct cmd_syndesc *	aSyntax,
				  char *		aRock
				));
IMPORT int bos_DoGetLog _TAKES((
				struct cmd_syndesc *	aSyntax,
				char *			aRock
			      ));
IMPORT int bos_DoGetRestart _TAKES((
				    struct cmd_syndesc *	aSyntax,
				    char *			aRock
				  ));
IMPORT int bos_DoInstall _TAKES((
				 struct cmd_syndesc *	aSyntax,
				 char *			aRock
			       ));
IMPORT int bos_DoKill _TAKES((
			      struct cmd_syndesc *	aSyntax,
			      char *			aRock
			    ));
IMPORT int bos_DoLsCell _TAKES((
				 struct cmd_syndesc *	aSyntax,
				 char *			aRock
			       ));
IMPORT int bos_DoLsKeys _TAKES((
				struct cmd_syndesc *	aSyntax,
				char *			aRock
			      ));
IMPORT int bos_DoLsUsers _TAKES((
				 struct cmd_syndesc *	aSyntax,
				 char *			aRock
			       ));
IMPORT int bos_DoPrune _TAKES((
			       struct cmd_syndesc *	aSyntax,
			       char *			aRock
			     ));
IMPORT int bos_DoRmKey _TAKES((
			       struct cmd_syndesc *	aSyntax,
			       char *			aRock
			     ));
IMPORT int bos_DoRmUser _TAKES((
				struct cmd_syndesc *	aSyntax,
				char *			aRock
			      ));
IMPORT int bos_DoRestart _TAKES((
				 struct cmd_syndesc *	aSyntax,
				 char *			aRock
			       ));
IMPORT int bos_DoSetAuth _TAKES((
				 struct cmd_syndesc *	aSyntax,
				 char *			aRock
			       ));
#if defined(DFS_BOS_DEBUG_BOSSERVER)
IMPORT int bos_DoSetDebug _TAKES((
				  struct cmd_syndesc *	aSyntax,
				  char *			aRock
				));
#endif /* defined(DFS_BOS_DEBUG_BOSSERVER) */
IMPORT int bos_DoSetRestart _TAKES((
				    struct cmd_syndesc *	aSyntax,
				    char *			aRock
				  ));
IMPORT int bos_DoShutdown _TAKES((
				  struct cmd_syndesc *	aSyntax,
				  char *			aRock
				));
IMPORT int bos_DoStart _TAKES((
			       struct cmd_syndesc *	aSyntax,
			       char *			aRock
			     ));
IMPORT int bos_DoStartup _TAKES((
				 struct cmd_syndesc *	aSyntax,
				 char *			aRock
			       ));
IMPORT int bos_DoStatus _TAKES((
				struct cmd_syndesc *	aSyntax,
				char *			aRock
			      ));
IMPORT int bos_DoStop _TAKES((
			      struct cmd_syndesc *	aSyntax,
			      char *			aRock
			    ));
IMPORT int bos_DoUnInstall _TAKES((
				   struct cmd_syndesc *	aSyntax,
				   char *		aRock
				 ));
