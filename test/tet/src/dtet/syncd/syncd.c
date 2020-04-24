/*
 *      SCCS:  @(#)syncd.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)syncd.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)syncd.c	1.2 92/09/30
NAME:		syncd.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	server-specific functions for syncd server

MODIFICATIONS:

************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "stab.h"
#include "synreq.h"
#include "ltoa.h"
#include "error.h"
#include "bstring.h"
#include "syncd.h"
#include "server.h"
#include "servlib.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/* these two filled in by si_main() */
char *Tet_root;				/* TET_ROOT from the command line */
int Mypid = -1;				/* my process id */

char *Progname = "tetsyncd";		/* my program name */
int Myptype = PT_SYNCD;			/* my process type */
int Mysysid = 0;			/* my system id (master system) */

/* pointers to error handlers - used in the generic code and libraries */
void (*Error)() = generror;
void (*Fatal)() = genfatal;

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#include <time.h>
#else
extern char *malloc();
extern void exit(), free();
extern time_t time();
#endif


/************************************************************************
**									*
**	MAIN ROUTINES							*
**									*
************************************************************************/

int main(argc, argv)
int argc;
char **argv;
{
	return(si_main(argc, argv, 1));
}

/*
**	ss_argproc() - syncd command-line argument processing
*/

/* ARGSUSED */
int ss_argproc(firstarg, nextarg)
char *firstarg, *nextarg;
{
	switch (*(firstarg + 1)) {
	default:
		fatal(0, "invalid argument:", firstarg);
		/* NOTREACHED */
	}
}

/*
**	ss_initdaemon() - syncd daemon initialisation
*/

void ss_initdaemon()
{
	/* perform syncd transport-specific initialisation */
	ss_tsinitb4fork();

	/* start the daemon */
	si_forkdaemon();

	/* detach from the control terminal */
	tiocnotty();
}

/*
**	ss_serverloop() - syncd main processing loop
*/

int ss_serverloop()
{
	extern struct ptab *Ptab;

	/* perform the generic server loop */
	si_serverloop();

	/* exit if the proc table is empty,
		otherwise arrange to come back */
	return(Ptab ? 1 : 0);
}

/*
**	ss_procrun() - server-specific end-procrun routine
*/

void ss_procrun()
{
	/* perform the sync table service loop */
	stloop();
}

/*
**	ss_timeout() - server-specific timeout processing routine
*/

void ss_timeout(pp)
register struct ptab *pp;
{
	register struct stab *sp;
	register struct ustab *up;

	if (pp->pt_state != PS_WAITSYNC)
		return;

	sp = ((struct sptab *) pp->pt_sdata)->sp_stab;
	ASSERT(sp);

	TRACE4(Tsyncd, 6, "%s: sync timed out, snid = %s, xrid = %s",
		rtoa(&pp->pt_rid), ltoa(sp->st_snid), ltoa(sp->st_xrid));

	/* update the sync state for this process */
	for (up = sp->st_ud; up < sp->st_ud + sp->st_nud; up++)
		if (up->us_ptab == pp) {
			ASSERT(up->us_state != SS_NOTSYNCED);
			switch (up->us_state) {
			case SS_SYNCYES:
			case SS_SYNCNO:
				up->us_state = SS_TIMEDOUT;
				stcheck(sp);
				break;
			}
			break;
		}

	ASSERT(up < sp->st_ud + sp->st_nud);

	pp->pt_flags &= ~PF_TIMEDOUT;
}

/************************************************************************
**									*
**	SERVER-SPECIFIC PARTS OF GENERIC SERVICE ROUTINES		*
**									*
************************************************************************/

/*
**	ss_dead() - server-specific routine to handle a dead process
*/

void ss_dead(pp)
register struct ptab *pp;
{
	/* emit a diagnostic message if this is unexpected */
	if ((pp->pt_flags & PF_LOGGEDOFF) == 0)
		error(0, "client connection closed", rtoa(&pp->pt_rid));

	pp->pt_flags = (pp->pt_flags & ~PF_LOGGEDON) | PF_LOGGEDOFF;

	/* update all sync events referencing this process */
	stdead(pp);

	/* syncd is never a client - call the server-only dead process
		routine to remove the ptab entry */
	so_dead(pp);
}

