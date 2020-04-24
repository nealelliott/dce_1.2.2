/*
 *      SCCS:  @(#)mkdir.c	1.3 (92/10/06) 
 *
 *	UniSoft Ltd., London, England
 *
 * (C) Copyright 1992 X/Open Company Limited
 *
 * All rights reserved.  No part of this source code may be reproduced,
 * stored in a retrieval system, or transmitted, in any form or by any
 * means, electronic, mechanical, photocopying, recording or otherwise,
 * except as stated in the end-user licence agreement, without the prior
 * permission of the copyright owners.
 *
 * X/Open and the 'X' symbol are trademarks of X/Open Company Limited in
 * the UK and other countries.
 */

#ifndef lint
static char sccsid[] = "@(#)mkdir.c	1.3 (92/10/06) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)mkdir.c	1.3 92/10/06
NAME:		mkdir.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	functions to create and remove a directory

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <errno.h>
#include "dtmac.h"
#include "dtetlib.h"

#ifdef NOMKDIR
#include <sys/types.h>
#include <sys/stat.h>
#include "error.h"
#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif /* NEEDsrcFile */
#endif /* NOMKDIR */

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <unistd.h>
#else
extern void _exit();
#endif

/* static function declarations */
#if __STDC__
#ifdef NOMKDIR
static int hardmkrmdir(char *, char *, int);
#endif /* NOMKDIR */
#else /* __STDC__ */
#ifdef NOMKDIR
static int hardmkrmdir();
#endif /* NOMKDIR */
#endif /* __STDC__ */


/*
**	domkdir() - create a directory
**
**	return 0 if successful, -1 otherwise
*/

int domkdir(path, mode)
char *path;
{
	register int rc;

#ifdef NOMKDIR

	register char *p;
	register int n;
	struct stat stbuf;
	char dir[MAXPATH + 1];

	/* see if the directory exists already */
	if (stat(path, &stbuf) == 0) {
		errno = EEXIST;
		return(-1);
	}

	/* see if a mkdir is likely to succeed */
	for (p = path + strlen(path) - 1; p >= path; p--)
		if (*p == '/')
			break;
	if (p > path) {
		n = p - path;
		(void) sprintf(dir, "%.*s", MIN(n, sizeof dir - 1), path);
		if (stat(dir, &stbuf) < 0)
			return(-1);
		if ((stbuf.st_mode & S_IFMT) != S_IFDIR) {
			errno = ENOTDIR;
			return(-1);
		}
		if (tet_eaccess(dir, 02) < 0)
			return(-1);
	}

	/* we must make it the hard way */
	if ((rc = hardmkrmdir("mkdir", path, mode)) < 0)
		if (rc == -1)
			errno = EEXIST;
		else
			rc = -1;

#else /* NOMKDIR */

	/* easy - we have mkdir(2) */
	rc = mkdir(path, mode);

#endif /* NOMKDIR */

	return(rc);
}

/*
**	dormdir() - remove a directory
**
**	return 0 if successful, -1 otherwise
*/

int dormdir(path)
char *path;
{
	register int rc;

#ifdef NOMKDIR

	register char *p;
	register int n;
	struct stat stbuf;
	char dir[MAXPATH + 1];

	/* see if the path exists and is a directory */
	if (stat(path, &stbuf) < 0)
		return(-1);
	if ((stbuf.st_mode & S_IFMT) != S_IFDIR) {
		errno = ENOTDIR;
		return(-1);
	}

	/* see if a rmdir is likely to succeed */
	for (p = path + strlen(path) - 1; p >= path; p--)
		if (*p == '/')
			break;
	if (p > path) {
		n = p - path;
		(void) sprintf(dir, "%.*s", MIN(n, sizeof dir - 1), path);
		if (tet_eaccess(dir, 02) < 0)
			return(-1);
	}

	/* we must remove it the hard way */
	if ((rc = hardmkrmdir("rmdir", path, 0777)) < 0)
		if (rc == -1)
			errno = ENOENT;
		else
			rc = -1;

#else /* NOMKDIR */

	/* easy - we have rmdir(2) */
	rc = rmdir(path);

	/* band-aid for non-POSIX systems */
#ifdef ENOTEMPTY
	if (rc < 0 && errno == ENOTEMPTY)
		errno = EEXIST;
#endif /* ENOTEMPTY */

#endif /* NOMKDIR */

	return(rc);
}

#ifdef NOMKDIR

/*
**	hardmkrmdir() - fork and exec mkdir or rmdir
**
**	return   0 if the program succeeded
**		-1 if it failed
**		-2 for some other error
*/

static int hardmkrmdir(prog, path, mode)
char *prog, *path;
int mode;
{
	char *argv[3];
	register int pid, rc;
	int status, save_errno;
	char msg[32];

	switch (pid = dofork()) {
	case 0:
		argv[0] = prog;
		argv[1] = path;
		argv[2] = (char *) 0;
		(void) umask(~(mode & ~umask(0)) & 077);
		(void) execvp(prog, argv);
		(void) sprintf(msg, "can't exec: %s", prog);
		error(errno, msg, path);
		_exit(~0);
		/* NOTREACHED */
	case -1:
		save_errno = errno;
		(void) sprintf(msg, "can't fork: %s", prog);
		error(errno, msg, path);
		errno = save_errno;
		return(-2);
	default:
		status = 0;
		while ((rc = wait(&status)) > 0)
			if (rc == pid)
				break;
		if (rc < 0) {
			save_errno = errno;
			(void) sprintf(msg, "wait failed: %s", prog);
			error(errno, prog, msg);
			errno = save_errno;
			return(-2);
		}
		if (status & 0xffff) {
			if ((status & 0xff00) != 0xff00) {
				(void) sprintf(msg, "%s failed:", prog);
				error(0, msg, path);
			}
			return(-1);
		}
		break;
	}

	return(0);
}

#endif /* NOMKDIR */

