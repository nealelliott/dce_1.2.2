/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bbos_pathnames.c,v $
 * Revision 1.1.49.1  1996/10/02  17:03:56  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:50  damon]
 *
 * Revision 1.1.43.2  1994/06/09  13:52:06  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:25:47  annie]
 * 
 * Revision 1.1.43.1  1994/02/04  20:06:16  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:06:38  devsrc]
 * 
 * Revision 1.1.41.1  1993/12/07  17:12:56  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  12:55:58  jaffe]
 * 
 * Revision 1.1.3.5  1993/01/18  19:55:26  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:31:27  cjd]
 * 
 * Revision 1.1.3.4  1992/11/24  15:40:32  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:08:27  bolinger]
 * 
 * Revision 1.1.3.3  1992/10/02  20:50:21  toml
 * 	Change sense of #ifdefs for better portability.
 * 	[1992/10/02  18:40:14  toml]
 * 
 * Revision 1.1.3.2  1992/08/31  18:24:04  jaffe
 * 	Replace missing RCS id 'Header'
 * 	[1992/08/31  13:56:34  jaffe]
 * 
 * 	Transarc delta: bab-ot4423-bosserver-remove-BOSSVR_LOCAL_DEBUG 1.1
 * 	  Selected comments:
 * 	    All uses of BOSSVR_LOCAL_DEBUG are obsolete (and some of them
 * 	    got in the way of debugging).  They were removed.
 * 	    ot 4423
 * 	    See above.
 * 	Transarc delta: jess-ot3775-remove-DCELOCAL-dups 1.1
 * 	  Selected comments:
 * 	    Now that both reference ports use DCEPATHS to provide definitions of
 * 	    DCELOCAL_PATH and DCESHARED_PATH, the user space server can be cleaned
 * 	    upto not have local definitions of these parameters.
 * 	    see above.
 * 	[1992/08/28  20:47:52  jaffe]
 * 
 * Revision 1.1  1992/01/19  02:49:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  bbos_pathnames.c -- variables and routines for maintaining pathnames used by the
 * bosserver and bos
 *
 *
 *	Copyright (C) 1991 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/bosserver/bbos_pathnames.c,v 1.1.49.1 1996/10/02 17:03:56 damon Exp $")

#include <stdio.h>

#if defined(AFS_HPUX_ENV)
#include <stdlib.h>
#endif /* defined(AFS_HPUX_ENV) */

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

/**** space for each of the interesting pathnames ****/
char dce_base_dir[BUFSIZ];		/* $DCELOCAL_PATH */
char dce_subcomponents_dir[BUFSIZ];	/* $DCELOCAL_PATH/var */
char afs_server_config_dir[BUFSIZ];	/* $DCELOCAL_PATH/var/dfs */
char afs_server_admin_dir[BUFSIZ];	/* $DCELOCAL_PATH/var/dfs/adm */
char afs_bin_dir[BUFSIZ];		/* $DCELOCAL_PATH/bin */

/* configuration files */
char afs_noauth_file[BUFSIZ];
char afs_bossvr_users_file[BUFSIZ];
char afs_bossvr_config_file[BUFSIZ];
char afs_bossvr_license_file[BUFSIZ];

/* log files */
char afs_bosserver_log_file[BUFSIZ];
char afs_salvager_log_file[BUFSIZ];

/* binaries */
char afs_bosserver_binary[BUFSIZ];
char afs_salvager_binary[BUFSIZ];

#if defined(AFS_AIX_ENV) || defined(AFS_OSF_ENV)
IMPORT char * getenv _TAKES((
			     char *	varName
			   ));
#endif /* defined(AFS_AIX_ENV) || defined(AFS_OSF_ENV) */

/**** the following are relative to $DCELOCAL_PATH ****/
#define	AFS_BIN_DIR		"/bin"
#define AFS_DCEBASE_DIR		"/var"
#define	AFS_DCEDIR_SUFFIX	"/dfs"
#define AFS_ADMIN_SUFFIX	"/adm"

/**** the following are relative to afs_server_admin_dir ****/
#define AFS_BOSSERVER_LOG_FILE	"BosLog"
#define AFS_SALVAGER_LOG_FILE	"SalvageLog"

/**** the following are relative to afs_bin_dir ****/
#define AFS_BOSSERVER_BINARY_FILENAME	"bosserver"
#define AFS_SALVAGER_BINARY_FILENAME	"salvager"

/**** the following are relative to afs_server_config_dir ****/
#define AFS_NOAUTH_FILENAME		"NoAuth"
#define AFS_BOSSVR_USERLIST_DB_FILENAME	"admin.bosserver"
#define AFS_BOSSVR_CONFIG_DB_FILENAME	"BosConfig"
#define AFS_LICENSE_DB_FILENAME		"License"

#define LONGEST_PATH_FROM_DCELOCAL	"/var/dfs/admin.bosserver"


EXPORT void InitPaths()
{
  char *	dceEnvPath;
  int		charsLeft;
  
  (void)strncpy(dce_base_dir, DCELOCAL_PATH, sizeof(dce_base_dir) - 1);
  if (strlen(dce_base_dir) > (BUFSIZ - strlen(LONGEST_PATH_FROM_DCELOCAL) - 1)) {
    fprintf(stderr, "Error: DCELOCAL pathname too long.\n");
    exit(1);
  }
  
  /**** init afs bin and dce subcomponents base, relative to the dce base dir ****/

  (void)sprintf(afs_bin_dir, "%s%s", dce_base_dir, AFS_BIN_DIR);

  /* $DCELOCAL/var */
  (void)sprintf(dce_subcomponents_dir, "%s%s", dce_base_dir, AFS_DCEBASE_DIR);

  /* $DCELOCAL/var/dfs */
  (void)sprintf(afs_server_config_dir, "%s%s", dce_subcomponents_dir, AFS_DCEDIR_SUFFIX);

  /* $DCELOCAL/var/dfs/adm */
  (void)sprintf(afs_server_admin_dir, "%s%s", afs_server_config_dir, AFS_ADMIN_SUFFIX);
  
  /**** interesting binaries in $DCEBASE/bin ****/
  (void)sprintf(afs_bosserver_binary, "%s/%s", afs_bin_dir, AFS_BOSSERVER_BINARY_FILENAME);
  (void)sprintf(afs_salvager_binary, "%s/%s", afs_bin_dir, AFS_SALVAGER_BINARY_FILENAME);

  /**** the server configuration files ****/
  (void)sprintf(afs_noauth_file, "%s/%s", afs_server_config_dir, AFS_NOAUTH_FILENAME);
  (void)sprintf(afs_bossvr_users_file, "%s/%s", afs_server_config_dir,
		AFS_BOSSVR_USERLIST_DB_FILENAME);
  (void)sprintf(afs_bossvr_config_file, "%s/%s", afs_server_config_dir,
		AFS_BOSSVR_CONFIG_DB_FILENAME);
  (void)sprintf(afs_bossvr_license_file, "%s/%s", afs_server_config_dir,
		AFS_LICENSE_DB_FILENAME);

  /**** the server log files ****/
  (void)sprintf(afs_bosserver_log_file, "%s/%s", afs_server_admin_dir,
		AFS_BOSSERVER_LOG_FILE);
  (void)sprintf(afs_salvager_log_file, "%s/%s", afs_server_admin_dir,
		AFS_SALVAGER_LOG_FILE);
}

