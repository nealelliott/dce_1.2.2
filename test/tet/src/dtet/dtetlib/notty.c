/*
 *      SCCS:  @(#)notty.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)notty.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)notty.c	1.2 92/09/30
NAME:		notty.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to dissociate from control terminal and start a new process
	group

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <sys/ioctl.h>
#include "dtetlib.h"

#ifdef TIOCNOTTY
#include <fcntl.h>
#endif

/*
**	tiocnotty() - dissociate from control terminal
**		and start a new process group
*/

void tiocnotty()
{
#ifdef TIOCNOTTY
	int ttyfd;
#endif

#if defined(SVR2) || defined(SVR3) || defined(SVR4) || defined(__hpux)
	(void) setpgrp();
#else
	int pid = getpid();
	(void) setpgrp(pid, pid);
#endif


#ifdef TIOCNOTTY
	/* this for BSD systems where setpgrp() does not change the
		control terminal */
	if ((ttyfd = open("/dev/tty", O_RDONLY | O_NDELAY)) >= 0) {
		(void) ioctl(ttyfd, TIOCNOTTY, 0);
		(void) close(ttyfd);
	}
#endif
}

