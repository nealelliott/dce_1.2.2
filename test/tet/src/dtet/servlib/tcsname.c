/*
 *      SCCS:  @(#)tcsname.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tcsname.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tcsname.c	1.2 92/09/30
NAME:		tcsname.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	May 1992

DESCRIPTION:
	function to send system name list to tccd

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
**	tc_sysname() - send OP_SYSNAME message to STCC and receive a reply
**
**	return 0 if successful or -1 on error
*/

int tc_sysname(sysid, snames, nsname)
int sysid;
register int *snames, nsname;
{
	register struct valmsg *mp;
	register int n;
	extern char Tcerrmsg[];

	/* make sure that snames is non-zero and that nsname is +ve */
	if (!snames || nsname <= 0) {
		tc_errno = ER_INVAL;
		return(-1);
	}

	/* get the STCC message buffer */
	if ((mp = (struct valmsg *) tc_msgbuf(sysid, valmsgsz(OP_SYSNAME_NVALUE(nsname)))) == (struct valmsg *) 0) {
		tc_errno = ER_ERR;
		return(-1L);
	}

	/* set up the request message */
	mp->vm_nvalue = OP_SYSNAME_NVALUE(nsname);
	for (n = 0; n < nsname; n++)
		VM_SYSNAME(mp, n) = (long) *snames++;

	/* send the request and receive the reply */
	mp = (struct valmsg *) tc_talk(sysid, OP_SYSNAME, TALK_DELAY);

	/* handle the return codes */
	switch (tc_errno) {
	case ER_OK:
		return(0);
	case ER_INVAL:
	case ER_DUPS:
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

