/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: fts_userint.h,v $
 * $EndLog$
 */
/*
*/
/*
 * fts_userint.h -- declarations of the routines and constants that are shared by
 * fts.c and fts_userint.c
 *
 *
 *	Copyright (C) 1996, 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */

#ifndef TRANSARC_FTS_USERINT_H
#define TRANSARC_FTS_USERINT_H

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include <dcedfs/cmd.h>

#define FTS_COMMON_OFFSET	12
#define FTS_CELL_OFFSET		(FTS_COMMON_OFFSET + 0)
#define FTS_NOAUTH_OFFSET	(FTS_COMMON_OFFSET + 1)
#define FTS_LOCALAUTH_OFFSET	(FTS_COMMON_OFFSET + 2)
#define FTS_VERBOSE_OFFSET		(FTS_COMMON_OFFSET + 3)

#define FTS_NORMAL_FILESET_OFFSET	(FTS_COMMON_OFFSET - 1)

/**** the param offsets by command ****/

/**** addsite ****/
#define ADDSITE_FILESET_OFFSET		0
#define ADDSITE_SERVER_OFFSET		1
#define ADDSITE_AGGREGATE_OFFSET	2
#define ADDSITE_MAXREPL_OFFSET		3
/* also uses common params (w/o fileset) */

/**** aggrinfo ****/
#define AGGRINFO_SERVER_OFFSET		0
#define AGGRINFO_AGGREGATE_OFFSET	1
/* also uses common params (w/o fileset) */

/**** clone ****/
/* only uses common params (w/ fileset) */
#define CLONE_FILESET_OFFSET		FTS_NORMAL_FILESET_OFFSET

/**** clonesys ****/
#define CLONESYS_PREFIX_OFFSET		0
#define CLONESYS_SERVER_OFFSET		1
#define CLONESYS_AGGREGATE_OFFSET	2
/* also uses default params (w/o fileset) */

/**** create ****/
#define CREATE_FSNAME_OFFSET	0
#define CREATE_SERVER_OFFSET	1
#define CREATE_AGGREGATE_OFFSET	2
/* also uses default parms (w/o fileset) */

/**** delete ****/
#define DELETE_FILESET_OFFSET	0
#define DELETE_SERVER_OFFSET	1
#define DELETE_AGGREGATE_OFFSET	2
/* also uses default parms (w/o fileset) */

/**** delfldb ****/
#define DELFLDB_FILESET_OFFSET		0
#define DELFLDB_PREFIX_OFFSET		1
#define DELFLDB_SERVER_OFFSET		2
#define DELFLDB_AGGREGATE_OFFSET	3
/* also uses default parms (w/o fileset) */

/**** dump ****/
#define DUMP_FILESET_OFFSET	0
#define DUMP_TIME_OFFSET	1
#define	DUMP_VERSION_OFFSET 	2
#define DUMP_SERVER_OFFSET	3
#define DUMP_FILE_OFFSET	4
#define DUMP_SOCKFLAG_OFFSET	5
/* also uses default parms (w/o fileset) */

/**** examine ****/
#define EXAMINE_PATH_OFFSET	0
#define EXAMINE_FILESET_OFFSET	1
#define EXAMINE_SERVER_OFFSET	2
/* also uses default parms (w/o fileset) */

/**** lock ****/
/* only uses default parms (w/ fileset) */
#define LOCK_FILESET_OFFSET	FTS_NORMAL_FILESET_OFFSET

/**** lsaggr ****/
#define LSAGGR_SERVER_OFFSET	0
/* also uses common parms (w/o fileset) */

/**** lsfldb ****/
#define LSFLDB_FILESET_OFFSET	0
#define LSFLDB_SERVER_OFFSET	1
#define LSFLDB_AGGREGATE_OFFSET	2
#define LSFLDB_LOCKED_OFFSET	3
/* also uses common parms (w/o fileset) */

/**** lsfs ****/
#define LSFS_SERVER_OFFSET	0
#define LSFS_AGGREGATE_OFFSET	1
#define LSFS_FAST_OFFSET	2
#define LSFS_LONG_OFFSET	3
/* also uses default parms (w/o fileset) */

/**** lsquota ****/
#define LSQUOTA_PATH_OFFSET	0
#define LSQUOTA_FILESET_OFFSET	1
/* also uses default parms (w/o fileset) */

/**** lsmount ****/
#define LSMOUNT_DIR_OFFSET	0

