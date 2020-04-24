/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prvb_mut_002.c,v $
 * Revision 1.1.38.2  1996/02/17  23:31:57  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:23  marty]
 *
 * Revision 1.1.38.1  1995/12/11  23:03:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:37  root]
 * 
 * Revision 1.1.36.1  1994/04/01  20:12:10  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:19:51  jd]
 * 
 * Revision 1.1.34.1  1993/10/12  19:37:55  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:11  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:55:00  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:13:04  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:19:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1990, 1991 by
 *  Digital Equipment Corporation, Maynard Massachusetts.
 *  All rights reserved.
 *
 *  This software is furnished under a license and may be used and copied
 *  only in  accordance  with  the  terms  of  such  license and with the
 *  inclusion of the above copyright notice.  This software or  any other
 *  copies thereof may not be provided or otherwise made available to any
 *  other person.  No title to and ownership of  the  software  is hereby
 *  transferred.
 *
 *  The information in this software is subject to change  without notice
 *  and should  not  be  construed  as  a commitment by DIGITAL Equipment
 *  Corporation.
 *
 *  DIGITAL assumes no responsibility for the use or  reliability  of its
 *  software on equipment which is not supplied by DIGITAL.
 */

/*
 *  FACILITY:
 *
 *	Pthreads test Programs
 *
 *  FILENAME:
 *
 *	PRVB_MUT_002
 *
 *  TEST TOPICS:
 *
 *	MUT  -  Mutex Operations
 *
 *  ABSTRACT:
 *
 *	Test new "mutex kind" attribute; ensure that a mutex with a
 *	kind of MUTEX_RECURSIVE_NP will allow the same thread to nest
 *	multiple locks, and correctly maintain a reference count; and
 *	that it won't allow another thread to lock it (but that the
 *	other thread will be correctly unblocked when the owner
 *	releases all nested locks).
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	2 July 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Webb Scales	1990
 *		Corrected filename references.
 *	002	Dave Butenhof	12 February 1991
 *		Now "friendly" mutex is "recursive"
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>

#define LOOP	10
#define NUMTHD	10

pthread_t		thd[NUMTHD], guard;
pthread_mutex_t		recursive;
pthread_mutexattr_t	recursive_attr;
int			nest_count, mutex_owner;

static pthread_addr_t thread  (int);

static pthread_addr_t guardian  (pthread_addr_t);

int
main	(int argc, char *argv[], char *envp[])
    {
    pthread_addr_t	value;
    int	i;


    cts_test ("PRVB_MUT_002", "Test recursive mutexes");

    thd[0] = pthread_self ();		/* Store main thread ID for guardian */
    pthread_mutexattr_create (&recursive_attr);
    pthread_mutexattr_setkind_np (&recursive_attr, MUTEX_RECURSIVE_NP);
    pthread_mutex_init (&recursive, recursive_attr);
    nest_count = 0;
    mutex_owner = -1;

    pthread_create (
	    &guard,
	    pthread_attr_default,
	    guardian,
	    (pthread_addr_t)0);

    for (i = 1; i < NUMTHD; i++)
	pthread_create (
		&thd[i],
		pthread_attr_default,
		(pthread_startroutine_t)thread,
		(pthread_addr_t)i);

    thread (0);				/* Call work routine for main thread */

    for (i = 1; i < NUMTHD; i++) {
	pthread_join (thd[i], &value);

	if (value != 0)
	    cts_failed ("thread %d terminated with error", i);

	}

    pthread_cancel (guard);

    if (mutex_owner != -1)
	cts_failed ("final mutex owner is %d (should be -1)", mutex_owner);

    if (nest_count != 0)
	cts_failed ("final nest is %d (should be 0)", nest_count);

    cts_result ();
    }

static pthread_addr_t
thread	(int	thdnum)
    {
    int	i;


#if 0
    if (pthread_mutex_trylock (&recursive)) {
	pthread_mutex_unlock (&recursive);
	printf ("%d starting with mutex unlocked\n", thdnum);
	}
    else
	printf ("%d starting with mutex locked\n", thdnum);
#endif

    pthread_mutex_lock (&recursive);
#if 0
    printf ("%d locking mutex (%d)\n", thdnum, nest_count);
#endif

    if (mutex_owner != -1)
	cts_failed (
		"%d acquired lock with owner == %d",
		thdnum,
		mutex_owner);

    if (nest_count != 0)
	cts_failed (
		"%d acquired lock with nest count == %d",
		thdnum,
		nest_count);

    nest_count++;
    mutex_owner = thdnum;

    for (i = 1; i < LOOP; i++) {
	pthread_mutex_lock (&recursive);
#if 0
	printf ("%d locking mutex (%d)\n", thdnum, nest_count);
#endif

	if (mutex_owner != thdnum)
	    cts_failed (
		    "lock stolen from %d by %d during lock",
		    thdnum,
		    mutex_owner);
	
	if (nest_count != i)
	    cts_failed (
		    "%d: nesting corrupted during lock (%d instead of %d)",
		    thdnum,
		    nest_count,
		    i);

	nest_count++;
	pthread_yield ();
	}

    for (i = LOOP - 1; i >= 0; i--) {

	if (mutex_owner != thdnum)
	    cts_failed (
		    "lock stolen from %d by %d during unlock",
		    thdnum,
		    mutex_owner);
	
	if (--nest_count == 0)
	    mutex_owner = -1;

	if (nest_count != i)
	    cts_failed (
		    "%d: nesting corrupted during unlock (%d instead of %d)",
		    thdnum,
		    nest_count,
		    i);

#if 0
	printf ("%d unlocking mutex (%d)\n", thdnum, nest_count);
#endif
	pthread_mutex_unlock (&recursive);
	pthread_yield ();
	}

    return 0;
    }

static pthread_addr_t
guardian	(pthread_addr_t	arg)
    {
    int			i;
    struct timespec	two_min;


    two_min.tv_sec = 120;
    two_min.tv_nsec = 0;

    /*
     * Just wait for a reasonable amount of time.  If we haven't been
     * terminated by then (by the completion of the test), then alert the
     * main thread to ensure that the test fails.
     */
    pthread_delay_np (&two_min);	/* Wait for 2 minutes */
    cts_failed ("test timed out");

    for (i = 1; i < NUMTHD; i++)
	pthread_cancel (thd[i]);

    pthread_delay_np (&two_min);	/* Let main thread join & report */
    pthread_cancel (thd[0]);
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element PRVB_MUT_002.C */
/*  *8    13-FEB-1991 18:00:44 BUTENHOF "Change names of mutex attributes" */
/*  *7    12-FEB-1991 23:08:52 BUTENHOF "Change 'friendly' to 'recursive'" */
/*  *6    18-JUL-1990 16:28:09 SCALES "Fix filename references" */
/*  *5     3-JUL-1990 12:09:53 BUTENHOF "Change copyright" */
/*  *4     3-JUL-1990 11:24:36 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *3     3-JUL-1990 11:13:15 BUTENHOF "Fix MIPS cc errors" */
/*  *2     2-JUL-1990 15:35:08 BUTENHOF "Put in the element" */
/*  *1     2-JUL-1990 15:26:23 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element PRVB_MUT_002.C */
