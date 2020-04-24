/*
 *      SCCS:  @(#)tctalk.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tctalk.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tctalk.c	1.2 92/09/30
NAME:		tctalk.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	tccd communication functions

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <sys/types.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "valmsg.h"
#include "avmsg.h"
#include "error.h"
#include "ltoa.h"
#include "server.h"
#include "tslib.h"
#include "servlib.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

int tc_errno;				/* tc_talk message reply code */

/* extern function declarations */
#ifndef LOCAL_FUNCTION_DECL
#include <stdlib.h>
#else
extern void free();
#endif

/* static function declarations */
#if __STDC__
static int tc_l2(struct ptab *, int);
static int tc_l3(struct ptab *);
static int tc_tsinfo(struct ptab *, int);
static struct ptab *tc_getptab(int);
#ifndef NOTRACE
static int tc_traceargs(struct ptab *);
#endif /* NOTRACE */
#else /* __STDC__ */
static int tc_l2();
static int tc_l3();
static int tc_tsinfo();
static struct ptab *tc_getptab();
#ifndef NOTRACE
static int tc_traceargs();
#endif /* NOTRACE */
#endif /* __STDC__ */


/*
**	tc_logon() - connect to an stcc and log on to it
**
**	return 0 if successful or -1 on error
**
**	if the logon is successful, the allocated ptab element is
**	added to the global process table
*/

int tc_logon(sysid)
int sysid;
{
	register struct ptab *pp;

	/* make sure that we aren't logged on already */
	if ((pp = getptbysysptype(sysid, PT_STCC)) != (struct ptab *) 0) {
		error(0, "already logged on to", rtoa(&pp->pt_rid));
		return(-1);
	}

	/* get a ptab entry and set it up */
	if ((pp = ptalloc()) == (struct ptab *) 0)
		return(-1);

	pp->ptr_sysid = sysid;
	pp->ptr_ptype = PT_STCC;
	pp->pt_flags = PF_SERVER;

	/* do the rest of the logon processing */
	if (tc_l2(pp, sysid) < 0) {
		ts_dead(pp);
		ptfree(pp);
		return(-1);
	}

	/* all ok so add the ptab entry to the process table */
	ptadd(pp);
	return(0);
}

/*
**	tc_l2() - extend the tc_logon processing
**
**	return 0 if successful or -1 on error
*/

static int tc_l2(pp, sysid)
struct ptab *pp;
int sysid;
{
	register int rc;

	/* connect to the stcc and log on to it */
	rc = ti_logon(pp);
	pp->ptr_sysid = sysid;
	if (rc < 0)
		return(-1);

	/* do the rest of the logon processing */
	if (tc_l3(pp) < 0) {
		(void) ti_logoff(pp, 0);
		return(-1);
	}

	return(0);
}

/*
**	tc_l3() - extend the tc_logon processing some more
**
**	return 0 if successful or -1 on error
*/

static int tc_l3(pp)
struct ptab *pp;
{
	register struct valmsg *mp;

#ifndef NOTRACE
	/* send trace args to stcc */
	if (tc_traceargs(pp) < 0)
		return(-1);
#endif

	/* assign a sysid to the stcc */
	if ((mp = (struct valmsg *) ti_msgbuf(pp, valmsgsz(OP_SYSID_NVALUE))) == (struct valmsg *) 0)
		return(-1);

	VM_SYSID(mp) = (long) pp->ptr_sysid;
	mp->vm_nvalue = OP_SYSID_NVALUE;
	pp->ptm_req = OP_SYSID;
	pp->ptm_len = sizeof *mp;
	if (ti_talk(pp, TALK_DELAY) < 0)
		return(-1);

	if (pp->ptm_rc != ER_OK) {
		error(0, "assign sysid request failed", rtoa(&pp->pt_rid));
		return(-1);
	}

	/* inform stcc of syncd and xresd transport-specific details */
	if (tc_tsinfo(pp, PT_SYNCD) < 0 || tc_tsinfo(pp, PT_XRESD) < 0)
		return(-1);

	return(0);
}

/*
**	tc_tsinfo() - send server transport-specific details to stcc
**
**	return 0 if successful or -1 on error
*/

static int tc_tsinfo(pp, ptype)
struct ptab *pp;
int ptype;
{
	if (ss_tsinfo(pp, ptype) < 0)
		return(-1);

	pp->ptm_req = OP_TSINFO;
	if (ti_talk(pp, TALK_DELAY) < 0)
		return(-1);

	if (pp->ptm_rc != ER_OK) {
		error(0, "send tsinfo request failed", rtoa(&pp->pt_rid));
		return(-1);
	}

	return(0);
}

/*
**	tc_logoff() - log off from an stcc and close the connection
**
**	return 0 if successful or -1 on error
*/

int tc_logoff(sysid)
int sysid;
{
	register struct ptab *pp;
	register int rc;

	/* get the ptab entry for this stcc */
	if ((pp = tc_getptab(sysid)) == (struct ptab *) 0)
		return(-1);

	/* log off from the stcc and remove its ptab entry */
	rc = ti_logoff(pp, 0);
	ptrm(pp);
	ptfree(pp);

	return(rc);
}

/*
**	tc_msgbuf() - return pointer to an stcc message data buffer at least
**		len bytes long, growing it if necessary
**
**	return (char *) 0 on error
*/

