/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: fcc_maybe.c,v $
 * Revision 1.1.9.1  1996/06/04  22:01:37  arvind
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:30 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 *
 * 	Kerb5Beta5 KRB5_FCC_FVNO_3 merge.
 * 	[1996/01/03  19:13 UTC  psn  /main/DCE_1.2/1]
 *
 * 	Merge fix for CHFts15637
 * 	[1995/08/22  21:07 UTC  mullan_s  /main/HPDCE02/2]
 *
 * 	Use fcntl(,F_SETLK,) & pthread_delay_np() to avoid process blocking
 * 	when locking files.
 * 	[1995/08/10  18:21 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/1]
 *
 * 	Binary Compatibility Merge
 * 	[1995/04/03  22:10 UTC  mullan_s  /main/HPDCE02/1]
 *
 * 	Call sec_login_krb_get_ccache_version() to get cache vno.
 * 	[1995/01/24  15:15 UTC  mullan_s  /main/mullan_mothra_bin_compat2/1]
 *
 * Revision 1.1.7.2  1996/02/18  00:09:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:02  marty]
 * 
 * Revision 1.1.7.1  1995/12/08  17:47:04  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/08/22  21:07 UTC  mullan_s
 * 	Merge fix for CHFts15637
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts15637/1  1995/08/10  18:21 UTC  mullan_s
 * 	Use fcntl(,F_SETLK,) & pthread_delay_np() to avoid process blocking
 * 	when locking files.
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/03  22:10 UTC  mullan_s
 * 	Binary Compatibility Merge
 * 
 * 	HP revision /main/mullan_mothra_bin_compat2/1  1995/01/24  15:15 UTC  mullan_s
 * 	Call sec_login_krb_get_ccache_version() to get cache vno.
 * 	[1995/12/08  16:57:57  root]
 * 
 * Revision 1.1.4.1  1994/08/26  16:01:37  sommerfeld
 * 	Krb5beta4 merge.
 * 	Add ODE header & copyright
 * 	[1994/08/26  15:53:33  sommerfeld]
 * 
 * $EndLog$
 */
/*
**
** Copyright (c) Hewlett-Packard Company 1991, 1992
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/ccache/file/fcc_maybe.c,v $
 * $Author: arvind $
 *
 * Copyright 1990, 1991 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
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
 * This file contains the source code for conditional open/close calls.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_fcc_maybe_c[] =
"$Id: fcc_maybe.c,v 1.1.9.1 1996/06/04 22:01:37 arvind Exp $";
#endif	/* !lint & !SABER */

#ifdef OSF_DCE

/*
 * The DCE environment runs in a multi-threaded POSIX world.  We
 * must be sure to pick up pthread.h to work with mutexes.
 */

#   include <pthread.h>

#include <sec_login_pvt.h>

#endif

#include "fcc.h"
#include <errno.h>
#include <krb5/osconf.h>

int krb5_fcc_default_format = KRB5_FCC_DEFAULT_FVNO;

#ifdef KRB5_USE_INET
#include <netinet/in.h>
#else
 #error find some way to use net-byte-order file version numbers.
#endif

#include <krb5/libos.h>
#include <krb5/los-proto.h>
#include <stdio.h>

#ifdef POSIX_FILE_LOCKS
#ifndef unicos61
#include <fcntl.h>
#endif /* unicos61 */
#define SHARED_LOCK	F_RDLCK
#define EXCLUSIVE_LOCK	F_WRLCK
#define UNLOCK_LOCK	F_UNLCK
#else /* !POSIX_FILE_LOCKS */
#ifndef sysvimp
#include <sys/file.h>
#endif /* sysvimp */
#define SHARED_LOCK	LOCK_SH
#define EXCLUSIVE_LOCK	LOCK_EX
#define UNLOCK_LOCK	LOCK_UN
#endif /* POSIX_FILE_LOCKS */

#define LOCK_IT 0
#define UNLOCK_IT 1

static krb5_error_code fcc_lock_file PROTOTYPE((krb5_fcc_data *,
						int, int));

static krb5_error_code
fcc_lock_file(data, fd, lockunlock)
krb5_fcc_data *data;
int fd;
int lockunlock;
{
    /* XXX need to in-line lock_file.c here, but it's sort-of OK since
       we're already unix-dependent for file descriptors */

#ifdef POSIX_FILE_LOCKS
#ifdef OSF_DCE
    struct timespec wait_for_lock = {0,200}; 
    int lock_cmd = F_SETLK;
#else
    int lock_cmd = F_SETLKW;
#endif /* OSF_DCE */
    struct flock lock_arg;
#define lock_flag lock_arg.l_type
    lock_flag = -1;
#else
    int lock_flag = -1;
#endif

    if (lockunlock == LOCK_IT)
	switch (data->mode) {
	case FCC_OPEN_RDONLY:
	    lock_flag = SHARED_LOCK;
	    break;
	case FCC_OPEN_RDWR:
	case FCC_OPEN_AND_ERASE:
        case FCC_OPEN_FOR_ERASE:
	    lock_flag = EXCLUSIVE_LOCK;
	    break;
	}
    else
	lock_flag = UNLOCK_LOCK;

    if (lock_flag == -1)
	return(KRB5_LIBOS_BADLOCKFLAG);

#ifdef POSIX_FILE_LOCKS
    lock_arg.l_whence = 0;
    lock_arg.l_start = 0;
    lock_arg.l_len = 0;
#ifdef OSF_DCE
    while
#else
    if 
#endif /* OSF_DCE */
       (fcntl(fd, lock_cmd, &lock_arg) == -1) {
	if (errno == EACCES || errno == EAGAIN)	/* see POSIX/IEEE 1003.1-1988,
						   6.5.2.4 */
#ifdef OSF_DCE
	    pthread_delay_np(&wait_for_lock);
	else
#else
	    return(EAGAIN);
#endif /* OSF_DCE */
	return(errno);
    }
#else
    if (flock(fd, lock_flag) == -1)
	return(errno);
#endif
    return 0;
}

