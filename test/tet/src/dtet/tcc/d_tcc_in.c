/*
 *      SCCS:  @(#)d_tcc_in.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)d_tcc_in.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)d_tcc_in.c	1.2 92/09/30
NAME:		tcc_in.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	client-specific functions for tcc INET version

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "dtmac.h"
#include "ltoa.h"
#include "error.h"
#include "dtmsg.h"
#include "ptab.h"
#include "tptab_in.h"
#include "tsinfo_in.h"
#include "bstring.h"
#include "tccfuncs.h"
#include "server.h"
#include "server_in.h"
#include "inetlib_in.h"
#include "dtetlib.h"
#include "tslib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <unistd.h>
#else
extern void _exit();
#endif

/* static function declarations */
#if __STDC__
static int ts_ss2(struct ptab *, char **, int);
#else /* __STDC__ */
static int ts_ss2();
#endif /* __STDC__ */


/*
**	ts_tccinit() - transport-specific tcc initialisation
**
**	return 0 if successful or -1 on error
*/

int ts_tccinit()
{
	/* get localhost net address */
	return(getlocalhostaddr() ? 0 : -1);
}

/*
**	ts_stserver() - start an INET server
**
**	return 0 if successful or -1 on error
*/

int ts_stserver(pp, argv)
struct ptab *pp;
char **argv;
{
	register int rc, sd;

	/* get a socket for the server */
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		error(errno, "can't create server socket", (char *) 0);
		return(-1);
	}

	rc = ts_ss2(pp, argv, sd);
	(void) close(sd);

	return(rc);
}

/*
**	ts_ss2() - extend ts_stserver() processing
**
**	return 0 if successful or -1 on error
*/

static int ts_ss2(pp, argv, sd)
struct ptab *pp;
char **argv;
int sd;
{
	register struct tptab *tp = (struct tptab *) pp->pt_tdata;
	register int fd, pid, rc;
	struct sockaddr_in sin;
	int len, status;
	char path[MAXPATH +1];
	extern char tet_root[];
#ifndef NOTRACE
	char **ap;
#endif

	/* bind the socket to an ephemeral port */
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = 0;
	if (bind(sd, (struct sockaddr *) &sin, sizeof sin) < 0) {
		error(errno, "can't bind to server socket", (char *) 0);
		return(-1);
	}

	/* determine the port number */
	len = sizeof sin;
	if (getsockname(sd, &sin, &len) < 0) {
		error(errno, "can't get server socket name", (char *) 0);
		(void) close(sd);
		return(-1);
	}

	/* fork and exec the daemon */
	if ((pid = dofork()) < 0) {
		error(errno, "can't fork", (char *) 0);
		return(-1);
	}
	else if (pid == 0) {
		/* in child */
		(void) sprintf(path, "%.*s/bin/%s",
			sizeof path - strlen(*argv) - 6, tet_root, *argv);
#ifndef NOTRACE
		TRACE3(Ttcc, 2, "start server \"%s\", port = %s",
			path, itoa(ntohs(sin.sin_port)));
		if (Ttcc)
			for (ap = argv; *ap; ap++)
				TRACE2(Ttcc, 6, "argv = \"%s\"", *ap);
#endif
		/* dup the socket on to fd 0 and close all other
			files except 1 and 2 */
		(void) close(0);
		if ((rc = fcntl(sd, F_DUPFD, 0)) != 0) {
			error(errno, "server socket: fcntl(F_DUPFD) returned",
				itoa(rc));
			_exit(~0);
		}
		for (fd = dogetdtablesize() - 1; fd > 2; fd--)
			(void) close(fd);
		(void) execv(path, argv);
		error(errno, "can't exec", path);
		_exit(~0);
	}
	else {
		/* in parent - wait for daemon to start */
		status = 0;
		while ((rc = wait(&status)) != pid)
			if (rc < 0) {
				error(errno, "wait failed:", *argv);
				return(-1);
			}
	}

	if (status) {
		TRACE4(Ttcc, 2, "%s: exit status %s, signal %s", *argv,
			itox((status >> 8) & 0xff), itoa(status & 0xff));
		return(-1);
	}
	else
		TRACE2(Ttcc, 8, "%s started ok", *argv);

	/* remember the server's inet address and port no for later */
	tp->tp_sin = sin;
	tp->tp_sin.sin_addr = *getlocalhostaddr();

	return(0);
}

/*
**	ss_tsconnect() - tcc transport-specific connect routine
**
**	return 0 if successful or -1 on error
*/

int ss_tsconnect(pp)
struct ptab *pp;
{
	/* work out where the process is on the network -
		the addresses for SYNCD and XRESD were stored when they
		were started up
		STCC address must be looked up in the DTET systems file,
		and in the hosts and services files */
	switch (pp->ptr_ptype) {
	case PT_SYNCD:
	case PT_XRESD:
		return(0);
	case PT_STCC:
		return(gettccdaddr(pp));
	}

	error(0, "don't know how to connect to", ptptype(pp->ptr_ptype));
	return(-1);
}

/*
**	ss_tsinfo() - construct a tsinfo message relating to a server process
**
**	return 0 if successful or -1 on error
*/

int ss_tsinfo(pp, ptype)
struct ptab *pp;
register int ptype;
{
	register struct tptab *tp;
	register struct tsinfo *mp;
	struct in_addr *ap;
	char hostname[SNAMELEN];
	extern struct ptab *Sd_ptab, *Xd_ptab;

	if ((mp = (struct tsinfo *) ti_msgbuf(pp, sizeof *mp)) == (struct tsinfo *) 0)
		return(-1);

	/* make tp point to the tptab for the server */
	switch (ptype) {
	case PT_SYNCD:
		if (!Sd_ptab) {
			error(0, "must start syncd first", (char *) 0);
			return(-1);
		}
		tp = (struct tptab *) Sd_ptab->pt_tdata;
		break;
	case PT_XRESD:
		if (!Xd_ptab) {
			error(0, "must start xresd first", (char *) 0);
			return(-1);
		}
		tp = (struct tptab *) Xd_ptab->pt_tdata;
		break;
	default:
		error(0, "no tsinfo for", ptptype(ptype));
		return(-1);
	}

	/* all ok so copy over the data and return */
	mp->ts_ptype = ptype;
	mp->ts_port = ntohs(tp->tp_sin.sin_port);

	/* we talk to syncd and xresd using the loopback address, so that
		is what is stored in the tptab structure;
		if the message destination is also localhost,
		simply copy the stored (localhost) address -
		otherwise, the destination is on another machine so
		find our (external) Internet address and use that */
	if (((struct tptab *) pp->pt_tdata)->tp_sin.sin_addr.s_addr == tp->tp_sin.sin_addr.s_addr)
		mp->ts_addr = ntohl(tp->tp_sin.sin_addr.s_addr);
	else {
		if (gethostname(hostname, sizeof hostname) < 0) {
			error(errno, "gethostname() failed", (char *) 0);
			return(-1);
		}
		if ((ap = gethostaddr(hostname)) == (struct in_addr *) 0)
			return(-1);
		mp->ts_addr = ntohl(ap->s_addr);
	}

	pp->ptm_len = sizeof *mp;
	return(0);
}

/*
**	ts_tsinfolen() - return length of a machine-independent tsinfo
**		structure
*/

int ts_tsinfolen()
{
	return(TS_TSINFOSZ);
}

/*
**	ts_tsinfo2bs() - call tsinfo2bs()
*/

int ts_tsinfo2bs(from, to)
char *from, *to;
{
	return(tsinfo2bs((struct tsinfo *) from, to));
}

