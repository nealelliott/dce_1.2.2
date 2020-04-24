/*
 *      SCCS:  @(#)prerror.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)prerror.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)prerror.c	1.2 92/09/30
NAME:		prerror.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	error message formatting and printing function

MODIFICATIONS:

************************************************************************/

#include <stdio.h>
#include <errno.h>
#include "dtetlib.h"

/*
**	prerror() - format and print an error message
*/

void prerror(fp, errnum, hdr, file, line, s1, s2)
FILE *fp;
int errnum, line;
char *hdr, *file, *s1, *s2;
{
	extern int sys_nerr;
	extern char *sys_errlist[];

	(void) fprintf(fp, "%s (%s, %d): %s", hdr, file, line, s1);
	if (s2 && *s2)
		(void) fprintf(fp, " %s", s2);
	if (errnum > 0 && errnum < sys_nerr)
		(void) fprintf(fp, ": %s", sys_errlist[errnum]);
	(void) putc('\n', fp);
	(void) fflush(fp);

	errno = 0;
}

