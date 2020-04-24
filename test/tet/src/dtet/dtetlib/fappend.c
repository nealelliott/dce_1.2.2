/*
 *      SCCS:  @(#)fappend.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)fappend.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)fappend.c	1.2 92/09/30
NAME:		fappend.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to set append mode on a file

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <errno.h>
#include <fcntl.h>
#include "error.h"
#include "ltoa.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	fappend() - set append mode on a file
**
**	return 0 if successful or -1 on error
*/

int fappend(fd)
int fd;
{
	register int flags;

	if ((flags = fcntl(fd, F_GETFL, 0)) < 0) {
		error(errno, "can't get file status flags for fd", itoa(fd));
		return(-1);
	}

#ifdef FAPPEND
	/* BSD style */
	flags |= FAPPEND;
#else
	/* SYSV style */
	flags |= O_APPEND;
#endif

	if (fcntl(fd, F_SETFL, flags) < 0) {
		error(errno, "can't set file status flags on fd", itoa(fd));
		return(-1);
	}

	return(0);
}

