/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_lrgy.c,v $
 * Revision 1.1.16.2  1996/11/13  18:00:10  arvind
 * 	Merge to DCE 1.2.2
 * 	[1996/10/30  23:04 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth2/1]
 *
 * 	sec_lrgy_encode_kdc_rep: fix memory leak of encrypt_key.
 * 	[1996/10/28  20:44 UTC  mullan_s  /main/DCE_1.2.2/2]
 *
 * 	Merge to DCE_1.2.2
 * 	[1996/10/28  15:11 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/3]
 *
 * 	Fix EXTERN/INITVAL macro duplication.
 * 	[1996/10/24  17:05 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/2]
 *
 * 	Add sec_lrgy_encode_kdc_rep
 * 	[1996/10/14  14:34 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/1]
 *
 * 	Add sec_lrgy_krbtgt_lookup.
 * 	[1996/09/26  23:16 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.16.1  1996/10/03  14:48:00  arvind
 * 	Mark as volatile variables live across setjmp
 * 	[1996/09/16  20:31 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 * 
 * Revision 1.1.14.2  1996/02/18  00:07:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:58:35  marty]
 * 
 * Revision 1.1.14.1  1995/12/13  16:23:51  root
 * 	Submit
 * 	[1995/12/11  15:13:43  root]
 * 
 * Revision 1.1.9.2  1993/11/09  15:21:14  mdf
 * 	Only unlock LRGY_MUTEX if failure in lrgy_write_lock
 * 	[1993/11/05  21:02:19  mdf]
 * 
 * Revision 1.1.9.1  1993/11/04  20:30:13  mdf
 * 	CHFts09117, OT9359 Fixed so that locks are not being left around if
 * 	lrgy_read_lock or lrgy_write_lock fail.
 * 	[1993/11/04  18:02:34  mdf]
 * 
 * Revision 1.1.7.7  1993/02/09  16:24:46  burati
 * 	CR6493 Use mutex locks around advisory locks (flock or fcntl)
 * 	because of the incorrect behavior of those calls wrt threads.
 * 	Put TRY/CATCH around lock/unlock to make sure advisory locks
 * 	aren't left locked (which would keep other processes from locking).
 * 	[1993/02/09  00:20:55  burati]
 * 
 * Revision 1.1.7.6  1993/01/22  23:09:45  burati
 * 	CR6726 Fix memory leaks (var_ptr wasn't being free'd)
 * 	[1993/01/22  20:33:57  burati]
 * 
 * Revision 1.1.7.5  1992/12/29  19:13:49  burati
 * 	CR6480 Change F_RDLCK to F_WRLCK in lrgy_write_lock
 * 	[1992/12/29  17:33:44  burati]
 * 
 * Revision 1.1.7.4  1992/12/29  13:04:30  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:42:03  zeliff]
 * 
 * Revision 1.1.7.3  1992/12/17  19:35:31  burati
 * 	CR6519 Wasn't keeping track of when to close dummy_fd correctly
 * 	[1992/12/17  19:31:39  burati]
 * 
 * Revision 1.1.7.2  1992/09/29  21:16:28  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:20:18  sekhar]
 * 
 * Revision 1.1.2.6  1992/06/09  18:47:04  burati
 * 	CR4096 pw_shell not copied                                         d
 * 	CR4095 function call has incorrect arguments                       d
 * 	[1992/06/09  18:09:05  burati]
 * 
 * Revision 1.1.2.5  1992/05/14  18:58:14  burati
 * 	Fix up off by 1 problem on replacement routine
 * 	[1992/05/14  01:51:59  burati]
 * 
 * Revision 1.1.2.4  1992/04/27  18:57:28  burati
 * 	Be paranoid and globalize name in sec_lrgy_entry_lookup in case caller didn't
 * 	[1992/04/27  18:54:04  burati]
 * 
 * 	Changed two status codes to match new ones in sec.msg (now < 31 chars)
 * 	[1992/04/24  18:07:53  burati]
 * 
 * Revision 1.1.2.3  1992/04/03  21:20:55  burati
 * 	CR2419 Porting change (O_RDONLY -> O_RDWR) to handle bug in RIOS flock(...)
 * 	[1992/04/02  20:27:48  burati]
 * 
 * Revision 1.1.2.2  1992/03/31  22:03:20  burati
 * 	CR2419 Porting changes to make non __STDC__ case work.
 * 	[1992/03/31  01:32:01  burati]
 * 
 * 	CR2419 Initial version of local registry routines.
 * 	[1992/03/30  21:55:20  burati]
 * 
 * $EndLog$
 */

/*  sec_lrgy.c
**
** Copyright (c) Hewlett-Packard Company 1991, 1992, 1993
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/* 
 *  Local Registry routines
 */

#include <sys/types.h>
#include <sys/errno.h> 
#include <sys/file.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h> 

#include <pthread.h>
#include <dce/sec_login.h>

#define SEC_LRGY_MAIN
#include <sec_lrgy.h>
#include <sec_krb_util.h>
#include <macros.h>
#include <u_str.h>
#include <un_io.h>
#include <un_time.h>
#include <bstring.h>

/*
 * Private data types
 */

/* Fixed portion of entries */
typedef struct _sec_lrgy_fixed_data_t {
    unsigned32              cell_name_len;
    unsigned32              princ_name_len;
    sec_timeval_sec_t       timestamp;
        /* unix identity */
    signed32                pw_uid;
    signed32                pw_gid;
    time_t                  pw_change;
    time_t                  pw_expire;
    unsigned32              num_unix_groups;
    unsigned32              pw_name_len;
    unsigned32              pw_passwd_len;
    unsigned32              pw_class_len;
    unsigned32              pw_gecos_len;
    unsigned32              pw_dir_len;
    unsigned32              pw_shell_len;

        /* admin info */
    sec_timeval_sec_t       acct_exp;
    sec_timeval_sec_t       passwd_exp;
    sec_timeval_sec_t       identity_exp;
    boolean32               passwd_valid;

       /* network identity */
    unsigned32              ppac_len;

    unsigned32              var_len;
} sec_lrgy_fixed_data_t;

typedef struct _sec_lrgy_tgt_fixed_t {
    unsigned32              cell_name_len;
    unsigned32              princ_name_len;
    unsigned32              timestamp;
    krb5_flags              options;
    krb5_enctype            enc_type;
    krb5_int32              nonce;
    int                     reply_len;
    unsigned32              var_len;
} sec_lrgy_tgt_fixed_t;

typedef struct _sec_lrgy_pvt_cursor_t {
    unsigned32              index;
    time_t                  last_update_time;
} sec_lrgy_pvt_cursor_t;
#define LAST_UPDATE_TIME(cursor) ((sec_lrgy_pvt_cursor_t *)cursor)->last_update_time
#define ENTRY_INDEX(cursor) ((sec_lrgy_pvt_cursor_t *)cursor)->index

/* Internal state */
static boolean32 lrgy_mutex_inited = false;
static pthread_mutex_t lrgy_mutex;
static pthread_once_t  lrgy_mutex_once = pthread_once_init;

/*
 * MUTEX Locking for local registry
 * (advisory locks not usable for inter-thread locking in 1.0.2)
 */
PRIVATE void lrgy_mutex_init(
#ifdef __STDC__
    void
#endif
)
{
    pthread_mutex_init(&lrgy_mutex, pthread_mutexattr_default);
    lrgy_mutex_inited = true;
}
#define LRGY_MUTEX_INIT() \
    if (!lrgy_mutex_inited) pthread_once(&lrgy_mutex_once, lrgy_mutex_init)
#define LRGY_MUTEX_LOCK() \
    LRGY_MUTEX_INIT(); \
    pthread_mutex_lock(&lrgy_mutex);
#define LRGY_MUTEX_UNLOCK() \
    pthread_mutex_unlock(&lrgy_mutex);
    
/*
 * lrgy_read_lock -
 *      Make sure <dcelocal>/var/security exists.
 *      Lock 'lrgy_lock' file, creating new one if necessary.
 */
PRIVATE void lrgy_read_lock
#ifndef __STDC__
    (lfd, status)
    int             *lfd;
    error_status_t  *status;
#else
  (
    int             *lfd,
    error_status_t  *status
  )
#endif
{
#ifdef DCE_SEC_POSIX_F_LOCK
    struct flock    lock_arg;
#endif
    int             result;

    LRGY_MUTEX_LOCK();

    /* Make sure <dcelocal>/var/security exists */
    result = mkdir(LRGY_DIR, LRGY_DIR_PROT);
    if (result == -1) {
        if (errno != EEXIST) {
            SET_STATUS(status, sec_lrgy_s_not_found);
    		LRGY_MUTEX_UNLOCK();
			return;
        }
    } else {
        chmod(LRGY_DIR, LRGY_DIR_PROT);
    }

    *lfd = open(LRGY_LOCK_FILE, (O_RDONLY | O_CREAT), LRGY_FILE_PROT);
    if (*lfd < 0) {
        SET_STATUS(status, sec_lrgy_s_no_access);
   		LRGY_MUTEX_UNLOCK();
        return;
    } else {
#ifdef DCE_SEC_POSIX_F_LOCK
    lock_arg.l_type = F_RDLCK;
    lock_arg.l_whence = 0;
    lock_arg.l_start = 0;
    lock_arg.l_len = 0;

    if (fcntl(*lfd, F_SETLKW, &lock_arg) == -1)  {
            SET_STATUS(status, sec_lrgy_s_no_access);
            close(*lfd);
			LRGY_MUTEX_UNLOCK();
    }
#else
        if (flock(*lfd, LOCK_SH) != 0) {
            SET_STATUS(status, sec_lrgy_s_no_access);
            close(*lfd);
			LRGY_MUTEX_UNLOCK();
        }
#endif /* DCE_SEC_POSIX_F_LOCK */
    }
}


/*
 * lrgy_write_lock -
 *      Make sure <dcelocal>/var/security exists.
 *      Lock 'lrgy_lock' file, creating new one if necessary.
 */
PRIVATE void lrgy_write_lock
#ifndef __STDC__
    (lfd, status)
    int             *lfd;
    error_status_t  *status;
#else
  (
    int             *lfd,
    error_status_t  *status
  )
#endif
{
#ifdef DCE_SEC_POSIX_F_LOCK
    struct flock    lock_arg;
#endif
    int             result;

    LRGY_MUTEX_LOCK();
    /* Make sure <dcelocal>/var/security exists */
    result = mkdir(LRGY_DIR, LRGY_DIR_PROT);
    if (result == -1) {
        if (errno != EEXIST) {
            SET_STATUS(status, sec_lrgy_s_not_found);
    		LRGY_MUTEX_UNLOCK();
        	return;
        }
    } else {
        chmod(LRGY_DIR, LRGY_DIR_PROT);
    }

    *lfd = open(LRGY_LOCK_FILE, (O_RDWR | O_CREAT), LRGY_FILE_PROT);
    if (*lfd < 0) {
        SET_STATUS(status, sec_lrgy_s_no_access);
   		LRGY_MUTEX_UNLOCK();
        return;
    } else {
#ifdef DCE_SEC_POSIX_F_LOCK
    lock_arg.l_type = F_WRLCK;
    lock_arg.l_whence = 0;
    lock_arg.l_start = 0;
    lock_arg.l_len = 0;

    if (fcntl(*lfd, F_SETLKW, &lock_arg) == -1)  {
            SET_STATUS(status, sec_lrgy_s_no_access);
   	    LRGY_MUTEX_UNLOCK();
            close(*lfd);
    }
#else
        if (flock(*lfd, LOCK_EX) != 0) {
            SET_STATUS(status, sec_lrgy_s_no_access);
   	    LRGY_MUTEX_UNLOCK();
            close(*lfd);
        }
#endif /* DCE_SEC_POSIX_F_LOCK */
    }
}


