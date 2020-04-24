/*
 *      SCCS:  @(#)tcrsys.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tcrsys.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tcrsys.c	1.2 92/09/30
NAME:		tcrsys.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	June 1992

DESCRIPTION:
	common tc_talk interface for TCCD functions that behave like
	remote system calls

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <errno.h>
#include "dtmsg.h"
#include "error.h"
#include "servlib.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif


/*
**	tc_rsys() - send remote system call message to TCCD
**		and receive a reply
**
**	return 0 if successful or -1 on error
**
**	if -1 is returned, the local errno is set to the equivalent of
**	the remote errno value if possible
*/

int tc_rsys(sysid, request)
int sysid, request;
{
	register char *dp;
	extern char Tcerrmsg[];

	/* send the request and receive the reply */
	dp = tc_talk(sysid, request, TALK_DELAY);

	/* handle the return codes */
	if (tc_errno == ER_OK)
		return(0);
	else if ((errno = unmaperrno(tc_errno)) == 0)
		switch (tc_errno) {
		case ER_ERR:
			if (!dp)
				break;
			/* else fall through */
		default:
			error(0, Tcerrmsg, ptrepcode(tc_errno));
			break;
		}

	/* here for server error return */
	return(-1);
}

