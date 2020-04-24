/*
 *      SCCS:  @(#)remexec.c	1.3 (92/10/02) 
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
static char sccsid[] = "@(#)remexec.c	1.3 (92/10/02) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)remexec.c	1.3 92/10/02
NAME:		remexec.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

SYNOPSIS:
	#include <dapi.h>
	int tet_remexec(int sysname, char *file, char **argv);

DESCRIPTION:
	DTET API function

	execute process on remote system
	return remoteid if successful or -1 on error

	errno is set to one of the following when tet_remexec returns -1
	(a * indicates a value not mentioned in the DTET spec)

		EINVAL	sysname does not refer to a known slave system

		ENOEXEC file cannot be executed on the remote system,
			or sync with remote process failed

		ENOMEM*	out of memory on the local system

		EFAULT*	null file or argv value

		EIO*	communication with server processes failed

MODIFICATIONS:

************************************************************************/


#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "rtab.h"
#include "avmsg.h"
#include "synreq.h"
#include "ltoa.h"
#include "error.h"
#include "servlib.h"
#include "dtetlib.h"
#include "dapi.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

static char **econf;			/* master exec econfig variables */
static int neconf;			/* number of master exec variables */
static int leconf;			/* number of bytes in *econf */
static char **dconf;			/* master distrib config variables */
static int ndconf;			/* number of master dist variables */
static int ldconf;			/* number of bytes in *dconf */
static char **cconf;			/* MTCC cmd line config variables */
static int ncconf;			/* number of MTCC cmd line variables */
static int lcconf;			/* number of bytes in *cconf */

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#else
extern void free();
#endif

/* static function declarations */
#if __STDC__
static int do_tclogon(int);
static int getcf2(char *, char ***, int *, int *);
static int getcf3(int, char ***, int *, int *);
static int getconf();
static long tet_re2(int, char *, char **);
static int xconfig(int);
static int xcf2(int, char **, int);
#else
static int do_tclogon();
static int getcf2();
static int getcf3();
static int getconf();
static long tet_re2();
static int xconfig();
static int xcf2();
#endif

int tet_remexec(sysname, file, argv)
int sysname;
char *file;
register char **argv;
{
	register struct rtab *rp;
	register char **ap;
	register long pid;
	register int needlen;
	char thistest[LNUMSZ], activity[LNUMSZ];
	char context[LNUMSZ], block[LNUMSZ];
	static char **newargv;
	static int nalen;
	extern int Mysysid, tet_thistest;
	extern long tet_activity, tet_context, tet_block;

	/* do a sanity check on the arguments */
	if (sysname <= 0 || sysname == Mysysid) {
		errno = EINVAL;
		return(-1);
	}
	if (!file || !argv) {
		errno = EFAULT;
		return(-1);
	}

	/* count the arguments */
	for (ap = argv; *ap; ap++)
		;

	/* build the new argv with thistest, activity, context and block
		at the front */
	needlen = ((ap - argv) + 5) * sizeof *newargv;
	if (BUFCHK((char **) &newargv, &nalen, needlen) < 0) {
		errno = ENOMEM;
		return(-1);
	}
	(void) sprintf(thistest, "%d", tet_thistest);
	(void) sprintf(activity, "%ld", tet_activity);
	(void) sprintf(context, "%ld", tet_context);
	(void) sprintf(block, "%ld", tet_block);
	ap = newargv;
	*ap++ = thistest;
	*ap++ = activity;
	*ap++ = context;
	*ap++ = block;

#if 0
        /* this code doesn't load a NULL at the end UNLESS NULL was
         * the first arg in the arg!
	 */
	do {
		*ap++ = *argv++;
	} while (*argv);
#else
        /* walk the argv to the terminating NULL, and then
         * add a NULL of our own.
         */
        for(; *argv; *ap++ = *argv++)
            ;
        *ap = NULL; /* terminating arg */
#endif

	/* allocate an rtab element for the result */
	if ((rp = rtalloc()) == (struct rtab *) 0) {
		errno = ENOMEM;
		return(-1);
	}

	if ((pid = tet_re2(sysname, file, newargv)) < 0) {
		rtfree(rp);
		return(-1);
	}

	rp->rt_sysid = sysname;
	rp->rt_pid = pid;
	rtadd(rp);

	return(rp->rt_remoteid);
}

