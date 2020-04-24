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
	file used when running tccd source code through lint

MODIFICATIONS:

************************************************************************/


#ifdef lint

#include <sys/types.h>
#include "dtmsg.h"
#include "ptab.h"
#include "server.h"
#include "tccd.h"

/*
**	ti_lintstuff() - calls to tccd routines made from the libraries
*/

void ti_lintstuff()
{
	static struct ptab *pp;
	static char *p;
	static int n;
	extern int Myptype;

	n = Myptype;
	if (n) ss_timeout(pp);
	(void) ss_argproc(p, p);
	ss_initdaemon();
	(void) ss_serverloop();
	ss_connect(pp);
	ss_logoff(pp);
	ss_dead(pp);
	ss_process(pp);
	ss_procrun();
	ss_serverproc(pp);
	(void) ss_logon(pp);
	(void) ss_ptalloc(pp);
	ss_ptfree(pp);
	ss_newptab(pp);
	ts_lintstuff();
}

#endif /* lint */
