/*
 *      SCCS:  @(#)tccdport.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tccdport.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tccdport.c	1.2 92/09/30
NAME:		tccdport.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to return the tccd well-known port number

MODIFICATIONS:

************************************************************************/

#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include "error.h"
#include "dtmac.h"
#include "inetlib_in.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

#ifndef NOTRACE
#include "ltoa.h"
#endif

/*
**	gettccdport() - return tccd port number in host byte order
**
**	return -1 on error
**	(so don't define the tccd port number as > 32767 on 16 bit machines!)
*/

int gettccdport()
{
	static int port;
	register struct servent *sp;

	if (port)
		return(port);

	errno = 0;
	if ((sp = getservbyname("tcc", "tcp")) == (struct servent *) 0) {
		error(errno != ENOTTY ? errno : 0,
			"tcc/tcp: unknown service", (char *) 0);
		port = -1;
	}
	else
		port = ntohs((u_short) sp->s_port);

	endservent();

	TRACE2(Tio, 2, "tccd port = %s", itoa(port));

	return(port);
}

