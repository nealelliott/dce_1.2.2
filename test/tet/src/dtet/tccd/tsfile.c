/*
 *      SCCS:  @(#)tsfile.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tsfile.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tsfile.c	1.2 92/09/30
NAME:		tsfile.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	June 1992

DESCRIPTION:
	transfer save files functions

MODIFICATIONS:

************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "avmsg.h"
#include "error.h"
#include "ltoa.h"
#include "servlib.h"
#include "dtetlib.h"
#include "tccd.h"

#ifdef NDIR
#include <sys/ndir.h>
#else
#ifdef DIRENT
#include <dirent.h>
#define direct	dirent
#else
#ifdef SYSDIR
#include <sys/dir.h>
#else
one of these must be defined in dtmac.h !!
#endif /* SYSDIR */
#endif /* DIRENT */
#endif /* NDIR */

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

static char *sfdir;			/* saved files directory */

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#else
extern void free();
#endif

/* static function declarations */
#if __STDC__
static int copydir(char *, char *, int);
static int copyfile(char *, char *, struct stat *, int);
static char **lsdir(char *);
static int procdir(char *, char *, char **, int, int);
static int procfile(char *, char *, char *, char **, int, int);
static int tsave(char *, char *, int);
static int tsc2(int, char *, char *);
static int tscopy(char *, char *);
#else /* __STDC__ */
static int copydir();
static int copyfile();
static char **lsdir();
static int procdir();
static int procfile();
static int tsave();
static int tsc2();
static int tscopy();
#endif /* __STDC__ */


/*
**	op_mksdir() - make a new save files directory
*/

void op_mksdir(pp)
register struct ptab *pp;
{
	register struct avmsg *mp = (struct avmsg *) pp->ptm_data;
	register int n, nmax, rc;
	register struct direct *dp;
	DIR *dirp;
	char savdir[MAXPATH + 1], lokdir[sizeof savdir];
	char dir[sizeof savdir - 8]; /* the 8 is strlen("/NNNNbec") */
	static char errmsg[] = "can't make directory";

	/* do a sanity check on the request */
	if (mp->av_argc != OP_MKSDIR_ARGC ||
		!AV_DIR(mp) || !*AV_DIR(mp) ||
		!AV_SUFFIX(mp) || !*AV_SUFFIX(mp)) {
			pp->ptm_rc = ER_INVAL;
			pp->ptm_len = 0;
			return;
	}

	TRACE3(Ttccd, 4, "OP_MKSDIR: dir = \"%s\", suffix = \"%s\"",
		AV_DIR(mp), AV_SUFFIX(mp));

	/* construct the specified directory path name */
	(void) sprintf(dir, "%.*s", sizeof dir - 1, AV_DIR(mp));

	/* open the directory */
	if ((dirp = opendir(dir)) == (DIR *) 0) {
		error(errno, "can't open directory", dir);
		pp->ptm_rc = ER_NOENT;
		pp->ptm_len = 0;
		return;
	}

	/* read each slot in turn, looking for the highest numeric prefix */
	nmax = 0;
	while ((dp = readdir(dirp)) != (struct direct *) 0)
		if ((n = atoi(dp->d_name)) > nmax)
			nmax = n;
	(void) closedir(dirp);

	/* make the directory first without the suffix (try a few times) */
	for (n = 0; n < 5; n++) {
		if (++nmax > 9999) {
			error(0, "out of subdirectory sequence numbers in",
				dir);
			pp->ptm_rc = ER_ERR;
			pp->ptm_len = 0;
			return;
		}
		(void) sprintf(lokdir, "%s/%04d", dir, nmax);
		TRACE2(Ttccd, 6, "mksdir: try lokdir = \"%s\"", lokdir);
		if (domkdir(lokdir, 0777) == 0 || errno != EEXIST)
			break;
	}
	if (n >= 5) {
		error(errno, errmsg, lokdir);
		pp->ptm_rc = ER_ERR;
		pp->ptm_len = 0;
		return;
	}

	/* then make the directory with the suffix
		and remove the other one */
	(void) sprintf(savdir, "%s%.3s", lokdir, AV_SUFFIX(mp));
	TRACE2(Ttccd, 6, "mksdir: make savdir = \"%s\"", savdir);
	if ((rc = domkdir(savdir, 0777)) < 0)
		error(errno, errmsg, savdir);
	(void) dormdir(lokdir);
	if (rc < 0) {
		pp->ptm_rc = ER_ERR;
		pp->ptm_len = 0;
		return;
	}

	/* remember the savedir name for later */
	if (sfdir) {
		TRACE2(Tbuf, 6, "free sfdir = %s", itox(sfdir));
		free(sfdir);
	}
	if ((sfdir = strstore(savdir)) == (char *) 0) {
		(void) dormdir(savdir);
		pp->ptm_rc = ER_ERR;
		pp->ptm_len = 0;
		return;
	}

	/* all ok so return success */
	mp->av_argc = OP_MKSDIR_ARGC;
	AV_DIR(mp) = sfdir;
	AV_SUFFIX(mp) = (char *) 0;
	pp->ptm_rc = ER_OK;
	pp->ptm_len = avmsgsz(OP_MKSDIR_ARGC);
}

