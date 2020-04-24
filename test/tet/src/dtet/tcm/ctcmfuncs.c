/*
 *      SCCS:  @(#)ctcmfuncs.c	1.1 (92/10/02) 
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
static char sccsid[] = "@(#)ctcmfuncs.c	1.1 (92/10/02) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)ctcmfuncs.c	1.1 92/10/02
NAME:		ctcmfuncs.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	October 1992

DESCRIPTION:
	client-related functions for processes started by tet_exec()

MODIFICATIONS:

************************************************************************/

#include "error.h"
#include "dtmsg.h"
#include "synreq.h"
#include "tcmfuncs.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;
#endif


/*
**	tet_callfuncname() - return name of tcmchild's calling function
**		for use in error messages
*/

char *tet_callfuncname()
{
	return("tet_exec()");
}

/*
**	tcmptype() - return process type for child TCM
**
**	this function is called from tcminit() after Mysysid has been assigned
*/

int tcmptype()
{
	extern int Mysysid;

	return(Mysysid == 0 ? PT_MTCM : PT_STCM);
}

/*
**	doasync() - dummy auto-sync function for child TCMs
**
**	child TCMs do not do auto-syncs, so this function should only be
**	called with spno == SV_EXEC_SPNO from tcminit()
*/

/* ARGSUSED */
int doasync(spno, vote, timeout, synreq, nsys)
long spno;
int vote, timeout, *nsys;
struct synreq *synreq;
{
	extern int sd_errno;

	ASSERT(spno == SV_EXEC_SPNO);

	sd_errno = ER_OK;
	return(0);
}

