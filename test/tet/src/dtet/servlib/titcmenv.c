/*
 *      SCCS:  @(#)titcmenv.c	1.3 (92/10/06) 
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
static char sccsid[] = "@(#)titcmenv.c	1.3 (92/10/06) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)titcmenv.c	1.3 92/10/06
NAME:		titcmenv.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to put transport-independent arguments in the environment to
	be received by the tcm

MODIFICATIONS:
	Andrew Dingwall, UniSoft Ltd., October 1992
	Modified so as to allow ti_tcmputenv() to be called from
	TCMs as well as from TCCs - this is required by tet_exec().

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include "dtmac.h"
#include "dtmsg.h"
#include "ltoa.h"
#include "servlib.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#else
extern void free();
#endif

/*
**	ti_tcmputenv() - put ti args in the environment for TCM
**
**	return 0 if successful or -1 on error
*/

int ti_tcmputenv(psysid, snid, xrid, sname, nsname)
int psysid;
long snid, xrid;
register int *sname, nsname;
{
	register char *p1, *p2;
	register int n, needlen;
	char sysidstr[LNUMSZ], psysidstr[LNUMSZ];
	char snidstr[LNUMSZ], xridstr[LNUMSZ];
	static char envname[] = "TET_TIARGS=";
	static char *argbuf, *laststring;
	static int ablen;
	extern int Mysysid;
#ifndef NOTRACE
	extern int Myptype;
	register char **ap, **targv;
	int tcmptype;
#endif

	/* start with the sysid and parent sysid arguments */
	(void) sprintf(sysidstr, "%d", Mysysid);
	(void) sprintf(psysidstr, "%d", psysid);
	needlen = sizeof envname + strlen(sysidstr) + strlen(psysidstr) + 6;

	/* add the snid argument */
	if (snid >= 0L) {
		(void) sprintf(snidstr, "%ld", snid);
		needlen += strlen(snidstr) + 3;
	}

	/* add the xrid argument */
	if (xrid >= 0L) {
		(void) sprintf(xridstr, "%ld", xrid);
		needlen += strlen(xridstr) + 3;
	}

	/* add the sysname argument */
	if (nsname > 0)
		needlen += (nsname * (LNUMSZ + 1)) + 2;

#ifndef NOTRACE
	/* get the set of trace flags for a TCM */
	switch (Myptype) {
	case PT_MTCC:
	case PT_MTCM:
		tcmptype = PT_MTCM;
		break;
	default:
		tcmptype = PT_STCM;
		break;
	}
	if ((targv = traceargs(tcmptype, (char **) 0)) == (char **) 0)
		return(-1);

	/* count the trace flags and add their sizes */
	for (ap = targv; *ap; ap++)
		needlen += strlen(*ap) + 1;
#endif

	/* get a buffer for the argument list */
	if (BUFCHK(&argbuf, &ablen, needlen) < 0)
		return(-1);

	/* copy across the envar name */
	p1 = argbuf;
	for (p2 = envname; *p2; p2++)
		*p1++ = *p2;

	/* copy across the sysid and parent sysid arguments */
	p1 += mkoptarg(p1, 's', sysidstr, 1);
	p1 += mkoptarg(p1, 'p', psysidstr, 0);

	/* copy across the snid argument */
	if (snid >= 0L)
		p1 += mkoptarg(p1, 'n', snidstr, 0);

	/* copy across the xrid argument */
	if (xrid >= 0L)
		p1 += mkoptarg(p1, 'r', xridstr, 0);

	/* copy across the sysname list argument */
	if (nsname > 0) {
		p1 += mkoptarg(p1, 'l', (char *) 0, 0);
		for (n = 0; n < nsname; n++) {
			if (n)
				*p1++ = ',';
			for (p2 = itoa(*sname++); *p2; p2++)
				*p1++ = *p2;
		}
	}

#ifndef NOTRACE
	/* copy across the trace flags */
	for (ap = targv; *ap; ap++) {
		*p1++ = ' ';
		for (p2 = *ap; *p2; p2++)
			*p1++ = *p2;
	}
#endif

	*p1 = '\0';

	/* store the string in static memory and put it in the environment */
	if ((p1 = strstore(argbuf)) == (char *) 0 || dtputenv(p1) < 0)
		return(-1);

	/* free any previous one and remember the new one if successful */
	if (laststring) {
		TRACE2(Tbuf, 6, "free old ti env string = %s",
			itox(laststring));
		free(laststring);
	}
	laststring = p1;

	return(0);
}

