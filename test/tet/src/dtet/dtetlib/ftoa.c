/*
 *      SCCS:  @(#)ftoa.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)ftoa.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)ftoa.c	1.2 92/09/30
NAME:		ftoa.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	June 1992

DESCRIPTION:
	function to return a printable representation of a set of flags

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <sys/types.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "ltoa.h"
#include "ftoa.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	ftoa() - return printable representation of ptab pt_flags value
**
**	the return value points to a static area whose contents are
**	overwritten by each call
*/

char *ftoa(fval, flags, nflags)
int fval, nflags;
struct flags flags[];
{
	static char *bp;
	static int buflen;
	register struct flags *fp;
	register char *p1, *p2;
	register unsigned ftmp;
	register int n, needlen;

	/* work out the required output buffer size */
	for (needlen = 0, ftmp = fval, n = 0; ftmp; ftmp >>= 1, n++) {
		if (!(ftmp & 1))
			continue;
		for (fp = &flags[nflags - 1]; fp >= flags; fp--)
			if (fp->fl_value == (1 << n)) {
				needlen += strlen(fp->fl_name) + 1;
				break;
			}
		if (fp < flags)
			needlen += strlen(itoo(1 << n)) + 1;
	}

	/* get the buffer to put the flag names in */
	if (BUFCHK(&bp, &buflen, MAX(needlen, 2)) < 0)
		return("");

	/* copy the flag names in to the buffer */
	for (p1 = bp, ftmp = fval, n = 0; ftmp; ftmp >>= 1, n++) {
		if (!(ftmp & 1))
			continue;
		for (fp = &flags[nflags - 1]; fp >= flags; fp--)
			if (fp->fl_value == (1 << n)) {
				for (p2 = fp->fl_name; *p2; p2++)
					*p1++ = *p2;
				break;
			}
		if (fp < flags)
			for (p2 = itoo(1 << n); *p2; p2++)
				*p1++ = *p2;
		if (ftmp & ~1)
			*p1++ = '|';
	}

	if (p1 == bp)
		*p1++ = '0';

	*p1 = '\0';

	return(bp);
}