/*
**	op_tsfiles() - perform a transfer save files operation
*/

void op_tsfiles(pp)
struct ptab *pp;
{
	register struct avmsg *mp = (struct avmsg *) pp->ptm_data;

	/* all reply messages have no data */
	pp->ptm_len = 0;

	/* do a sanity check on the request */
	if (OP_TSFILES_NFILES(mp) == 0) {
		pp->ptm_rc = ER_INVAL;
		return;
	}

#ifndef NOTRACE
	if (Ttccd > 0) {
		register int n;
		TRACE3(Ttccd, 4, "OP_TSFILES: nfiles = %s, flag = %s",
			itoa(OP_TSFILES_NFILES(mp)), ltoa(AV_FLAG(mp)));
		for (n = 0; n < OP_TSFILES_NFILES(mp); n++)
			TRACE2(Ttccd, 4, "file = \"%s\"", AV_TSFILE(mp, n));
	}
#endif

	/* for a local transfer,
		make sure that we have a save file directory */
	switch (AV_FLAG(mp)) {
	case AV_TS_LOCAL:
		if (!sfdir || !*sfdir) {
			pp->ptm_rc = ER_CONTEXT;
			return;
		}
		break;
	case AV_TS_MASTER:
		break;
	default:
		pp->ptm_rc = ER_INVAL;
		return;
	}

	/* process each save file (or directory) in turn */
	pp->ptm_rc = procdir(".", AV_SUBDIR(mp), &AV_TSFILE(mp, 0), OP_TSFILES_NFILES(mp), (int) AV_FLAG(mp));
}

/*
**	procdir() - search a directory for transfer save files and
**		process them
**
**	return ER_OK if successful or other ER_* error code on error
*/

static int procdir(fromdir, todir, sfiles, nsfile, flag)
char *fromdir, *todir, *sfiles[];
int nsfile, flag;
{
	register char **fip, **fromfiles;
	register int rc, rctmp;

	TRACE2(Ttccd, 8, "procdir(): fromdir = \"%s\"", fromdir);

	/* get a directory list */
	if ((fromfiles = lsdir(fromdir)) == (char **) 0)
		return(ER_ERR);

	/* process each entry in turn */
	rc = ER_OK;
	for (fip = fromfiles; *fip; fip++) {
		if ((rctmp = procfile(fromdir, todir, *fip, sfiles, nsfile, flag)) != ER_OK)
			rc = rctmp;
		TRACE2(Tbuf, 6, "free file name = %s", itox(*fip));
		free(*fip);
	}

	TRACE2(Tbuf, 6, "free file list = %s", itox(*fip));
	free((char *) fromfiles);
	return(rc);
}

/*
**	procfile() - see if this file name is in the list of save files
**		and save it if it is
**
**	return ER_OK if successful or other ER_* error code on error
*/

