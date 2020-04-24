/*
 *      SCCS:  @(#)tcexec.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tcexec.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tcexec.c	1.2 92/09/30
NAME:		tcexec.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to perform a remote execution request

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include "dtmsg.h"
#include "avmsg.h"
#include "valmsg.h"
#include "error.h"
#include "servlib.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif


/*
**	tc_exec() - send OP_EXEC message to STCC and receive a reply
**
**	return pid of exec'd process if successful or -1 on error
*/

long tc_exec(sysid, path, argv, outfile, snid, xrid, flag)
int sysid, flag;
char *path, **argv, *outfile;
long snid, xrid;
{
	register char *dp;
	register char **ap;
	register int n, nargs;
	extern char Tcerrmsg[];

	/* make sure that path and argv are non-null */
	if (!path || !*path || !argv) {
		tc_errno = ER_INVAL;
		return(-1L);
	}

	/* count the arguments */
	for (ap = argv; *ap; ap++)
		;
	nargs = ap - argv;

	/* get the STCC message buffer */
	if ((dp = tc_msgbuf(sysid, avmsgsz(OP_EXEC_ARGC(nargs)))) == (char *) 0) {
		tc_errno = ER_ERR;
		return(-1L);
	}

#define mp	((struct avmsg *) dp)

	/* set up the request message */
	mp->av_argc = OP_EXEC_ARGC(nargs);
	AV_FLAG(mp) = flag;
	AV_SNID(mp) = snid;
	AV_XRID(mp) = xrid;
	AV_PATH(mp) = path;
	AV_OUTFILE(mp) = outfile;
	for (ap = argv, n = 0; n < nargs; ap++, n++)
		AV_ARG(mp, n) = *ap;

#undef mp

	/* send the request and receive the reply */
	dp = tc_talk(sysid, OP_EXEC, TALK_DELAY);

#define rp	((struct valmsg *) dp)

	/* handle the return codes */
	switch (tc_errno) {
	case ER_OK:
		return(VM_PID(rp));
	case ER_FORK:
	case ER_NOENT:
	case ER_INVAL:
		break;
	case ER_ERR:
		if (!dp)
			break;
		/* else fall through */
	default:
		error(0, Tcerrmsg, ptrepcode(tc_errno));
		break;
	}

#undef rp

	/* here for server error return */
	return(-1L);
}

