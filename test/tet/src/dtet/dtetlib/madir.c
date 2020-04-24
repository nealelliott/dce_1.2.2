/*
 *      SCCS:  @(#)madir.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)madir.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)madir.c	1.2 92/09/30
NAME:		madir.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	June 1992

DESCRIPTION:
	recursive directory creation function

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "dtmac.h"
#include "error.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/* static function declarations */
#if __STDC__
static int mkad2(char *);
#else /* __STDC__ */
static int mkad2();
#endif /* __STDC__ */


/*
**	mkalldirs() - make directories as necessary in a directory path
**
**	return 0 if successful, -1 otherwise
*/

int mkalldirs(path)
char *path;
{
	register int rc;
	struct stat stbuf;
	char buf[MAXPATH + 1];

	if (stat(path, &stbuf) < 0)
		if (errno == ENOENT) {
			(void) sprintf(buf, "%.*s", sizeof buf - 1, path);
			rc = mkad2(buf);
		}
		else {
			error(errno, "unexpected stat fail:", path);
			rc = -1;
		}
	else
		rc = 0;

	return(rc);
}

/*
**	mkad2() - make directories as necessary when the last component in the
**	path is known not to exist
**
**	return 0 if successful, -1 otherwise
*/

static int mkad2(path)
char *path;
{
	register char *p;
	register int rc;
	struct stat stbuf;

	ASSERT(*path);

	/* find the last / character (if any) */
	for (p = path + strlen(path) - 1; p >= path; p--)
		if (*p == '/')
			break;

	/* if found:
		replace it with a \0 
		if the parent directory does not exist, create that as well
		restore the / */
	if (p >= path) {
		*p = '\0';
		if (stat(path, &stbuf) < 0)
			rc = mkad2(path);
		else if ((stbuf.st_mode & S_IFMT) != S_IFDIR) {
			error(ENOTDIR, path, (char *) 0);
			rc = -1;
		}
		else
			rc = 0;
		*p = '/';
		if (rc < 0)
			return(rc);
	}

	/* finally, create the directory on this level */
	if ((rc = domkdir(path, 0777)) < 0)
		error(errno, "can't make directory", path);

	return(rc);
}

