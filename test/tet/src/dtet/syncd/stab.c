/*
 *      SCCS:  @(#)stab.c	1.5 (92/12/01) 
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
static char sccsid[] = "@(#)stab.c	1.5 (92/12/01) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)stab.c	1.5 92/12/01
NAME:		stab.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	sync event table administration functions

MODIFICATIONS:
	Andrew Dingwall, UniSoft Ltd., October 1992
	changed stservice() to only consider sync points from
	processes that have actually voted when calculating the sync point
	number of an event

	Andrew Dingwall, UniSoft Ltd., November 1992
	End all related user sync events when a process performs an autosync.
	Removed snid from stufind() call - a user sync is now identified
	only by xrid and list of participating systems - this allows a
	TCMrem process (which has a different snid) to participate in a
	user sync event.
	Clear process timeout after queueing a sync reply message.

************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "stab.h"
#include "synreq.h"
#include "valmsg.h"
#include "error.h"
#include "llist.h"
#include "bstring.h"
#include "dtetlib.h"
#include "syncd.h"

#ifndef NOTRACE
#include "ltoa.h"
#include "ftoa.h"
#endif

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/* the largest sync point number */
#define SPMAX	((long) ((unsigned long) ~0 >> 1))

static struct stab *stab;		/* ptr to head of sync event list */

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#else
extern char *malloc();
extern void free();
#endif

/* static function declarations */
#if __STDC__
static void stservice(struct stab *);
static int sts2(struct stab *, long);
static void stpok(struct stab *, long);
static void stperr(struct stab *, long);
static void syncmsg(struct stab *, struct ustab *);
static void synmsg2(struct stab *, struct ustab *, struct ptab *);
#else /* __STDC__ */
static void stservice();
static int sts2();
static void stpok();
static void stperr();
static void syncmsg();
static void synmsg2();
#endif /* __STDC__ */


/*
**	stalloc() - allocate a sync table element and return a pointer thereto
**
**	return (struct stab *) 0 on error
*/

struct stab *stalloc()
{
	register struct stab *sp;
	static long snid;

	/* allocate memory for the stab element */
	errno = 0;
	if ((sp = (struct stab *) malloc(sizeof *sp)) == (struct stab *) 0) {
		error(errno, "can't allocate stab element", (char *) 0);
		return((struct stab *) 0);
	}
	TRACE2(Tbuf, 6, "allocate stab = %s", itox(sp));
	bzero((char *) sp, sizeof *sp);

	sp->st_snid = ++snid;
	sp->st_xrid = -1L;

	return(sp);
}

/*
**	stfree() - free storage occupied by an stab element
*/

void stfree(sp)
struct stab *sp;
{
	TRACE2(Tbuf, 6, "free stab = %s", itox(sp));

	if (sp) {
		if (sp->st_ud) {
			TRACE2(Tbuf, 6, "free ustab = %s", itox(sp));
			free((char *) sp->st_ud);
		}
		free((char *) sp);
	}
}

/*
**	ustalloc() - allocate a sync table per-user details element or
**		grow an existing one
**
**	return 0 if successful or -1 on error
*/

int ustalloc(sp, nud)
struct stab *sp;
int nud;
{
	register int needlen;

	ASSERT(sp);
	needlen = nud * sizeof *sp->st_ud;

	if (BUFCHK((char **) &sp->st_ud, &sp->st_udlen, needlen) < 0) {
		if (sp->st_udlen == 0)
			sp->st_nud = 0;
		return(-1);
	}
	bzero((char *) sp->st_ud, needlen);

	sp->st_nud = nud;
	return(0);
}

/*
**	stadd() - add an stab element to the sync table
*/

void stadd(sp)
struct stab *sp;
{
	listinsert((struct llist **) &stab, (struct llist *) sp);
}

/*
**	strm() - remove an stab element from the sync table
*/

void strm(sp)
struct stab *sp;
{
	listremove((struct llist **) &stab, (struct llist *) sp);
}

/*
**	stafind() - find an auto-sync stab element and return a pointer thereto
**
**	return (struct stab *) 0 if not found
*/

struct stab *stafind(snid)
register long snid;
{
	register struct stab *sp;

	for (sp = stab; sp; sp = sp->st_next)
		if ((sp->st_flags & SF_USYNC) == 0 && sp->st_snid == snid)
			break;

	return(sp);
}

/*
**	stufind() - find a user-sync stab element and return a pointer thereto
**
**	return (struct stab *) 0 if not found
*/