krb5_error_code
krb5_fcc_close_file (id)
    krb5_ccache id;
{
     int ret;
     krb5_fcc_data *data = (krb5_fcc_data *)id->data;
     krb5_error_code retval;
#ifdef OSF_DCE
     pthread_mutex_t *mutex = (pthread_mutex_t *) data->mutex;
#endif

     if (data->fd == -1)
	 return KRB5_FCC_INTERNAL;

     retval = fcc_lock_file(data, data->fd, UNLOCK_IT);
     ret = close (data->fd);
     data->fd = -1;
#ifdef OSF_DCE
     pthread_mutex_unlock(mutex);
#endif
     if (retval)
	 return retval;
     else
     return (ret == -1) ? krb5_fcc_interpret (errno) : 0;
}

krb5_error_code
krb5_fcc_open_file (id, mode)
    krb5_ccache id;
    int mode;
{
     krb5_fcc_data *data = (krb5_fcc_data *)id->data;
     krb5_ui_2 fcc_fvno;
     int fd;
     int open_flag;
     krb5_error_code retval = 0;
#ifdef OSF_DCE
     pthread_mutex_t *mutex = (pthread_mutex_t *) data->mutex;
#endif

#ifdef OSF_DCE
     pthread_mutex_lock(mutex);
#endif
     if (data->fd != -1) {
	  /* Don't know what state it's in; shut down and start anew.  */
	  (void) fcc_lock_file(data, data->fd, UNLOCK_IT);
	  (void) close (data->fd);
	  data->fd = -1;
     }
     data->mode = mode;
     switch(mode) {
     case FCC_OPEN_AND_ERASE:
	 open_flag = O_CREAT|O_TRUNC|O_RDWR;
	 break;
     case FCC_OPEN_RDWR:
     case FCC_OPEN_FOR_ERASE:
	 open_flag = O_RDWR;
	 break;
     case FCC_OPEN_RDONLY:
     default:
	 open_flag = O_RDONLY;
	 break;
     }

     fd = open (data->filename, open_flag, 0600);
     if (fd == -1) {
	 retval = krb5_fcc_interpret (errno);
#ifdef OSF_DCE
         pthread_mutex_unlock(mutex);
#endif
         return retval;
     }

     if (retval = fcc_lock_file(data, fd, LOCK_IT)) {
	 (void) close(fd);
#ifdef OSF_DCE
         pthread_mutex_unlock(mutex);
#endif
	 return retval;
     }
	 
     if (mode == FCC_OPEN_AND_ERASE) {
	 /* write the version number */
	 int errsave, cnt;

#ifdef OSF_DCE
         /*
          * Retrieve krb5 file format vno from dce_cf.db
          */
         sec_login_pvt_get_krb5_ccache_vno(&krb5_fcc_default_format);
#endif /* OSF_DCE */

	 fcc_fvno = htons(krb5_fcc_default_format);
	 data->version = krb5_fcc_default_format;
	 if ((cnt = write(fd, (char *)&fcc_fvno, sizeof(fcc_fvno))) !=
	     sizeof(fcc_fvno)) {
	     errsave = errno;
	     (void) fcc_lock_file(data, fd, UNLOCK_IT);
	     (void) close(fd);
#ifdef OSF_DCE
             pthread_mutex_unlock(mutex);
#endif
	     return (cnt == -1) ? krb5_fcc_interpret(errsave) : KRB5_CC_IO;
	 }
     } else if (mode != FCC_OPEN_FOR_ERASE) {
	 /* verify a valid version number is there */
	 if (read(fd, (char *)&fcc_fvno, sizeof(fcc_fvno)) !=
	     sizeof(fcc_fvno)) {
	     (void) fcc_lock_file(data, fd, UNLOCK_IT);
	     (void) close(fd);
#ifdef OSF_DCE
             pthread_mutex_unlock(mutex);
#endif
	     return KRB5_CCACHE_BADVNO;
	 }
	 if ((fcc_fvno != htons(KRB5_FCC_FVNO_3)) &&
	     (fcc_fvno != htons(KRB5_FCC_FVNO_2)) &&
	     (fcc_fvno != htons(KRB5_FCC_FVNO_1))) {
	     (void) fcc_lock_file(data, fd, UNLOCK_IT);
	     (void) close(fd);
#ifdef OSF_DCE
             pthread_mutex_unlock(mutex);
#endif
	     return KRB5_CCACHE_BADVNO;
	 }
	 data->version = ntohs(fcc_fvno);
     }
     data->fd = fd;
     return 0;
}
