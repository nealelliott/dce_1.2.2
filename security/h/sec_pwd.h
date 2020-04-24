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
 * Revision 1.1.5.2  1996/02/18  23:01:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:38  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:29:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:24  root]
 * 
 * Revision 1.1.3.4  1992/12/29  13:27:52  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:47:17  zeliff]
 * 
 * Revision 1.1.3.3  1992/12/04  21:27:13  burati
 * 	CR5978 Break out OS dependent info into machine specific dirs
 * 	       (rename struct passwd to struct sec_passwd)
 * 	[1992/12/04  16:45:15  burati]
 * 
 * Revision 1.1.3.2  1992/10/07  20:29:32  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:13:40  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:48:09  burati
 * 	 Second replication code drop: bl5
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1991,1992
 * Unpublished work. All Rights Reserved.
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
 */

#include <sys/types.h>

/*
 * The sec_passwd structure is used internally in the sec_login
 * code that handles login contexts.  The data is then copied
 * into the correct struct passwd for each particular OS by machine
 * dependent copies of sec_login_np_lc_to_pwent.
 */
struct sec_passwd {
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