/*
 * lrgy_unlock -
 *      Make sure <dcelocal>/var/security exists.
 *      Lock 'lrgy_lock' file, creating new one if necessary.
 */
PRIVATE void lrgy_unlock
#ifndef __STDC__
    (lfd)
    int             *lfd;
#else
  (
    int             *lfd
  )
#endif
{
    TRY {
#ifdef DCE_SEC_POSIX_F_LOCK
        struct flock    lock_arg;

        lock_arg.l_type = F_UNLCK;
        lock_arg.l_whence = 0;
        lock_arg.l_start = 0;
        lock_arg.l_len = 0;

        fcntl(*lfd, F_SETLKW, &lock_arg);
#else
        flock(*lfd, LOCK_UN);
#endif /* DCE_SEC_POSIX_F_LOCK */

        close(*lfd);
        LRGY_MUTEX_UNLOCK();
    } CATCH_ALL {
        LRGY_MUTEX_UNLOCK();
	RERAISE;
    } ENDTRY;
}


/*
 * lrgy_setup -
 *      Make sure <dcelocal>/var/security exists.
 *      Open existing lrgy file or create new one if necessary.
 *      Open new lrgy file if requested.
 */
PRIVATE void lrgy_setup
#ifndef __STDC__
    (fd, nfd, lrgy_hdr, for_write, status)
    int             *fd;
    int             *nfd;
    sec_lrgy_hdr_t  *lrgy_hdr;
    boolean32       for_write;
    error_status_t  *status;
#else
  (
    int             *fd,
    int             *nfd,
    sec_lrgy_hdr_t  *lrgy_hdr,
    boolean32       for_write,
    error_status_t  *status
  )
#endif
{
    int             result, nbytes, dummy_fd = -1;
    boolean32       close_dummy = false;
    sec_lrgy_hdr_t  dummy;
    CLEAR_STATUS(status);

    if (!fd) {
        fd = &dummy_fd;
        close_dummy = true;
    }
    if (!lrgy_hdr)
        lrgy_hdr = &dummy;

    /* Make sure <dcelocal>/var/security exists */
    result = mkdir(LRGY_DIR, LRGY_DIR_PROT);
    if (result == -1) {
        if (errno != EEXIST) {
            SET_STATUS(status, sec_lrgy_s_not_found);
        }
    } else {
        chmod(LRGY_DIR, LRGY_DIR_PROT);
    }

    /* Open existing lrgy file or create new one if necessary */
    *fd = -1;
    if (GOOD_STATUS(status)) {
        *fd = open(LRGY_FILE, O_RDWR, 0);
        if (*fd < 0) {
            if (errno != ENOENT) {      /* Can't get at it */
                SET_STATUS(status, sec_lrgy_s_no_access);
            } else if (!for_write) {
                SET_STATUS(status, sec_lrgy_s_not_found);
            } else {                    /* Doesn't exist yet, so create it */
                *fd = open(LRGY_FILE, (O_RDWR | O_CREAT | O_NDELAY | O_EXCL),
                            LRGY_FILE_PROT);
                if (*fd < 0) {           /* Can't create it */
                    *status = sec_lrgy_s_cannot_create;
                } else {                /* Created: initialize it */
                    chmod(LRGY_FILE, LRGY_FILE_PROT);
                    *lrgy_hdr = sec_lrgy_default_hdr;
                    nbytes = write(*fd, (char *) &sec_lrgy_default_hdr,
                                    sizeof(sec_lrgy_hdr_t));
                    if (nbytes != sizeof(sec_lrgy_hdr_t))
                        *status = sec_lrgy_s_internal_error; /* Can't write to it */
                }
            }
        } else {                        /* Read the hdr on existing file */
            nbytes = read(*fd, (char *)lrgy_hdr, sizeof(sec_lrgy_hdr_t));
            if (nbytes != sizeof(sec_lrgy_hdr_t)) {
                SET_STATUS(status, sec_lrgy_s_no_access);
            }
        }
    }

    /* Open new lrgy file if requested */
    if (GOOD_STATUS(status) && nfd) {
        result = unlink(LRGY_NEW_FILE);
        if ((result == 0) || (errno == ENOENT)) {
            *nfd = -1;
            *nfd = open(LRGY_NEW_FILE, (O_RDWR | O_CREAT | O_NDELAY | O_EXCL), 0);
            if (*nfd < 0) {
                SET_STATUS(status, sec_lrgy_s_cannot_create);
            } else {
                chmod(LRGY_NEW_FILE, LRGY_FILE_PROT);
                nbytes = write(*nfd,(char *)lrgy_hdr, sizeof(sec_lrgy_hdr_t));
                if (nbytes != sizeof(sec_lrgy_hdr_t)) {
                    SET_STATUS(status, sec_lrgy_s_internal_error);
                }
            }
        } else {
            SET_STATUS(status, sec_lrgy_s_no_access);
        }
    }
    if ((close_dummy) && (dummy_fd != -1)) {
        close(dummy_fd);
    }
}


/*
 * lrgy_tgt_setup -
 *      Make sure <dcelocal>/var/security exists.
 *      Open existing lrgy_tgt file or create new one if necessary.
 */
PRIVATE void lrgy_setup_tgt
#ifndef __STDC__
    (fd, nfd, num_entries, status)
    int             *fd;
    int             *nfd;
    int             *num_entries;
    error_status_t  *status;
#else
  (
    int             *fd,
    int             *nfd,
    int             *num_entries,
    error_status_t  *status
  )
#endif
{
    int result, nbytes, dummy_fd = -1;
    boolean32 close_dummy = false;
    CLEAR_STATUS(status);
    if (!fd) {
        fd = &dummy_fd;
        close_dummy = true;
    }

    /* Make sure <dcelocal>/var/security exists */
    result = mkdir(LRGY_DIR, LRGY_DIR_PROT);
    if (result == -1) {
        if (errno != EEXIST) {
            *status = sec_lrgy_s_no_access;
        }
    } else {
        chmod(LRGY_DIR, LRGY_DIR_PROT);
    }

    /* Open existing lrgy_tgt file or create new one if necessary */
    *fd = -1;
    if (GOOD_STATUS(status)) {
        *fd = open(LRGY_TGT_FILE, O_RDWR, 0);
        if (*fd < 0) {
            if (errno != ENOENT) {      /* Can't get at it */
                *status = sec_lrgy_s_no_access;
            } else {                    /* Doesn't exist yet */
                *fd = open( LRGY_TGT_FILE,
                            (O_RDWR | O_CREAT | O_NDELAY | O_EXCL),
                            LRGY_FILE_PROT );
                if (*fd < 0) {           /* Can't create it */
                    *status = sec_lrgy_s_cannot_create;
                } else {                /* Created: initialize it */
                    chmod(LRGY_TGT_FILE, LRGY_FILE_PROT);
                    *num_entries = 0;
                    nbytes = write(*fd, (char *) num_entries, sizeof(int));
                    if (nbytes != sizeof(int))
                        *status = sec_lrgy_s_internal_error; /* Can't write to it */
                }
            }
        } else {                        /* Read the hdr on existing file */
            nbytes = read(*fd, (char *) num_entries, sizeof(int));
            if (nbytes != sizeof(int)) {
                *status = sec_lrgy_s_no_access;
            }
        }
    }

    /* Open new lrgy_tgts file if requested */
    if (GOOD_STATUS(status) && nfd) {
        result = unlink(LRGY_NEW_TGT_FILE);
        if ((result == 0) || (errno == ENOENT)) {
            *nfd = -1;
            *nfd = open(LRGY_NEW_TGT_FILE, (O_RDWR | O_CREAT | O_NDELAY | O_EXCL), 0);
            if (*nfd < 0) {
                SET_STATUS(status, sec_lrgy_s_cannot_create);
            } else {
                chmod(LRGY_NEW_TGT_FILE, LRGY_FILE_PROT);
                nbytes = write(*nfd, (char *) num_entries, sizeof(int));
                if (nbytes != sizeof(int)) {
                    SET_STATUS(status, sec_lrgy_s_internal_error);
                }
            }
        } else {
            SET_STATUS(status, sec_lrgy_s_no_access);
        }
    }
    if ((close_dummy) && (dummy_fd != -1)) {
        close(dummy_fd);
    }
}


/* 
 * lrgy_pack_tgt_data - Pack the specified data into form suitable for storage
 *
 *  Pack the varying length fields from var_rec, based
 *  on lengths given in entry, into a stream of bytes.
 */
PRIVATE void lrgy_pack_tgt_data
#ifndef __STDC__
    (entry, fixed, var_ptr, status)
    sec_lrgy_tgt_entry_t    *entry;
    sec_lrgy_tgt_fixed_t    *fixed;
    char                    **var_ptr;
    error_status_t          *status;
#else
  (
    sec_lrgy_tgt_entry_t    *entry,
    sec_lrgy_tgt_fixed_t    *fixed,
    char                    **var_ptr,
    error_status_t          *status
  )
#endif
{
    char    *p;
    struct  timeval tm;

    fixed->options      = entry->options;
    fixed->enc_type     = entry->enc_type;
    fixed->nonce        = entry->nonce;

    fixed->var_len = 0;
    fixed->var_len += fixed->cell_name_len = u_strlen(entry->cell);
    fixed->var_len += fixed->princ_name_len = u_strlen(entry->principal);
    fixed->var_len += fixed->reply_len = entry->reply_len;

#ifdef SNI_SVR4_POSIX
    gettimeofday(&tm);
#else
    gettimeofday(&tm, 0);
#endif /* SNI_SVR4_POSIX */

    fixed->timestamp = tm.tv_sec;

        /* allocate and populate varying portion */
    p = *var_ptr = (char *) malloc(fixed->var_len);
    if (*var_ptr == NULL) {
        SET_STATUS(status, sec_s_no_memory);
    } else {
        bcopy(entry->cell, p, fixed->cell_name_len);
        p += fixed->cell_name_len;
        bcopy(entry->principal, p, fixed->princ_name_len);
        p += fixed->princ_name_len;
        bcopy(entry->reply, p, fixed->reply_len);
    }
}


/* 
 * lrgy_pack_data - Pack the specified data into form suitable for storage.
 *
 *  Pack the varying length fields from var_rec, based
 *  on lengths given in entry, into a stream of bytes.
 */
