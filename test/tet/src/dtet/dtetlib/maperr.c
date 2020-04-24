/*
 *      SCCS:  @(#)maperr.c	1.3 (92/12/01) 
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
static char sccsid[] = "@(#)maperr.c	1.3 (92/12/01) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)maperr.c	1.3 92/12/01
NAME:		maperr.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	May 1992

DESCRIPTION:
	function to convert local errno value to DTET message reply code

	on systems where one error name is defined in terms of another, you
	should add a suitable #if line to avoid a compiler "duplicate case in
	switch" diagnostic
	an example is presented below (for AIX) where ENOTEMPTY is
	defined as EEXIST

MODIFICATIONS:
	Andrew Dingwall, UniSoft Ltd., November 1992
	AIX-specific modifications.

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <errno.h>
#include "dtmsg.h"
#include "error.h"
#include "dtetlib.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif

/*
**	maperrno() - map errno value to DTET message reply code
*/

int maperrno(errnum)
int errnum;
{
	switch (errnum) {

#ifdef EPERM
	case EPERM:
		return(ER_EPERM);
#endif

#ifdef ENOENT
	case ENOENT:
		return(ER_ENOENT);
#endif

#ifdef ESRCH
	case ESRCH:
		return(ER_ESRCH);
#endif

#ifdef EINTR
	case EINTR:
		return(ER_EINTR);
#endif

#ifdef EIO
	case EIO:
		return(ER_EIO);
#endif

#ifdef ENXIO
	case ENXIO:
		return(ER_ENXIO);
#endif

#ifdef E2BIG
	case E2BIG:
		return(ER_E2BIG);
#endif

#ifdef ENOEXEC
	case ENOEXEC:
		return(ER_ENOEXEC);
#endif

#ifdef EBADF
	case EBADF:
		return(ER_EBADF);
#endif

#ifdef ECHILD
	case ECHILD:
		return(ER_ECHILD);
#endif

#ifdef EAGAIN
	case EAGAIN:
		return(ER_EAGAIN);
#endif

#ifdef ENOMEM
	case ENOMEM:
		return(ER_ENOMEM);
#endif

#ifdef EACCES
	case EACCES:
		return(ER_EACCES);
#endif

#ifdef EFAULT
	case EFAULT:
		return(ER_EFAULT);
#endif

#ifdef ENOTBLK
	case ENOTBLK:
		return(ER_ENOTBLK);
#endif

#ifdef EBUSY
	case EBUSY:
		return(ER_EBUSY);
#endif

#ifdef EEXIST
	case EEXIST:
		return(ER_EEXIST);
#endif

#ifdef EXDEV
	case EXDEV:
		return(ER_EXDEV);
#endif

#ifdef ENODEV
	case ENODEV:
		return(ER_ENODEV);
#endif

#ifdef ENOTDIR
	case ENOTDIR:
		return(ER_ENOTDIR);
#endif

#ifdef EISDIR
	case EISDIR:
		return(ER_EISDIR);
#endif

#ifdef EINVAL
	case EINVAL:
		return(ER_EINVAL);
#endif

#ifdef ENFILE
	case ENFILE:
		return(ER_ENFILE);
#endif

#ifdef EMFILE
	case EMFILE:
		return(ER_EMFILE);
#endif

#ifdef ENOTTY
	case ENOTTY:
		return(ER_ENOTTY);
#endif

#ifdef ETXTBSY
	case ETXTBSY:
		return(ER_ETXTBSY);
#endif

#ifdef EFBIG
	case EFBIG:
		return(ER_EFBIG);
#endif

#ifdef ENOSPC
	case ENOSPC:
		return(ER_ENOSPC);
#endif

#ifdef ESPIPE
	case ESPIPE:
		return(ER_ESPIPE);
#endif

#ifdef EROFS
	case EROFS:
		return(ER_EROFS);
#endif

#ifdef EMLINK
	case EMLINK:
		return(ER_EMLINK);
#endif

#ifdef EPIPE
	case EPIPE:
		return(ER_EPIPE);
#endif

/* this for AIX where ENOTEMPTY is defined as EEXIST */
#if defined(ENOTEMPTY) && (!defined(EEXIST) || (ENOTEMPTY != EEXIST))
	case ENOTEMPTY:
		return(ER_ENOTEMPTY);
#endif

	default:
		error(errnum, errname(errnum),
			"has no equivalent DTET message reply code");
		return(ER_ERR);
	}
}