/************************************************************************
**									*
**	FULL REQUEST PROCESSING ROUTINES				*
**									*
************************************************************************/

/*
**	ss_process() - request processing routine
*/

void ss_process(pp)
struct ptab *pp;
{
	si_serverproc(pp);
}

/*
**	ss_serverproc() - request processing as a server
*/

void ss_serverproc(pp)
register struct ptab *pp;
{
	switch (pp->ptm_req) {
	case OP_SNGET:
		op_snget(pp);
		break;
	case OP_SNSYS:
		op_snsys(pp);
		break;
	case OP_ASYNC:
		op_async(pp);
		break;
	case OP_USYNC:
		op_usync(pp);
		break;
	default:
		pp->ptm_rc = ER_REQ;
		pp->ptm_len = 0;
		break;
	}

	if (pp->pt_state != PS_WAITSYNC) {
		pp->pt_state = PS_SNDMSG;
		pp->pt_flags = (pp->pt_flags & ~PF_IODONE) | PF_ATTENTION;
	}
}

/************************************************************************
**									*
**	SERVER-SPECIFIC PARTS OF GENERIC REQUEST PROCESSING		*
**									*
************************************************************************/

/*
**	ss_logon() - server-specific logon processing routine
**
**	return ER_OK if successful or other ER_* error code on error
*/

int ss_logon(pp)
register struct ptab *pp;
{
	register struct ptab *pp1, *pp2;
	register int count;
	extern struct ptab *Ptab;

	/* make sure that we only have one MTCC and one XRESD logged on */
	switch (pp->ptr_ptype) {
	case PT_MTCC:
	case PT_XRESD:
		pp1 = Ptab;
		count = 0;
		while ((pp2 = getptbyptype(pp->ptr_ptype, pp1)) != (struct ptab *) 0) {
			if (pp2 != pp)
				count++;
			if ((pp1 = pp2->pt_next) == (struct ptab *) 0)
				break;
		}
		if (count > 0) {
			error(0, "client of this type already logged on",
				rtoa(&pp->pt_rid));
			return(ER_LOGON);
		}
		break;
	}

	return(ER_OK);
}

/*
**	ss_logoff() - server-specific logoff processing
*/

void ss_logoff(pp)
struct ptab *pp;
{
	stdead(pp);
}

/*
**	ss_cleanup() - clean up and exit
*/

void ss_cleanup()
{
	exit(0);
}

/************************************************************************
**									*
**	PUBLIC SUBROUTINES						*
**									*
************************************************************************/

/*
**	ss_ptalloc() - allocate server-specific data element in a ptab
**		structure
**
**	return 0 if successful or -1 on error
*/

int ss_ptalloc(pp)
struct ptab *pp;
{
	register struct sptab *sp;

	errno = 0;
	if ((sp = (struct sptab *) malloc(sizeof *sp)) == (struct sptab *) 0) {
		error(errno, "can't get memory for ss data", (char *) 0);
		pp->pt_sdata = (char *) 0;
		return(-1);
	}
	TRACE2(Tbuf, 6, "allocate sptab = %s", itox(sp));
	bzero((char *) sp, sizeof *sp);

	pp->pt_sdata = (char *) sp;
	return(0);
}

/*
**	ss_ptfree() - free server-specific data element in a ptab structure
*/

void ss_ptfree(pp)
struct ptab *pp;
{
	TRACE2(Tbuf, 6, "free sptab = %s", itox(pp->pt_sdata));

	if (pp->pt_sdata) {
		free(pp->pt_sdata);
		pp->pt_sdata = (char *) 0;
	}
}

/*
**	ss_newptab() - server-specific new ptab entry handler
*/

void ss_newptab(pp)
struct ptab *pp;
{
	/* add the entry to the process table */
	ptadd(pp);

#ifdef lint
	/* this bit just to keep lint happy - it has to go somewhere! */
	ti_lintstuff();
#endif
}

