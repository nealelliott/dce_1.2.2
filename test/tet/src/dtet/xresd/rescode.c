/*
 *      SCCS:  @(#)rescode.c	1.3 (92/11/10) 
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
static char sccsid[] = "@(#)rescode.c	1.3 (92/11/10) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)rescode.c	1.3 92/11/10
NAME:		rescode.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		David Sawyer, UniSoft Ltd.
DATE CREATED:	August 1992

DESCRIPTION:
	open result codes file and generate table.
	
	return result codes string matching result code etc.

	This file is based on parts of apilib/dresfile.c

MODIFICATIONS:

************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dtet/tet_api.h>
#include "error.h"
#include "xresd.h"


#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

struct rescode_tab {
	char *name;
	int code;
	int abrt;
} restab_dflt[] = {
	/* default names for TET defined codes */
	"PASS",		TET_PASS,	0,
	"FAIL",		TET_FAIL,	0,
	"UNRESOLVED",	TET_UNRESOLVED,	0,
	"NOTINUSE",	TET_NOTINUSE,	0,
	"UNSUPPORTED",	TET_UNSUPPORTED,0,
	"UNTESTED",	TET_UNTESTED,	0,
	"UNINITIATED",	TET_UNINITIATED,0,
	"NORESULT",	TET_NORESULT,	0,
	NULL,		-1,		0
};



/* This just for the static functions in ths file */
#if __STDC__
static int read_codes( struct rescode_tab **);
static int skipline( char *);
#else
static int read_codes();
static int skipline();
#endif



static int
read_codes(tabp)
struct rescode_tab **tabp;
{
	/* get result codes from tet_code file */

	FILE *fp;
	struct rescode_tab *restab;
	char *fname, *cp, *endp;
	int err, line, nres, ires;
	char buf[BUFSIZ];

	/* Get the result codes file name */
	fname = rcodesf();
	if (fname == NULL || *fname == '\0')
	{
		error(0,"could not get result codes file name", (char *) 0);
		return -1;
	}

	if ((fp = fopen(fname, "r")) == NULL)
	{
		err = errno;
		(void) sprintf(buf, "could not open result code file \"%s\"",
			fname);
		error(err, buf, (char *) 0);
		return -1;
	}

	/* count the number of result codes in the file */
	nres = 0;
	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		if (!skipline(buf))
			nres++;
	}
	
	/* allocate space for structures */
	*tabp = (struct rescode_tab *)malloc(
			(size_t)((nres + 1) * sizeof(**tabp)));
	if (*tabp == NULL)
	{
		error(errno, "malloc() failed in read_codes()", (char *) 0);
		(void) fclose(fp);
		return -1;
	}
	restab = *tabp;

	/* now read each line and place the info in the structure */
	rewind(fp);
	ires = -1;
	for (line = 1; ires < nres && fgets(buf, sizeof(buf), fp) != NULL;
		line++)
	{
		if (skipline(buf))
			continue;

		ires++;

		/* first field is code number */
		restab[ires].code = atoi(buf);

		/* put temporary values in other fields */
		restab[ires].name = "INVALID RESULT";
		restab[ires].abrt = 0;

		/* second field is quote-delimited name */
		cp = strchr(buf, '"');
		if (cp != NULL)
		{
			cp++;
			endp = strchr(cp, '"');
		}
		if (cp == NULL || endp == NULL)
		{
			(void) sprintf(buf,
				"quotes missing on line %d of \"%s\"",
				line, fname);
			error(0, buf, (char *) 0);
			continue;
		}
		restab[ires].name = (char *)malloc((size_t)(endp-cp+1));
		if (restab[ires].name == NULL)
		{
			error(errno, "malloc() failed in read_codes()", (char *) 0);
			break;
		}
		(void) strncpy(restab[ires].name, cp, (size_t)(endp-cp));
		restab[ires].name[endp-cp] = '\0';

		/* last field is action: Continue (default) or Abort */
		endp++;
		while (*endp == ' ' || *endp == '\t')
			endp++;
		if (*endp == '\n' || strcmp(endp, "Continue\n") == 0)
			restab[ires].abrt = 0;
		else if (strcmp(endp, "Abort\n") == 0)
			restab[ires].abrt = 1;
		else
		{
			(void) sprintf(buf,
				"invalid action field on line %d of \"%s\"",
				line, fname);
			error(0, buf, (char *) 0);
		}
	}

	/* add null terminator */
	restab[++ires].name = NULL;

	(void) fclose(fp);
	return 0;
}


char *
get_code(result, abortflag)
int result;
int *abortflag;
{
	/* look up result code in rescodes structure, return name if
	   found, otherwise NULL.  If abortflag is not NULL then set
	   (*abortflag) to true if corresponding action is to abort */

	char *res;
	int i;
	static struct rescode_tab *restab = NULL;

	if (restab == NULL)
	{
		if (read_codes(&restab) != 0)
			restab = restab_dflt;
	}

	res = NULL;
	for (i = 0; restab[i].name != NULL; i++)
	{
		if (restab[i].code == result)
		{
			if (abortflag != (int *) NULL)
				*abortflag = restab[i].abrt;
			res = restab[i].name;
			break;
		}
	}

	/* if not found in supplied file, use default name */

	if (res == NULL && restab != restab_dflt)
	{
		for (i = 0; restab_dflt[i].name != NULL; i++)
		{
			if (restab_dflt[i].code == result)
			{
				if (abortflag != (int *) NULL)
					*abortflag = restab_dflt[i].abrt;
				res = restab_dflt[i].name;
				break;
			}
		}
	}

	return res;
}


static int
skipline(linebuf)
char *linebuf;
{
        /* return true if input line is to be skipped (comment or blank line) */

        char *cp;

        for (cp = linebuf; *cp == ' ' || *cp == '\t'; cp++)
                ;
        return (*cp == '#' || *cp == '\n' || *cp == '\0');
}
