/*
 *      SCCS:  @(#)tcputenv.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tcputenv.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tcputenv.c	1.2 92/09/30
NAME:		tcputenv.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	May 1992

DESCRIPTION:
	function to put strings in the STCC environment

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include "dtmsg.h"
#include "avmsg.h"
#include "error.h"
#include "servlib.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif


/*
**	tc_putenv() - send a single line OP_PUTENV message to STCC and receive
**	a reply
**
**	return 0 if successful or -1 on error
*/

int tc_putenv(sysid, env)
int sysid;
char *env;
{
	return(tc_putenvv(sysid, &env, 1));
}

/*
**	tc_putenvv() - send a multi-line OP_PUTENV message to STCC and receive
**	a reply
**
**	return 0 if successful or -1 on error
*/

int tc_putenvv(sysid, envp, nenv)
int sysid;
register char **envp;
register int nenv;
{
	register struct avmsg *mp;
	register int n;
	extern char Tcerrmsg[];

	/* make sure that envp is non-zero and that nenv is +ve */
	if (!envp || nenv <= 0) {
		tc_errno = ER_INVAL;
		return(-1);
	}

	/* get the STCC message buffer */
	if ((mp = (struct avmsg *) tc_msgbuf(sysid, avmsgsz(OP_PUTENV_ARGC(nenv)))) == (struct avmsg *) 0) {
		tc_errno = ER_ERR;
		return(-1);
	}

	/* set up the request message */
	mp->av_argc = OP_PUTENV_ARGC(nenv);
	for (n = 0; n < nenv; n++)
		AV_ENVAR(mp, n) = *envp++;

	/* send the request and receive the reply */
	mp = (struct avmsg *) tc_talk(sysid, OP_PUTENV, TALK_DELAY);

	/* handle the return codes */
	switch (tc_errno) {
	case ER_OK:
		return(0);
	case ER_INVAL:
		break;
	case ER_ERR:
		if (!mp)
			break;
		/* else fall through */
	default:
		error(0, Tcerrmsg, ptrepcode(tc_errno));
		break;
	}

	/* here for server error return */
	return(-1);
}

