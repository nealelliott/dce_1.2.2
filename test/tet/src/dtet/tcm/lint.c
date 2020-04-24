/*
 *      SCCS:  @(#)lint.c	1.2 (92/09/30) 
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

/************************************************************************

SCCS:   	@(#)lint.c	1.2 92/09/30
NAME:		lint.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	file used when running tcm source code through lint

MODIFICATIONS:

************************************************************************/


#ifdef lint
#include <sys/types.h>
#include "dtmsg.h"
#include "ptab.h"
#include "synreq.h"
#include "tcmfuncs.h"
#include "server.h"

/*
**	XXX - the real one is somewhere else in the TCM
**
**	remove this when andrew's code is integrated with the rest
*/

int main(argc, argv)
int argc;
char **argv;
{
	tcminit(argc, argv);
	ti_lintstuff();
	return(0);
}

/*
**	ti_lintstuff() - calls to tcc routines made from the libraries
*/

void ti_lintstuff()
{
	static struct ptab *pp;
	static int n;

	(void) doasync(0L, 0, 0, (struct synreq *) 0, (int *) 0);
	(void) ss_serverloop();
	ss_connect(pp);
	ss_dead(pp);
	ss_process(pp);
	(void) ss_ptalloc(pp);
	ss_ptfree(pp);
	(void) ss_tsinfo(pp, n);
	ts_lintstuff();
}

#endif /* lint */

