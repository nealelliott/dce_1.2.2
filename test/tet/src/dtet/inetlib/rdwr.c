/*
 *      SCCS:  @(#)rdwr.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)rdwr.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)rdwr.c	1.2 92/09/30
NAME:		rdwr.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	required transport-specific library interfaces

	functions to perform i/o on remote process connections

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <errno.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "tptab_in.h"
#include "bstring.h"
#include "error.h"
#include "ltoa.h"
#include "dtetlib.h"
#include "tslib.h"
#include "server_bs.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

#ifndef NOTRACE
static char msghdr[] = "message header";
static char msgdata[] = "message data";
#endif

/* extern function declarations */
#ifndef __STDC__
extern void longjmp();
#endif

/* declarations for static functions */
#if __STDC__
static int doread(struct ptab *);
static SIG_FUNC_T pipetrap(int);
static int bs2md(char *, struct ptab *);
static int md2bs(struct ptab *);
#else
static int doread();
static SIG_FUNC_T pipetrap();
static int bs2md();
static int md2bs();
#endif


/*
**	ts_rcvmsg() - recieve an inet message on a socket with
**		at least some data to read
**
**	set one of PF_INPROGRESS, PF_IODONE or PF_IOERR on return
**	change connected process state to PS_DEAD on EOF
**
**	return an ER_ code to show the state of the local receive processing
*/

int ts_rcvmsg(pp)
register struct ptab *pp;
{
	register struct tptab *tp = (struct tptab *) pp->pt_tdata;
	register int rc;

	/* set up variables if first attempt to read this message */
	if ((pp->pt_flags & PF_INPROGRESS) == 0) {
		tp->tp_cnt = DM_HDRSZ;
		tp->tp_ptr = tp->tp_buf;
		pp->pt_flags |= (PF_INPROGRESS | PF_RCVHDR);
		TRACE2(Tio, 4, "rcvmsg: read %s bytes header",
			itoa(tp->tp_cnt));
	}

	/* process the next bit of the message */
	switch (pp->pt_flags & PF_RCVHDR) {
	case PF_RCVHDR:
		/* read the message header */
		if ((rc = doread(pp)) <= 0) {
			if (rc < 0)
				pp->pt_flags &= ~PF_RCVHDR;
			return(ER_OK);
		}
		TDUMP(Tio, 10, tp->tp_buf, DM_HDRSZ, msghdr);
		pp->pt_flags &= ~PF_RCVHDR;
		(void) bs2dtmhdr(tp->tp_buf, &pp->ptm_hdr, DM_HDRSZ);
		if (pp->ptm_magic != DTM_MAGIC && pp->ptm_len) {
			/* we are probably out of sync with sender */
			error(0, "received bad message header,",
				"ignored message data");
			pp->ptm_len = 0;
		}
		if (pp->ptm_len <= 0) {
			/* no data portion */
			pp->pt_flags |= PF_IODONE;
			return(ER_OK);
		}
		if (BUFCHK(&tp->tp_buf, &tp->tp_len, pp->ptm_len) < 0) {
			/* can't grow data buffer */
			pp->pt_flags |= PF_IODONE;
			return(ER_ERR);
		}
		pp->pt_flags |= PF_INPROGRESS;
		tp->tp_ptr = tp->tp_buf;
		tp->tp_cnt = pp->ptm_len;
		TRACE2(Tio, 4, "rcvmsg: read %s bytes data",
			itoa(tp->tp_cnt));
		/* fall through */
	default:
		/* read the message data */
		if (doread(pp) <= 0)
			return(ER_OK);
		TDUMP(Tio, 10, tp->tp_buf, pp->ptm_len, msgdata);
		if ((rc = bs2md(tp->tp_buf, pp)) != ER_OK) {
			pp->pt_flags |= PF_IODONE;
			return(rc);
		}
		pp->pt_flags |= PF_IODONE;
		break;
	}

	return(ER_OK);
}

/*
**	doread() - common read processing for both header and data
**
**	return	1 when done
**		0 for not done
**		-1 on error
*/