static int procfile(fromdir, todir, fromfile, sfiles, nsfile, flag)
register char *fromdir, *fromfile;
char *todir;
register char *sfiles[];
register int nsfile;
int flag;
{
	register int n;
	struct stat stbuf;
	char path[MAXPATH + 1];

	TRACE3(Ttccd, 8, "procfile(): fromdir = \"%s\", fromfile = \"%s\"",
		fromdir, fromfile);

	/* strip off an initial "./" from dir */
	if (*fromdir == '.' && *(fromdir + 1) == '/')
		fromdir += 2;

	/* construct the path name to this file */
	if (!*fromdir || (*fromdir == '.' && !*(fromdir + 1)))
		(void) sprintf(path, "%.*s", sizeof path - 1, fromfile);
	else
		(void) sprintf(path, "%.*s/%.*s",
			sizeof path - 2, fromdir,
			sizeof path - strlen(fromdir) - 2, fromfile);

	TRACE2(Ttccd, 9, "procfile(): path name = \"%s\"", path);

	/* get the file stats */
	if (stat(path, &stbuf) < 0) {
		error(errno, "warning: can't stat", path);
		return(ER_OK);
	}

	/* see if the file is in the save files list */
	for (n = 0; n < nsfile; n++)
		if (!strcmp(fromfile, sfiles[n]))
			return(copyfile(path, todir, &stbuf, flag));

	/* not in the list - if the file is a directory, search that as well */
	if ((stbuf.st_mode & S_IFMT) == S_IFDIR &&
		!(*fromfile == '.' && !*(fromfile + 1)))
			return(procdir(path, todir, sfiles, nsfile, flag));

	return(ER_OK);
}

/*
**	copyfile() - process a transfer save file (or directory)
**
**	return ER_OK if successful or other ER_* error code on error
*/

static int copyfile(fromfile, todir, stp, flag)
char *fromfile, *todir;
struct stat *stp;
int flag;
{
	register int mode;

	TRACE2(Ttccd, 8, "copyfile(): fromfile = \"%s\"", fromfile);

	/* see if it is a file or a directory and process it accordingly */
	switch (mode = (stp->st_mode & S_IFMT)) {
	case S_IFDIR:
		return(copydir(fromfile, todir, flag));
	case S_IFREG:
		return(tsave(fromfile, todir, flag));
	default:
		error(0, "ignored save file", fromfile);
		error(0, "type", itoo(mode));
		return(ER_OK);
	}
}

/*
**	copydir() - process a transfer save file that is a directory
**
**	return ER_OK if successful or other ER_* error code on error
*/

static int copydir(fromdir, todir, flag)
char *fromdir, *todir;
int flag;
{
	register char **fip, **fromfiles;
	register int rc, rctmp;
	struct stat stbuf;
	char path[MAXPATH + 1];

	TRACE2(Ttccd, 8, "copydir(): fromdir = \"%s\"", fromdir);

	/* get a directory list */
	if ((fromfiles = lsdir(fromdir)) == (char **) 0)
		return(ER_ERR);

	/* process each file name in turn */
	rc = ER_OK;
	for (fip = fromfiles; *fip; fip++) {
		if (**fip == '.' && !*(*fip + 1))
			continue;
		(void) sprintf(path, "%.*s/%.*s",
			sizeof path - 2, fromdir,
			sizeof path - strlen(fromdir) - 2, *fip);
		if (stat(path, &stbuf) < 0) {
			error(errno, "warning: can't stat", path);
			continue;
		}
		if ((rctmp = copyfile(path, todir, &stbuf, flag)) != ER_OK)
			rc = rctmp;
		TRACE2(Tbuf, 6, "free tsfile name = %s", itox(*fip));
		free(*fip);
	}

	TRACE2(Tbuf, 6, "free tsfile list = %s", itox(*fip));
	free((char *) fromfiles);
	return(rc);
}

/*
**	lsdir() - return a pointer to a list of pointers to directory entries
**
**	return (char **) 0 on error
**
**	the return list excludes ".." but includes "."
*/

static char **lsdir(dir)
char *dir;
{
	register DIR *dirp;
	register struct direct *dp;
	register int n, nfiles;
	register char **fip;
	char **files = (char **) 0;
	int flen = 0;
	char name[MAXNAMLEN + 1];

	/* open the directory */
	if ((dirp = opendir(dir)) == (DIR *) 0) {
		error(errno, "can't open", dir);
		return((char **) 0);
	}

	/* count the files in the directory and store their names */
	nfiles = 0;
	while ((dp = readdir(dirp)) != (struct direct *) 0) {
		(void) sprintf(name, "%.*s", sizeof name - 1, dp->d_name);
		if (name[0] == '.' && name[1] == '.' && !name[2])
			continue;
		n = (nfiles + 2) * sizeof *files;
		if (BUFCHK((char **) &files, &flen, n) < 0) {
			break;
		}
		fip = files + nfiles;
		if ((*fip = strstore(name)) == (char *) 0) {
			break;
		}
		*++fip = (char *) 0;
		nfiles++;
	}
	(void) closedir(dirp);

	return(files);
}