/**** mkmount ****/
#define CRMOUNT_DIR_OFFSET	0
#define CRMOUNT_FILESET_OFFSET	1
#define CRMOUNT_GLOBAL_OFFSET	2
#define CRMOUNT_CELL_OFFSET	3
#define CRMOUNT_RW_OFFSET	4
#define CRMOUNT_FAST_OFFSET	5

/**** move ****/
#define MOVE_FILESET_OFFSET		0
#define MOVE_FROMSERVER_OFFSET		1
#define MOVE_FROMAGGREGATE_OFFSET	2
#define MOVE_TOSERVER_OFFSET		3
#define MOVE_TOAGGREGATE_OFFSET		4
#define MOVE_SOCKFLAG_OFFSET		5
/* also uses default parms (w/o fileset) */

/**** release ****/
#define RELEASE_FILESET_OFFSET	0
#define RELEASE_WAIT_OFFSET	1
/* also uses default parms (w/o fileset) */

/**** rename ****/
#define RENAME_OLDNAME_OFFSET	0
#define RENAME_NEWNAME_OFFSET	1
/* also uses default parms (w/o fileset) */

/**** restore ****/
#define RESTORE_FSNAME_OFFSET		0
#define RESTORE_SERVER_OFFSET		1
#define RESTORE_AGGREGATE_OFFSET	2
#define RESTORE_FILE_OFFSET		3
#define RESTORE_FSID_OFFSET		4
#define RESTORE_OVERWRITE_OFFSET	5
#define RESTORE_DISJOINT_OFFSET		6
#define RESTORE_SOCKFLAG_OFFSET		7
/* also uses default parms (w/o fileset) */

/**** rmmount ****/
#define RMMOUNT_DIR_OFFSET	0

/**** rmsite ****/
#define RMSITE_FILESET_OFFSET	0
#define RMSITE_SERVER_OFFSET	1
#define RMSITE_AGGREGATE_OFFSET	2
/* also uses default parms (w/o fileset) */

/**** setquota ****/
#define SETQUOTA_PATH_OFFSET	0
#define SETQUOTA_FILESET_OFFSET	1
#define SETQUOTA_SIZE_OFFSET	2
/* also uses default parms (w/o fileset) */

/**** setreplvals ****/
#define SETREPL_FILESET_OFFSET		0
#define SETREPL_RELEASE_OFFSET		1
#define SETREPL_SCHEDULED_OFFSET		2
#define SETREPL_CHANGE_OFFSET		3
#define SETREPL_MAXAGE_OFFSET		4
#define SETREPL_FAILAGE_OFFSET	5
#define SETREPL_RECLAIMWAIT_OFFSET	6
#define SETREPL_MINREPDELAY_OFFSET	7
#define SETREPL_DFLTSITEAGE_OFFSET	8
#define SETREPL_CLEAR_OFFSET		9
#define SETREPL_SOCKFLAG_OFFSET		10
/* also uses default parms (w/o fileset) */

/**** status ****/
#define STATUS_SERVER_OFFSET	0
/* also uses default parms (w/o fileset) */

/**** syncfldb ****/
#define SYNCFLDB_SERVER_OFFSET		0
#define SYNCFLDB_AGGREGATE_OFFSET	1
/* also uses default parms (w/o fileset) */

/**** syncserv ****/
#define SYNCSERV_SERVER_OFFSET		0
#define SYNCSERV_AGGREGATE_OFFSET	1
/* also uses default parms (w/o fileset) */

/**** unlock ****/
/* only uses default parms (w/ fileset) */
#define UNLOCK_FILESET_OFFSET	FTS_NORMAL_FILESET_OFFSET

/**** unlockfldb ****/
#define UNLOCKFLDB_SERVER_OFFSET	0
#define UNLOCKFLDB_AGGREGATE_OFFSET	1
/* also uses default parms (w/o fileset) */

/**** update ****/
#define UPDATE_FILESET_OFFSET	0
#define UPDATE_ALL_OFFSET		1
#define UPDATE_SERVER_OFFSET	2
/* also uses default parms (w/o fileset) */

/**** zap ****/
#define ZAP_FSID_OFFSET		0
#define ZAP_SERVER_OFFSET	1
#define ZAP_AGGREGATE_OFFSET	2
/* also uses default parms (w/o fileset) */

/* *** newserver *** */
#define	NEWSRV_ADDRESS_OFFSET	0
#define	NEWSRV_PRINCIPAL_OFFSET	1
#define	NEWSRV_QUOTA_OFFSET	2
#define	NEWSRV_OWNER_OFFSET	3
#if 0
#define	NEWSRV_OBJID_OFFSET		4
#endif /* 0 */
/* also uses default parms (w/o fileset) */

