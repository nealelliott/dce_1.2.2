/*
 *      SCCS:  @(#)rtab.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)rtab.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)rtab.c	1.2 92/09/30
NAME:		rtab.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	DTET API remote execution support routines

MODIFICATIONS:

************************************************************************/


#include <errno.h>
#include "dtmac.h"
#include "error.h"
#include "bstring.h"
#include "llist.h"
#include "rtab.h"

#ifndef NOTRACE
#include "ltoa.h"
#endif

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

static struct rtab *rtab;	/* ptr to head of remote execution table */

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#else
extern char *malloc();
extern void free();
#endif

/*
**	rtalloc() - allocate a remote execution table element and return
**		a pointer thereto
**
**	return (struct rtab *) 0 on error
*/

struct rtab *rtalloc()
{
	register struct rtab *rp;
	static int remoteid;

	errno = 0;
	if ((rp = (struct rtab *) malloc(sizeof *rp)) == (struct rtab *) 0) {
		error(errno, "can't allocate rtab element", (char *) 0);
		return((struct rtab *) 0);
	}
	TRACE2(Tbuf, 6, "allocate rtab element = %s", itox(rp));
	bzero((char *) rp, sizeof *rp);

	rp->rt_sysid = -1;
	rp->rt_pid = -1L;
	rp->rt_magic = RT_MAGIC;
	rp->rt_remoteid = ++remoteid;

	return(rp);
}

/*
**	rtfree() - free remote execution table element
*/

void rtfree(rp)
struct rtab *rp;
{
	TRACE2(Tbuf, 6, "free rtab = %s", itox(rp));

	if (rp)
		free((char *) rp);
}

/*
**	rtadd() - add an element to the remote execution table
*/

void rtadd(rp)
struct rtab *rp;
{
	listinsert((struct llist **) &rtab, (struct llist *) rp);
}

/*
**	rtrm() - remove an element from the remote execution list
*/

void rtrm(rp)
struct rtab *rp;
{
	listremove((struct llist **) &rtab, (struct llist *) rp);
}

/*
**	rtfind() - find remote execution table element matching remoteid
**		and return a pointer thereto
**
**	return (struct rtab *) 0 if not found
*/

struct rtab *rtfind(remoteid)
register int remoteid;
{
	register struct rtab *rp;

	TRACE3(Ttcm, 6, "rtfind(%s): rtab = %s", itoa(remoteid), itox(rtab));

	for (rp = rtab; rp; rp = rp->rt_next) {
		ASSERT(rp->rt_magic == RT_MAGIC);
		TRACE5(Ttcm, 8,
			"rtab: addr = %s, remoteid = %s, sysid = %s, pid = %s",
			itox(rp), itoa(rp->rt_remoteid), itoa(rp->rt_sysid),
			ltoa(rp->rt_pid));
		if (rp->rt_remoteid == remoteid)
			break;
	}

	TRACE2(Ttcm, 6, "rtfind() returns %s", itox(rp));

	return(rp);
}

