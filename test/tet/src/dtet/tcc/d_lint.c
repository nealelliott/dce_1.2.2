/*
 *      SCCS:  @(#)d_lint.c	1.2 (92/09/30) 
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

SCCS:   	@(#)d_lint.c	1.2 92/09/30
NAME:		lint.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	file used when running tcc source code through lint

MODIFICATIONS:

************************************************************************/


#ifdef lint
#include <sys/types.h>
#include "dtmsg.h"
#include "ptab.h"
#include "tccfuncs.h"
#include "server.h"

/*
**	ti_lintstuff() - calls to tcc routines made from the libraries
*/

void ti_lintstuff()
{
	static struct ptab *pp;
	static char *p;
	static int n;
	extern int Myptype, Mysysid;
	extern char *Progname;

	n = Myptype;
	if (n) n = Mysysid;
	p = Progname;
	if (p) (void) ss_serverloop();
	ss_connect(pp);
	ss_dead(pp);
	ss_process(pp);
	(void) ss_ptalloc(pp);
	ss_ptfree(pp);
	(void) ss_tsinfo(pp, n);
	ts_lintstuff();
}

#endif /* lint */

