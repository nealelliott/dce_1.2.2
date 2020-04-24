/*
 *      SCCS:  @(#)tstcmenv.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tstcmenv.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tstcmenv.c	1.2 92/09/30
NAME:		tstcmenv.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to put transport-specific arguments in the environment to be
	reveived by the tcm

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <sys/types.h>
#include <netinet/in.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "tptab_in.h"
#include "ltoa.h"
#include "dtetlib.h"
#include "tslib.h"

#ifdef __hpux
#include <sys/socket.h>
#else
#include <arpa/inet.h>
#endif

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#else
extern void free();
#endif

/*
**	ts_tcmputenv() - put ts args in the environment for TCM
**
**	return 0 if successful or -1 on error
*/

int ts_tcmputenv()
{
	register char *p1, *p2;
	register int first;
	register struct sockaddr_in *ap;
	char envstring[1024];
	static char envname[] = "TET_TSARGS=";
	static char *laststring;
	extern struct ptab *Sd_ptab, *Xd_ptab;

	/* start the environment string */
	first = 1;
	p1 = envstring;
	for (p2 = envname; *p2; p2++)
		*p1++ = *p2;

	/* see if there is any tsinfo for syncd */
	ap = Sd_ptab ? &((struct tptab *) Sd_ptab->pt_tdata)->tp_sin : (struct sockaddr_in *) 0;
	if (ap && ap->sin_port) {
		p1 += mkoptarg(p1, 'y', inet_ntoa(ap->sin_addr), first);
		*p1++ = ',';
		for (p2 = itoa(ntohs(ap->sin_port)); *p2; p2++)
			*p1++ = *p2;
		first = 0;
	}

	/* see if there is any tsinfo for xresd */
	ap = Xd_ptab ? &((struct tptab *) Xd_ptab->pt_tdata)->tp_sin : (struct sockaddr_in *) 0;
	if (ap && ap->sin_port) {
		p1 += mkoptarg(p1, 'x', inet_ntoa(ap->sin_addr), first);
		*p1++ = ',';
		for (p2 = itoa(ntohs(ap->sin_port)); *p2; p2++)
			*p1++ = *p2;
	}

	*p1 = '\0';

	/* store the string in static memory and put it in the environment */
	if ((p1 = strstore(envstring)) == (char *) 0 || dtputenv(p1) < 0)
		return(-1);

	/* free any previous one and remember the new one if successful */
	if (laststring) {
		TRACE2(Tbuf, 6, "free old ts env string = %s",
			itox(laststring));
		free(laststring);
	}
	laststring = p1;

	return(0);
}

