/*
 *      SCCS:  @(#)mtcmdist.c	1.4 (92/11/10) 
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
static char sccsid[] = "@(#)mtcmdist.c	1.4 (92/11/10) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)mtcmdist.c	1.4 92/12/11
NAME:		mtcmdist.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		David G. Sawyer, Unisoft Ltd.
DATE CREATED:	June 1992

DESCRIPTION:
	Master TCM DTET-specific functions

MODIFICATIONS:
	Andrew Dingwall, UniSoft Ltd., October 1992
	Moved tet_xdline() and tet_xdresult() to apilib/dresfile.c.
	Put IC/TP start before initial auto-sync so as to make sure that
	XRESD is ready before IC/TP starts in TCMs.

	Moved all common code from [ms]tcmdist.c to ictp.c.
	[ms]tcmdist.c now only contain [MS]TCM-specific code.
	Updated Abort and Delete processing to work in the DTET environment.

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include "error.h"
#include "dtmac.h"
#include "dtmsg.h"
#include "synreq.h"
#include "servlib.h"
#include "dtetlib.h"
#include "tcmfuncs.h"
#include "dtet/tet_api.h"
#include "dtet/tet_jrnl.h"

extern long Xrid;
extern int *Snames;
extern int Nsname;
extern int sd_errno;
extern int xd_errno;

/* variables in apilib/dresfile.c */
extern long tet_activity, tet_block, tet_sequence;


/*
**	tet_opencom() - get XRESD to open the combined execution results file
**		and send down the list of system names
*/

void tet_opencom()
{
	register char *cwd;
	char filename[MAXPATH];
	static char fname[] = "tet_combined";
	extern int combined_ok;

	extern int Mysysid;

	if (Mysysid != 0)
	  return;

	/* Build up the combined execution results file name */
	if ((cwd = getcwd((char *) 0, _POSIX_PATH_MAX)) == (char *) 0) {
		tet_error(errno, "can't establish current working directory");
		tet_exit(EXIT_FAILURE);
	}
	(void) sprintf(filename, "%.*s/%s", sizeof filename - sizeof fname - 2,
		cwd, fname);

	/* Remove the old file if it's there */
	(void) unlink(filename);

	/* Tell tetxresd the name of the file - so that it can be opened */
	if ((Xrid = xd_xropen(filename)) < 0L) {
		tet_error(xd_errno,
			"XRESD can't open combined execution results file");
		tet_exit(EXIT_FAILURE);
	}
	combined_ok = 1;

	/* Inform tetxresd of the systems participating */
	if (xd_xrsys(Xrid, Snames, Nsname) < 0) {
		tet_error(xd_errno, "can't send system list to XRESD");
		tet_exit(EXIT_FAILURE);
	}
}


/*
**	tet_xdicstart() - inform XRESD of IC start from MTCM
*/

void tet_xdicstart(icno, tpcount)
int icno, tpcount;
{
	extern int Mysysid;

	if (Mysysid != 0)
	  return;

	if (xd_icstart(Xrid, icno, tet_activity, tpcount) < 0) {
		tet_error(xd_errno, "can't inform XRESD of IC start");
		tet_exit(EXIT_FAILURE);
	}
}

/*
**	tet_xdicend() - inform XRESD of IC end from MTCM
*/

/* ARGSUSED */
void tet_xdicend(icno, tpcount)
int icno, tpcount;
{
	extern int Mysysid;

	if (Mysysid != 0)
	  return;

	if (xd_icend(Xrid) < 0) {
		tet_error(xd_errno, "can't inform XRESD of IC end");
		tet_exit(EXIT_FAILURE);
	}
}

/*
**	tet_xdtpstart() - inform XRESD of TP start from MTCM
*/

/* ARGSUSED */
void tet_xdtpstart(icno, tpno)
int icno, tpno;
{
	extern int Mysysid;

	if (Mysysid != 0)
	  return;

	if (xd_tpstart(Xrid, tpno) < 0) {
		tet_error(xd_errno, "can't inform XRESD of TP start");
		tet_exit(EXIT_FAILURE);
	}
}

/*
**	tet_xdtpend() - inform XRESD of TP end from MTCM
*/

/* ARGSUSED */
void tet_xdtpend(icno, tpno)
int icno, tpno;
{
	char errmsg[128];
	struct synreq *synreq;
	int nsys, errflag;
	register struct synreq *sp;
	extern int tet_iclast;

	extern int Mysysid;

	if (Mysysid != 0)
	  return;

	(void) xd_tpend(Xrid);
	switch (xd_errno) {
	case ER_OK:
		return;
	case ER_ABORT:
		break;
	default:
		tet_error(xd_errno, "can't inform XRESD of TP end");
		tet_exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	/* here if previous TP result code action was Abort -
		allocate memory for synreq array; don't worry if it fails */
	errno = 0;
	if ((synreq = (struct synreq *) malloc(sizeof *synreq * Nsname)) == (struct synreq *) 0) {
		tet_error(errno, " can't allocate synreq array for Abort sync");
		nsys = 0;
	}
	else
		nsys = Nsname;

	/* sync NO to the end of the last IC */
	if (doasync(MK_ASPNO(tet_iclast, ~0, S_ICEND), SV_NO, SV_SYNC_TIMEOUT,
		synreq, &nsys) < 0) {
			tet_error(sd_errno, "Abort Auto Sync failed");
			tet_exit(EXIT_FAILURE);
	}

	/* make sure that the other TCMs are still alive */
	if (synreq) {
		errflag = 0;
		for (sp = synreq; sp < synreq + nsys; sp++)
			switch (sp->sy_state) {
			case SS_SYNCYES:
			case SS_SYNCNO:
				break;
			default:
				(void) sprintf(errmsg,
			"Abort Auto Sync error, sysid = %d, state = %s",
					sp->sy_sysid, systate(sp->sy_state));
				tet_error(sd_errno, errmsg);
				errflag = 1;
				break;
			}
		if (errflag)
			tet_exit(EXIT_FAILURE);
	}

	/* here if we should attempt to call (*tet_cleanup)() */
	tet_docleanup();
	tet_exit(EXIT_FAILURE);
}

/*
**	tet_tcmstart() - inform XRESD of TCM start
*/

void tet_tcmstart(versn, no_ics)
char *versn;
int no_ics;
{
	char buf[128];
	extern int Mysysid;

	if (Mysysid != 0)
	  return;

	(void) sprintf(buf, "%d|%ld %s %d|TCM Start", 
		TET_JNL_TCM_START, tet_activity, versn, no_ics);

	if (xd_xres(Xrid, buf) < 0) {
		tet_error(xd_errno, "can't inform XRESD of TCM start");
		tet_exit(EXIT_FAILURE);
	}
}

