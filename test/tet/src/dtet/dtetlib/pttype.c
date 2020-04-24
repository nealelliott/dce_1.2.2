/*
 *      SCCS:  @(#)pttype.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)pttype.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)pttype.c	1.2 92/09/30
NAME:		pttype.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	process table search function

MODIFICATIONS:

************************************************************************/

#include <sys/types.h>
#include "dtmsg.h"
#include "ptab.h"
#include "error.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	getptbyptype() - find a ptab element by ptype
**		and return a pointer thereto
**
**	return (struct ptab *) 0 if not found
*/

struct ptab *getptbyptype(ptype, start)
register int ptype;
struct ptab *start;
{
	register struct ptab *pp;

	for (pp = start; pp; pp = pp->pt_next) {
		ASSERT(pp->pt_magic == PT_MAGIC);
		if (pp->ptr_ptype == ptype)
			break;
	}

	return(pp);
}

