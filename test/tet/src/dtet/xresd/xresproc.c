/*
 *      SCCS:  @(#)xresproc.c	1.4 (92/11/16) 
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
static char sccsid[] = "@(#)xresproc.c	1.4 (92/11/16) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)xresproc.c	1.4 92/11/16
NAME:		xresproc.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	execution results file request processing functions

MODIFICATIONS:
	Andrew Dingwall, UniSoft Ltd., October 1992
	Inform MTCM if result code action is Abort.

	Andrew Dingwall, UniSoft Ltd., November 1992
	Reset per-system xres states and results on TP start.

************************************************************************/


#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

/* #include <sys/types.h> included by tet_api.h */
#if defined(SVR2) || defined(BSD42) || defined(BSD43)
typedef int pid_t;	/* band-aid for non-posix systems */
#endif
#include "dtet/tet_api.h"

#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "xtab.h"
#include "avmsg.h"
#include "valmsg.h"
#include "error.h"
#include "ltoa.h"
#include "xresd.h"
#include "dtetlib.h"
#include "dtet/tet_jrnl.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/* extern function declarations */
#ifndef __STDC__
extern time_t time();
#endif

/* static function declarations */
#if __STDC__
static int op_xr2(struct ptab *, struct xtab *);
static void xrinfo(struct xtab *, char *, char *);
static int xrmsg(struct xtab *, int);
static int xrwrite(struct xtab *, struct avmsg *);
static int addresult(int, int);
#ifndef NOTRACE
static char *xrstate(int);
#endif /* !NOTRACE */
#else /* __STDC__ */
static int op_xr2();
static void xrinfo();
static int xrmsg();
static int xrwrite();
static int addresult();
#ifndef NOTRACE
static char *xrstate();
#endif /* !NOTRACE */
#endif /* __STDC__ */


/*
**	op_xropen() - open a tet_xres file
**
**	in the request message:
**		AV_XFNAME = tet_xres file to open
**
**	in the reply message (rc = ER_OK):
**		VM_XRID = xrid to use in all subsequent references to the file
*/

void op_xropen(pp)
register struct ptab *pp;
{
	register struct avmsg *mp = (struct avmsg *) pp->ptm_data;
	register struct xtab *xp;

	/* do some sanity checks on the request message */
	if ((int) mp->av_argc != OP_XROPEN_ARGC || !AV_XFNAME(mp) || !*AV_XFNAME(mp)) {
		pp->ptm_rc = ER_INVAL;
		pp->ptm_len = 0;
		return;
	}

	TRACE2(Txresd, 4, "OP_XROPEN: file = \"%s\"", AV_XFNAME(mp));

	/* get a new xtab element for this request */
	if ((xp = xtalloc()) == (struct xtab *) 0) {
		pp->ptm_rc = ER_ERR;
		pp->ptm_len = 0;
		return;
	}

	if ((pp->ptm_rc = op_xr2(pp, xp)) == ER_OK) {
		xp->xt_ptab = pp;
		xtadd(xp);
		pp->ptm_len = valmsgsz(OP_XROPEN_NVALUE);
	}
	else {
		xtfree(xp);
		pp->ptm_len = 0;
	}
}

/*
**	op_xr2() - extend the op_xropen() processing
**
**	return ER_OK if successful or other ER_* error code on error
*/

static int op_xr2(pp, xp)
register struct ptab *pp;
register struct xtab *xp;
{
	register char *dp = pp->ptm_data;
	static char msg[] = "can't open";

#define mp	((struct avmsg *) dp)

	/* fill in the xtab details */
	if ((xp->xt_xfname = strstore(AV_XFNAME(mp))) == (char *) 0)
		return(ER_ERR);

	/* open the tet_xres files */
	if ((xp->xt_xfp = fopen(xp->xt_xfname, "w")) == NULL) {
		error(errno, msg, xp->xt_xfname);
		return(ER_ERR);
	}

#undef mp
#define rp	((struct valmsg *) dp)

	/* make sure that the message buffer is big enough for the reply */
	if (BUFCHK(&pp->ptm_data, &pp->pt_mdlen, valmsgsz(OP_XROPEN_NVALUE)) < 0)
		return(ER_ERR);
	dp = pp->ptm_data;

	/* all ok so fill in the reply message and return */
	TRACE2(Txresd, 4, "OP_XROPEN: return xrid = %s", ltoa(xp->xt_xrid));
	VM_XRID(rp) = xp->xt_xrid;
	rp->vm_nvalue = OP_XROPEN_NVALUE;
	return(ER_OK);

#undef rp
}

