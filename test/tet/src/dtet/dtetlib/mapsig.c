/*
 *      SCCS:  @(#)mapsig.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)mapsig.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)mapsig.c	1.2 92/09/30
NAME:		mapsig.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to convert local signal number to machine-independent
	signal number

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include "sigmap.h"
#include "error.h"
#include "ltoa.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	mapsignal() - map local signal number to DTET signal number
**
**	return DTET signal number if successful or -1 on error
*/

int mapsignal(sig)
register int sig;
{
	register struct sigmap *sp, *se;
	extern struct sigmap Sigmap[];
	extern int Nsigmap;

	/* if the DTET signal number is the same as the local one, use that;
		otherwise we have to search the whole table */
	if (sig >= 0 && sig < Nsigmap && sig == Sigmap[sig].sig_dtet)
		return(sig);
	else
		for (sp = Sigmap, se = sp + Nsigmap; sp < se; sp++)
			if (sp->sig_local == sig)
				return(sp->sig_dtet);

	error(0, "local signal not found in sigmap:", itoa(sig));
	return(-1);
}

