/*
 *      SCCS:  @(#)stcmfuncs.c	1.3 (92/12/02) 
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
static char sccsid[] = "@(#)stcmfuncs.c	1.3 (92/12/02) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)stcmfuncs.c	1.3 92/12/02
NAME:		stcmfuncs.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	slave tcm client-related functions

MODIFICATIONS:

************************************************************************/

#include "dtmsg.h"
#include "error.h"
#include "synreq.h"
#include "tcmfuncs.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif


/*
**	ti_tcminit() - transport-independent initialisation for slave TCM
*/

void ti_tcminit()
{
	extern int Mysysid;

	if (!Mysysid)
		fatal(0, "Incorrectly linked with STCM. Should be linked with MTCM", (char *) 0);
}

/*
**	tcmptype() - return process type for slave TCM
*/

int tcmptype()
{
	return(PT_STCM);
}

/*
**	doasync() - do an automatic sync from the STCM
*/

int doasync(spno, vote, timeout, synreq, nsys)
long spno;
int vote, timeout, *nsys;
struct synreq *synreq;
{
	register int rc;
	long xrid;
	extern long Snid, Xrid;

	xrid = -1L;

	rc = sd_sasync(Snid, spno, vote, timeout, synreq, nsys, &xrid);

	if (xrid > 0L)
		Xrid = xrid;

	return(rc);
}

