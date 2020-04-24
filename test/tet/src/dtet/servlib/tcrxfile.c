/*
 *      SCCS:  @(#)tcrxfile.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tcrxfile.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tcrxfile.c	1.2 92/09/30
NAME:		tcrxfile.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	May 1992

DESCRIPTION:
	function to initiate remote file transfer from STCC

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <errno.h>
#include "dtmsg.h"
#include "avmsg.h"
#include "error.h"
#include "servlib.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	tc_rxfile() - send an OP_RXFILE message to STCC and receive a reply
**
**	return 0 if successful or -1 on error
*/

int tc_rxfile(sysid, from, to)
int sysid;
char *from, *to;
{
	register struct avmsg *mp;
	extern char Tcerrmsg[];

	/* make sure that file names are non-null */
	if (!from || !*from || !to || !*to) {
		tc_errno = ER_INVAL;
		return(-1);
	}

	/* get the STCC message buffer */
	if ((mp = (struct avmsg *) tc_msgbuf(sysid, avmsgsz(OP_RXFILE_ARGC))) == (struct avmsg *) 0) {
		tc_errno = ER_ERR;
		return(-1);
	}

	/* set up the request message */
	mp->av_argc = OP_RXFILE_ARGC;
	AV_XFROM(mp) = from;
	AV_XTO(mp) = to;

	/* send the request and receive the reply */
	mp = (struct avmsg *) tc_talk(sysid, OP_RXFILE, TALK_DELAY * 3);

	/* handle the return codes */
	switch (tc_errno) {
	case ER_OK:
		return(0);
	case ER_ERR:
		if (!mp)
			break;
		/* else fall through */
	default:
		error(0, Tcerrmsg, ptrepcode(tc_errno));
		break;
	}

	/* here for server error return */
	return(-1);
}

