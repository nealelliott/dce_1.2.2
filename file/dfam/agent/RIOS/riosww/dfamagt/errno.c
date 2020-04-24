/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.
 */
#ifdef HITACHI_REV_CHECK
static char *RCSID_errno_c="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/dfamagt/errno.c,v 1.1.2.2 1996/03/09 20:44:28 marty Exp $";
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: errno.c,v $
 * Revision 1.1.2.2  1996/03/09  20:44:28  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:24:18  marty]
 *
 * Revision 1.1.2.1  1996/02/16  05:08:53  dce-sdc
 * 	First Submit
 * 	[1996/02/16  05:08:25  dce-sdc]
 * 
 * Revision 1.1.2.5  96/01/12  05:09:09  05:09:09  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:08:58  kishi]
 * 
 * Revision 1.1.2.4  1995/10/26  01:34:50  kishi
 * 	Change Copyright.
 * 	[1995/10/26  01:34:42  kishi]
 * 
 * Revision 1.1.2.3  1995/09/29  00:33:55  kishi
 * 	Add header comment.
 * 	[1995/09/29  00:33:32  kishi]
 * 
 * Revision 1.1.2.2  1995/09/14  14:05:12  ichikawa
 * 	Argument is change from unsigned short to int.
 * 	[1995/09/14  14:04:53  ichikawa]
 * 
 * Revision 1.1.2.1  1995/09/09  06:46:57  kishi
 * 	Initial revision
 * 	[1995/09/09  06:46:35  kishi]
 * 
 * $EndLog$
 */

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include "agt_errno.h"
#include "hi_err.h"

