/*
 *      SCCS:  @(#)codesfile.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)codesfile.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)codesfile.c	1.2 92/09/30
NAME:		codesfile.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		David Sawyer, UniSoft Ltd.
DATE CREATED:	August 1992

DESCRIPTION:
	tet result codes file name store and return

MODIFICATIONS:

************************************************************************/


#if __STDC__
#include <stdio.h>
#endif

#include <sys/types.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "avmsg.h"
#include "ltoa.h"
#include "xresd.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

static char *codesfile[XD_NCFNAME];	/* the stored config file names */

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#else
extern void free();
#endif


/*
**	op_codesf() - store a tet result codes file name
*/

void op_codesf(pp)
register struct ptab *pp;
{
	register struct avmsg *mp = (struct avmsg *) pp->ptm_data;
	register int n;

	/* all reply messages have no data */
	pp->ptm_len = 0;

	/* make sure that client is MTCC */
	if (pp->ptr_ptype != PT_MTCC) {
		pp->ptm_rc = ER_PERM;
		return;
	}

	/* do some sanity checks on the request message */
	if (OP_CODESF_NCODESF(mp) != XD_NCODESF) {
		pp->ptm_rc = ER_INVAL;
		return;
	}
	for (n = 0; n < XD_NCODESF; n++)
		if (!AV_CODESF(mp, n) || !*AV_CODESF(mp, n)) {
			pp->ptm_rc = ER_INVAL;
			return;
		}

	/* free the existing tet result codes file and store the new one */
	for (n = 0; n < XD_NCODESF; n++) {
		if (codesfile[n]) {
			TRACE3(Tbuf, 6, "free codesfile[%s] = %s",
				itoa(n), itox(codesfile[n]));
			free(codesfile[n]);
		}
		if ((codesfile[n] = strstore(AV_CODESF(mp, n))) == (char *) 0) {
			pp->ptm_rc = ER_ERR;
			return;
		}
		TRACE3(Txresd, 4, "receive result codes file name %s = \"%s\"",
			itoa(n), codesfile[n]);
	}

	/* all ok so return success */
	pp->ptm_rc = ER_OK;
}


/*
**	rcodesf() - return stored tet results codes file name
*/

char * rcodesf()
{

	/* make sure that we have a tet result codes file to return */
	if (!codesfile[XD_NCODESF - 1]) {
		return ((char *) 0);
	}
	else {
		TRACE2(Txresd, 4, "results codes file name = \"%s\"",
			codesfile[XD_NCODESF - 1]);

		return((char *) codesfile[XD_NCODESF - 1]);
	}
}

