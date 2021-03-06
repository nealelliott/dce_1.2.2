/*
 *      SCCS:  @(#)sdsasync.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)sdsasync.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)sdsasync.c	1.2 92/09/30
NAME:		sdsasync.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	slave system auto-sync request function

MODIFICATIONS:

************************************************************************/

#include "synreq.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	sd_sasync() - perform an automatic sync from a slave system
**
**	return 0 if successful or -1 on error
*/

int sd_sasync(snid, spno, vote, timeout, synreq, nsys, xrid)
long snid, spno, *xrid;
int vote, timeout, *nsys;
struct synreq *synreq;
{
	register int rc;
	int ntmp;
	long xret;

	ntmp = nsys ? *nsys : 0;
	xret = -1L;

	rc = sd_async(snid, 0L, spno, vote, timeout, synreq, &ntmp, &xret);

	if (nsys)
		*nsys = ntmp;
	if (xrid && xret >= 0L)
		*xrid = xret;

	return(rc);
}

