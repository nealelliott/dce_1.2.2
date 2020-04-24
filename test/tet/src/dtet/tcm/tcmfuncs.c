/*
 *      SCCS:  @(#)tcmfuncs.c	1.5 (92/11/29) 
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
static char sccsid[] = "@(#)tcmfuncs.c	1.5 (92/11/29) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tcmfuncs.c	1.5 92/11/29
NAME:		tcmfuncs.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	generic tcm client-related functions

MODIFICATIONS:
	Andrew Dingwall, UniSoft Ltd., October 1992
	Set Progname to basename(argv[0]) so as to avoid long path names
	in error messages.
	Moved tcmptype() call to after TET_TIARGS parsing so that tcmchild
	processes can determine which system they are on.
	Moved tet_root[] from dtcm.c to here so that it is available
	to tcmchild processes as well.

************************************************************************/


#if __STDC__
#include <stdio.h>
#endif

#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "error.h"
#include "synreq.h"
#include "server.h"
#include "servlib.h"
#include "dtetlib.h"
#include "tslib.h"
#include "dapi.h"
#include "tcmfuncs.h"

#ifndef NOTRACE
#include "ltoa.h"
#endif

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

int Mypid = -1;				/* my process id */
int Myptype = PT_NOPROC;		/* my process type */
int Mysysid = -1;			/* my system id */
int Psysid = -1;			/* parent's system id */
char *Progname = "???";			/* my program name */
long Snid = -1L;			/* sync id */
long Xrid = -1L;			/* xres id */
int *Snames;				/* system name list */
int Nsname;				/* number of system names */
struct ptab *Sd_ptab, *Xd_ptab;		/* ptab elements for syncd and xresd */
char tet_root[MAXPATH];			/* TET_ROOT from the environment */

/* error handlers in dtetlib */
void (*Error)() = generror;
void (*Fatal)() = genfatal;

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#else
extern char *getenv(), *malloc();
extern long atol();
extern void exit(), free();
#endif

/*
**	tcminit() - initialisation for both master and slave TCMs
*/

