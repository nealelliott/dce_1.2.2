/*
 *      SCCS:  @(#)exec.c	1.5 (92/12/01) 
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
static char sccsid[] = "@(#)exec.c	1.5 (92/12/01) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)exec.c	1.5 92/12/01
NAME:		exec.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	exec request processing functions

MODIFICATIONS:

************************************************************************/


#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <setjmp.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "sptab.h"
#include "etab.h"
#include "ltoa.h"
#include "error.h"
#include "avmsg.h"
#include "valmsg.h"
#include "synreq.h"
#include "bstring.h"
#include "tslib.h"
#include "servlib.h"
#include "dtetlib.h"
#include "tccd.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#else
extern char *getenv(), *malloc();
extern unsigned alarm();
extern void _exit(), free(), longjmp();
extern time_t time();
#endif

/* static function declarations */
#if __STDC__
static int op_e2(struct ptab *);
static int op_e3(struct ptab *, struct etab *);
static int op_w2(struct ptab *);
static int checkexec(char *);
static int waitforchild(int, int);
static int wfc2(int, int, time_t);
static SIG_FUNC_T catchalarm(int);
static void donasync(struct avmsg *);
#else /* __STDC__ */
static int op_e2();
static int op_e3();
static int op_w2();
static int checkexec();
static int waitforchild();
static int wfc2();
static SIG_FUNC_T catchalarm();
static void donasync();
#endif /* __STDC__ */


/*
**	op_exec() - fork and exec a process
**
**	if the exec failed, a subsequent op_wait will show a process
**	exit status of ~0 (255)
*/

void op_exec(pp)
register struct ptab *pp;
{
	if ((pp->ptm_rc = op_e2(pp)) == ER_OK) {
		((struct valmsg *) pp->ptm_data)->vm_nvalue = OP_EXEC_NVALUE;
		pp->ptm_len = valmsgsz(OP_EXEC_NVALUE);
	}
	else
		pp->ptm_len = 0;
}

/*
**	op_e2() - extend the op_exec() processing
**
**	return ER_OK if successful or other ER_* error code on error
*/

static int op_e2(pp)
register struct ptab *pp;
{
	register struct avmsg *mp = (struct avmsg *) pp->ptm_data;
	register struct sptab *sp = (struct sptab *) pp->pt_sdata;
	register struct etab *ep;
	register int rc;

	/* do a sanity check on the request message */
	if (OP_EXEC_NARG(mp) < 1 || !AV_PATH(mp) || !AV_ARG(mp, 0))
		return(ER_INVAL);

	/* put ti and ts args in the environment for exec */
	if (ti_tcmputenv(pp->ptr_sysid, AV_SNID(mp), AV_XRID(mp), sp->sp_snames, sp->sp_nsname) < 0 || ts_tcmputenv() < 0)
		return(ER_ERR);

	/* get an etab element to hold the exec'd process details */
	if ((ep = etalloc()) == (struct etab *) 0)
		return(ER_ERR);

	if ((rc = op_e3(pp, ep)) == ER_OK)
		etadd(ep);
	else
		etfree(ep);

	return(rc);
}

/*
**	op_e3() - extend the op_exec() processing some more
**
**	return ER_OK if successful or other ER_* error code on error
*/

static int op_e3(pp, ep)
struct ptab *pp;
struct etab *ep;
{
	register char *dp = pp->ptm_data;
	register int pid, n, rc, fd;
	char *dir;
	extern char *Tet_root, *Tet_execute;
#ifndef NOTRACE
	register char **envp;
	extern char **environ;
#endif

#define mp	((struct avmsg *) dp)		/* ptr to request message */

	/* grow the buffer so that there is enough room for a null
		terminating arg - also make sure that it is big enough for
		the reply message */
	n = MAX(avmsgsz(mp->av_argc + 1), valmsgsz(OP_EXEC_NVALUE));
	if (BUFCHK(&pp->ptm_data, &pp->pt_mdlen, n) < 0)
		return(ER_ERR);
	dp = pp->ptm_data;

