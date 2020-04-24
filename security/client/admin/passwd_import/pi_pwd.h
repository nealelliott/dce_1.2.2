/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pi_pwd.h,v $
 * Revision 1.1.6.2  1996/03/11  13:26:30  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:30  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:57:32  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/15  21:21 UTC  mullan_s
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE01/1  1994/03/23  22:18 UTC  mob
 * 
 * 	HP revision /main/mob_rodan/1  1994/03/11  19:47 UTC  mob
 * 	v4fs cleanup
 * 	[1995/12/08  16:29:17  root]
 * 
 * Revision 1.1.4.2  1992/12/29  12:38:27  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:34:30  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/23  14:16:44  burati
 * 	Initial version, modified from original 4.4 source.
 * 	[1992/04/23  01:43:27  burati]
 * 
 * $EndLog$
 */

/*-
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
 */

#define	_PATH_PASSWD		"/etc/passwd"
#define	_PASSWORD_LEN		128

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

#if __STDC__ || c_plusplus
struct passwd *pi_getpwuid(int);
struct passwd *pi_getpwnam(char *);
struct passwd *pi_getpwent(void);
int pi_setpwent(void);
void pi_endpwent(void);
void pi_setpwfile(char *);
int pi_setpassent(int);
#else
struct passwd *pi_getpwuid();
struct passwd *pi_getpwnam();
struct passwd *pi_getpwent();
int pi_setpwent();
void pi_endpwent();
void pi_setpwfile();
int pi_setpassent();
#endif
