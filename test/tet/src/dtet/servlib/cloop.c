/*
 *      SCCS:  @(#)cloop.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)cloop.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)cloop.c	1.2 92/09/30
NAME:		cloop.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to manage a conversation with a client process

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <sys/types.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "ltoa.h"
#include "error.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	si_clientloop() - perform a simple client service loop iteration
**		for a single process, waiting for it to complete
**
**	if the i/o is non-blocking, an i/o operation times out after
**	delay seconds
*/

void si_clientloop(pp, delay)
register struct ptab *pp;
int delay;
{
	register int done;

	TRACE3(Tloop, 2, "clientloop START %s, delay = %s", rtoa(&pp->pt_rid),
		itoa(delay));

	pp->pt_state = PS_SNDMSG;
	pp->pt_flags &= ~(PF_ATTENTION | PF_INPROGRESS | PF_IODONE | PF_IOERR);
	done = 0;

	while (!done) {
		TRACE4(Tloop, 4, "%s clientloop TOP: state = %s, flags = %s",
			rtoa(&pp->pt_rid), ptstate(pp->pt_state),
			ptflags(pp->pt_flags));
		si_servwait(pp, delay);
		if (pp->pt_flags & PF_IOERR) {
			pp->pt_flags &= ~(PF_INPROGRESS | PF_IODONE);
			done = 1;
			continue;
		}
		switch (pp->pt_state) {
		case PS_SNDMSG:
			if (pp->pt_flags & PF_INPROGRESS)
				break;
			/* else fall through */
		case PS_IDLE:
			pp->pt_state = PS_RCVMSG;
			pp->pt_flags &= ~PF_IODONE;
			break;
		case PS_RCVMSG:
			if ((pp->pt_flags & PF_INPROGRESS) == 0)
				pp->pt_state = PS_PROCESS;
			break;
		case PS_PROCESS:
			done = 1;
			continue;
		case PS_DEAD:
			si_service(pp);
			done = 1;
			continue;
		default:
			error(0, ptstate(pp->pt_state), "unexpected");
			done = 1;
			continue;
		}
	}

	TRACE4(Tloop, 2, "%s clientloop RETURN: state = %s, flags = %s",
		rtoa(&pp->pt_rid), ptstate(pp->pt_state),
		ptflags(pp->pt_flags));
}

