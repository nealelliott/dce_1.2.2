/*
 *      SCCS:  @(#)tccfnam.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tccfnam.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tccfnam.c	1.2 92/09/30
NAME:		tccfnam.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	May 1992

DESCRIPTION:
	function to send config file name to STCC

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include "dtmsg.h"
#include "avmsg.h"
#include "error.h"
#include "servlib.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	tc_cfname() - send OP_CFNAME message to STCC and receive a reply
**
**	return 0 if successful or -1 on error
*/

int tc_cfname(sysid, cfname)
int sysid;
char *cfname;
{
	register struct avmsg *mp;
	extern char Tcerrmsg[];

	/* make sure that cfname is non-null */
	if (!cfname || !*cfname) {
		tc_errno = ER_INVAL;
		return(-1);
	}

	/* get the STCC message buffer */
	if ((mp = (struct avmsg *) tc_msgbuf(sysid, avmsgsz(OP_CFNAME_ARGC(TC_NCFNAME)))) == (struct avmsg *) 0) {
		tc_errno = ER_ERR;
		return(-1);
	}

	/* set up the request message */
	mp->av_argc = OP_CFNAME_ARGC(TC_NCFNAME);
	AV_CFNAME(mp, 0) = cfname;

	/* send the request and receive the reply */
	mp = (struct avmsg *) tc_talk(sysid, OP_CFNAME, TALK_DELAY);

	/* handle the return codes */
	switch (tc_errno) {
	case ER_OK:
		return(0);
	case ER_INVAL:
	case ER_CONTEXT:
		break;
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