struct stab *stufind(xrid, udp, nud)
register long xrid;
register struct ustab *udp;
register int nud;
{
	register struct stab *sp;
	register struct ustab *up1, *up2;
	register int count;

	for (sp = stab; sp; sp = sp->st_next) {
		if ((sp->st_flags & SF_USYNC) == 0 || sp->st_xrid != xrid ||
			sp->st_nud != nud)
				continue;
		count = 0;
		for (up1 = sp->st_ud; up1 < sp->st_ud + sp->st_nud; up1++)
			for (up2 = udp; up2 < udp + nud; up2++)
				if (up1->us_sysid == up2->us_sysid &&
					up1->us_ptype == up2->us_ptype) {
						count++;
						break;
				}
		if (count == nud)
			break;
	}

	return(sp);
}

/*
**	stcheck() - see if a sync event has occurred yet
*/

void stcheck(sp)
register struct stab *sp;
{
	register struct ustab *up;
	register int count, vote;

	if (sp->st_flags & SF_ATTENTION)
		return;

	TRACE6(Tsyncd, 7, "stcheck: addr = %s, snid = %s, xrid = %s, last spno = %s, flags = %s",
		itox(sp), ltoa(sp->st_snid), ltoa(sp->st_xrid),
		ltoa(sp->st_lastspno), stflags(sp->st_flags));

	count = 0;
	vote = SV_YES;
	for (up = sp->st_ud; up < sp->st_ud + sp->st_nud; up++) {
		TRACE6(Tsyncd, 8, "per-user: sysid = %s, ptype = %s, state = %s, spno = %s, ntimedout = %s",
			itoa(up->us_sysid), ptptype(up->us_ptype),
			systate(up->us_state), ltoa(up->us_spno),
			itoa(up->us_ntimedout));
		switch (up->us_state) {
		case SS_TIMEDOUT:
			count = sp->st_nud;
			sp->st_flags |= SF_TIMEDOUT;
			/* fall through */
		case SS_SYNCNO:
		case SS_DEAD:
			vote = SV_NO;
			/* fall through */
		case SS_SYNCYES:
			count++;
			break;
		}
	}

	/* see if the event has occurred and set flags accordingly */
	if (count >= sp->st_nud)
		sp->st_flags = (sp->st_flags & ~SF_INPROGRESS) | ((vote == SV_YES) ? SF_OK : SF_ERR) | SF_ATTENTION;

	TRACE2(Tsyncd, 7, "after stcheck: flags = %s", stflags(sp->st_flags));
}

/*
**	stloop() - sync table servicing loop
*/

void stloop()
{
	register struct stab *sp;
	register int done;

	TRACE2(Tsyncd, 7, "stloop TOP: stab = %s", itox(stab));

	/* service all sync table entries needing attention */
	do {
		done = 1;
		for (sp = stab; sp; sp = sp->st_next)
			if (sp->st_flags & SF_ATTENTION) {
				sp->st_flags &= ~SF_ATTENTION;
				stservice(sp);
				done = 0;
				break;
			}
	} while (!done);

	TRACE1(Tsyncd, 7, "stloop END");
}


/*
**	stservice() - service a single sync table entry that needs attention
*/

static void stservice(sp)
register struct stab *sp;
{
	register struct ustab *up;
	register long spno = SPMAX;

	/* find the lowest non-zero sync point number
		from participating requests */
	for (up = sp->st_ud; up < sp->st_ud + sp->st_nud; up++)
		switch (up->us_state) {
		case SS_SYNCYES:
		case SS_SYNCNO:
		case SS_TIMEDOUT:
			if (up->us_spno > 0L && up->us_spno < spno)
				spno = up->us_spno;
			break;
		}

	/* if all user sync point numbers were zero, this event is zero */
	if (spno == SPMAX)
		spno = 0L;
	sp->st_lastspno = spno;

	TRACE6(Tsyncd, 7, "stservice: addr = %s, snid = %s, xrid = %s, this spno = %s, flags = %s",
		itox(sp), ltoa(sp->st_snid), ltoa(sp->st_xrid), ltoa(spno),
		stflags(sp->st_flags));

	/* process the event */
	if (sp->st_flags & SF_ERR) {
		TRACE1(Tsyncd, 7, "stservice: sync event was unsuccessful");
		stperr(sp, spno);
	}
	else if (sp->st_flags & SF_OK) {
		TRACE1(Tsyncd, 7, "stservice: sync event was successful");
		stpok(sp, spno);
	}

	/* remove the element if no longer required */
	if ((sp->st_flags & (SF_ERR | SF_OK)) && !sts2(sp, spno)) {
		TRACE1(Tsyncd, 7, "stservice: stab entry no longer required");
		strm(sp);
		stfree(sp);
		return;
	}

	/* element still required - reset the event flags */
	sp->st_flags &= ~(SF_OK | SF_ERR | SF_TIMEDOUT);

	TRACE2(Tsyncd, 7, "after stservice: flags = %s",
		stflags(sp->st_flags));
}

