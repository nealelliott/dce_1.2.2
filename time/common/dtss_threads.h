/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtss_threads.h,v $
 * Revision 1.1.7.2  1996/02/18  23:11:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:36:25  marty]
 *
 * Revision 1.1.7.1  1995/12/08  18:07:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:23:50  root]
 * 
 * Revision 1.1.5.2  1994/08/16  18:12:47  cbrooks
 * 	CR 11494
 * 	[1994/08/09  18:59:54  cbrooks]
 * 
 * Revision 1.1.5.1  1994/05/12  15:17:12  rhw
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:27  rhw]
 * 
 * Revision 1.1.3.2  1992/12/30  20:39:18  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  19:07:44  htf]
 * 
 * Revision 1.1  1992/01/19  15:32:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * Copyright (c) 1990 by
 * Digital Equipment Corporation, Maynard, Mass.
 * 
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 *
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 *
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 *
 * FACILITY:	Distributed Time Synchronization Service (DTSS)
 *
 * ABSTRACT:	Threads header file
 *
 * ENVIRONMENT:	VAX/VMS V5.0
 *
 * AUTHORS:
 *		Brian Bailey
 *		Distributed Processing Engineering (DPE) 
 * 
 * DATE:	August 28th, 1990
 *
 */


#ifndef _DTSS_THREADS_H_
#define _DTSS_THREADS_H_
#if CONFIG_THREADS

#include <pthread.h>

/*
 * Macros
 */
#if _DECTHREADS_

#define BEGIN_NONREENTRANT pthread_lock_global_np()
#define END_NONREENTRANT pthread_unlock_global_np()

#else

#define BEGIN_NONREENTRANT
#define END_NONREENTRANT

#endif /* _DECTHREADS */


/*
 * Check a pthread function return value and bug check if it's an error
 */
#define CheckPTerr(pthreadFunc)				\
    {							\
	if ((pthreadFunc) < 0)				\
	    BugCheckOSstatus(errno);			\
    }

#define MakeThreadsafe( code )                          \
{                                                       \
    BEGIN_NONREENTRANT;                                 \
    {                                                   \
	code                                            \
    }                                                   \
    END_NONREENTRANT;                                   \
}


/*
 * Mutex operations
 */
#define dtss_mutex_init(mutex_p)			\
    CheckPTerr(pthread_mutex_init((mutex_p),pthread_mutexattr_default))

#define dtss_mutex_destroy(mutex_p)			\
    CheckPTerr(pthread_mutex_destroy((mutex_p)))

#define dtss_mutex_lock(mutex_p)			\
    CheckPTerr(pthread_mutex_lock((mutex_p)))

#define dtss_mutex_unlock(mutex_p)			\
    CheckPTerr(pthread_mutex_unlock((mutex_p)))

/*
 * Condition variable operations
 */
#define dtss_cond_init(cond_p)				\
    CheckPTerr(pthread_cond_init((cond_p),pthread_condattr_default))

#define dtss_cond_wait(cond_p,mutex_p)			\
    CheckPTerr(pthread_cond_wait((cond_p),(mutex_p)))

#define dtss_cond_signal(cond_p)			\
    CheckPTerr(pthread_cond_signal((cond_p)))

#define dtss_cond_broadcast(cond_p)			\
    CheckPTerr(pthread_cond_broadcast((cond_p)))



#else /* !CONFIG_THREADS */

#define MakeThreadsafe(code)				\
	{						\
	 code						\
	}

#endif /* CONFIG_THREADS */
#endif /* _DTSS_THREADS_H_ */
