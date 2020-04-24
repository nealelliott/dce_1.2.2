/*
 *      SCCS:  @(#)log.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)log.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)log.c	1.2 92/09/30
NAME:		log.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	tccd error reporting functions

MODIFICATIONS:

************************************************************************/


#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "dtmac.h"
#include "error.h"
#include "tccd.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

static FILE *lfp = stderr;		/* the log file stream pointer */

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#else
extern void exit();
extern time_t time();
#endif

/* static function declarations */
#if __STDC__
static char *loghdr();
static FILE *getlogfp();
#else /* __STDC__ */
static char *loghdr();
static FILE *getlogfp();
#endif /* __STDC__ */


/*
**	loginit() - open the TCCD log file
*/

void loginit()
{
	FILE *fp;
	int n;
	struct stat stbuf;
	static char devnull[] = "/dev/null";
	static char buf[BUFSIZ];
	extern char *Logfile;

	/* make sure that the logfile open will get a fd > 2, otherwise
		all kinds of trouble breaks out when we dup fds later */
	for (n = 0; n < 3; n++)
		if (fstat(n, &stbuf) < 0 && open(devnull, 0) < 0)
			fatal(errno, "can't open", devnull);
	errno = 0;

	/* open the log file */
	if ((fp = fopen(Logfile, "a")) == NULL)
		fatal(errno, "can't open log file", Logfile);

	/* attach stderr to the logfile */
	errno = 0;
	if ((lfp = freopen(Logfile, "a", stderr)) != stderr) {
		lfp = fp;
		fatal(errno, "can't reopen stderr as", Logfile);
	}

	(void) fclose(fp);

	setbuf(lfp, buf);

	if (fappend(fileno(lfp)) < 0)
		exit(1);
}

/*
**	logent() - make an entry in the TCCD log file
*/

void logent(s1, s2)
char *s1, *s2;
{
	register FILE *fp;

	fp = getlogfp();

	(void) fprintf(fp, "%s: %s", loghdr(), s1);
	if (s2 && *s2)
		(void) fprintf(fp, " %s", s2);
	(void) putc('\n', fp);
	(void) fflush(fp);
}

/*
**	logerror() - TCCD error printing routine
*/

void logerror(errnum, file, line, s1, s2)
int errnum, line;
char *file, *s1, *s2;
{
	prerror(getlogfp(), errnum, loghdr(), file, line, s1, s2);
}

/*
**	loghdr() - construct a TCCD error message preamble
**		and return a pointer thereto
*/

static char *loghdr()
{
	static char *month[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	time_t t;
	register struct tm *tp;
	static char header[50];
	extern int Mypid;
	extern char *Progname;

	t = time((time_t *) 0);
	tp = localtime(&t);
	(void) sprintf(header, "%.16s (%d) %d %s %d:%02d:%02d",
		Progname, Mypid, tp->tm_mday, month[tp->tm_mon], tp->tm_hour,
		tp->tm_min, tp->tm_sec);

	return(header);
}

/*
**	getlogfp() - return the logfile stream pointer
*/

static FILE *getlogfp()
{
	struct stat stbuf;
	static FILE *cfp;

	if (!lfp || fstat(fileno(lfp), &stbuf) < 0) {
		if (!cfp)
			cfp = fopen("/dev/console", "w");
		return(cfp);
	}
	else
		return(lfp);
}

