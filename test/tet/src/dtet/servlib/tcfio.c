/*
 *      SCCS:  @(#)tcfio.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tcfio.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tcfio.c	1.2 92/09/30
NAME:		tcfio.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	May 1992

DESCRIPTION:
	functions to request STCC to open, close and write ascii files

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <errno.h>
#include "dtmsg.h"
#include "avmsg.h"
#include "valmsg.h"
#include "error.h"
#include "servlib.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/* static function declarations */
#if __STDC__
static char *tc_fio(int, int);
#else /* __STDC__ */
static char *tc_fio();
#endif /* __STDC__ */


/*
**	tc_fopen() - send an OP_FOPEN message to STCC and receive a reply
**
**	return file ID if successful or -1 on error
*/

int tc_fopen(sysid, file)
int sysid;
char *file;
{
	register char *dp;

	/* make sure that file is non-null */
	if (!file || !*file) {
		tc_errno = ER_INVAL;
		return(-1);
	}

	/* get the STCC message buffer */
	if ((dp = tc_msgbuf(sysid, avmsgsz(OP_FOPEN_ARGC))) == (char *) 0) {
		tc_errno = ER_ERR;
		return(-1);
	}

#define mp	((struct avmsg *) dp)

	/* set up the request message */
	mp->av_argc = OP_FOPEN_ARGC;
	AV_FNAME(mp) = file;
	AV_FTYPE(mp) = "w";

#undef mp

	dp = tc_fio(sysid, OP_FOPEN);

#define rp ((struct valmsg *) dp)

	return(dp ? (int) VM_FID(rp) : -1);

#undef rp

}

/*
**	tc_fclose() - send an OP_FCLOSE message to STCC and receive a reply
**
**	return file ID if successful or -1 on error
*/

int tc_fclose(sysid, fid)
int sysid, fid;
{
	register struct valmsg *mp;

	/* get the STCC message buffer */
	if ((mp = (struct valmsg *) tc_msgbuf(sysid, valmsgsz(OP_FCLOSE_NVALUE))) == (struct valmsg *) 0) {
		tc_errno = ER_ERR;
		return(-1);
	}

	/* set up the request message */
	mp->vm_nvalue = OP_FCLOSE_NVALUE;
	VM_FID(mp) = (long) fid;

	return(tc_fio(sysid, OP_FCLOSE) == (char *) 0 ? -1 : 0);
}

/*
**	tc_puts() - send a single line OP_PUTS message to STCC and receive a
**		reply
**
**	return 0 if successful or -1 on error
*/

int tc_puts(sysid, fid, line)
int sysid, fid;
char *line;
{
	return(tc_putsv(sysid, fid, &line, 1));
}

/*
**	tc_putsv() - send an OP_PUTS message to STCC and receive a reply
**
**	return 0 if successful or -1 on error
*/

int tc_putsv(sysid, fid, lines, nline)
int sysid, fid;
register int nline;
register char **lines;
{

	register struct avmsg *mp;
	register int n;

	/* make sure that lines is non-zero and that nline is +ve */
	if (!lines || nline <= 0) {
		tc_errno = ER_INVAL;
		return(-1);
	}

	/* get the STCC message buffer */
	if ((mp = (struct avmsg *) tc_msgbuf(sysid, avmsgsz(OP_PUTS_ARGC(nline)))) == (struct avmsg *) 0) {
		tc_errno = ER_ERR;
		return(-1);
	}

	/* set up the request message */
	mp->av_argc = OP_PUTS_ARGC(nline);
	AV_FID(mp) = (long) fid;
	for (n = 0; n < nline; n++)
		AV_FLINE(mp, n) = *lines++;

	return(tc_fio(sysid, OP_PUTS) == (char *) 0 ? -1 : 0);
}

/*
**	tc_fio() - common tc_talk() interface used by several functions
**
**	return pointer to STCC reply buffer if successful
**		or (char *) 0 on error
*/

static char *tc_fio(sysid, request)
int sysid, request;
{
	register char *dp;
	extern char Tcerrmsg[];

	/* send the request and receive the reply */
	dp = tc_talk(sysid, request, TALK_DELAY);

	/* handle the return codes */
	if (tc_errno == ER_OK)
		return(dp);
	else if ((errno = unmaperrno(tc_errno)) == 0)
		switch (tc_errno) {
		case ER_FID:
		case ER_INVAL:
			break;
		case ER_ERR:
			if (!dp)
				break;
			/* else fall through */
		default:
			error(0, Tcerrmsg, ptrepcode(tc_errno));
			break;
		}

	/* here for server error return */
	return((char *) 0);
}

