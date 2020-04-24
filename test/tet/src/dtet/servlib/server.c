/*
 *      SCCS:  @(#)server.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)server.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)server.c	1.2 92/09/30
NAME:		server.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	generic server request processing functions

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
#include "server.h"
#include "tslib.h"
#include "servlib.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/* extern function declarations */
#ifdef __STDC_
#include <time.h>
#else
extern time_t time();
#endif

/* static function declarations */
#if __STDC__
static void ti_rcvmsg(struct ptab *);
static void ti_rcv2(struct ptab *);
static void ti_sndmsg(struct ptab *);
#else /* __STDC__ */
static void ti_rcvmsg();
static void ti_rcv2();
static void ti_sndmsg();
#endif /* __STDC__ */


/*
**	si_service() - attention service routine
**
**	the basic cycle of events when servicing each connected process is:
**		read a message
**		process it
**		send a reply
**
**	the basic cycle of events when connecting to a server is:
**		send a message
**		idle
**		receive a reply
**
**	a number of message processing threads may be active at one time
**	but at most one for each connected process
**	the state of each process is stored in pt_state and bits in pt_flags
**	indicate the condition of the message connection
*/

void si_service(pp)
register struct ptab *pp;
{
	TRACE4(Tserv, 2, "%s si_service: state = %s, flags = %s",
		rtoa(&pp->pt_rid), ptstate(pp->pt_state),
		ptflags(pp->pt_flags));

	switch (pp->pt_state) {
	case PS_DEAD:
		if ((pp->pt_flags & PF_SERVWAIT) == 0) {
			ts_dead(pp);
			ss_dead(pp);
		}
		break;
	case PS_IDLE:
		return;
	case PS_RCVMSG:
		ti_rcvmsg(pp);
		break;
	case PS_SNDMSG:
		ti_sndmsg(pp);
		break;
	case PS_PROCESS:
		pp->pt_flags &= ~PF_IODONE;
		ss_process(pp);
		break;
	case PS_CONNECT:
		ss_connect(pp);
		break;
	default:
		error(0, ptstate(pp->pt_state), "unexpected");
		break;
	}
}

/*
**	si_servwait() - wait for service to complete while continuing to
**		process other requests
*/

void si_servwait(pp, timeout)
register struct ptab *pp;
int timeout;
{
	register time_t now = time((time_t *) 0);
	register time_t tsave;
	register int inptab;
	extern struct ptab *Ptab;

	TRACE4(Tserv, 4, "%s servwait START: state = %s, flags = %s",
		rtoa(&pp->pt_rid), ptstate(pp->pt_state),
		ptflags(pp->pt_flags));

	/* try to do i/o straight off */
	si_service(pp);

	/* return if i/o is complete */
	if ((pp->pt_flags & PF_INPROGRESS) == 0) {
		TRACE2(Tserv, 4, "%s servwait quick RETURN",
			rtoa(&pp->pt_rid));
		return;
	}

	/* save any existing timeout and install a new one */
	tsave = pp->pt_timeout;
	if (timeout > 0 && (tsave == 0 || tsave > now + timeout))
		pp->pt_timeout = timeout + now;

	/* if this ptab is not in the proc table,
		put it there temporarily so that ss_serverloop() can
		complete the i/o */
	if (pp != Ptab && !pp->pt_next && !pp->pt_last) {
		ptadd(pp);
		inptab = 1;
	}
	else
		inptab = 0;

	/* wait until i/o is complete or a timeout occurs */
	pp->pt_flags |= PF_SERVWAIT;
	do {
		if (ss_serverloop() <= 0) {
			TRACE1(Tserv, 6, "serverloop returned <= 0 !!");
			break;
		}
	} while ((pp->pt_flags & (PF_INPROGRESS | PF_TIMEDOUT)) == PF_INPROGRESS);
	pp->pt_flags &= ~PF_SERVWAIT;

	/* if we did a temp proc table insert, undo it here */
	if (inptab)
		ptrm(pp);

	/* restore the previous timeout state */
	pp->pt_timeout = tsave;

	TRACE2(Tserv, 4, "%s servwait RETURN after serverloop",
		rtoa(&pp->pt_rid));
}

/*
**	ti_rcvmsg() - transport-independent receive message processing
**
**	the transport-specific ts_rcvmsg() should raise one of PF_INPROGRESS,
**	PF_IODONE and PF_IOERR on return
**	if EOF is encountered on input, PF_IODONE should be raised and the
**	state changed to PS_DEAD
**
**	ts_rcvmsg() should return an ER_* error code to indicate the state of
**	local processing
*/

static void ti_rcvmsg(pp)
register struct ptab *pp;
{
	/* receive the message */
	ti_rcv2(pp);

	/*
	** each client request contains a new sequence number which is
	** returned in the server reply
	** if the client is interrupted before reading a reply and the
	** interrupt handler does not return, the reply message will remain
	** unread on the input queue
	** if a subsequent request is sent to the server, the first reply read
	** will contain the previous sequence number and is discarded here
	*/

	if ((pp->pt_flags & PF_IODONE) && pp->pt_state == PS_PROCESS &&
		pp->ptm_seq != pp->pt_seqno) {
			TRACE2(Tserv, 3,
				"wanted message seq = %s, will try again",
				ltoa(pp->pt_seqno));
			pp->pt_state = PS_IDLE;
	}
}

/*
**	ti_rcv2() - extend the ti_rcvmsg() processing
*/

