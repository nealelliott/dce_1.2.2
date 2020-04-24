/*
 *      SCCS:  @(#)sysbyid.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)sysbyid.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)sysbyid.c	1.2 92/09/30
NAME:		sysbyid.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	systems file search function

MODIFICATIONS:

************************************************************************/

#include <errno.h>
#include "sysent.h"

/*
**	getsysbyid() - get systems file entry for sysid and return a pointer
**		thereto
**
**	return (struct sysent *) 0 on if not found or on error
*/

struct sysent *getsysbyid(sysid)
register int sysid;
{
	static struct sysent *sp;

	if (setsysent() < 0)
		return((struct sysent *) 0);

	errno = 0;
	do {
		if (sp && sp->sy_sysid == sysid)
			break;
	} while ((sp = getsysent()) != (struct sysent *) 0);

	return(sp);
}

