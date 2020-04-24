/*
 *      SCCS:  @(#)tfname.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tfname.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tfname.c	1.2 92/09/30
NAME:		tfname.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to generate a temporary file name with some guarantee that it
	will be useful

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <fcntl.h>
#include <errno.h>
#include "dtmac.h"
#include "error.h"
#include "ltoa.h"
#include "bstring.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

static char *dirs[] = { (char *) 0, "/usr/tmp", "/tmp", (char *) 0 };
static char **tmpdirs;
static char salt[] = "\000AAA";

/* tryone() return codes */
#define TR_OK		1	/* file could be created */
#define TR_NEXTDIR	2	/* try again in next directory */
#define TR_NEXTFNAME	3	/* try again with next file name */
#define TR_ERROR	-1	/* error - give up */

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#else
extern char *getenv(), *malloc();
extern void free();
#endif

/* static function declarations */
#if __STDC__
static int tryone(char *, char *, char **);
#else
static int tryone();
#endif


/*
**	mktfname() - generate a temporary file name and return a pointer
**		thereto
**
**	return (char *) 0 on error
*/

char *mktfname(prefix)
char *prefix;
{
	register char **tdp;
	register char *p;
	register int rc;
	char *fname;

	/* do initial setup first time through */
	if (!tmpdirs) {
		if ((p = getenv("TMPDIR")) == (char *) 0 || !*p)
			tmpdirs = &dirs[1];
		else {
			dirs[0] = p;
			tmpdirs = &dirs[0];
		}
	}

	/* generate the file name */
	for (;;) {
		if (salt[0]) {
			error(0, "out of tmp file names", (char *) 0);
			break;
		}
		for (tdp = tmpdirs; *tdp; tdp++)
			if ((rc = tryone(*tdp, prefix, &fname)) != TR_NEXTDIR)
				break;
		if (rc != TR_OK && rc != TR_NEXTFNAME)
			break;
		for (p = &salt[sizeof salt - 2]; p >= salt; p--)
			if (++*p > 'Z')
				*p = 'A';
			else
				break;
		if (rc == TR_OK)
			return(fname);
	}

	/* here to return after an error */
	return((char *) 0);
}

/*
**	tryone() - try to create a temporary file
**
**	return TR_* return code to indicate success or failure
**	if successful, the generated file name is stored indirectly through *np
**
**	we try to make sure that there is a reasonable chance of being able to
**	write some data to the file
*/

static int tryone(dir, prefix, np)
char *dir, *prefix;
char **np;
{
	register char *fname, *pidstr;
	register unsigned needlen;
	register int fd, n, rc;
	char buf[1024];
	extern int Mypid;

	/* work out how big a buffer is needed */
	pidstr = itoa(Mypid);
	needlen = strlen(dir) + strlen(prefix) + sizeof salt + strlen(pidstr);

	/* get a buffer for the file name */
	errno = 0;
	if ((fname = malloc(needlen)) == (char *) 0) {
		error(errno, "can't get tmp file name buffer", (char *) 0);
		return(TR_ERROR);
	}
	TRACE2(Tbuf, 6, "allocate tfname = %s", itox(fname));

	/* generate the file name */
	(void) sprintf(fname, "%s/%s%s%s", dir, prefix, &salt[1], pidstr);

	/* try to open the file */
	if ((fd = open(fname, O_RDWR | O_CREAT | O_EXCL, 0666)) < 0)
		switch (errno) {
		case EMFILE:
		case ENFILE:
			/* caller will have to take his chances! */
			rc = TR_OK;
			break;
		case EEXIST:
		case EISDIR:
		case ENXIO:
			rc = TR_NEXTFNAME;
			break;
		default:
			rc = TR_NEXTDIR;
			break;
		}
	else {
		/* try to write a reasonable amount of data to the file */
		bzero(buf, sizeof buf);
		rc = TR_OK;
		for (n = 0; n < 10; n++)
			if (write(fd, buf, sizeof buf) != sizeof buf) {
				rc = TR_NEXTDIR;
				break;
			}
		/* close the file and unlink it */
		(void) close(fd);
		if (unlink(fname) < 0)
			error(errno, "can't unlink", fname);
	}

	/* store the file name if ok, otherwise free the buffer */
	if (rc == TR_OK)
		*np = fname;
	else {
		TRACE2(Tbuf, 6, "free tfname = %s", itox(fname));
		free(fname);
	}

	return(rc);
}