	/*
	** change directory if so required:
	**	for user execs, cd to TET_EXECUTE if one has been specified,
	**		otherwise to TET_ROOT
	*/
	dir = (char *) 0;
	switch (AV_FLAG(mp)) {
	case AV_EXEC_USER:
		if (Tet_execute && *Tet_execute) {
			TRACE2(Ttccd, 4, "chdir to TET_EXECUTE: \"%s\"",
				Tet_execute);
			dir = Tet_execute;
		}
		else if (Tet_root && *Tet_root) {
			TRACE2(Ttccd, 4, "chdir to TET_ROOT: \"%s\"",
				Tet_root);
			dir = Tet_root;
		}
		break;
	}
	if (dir && chdir(dir) < 0)
		error(errno, "can't chdir to", dir);

	/* see if an exec has any chance of succeeding */
	if (checkexec(AV_PATH(mp)) < 0) {
		TRACE3(Ttccd, 4, "checkexec(\"%s\") failed, errno = %s",
			AV_PATH(mp), errname(errno));
		return(ER_NOENT);
	}

	/* open the output file if one has been specified */
	if (AV_OUTFILE(mp) && *AV_OUTFILE(mp)) {
		TRACE2(Ttccd, 4, "send output to \"%s\"", AV_OUTFILE(mp));
		if ((fd = open(AV_OUTFILE(mp), O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
			error(errno, "can't open", AV_OUTFILE(mp));
			return(ER_ERR);
		}
		else if (fioclex(fd) < 0)
			return(ER_ERR);
	}
	else
		fd = -1;

	/* add a null-terminating arg (just to make sure) */
	AV_ARG(mp, OP_EXEC_NARG(mp)) = (char *) 0;

#ifndef NOTRACE
	if (Ttccd > 0) {
		TRACE2(Ttccd, 4, "exec \"%s\"", AV_PATH(mp));
		for (n = 0; AV_ARG(mp, n); n++)
			TRACE3(Ttccd, 6, "arg %s = \"%s\"",
				itoa(n), AV_ARG(mp, n));
		for (envp = environ; *envp; envp++)
			TRACE2(Ttccd, 8, "env = \"%s\"", *envp);
	}
#endif

	/* do the fork and exec -
		in the child process:
		if outfile was specified, attach stdout and stderr to outfile
		the original outfile fd is already close-on-exec
		stdin is attached to /dev/null
		close all other file descriptors
		the "can't exec" message goes to outfile if one is specified */
	if ((pid = dofork()) == 0) {
		/* in child */
		if (fd >= 0) {
			(void) fflush(stdout);
			(void) close(1);
			if (fcntl(fd, F_DUPFD, 1) != 1) {
				error(errno, "can't dup stdout", (char *) 0);
				_exit(~0);
			}
			(void) fflush(stderr);
			(void) close(2);
			if (fcntl(fd, F_DUPFD, 2) != 2) {
				/* can't print an error message :-{ */
				_exit(~0 - 1);
			}
		}
		for (n = dogetdtablesize() - 1; n > 2; n--) {
			if (n != fd)
				(void) close(n);
		}
		(void) execvp(AV_PATH(mp), &AV_ARG(mp, 0));
		error(errno, "can't exec", AV_PATH(mp));
		switch (AV_FLAG(mp)) {
		case AV_EXEC_TEST:
		case AV_EXEC_USER:
			donasync(mp);
			break;
		}
		_exit(~0);
		/* NOTREACHED */
	}
	else if (pid < 0) {
		error(errno, "fork failed: path =", AV_PATH(mp));
		rc = ER_FORK;
	}
	else {
		/* in parent */
		ep->et_ptab = pp;
		ep->et_pid = pid;
		ep->et_state = ES_RUNNING;
		rc = ER_OK;
	}

	/* close outfile in the parent if one was specified */
	if (fd >= 0)
		(void) close(fd);

#undef mp

	TRACE3(Ttccd, 4, "after exec: pid = %s, return %s",
		itoa(pid), ptrepcode(rc));

#define rp	((struct valmsg *) dp)		/* ptr to reply message */

	VM_PID(rp) = (long) pid;
	return(rc);

#undef rp

}

/*
**	op_wait() - wait for a process
**
**	some possible message returns are:
**		-ve timeout:				rc = ER_INVAL
**		wait timed out:				rc = ER_TIMEDOUT
**		timeout = 0 and proc not terminated:	rc = ER_WAIT
**		proc not started by OP_EXEC:		rc = ER_PID
**		ok with status in VM_STATUS:		rc = ER_OK
*/

void op_wait(pp)
register struct ptab *pp;
{
	if ((pp->ptm_rc = op_w2(pp)) == ER_OK) {
		((struct valmsg *) pp->ptm_data)->vm_nvalue = OP_WAIT_NVALUE;
		pp->ptm_len = valmsgsz(OP_WAIT_NVALUE);
	}
	else
		pp->ptm_len = 0;
}

/*
**	op_w2() - extend the op_wait() processing
**
**	return ER_OK if successful or other ER_* error code on error
*/

static int op_w2(pp)
register struct ptab *pp;
{
	register struct valmsg *mp = (struct valmsg *) pp->ptm_data;
	register struct etab *ep;
	register int rc;

	TRACE3(Ttccd, 4, "op_wait: pid = %s, timeout = %s",
		ltoa(VM_PID(mp)), ltoa(VM_WTIMEOUT(mp)));

	/* do a sanity check on the request message */
	if ((int) mp->vm_nvalue != OP_WAIT_NVALUE)
		return(ER_INVAL);

	/* make sure that the buffer is big enough for the reply */
	if (BUFCHK(&pp->ptm_data, &pp->pt_mdlen, valmsgsz(OP_WAIT_NVALUE)) < 0)
		return(ER_ERR);
	mp = (struct valmsg *) pp->ptm_data;

	/* find exec table entry for this process */
	if ((ep = etfind((int) VM_PID(mp))) == (struct etab *) 0 ||
		ep->et_ptab != pp) {
			TRACE1(Ttccd, 4, "can't find proc in exec table");
			return(ER_PID);
	}

	/* wait for the process to terminate if necessary */
	if (ep->et_state != ES_TERMINATED) {
		rc = waitforchild((int) VM_PID(mp), (int) VM_WTIMEOUT(mp));
		TRACE2(Ttccd, 4, "waitforchild() returned %s", ptrepcode(rc));
	}
	else {
		rc = ER_OK;
		TRACE1(Ttccd, 4, "proc already terminated");
	}

	/* handle return code */
	switch (rc) {
	case ER_WAIT:
	case ER_TIMEDOUT:
		break;
	case ER_OK:
		VM_STATUS(mp) = (long) ((unsigned) ep->et_status);
		TRACE3(Ttccd, 4, "op_wait: return status = %s, signal %s",
			ltoa((VM_STATUS(mp) >> 8) & 0xff),
			ltoa(VM_STATUS(mp) & 0xff));
		/* fall through */
	/* case ER_PID: */
	default:
		etrm(ep);
		etfree(ep);
	}

	TRACE2(Ttccd, 4, "op_w2() returning %s", ptrepcode(rc));
	return(rc);
}

/*
**	op_kill() - send signal to process
**
*/

void op_kill(pp)
register struct ptab *pp;
{
	register struct valmsg *mp = (struct valmsg *) pp->ptm_data;
	register struct etab *ep;
	register int signum, rc;

	TRACE3(Ttccd, 4, "op_kill: pid = %s, signum = %s",
		ltoa(VM_PID(mp)), ltoa(VM_SIGNUM(mp)));

	/* all reply messages contain no data */
	pp->ptm_len = 0;

	/* do a sanity check on the request message */
	if ((int) mp->vm_nvalue != OP_KILL_NVALUE) {
		pp->ptm_rc = ER_INVAL;
		return;
	}

	/* make sure that the process to be killed was started by
		the requester */
	if ((ep = etfind((int) VM_PID(mp))) == (struct etab *) 0 ||
		ep->et_ptab != pp) {
			TRACE1(Ttccd, 4, "can't find proc in exec table");
			pp->ptm_rc = ER_PID;
			return;
	}

	/* map the machine-independent signal number to its local equivalent */
	if ((signum = unmapsignal((int) VM_SIGNUM(mp))) < 0) {
		pp->ptm_rc = ER_SIGNUM;
		return;
	}

#ifndef NOTRACE
	if (signum != (int) VM_SIGNUM(mp))
		TRACE3(Ttccd, 4, "unmapsig(%s) returns signal %s",
			ltoa(VM_SIGNUM(mp)), itoa(signum));
#endif

	/* do the kill */
	if (kill((int) VM_PID(mp), signum) < 0)
		switch (errno) {
		case EINVAL:
			rc = ER_SIGNUM;
			break;
		case ESRCH:
			rc = ER_PID;
			break;
		case EPERM:
			rc = ER_PERM;
			break;
		default:
			error(errno, "kill failed on pid", ltoa(VM_PID(mp)));
			rc = ER_ERR;
			break;
		}
	else
		rc = ER_OK;

	pp->ptm_rc = rc;
}

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

	sp->sp_snames = (int *) 0;
	sp->sp_nsname = 0;

	pp->pt_sdata = (char *) sp;

	return(0);
}

/*
**	ss_ptfree() - free server-specific data element in a ptab structure
*/

void ss_ptfree(pp)
struct ptab *pp;
{
	register struct sptab *sp = (struct sptab *) pp->pt_sdata;

