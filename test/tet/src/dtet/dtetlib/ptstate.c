/*
 *      SCCS:  @(#)ptstate.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)ptstate.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)ptstate.c	1.2 92/09/30
NAME:		ptstate.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to return printable representation of process state

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <sys/types.h>
#include "dtmsg.h"
#include "ptab.h"
#include "ltoa.h"
#include "dtetlib.h"

/*
**	ptstate() - return a printable representation of a ptab pt_state
**		value
*/

char *ptstate(state)
int state;
{
	static char text[] = "process-state ";
	static char msg[sizeof text + LNUMSZ];

	switch (state) {
	case PS_DEAD:
		return("DEAD");
	case PS_IDLE:
		return("IDLE");
	case PS_RCVMSG:
		return("RCVMSG");
	case PS_SNDMSG:
		return("SNDMSG");
	case PS_PROCESS:
		return("PROCESS");
	case PS_WAITSYNC:
		return("WAITSYNC");
	case PS_CONNECT:
		return("CONNECT");
	default:
		(void) sprintf(msg, "%s%d", text, state);
		return(msg);
	}
}

