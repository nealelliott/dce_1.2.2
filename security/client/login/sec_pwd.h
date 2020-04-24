/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_pwd.h,v $
 * Revision 1.1.5.2  1996/03/11  13:27:58  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:45  marty]
 *
 * Revision 1.1.5.1  1995/12/08  18:05:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:31:52  root]
 * 
 * Revision 1.1.3.2  1992/12/29  13:02:23  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:13:30  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:46:48  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  sec_pwd.h V=1 11/04/90 //littl/prgy/src/client/sec_login
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*- sec_pwd.h - struct passwd "borrowed" from Berkeley 4.4 pwd.h
 *
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
 *	@(#)pwd.h	5.5 (Berkeley) 5/29/90
 *
 *  HISTORY 
 *  Date    Author  Comments
 *  11/4/90 ahop    File created.  Borrowed struct passwd from BSD 4.4 pwd.h.
 */                                                             


#include <sys/types.h>

struct passwd {
	char	*pw_name;		/* user name */
	char	*pw_passwd;		/* encrypted password */
	int	pw_uid;			/* user uid */
	int	pw_gid;			/* user gid */
	time_t	pw_change;		/* password change time */
	char	*pw_class;		/* user access class */
	char	*pw_gecos;		/* Honeywell login info */
	char	*pw_dir;		/* home directory */
	char	*pw_shell;		/* default shell */
	time_t	pw_expire;		/* account expiration */
};

