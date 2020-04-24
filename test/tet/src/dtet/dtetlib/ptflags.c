/*
 *      SCCS:  @(#)ptflags.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)ptflags.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)ptflags.c	1.2 92/09/30
NAME:		ptflags.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to return a printable representation of process table flags

MODIFICATIONS:

************************************************************************/

#include <sys/types.h>
#include "dtmsg.h"
#include "ptab.h"
#include "ftoa.h"

/*
**	ptflags() - return printable representation of ptab pt_flags value
*/

char *ptflags(fval)
int fval;
{
	static struct flags flags[] = {
		{ PF_ATTENTION, "ATTENTION" },
		{ PF_INPROGRESS, "INPROGRESS" },
		{ PF_IODONE, "IODONE" },
		{ PF_IOERR, "IOERR" },
		{ PF_TIMEDOUT, "TIMEDOUT" },
		{ PF_CONNECTED, "CONNECTED" },
		{ PF_LOGGEDON, "LOGGEDON" },
		{ PF_LOGGEDOFF, "LOGGEDOFF" },
		{ PF_RCVHDR, "RCVHDR" },
		{ PF_SERVER, "SERVER" },
		{ PF_NBIO, "NBIO" },
		{ PF_SERVWAIT, "SERVWAIT" }
	};

	return(ftoa(fval, flags, sizeof flags / sizeof flags[0]));
}

