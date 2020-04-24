/*
 *      SCCS:  @(#)child.c	1.1 (92/10/02) 
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
static char sccsid[] = "@(#)child.c	1.1 (92/10/02) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)child.c	1.1 92/10/02
NAME:		child.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	October 1992

DESCRIPTION:
	main() function for processes started by tet_exec() and tet_remexec()

	this code has moved here from rtcmfuncs.c, so as to allow use
	when building dtcmchild.o as well as tcmrem.o

MODIFICATIONS:

************************************************************************/

/* band-aid for non-posix systems */
#if defined(SVR2) || defined(BSD42) || defined(BSD43)
typedef int pid_t;
#endif

#if __STDC__
#include <stdio.h>
#endif

#include "dtmsg.h"
#include "error.h"
#include "synreq.h"
#include "dtetlib.h"
#include "tcmfuncs.h"
#include "dapi.h"
#include "dtet/tet_api.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/* global variables, values inhereted from tet_exec() and tet_remexec() */
int tet_thistest = -1;
extern long tet_activity;
extern long tet_context;
extern long tet_block;
char *tet_pname;

/* local function declarations */
#if __STDC__
int main(int, char **);
#else /* __STDC__ */
int main();
#endif /* __STDC__ */


int main(argc, argv)
int argc;
char **argv;
{
	register char **ap1, **ap2;
	register int rc;

	if (argc < 5)
		fatal(0, "argument count wrong; process must be executed by",
			tet_callfuncname());

	/* set the global variables from the command line */
	tet_thistest = atoi(argv[0]);
	tet_activity = atoi(argv[1]);
	tet_context = atoi(argv[2]);
	tet_block = atoi(argv[3]);

	/* shuffle the other arguments down */
	argc -= 4;
	ap1 = argv + 4;
	ap2 = argv;
	do {
		*ap2++ = *ap1++;
	} while (*ap1);
	tet_pname = argv[0];

	/* initialise the server and transport stuff */
	tcminit(argc, argv);

	/* start a new block and read in the config variables */
	tet_setblock();
	tet_config();

	/* call the user-supplied program */
	rc = tet_main(argc, argv);

	/* log off all the servers and exit */
	tet_logoff();
	return(rc);
}

/*
**	ti_tcminit() - transport-independent initialisation for child TCM
*/

void ti_tcminit()
{
	/* nothing */
}

