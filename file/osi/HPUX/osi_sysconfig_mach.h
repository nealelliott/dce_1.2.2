/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_sysconfig_mach.h,v $
 * Revision 1.1.15.1  1996/10/02  17:57:51  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:43:36  damon]
 *
 * Revision 1.1.10.2  1994/06/09  14:15:18  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:27:50  annie]
 * 
 * Revision 1.1.10.1  1994/02/04  20:24:21  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:15:33  devsrc]
 * 
 * Revision 1.1.8.1  1994/01/28  20:43:48  annie
 * 	expand OSF copyright
 * 	[1994/01/28  20:42:22  annie]
 * 
 * Revision 1.1.6.2  1993/07/19  19:33:20  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:25:48  zeliff]
 * 
 * Revision 1.1.4.3  1993/07/16  20:19:09  kissel
 * 	*** empty log message ***
 * 	[1993/06/21  14:34:32  kissel]
 * 
 * 	Initial GAMERA branch
 * 	[1993/04/02  11:58:18  mgm]
 * 
 * Revision 1.1.2.2  1993/06/04  15:11:38  kissel
 * 	Initial HPUX RP version.
 * 	[1993/06/03  21:43:42  kissel]
 * 
 * Revision 1.1.2.3  1993/03/15  16:40:00  toml
 * 	Kernel extensions support.
 * 	[1993/03/15  16:01:45  toml]
 * 
 * Revision 1.1.2.2  1992/10/15  21:01:20  toml
 * 	Initial revision.
 * 	[1992/10/14  16:56:59  toml]
 * 
 * $EndLog$
*/

#ifndef _OSI_SYSCONFIG_MACH_H
#define _OSI_SYSCONFIG_MACH_H

#include <sys/kload.h>

typedef int   sysconfig_op_t;			/* configuration operation */

#define SYSCONFIG_NOSPEC	??
#define SYSCONFIG_CONFIGURE	KLOAD_INIT_CONFIG
#define SYSCONFIG_UNCONFIGURE	KLOAD_INIT_UNCONFIG
#define SYSCONFIG_QUERY		??
#define SYSCONFIG_RECONFIGURE	??
#define SYSCONFIG_QUERYSIZE	??

typedef	int filesys_config_t;

#endif	/* _OSI_SYSCONFIG_MACH_H */