/*
**	tet_re2() - extend the tet_remexec() processing
**
**	return pid of exec'd process if successful or -1 (with errno set)
**	on error
*/

static long tet_re2(sysid, file, argv)
int sysid;
char *file, **argv;
{
	long pid;
	int sysnames[2];
	register long snid;
	extern int Mysysid;
	extern long Xrid;

	/* log on to the STCC if necessary */
	if (!getptbysysptype(sysid, PT_STCC) && do_tclogon(sysid) < 0) {
		errno = EINVAL;
		return(-1L);
	}

	/* get a snid for use in the sync after the exec */
	if ((snid = sd_snget()) < 0L) {
		errno = EIO;
		return(-1L);
	}

	/* register our sysid and the remote sysid with SYNCD */
	sysnames[0] = Mysysid;
	sysnames[1] = sysid;
	if (sd_snsys(snid, sysnames, 2) < 0) {
		errno = EIO;
		return(-1L);
	}

	/* do the remote exec */
	if ((pid = tc_uexec(sysid, file, argv, snid, Xrid)) < 0L) {
		errno = ENOEXEC;
		return(-1L);
	}

	/* do an auto-sync with the new process, kill the process and wait
		for it a bit if this fails */
	if (sd_sasync(snid, SV_EXEC_SPNO, SV_YES, SV_EXEC_TIMEOUT, (struct synreq *) 0, (int *) 0, (long *) 0) < 0) {
		(void) tc_kill(sysid, pid, SIGTERM);
		(void) tc_wait(sysid, pid, 10, (int *) 0);
		errno = ENOEXEC;
		return(-1L);
	}

	return(pid);
}

/*
**	do_tclogon() - log on to the STCC and do a config variable exchange
**
**	return 0 if successful or -1 on error
*/

static int do_tclogon(sysid)
int sysid;
{
	extern int *Snames, Nsname;
	static int been_here = 0;

	/* log on to the STCC */
	if (tc_logon(sysid) < 0)
		return(-1);

	/* get the config variables from XRESD first time through */
	if (!been_here) {
		if (getconf() < 0)
			return(-1);
		been_here = 1;
	}

	/* send the system name list to the STCC */
	if (tc_sysname(sysid, Snames, Nsname) < 0) {
		error(0, "tc_sysname failed, rc =", ptrepcode(tc_errno));
		return(-1);
	}

	/* perform a config variable exchange with STCC */
	if (xconfig(sysid) < 0)
		return(-1);

	/* all ok so return success */
	return(0);
}

/*
**	getconf() - get the set of master config lines from XRESD
**
**	return 0 if successful or -1 on error
*/

static int getconf()
{
	register int n;
	register char **ap;
	static char *cfname[XD_NCFNAME];

	/* get the config file names from XRESD */
	if (!cfname[XD_NCFNAME - 1]) {
		if ((ap = xd_rcfname()) == (char **) 0) {
			error(0, "xd_rcfname failed, rc =",
				ptrepcode(xd_errno));
			return(-1);
		}
		for (n = 0; n < XD_NCFNAME; n++) {
			if ((cfname[n] = strstore(*ap++)) == (char *) 0)
				return(-1);
			TRACE3(Ttcm, 4, "master config file name %s = \"%s\"",
				itoa(n), cfname[n]);
		}
	}

	/* get the master exec and distrib config lines, and the MTCC
		cmd line config lines */
	if (getcf2(cfname[0], &econf, &leconf, &neconf) < 0 ||
		getcf2(cfname[1], &dconf, &ldconf, &ndconf) < 0 ||
		getcf2(cfname[2], &cconf, &lcconf, &ncconf) < 0)
			return(-1);

	/* all ok so return success */
	return(0);
}

/*
**	getcf2() - get a set of lines from a single master config file
**
**	return 0 if successful or -1 on error
*/