PRIVATE void lrgy_pack_data
#ifndef __STDC__
    (entry, fixed, var_ptr, status)
    sec_lrgy_entry_t        *entry;
    sec_lrgy_fixed_data_t   *fixed;
    char                    **var_ptr;
    error_status_t          *status;
#else
  (
    sec_lrgy_entry_t        *entry,
    sec_lrgy_fixed_data_t   *fixed,
    char                    **var_ptr,
    error_status_t          *status
  )
#endif
{
    char    *p;
    struct  timeval tm;

    fixed->pw_uid           = entry->pw_uid;
    fixed->pw_gid           = entry->pw_gid;
    fixed->pw_change        = entry->pw_change;
    fixed->pw_expire        = entry->pw_expire;
    fixed->num_unix_groups  = entry->num_unix_groups;
    fixed->acct_exp         = entry->acct_exp;
    fixed->passwd_exp       = entry->passwd_exp;
    fixed->identity_exp     = entry->identity_exp;
    fixed->passwd_valid     = entry->passwd_valid;

    fixed->var_len = 0;
    fixed->var_len += fixed->cell_name_len  = u_strlen(entry->cell);
    fixed->var_len += fixed->princ_name_len = u_strlen(entry->principal);
    fixed->var_len += fixed->pw_name_len    = u_strlen(entry->pw_name);
    fixed->var_len += fixed->pw_passwd_len  = u_strlen(entry->pw_passwd);
    fixed->var_len += fixed->pw_class_len   = u_strlen(entry->pw_class);
    fixed->var_len += fixed->pw_gecos_len   = u_strlen(entry->pw_gecos);
    fixed->var_len += fixed->pw_dir_len     = u_strlen(entry->pw_dir);
    fixed->var_len += fixed->pw_shell_len   = u_strlen(entry->pw_shell);
    fixed->var_len += fixed->ppac_len       = entry->ppac_len;
    fixed->var_len += fixed->num_unix_groups * sizeof(signed32);

#ifdef SNI_SVR4_POSIX
    gettimeofday(&tm);
#else
    gettimeofday(&tm, 0);
#endif /* SNI_SVR4_POSIX */

    fixed->timestamp = tm.tv_sec;

        /* allocate and populate varying portion */
    p = *var_ptr = (char *) malloc(fixed->var_len);
    if (*var_ptr == NULL) {
        SET_STATUS(status, sec_s_no_memory);
    } else {
        bcopy(entry->cell, p, fixed->cell_name_len);
        p += fixed->cell_name_len;
        bcopy(entry->principal, p, fixed->princ_name_len);
        p += fixed->princ_name_len;
        bcopy(entry->pw_name, p, fixed->pw_name_len);
        p += fixed->pw_name_len;
        bcopy(entry->pw_passwd, p, fixed->pw_passwd_len);
        p += fixed->pw_passwd_len;
        bcopy(entry->pw_class, p, fixed->pw_class_len);
        p += fixed->pw_class_len;
        bcopy(entry->pw_gecos, p, fixed->pw_gecos_len);
        p += fixed->pw_gecos_len;
        bcopy(entry->pw_dir, p, fixed->pw_dir_len);
        p += fixed->pw_dir_len;
        bcopy(entry->pw_shell, p, fixed->pw_shell_len);
        p += fixed->pw_shell_len;
        bcopy(entry->groups, p, (fixed->num_unix_groups * sizeof(signed32)) );
        p += (fixed->num_unix_groups * sizeof(signed32));
        bcopy(entry->ppac, p, fixed->ppac_len);
    }
}


/* 
 * lrgy_unpack_data - Unpack the specified data from storage format.
 */
PRIVATE void lrgy_unpack_data
#ifndef __STDC__
    (entry, fixed, var_ptr, status)
    sec_lrgy_entry_t        *entry;
    sec_lrgy_fixed_data_t   *fixed;
    char                    *var_ptr;
    error_status_t          *status;
#else
  (
    sec_lrgy_entry_t        *entry,
    sec_lrgy_fixed_data_t   *fixed,
    char                    *var_ptr,
    error_status_t          *status
  )
#endif
{
    char        *p;
    unsigned32  grp_len;

    p = var_ptr;
    entry->pw_uid           = fixed->pw_uid;
    entry->pw_gid           = fixed->pw_gid;
    entry->pw_change        = fixed->pw_change;
    entry->pw_expire        = fixed->pw_expire;
    entry->num_unix_groups  = fixed->num_unix_groups;
    entry->acct_exp         = fixed->acct_exp;
    entry->passwd_exp       = fixed->passwd_exp;
    entry->identity_exp     = fixed->identity_exp;
    entry->passwd_valid     = fixed->passwd_valid;
    entry->ppac_len         = fixed->ppac_len;

    entry->cell         = (char *)malloc(fixed->cell_name_len+1);
    bcopy(p, entry->cell, fixed->cell_name_len);
    p += fixed->cell_name_len;
    entry->cell[fixed->cell_name_len] = '\0';

    entry->principal    = (char *)malloc(fixed->princ_name_len+1);
    bcopy(p, entry->principal, fixed->princ_name_len);
    p += fixed->princ_name_len;
    entry->principal[fixed->princ_name_len] = '\0';

    entry->pw_name      = (char *)malloc(fixed->pw_name_len+1);
    bcopy(p, entry->pw_name, fixed->pw_name_len);
    p += fixed->pw_name_len;
    entry->pw_name[fixed->pw_name_len] = '\0';

    entry->pw_passwd    = (char *)malloc(fixed->pw_passwd_len+1);
    bcopy(p, entry->pw_passwd, fixed->pw_passwd_len);
    p += fixed->pw_passwd_len;
    entry->pw_passwd[fixed->pw_passwd_len] = '\0';

    entry->pw_class     = (char *)malloc(fixed->pw_class_len+1);
    bcopy(p, entry->pw_class, fixed->pw_class_len);
    p += fixed->pw_class_len;
    entry->pw_class[fixed->pw_class_len] = '\0';

    entry->pw_gecos     = (char *)malloc(fixed->pw_gecos_len+1);
    bcopy(p, entry->pw_gecos, fixed->pw_gecos_len);
    p += fixed->pw_gecos_len;
    entry->pw_gecos[fixed->pw_gecos_len] = '\0';

    entry->pw_dir       = (char *)malloc(fixed->pw_dir_len+1);
    bcopy(p, entry->pw_dir, fixed->pw_dir_len);
    p += fixed->pw_dir_len;
    entry->pw_dir[fixed->pw_dir_len] = '\0';

    entry->pw_shell     = (char *)malloc(fixed->pw_shell_len+1);
    bcopy(p, entry->pw_shell, fixed->pw_shell_len);
    p += fixed->pw_shell_len;
    entry->pw_shell[fixed->pw_shell_len] = '\0';

    if (fixed->num_unix_groups > 0) {
        grp_len = fixed->num_unix_groups * sizeof(signed32);
        entry->groups = (char *) malloc(grp_len);
        bcopy(p, entry->groups, grp_len);
        p += grp_len;
    } else {
        entry->groups = NULL;
    }

    entry->ppac         = (char *)malloc(fixed->ppac_len);
    bcopy(p, entry->ppac, fixed->ppac_len);
}


/*
 * Search for matching entry and oldest entry.
 *  Simple linear search, assumes max_entries has not been increased
 *  too many orders of magnitude over it's default of 25.
 */
PRIVATE boolean32 lrgy_search
#ifndef __STDC__
    (fd, mpos, opos, new_gname, num_entries, mentry, status)
    int                     fd;
    off_t                   *mpos;
    off_t                   *opos;
    unsigned_char_p_t       new_gname;
    int                     num_entries;
    sec_lrgy_entry_t        *mentry;
    error_status_t          *status;
#else
  (
    int                     fd,
    off_t                   *mpos,
    off_t                   *opos,
    unsigned_char_p_t       new_gname,
    int                     num_entries,
    sec_lrgy_entry_t        *mentry,
    error_status_t          *status
  )
#endif
{
    off_t                   tmp_pos;
    int                     nbytes, i;
    char                    *p, *varying, *name;
    unsigned_char_p_t       global_name;
    unsigned32              oldest_ts = 0;
    sec_lrgy_fixed_data_t   fixed;
    sec_rgy_name_t          cell, princ;
    boolean32               match = false;

    /* Advance past the header */
    tmp_pos = lseek(fd, sizeof(sec_lrgy_hdr_t), L_SET);
    if (mpos)
        *mpos = tmp_pos;
    if (opos)
        *opos = tmp_pos;

    for (i = 0;(i < num_entries) && GOOD_STATUS(status); i++) {

        nbytes = read(fd, (char *) &fixed, sizeof(sec_lrgy_fixed_data_t));
        if (nbytes != sizeof(sec_lrgy_fixed_data_t)) {
            SET_STATUS(status, sec_lrgy_s_internal_error);
        } else {
            if (opos && ((fixed.timestamp < oldest_ts) || (oldest_ts == 0)) ) {
                oldest_ts = fixed.timestamp;
                *opos = tmp_pos;
            }
            varying = (char *)malloc(fixed.var_len);
            if (varying) {
                nbytes = read(fd, (char *) varying, fixed.var_len);
                if (nbytes != fixed.var_len) {
                    SET_STATUS(status, sec_lrgy_s_internal_error);
                    free(varying);
                    return false;
                }
                name = (char *) malloc(fixed.cell_name_len + 2 +
                                        fixed.princ_name_len);
                if (!name) {
                    SET_STATUS(status, sec_s_no_memory);
                    free(varying);
                    return false;
                }
                p = varying;
                bcopy(p, name, fixed.cell_name_len);
                name[fixed.cell_name_len] = '/';
                p += fixed.cell_name_len;
                bcopy(p, &(name[fixed.cell_name_len+1]),
                        fixed.princ_name_len);
                name[fixed.cell_name_len + fixed.princ_name_len + 1] = '\0';

                    /* Make sure global name is in canonical form */
                *status = sec_id_globalize_name((unsigned_char_p_t) name,
                                                &global_name);
                    /* Does it match? */
                if (u_strcmp((char *)global_name, (char *)new_gname)==0) {
                    if (mpos)
                        *mpos = tmp_pos;
                    match = true;
                    /* If caller wants data, give it to them */
                    if (mentry)
                        lrgy_unpack_data(mentry, &fixed, varying, status);
                    /* If caller doesn't care about oldest, then we're done */
                    if (!opos)
                        i = num_entries;
                }
                    /* Clean up */
                if (GOOD_STATUS(status) && (global_name))
                    free((char *)global_name);
                free(name);
                free(varying);
            } else {
                SET_STATUS(status, sec_s_no_memory);
            }
        }
        tmp_pos = tmp_pos + sizeof(sec_lrgy_fixed_data_t) + fixed.var_len;
    }
    return match;
}


/* 
 * lrgy_unpack_tgt_data - Unpack the tgt data from storage format.
 */
PRIVATE void lrgy_unpack_tgt_data
#ifndef __STDC__
    (entry, fixed, var_ptr, status)
    sec_lrgy_tgt_entry_t    *entry;
    sec_lrgy_tgt_fixed_t    *fixed;
    char                    *var_ptr;
    error_status_t          *status;
