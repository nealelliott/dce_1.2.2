/*
 *      SCCS:  @(#)unmaperr.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)unmaperr.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)unmaperr.c	1.2 92/09/30
NAME:		unmaperr.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	May 1992

DESCRIPTION:
	function to convert DTET message reply code values to local errno
	values

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <errno.h>
#include "dtmsg.h"
#include "dtetlib.h"

#undef BREAKUSED

/*
**	unmaperrno() - map DTET message reply code values to local errno
**		values
**
**	return 0 if there is no local errno equivalent
*/

int unmaperrno(repcode)
int repcode;
{
	switch (repcode)  {

	case ER_EPERM:
#ifdef EPERM
		return(EPERM);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* EPERM */

	case ER_ENOENT:
#ifdef ENOENT
		return(ENOENT);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* ENOENT */

	case ER_ESRCH:
#ifdef ESRCH
		return(ESRCH);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* ESRCH */

	case ER_EINTR:
#ifdef EINTR
		return(EINTR);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* EINTR */

	case ER_EIO:
#ifdef EIO
		return(EIO);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* EIO */

	case ER_ENXIO:
#ifdef ENXIO
		return(ENXIO);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* ENXIO */

	case ER_E2BIG:
#ifdef E2BIG
		return(E2BIG);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* E2BIG */

	case ER_ENOEXEC:
#ifdef ENOEXEC
		return(ENOEXEC);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* ENOEXEC */

	case ER_EBADF:
#ifdef EBADF
		return(EBADF);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* EBADF */

	case ER_ECHILD:
#ifdef ECHILD
		return(ECHILD);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* ECHILD */

	case ER_EAGAIN:
#ifdef EAGAIN
		return(EAGAIN);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* EAGAIN */

	case ER_ENOMEM:
#ifdef ENOMEM
		return(ENOMEM);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* ENOMEM */

	case ER_EACCES:
#ifdef EACCES
		return(EACCES);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* EACCES */

	case ER_EFAULT:
#ifdef EFAULT
		return(EFAULT);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* EFAULT */

	case ER_ENOTBLK:
#ifdef ENOTBLK
		return(ENOTBLK);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* ENOTBLK */

	case ER_EBUSY:
#ifdef EBUSY
		return(EBUSY);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* EBUSY */

	case ER_EEXIST:
#ifdef EEXIST
		return(EEXIST);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* EEXIST */

	case ER_EXDEV:
#ifdef EXDEV
		return(EXDEV);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* EXDEV */

	case ER_ENODEV:
#ifdef ENODEV
		return(ENODEV);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* ENODEV */

	case ER_ENOTDIR:
#ifdef ENOTDIR
		return(ENOTDIR);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* ENOTDIR */

	case ER_EISDIR:
#ifdef EISDIR
		return(EISDIR);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* EISDIR */

	case ER_EINVAL:
#ifdef EINVAL
		return(EINVAL);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* EINVAL */

	case ER_ENFILE:
#ifdef ENFILE
		return(ENFILE);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* ENFILE */

	case ER_EMFILE:
#ifdef EMFILE
		return(EMFILE);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* EMFILE */

	case ER_ENOTTY:
#ifdef ENOTTY
		return(ENOTTY);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* ENOTTY */

	case ER_ETXTBSY:
#ifdef ETXTBSY
		return(ETXTBSY);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* ETXTBSY */

	case ER_EFBIG:
#ifdef EFBIG
		return(EFBIG);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* EFBIG */

	case ER_ENOSPC:
#ifdef ENOSPC
		return(ENOSPC);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* ENOSPC */

	case ER_ESPIPE:
#ifdef ESPIPE
		return(ESPIPE);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* ESPIPE */

	case ER_EROFS:
#ifdef EROFS
		return(EROFS);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* EROFS */

	case ER_EMLINK:
#ifdef EMLINK
		return(EMLINK);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* EMLINK */

	case ER_EPIPE:
#ifdef EPIPE
		return(EPIPE);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* EPIPE */

	case ER_ENOTEMPTY:
#ifdef ENOTEMPTY
		return(ENOTEMPTY);
#else
		break;
#ifndef BREAKUSED
#define BREAKUSED
#endif /* !BREAKUSED */
#endif /* ENOTEMPTY */

	default:
		return(0);
	}

#ifdef BREAKUSED
	{

#include "error.h"

		static char srcFile[] = __FILE__;

		error(0, ptrepcode(repcode),
			"has no equivalent local errno value");
	}

	return(0);

#endif /* BREAKUSED */

}

