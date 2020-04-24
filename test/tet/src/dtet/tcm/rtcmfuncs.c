/*
 *      SCCS:  @(#)rtcmfuncs.c	1.3 (92/10/06) 
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
static char sccsid[] = "@(#)rtcmfuncs.c	1.3 (92/10/06) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)rtcmfuncs.c	1.3 92/10/06
NAME:		rtcmfuncs.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	May 1992

DESCRIPTION:
	client-related functions for processes started by tet_remexec()

MODIFICATIONS:
	Andrew Dingwall, UniSoft Ltd., October 1992
	Most of the code that was in this file is now in child.c and is
	build into dtcmchild.o as well as tcmrem.o.
	This file now only contains code specific to tcmrem.o.

************************************************************************/

#include "dtmsg.h"
#include "synreq.h"
#include "tcmfuncs.h"


/*
**	tet_callfuncname() - return name of tcmrem's calling function
**		for use in error messages
*/

char *tet_callfuncname()
{
	return("tet_remexec()");
}

/*
**	tcmptype() - return process type for slave TCM
*/

int tcmptype()
{
	return(PT_STCM);
}

/*
**	doasync() - do an automatic sync from a tcmrem STCM
*/

int doasync(spno, vote, timeout, synreq, nsys)
long spno;
int vote, timeout, *nsys;
struct synreq *synreq;
{
	long xrid;
	extern long Snid;

	return(sd_sasync(Snid, spno, vote, timeout, synreq, nsys, &xrid));
}

