/*
 *      SCCS:  @(#)svote.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)svote.c	1.2 (92/09/30) DTET release 1.0b";
#endif


/************************************************************************

SCCS:   	@(#)svote.c	1.2 92/09/30
NAME:		svote.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	June 1992

DESCRIPTION:
	function to return printable representation of a sync vote

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <sys/types.h>
#include "synreq.h"
#include "ltoa.h"
#include "dtetlib.h"

/*
**	ptsvote() - return a printable representation of a sync vote
*/

char *ptsvote(vote)
int vote;
{
	static char text[] = "unknown sync-vote ";
	static char msg[sizeof text + LNUMSZ];

	switch (vote) {
	case SV_YES:
		return("YES");
	case SV_NO:
		return("NO");
	default:
		(void) sprintf(msg, "%s%d", text, vote);
		return(msg);
	}
}

