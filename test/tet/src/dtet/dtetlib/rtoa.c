/*
 *      SCCS:  @(#)rtoa.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)rtoa.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)rtoa.c	1.2 92/09/30
NAME:		rtoa.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to return printable representation of a remote client/server
	identifier
	(not a DTET API remoteid)

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <sys/types.h>
#include "dtmsg.h"
#include "ptab.h"
#include "ltoa.h"
#include "dtetlib.h"

#define RBUFSZ		(sizeof fmt + ((LNUMSZ - 2) * 2) + (20 - 5))

/*
**	rtoa() - return printable representation of remote process id
*/

char *rtoa(rp)
struct remid *rp;
{
	static char fmt[] = "(sysid = %d, pid = %ld: %.20s)";
	static char buf[NLBUF][RBUFSZ];
	static int count;
	register char *p;

	if (++count >= NLBUF)
		count = 0;
	p = buf[count];

	(void) sprintf(p, fmt, rp->re_sysid, rp->re_pid,
		ptptype(rp->re_ptype));
	return(p);
}