/*
**	op_xrsys() - receive system name list for tet_xres file
**
**	in the request message:
**		VM_XRID = xrid to use
**		VM_XSYSID(0) .. VM_XSYSID(OP_XRSYS_NSYS - 1) = system names
*/

void op_xrsys(pp)
register struct ptab *pp;
{
	register struct valmsg *mp = (struct valmsg *) pp->ptm_data;
	register struct xtab *xp;
	register struct uxtab *up;
	register int i, j, nsys;

	/* all reply messages have no data */
	pp->ptm_len = 0;

	/* do a sanity check on the request message */
	if (mp->vm_nvalue < (unsigned short) OP_XRSYS_NVALUE(1)) {
		pp->ptm_rc = ER_INVAL;
		return;
	}

	/* calculate the number of systems in the list */
	nsys = OP_XRSYS_NSYS(mp);

#ifndef NOTRACE
	if (Txresd > 0) {
		TRACE2(Txresd, 4, "OP_XRSYS: nsys = %s", itoa(nsys));
		for (i = 0; i < nsys; i++)
			TRACE2(Txresd, 6, "xrsys: sysid = %s",
				ltoa(VM_XSYSID(mp, i)));
	}
#endif

	/* do some more sanity checks on the request message -
		make sure that there are no duplicate sysids in the list */
	for (i = 0; i < nsys; i++) {
		if (VM_XSYSID(mp, i) < 0) {
			pp->ptm_rc = ER_INVAL;
			return;
		}
		for (j = 0; j < i; j++)
			if (VM_XSYSID(mp, j) == VM_XSYSID(mp, i)) {
				pp->ptm_rc = ER_DUPS;
				return;
			}
	}

	/* find the xtab entry */
	if ((xp = xtfind(VM_XRID(mp))) == (struct xtab *) 0) {
		pp->ptm_rc = ER_XRID;
		return;
	}

	/* make sure that neither IC nor TP have been started */
	if (xp->xt_flags & (XF_ICINPROGRESS | XF_TPINPROGRESS)) {
		pp->ptm_rc = ER_INPROGRESS;
		return;
	}

	/* allocate storage for the xres per-user details list */
	if (uxtalloc(xp, nsys) < 0) {
		pp->ptm_rc = ER_ERR;
		return;
	}

	/* copy in the per-user details list */
	for (i = 0, up = xp->xt_ud; i < xp->xt_nud; i++, up++)
		up->ux_sysid = VM_XSYSID(mp, i);

	/* here if all is ok */
	pp->ptm_rc = ER_OK;
}

/*
**	op_icstart() - signal IC start
*/

void op_icstart(pp)
struct ptab *pp;
{
	register struct valmsg *mp = (struct valmsg *) pp->ptm_data;
	register struct uxtab *up;
	register struct ptab *q;
	register struct xtab *xp;
	register int rc, count;
	struct ptab *pte;
	extern struct ptab *Ptab;

	/* all reply messages have no data */
	pp->ptm_len = 0;

	/* make sure requester is MTCM */
	if (pp->ptr_ptype != PT_MTCM) {
		pp->ptm_rc = ER_PERM;
		return;
	}

	/* make sure that vm_nvalue has a plausible value */
	if ((int) mp->vm_nvalue != OP_ICSTART_NVALUE) {
		pp->ptm_rc = ER_INVAL;
		return;
	}

	TRACE5(Txresd, 4, "OP_ICSTART: xrid = %s, icno = %s, activity = %s, tpcount = %s",
		ltoa(VM_XRID(mp)), ltoa(VM_ICNO(mp)),
		ltoa(VM_ACTIVITY(mp)), ltoa(VM_TPCOUNT(mp)));

	/* find the xtab element */
	if ((xp = xtfind(VM_XRID(mp))) == (struct xtab *) 0) {
		pp->ptm_rc = ER_XRID;
		return;
	}

	/* make sure that neither IC nor TP have been started */
	if (xp->xt_flags & (XF_ICINPROGRESS | XF_TPINPROGRESS)) {
		pp->ptm_rc = ER_INPROGRESS;
		return;
	}