#else
  (
    sec_lrgy_tgt_entry_t    *entry,
    sec_lrgy_tgt_fixed_t    *fixed,
    char                    *var_ptr,
    error_status_t          *status
  )
#endif
{
    char        *p;
    unsigned32  grp_len;

    p = var_ptr;
    entry->options          = fixed->options;
    entry->enc_type         = fixed->enc_type;
    entry->nonce            = fixed->nonce;
    entry->reply_len        = fixed->reply_len;

    entry->cell             = (char *)malloc(fixed->cell_name_len+1);
    bcopy(p, entry->cell, fixed->cell_name_len);
    p += fixed->cell_name_len;
    entry->cell[fixed->cell_name_len] = '\0';
    entry->principal        = (char *)malloc(fixed->princ_name_len+1);
    bcopy(p, entry->principal, fixed->princ_name_len);
    p += fixed->princ_name_len;
    entry->principal[fixed->princ_name_len] = '\0';

    entry->reply            = (char *)malloc(fixed->reply_len);
    bcopy(p, entry->reply, fixed->reply_len);
}


/*
 * Search for matching tgt entry and oldest tgt entry.
 *  Simple linear search, assumes max_entries has not been increased
 *  too many orders of magnitude over it's default of 25.
 */
PRIVATE boolean32 lrgy_tgt_search
#ifndef __STDC__
    (fd, mpos, opos, new_gname, num_entries, mentry, status)
    int                     fd;
    off_t                   *mpos;
    off_t                   *opos;
    unsigned_char_p_t       new_gname;
    int                     num_entries;
    sec_lrgy_tgt_entry_t    *mentry;
    error_status_t          *status;
#else
  (
    int                     fd,
    off_t                   *mpos,
    off_t                   *opos,
    unsigned_char_p_t       new_gname,
    int                     num_entries,
    sec_lrgy_tgt_entry_t    *mentry,
    error_status_t          *status
  )
#endif
{
    off_t                   tmp_pos;
    int                     nbytes, i;
    char                    *p, *varying, *name;
    unsigned_char_p_t       global_name;
    unsigned32              oldest_ts = 0;
    sec_lrgy_tgt_fixed_t    fixed;
    sec_rgy_name_t          cell, princ;
    boolean32               match = false;

    /* Advance past the header */
    tmp_pos = lseek(fd, sizeof(int), L_SET);
    if (mpos)
        *mpos = tmp_pos;
    if (opos)
        *opos = tmp_pos;

    for (i = 0;(i < num_entries) && GOOD_STATUS(status); i++) {

        nbytes = read(fd, (char *) &fixed, sizeof(sec_lrgy_tgt_fixed_t));
        if (nbytes != sizeof(sec_lrgy_tgt_fixed_t)) {
            SET_STATUS(status, sec_lrgy_s_internal_error);
        } else {
            if (opos && ((fixed.timestamp < oldest_ts) || (oldest_ts == 0)) ) {
                oldest_ts = fixed.timestamp;
                *opos = tmp_pos;
            }
            varying = (char *)malloc(fixed.var_len);
            if (varying) {
                nbytes = read(fd, (char *) varying, fixed.var_len);
                if (nbytes != fixed.var_len) {
                    SET_STATUS(status, sec_lrgy_s_internal_error);
                    free(varying);
                    return false;
                }
                name = (char *) malloc(fixed.cell_name_len + 2 +
                                        fixed.princ_name_len);
                if (!name) {
                    SET_STATUS(status, sec_s_no_memory);
                    free(varying);
                    return false;
                }
                p = varying;
                bcopy(p, name, fixed.cell_name_len);
                name[fixed.cell_name_len] = '/';
                p += fixed.cell_name_len;
                bcopy(p, &(name[fixed.cell_name_len+1]),
                        fixed.princ_name_len);
                name[fixed.cell_name_len + fixed.princ_name_len + 1] = '\0';

                    /* Make global name is in canonical form */
                *status = sec_id_globalize_name((unsigned_char_p_t) name,
                                                &global_name);
                    /* Does it match? */
                if (u_strcmp((char *)global_name, (char *)new_gname)==0) {
                    if (mpos)
                        *mpos = tmp_pos;
                    match = true;
                    /* If caller wants data, give it to them */
                    if (mentry)
                        lrgy_unpack_tgt_data(mentry, &fixed, varying, status);
                    /* If caller doesn't care about oldest, then we're done */
                    if (!opos)
                        i = num_entries;
                }
                    /* Clean up */
                if (GOOD_STATUS(status) && (global_name))
                    free((char *)global_name);
                free(name);
                free(varying);
            } else {
                SET_STATUS(status, sec_s_no_memory);
            }
        }
        tmp_pos = tmp_pos + sizeof(sec_lrgy_tgt_fixed_t) + fixed.var_len;
    }
    return match;
}


/*
 * Copy all entries from fd to nfd
 */
PRIVATE void lrgy_copy_entries
#ifndef __STDC__
    (fd, nfd, properties, status)
    int             fd;
    int             nfd;
    sec_lrgy_hdr_t  *properties;
    error_status_t  *status;
#else
  (
    int             fd,
    int             nfd,
    sec_lrgy_hdr_t  *properties,
    error_status_t  *status
  )
#endif
{
    int                     i, nbin, nbout;
    off_t                   pos;
    char                    *varying;
    sec_lrgy_fixed_data_t   fixed;

    /* Make sure we're starting after the header, before the entries */
    pos = lseek(fd, sizeof(sec_lrgy_hdr_t), L_SET);

    for (i = 0;(GOOD_STATUS(status)) && (i < properties->num_entries); i++) {

        nbin = read(fd, (char *) &fixed, sizeof(sec_lrgy_fixed_data_t));
        if (nbin != sizeof(sec_lrgy_fixed_data_t)) {
            SET_STATUS(status, sec_lrgy_s_internal_error);
        } else {
            nbout = write(nfd, (char *) &fixed, sizeof(sec_lrgy_fixed_data_t));
            if (nbout != sizeof(sec_lrgy_fixed_data_t)) {
                SET_STATUS(status, sec_lrgy_s_internal_error);
            } else {
                varying = (char *) malloc(fixed.var_len);
                if (!varying) {
                    SET_STATUS(status, sec_s_no_memory);
                } else {
                    nbin = read(fd, (char *) varying, fixed.var_len);
                    if (nbin != fixed.var_len) {
                        SET_STATUS(status, sec_lrgy_s_internal_error);
                    } else {
                        nbout = write(nfd, (char *) varying, fixed.var_len);
                        if (nbout != fixed.var_len) {
                            SET_STATUS(status, sec_lrgy_s_internal_error);
                        } /* write varying */
                    } /* read varying */
                    free(varying);
                } /* allocate varying */
            } /* write entry */
        } /* read entry */

    }   /* for num_entries loop */

        /* If all went well, update num_entries */
    if (GOOD_STATUS(status)) {
        properties->num_entries++;
        pos = lseek(nfd, 0, L_SET);
        if (pos == 0) {
            nbout = write(nfd, (char *) properties, sizeof(sec_lrgy_hdr_t));
            if (nbout != sizeof(sec_lrgy_hdr_t)) {
                SET_STATUS(status, sec_lrgy_s_internal_error);
            }
        } else {
            SET_STATUS(status, sec_lrgy_s_internal_error);
        }
    }
}


/*
 * Copy all entries from fd to nfd, except the one at offset 'epos'.
 */
PRIVATE void lrgy_copy_entries_except
#ifndef __STDC__
    (fd, nfd, reset_nfd, properties, epos, status)
    int             fd;
    int             nfd;
    boolean32       reset_nfd;
    sec_lrgy_hdr_t  *properties;
    off_t           epos;
    error_status_t  *status;
#else
  (
    int             fd,
    int             nfd,
    boolean32       reset_nfd,
    sec_lrgy_hdr_t  *properties,
    off_t           epos,
    error_status_t  *status
  )
#endif
{
    int                     i, nbin, nbout;
    off_t                   pos;
    char                    *varying;
    sec_lrgy_fixed_data_t   fixed;

    /* Make sure we're starting after the header, before the entries */
    pos = lseek(fd, sizeof(sec_lrgy_hdr_t), L_SET);

    /* Only start at beginning of new file if caller didn't
     * already start writing to it.
     */
    if (reset_nfd) {
        pos = lseek(nfd, sizeof(sec_lrgy_hdr_t), L_SET);
    }

    for (i = 0;(GOOD_STATUS(status)) && (i < properties->num_entries); i++) {

            /* Do we skip this one? */
        if (lseek(fd, 0, L_INCR) == epos) {

            nbin = read(fd, (char *) &fixed, sizeof(sec_lrgy_fixed_data_t));
            if (nbin != sizeof(sec_lrgy_fixed_data_t)) {
                SET_STATUS(status, sec_lrgy_s_internal_error);
            } else {    /* Skip over the varying part of the record */
                lseek(fd, fixed.var_len, L_INCR);
            }

        } else {  /* Didn't skip over, so copy it */

            nbin = read(fd, (char *) &fixed, sizeof(sec_lrgy_fixed_data_t));
            if (nbin != sizeof(sec_lrgy_fixed_data_t)) {
                SET_STATUS(status, sec_lrgy_s_internal_error);
            } else {
                nbout = write(nfd, (char *) &fixed, sizeof(sec_lrgy_fixed_data_t));
                if (nbout != sizeof(sec_lrgy_fixed_data_t)) {
                    SET_STATUS(status, sec_lrgy_s_internal_error);
                } else {
                    varying = (char *) malloc(fixed.var_len);
                    if (!varying) {
                        SET_STATUS(status, sec_s_no_memory);
                    } else {
                        nbin = read(fd, (char *) varying, fixed.var_len);
                        if (nbin != fixed.var_len) {
                            SET_STATUS(status, sec_lrgy_s_internal_error);
                        } else {
                            nbout = write(nfd, (char *) varying, fixed.var_len);
                            if (nbout != fixed.var_len) {
                                SET_STATUS(status, sec_lrgy_s_internal_error);
                            } /* write varying */
                        } /* read varying */
                        free(varying);
                    } /* allocate varying */
                } /* write entry */
            } /* read entry */
        } /* skip entry? */
    }   /* for num_entries loop */
}


/*
 * Copy all tgt entries from fd to nfd
 */
PRIVATE void lrgy_copy_tgt_entries
#ifndef __STDC__
    (fd, nfd, num_entries, status)
    int             fd;
    int             nfd;
    int             num_entries;
    error_status_t  *status;
#else
  (
    int             fd,
    int             nfd,
    int             num_entries,
    error_status_t  *status
  )
