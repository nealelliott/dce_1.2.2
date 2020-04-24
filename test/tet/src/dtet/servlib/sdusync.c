/*
 *      SCCS:  @(#)sdusync.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)sdusync.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)sdusync.c	1.2 92/09/30
NAME:		sdusync.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	user-sync request function

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include "dtmsg.h"
#include "valmsg.h"
#include "synreq.h"
#include "error.h"
#include "servlib.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	sd_usync() - perform a user-sync request
**
**	return 0 if successful or -1 on error
**
**	if successful, the sysid and sync point numbers (for ER_OK)
**	or sync states (for ER_SYNCERR or ER_TIMEDOUT) are stored in the
**	array at *synreq which contains nsys elements
*/

int sd_usync(snid, xrid, spno, vote, timeout, synreq, nsys)
long snid, xrid, spno;
int vote, timeout, nsys;
struct synreq *synreq;
{
	register struct valmsg *mp;
	register struct synreq *sp;
	register int n;
	extern char Sderrmsg[];

	/* get the SYNCD message buffer */
	if ((mp = (struct valmsg *) sd_msgbuf(valmsgsz(OP_AUSYNC_NVALUE(nsys)))) == (struct valmsg *) 0) {
		sd_errno = ER_ERR;
		return(-1);
	}

	/* construct the message */
	mp->vm_nvalue = OP_AUSYNC_NVALUE(nsys);
	VM_SNID(mp) = snid;
	VM_XRID(mp) = xrid;
	VM_SPNO(mp) = spno;
	VM_SVOTE(mp) = (long) vote;
	VM_STIMEOUT(mp) = (long) timeout;
	for (n = 0, sp = synreq; n < nsys; n++, sp++) {
		VM_SSYSID(mp, n) = (long) sp->sy_sysid;
		VM_SPTYPE(mp, n) = (long) sp->sy_ptype;
	}

	/* perform the conversation */
	if (timeout >= 0) {
		if ((n = TALK_DELAY + timeout) < 0)
			n = (int) ((unsigned) ~0 >> 1);
	}
	else
		n = 0;
	mp = (struct valmsg *) sd_talk(OP_USYNC, n);

	/* handle the reply codes */
	switch (sd_errno) {
	case ER_OK:
	case ER_SYNCERR:
	case ER_TIMEDOUT:
		break;
	case ER_INVAL:
	case ER_DONE:
	case ER_DUPS:
		return(-1);
	case ER_ERR:
		if (!mp)
			return(-1);
		/* else fall through */
	default:
		error(0, Sderrmsg, ptrepcode(sd_errno));
		return(-1);
	}

	/* here to build the synreq list for a normal return */
	for (n = 0, sp = synreq; n < nsys && n < OP_AUSYNC_NSYS(mp); n++, sp++) {
		sp->sy_sysid = (int) VM_SSYSID(mp, n);
		switch (sd_errno) {
		case ER_OK:
			sp->sy_spno = VM_RSPNO(mp, n);
			break;
		case ER_SYNCERR:
		case ER_TIMEDOUT:
			sp->sy_state = (int) VM_STATE(mp, n);
			break;
		}
	}

	return(0);
}

