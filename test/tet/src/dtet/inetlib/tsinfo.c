/*
 *      SCCS:  @(#)tsinfo.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tsinfo.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tsinfo.c	1.2 92/09/30
NAME:		tsinfo.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	functions to convert DTET interprocess INET transport-specific
	information messages between internal and machine-independent format

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include "dtmac.h"
#include "ldst.h"
#include "dtmsg.h"
#include "tsinfo_in.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

static struct stdesc st[] = {
	TSINFO_DESC
};

static short nst = -1;

/* static function declarations */
#if __STDC__
static void stinit();
#else /* __STDC__ */
static void stinit();
#endif /* __STDC__ */


static void stinit()
{
	register struct tsinfo *sp = (struct tsinfo *) 0;
	register int n = 0;

	TSINFO_INIT(st, sp, n, nst);
}

int tsinfo2bs(from, to)
register struct tsinfo *from;
register char *to;
{
	if (nst < 0)
		stinit();

	return(st2bs((char *) from, to, st, nst));
}

/*
**	bs2tsinfo() - convert a tsinfo message to internal format
*/

int bs2tsinfo(from, fromlen, to, tolen)
register char *from;
register int fromlen;
register struct tsinfo **to;
register int *tolen;
{
	if (nst < 0)
		stinit();

	if (BUFCHK((char **) to, tolen, sizeof **to) < 0)
		return(-1);

	if (bs2st(from, (char *) *to, st, nst, fromlen) < 0)
		return(-1);

	return(sizeof **to);
}