#endif
{
    int                     i, nbin, nbout;
    off_t                   pos;
    char                    *varying;
    sec_lrgy_tgt_fixed_t    fixed;

    /* Make sure we're starting after the header, before the entries */
    pos = lseek(fd, sizeof(int), L_SET);

    for (i = 0;(GOOD_STATUS(status)) && (i < num_entries); i++) {

        nbin = read(fd, (char *) &fixed, sizeof(sec_lrgy_tgt_fixed_t));
        if (nbin != sizeof(sec_lrgy_tgt_fixed_t)) {
            SET_STATUS(status, sec_lrgy_s_internal_error);
        } else {
            nbout = write(nfd, (char *) &fixed, sizeof(sec_lrgy_tgt_fixed_t));
            if (nbout != sizeof(sec_lrgy_tgt_fixed_t)) {
                SET_STATUS(status, sec_lrgy_s_internal_error);
            } else {
                varying = (char *) malloc(fixed.var_len);
                if (!varying) {
                    SET_STATUS(status, sec_s_no_memory);
                } else {
                    nbin = read(fd, (char *) varying, fixed.var_len);
                    if (nbin != fixed.var_len) {
                        SET_STATUS(status, sec_lrgy_s_internal_error);
                    } else {
                        nbout = write(nfd, (char *) varying, fixed.var_len);
                        if (nbout != fixed.var_len) {
                            SET_STATUS(status, sec_lrgy_s_internal_error);
                        } /* write varying */
                    } /* read varying */
                    free(varying);
                } /* allocate varying */
            } /* write entry */
        } /* read entry */

    }   /* for num_entries loop */

        /* If all went well, update num_entries */
    if (GOOD_STATUS(status)) {
        num_entries++;

        pos = lseek(nfd, 0, L_SET);
        if (pos == 0) {
            nbout = write(nfd, (char *) &num_entries, sizeof(int));
            if (nbout != sizeof(int)) {
                SET_STATUS(status, sec_lrgy_s_internal_error);
            }
        } else {
            SET_STATUS(status, sec_lrgy_s_internal_error);
        }
    }
}


/*
 * Copy all tgt entries from fd to nfd, except the one at offset 'epos'.
 */
PRIVATE void lrgy_copy_tgt_entries_except
#ifndef __STDC__
    (fd, nfd, reset_nfd, num_entries, epos, status)
    int             fd;
    int             nfd;
    boolean32       reset_nfd;
    int             num_entries;
    off_t           epos;
    error_status_t  *status;
#else
  (
    int             fd,
    int             nfd,
    boolean32       reset_nfd,
    int             num_entries,
    off_t           epos,
    error_status_t  *status
  )
#endif
{
    int                     i, nbin, nbout;
    off_t                   pos;
    char                    *varying;
    sec_lrgy_tgt_fixed_t    fixed;

    /* Make sure we're starting after the header, before the entries */
    pos = lseek(fd, sizeof(int), L_SET);

    /* Only start at beginning of new file if caller didn't
     * already start writing to it.
     */
    if (reset_nfd) {
        pos = lseek(nfd, sizeof(int), L_SET);
    }

    for (i = 0;(GOOD_STATUS(status)) && (i < num_entries); i++) {

            /* Do we skip this one? */
        if (lseek(fd, 0, L_INCR) == epos) {

            nbin = read(fd, (char *) &fixed, sizeof(sec_lrgy_tgt_fixed_t));
            if (nbin != sizeof(sec_lrgy_tgt_fixed_t)) {
                SET_STATUS(status, sec_lrgy_s_internal_error);
            } else {    /* Skip over the varying part of the record */
                lseek(fd, fixed.var_len, L_INCR);
            }

        } else {  /* Didn't skip over, so copy it */

            nbin = read(fd, (char *) &fixed, sizeof(sec_lrgy_tgt_fixed_t));
            if (nbin != sizeof(sec_lrgy_tgt_fixed_t)) {
                SET_STATUS(status, sec_lrgy_s_internal_error);
            } else {
                nbout = write(nfd, (char *) &fixed, sizeof(sec_lrgy_tgt_fixed_t));
                if (nbout != sizeof(sec_lrgy_tgt_fixed_t)) {
                    SET_STATUS(status, sec_lrgy_s_internal_error);
                } else {
                    varying = (char *) malloc(fixed.var_len);
                    if (!varying) {
                        SET_STATUS(status, sec_s_no_memory);
                    } else {
                        nbin = read(fd, (char *) varying, fixed.var_len);
                        if (nbin != fixed.var_len) {
                            SET_STATUS(status, sec_lrgy_s_internal_error);
                        } else {
                            nbout = write(nfd, (char *) varying, fixed.var_len);
                            if (nbout != fixed.var_len) {
                                SET_STATUS(status, sec_lrgy_s_internal_error);
                            } /* write varying */
                        } /* read varying */
                        free(varying);
                    } /* allocate varying */
                } /* write entry */
            } /* read entry */
        } /* skip entry? */
    }   /* for num_entries loop */
}


/*
 * Copy all entries from fd to nfd, except expired entries.
 */
PRIVATE void lrgy_copy_nonexpired_entries
#ifndef __STDC__
    (fd, nfd, properties, num_purged, status)
    int             fd;
    int             nfd;
    sec_lrgy_hdr_t  *properties;
    unsigned32      *num_purged;
    error_status_t  *status;
#else
  (
    int             fd,
    int             nfd,
    sec_lrgy_hdr_t  *properties,
    unsigned32      *num_purged,
    error_status_t  *status
  )
#endif
{
    int                     i, nbin, nbout;
    off_t                   pos, pos2;
    char                    *varying;
    sec_lrgy_fixed_data_t   fixed;
    struct timeval          tm;
    sec_timeval_sec_t       expired;

    /* Make sure we're starting after the header, before the entries */
    pos  = lseek(fd,  sizeof(sec_lrgy_hdr_t), L_SET);
    pos2 = lseek(nfd, sizeof(sec_lrgy_hdr_t), L_SET);

    /* All expired entries were created before (now - lifespan) seconds ago */
    if (properties->lifespan) {
#ifdef SNI_SVR4_POSIX
        gettimeofday(&tm);
#else
        gettimeofday(&tm, NULL);
#endif /* SNI_SVR4_POSIX */
        expired = tm.tv_sec - properties->lifespan;
    } else {    /* Never expire */
        expired = 0;
    }

    for (i = 0;(GOOD_STATUS(status)) && (i < properties->num_entries); i++) {

        nbin = read(fd, (char *) &fixed, sizeof(sec_lrgy_fixed_data_t));
        if (nbin != sizeof(sec_lrgy_fixed_data_t)) {
            SET_STATUS(status, sec_lrgy_s_internal_error);
        } else {

            /* Don't copy if expired */
            if ((expired) && (fixed.timestamp < expired)) {
                lseek(fd, fixed.var_len, L_INCR);
                *num_purged+= 1;
            } else {                                /* Not expired, copy it */
                nbout = write(nfd, (char *) &fixed, sizeof(sec_lrgy_fixed_data_t));
                if (nbout != sizeof(sec_lrgy_fixed_data_t)) {
                    SET_STATUS(status, sec_lrgy_s_internal_error);
                } else {
                    varying = (char *) malloc(fixed.var_len);
                    if (!varying) {
                        SET_STATUS(status, sec_s_no_memory);
                    } else {
                        nbin = read(fd, (char *) varying, fixed.var_len);
                        if (nbin != fixed.var_len) {
                            SET_STATUS(status, sec_lrgy_s_internal_error);
                        } else {
                            nbout = write(nfd, (char *) varying, fixed.var_len);
                            if (nbout != fixed.var_len) {
                                SET_STATUS(status, sec_lrgy_s_internal_error);
                            } /* write varying */
                        } /* read varying */
                        free(varying);
                    } /* allocate varying */
                } /* write entry */
            } /* if expired skip else write */
        } /* read entry */
    }   /* for num_entries loop */
}


/*
 * Copy all nonexpired tgt entries from fd to nfd.
 */
PRIVATE void lrgy_copy_nonexpired_tgt_entries
#ifndef __STDC__
    (fd, nfd, num_entries, properties, num_purged, status)
    int             fd;
    int             nfd;
    int             num_entries;
    sec_lrgy_hdr_t  *properties;
    unsigned32      *num_purged;
    error_status_t  *status;
#else
  (
    int             fd,
    int             nfd,
    int             num_entries,
    sec_lrgy_hdr_t  *properties,
    unsigned32      *num_purged,
    error_status_t  *status
  )
#endif
{
    int                     i, nbin, nbout;
    off_t                   pos, pos2;
    char                    *varying;
    sec_lrgy_tgt_fixed_t    fixed;
    struct timeval          tm;
    sec_timeval_sec_t       expired;

    /* Make sure we're starting after the header, before the entries */
    pos  = lseek(fd,  sizeof(int), L_SET);
    pos2 = lseek(nfd, sizeof(int), L_SET);

    /* All expired entries were created before (now - lifespan) seconds ago */
    if (properties->lifespan) {
#ifdef SNI_SVR4_POSIX
        gettimeofday(&tm);
#else
        gettimeofday(&tm, NULL);
#endif /* SNI_SVR4_POSIX */
        expired = tm.tv_sec - properties->lifespan;
    } else {    /* Never expire */
        expired = 0;
    }

    for (i = 0;(GOOD_STATUS(status)) && (i < num_entries); i++) {

        nbin = read(fd, (char *) &fixed, sizeof(sec_lrgy_tgt_fixed_t));
        if (nbin != sizeof(sec_lrgy_tgt_fixed_t)) {
            SET_STATUS(status, sec_lrgy_s_internal_error);
        } else {

            /* Don't copy if expired */
            if ((expired) && (fixed.timestamp < expired)) {
                lseek(fd, fixed.var_len, L_INCR);
                *num_purged+= 1;
            } else {                                /* Not expired, copy it */
                nbout = write(nfd, (char *) &fixed, sizeof(sec_lrgy_tgt_fixed_t));
                if (nbout != sizeof(sec_lrgy_tgt_fixed_t)) {
                    SET_STATUS(status, sec_lrgy_s_internal_error);
                } else {
                    varying = (char *) malloc(fixed.var_len);
                    if (!varying) {
                        SET_STATUS(status, sec_s_no_memory);
                    } else {
                        nbin = read(fd, (char *) varying, fixed.var_len);
                        if (nbin != fixed.var_len) {
                            SET_STATUS(status, sec_lrgy_s_internal_error);
                        } else {
                            nbout = write(nfd, (char *) varying, fixed.var_len);
                            if (nbout != fixed.var_len) {
                                SET_STATUS(status, sec_lrgy_s_internal_error);
                            } /* write varying */
                        } /* read varying */
                        free(varying);
                    } /* allocate varying */
                } /* write entry */
            } /* if expired skip else write */
        } /* read entry */
    }   /* for num_entries loop */
}


/*
 * PUBLIC routines
 */

/*
 * sec_lrgy_properties_get_info - returns local registry properties.
 */
void sec_lrgy_properties_get_info
#ifndef __STDC__
    (properties, status)
    sec_lrgy_hdr_t          *properties;
    error_status_t          *status;
#else
  (
    sec_lrgy_hdr_t          *properties,
    error_status_t          *status
  )
