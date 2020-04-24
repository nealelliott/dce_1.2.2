/*
 *      SCCS:  @(#)xresd.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)xresd.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)xresd.c	1.2 92/09/30
NAME:		xresd.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	server-specific routines for xresd server

MODIFICATIONS:

************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "xtab.h"
#include "error.h"
#include "ltoa.h"
#include "xresd.h"
#include "server.h"
#include "servlib.h"
#include "dtetlib.h"

/* these two filled in by si_main() */
char *Tet_root;				/* TET_ROOT from the command line */
int Mypid = -1;				/* my process id */

char *Progname = "tetxresd";		/* my program name */
int Myptype = PT_XRESD;			/* my process type */
int Mysysid = 0;			/* my system id (master system) */
char *Tet_savedir;			/* saved files directory */

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/* pointers to error handlers - used in the generic code and libraries */
void (*Error)() = generror;
void (*Fatal)() = genfatal;

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#else
extern char *malloc();
extern void exit(), free();
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
**	ss_argproc() - xresd command-line argument processing
**
**	return 0 if only first arg was used or 1 if both args were used
*/

int ss_argproc(firstarg, nextarg)
char *firstarg, *nextarg;
{
	register int rc;

	switch (*(firstarg + 1)) {
	case 's':
		if (*(firstarg + 2)) {
			Tet_savedir = firstarg + 2;
			rc = 0;
		}
		else {
			Tet_savedir = nextarg;
			rc = 1;
		}
		break;
	default:
		fatal(0, "unknown option:", firstarg);
		/* NOTREACHED */
	}

	return(rc);
}

/*
**	ss_initdaemon() - xresd daemon initialisation
*/

void ss_initdaemon()
{
	/* make sure that we have a Tet_savedir from the command line */
	if (!Tet_savedir)
		fatal(0, "must specify a savefiles directory", (char *) 0);

	/* perform xresd transport-specific initialisation */
	ss_tsinitb4fork();

	/* start the daemon */
	si_forkdaemon();

	/* detach from the control terminal */
	tiocnotty();
}

/*
**	ss_serverloop() - xresd main processing loop
**
**	return 1 to be recalled, 0 otherwise
*/

int ss_serverloop()
{
	extern struct ptab *Ptab;

	/* perform the generic server loop */
	si_serverloop();

	/* exit if there are no more connected processes,
		otherwise arrange to come back */
	return(Ptab ? 1 : 0);
}

/************************************************************************
**									*
**	SERVER-SPECIFIC PARTS OF GENERIC SERVICE ROUTINES		*
**									*
************************************************************************/

/*
**	ss_dead() - server-specific routine to handle a dead process
**
**	server logoff routines should not come here
*/

void ss_dead(pp)
register struct ptab *pp;
{
	static char fmt[] = "%s connection closed";
	static char cl[] = "client";
	static char se[] = "server";
	char msg[sizeof fmt + sizeof cl];

	/* emit a diagnostic message if this is unexpected */
	if ((pp->pt_flags & PF_LOGGEDOFF) == 0) {
		(void) sprintf(msg, fmt, (pp->pt_flags & PF_SERVER) ? se : cl);
		error(0, msg, rtoa(&pp->pt_rid));
	}

	pp->pt_flags = (pp->pt_flags & ~PF_LOGGEDON) | PF_LOGGEDOFF;

	/* for clients, remove the related table entries
		and exit if the process table is empty */
	if ((pp->pt_flags & PF_SERVER) == 0) {
		xtdead(pp);
		tfdead(pp);
		so_dead(pp);
	}
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
	/* a server ptab might get here via si_servwait() */
	if ((pp->pt_flags & PF_SERVER) == 0)
		si_serverproc(pp);
}

/*
**	ss_serverproc() - request processing as a server
*/

void ss_serverproc(pp)
register struct ptab *pp;
{
	switch (pp->ptm_req) {
	case OP_XROPEN:
		op_xropen(pp);
		break;
	case OP_XRSYS:
		op_xrsys(pp);
		break;
	case OP_ICSTART:
		op_icstart(pp);
		break;
	case OP_TPSTART:
		op_tpstart(pp);
		break;
	case OP_ICEND:
		op_icend(pp);
		break;
	case OP_TPEND:
		op_tpend(pp);
		break;
	case OP_XRES:
		op_xres(pp);
		break;
	case OP_RESULT:
		op_result(pp);
		break;
	case OP_TFOPEN:
		op_tfopen(pp);
		break;
	case OP_TFCLOSE:
		op_tfclose(pp);
		break;
	case OP_TFWRITE:
		op_tfwrite(pp);
		break;
	case OP_FOPEN:
		op_fopen(pp);
		break;
	case OP_FCLOSE:
		op_fclose(pp);
		break;
	case OP_GETS:
		op_gets(pp);
		break;
	case OP_CFNAME:
		op_cfname(pp);
		break;
	case OP_CODESF:
		op_codesf(pp);
		break;
	case OP_RCFNAME:
		op_rcfname(pp);
		break;
	default:
		pp->ptm_rc = ER_REQ;
		pp->ptm_len = 0;
		break;
	}

	/* here to send a reply message */
	pp->pt_state = PS_SNDMSG;
	pp->pt_flags = (pp->pt_flags & ~PF_IODONE) | PF_ATTENTION;
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

	/* make sure that we only have one MTCC logged on */
	switch (pp->ptr_ptype) {
	case PT_MTCC:
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
	xtdead(pp);
	tfdead(pp);
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

/* ARGSUSED */
int ss_ptalloc(pp)
struct ptab *pp;
{
	return(0);
}

/*
**	ss_ptfree() - free server-specific data element in a ptab structure
*/

/* ARGSUSED */
void ss_ptfree(pp)
struct ptab *pp;
{
	/* nothing */
}

/*
**	ss_newptab() - server-specific new ptab entry handler
*/

void ss_newptab(pp)
struct ptab *pp;
{
	/* add the entry to the process table */
	ptadd(pp);
}

