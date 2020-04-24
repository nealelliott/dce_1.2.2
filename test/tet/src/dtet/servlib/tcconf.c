/*
 *      SCCS:  @(#)tcconf.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tcconf.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tcconf.c	1.2 92/09/30
NAME:		tcconf.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	May 1992

DESCRIPTION:
	functions to send config variables to/from STCC

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include "dtmac.h"
#include "dtmsg.h"
#include "avmsg.h"
#include "error.h"
#include "servlib.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/* static function declarations */
#if __STDC__
static int tc_cs(int, int, char **, int);
static int tc_cs2(int, int, char **, int, int);
static char *tc_csr(int, int);
#else /* __STDC__ */
static int tc_cs();
static int tc_cs2();
static char *tc_csr();
#endif /* __STDC__ */


/*
**	tc_configv() - send OP_CONFIG messages to STCC and receive replies
**
**	return 0 if successful or -1 on error
*/

int tc_configv(sysid, lines, nline)
int sysid, nline;
char **lines;
{
	return(tc_cs(sysid, OP_CONFIG, lines, nline));
}

/*
**	tc_sndconfv() - send OP_SNDCONF messages to STCC and receive replies
**
**	return 0 if successful or -1 on error
*/

int tc_sndconfv(sysid, lines, nline)
int sysid, nline;
char **lines;
{
	return(tc_cs(sysid, OP_SNDCONF, lines, nline));
}

/*
**	tc_cs() - common function for OP_SNDCONF and OP_CONFIG
*/

static int tc_cs(sysid, request, lines, nline)
int sysid, request;
register char **lines;
register int nline;
{
	/* make sure that lines is non-zero and that nline is +ve */
	if (!lines || nline <= 0) {
		tc_errno = ER_INVAL;
		return(-1);
	}

	/* send as many messages as necessary */
	while (nline > 0) {
		if (tc_cs2(sysid, request, lines, MIN(nline, AV_NLINE),
			nline > AV_NLINE ? 0 : 1) < 0)
				return(-1);
		nline -= AV_NLINE;
		lines += AV_NLINE;
	}

	return(0);
}

/*
**	tc_cs2() - send a single OP_CONFIG or OP_SNDCONF message
**		and receive a reply
**
**	return 0 if successful or -1 on error
*/

static int tc_cs2(sysid, request, lines, nline, done)
int sysid, request, done;
register int nline;
register char **lines;
{
	register struct avmsg *mp;
	register int n;

	/* get the STCC message buffer */
	if ((mp = (struct avmsg *) tc_msgbuf(sysid, avmsgsz(OP_CONF_ARGC(nline)))) == (struct avmsg *) 0) {
		tc_errno = ER_ERR;
		return(-1);
	}

	/* set up the request message */
	mp->av_argc = OP_CONF_ARGC(nline);
	AV_FLAG(mp) = done ? AV_DONE : AV_MORE;
	for (n = 0; n < nline; n++)
		AV_CLINE(mp, n) = *lines++;

	return(tc_csr(sysid, request) == (char *) 0 ? -1 : 0);
}

/*
**	tc_rcvconfv() - send an OP_RCVCONF message to STCC and receive a
**		reply
**
**	return a pointer to the first in a list of received config lines if
**	successful, or (char **) 0 on error
**
**	if successful, the number of lines in the list is returned indirectly
**	through *nlines and *done is set to 0 or 1 depending on whether or not
**	there are any more lines to come
**
**	the lines and their pointers are held in memory owned by the tc_talk()
**	subsystem, so they must be copied if required before another STCC
**	request to the same sysid is issued
*/

char **tc_rcvconfv(sysid, nlines, done)
int sysid, *nlines, *done;
{
	register struct avmsg *rp;

	/* make sure that nlines and done are non-zero */
	if (!nlines || !done) {
		tc_errno = ER_INVAL;
		return((char **) 0);
	}

	if ((rp = (struct avmsg *) tc_csr(sysid, OP_RCVCONF)) == (struct avmsg
*) 0)
		return((char **) 0);

	/* all ok so return all the return values */
	*nlines = (int) OP_CONF_NLINE(rp);
	*done = AV_FLAG(rp) == AV_DONE ? 1 : 0;
	return(&AV_CLINE(rp, 0));
}

/*
**	tc_csr() - common tc_talk() interface used by several functions
**
**	return pointer to STCC reply buffer if successful
**		or (char *) 0 on error
*/

static char *tc_csr(sysid, request)
int sysid, request;
{
	register char *dp;
	extern char Tcerrmsg[];

	/* send the request and receive the reply */
	dp = tc_talk(sysid, request, TALK_DELAY);

	/* handle the return codes */
	switch (tc_errno) {
	case ER_OK:
		return(dp);
	case ER_INVAL:
	case ER_CONTEXT:
	case ER_INPROGRESS:
	case ER_DONE:
		break;
	case ER_ERR:
		if (!dp)
			break;
		/* else fall through */
	default:
		error(0, Tcerrmsg, ptrepcode(tc_errno));
		break;
	}

	/* here for server error return */
	return((char *) 0);
}

