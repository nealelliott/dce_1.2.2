/*
 *      SCCS:  @(#)tccd.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tccd.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tccd.c	1.2 92/09/30
NAME:		tccd.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	server-specific functions for tccd server

MODIFICATIONS:

************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <pwd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "sptab.h"
#include "ltoa.h"
#include "error.h"
#include "avmsg.h"
#include "valmsg.h"
#include "server.h"
#include "servlib.h"
#include "dtetlib.h"
#include "tslib.h"
#include "tccd.h"

int Mypid = -1;				/* my process id */
char *Progname = "tccd";		/* my program name */
int Myptype = PT_STCC;			/* my process type */
int Mysysid = -1;			/* my system id (assigned later) */
char *Myusername = "tet";		/* user name to run as */
char *Logfile = "/tmp/tccdlog";		/* file name for stdout and stderr */
char *Tet_root;				/* TET_ROOT value */
char *Tet_execute;			/* TET_EXECUTE value (if any) */
char *Tet_tsroot;			/* TET_TSROOT value */
char *Tet_config;			/* TET_CONFIG value */

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/* pointers to error handlers - used in the generic code and libraries */
void (*Error)() = logerror;
void (*Fatal)() = genfatal;

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#else
extern char *getenv();
extern struct passwd *getpwnam(), *getpwuid();
extern void endpwent(), exit(), free();
#endif

/* local function declarations */
#if __STDC__
int main(int, char **);
static void op_sysid(struct ptab *);
static void op_sysname(struct ptab *);
static SIG_FUNC_T terminate(int);
static int varset(char *, char **, char *);
#else /* __STDC__ */
int main();
static void op_sysid();
static void op_sysname();
static SIG_FUNC_T terminate();
static int varset();
#endif /* __STDC__ */


/************************************************************************
**									*
**	MAIN ROUTINES							*
**									*
************************************************************************/

int main(argc, argv)
int argc;
char **argv;
{
	SIG_FUNC_T terminate();

	/* ignore certain signals */
	(void) signal(SIGHUP, SIG_IGN);
	(void) signal(SIGINT, SIG_IGN);
	(void) signal(SIGQUIT, SIG_IGN);

	/* open the error log file */
	(void) umask(022);
	loginit();

	/* catch SIGTERM */
	if (signal(SIGTERM, SIG_IGN) != SIG_IGN)
		(void) signal(SIGTERM, terminate);

	/* call the generic server */
	return(si_main(argc, argv, 0));
}

/*
**	ss_argproc() - tccd command-line argument processing
**
**	return 0 if only firstarg was used or 1 if both args were used
*/

int ss_argproc(firstarg, nextarg)
char *firstarg, *nextarg;
{
	register int rc = 0;
	register int mask;
	register char *p;

	switch (*(firstarg + 1)) {
	case 'u':
		if (*(firstarg + 2))
			Myusername = firstarg + 2;
		else {
			Myusername = nextarg;
			rc = 1;
		}
		break;
	case 'l':
		if (*(firstarg + 2))
			Logfile = firstarg + 2;
		else {
			Logfile = nextarg;
			rc = 1;
		}
		loginit();
		break;
	case 'm':
		if (*(firstarg + 2))
			p = firstarg + 2;
		else {
			p = nextarg;
			rc = 1;
		}
		mask = 0;
		while (*p >= '0' && *p <= '7')
			mask = (mask << 3) | (*p++ & 07);
		(void) umask(mask & 077);
		break;
	default:
		return(ss_tsargproc(firstarg, nextarg));
	}

	return(rc);
}

/*
**	ss_initdaemon() - tccd daemon initialisation
*/

