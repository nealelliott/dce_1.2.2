/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pathnames.h,v $
 * Revision 1.1.4.2  1996/02/18  22:57:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:37  marty]
 *
 * Revision 1.1.4.1  1995/12/13  16:24:18  root
 * 	Submit
 * 	[1995/12/11  15:14:05  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:05:31  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:13:56  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:47:41  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  pathnames.h V=1 10/24/90 //littl/prgy/src/client/unix_tools/login
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *	@(#)pathnames.h	5.5 (Berkeley) 6/24/90
 */

#include <paths.h>

#define	_PATH_DEFPATH	"/bin:/usr/bin:/usr/games:"
#define	_PATH_HUSHLOGIN	".hushlogin"
#define	_PATH_MOTDFILE	"/etc/motd"
