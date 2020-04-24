/*
 *      SCCS:  @(#)strstore.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)strstore.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)strstore.c	1.2 92/09/30
NAME:		strstore.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to allocate string storage in static memory

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <errno.h>
#include "dtmac.h"
#include "error.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

#ifndef NOTRACE
#include "ltoa.h"
#endif

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#else
extern char *malloc();
#endif /* __STDC__ */
#if defined(BSD42) || defined(BSD43) || defined(ultrix)
#include <strings.h>
#else
#include <string.h>
#endif


/*
**	strstore() - store a string in memory obtained from malloc and return
**		a pointer thereto
**
**	return (char *) 0 on error
*/

char *strstore(s)
char *s;
{
	register unsigned len;
	register char *p;

	len = strlen(s) + 1;

	errno = 0;
	if ((p = malloc(len)) == (char *) 0)
		error(errno, "can't get memory for string:", s);
	else
		(void) strcpy(p, s);

	TRACE4(Tbuf, 6, "strstore(\"%.20s%s\") returns %s",
		s, len >= 20 ? " ..." : "", itox(p));
	return(p);
}