	/* make sure that at least one TCM is logged on from each
		participating system;
		make sure that ux_ptab is filled in
		ptab elements are stored LIFO so we search the table
		backwards to find the first TCM logged on from each system */

	for (pte = Ptab; pte->pt_next; pte = pte->pt_next)
		;

	count = 0;
	for (up = xp->xt_ud; up < xp->xt_ud + xp->xt_nud; up++) {
		if (!up->ux_ptab) {
			for (q = pte; q; q = q->pt_last) {
				if (q->ptr_sysid == up->ux_sysid &&
					(q->pt_flags & PF_LOGGEDON))
						switch (q->ptr_ptype) {
						case PT_MTCM:
						case PT_STCM:
							up->ux_ptab = q;
							break;
						default:
							continue;
						}
				else
					continue;
				break;
			}
		}
		if (!up->ux_ptab)
			error(0, "no TCM logged on from system",
				itoa(up->ux_sysid));
		else
			count++;
	}
	if (count < xp->xt_nud) {
		pp->ptm_rc = ER_SYSID;
		return;
	}

	/* store the values, emit the ICstart message and return */
	xp->xt_icno = (int) VM_ICNO(mp);
	xp->xt_activity = VM_ACTIVITY(mp);
	xp->xt_tpcount = (int) VM_TPCOUNT(mp);
	xp->xt_flags |= XF_ICINPROGRESS;
	rc = xrmsg(xp, TET_JNL_IC_START);
	xp->xt_tpcount = 0;
	pp->ptm_rc = rc;
}

/*
**	op_icend() - receive notification of IC end
*/

void op_icend(pp)
register struct ptab *pp;
{
	register struct valmsg *mp = (struct valmsg *) pp->ptm_data;
	register struct xtab *xp;

	/* all reply messages have no data */
	pp->ptm_len = 0;

	/* make sure requester is MTCM */
	if (pp->ptr_ptype != PT_MTCM) {
		pp->ptm_rc = ER_PERM;
		return;
	}

	/* make sure that vm_nvalue has a plausible value */
	if ((int) mp->vm_nvalue != OP_ICEND_NVALUE) {
		pp->ptm_rc = ER_INVAL;
		return;
	}

	TRACE2(Txresd, 4, "OP_ICEND: xrid = %s", ltoa(VM_XRID(mp)));

	/* find the xtab element */
	if ((xp = xtfind(VM_XRID(mp))) == (struct xtab *) 0) {
		pp->ptm_rc = ER_XRID;
		return;
	}

	/* make sure that an IC is in progress but that a TP is not */
	if ((xp->xt_flags & XF_ICINPROGRESS) == 0) {
		pp->ptm_rc = ER_DONE;
		return;
	}
	else if (xp->xt_flags & XF_TPINPROGRESS) {
		pp->ptm_rc = ER_INPROGRESS;
		return;
	}

	/* mark the IC as finished, emit the ICend message and return */
	xp->xt_flags &= ~XF_ICINPROGRESS;
	pp->ptm_rc = icend(xp);
}

/*
**	op_tpstart() - receive notification of TP start
*/

void op_tpstart(pp)
register struct ptab *pp;
{
	register struct valmsg *mp = (struct valmsg *) pp->ptm_data;
	register struct xtab *xp;
	register struct uxtab *up;

	/* all reply messages have no data */
	pp->ptm_len = 0;

	/* make sure requester is MTCM */
	if (pp->ptr_ptype != PT_MTCM) {
		pp->ptm_rc = ER_PERM;
		return;
	}

	/* make sure that vm_nvalue has a plausible value */
	if ((int) mp->vm_nvalue != OP_TPSTART_NVALUE) {
		pp->ptm_rc = ER_INVAL;
		return;
	}

	TRACE3(Txresd, 4, "OP_TPSTART: xrid = %s, tpno = %s",
		ltoa(VM_XRID(mp)), ltoa(VM_TPNO(mp)));

	/* find the xtab element */
	if ((xp = xtfind(VM_XRID(mp))) == (struct xtab *) 0) {
		pp->ptm_rc = ER_XRID;
		return;
	}

	/* make sure the an IC has been started,
		and that a TP has not already been started */
	if ((xp->xt_flags & XF_ICINPROGRESS) == 0) {
		pp->ptm_rc = ER_DONE;
		return;
	}
	else if (xp->xt_flags & XF_TPINPROGRESS) {
		pp->ptm_rc = ER_INPROGRESS;
		return;
	}

	/* reset the result codes */
	xp->xt_result = -1;
	for (up = xp->xt_ud; up < xp->xt_ud + xp->xt_nud; up++) {
		up->ux_state = XS_NOTREPORTED;
		up->ux_result = -1;
	}

	/* store the values, emit the TPstart message and return */
	xp->xt_flags |= XF_TPINPROGRESS;
	xp->xt_tpno = (int) VM_TPNO(mp);
	pp->ptm_rc = xrmsg(xp, TET_JNL_TP_START);
}

