/*
 * Copyright 1990 Open Software Foundation (OSF)
 * Copyright 1990 Unix International (UI)
 * Copyright 1990 X/Open Company Limited (X/Open)
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of OSF, UI or X/Open not be used in 
 * advertising or publicity pertaining to distribution of the software 
 * without specific, written prior permission.  OSF, UI and X/Open make 
 * no representations about the suitability of this software for any purpose.  
 * It is provided "as is" without express or implied warranty.
 *
 * OSF, UI and X/Open DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, 
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO 
 * EVENT SHALL OSF, UI or X/Open BE LIABLE FOR ANY SPECIAL, INDIRECT OR 
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF 
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR 
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR 
 * PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef lint
static char sccsid[] = "@(#)ictp.c	1.3 (92/11/12) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)ictp.c	1.3 92/11/12
NAME:		ictp.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Geoff Clare, UniSoft Ltd.
DATE CREATED:	25 July 1990
DESCRIPTION:

	tet_icstart(), tet_icend(), tet_tpstart() and
	tet_tpend() are used by the TCM to output TCM start lines, IC
	start and end lines and test purpose start and result lines to
	the combined execution results file maintained by XRESD

MODIFICATIONS:
	
	June 1992
	DTET development - apilib/dresfile.c is derived from
	TET release 1.10

	Andrew Dingwall, UniSoft Ltd,. October 1992
	Some non-API functions specific to parent TCMs moved from
	apilib/dresfile.c to here.

	All vestages of the local execution results file and temporary
	execution results file removed - in the DTET, all the processing
	associated with these files is done by XRESD.

	All these functions completely re-written - auto-syncs are used
	to pass deletion and Abort information between TCMs as follows:

		if a TP is deleted within a TCM, that TCM votes NO at the
		TP start sync point
		if a TCM receives a NO sync vote at TP start, it knows that
		the TP has been deleted in another TCM

		if a result code action is Abort, XRESD returns ER_ABORT
		when the MTCM calls xd_tpend() from tet_tpend();
		the MTCM syncs with a NO vote to the end of the last TP

		STCMs will receive this NO vote at the start of any remaining
		TPs in the current IC, thus causing them to believe that
		these TPs have been deleted in the MTCM

		when STCMs receive the NO vote at the start of the next IC,
		they will interpret it as the signal to perform Abort
		processing

	Andrew Dingwall, UniSoft Ltd., November 1992
	Increased tpend sync timeout to 10 mins so as to allow for
	larger inbalance between test part execution times.

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <stdlib.h>
#include <errno.h>
#include "dtmsg.h"
#include "synreq.h"
#include "dtetlib.h"
#include "tcmfuncs.h"
#include "dtet/tet_api.h"

/* variables in apilib/dresfile.c */
extern long tet_activity, tet_block, tet_sequence;

extern struct synreq *tet_synreq;	/* to receive sync reply info */
extern int Nsname;	/* number of systems - in tcmfuncs.c */
extern int sd_errno;	/* SYNCD reply code */

/*
**	tet_icstart() - signal IC start
**
**	if the previous result code action was Abort, this function is only
**	executed by STCMs
*/

void tet_icstart(icno, tpcount)
int icno, tpcount;
{
	register struct synreq *sp;
	int nsys = Nsname;
	int errflag;
	char errmsg[128];

	/* the MTCM informs XRESD of IC start */
	tet_xdicstart(icno, tpcount);

	/* then all TCMs sync on IC start -
		if the MTCM votes NO, this means that the previous result
		code action was Abort */
	if (doasync(MK_ASPNO(icno, 0, S_ICSTART), SV_YES, SV_SYNC_TIMEOUT,
		tet_synreq, &nsys) < 0) {
			(void) sprintf(errmsg,
				"Auto Sync failed at start of IC %d", icno);
			tet_error(sd_errno, errmsg);
			tet_exit(EXIT_FAILURE);
	}

	if (sd_errno == ER_OK)
		return;

	/* here if the auto sync failed in an expected way -
		a NO vote from the MTCM means that the test is to be aborted */
	errflag = 0;
	for (sp = tet_synreq; sp < tet_synreq + nsys; sp++)
		switch (sp->sy_state) {
		case SS_SYNCYES:
			break;
		case SS_SYNCNO:
			if (sp->sy_sysid == 0)
				break;
			/* else fall through */
		default:
			(void) sprintf(errmsg,
		"Auto sync error at start of IC %d, sysid = %d, state = %s",
				icno, sp->sy_sysid, systate(sp->sy_state));
			tet_error(sd_errno, errmsg);
			errflag = 1;
			break;
		}


	if (errflag)
		tet_exit(EXIT_FAILURE);

	/* here if MTCM voted NO - cleanup and exit */
	tet_docleanup();
	tet_exit(EXIT_FAILURE);
}

