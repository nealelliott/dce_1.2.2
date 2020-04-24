/*
 *      SCCS:  @(#)trace.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)trace.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)trace.c	1.2 92/09/30
NAME:		trace.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	trace subsystem functions

MODIFICATIONS:

************************************************************************/

#ifndef NOTRACE

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include "dtmac.h"
#include "trace.h"
#include "dtmsg.h"
#include "error.h"
#include "ltoa.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

FILE *Tfp = NULL;			/* trace file pointer */

/* the trace flags themselves */
int Tbuf, Tio, Tloop, Tserv, Tsyncd, Ttcc, Ttccd, Ttcm, Ttrace, Txresd;

/* trace flag cmd line identifiers and addresses */
struct tflags Tflags[] = {
	{ 't', &Ttrace,	0, 0 },	/* must be first */
	{ 'b', &Tbuf,	0, 0 },
	{ 'c', &Ttcm,	0, 0 },
	{ 'e', &Tserv,	0, 0 },
	{ 'i', &Tio,	0, 0 },
	{ 'l', &Tloop,	0, 0 },
	{ 'm', &Ttcc,	0, 0 },
	{ 's', &Ttccd,	0, 0 },
	{ 'x', &Txresd,	0, 0 },
	{ 'y', &Tsyncd,	0, 0 }
};
int Ntflags = sizeof Tflags / sizeof Tflags[0];

/* map of trace flag cmd line system names to process types */
struct stype Stype[] = {
	'M', PT_MTCC,
	'S', PT_STCC,
	'C', PT_MTCM,
	'D', PT_STCM,
	'X', PT_XRESD,
	'Y', PT_SYNCD
};
int Nstype = sizeof Stype / sizeof Stype[0];

/* extern fucntion declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#else
extern char *malloc();
#endif

/* static function declarations */
#if __STDC__
static int tflagset(char *, int);
#else
static int tflagset();
#endif


/*
**	traceinit() - scan the argv for -T options and process them
**
**	options can take one of the forms:
**
**		-Txvalue
**		-Tx value
**		-TP,xvalue
**		-TP,x value
**
**	where x is a flag name and value is its value
**	x can be "all" to set all flags to the given value
**	more than one -T option may appear; the effect of -T options accumulate
**	trace options are normally passed to exec'd processes as well
**	if P is present, this restricts the passing of the flag value to
**	processes of type P; more than one P may be present
**
**	a sequence -Tx4 -TP,x2 will cause flag x to have value 4 in the
**	current process and value 2 in process P
**	the sequence -TP,x4 -TQ,x2 will cause flag x to have value 2 in
**	processes P and Q, unless P is the current process in which case its
**	flag x will have value 4
**
**	it is currently not possible to pass different values of the same flag
**	to different processes
**
**	disclaimer: the whole scheme is not perfect (or bug-free), but will
**	probably do what you want if you are persistent enough!
*/

void traceinit(argc, argv)
register int argc;
register char **argv;
{
	register struct tflags *tp;
	register int value;
	register long sys;

	while (++argv, --argc > 0)
		if (**argv == '-' && *(*argv + 1) == 'T') {
			TRACE2(Ttrace, 10, "traceinit: arg = \"%s\"", *argv);
			value = argc > 1 ? atoi(*(argv + 1)) : 0;
			if (tflagset(*argv + 2, value) > 0)
				if (argc > 1) {
					*(argv + 1) = *argv;
					++argv;
					--argc;
				}
		}

	/* avoid exporting trace flag arguments to processes that don't
		need them */
	for (tp = Tflags; tp < &Tflags[Ntflags]; tp++) {
		if (tp->tf_value <= 0) {
			tp->tf_sys = 0;
			continue;
		}
		sys = 0;
		if (tp->tf_vp == &Ttcc)
			TF_SET(sys, PT_MTCC);
		else if (tp->tf_vp == &Ttccd)
			TF_SET(sys, PT_STCC);
		else if (tp->tf_vp == &Ttcm) {
			TF_SET(sys, PT_MTCM);
			TF_SET(sys, PT_STCM);
		}
		else if (tp->tf_vp == &Tsyncd)
			TF_SET(sys, PT_SYNCD);
		else if (tp->tf_vp == &Txresd)
			TF_SET(sys, PT_XRESD);
		else
			continue;
		tp->tf_sys &= sys;
	}

	if (Ttrace > 0)
		tftrace();
}

