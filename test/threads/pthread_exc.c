/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pthread_exc.c,v $
 * Revision 1.1.8.2  1996/02/17  23:32:10  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:34  marty]
 *
 * Revision 1.1.8.1  1995/12/11  23:04:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:48  root]
 * 
 * Revision 1.1.6.1  1994/04/01  20:12:36  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:20:54  jd]
 * 
 * Revision 1.1.4.2  1992/12/30  16:55:47  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:13:49  zeliff]
 * 
 * Revision 1.1.2.2  1992/06/30  13:51:17  bolinger
 * 	Fix OT defect 4010: use _CMA_PROTO_ to declare functions correctly
 * 	for both ANSI and non-ANSI compilers.
 * 	[1992/06/29  19:36:18  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:22:21  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <pthread.h>
#include <exc_handling.h>
#include <errno.h>

EXCEPTION enomem_e, eagain_e, esrch_e, einval_e;

#define RAISE_ERRNO_AS_EXCEPTION(status) \
    if (status == -1) { \
	EXCEPTION	exc; \
	EXCEPTION_INIT (exc); \
	exc_set_status (&exc, errno); \
	exc_raise (&exc); \
	}

static pthread_once_t	once_block = pthread_once_init;

static void
init_pthread_exc
	(void)
    {
    EXCEPTION_INIT (enomem_e);
    exc_set_status (&enomem_e, ENOMEM);
    EXCEPTION_INIT (eagain_e);
    exc_set_status (&eagain_e, EAGAIN);
    EXCEPTION_INIT (esrch_e);
    exc_set_status (&esrch_e, ESRCH);
    EXCEPTION_INIT (einval_e);
    exc_set_status (&einval_e, EINVAL);
    }

extern int
pthread_exc_create(
	pthread_t	*thread,
	pthread_attr_t	attr,
	void		*(*start_routine)(void *targ),
	void		*arg)
    {
    int		stat;

    pthread_once (&once_block, init_pthread_exc);
    stat = pthread_create (
	    thread,
	    attr,
	    start_routine,
	    arg);

    RAISE_ERRNO_AS_EXCEPTION (stat)
    }

extern int
pthread_exc_join(
	pthread_t	thread,
	void		**status)
    {
    int		stat;

    pthread_once (&once_block, init_pthread_exc);
    stat = pthread_join (
	    thread,
	    status);

    RAISE_ERRNO_AS_EXCEPTION (stat)
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element PTHREAD_EXC.C */
/*  *3     3-JUL-1990 12:10:47 BUTENHOF "Change copyright" */
/*  *2    21-JUN-1990 08:43:40 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *1    17-MAY-1990 13:56:46 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element PTHREAD_EXC.C */