#endif
{
    int lfd, fd = -1;
    CLEAR_STATUS(status);

        /* Don't do anything if we can't lock the local registry for read */
    TRY {
        lrgy_read_lock(&lfd, status);
        if (!GOOD_STATUS(status)) {
            return;
        }

        lrgy_setup(0, 0, properties, false, status);
        lrgy_unlock(&lfd);
    } CATCH_ALL {
        lrgy_unlock(&lfd);
        RERAISE;
    } ENDTRY;
}


/*
 * sec_lrgy_properties_tgt_get_info - returns local rgy tgt properties.
 */
void sec_lrgy_properties_tgt_get_info
#ifndef __STDC__
    (num_tgt_entries, status)
    unsigned32              *num_tgt_entries;
    error_status_t          *status;
#else
  (
    unsigned32              *num_tgt_entries,
    error_status_t          *status
  )
#endif
{
    int         lfd;
    CLEAR_STATUS(status);

        /* Don't do anything if we can't lock the local registry for read */
    TRY {
        lrgy_read_lock(&lfd, status);
        if (!GOOD_STATUS(status)) {
            return;
        }
        lrgy_setup_tgt(0, 0, (int *)num_tgt_entries, status);
        lrgy_unlock(&lfd);
    } CATCH_ALL {
        lrgy_unlock(&lfd);
        RERAISE;
    } ENDTRY;
}


/*
 * sec_lrgy_properties_set_info - sets local registry properties.
 */
void sec_lrgy_properties_set_info
#ifndef __STDC__
    (properties, status)
    sec_lrgy_hdr_t          *properties;
    error_status_t          *status;
#else
  (
    sec_lrgy_hdr_t          *properties,
    error_status_t          *status
  )
#endif
{
    int nbytes, pos, lfd, fd = -1;
    CLEAR_STATUS(status);

        /* Can't handle purging oldest entries yet */
    if (properties->max_entries < properties->num_entries) {
        SET_STATUS(status, sec_lrgy_s_max_lt_num_entries);
        return;
    }

        /* Don't do anything if we can't lock the local registry for write */
    TRY {
        lrgy_write_lock(&lfd, status);
        if (!GOOD_STATUS(status)) {
            return;
        }

        lrgy_setup(&fd, NULL, NULL, true, status);
        if (GOOD_STATUS(status) && (fd > 0)) {
            pos = lseek(fd, 0, L_SET);
            if (pos == 0) {
                nbytes = write(fd, (char *) properties, sizeof(sec_lrgy_hdr_t));
                if (nbytes != sizeof(sec_lrgy_hdr_t)) {
                    SET_STATUS(status, sec_lrgy_s_internal_error);
                }
            } else {
                SET_STATUS(status, sec_lrgy_s_internal_error);
            }
        }
        if (fd > 0) {
            close(fd);
        }
        lrgy_unlock(&lfd);
    } CATCH_ALL {
        lrgy_unlock(&lfd);
        RERAISE;
    } ENDTRY;
}


/*
 * sec_lrgy_entry_store - Add/Replace Unix and login context data.
 *
 *      Open lrgy_data, retrieve header/property info
 *      Open lrgy_data.new, write header
 *      Allocate/Populate Varying Portion
 *      Search lrgy_data for matching entry; find oldest entry
 *      Write new entry to new file.
 *      Copy old entries to new file using the following criteria:
 *          no_match/<max_entries: copy all lrgy_data entries
 *          no_match/=max_entries: copy all lrgy_data entries except oldest
 *          match: copy all lrgy_data entries except matching one.
 *      fsync, then close new file.
 *      If status_ok, rename old file lrgy_data.bak, rename new file lrgy_data
 */
void sec_lrgy_entry_store
#ifndef __STDC__
    (entry, status)
    sec_lrgy_entry_t        *entry;
    error_status_t          *status;
#else
  (
    sec_lrgy_entry_t        *entry,
    error_status_t          *status
  )
#endif
{
    int                     lfd, fd = -1, nfd = -1;
    int                     nbytes;
    off_t                   mpos, opos, pos;    /* File offset (match,oldest)*/
    sec_lrgy_hdr_t          properties;
    char                    *var_ptr = NULL, *name;
    unsigned_char_p_t       global_name = NULL;
    volatile boolean32               match = false;
    sec_lrgy_fixed_data_t   fixed;
    CLEAR_STATUS(status);

        /* Don't do anything if we can't lock the local registry for write */
    TRY {
        lrgy_write_lock(&lfd, status);
        if (!GOOD_STATUS(status)) {
            return;
        }

        /* Open the lrgy file and retrieve the header/property info */
        lrgy_setup(&fd, &nfd, &properties, true, status);
        if (GOOD_STATUS(status) && (fd > 0)) {

            /* allocate and populate varying portion */
            lrgy_pack_data(entry, &fixed, &var_ptr, status);

            /* Make global name is in canonical form */
            name = (char *)malloc( u_strlen(entry->cell) +
                u_strlen(entry->principal) + 2);
            if (!name) {
                SET_STATUS(status, sec_s_no_memory);
            } else {
                u_strcpy(name, entry->cell);
                u_strcat(name, "/");
                u_strcat(name, entry->principal);
                *status = sec_id_globalize_name((unsigned_char_p_t) name,
                    &global_name);
                free(name);
            }

            /* Search for matching entry */
            if (GOOD_STATUS(status)) {
                match = lrgy_search(fd, &mpos, &opos, global_name,
                    properties.num_entries, 0, status);

                /* Write new entry to new file */
                nbytes = write(nfd,(char *)&fixed,sizeof(sec_lrgy_fixed_data_t));
                if (nbytes != sizeof(sec_lrgy_fixed_data_t)) {
                    SET_STATUS(status, sec_lrgy_s_internal_error);
                } else {
                    nbytes = write(nfd, (char *) var_ptr, fixed.var_len);
                    if (nbytes != fixed.var_len) {
                	SET_STATUS(status, sec_lrgy_s_internal_error);
                    }
                }
            }

            /* Copy rest of entries using criteria described above */
            if (GOOD_STATUS(status)) {
                if (match) {
                    /* copy all lrgy_data entries except matching one */
                    lrgy_copy_entries_except(fd, nfd, false, &properties, mpos,
                        status);

                } else {        /* No matching entry */
                    if (properties.num_entries < properties.max_entries) {
                        /* < max_entries: copy all lrgy_data entries */
                        lrgy_copy_entries(fd, nfd, &properties, status);
                    } else {
                        /* max_entries: copy all data entries except oldest */
                        lrgy_copy_entries_except(fd, nfd, false, &properties,
                            opos, status);
                    }
                }
            }
        }
        /* Clean up */
        if (fd != -1) {
            close(fd);
        }
        if (nfd != -1) {
            fsync(nfd);
            close(nfd);
            if (GOOD_STATUS(status)) {
                unlink(LRGY_FILE_BAK);
                rename(LRGY_FILE, LRGY_FILE_BAK);
                rename(LRGY_NEW_FILE, LRGY_FILE);
            }
        }
        lrgy_unlock(&lfd);
    } CATCH_ALL {
        lrgy_unlock(&lfd);
        RERAISE;
    } ENDTRY;
    if (global_name) {
        free(global_name);
    }
    if (var_ptr) {
        free(var_ptr);
    }
}


/*
 * sec_lrgy_entry_lookup - Attempt to lookup an entry by globalname.
 *
 *  Caller must free varying entries after processing.
 */
void sec_lrgy_entry_lookup
#ifndef __STDC__
    (name, entry, status)
    unsigned_char_p_t       name;
    sec_lrgy_entry_t        *entry;
    error_status_t          *status;
#else
  (
    unsigned_char_p_t       name,
    sec_lrgy_entry_t        *entry,
    error_status_t          *status
  )
#endif
{
    int                     lfd, fd = -1;
    boolean32               match;
    sec_lrgy_hdr_t          properties;
    unsigned_char_p_t       global_name;
    CLEAR_STATUS(status);

        /* Don't do anything if we can't lock the local registry for read */
    TRY {
        lrgy_read_lock(&lfd, status);
        if (!GOOD_STATUS(status)) {
            return;
        }

	/* Be paranoid and globalize name just in case caller neglected to */
	*status = sec_id_globalize_name(name, &global_name);
	if (GOOD_STATUS(status)) {

            /* Open the lrgy file and retrieve the header/property info */
	    lrgy_setup(&fd, 0, &properties, false, status);
	    if (GOOD_STATUS(status) && (fd > 0)) {
		match = lrgy_search(fd, NULL, NULL, global_name,
		    properties.num_entries, entry, status);
		if (!match)
		    SET_STATUS(status, sec_lrgy_s_no_matching_entry);
	    }
	    if (fd != -1)
		close(fd);
	    free((char *)global_name);
	}

	lrgy_unlock(&lfd);
    } CATCH_ALL {
        lrgy_unlock(&lfd);
        RERAISE;
    } ENDTRY;
}


/*
 * sec_lrgy_entry_delete - Attempt to delete an entry by globalname.
 */
void sec_lrgy_entry_delete
#ifndef __STDC__
    (name, status)
    unsigned_char_p_t       name;
    error_status_t          *status;
#else
  (
    unsigned_char_p_t       name,
    error_status_t          *status
  )
#endif
{
    int                     nbout, lfd, fd = -1, nfd = -1;
    off_t                   pos, mpos;
    sec_lrgy_hdr_t          properties;
    unsigned_char_p_t       global_name = NULL;
    volatile boolean32               match = false;
    CLEAR_STATUS(status);

        /* Don't do anything if we can't lock the local registry for write */
    TRY {
	lrgy_write_lock(&lfd, status);
	if (!GOOD_STATUS(status)) {
	    return;
	}

        /* Open the lrgy file and retrieve the header/property info */
	lrgy_setup(&fd, &nfd, &properties, false, status);
	if (GOOD_STATUS(status) && (fd > 0)) {
            /* Make sure global name is in canonical form */
	    *status = sec_id_globalize_name(name, &global_name);

            /* Search for matching entry */
	    if (GOOD_STATUS(status)) {
		match = lrgy_search(fd, &mpos, NULL, global_name,
		    properties.num_entries, NULL, status);

                /* If match found, copy all but that entry */
		if (match && GOOD_STATUS(status)) {
		    lrgy_copy_entries_except(fd, nfd, true, &properties, mpos,
			status);

		    /* If all went well, there's 1 less entry in the new file */
		    if (GOOD_STATUS(status)) {
			properties.num_entries--;
			pos = lseek(nfd, 0, L_SET);
			if (pos == 0) {
			    nbout = write(nfd, (char *) &properties,
				sizeof(sec_lrgy_hdr_t));
			    if (nbout != sizeof(sec_lrgy_hdr_t)) {
				SET_STATUS(status, sec_lrgy_s_internal_error);
			    }
			} else {
			    SET_STATUS(status, sec_lrgy_s_internal_error);
			}
		    }
		}
	    }
	}
	if (GOOD_STATUS(status) && !match) {
	    SET_STATUS(status, sec_lrgy_s_no_matching_entry);
	}

        /* Clean up */
	if (fd != -1) {
	    close(fd);
	}
	if (nfd != -1) {
	    fsync(nfd);
	    close(nfd);
	    if (GOOD_STATUS(status)) {
		unlink(LRGY_FILE_BAK);
		rename(LRGY_FILE, LRGY_FILE_BAK);
		rename(LRGY_NEW_FILE, LRGY_FILE);
	    }
	}
	lrgy_unlock(&lfd);
    } CATCH_ALL {
        lrgy_unlock(&lfd);
        RERAISE;
    } ENDTRY;
    if (global_name) {
        free(global_name);
    }
}


