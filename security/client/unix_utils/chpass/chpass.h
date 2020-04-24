/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: chpass.h,v $
 * Revision 1.1.6.2  1996/02/18  22:57:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:35  marty]
 *
 * Revision 1.1.6.1  1995/12/13  16:24:09  root
 * 	Submit
 * 	[1995/12/11  15:13:58  root]
 * 
 * Revision 1.1.4.2  1992/12/29  13:05:00  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:42:39  zeliff]
 * 
 * Revision 1.1.2.2  1992/07/17  18:59:26  burati
 * 	Integration with DCE
 * 	[1992/07/17  18:48:31  burati]
 * 
 * 	Initial version from BSD4.4
 * 	[1992/07/13  18:53:35  burati]
 * 
 * $EndLog$
 */

/*
 * Copyright (c) 1988 The Regents of the University of California.
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
 *	@(#)chpass.h	5.2 (Berkeley) 6/1/90
 */

struct entry {
	char *prompt;
	int (*func)(), restricted, len;
	char *except, *save;
};

extern uid_t uid;

#define GOOD_STATUS(stp)          ((stp)==NULL || (*stp) == error_status_ok)
#define BAD_STATUS(stp)           ((*stp) != error_status_ok)
#define SET_STATUS(stp, val)      (*stp) = val
#define CLEAR_STATUS(stp)         (*stp) = error_status_ok
#define STATUS_EQUAL(stp, st_val) ((*stp) == (st_val))

#ifdef MIN
#undef MIN
#endif
#define MIN(a, b)           (a) > (b) ? (b) : (a)

int p_change(
#ifdef __STDC__
    char *p,
    struct passwd *pw,
    struct entry *ep
#endif
);

int p_class(
#ifdef __STDC__
    char *p,
    struct passwd *pw,
    struct entry *ep
#endif
);

int p_expire(
#ifdef __STDC__
    char *p,
    struct passwd *pw,
    struct entry *ep
#endif
);

int p_gecos(
#ifdef __STDC__
    char *p,
    struct passwd *pw,
    struct entry *ep
#endif
);

int p_gid(
#ifdef __STDC__
    register char *p,
    struct passwd *pw,
    struct entry *ep
#endif
);

int p_hdir(
#ifdef __STDC__
    char *p,
    struct passwd *pw,
    struct entry *ep
#endif
);

int p_login(
#ifdef __STDC__
    char *p,
    struct passwd *pw,
    struct entry *ep
#endif
);

int p_passwd(
#ifdef __STDC__
    char *p,
    struct passwd *pw,
    struct entry *ep
#endif
);

int p_shell(
#ifdef __STDC__
    register char *p,
    struct passwd *pw,
    struct entry *ep
#endif
);

int p_uid(
#ifdef __STDC__
    register char *p,
    struct passwd *pw,
    struct entry *ep
#endif
);

void get_account_data(
#ifdef __STDC__
    unsigned_char_p_t   name,
    boolean32           get_pwent,
    struct passwd       *pwent,
    error_status_t      *st_p
#endif
);