	TRACE2(Tbuf, 6, "free sptab = %s", itox(sp));

	if (sp) {
		if (sp->sp_snames) {
			TRACE2(Tbuf, 6, "free snames = %s",
				itox(sp->sp_snames));
			free((char *) sp->sp_snames);
		}
		free((char *) sp);
		pp->pt_sdata = (char *) 0;
	}
}

/*
**	checkexec() - see if an exec has any chance of succeeding
**
**	return 0 if successful or -1 on error
*/

static int checkexec(file)
char *file;
{
	register char *p1, *p2;
	register char *path;
	char fname[MAXPATH];

	/* see if there is a / in the file name */
	for (p1 = file; *p1; p1++)
		if (*p1 == '/')
			break;

	/* if there is or there is no PATH, just check the file name */
	if (*p1 || (path = getenv("PATH")) == (char *) 0 || !*path) {
		TRACE2(Ttccd, 6, "checkexec: try \"%s\"", file);
		return(tet_eaccess(file, 01));
	}

	/* otherwise, try prepending each component of the PATH environment
		variable */
	p1 = path;
	p2 = fname;
	do {
		if (!*p1 || *p1 == ':') {
			if (p2 > fname && p2 < &fname[sizeof fname - 2])
				*p2++ = '/';
			*p2 = '\0';
			(void) sprintf(p2, "%.*s",
				&fname[sizeof fname - 1] - p2, file);
			TRACE2(Ttccd, 6, "checkexec: try \"%s\"", fname);
			if (tet_eaccess(fname, 01) == 0)
				return(0);
			p2 = fname;
		}
		else if (p2 < &fname[sizeof fname - 2])
			*p2++ = *p1;
	} while (*p1++);

	/* here if not found anywhere */
	return(-1);
}

/*
**	waitforchild() - wait for child processes to termitate, or until
**		timeout expires
**
**	return ER_OK if successful or other ER_* error code on error
*/

static jmp_buf wait_env;
static int wait_timedout;

static int waitforchild(pid, timeout)
int pid;
register int timeout;
{
	register int rc;
	register SIG_T (*sig_save)();
	register unsigned alarm_save;
	register time_t interval, start;
	SIG_FUNC_T catchalarm();

	/* remember current time */
	start = time((time_t *) 0);

	/* remember alarm state and signal disposition */
	if (timeout > 0) {
		alarm_save = alarm(0);
		sig_save = signal(SIGALRM, catchalarm);
	}

	/* mark current place for timeout processing */
	wait_timedout = 0;
	rc = setjmp(wait_env) ? ER_TIMEDOUT : wfc2(pid, timeout, start);

	/* restore previous alarm and signal disposition,
		allowing for elapsed time spent in wfc2() */
	if (timeout > 0) {
		if (alarm_save != 0) {
			interval = time((time_t *) 0) - start;
			if ((unsigned) interval < alarm_save - 1)
				alarm_save -= (unsigned) interval;
			else
				alarm_save = 2;
		}
		(void) alarm(alarm_save);
		(void) signal(SIGALRM, (SIG_FUNC_T (*)()) sig_save);
	}

	return(rc);
}

/*
**	wfc2() - extend the waitforchild() processing
**
**	return ER_OK if successful or other ER_* error code on error
**	or if the timeout expires
*/

static int wfc2(pid, timeout, start)
int pid;
register int timeout;
time_t start;
{
	register struct etab *ep;
	register int rc, tleft, save_errno;
	int status;

	/* reap all terminated children until pid is found, storing
		exit status in the exec table */
	tleft = timeout > 0 ? timeout : 0;
	for (;;) {
		status = 0;
		if (timeout > 0)
			(void) alarm((unsigned) MAX(tleft, 2));
		rc = dowait3(&status, timeout ? 0 : WNOHANG);
		save_errno = errno;
		if (timeout > 0)
			(void) alarm(0);
		TRACE4(Ttccd, 4,
			"wait3 returned pid = %s, status = %s, signal %s",
			itoa(rc), itoa((status >> 8) & 0xff),
			itoa(status & 0xff));
		if (!rc)
			return(ER_WAIT);
		else if (rc < 0)
			switch (save_errno) {
			case ECHILD:
				return(ER_PID);
			case EINTR:
				if (wait_timedout) {
					wait_timedout = 0;
					return(ER_TIMEDOUT);
				}
				else if ((rc = maperrno(save_errno)) != ER_ERR)
					return(rc);
				/* else fall through */
			default:
				error(save_errno, "wait failed for pid",
					itoa(pid));
				return(ER_ERR);
			}
		if ((ep = etfind(rc)) != (struct etab *) 0) {
			ep->et_state = ES_TERMINATED;
			ep->et_status = status & 0xffff;
		}
		if (rc == pid)
			return(ER_OK);
		if (timeout > 0 && (tleft -= time((time_t *) 0) - start) < 1)
			return(ER_TIMEDOUT);
	}
}

/*
**	catchalarm() - SIGALRM signal handler
*/

/* ARGSUSED */
static SIG_FUNC_T catchalarm(sig)
int sig;
{
	(void) signal(SIGALRM, SIG_IGN);
	wait_timedout = 1;
	longjmp(wait_env, 1);
}

/*
**	donasync() - do a NO auto-sync from the child if an exec fails
*/

static void donasync(mp)
struct avmsg *mp;
{
	if (sd_logon() < 0)
		return;

	if (sd_sasync(AV_SNID(mp), SV_EXEC_SPNO, SV_NO, SV_EXEC_TIMEOUT, (struct synreq *) 0, (int *) 0, (long *) 0) < 0)
		error(0, "after failed exec, autosync(NO) failed:",
			ptrepcode(sd_errno));

	(void) sd_logoff(0);
}

