/*
 *      SCCS:  @(#)tctdir.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tctdir.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tctdir.c	1.2 92/09/30
NAME:		tctdir.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	May 1992

DESCRIPTION:
	function to request TCCD to make a temporary subdirectory

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
**	tc_mktmpdir() - send an OP_MKTMPDIR message to TCCD and receive a reply
**
**	return pointer to the new directory name if successful
**		or (char *) 0 on error
*/

char *tc_mktmpdir(sysid, dir)
int sysid;
char *dir;
{
	register struct avmsg *mp;
	extern char Tcerrmsg[];

	/* make sure that dir is non-null */
	if (!dir || !*dir) {
		tc_errno = ER_INVAL;
		return((char *) 0);
	}

	/* get the TCCD message buffer */
	if ((mp = (struct avmsg *) tc_msgbuf(sysid, avmsgsz(OP_DIR_ARGC))) == (struct avmsg *) 0) {
		tc_errno = ER_ERR;
		return((char *) 0);
	}

	/* set up the request message */
	mp->av_argc = OP_DIR_ARGC;
	AV_DIR(mp) = dir;

	/* send the request and receive the reply */
	mp = (struct avmsg *) tc_talk(sysid, OP_MKTMPDIR, TALK_DELAY);

	/* handle the return codes */
	if (tc_errno == ER_OK)
		return(AV_DIR(mp));
	else if ((errno = unmaperrno(tc_errno)) == 0)
		switch (tc_errno) {
		case ER_ERR:
			if (!mp)
				break;
			/* else fall through */
		default:
			error(0, Tcerrmsg, ptrepcode(tc_errno));
			break;
		}

	/* here for server error return */
	return((char *) 0);
}