/*
**	tflagset() - set value and inheretance for a flag
**
**	return 0 if the flag value was part of arg
**	or 1 if value (taken from *(argv + 1)) was used
*/

static int tflagset(arg, value)
char *arg;
int value;
{
	register struct tflags *tp;
	register struct stype *sp;
	register char *p;
	register int all, rc;
	register long sys;
	char buf[2];
	extern int Myptype;

	buf[1] = '\0';

	all = 1;
	for (p = arg; *p; p++)
		if (*p == ',') {
			all = 0;
			break;
		}

	if (all) {
		p = arg;
		sys = ~0;
	}
	else {
		sys = 0;
		for (p = arg; *p != ','; p++)
			for (sp = Stype; sp < &Stype[Nstype]; sp++)
				if (*p == sp->st_name)
					TF_SET(sys, sp->st_ptype);
		p++;
	}

	all = strncmp(p, "all", 3) ? 0 : 1;

	p += (all * 2);

	if (*(p + 1)) {
		value = atoi(p + 1);
		rc = 0;
	}
	else
		rc = 1;

	for (tp = Tflags; tp < &Tflags[Ntflags]; tp++) {
		if (all || tp->tf_name == *p) {
			buf[0] = tp->tf_name;
			tp->tf_sys |= sys;
			if (value > tp->tf_value) {
				TRACE3(Ttrace, 10, "global trace flag %s = %s",
					buf, itoa(value));
				tp->tf_value = value;
			}
			if (TF_ISSET(sys, Myptype) && value > *tp->tf_vp) {
				TRACE3(Ttrace, 10, "local trace flag %s = %s",
					buf, itoa(value));
				*tp->tf_vp = value;
			}
		}
		if (!all && tp->tf_name == *p)
			break;
	}

	if (!all && tp >= &Tflags[Ntflags]) {
		buf[0] = *p;
		error(0, "unknown trace flag name", buf);
	}

	return(rc);
}

/*
**	tftrace() - print trace flag information
*/

void tftrace()
{
	register struct tflags *tp;
	char name[2];

	TRACE1(Ttrace, 10, "trace flags:");
	name[1] = '\0';
	for (tp = Tflags; tp < &Tflags[Ntflags]; tp++) {
		name[0] = tp->tf_name;
		TRACE5(Ttrace, 10, "name = '%s', lvalue = %s, gvalue = %s, sys = %s",
			name, itoa(*tp->tf_vp), itoa(tp->tf_value),
			itox(tp->tf_sys));
	}
}

/*
**	trace() - print trace info to a file, opening it if necessary
*/

void trace(s1, s2, s3, s4, s5, s6)
char *s1, *s2, *s3, *s4, *s5, *s6;
{
	register int save_errno;
	extern char *Progname;

	save_errno = errno;

	if (!Tfp)
		tfopen();

	(void) fprintf(Tfp, "%s (%d): ", Progname, (int) getpid());
	(void) fprintf(Tfp, s1, s2, s3, s4, s5, s6);
	(void) putc('\n', Tfp);
	(void) fflush(Tfp);

	errno = save_errno;
}

/*
**	tfopen() - open the trace file
*/

void tfopen()
{
	register char *p;
	register int fd;

	if (Tfp)
		return;

	if ((fd = fcntl(fileno(stderr), F_DUPFD, 2)) < 0)
		fatal(errno, "can't dup", itoa(fileno(stderr)));

	if (fappend(fd) < 0 || fioclex(fd) < 0)
		fatal(0, "can't continue", (char *) 0);

	if ((Tfp = fdopen(fd, "a")) == NULL)
		fatal(errno, "fdopen failed on fd", itoa(fd));

	errno = 0;
	if ((p = malloc(BUFSIZ)) == (char *) 0)
		fatal(errno, "can't allocate buffer for trace file",
			(char *) 0);
	TRACE2(Tbuf, 6, "allocate trace file stdio buffer = %s",
		itox(p));

	setbuf(Tfp, p);
}

#endif /* !NOTRACE */