static void ti_rcv2(pp)
register struct ptab *pp;
{
	register int rc;

	/* do initial flag check and set */
	if ((pp->pt_flags & PF_INPROGRESS) == 0)
		pp->pt_flags &= ~(PF_IODONE | PF_IOERR);
	if ((pp->pt_flags & PF_CONNECTED) == 0) {
		error(0, "rcvmsg: not connected to", rtoa(&pp->pt_rid));
		pp->pt_flags = (pp->pt_flags & ~(PF_INPROGRESS | PF_IODONE)) | PF_IOERR;
		return;
	}

	/* call the transport-specific receive message routine */
	do {
		rc = ts_rcvmsg(pp);
	} while ((pp->pt_flags & (PF_INPROGRESS | PF_NBIO)) == PF_INPROGRESS);

#ifndef NOTRACE
	if (rc != ER_OK)
		TRACE2(Tserv, 3, "ts_rcvmsg() returned %s", ptrepcode(rc));
#endif

	/* handle the returned state and flags */
	if (pp->pt_flags & PF_INPROGRESS)
		return;
	else if (pp->pt_flags & PF_IOERR) {
		if ((pp->pt_flags & PF_SERVER) == 0) {
			pp->ptm_rc = ER_RCVERR;
			pp->ptm_len = 0;
			pp->pt_state = PS_SNDMSG;
			si_servwait(pp, SHORTDELAY);
		}
		pp->pt_state = PS_DEAD;
	}
	else if ((pp->pt_flags & PF_IODONE) && (pp->pt_state == PS_RCVMSG)) {
		if (pp->ptm_magic != DTM_MAGIC) {
			TRACE3(Tserv, 3, "expected message magic number = %s, received %s",
				ltox(DTM_MAGIC), ltox(pp->ptm_magic));
			rc = ER_MAGIC;
		}
		if (pp->pt_flags & PF_SERVER) {
			TRACE5(Tserv, 3, "receive reply to %s request, rc = %s, data len = %s, seq = %s",
				ptreqcode(pp->pt_savreq),
				ptrepcode(pp->ptm_rc), itoa(pp->ptm_len),
				ltoa(pp->ptm_seq));
			if (rc != ER_OK) {
				pp->ptm_rc = rc;
				pp->ptm_len = 0;
			}
		}
		else {
			TRACE4(Tserv, 3, "receive request = %s, data len = %s, seq = %s",
				ptreqcode(pp->ptm_req), itoa(pp->ptm_len),
				ltoa(pp->ptm_seq));
			pp->pt_seqno = pp->ptm_seq;
			if (rc != ER_OK) {
				pp->ptm_rc = rc;
				pp->ptm_len = 0;
				pp->pt_state = PS_SNDMSG;
				si_servwait(pp, LONGDELAY);
				pp->pt_state = PS_IDLE;
				return;
			}
		}
		pp->pt_state = PS_PROCESS;
	}

	pp->pt_flags |= PF_ATTENTION;
}

/*
**	ti_sndmsg() - transport-independent send message processing
**
**	the transport-specific ts_sndmsg() should raise one of PF_INPROGRESS,
**	PF_IODONE and PF_IOERR on return
**	if the send fails because the connected process dies,
**	PF_IODONE should be raised and the state changed to PS_DEAD
**
**	ts_sndmsg() should return an ER_* error code to indicate the state of
**	local processing
*/

static void ti_sndmsg(pp)
register struct ptab *pp;
{
	register int rc;
	extern int Mypid, Myptype, Mysysid;
	static long seqno;

	/* set up flags and message header first time through */
	if ((pp->pt_flags & PF_INPROGRESS) == 0) {
		if (pp->pt_flags & PF_SERVER) {
			pp->pt_savreq = pp->ptm_req;
			pp->pt_seqno = seqno++;
			TRACE4(Tserv, 3, "send request = %s, data len = %s, seq = %s",
				ptreqcode(pp->ptm_req), itoa(pp->ptm_len),
				ltoa(pp->pt_seqno));
		}
		else
			TRACE5(Tserv, 3, "send reply to %s request, rc = %s, data len = %s, seq = %s",
				ptreqcode(pp->ptm_req), ptrepcode(pp->ptm_rc),
				itoa(pp->ptm_len), ltoa(pp->pt_seqno));
		pp->ptm_magic = DTM_MAGIC;
		pp->ptm_sysid = Mysysid;
		pp->ptm_pid = Mypid;
		pp->ptm_ptype = Myptype;
		pp->ptm_seq = pp->pt_seqno;
		pp->pt_flags &= ~PF_IODONE;
	}

	/* check that we are still connected to recipient */
	if ((pp->pt_flags & PF_CONNECTED) == 0) {
		error(0, "rcvmsg: not connected to", rtoa(&pp->pt_rid));
		pp->pt_flags = (pp->pt_flags & ~(PF_INPROGRESS | PF_IODONE)) | PF_IOERR;
		return;
	}

	/* call the transport-specific send message routine */
	rc = ts_sndmsg(pp);

	/* handle the returned state and flags */
	if (pp->pt_flags & PF_INPROGRESS)
		return;
	else if (pp->pt_flags & PF_IODONE)
		if (rc != ER_OK && (pp->pt_flags & PF_SERVER)) {
			pp->ptm_rc = rc;
			pp->ptm_len = 0;
			pp->pt_state = PS_PROCESS;
		}
		else {
			pp->pt_state = PS_IDLE;
			return;
		}
	else if (pp->pt_flags & PF_IOERR) {
		pp->pt_state = PS_DEAD;
	}

	pp->pt_flags |= PF_ATTENTION;
}