/*
**	sts2() - extend the stservice() processing
**
**	called once a sync event has happened
**
**	return	1 if the stab element is still required
**		0 if the element should be removed
*/

static int sts2(sp, spno)
register struct stab *sp;
register long spno;
{
	register struct ustab *up;
	register int count;

	/* reset the event flags and user states after the event has happened;
	   see if the sync table entry is still required:
		a user-sync entry is not required if:
			all participants have voted or logged off (or died)
		an auto-sync entry is not required if:
			all participants have voted or logged off (or died)
			and the "owener" has logged off as well */
	sp->st_flags &= ~SF_INPROGRESS;
	count = 0;
	for (up = sp->st_ud; up < sp->st_ud + sp->st_nud; up++)
		switch (up->us_state) {
		case SS_SYNCYES:
		case SS_SYNCNO:
		case SS_TIMEDOUT:
			if (up->us_spno <= spno) {
				up->us_state = SS_NOTSYNCED;
				if (sp->st_flags & SF_USYNC)
					count++;
			}
			else
				sp->st_flags |= SF_INPROGRESS;
			break;
		case SS_DEAD:
			count++;
			break;
		}

	return((count == sp->st_nud && ((sp->st_flags & SF_USYNC) || !sp->st_ptab)) ? 0 : 1);
}

/*
**	stpok() - process a sync event that has completed successfully
*/

static void stpok(sp, spno)
register struct stab *sp;
register long spno;
{
	register struct ustab *up;

	/* wake up processes waiting on the event */
	for (up = sp->st_ud; up < sp->st_ud + sp->st_nud; up++)
		if (up->us_spno <= spno)
			syncmsg(sp, up);
}

/*
**	stperr() - process a sync event that has completed unsuccessfully
*/

static void stperr(sp, spno)
register struct stab *sp;
register long spno;
{
	register struct ustab *up;

	/* see if there are any dead processes in the list -
		if there are, set spno to SPMAX so as to end all future events
		as well */
	for (up = sp->st_ud; up < sp->st_ud + sp->st_nud; up++)
		if (up->us_state == SS_DEAD) {
			spno = SPMAX;
			break;
		}
	sp->st_lastspno = spno;

	/* wake up processes waiting on the event */
	for (up = sp->st_ud; up < sp->st_ud + sp->st_nud; up++)
		switch (up->us_state) {
		case SS_SYNCYES:
		case SS_SYNCNO:
		case SS_TIMEDOUT:
			if (up->us_spno <= spno)
				syncmsg(sp, up);
			break;
		case SS_NOTSYNCED:
			if (sp->st_flags & SF_TIMEDOUT)
				up->us_ntimedout++;
			break;
		case SS_DEAD:
			break;
		default:
			error(0, systate(up->us_state), "unexpected");
			break;
		}
}

/*
**	syncmsg() - construct a sync reply message
*/

static void syncmsg(sp, up)
struct stab *sp;
register struct ustab *up;
{
	register struct ptab *pp;
	register struct sptab *stp;

	ASSERT(up->us_ptab);
	pp = up->us_ptab;
	ASSERT(pp->pt_state == PS_WAITSYNC);

	synmsg2(sp, up, pp);

	TRACE4(Tsyncd, 6, "%sSYNC reply: wake up %s, rc = %s",
		sp->st_flags & SF_USYNC ? "U" : "A",
		rtoa(&pp->pt_rid), ptrepcode(pp->ptm_rc));

	stp = (struct sptab *) pp->pt_sdata;
	stp->sp_stab = (struct stab *) 0;
	stp->sp_timeout = 0L;

	pp->pt_timeout = (time_t) 0;
	pp->pt_state = PS_SNDMSG;
	pp->pt_flags |= PF_ATTENTION;
}