/*
 * sec_lrgy_tgt_store - Add/Replace tgt data.
 *      Open lrgy_data, retrieve header/property info
 *      Open lrgy_tgts, retrieve num_entries
 *      Open lrgy_tgts.new, write header
 *      Allocate/Populate Varying Portion
 *      Search lrgy_tgts for matching entry; find oldest entry
 *      Write new entry to new file.
 *      Copy old entries to new file using the following criteria:
 *          no_match/<max_entries: copy all lrgy_tgts
 *          no_match/=max_entries: copy all lrgy_tgts except oldest
 *          match: copy all lrgy_tgts except matching one.
 *      fsync, then close new file.
 *      If status_ok, rename old file lrgy_tgts.bak, rename new file lrgy_tgts
 */
void sec_lrgy_tgt_store
#ifndef __STDC__
    (entry, status)
    sec_lrgy_tgt_entry_t    *entry;
    error_status_t          *status;
#else
  (
    sec_lrgy_tgt_entry_t    *entry,
    error_status_t          *status
  )
#endif
{
    int                     lfd, fd = -1, nfd = -1;
    int                     nbytes, num_entries;
    off_t                   mpos, opos, pos;    /* File offset (match,oldest)*/
    sec_lrgy_hdr_t          properties;
    char                    *var_ptr = NULL, *name;
    unsigned_char_p_t       global_name = NULL;
    volatile boolean32               match = false;
    sec_lrgy_tgt_fixed_t    fixed;
    CLEAR_STATUS(status);

        /* Don't do anything if we can't lock the local registry for write */
    TRY {
	lrgy_write_lock(&lfd, status);
	if (!GOOD_STATUS(status)) {
	    return;
	}

        /* Open the lrgy file and retrieve the header/property info */
	lrgy_setup(NULL, NULL, &properties, true, status);
	if (GOOD_STATUS(status))
	    lrgy_setup_tgt(&fd, &nfd, &num_entries, status);

	if (GOOD_STATUS(status) && (fd > 0)) {
            /* allocate and populate varying portion */
	    lrgy_pack_tgt_data(entry, &fixed, &var_ptr, status);

            /* Make global name is in canonical form */
	    name = (char *)malloc( u_strlen(entry->cell) +
		u_strlen(entry->principal) + 2);
	    if (!name) {
		SET_STATUS(status, sec_s_no_memory);
	    } else {
		u_strcpy(name, entry->cell);
		u_strcat(name, "/");
		u_strcat(name, entry->principal);
		*status = sec_id_globalize_name((unsigned_char_p_t) name,
		    &global_name);
		free(name);
	    }

            /* Search for matching entry */
	    if (GOOD_STATUS(status)) {
		match = lrgy_tgt_search(fd, &mpos, &opos, global_name,
		    num_entries, NULL, status);

                /* Write new entry to new file */
		nbytes = write(nfd, (char *)&fixed, sizeof(sec_lrgy_tgt_fixed_t));
		if (nbytes != sizeof(sec_lrgy_tgt_fixed_t)) {
		    SET_STATUS(status, sec_lrgy_s_internal_error);
		} else {
		    nbytes = write(nfd, (char *) var_ptr, fixed.var_len);
		    if (nbytes != fixed.var_len) {
			SET_STATUS(status, sec_lrgy_s_internal_error);
		    }
		}
	    }

            /* Copy rest of entries using criteria described above */
	    if (GOOD_STATUS(status)) {
		if (match) {
		    /* copy all lrgy_data entries except matching one */
		    lrgy_copy_tgt_entries_except(fd, nfd, false, num_entries,
			mpos, status);

		} else {	/* No matching entry */
		    if (num_entries < properties.max_entries) {
			/* < max_entries: copy all lrgy_data entries */
			lrgy_copy_tgt_entries(fd, nfd, num_entries, status);
		    } else {
			/* max_entries: copy all data entries except oldest*/
			lrgy_copy_tgt_entries_except(fd, nfd, false, num_entries,
			    opos, status);
		    }
		}
	    }
	}
        /* Clean up */
	if (fd != -1) {
	    close(fd);
	}
	if (nfd != -1) {
	    fsync(nfd);
	    close(nfd);
	    if (GOOD_STATUS(status)) {
		unlink(LRGY_TGT_FILE_BAK);
		rename(LRGY_TGT_FILE, LRGY_TGT_FILE_BAK);
		rename(LRGY_NEW_TGT_FILE, LRGY_TGT_FILE);
	    }
	}
	lrgy_unlock(&lfd);
    } CATCH_ALL {
        lrgy_unlock(&lfd);
        RERAISE;
    } ENDTRY;
    if (global_name) {
        free(global_name);
    }
    if (var_ptr) {
        free(var_ptr);
    }
}


/*
 * sec_lrgy_tgt_lookup - Attempt to lookup a tgt by globalname.
 *
 *  Caller must free varying entries after processing.
 */
void sec_lrgy_tgt_lookup
#ifndef __STDC__
    (global_name, entry, status)
    unsigned_char_p_t       global_name;
    sec_lrgy_tgt_entry_t    *entry;
    error_status_t          *status;
#else
  (
    unsigned_char_p_t       global_name,
    sec_lrgy_tgt_entry_t    *entry,
    error_status_t          *status
  )
#endif
{
    int                     lfd, fd = -1;
    boolean32               match;
    int                     num_entries;
    unsigned_char_p_t       cgname;
    CLEAR_STATUS(status);

        /* Don't do anything if we can't lock the local registry for read */
    TRY {
	lrgy_read_lock(&lfd, status);
	if (!GOOD_STATUS(status)) {
	    return;
	}

        /* Canonicalize the name */
	sec_id_globalize_name(global_name, &cgname);

        /* Open the lrgy file and retrieve the header/property info */
	lrgy_setup_tgt(&fd, NULL, &num_entries, status);
	if (GOOD_STATUS(status) && (fd > 0)) {
	    match = lrgy_tgt_search(fd, NULL, NULL, cgname,
		num_entries, entry, status);
	    if (!match)
		SET_STATUS(status, sec_lrgy_s_no_matching_entry);
	}
	if (fd != -1)
	    close(fd);
	u_free(cgname);

	lrgy_unlock(&lfd);
    } CATCH_ALL {
        lrgy_unlock(&lfd);
        RERAISE;
    } ENDTRY;
}


/*
 * sec_lrgy_tgt_delete - Attempt to delete a tgt by globalname.
 */
void sec_lrgy_tgt_delete
#ifndef __STDC__
    (name, status)
    unsigned_char_p_t       name;
    error_status_t          *status;
#else
  (
    unsigned_char_p_t       name,
    error_status_t          *status
  )
#endif
{
    int                     lfd, fd = -1, nfd = -1;
    int                     nbout, num_entries;
    off_t                   pos, mpos;
    sec_lrgy_hdr_t          properties;
    unsigned_char_p_t       global_name = NULL;
    volatile boolean32               match = false;
    CLEAR_STATUS(status);

        /* Don't do anything if we can't lock the local registry for write */
    TRY {
	lrgy_write_lock(&lfd, status);
	if (!GOOD_STATUS(status)) {
	    return;
	}

        /* Open the lrgy file and retrieve the header/property info */
	lrgy_setup(NULL, NULL, &properties, true, status);
	if (GOOD_STATUS(status))
	    lrgy_setup_tgt(&fd, &nfd, &num_entries, status);

	if (GOOD_STATUS(status) && (fd > 0)) {
            /* Make sure global name is in canonical form */
	    *status = sec_id_globalize_name(name, &global_name);

            /* Search for matching entry */
	    if (GOOD_STATUS(status)) {
		match = lrgy_tgt_search(fd, &mpos, NULL, global_name,
		    num_entries, NULL, status);

                /* If match found, copy all but that entry */
		if (GOOD_STATUS(status)) {
		    lrgy_copy_tgt_entries_except(fd, nfd, true, num_entries,
			mpos, status);
		    /* If all went well, there's 1 less entry in the new file */
		    if (GOOD_STATUS(status)) {
			num_entries--;
			pos = lseek(nfd, 0, L_SET);
			if (pos == 0) {
			    nbout = write(nfd,(char *)&num_entries,sizeof(int));
			    if (nbout != sizeof(int)) {
				SET_STATUS(status, sec_lrgy_s_internal_error);
			    }
			} else {
			    SET_STATUS(status, sec_lrgy_s_internal_error);
			}
		    }
		}
	    }
	}
	if (GOOD_STATUS(status) && !match) {
	    SET_STATUS(status, sec_lrgy_s_no_matching_tgt);
	}
        /* Clean up */
	if (fd != -1) {
	    close(fd);
	}
	if (nfd != -1) {
	    fsync(nfd);
	    close(nfd);
	    if (GOOD_STATUS(status)) {
		unlink(LRGY_TGT_FILE_BAK);
		rename(LRGY_TGT_FILE, LRGY_TGT_FILE_BAK);
		rename(LRGY_NEW_TGT_FILE, LRGY_TGT_FILE);
	    }
	}
	lrgy_unlock(&lfd);
    } CATCH_ALL {
        lrgy_unlock(&lfd);
        RERAISE;
    } ENDTRY;
    if (global_name) {
        free(global_name);
    }
}


PUBLIC void sec_lrgy_purge_expired
#ifndef __STDC__
    (num_purged, num_tgts_purged, status)
    unsigned32              *num_purged;        /* [out] */
    unsigned32              *num_tgts_purged;   /* [out] */
    error_status_t          *status;            /* [out] */
#else
  (
    unsigned32              *num_purged,        /* [out] */
    unsigned32              *num_tgts_purged,   /* [out] */
    error_status_t          *status             /* [out] */
  )