static int doread(pp)
register struct ptab *pp;
{
	register struct tptab *tp = (struct tptab *) pp->pt_tdata;
	register int cnt, rc = -1;

	TRACE3(Tio, 4, "doread: expect %s bytes on sd %s",
		itoa(tp->tp_cnt), itoa(tp->tp_sd));

	do {
		errno = 0;
		cnt = read(tp->tp_sd, tp->tp_ptr, (unsigned) tp->tp_cnt);
	} while (cnt < 0 && errno == EINTR);

	if (cnt < 0) {
		/* read error */
		TRACE2(Tio, 4, "read error, errno = %s", errname(errno));
		switch (errno) {
		case ECONNRESET:
			error(0, "unexpected EOF", rtoa(&pp->pt_rid));
			pp->pt_state = PS_DEAD;
			break;
		case EWOULDBLOCK:
			if (pp->pt_flags & PF_NBIO)
				return(0);
			/* else fall through */
		default:
			pp->pt_flags |= PF_IOERR;
			break;
		}
	}
	else if (cnt == 0) {
		/* end-of-file */
		TRACE1(Tio, 4, "encountered EOF");
		pp->pt_state = PS_DEAD;
	}
	else if ((tp->tp_cnt -= cnt) > 0) {
		/* partial read - come back for more later */
		TRACE3(Tio, 4, "partial read: wanted %s, received %s",
			itoa(tp->tp_cnt + cnt), itoa(cnt));
		tp->tp_ptr += cnt;
		return(0);
	}
	else {
		/* all done for now */
		TRACE1(Tio, 4, "this read complete");
		rc = 1;
	}

	if (pp->pt_state == PS_DEAD)
		pp->pt_flags |= PF_IODONE;

	if (pp->pt_flags & PF_IOERR)
		error(errno, "socket read error", rtoa(&pp->pt_rid));

	/* all non-zero return codes come here */
	pp->pt_flags &= ~PF_INPROGRESS;
	return(rc);
}

/*
**	pipetrap() - SIGPIPE signal handler for ts_sndmsg()
*/

static jmp_buf sndmsg_env;

/* ARGSUSED */
static SIG_FUNC_T pipetrap(sig)
int sig;
{
	longjmp(sndmsg_env, 1);
}

/*
**	ts_sndmsg() - send a message to a connected process
**
**	set one of PF_INPROGRESS, PF_IODONE or PF_IOERR on return
**
**	return an ER_ code to show the state of the local send processing
*/

