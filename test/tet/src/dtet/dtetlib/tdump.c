/*
 *      SCCS:  @(#)tdump.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tdump.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tdump.c	1.2 92/09/30
NAME:		tdump.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	trace interface to hexdump()

MODIFICATIONS:

************************************************************************/

#ifndef NOTRACE

#include <stdio.h>
#include <errno.h>
#include "dtmac.h"
#include "trace.h"
#include "dtetlib.h"

/*
**	tdump() - print a memory dump to the trace file
*/

void tdump(from, len, title)
char *from, *title;
int len;
{
	register int save_errno = errno;
	extern FILE *Tfp;

	if (!Tfp)
		tfopen();

	(void) fprintf(Tfp, "%s:\n", title && *title ? title : "data dump:");

	hexdump(from, len, Tfp);

	errno = save_errno;
}

#endif /* NOTRACE */