/* ARGSUSED */
void tcminit(argc, argv)
int argc;
char **argv;
{
	register char **ap;
	register char *envstring, *p;
	register int *ip;
	char **args;
	register int count, nargs;
	struct synreq *synreq, *sp;
	int nsys;
	char errmsg[128];
	static char tetroot_name[] = "TET_ROOT";
	static char tiargs_name[] = "TET_TIARGS";
	static char envmsg[] = "null or not set";
#ifdef NOTRACE
	int twarn = 0;
#endif

	/* set up Progname and Mypid - we set Progname to basename(*argv)
		because testsets always seem to have such long path names */
	Progname = *argv;
	for (p = *argv + strlen(*argv) - 1; p > *argv; p--)
		if (*(p - 1) == '/') {
			Progname = p;
			break;
		}
	Mypid = getpid();

	/* get TET_ROOT out of the environment */
	if ((envstring = getenv(tetroot_name)) == (char *) 0 || !*envstring)
		fatal(0, tiargs_name, envmsg);
	(void) sprintf(tet_root, "%.*s", sizeof tet_root - 1, envstring);

	/* get the dtet ti args out of the environment and count them */
	if ((envstring = getenv(tiargs_name)) == (char *) 0 || !*envstring)
		fatal(0, tiargs_name, envmsg);
	nargs = 1;
	for (p = envstring; *p; p++)
		if (isspace(*p))
			nargs++;

	/* allocate some space for argument pointers */
	errno = 0;
	if ((args = (char **) malloc(nargs * sizeof *args)) == (char **) 0)
		fatal(errno, "can't get memory for arg list", (char *) 0);
	TRACE2(Tbuf, 6, "allocate ti env args = %s", itox(args));

	/* split the arg string into fields */
	nargs = getargs(envstring, args, nargs);

	/* process each argument in turn */
#ifndef NOTRACE
	traceinit(nargs + 1, args - 1);
#endif
	for (ap = args; nargs > 0; ap++, nargs--) {
		if (*(p = *ap) != '-')
			continue;
		TRACE2(Ttcm, 6, "TI arg = \"%s\"", p);
		switch (*++p) {
		case 'T':
#ifdef NOTRACE
			if (!twarn) {
				error(0, "tracing not configured", (char *) 0);
				twarn = 1;
			}
#endif
			break;
		case 'l':
			count = isdigit(*++p) ? 1 : 0;
			do {
				if (*p == ',')
					count++;
				else if (!isdigit(*p))
					fatal(0, "bad sysname string", *ap);
			} while (*++p);
			errno = 0;
			if ((Snames = (int *) malloc(count * sizeof *Snames)) == (int *) 0)
				fatal(errno, "can't get memory for sname list",
					(char *) 0);
			TRACE2(Tbuf, 6, "allocate Snames = %s", itox(Snames));
			p = *ap + 2;
			ip = Snames;
			Nsname = count;
			while (--count >= 0) {
				*ip = atoi(p);
				ip++;
				while (*p)
					if (*p++ == ',')
						break;
			}
			break;
		case 'n':
			if ((Snid = atol(p + 1)) <= 0)
				fatal(0, "bad sync id", *ap);
			break;
		case 'p':
			if ((Psysid = atoi(p + 1)) < 0)
				fatal(0, "bad parent system id", *ap);
			break;
		case 'r':
			if ((Xrid = atol(p + 1)) <= 0)
				fatal(0, "bad xres id", *ap);
			break;
		case 's':
			if ((Mysysid = atoi(p + 1)) < 0)
				fatal(0, "bad system id", *ap);
			break;
		default:
			fatal(0, "bad ti env argument", *ap);
			/* NOTREACHED */
		}
	}

	TRACE2(Tbuf, 6, "free ti env args = %s", itox(args));
	free((char *) args);

	if (Mysysid < 0)
		fatal(0, "sysid not assigned", (char *) 0);

	if (Snid < 0)
		fatal(0, "snid not assigned", (char *) 0);

	if (Nsname <= 0)
		fatal(0, "system name list not assigned", (char *) 0);

	/* assign my process type */
	switch (Myptype = tcmptype()) {
	case PT_MTCM:
	case PT_STCM:
		break;
	default:
		fatal(0, "ptype assignment error:", ptptype(Myptype));
	}

	/* perform server- and transport-specific initialisation */
	ti_tcminit();
	ts_tcminit();

	/* log on to syncd and xresd */
	if (sd_logon() < 0 || xd_logon() < 0)
		exit(1);

	/* get some memory for the autosync results */
	if ((synreq = (struct synreq *) malloc(Nsname * sizeof *synreq)) == (struct synreq *) 0) {
		tet_error(errno, "can't get memory for synreq array");
		tet_exit(1);
	}
	TRACE2(Tbuf, 6, "allocate synreq = %s", itox(synreq));

	/* here, TCMs sync with each other, or a TCMrem process syncs
		with its parent */
	nsys = Nsname;
	if (doasync(SV_EXEC_SPNO, SV_YES, SV_EXEC_TIMEOUT, synreq, &nsys) < 0) {
		tet_error(sd_errno, "initial sync failed");
		tet_exit(1);
	}

	/* report a sync that failed in an "expected" way */
	if (sd_errno != ER_OK) {
		for (sp = synreq; sp < synreq + nsys; sp++)
			switch (sp->sy_state) {
			case SS_SYNCYES:
				break;
			default:
				(void) sprintf(errmsg,
				"initial sync error, sysid = %d, state = %s",
					sp->sy_sysid, systate(sp->sy_state));
				tet_error(sd_errno, errmsg);
			}
		tet_exit(1);
	}

	TRACE2(Tbuf, 6, "free synreq = %s", itox(synreq));
	free((char *) synreq);
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
	/* emit a diagnostic if this is unexpected */
	if ((pp->pt_flags & PF_LOGGEDOFF) == 0)
		error(0, "server connection closed", rtoa(&pp->pt_rid));

	pp->pt_flags = (pp->pt_flags & ~PF_LOGGEDON) | PF_LOGGEDOFF;
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
**	tcm does not make use of server-specific data
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

