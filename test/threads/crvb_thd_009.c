/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_thd_009.c,v $
 * Revision 1.1.35.2  1996/02/17  23:30:39  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:21  marty]
 *
 * Revision 1.1.35.1  1995/12/11  23:00:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:38  root]
 * 
 * Revision 1.1.33.1  1994/04/01  20:10:23  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:15:45  jd]
 * 
 * Revision 1.1.31.1  1993/10/12  19:36:03  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:18  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:32:36  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:22:08  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:21:19  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1990, 1991 by Digital Equipment Corporation, Maynard
 *  Massachusetts. All rights reserved.
 *
 *  This software is furnished under a license and may be used and copied
 *  only in  accordance  with  the  terms  of  such  license and with the
 *  inclusion of the above copyright notice.  This software or  any other
 *  copies thereof may not be provided or otherwise made available to any
 *  other person.  No title to and ownership of  the  software  is hereby
 *  transferred.
 *
 *  The information in this software is subject to change  without notice and
 *  should  not  be  construed  as  a commitment by DIGITAL Equipment
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
 *	CRVB_THD_009
 *
 *  TEST TOPICS:
 *
 *	THD  -  Thread Operations
 *
 *  ABSTRACT:
 *
 *	This program insures that threads run in the correct order
 *	based on their respective priorities and policies.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	26 June 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Paul Curtin	4 September 1990
 *		Convert to use new CMA interfaces.
 *	002	Webb Scales	15 October 1990
 *		Remove "idle" scheduling policy.
 */


/*
 *  INCLUDE FILES
 */
#include <dce/cma.h>
#include <dce/cts_test.h>


cma_t_mutex	the_mutex;
cma_t_boolean	stage = 0;
cma_t_integer	stage_cnt[80];

struct a_thread {
	cma_t_thread		handle;
	cma_t_sched_policy	policy;
	cma_t_priority		priority;
	cma_t_exit_status	status;
	cma_t_address		result;
	cma_t_integer		stage_my;
	cma_t_integer		stage_lo;
	cma_t_integer		stage_hi;
	} thread[80];


#define YIELD_CNT	10
#define COMPUTE_CNT	10000

#define policy_string(p) \
	((p == cma_c_sched_fifo		? "FIFO"	\
	:(p == cma_c_sched_rr		? "RR"		\
	:(p == cma_c_sched_throughput	? "T-put"	\
	:(p == cma_c_sched_background	? "Back"	\
	:(p == cma_c_sched_ada_low	? "Ada_low"	\
	: "*Unknown*"))))))


cma_t_address
thread_rtn	(
	cma_t_address	id_p)
    {
    cma_t_boolean	ooo;
    cma_t_integer	y, c,
			id = (cma_t_integer)id_p;


    cma_mutex_lock (&the_mutex);

    ooo = (stage < thread[id].stage_lo || stage > thread[id].stage_hi);

    if (ooo)
	cts_failed (
		"Thread #%-3d running, out of order: my stage = %d, current = %d\n",
		id,
		thread[id].stage_my,
		stage);
    else
	printf ("Thread #%-3d running. %2d/%s.\n",
		id, 
		thread[id].priority,
                policy_string (thread[id].policy));

    if (--stage_cnt[thread[id].stage_my] < 0)
	cts_failed (
		"Thread #%-3d test bug: my stage = %d, count = %d\n",
		id,
		thread[id].stage_my,
		stage_cnt[thread[id].stage_my]);

    if (thread[id].stage_my > stage)
	cts_failed (
		"Thread #%-3d test bug: my stage = %d, current stage = %d\n",
		id,
		thread[id].stage_my,
		stage);

    for (y = 0; y < YIELD_CNT; y++) {
	cma_mutex_unlock (&the_mutex);
	for (c = 0; c < COMPUTE_CNT; c++);
	cma_yield ();
	cma_mutex_lock (&the_mutex);
	}

    if (thread[id].stage_my == stage)
	while (stage_cnt[stage] == 0) 
	    stage--;

    cma_mutex_unlock (&the_mutex);

    return (cma_t_address)ooo;
    }


