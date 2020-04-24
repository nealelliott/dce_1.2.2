/*
 *      SCCS:  @(#)tckill.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tckill.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tckill.c	1.2 92/09/30
NAME:		tckill.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to kill a remote process

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <errno.h>
#include "dtmsg.h"
#include "valmsg.h"
#include "error.h"
#include "servlib.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	tc_kill() - send an OP_KILL message to STCC and receive a reply
**
**	return 0 if successful or -1 on error
*/

int tc_kill(sysid, pid, signum)
int sysid, signum;
long pid;
{
	register struct valmsg *mp;
	register int remsig;
	extern char Tcerrmsg[];

	/* convert the signal number to its machine-independent value */
	if ((remsig = mapsignal(signum)) < 0) {
		tc_errno = ER_ERR;
		return(-1);
	}

	/* get the STCC message buffer */
	if ((mp = (struct valmsg *) tc_msgbuf(sysid, valmsgsz(OP_KILL_NVALUE))) == (struct valmsg *) 0) {
		tc_errno = ER_ERR;
		return(-1);
	}

	/* set up the request message */
	mp->vm_nvalue = OP_KILL_NVALUE;
	VM_PID(mp) = pid;
	VM_SIGNUM(mp) = remsig;

	/* perform the conversation and handle the reply codes */
	mp = (struct valmsg *) tc_talk(sysid, OP_KILL, TALK_DELAY);
	switch (tc_errno) {
	case ER_OK:
		return(0);
	case ER_PID:
	case ER_SIGNUM:
	case ER_PERM:
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

