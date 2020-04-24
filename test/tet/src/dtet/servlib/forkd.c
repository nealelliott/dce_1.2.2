/*
 *      SCCS:  @(#)forkd.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)forkd.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)forkd.c	1.2 92/09/30
NAME:		fork.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to start a daemon process

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <errno.h>
#include "dtmac.h"
#include "error.h"
#include "servlib.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <unistd.h>
#else
extern void _exit();
#endif

/*
**	si_forkdaemon() - fork off a daemon process
*/

void si_forkdaemon()
{
	int rc;
	extern int Mypid;

	if ((rc = dofork()) < 0)
		fatal(errno, "can't fork", (char *) 0);
	else if (rc > 0)
		_exit(0);

	Mypid = getpid();
}