void ss_initdaemon()
{
	register struct passwd *pw;
	register int euid, egid;
	register char *p;
	static char home[] = "HOME";
	char buf[sizeof home + MAXPATH + 1];
	static char null[] = "/dev/null";
	static char msg[] = "can't find passwd entry for";

	/* get effecive ids */
	euid = geteuid();
	egid = getegid();

	/* get password file entry for the specified user name */
	errno = 0;
	if ((pw = getpwnam(Myusername)) == (struct passwd *) 0) {
		error(errno != ENOTTY ? errno : 0, msg, Myusername);
		if (euid < 100)
			exit(1);
		errno = 0;
		if ((pw = getpwuid(euid)) == (struct passwd *) 0)
			fatal(errno, msg, itoa(euid));
	}
	endpwent();

	/* change uid and gid if necessary */
	if (getgid() != pw->pw_gid || egid != pw->pw_gid)
		if (setgid(pw->pw_gid) < 0) {
			error(errno, "can't set gid to", itoa(pw->pw_gid));
			if (egid < 100)
				exit(1);
		}
	if (getuid() != pw->pw_uid || euid != pw->pw_uid)
		if (setuid(pw->pw_uid) < 0) {
			error(errno, "can't set uid to", itoa(pw->pw_uid));
			if (euid < 100)
				exit(1);
		}

	/* fix up HOME environment variable if necessary */
	if ((p = getenv(home)) == (char *) 0 || strcmp(p, pw->pw_dir)) {
		(void) sprintf(buf, "%s=%s", home, pw->pw_dir);
		if ((p = strstore(buf)) == (char *) 0 || dtputenv(p) < 0)
			exit(1);
	}

	/* remember home directory for later and chdir to there */
	p = getenv(home);
	ASSERT(p && *p);
	if (tetrootset(p) < 0)
		exit(1);
	TRACE3(Ttccd, 4, "chdir to %s HOME: \"%s\"", pw->pw_name, Tet_root);
	if (chdir(Tet_root) < 0)
		fatal(errno, "can't chdir to", Tet_root);

	/* perform syncd transport-specific initialisation */
	ss_tsinitb4fork();

	/* attach stdin to /dev/null and stdout to stderr (which goes
		to the log file */
	(void) close(0);
	errno = 0;
	if (open(null, O_RDONLY) != 0)
		fatal(errno, "can't open", null);
	(void) close(1);
	errno = 0;
	if (fcntl(fileno(stderr), F_DUPFD, 1) != 1)
		fatal(errno, "can't attach stdout to stderr", (char *) 0);

#ifndef INITTAB
	/* start the daemon */
	ts_forkdaemon();
#endif

	/* detach from the control terminal (if any) */
	tiocnotty();

	/* make a log file entry */
	ts_logstart();

}

/*
**	ss_serverloop() - tccd daemon main processing loop
**
**	always return 1 because we always want to come back
*/

int ss_serverloop()
{
	si_serverloop();
	return(1);
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
	static char fmt[] = "%s connection closed";
	static char cl[] = "client";
	static char se[] = "server";
	char msg[sizeof fmt + sizeof cl];
	extern struct ptab *Ptab;

	/* emit a diagnostic message if this is unexpected */
	if ((pp->pt_flags & PF_LOGGEDOFF) == 0) {
		(void) sprintf(msg, fmt, (pp->pt_flags & PF_SERVER) ? se : cl);
		error(0, msg, rtoa(&pp->pt_rid));
	}

	pp->pt_flags = (pp->pt_flags & ~PF_LOGGEDON) | PF_LOGGEDOFF;

	/* for clients:
		remove any related file and exec table elements
		remove ptab entry
		exit if it was the last one */
	if ((pp->pt_flags & PF_SERVER) == 0) {
		fiodead(pp);
		etdead(pp);
		ptrm(pp);
		ptfree(pp);
		if (!Ptab)
			ss_cleanup();
	}
}

/*
**	ss_connect() - connect to remote process
*/

void ss_connect(pp)
struct ptab *pp;
{
	ts_connect(pp);
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
	case OP_SYSID:
		op_sysid(pp);
		break;
	case OP_TSINFO:
		op_tsinfo(pp);
		break;
	case OP_SYSNAME:
		op_sysname(pp);
		break;
	case OP_EXEC:
		op_exec(pp);
		break;
	case OP_WAIT:
		op_wait(pp);
		break;
	case OP_KILL:
		op_kill(pp);
		break;
	case OP_CFNAME:
		op_cfname(pp);
		break;
	case OP_SNDCONF:
		op_sndconf(pp);
		break;
	case OP_RCVCONF:
		op_rcvconf(pp);
		break;
	case OP_PUTENV:
		op_putenv(pp);
		break;
	case OP_ACCESS:
		op_access(pp);
		break;
	case OP_MKDIR:
		op_mkdir(pp);
		break;
	case OP_RMDIR:
		op_rmdir(pp);
		break;
	case OP_CHDIR:
		op_chdir(pp);
		break;
	case OP_FOPEN:
		op_fopen(pp);
		break;
	case OP_FCLOSE:
		op_fclose(pp);
		break;
	case OP_PUTS:
		op_puts(pp);
		break;
	case OP_LOCKFILE:
		op_lockfile(pp);
		break;
	case OP_SHARELOCK:
		op_sharelock(pp);
		break;
	case OP_MKTMPDIR:
		op_mktmpdir(pp);
		break;
	case OP_UNLINK:
		op_unlink(pp);
		break;
	case OP_CONFIG:
		op_config(pp);
		break;
	case OP_RXFILE:
		op_rxfile(pp);
		break;
	case OP_MKSDIR:
		op_mksdir(pp);
		break;
	case OP_TSFILES:
		op_tsfiles(pp);
		break;
	default:
		pp->ptm_rc = ER_REQ;
		pp->ptm_len = 0;
		break;
	}

	/* here to send a reply message */
	pp->pt_state = PS_SNDMSG;
	pp->pt_flags |= PF_ATTENTION;
}

/*
**	op_sysid() - assign system id
*/

