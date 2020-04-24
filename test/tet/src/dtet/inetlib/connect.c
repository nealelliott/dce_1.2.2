/*
 *      SCCS:  @(#)connect.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)connect.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)connect.c	1.2 92/09/30
NAME:		connect.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	required transport-specific library interface

	function to initiate connection to remote process

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
#include "error.h"
#include "ltoa.h"
#include "tslib.h"
#include "server_in.h"

#if !defined(NOTRACE) && !defined(__hpux)
#include <arpa/inet.h>
#endif

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	ts_connect() - make a connection to a remote process
*/

void ts_connect(pp)
register struct ptab *pp;
{
	register struct tptab *tp = (struct tptab *) pp->pt_tdata;
	register int rc, sd;

	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		error(errno, "can't get socket", (char *) 0);
		pp->pt_state = PS_DEAD;
		pp->pt_flags |= PF_ATTENTION;
		return;
	}
	else
		tp->tp_sd = sd;

	/* call the server-specific connect routine to massage the socket and
		fill in network address */
	if (ss_tsconnect(pp) < 0) {
		pp->pt_state = PS_DEAD;
		pp->pt_flags |= PF_ATTENTION;
		return;
	}

	TRACE3(Tio, 4, "connect to %s port %s", inet_ntoa(tp->tp_sin.sin_addr),
		itoa(ntohs(tp->tp_sin.sin_port)));

	/* attempt the connection */
	do {
		errno = 0;
		rc = connect(sd, (struct sockaddr *) &tp->tp_sin, sizeof tp->tp_sin);
	} while (rc < 0 && errno == EINTR);

	/* handle errors */
	if (rc < 0) {
		switch (errno) {
		case EWOULDBLOCK:
		case EINPROGRESS:
			if (pp->pt_flags & PF_NBIO) {
				TRACE1(Tio, 6, "connect in progress");
				pp->pt_flags |= PF_INPROGRESS;
				return;
			}
			/* else fall through */
		default:
			error(errno, "connect failed", rtoa(&pp->pt_rid));
			pp->pt_state = PS_DEAD;
		}
	}
	else {
		TRACE1(Tio, 6, "connect succeeded");
		pp->pt_flags |= PF_CONNECTED;
	}

	/* here if the connect call completed one way or the other */
	pp->pt_flags |= PF_ATTENTION;
}

