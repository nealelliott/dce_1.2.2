/*
 *      SCCS:  @(#)msgbuf.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)msgbuf.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)msgbuf.c	1.2 92/09/30
NAME:		msgbuf.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	server message buffer access function

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <sys/types.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "bstring.h"
#include "servlib.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	ti_msgbuf() - return a pointer to a ptab message data buffer,
**		growing it if necessary
**
**	return (char *) 0 on error
*/

char *ti_msgbuf(pp, newlen)
struct ptab *pp;
int newlen;
{
	if (BUFCHK(&pp->ptm_data, &pp->pt_mdlen, newlen) < 0)
		return((char *) 0);

	bzero(pp->ptm_data, newlen);
	return(pp->ptm_data);
}

