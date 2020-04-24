/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dblock.c,v $
 * Revision 1.1.6.1  1996/05/10  13:06:41  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/04/19  20:42 UTC  arvind
 * 	Re-merge from /HPDCE2/3 to put back db_s_file_lock_fail
 *
 * 	HP revision /main/DCE_1.2/arvind_checkout/1  1996/04/19  20:41 UTC  arvind
 * 	Re-merge from /HPDCE02/3 to put back  db_s_file_lock_fail
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:34 UTC  psn
 * 	Submitted the fix for CHFts16512.
 * 	[1995/09/29  20:40 UTC  tatsu_s  /main/HPDCE02/5]
 *
 * 	Added db_c_lock_nowait.
 * 	[1995/09/23  00:16 UTC  tatsu_s  /main/HPDCE02/tatsu_s.dced_local.b0/1]
 *
 * 	Merge fix for CHFts16255
 * 	[1995/09/06  19:18 UTC  mullan_s  /main/HPDCE02/4]
 *
 * 	Remove db_s_file_lock_fail error.
 * 	[1995/09/01  19:23 UTC  mullan_s  /main/HPDCE02/mullan_bc_errtable_skew/1]
 *
 * 	Merge fix for CHFts15637
 * 	[1995/08/22  21:08 UTC  mullan_s  /main/HPDCE02/3]
 *
 * 	Use fcntl(,F_SETLK,) & pthread_delay_np() to avoid process blocking
 * 	when locking files.
 * 	[1995/08/10  18:21 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/1]
 *
 * 	Merge
 * 	[1995/05/22  17:04 UTC  mullan_s  /main/HPDCE02/2]
 *
 * 	Remove call to dce__db_lock_file from dce_db_lock & dce_db_unlock.
 * 	[1995/05/22  15:37 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat9/1]
 *
 * 	HPDCE02 branch
 * 	[1995/05/08  21:10 UTC  mullan_s  /main/HPDCE02/1]
 *
 * 	Add support for locking db files.
 * 	[1995/05/08  19:37 UTC  mullan_s  /main/mullan_mothra_bin_compat4/1]
 *
 * Revision 1.1.2.3  1994/09/16  20:22:18  bowe
 * 	Use app_mutex, not mutex. [CR 12231]
 * 	[1994/09/16  20:20:25  bowe]
 * 
 * Revision 1.1.2.2  1994/03/29  21:04:33  bowe
 * 	Added error checking for lock/unlock
 * 	[1994/03/29  21:03:17  bowe]
 * 
 * Revision 1.1.2.1  1994/01/24  22:01:43  bowe
 * 	Initial checkin
 * 	[1994/01/24  21:59:33  bowe]
 * 
 * Revision 1.1.2.1  1993/12/02  20:46:12  bowe
 * 	Initial revision.
 * 	[1993/12/02  20:44:50  bowe]
 * 
 * $EndLog$
 */

/* ______________________________________________________________________ 
 *
 *  Database / backing store locking routines
 *  External interfaces:
 *	dce_db_lock()
 *	dce_db_unlock()
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include <dce/uuid.h>
#include <dbimpl.h>

/*
 * dce_db_lock
 *
 * Acquires a mutex lock on the dce_db_handle passed in.
 *
 * NOTE! This only mutex locks the in-memory copy of the dce_db_handle!
 * It does not mutex lock the file from which this handle was read!
 * Subsequent calls to dce_db_open will create a new mutex!
 */
void
dce_db_lock(
    dce_db_handle_t	h,
    error_status_t	*st
)
{
    *st = error_status_ok;
    if (pthread_mutex_lock(&h->app_mutex) == -1) {
	if (errno == EDEADLK)
	    *st = db_s_already_locked;
	else
	    *st = db_s_mutex_lock_fail;
        return;
    }
}

/*
 *
 * dce_db_unlock
 *
 * Releases a mutex lock on the dce_db_handle passed in. See
 * comments in dce_db_lock for important notes.
 */
void
dce_db_unlock(
    dce_db_handle_t	h,
    error_status_t	*st
)
{
    *st = error_status_ok;
    if (pthread_mutex_unlock(&h->app_mutex) == -1) {
	if (errno == EINVAL)
	    *st = db_s_not_locked;
	else
	    *st = db_s_mutex_lock_fail;
    }
}

/*
 * dce__db_lock_file
 *
 * Locks the db file. If the file was opened readonly, a
 * shared lock is requested, otherwise an exclusive lock.
 * If the lock_flag parameter is false, the file is unlocked.
 */
void
dce__db_lock_file(
    dce_db_handle_t	h,
    boolean32		lock_flag,
    boolean32		nowait,
    error_status_t	*st
)
{
    struct flock	file_lock;
    struct timespec	wait_for_lock = {0,200};

    *st = error_status_ok;

    file_lock.l_whence = 0;
    file_lock.l_start = 0;
    file_lock.l_len = 0;
    if (lock_flag) {
	if (h->readonly)
            file_lock.l_type = F_RDLCK;
        else
            file_lock.l_type = F_WRLCK;
    } else
	file_lock.l_type = F_UNLCK;
   
    while (fcntl((h->db->fd)(h->db), F_SETLK, &file_lock) == -1) {
        if (errno == EACCES || errno == EAGAIN) {
	    if (nowait) {
		*st = db_s_already_locked;
		break;
	    }
	    (void) pthread_delay_np(&wait_for_lock);
	}
        else {
            *st = db_s_file_lock_fail;
	    break;
	}
    }
}
