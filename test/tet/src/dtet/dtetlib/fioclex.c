/*
 *      SCCS:  @(#)fioclex.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)fioclex.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)fioclex.c	1.2 92/09/30
NAME:		fioclex.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to set the close-on-exec bit on a file descriptor

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <fcntl.h>
#include <errno.h>
#include "ltoa.h"
#include "error.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	fioclex() - set the close-on-exec bit on a file descriptor
**
**	return 0 if successful or -1 on error
*/

int fioclex(fd)
int fd;
{
	if (fcntl(fd, F_SETFD, 1) < 0) {
		error(errno, "can't set close-on-exec flag on fd", itoa(fd));
		return(-1);
	}

	return(0);
}

