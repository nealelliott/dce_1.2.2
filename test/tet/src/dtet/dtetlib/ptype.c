/*
 *      SCCS:  @(#)ptype.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)ptype.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)ptype.c	1.2 92/09/30
NAME:		ptype.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to return printable representation of process type

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include "dtmsg.h"
#include "ltoa.h"
#include "dtetlib.h"

/*
**	ptptype() - return a printable representation of a process type
*/

char *ptptype(ptype)
int ptype;
{
	static char text[] = "process-type ";
	static char msg[sizeof text + LNUMSZ];
	
	switch (ptype) {
	case PT_NOPROC:
		return("<no process>");
	case PT_MTCC:
		return("MTCC");
	case PT_STCC:
		return("STCC");
	case PT_MTCM:
		return("MTCM");
	case PT_STCM:
		return("STCM");
	case PT_XRESD:
		return("XRESD");
	case PT_SYNCD:
		return("SYNCD");
	default:
		(void) sprintf(msg, "%s%d", text, ptype);
		return(msg);
	}
}

