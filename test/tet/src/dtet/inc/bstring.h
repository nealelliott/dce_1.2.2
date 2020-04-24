/*
 *      SCCS:  @(#)bstring.h	1.2 (92/09/30) 
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

/************************************************************************

SCCS:   	@(#)bstring.h	1.2 92/09/30 DTET release 1.0b
NAME:		bstring.h
PRODUCT:	DTET (Distributed Test Environment Toolkit)
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	definitions of the BSD bstring(3C) routines in terms of the
	SYSV memory(3C) routines

MODIFICATIONS:

************************************************************************/


/*
**	for HP/UX -
**	the bstring stuff is in libBSD.a, but so is a BSD setpgrp()
**	we need a SYSV setpgrp() (to detach from the controlling terminal)
**	because TIOCNOTTY is ifdefed out in <sys/ioctl.h>
**	(see notty.c)
*/

#ifdef __hpux
#define NOBSTRING
#endif

/*
**	for SVR4 -
**	the bstring stuff is in libucb.a, but so is a lot of other stuff
**	that seems not to work very well
**	it seems best to avoid libucb.a if at all possible
*/

#ifdef SVR4
#define NOBSTRING
#endif


#ifdef NOBSTRING
#if __STDC__
#include <sys/types.h>
#endif /* __STDC__ */
#include <memory.h>
#define bcopy(from, to, count)	((void) memcpy(to, from, count))
#define bzero(p, len)		((void) memset(p, '\0', len))
#endif /* NOBSTRING */

