/*
 *      SCCS:  @(#)cfname.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)cfname.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)cfname.c	1.2 92/09/30
NAME:		cfname.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	config file name store and return

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

static char *cfname[XD_NCFNAME];	/* the stored config file names */

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#else
extern void free();
#endif


/*
**	op_cfname() - store a config file name
*/

void op_cfname(pp)
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
	if (OP_CFNAME_NCFNAME(mp) != XD_NCFNAME) {
		pp->ptm_rc = ER_INVAL;
		return;
	}
	for (n = 0; n < XD_NCFNAME; n++)
		if (!AV_CFNAME(mp, n) || !*AV_CFNAME(mp, n)) {
			pp->ptm_rc = ER_INVAL;
			return;
		}

	/* free any existing cfnames and store the new ones */
	for (n = 0; n < XD_NCFNAME; n++) {
		if (cfname[n]) {
			TRACE3(Tbuf, 6, "free cfname[%s] = %s",
				itoa(n), itox(cfname[n]));
			free(cfname[n]);
		}
		if ((cfname[n] = strstore(AV_CFNAME(mp, n))) == (char *) 0) {
			pp->ptm_rc = ER_ERR;
			return;
		}
		TRACE3(Txresd, 4, "receive config file name %s = \"%s\"",
			itoa(n), cfname[n]);
	}

	/* all ok so return success */
	pp->ptm_rc = ER_OK;
}

/*
**	op_rcfname() - return stored config file name
*/

void op_rcfname(pp)
register struct ptab *pp;
{
	register struct avmsg *rp;
	register int n;

	/* make sure that we have some cfnames to return */
	if (!cfname[XD_NCFNAME - 1]) {
		pp->ptm_rc = ER_CONTEXT;
		pp->ptm_len = 0;
		return;
	}

	/* make sure that the message buffer is big enough for the reply */
	if (BUFCHK(&pp->ptm_data, &pp->pt_mdlen, avmsgsz(OP_CFNAME_ARGC(XD_NCFNAME))) < 0) {
		pp->ptm_rc = ER_ERR;
		pp->ptm_len = 0;
		return;
	}
	rp = (struct avmsg *) pp->ptm_data;

	/* fill in the reply message and return */
	rp->av_argc = OP_CFNAME_ARGC(XD_NCFNAME);
	for (n = 0; n < XD_NCFNAME; n++) {
		TRACE3(Txresd, 4, "return config file name %s = \"%s\"",
			itoa(n), cfname[n]);
		AV_CFNAME(rp, n) = cfname[n];
	}
	pp->ptm_rc = ER_OK;
	pp->ptm_len = avmsgsz(OP_CFNAME_ARGC(XD_NCFNAME));
}

