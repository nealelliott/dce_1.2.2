/*
 *      SCCS:  @(#)reqcode.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)reqcode.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)reqcode.c	1.2 92/09/30
NAME:		reqcode.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to return printable representation of a DTET interprocess
	message request code

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include "dtmsg.h"
#include "ltoa.h"
#include "dtetlib.h"

/*
**	ptreqcode() - return printable representation of message request code
*/

char *ptreqcode(request)
int request;
{
	static char text[] = "request-code ";
	static char msg[sizeof text + LNUMSZ];

	switch (request) {
	case OP_LOGON:
		return("LOGON");
	case OP_LOGOFF:
		return("LOGOFF");
	case OP_NULL:
		return("NULL");
	case OP_SNGET:
		return("SNGET");
	case OP_SNSYS:
		return("SNSYS");
	case OP_ASYNC:
		return("ASYNC");
	case OP_USYNC:
		return("USYNC");
	case OP_SYSID:
		return("SYSID");
	case OP_SYSNAME:
		return("SYSNAME");
	case OP_TSINFO:
		return("TSINFO");
	case OP_TRACE:
		return("TRACE");
	case OP_EXEC:
		return("EXEC");
	case OP_WAIT:
		return("WAIT");
	case OP_KILL:
		return("KILL");
	case OP_XROPEN:
		return("XROPEN");
	case OP_XRSYS:
		return("XRSYS");
	case OP_ICSTART:
		return("ICSTART");
	case OP_TPSTART:
		return("TPSTART");
	case OP_ICEND:
		return("ICEND");
	case OP_TPEND:
		return("TPEND");
	case OP_XRES:
		return("XRES");
	case OP_RESULT:
		return("RESULT");
	case OP_CFNAME:
		return("CFNAME");
	case OP_RCFNAME:
		return("RCFNAME");
	case OP_SNDCONF:
		return("SNDCONF");
	case OP_RCVCONF:
		return("RCVCONF");
	case OP_CONFIG:
		return("CONFIG");
	case OP_TFOPEN:
		return("TFOPEN");
	case OP_TFCLOSE:
		return("TFCLOSE");
	case OP_TFWRITE:
		return("TFWRITE");
	case OP_PUTENV:
		return("PUTENV");
	case OP_ACCESS:
		return("ACCESS");
	case OP_MKDIR:
		return("MKDIR");
	case OP_RMDIR:
		return("RMDIR");
	case OP_CHDIR:
		return("CHDIR");
	case OP_FOPEN:
		return("FOPEN");
	case OP_FCLOSE:
		return("FCLOSE");
	case OP_GETS:
		return("GETS");
	case OP_PUTS:
		return("PUTS");
	case OP_LOCKFILE:
		return("LOCKFILE");
	case OP_SHARELOCK:
		return("SHARELOCK");
	case OP_MKTMPDIR:
		return("MKTMPDIR");
	case OP_UNLINK:
		return("UNLINK");
	case OP_RXFILE:
		return("RXFILE");
	case OP_MKSDIR:
		return("MKSDIR");
	case OP_TSFILES:
		return("TSFILES");
#if TESTING
	case OP_PRINT:
		return("PRINT");
#endif
	default:
		(void) sprintf(msg, "%s%d", text, request);
		return(msg);
	}
}

