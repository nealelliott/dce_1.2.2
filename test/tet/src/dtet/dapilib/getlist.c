/*
 *      SCCS:  @(#)getlist.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)getlist.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)getlist.c	1.2 92/09/30
NAME:		getlist.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

SYNOPSIS:
	#include <dapi.h>
	int tet_remgetlist(int **sysnames);

DESCRIPTION:
	DTET API function

	return number of currently available slave systems or -1 on error

	if successful, a pointer to a list of the (numeric) system names is
	returned indirectly through *sysnames

MODIFICATIONS:

************************************************************************/


#if __STDC__
#include <stdio.h>
#endif

#include "dtmac.h"
#include "dtetlib.h"
#include "dapi.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif


/*
**	tet_remgetlist() - return the number of available slave systems
**		or -1 on error
**
**	if successful, and there is at least one slave system available,
**	a pointer to the slave system name list is returned
**	indirectly through *sysnames
*/

int tet_remgetlist(sysnames)
int **sysnames;
{
	register int n, nsys, *ip1, *ip2;
	static int *snames;
	static int slen;
	static int nsname = -1;
	extern int *Snames, Nsname;

	/* initialise the slave system list first time through */
	if (nsname < 0) {
		nsys = 0;
		for (n = 0, ip1 = Snames; n < Nsname; n++, ip1++)
			if (*ip1 > 0)
				nsys++;
		if ((n = nsys * sizeof *snames) > 0) {
			if (BUFCHK((char **) &snames, &slen, n) < 0)
				return(-1);
			ip2 = snames;
			for (n = 0, ip1 = Snames; n < Nsname; n++, ip1++)
				if (*ip1 > 0)
					*ip2++ = *ip1;
		}
		nsname = nsys;
	}

	if (sysnames && nsname > 0)
		{
		/*
 		 * Current bug in TET that the syncnames is not 0 terminated this can cause
 		 * the mesage dupilcate names in sync list
 	         */
		snames[nsname] = 0;

		*sysnames = snames;
		}

	return(nsname);
}