/*
**	op_tpend() - receive notification of TP end
*/

void op_tpend(pp)
register struct ptab *pp;
{
	register struct valmsg *mp = (struct valmsg *) pp->ptm_data;
	register struct xtab *xp;

	/* all reply messages have no data */
	pp->ptm_len = 0;

	/* make sure requester is MTCM */
	if (pp->ptr_ptype != PT_MTCM) {
		pp->ptm_rc = ER_PERM;
		return;
	}

	/* make sure that vm_nvalue has a plausible value */
	if ((int) mp->vm_nvalue != OP_TPEND_NVALUE) {
		pp->ptm_rc = ER_INVAL;
		return;
	}

	TRACE2(Txresd, 4, "OP_TPEND: xrid = %s", ltoa(VM_XRID(mp)));

	/* find the xtab element */
	if ((xp = xtfind(VM_XRID(mp))) == (struct xtab *) 0) {
		pp->ptm_rc = ER_XRID;
		return;
	}

	/* make sure that both an IC and a TP are in progress */
	if ((xp->xt_flags & (XF_ICINPROGRESS | XF_TPINPROGRESS)) != (XF_ICINPROGRESS | XF_TPINPROGRESS)) {
		pp->ptm_rc = ER_DONE;
		return;
	}

	/* mark the TP as finished and return */
	xp->xt_tpcount++;
	xp->xt_flags &= ~XF_TPINPROGRESS;
	pp->ptm_rc = tpend(xp);
}

/*
**	op_xres() - receive xres lines from clients
*/

void op_xres(pp)
register struct ptab *pp;
{
	register struct avmsg *mp = (struct avmsg *) pp->ptm_data;
	register struct xtab *xp;

	/* all reply messages have no data */
	pp->ptm_len = 0;

	TRACE2(Txresd, 4, "OP_XRES: xrid = %s", ltoa(AV_XRID(mp)));

	/* find the xtab element for this request */
	if ((xp = xtfind(AV_XRID(mp))) == (struct xtab *) 0) {
		pp->ptm_rc = ER_XRID;
		return;
	}

	/* write out the lines to the tet_xres file */
	pp->ptm_rc = xrwrite(xp, mp);
}

/*
**	op_result() - receive a TP result
*/

void op_result(pp)
register struct ptab *pp;
{
	register struct valmsg *mp = (struct valmsg *) pp->ptm_data;
	register struct xtab *xp;
	register struct uxtab *up;
	register int result;

	/* all reply messages have no data */
	pp->ptm_len = 0;

	/* do a sanity check on the request */
	if (mp->vm_nvalue != OP_RESULT_NVALUE) {
		pp->ptm_rc = ER_INVAL;
		return;
	}

	TRACE3(Txresd, 4, "OP_RESULT: xrid = %s, result = %s",
		ltoa(VM_XRID(mp)), ltoa(VM_RESULT(mp)));

	/* check that the result code is within the range
		allowed by the spec */
	if ((result = (int) VM_RESULT(mp)) < 0 || result > 127) {
		pp->ptm_rc = ER_INVAL;
		return;
	}

	/* find the xtab element for this request */
	if ((xp = xtfind(VM_XRID(mp))) == (struct xtab *) 0) {
		pp->ptm_rc = ER_XRID;
		return;
	}

	/* make sure that a TP is in progress */
	if ((xp->xt_flags & (XF_ICINPROGRESS | XF_TPINPROGRESS)) != (XF_ICINPROGRESS | XF_TPINPROGRESS)) {
		pp->ptm_rc = ER_DONE;
		return;
	}

	/* make sure that this system is participating in the test */
	for (up = xp->xt_ud; up < xp->xt_ud + xp->xt_nud; up++)
		if (up->ux_sysid == pp->ptr_sysid)
			break;
	if (up >= xp->xt_ud + xp->xt_nud) {
		pp->ptm_rc = ER_SYSID;
		return;
	}

	/* register the result */
	switch (up->ux_state) {
	default:
		error(0, "internal error: unknown uxtab state",
			itoa(up->ux_state));
		result = addresult(up->ux_result, result);
		/* fall through */
	case XS_NOTREPORTED:
		up->ux_result = result;
		up->ux_state = XS_REPORTED;
		break;
	case XS_DEAD:
	case XS_REPORTED:
		up->ux_result = addresult(up->ux_result, result);
		break;
	}

	pp->ptm_rc = ER_OK;
}

