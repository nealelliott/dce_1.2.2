/*
 *      SCCS:  @(#)remwait.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)remwait.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)remwait.c	1.2 92/09/30
NAME:		remwait.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

SYNOPSIS:
	#include <dapi.h>
	int tet_remwait(int remoteid, int waittime, int *statloc);

DESCRIPTION:
	DTET API function

	wait for process started by tet_remexec()
	return 0 if successful or -1 on error
	if successful, the exit status of the remote process is returned
	indirectly through *statloc

MODIFICATIONS:

************************************************************************/


#include <errno.h>
#include <sys/types.h>
#include "dtmsg.h"
#include "ptab.h"
#include "rtab.h"
#include "valmsg.h"
#include "servlib.h"
#include "dapi.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif


int tet_remwait(remoteid, waittime, statloc)
int remoteid, waittime, *statloc;
{
	register struct rtab *rp;
	register int rc;

	/* see if the process was started by tet_remexec() */
	if ((rp = rtfind(remoteid)) == (struct rtab *) 0) {
		errno = EINVAL;
		return(-1);
	}

	/* see if the process has already been waited for */
	if (rp->rt_pid < 0L) {
		errno = ECHILD;
		return(-1);
	}

	/* do the remote wait operation and handle the reply code */
	if (tc_wait(rp->rt_sysid, rp->rt_pid, waittime, statloc) < 0)
		switch (tc_errno) {
		case ER_WAIT:
		case ER_TIMEDOUT:
			errno = EAGAIN;
			return(-1);
		case ER_EINTR:
			errno = EINTR;
			return(-1);
		case ER_PID:
			errno = ECHILD;
			rc = -1;
			break;
		default:
			errno = EIO;
			rc = -1;
			break;
		}
	else
		rc = 0;

	/* here to show the process as no longer running */
	rp->rt_pid = -1L;
	return(rc);
}

