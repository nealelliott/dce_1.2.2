/*
 *      SCCS:  @(#)tccd_in.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tccd_in.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tccd_in.c	1.2 92/09/30
NAME:		tccd_in.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	server-specific functions for tccd INET version

MODIFICATIONS:

************************************************************************/


#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <netdb.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "tptab_in.h"
#include "tsinfo_in.h"
#include "error.h"
#include "ltoa.h"
#include "tccd.h"
#include "tccd_bs.h"
#include "server.h"
#include "inetlib_in.h"
#include "servlib.h"
#include "dtetlib.h"

#ifndef __hpux
#include <arpa/inet.h>
#endif

#ifndef INETD
#include <signal.h>
#include <sys/wait.h>
#endif

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

struct ptab *Sd_ptab, *Xd_ptab;		/* ptab elements for syncd and xresd */
static char rhostname[64];		/* remote host name */

#ifdef INETD
int Listen_sd = -1;		/* inetd listens on the socket for us */
#else
int Listen_sd;			/* socket on which to listen */
int Tccdport = 0;		/* tccd well-known port number */
#endif

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#else
extern void exit();
#endif

/* static function declarations */
#if __STDC__
#ifndef INETD
static SIG_FUNC_T waitchild(int);
#endif /* !INETD */
#else /* __STDC__ */
#ifndef INETD
static SIG_FUNC_T waitchild();
#endif /* !INETD */
#endif /* __STDC__ */


/*
**	ss_tsargproc() - transport-specific tccd command-line argument
**		processing
**
**	return 0 if only firstarg was used or 1 if both args were used
*/

#ifdef INETD
/* ARGSUSED */
#endif
int ss_tsargproc(firstarg, nextarg)
char *firstarg, *nextarg;
{
	register int rc = 0;

	switch (*(firstarg + 1)) {
#ifndef INETD
	case 'p':
		if (*(firstarg + 2))
			Tccdport = atoi(firstarg + 2);
		else {
			Tccdport = atoi(nextarg);
			rc = 1;
		}
		if (Tccdport <= 0)
			fatal(0, "bad port number:",
				rc == 1 ? nextarg : firstarg + 2);
		break;
#endif
	default:
		fatal(0, "unknown option", firstarg);
		/* NOTREACHED */
	}

	return(rc);
}

/*
**	ss_tsinitb4fork() - tccd INET initialisation before forking a daemon
*/

void ss_tsinitb4fork()
{
#ifdef INETD

	register struct ptab *pp;
	register struct tptab *tp;
	register int sd;
	int len;

	/* move the incoming request socket off stdin */
	errno = 0;
	if ((sd = fcntl(0, F_DUPFD, 3)) < 3)
		fatal(errno, "can't dup stdin socket", (char *) 0);
	(void) close(0);

	/* allocate a ptab entry for the incoming request */
	if ((pp = ptalloc()) == (struct ptab *) 0)
		exit(1);
	tp = (struct tptab *) pp->pt_tdata;
	tp->tp_sd = sd;

	/* get remote address and store it */
	len = sizeof tp->tp_sin;
	if (getpeername(tp->tp_sd, (struct sockaddr *) &tp->tp_sin, &len) < 0)
		fatal(errno, "can't get name of connected peer", (char *) 0);

	/* log the connection */
	(void) ss_tsafteraccept(pp);

	/* register the ptab entry */
	ss_newptab(pp);

	/* prepare to receive the message */
	pp->pt_state = PS_RCVMSG;
	pp->pt_flags |= PF_ATTENTION;

#else /* INETD */

	struct sockaddr_in sin;
	register int sd;
	SIG_FUNC_T waitchild();

	/* get the tccd port number if not overridden on the cmd line */
	if (Tccdport <= 0 && (Tccdport = gettccdport()) < 0)
		exit(1);
	else
		sin.sin_port = htons((u_short) Tccdport);

	/* get a socket to listen on */
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		fatal(errno, "can't create listen socket", (char *) 0);

	/* make sure that listen socket is not stdin, stdout or stderr */
	if (sd < 3) {
		errno = 0;
		if ((Listen_sd = fcntl(sd, F_DUPFD, 3)) < 3)
			fatal(errno, "can't dup listen socket", (char *) 0);
		(void) close(sd);
	}
	else
		Listen_sd = sd;

	/* bind the socket to the tccd port */
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	if (bind(Listen_sd, (struct sockaddr *) &sin, sizeof sin) < 0)
		fatal(errno, "can't bind to listen socket", (char *) 0);

	/* arrange to accept incoming connections */
	ts_listen(Listen_sd);

	/* arrange to reap child daemon processes */
	(void) signal(SIGCHLD, waitchild);

#endif /* INETD */
}

