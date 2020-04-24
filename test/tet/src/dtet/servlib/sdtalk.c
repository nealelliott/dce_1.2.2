/*
 *      SCCS:  @(#)sdtalk.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)sdtalk.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)sdtalk.c	1.2 92/09/30
NAME:		sdtalk.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	syncd communication functions

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <sys/types.h>
#include "dtmsg.h"
#include "ptab.h"
#include "error.h"
#include "dtmac.h"
#include "valmsg.h"
#include "servlib.h"
#include "dtetlib.h"

#ifndef NOTRACE
#include "ltoa.h"
#endif

#if TESTING
#include "avmsg.h"
#endif

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

extern struct ptab *Sd_ptab;		/* ptr to syncd ptab entry */
int sd_errno;				/* sd_talk reply code */

/* static function declarations */
#if __STDC__
static int sd_ptcheck();
#else /* __STDC__ */
static int sd_ptcheck();
#endif /* __STDC__ */


/*
**	sd_logon() - log on to syncd
**
**	return 0 if successful or -1 on error
*/

int sd_logon()
{
	if (!Sd_ptab) {
		error(0, "don't know how to connect to syncd", (char *) 0);
		return(-1);
	}

	return(ti_logon(Sd_ptab));
}

/*
**	sd_logoff() - log off from the syncd
**
**	if stayopen is non-zero, the connection to syncd is left open;
**	otherwise it is closed
**
**	return 0 if successful or -1 on error
*/

int sd_logoff(stayopen)
int stayopen;
{
	register int rc;

	if (!Sd_ptab || (Sd_ptab->pt_flags & PF_LOGGEDON) == 0)
		return(0);

	rc = ti_logoff(Sd_ptab, stayopen);

	return(rc);
}

/*
**	sd_msgbuf() - return pointer to syncd message data buffer at least len
**	bytes long, growing it if necessary
**
**	return (char *) 0 on error
*/

char *sd_msgbuf(len)
int len;
{
	return(sd_ptcheck() < 0 ? (char *) 0 : ti_msgbuf(Sd_ptab, len));
}

/*
**	sd_talk() - talk to syncd, return a pointer to the reply message data
**		with the reply code in sd_errno
**
**	return (char *) 0 on error
*/

char *sd_talk(req, delay)
register int req;
int delay;
{
	register int len;

	TRACE3(Tsyncd, 1, "sdtalk: request = %s, delay = %s",
		ptreqcode(req), itoa(delay));

	if (sd_ptcheck() < 0) {
		sd_errno = ER_LOGON;
		return((char *) 0);
	}

	/* determine the size of the message data */
	switch (req) {
	case OP_SNSYS:
	case OP_ASYNC:
	case OP_USYNC:
		len = valmsgsz(((struct valmsg *) Sd_ptab->ptm_data)->vm_nvalue);
		break;
	case OP_SNGET:
	case OP_NULL:
		len = 0;
		break;
#if TESTING
	case OP_PRINT:
		len = avmsgsz(((struct avmsg *) Sd_ptab->ptm_data)->av_argc);
		break;
#endif
	default:
		error(0, "unknown SYNCD request:", ptreqcode(req));
		sd_errno = ER_REQ;
		return((char *) 0);
	}

	/* send the message and receive a reply */
	Sd_ptab->ptm_req = req;
	Sd_ptab->ptm_len = len;
	if (ti_talk(Sd_ptab, delay) < 0) {
		sd_errno = ER_ERR;
		return((char *) 0);
	}

	/* return the reply code and message data */
	sd_errno = Sd_ptab->ptm_rc;
	return(Sd_ptab->ptm_data);
}

/*
**	sd_ptcheck() - check that there is a syncd ptab and that we are
**		logged on
**
**	return 0 if successful or -1 on error
*/

static int sd_ptcheck()
{
	if (!Sd_ptab) {
		error(0, "syncd ptab not initialised", (char *) 0);
		return(-1);
	}

	if ((Sd_ptab->pt_flags & PF_LOGGEDON) == 0) {
		error(0, "not logged on to syncd", (char *) 0);
		return(-1);
	}

	return(0);
}

