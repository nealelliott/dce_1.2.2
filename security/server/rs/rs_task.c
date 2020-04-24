/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_task.c,v $
 * Revision 1.1.9.2  1996/02/18  00:20:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:08:22  marty]
 *
 * Revision 1.1.9.1  1995/12/08  17:57:02  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/07  20:05 UTC  psn
 * 	Fix parameter type for call to rs_task_join().
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/08  17:30 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/hanfei_cpi_bl2/1  1994/12/07  19:32 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/hanfei_cpi_bl1/1  1994/12/05  14:49 UTC  hanfei
 * 	work for rgy checkpoint to be configurable
 * 	[1995/12/08  17:21:43  root]
 * 
 * Revision 1.1.7.2  1994/08/04  16:14:08  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:12:38  mdf]
 * 
 * Revision 1.1.7.1  1994/06/02  21:20:44  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:20:19  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:57:16  mdf]
 * 
 * 	HP revision /main/ODESSA_2/1  1994/05/03  21:16 UTC  hanfei
 * 	 merge
 * 
 * 	HP revision /main/hanfei_ch_master/1  1994/05/03  20:42 UTC  hanfei
 * 	label change master work
 * 
 * Revision 1.1.5.2  1993/08/23  17:50:37  hanfei
 * 	[7763] rs_task_fatal calls abort(), ensuring that a stack trace is
 * 	available.
 * 	[1993/08/23  17:49:28  hanfei]
 * 
 * Revision 1.1.3.3  1992/12/29  16:37:36  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:27:04  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  20:13:08  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:38:07  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:44:01  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rs_task.c V=11 10/18/91 //littl/prgy/src/server/rs
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
 *      Registry Server - Task Management 
 *
 *   Encapsulates the registry server tasking calls in a single place to ease
 *   porting effort.
 * 
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rs_task.c	11 - 10/18/91";
#endif
 
#include <stdio.h>
#include <un_strng.h>

#include <dce/dce.h>
#include <sec_svc.h>

#include <rs_task.h>
#include <rs_base.h>
#include <rs_util.h>
#include <pthread.h>

PRIVATE boolean32      fatal_error_ocurred  = false;
PRIVATE boolean32      rs_task_module_inited = false;
PRIVATE pthread_t      distinguished_task;
PRIVATE pthread_key_t  name_key;     /* we store task names in per-thread storage */
PRIVATE pthread_attr_t rs_task_attr; /* our tasks don't use the default attribues */

PRIVATE pthread_cond_t		sleep_cond;
PRIVATE pthread_mutex_t		sleep_mutex;

#define TASK_STACK_SIZE     64000

#ifdef pthread_equal
#define THREAD_EQUAL(thr1, thr2) (pthread_equal(thr1, thr2))
#else
#define THREAD_EQUAL(thr1, thr2) (pthread_equal_np(thr1, thr2))
#endif

/* must be called once by the distinguished task before any sub-tasks are
 * created
 */
PUBLIC boolean32 rs_task_init_module (
    void
)
{
    if (!rs_task_module_inited) {
	rs_task_module_inited = true;

	distinguished_task = pthread_self();
	
	if (pthread_attr_create(&rs_task_attr) == -1) {
            dce_svc_printf(SEC_SYS_ERRNO_TEXT_MSG,
                           "pthread_attr_create",
                           strerror(errno));
	    return false;
	}

	if (pthread_attr_setstacksize(&rs_task_attr, TASK_STACK_SIZE) == -1) {
            dce_svc_printf(SEC_SYS_ERRNO_TEXT_MSG,
                           "pthread_attr_setstacksize",
                           strerror(errno));
	    return false;
	}

	/*XXX may need to set priority to something other than the default */
    }

    return true;
}


/* must be called once every (non-distinguished) task as its first action */
PUBLIC boolean32 rs_task_init
  (
    void  *arg
  )
{

    return true;  /* currently a no-op */
}   


PUBLIC char *rs_task_get_name
  (
    long    *task_name_len
  )
{
    static char task_name[] = "Unknown task";

    *task_name_len = strlen(task_name);

    return task_name;
}
 

PUBLIC void rs_task_yield ( )
{

    pthread_yield();
}


PUBLIC boolean32 rs_task_create
  (
    rs_task_handle_t    *task_handle,
    void                *(*task_fn)(),
    char                *task_name,
    boolean32           join,
    void                *arg
  )
{
    int         result; 
    void        *thread_status;
    char        msg[256];

    result = pthread_create(task_handle, rs_task_attr,
                (cma_t_address /* pthread_startroutine_t */(*)(cma_t_address)) task_fn,
                (void *) arg);
    if (result == -1) {
        dce_svc_printf(SEC_SYS_ERRNO_TEXT_MSG,
                       "pthread_create",
                       strerror(errno));
        dce_svc_printf(SEC_RS_THR_CREATE_FAIL_MSG, task_name);
	return false;
    } else {
	if (!join) {
	    (void) pthread_detach(task_handle);
	}
    }
    return true;
}


