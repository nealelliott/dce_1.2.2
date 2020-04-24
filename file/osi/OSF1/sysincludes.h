/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1990, 1989 Transarc Corporation - All rights reserved */

/*
 * HISTORY
 * $Log: sysincludes.h,v $
 * Revision 1.1.590.1  1996/10/02  17:58:50  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:44:12  damon]
 *
 * Revision 1.1.584.2  1994/06/09  14:16:01  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:28:25  annie]
 * 
 * Revision 1.1.584.1  1994/02/04  20:25:15  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:15:55  devsrc]
 * 
 * Revision 1.1.581.1  1993/12/07  17:30:14  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  16:03:47  jaffe]
 * 
 * Revision 1.1.4.3  1993/01/21  14:50:56  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  14:53:21  cjd]
 * 
 * Revision 1.1.4.2  1992/12/10  22:27:49  marty
 * 	add #include ucred.h in user space for 1.1.1
 * 	[1992/10/28  20:16:48  marty]
 * 
 * Revision 1.1.2.4  1992/05/22  19:37:04  garyf
 * 	reduce number of includes
 * 	[1992/05/22  03:11:16  garyf]
 * 
 * Revision 1.1.2.3  1992/03/30  17:25:51  garyf
 * 	remove <assert.h> for OSF/1 kernel
 * 	[1992/03/30  17:22:54  garyf]
 * 
 * Revision 1.1.2.2  1992/01/24  03:46:56  devsrc
 * 	Fixed logs
 * 	[1992/01/23  22:19:17  devsrc]
 * 
 * $EndLog$
*/
#ifndef	TRANSARC_OSI_SYSINCLUDES_H
#define	TRANSARC_OSI_SYSINCLUDES_H 1


/*
 * kernel include files
 */
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/uio.h>
#include <sys/file.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/buf.h>
#include <sys/stat.h>
#include <sys/vnode.h>
#include <sys/mount.h>
#include <ufs/fs.h>
#include <ctype.h>
#include <stdarg.h>
#include <limits.h>
#ifdef KERNEL
#include <sys/kernel.h>
#include <sys/fcntl.h>
#else /* KERNEL */
#include <stdio.h>
#include <signal.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/dir.h>
#include <sys/ucred.h>
#include <sys/fs/nfs/nfs.h>
#endif /* KERNEL */

#endif	/* TRANSARC_OSI_SYSINCLUDES_H */
