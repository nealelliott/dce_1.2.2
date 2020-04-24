/*
 *      SCCS:  @(#)poll.c	1.3 (92/12/01) 
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
static char sccsid[] = "@(#)poll.c	1.3 (92/12/01) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)poll.c	1.3 92/12/01
NAME:		poll.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	required transport-specific library interface

	poll remote process connections for the posibility of i/o

MODIFICATIONS:
	Andrew Dingwall, UniSoft Ltd., November 1992
	AIX-specific modifications.

************************************************************************/

#include <errno.h>
#include <sys/types.h>
#ifdef _AIX
#include <sys/select.h>
#endif
#include <sys/time.h>
#include <netinet/in.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "error.h"
#include "tptab_in.h"
#include "server_in.h"
#include "tslib.h"

#ifndef NOTRACE
#include "ltoa.h"
#endif

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif


/*
** compatability for systems with pre-BSD4.3 select() call -
**	only one long in fd_set
**
** in UniPlus V.2, fd_set is already typedefed in <sys/types.h>
** on systems where this is not done, you will need something like
**
**	typedef struct { long fd_bits[1]; } fd_set;
**
** here as well
*/

#ifndef FD_SET

#define	FD_SET(n, p)	((p)->fds_bits[0] |= (1L << (n)))
#define	FD_CLR(n, p)	((p)->fds_bits[0] &= ~(1L << (n)))
#define	FD_ISSET(n, p)	((p)->fds_bits[0] & (1L << (n)))
#define FD_ZERO(p)	((p)->fds_bits[0] = 0L)

#endif /* FD_SET */

/*
**	ts_poll() - poll for connection attempts, and pending i/o to/from
**		connected processes in the ptab list at *pp
**
**	return	1 if there is at least one i/o request to service
**		0 if no requests arrive before the timeout expires
**		-1 on error
*/

int ts_poll(pp, timeout)
struct ptab *pp;
int timeout;
{
	register struct ptab *q;
	register struct tptab *tp;
	fd_set rfds, wfds, rd, wd;
	int nfds, nfound;
	struct timeval tv;
	register struct timeval *tvp;
	extern int Listen_sd;

	TRACE2(Tio, 4, "ts_poll: timeout = %s", itoa(timeout));

	/* clear descriptor masks for select -
		arrange to receive new connection notification */
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	if (Listen_sd >= 0) {
		TRACE2(Tio, 6, "poll listen sd %s for reading",
			itoa(Listen_sd));
		FD_SET(Listen_sd, &rfds);
		nfds = Listen_sd + 1;
	}
	else
		nfds = 0;

	/* set fd_set bits according to process state */
	for (q = pp; q; q = q->pt_next) {
		ASSERT(q->pt_magic == PT_MAGIC);
		tp = (struct tptab *) q->pt_tdata;
		if (tp->tp_sd < 0)
			continue;
		switch (q->pt_state) {
		case PS_IDLE:
			q->pt_state = PS_RCVMSG;
			/* fall through */
		case PS_RCVMSG:
			TRACE2(Tio, 6, "poll sd %s for reading",
				itoa(tp->tp_sd));
			FD_SET(tp->tp_sd, &rfds);
			if (tp->tp_sd >= nfds)
				nfds = tp->tp_sd + 1;
			break;
		case PS_CONNECT:
		case PS_SNDMSG:
			TRACE2(Tio, 6, "poll sd %s for writing",
				itoa(tp->tp_sd));
			FD_SET(tp->tp_sd, &wfds);
			if (tp->tp_sd >= nfds)
				nfds = tp->tp_sd + 1;
			break;
		}
	}

	/* set select timeout */
	if (timeout >= 0) {
		tv.tv_sec = timeout;
		tv.tv_usec = 0;
		tvp = &tv;
	}
	else
		tvp = (struct timeval *) 0;

	/* do the select operation -
		we use temporary copies of rfds and wfds because they
		get trashed if select returns with EINTR */
	do {
		TRACE3(Tio, 8, "select: rfds = %s, wfds = %s",
			ltox(rfds.fds_bits[0]), ltox(wfds.fds_bits[0]));
		rd = rfds;
		wd = wfds;
		errno = 0;
		nfound = select(nfds, &rd, &wd, (fd_set *) 0, tvp);
	} while (nfound < 0 && errno == EINTR);
	rfds = rd;
	wfds = wd;

	if (nfound >= 0)
		TRACE2(Tio, 6, "select returns %s", itoa(nfound));

	if (nfound < 0) {
		error(errno, "select() failed", (char *) 0);
		return(-1);
	}
	else if (nfound == 0)
		return(0);

	/* accept a new connection if one is pending on the listen socket */
	if (Listen_sd >= 0 && FD_ISSET(Listen_sd, &rfds)) {
		TRACE1(Tio, 6, "i/o possible on listen sd");
		FD_CLR(Listen_sd, &rfds);
		ss_tsaccept();
		nfound--;
	}

	/* arrange to service processes where i/o is possible */
	for (q = pp; q && nfound >= 0; q = q->pt_next) {
		ASSERT(q->pt_magic == PT_MAGIC);
		tp = (struct tptab *) q->pt_tdata;
		if (FD_ISSET(tp->tp_sd, &wfds)) {
			TRACE2(Tio, 6, "i/o possible on write sd %s",
				itoa(tp->tp_sd));
			nfound--;
			if (q->pt_state == PS_CONNECT) {
				q->pt_state = PS_SNDMSG;
				q->pt_flags = (q->pt_flags & ~PF_INPROGRESS) | PF_CONNECTED;
			}
			q->pt_flags |= PF_ATTENTION;
		}
		if (FD_ISSET(tp->tp_sd, &rfds)) {
			TRACE2(Tio, 6, "i/o possible on read sd %s",
				itoa(tp->tp_sd));
			nfound--;
			q->pt_flags |= PF_ATTENTION;
		}
	}

	return(1);
}

