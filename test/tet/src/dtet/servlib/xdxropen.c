/*
 *      SCCS:  @(#)xdxropen.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)xdxropen.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)xdxropen.c	1.2 92/09/30
NAME:		xdxropen.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	May 1992

DESCRIPTION:
	function to open an execution results file and return the resulting
	xrid

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include "dtmsg.h"
#include "avmsg.h"
#include "valmsg.h"
#include "error.h"
#include "servlib.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	xd_xropen() - send an OP_XROPEN message to XRESD and receive a reply
**
**	return xrid of xres file if successful or -1 on error
*/

long xd_xropen(xfname)
char *xfname;
{
	register char *dp;
	extern char Xderrmsg[];

	/* make sure that xfname is non-null */
	if (!xfname || !*xfname) {
		xd_errno = ER_INVAL;
		return(-1L);
	}

	/* get the XRESD message buffer */
	if ((dp = xd_msgbuf(avmsgsz(OP_XROPEN_ARGC))) == (char *) 0) {
		xd_errno = ER_ERR;
		return(-1L);
	}

#define mp	((struct avmsg *) dp)

	/* set up the request message */
	mp->av_argc = OP_XROPEN_ARGC;
	AV_XFNAME(mp) = xfname;

#undef mp

	/* send the request and receive the reply */
	dp = xd_talk(OP_XROPEN, TALK_DELAY);

#define rp	((struct valmsg *) dp)

	/* handle the return codes */
	switch (xd_errno) {
	case ER_OK:
		return(VM_XRID(rp));
	case ER_PERM:
		break;
	case ER_ERR:
		if (!dp)
			break;
		/* else fall through */
	default:
		error(0, Xderrmsg, ptrepcode(xd_errno));
		break;
	}

	/* here for server error return */
	return(-1L);
}

