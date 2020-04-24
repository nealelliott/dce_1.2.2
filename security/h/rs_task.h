/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_task.h,v $
 * Revision 1.1.35.2  1996/02/18  22:59:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:17  marty]
 *
 * Revision 1.1.35.1  1995/12/08  17:27:45  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/08  17:29 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/hanfei_cpi_bl2/1  1994/12/07  19:31 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/hanfei_cpi_bl1/1  1994/12/05  14:48 UTC  hanfei
 * 	work for rgy checkpoint to be configurable
 * 	[1995/12/08  16:53:36  root]
 * 
 * Revision 1.1.33.1  1993/10/05  22:30:28  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:11:03  mccann]
 * 
 * Revision 1.1.2.2  1992/12/29  13:25:37  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:15:12  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:42:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rs_task.h V=8 10/03/91 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Server - Task Management Declarations
 * 
 */

#ifndef rs_task__included
#define rs_task__included

#include <dce/nbase.h>

#include <pthread.h>

typedef pthread_t  rs_task_handle_t;
 
typedef void *(*rs_task_func_ptr_t)(
    void  *arg
);

/* Must be called by the distinguished task before any sub-tasks are
 * created. returns false if init fails, true otherwise
 */
boolean32 rs_task_init_module ( 
    void
);
 

/* must be called once by every (non-distinguished) task as its first action */
boolean32 rs_task_init ( 
    void  *arg
);


/* rs_task_get_name
 *
 * returns a pointer to static storage containing the name of the current
 * task.  The task name is always NULL terminated.  The length of the task name is
 * returned in the task_name_len parameter.  If unable to obtain a task
 * name, task_name_len is set to zero, and the function returns NULL.
 */
char *rs_task_get_name ( 
    long    *task_name_len
);


/* rs_task_yield
*
* Causes current task to voluntarily give up the processor if any other
* tasks are waiting
*/
void rs_task_yield (
   void
);

/* rs_task_create
*
* Create a new task.
*/
boolean32 rs_task_create ( 
    rs_task_handle_t    *task_handle,
    rs_task_func_ptr_t  task_func_ptr,
    char                *task_name,
    boolean32           join,       /* will any task join with this one? */
    void                *arg
);

/* rs_task_join
*
* wait for the specified task to finish before continuing.  Only works
* if the task being joined with was created with a "join" parameter of
* true.  Results are unpredictable otherwise.
*/
boolean32 rs_task_join ( 
    rs_task_handle_t  task_handle,
    pthread_addr_t    *task_status
);


/* a sleep function that is guaranteed to be "cancellable" (posix
 * sleep() is not)
 */
void rs_task_sleep ( 
    unsigned32  secs
);

/* a sleep function that is guaranteed to be "cancellable" (posix
 * sleep() is not).  Backoff to 2 * sleep time until max_time reached.
 */
void rs_task_sleep_backoff (
    unsigned32  *secs,
    unsigned32  max_time
);

/* frees up any resources allocated to the indicated task.  Only  needed
 * if the task was created with a "join" parameter of "false".
 */
void  rs_task_release ( 
    rs_task_handle_t  *task_handle
);


/* rs_task_fatal
*
* Signal the current task (but not if it's the Distinguished Task)
*/
void rs_task_fatal( 
   error_status_t    fault_st
);

/* returns true if a fatal error has ocurred in in any but the distinguished
 * task (i.e. some task has called rs_task_fatal), false otherwise.
 */
boolean32 rs_task_test_fatal (
    void
);

/* 
 * initialize condition and mutex needed for pthread_cond_time_wait() 
 * used by rgy checkpoint
 */
boolean32 rs_task_sleep_init (
    void
); 
 
/*
 * Put current thread to sleep for a specified interval,
 * can be interrupted by calling rs_task_sleep_cancel().
 */
void rs_task_sleep_interruptable (
    signed32  	secs_sleep,
    signed32  	*secs_left,
    error_status_t     	*st_p
);

/* 
 * used for waking up a thread that is in
 * rs_task_sleep_interruptable().
 */
boolean32 rs_task_sleep_cancel (
    void
); 

#endif
