/*
 *      SCCS:  @(#)bufchk.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)bufchk.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)bufchk.c	1.2 92/09/30
NAME:		bufchk.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	buffer check and (re)allocation function

MODIFICATIONS:

************************************************************************/


#if __STDC__
#include <stdio.h>
#endif

#include <errno.h>
#include "dtmac.h"
#include "error.h"
#include "ltoa.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#else
extern char *malloc(), *realloc();
#endif

/*
**	bufchk() - check that there is enough space in a data buffer
**		pointed to by *bpp, length at *lp, and grow it if necessary
**
**	return 0 if successful or -1 on error
*/

int bufchk(bpp, lp, newlen)
register char **bpp;
register int *lp, newlen;
{
	register char *bp;

#ifndef NOTRACE
	/* the assert was done in buftrace.c */
#else
	ASSERT(newlen >= 0);
#endif

	if (*lp >= newlen)
		return(0);

	errno = 0;
	if (*bpp == (char *) 0) {
		TRACE1(Tbuf, 8, "allocate new buffer");
		bp = malloc((unsigned) newlen);
	}
	else {
		TRACE1(Tbuf, 8, "grow existing buffer");
		bp = realloc(*bpp, (unsigned) newlen);
	}

	if (bp) {
		*bpp = bp;
		*lp = newlen;
		return(0);
	}

	/* here if malloc/realloc failed */
	error(errno, "can't grow data buffer, wanted", itoa(newlen));
	if (*bpp) {
		errno = 0;
		if ((bp = realloc(*bpp, (unsigned) *lp)) == (char *) 0) {
			error(errno, "can't realloc old data buffer",
				(char *) 0);
			*lp = 0;
		}
		else
			TRACE2(Tbuf, 8, "realloc old buffer at %s", itox(bp));
		*bpp = bp;
	}
	else
		*lp = 0;

	return(-1);
}

