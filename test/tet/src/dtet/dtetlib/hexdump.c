/*
 *      SCCS:  @(#)hexdump.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)hexdump.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)hexdump.c	1.2 92/09/30
NAME:		hexdump.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	hex dump function
	might be useful when debugging

MODIFICATIONS:

************************************************************************/

#include <stdio.h>
#include "dtmac.h"
#include "dtetlib.h"

/*
**	hexdump() - print a dump of n bytes starting at dp
*/

void hexdump(dp, n, fp)
char *dp;
int n;
FILE *fp;
{
	register char *start, *end;
	register char *p1, *p2;

	end = dp + n;	/* end is first location not to dump */

	start = dp;
	do {
		(void) fprintf(fp, "%#lx:", (long) start);
		if (start >= end)
			continue;
		p2 = MIN(start + 16, end);
		for (p1 = start; p1 < p2; p1++)
			(void) fprintf(fp, " %02x", (unsigned char) *p1);
		while (p1++ <= start + 16)
			(void) fprintf(fp, "   ");
		for (p1 = start; p1 < p2; p1++)
			(void) fprintf(fp, "%c",
				*p1 > '\040' && *p1 < '\177' ? *p1 : '.');
		(void) fprintf(fp, "\n");
	} while ((start += 16) < end);

	(void) fprintf(fp, "\n");
	(void) fflush(fp);
}

