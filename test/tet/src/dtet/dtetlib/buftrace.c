/*
 *      SCCS:  @(#)buftrace.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)buftrace.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)buftrace.c	1.2 92/09/30
NAME:		buftrace.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	trace interface to bufchk()

MODIFICATIONS:

************************************************************************/

#ifndef NOTRACE

#if __STDC__
#include <stdio.h>
#endif

#include "dtmac.h"
#include "error.h"
#include "ltoa.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	buftrace() - call bufchk, emit trace information
**
**	return 0 if successful or -1 on error
*/

int buftrace(bpp, lp, newlen, file, line)
char **bpp, *file;
int *lp, newlen, line;
{
	register int rc;

	TRACE6(Tbuf, 6,
		"call bufchk from %s, %s: buf = %s, len = %s, newlen = %s",
		file ? file : "??", itoa(line), itox(*bpp), itoa(*lp),
		itoa(newlen));

	ASSERT(newlen >= 0);
	if (*lp >= newlen) {
		TRACE1(Tbuf, 6, "buffer was big enough");
		return(0);
	}

	rc = bufchk(bpp, lp, newlen);

	TRACE2(Tbuf, 6, "new buffer = %s", itox(*bpp));
	return(rc);
}

#endif /* !NOTRACE */

