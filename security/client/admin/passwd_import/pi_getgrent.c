/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pi_getgrent.c,v $
 * Revision 1.1.7.2  1996/03/11  13:26:27  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:28  marty]
 *
 * Revision 1.1.7.1  1995/12/08  17:57:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:29:09  root]
 * 
 * Revision 1.1.4.2  1992/12/29  12:38:16  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:34:16  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/23  14:15:33  burati
 * 	Initial version, modified from original 4.4 source.
 * 	[1992/04/23  01:36:40  burati]
 * 
 * $EndLog$
 */

/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted provided
 * that: (1) source distributions retain this entire copyright notice and
 * comment, and (2) distributions including binaries display the following
 * acknowledgement:  ``This product includes software developed by the
 * University of California, Berkeley and its contributors'' in the
 * documentation or other materials provided with the distribution and in
 * all advertising materials mentioning features or use of this software.
 * Neither the name of the University nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)getgrent.c	5.7 (Berkeley) 6/24/90";
#endif /* LIBC_SCCS and not lint */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/param.h>
#include <pi_grp.h>

static FILE *_gr_fp;
static struct group _gr_group;
static int _gr_stayopen;
static char *_gr_file = _PATH_GROUP;
static char GR_FILE[MAXPATHLEN+1];

#define	MAXGRP		200
static char *members[MAXGRP];
#define	MAXLINELENGTH	4096
static char line[MAXLINELENGTH];

static pi_start_gr(
#ifdef __STDC__
void
#endif
)
{
	if (_gr_fp) {
		rewind(_gr_fp);
		return(1);
	}
	return((_gr_fp = fopen(_gr_file, "r")) ? 1 : 0);
}

static pi_grscan
#ifndef __STDC__
    (search, gid, name)
	register int search, gid;
	register char *name;
#else
  (
	register int search,
        register int gid,
	register char *name
  )
#endif
{
	register char *cp, **m;
	char *bp;

	for (;;) {
		if (!fgets(line, sizeof(line), _gr_fp))
			return(0);
		bp = line;
		/* skip lines that are too big */
		if (!index(line, '\n')) {
			int ch;

			while ((ch = getc(_gr_fp)) != '\n' && ch != EOF)
				;
			continue;
		}
		_gr_group.gr_name = (char *)strsep(&bp, ":\n");
		if (search && name && strcmp(_gr_group.gr_name, name))
			continue;
		_gr_group.gr_passwd = (char *)strsep(&bp, ":\n");
		if (!(cp = (char *)strsep(&bp, ":\n")))
			continue;
		_gr_group.gr_gid = atoi(cp);
		if (search && name == NULL && _gr_group.gr_gid != gid)
			continue;
		for (m = _gr_group.gr_mem = members;; ++m) {
			if (m == &members[MAXGRP - 1]) {
				*m = NULL;
				break;
			}
			if ((*m = (char *)strsep(&bp, ", \n")) == NULL)
				break;
		}
		return(1);
	}
	/* NOTREACHED */
}

struct group *
pi_getgrent(
#ifdef __STDC__
void
#endif
)
{
	if (!_gr_fp && !pi_start_gr() || !pi_grscan(0, 0, (char *)NULL))
		return((struct group *)NULL);
	return(&_gr_group);
}

struct group *pi_getgrnam
#ifndef __STDC__
    (name)
	char *name;
#else
  (
	char *name
  )
#endif
{
	int rval;

	if (!pi_start_gr())
		return((struct group *)NULL);
	rval = pi_grscan(1, 0, name);
	if (!_gr_stayopen)
		pi_endgrent();
	return(rval ? &_gr_group : (struct group *)NULL);
}

struct group *pi_getgrgid
#ifndef __STDC__
    (gid)
	int gid;
#else
  (
	int gid
  )
#endif
{
	int rval;

	if (!pi_start_gr())
		return((struct group *)NULL);
	rval = pi_grscan(1, gid, (char *)NULL);
	if (!_gr_stayopen)
		pi_endgrent();
	return(rval ? &_gr_group : (struct group *)NULL);
}

pi_setgrent(
#ifdef __STDC__
void
#endif
)
{
	return(pi_setgroupent(1));
}

pi_setgroupent
#ifndef __STDC__
    (stayopen)
	int stayopen;
#else
  (
	int stayopen
  )
#endif
{
	if (!pi_start_gr())
		return(0);
	_gr_stayopen = stayopen;
	return(1);
}

void
pi_endgrent(
#ifdef __STDC__
void
#endif
)
{
	if (_gr_fp) {
		(void)fclose(_gr_fp);
		_gr_fp = (FILE *)NULL;
	}
}

void pi_setgrfile
#ifndef __STDC__
    (file)
	char *file;
#else
  (
	char *file
  )
#endif
{
        _gr_file = &(GR_FILE[0]);
        strncpy(_gr_file, file, MAXPATHLEN);
        _gr_file[MAXPATHLEN] = '\0';
}

