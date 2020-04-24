/*
 *      SCCS:  @(#)xdfio.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)xdfio.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)xdfio.c	1.2 92/09/30
NAME:		xdfio.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	May 1992

DESCRIPTION:
	functions to request XRESD to open, close and read ascii files

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
static char *xd_fio(int);
#else /* __STDC__ */
static char *xd_fio();
#endif /* __STDC__ */


/*
**	xd_fopen() - send an OP_FOPEN message to XRESD and receive a reply
**
**	return file ID if successful or -1 on error
*/

int xd_fopen(file)
char *file;
{
	register char *dp;

	/* make sure that file is non-null */
	if (!file || !*file) {
		xd_errno = ER_INVAL;
		return(-1);
	}

	/* get the XRESD message buffer */
	if ((dp = xd_msgbuf(avmsgsz(OP_FOPEN_ARGC))) == (char *) 0) {
		xd_errno = ER_ERR;
		return(-1);
	}

#define mp	((struct avmsg *) dp)

	/* set up the request message */
	mp->av_argc = OP_FOPEN_ARGC;
	AV_FNAME(mp) = file;
	AV_FTYPE(mp) = "r";

#undef mp

	dp = xd_fio(OP_FOPEN);

#define rp ((struct valmsg *) dp)

	return(dp ? (int) VM_FID(rp) : -1);

#undef rp

}

/*
**	xd_fclose() - send an OP_FCLOSE message to XRESD and receive a reply
**
**	return file ID if successful or -1 on error
*/

int xd_fclose(fid)
int fid;
{
	register struct valmsg *mp;

	/* get the XRESD message buffer */
	if ((mp = (struct valmsg *) xd_msgbuf(valmsgsz(OP_FCLOSE_NVALUE))) == (struct valmsg *) 0) {
		xd_errno = ER_ERR;
		return(-1);
	}

	/* set up the request message */
	mp->vm_nvalue = OP_FCLOSE_NVALUE;
	VM_FID(mp) = (long) fid;

	return(xd_fio(OP_FCLOSE) == (char *) 0 ? -1 : 0);
}

/*
**	xd_gets() - read a single line from a file opened by xd_fopen()
**		and return a pointer thereto
**
**	return (char *) 0 on EOF or error
*/

char *xd_gets(fid)
int fid;
{
	register char **rp;
	int nline = 1;

	if ((rp = xd_getsv(fid, &nline, (int *) 0)) == (char **) 0 ||
		nline != 1)
			return((char *) 0);

	return(*rp);
}

/*
**	xd_getsv() - read one or more lines from a file opened by xd_fopen()
**
**	up to *nlines lines will be read from the file
**
**	return a pointer to the first in a list of string pointers
**	if successful, or (char **) 0 on error
**
**	if at least one line could be read, the number of lines actually read
**	is returned indirectly through *nlines, and *eof is set to 1 or 0
**	depending on whether or not the file is at EOF
**
**	the lines and their pointers are held in memory owned by the xd_talk()
**	subsystem, so they must be copied if required before another XRESD
**	request is issued
*/

char **xd_getsv(fid, nlines, eof)
int fid, *eof;
register int *nlines;
{
	register char *dp;

	/* make sure that nlines is non-zero and that *nlines is +ve */
	if (!nlines || *nlines <= 0) {
		xd_errno = ER_INVAL;
		return((char **) 0);
	}

	/* get the XRESD message buffer */
	if ((dp = xd_msgbuf(valmsgsz(OP_GETS_NVALUE))) == (char *) 0) {
		xd_errno = ER_ERR;
		return((char **) 0);
	}

#define mp	((struct valmsg *) dp)

	/* set up the request message */
	mp->vm_nvalue = OP_GETS_NVALUE;
	VM_FID(mp) = (long) fid;
	VM_NLINES(mp) = (long) *nlines;

#undef mp

	if ((dp = xd_fio(OP_GETS)) == (char *) 0)
		return((char **) 0);

#define rp	((struct avmsg *) dp)

	/* all ok so return all the return values */
	*nlines = (int) OP_GETS_NLINE(rp);
	if (eof)
		*eof = (AV_FLAG(rp) == AV_DONE || *nlines <= 0) ? 1 : 0;
	return(&AV_FLINE(rp, 0));

#undef rp
}

/*
**	xd_fio() - common xd_talk() interface used by several functions
**
**	return pointer to XRESD reply buffer if successful
**		or (char *) 0 on error
*/

static char *xd_fio(request)
int request;
{
	register char *dp;
	extern char Xderrmsg[];

	/* send the request and receive the reply */
	dp = xd_talk(request, TALK_DELAY);

	/* handle the return codes */
	switch (xd_errno) {
	case ER_OK:
		return(dp);
	case ER_FID:
	case ER_INVAL:
		break;
	case ER_ERR:
		if (!dp)
			break;
		/* else fall through */
	default:
		error(unmaperrno(xd_errno), Xderrmsg, ptrepcode(xd_errno));
		break;
	}

	/* here for server error return */
	return((char *) 0);
}

