/*
 *      SCCS:  @(#)d_tccfuncs.c	1.3 (92/10/06) 
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
static char sccsid[] = "@(#)d_tccfuncs.c	1.3 (92/10/06) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)d_tccfuncs.c	1.3 92/10/06
NAME:		tccfuncs.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	tcc client-related functions

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <sys/types.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "error.h"
#include "tccfuncs.h"
#include "server.h"
#include "dtetlib.h"
#include "tslib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

int Mypid = -1;				/* my process id */
int Myptype = PT_MTCC;			/* my process type */
int Mysysid = 0;			/* my system id */
char *Progname = "tcc";			/* my program name */
struct ptab *Sd_ptab, *Xd_ptab;		/* ptrs to syncd and xresd ptabs */


/* error handlers */
extern void dtccerror(), dtccfatal();
void (*Error)() = dtccerror;
void (*Fatal)() = dtccfatal;

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#else
extern void free();
#endif

/* static function declarations */
#if __STDC__
static struct ptab *ti_stserver(int, char **);
static int ti_st2(struct ptab *, char **);
#else /* __STDC__ */
static struct ptab *ti_stserver();
static int ti_st2();
#endif /* __STDC__ */


/*
**	tccinit() - tcc initialisation
**
**	return 0 if successful or -1 on error
*/

#ifdef NOTRACE
/* ARGSUSED */
#endif

int tccinit(argc, argv)
int argc;
char **argv;
{
	extern pid_t getpid();

	Mypid = (int) getpid();

#ifndef NOTRACE
	traceinit(argc, argv);
#endif

	return(ts_tccinit());
}

/*
**	sd_start() - start a syncd
**
**	return 0 if successful or -1 on error
*/

int sd_start()
{
	extern char tet_root[];
	static char *argv[] = { "tetsyncd", tet_root, (char *) 0 };

	if (Sd_ptab) {
		error(0, "syncd already started", (char *) 0);
		return(-1);
	}

	if ((Sd_ptab = ti_stserver(PT_SYNCD, argv)) == (struct ptab *) 0)
		return(-1);

	return(0);
}

/*
**	xd_start() - start an xresd
**
**	return 0 if successful or -1 on error
*/

int xd_start(savedir)
char *savedir;
{
	extern char tet_root[];
	static char *argv[] = {
		"tetxresd",
		"-s", (char *) 0,
		tet_root,
		(char *) 0
	};

	if (Xd_ptab) {
		error(0, "xresd already started", (char *) 0);
		return(-1);
	}

	argv[2] = savedir;
	if ((Xd_ptab = ti_stserver(PT_XRESD, argv)) == (struct ptab *) 0)
		return(-1);

	return(0);
}

/*
**	ti_stserver() - start a server
**
**	return ptr to allocated ptab entry if successful,
**	(struct ptab *) 0 otherwise
*/

static struct ptab *ti_stserver(ptype, argv)
int ptype;
char **argv;
{
	register struct ptab *pp;

	/* allocate a new ptab for the server and initialise it */
	if ((pp = ptalloc()) == (struct ptab *) 0)
		return(pp);
	pp->ptr_sysid = 0;
	pp->ptr_ptype = ptype;
	pp->pt_flags = PF_SERVER;

	if (ti_st2(pp, argv) < 0) {
		ptfree(pp);
		return((struct ptab *) 0);
	}

	return(pp);
}

/*
**	ti_st2() - extend ti_stserver processing
**
**	return 0 if successful, -1 otherwise
*/

static int ti_st2(pp, argv)
struct ptab *pp;
char **argv;
{
	register int rc;
	register char **avp;
#ifndef NOTRACE

	/* add trace flags to the argv list */
	if ((avp = traceargs(pp->ptr_ptype, argv)) == (char **) 0)
		return(-1);
#else
	avp = argv;
#endif

	/* start the server */
	rc = ts_stserver(pp, avp);

	return(rc);
}

/*
**	ss_dead() - server-specific dead process handler
**
**	should only be called from si_service() when a server dies
**	unexpectedly
**
**	server logoff routines do not come here
*/

void ss_dead(pp)
struct ptab *pp;
{
	extern int server_ok;

	/* emit a diagnostic if this is unexpected */
	if ((pp->pt_flags & PF_LOGGEDOFF) == 0)
		error(0, "server connection closed", rtoa(&pp->pt_rid));

	pp->pt_flags = (pp->pt_flags & ~PF_LOGGEDON) | PF_LOGGEDOFF;

	/* server dead - no more remote calls will be made by this process */
	server_ok = 0;
}

/*
**	ss_connect() - connect to remote process
*/

void ss_connect(pp)
struct ptab *pp;
{
	ts_connect(pp);
}

/*
**	ss_ptalloc(), ss_ptfree()  - allocate and free server-specific
**		ptab data area
**
**	tcc does not make use of server-specific data
*/

int ss_ptalloc(pp)
struct ptab *pp;
{
	pp->pt_sdata = (char *) 0;
	return(0);
}

/* ARGSUSED */
void ss_ptfree(pp)
struct ptab *pp;
{
	/* nothing */
}