/* *** alterserver *** */
#define	ALTSRV_ADDRESS_OFFSET	0
#define	ALTSRV_DELADDR_OFFSET	1
#define	ALTSRV_ADDADDR_OFFSET	2
#define	ALTSRV_CHANGEADDR_OFFSET    3
#define	ALTSRV_SETPRINCIPAL_OFFSET	    4
#define	ALTSRV_SETQUOTA_OFFSET	5
#define	ALTSRV_SETOWNER_OFFSET	6
#define	ALTSRV_NOOWNER_OFFSET	7
#if 0
#define	ALTSRV_SETOBJID_OFFSET	8
#endif /* 0 */
/* also uses default parms (w/o fileset) */

/* *** rmserver *** */
#define DELSRV_ADDRESS_OFFSET 0
/* also uses default parms (w/o fileset) */

/* *** showserver *** */
#define	SHOWSRV_ADDRESS_OFFSET	0
#define	SHOWSRV_ALL_OFFSET		1
/* also uses default parms (w/o fileset) */

/* *** reserveids *** */
#define	RSVIDS_COUNT_OFFSET		0
#define	RSVIDS_SERVER_OFFSET	1
/* also uses default parms (w/o fileset) */

/* *** getrepstatus *** */
#define	GETRSTAT_FILESET_OFFSET	0
#define	GETRSTAT_ALLFLAG_OFFSET	1
#define	GETRSTAT_SERVER_OFFSET	2
/* also uses default parms (w/o fileset) */

/* *** getrepserverstatus *** */
#define	GETRSSTAT_SERVER_OFFSET	0
#define	GETRSSTAT_LONG_OFFSET	1
/* also uses default parms (w/o fileset) */

/**** crfldb ****/
#define	CRFLDB_FTNAME_OFFSET	0
#define	CRFLDB_SERVER_OFFSET	1
#define	CRFLDB_AGGRID_OFFSET	2
/* also uses default parms (w/o fileset) */

/**** setprotectlevels ****/
#define SETPROT_FILESET_OFFSET		0
#define SETPROT_MINLCLAUTHN_OFFSET	1
#define SETPROT_MAXLCLAUTHN_OFFSET	2
#define SETPROT_MINRMTAUTHN_OFFSET	3
#define SETPROT_MAXRMTAUTHN_OFFSET	4
/* also uses default parms (w/o fileset) */

/**** etrans ****/
#define ETRANS_SERVER_OFFSET	0
#define ETRANS_TRANID_OFFSET	1


IMPORT int fts_DoAddSite _TAKES((
				 struct cmd_syndesc *	aSyntax,
				 char *			aRock
			       ));
IMPORT int fts_DoAggregateInfo _TAKES((
				       struct cmd_syndesc *	aSyntax,
				       char *			aRock
				     ));
IMPORT int fts_DoBackupVolume _TAKES((
				      struct cmd_syndesc *	aSyntax,
				      char *			aRock
				    ));
IMPORT int fts_DoBackSys _TAKES((
				 struct cmd_syndesc *	aSyntax,
				 char *			aRock
			       ));
IMPORT int fts_DoCreateVolume _TAKES((
				      struct cmd_syndesc *	aSyntax,
				      char *			aRock
				    ));
IMPORT int fts_DoDeleteVolume _TAKES((
				      struct cmd_syndesc *	aSyntax,
				      char *			aRock
				    ));
IMPORT int fts_DoDeleteEntry _TAKES((
				     struct cmd_syndesc *	aSyntax,
				     char *			aRock
				   ));
IMPORT int fts_DoDumpVolume _TAKES((
				    struct cmd_syndesc *	aSyntax,
				    char *			aRock
				  ));
IMPORT int fts_DoLSFT _TAKES((
				       struct cmd_syndesc *	aSyntax,
				       char *			aRock
				     ));
IMPORT int fts_DoGetRepParms _TAKES((
				     struct cmd_syndesc *	aSyntax,
				     char *			aRock
				   ));
IMPORT int fts_DoLockEntry _TAKES((
				   struct cmd_syndesc *	aSyntax,
				   char *		aRock
				 ));
IMPORT int fts_DoListAggregates _TAKES((
					struct cmd_syndesc *	aSyntax,
					char *			aRock
				      ));
IMPORT int fts_DoListVolumes _TAKES((
				     struct cmd_syndesc *	aSyntax,
				     char *			aRock
				   ));
