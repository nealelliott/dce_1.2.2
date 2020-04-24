/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_thd_008.c,v $
 * Revision 1.1.44.2  1996/02/17  23:30:38  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:20  marty]
 *
 * Revision 1.1.44.1  1995/12/11  23:00:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:38  root]
 * 
 * Revision 1.1.42.1  1994/04/01  20:10:22  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:15:41  jd]
 * 
 * Revision 1.1.40.1  1993/10/12  19:35:58  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:17  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:32:30  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:22:04  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:21:18  devrcs
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
 *	CMA Test Programs
 *
 *  FILENAME:
 *
 *	CRVB_THD_008
 *
 *  TEST TOPICS:
 *
 *	THD  -  Thread Operations
 *
 *  ABSTRACT:
 *
 *	This program insures that threads correctly preempt their creator
 *	based on their respective priorities and policies.
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
 *		Remove "idle" scheduling policy.
 *	003	Dave Butenhof	29 May 1991
 *		Don't try to test if scheduling policy isn't implemented on
 *		platform.
 */

/*
 *  INCLUDE FILES
 */
#include <dce/cma.h>
#include <dce/cts_test.h>


cma_t_mutex	the_mutex;
cma_t_boolean	start = cma_c_false;

struct a_thread {
	cma_t_thread		handle;
	cma_t_sched_policy	policy;
	cma_t_priority		priority;
	cma_t_exit_status	status;
	cma_t_address		result;
	};


#define YES	 1
#define NO	 0
#define MAYBE	-1

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
    cma_t_boolean	preempted;


    cma_mutex_lock (&the_mutex);
    preempted = !start;
    if (preempted)
	printf ("Thread #%-3d preempting.\n", id);
    else
	printf ("Thread #%-3d running.\n", id);
    cma_mutex_unlock (&the_mutex);

    return (cma_t_address)preempted;
    }


cma_t_integer
should_preempt	(
	struct a_thread thread[],
	cma_t_integer	id)
    {
    cma_t_boolean   result;


    switch (thread[id].policy) {
	case cma_c_sched_fifo:
	case cma_c_sched_rr:
	    {
	    if (thread[0].policy == cma_c_sched_fifo 
		    || thread[0].policy == cma_c_sched_rr)
		result = (thread[id].priority > thread[0].priority);
	    else
		result = YES;
	    break;
	    }
	case cma_c_sched_throughput:
	    {
	    switch (thread[0].policy) {
		case cma_c_sched_fifo:
		case cma_c_sched_rr:
		    {
		    result = NO;
		    break;
		    }
		case cma_c_sched_throughput:
		    {
		    result = MAYBE;
		    break;
		    }
		case cma_c_sched_background:
		case cma_c_sched_ada_low:
		    {
		    result = YES;
		    break;
		    }
		default:
		    {
		    cts_failed ("Bug in test program: thread[0].policy\n");
		    result = MAYBE;
		    break;
		    }
		}
	    break;
	    }
	case cma_c_sched_background:
	    {
	    switch (thread[0].policy) {
		case cma_c_sched_fifo:
		case cma_c_sched_rr:
		case cma_c_sched_throughput:
		    {
		    result = NO;
		    break;
		    }
		case cma_c_sched_background:
		    {
		    result = MAYBE;
		    break;
		    }
		case cma_c_sched_ada_low:
		    {
		    cts_failed ("This test cannot verify preemption of ada_low by background.\n");
		    result = MAYBE;
		    break;
		    }
		default:
		    {
		    cts_failed ("Bug in test program: thread[0].policy\n");
		    result = MAYBE;
		    break;
		    }
		}
	    break;
	    }
	case cma_c_sched_ada_low:
	    {
	    switch (thread[0].policy) {
		case cma_c_sched_fifo:
		case cma_c_sched_rr:
		case cma_c_sched_throughput:
		    {
		    result = NO;
		    break;
		    }
		case cma_c_sched_background:
		    {
		    cts_failed ("This test cannot verify preemption of background by ada_low.\n");
		    result = MAYBE;
		    break;
		    }
		case cma_c_sched_ada_low:
		    {
		    result = (thread[id].priority > thread[0].priority);
		    break;
		    }
		default:
		    {
		    cts_failed ("Bug in test program: thread[0].policy\n");
		    result = MAYBE;
		    break;
		    }
		}
	    break;
	    }
	default:
	    {
	    cts_failed ("Bug in test program: thread[id].policy\n");
	    result = MAYBE;
	    break;
	    }
	}

    return result;
    }


