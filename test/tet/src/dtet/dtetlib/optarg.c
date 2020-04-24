/*
 *      SCCS:  @(#)optarg.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)optarg.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)optarg.c	1.2 92/09/30
NAME:		optarg.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to generate an option argument string

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include "dtetlib.h"

/*
**	mkoptarg() - make a - option string
**
**	return the number of (non-null) characters in the string
**
**	if first is false, the string starts with a space
*/

int mkoptarg(s, opt, arg, first)
char *s;
char opt;
register char *arg;
int first;
{
	register char *p = s;

	if (!first)
		*p++ = ' ';

	*p++ = '-';
	*p++ = opt;

	if (arg)
		while (*arg)
			*p++ = *arg++;

	*p = '\0';

	return(p - s);
}

