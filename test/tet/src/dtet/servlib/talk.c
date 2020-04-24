/*
 *      SCCS:  @(#)talk.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)talk.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)talk.c	1.2 92/09/30
NAME:		talk.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to talk to a server

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <sys/types.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "error.h"
#include "servlib.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	ti_talk() - talk to a server and receive a reply
**
**	return 0 if successful or -1 on error
*/

int ti_talk(pp, delay)
struct ptab *pp;
int delay;
{
	register int rc;

	si_clientloop(pp, delay);

	if (pp->pt_state == PS_DEAD || (pp->pt_flags & PF_IOERR))
		rc = -1;
	else if (pp->pt_flags & PF_TIMEDOUT) {
		pp->pt_flags &= ~PF_TIMEDOUT;
		error(0, "server timed out", rtoa(&pp->pt_rid));
		rc = -1;
	}
	else
		rc = 0;

	return(rc);
}

