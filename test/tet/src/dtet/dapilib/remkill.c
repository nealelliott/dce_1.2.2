/*
 *      SCCS:  @(#)remkill.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)remkill.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)remkill.c	1.2 92/09/30
NAME:		remkill.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

SYNOPSIS:
	#include <dapi.h>
	int tet_remkill(int remoteid);

DESCRIPTION:
	DTET API function

	terminate process started by tet_remexec()
	return 0 if successful or -1 on error

MODIFICATIONS:

************************************************************************/


#include <signal.h>
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


int tet_remkill(remoteid)
int remoteid;
{
	register struct rtab *rp;

	/* see if the process was started by tet_remexec(),
		and has not aready been waited for */
	if ((rp = rtfind(remoteid)) == (struct rtab *) 0 || rp->rt_pid < 0L) {
		errno = EINVAL;
		return(-1);
	}

	/* do the remote kill operation and handle the reply code */
	if (tc_kill(rp->rt_sysid, rp->rt_pid, SIGTERM) < 0) {
		switch (tc_errno) {
		case ER_PID:
			errno = EINVAL;
			break;
		default:
			errno = EIO;
			break;
		}
		return(-1);
	}

	return(0);
}

