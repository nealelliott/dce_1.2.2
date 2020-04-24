/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: scd_thread.h,v $
 * Revision 1.1.38.2  1996/02/18  23:00:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:12  marty]
 *
 * Revision 1.1.38.1  1995/12/08  17:28:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:10  root]
 * 
 * Revision 1.1.36.1  1993/10/05  22:30:36  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:11:18  mccann]
 * 
 * Revision 1.1.2.2  1992/12/29  13:27:17  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:15:54  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:43:08  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  scd_thread.h V=2 10/18/91 //littl/prgy/src/client/sec_clientd
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**  sec_clientd - task module
** 
*/

#ifndef scd_task__included
#define scd_task__included

#include <dce/nbase.h>
#include <pthread.h>

/* Must be called by the distinguished thread before any sub-threads are
 * created. returns false if init fails, true otherwise
 */
boolean32 init_thread_module ( 
#ifdef __STDC__
    void
#endif
);
 

typedef void *(*scd_startroutine_t) (
#ifdef __STDC__
    void *arg
#endif
);



/* scd_create_thread
 *
 * Create a new thread.
 */
boolean32 scd_create_thread ( 
#ifdef __STDC__
    pthread_t               *handle,
    scd_startroutine_t      fcn,
    char                    *thd_name,
    boolean32               join,
    void                    *arg
#endif
);


/* scd_thread_join
 *
 * wait for the specified thread to finish before continuing.  Only works
 * if the thread being joined with was created with a "join" parameter of
 * true.  Results are unpredictable otherwise.
 */
boolean32 scd_thread_join ( 
#ifdef __STDC__
    pthread_t       handle,
    pthread_addr_t  *result
#endif
);


/* a sleep function that is guaranteed to be "cancellable" (posix
 * sleep() is not)
 */
void scd_thread_sleep ( 
#ifdef __STDC__
    unsigned32  secs
#endif
);


/* frees up any resources allocated to the indicated thread.  Only needed
 * if the thread was created with a "join" parameter of "false".
 */
void  scd_thread_release ( 
#ifdef __STDC__
    pthread_t       *handle
#endif
);


/* scd_thread_fatal
 *
 * Signal the current thread (but not if it's the Distinguished Thread)
 */
void scd_thread_fatal( 
#ifdef __STDC__
   error_status_t    fault_st
#endif
);


/* returns true if a fatal error has ocurred in in any but the distinguished
 * thread (i.e. some thread has called scd_thread_fatal), false otherwise.
 */
boolean32 scd_thread_test_fatal (
#ifdef __STDC__
    void
#endif
);  

#endif /* scd_task__included */

