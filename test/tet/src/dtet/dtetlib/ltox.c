/*
 *      SCCS:  @(#)ltox.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)ltox.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)ltox.c	1.2 92/09/30
NAME:		ltox.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to convert long int to hex ascii
	avoids having to use sprintf to format error messages

MODIFICATIONS:

************************************************************************/

#include "ltoa.h"

/*
**	ltox() - convert long int to hex ascii
*/

char *ltox(n)
long n;
{
	static char buf[NLBUF][LXNUMSZ];
	static int count;
	register char *p;
	register char c;
	register unsigned long u;

	if (++count >= NLBUF)
		count = 0;
	p = &buf[count][LXNUMSZ - 1];
	*p = '\0';

	if ((u = (unsigned long) n) != 0) {
		do {
			c = u & 0xf;
			*--p = c + (c >= 10 ? 'a' - 10 : '0');
		} while ((u >>= 4) != 0L);
		*--p = 'x';
	}

	*--p = '0';

	return(p);
}