/*
**	icend() - perform IC end processing
**
**	return ER_OK if successful or other ER_* error code on error
*/

int icend(xp)
register struct xtab *xp;
{
	/* make sure the event was expected */
	if (xp->xt_flags & XF_ICINPROGRESS) {
		xrinfo(xp, "MTCM did not signal IC end", (char *) 0);
		xp->xt_flags &= ~XF_ICINPROGRESS;
	}
	xp->xt_flags |= XF_ICDONE;

	/* emit the IC end message and return */
	return(xrmsg(xp, TET_JNL_IC_END));
}

/*
**	tpend() - perform TP end processing
**
**	return ER_OK if successful or other ER_* error code on error
*/

int tpend(xp)
register struct xtab *xp;
{
	register struct uxtab *up;
	register int result;

	/* make sure that the event was expected */
	if (xp->xt_flags & XF_TPINPROGRESS) {
		xrinfo(xp, "MTCM did not signal TP end", (char *) 0);
		xp->xt_flags &= ~XF_TPINPROGRESS;
	}
	xp->xt_flags |= XF_TPDONE;

	TRACE4(Txresd, 6, "tpend() called, xrid = %s, icno = %s, tpno = %s",
		ltoa(xp->xt_xrid), itoa(xp->xt_icno), itoa(xp->xt_tpno));

	/* consolidate the per-system results */
	xp->xt_result = -1;
	for (up = xp->xt_ud; up < xp->xt_ud + xp->xt_nud; up++) {
		switch (up->ux_state) {
		case XS_DEAD:
			if (up->ux_result < 0) {
				result = TET_UNRESOLVED;
				break;
			}
			xrinfo(xp, "TCM exited before TP end, system",
				itoa(up->ux_sysid));
			/* fall through */
		case XS_REPORTED:
			result = up->ux_result;
			break;
		case XS_NOTREPORTED:
			result = TET_UNRESOLVED;
			break;
		}
		xp->xt_result = addresult(xp->xt_result, result);
		TRACE6(Txresd, 6, "sysid = %s, state = %s, sys result = %s, use result = %s, cons result = %s",
			itoa(up->ux_sysid), xrstate(up->ux_state),
			itoa(up->ux_result), itoa(result),
			itoa(xp->xt_result));
	}

	/* emit the TP result message and return */
	return(xrmsg(xp, TET_JNL_TP_RESULT));
}

/*
**	xrmsg() - emit an {IC|TP}{start|end} message to the tet_xres file
**
**	return ER_OK if successful or other ER_* error code on error
**
**	as a special case, ER_ABORT is returned if code is TET_JNL_TP_RESULT
**	and the action for the consolidated result code is to abort all TCMs
*/

static char wrfail[] = "write failed on";

