/*
 *      SCCS:  @(#)fake.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)fake.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)fake.c	1.2 92/09/30
NAME:		fake.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	fake server-specific functions for syncd

MODIFICATIONS:

************************************************************************/

#include <sys/types.h>
#include "dtmsg.h"
#include "ptab.h"
#include "error.h"
#include "server.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	ss_connect() - fake connect routine for syncd
**
**	syncd is never a client, so never connects to other processes
*/

void ss_connect(pp)
struct ptab *pp;
{
	error(0, "internal error - connect called", rtoa(&pp->pt_rid));
}

