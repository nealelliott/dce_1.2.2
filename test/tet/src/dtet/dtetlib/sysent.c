/*
 *      SCCS:  @(#)sysent.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)sysent.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)sysent.c	1.2 92/09/30
NAME:		sysent.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	systems file search function

MODIFICATIONS:

************************************************************************/

#include <stdio.h>
#include <errno.h>
#include "sysent.h"
#include "dtmac.h"
#include "error.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

static FILE *sfp = NULL;		/* systems file stream pointer */

/*
**	getsysent() - get an entry from the systems file
**		and return a pointer thereto
**
**	return (struct sysent *) 0 on error
**
**	warning: the string pointed to by sy.sy_name is stored in the
**	static buffer owned by fgetargs() so, if it is needed, it should be
**	copied to somewhere less exposed soon after a call to getsysent()
*/

struct sysent *getsysent()
{
	static struct sysent sysent;
	char *args[2];
	register int rc;

	if (sfp == NULL && setsysent() < 0)
		return((struct sysent *) 0);

	rc = 0;
	while (rc != 2)
		if ((rc = fgetargs(sfp, args, 2)) == EOF)
			return((struct sysent *) 0);

	sysent.sy_sysid = atoi(args[0]);
	sysent.sy_name = args[1];

	return(&sysent);
}

/*
**	setsysent() - rewind to the start of the systems file, opening it
**		if necessary
**
**	return 0 if successful or -1 on error
*/

int setsysent()
{
	static char file[] = "systems";
	char path[MAXPATH + 1];
	extern char tet_root[];

	if (sfp != NULL) {
		rewind(sfp);
		return(0);
	}

	(void) sprintf(path, "%.*s/%s",
		sizeof path - sizeof file - 1, tet_root, file);

	if ((sfp = fopen(path, "r")) == NULL) {
		error(errno, "can't open", path);
		return(-1);
	}

	(void) fioclex(fileno(sfp));
	return(0);
}

/*
**	endsysent() - close the systems file
*/

void endsysent()
{
	if (sfp != NULL) {
		(void) fclose(sfp);
		sfp = NULL;
	}
}