int ts_sndmsg(pp)
register struct ptab *pp;
{
	register struct tptab *tp = (struct tptab *) pp->pt_tdata;
	register int rc, err;
	int save_errno;
	SIG_T (*pipe_save)();

	/* set up variables for this message on first time through */
	if ((pp->pt_flags & PF_INPROGRESS) == 0) {
		do {
			err = 0;
			tp->tp_cnt = dtmhdr2bs(&pp->ptm_hdr, tp->tp_buf);
			if (pp->ptm_len <= 0)
				rc = 0;
			else if ((rc = md2bs(pp)) < 0)
				if (pp->pt_flags & PF_SERVER) {
					pp->pt_flags |= PF_IODONE;
					return(rc);
				}
				else {
					pp->ptm_rc = ER_ERR;
					pp->ptm_len = 0;
					TRACE2(Tserv, 2,
						"md2bs failed, new rc = %s",
						ptrepcode(pp->ptm_rc));
					err = 1;
					continue;
				}
			TDUMP(Tio, 10, tp->tp_buf, tp->tp_cnt, msghdr);
			if (rc > 0) {
				TDUMP(Tio, 10, tp->tp_buf + tp->tp_cnt,
					rc, msgdata);
				tp->tp_cnt += rc;
			}
			(void) dmlen2bs((short) rc, tp->tp_buf);
		} while (err);
		tp->tp_ptr = tp->tp_buf;
		pp->pt_flags |= PF_INPROGRESS;
	}

	TRACE3(Tio, 4, "sendmsg: want to write %s bytes on sd %s",
		itoa(tp->tp_cnt), itoa(tp->tp_sd));

	/* write out the message -
		if a SIGPIPE is raised, the handler returns via longjmp; other
		signals just cause the write to be restarted (in an attempt
		to cope with systems without restartable system calls) */
	pipe_save = signal(SIGPIPE, pipetrap);
	if (setjmp(sndmsg_env) == 0) {
		do {
			errno = 0;
			rc = write(tp->tp_sd, tp->tp_ptr,
				(unsigned) tp->tp_cnt);
		} while (rc < 0 && errno == EINTR);
		save_errno = errno;
	}
	else {
		rc = -1;
		save_errno = EINTR;
	}
	(void) signal(SIGPIPE, (SIG_FUNC_T (*) ()) pipe_save);

	/* set state and flags according to results */
	if (rc < 0) {
		/* write error */
		TRACE2(Tio, 4, "write error, errno = %s", errname(save_errno));
		switch (save_errno) {
		case EINTR:
			error(0, "process connection broken",
				rtoa(&pp->pt_rid));
			pp->pt_flags |= PF_IODONE;
			pp->pt_state = PS_DEAD;
			break;
		case EWOULDBLOCK:
			if (pp->pt_flags & PF_NBIO)
				return(ER_OK);
			/* else fall through */
		default:
			error(save_errno, "socket write error",
				rtoa(&pp->pt_rid));
			pp->pt_flags |= PF_IOERR;
			break;
		}
	}
	else if ((tp->tp_cnt -= rc) > 0) {
		/* partial write - try again later */
		TRACE3(Tio, 4, "partial write: requested %s, only wrote %s",
			itoa(tp->tp_cnt + rc), itoa(rc));
		tp->tp_ptr += rc;
		return(ER_OK);
	}
	else {
		/* all done */
		TRACE1(Tio, 4, "write complete");
		pp->pt_flags |= PF_IODONE;
	}

	/* here if no more data to write now */
	pp->pt_flags &= ~PF_INPROGRESS;
	return(ER_OK);
}

/*
**	bs2md() - convert machine-independent message data to internal format
**		depending on message type
**
**	return ER_OK if successful, or -ve error code on error
*/

static int bs2md(bp, pp)
char *bp;
struct ptab *pp;
{
	register int rc;

	if ((pp->pt_flags & PF_SERVER) == 0)
		pp->pt_savreq = pp->ptm_req;

	switch (pp->pt_savreq) {
	case OP_NULL:
		pp->ptm_len = 0;
		rc = ER_OK;
		break;
	default:
		if ((rc = ss_bs2md(bp, pp)) < 0)
			pp->ptm_len = 0;
		else {
			pp->ptm_len = (short) rc;
			rc = ER_OK;
		}
		break;
	}

	return(rc);
}

/*
**	md2bs() - convert message data to machine-independent format
**		depending on message type
**
**	return the number of bytes occupied by the message, or -ve error code
**	on error
*/

static int md2bs(pp)
register struct ptab *pp;
{
	register struct tptab *tp = (struct tptab *) pp->pt_tdata;
	register int len = 0;

	/* work out the required buffer size */
	switch (pp->ptm_req) {
	case OP_NULL:
		len = pp->ptm_len;
		break;
	}

	/* check that the buffer is big enough */
	if (len > 0 && BUFCHK(&tp->tp_buf, &tp->tp_len, tp->tp_cnt + len) < 0)
		return(ER_ERR);

	/* copy the internal data into the machine-independent buffer */
	switch (pp->ptm_req) {
	case OP_NULL:
		bcopy(pp->ptm_data, tp->tp_buf + tp->tp_cnt, len);
		break;
	default:
		len = ss_md2bs(pp, &tp->tp_buf, &tp->tp_len, tp->tp_cnt);
		break;
	}

	return(len);
}

/*
**	ts_dead() - perform transport-specific actions for a
**		dead connected process
*/

void ts_dead(pp)
register struct ptab *pp;
{
	register struct tptab *tp = (struct tptab *) pp->pt_tdata;

	TRACE2(Tio, 4, "ts_dead: close sd %s", itoa(tp->tp_sd));

	(void) close(tp->tp_sd);
	tp->tp_sd = -1;
	pp->pt_flags &= ~(PF_CONNECTED | PF_INPROGRESS);
}

