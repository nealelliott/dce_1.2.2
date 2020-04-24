/*
 *      SCCS:  @(#)dtsize.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)dtsize.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)dtsize.c	1.2 92/09/30
NAME:		dtsize.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to determine size of descriptor table

MODIFICATIONS:

************************************************************************/


#include <stdio.h>
#include "dtetlib.h"

#ifdef SVR4
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "error.h"
#endif


#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	dogetdtablesize() - return size of file descriptor table
*/

int dogetdtablesize()
{
	register int rc;

#ifdef SVR4

	struct rlimit rlimit;

	if (getrlimit(RLIMIT_NOFILE, &rlimit) < 0) {
		error(errno, "getrlimit(RLIMIT_NOFILE) failed", (char *) 0);
		rc = _NFILE;
	}
	else
		rc = (int) rlimit.rlim_cur;
#else /* SVR4 */
#ifdef __hpux

	rc = _NFILE;

#else /* __hpux */

	rc = getdtablesize();

#endif /* __hpux */
#endif /* SVR4 */

	return(rc);
}

