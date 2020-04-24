/*
 *      SCCS:  @(#)ltoa.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)ltoa.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)ltoa.c	1.2 92/09/30
NAME:		ltoa.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to convert long int to ascii
	avoids having to use sprintf to format error messages

MODIFICATIONS:

************************************************************************/

#include "ltoa.h"

/*
**	ltoa() - convert long int to ascii
*/

char *ltoa(n)
register long n;
{
	static char buf[NLBUF][LNUMSZ];
	static int count;
	register char *p;
	register int sign;

	if (n < 0L) {
		sign = 1;
		n = -n;
	}
	else
		sign = 0;

	if (++count >= NLBUF)
		count = 0;
	p = &buf[count][LNUMSZ - 1];
	*p = '\0';

	do {
		*--p = (n % 10L) + '0';
	} while ((n /= 10L) != 0L);

	if (sign)
		*--p = '-';

	return(p);
}

