/*
 *      SCCS:  @(#)ptab.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)ptab.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)ptab.c	1.2 92/09/30
NAME:		ptab.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	process table management functions

MODIFICATIONS:

************************************************************************/

#include <sys/types.h>
#include <errno.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "error.h"
#include "bstring.h"
#include "llist.h"
#include "server.h"
#include "tslib.h"

#ifndef NOTRACE
#include "ltoa.h"
#endif

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

struct ptab *Ptab;			/* ptr to start of process table */

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#else
extern char *malloc();
extern void free();
#endif

/*
**	ptalloc() - allocate a process table entry and return a pointer
**		thereto, or (struct ptab *) 0 on error
**
**
*/

struct ptab *ptalloc()
{
	register struct ptab *pp;

	/* allocate transport-independent data area */
	errno = 0;
	if ((pp = (struct ptab *) malloc(sizeof *pp)) == (struct ptab *) 0) {
		error(errno, "can't malloc ptab entry", (char *) 0);
		return((struct ptab *) 0);
	}
	TRACE2(Tbuf, 6, "allocate ptab = %s", itox(pp));
	bzero((char *) pp, sizeof *pp);

	/* call the routine to allocate transport-specific data */
	if (ts_ptalloc(pp) < 0) {
		ptfree(pp);
		return((struct ptab *) 0);
	}

	/* call the routine to allocate server-specific data space */
	if (ss_ptalloc(pp) < 0) {
		ptfree(pp);
		return((struct ptab *) 0);
	}

	/* initialise variables */
	pp->pt_next = pp->pt_last = (struct ptab *) 0;
	pp->pt_magic = PT_MAGIC;
	pp->ptr_sysid = -1;
	pp->ptr_pid = -1L;
	pp->ptr_ptype = PT_NOPROC;
	pp->pt_state = PS_IDLE;

	return(pp);
}

/*
**	ptfree() - free ptab element memory
*/

void ptfree(pp)
register struct ptab *pp;
{
	TRACE2(Tbuf, 6, "free ptab = %s", itox(pp));

	if (pp) {
		ts_ptfree(pp);
		ss_ptfree(pp);
		if (pp->ptm_data) {
			TRACE2(Tbuf, 6, "free ptmdata = %s",
				itox(pp->ptm_data));
			free(pp->ptm_data);
		}
		free((char *) pp);
	}
}

/*
**	ptadd() - insert an element in the ptab list
*/

void ptadd(pp)
struct ptab *pp;
{
	listinsert((struct llist **) &Ptab, (struct llist *) pp);
}

/*
**	ptrm() - remove an element from the ptab list
*/

void ptrm(pp)
struct ptab *pp;
{
	listremove((struct llist **) &Ptab, (struct llist *) pp);
}

