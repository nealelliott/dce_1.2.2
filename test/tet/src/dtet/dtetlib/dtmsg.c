/*
 *      SCCS:  @(#)dtmsg.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)dtmsg.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)dtmsg.c	1.2 92/09/30
NAME:		dtmsg.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	functions to convert a DTET interprocess message header between
	internal and machine-independent format

MODIFICATIONS:

************************************************************************/


#include "ldst.h"
#include "dtmsg.h"

static struct stdesc st[] = {
	DTMHDR_DESC
};

static short nst = -1;
static short stlen = -1;


/* static function declarations */
#if __STDC__
static void stinit();
#else /* __STDC__ */
static void stinit();
#endif /* __STDC__ */


static void stinit()
{
	register struct dtmhdr *hp = (struct dtmhdr *) 0;
	register int n = 0;

	DTMHDR_INIT(st, hp, n, nst, stlen);
}

int dtmhdr2bs(from, to)
struct dtmhdr *from;
char *to;
{
	if (nst < 0)
		stinit();

	return(st2bs((char *) from, to, st, nst));
}

int dmlen2bs(from, to)
register short from;
register char *to;
{
	struct dtmsg dummy;

	if (nst < 0)
		stinit();

	dummy.dm_len = from;
	return(st2bs((char *) &dummy, to, &st[stlen], 1));
}

int bs2dtmhdr(from, to, len)
char *from;
struct dtmhdr *to;
int len;
{
	if (nst < 0)
		stinit();

	return(bs2st(from, (char *) to, st, nst, len));
}

