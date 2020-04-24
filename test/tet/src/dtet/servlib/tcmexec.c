/*
 *      SCCS:  @(#)tcmexec.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tcmexec.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tcmexec.c	1.2 92/09/30
NAME:		tcmexec.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	May 1992

DESCRIPTION:
	function to perform a remote execution request

MODIFICATIONS:

************************************************************************/

#include "avmsg.h"
#include "servlib.h"


/*
**	tc_mexec() - execute a unix process (eg: cp or find)
**		on a remote system
**
**	return pid of exec'd process if successful or -1 on error
*/

long tc_mexec(sysid, path, argv, outfile)
int sysid;
char *path, **argv, *outfile;
{
	return(tc_exec(sysid, path, argv, outfile, -1L, -1L, AV_EXEC_MISC));
}

