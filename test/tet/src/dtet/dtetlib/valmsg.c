/*
 *      SCCS:  @(#)valmsg.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)valmsg.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)valmsg.c	1.2 92/09/30
NAME:		valmsg.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	function to convert DTET interprocess numeric value message between
	internal and machine-independent format

MODIFICATIONS:

************************************************************************/


#if __STDC__
#include <stdio.h>
#endif

#include "dtmac.h"
#include "ldst.h"
#include "dtmsg.h"
#include "valmsg.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

static struct stdesc st[] = {
	VALMSG_DESC
};

static short nst = -1;
static short fixed = -1;

/* static function declarations */
#if __STDC__
static void stinit();
#else /* __STDC__ */
static void stinit();
#endif /* __STDC__ */

static void stinit()
{
	register struct valmsg *sp = (struct valmsg *) 0;
	register int n = 0;

	VALMSG_INIT(st, sp, n, nst, fixed);
}

/*
**	valmsg2bs() - convert a valmsg to machine-independent format
**
**	return the number of bytes occupied by the result
*/

int valmsg2bs(from, to)
register struct valmsg *from;
register char *to;
{
	register int count;
	struct stdesc tmp;

	if (nst < 0)
		stinit();

	/* convert the fixed part */
	count = st2bs((char *) from, to, st, fixed);

	/* then convert the variable part */
	tmp = st[fixed];
	tmp.st_type = (tmp.st_type & ST_TYPEMASK) | (from->vm_nvalue & ST_COUNTMASK);
	count += st2bs((char *) from, to + VM_VALUESTART, &tmp, 1);

	return(count);
}

/*
**	bs2valmsg() - convert a valmsg message to internal format
**
**	return the number of bytes in the valmsg result, or -1 on error
*/

int bs2valmsg(from, fromlen, to, tolen)
register char *from;
register int fromlen;
register struct valmsg **to;
register int *tolen;
{
	struct stdesc tmp;

	if (nst < 0)
		stinit();

	/* make sure that the buffer is big enough for a minimal message */
	if (BUFCHK((char **) to, tolen, valmsgsz(1)) < 0)
		return(-1);

	/* convert the fixed part */
	if (bs2st(from, (char *) *to, st, fixed,
		MIN(fromlen, VM_VALUESTART)) < 0)
			return(-1);

	/* make sure that the buffer is big enough for the actual message */
	if ((int) (*to)->vm_nvalue > 1 &&
		BUFCHK((char **) to, tolen, valmsgsz((*to)->vm_nvalue)) < 0)
			return(-1);

	/* then convert the variable part */
	tmp = st[fixed];
	tmp.st_type = (tmp.st_type & ST_TYPEMASK) | ((*to)->vm_nvalue & ST_COUNTMASK);
	if (bs2st(from + VM_VALUESTART, (char *) *to,
		&tmp, 1, fromlen - VM_VALUESTART) < 0)
			return(-1);

	/* return the number of bytes in the result */
	return(valmsgsz((*to)->vm_nvalue));
}

