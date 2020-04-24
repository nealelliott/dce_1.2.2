/*
 *      SCCS:  @(#)tcuexec.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tcuexec.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tcuexec.c	1.2 92/09/30
NAME:		tcuexec.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	May 1992

DESCRIPTION:
	function to perform a user execution request on a remote system

MODIFICATIONS:

************************************************************************/

#include "avmsg.h"
#include "servlib.h"

/*
**	tc_uexec() - execute a user process (as from tet_remexec())
**		on a remote system
**
**	return pid of exec'd process if successful or -1 on error
*/

long tc_uexec(sysid, path, argv, snid, xrid)
int sysid;
char *path, **argv;
long snid, xrid;
{
	return(tc_exec(sysid, path, argv, (char *) 0, snid, xrid, AV_EXEC_USER));
}

