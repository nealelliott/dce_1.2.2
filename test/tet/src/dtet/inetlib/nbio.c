/*
 *      SCCS:  @(#)nbio.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)nbio.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)nbio.c	1.2 92/09/30
NAME:		nbio.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to establish non-blocking i/o on an INET socket

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <sys/types.h>
#include <netinet/in.h>
#include "dtmsg.h"
#include "ptab.h"
#include "tptab_in.h"
#include "inetlib_in.h"
#include "dtetlib.h"

/*
**	ts_nbio() - establish non-blocking i/o on a socket
**
**	return 0 if successful or -1 on error
*/

int ts_nbio(pp)
struct ptab *pp;
{
	register int rc;

	if ((rc = fionbio(((struct tptab *) pp->pt_tdata)->tp_sd)) == 0)
		pp->pt_flags |= PF_NBIO;

	return(rc);
}

