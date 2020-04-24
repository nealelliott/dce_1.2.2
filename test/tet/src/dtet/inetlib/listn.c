/*
 *      SCCS:  @(#)listn.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)listn.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)listn.c	1.2 92/09/30
NAME:		listn.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to listen on an INET socket

MODIFICATIONS:

************************************************************************/

#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "dtmac.h"
#include "error.h"
#include "ltoa.h"
#include "inetlib_in.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif


/*
**	ts_listen() - arrange to listen on the incoming message socket
*/

void ts_listen(sd)
int sd;
{
	TRACE2(Tio, 4, "listen on sd %s", itoa(sd));

	if (listen(sd, 10) < 0)
		fatal(errno, "listen() failed on sd", itoa(sd));
}

