/*
 *      SCCS:  @(#)putenv.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)putenv.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)putenv.c	1.2 92/09/30
NAME:		putenv.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	environment manipulation function

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include "dtmac.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	dtputenv() - add an environment string to the environment
**
**	return 0 if successful or -1 on error
**
**	this routine is here because not all systems have putenv(3)
*/

int dtputenv(s)
char *s;
{
	static char **env;
	static int envlen;
	register char *p1, *p2;
	register char **ep1, **ep2;
	extern char **environ;

	/* see if the 'name' part is already in the environment */
	for (ep1 = environ; *ep1; ep1++) {
		for (p1 = *ep1, p2 = s; *p1 && *p2; p1++, p2++)
			if (*p1 != *p2 || *p1 == '=')
				break;
		if (*p1 == '=' && *p2 == '=') {
			*ep1 = s;
			return(0);
		}
	}

	/* not there so:
		see if we have been here before
		allocate a new environment space */
	ep2 = env;
	if (BUFCHK((char **) &env, &envlen, (int) (((ep1 - environ) + 2) * sizeof *env)) < 0)
		return(-1);

	/* make ep2 point to the end of the new environment,
		copy in the old environment first time through */
	if (ep2)
		ep2 = env + (ep1 - environ);
	else
		for (ep1 = environ, ep2 = env; *ep1; ep1++, ep2++)
			*ep2 = *ep1;

	/* add the new string to the new environment */
	*ep2++ = s;
	*ep2 = (char *) 0;
	environ = env;

	return(0);
}

