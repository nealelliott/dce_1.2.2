/*
 *      SCCS:  @(#)avmsg.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)avmsg.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)avmsg.c	1.2 92/09/30
NAME:		avmsg.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	functions to convert a DTET interprocess character string message
	between internal and machine-independent format

MODIFICATIONS:

************************************************************************/


#if __STDC__
#include <stdio.h>
#endif

#include "ldst.h"
#include "dtmac.h"
#include "dtmsg.h"
#include "avmsg.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

static struct stdesc st[] = {
	AVMSG_DESC
};

static struct stdesc offst[] = {
	OFFSET_DESC
};

static short fixed = -1;


/* static function declarations */
#if __STDC__
static void stinit();
static int bs2avmargv(char *, char **, int, int);
#else
static void stinit();
static int bs2avmargv();
#endif


static void stinit()
{
	register struct avmsg *ap = (struct avmsg *) 0;
	register int n = 0;

	AVMSG_INIT(st, ap, n, fixed);
	OFFSET_INIT(offst);
}

/*
**	avmsg2bs() - put an avmsg and associated strings on a byte stream
*/

int avmsg2bs(from, to)
register struct avmsg *from;
register char *to;
{
	register int n;
	register char *p, *sp;
	unsigned short offset;

	if (fixed < 0)
		stinit();

	/* convert the fixed part */
	sp = to + st2bs((char *) from, to, st, fixed);

	/* allocate space on the byte stream for the argv list */
	offset = 0;
	for (n = 0; n < (int) from->av_argc; n++) {
		sp += st2bs((char *) &offset,
			to + AV_AVMSGSZ(n), &offst[0], 1);
	}

	/* sp points to the place to store the strings themselves -
		copy the strings and fill in the offsets */
	for (n = 0; n < (int) from->av_argc; n++)
		if (from->av_argv[n]) {
			offset = sp - to;
			(void) st2bs((char *) &offset,
				to + AV_AVMSGSZ(n), &offst[0], 1);
			for (p = from->av_argv[n]; *p; p++)
				*sp++ = *p;
			*sp++ = '\0';
		}

	return(sp - to);
}

/*
**	bs2avmsg() - convert an avmsg to internal format
**
**	return number of bytes in the avmsg result
**
**	on return, av_argv contains the string pointers
**	the strings themselves are not copied from the machine-independent
**	area
*/

int bs2avmsg(from, fromlen, to, tolen)
register char *from;
register int fromlen;
register struct avmsg **to;
register int *tolen;
{
	if (fixed < 0)
		stinit();

	/* make sure the buffer is big enough for a minimal message */
	if (BUFCHK((char **) to, tolen, avmsgsz(1)) < 0)
		return(-1);

	/* convert the fixed part */
	if (bs2st(from, (char *) *to, st, fixed,
		MIN(fromlen, AV_ARGVSTART)) < 0)
			return(-1);

	/* make sure the buffer is big enough for the actual message */
	if ((int) (*to)->av_argc > 1 &&
		BUFCHK((char **) to, tolen, avmsgsz((*to)->av_argc)) < 0)
			return(-1);

	/* then convert the variable part */
	fromlen -= AV_ARGVSTART;
	if (bs2avmargv(from, (*to)->av_argv, (int) (*to)->av_argc,
		MIN((int) (*to)->av_argc * AV_ARGVSZ, fromlen)) < 0)
			return(-1);

	/* return the number of bytes in the result */
	return(avmsgsz((*to)->av_argc));
}

/*
**	bs2avmargv() - convert argv byte-stream offsets to internal
**		addresses
**
**	note it is only the offsets that are copied to the internal format
**	area, not the strings themselves
*/

static int bs2avmargv(from, to, nargv, len)
register char *from;
register char **to;
register int nargv, len;
{
	register int n;
	unsigned short offset;

	if (fixed < 0)
		stinit();

	for (n = 0; n < nargv && len > 0; n++) {
		if (bs2st(from + AV_AVMSGSZ(n), (char *) &offset,
			&offst[0], 1, MIN(AV_ARGVSZ, len)) < 0)
				break;
		*(to + n) = offset ? from + offset : (char *) 0;
		len -= AV_ARGVSZ;
	}

	return((len == 0 && n == nargv) ? 0 : -1);
}

/*
**	avmsgbslen() - return length of an avmsg on a byte stream
*/

int avmsgbslen(mp)
register struct avmsg *mp;
{
	register int len, n;

	len = AV_AVMSGSZ(mp->av_argc);
	for (n = 0; n < (int) mp->av_argc; n++)
		if (mp->av_argv[n])
			len += strlen(mp->av_argv[n]) + 1;

	return(len);
}

