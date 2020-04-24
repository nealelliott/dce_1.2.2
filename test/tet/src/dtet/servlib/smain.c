/*
 *      SCCS:  @(#)smain.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)smain.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)smain.c	1.2 92/09/30
NAME:		smain.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	generic server main() function

MODIFICATIONS:

************************************************************************/

#include "dtmac.h"
#include "error.h"
#include "server.h"
#include "servlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	si_main() - generic server main processing function
**
**	return 0 if successful or 1 (for process exit code) on error
*/

int si_main(argc, argv, needtetroot)
int argc, needtetroot;
char **argv;
{
	register char *p;
	register int rc;
	char buf[30];
	extern char *Progname, *Tet_root;
	extern int Mypid;

	Mypid = getpid();

#ifndef NOTRACE
	traceinit(argc, argv);
#endif

	while (++argv, --argc > 0) {
		if (*(p = *argv) != '-')
			break;
		switch (*++p) {
		case 'T':
			break;
		default:
			if (ss_argproc(*argv, argc > 1 ? *(argv + 1) : (char *) 0) > 0) {
				--argc;
				++argv;
			}
			break;
		}
	}

	if (needtetroot) {
		if (argc > 0)
			Tet_root = *argv;
		else {
			(void) sprintf(buf, "%.14s [options] tetrootdir",
				Progname);
			fatal(0, "usage:", buf);
		}
	}

	/* perform server-specific daemon initialisation */
	ss_initdaemon();

	/* start main processing loop */
	while ((rc = ss_serverloop()) > 0)
		;

	return(-rc);
}

