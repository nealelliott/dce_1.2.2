/*
 *      SCCS:  @(#)remvar.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)remvar.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)remvar.c	1.2 92/09/30
NAME:		remvar.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	June 1992

DESCRIPTION:
	function to strip TET_REMnnn_ prefix from remote config variable
	assignment

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <ctype.h>
#include "dtetlib.h"

/*
**	remvar() - process remote config variable assignment
**
**	if the name part starts with the prefix TET_REMnnn_ and nnn matches
**	sysid, remvar() returns a pointer to the start of the rest of the
**	assignment string following the TET_REMnnn_ prefix
**
**	if the name part does not start with the prefix TET_REMnnn_ or nnn
**	does not match sysid, remvar() returns its first argument
**
**	if sysid is -1, any sysid is matched
**
**	remvar() returns (char *) 0 if the variable name is malformed
*/

char *remvar(s, sysid)
register char *s;
{
	register char *p;
	register int sys;
	static char fmt[] = "TET_REM";

	/* see if this is a TET_REM variable */
	if (strncmp(s, fmt, sizeof fmt - 1))
		return(s);

	/* make p point past the "TET_REM" and extract the nnn part -
		we don't really mind how many digits there are */
	sys = 0;
	for (p = s + sizeof fmt - 1; *p && isdigit(*p); p++)
		sys = (sys * 10) + (*p & 017);

	/* next char should be '_'; skip over it */
	if (*p++ != '_')
		return((char *) 0);

	return((sysid == sys || sysid == -1) ? p : s);
}

