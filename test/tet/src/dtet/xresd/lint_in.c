/*
 *      SCCS:  @(#)lint_in.c	1.2 (92/09/30) 
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

SCCS:   	@(#)lint_in.c	1.2 92/09/30
NAME:		lint_in.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	file used when running xresd source code through lint

MODIFICATIONS:

************************************************************************/


#ifdef lint

#include <sys/types.h>
#include "dtmsg.h"
#include "ptab.h"
#include "server_in.h"
#include "server_bs.h"

/*
**	ts_lintstuff() - calls to xresd routines made from the libraries
*/

void ts_lintstuff()
{
	static struct ptab *pp;
	static char *p;
	static int n;

	(void) ss_bs2md(p, pp);
	(void) ss_md2bs(pp, &p, &n, n);
	ss_tsaccept();
	(void) ss_tsafteraccept(pp);
}

#endif /* lint */