char *tc_msgbuf(sysid, len)
int sysid, len;
{
	register struct ptab *pp;

	/* get the ptab entry for this stcc */
	if ((pp = tc_getptab(sysid)) == (struct ptab *) 0)
		return((char *) 0);

	return(ti_msgbuf(pp, len));
}

/*
**	tc_talk() - talk to an stcc, return a pointer to the reply message data
**		with the reply code in tc_errno
**
**	return (char *) 0 on error
*/

char *tc_talk(sysid, req, delay)
register int sysid, req;
int delay;
{
	register int len;
	register struct ptab *pp;

	TRACE4(Ttccd, 1, "tctalk: sysid = %s, request = %s, delay = %s",
		itoa(sysid), ptreqcode(req), itoa(delay));

	/* get the ptab entry for this stcc */
	if ((pp = tc_getptab(sysid)) == (struct ptab *) 0) {
		tc_errno = ER_LOGON;
		return((char *) 0);
	}

	/* determine the size of the message data */
	switch (req) {
	case OP_SYSNAME:
		len = valmsgsz(((struct valmsg *) pp->ptm_data)->vm_nvalue);
		break;
	case OP_WAIT:
		len = valmsgsz(OP_WAIT_NVALUE);
		break;
	case OP_KILL:
		len = valmsgsz(OP_KILL_NVALUE);
		break;
	case OP_CFNAME:
		len = avmsgsz(OP_CFNAME_ARGC(TC_NCFNAME));
		break;
	case OP_ACCESS:
		len = avmsgsz(OP_ACCESS_ARGC);
		break;
	case OP_MKDIR:
	case OP_RMDIR:
	case OP_CHDIR:
	case OP_MKTMPDIR:
	case OP_MKSDIR:
		len = avmsgsz(OP_DIR_ARGC);
		break;
	case OP_FOPEN:
		len = avmsgsz(OP_FOPEN_ARGC);
		break;
	case OP_FCLOSE:
		len = valmsgsz(OP_FCLOSE_NVALUE);
		break;
	case OP_LOCKFILE:
		len = avmsgsz(OP_LOCKFILE_ARGC);
		break;
	case OP_SHARELOCK:
		len = avmsgsz(OP_SHARELOCK_ARGC);
		break;
	case OP_UNLINK:
		len = avmsgsz(OP_UNLINK_ARGC);
		break;
	case OP_RXFILE:
		len = avmsgsz(OP_RXFILE_ARGC);
		break;
	case OP_EXEC:
	case OP_SNDCONF:
	case OP_PUTENV:
	case OP_PUTS:
	case OP_CONFIG:
	case OP_TSFILES:
#if TESTING
	case OP_PRINT:
#endif
		len = avmsgsz(((struct avmsg *) pp->ptm_data)->av_argc);
		break;
	case OP_RCVCONF:
	case OP_NULL:
		len = 0;
		break;
	default:
		error(0, "unknown TCCD request:", ptreqcode(req));
		tc_errno = ER_REQ;
		return((char *) 0);
	}

	/* send the message and receive a reply */
	pp->ptm_req = req;
	pp->ptm_len = len;
	if (ti_talk(pp, delay) < 0) {
		tc_errno = ER_ERR;
		return((char *) 0);
	}

	/* return the reply code and message data */
	tc_errno = pp->ptm_rc;
	return(pp->ptm_data);
}

/*
**	tc_getptab() - find stcc ptab entry and return a pointer thereto
**
**	return (struct ptab *) 0 if not found
*/

static struct ptab *tc_getptab(sysid)
int sysid;
{
	register struct ptab *pp;

	/* get the ptab entry for this stcc */
	if ((pp = getptbysysptype(sysid, PT_STCC)) == (struct ptab *) 0 || (pp->pt_flags & PF_LOGGEDON) == 0)
		error(0, "not logged on to TCCD on system", itoa(sysid));

	return(pp);
}

/*
**	tc_traceargs() - send trace flags to stcc
**
**	return 0 if successful or -1 on error
*/

#ifndef NOTRACE

static int tc_traceargs(pp)
struct ptab *pp;
{
	register char **avp, **argv;
	register int argc;
	register struct avmsg *ap;

	/* get a set of trace flags */
	if ((argv = traceargs(PT_STCC, (char **) 0)) == (char **) 0)
		return(-1);

	/* see if there are any to send */
	for (argc = 0, avp = argv; *avp; avp++)
		argc++;

	/* if none, return */
	if (!argc)
		return(0);

	/* make sure that the message buffer is big enough */
	if ((ap = (struct avmsg *) ti_msgbuf(pp, avmsgsz(argc))) == (struct avmsg *) 0)
		return(-1);

	/* copy in the message */
	pp->ptm_req = OP_TRACE;
	pp->ptm_len = avmsgsz(argc);
	ap->av_argc = argc;
	for (argc = 0, avp = argv; *avp; argc++, avp++)
		ap->av_argv[argc] = *avp;

	/* send the message and handle the return code */
	if (ti_talk(pp, TALK_DELAY) < 0)
		return(-1);
	if (pp->ptm_rc != ER_OK) {
		error(0, "send traceargs request failed", rtoa(&pp->pt_rid));
		return(-1);
	}

	return(0);
}

#endif /* !NOTRACE */

