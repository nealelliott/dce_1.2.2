/*
 *      SCCS:  @(#)sync.c	1.3 (92/11/04) 
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
static char sccsid[] = "@(#)sync.c	1.3 (92/11/04) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)sync.c	1.3 92/11/04
NAME:		sync.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

SYNOPSIS:
	#include <dapi.h>
	int tet_sync(int syncptno, int *syncnames, int waittime);

DESCRIPTION:
	DTET API function

	perform a user sync operation
	return 0 if successful or -1 on error

MODIFICATIONS:

************************************************************************/


#if __STDC__
#include <stdio.h>
#endif

#include "dtmac.h"
#include "dtmsg.h"
#include "synreq.h"
#include "error.h"
#include "servlib.h"
#include "dtetlib.h"
#include "dapi.h"
#include "dtet/tet_jrnl.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif


int tet_sync(syncptno, syncnames, waittime)
int syncptno, *syncnames, waittime;
{
	register struct synreq *sp;
	register int *ip;
	register int nsys;
	register char *p;
	char buf[512];
	static char nullstr[] = "";
	static char sstr[] = "s";
	static struct synreq *synreq;
	static int synlen;
	extern int Mysysid, Myptype;
	extern long Snid, Xrid;
	extern void tet_error();

	/* make sure that syncnames is non-empty for the master system */
	if (Myptype == PT_MTCM && (!syncnames || !*syncnames)) {
		error(0, "syncnames empty on master system", (char *) 0);
		return(-1);
	}

	/* count the syncnames;
		on slave systems, add one for the master system */
	nsys = (Myptype == PT_MTCM) ? 0 : 1;
	if (syncnames)
		for (ip = syncnames; *ip; ip++)
			if (*ip != Mysysid)
				nsys++;

	/* make sure that the synreq buffer is big enough */
	ASSERT(nsys > 0);
	if (BUFCHK((char **) &synreq, &synlen, (int) (nsys * sizeof *synreq)) < 0)
		return(-1);

	/* copy the system names in to the synreq buffer */
	sp = synreq;
	if (Myptype != PT_MTCM) {
		sp->sy_sysid = 0;
		(sp++)->sy_ptype = PT_MTCM;
	}
	if (syncnames)
		for (ip = syncnames; *ip; ip++)
			if (*ip != Mysysid) {
				sp->sy_sysid = *ip;
				(sp++)->sy_ptype = PT_STCM;
			}

	/* perform the sync operation and handle the reply codes */
	if (sd_usync(Snid, Xrid, (long) syncptno, SV_YES, waittime, synreq, nsys) < 0) {
		switch (sd_errno) {
		case ER_DONE:
			break;
		case ER_INVAL:
			error(0, "invalid request parameter", (char *) 0);
			return(-1);
		case ER_DUPS:
			error(0, "duplicates in syncnames list", (char *) 0);
			/* fall through */
		default:
			return(-1);
		}
	}

	/* return now if request succeeded */
	if (sd_errno == ER_OK)
		return(0);

	/* here if the request failed in an "expected" way -
		emit a TCM info message detailing processes that caused the
		sync to fail (as required by the spec) */
	(void) sprintf(buf, "tet_sync() failed, syncptno = %d, ", syncptno);
	p = buf + strlen(buf);
	switch (sd_errno) {
	case ER_SYNCERR:
		(void) sprintf(p, "%sother system%s synced unsuccessfully or timed out", nsys > 1 ? "one or more of the " : nullstr, nsys == 1 ? nullstr : sstr);
		break;
	case ER_TIMEDOUT:
		(void) sprintf(p, "request timed out after waittime of %d second%s", waittime, waittime == 1 ? nullstr : sstr);
		break;
	case ER_DONE:
		(void) sprintf(p, "event already happened");
		break;
	default:
		(void) sprintf(p, "internal error, sd_errno = %s",
			ptrepcode(sd_errno));
		break;
	}

	/* write the error to the tet_xres file and the stderr */
	tet_error(0, buf);

	switch (sd_errno) {
	case ER_SYNCERR:
	case ER_TIMEDOUT:
		for (sp = synreq; sp < synreq + nsys; sp++) {
			(void) sprintf(buf, "system = %2d, state = %s",
				sp->sy_sysid, systate(sp->sy_state));
			tet_error(0, buf);
		}
		break;
	}

	return(-1);
}

