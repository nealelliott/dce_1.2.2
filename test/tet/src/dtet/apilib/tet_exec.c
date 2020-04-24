/*
 * Copyright 1990 Open Software Foundation (OSF)
 * Copyright 1990 Unix International (UI)
 * Copyright 1990 X/Open Company Limited (X/Open)
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of OSF, UI or X/Open not be used in 
 * advertising or publicity pertaining to distribution of the software 
 * without specific, written prior permission.  OSF, UI and X/Open make 
 * no representations about the suitability of this software for any purpose.  
 * It is provided "as is" without express or implied warranty.
 *
 * OSF, UI and X/Open DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, 
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO 
 * EVENT SHALL OSF, UI or X/Open BE LIABLE FOR ANY SPECIAL, INDIRECT OR 
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF 
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR 
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR 
 * PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef lint
static char sccsid[] = "@(#)tet_exec.c	1.3 (92/10/06) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tet_exec.c	1.3 92/10/06
NAME:		'C' API exec process function
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Geoff Clare, UniSoft Ltd.
DATE CREATED:	31 July 1990
SYNOPSIS:

	int tet_exec(const char *file, char *const argv[],
	 	     char *const envp[]);

DESCRIPTION:

	Tet_exec() is used to execute the specified file in the same
	way as the POSIX.1 execve() interface except that it passes
	extra information on the command line and in the environment for
	use by a main() provided with the API.  This then calls the
	user-supplied tet_main() routine with the argv[] which was
	passed to tet_exec().

	Tet_exec() does not return if successful, but may return -1 if
	either execve() or malloc() fails.

MODIFICATIONS:

	June 1992
	This file is derived from TET release 1.10

	Andrew Dingwall, UniSoft Ltd., October 1992
	Modified to work with DTET tcmchild processes.

************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "dtet/tet_api.h"
#include "servlib.h"
#include "dtetlib.h"
#include "error.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;
#endif

#define TET_CONFIG	"TET_CONFIG"
#define TET_RESFD	"TET_RESFD"
#define TET_TMPRESFD	"TET_TMPRESFD"

extern long	tet_activity;
extern long	tet_context;
extern long	tet_block;

static char	buf[4][25];

int
tet_exec(file, argv, envp)
char *file;
char *argv[];
char *envp[];
{
	char **newargv, **newenvp, *cp, *tiargs_env, *tsargs_env;
	register char **ep;
	int cnt, config_ok, resfd_ok, tmpfd_ok, tiargs_ok, tsargs_ok;
	static char tiargs_name[] = "TET_TIARGS";
	static char tsargs_name[] = "TET_TSARGS";
	extern char **environ;
	extern int Mysysid, *Snames, Nsname;
	extern long Snid, Xrid;

	/* allocate new argv array, with room for four extra args plus
	   NULL terminator */

	for (cnt = 0; argv[cnt] != NULL; cnt++)
		;
	cnt += 5;
	newargv = (char **) malloc((size_t)(cnt * sizeof(char *)));
	if (newargv == NULL) {
		errno = ENOMEM;
		return -1;
	}

	/* first 4 args are TP number, current activity, context and block */

	(void) sprintf(buf[0], "%d", tet_thistest);
	(void) sprintf(buf[1], "%ld", tet_activity);
	(void) sprintf(buf[2], "%ld", tet_context);
	(void) sprintf(buf[3], "%ld", tet_block);
	newargv[0] = buf[0];
	newargv[1] = buf[1];
	newargv[2] = buf[2];
	newargv[3] = buf[3];

	/* copy remaining args from argv[] and add NULL terminator */

	for (cnt = 4; *argv != NULL; cnt++, argv++)
		newargv[cnt] = *argv;
	newargv[cnt] = NULL;

	/* generate an updated TET_TIARGS string -
		ti_tcmputenv does this and puts it in the environment */
	if (ti_tcmputenv(Mysysid, Snid, Xrid, Snames, Nsname) < 0) {
		errno = ENOMEM;
		return(-1);
	}

	/* generate a TET_TSARGS string -
		ts_tcmputenv does this and puts it in the environment */
	if (ts_tcmputenv() < 0) {
		errno = ENOMEM;
		return(-1);
	}

	/* then locate the new TET_TIARGS and TET_TSARGS strings */
	tiargs_env = tsargs_env = (char *) 0;
	for (ep = environ; *ep && !(tiargs_env && tsargs_env); ep++) {
		if ((cp = equindex(*ep)) == (char *) 0)
			continue;
		if (tiargs_env == (char *) 0)
			if (!strncmp(*ep, tiargs_name, (size_t) (cp - *ep))) {
				tiargs_env = *ep;
				continue;
			}
		if (tsargs_env == (char *) 0)
			if (!strncmp(*ep, tsargs_name, (size_t) (cp - *ep))) {
				tsargs_env = *ep;
				continue;
			}
	}
	ASSERT(tiargs_env);
	ASSERT(tsargs_env);

	/* see if TET_CONFIG, TET_RESFD, TET_TMPRESFD and TET_TSARGS
		are in the environment passed to tet_exec() -
		also, overwrite any TET_TIARGS and TET_TSARGS settings with
		the new values */
	config_ok = resfd_ok = tmpfd_ok = tiargs_ok = tsargs_ok = 0;
	for (cnt = 0; envp[cnt] != NULL; cnt++)
	{
		if (strncmp(envp[cnt], TET_CONFIG,
				(size_t)(sizeof(TET_CONFIG)-1)) == 0 &&
				envp[cnt][sizeof(TET_CONFIG)] == '=')
			config_ok++;
		else if (strncmp(envp[cnt], TET_RESFD,
				(size_t)(sizeof(TET_RESFD)-1)) == 0 &&
				envp[cnt][sizeof(TET_RESFD)] == '=')
			resfd_ok++;
		else if (strncmp(envp[cnt], TET_TMPRESFD,
				(size_t)(sizeof(TET_TMPRESFD)-1)) == 0 &&
				envp[cnt][sizeof(TET_TMPRESFD)] == '=')
			tmpfd_ok++;
		else if (strncmp(envp[cnt], tiargs_name,
				(size_t) (sizeof tiargs_name - 1)) == 0 &&
				envp[cnt][sizeof tiargs_name] == '=') {
			envp[cnt] = tiargs_env;
			tiargs_ok++;
		}
		else if (strncmp(envp[cnt], tsargs_name,
				(size_t) (sizeof tsargs_name - 1)) == 0 &&
				envp[cnt][sizeof tsargs_name] == '=') {
			envp[cnt] = tsargs_env;
			tsargs_ok++;
		}
	}

	/* if all the variables are present in the environment passed to
		tet_exec(), then we can use that -
		otherwise, we must allocate a new envp array and add
		variables that are missing */
	if (config_ok && resfd_ok && tmpfd_ok && tiargs_ok && tsargs_ok)
		newenvp = (char **) envp;
	else
	{
		if (!config_ok)
			cnt++;
		if (!resfd_ok)
			cnt++;
		if (!tmpfd_ok)
			cnt++;
		if (!tiargs_ok)
			cnt++;
		if (!tsargs_ok)
			cnt++;
		cnt++;
		newenvp = (char **) malloc((size_t)(cnt * sizeof(char *)));
		if (newenvp == NULL) {
			errno = ENOMEM;
			return -1;
		}

		for (cnt = 0; *envp != NULL; cnt++, envp++)
			newenvp[cnt] = *envp;
		if (!config_ok && (cp = getenv(TET_CONFIG)) != NULL)
		{
			cp -= sizeof(TET_CONFIG);
			newenvp[cnt++] = cp;
		}
		if (!resfd_ok && (cp = getenv(TET_RESFD)) != NULL)
		{
			cp -= sizeof(TET_RESFD);
			newenvp[cnt++] = cp;
		}
		if (!tmpfd_ok && (cp = getenv(TET_TMPRESFD)) != NULL)
		{
			cp -= sizeof(TET_TMPRESFD);
			newenvp[cnt++] = cp;
		}
		if (!tiargs_ok)
			newenvp[cnt++] = tiargs_env;
		if (!tsargs_ok)
			newenvp[cnt++] = tsargs_env;
		newenvp[cnt] = NULL;
	}

	return execve(file, newargv, newenvp);
}
