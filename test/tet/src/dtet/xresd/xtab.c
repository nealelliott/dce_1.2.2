/*
 *      SCCS:  @(#)xtab.c	1.3 (92/12/01) 
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
static char sccsid[] = "@(#)xtab.c	1.3 (92/12/01) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)xtab.c	1.3 92/12/01
NAME:		xtab.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	execution results table administration functions

MODIFICATIONS:

************************************************************************/


#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "xtab.h"
#include "valmsg.h"
#include "error.h"
#include "ltoa.h"
#include "llist.h"
#include "bstring.h"
#include "dtetlib.h"
#include "xresd.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

static struct xtab *xtab;		/* ptr to head of xres table */

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#include <time.h>
#else
extern char *malloc();
extern void free();
extern time_t time();
#endif

/* static function declarations */
#if __STDC__
static void xtprocess(struct xtab *);
#else /* __STDC__ */
static void xtprocess();
#endif /* __STDC__ */


/*
**	xtalloc() - allocate an xres table element and return a pointer thereto
**
**	return (struct xtab *) 0 on error
*/

struct xtab *xtalloc()
{
	register struct xtab *xp;
	static long xrid;

	errno = 0;

	if ((xp = (struct xtab *) malloc(sizeof *xp)) == (struct xtab *) 0) {
		error(errno, "can't allocate xtab element", (char *) 0);
		return((struct xtab *) 0);
	}
	TRACE2(Tbuf, 6, "allocate xtab = %s", itox(xp));
	bzero((char *) xp, sizeof *xp);

	xp->xt_xrid = ++xrid;

	return(xp);
}

/*
**	xtfree() - close files and free an xres table element
*/

void xtfree(xp)
struct xtab *xp;
{
	TRACE2(Tbuf, 6, "free xtab = %s", itox(xp));

	if (xp) {
		if (xp->xt_xfp)
			(void) fclose(xp->xt_xfp);
		if (xp->xt_xfname) {
			TRACE2(Tbuf, 6, "free xtab xfname = %s",
				itox(xp->xt_xfname));
			free(xp->xt_xfname);
		}
		if (xp->xt_ud) {
			TRACE2(Tbuf, 6, "free uxtab = %s", itox(xp->xt_ud));
			free((char *) xp->xt_ud);
		}
		free((char *) xp);
	}
}

/*
**	uxtalloc() - allocate an xtab per-user details element or grow an
**		existing one
**
**	return 0 if successful or -1 on error
*/

int uxtalloc(xp, nud)
struct xtab *xp;
int nud;
{
	register int needlen;

	ASSERT(xp);
	needlen = nud * sizeof *xp->xt_ud;

	if (BUFCHK((char **) &xp->xt_ud, &xp->xt_udlen, needlen) < 0) {
		if (xp->xt_udlen == 0)
			xp->xt_nud = 0;
		return(-1);
	}
	bzero((char *) xp->xt_ud, needlen);

	xp->xt_nud = nud;
	return(0);
}

/*
**	xtadd() - insert an element in the xtab list
*/

void xtadd(xp)
struct xtab *xp;
{
	listinsert((struct llist **) &xtab, (struct llist *) xp);
}

/*
**	xtrm() - remove an element from the xtab list
*/

void xtrm(xp)
struct xtab *xp;
{
	if (xp->xt_flags & XF_TPINPROGRESS)
		(void) tpend(xp);
	if (xp->xt_flags & XF_ICINPROGRESS)
		(void) icend(xp);

	listremove((struct llist **) &xtab, (struct llist *) xp);
}

/*
**	xtfind() - find an xtab element matching xrid and return a pointer
**		thereto
**
**	return (struct xtab *) 0 if none can be found
*/

struct xtab *xtfind(xrid)
register long xrid;
{
	register struct xtab *xp;

	for (xp = xtab; xp; xp = xp->xt_next)
		if (xp->xt_xrid == xrid)
			break;

	return(xp);
}

/*
**	xtdead() - xtab processing when a connection closes
*/

void xtdead(pp)
register struct ptab *pp;
{
	register struct xtab *xp;
	register struct uxtab *up;
	register int count, done;

	/* find related xres table entries and update them
		remove an entry that is no longer required */
	do {
		done = 1;
		for (xp = xtab; xp; xp = xp->xt_next) {
			count = 0;
			if (xp->xt_ptab == pp)
				xp->xt_ptab = (struct ptab *) 0;
			for (up = xp->xt_ud; up < xp->xt_ud + xp->xt_nud; up++)
				if (up->ux_ptab == pp) {
					up->ux_state = XS_DEAD;
					up->ux_ptab = (struct ptab *) 0;
					count++;
				}
				else if (up->ux_state == XS_DEAD)
					count++;
			if (!xp->xt_ptab && count == xp->xt_nud) {
				xtrm(xp);
				xtfree(xp);
				done = 0;
				break;
			}
		}
	} while (!done);
}

