/*
 *      SCCS:  @(#)xdtalk.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)xdtalk.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)xdtalk.c	1.2 92/09/30
NAME:		xdtalk.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	xresd communication functions

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <sys/types.h>
#include "dtmsg.h"
#include "ptab.h"
#include "avmsg.h"
#include "btmsg.h"
#include "valmsg.h"
#include "error.h"
#include "dtmac.h"
#include "servlib.h"
#include "dtetlib.h"

#ifndef NOTRACE
#include "ltoa.h"
#endif

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

extern struct ptab *Xd_ptab;		/* ptr to xresd ptab entry */
int xd_errno;				/* xd_talk reply code */

/* static function declarations */
#ifndef LOCAL_FUNCTION_DECL
static int xd_ptcheck();
#else /* __STDC__ */
static int xd_ptcheck();
#endif /* __STDC__ */


/*
**	xd_logon() - log on to the xresd
**
**	return 0 if successful or -1 on error
*/

int xd_logon()
{
	if (!Xd_ptab) {
		error(0, "don't know how to connect to xresd", (char *) 0);
		return(-1);
	}

	return(ti_logon(Xd_ptab));
}

/*
**	xd_logoff() - log off from the xresd
**
**	return 0 if successful or -1 on error
*/

int xd_logoff()
{
	register int rc;

	if (!Xd_ptab || (Xd_ptab->pt_flags & PF_LOGGEDON) == 0)
		return(0);

	rc = ti_logoff(Xd_ptab, 0);

	return(rc);
}

/*
**	xd_msgbuf() - return pointer to the xresd message data buffer at least
**	len bytes long, growing it if necessary
**
**	return (char *) 0 on error
*/

char *xd_msgbuf(len)
int len;
{
	return(xd_ptcheck() < 0 ? (char *) 0 : ti_msgbuf(Xd_ptab, len));
}

/*
**	xd_talk() - talk to the xresd, return a pointer to the reply message
**		data with the reply code in xd_errno
**
**	return (char *) 0 on error
*/

char *xd_talk(req, delay)
register int req;
int delay;
{
	register int len;

	TRACE3(Txresd, 1, "xdtalk: request = %s, delay = %s",
		ptreqcode(req), itoa(delay));

	if (xd_ptcheck() < 0) {
		xd_errno = ER_LOGON;
		return((char *) 0);
	}

	/* determine the size of the message data */
	switch (req) {
	case OP_XROPEN:
		len = avmsgsz(OP_XROPEN_ARGC);
		break;
	case OP_XRSYS:
		len = valmsgsz(((struct valmsg *) Xd_ptab->ptm_data)->vm_nvalue);
		break;
	case OP_ICSTART:
		len = valmsgsz(OP_ICSTART_NVALUE);
		break;
	case OP_TPSTART:
		len = valmsgsz(OP_TPSTART_NVALUE);
		break;
	case OP_ICEND:
		len = valmsgsz(OP_ICEND_NVALUE);
		break;
	case OP_TPEND:
		len = valmsgsz(OP_TPEND_NVALUE);
		break;
	case OP_TFOPEN:
		len = avmsgsz(OP_TFOPEN_ARGC);
		break;
	case OP_TFCLOSE:
		len = valmsgsz(OP_TFCLOSE_NVALUE);
		break;
	case OP_TFWRITE:
		len = BT_BTMSGSZ;
		break;
	case OP_FOPEN:
		len = avmsgsz(OP_FOPEN_ARGC);
		break;
	case OP_FCLOSE:
		len = valmsgsz(OP_FCLOSE_NVALUE);
		break;
	case OP_GETS:
		len = valmsgsz(OP_GETS_NVALUE);
		break;
	case OP_CFNAME:
		len = avmsgsz(OP_CFNAME_ARGC(XD_NCFNAME));
		break;
	case OP_CODESF:
		len = avmsgsz(OP_CODESF_ARGC(XD_NCODESF));
		break;
	case OP_RESULT:
		len = valmsgsz(OP_RESULT_NVALUE);
		break;
	case OP_XRES:
#if TESTING
	case OP_PRINT:
#endif
		len = avmsgsz(((struct avmsg *) Xd_ptab->ptm_data)->av_argc);
		break;
	case OP_RCFNAME:
	case OP_NULL:
		len = 0;
		break;
	default:
		error(0, "unknown XRESD request:", ptreqcode(req));
		xd_errno = ER_REQ;
		return((char *) 0);
	}

	/* send the message and receive a reply */
	Xd_ptab->ptm_req = req;
	Xd_ptab->ptm_len = len;
	if (ti_talk(Xd_ptab, delay) < 0) {
		xd_errno = ER_ERR;
		return((char *) 0);
	}

	/* return the reply code and message data */
	xd_errno = Xd_ptab->ptm_rc;
	return(Xd_ptab->ptm_data);
}

/*
**	xd_ptcheck() - check that there is an xresd ptab and that we are
**		logged on
**
**	return 0 if successful or -1 on error
*/

static int xd_ptcheck()
{
	if (!Xd_ptab) {
		error(0, "xresd ptab not initialised", (char *) 0);
		return(-1);
	}

	if ((Xd_ptab->pt_flags & PF_LOGGEDON) == 0) {
		error(0, "not logged on to xresd", (char *) 0);
		return(-1);
	}

	return(0);
}

