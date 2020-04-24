/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bbos_pathnames.h,v $
 * Revision 1.1.10.1  1996/10/02  17:03:58  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:50  damon]
 *
 * Revision 1.1.4.1  1994/06/09  13:52:09  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:25:49  annie]
 * 
 * Revision 1.1.2.2  1993/01/18  19:55:34  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:31:35  cjd]
 * 
 * Revision 1.1  1992/01/19  02:49:11  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  bbos_pathnames.h -- variables and routines for maintaining pathnames used by the
 * bosserver and bos
 *
 *
 *	Copyright (C) 1991 Transarc Corporation
 *	All rights reserved.
 */

/* various filenames the bosserver parts find useful */
IMPORT char dce_base_dir[];
IMPORT char dce_subcomponents_dir[];
IMPORT char afs_server_config_dir[];
IMPORT char afs_server_admin_dir[];
IMPORT char afs_bin_dir[];

IMPORT char afs_noauth_file[];
IMPORT char afs_bossvr_users_file[];
IMPORT char afs_bossvr_config_file[];
IMPORT char afs_bossvr_license_file[];

IMPORT char afs_bosserver_binary[];
IMPORT char afs_salvager_binary[];

IMPORT char afs_bosserver_log_file[];
IMPORT char afs_salvager_log_file[];

/* exported routines */
IMPORT void InitPaths _TAKES((
			      void
			    ));


