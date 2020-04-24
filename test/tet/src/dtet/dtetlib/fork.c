/*
 *      SCCS:  @(#)fork.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)fork.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)fork.c	1.2 92/09/30
NAME:		fork.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to fork a new process

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include "dtmac.h"
#include "dtetlib.h"

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <unistd.h>
#else
extern unsigned sleep();
#endif

/*
**	dofork() - try to fork a few times until successful
**
**	return fork() return value
*/

int dofork()
{
	register int rc, try;

	for (try = 0; (rc = fork()) < 0 && try < 5; try++)
		(void) sleep((unsigned) MAX(1 << try, 2));

	return(rc);
}

