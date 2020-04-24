/*
 *      SCCS:  @(#)stcmdist.c	1.4 (92/11/10) 
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
static char sccsid[] = "@(#)stcmdist.c	1.4 (92/11/10) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)stcmdist.c	1.4 92/12/11
NAME:		stcmdist.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		David G. Sawyer, UniSoft Ltd.
DATE CREATED:	June 1992

DESCRIPTION:
	Slave TCM DTET-specific functions

MODIFICATIONS:
	Andrew Dingwall, UniSoft Ltd., October 1992
	Moved tet_xdline() and tet_xdresult() to apilib/dresfile.c.

	Moved all the rest of the functionality common to both
	MTCM and STCMs to ictp.c; now, all these routines are dummy
	equivalents to the MTCM-specific functions in mtcmdist.c.

************************************************************************/

#include "tcmfuncs.h"

void tet_opencom()
{
	/* nothing */
}

/* ARGSUSED */
void tet_xdicstart(icno, tpcount)
int icno, tpcount;
{
	/* nothing */
}

/* ARGSUSED */
void tet_xdicend(icno, tpcount)
int icno, tpcount;
{
	/* nothing */
}

/* ARGSUSED */
void tet_xdtpstart(icno, tpno)
int icno, tpno;
{
	/* nothing */
}

/* ARGSUSED */
void tet_xdtpend(icno, tpno)
int icno, tpno;
{
	/* nothing */
}

/* ARGSUSED */
void tet_tcmstart(versn, no_ics)
char *versn;
int no_ics;
{
	/* nothing */
}