static int getcf2(cfname, confp, lconfp, nconfp)
char *cfname;
char ***confp;
int *lconfp, *nconfp;
{
	register fid, rc;
	static char fmt[] = "xd_open(\"%.*s\") failed, rc =";
	char msg[sizeof fmt + MAXPATH];

	TRACE2(Ttcm, 4, "get master config variables from \"%s\"", cfname);

	/* open the master config file */
	if ((fid = xd_fopen(cfname)) < 0) {
		(void) sprintf(msg, fmt, MAXPATH, cfname);
		error(0, msg, ptrepcode(xd_errno));
		return(-1);
	}

	/* get the config lines and close the file */
	rc = getcf3(fid, confp, lconfp, nconfp);
	(void) xd_fclose(fid);

	return(rc);
}

/*
**	getcf3() - get master config lines and add them to the list pointed to
**		by *confp containing *nconfp entries
**
**	return 0 if successful or -1 on error
*/

static int getcf3(fid, confp, lconfp, nconfp)
int fid;
char ***confp;
int *lconfp, *nconfp;
{
	register char **p1, **p2;
	register int n;
	int nlines, eof;
	char *cp[AV_NLINE];

	/* read in all the config lines and store them */
	do {
		nlines = AV_NLINE;
		if ((p1 = xd_getsv(fid, &nlines, &eof)) == (char **) 0) {
			error(0, "xd_getsv failed, rc =", ptrepcode(xd_errno));
			return(-1);
		}
		if (nlines <= 0)
			continue;
		for (p2 = cp, n = 0; n < nlines && n < AV_NLINE; n++, p1++)
			if (*p1 && **p1 && **p1 != '#') {
				TRACE2(Ttcm, 4, "read variable \"%s\"", *p1);
				if ((*p2++ = strstore(*p1)) == (char *) 0)
					return(-1);
			}
		nlines = p2 - cp;
		if (BUFCHK((char **) confp, lconfp, (int) ((*nconfp + nlines) * sizeof **confp)) < 0)
			return(-1);
		p1 = cp;
		p2 = *confp + *nconfp;
		for (n = 0; n < nlines; n++)
			*p2++ = *p1++;
		*nconfp += n;
	} while (!eof);

	/* all ok so return success */
	return(0);
}

/*
**	xconfig() - perform a config variable exchange
**
**	return 0 if successful or -1 on error
*/

static int xconfig(sysid)
int sysid;
{
	char **sconf = (char **) 0;
	int lsconf = 0, nsconf = 0;
	char **tconf = (char **) 0;
	int ltconf = 0, ntconf = 0;
	register char **p1, **p2, **p3;
	register char *s1, *s2;
	register int n, rc;
	char buf[BUFSIZ];
	static char name[] = "TET_TSROOT";
	static char ecfname[] = "tetexec.cfg";

	if ((nsconf = neconf + ndconf + ncconf) <= 0)
		return(0);

	/* look for TET_REMnnn_TET_TSROOT in the distrib config information */
	for (p1 = dconf + ndconf - 1; p1 >= dconf; p1--) {
		if ((s1 = remvar(*p1, sysid)) == (char *) 0 ||
			(s2 = equindex(*p1)) == (char *) 0) {
				error(0, "ignored bad format distrib config line:", *p1);
			continue;
		}
		if (s1 != *p1 && !strncmp(s1, name, s2 - s1))
			break;
	}

	/* if not found, look for TET_TSROOT without the REMnnn */
	if (p1 < dconf)
		for (p1 = dconf + ndconf - 1; p1 >= dconf; p1--)
			if ((s2 = equindex(*p1)) != (char *) 0 &&
				!strncmp(*p1, name, s2 - *p1))
					break;

	/* if still not found, return error */
	if (p1 < dconf) {
		error(0, "no TET_TSROOT for system", itoa(sysid));
		return(-1);
	}

	/* now *p1 points to TET_TSROOT line and s2 points to the '=';
		construct target system exec config file name and send it
		to TCCD */
	(void) sprintf(buf, "%.*s/%s",
		MAXPATH - sizeof ecfname, ++s2, ecfname);
	TRACE3(Ttcm, 6, "send exec config file name \"%s\" to system %s",
		buf, itoa(sysid));

	/* send exec config file name to TCCD */
	if (tc_cfname(sysid, buf) < 0) {
		error(0, "tc_cfname failed, rc =", ptrepcode(tc_errno));
		return(-1);
	}

	/* allocate storage for the list of config lines to send */
	if (BUFCHK((char **) &sconf, &lsconf, (int) (nsconf * sizeof *sconf)) < 0)
		return(-1);

	/* copy over the master exec and distrib config lines */
	p2 = sconf;
	for (p1 = econf, n = 0; n < neconf; n++)
		*p2++ = *p1++;
	for (p1 = dconf, n = 0; n < ndconf; n++)
		*p2++ = *p1++;

	/*
	** do two passes over the MTCC cmd line variables:
	**	in the first pass:
	**		if a variable is not a TET_REMnnn one, prepend
	**		a TET_REMnnn string so that it will override a
	**		local value on the remote system;
	**		a pointer to the new string is stored in a temporary
	**		list (tconf) so that it can be freed later
	**	in the second pass:
	**		add the TET_REMnnn variables last of all,
	**		giving them precedence over all the others
	*/

	/* first pass */
	for (p1 = cconf, n = ncconf; n > 0; p1++, n--) {
		if (remvar(*p1, -1) != *p1)
			continue;
		(void) sprintf(buf, "TET_REM%03d_%s", sysid % 1000, *p1);
		if (BUFCHK((char **) &tconf, &ltconf, (int) ((ntconf + 1) * sizeof *tconf)) < 0)
			return(-1);
		p3 = tconf + ntconf++;
		if ((*p3 = strstore(buf)) == (char *) 0)
			return(-1);
		if (p2 < sconf + nsconf)
			*p2++ = *p3;
	}

	/* second pass */
	for (p1 = cconf, n = ncconf; n > 0; p1++, n--) {
		if (remvar(*p1, sysid) == *p1)
			continue;
		if (p2 < sconf + nsconf)
			*p2++ = *p1;
	}

	rc = xcf2(sysid, sconf, nsconf);

	/* free memory allocated above */
	if (tconf) {
		for (p1 = tconf + ntconf - 1; p1 >= tconf; p1--)
			if (*p1) {
				TRACE2(Tbuf, 6, "free tmp conf line = %s",
					itox(*p1));
				free(*p1);
			}
		TRACE2(Tbuf, 6, "free tconf = %s", itox(tconf));
		free((char *) tconf);
	}
	if (sconf) {
		TRACE2(Tbuf, 6, "free sconf = %s", itox(sconf));
		free((char *) sconf);
	}

	return(rc);
}