/*
**	synmsg2() - extend the syncmsg() processing
*/

static void synmsg2(sp, up, pp)
register struct stab *sp;
register struct ustab *up;
register struct ptab *pp;
{
	register struct valmsg *rp;
	register int len, n, rc;

	/* decide what type of reply to send */
	if (sp->st_flags & SF_OK) {
		ASSERT(up->us_state == SS_SYNCYES);
		rc = ER_OK;
	}
	else if (up->us_state == SS_TIMEDOUT)
		rc = ER_TIMEDOUT;
	else
		rc = ER_SYNCERR;

	/* make sure that the message buffer is big enough for the reply */
	switch (rc) {
	case ER_OK:
	case ER_SYNCERR:
	case ER_TIMEDOUT:
		len = valmsgsz(OP_AUSYNC_NVALUE(sp->st_nud - 1));
		break;
	default:
		error(0, ptrepcode(rc), "unexpected");
		len = valmsgsz(OP_AUSYNC_NVALUE(0));
		rc = ER_ERR;
		break;
	}

	/* make sure that the message buffer is big enough for the reply */
	if (BUFCHK(&pp->ptm_data, &pp->pt_mdlen, len) < 0) {
		pp->ptm_rc = ER_ERR;
		pp->ptm_len = 0;
		return;
	}
	rp = (struct valmsg *) pp->ptm_data;

	/* construct the reply message */
	VM_XRID(rp) = sp->st_xrid;
	VM_SNID(rp) = sp->st_snid;
	VM_SPNO(rp) = sp->st_lastspno;
	VM_SVOTE(rp) = 0L;
	VM_STIMEOUT(rp) = 0L;
	switch (rc) {
	case ER_OK:
	case ER_SYNCERR:
	case ER_TIMEDOUT:
		n = 0;
		for (up = sp->st_ud; up < sp->st_ud + sp->st_nud; up++) {
			if (up->us_ptab == pp)
				continue;
			VM_SSYSID(rp, n) = (long) up->us_sysid;
			switch (rc) {
			case ER_OK:
				VM_RSPNO(rp, n) = (long) up->us_spno;
				break;
			default:
				VM_STATE(rp, n) = (long) up->us_state;
				break;
			}
			n++;
		}
		ASSERT(n == sp->st_nud - 1);
		break;
	}

	pp->ptm_rc = rc;
	pp->ptm_len = len;
}

/*
**	stuend() - force end for all user syncs involving a particular sysid
*/

void stuend(sysid)
register int sysid;
{
	register struct stab *sp;
	register struct ustab *up;

	TRACE2(Tsyncd, 7, "stuend() called, sysid = %s", itoa(sysid));

	for (sp = stab; sp; sp = sp->st_next)
		if ((sp->st_flags & (SF_INPROGRESS | SF_USYNC)) == (SF_INPROGRESS | SF_USYNC))
			for (up = sp->st_ud; up < sp->st_ud + sp->st_nud; up++) 
				if (up->us_sysid == sysid) {
					TRACE2(Tsyncd, 7, "force user sync end: addr = %s", itox(sp));
					sp->st_flags = (sp->st_flags & ~SF_INPROGRESS) | SF_ERR | SF_ATTENTION;
					break;
				}
}

/*
**	stdead() - stab processing when a process logs off or dies
*/

void stdead(pp)
struct ptab *pp;
{
	register struct stab *sp;
	register struct ustab *up;

	for (sp = stab; sp; sp = sp->st_next) {
		if (sp->st_ptab == pp)
			sp->st_ptab = (struct ptab *) 0;
		for (up = sp->st_ud; up < sp->st_ud + sp->st_nud; up++) 
			if (up->us_ptab == pp) {
				up->us_ptab = (struct ptab *) 0;
				up->us_state = SS_DEAD;
			}
		stcheck(sp);
	}
}

/*
**	stflags() - return printable representation of stab flags value
*/

#ifndef NOTRACE
char *stflags(fval)
int fval;
{
	static struct flags flags[] = {
		{ SF_ATTENTION, "ATTENTION" },
		{ SF_INPROGRESS, "INPROGRESS" },
		{ SF_OK, "OK" },
		{ SF_ERR, "ERR" },
		{ SF_TIMEDOUT, "TIMEDOUT" },
		{ SF_USYNC, "USYNC" },
	};

	return(ftoa(fval, flags, sizeof flags / sizeof flags[0]));
}
#endif

