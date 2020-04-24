/*
 *      SCCS:  @(#)tcdir.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)tcdir.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)tcdir.c	1.2 92/09/30
NAME:		tcdir.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	May 1992

DESCRIPTION:
	functions to request TCCD to make, remove and change directories

MODIFICATIONS:

************************************************************************/

#include "dtmsg.h"
#include "avmsg.h"
#include "servlib.h"

/* static function declarations */
#if __STDC__
static int tc_dir(int, char *, int);
#else /* __STDC__ */
static int tc_dir();
#endif /* __STDC__ */


/*
**	tc_mkdir() - send an OP_MKDIR message to TCCD and receive a reply
**
**	return 0 if successful or -1 on error
*/

int tc_mkdir(sysid, dir)
int sysid;
char *dir;
{
	return(tc_dir(sysid, dir, OP_MKDIR));
}

/*
**	tc_rmdir() - send an OP_RMDIR message to TCCD and receive a reply
**
**	return 0 if successful or -1 on error
*/

int tc_rmdir(sysid, dir)
int sysid;
char *dir;
{
	return(tc_dir(sysid, dir, OP_RMDIR));
}

/*
**	tc_chdir() - send an OP_CHDIR message to TCCD and receive a reply
**
**	return 0 if successful or -1 on error
*/

int tc_chdir(sysid, dir)
int sysid;
char *dir;
{
	return(tc_dir(sysid, dir, OP_CHDIR));
}

/*
**	tc_dir() - common routine for tc_mkdir, tc_rmdir and tc_chdir
**
**	return 0 if successful or -1 on error
*/

static int tc_dir(sysid, dir, request)
int sysid, request;
char *dir;
{
	register struct avmsg *mp;

	/* make sure that dir is non-null */
	if (!dir || !*dir) {
		tc_errno = ER_INVAL;
		return(-1);
	}

	/* get the TCCD message buffer */
	if ((mp = (struct avmsg *) tc_msgbuf(sysid, avmsgsz(OP_DIR_ARGC))) == (struct avmsg *) 0) {
		tc_errno = ER_ERR;
		return(-1);
	}

	/* set up the request message */
	mp->av_argc = OP_DIR_ARGC;
	AV_DIR(mp) = dir;

	/* send the request and receive the reply */
	return(tc_rsys(sysid, request));
}

