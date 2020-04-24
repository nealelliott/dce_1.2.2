/*
 *      SCCS:  @(#)accept.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)accept.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)accept.c	1.2 92/09/30
NAME:		accept.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to accept a new INET connection

MODIFICATIONS:

************************************************************************/

#include <errno.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "tptab_in.h"
#include "ltoa.h"
#include "error.h"
#include "tslib.h"
#include "server.h"
#include "server_in.h"
#include "inetlib_in.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	ts_accept() - accept a new connection on a listening socket
**		and allocate a ptab entry for it
*/

void ts_accept(lsd)
int lsd;
{
	struct sockaddr_in sin;
	register struct ptab *pp;
	register struct tptab *tp;
	register int nsd;
	int len;

	TRACE2(Tio, 4, "accept connection on sd %s", itoa(lsd));

	/* allocate a proc table entry for this connection */
	if ((pp = ptalloc()) == (struct ptab *) 0)
		return;

	/* accept the connection */
	do {
		errno = 0;
		len = sizeof sin;
		nsd = accept(lsd, (struct sockaddr *) &sin, &len);
	} while (nsd < 0 && errno == EINTR);

	if (nsd < 0) {
		error(errno, "accept() failed on sd", itoa(lsd));
		ptfree(pp);
		return;
	}

	TRACE2(Tio, 4, "accept: new sd = %s", itoa(nsd));

	/* store the remote address */
	tp = (struct tptab *) pp->pt_tdata;
	tp->tp_sd = nsd;
	tp->tp_sin = sin;

	/* call server-specific routine to massage socket */
	if (ss_tsafteraccept(pp) < 0) {
		ts_dead(pp);
		ptfree(pp);
		return;
	}

	/* pass the new ptab entry to the server for registration */
	ss_newptab(pp);

	pp->pt_flags |= PF_CONNECTED;
}