/*
**	tsave() - process a single transfer save file
**
**	return ER_OK if successful or other ER_* error code on error
*/

static int tsave(fromfile, todir, flag)
char *fromfile, *todir;
int flag;
{
	char tofile[MAXPATH + 1];
	extern struct ptab *Xd_ptab;

	TRACE3(Ttccd, 8, "tsave(): fromfile = \"%s\", flag = %s",
		fromfile, itoa(flag));

	switch (flag) {
	case AV_TS_LOCAL:
		return(tscopy(fromfile, todir));
	case AV_TS_MASTER:
		ASSERT(Xd_ptab);
		if ((Xd_ptab->pt_flags & PF_LOGGEDON) == 0 && xd_logon() < 0)
			return(ER_ERR);
		(void) sprintf(tofile, "%.*s/%.*s",
			sizeof tofile - 2, todir,
			sizeof tofile - strlen(todir) - 2, fromfile);
		TRACE3(Ttccd, 8, "call xd_rxfile: from = \"%s\", to = \"%s\"",
			fromfile, tofile);
		if (xd_xfile(fromfile, tofile) < 0) {
			error(0, "transfer save file failed, rc =",
				ptrepcode(xd_errno));
			return(ER_ERR);
		}
		return(ER_OK);
	default:
		error(0, "unexpected flag value:", itoa(flag));
		return(ER_ERR);
	}
}

/*
**	tscopy() - copy a save file on the local system
**
**	return ER_OK if successful or other ER_* error code on error
*/

static int tscopy(fromfile, todir)
char *fromfile, *todir;
{
	register char *p;
	register int ifd, rc;
	char tofile[MAXPATH + 1];
	static char fmt[] = "/%.*s";

	/* construct the destination file name */
	p = tofile;
	(void) sprintf(p, &fmt[1], &tofile[sizeof tofile - 2] - p, sfdir);
	p += strlen(p);
	if (todir && *todir) {
		(void) sprintf(p, fmt, &tofile[sizeof tofile - 2] - p, todir);
		p += strlen(p);
	}
	(void) sprintf(p, fmt, &tofile[sizeof tofile - 1] - p, fromfile);
	p += strlen(p);

	TRACE3(Ttccd, 8, "tscopy(): fromfile = \"%s\", tofile = \"%s\"",
		fromfile, tofile);

	/* make any required directories for the destination file */
	while (--p >= tofile)
		if (*p == '/')
			break;
	if (p > tofile) {
		*p = '\0';
		if (mkalldirs(tofile) < 0)
			return(ER_ERR);
		*p = '/';
	}

	/* open the source file, copy it if successful */
	if ((ifd = open(fromfile, O_RDONLY)) < 0) {
		error(errno, "can't open", fromfile);
		rc = ER_ERR;
	}
	else {
		rc = tsc2(ifd, fromfile, tofile);
		(void) close(ifd);
	}

	return(rc);
}

/*
**	tsc2() - extend the tscopy() processing
**
**	return ER_OK if successful or other ER_* error code on error
*/

static int tsc2(ifd, fromfile, tofile)
int ifd;
char *fromfile, *tofile;
{
	register int n, ofd, rc;
	char buf[BUFSIZ];
	struct stat stbuf;

	/* open the destination file */
	if ((ofd = open(tofile, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
		error(errno, "can't open", tofile);
		return(ER_ERR);
	}

	/* copy the file */
	rc = ER_OK;
	while ((n = read(ifd, buf, sizeof buf)) > 0)
		if (write(ofd, buf, (unsigned) n) != n) {
			error(errno, "write error on", tofile);
			rc = ER_ERR;
			break;
		}
	if (n < 0) {
		error(errno, "read error on", fromfile);
		rc = ER_ERR;
	}

	/* set the mode on the destination file */
	if (fstat(ifd, &stbuf) < 0)
		error(errno, "warning: can't stat", fromfile);
	else if (chmod(tofile, (int) (stbuf.st_mode & ~S_IFMT)) < 0)
		error(errno, "warning: can't chmod", tofile);

	(void) close(ofd);
	return(rc);
}

