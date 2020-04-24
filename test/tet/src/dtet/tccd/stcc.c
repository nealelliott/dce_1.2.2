/*
 *      SCCS:  @(#)stcc.c	1.3 (92/10/06) 
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
static char sccsid[] = "@(#)stcc.c	1.3 (92/10/06) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)stcc.c	1.3 92/10/06
NAME:		stcc.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	May 1992

DESCRIPTION:
	MTCC request processing routines

MODIFICATIONS:

************************************************************************/


#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "error.h"
#include "ltoa.h"
#include "avmsg.h"
#include "valmsg.h"
#include "servlib.h"
#include "dtetlib.h"
#include "tccd.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <unistd.h>
#include <time.h>
#else
extern unsigned sleep();
extern time_t time();
#endif


/*
**	op_putenv() - put strings in the environment
**
**	strings put in the environment by OP_PUTENV are stored in memory
**	obtained from malloc()
**	if an OP_PUTENV request overwrites an existing environment string that
**	is itself stored in malloc'd memory, there is no provision for freeing
**	that memory
**
**	this is not usually a problem, however, for the typical use of this
**	request to set up an environment to be passed to a STCM via exec()
*/

void op_putenv(pp)
struct ptab *pp;
{
	register struct avmsg *mp = (struct avmsg *) pp->ptm_data;
	register char *p;
	register int n;

	/* all reply messages have no data */
	pp->ptm_len = 0;

	/* do a sanity check on the environment strings */
	for (n = 0; n < OP_PUTENV_NLINE(mp); n++) {
		if ((p = AV_ENVAR(mp, n)) == (char *) 0 || !*p) {
			pp->ptm_rc = ER_INVAL;
			return;
		}
		TRACE2(Ttccd, 4, "receive env string = \"%s\"", p);
		if (!equindex(p)) {
			pp->ptm_rc = ER_INVAL;
			return;
		}
	}

	/* put each string in the environment in turn */
	for (n = 0; n < OP_PUTENV_NLINE(mp); n++)
		if ((p = strstore(AV_ENVAR(mp, n))) == (char *) 0 ||
			dtputenv(p) < 0)
				break;

	pp->ptm_rc = (n == OP_PUTENV_NLINE(mp)) ? ER_OK : ER_ERR;
}

/*
**	op_access() - determine the accessibility of a file wrt euid and egid
*/

void op_access(pp)
struct ptab *pp;
{
	register struct avmsg *mp = (struct avmsg *) pp->ptm_data;

	/* all reply messages have no data */
	pp->ptm_len = 0;

	/* do a sanity check on the request message */
	if (mp->av_argc != OP_ACCESS_ARGC || !AV_PATH(mp) || !*AV_PATH(mp)) {
		pp->ptm_rc = ER_INVAL;
		return;
	}

	TRACE2(Ttccd, 4, "OP_ACCESS: path = \"%s\"", AV_PATH(mp));

	/* check the access permissions */
	if (tet_eaccess(AV_PATH(mp), (int) AV_MODE(mp)) < 0) {
		pp->ptm_rc = maperrno(errno);
		return;
	}

	pp->ptm_rc = ER_OK;
}

/*
**	op_mkdir() - make a directory
*/

void op_mkdir(pp)
struct ptab *pp;
{
	register struct avmsg *mp = (struct avmsg *) pp->ptm_data;

	/* all reply messages have no data */
	pp->ptm_len = 0;

	/* do a sanity check on the request message */
	if (mp->av_argc != OP_DIR_ARGC || !AV_DIR(mp) || !*AV_DIR(mp)) {
		pp->ptm_rc = ER_INVAL;
		return;
	}

	TRACE2(Ttccd, 4, "OP_MKDIR: dir = \"%s\"", AV_DIR(mp));

	/* make the directory */
	if (domkdir(AV_DIR(mp), 0777) < 0) {
		pp->ptm_rc = maperrno(errno);
		return;
	}

	pp->ptm_rc = ER_OK;
}

/*
**	op_rmdir() - remove a directory
*/

void op_rmdir(pp)
struct ptab *pp;
{
	register struct avmsg *mp = (struct avmsg *) pp->ptm_data;

	/* all reply messages have no data */
	pp->ptm_len = 0;

	/* do a sanity check on the request message */
	if (mp->av_argc != OP_DIR_ARGC || !AV_DIR(mp) || !*AV_DIR(mp)) {
		pp->ptm_rc = ER_INVAL;
		return;
	}

	TRACE2(Ttccd, 4, "OP_RMDIR: dir = \"%s\"", AV_DIR(mp));

	/* remove the directory */
	if (dormdir(AV_DIR(mp)) < 0) {
		pp->ptm_rc = maperrno(errno);
		return;
	}

	pp->ptm_rc = ER_OK;
}

/*
**	op_chdir() - change directory
*/

void op_chdir(pp)
struct ptab *pp;
{
	register struct avmsg *mp = (struct avmsg *) pp->ptm_data;
	register int errsave;

	/* all reply messages have no data */
	pp->ptm_len = 0;

