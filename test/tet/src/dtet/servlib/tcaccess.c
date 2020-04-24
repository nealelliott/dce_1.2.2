/*
 *      SCCS:  @(#)tcaccess.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tcaccess.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tcaccess.c	1.2 92/09/30
NAME:		tcaccess.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	June 1992

DESCRIPTION:
	function to request TCCD to check the accessibility of a file

MODIFICATIONS:

************************************************************************/

#include "dtmsg.h"
#include "avmsg.h"
#include "servlib.h"

/*
**	tc_access() - send an OP_ACCESS message to TCCD and receive a reply
**
**	return 0 if successful or -1 on error
*/

int tc_access(sysid, path, mode)
int sysid, mode;
char *path;
{
	register struct avmsg *mp;

	/* make sure that path is non-null */
	if (!path || !*path) {
		tc_errno = ER_INVAL;
		return(-1);
	}

	/* get the TCCD message buffer */
	if ((mp = (struct avmsg *) tc_msgbuf(sysid, avmsgsz(OP_ACCESS_ARGC))) == (struct avmsg *) 0) {
		tc_errno = ER_ERR;
		return(-1);
	}

	/* set up the request message */
	mp->av_argc = OP_ACCESS_ARGC;
	AV_MODE(mp) = (long) mode;
	AV_PATH(mp) = path;

	/* send the request and receive the reply */
	return(tc_rsys(sysid, OP_ACCESS));
}