/*
**	tet_icend() - signal IC end
*/

void tet_icend(icno, tpcount)
int icno, tpcount;
{
	/* the MTCM informs XRESD of IC end */
	tet_xdicend(icno, tpcount);
}

/*
**	tet_tpstart() - signal TP start
*/

void tet_tpstart(icno, tpno)
int icno, tpno;
{
	register struct synreq *sp;
	register int vote;
	int nsys = Nsname;
	int errflag;
	char errmsg[128];
	extern int tet_sync_del;

	tet_block = 1;
	tet_sequence = 1;

	/* the MTCM informs XRESD of TP start */
	tet_xdtpstart(icno, tpno);

	/* see if the TP has been deleted in this TCM and set our vote
		accordingly */
	vote = tet_reason(tpno) ? SV_NO : SV_YES;

	/* then all the TCMs sync on TP start */
	if (doasync(MK_ASPNO(icno, tpno, S_TPSTART), vote, SV_SYNC_TIMEOUT,
		tet_synreq, &nsys) < 0) {
			(void) sprintf(errmsg,
				"Auto Sync failed at start of TP %d", tpno);
			tet_error(sd_errno, errmsg);
			tet_exit(EXIT_FAILURE);
	}

	if (sd_errno == ER_OK)
		return;

	/* here if sync failed in an expected way -
		a NO vote means that the TP has been deleted in another TCM */
	errflag = 0;
	for (sp = tet_synreq; sp < tet_synreq + nsys; sp++)
		switch (sp->sy_state) {
		case SS_SYNCYES:
			break;
		case SS_SYNCNO:
			tet_sync_del = 1;
			break;
		default:
			(void) sprintf(errmsg,
		"Auto Sync error at start of TP %d, sysid = %d, state = %s",
				tpno, sp->sy_sysid, systate(sp->sy_state));
			tet_error(sd_errno, errmsg);
			errflag = 1;
			break;
		}

	if (errflag)
		tet_exit(EXIT_FAILURE);
}

void tet_tpend(icno, tpno)
int icno, tpno;
{
	register struct synreq *sp;
	int err;
	int nsys = Nsname;
	char errmsg[128];

	/* all the TCMs sync YES on TP end -
		there is an assumption here that the parts of a TP will
		arrive at their ends within 10 minutes of each other;
		if this is not so for a particular test, the test writer
		is expected to use tet_sync() with a longer timeout
		to delay the ends of the quicker test parts */
	if (doasync(MK_ASPNO(icno, tpno, S_TPEND), SV_YES,
		SV_SYNC_TIMEOUT * 10, tet_synreq, &nsys) < 0) {
			(void) sprintf(errmsg, 
				"Auto Sync failed at end of TP %d", tpno);
			tet_error(sd_errno, errmsg);
			tet_exit(EXIT_FAILURE);
	}
	err = sd_errno;

	/* then the MTCM informs XRESD of TP end -
		if XRESD returns ER_ABORT when xd_tpend() is called, this
		function does not return in the MTCM */
	tet_xdtpend(icno, tpno);

	if (err == ER_OK)
		return;

	/* here if a TCM voted NO ("can't happen"), timed out or died */
	for (sp = tet_synreq; sp < tet_synreq + nsys; sp++)
		switch (sp->sy_state) {
		case SS_SYNCYES:
			break;
		default:
			(void) sprintf(errmsg,
		"Auto Sync error at end of TP %d, sysid = %d, state = %s",
				tpno, sp->sy_sysid, systate(sp->sy_state));
			tet_error(err, errmsg);
			break;
		}

	tet_exit(EXIT_FAILURE);
}

