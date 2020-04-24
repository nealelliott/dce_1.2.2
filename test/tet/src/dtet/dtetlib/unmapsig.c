/*
 *      SCCS:  @(#)unmapsig.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)unmapsig.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)unmapsig.c	1.2 92/09/30
NAME:		unmapsig.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to convert machine-independent signal number to local
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
**	unmapsignal() - map DTET signal number to local signal number
**
**	return local signal number if successful or -1 on error
*/

int unmapsignal(sig)
register int sig;
{
	register struct sigmap *sp, *se;
	extern struct sigmap Sigmap[];
	extern int Nsigmap;

	/* if the local signal number is the same as the DTET one, use that;
		otherwise we have to search the whole table */
	if (sig >= 0 && sig < Nsigmap && sig == Sigmap[sig].sig_local)
		return(sig);
	else
		for (sp = Sigmap, se = sp + Nsigmap; sp < se; sp++)
			if (sp->sig_dtet == sig)
				return(sp->sig_local);

	error(0, "no local equivalent to DTET signal", itoa(sig));
	return(-1);
}