IMPORT int fts_DoLsQuota _TAKES((
				 struct cmd_syndesc *	aSyntax,
				 char *			aRock
				 ));
IMPORT int fts_DoLsMount _TAKES((
				 struct cmd_syndesc *	aSyntax,
				 char *			aRock
				 ));
IMPORT int fts_DoListFLDB _TAKES((
				  struct cmd_syndesc *	aSyntax,
				  char *			aRock
				));
IMPORT int fts_DoMkMount _TAKES((
				 struct cmd_syndesc *	aSyntax,
				 char *			aRock
				 ));
IMPORT int fts_DoMoveVolume _TAKES((
				    struct cmd_syndesc *	aSyntax,
				    char *			aRock
				  ));
IMPORT int fts_DoReleaseVolume _TAKES((
				       struct cmd_syndesc *	aSyntax,
				       char *			aRock
				     ));
IMPORT int fts_DoReleaseEntry _TAKES((
				      struct cmd_syndesc *	aSyntax,
				      char *			aRock
				    ));
IMPORT int fts_DoRenameVolume _TAKES((
				      struct cmd_syndesc *	aSyntax,
				      char *			aRock
				    ));
IMPORT int fts_DoRestoreVolume _TAKES((
				       struct cmd_syndesc *	aSyntax,
				       char *			aRock
				     ));
IMPORT int fts_DoDelMount _TAKES((
				 struct cmd_syndesc *	aSyntax,
				 char *			aRock
			       ));
IMPORT int fts_DoRemoveSite _TAKES((
				    struct cmd_syndesc *	aSyntax,
				    char *			aRock
				  ));
IMPORT int fts_DoSetQuota _TAKES((
				  struct cmd_syndesc *	aSyntax,
				  char *			aRock
				));
IMPORT int fts_DoSetRepInfo _TAKES((
				     struct cmd_syndesc *	aSyntax,
				     char *			aRock
				   ));
IMPORT int fts_DoVolserStatus _TAKES((
				      struct cmd_syndesc *	aSyntax,
				      char *			aRock
				    ));
IMPORT int fts_DoSyncVldb _TAKES((
				  struct cmd_syndesc *	aSyntax,
				  char *		aRock
				));
IMPORT int fts_DoSyncServer _TAKES((
				    struct cmd_syndesc *	aSyntax,
				    char *			aRock
				  ));
IMPORT int fts_DoUnlockFLDB _TAKES((
				    struct cmd_syndesc *	aSyntax,
				    char *			aRock
				  ));
IMPORT int fts_DoRepUpdate _TAKES((
				   struct cmd_syndesc *	aSyntax,
				   char *		aRock
				 ));
IMPORT int fts_DoVolumeZap _TAKES((
				   struct cmd_syndesc *	aSyntax,
				   char *		aRock
				 ));
IMPORT int fts_DoCrServer _TAKES((
				   struct cmd_syndesc *	aSyntax,
				   char *		aRock
				 ));
IMPORT int fts_DoAlterServer _TAKES((
				   struct cmd_syndesc *	aSyntax,
				   char *		aRock
				 ));
IMPORT int fts_DoDelServer _TAKES((
				   struct cmd_syndesc *  aSyntax,
				   char *                aRock
				 ));
IMPORT int fts_DoLsServer _TAKES((
				   struct cmd_syndesc *	aSyntax,
				   char *		aRock
				 ));
IMPORT int fts_DoShowAllServers _TAKES((
				   struct cmd_syndesc *	aSyntax,
				   char *		aRock
				 ));
IMPORT int fts_DoReserveIDs _TAKES((
				   struct cmd_syndesc *	aSyntax,
				   char *		aRock
				 ));
IMPORT int fts_DoGetRepStatus _TAKES((
				   struct cmd_syndesc *	aSyntax,
				   char *		aRock
				 ));
IMPORT int fts_DoGetRepServerStatus _TAKES((
				   struct cmd_syndesc *	aSyntax,
				   char *		aRock
				 ));
IMPORT int fts_DoCrFLDBEntry _TAKES((
				   struct cmd_syndesc *	aSyntax,
				   char *		aRock
				 ));
IMPORT int fts_DoEndTrans _TAKES((
				  struct cmd_syndesc *	aSyntax,
				  char *		aRock
				));
IMPORT int fts_DoSetProtectLevels _TAKES((
					  struct cmd_syndesc *	aSyntax,
					  char *		aRock
					  ));

IMPORT void fts_ui_SetUpSyntax _TAKES((
				       void
				     ));

#endif /* TRANSARC_FTS_USERINT_H */