/*
**	ts_forkdaemon() - start a daemon process
*/

#ifndef INITTAB
void ts_forkdaemon()
{
#ifdef INETD

	/* inetd has already done it for us */

#else /* INETD */

	si_forkdaemon();

#endif /* INETD */
}
#endif /* !INITTAB */

/*
**	ts_logstart() - make a log file entry when the master daemon starts
*/

void ts_logstart()
{
#ifdef INETD

	/* no master daemon */

#else

	logent("START", (char *) 0);

#endif
}

/*
**	ss_tsconnect() - server-specific connect processing
**
**	return 0 if successful or -1 on error
*/

int ss_tsconnect(pp)
struct ptab *pp;
{
	/* syncd and xresd addresses have alread been stored when
		op_tsinfo() was called (which also allocated the ptabs) -
		if this had not happened, we would not have got past
		ti_logon() */
	switch (pp->ptr_ptype) {
	case PT_SYNCD:
	case PT_XRESD:
		return(0);
	}

	error(0, "don't know how to connect to", ptptype(pp->ptr_ptype));
	return(-1);
}

/*
**	ss_tsaccept() - server-specific accept() processing
*/

void ss_tsaccept()
{
#ifdef INETD

	/* inetd has already accepted the connection */

#else

	/* accept the connection unless we are closing down */
	if (Listen_sd >= 0)
		ts_accept(Listen_sd);

#endif
}

/*
**	ss_tsafteraccept() - server-specific things to do after an accept()
**
**	return 0 if successful or -1 on error
*/

int ss_tsafteraccept(pp)
struct ptab *pp;
{
	register struct tptab *tp = (struct tptab *) pp->pt_tdata;
	register struct hostent *hp;
	register char *p;

#ifndef INETD
	register int pid;
	extern int Mypid;
#endif

	/* log the connection */
	if ((hp = gethostbyaddr((char *) &tp->tp_sin.sin_addr, sizeof tp->tp_sin.sin_addr, tp->tp_sin.sin_family)) != (struct hostent *) 0)
		p = hp->h_name;
	else
		p = inet_ntoa(tp->tp_sin.sin_addr);
	(void) sprintf(rhostname, "%.*s", sizeof rhostname - 1, p);
	logent("connection received from", rhostname);

#ifdef INETD

	/* inetd does the accept for us */
	pp->pt_flags |= PF_CONNECTED;

#else

	if ((pid = dofork()) < 0) {
		error(errno, "can't fork", (char *) 0);
		return(-1);
	}
	else if (!pid) {
		/* in child */
		Mypid = getpid();
		(void) close(Listen_sd);
		Listen_sd = -1;
		(void) signal(SIGCHLD, SIG_DFL);
	}
	else {
		/* in parent - return -1 to close the socket and
			free the ptab entry */
		return(-1);
	}

#endif /* INETD */

	return(0);
}

/*
**	ss_tslogon() - make sure that we want to accept a logon request
**		from the connected remote host
**
**	return ER_OK if successful or other ER_* error code on error
*/