PUBLIC boolean32 rs_task_join
  (
    rs_task_handle_t  task_handle,
    pthread_addr_t    *result
  )
{
    char              msg[256];

    if (pthread_join(task_handle, result) == -1) {
        dce_svc_printf(SEC_SYS_ERRNO_TEXT_MSG,
                       "pthread_join",
                       strerror(errno));
	return false;
    }

    return true;
}


/* frees up any resources allocated to the indicated task.  Only  needed
 * if the task was created with a "join" parameter of "false".
 */
PUBLIC void rs_task_release
  (
    rs_task_handle_t  *task_handle
  )
{
    char              msg[256];

    if (pthread_detach(task_handle) == -1) {
        dce_svc_printf(SEC_SYS_ERRNO_TEXT_MSG,
                       "pthread_detach",
                       strerror(errno));

    }
}


PUBLIC void rs_task_sleep
  (
    unsigned32  secs
  )
{
    struct timespec delay_time;

    delay_time.tv_sec = secs;
    delay_time.tv_nsec = 0;

    pthread_delay_np(&delay_time);
}


PUBLIC void rs_task_sleep_backoff
  (
    unsigned32  *secs,
    unsigned32  max_time
  )
{
    struct timespec delay_time;

    if (*secs > max_time) {
        *secs = max_time;
    }

    delay_time.tv_sec = *secs;
    delay_time.tv_nsec = 0;

    pthread_delay_np(&delay_time);

    if (*secs < max_time) {
        *secs = *secs * 2;
    } 

}



/*
 * A disaster occurred, abort the process.
 */
PUBLIC void rs_task_fatal
  (
    error_status_t    fault_st
  )
{
    abort();
}  


PUBLIC boolean32 rs_task_test_fatal (
    void
)
{
    return fatal_error_ocurred;
}


/* 
 * initialize condition and mutex needed for pthread_cond_time_wait() 
 * used by rgy checkpoint
 */
PUBLIC boolean32 rs_task_sleep_init
  (
  )
{
    if ( pthread_mutex_init ( &sleep_mutex, pthread_mutexattr_default) != 0 ) {
        dce_svc_printf(SEC_SYS_ERRNO_TEXT_MSG,
                       "pthread_mutex_init",
                       strerror(errno));
	return false;
    }

    if ( pthread_cond_init ( &sleep_cond, pthread_condattr_default ) == -1 ) {
        dce_svc_printf(SEC_SYS_ERRNO_TEXT_MSG,
                       "pthread_cond_init",
                       strerror(errno));
	return false;
    }
    return true;
}


/*
 * Put current thread to sleep for a specified interval,
 * can be interrupted by calling rs_task_sleep_cancel().
 */
PUBLIC void rs_task_sleep_interruptable
  (
    signed32  	secs_sleep,
    signed32  	*secs_left,
    error_status_t     	*st_p
  )
{
    struct timespec delay_time;
    struct timespec wait_time;
    struct timespec curr_time;

    CLEAR_STATUS(st_p);
    delay_time.tv_sec = secs_sleep;
    delay_time.tv_nsec = 0;

    if ( pthread_get_expiration_np(&delay_time, &wait_time) == -1 ) {
        dce_svc_printf(SEC_SYS_ERRNO_TEXT_MSG,
                       "pthread_get_expiration_np",
                       strerror(errno));
	SET_STATUS(st_p, sec_s_pgmerr);
	return;
    }
       
    if ( pthread_mutex_lock ( &sleep_mutex ) == -1 ) {
        dce_svc_printf(SEC_SYS_ERRNO_TEXT_MSG,
                       "pthread_mutex_lock",
                       strerror(errno));
	SET_STATUS(st_p, sec_s_pgmerr);
	return;
    }

    pthread_cond_timedwait(&sleep_cond, &sleep_mutex, &wait_time);

    if ( pthread_mutex_unlock ( &sleep_mutex ) == -1 ) {
        dce_svc_printf(SEC_SYS_ERRNO_TEXT_MSG,
                       "pthread_mutex_unlock",
                       strerror(errno));
	SET_STATUS(st_p, sec_s_pgmerr);
	return;
    }

    delay_time.tv_sec = 0;
    delay_time.tv_nsec = 0;
    if ( pthread_get_expiration_np(&delay_time, &curr_time) == -1 ) {
        dce_svc_printf(SEC_SYS_ERRNO_TEXT_MSG,
                       "pthread_get_expiration_np",
                       strerror(errno));
	SET_STATUS(st_p, sec_s_pgmerr);
	return;
    }

    *secs_left = wait_time.tv_sec - curr_time.tv_sec;
    if ( *secs_left < 0 ) {
	*secs_left = 0;
    }
}


/* 
 * used for waking up a thread that is in
 * rs_task_sleep_interruptable().
 */
PUBLIC boolean32 rs_task_sleep_cancel
  (
  )
{
    if (pthread_cond_signal(&sleep_cond) == -1 ) {
        dce_svc_printf(SEC_SYS_ERRNO_TEXT_MSG,
                       "pthread_cond_signal",
                       strerror(errno));
	return false;
    }
    return true;
}