main ()
    {
    cma_t_integer   i, id = 0, t = 1, b_stage;
    cma_t_attr	    attr;


    cts_test ("CRVB_THD_009", "Test thread execution order.");
    cma_init ();

    cma_mutex_create (&the_mutex, &cma_c_null);
    cma_attr_create  (&attr,	  &cma_c_null);
    cma_attr_set_inherit_sched (&attr, cma_c_sched_use_default);

    thread[id].policy = cma_c_sched_fifo;
    thread[id].priority = cma_c_prio_fifo_max;
    thread[id].status = cma_c_term_normal;
    thread[id].result = cma_c_null_ptr;
    thread[id].stage_my = stage;
    thread[id].stage_lo = stage;
    thread[id].stage_hi = stage;
    cma_thread_get_self (&thread[id].handle);

    /*
     * Note: the following must be the first attempt to get or set scheduling
     * information, since we depend on catching the cma_e_unimp exception to
     * determine whether scheduling info is supported!
     */
    TRY {
	cma_thread_set_sched (
		&thread[id].handle,
		thread[id].policy,
		thread[id].priority);
	}
    CATCH (cma_e_unimp) {
	cts_comment ("scheduling not implemented on this platform");
	cts_result ();
	exit (0);
	}
    ENDTRY

    /*
     * Ada Low
     */
    b_stage = stage + 1;
    for (i = cma_c_prio_ada_low_min; i <= cma_c_prio_ada_low_max; i++, t++) {
	thread[t].policy = cma_c_sched_ada_low;
	thread[t].priority = (cma_t_priority)i;
	stage++;
	thread[t].stage_my = stage;
	thread[t].stage_lo = stage;
	thread[t].stage_hi = stage;
	stage_cnt[stage] += 1;
	}

    /*
     * Background
     */
    for (i = cma_c_prio_back_min; i <= cma_c_prio_back_max; i++, t++) {
	thread[t].policy = cma_c_sched_background;
	thread[t].priority = (cma_t_priority)i;
	thread[t].stage_my = b_stage;
	thread[t].stage_lo = b_stage;
	thread[t].stage_hi = stage;
	stage_cnt[b_stage] += 1;
	}

    /*
     * Throughput
     */
    stage++;
    for (i = cma_c_prio_through_min; i <= cma_c_prio_through_max; i++, t++) {
	thread[t].policy = cma_c_sched_throughput;
	thread[t].priority = (cma_t_priority)i;
	thread[t].stage_my = stage;
	thread[t].stage_lo = stage;
	thread[t].stage_hi = stage;
	stage_cnt[stage] += 1;
	}

    /*
     * First In First Out
     */
    b_stage = stage;
    for (i = cma_c_prio_fifo_min; i <= cma_c_prio_fifo_max; i++, t++) {
	thread[t].policy = cma_c_sched_fifo;
	thread[t].priority = (cma_t_priority)i;
	stage++;
	thread[t].stage_my = stage;
	thread[t].stage_lo = stage;
	thread[t].stage_hi = stage;
	stage_cnt[stage] += 1;
	}

    /*
     * We need to insure that FIFO and RR threads with the same numerical
     * priorities end up in the same stage...
     */
    if (cma_c_prio_fifo_min != cma_c_prio_rr_min
	    || cma_c_prio_fifo_max != cma_c_prio_rr_max)
	cts_failed (
		"Thread #%-3d test bug: \"FIFO\" & \"RR\" priority ranges are different.\n",
		id);

    /*
     * Round Robin
     */
    for (i = cma_c_prio_rr_min; i <= cma_c_prio_rr_max; i++, t++) {
	thread[t].policy = cma_c_sched_rr;
	thread[t].priority = (cma_t_priority)i;
	b_stage++;
	thread[t].stage_my = b_stage;
	thread[t].stage_lo = b_stage;
	thread[t].stage_hi = b_stage;
	stage_cnt[b_stage] += 1;
	}

    stage++;	/* Create stage, no threads should run during this stage. */

    for (i = 1; i < t; i++) {
	cma_attr_set_sched (&attr, thread[i].policy, thread[i].priority);
	
	cma_mutex_lock (&the_mutex);
	printf ("Thread #%-3d creating thread #%-3d %2d/%s.\n",
		id, 
		i,
		thread[i].priority,
                policy_string (thread[i].policy));
	cma_mutex_unlock (&the_mutex);

	cma_thread_create (
	    &thread[i].handle,
	    &attr,
	    thread_rtn,
	    (cma_t_address)i);
	}

    cma_mutex_lock (&the_mutex);
    printf ("\n");
    stage--;	/* Threads start running as of the first join. */
    cma_mutex_unlock (&the_mutex);

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
	}

    printf ("\n");
    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_THD_009.C */
/*  *6    29-MAY-1991 18:02:47 BUTENHOF "Don't test if priority isn't supported!" */
/*  *5    15-OCT-1990 11:33:26 SCALES "Remove idle scheduling policy" */
/*  *4     5-SEP-1990 13:40:46 CURTIN "Converted to use new CMA interface" */
/*  *3     3-JUL-1990 12:06:18 BUTENHOF "Change copyright" */
/*  *2     3-JUL-1990 08:26:38 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *1    28-JUN-1990 19:16:20 SCALES "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_THD_009.C */
