/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: lock_file.c,v $
 * Revision 1.1.5.2  1996/02/18  00:15:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:52  marty]
 *
 * Revision 1.1.5.1  1995/12/08  18:04:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:42  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:30:36  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:18:09  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  lock_file.c V=4 10/24/91 //littl/prgy/krb5/lib/os
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/lock_file.c,v $
 * $Author: marty $
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
 * libos: krb5_lock_file routine
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_lock_file_c [] =
"$Id: lock_file.c,v 1.1.5.2 1996/02/18 00:15:32 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/libos.h>

#include <stdio.h>

/* POSIX_* are auto-magically defined in <krb5/config.h> at source
   configuration time. */

#ifdef POSIX_FILE_LOCKS
#include <errno.h>
#include <fcntl.h>
#define SHARED_LOCK	F_RDLCK
#define EXCLUSIVE_LOCK	F_WRLCK
#define UNLOCK_LOCK	F_UNLCK
#else
#include <sys/file.h>
#define SHARED_LOCK	LOCK_SH
#define EXCLUSIVE_LOCK	LOCK_EX
#define UNLOCK_LOCK	LOCK_UN
#endif

#include <sys/types.h>
#include <krb5/ext-proto.h>

extern int errno;

/*ARGSUSED*/
krb5_error_code
krb5_lock_file(filep, pathname, mode)
FILE *filep;
char *pathname;
int mode;
{
#ifdef POSIX_FILE_LOCKS
    int lock_cmd = F_SETLKW;
    struct flock lock_arg;
#define lock_flag lock_arg.l_type
    lock_flag = -1;
#else
    int lock_flag = -1;
#endif

    switch (mode & ~KRB5_LOCKMODE_DONTBLOCK) {
    case KRB5_LOCKMODE_SHARED:
	lock_flag = SHARED_LOCK;
	break;
    case KRB5_LOCKMODE_EXCLUSIVE:
	lock_flag = EXCLUSIVE_LOCK;
	break;
    case KRB5_LOCKMODE_UNLOCK:
	lock_flag = UNLOCK_LOCK;
	break;
    }

    if (lock_flag == -1)
	return(KRB5_LIBOS_BADLOCKFLAG);

    if (mode & KRB5_LOCKMODE_DONTBLOCK) {
#ifdef POSIX_FILE_LOCKS
	lock_cmd = F_SETLK;
#else
	lock_flag |= LOCK_NB;
#endif
    }

#ifdef POSIX_FILE_LOCKS
    lock_arg.l_whence = 0;
    lock_arg.l_start = 0;
    lock_arg.l_len = 0;
    if (fcntl(fileno(filep), lock_cmd, &lock_arg) == -1) {
	if (errno == EACCES || errno == EAGAIN)	/* see POSIX/IEEE 1003.1-1988,
						   6.5.2.4 */
	    return(EAGAIN);
	return(errno);
    }
#else
    if (flock(fileno(filep), lock_flag) == -1)
	return(errno);
#endif
    return 0;
}
