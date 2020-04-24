/*
 *      SCCS:  @(#)xdrcfnam.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)xdrcfnam.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)xdrcfnam.c	1.2 92/09/30
NAME:		xdrcfname.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	May 1992

DESCRIPTION:
	function to retrieve config file name from XRESD

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
**	xd_rcfname() - send OP_RCFNAME message to XRESD and receive a reply
**
**	return pointer to config file names if successful or (char **) 0
**	on error
*/

char **xd_rcfname()
{
	register struct avmsg *rp;
	extern char Xderrmsg[];

	/* send the request and receive the reply */
	rp = (struct avmsg *) xd_talk(OP_RCFNAME, TALK_DELAY);

	/* handle the return codes */
	switch (xd_errno) {
	case ER_OK:
		ASSERT(OP_CFNAME_NCFNAME(rp) == XD_NCFNAME);
		return(&AV_CFNAME(rp, 0));
	case ER_ERR:
		if (!rp)
			break;
		/* else fall through */
	default:
		error(0, Xderrmsg, ptrepcode(xd_errno));
		break;
	}

	/* here for server error return */
	return((char **) 0);
}

