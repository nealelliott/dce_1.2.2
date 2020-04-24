/*
 *      SCCS:  @(#)tccdaddr.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tccdaddr.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tccdaddr.c	1.2 92/09/30
NAME:		tccdaddr.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to determine tccd INET address and port number

MODIFICATIONS:

************************************************************************/

#include <errno.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "dtmsg.h"
#include "ptab.h"
#include "tptab_in.h"
#include "sysent.h"
#include "error.h"
#include "ltoa.h"
#include "inetlib_in.h"

#ifndef __hpux
#include <arpa/inet.h>
#endif

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	gettccdaddr() - look up the INET address and port number for an STCC
**		and store it in the related tptab entry
**
**	return 0 if successful or -1 on error
*/

int gettccdaddr(pp)
struct ptab *pp;
{
	struct in_addr addr, *ap;
	register struct sysent *sp;
	register struct tptab *tp = (struct tptab *) pp->pt_tdata;
	register int port;

	/* look up the host name in the systems file */
	errno = 0;
	if ((sp = getsysbyid(pp->ptr_sysid)) == (struct sysent *) 0) {
		error(errno, "can't get systems file entry for sysid",
			itoa(pp->ptr_sysid));
		return(-1);
	}

	/* if the system name is a host name, get the INET address */
	if ((long) (addr.s_addr = inet_addr(sp->sy_name)) == -1L)
		if ((ap = gethostaddr(sp->sy_name)) == (struct in_addr *) 0)
			return(-1);
		else
			addr = *ap;

	/* get the port number */
	if ((port = gettccdport()) == -1)
		return(-1);

	/* all ok so fill in the address details */
	tp->tp_sin.sin_family = AF_INET;
	tp->tp_sin.sin_addr = addr;
	tp->tp_sin.sin_port = htons((u_short) port);

	return(0);
}

