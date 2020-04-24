/*
 *      SCCS:  @(#)fake.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)fake.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)fake.c	1.2 92/09/30
NAME:		fake.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	fake client-specific functions for tcm

MODIFICATIONS:

************************************************************************/


#include <sys/types.h>
#include <dtmsg.h>
#include "ptab.h"
#include "error.h"
#include "server.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	ss_serverloop() - server-specific server loop
**
**	this may be called from si_servwait() if non-blocking message i/o
**	would block
**
**	tcm does not do non-blocking i/o, so this should never occur
*/

int ss_serverloop()
{
	error(0, "internal error - serverloop called!", (char *) 0);
	return(-1);
}

/*
**	ss_process() - server-specific request process routine
**
**	would be called from si_service() when state is PS_PROCESS
**
**	tcm only uses si_clientloop() which itself returns as soon as a
**	process reaches this state, so ss_process() should never be called
**/

void ss_process(pp)
struct ptab *pp;
{
	error(0, "internal error - ss_process called!", rtoa(&pp->pt_rid));
}

