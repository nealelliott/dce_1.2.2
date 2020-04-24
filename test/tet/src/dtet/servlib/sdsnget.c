/*
 *      SCCS:  @(#)sdsnget.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)sdsnget.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)sdsnget.c	1.2 92/09/30
NAME:		sdsnget.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to get a sync identifier from syncd

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
**	sd_snget() - get a sync id from SYNCD
**
**	return the sync id if successful or -1 on error
*/

long sd_snget()
{
	register struct valmsg *mp;
	extern char Sderrmsg[];

	/* perform the conversation and handle the reply codes */
	mp = (struct valmsg *) sd_talk(OP_SNGET, TALK_DELAY);
	switch (sd_errno) {
	case ER_OK:
		return(VM_SNID(mp));
	case ER_ERR:
		if (!mp)
			break;
		/* else fall through */
	default:
		error(0, Sderrmsg, ptrepcode(sd_errno));
		break;
	}

	/* here for server error return */
	return(-1L);
}

