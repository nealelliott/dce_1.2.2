/*
 *      SCCS:  @(#)xdictp.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)xdictp.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)xdictp.c	1.2 92/09/30
NAME:		xdictp.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	May 1992

DESCRIPTION:
	functions to signal IC start/end and TP start/end to xresd

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include "dtmsg.h"
#include "valmsg.h"
#include "error.h"
#include "servlib.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif


/*
**	xd_icstart() - send an OP_ICSTART message to XRESD and receive a reply
**
**	return 0 if successful or -1 on error
*/

int xd_icstart(xrid, icno, activity, tpcount)
long xrid, activity;
int icno, tpcount;
{
	register struct valmsg *mp;
	extern char Xderrmsg[];

	/* get the XRESD message buffer */
	if ((mp = (struct valmsg *) xd_msgbuf(valmsgsz(OP_ICSTART_NVALUE))) == (struct valmsg *) 0) {
		xd_errno = ER_ERR;
		return(-1);
	}

	/* set up the request message */
	mp->vm_nvalue = OP_ICSTART_NVALUE;
	VM_XRID(mp) = xrid;
	VM_ICNO(mp) = (long) icno;
	VM_ACTIVITY(mp) = activity;
	VM_TPCOUNT(mp) = (long) tpcount;

	/* send the request and receive the reply */
	mp = (struct valmsg *) xd_talk(OP_ICSTART, TALK_DELAY);

	/* handle the return codes */
	switch (xd_errno) {
	case ER_OK:
		return(0);
	case ER_PERM:
	case ER_XRID:
	case ER_SYSID:
	case ER_INPROGRESS:
		break;
	case ER_ERR:
		if (!mp)
			break;
		/* else fall through */
	default:
		error(0, Xderrmsg, ptrepcode(xd_errno));
		break;
	}

	/* here for server error return */
	return(-1);
}

/*
**	xd_icend() - send an OP_ICEND message to XRESD and receive a reply
**
**	return 0 if successful or -1 on error
*/

int xd_icend(xrid)
long xrid;
{
	register struct valmsg *mp;
	extern char Xderrmsg[];

	/* get the XRESD message buffer */
	if ((mp = (struct valmsg *) xd_msgbuf(valmsgsz(OP_ICEND_NVALUE))) == (struct valmsg *) 0) {
		xd_errno = ER_ERR;
		return(-1);
	}

	/* set up the request message */
	mp->vm_nvalue = OP_ICEND_NVALUE;
	VM_XRID(mp) = xrid;

	/* send the request and receive the reply */
	mp = (struct valmsg *) xd_talk(OP_ICEND, TALK_DELAY);

	/* handle the return codes */
	switch (xd_errno) {
	case ER_OK:
		return(0);
	case ER_PERM:
	case ER_XRID:
	case ER_INPROGRESS:
	case ER_DONE:
		break;
	case ER_ERR:
		if (!mp)
			break;
		/* else fall through */
	default:
		error(0, Xderrmsg, ptrepcode(xd_errno));
		break;
	}

	/* here for server error return */
	return(-1);
}

/*
**	xd_tpstart() - send an OP_TPSTART message to XRESD and receive a reply
**
**	return 0 if successful or -1 on error
*/

int xd_tpstart(xrid, tpno)
long xrid;
int tpno;
{
	register struct valmsg *mp;
	extern char Xderrmsg[];

	/* get the XRESD message buffer */
	if ((mp = (struct valmsg *) xd_msgbuf(valmsgsz(OP_TPSTART_NVALUE))) == (struct valmsg *) 0) {
		xd_errno = ER_ERR;
		return(-1);
	}

	/* set up the request message */
	mp->vm_nvalue = OP_TPSTART_NVALUE;
	VM_XRID(mp) = xrid;
	VM_TPNO(mp) = (long) tpno;

	/* send the request and receive the reply */
	mp = (struct valmsg *) xd_talk(OP_TPSTART, TALK_DELAY);

	/* handle the return codes */
	switch (xd_errno) {
	case ER_OK:
		return(0);
	case ER_PERM:
	case ER_XRID:
	case ER_INPROGRESS:
	case ER_DONE:
		break;
	case ER_ERR:
		if (!mp)
			break;
		/* else fall through */
	default:
		error(0, Xderrmsg, ptrepcode(xd_errno));
		break;
	}

	/* here for server error return */
	return(-1);
}

/*
**	xd_tpend() - send an OP_TPEND message to XRESD and receive a reply
**
**	return 0 if successful or -1 on error
*/

int xd_tpend(xrid)
long xrid;
{
	register struct valmsg *mp;
	extern char Xderrmsg[];

	/* get the XRESD message buffer */
	if ((mp = (struct valmsg *) xd_msgbuf(valmsgsz(OP_TPEND_NVALUE))) == (struct valmsg *) 0) {
		xd_errno = ER_ERR;
		return(-1);
	}

	/* set up the request message */
	mp->vm_nvalue = OP_TPEND_NVALUE;
	VM_XRID(mp) = xrid;

	/* send the request and receive the reply */
	mp = (struct valmsg *) xd_talk(OP_TPEND, TALK_DELAY);

	/* handle the return codes */
	switch (xd_errno) {
	case ER_OK:
		return(0);
	case ER_PERM:
	case ER_XRID:
	case ER_DONE:
		break;
	case ER_ERR:
		if (!mp)
			break;
		/* else fall through */
	default:
		error(0, Xderrmsg, ptrepcode(xd_errno));
		break;
	}

	/* here for server error return */
	return(-1);
}

