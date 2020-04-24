/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pi_grp.h,v $
 * Revision 1.1.6.2  1996/03/11  13:26:29  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:30  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:57:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:29:14  root]
 * 
 * Revision 1.1.4.2  1992/12/29  12:38:23  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:34:26  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/23  14:16:22  burati
 * 	Initial version, modified from original 4.4 source.
 * 	[1992/04/23  01:42:20  burati]
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
 *	@(#)grp.h	5.2 (Berkeley) 5/29/90
 */

#define	_PATH_GROUP		"/etc/group"

struct group {
	char	*gr_name;		/* group name */
	char	*gr_passwd;		/* group password */
	int	gr_gid;			/* group id */
	char	**gr_mem;		/* group members */
};

#if __STDC__ || c_plusplus
extern struct group *pi_getgrgid(int);
extern struct group *pi_getgrnam(char *);
extern struct group *pi_getgrent(void);
extern int pi_setgrent(void);
extern void pi_endgrent(void);
extern void pi_setgrfile(char *);
extern int pi_setgroupent(int);
#else
extern struct group *pi_getgrgid();
extern struct group *pi_getgrnam();
extern struct group *pi_getgrent();
extern int pi_setgrent();
extern void pi_endgrent();
extern void pi_setgrfile();
extern int pi_setgroupent();
#endif
