/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sysincludes.h,v $
 * Revision 1.1.732.1  1996/10/02  17:58:06  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:43:47  damon]
 *
 * Revision 1.1.727.2  1994/06/09  14:15:27  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:27:57  annie]
 * 
 * Revision 1.1.727.1  1994/02/04  20:24:35  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:15:39  devsrc]
 * 
 * Revision 1.1.725.2  1994/01/20  18:43:31  annie
 * 	added copyright header
 * 	[1994/01/20  18:39:46  annie]
 * 
 * Revision 1.1.725.1  1993/12/07  17:29:47  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  16:01:05  jaffe]
 * 
 * Revision 1.1.6.2  1993/07/19  19:33:33  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:25:57  zeliff]
 * 
 * 	GAMERA Merge : #include <locale.h>
 * 	[1993/04/09  20:37:37  toml]
 * 
 * Revision 1.1.4.3  1993/07/16  20:19:25  kissel
 * 	*** empty log message ***
 * 	[1993/06/21  14:35:09  kissel]
 * 
 * 	Remove typo.
 * 	[1992/10/15  22:04:04  toml]
 * 
 * 	Initial revision for HPUX.
 * 	[1992/10/14  16:57:17  toml]
 * 
 * Revision 1.1.2.2  1993/06/04  15:12:18  kissel
 * 	Initial HPUX RP version.
 * 	[1993/06/03  21:44:35  kissel]
 * 
 * $EndLog$
*/
#ifndef	TRANSARC_OSI_SYSINCLUDES_H
#define	TRANSARC_OSI_SYSINCLUDES_H 1

#include <sys/types.h>
#include <sys/param.h>
#include <sys/vnode.h>
#include <sys/vfs.h>
#include <sys/mount.h>
#include <stdarg.h>
#include <stddef.h>
#include <limits.h>

#ifdef	KERNEL
#include <sys/errno.h>
#include <sys/kernel.h>
#include <dux/lookupops.h>
#include <sys/malloc.h>
#include <sys/inode.h>

#else	/* KERNEL */

#include <errno.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <strings.h>
#include <locale.h>
	
#endif	/* KERNEL */

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/buf.h>
#include <sys/file.h>

#endif	/* TRANSARC_OSI_SYSINCLUDES_H */

