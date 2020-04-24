/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sysincl.h,v $
 * Revision 1.1.7.2  1996/02/18  23:03:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:40  marty]
 *
 * Revision 1.1.7.1  1995/12/08  17:44:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:25  root]
 * 
 * Revision 1.1.5.3  1992/12/29  14:02:03  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:54:07  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  21:19:07  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:33:17  sekhar]
 * 
 * Revision 1.1.2.2  1992/06/30  21:52:57  burati
 * 	CR4516 Portability changes
 * 	[1992/06/30  20:37:23  burati]
 * 
 * Revision 1.1  1992/01/19  14:48:59  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  sysincl.h V=3 10/23/91 //littl/prgy/krb5/include/krb5
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/include/krb5/sysincl.h,v $
 * $Author: marty $
 * $Id: sysincl.h,v 1.1.7.2 1996/02/18 23:03:08 marty Exp $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America is assumed
 *   to require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 * 
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 * 
 *
 * System include files, for various things.
 */


#ifndef KRB5_SYSINCL__
#define KRB5_SYSINCL__

#ifndef KRB5_SYSTYPES__
#define KRB5_SYSTYPES__
#include <sys/types.h>			/* needed for much of the reset */
#endif /* KRB5_SYSTYPES__ */

#include <krb5/osconf.h>		/* USE*TIME_H macros */
#ifdef USE_TIME_H
#include <time.h>
#endif
#ifdef USE_SYS_TIME_H
#include <sys/time.h>			/* struct timeval, utimes() */
#endif
#include <sys/stat.h>			/* struct stat, stat() */
#include <sys/param.h>			/* MAXPATHLEN */
#if defined(unix) || defined(__unix__) || defined(__unix)
#include <sys/file.h>			/* prototypes for file-related
					   syscalls; flags for open &
					   friends */
#if defined(SYSV) || defined(SNI_SVR4_POSIX)
#include <sys/fcntl.h>
#endif

#ifdef SNI_SVR4_POSIX

#include <unistd.h>

/* don't worry about L_SET because that's defined ins <sys/file.h> */

#ifndef L_INCR
#define L_INCR          SEEK_CUR       /* relative to current offset */
#endif

#ifndef L_XTND
#define L_XTND          SEEK_END       /* relative to end of file */
#endif

#else

#ifndef L_SET
#define L_SET           0       /* absolute offset */
#define L_INCR          1       /* relative to current offset */
#define L_XTND          2       /* relative to end of file */
#endif /* L_SET */

#endif /* SNI_SVR4_POSIX */

#endif
#endif /* KRB5_SYSINCL__ */
