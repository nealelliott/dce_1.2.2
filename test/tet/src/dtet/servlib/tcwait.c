/*
 *      SCCS:  @(#)tcwait.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tcwait.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tcwait.c	1.2 92/09/30
NAME:		tcwait.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	wait for remote process to terminate

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
**	tc_wait() - send an OP_WAIT message to STCC and receive a reply
**
**	return 0 if successful or -1 on error
**
**	if successful, the status from the remote system is returned
**	indirectly through *statp
*/

int tc_wait(sysid, pid, timeout, statp)
int sysid, timeout;
long pid;
int *statp;
{
	register struct valmsg *mp;
	register int delay;
	extern char Tcerrmsg[];

	/* get the STCC message buffer */
	if ((mp = (struct valmsg *) tc_msgbuf(sysid, valmsgsz(OP_WAIT_NVALUE))) == (struct valmsg *) 0) {
		tc_errno = ER_ERR;
		return(-1);
	}

	/* set up the request message */
	mp->vm_nvalue = OP_WAIT_NVALUE;
	VM_PID(mp) = pid;
	VM_WTIMEOUT(mp) = timeout;

	/* perform the conversation and handle the reply codes */
	if (timeout >= 0) {
		if ((delay = TALK_DELAY + timeout) < 0)
			delay = (int) ((unsigned) ~0 >> 1);
	}
	else
		delay = 0;
	mp = (struct valmsg *) tc_talk(sysid, OP_WAIT, delay);
	switch (tc_errno) {
	case ER_OK:
		if (statp)
			*statp = (int) VM_STATUS(mp);
		return(0);
	case ER_INVAL:
	case ER_WAIT:
	case ER_PID:
	case ER_TIMEDOUT:
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

