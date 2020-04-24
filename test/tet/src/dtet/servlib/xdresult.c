/*
 *      SCCS:  @(#)xdresult.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)xdresult.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)xdresult.c	1.2 92/09/30
NAME:		xdresult.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	June 1992

DESCRIPTION:
	function to send a TP result to XRESD

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include "dtmsg.h"
#include "valmsg.h"
#include "error.h"
#include "servlib.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif


/*
**	xd_result() - send an OP_RESULT message to XRESD and receive
**	a reply
**
**	return 0 if successful or -1 on error
*/

int xd_result(xrid, result)
long xrid;
int result;
{
	register struct valmsg *mp;
	extern char Xderrmsg[];

	/* get the XRESD message buffer */
	if ((mp = (struct valmsg *) xd_msgbuf(valmsgsz(OP_RESULT_NVALUE))) == (struct valmsg *) 0) {
		xd_errno = ER_ERR;
		return(-1);
	}

	/* set up the request message */
	mp->vm_nvalue = OP_RESULT_NVALUE;
	VM_XRID(mp) = xrid;
	VM_RESULT(mp) = (long) result;

	/* send the request and receive the reply */
	mp = (struct valmsg *) xd_talk(OP_RESULT, TALK_DELAY);

	/* handle the return codes */
	switch (xd_errno) {
	case ER_OK:
		return(0);
	case ER_XRID:
	case ER_SYSID:
	case ER_DONE:
		break;
	case ER_ERR:
		if (!mp)
			break;
		/* else fall through */
	default:
		error(0, Xderrmsg, ptrepcode(xd_errno));
		break;
	}

	/* here for server error return */
	return(-1);
}