	/* do a sanity check on the request message */
	if (mp->av_argc != OP_DIR_ARGC || !AV_DIR(mp) || !*AV_DIR(mp)) {
		pp->ptm_rc = ER_INVAL;
		return;
	}

	/* change directory */
	TRACE2(Ttccd, 4, "OP_CHDIR: chdir to \"%s\"", AV_DIR(mp));
	if (chdir(AV_DIR(mp)) < 0) {
		errsave = errno;
		if ((pp->ptm_rc = maperrno(errsave)) == ER_ERR)
			error(errsave, "can't chdir to", AV_DIR(mp));
		return;
	}

	pp->ptm_rc = ER_OK;
}

/*
**	op_unlink() - unlink a file
*/

void op_unlink(pp)
struct ptab *pp;
{
	register struct avmsg *mp = (struct avmsg *) pp->ptm_data;
	register int errsave;

	/* all reply messages have no data */
	pp->ptm_len = 0;

	/* do a sanity check on the request message */
	if (mp->av_argc != OP_UNLINK_ARGC || !AV_FNAME(mp) || !*AV_FNAME(mp)) {
		pp->ptm_rc = ER_INVAL;
		return;
	}

	TRACE2(Ttccd, 4, "OP_UNLINK: file = \"%s\"", AV_FNAME(mp));

	/* unlink the file */
	if (unlink(AV_FNAME(mp)) < 0) {
		errsave = errno;
		if ((pp->ptm_rc = maperrno(errsave)) == ER_ERR)
			error(errsave, "can't unlink", AV_FNAME(mp));
		return;
	}

	pp->ptm_rc = ER_OK;
}

/*
**	op_lockfile() - create a lock file (exclusive lock)
**
**	a zero timeout means return immediately if the operation fails
**	a -ve timeout means try indefinately until a lock is obtained or an
**	error occurs
*/

void op_lockfile(pp)
struct ptab *pp;
{
	register struct avmsg *mp = (struct avmsg *) pp->ptm_data;
	register int timeout, fd, errsave;
	register char *fname;
	register time_t start;

	/* all reply messages have no data */
	pp->ptm_len = 0;

	/* do a sanity check on the request message */
	fname = AV_FNAME(mp);
	timeout = (int) AV_TIMEOUT(mp);
	if (mp->av_argc != OP_LOCKFILE_ARGC || !fname || !*fname) {
		pp->ptm_rc = ER_INVAL;
		return;
	}

	TRACE3(Ttccd, 4, "OP_LOCKFILE: file = %s, timeout = %s",
		fname, itoa(timeout));

	/* create the lock file, sleeping a bit if it fails */
	start = time((time_t *) 0);
	while ((fd = open(fname, O_RDONLY | O_CREAT | O_EXCL, 0777)) < 0) {
		if ((errsave = errno) != EEXIST || !timeout)
			break;
		if (timeout > 0 && time((time_t *) 0) > start + timeout) {
			pp->ptm_rc = ER_TIMEDOUT;
			return;
		}
		(void) sleep(2);
	}

	/* handle unexpected errors */
	if (fd < 0) {
		if ((pp->ptm_rc = maperrno(errsave)) == ER_ERR)
			error(errsave, "can't create", fname);
		return;
	}

	/* all ok so close the file and return success */
	(void) close(fd);
	pp->ptm_rc = ER_OK;
}

/*
**	op_sharelock() - create a file in a lock directory (shared lock)
**
**	a zero timeout means return immediately if the operation fails
**	a -ve timeout means try indefinately until a lock is obtained or an
**	error occurs
*/

void op_sharelock(pp)
struct ptab *pp;
{
	register struct avmsg *mp = (struct avmsg *) pp->ptm_data;
	register time_t start;
	register int timeout, fd, errsave;
	register char *dir;
	register char salt;
	char pidstr[LNUMSZ];
	static char *fname;
	static int fnamelen;

	/* do a sanity check on the request message */
	dir = AV_DIR(mp);
	timeout = (int) AV_TIMEOUT(mp);
	if (mp->av_argc != OP_SHARELOCK_ARGC || !dir || !*dir) {
		pp->ptm_rc = ER_INVAL;
		pp->ptm_len = 0;
		return;
	}

	TRACE3(Ttccd, 4, "OP_SHARELOCK: dir = %s, timeout = %s",
		dir, itoa(timeout));

	/* format the pid string */
	(void) sprintf(pidstr, "%05lu", pp->ptr_pid);

	/* allocate storage for the lock file name (used in the reply) */
	if (BUFCHK(&fname, &fnamelen, strlen(dir) + strlen(pidstr) + 3) < 0) {
		pp->ptm_rc = ER_ERR;
		pp->ptm_len = 0;
		return;
	}

