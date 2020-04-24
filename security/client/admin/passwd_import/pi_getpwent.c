/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pi_getpwent.c,v $
 * Revision 1.1.8.2  1996/03/11  13:26:28  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:29  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:57:22  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/15  21:21 UTC  mullan_s
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE01/1  1994/03/23  22:17 UTC  mob
 * 
 * 	HP revision /main/mob_rodan/1  1994/03/23  22:16 UTC  mob
 * 	remove master.passwd for V4FS changes
 * 	[1995/12/08  16:29:11  root]
 * 
 * Revision 1.1.5.3  1992/12/29  12:38:18  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:34:21  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  21:14:43  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:18:35  sekhar]
 * 
 * Revision 1.1.2.2  1992/04/23  14:15:59  burati
 * 	Initial version, modified from original 4.4 source.
 * 	[1992/04/23  01:37:29  burati]
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
 */
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)getpwent.c	5.14 (Berkeley) 6/1/90";
#endif /* LIBC_SCCS and not lint */

#include <sys/types.h>
#ifdef SNI_SVR4
#include <fcntl.h>
#endif /* SNI_SVR4 */
#include <sys/file.h>
#include <sys/param.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pi_pwd.h>

static FILE *_pw_fp;
static struct passwd _pw_passwd;
static int _pw_getfirstkey, _pw_stayopen;
static char _pw_flag, *_pw_file = _PATH_PASSWD;
static char PW_FILE[MAXPATHLEN+1];

#define	MAXLINELENGTH	1024
static char line[MAXLINELENGTH];

static pi_start_pw
#ifndef __STDC__
    (want_fp)
	char want_fp;		/* open _pw_fp also */
#else
  (
	char want_fp
  )
#endif
{
	if (_pw_fp) {
		rewind(_pw_fp);
		return(1);
	}
	/*
	 * If we really want to set up _pw_fp, then try 
	 * the old file.
	 */
	if (want_fp && (_pw_fp = fopen(_pw_file, "r"))) {
		return(1);
	}
	return(0);
}

static scanpw(
#ifdef __STDC__
void
#endif
)
{
	register char *cp;
	long atol();
	char *bp;

	for (;;) {
		if (!(fgets(line, sizeof(line), _pw_fp)))
			return(0);
		bp = line;
		/* skip lines that are too big */
		if (!index(line, '\n')) {
			int ch;

			while ((ch = getc(_pw_fp)) != '\n' && ch != EOF)
				;
			continue;
		}
		_pw_passwd.pw_name = (char *)strsep(&bp, ":\n");
		_pw_passwd.pw_passwd = (char *)strsep(&bp, ":\n");
		if (!(cp = (char *)strsep(&bp, ":\n")))
			continue;
		_pw_passwd.pw_uid = atoi(cp);
		if (!(cp = (char *)strsep(&bp, ":\n")))
			continue;
		_pw_passwd.pw_gid = atoi(cp);
		_pw_passwd.pw_gecos = (char *)strsep(&bp, ":\n");
		_pw_passwd.pw_dir = (char *)strsep(&bp, ":\n");
		_pw_passwd.pw_shell = (char *)strsep(&bp, ":\n");
		if (!_pw_passwd.pw_shell)
			continue;
		return(1);
	}
	/* NOTREACHED */
}

#define	_MAX_PASSWD_SIZE	50
static char pwbuf[_MAX_PASSWD_SIZE];

static pi_getpw(
#ifdef __STDC__
void
#endif
)
{
	long pos, atol();
	int fd, n;
	char *p;
	off_t lseek();

	if ((fd = open(_pw_file, O_RDONLY, 0)) < 0)
		return;
	pos = atol(_pw_passwd.pw_passwd);
	if (lseek(fd, pos, L_SET) != pos)
		goto bad;
	if ((n = read(fd, pwbuf, sizeof(pwbuf) - 1)) < 0)
		goto bad;
	pwbuf[n] = '\0';
	for (p = pwbuf; *p; ++p)
		if (*p == ':') {
			*p = '\0';
			_pw_passwd.pw_passwd = pwbuf;
			break;
		}
bad:	(void)close(fd);
}

struct passwd *pi_getpwent(
#ifdef __STDC__
void
#endif
)
{
	if (!_pw_fp && !pi_start_pw(1))
		return((struct passwd *)NULL);
	if (!scanpw())
		return((struct passwd *)NULL);
	pi_getpw();
	return(&_pw_passwd);
}

struct passwd *pi_getpwnam
#ifndef __STDC__
    (nam)
	char *nam;
#else
  (
	char *nam
  )
#endif
{
	int rval;

	if (!pi_start_pw(1))
		return((struct passwd *)NULL);
	for (rval = 0; scanpw();)
		if (!strcmp(nam, _pw_passwd.pw_name)) {
			rval = 1;
			break;
		}
	if (!_pw_stayopen)
		pi_endpwent();
	if (rval)
		pi_getpw();
	return(rval ? &_pw_passwd : (struct passwd *)NULL);
}

struct passwd *pi_getpwuid
#ifndef __STDC__
    (uid)
	int uid;
#else
  (
	int uid
  )
#endif
{
	int rval;

	if (!pi_start_pw(1))
		return((struct passwd *)NULL);
	for (rval = 0; scanpw();)
		if (_pw_passwd.pw_uid == uid) {
			rval = 1;
			break;
		}
	if (!_pw_stayopen)
		pi_endpwent();
	if (rval)
		pi_getpw();
	return(rval ? &_pw_passwd : (struct passwd *)NULL);
}

pi_setpwent(
#ifdef __STDC__
void
#endif
)
{
	return(pi_setpassent(0));
}

pi_setpassent
#ifndef __STDC__
    (stayopen)
	int stayopen;
#else
  (
	int stayopen
  )
#endif
{
	if (!pi_start_pw(1))
		return(0);
	_pw_stayopen = stayopen;
	return(1);
}

void pi_endpwent(
#ifdef __STDC__
void
#endif
)
{
	if (_pw_fp) {
		(void)fclose(_pw_fp);
		_pw_fp = (FILE *)NULL;
	}
}

void pi_setpwfile
#ifndef __STDC__
    (file)
	char *file;
#else
  (
	char *file
  )
#endif
{
        _pw_file = &(PW_FILE[0]);
	strncpy(_pw_file, file, MAXPATHLEN);
        _pw_file[MAXPATHLEN] = '\0';
}

