/*
 *      SCCS:  @(#)xdxrsys.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)xdxrsys.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)xdxrsys.c	1.2 92/09/30
NAME:		xdxrsys.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	May 1992

DESCRIPTION:
	function to assign a system name list to a previously opened execution
	results file

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
**	xd_xrsys() - send an OP_XRSYS message to XRESD and receive a reply
**
**	return 0 if successful or -1 on error
*/

int xd_xrsys(xrid, snames, nsname)
long xrid;
register int *snames, nsname;
{
	register struct valmsg *mp;
	register int n;
	extern char Xderrmsg[];

	/* make sure that snames is non-zero and nsname is +ve */
	if (!snames || nsname <= 0) {
		xd_errno = ER_INVAL;
		return(-1);
	}

	/* get the XRESD message buffer */
	if ((mp = (struct valmsg *) xd_msgbuf(valmsgsz(OP_XRSYS_NVALUE(nsname)))) == (struct valmsg *) 0) {
		xd_errno = ER_ERR;
		return(-1);
	}

	/* set up the request message */
	mp->vm_nvalue = OP_XRSYS_NVALUE(nsname);
	VM_XRID(mp) = xrid;
	for (n = 0; n < nsname; n++)
		VM_XSYSID(mp, n) = (long) *snames++;

	/* send the request and receive the reply */
	mp = (struct valmsg *) xd_talk(OP_XRSYS, TALK_DELAY);

	/* handle the return codes */
	switch (xd_errno) {
	case ER_OK:
		return(0);
	case ER_INVAL:
	case ER_PERM:
	case ER_DUPS:
	case ER_XRID:
	case ER_INPROGRESS:
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

