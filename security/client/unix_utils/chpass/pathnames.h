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
 * Revision 1.1.6.2  1996/02/18  22:57:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:36  marty]
 *
 * Revision 1.1.6.1  1995/12/13  16:24:11  root
 * 	Submit
 * 	[1995/12/11  15:14:00  root]
 * 
 * Revision 1.1.4.2  1992/12/29  13:05:10  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:42:46  zeliff]
 * 
 * Revision 1.1.2.2  1992/07/17  19:01:08  burati
 * 	Integration with DCE
 * 	[1992/07/17  18:49:42  burati]
 * 
 * 	Initial version from BSD4.4
 * 	[1992/07/13  18:54:30  burati]
 * 
 * $EndLog$
 */

/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that: (1) source distributions retain this entire copyright
 * notice and comment, and (2) distributions including binaries display
 * the following acknowledgement:  ``This product includes software
 * developed by the University of California, Berkeley and its contributors''
 * in the documentation or other materials provided with the distribution
 * and in all advertising materials mentioning features or use of this
 * software. Neither the name of the University nor the names of its
 * contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *	@(#)pathnames.h	5.3 (Berkeley) 6/1/90
 */

#undef _PATH_TMP
#define	_PATH_TMP	"/tmp/passwd.XXXXXX"

#ifndef _PATH_BSHELL
#define _PATH_BSHELL    "/bin/sh"
#endif

