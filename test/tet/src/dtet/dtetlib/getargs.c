/*
 *      SCCS:  @(#)getargs.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)getargs.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)getargs.c	1.2 92/09/30
NAME:		getargs.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to split a string into fields

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <ctype.h>
#include "dtetlib.h"

/*
**	getargs() - split a string up into at most maxargs fields,
**		discarding excess fields
**
**	return the number of fields found
*/

int getargs(s, argv, maxargs)
register char *s, **argv;
register int maxargs;
{
	register int argc, new;

	for (argc = 0, new = 1; *s; s++)
		if (isspace(*s)) {
			*s = '\0';
			new = 1;
			if (argc >= maxargs)
				break;
		}
		else if (new && argc++ < maxargs) {
			*argv++ = s;
			new = 0;
		}

	return(argc);
}

