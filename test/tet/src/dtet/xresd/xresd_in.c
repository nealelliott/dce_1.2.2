/*
 *      SCCS:  @(#)xresd_in.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)xresd_in.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)xresd_in.c	1.2 92/09/30
NAME:		xresd_in.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	server-specific functions for INET xresd server

MODIFICATIONS:

************************************************************************/


#if __STDC__
#include <stdio.h>
#endif

#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "dtmsg.h"
#include "ptab.h"
#include "server_in.h"
#include "inetlib_in.h"

int Listen_sd = 0;			/* socket on which to listen */

/*
**	ss_tsaccept() - server-specific accept() processing
*/

void ss_tsaccept()
{
	/* accept the connection unless we are closing down */
	if (Listen_sd >= 0)
		ts_accept(Listen_sd);
}

/*
**	ss_tsafteraccept() - server-specific things to do after an accept()
*/

int ss_tsafteraccept(pp)
struct ptab *pp;
{
	/* establish non-blocking i/o on the connection */
	return(ts_nbio(pp));
}

/*
**	ss_tsinitb4fork() - xresd transport-specific initialisation
*/

void ss_tsinitb4fork()
{
	/* arrange to accept incoming connections */
	ts_listen(Listen_sd);
}

