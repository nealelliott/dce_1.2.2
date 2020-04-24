/*
 *      SCCS:  @(#)sproc.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)sproc.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)sproc.c	1.2 92/09/30
NAME:		sproc.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	generic server request processing functions

MODIFICATIONS:

************************************************************************/

#if __STDC__ || TESTING
#include <stdio.h>
#endif

#include <sys/types.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "error.h"
#include "server.h"
#include "dtetlib.h"

#if TESTING || !defined(NOTRACE)
#include "avmsg.h"
#endif

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif


/* static function declarations */
#if __STDC__
static void op_logon(struct ptab *);
static void logonfail(struct ptab *, int);
static void op_logoff(struct ptab *);
static void op_trace(struct ptab *);
static void op_null(struct ptab *);
static void op_print(struct ptab *);
#else /* __STDC__ */
static void op_logon();
static void logonfail();
static void op_logoff();
static void op_trace();
static void op_null();
static void op_print();
#endif /* __STDC__ */


/*
**	si_serverproc() - server-independent message processing
*/

void si_serverproc(pp)
register struct ptab *pp;
{
	TRACE3(Tserv, 1, "%s serverproc: request = %s", rtoa(&pp->pt_rid),
		ptreqcode(pp->ptm_req));

	/* ensure process is logged on exactly once */
	if ((pp->ptm_req != OP_LOGON && (pp->pt_flags & PF_LOGGEDON) == 0) ||
		(pp->ptm_req == OP_LOGON && (pp->pt_flags & PF_LOGGEDON))) {
			pp->ptm_rc = ER_LOGON;
			pp->ptm_len = 0;
			pp->pt_state = PS_SNDMSG;
			pp->pt_flags |= PF_ATTENTION;
			return;
	}

	/* set a return code in case the processing routine forgets */
	pp->ptm_rc = ER_INTERN;

	/* process the request */
	switch (pp->ptm_req) {
	case OP_LOGON:
		op_logon(pp);
		break;
	case OP_LOGOFF:
		op_logoff(pp);
		break;
	case OP_TRACE:
		op_trace(pp);
		break;
	case OP_NULL:
		op_null(pp);
		break;
#if TESTING
	case OP_PRINT:
		op_print(pp);
		break;
#endif
	default:
		ss_serverproc(pp);
		break;
	}
}

/*
**	op_logon() - process a logon request
*/

static void op_logon(pp)
register struct ptab *pp;
{
	register struct ptab *q;
	register int errflag, rc;
	struct remid rid;
	extern int Myptype;

	/* see if process is already connected via another ptab entry -
		"can't happen" */
	if ((q = getptbysyspid(pp->ptm_sysid, pp->ptm_pid)) != (struct ptab *) 0 && q != pp) {
		error(0, "process already connected!", rtoa(&q->pt_rid));
		logonfail(pp, ER_LOGON);
		return;
	}

	/* do some sanity checking on the remid */
	rid.re_sysid = pp->ptm_sysid;
	rid.re_pid = pp->ptm_pid;
	rid.re_ptype = pp->ptm_ptype;
	errflag = 0;
	if (pp->ptm_sysid < 0 || pp->ptm_pid <= 0) {
		errflag = 1;
	}
	else switch (pp->ptm_ptype) {
		case PT_MTCC:
		case PT_STCC:
		case PT_MTCM:
		case PT_STCM:
		case PT_XRESD:
		case PT_SYNCD:
			if (pp->ptm_ptype != Myptype)
				break;
			/* else fall through */
		default:
			errflag = 1;
			break;
	}

	if (errflag) {
		error(0, "bad remid in logon request:", rtoa(&rid));
		logonfail(pp, ER_INVAL);
		return;
	}

	/* call server-specific logon routine */
	pp->pt_rid = rid;
	if ((rc = ss_logon(pp)) != ER_OK) {
		logonfail(pp, rc);
		return;
	}

	/* all ok so show process as logged on */
	pp->ptm_rc = ER_OK;
	pp->ptm_len = 0;
	pp->pt_state = PS_SNDMSG;
	pp->pt_flags = (pp->pt_flags & ~PF_LOGGEDOFF) | PF_LOGGEDON | PF_ATTENTION;
}

/*
**	logonfail() - send an error message after failed logon
*/

static void logonfail(pp, rc)
register struct ptab *pp;
int rc;
{
	pp->ptm_rc = rc;
	pp->ptm_len = 0;
	pp->pt_state = PS_SNDMSG;
	si_servwait(pp, LONGDELAY);
	pp->pt_state = PS_DEAD;
	pp->pt_flags |= PF_ATTENTION;
}

/*
**	op_logoff() - process a logoff request
*/

static void op_logoff(pp)
register struct ptab *pp;
{
	/* call the server-specific logoff routine */
	ss_logoff(pp);

	pp->ptm_rc = ER_OK;
	pp->ptm_len = 0;
	pp->pt_state = PS_SNDMSG;
	pp->pt_flags = (pp->pt_flags & ~PF_LOGGEDON) | PF_LOGGEDOFF | PF_ATTENTION;
}

/*
**	op_trace() - process a trace request
*/

static void op_trace(pp)
register struct ptab *pp;
{
#ifdef NOTRACE
	pp->ptm_rc = ER_TRACE;
#else
	register struct avmsg *mp = (struct avmsg *) pp->ptm_data;

	traceinit((int) mp->av_argc + 1, mp->av_argv - 1);
	pp->ptm_rc = ER_OK;
#endif

	pp->ptm_len = 0;
	pp->pt_state = PS_SNDMSG;
	pp->pt_flags |= PF_ATTENTION;
}

/*
**	op_null() - process a null request
*/

static void op_null(pp)
register struct ptab *pp;
{
	/* no nothing successfully */
	pp->ptm_rc = ER_OK;
	pp->ptm_len = 0;
	pp->pt_state = PS_SNDMSG;
	pp->pt_flags |= PF_ATTENTION;
}

/*
**	op_print() - print some lines on standard output
**
**	for testing only - could block!!
*/

#if TESTING

static void op_print(pp)
register struct ptab *pp;
{

	register struct avmsg *mp = (struct avmsg *) pp->ptm_data;
	register int n;
	extern char *Progname;

	(void) printf("%s: call to op_print(): argc = %d\n",
		Progname, mp->av_argc);
	for (n = 0; n < (int) mp->av_argc; n++)
		(void) printf("%s\n", mp->av_argv[n]);
	(void) fflush(stdout);

	pp->ptm_rc = ER_OK;
	pp->ptm_len = 0;
	pp->pt_state = PS_SNDMSG;
	pp->pt_flags |= PF_ATTENTION;
}

#endif /* TESTING */

