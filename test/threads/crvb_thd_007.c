/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_thd_007.c,v $
 * Revision 1.1.75.1  1996/06/04  22:13:57  arvind
 * 	Fix wait condition variable
 * 	[1996/03/08  22:19 UTC  bissen  /main/DCE_1.2/1]
 *
 * Revision 1.1.73.2  1996/02/17  23:30:37  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:19  marty]
 * 
 * Revision 1.1.73.1  1995/12/11  23:00:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:37  root]
 * 
 * Revision 1.1.71.1  1994/04/01  20:10:20  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:15:36  jd]
 * 
 * Revision 1.1.69.1  1993/10/12  19:35:55  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:16  rhw]
 * 
 * Revision 1.1.5.3  1993/02/01  21:34:01  hinman
 * 	[hinman] - SVR4 changes
 * 	[1993/02/01  21:05:47  hinman]
 * 
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:06:45  hinman]
 * 
 * Revision 1.1.5.2  1992/12/30  16:32:24  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:22:00  zeliff]
 * 	Revision 1.1.3.2  1993/01/11  16:16:23  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * Revision 1.1.2.2  1992/06/12  19:28:02  bolinger
 * 	Fix up log message.
 * 	[1992/06/12  19:27:16  bolinger]
 * 
 * 	Fix OT defect 4196:  Split long output lines into SCREEN_WIDTH-sized
 * 	pieces, to permit subsequent automated processing by programs (e.g.,
 * 	most Unix utilities) too brain-dead to cope with arbitrarily-long
 * 	lines.
 * 	[1992/06/10  21:48:39  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:21:17  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1990 by
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
 *	CMA Test Programs
 *
 *  FILENAME:
 *
 *	CRVB_THD_007
 *
 *  TEST TOPICS:
 *
 *	THD  -  Thread Operations
 *
 *  ABSTRACT:
 *
 *	This program insures that threads can be created with
 *	the proper combinations of priorities and policies.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	19 June 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Paul Curtin	4 September 1990
 *		Convert to new CMA interface.
 *	002	Webb Scales	15 October 1990
 *		Removed the "idle" scheduling policy
 */

/*
 *  INCLUDE FILES
 */
#include <dce/cma.h>
#include <dce/cts_test.h>
#if (_CMA_UNIX_TYPE == _CMA__SVR4)
# include <stdio.h>
#endif


cma_t_cond	wait_cv;
cma_t_mutex	the_mutex;
cma_t_boolean	start = cma_c_false,
		stop  = cma_c_false;

struct a_thread {
	cma_t_thread		handle;
	cma_t_sched_policy	policy;
	cma_t_priority		priority;
	cma_t_exit_status	status;
	cma_t_address		result;
	};


#define SCREEN_WIDTH 80
#define OUTPUT_WIDTH 31
#define MAX_ITERATES 512
#define H_CHUNK	    (MAX_ITERATES / (SCREEN_WIDTH - OUTPUT_WIDTH - 1))
#define TEST_TIME   60.0
#define W_DELAY	    0.000001
#define C_DELAY	    1000
#define P_INTERVAL  100

#define policy_string(p) \
	((p == cma_c_sched_fifo		? "FIFO"	\
	:(p == cma_c_sched_rr		? "RR"		\
	:(p == cma_c_sched_throughput	? "T-put"	\
	:(p == cma_c_sched_background	? "Back"	\
	:(p == cma_c_sched_ada_low	? "Ada_low"	\
	: "*Unknown*"))))))


cma_t_address
thread_rtn	(
	cma_t_address	id)
    {
    cma_t_integer   i, cnt = 0;


    cma_mutex_lock (&the_mutex);
    if (!start)  
        cts_failed ("Thread #%d:  incorrectly preempted parent.\n", id);

    while (!start) cma_cond_wait (&wait_cv, &the_mutex);

    while (!stop) {
	if (cnt%P_INTERVAL == 0) 
	    printf ("Thread #%-3d at iteration #%d\n", id, cnt);
	cma_mutex_unlock (&the_mutex);

	cnt++;

	for (i = 0; i < C_DELAY; i++);

	cma_delay (W_DELAY);
	cma_mutex_lock (&the_mutex);
	}

    cma_mutex_unlock (&the_mutex);

    return (cma_t_address)cnt;
    }


cma_t_integer
comp_result	(
	struct a_thread *first,
	struct a_thread *second)
    { 
    return (first->result == second->result ? 0
	   :(first->result > second->result ? -1 : 1));
    }

cma_t_integer
comp_priority	(
	struct a_thread *first,
	struct a_thread *second)
    { 
    return (first->priority == second->priority ? 0
	   :(first->priority > second->priority ? -1 : 1));
    }


main ()
    {
    cma_t_integer   i, id = 0, t = 1;
    cma_t_attr	    attr;
    struct a_thread thread[80];
	char buf[SCREEN_WIDTH+1];
	cma_t_integer col;


    cts_test ("CRVB_THD_007", "Test thread scheduling policies and priorities");
    cma_init ();

    cma_mutex_create (&the_mutex, &cma_c_null);
    cma_cond_create  (&wait_cv,	  &cma_c_null);
    cma_attr_create  (&attr,	  &cma_c_null);
    cma_attr_set_inherit_sched (&attr, cma_c_sched_use_default);

    /*
     * "I am the master thread.  I will tolerate no interruptions from others."
     */
    cma_thread_get_self (&thread[id].handle);
    thread[id].policy = cma_c_sched_fifo;
    thread[id].priority = cma_c_prio_fifo_max;
    cma_thread_set_sched (
	    &thread[id].handle, 
	    thread[id].policy, 
	    thread[id].priority);

    /*
     * Background
     */
    cma_mutex_lock (&the_mutex);
    printf ("Thread #%-3d beginning \"background\" threads.\n", id);
    cma_mutex_unlock (&the_mutex);

    cma_attr_set_sched (&attr, cma_c_sched_background, cma_c_prio_back_mid);

    if ((cma_c_prio_back_min > cma_c_prio_back_mid) 
	    || (cma_c_prio_back_mid > cma_c_prio_back_max))
	cts_failed ("Bad value(s) for \"background\" priority constant(s)");

    for (i = cma_c_prio_back_max; i >= cma_c_prio_back_min; i--, t++) {
	thread[t].policy = cma_c_sched_background;
	thread[t].priority = (cma_t_priority)i;
	cma_attr_set_priority (&attr, i);

	cma_mutex_lock (&the_mutex);
	printf ("Thread #%-3d creating thread #%d.\n", id, t);
	cma_mutex_unlock (&the_mutex);

	cma_thread_create (
	    &thread[t].handle,
	    &attr,
	    thread_rtn,
	    (cma_t_address) t);
	}

    /*
     * Ada Low
     */
    cma_mutex_lock (&the_mutex);
    printf ("Thread #%-3d beginning \"ada_low\" threads.\n", id);
    cma_mutex_unlock (&the_mutex);

    cma_attr_set_sched (&attr, cma_c_sched_ada_low, cma_c_prio_ada_low_mid);

    if ((cma_c_prio_ada_low_min > cma_c_prio_ada_low_mid) 
	    || (cma_c_prio_ada_low_mid > cma_c_prio_ada_low_max))
	cts_failed ("Bad value(s) for \"ada_low\" priority constant(s)");

    for (i = cma_c_prio_ada_low_max; i >= cma_c_prio_ada_low_min; i--, t++) {
	thread[t].policy = cma_c_sched_ada_low;
	thread[t].priority = (cma_t_priority)i;
	cma_attr_set_priority (&attr, i);

	cma_mutex_lock (&the_mutex);
	printf ("Thread #%-3d creating thread #%d.\n", id, t);
	cma_mutex_unlock (&the_mutex);

	cma_thread_create (
	    &thread[t].handle,
	    &attr,
	    thread_rtn,
	    (cma_t_address) t);
	}

    /*
     * Throughput
     */
    cma_mutex_lock (&the_mutex);
    printf ("Thread #%-3d beginning \"throughput\" threads.\n", id);
    cma_mutex_unlock (&the_mutex);

    cma_attr_set_sched (&attr, cma_c_sched_throughput, cma_c_prio_through_mid);

    if ((cma_c_prio_through_min > cma_c_prio_through_mid) 
	    || (cma_c_prio_through_mid > cma_c_prio_through_max))
	cts_failed ("Bad value(s) for \"throughput\" priority constant(s)");

    for (i = cma_c_prio_through_max; i >= cma_c_prio_through_min; i--, t++) {
	thread[t].policy = cma_c_sched_throughput;
	thread[t].priority = (cma_t_priority)i;
	cma_attr_set_priority (&attr, i);

	cma_mutex_lock (&the_mutex);
	printf ("Thread #%-3d creating thread #%d.\n", id, t);
	cma_mutex_unlock (&the_mutex);

	cma_thread_create (
	    &thread[t].handle,
	    &attr,
	    thread_rtn,
	    (cma_t_address) t);
	}

    /*
     * First In First Out
     */
    cma_mutex_lock (&the_mutex);
    printf ("Thread #%-3d beginning \"fifo\" threads.\n", id);
    cma_mutex_unlock (&the_mutex);

    cma_attr_set_sched (&attr, cma_c_sched_fifo, cma_c_prio_fifo_mid);

    if ((cma_c_prio_fifo_min > cma_c_prio_fifo_mid) 
	    || (cma_c_prio_fifo_mid > cma_c_prio_fifo_max))
	cts_failed ("Bad value(s) for \"fifo\" priority constant(s)");

    for (i = cma_c_prio_fifo_max; i >= cma_c_prio_fifo_min; i--, t++) {
	thread[t].policy = cma_c_sched_fifo;
	thread[t].priority = (cma_t_priority)i;
	cma_attr_set_priority (&attr, i);

	cma_mutex_lock (&the_mutex);
	printf ("Thread #%-3d creating thread #%d.\n", id, t);
	cma_mutex_unlock (&the_mutex);

	cma_thread_create (
	    &thread[t].handle,
	    &attr,
	    thread_rtn,
	    (cma_t_address) t);
	}

    /*
     * Round Robin
     */
    cma_mutex_lock (&the_mutex);
    printf ("Thread #%-3d beginning \"round robin\" threads.\n", id);
    cma_mutex_unlock (&the_mutex);

    cma_attr_set_sched (&attr, cma_c_sched_rr, cma_c_prio_rr_mid);

    if ((cma_c_prio_rr_min > cma_c_prio_rr_mid) 
	    || (cma_c_prio_rr_mid > cma_c_prio_rr_max))
	cts_failed ("Bad value(s) for \"rr\" priority constant(s)");

    for (i = cma_c_prio_rr_max; i >= cma_c_prio_rr_min; i--, t++) {
	thread[t].policy = cma_c_sched_rr;
	thread[t].priority = (cma_t_priority)i;
	cma_attr_set_priority (&attr, i);
	
	cma_mutex_lock (&the_mutex);
	printf ("Thread #%-3d creating thread #%d.\n", id, t);
	cma_mutex_unlock (&the_mutex);

	cma_thread_create (
	    &thread[t].handle,
	    &attr,
	    thread_rtn,
	    (cma_t_address) t);
	}

    cma_mutex_lock (&the_mutex);
    printf ("\nThread #%-3d Starting...\n\f", id);
    start = cma_c_true;
    cma_mutex_unlock (&the_mutex);

    cma_cond_broadcast (&wait_cv);
    cma_delay (TEST_TIME);

    cma_mutex_lock (&the_mutex);
    printf ("\nThread #%-3d Finishing...\n", id);
    stop = cma_c_true;
    cma_mutex_unlock (&the_mutex);

    thread[id].result = 0;
    for (i = 1; i < t; i++) {
	cma_thread_join (
	    &thread[i].handle,
	    &thread[i].status,
	    &thread[i].result);

	if (thread[i].status != cma_c_term_normal) {
	    cma_mutex_lock (&the_mutex);
            cts_failed ("Thread #%d exited abnormally.\n", i);
	    cma_mutex_unlock (&the_mutex);
	    }
	else
	    thread[id].result = (cma_t_address)((cma_t_integer)thread[id].result
		    + (cma_t_integer)thread[i].result);
	}

    printf ("Thread #%-3d %2d/%s\t%5d Total Iterations.\n", 
		id,
		thread[id].priority,
                policy_string (thread[id].policy),
		thread[id].result);

    printf ("\f\tSorted by priority\n\t====== == ========\n\n");
    qsort ((char *)&thread[1], t-1, sizeof (thread[1]), comp_priority);

    for (i = 1; i < t; i++) {
		cma_t_integer j;

        sprintf (buf, "Thread #%-3d %2d/%-9.9s %5d: ", 
			i,
			thread[i].priority,
            policy_string (thread[i].policy),
			thread[i].result);
		j = (cma_t_integer)thread[i].result;
		col = strlen(buf);
		fputs(buf, stdout);
		while (j) {
			if (SCREEN_WIDTH <= col) {
				printf("\n");
				col = 0;
			}
			printf ("*");
			j -= (j > H_CHUNK ? H_CHUNK : j);
			++col;
		}
		printf ("\n");
	}

    printf ("\f\tSorted by result\n\t====== == ======\n\n");
    qsort ((char *)&thread[1], t-1, sizeof (thread[1]), comp_result);

    for (i = 1; i < t; i++) {
		cma_t_integer j;

        sprintf (buf, "Thread #%-3d %2d/%-9.9s %5d: ", 
			i,
			thread[i].priority,
            policy_string (thread[i].policy),
			thread[i].result);

		j = (cma_t_integer)thread[i].result;
		col = strlen(buf);
		fputs(buf, stdout);
		while (j) {
			if (SCREEN_WIDTH <= col) {
				printf("\n");
				col = 0;
			}
			printf ("*");
			j -= (j > H_CHUNK ? H_CHUNK : j);
			++col;
		}
		printf ("\n");
	}

    printf ("\n\n\n");
    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_THD_007.C */
/*  *6    15-OCT-1990 11:42:25 SCALES "Remove idle scheduling policy" */
/*  *5     5-SEP-1990 13:32:49 CURTIN "Converted to use new CMA interface" */
/*  *4    21-AUG-1990 09:47:17 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *3    19-JUL-1990 17:50:06 SCALES "fix typos" */
/*  *2     3-JUL-1990 12:06:05 BUTENHOF "Change copyright" */
/*  *1    25-JUN-1990 18:37:52 SCALES "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_THD_007.C */