int ss_tslogon()
{
	static char equiv[] = "systems.equiv";
	char fname[MAXPATH + 1];
	char *argv[1];
	register int argc, rc;
	register unsigned long raddr;
	FILE *fp;
	extern char *Tet_root;

	(void) sprintf(fname, "%.*s/%s",
		sizeof fname - sizeof equiv - 1, Tet_root, equiv);

	/* open the systems.equiv file */
	if ((fp = fopen(fname, "r")) == NULL) {
		error(errno, "can't open", fname);
		return(ER_ERR);
	}

	/* look for the requesting system in the systems.equiv file */
	raddr = inet_addr(rhostname);
	rc = ER_ERR;
	while ((argc = fgetargs(fp, argv, 1)) != EOF)
		if (argc < 1)
			continue;
		else if ((raddr == -1 && strcmp(argv[0], rhostname) == 0) ||
			inet_addr(argv[0]) == raddr) {
				rc = ER_OK;
				break;
		}

	if (rc != ER_OK)
		error(0, "refused login request from", rhostname);

	(void) fclose(fp);
	return(rc);
}

/*
**	waitchild() - reap a child daemon
*/

#ifdef INETD

	/* no daemon children to wait for */

#else

/* ARGSUSED */
static SIG_FUNC_T waitchild(sig)
int sig;
{
	int status;
#ifndef NOTRACE
	register int pid;
#endif

	(void) signal(SIGCHLD, SIG_DFL);

#ifdef NOTRACE
	while (dowait3(&status, WNOHANG) > 0)
		;
#else
	while ((pid = dowait3(&status, WNOHANG)) > 0)
		TRACE4(Ttccd, 2, "waitchild reaped %s, status %s, signal %s",
			itoa(pid), itoa((status >> 8) & 0xff),
			itoa(status & 0xff));
#endif

	(void) signal(SIGCHLD, waitchild);
}

#endif

/*
**	ts_bs2tsinfo() - call bs2tsinfo()
*/

int ts_bs2tsinfo(from, fromlen, to, tolen)
char *from;
int fromlen;
char **to;
int *tolen;
{
	return(bs2tsinfo(from, fromlen, (struct tsinfo **) to, tolen));
}

/*
**	op_tsinfo() - receive transport-specific data
*/

void op_tsinfo(pp)
register struct ptab *pp;
{
	register struct tsinfo *mp = (struct tsinfo *) pp->ptm_data;
	register struct sockaddr_in *ap;

	/* all reply messages have no data */
	pp->ptm_len = 0;

	/* decide where to store the data;
		allocate a ptab entry for the server if necessary */
	switch (mp->ts_ptype) {
	case PT_SYNCD:
		if (!Sd_ptab) {
			if ((Sd_ptab = ptalloc()) == (struct ptab *) 0) {
				pp->ptm_rc = ER_ERR;
				return;
			}
			Sd_ptab->ptr_sysid = 0;
			Sd_ptab->ptr_ptype = PT_SYNCD;
			Sd_ptab->pt_flags = PF_SERVER;
		}
		ap = &((struct tptab *) Sd_ptab->pt_tdata)->tp_sin;
		break;
	case PT_XRESD:
		if (!Xd_ptab) {
			if ((Xd_ptab = ptalloc()) == (struct ptab *) 0) {
				pp->ptm_rc = ER_ERR;
				return;
			}
			Xd_ptab->ptr_sysid = 0;
			Xd_ptab->ptr_ptype = PT_XRESD;
			Xd_ptab->pt_flags = PF_SERVER;
		}
		ap = &((struct tptab *) Xd_ptab->pt_tdata)->tp_sin;
		break;
	default:
		error(0, "received tsinfo for unexpected ptype",
			ptptype(mp->ts_ptype));
		pp->ptm_rc = ER_ERR;
		return;
	}

	/* store the data */
	ap->sin_family = AF_INET;
	ap->sin_addr.s_addr = htonl(mp->ts_addr);
	ap->sin_port = htons(mp->ts_port);
	TRACE4(Ttccd, 2, "received tsinfo for %s: addr = %s, port = %s",
		ptptype(mp->ts_ptype), inet_ntoa(ap->sin_addr),
		itoa(ntohs(ap->sin_port)));

	/* all ok so set up the reply message and return */
	pp->ptm_rc = ER_OK;
}

