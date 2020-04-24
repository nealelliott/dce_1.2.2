/*
 *      SCCS:  @(#)exit.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)exit.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)exit.c	1.2 92/09/30
NAME:		exit.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	May 1992

SYNOPSIS:
	#include <dapi.h>
	void tet_exit(int status);

	void tet_logoff();

DESCRIPTION:
	DTET API functions

	Clean up and exit from a process linked with mtcm.o, stcm.o or
	tcmrem.o.

	tet_exit() calls tet_logoff(), then calls exit() to cause the calling
	process to terminate.
	tet_exit() should be called instead of exit() by a parent DTET TCM
	process.
	tet_exit() should never be called by a child DTET process.

	tet_logoff() logs off all servers to which the calling process is
	logged on.
	tet_logoff() should be called directly by a process linked with
	tcmrem.o which no longer wishes to use DTET servers
	eg: if the parent process is about to exec another process.

	If a client process exits without logging off a server accessed via a
	connection-based transport, the server will emit a diagnostic but
	should otherwise proceed as if the logoff had been performed.
	However, if the server is accessed via a conectionless transport,
	it will not be aware that the client has exited; the results are
	rather less predictable in this case.


MODIFICATIONS:

************************************************************************/


#include <sys/types.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "servlib.h"
#include "dapi.h"

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#else
extern void exit();
#endif


/*
**	tet_exit() - log off all servers and exit
*/

void tet_exit(status)
int status;
{
	tet_logoff();
	exit(status);
}

/*
**	tet_logoff() - log off all servers and close their connections
*/

void tet_logoff()
{
	register struct ptab *pp;
	extern struct ptab *Ptab;

	while (Ptab) {
		pp = Ptab;
		(void) ti_logoff(pp, 0);
		ptrm(pp);
		ptfree(pp);
	}

	(void) sd_logoff(0);
	(void) xd_logoff();
}