	/* try to make the lock directory if it does not exist already
		then create a file in the directory;
		if a file already exists whose name is the same as the
		specified lock directory, sleep a bit and try again */
	start = time((time_t *) 0);
	for (;;) {
		if (domkdir(dir, 0777) < 0 && (errsave = errno) != EEXIST) {
			if ((pp->ptm_rc = maperrno(errsave)) == ER_ERR)
				error(errsave, "can't make lock directory",
					dir);
			pp->ptm_len = 0;
			return;
		}
		for (salt = 'a'; salt <= 'z'; salt++) {
			(void) sprintf(fname, "%s/%s%c", dir, pidstr, salt);
			if ((fd = open(fname, O_RDONLY | O_CREAT | O_EXCL, 0777)) >= 0 || errno != EEXIST)
				break;
		}
		if (fd >= 0)
			break;
		else if (salt > 'z') {
			error(0, "out of lock file names:", fname);
			pp->ptm_rc = ER_ERR;
			pp->ptm_len = 0;
			return;
		}
		switch (errsave = errno) {
		case ENOTDIR:
		case ENOENT:
			/* parent dir was a plain file */
			if (timeout)
				break;
			/* else fall through */
		default:
			/* zero timeout or unexpected error */
			if ((pp->ptm_rc = maperrno(errsave)) == ER_ERR ||
				timeout != 0)
					error(errsave, "can't create shared lock file", fname);
			pp->ptm_len = 0;
			return;
		}
		if (timeout > 0 && time((time_t *) 0) > start + timeout) {
			pp->ptm_rc = ER_TIMEDOUT;
			pp->ptm_len = 0;
			return;
		}
		(void) sleep(2);
	}

	/* all ok so close the file and return success */
	(void) close(fd);
	TRACE2(Ttccd, 4, "OP_SHARELOCK: return \"%s\"", fname);
	mp->av_argc = OP_SHARELOCK_ARGC;
	AV_FNAME(mp) = fname;
	pp->ptm_rc = ER_OK;
	pp->ptm_len = avmsgsz(OP_SHARELOCK_ARGC);
}

/*
**	op_mktmpdir() - make a unique temporary directory
*/

void op_mktmpdir(pp)
struct ptab *pp;
{
	register struct avmsg *mp = (struct avmsg *) pp->ptm_data;
	register int salt, needlen, errsave;
	char pidstr[LNUMSZ];
	static char *subdir;
	static int sdlen;
	extern int Mypid;

	/* do a sanity check on the request message */
	if (mp->av_argc != OP_DIR_ARGC || !AV_DIR(mp) || !*AV_DIR(mp)) {
		pp->ptm_rc = ER_INVAL;
		pp->ptm_len = 0;
		return;
	}

	TRACE2(Ttccd, 4, "OP_MKTMPDIR: dir = \"%s\"", AV_DIR(mp));

	/* format the pid string */
	(void) sprintf(pidstr, "%05u", Mypid);

	/* get a buffer for the tmp dir name */
	needlen = strlen(AV_DIR(mp)) + strlen(pidstr) + 3;
	if (BUFCHK(&subdir, &sdlen, needlen) < 0) {
		pp->ptm_rc = ER_ERR;
		pp->ptm_len = 0;
		return;
	}

	/* try to make the directory a few times */
	for (salt = 'a'; salt <= 'z'; salt++) {
		(void) sprintf(subdir, "%s/%s%c", AV_DIR(mp), pidstr, salt);
		TRACE2(Ttccd, 6, "mktmpdir: try \"%s\"", subdir);
		if (domkdir(subdir, 0777) < 0) {
			if ((errsave = errno) != EEXIST) {
				if ((pp->ptm_rc = maperrno(errsave)) == ER_ERR)
					error(errsave, "can't make directory",
						subdir);
				pp->ptm_len = 0;
				return;
			}
		}
		else {
			TRACE2(Ttccd, 4, "OP_MKTMPDIR: return \"%s\"",
				subdir);
			mp->av_argc = OP_DIR_ARGC;
			AV_DIR(mp) = subdir;
			pp->ptm_rc = ER_OK;
			pp->ptm_len = avmsgsz(OP_DIR_ARGC);
			return;
		}
	}

	/* here if we are out of suffix letters */
	error(0, "out of tmp subdir names in", AV_DIR(mp));
	pp->ptm_rc = ER_ERR;
	pp->ptm_len = 0;
}

/*
**	op_rxfile() - transfer a file to the master system
*/

void op_rxfile(pp)
struct ptab *pp;
{
	register struct avmsg *mp = (struct avmsg *) pp->ptm_data;
	extern struct ptab *Xd_ptab;

	/* all reply messages have no data */
	pp->ptm_len = 0;

	/* perform a sanity check on the request message */
	if (mp->av_argc != OP_RXFILE_ARGC ||
		!AV_XFROM(mp) || !*AV_XFROM(mp) ||
		!AV_XTO(mp) || !*AV_XTO(mp)) {
			pp->ptm_rc = ER_INVAL;
			return;
	}

	TRACE3(Ttccd, 4, "OP_RXFILE: from = \"%s\", to = \"%s\"",
		AV_XFROM(mp), AV_XTO(mp));

	/* log on to the xresd if necessary */
	ASSERT(Xd_ptab);
	if ((Xd_ptab->pt_flags & PF_LOGGEDON) == 0 && xd_logon() < 0) {
		pp->ptm_rc = ER_ERR;
		return;
	}

	pp->ptm_rc = (xd_xfile(AV_XFROM(mp), AV_XTO(mp)) < 0) ? ER_ERR : ER_OK;
}

