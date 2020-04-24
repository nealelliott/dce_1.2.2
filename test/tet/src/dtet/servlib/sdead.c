/*
 *      SCCS:  @(#)sdead.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)sdead.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)sdead.c	1.2 92/09/30
NAME:		sdead.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	dead process handler function

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <sys/types.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "server.h"
#include "servlib.h"
#include "tslib.h"
#include "dtetlib.h"

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#else
extern void exit();
#endif

/*
**	so_dead() - server-only dead process routine
**
**	called when a client dies and we don't need the proc table entry
**	any more
*/

void so_dead(pp)
struct ptab *pp;
{
	extern struct ptab *Ptab;

	TRACE2(Tserv, 4, "%s process dead", rtoa(&pp->pt_rid));

	/* remove the ptab entry and free it */
	ptrm(pp);
	ptfree(pp);

	/* if no connected processes, wait a bit just in case anyone
		still wants to connect - exit if not */
	if (!Ptab && ts_poll(Ptab, SHORTDELAY) <= 0) {
		TRACE1(Tserv, 2, "no more connected processes - exiting");
		ss_cleanup();
	}
}

