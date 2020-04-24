/*
 *      SCCS:  @(#)sdsnsys.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)sdsnsys.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)sdsnsys.c	1.2 92/09/30
NAME:		sdsnsys.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to send system name list to syncd

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
**	sd_snsys() - send system name list to SYNCD
**
**	return 0 if successful or -1 on error
*/

int sd_snsys(snid, snames, nsname)
long snid;
register int *snames, nsname;
{
	register struct valmsg *mp;
	register int n;
	extern char Sderrmsg[];

	/* check snames and nsname */
	if (!snames || nsname <= 0) {
		sd_errno = ER_INVAL;
		return(-1);
	}

	/* get the SYNCD message buffer */
	if ((mp = (struct valmsg *) sd_msgbuf(valmsgsz(OP_SNSYS_NVALUE(nsname)))) == (struct valmsg *) 0) {
		sd_errno = ER_ERR;
		return(-1);
	}

	/* construct the message */
	mp->vm_nvalue = OP_SNSYS_NVALUE(nsname);
	VM_SNID(mp) = snid;
	for (n = 0; n < nsname; n++)
		VM_SSYSID(mp, n) = (long) *snames++;

	/* perform the conversation and handle the reply codes */
	mp = (struct valmsg *) sd_talk(OP_SNSYS, TALK_DELAY);
	switch (sd_errno) {
	case ER_OK:
		return(0);
	case ER_INVAL:
	case ER_DUPS:
	case ER_SNID:
	case ER_INPROGRESS:
		break;
	case ER_ERR:
		if (!mp)
			break;
		/* else fall through */
	default:
		error(0, Sderrmsg, ptrepcode(sd_errno));
		break;
	}

	/* here for server error return */
	return(-1);
}