static int xrmsg(xp, code)
register struct xtab *xp;
register int code;
{
	time_t now;
	register struct tm *tp;
	register char *text;
	int abflag = 0;
	char buf[64];

	/* format the variable part of the message */
	switch (code) {
	case TET_JNL_IC_START:
		(void) sprintf(buf, "%d %d", xp->xt_icno, xp->xt_tpcount);
		text = "IC Start";
		break;
	case TET_JNL_IC_END:
		(void) sprintf(buf, "%d %d", xp->xt_icno, xp->xt_tpcount);
		text = "IC End";
		break;
	case TET_JNL_TP_START:
		(void) sprintf(buf, "%d", xp->xt_tpno);
		text = "TP Start";
		break;
	case TET_JNL_TP_RESULT:
		(void) sprintf(buf, "%d %d", xp->xt_tpno, xp->xt_result);
		if ((text = get_code(xp->xt_result, &abflag)) == (char *) 0)
			text = "(NO RESULT NAME)";
		break;
	default:
		error(0, "unexpected xres code", itoa(code));
		return(ER_ERR);
	}

	/* output the message */
	now = time((time_t *) 0);
	tp = localtime(&now);
	if (fprintf(xp->xt_xfp, "%d|%ld %s %02d:%02d:%02d|%s\n",
		code, xp->xt_activity, buf,
		tp->tm_hour, tp->tm_min, tp->tm_sec, text) < 0 ||
			fflush(xp->xt_xfp) < 0) {
				error(errno, wrfail, xp->xt_xfname);
				return(ER_ERR);
	}

	/* report an Abort result code */
	if (abflag == 1) {
		(void) sprintf(buf, "ABORT on result code %d:", xp->xt_result);
		xrinfo(xp, buf, text);
		return(ER_ABORT);
	}

	return(ER_OK);
}

/*
**	xrinfo() - write a TCM/API error message to the xres file
*/

static void xrinfo(xp, s1, s2)
struct xtab *xp;
char *s1, *s2;
{
	if (fprintf(xp->xt_xfp, "%d|%ld|%s%s%s\n", TET_JNL_TCM_INFO,
		xp->xt_activity, s1, s2 && *s2 ? " " : "", s2 ? s2 : "") < 0 ||
			fflush(xp->xt_xfp) < 0)
				error(errno, wrfail, xp->xt_xfname);
}

/*
**	xrwrite() - write message lines to the xres file
**
**	return ER_OK if successful or other ER_* error code on error
*/

static int xrwrite(xp, mp)
register struct xtab *xp;
register struct avmsg *mp;
{
	register char *s;
	register int n;

	/* write out the lines to the tmp file */
	for (n = 0; n < OP_XRES_NLINE(mp); n++) {
		if ((s = AV_XLINE(mp, n)) == (char *) 0)
			continue;
		TRACE2(Txresd, 8, "xrwrite: line = \"%.40s\"", s);
		if (fprintf(xp->xt_xfp, "%s\n", s ? s : "<null>") < 0) {
			error(errno, wrfail, xp->xt_xfname);
			return(ER_ERR);
		}
	}

	/* flush out the file's stdio buffer */
	if (fflush(xp->xt_xfp) < 0) {
		error(errno, wrfail, xp->xt_xfname);
		return(ER_ERR);
	}

	return(ER_OK);
}

/*
**	addresult() - arbitrate between result codes
**
**	return result with highest priority
**
**	this function inplements the logic in tet_tpend() in the
**		(non-distributed) TET
*/

static int addresult(lastresult, thisresult)
register int lastresult, thisresult;
{
	if (lastresult < 0)
		return(thisresult);

	switch (thisresult) {
	case TET_PASS:
		return(lastresult);
	case TET_FAIL:
		return(thisresult);
	case TET_UNRESOLVED:
	case TET_UNINITIATED:
		switch (lastresult) {
		case TET_FAIL:
			return(lastresult);
		default:
			return(thisresult);
		}
	case TET_NORESULT:
		switch (lastresult) {
		case TET_FAIL:
		case TET_UNRESOLVED:
		case TET_UNINITIATED:
			return(lastresult);
		default:
			return(thisresult);
		}
	case TET_UNSUPPORTED:
	case TET_NOTINUSE:
	case TET_UNTESTED:
		switch (lastresult) {
		case TET_PASS:
			return(lastresult);
		default:
			return(thisresult);
		}
	default:
		switch (lastresult) {
		case TET_PASS:
		case TET_UNSUPPORTED:
		case TET_NOTINUSE:
		case TET_UNTESTED:
			return(thisresult);
		default:
			return(lastresult);
		}
	}
}

/*
**	xrstate() - return printable representation of system result state
*/

#ifndef NOTRACE
static char *xrstate(state)
int state;
{
	static char text[] = "xres-state ";
	static char msg[sizeof text + LNUMSZ];

	switch (state) {
	case XS_NOTREPORTED:
		return("NOTREPORTED");
	case XS_REPORTED:
		return("REPORTED");
	case XS_DEAD:
		return("DEAD");
	default:
		(void) sprintf(msg, "%s%d", text, state);
		return(msg);
	}
}
#endif /* NOTRACE */

