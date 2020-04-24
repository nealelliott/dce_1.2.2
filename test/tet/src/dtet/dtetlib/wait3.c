/*
 *      SCCS:  @(#)wait3.c	1.3 (92/12/01) 
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
static char sccsid[] = "@(#)wait3.c	1.3 (92/12/01) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)wait3.c	1.3 92/12/01
NAME:		wait3.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to wait for child process to terminate, possibly without
	blocking

MODIFICATIONS:
	Andrew Dingwall, UniSoft Ltd., November 1992
	Made call to waitpid() the default.

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <sys/types.h>
#include <sys/wait.h>
#include "dtetlib.h"

/*
**	dowait3() - call wait3() or waitpid()
**
**	return	pid of waited-for process,
**		0 if options indicates a non-blocking wait and there are no
**			un-waited-for children
**		-1 on error
**
**	if successful, the exit status of the child process is returned
**	indirectly through *statp
*/

int dowait3(statp, options)
int *statp, options;
{

#if defined(SVR2) || defined(BSD42) || defined(BSD43)

	register int rc;
	union wait status;

	if ((rc = wait3(&status, options, 0)) > 0)
		*statp = status.w_status;
	return(rc);

#else /* POSIX-conforming systems */

	return(waitpid(-1, statp, options));

#endif

}

