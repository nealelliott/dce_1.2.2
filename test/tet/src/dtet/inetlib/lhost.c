/*
 *      SCCS:  @(#)lhost.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)lhost.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)lhost.c	1.2 92/09/30
NAME:		lhost.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	INET localhost address lookup function

MODIFICATIONS:

************************************************************************/

#include <sys/types.h>
#include <netinet/in.h>
#include "inetlib_in.h"

/*
**	getlocalhostaddr() - return the INET address of localhost
*/

struct in_addr *getlocalhostaddr()
{
	return(gethostaddr("localhost"));
}