/*
**	xcf2() - extend the xconfig processing
**
**	return 0 if successful or -1 on error
*/

static int xcf2(sysid, sconf, nsconf)
int sysid, nsconf;
char **sconf;
{
	char **rconf = (char **) 0;
	int lrconf = 0, nrconf = 0;
	int nlines, done;
	register int n, err;
	register char **p1, **p2;

	/* send the config lines */
	if (tc_sndconfv(sysid, sconf, nsconf) < 0) {
		error(0, "tc_sndconfv failed, rc =", ptrepcode(tc_errno));
		return(-1);
	}

	/* receive the merged ones back */
	err = 0;
	do {
		if ((p1 = tc_rcvconfv(sysid, &nlines, &done)) == (char **) 0) {
			error(0, "tc_rcvconfv failed, rc =",
				ptrepcode(tc_errno));
			return(-1);
		}
		if (nlines <= 0)
			continue;
		if (BUFCHK((char **) &rconf, &lrconf, (int) ((nrconf + nlines) * sizeof *rconf)) < 0)
			return(-1);
		p2 = rconf + nrconf;
		for (n = 0; n < nlines; n++, p1++)
			if (*p1 && (*p2++ = strstore(*p1)) == (char *) 0) {
				err = 1;
				break;
			}
		nrconf = p2 - rconf;
	} while (!done && !err);

	/* assign the merged lines to the STCC */
	if (!err && tc_configv(sysid, rconf, nrconf) < 0) {
		error(0, "tc_configv failed, rc = ", ptrepcode(tc_errno));
		err = 1;
	}

	/* free storage occupied by the merged lines */
	if (rconf) {
		for (p1 = rconf + nrconf - 1; p1 >= rconf; p1--)
			if (*p1) {
				TRACE2(Tbuf, 6, "free rcvd config line = %s",
					itox(*p1));
				free(*p1);
			}
		TRACE2(Tbuf, 6, "free rconf = %s", itox(rconf));
		free((char *) rconf);
	}

	return(err ? -1 : 0);
}

