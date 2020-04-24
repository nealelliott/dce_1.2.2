/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/* Copyright (C) 1996, 1995 Transarc Corporation - All rights reserved */
/*
 * HISTORY
 * $Log: dfsgwd.h,v $
 * Revision 1.1.6.1  1996/10/02  17:50:15  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:27  damon]
 *
 * Revision /main/HPDCE02/2  1995/03/30  20:03 UTC  maunsell_c
 * 	CHFts14091 - dfsgwd forks itself when new -background switch used
 * 
 * Revision /main/HPDCE02/2  1995/03/24  20:49 UTC  maunsell_c
 * 	add background switch (CHFts14091)
 * 
 * Revision /main/HPDCE02/1  1995/02/14  22:42 UTC  maunsell_c
 * 	@sys @host changes for nfs/dfs gateway
 * 
 * Revision /main/maunsell_mothra1.45/1  1995/01/31  19:21 UTC  maunsell_c
 * 	add @sys @host support for dfs_login/dfsgwd/dfsgw
 * 
 * Revision 1.1.2.2  1994/08/22  16:16:20  maunsell_c
 * 	add offset defines for cmd parsed parms use
 * 	[1994/08/10  16:40:10  maunsell_c]
 * 
 * Revision 1.1.2.1  1994/07/26  19:13:23  mckeen
 * 	Expanded copyrights
 * 	[1994/07/26  18:57:58  mckeen]
 * 
 * 	Initial Version
 * 	[1994/07/26  16:05:39  mckeen]
 * 
 * $EndLog$
 */
/* $Header: /u0/rcs_trees/dce/rcs/file/gateway/dfsgwd/dfsgwd.h,v 1.1.6.1 1996/10/02 17:50:15 damon Exp $ */
/*
 * dfsgwd.h - Header file for dfsgwd daemon.
 */

#ifndef DFSGWD_H
#define DFSGWD_H

/*
 * Defines
 */
#define	LOGPATH		"/var/dfs/adm/"		/* suffix for DCELOCAL */
#define	LOGFILE		"DfsgwLog"		/* server log file */

#define SERVICE_OFFSET  0                       /* cmd_IndexedAddParm for -service */
#define LOGFILE_OFFSET  1                       /* cmd_IndexedAddParm for -file */
#define SYSNAME_OFFSET  2                       /* cmd_IndexedAddParm for -sysname */
#define DOMAINS_OFFSET  3                       /* cmd_IndexedAddParm for -nodomains */
#define VERBOSE_OFFSET  4                       /* cmd_IndexedAddParm for -verbose */
#define BGROUND_OFFSET  5                       /* cmd_IndexedAddParm for -background */


/*
 * Global data.
 */
extern char *progname;		/* Program name */
extern int debug;		/* Debug flag */

/*
 * Functions.
 */
int log_init
#ifdef __STDC__
	(char *logfile);
#else
	();
#endif

void log_close();
int log
#ifdef __STDC__
	(char *fmt, ...);
#else
	();
#endif 
#endif /* DFSGWD_H */