main ()
    {
    cma_t_integer   i, id = 0, t = 1;
    cma_t_attr	    attr;
    struct a_thread thread[80];


    cts_test ("CRVB_THD_008", "Test preemption during thread creation.");
    cma_init ();

    cma_mutex_create (&the_mutex, &cma_c_null);
    cma_attr_create  (&attr,	  &cma_c_null);
    cma_attr_set_inherit_sched (&attr, cma_c_sched_use_default);

    cma_thread_get_self (&thread[id].handle);

    /*
     * Note: the following must be the first attempt to get or set scheduling
     * information, since we depend on catching the cma_e_unimp exception to
     * determine whether scheduling info is supported!
     */
    TRY {
	cma_thread_get_sched (&thread[id].handle, &thread[id].policy);
	}
    CATCH (cma_e_unimp) {
	cts_comment ("scheduling not implemented on this platform");
	cts_result ();
	exit (0);
	}
    ENDTRY

    cma_thread_get_priority (&thread[id].handle, &thread[id].priority);

    /*
     * Background
     */
    cma_attr_set_sched (&attr, cma_c_sched_background, cma_c_prio_back_mid);

    for (i = cma_c_prio_back_min; i <= cma_c_prio_back_max; i++, t++) {
	thread[t].policy = cma_c_sched_background;
	thread[t].priority = (cma_t_priority)i;
	cma_attr_set_priority (&attr, i);

	cma_mutex_lock (&the_mutex);
	printf ("Thread #%-3d creating thread #%-3d %2d/%s.\n",
		id, 
		t,
		thread[t].priority,
                policy_string (thread[t].policy));
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
    cma_attr_set_sched (&attr, cma_c_sched_ada_low, cma_c_prio_ada_low_mid);

    for (i = cma_c_prio_ada_low_min; i <= cma_c_prio_ada_low_max; i++, t++) {
	thread[t].policy = cma_c_sched_ada_low;
	thread[t].priority = (cma_t_priority)i;
	cma_attr_set_priority (&attr, i);

	cma_mutex_lock (&the_mutex);
	printf ("Thread #%-3d creating thread #%-3d %2d/%s.\n",
		id, 
		t,
		thread[t].priority,
                policy_string (thread[t].policy));
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
    cma_attr_set_sched (&attr, cma_c_sched_throughput, cma_c_prio_through_mid);

    for (i = cma_c_prio_through_min; i <= cma_c_prio_through_max; i++, t++) {
	thread[t].policy = cma_c_sched_throughput;
	thread[t].priority = (cma_t_priority)i;
	cma_attr_set_priority (&attr, i);

	cma_mutex_lock (&the_mutex);
	printf ("Thread #%-3d creating thread #%-3d %2d/%s.\n",
		id, 
		t,
		thread[t].priority,
                policy_string (thread[t].policy));
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
    cma_attr_set_sched (&attr, cma_c_sched_fifo, cma_c_prio_fifo_mid);

    for (i = cma_c_prio_fifo_min; i <= cma_c_prio_fifo_max; i++, t++) {
	thread[t].policy = cma_c_sched_fifo;
	thread[t].priority = (cma_t_priority)i;
	cma_attr_set_priority (&attr, i);

	cma_mutex_lock (&the_mutex);
	printf ("Thread #%-3d creating thread #%-3d %2d/%s.\n",
		id, 
		t,
		thread[t].priority,
                policy_string (thread[t].policy));
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
    cma_attr_set_sched (&attr, cma_c_sched_rr, cma_c_prio_rr_mid);

    for (i = cma_c_prio_rr_min; i <= cma_c_prio_rr_max; i++, t++) {
	thread[t].policy = cma_c_sched_rr;
	thread[t].priority = (cma_t_priority)i;
	cma_attr_set_priority (&attr, i);
	
	cma_mutex_lock (&the_mutex);
	printf ("Thread #%-3d creating thread #%-3d %2d/%s.\n",
		id, 
		t,
		thread[t].priority,
                policy_string (thread[t].policy));
	cma_mutex_unlock (&the_mutex);

	cma_thread_create (
	    &thread[t].handle,
	    &attr,
	    thread_rtn,
	    (cma_t_address) t);
	}

    cma_mutex_lock (&the_mutex);
    printf ("\n");
    start = cma_c_true;
    cma_mutex_unlock (&the_mutex);

    thread[id].result = 0;
    for (i = t - 1; i > 0; i--) {
	cma_mutex_lock (&the_mutex);
	printf ("Thread #%-3d joining with thread #%-3d %2d/%s.\n",
		id, 
		i,
		thread[i].priority,
                policy_string (thread[i].policy));
	cma_mutex_unlock (&the_mutex);

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
	    if (thread[i].result) {
		if (should_preempt (thread, i) == NO)
		    cts_failed ("Thread #%d preempted parent.\n", i);
		}
	    else
		if (should_preempt (thread, i) == YES)
		    cts_failed ("Thread #%d failed to preempt parent.\n", i);
	}

    printf ("\n");
    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_THD_008.C */
/*  *8    29-MAY-1991 18:02:42 BUTENHOF "Don't test if priority isn't supported!" */
/*  *7    15-OCT-1990 11:33:20 SCALES "Remove idle scheduling policy" */
/*  *6     5-SEP-1990 13:38:59 CURTIN "Converted to use new CMA interface" */
/*  *5    21-AUG-1990 09:47:32 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *4    19-JUL-1990 17:50:17 SCALES "fix typos" */
/*  *3     3-JUL-1990 12:06:12 BUTENHOF "Change copyright" */
/*  *2    27-JUN-1990 18:14:04 SCALES "Correct a boo-boo" */
/*  *1    26-JUN-1990 19:44:46 SCALES "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_THD_008.C */
