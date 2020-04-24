/*
 *      SCCS:  @(#)sloop.c	1.3 (92/12/01) 
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
static char sccsid[] = "@(#)sloop.c	1.3 (92/12/01) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)sloop.c	1.3 92/12/01
NAME:		sloop.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	generic server loop processing functions

MODIFICATIONS:

************************************************************************/

#if !defined(NOTRACE) && __STDC__
#include <stdio.h>
#endif

#include <sys/types.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "error.h"
#include "server.h"
#include "servlib.h"
#include "tslib.h"

#ifndef NOTRACE
#include "ltoa.h"
#include "dtetlib.h"
#endif

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#include <time.h>
#else
extern void exit();
extern time_t time();
#endif

/* static function declarations */
#if __STDC__
static void si_procrun();
static int si_timeouts();
#else /* __STDC__ */
static void si_procrun();
static int si_timeouts();
#endif /* __STDC__ */


/*
**	si_serverloop() - perform a single server process loop iteration
**
**	poll for incoming requests, then service any resulting events;
**	do timeout processing, then service any resulting events
**
**	the ts_poll() routine should perform a poll of all connections to
**	processes in states PS_IDLE, PS_RCVMSG, PS_SNDMSG and PS_CONNECT
**	if data can be read (PS_IDLE or PS_RCVMSG) or written (PS_SNDMSG
**	or PS_CONNECT) without blocking, raise PF_ATTENTION
**	change the state to PS_RCVMSG or PS_SNDMSG as appropriate
**	if the poll is implemented by doing a non-blocking read or write,
**	set the state and flags as described for ts_rcvmsg() or ts_sndmsg()
**
**	if a new connection request is received, a new ptab element should be
**	allocated by ptalloc() and linked to the ptab list by insptabent() -
**	if a connection is accepted without reading any data, set the state to
**	PS_RCVMSG and raise PF_ATTENTION, otherwise set the state and flags as
**	described for ts_rcvmsg() below
**
**	ts_poll should return 1 as soon as i/o is possible, or 0 after
**	delay seconds if no i/o is possible
**	if delay is -ve, ts_poll should not return until i/o is possible
*/

#define INFINITY	((time_t) ((unsigned long) ~0 >> 1))

void si_serverloop()
{
	register struct ptab *pp;
	register time_t timeout;
	register int delay, rc;
	extern struct ptab *Ptab;

	/* see how long it is to the next timeout */
	timeout = INFINITY;
	for (pp = Ptab; pp; pp = pp->pt_next) {
		ASSERT(pp->pt_magic == PT_MAGIC);
		if (pp->pt_timeout && pp->pt_timeout < timeout)
			timeout = pp->pt_timeout;
	}
	if (timeout == INFINITY)
		delay = Ptab ? LONGDELAY : SHORTDELAY;
	else if ((delay = (int) (timeout - time((time_t *) 0))) < 0 ||
		delay > LONGDELAY)
			delay = LONGDELAY;

	TRACE3(Tloop, 2, "si_serverloop TOP: Ptab = %s, poll timeout = %s",
		itox(Ptab), itoa(delay));

	/* perform the main loop */
	if ((rc = ts_poll(Ptab, delay)) > 0)
		si_procrun();
	else if (rc < 0)
		exit(1);
	if (si_timeouts() > 0)
		si_procrun();
}

/*
**	si_procrun() - scan the process table for processes that need servicing
**		call the service routine for those that do
*/

static void si_procrun()
{
	extern struct ptab *Ptab;
	register struct ptab *pp;
	register int done;

	TRACE2(Tloop, 8, "si_procrun START: Ptab = %s", itox(Ptab));

	do {
		TRACE2(Tloop, 8, "si_procrun RESTART: Ptab = %s", itox(Ptab));
		done = 1;
		for (pp = Ptab; pp; pp = pp->pt_next) {
			TRACE3(Tloop, 8, "pp = %s, next = %s", itox(pp), itox(pp->pt_next));
			ASSERT(pp->pt_magic == PT_MAGIC);
			if (pp->pt_flags & PF_ATTENTION) {
				pp->pt_flags &= ~PF_ATTENTION;
				si_service(pp);
				done = 0;
				break;
			}
		}
	} while (!done);

	TRACE2(Tloop, 8, "si_procrun END: Ptab = %s", itox(Ptab));

	/* call the server-specific end-procrun routine */
	ss_procrun();
}

/*
**	si_timeouts() - process event timeouts
**
**	return 1 if any events need to be serviced, 0 otherwise
*/

static int si_timeouts()
{
	extern struct ptab *Ptab;
	register struct ptab *pp;
	register time_t now = time((time_t *) 0);
	register int found = 0;

	for (pp = Ptab; pp; pp = pp->pt_next) {
		ASSERT(pp->pt_magic == PT_MAGIC);
		if (pp->pt_timeout > 0 && pp->pt_timeout <= now) {
			pp->pt_flags |= PF_TIMEDOUT;
			pp->pt_timeout = 0;
			TRACE4(Tloop, 4, "%s: about to call ss_timeout: state = %s, flags = %s",
				rtoa(&pp->pt_rid), ptstate(pp->pt_state),
				ptflags(pp->pt_flags));
			ss_timeout(pp);
			found = 1;
		}
	}

	return(found);
}

