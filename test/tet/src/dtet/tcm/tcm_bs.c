/*
 *      SCCS:  @(#)tcm_bs.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tcm_bs.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tcm_bs.c	1.2 92/09/30
NAME:		tcm_bs.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	tcm-specific functions to convert DTET interprocess messages between
	machine-independent and internal format

MODIFICATIONS:

************************************************************************/


#if __STDC__
#include <stdio.h>
#endif

#include <sys/types.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "error.h"
#include "valmsg.h"
#include "avmsg.h"
#include "server_bs.h"
#include "dtetlib.h"
#include "tcmfuncs.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

static char reqerr[] = "unknown request code";

/*
**	ss_bs2md() - convert message data to internal format
**
**	return length of internal-format message, or -ve error code on error
*/

int ss_bs2md(from, pp)
char *from;
register struct ptab *pp;
{
	register int rc;
	register int request = pp->pt_savreq;

	switch (request) {
	case OP_EXEC:
	case OP_WAIT:
	case OP_KILL:
	case OP_XROPEN:
	case OP_SNGET:
	case OP_ASYNC:
	case OP_USYNC:
	case OP_FOPEN:
		rc = bs2valmsg(from, pp->ptm_len,
			(struct valmsg **) &pp->ptm_data, &pp->pt_mdlen);
		break;
	case OP_RCFNAME:
	case OP_GETS:
	case OP_RCVCONF:
	case OP_SHARELOCK:
		rc = bs2avmsg(from, pp->ptm_len,
			(struct avmsg **) &pp->ptm_data, &pp->pt_mdlen);
		break;
	default:
		error(0, reqerr, ptreqcode(request));
		return(ER_REQ);
	}

	return(rc < 0 ? ER_ERR : rc);
}

/*
**	ss_md2bs() - convert message data to machine-independent format
**
**	return length of machine-independent message
**	or -ve error code on error
*/

int ss_md2bs(pp, bp, lp, offs)
struct ptab *pp;
char **bp;
int *lp, offs;
{
	register char *mp = pp->ptm_data;
	register int request = pp->ptm_req;
	register int len, rc;

	/* calculate outgoing data size */
	switch (request) {
	case OP_SYSID:
	case OP_SYSNAME:
	case OP_SNSYS:
	case OP_XRSYS:
	case OP_ICSTART:
	case OP_TPSTART:
	case OP_ICEND:
	case OP_TPEND:
	case OP_RESULT:
	case OP_ASYNC:
	case OP_USYNC:
	case OP_FCLOSE:
	case OP_GETS:
	case OP_WAIT:
	case OP_KILL:
		len = VM_VALMSGSZ(((struct valmsg *) mp)->vm_nvalue);
		break;
	case OP_TRACE:
	case OP_EXEC:
	case OP_XROPEN:
	case OP_XRES:
	case OP_FOPEN:
	case OP_CFNAME:
	case OP_SNDCONF:
	case OP_CONFIG:
	case OP_LOCKFILE:
	case OP_SHARELOCK:
#if TESTING
	case OP_PRINT:
#endif
		len = avmsgbslen((struct avmsg *) mp);
		break;
	case OP_TSINFO:
		len = ts_tsinfolen();
		break;
	default:
		error(0, reqerr, ptreqcode(request));
		return(ER_REQ);
	}

	/* make sure that the receiving area is big enough */
	if (BUFCHK(bp, lp, len + offs) < 0)
		return(ER_ERR);

	/* copy the data to (*bp + offs) */
	switch (request) {
	case OP_SYSID:
	case OP_SYSNAME:
	case OP_SNSYS:
	case OP_XRSYS:
	case OP_ICSTART:
	case OP_TPSTART:
	case OP_ICEND:
	case OP_TPEND:
	case OP_RESULT:
	case OP_ASYNC:
	case OP_USYNC:
	case OP_FCLOSE:
	case OP_GETS:
	case OP_WAIT:
	case OP_KILL:
		rc = valmsg2bs((struct valmsg *) mp, *bp + offs);
		break;
	case OP_EXEC:
	case OP_TRACE:
	case OP_XROPEN:
	case OP_XRES:
	case OP_FOPEN:
	case OP_CFNAME:
	case OP_SNDCONF:
	case OP_CONFIG:
	case OP_LOCKFILE:
	case OP_SHARELOCK:
#if TESTING
	case OP_PRINT:
#endif
		rc = avmsg2bs((struct avmsg *) mp, *bp + offs);
		break;
	case OP_TSINFO:
		rc = ts_tsinfo2bs(mp, *bp + offs);
		break;
	default:
		error(0, reqerr, ptreqcode(request));
		return(ER_REQ);
	}

	return(rc < 0 ? ER_ERR : rc);
}

