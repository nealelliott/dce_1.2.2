/*
 *      SCCS:  @(#)generror.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)generror.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)generror.c	1.2 92/09/30
NAME:		generror.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	generic error message printing function

MODIFICATIONS:

************************************************************************/

#include <stdio.h>
#include "dtetlib.h"

/*
**	generror() - generic error printing routine
*/

void generror(errnum, file, line, s1, s2)
int errnum, line;
char *file, *s1, *s2;
{
	extern char *Progname;

	prerror(stderr, errnum, Progname, file, line, s1, s2);
}