#endif
{
    int                     fd = -1;
    int                     nfd = -1;
    int                     lfd, nbytes, num_entries;
    off_t                   pos;
    sec_lrgy_hdr_t          hdr;
    CLEAR_STATUS(status);
    *num_purged = 0;
    *num_tgts_purged = 0;

        /* Don't do anything if we can't lock the local registry for write */
    TRY {
	lrgy_write_lock(&lfd, status);
	if (!GOOD_STATUS(status)) {
	    return;
	}

        /* Open the lrgy file and retrieve the header/property info */
	lrgy_setup(&fd, &nfd, &hdr, false, status);

        /* Entries only expire if lifespan is nonzero */
	if (GOOD_STATUS(status) && hdr.lifespan) {
	    lrgy_copy_nonexpired_entries(fd, nfd, &hdr, num_purged, status);
	    if (fd > 0) {
		fsync(fd);
		close(fd);
	    }
	    if (nfd > 0) {
		pos = lseek(nfd, 0, L_SET);
		hdr.num_entries -= *num_purged;
		nbytes = write(nfd, (char *)&hdr, sizeof(sec_lrgy_hdr_t));
		fsync(nfd);
		close(nfd);
		if (GOOD_STATUS(status)) {
		    unlink(LRGY_FILE_BAK);
		    rename(LRGY_FILE, LRGY_FILE_BAK);
		    rename(LRGY_NEW_FILE, LRGY_FILE);
		}
	    }
            /* Now the tgt entries */
	    if (GOOD_STATUS(status)) {
		lrgy_setup_tgt(&fd, &nfd, &num_entries, status);
		if (GOOD_STATUS(status)) {
		    lrgy_copy_nonexpired_tgt_entries(fd, nfd, num_entries, &hdr,
			num_tgts_purged, status);
		    if (fd > 0) {
			fsync(fd);
			close(fd);
		    }
		    if (nfd > 0) {
			pos = lseek(nfd, 0, L_SET);
			num_entries -= *num_tgts_purged;
			nbytes = write(nfd, (char *) &num_entries, sizeof(int));
			fsync(nfd);
			close(nfd);
			if (GOOD_STATUS(status)) {
			    unlink(LRGY_TGT_FILE_BAK);
			    rename(LRGY_TGT_FILE, LRGY_TGT_FILE_BAK);
			    rename(LRGY_NEW_TGT_FILE, LRGY_TGT_FILE);
			}
		    }
		}
	    }
	}
        /* Allow access to the lrgy again */
	lrgy_unlock(&lfd);
    } CATCH_ALL {
        lrgy_unlock(&lfd);
        RERAISE;
    } ENDTRY;
}


/*
 * sec_lrgy_entry_cursor_init
 */
void sec_lrgy_entry_cursor_init
#ifndef __STDC__
    (cursorP, status)
    void                    **cursorP;      /* [out] */
    error_status_t          *status;        /* [out] */
#else
  (
    void                    **cursorP,      /* [out] */
    error_status_t          *status         /* [out] */
  )
#endif
{
    struct stat buffer;
    CLEAR_STATUS(status);

    if (stat(LRGY_FILE, &buffer) == 0) {
        *cursorP = (void *)malloc(sizeof(sec_lrgy_pvt_cursor_t));
        if (!*cursorP) {
            SET_STATUS(status, sec_s_no_memory);
        } else {
            LAST_UPDATE_TIME(*cursorP) = buffer.st_mtime;
            ENTRY_INDEX(*cursorP) = 0;  /* Next entry we'll try to read */
        }
    } else {
        SET_STATUS(status, sec_lrgy_s_not_found);
    }
}


/*
 * sec_lrgy_entry_get_next
 *
 *  Caller must free varying entries after processing.
 */
void sec_lrgy_entry_get_next
#ifndef __STDC__
    (cursor, entryP, timestamp, status)
    void                    *cursor;        /* [in]  */
    sec_lrgy_entry_t        *entryP;        /* [out] */
    sec_timeval_sec_t       *timestamp;     /* [out] */
    error_status_t          *status;        /* [out] */
#else
  (
    void                    *cursor,        /* [in]  */
    sec_lrgy_entry_t        *entryP,        /* [out] */
    sec_timeval_sec_t       *timestamp,     /* [out] */
    error_status_t          *status         /* [out] */
  )
#endif
{
    int                     i, nbytes, lfd, fd = -1;
    sec_lrgy_hdr_t          properties;
    sec_lrgy_fixed_data_t   fixed;
    char                    *varying;
    CLEAR_STATUS(status);

        /* Don't do anything if we can't lock the local registry for read */
    TRY {
	lrgy_read_lock(&lfd, status);
	if (!GOOD_STATUS(status)) {
	    return;
	}

        /* Open the lrgy file and retrieve the header/property info */
	lrgy_setup(&fd, 0, &properties, false, status);
	if (GOOD_STATUS(status)) {
	    if (ENTRY_INDEX(cursor) <= properties.num_entries) {
                /* Seek forward to the desired entry */
		lseek(fd, sizeof(sec_lrgy_hdr_t), L_SET);
		for (i = 0; i < ENTRY_INDEX(cursor); i++) {
		    nbytes = read(fd, (char *) &fixed,
			sizeof(sec_lrgy_fixed_data_t));
		    if (nbytes != sizeof(sec_lrgy_fixed_data_t)) {
			SET_STATUS(status, sec_lrgy_s_no_more_entries);
		    } else {
			lseek(fd, fixed.var_len, L_INCR);
		    }
		}
		ENTRY_INDEX(cursor)++;

                /* Now read the desired entry */
		nbytes = read(fd, (char *) &fixed,
		    sizeof(sec_lrgy_fixed_data_t));
		if (nbytes == sizeof(sec_lrgy_fixed_data_t)) {
		    varying = (char *)malloc(fixed.var_len);
		    if (varying) {
			nbytes = read(fd, (char *) varying, fixed.var_len);
			if (nbytes != fixed.var_len) {
			    SET_STATUS(status, sec_lrgy_s_internal_error);
			} else {
			    lrgy_unpack_data(entryP, &fixed, varying, status);
			    if (timestamp) {
				*timestamp = fixed.timestamp;
			    }
			}
			free(varying);
		    } else {
			SET_STATUS(status, sec_s_no_memory);
		    }
		} else {
		    SET_STATUS(status, sec_lrgy_s_no_more_entries);
		}
	    } else {
		SET_STATUS(status, sec_lrgy_s_no_more_entries);
	    }
	}
	if (fd != -1) {
	    close(fd);
	}
	lrgy_unlock(&lfd);
    } CATCH_ALL {
        lrgy_unlock(&lfd);
        RERAISE;
    } ENDTRY;
}


/*
 * sec_lrgy_entry_cursor_release
 */
void sec_lrgy_entry_cursor_release
#ifndef __STDC__
    (cursorP, modifiedP, status)
    void                    **cursorP;      /* [in]  */
    boolean32               *modifiedP;     /* [out] */
    error_status_t          *status;        /* [out] */
#else
  (
    void                    **cursorP,      /* [in]  */
    boolean32               *modifiedP,     /* [out] */
    error_status_t          *status         /* [out] */
  )
#endif
{
    struct stat buffer;
    CLEAR_STATUS(status);
    *modifiedP = false;

    if (stat(LRGY_FILE, &buffer) == 0) {
            /* If the local registry account data has been modified during
             * this cursor's lifetime, let the caller know...
             */
        if (difftime(LAST_UPDATE_TIME(*cursorP), buffer.st_mtime) < 0.0) {
            *modifiedP = true;
        }
    } else {
        SET_STATUS(status, sec_lrgy_s_not_found);
    }
    if (*cursorP) {
        free(*cursorP);
        *cursorP = NULL;
    }
}

/*
 * sec_lrgy_krbtgt_lookup
 *
 * Similar to sec_lrgy_tgt_lookup, but instead takes a krb5
 * principal as input and only returns the encoded as_reply.
 */
void sec_lrgy_krbtgt_lookup (
    krb5_principal          client,	    /* [in]  */
    krb5_data		    *reply,	    /* [out] */
    error_status_t	    *status	    /* [out] */
)
{
    sec_lrgy_tgt_entry_t lrgy_entry;
    sec_rgy_name_t global_name;
    char *cell = NULL, *princ = NULL;

    CLEAR_STATUS(status);

    /* initialize */
    memset(&lrgy_entry, 0, sizeof(lrgy_entry));

    sec_dce_pair_from_krb_princ(client, &cell, &princ, status);
    if (BAD_STATUS(status))
	return;

    u_strcpy(global_name, cell);
    u_strcat(global_name, "/");
    u_strcat(global_name, princ);

    sec_lrgy_tgt_lookup(global_name, &lrgy_entry, status);
    if (BAD_STATUS(status))
        goto cleanup;

    /* copy TGT to reply and free everything else */
    reply->length = lrgy_entry.reply_len;
    reply->data = lrgy_entry.reply;
    if (lrgy_entry.cell) 
        free(lrgy_entry.cell);
    if (lrgy_entry.principal)
	free(lrgy_entry.principal);

cleanup:
    if (cell)
	free(cell);
    if (princ)
	free(princ);
    return;
}

/*
 * sec_lrgy_encode_kdc_rep
 *
 * This routine re-encrypts the TGT in the client's key
 * and allocates storage for the encoded representation
 * of the as_reply in the login context provided. This
 * will later be stored in the local registry if the
 * user certifies their identity.
 *
 * The sole purpose of this routine is to avoid storing
 * the TGT in the local rgy encrypted under a temporary
 * key used by preauthentication.
 */
void sec_lrgy_encode_kdc_rep (
    sec_login_context_t	    *lcp,	    /* [in,out]  */
    krb5_kdc_rep	    *as_reply,	    /* [in]  */
    sec_passwd_type_t	    keytype,	    /* [in]  */
    void const * volatile   keyseed,	    /* [in]  */
    error_status_t          *status         /* [out] */
)
{
    char *pepper = NULL;
    krb5_data salt;
    krb5_keyblock *encrypt_key = NULL;
    krb5_data *lrgy_tgt = NULL;

    CLEAR_STATUS(status);

    /* compute salt, if necessary */
    if (keytype == sec_passwd_plain) {
        pepper = sec_login_pvt_get_pepper(lcp, status);
        if (BAD_STATUS(status))
	    goto cleanup;
        salt.length = u_strlen(pepper);
        salt.data = (char *)pepper;

        *status = pwd_keyproc(NULL, as_reply->enc_part.etype,
                              &salt, keyseed, &encrypt_key);
        if (BAD_STATUS(status))
	    goto cleanup;
    } else
	krb5_copy_keyblock(keyseed, &encrypt_key);

    /*
     * krb5_encode_kdc_rep will malloc and store encrypted
     * kdc reply in as_reply->enc_part.ciphertext.data, so
     * free any data that exists before calling.
     */
    if (as_reply->enc_part.ciphertext.data)
        free(as_reply->enc_part.ciphertext.data);

    *status = krb5_encode_kdc_rep(KRB5_AS_REP, as_reply->enc_part2,
                                  encrypt_key, as_reply, &lrgy_tgt);
    if (BAD_STATUS(status))
	goto cleanup;

    /* free reply stored in login context */
    if (KRB_REQUEST_INFO(lcp)->reply.data)
        free(KRB_REQUEST_INFO(lcp)->reply.data);

    KRB_REQUEST_INFO(lcp)->reply.length = lrgy_tgt->length;
    KRB_REQUEST_INFO(lcp)->reply.data = (char *) malloc(lrgy_tgt->length);
    if (KRB_REQUEST_INFO(lcp)->reply.data == NULL)
       SET_STATUS(status, sec_login_s_no_memory);
    else
       memcpy(KRB_REQUEST_INFO(lcp)->reply.data, lrgy_tgt->data, lrgy_tgt->length);

cleanup:
    if (encrypt_key)
	krb5_free_keyblock(encrypt_key);
    if (pepper)
	free(pepper);
    if (lrgy_tgt)
        krb5_free_data(lrgy_tgt);
    return;
}