static void op_sysid(pp)
register struct ptab *pp;
{
	register struct valmsg *mp = (struct valmsg *) pp->ptm_data;

	/* all reply messages have no data */
	pp->ptm_len = 0;

	/* do a sanity check on the request message */
	if ((int) mp->vm_nvalue != OP_SYSID_NVALUE) {
		pp->ptm_rc = ER_INVAL;
		return;
	}

	if (Mysysid >= 0) {
		error(0, "sysid already assigned:", itoa(Mysysid));
		pp->ptm_rc = ER_ERR;
		return;
	}

	Mysysid = (int) VM_SYSID(mp);
	logent("my sysid =", itoa(Mysysid));
	pp->ptm_rc = ER_OK;
}

/*
**	op_sysname() - receive a system name list
**
**	in the request message:
**		VM_SYSNAME(0) .. VM_SYSNAME(op_sysname_nsname() - 1) =
**			list of system names
**
*/

static void op_sysname(pp)
register struct ptab *pp;
{
	register struct valmsg *mp = (struct valmsg *) pp->ptm_data;
	register struct sptab *sp = (struct sptab *) pp->pt_sdata;
	register int *snp;
	register int i, j, needlen;

	/* all reply messages have no data */
	pp->ptm_len = 0;

	/* do a sanity check on the request message -
		make sure that there are no duplicates in the system
		name list */
	if (OP_SYSNAME_NSNAME(mp) < 1) {
		pp->ptm_rc = ER_INVAL;
		return;
	}
	for (i = 0; i < OP_SYSNAME_NSNAME(mp); i++) {
		if (VM_SYSNAME(mp, i) < 0) {
			pp->ptm_rc = ER_INVAL;
			return;
		}
		for (j = 0; j < i; j++)
			if (VM_SYSNAME(mp, j) == VM_SYSNAME(mp, i)) {
				pp->ptm_rc = ER_DUPS;
				return;
			}
	}

	/* allocate storage for the system name list */
	needlen = OP_SYSNAME_NSNAME(mp) * sizeof *sp->sp_snames;
	if (BUFCHK((char **) &sp->sp_snames, &sp->sp_snlen, needlen) < 0) {
		pp->ptm_rc = ER_ERR;
		return;
	}

	/* copy over the system name list */
	sp->sp_nsname = OP_SYSNAME_NSNAME(mp);
	for (i = 0, snp = sp->sp_snames; i < OP_SYSNAME_NSNAME(mp); i++, snp++)
		*snp = (int) VM_SYSNAME(mp, i);

	pp->ptm_rc = ER_OK;
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
struct ptab *pp;
{
	switch (pp->ptr_ptype) {
	case PT_MTCC:
	case PT_MTCM:
	case PT_STCM:
		return(ss_tslogon());
	default:
		error(0, "unexpected connected process type",
			rtoa(&pp->pt_rid));
		return(ER_ERR);
	}
}

/*
**	ss_logoff() - server-specific logoff processing routine
*/

void ss_logoff(pp)
struct ptab *pp;
{
	etdead(pp);
}

/*
**	ss_cleanup() - clean up and exit
*/

void ss_cleanup()
{
	if (Tet_config)
		(void) unlink(Tet_config);

	(void) xd_logoff();
	exit(0);
}

/************************************************************************
**									*
**	PUBLIC SUBROUTINES						*
**									*
************************************************************************/

/*
**	ss_newptab() - server-specific new ptab entry handler
*/

void ss_newptab(pp)
struct ptab *pp;
{
	/* add the entry to the process table */
	ptadd(pp);
}

/************************************************************************
**									*
**	PRIVATE SUBROUTINES						*
**									*
************************************************************************/

/*
**	terminate() - SIGTERM signal handler
*/

static SIG_FUNC_T terminate(sig)
int sig;
{
	logent("going down on signal", itoa(sig));
	exit(0);
}

/*
**	tetrootset(), tsrootset(), tetexecset() - assign a new value to
**		Tet_root, Tet_tsroot or Tet_exececute
**
**	return 0 if successful or -1 on error
*/

int tetrootset(s)
char *s;
{
	return(varset(s, &Tet_root, "Tet_root"));
}

int tsrootset(s)
char *s;
{
	return(varset(s, &Tet_tsroot, "Tet_tsroot"));
}

int tetexecset(s)
char *s;
{
	return(varset(s, &Tet_execute, "Tet_execute"));
}

/*
**	varset() - set a global variable
**
**	return 0 if successful or -1 on error
*/

#ifdef NOTRACE
/* ARGSUSED */
#endif
static int varset(s, vp, name)
char *s, **vp, *name;
{
	register char *oldval;

	oldval = *vp;

	if ((*vp = strstore(s)) == (char *) 0) {
		*vp = oldval;
		return(-1);
	}

	if (oldval) {
		TRACE3(Tbuf, 6, "free old %s = %s", name, itox(oldval));
		free(oldval);
	}

	return(0);
}

