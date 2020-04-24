/*
 *      SCCS:  @(#)fionbio.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)fionbio.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)fionbio.c	1.2 92/09/30
NAME:		fionbio.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to establish non-blocking i/o on a file descriptor

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <errno.h>
#include "ltoa.h"
#include "error.h"
#include "dtetlib.h"

#ifdef SVR4
#include <sys/filio.h>
#else
#include <sys/ioctl.h>
#endif

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	fionbio() - establish non-blocking i/o on a file descriptor
**
**	return 0 if successful or -1 on error
*/

int fionbio(fd)
int fd;
{
	int arg;

	arg = 1;
	if (ioctl(fd, FIONBIO, &arg) < 0) {
		error(errno, "ioctl(FIONBIO) failed on fd", itoa(fd));
		return(-1);
	}

	return(0);
}

