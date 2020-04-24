/*
 *      SCCS:  @(#)tcshlock.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tcshlock.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tcshlock.c	1.2 92/09/30
NAME:		tcshlock.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	June 1992

DESCRIPTION:
	function to create a file in a lock directory on a remote system

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <errno.h>
#include "dtmsg.h"
#include "avmsg.h"
#include "error.h"
#include "servlib.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif


/*
**	tc_sharelock() - send an OP_SHARELOCK message to STCC and receive a
**		reply
**
**	return pointer to lock file if successful or (char *) 0 on error
*/

char *tc_sharelock(sysid, lockdir, timeout)
int sysid, timeout;
char *lockdir;
{
	register struct avmsg *mp;
	register int delay;
	extern char Tcerrmsg[];

	/* make sure that lockdir is non-null */
	if (!lockdir || !*lockdir) {
		tc_errno = ER_INVAL;
		return((char *) 0);
	}

	/* get the STCC message buffer */
	if ((mp = (struct avmsg *) tc_msgbuf(sysid, avmsgsz(OP_SHARELOCK_ARGC))) == (struct avmsg *) 0) {
		tc_errno = ER_ERR;
		return((char *) 0);
	}

	/* set up the request message */
	mp->av_argc = OP_SHARELOCK_ARGC;
	AV_DIR(mp) = lockdir;
	AV_TIMEOUT(mp) = (long) timeout;

	/* send the request and receive the reply */
	if (timeout >= 0) {
		if ((delay = TALK_DELAY + timeout) < 0)
			delay = (int) ((unsigned) ~0 >> 1);
	}
	else
		delay = 0;
	mp = (struct avmsg *) tc_talk(sysid, OP_SHARELOCK, delay);

	/* handle the return codes */
	if (tc_errno == ER_OK)
		return(AV_FNAME(mp));
	else if ((errno = unmaperrno(tc_errno)) == 0)
		switch (tc_errno) {
		case ER_INVAL:
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
	return((char *) 0);
}