/*****************************************************************************/
/*  ID   = trnserr                                                           */
/*                                                                           */
/* name = Conversion of AIX errno into HI-UX errno                           */
/* func = Converts errno of AIX into errno of HI-UX                          */
/*                                                                           */
/* i/o  = errno_IN :int *:INP: errno of AIX (IBM)                            */
/*        errno_OUT:int *:OUT: errno of HI-UX                                */
/*                                                                           */
/* return = TRUE : normal end                                                */
/*          FALSE: abnormal end                                              */
/* common = none                                                             */
/*                                                                           */
/* note = If the returned value is FALSE, the outputted value of             */
/*        errno_OUT is not specified (can be any value).                     */
/*                                                                           */
/* date = August 2, 1995  by Kousuke Takano                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd. All Rights Reserved. Derechos Reservados.    */
/*****************************************************************************/
int trnserr( int *errno_IN , int *errno_OUT ){
	switch( *errno_IN ){

	case NORMAL:
	case EPERM:
	case ENOENT:
	case ESRCH:
	case EINTR:
	case EIO:
	case ENXIO:
	case E2BIG:
	case ENOEXEC:
	case EBADF:
	case ECHILD:
	case EAGAIN:
	case ENOMEM:
	case EACCES:
	case EFAULT:
	case EBUSY:
	case EEXIST:
	case EXDEV:
	case ENODEV:
	case ENOTDIR:
	case EISDIR:
	case EINVAL:
	case ENFILE:
	case EMFILE:
	case ENOTTY:
	case EFBIG:
	case ENOSPC:
	case ESPIPE:
	case EROFS:
	case EMLINK:
	case EPIPE:
	case EDEADLK:
	case ECOM:
	case DENOTDEL:
	case DEUNAVAILSRV:
	case DEUNNOWNPRI:
	case DENOMASK:
	case DENOACLPRI:
	case DENOKFILE:
	case DENOKEY:
	case DEKEYSIZE:
	case DEINVALAGT:
	case DENOENTRY:
	case DEINVALPSWD:
	case DEINVALACNT:
	case DELOCAL:
	case DEINVALGTWY:
	case DEUNKNOWN:
	case DEUPDATEKEY:
	case DENULLPSWD:
	case DENOAGTKF:
	case DEAGTKSIZE:
	case DENOACNT:
	case DENOGWENT:
		*errno_OUT = *errno_IN;

		return TRUE;

	case ENOTREADY:
		*errno_OUT = HI_ENXIO;
		return TRUE;

	case EWRPROTECT:
		*errno_OUT = HI_EROFS;
		return TRUE;

	case EFORMAT:
		*errno_OUT = HI_EFDIOE;
		return TRUE;

	case ENOLCK:
		*errno_OUT = HI_ENOLCK;
		return TRUE;

	case ENOCONNECT:
		*errno_OUT = HI_ECONNREFUSED;
		return TRUE;

	case ESTALE:
		*errno_OUT = HI_ESTALE;
		return TRUE;



	case EINPROGRESS:
		*errno_OUT = HI_EINPROGRESS;
		return TRUE;

	case EALREADY:
		*errno_OUT = HI_EALREADY;
		return TRUE;

	case ENOTSOCK:
		*errno_OUT = HI_ENOTSOCK;
		return TRUE;

	case EDESTADDRREQ:
		*errno_OUT = HI_EDESTADDRREQ;
		return TRUE;

	case EMSGSIZE:
		*errno_OUT = HI_EMSGSIZE;
		return TRUE;

	case EPROTOTYPE:
		*errno_OUT = HI_EPROTOTYPE;
		return TRUE;

	case ENOPROTOOPT:
		*errno_OUT = HI_ENOPROTOOPT;
		return TRUE;

	case EPROTONOSUPPORT:
		*errno_OUT = HI_EPROTONOSUPPORT;
		return TRUE;

	case ESOCKTNOSUPPORT:
		*errno_OUT = HI_ESOCKTNOSUPPORT;
		return TRUE;

	case EOPNOTSUPP:
		*errno_OUT = HI_EOPNOTSUPP;
		return TRUE;

	case EPFNOSUPPORT:
		*errno_OUT = HI_EPFNOSUPPORT;
		return TRUE;

	case EAFNOSUPPORT:
		*errno_OUT = HI_EAFNOSUPPORT;
		return TRUE;

	case EADDRINUSE:
		*errno_OUT = HI_EADDRINUSE;
		return TRUE;

	case EADDRNOTAVAIL:
		*errno_OUT = HI_EADDRNOTAVAIL;
		return TRUE;

	case ENETDOWN:
		*errno_OUT = HI_ENETDOWN;
		return TRUE;

	case ENETUNREACH:
		*errno_OUT = HI_ENETUNREACH;
		return TRUE;

	case ENETRESET:
		*errno_OUT = HI_ENETRESET;
		return TRUE;

	case ECONNABORTED:
		*errno_OUT = HI_ECONNABORTED;
		return TRUE;

	case ECONNRESET:
		*errno_OUT = HI_ECONNRESET;
		return TRUE;

	case ENOBUFS:
		*errno_OUT = HI_ENOBUFS;
		return TRUE;

	case EISCONN:
		*errno_OUT = HI_EISCONN;
		return TRUE;

	case ENOTCONN:
		*errno_OUT = HI_ENOTCONN;
		return TRUE;

	case ESHUTDOWN:
		*errno_OUT = HI_ESHUTDOWN;
		return TRUE;

	case ETIMEDOUT:
		*errno_OUT = HI_ETIMEDOUT;
		return TRUE;

	case ECONNREFUSED:
		*errno_OUT = HI_ECONNREFUSED;
		return TRUE;

	case EHOSTDOWN:
		*errno_OUT = HI_EHOSTDOWN;
		return TRUE;

	case EHOSTUNREACH:
		*errno_OUT = HI_EHOSTUNREACH;
		return TRUE;

	case ERESTART:
		*errno_OUT = HI_EIO;
		return TRUE;

	case EPROCLIM:
		*errno_OUT = HI_EAGAIN;
		return TRUE;

	case EUSERS:
		*errno_OUT = HI_EUSERS;
		return TRUE;

	case ELOOP:
		*errno_OUT = HI_ELOOP;
		return TRUE;

	case ENAMETOOLONG:
		*errno_OUT = HI_ENAMETOOLONG;
		return TRUE;



	case EDQUOT:
		*errno_OUT = HI_EDQUOT;
		return TRUE;

	case EREMOTE:
		*errno_OUT = HI_EREMOTE;
		return TRUE;

	case ENOSYS:
		*errno_OUT = HI_ENOSYS;
		return TRUE;

	case EMEDIA:
		*errno_OUT = HI_EFDIOE;
		return TRUE;

	case ESOFT:
		*errno_OUT = HI_EFDIOE;
		return TRUE;

	case ENOATTR:
		*errno_OUT = HI_EACCES;
		return TRUE;

	case ESAD:
		*errno_OUT = HI_EACCES;
		return TRUE;

	case ENOTRUST:
		*errno_OUT = HI_EACCES;
		return TRUE;

	case ETOOMANYREFS:
		*errno_OUT = HI_ETOOMANYREFS;
		return TRUE;

	case EILSEQ:
		*errno_OUT = HI_EILSEQ;
		return TRUE;

	case ECANCELED:
		*errno_OUT = HI_EIO;
		return TRUE;

	case ENOSR:
		*errno_OUT = HI_ENOSR;
		return TRUE;

	case ETIME:
		*errno_OUT = HI_ETIME;
		return TRUE;

	case EBADMSG:
		*errno_OUT = HI_EBADMSG;
		return TRUE;

	case EPROTO:
		*errno_OUT = HI_EPROTO;
		return TRUE;

	case ENODATA:
		*errno_OUT = HI_ENODATA;
		return TRUE;

	case ENOSTR:
		*errno_OUT = HI_ENOSTR;
		return TRUE;

	default:
		return FALSE;

	}
}
