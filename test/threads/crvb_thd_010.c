/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_thd_010.c,v $
 * Revision 1.1.36.2  1996/02/17  23:30:40  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:21  marty]
 *
 * Revision 1.1.36.1  1995/12/11  23:00:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:39  root]
 * 
 * Revision 1.1.34.1  1994/04/01  20:10:24  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:15:50  jd]
 * 
 * Revision 1.1.32.1  1993/10/12  19:36:05  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:18  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:32:41  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:22:12  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:19:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1991 by
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
 *	CRVB_THD_010
 *
 *  TEST TOPICS:
 *
 *	THD  -  Thread Operations
 *
 *  ABSTRACT:
 *
 *	This test sets the default thread to fifo/max and
 *	then creates a child thread at fifo/med.  The child
 *	thread should not run, unless the parent blocks or
 *	lowers its priority.  The parent does lower its 
 *	priority and the child thread should run, return 
 *	a particular value.  This return value is checked 
 *	to ascertain whether or not the child thread ran when
 *	expected.
 *
 *  AUTHORS:
 *
 *	Paul Curtin
 *
 *  CREATION DATE:
 *
 *	28 May 1991
 *
 *  MODIFICATION HISTORY:
 *
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

#define  HIGH   0
#define  LOW    1
#define  END    2

cma_t_integer   flag = HIGH;
cma_t_thread    thread1;

cma_t_integer thread_routine  (void);
       
int
main	(int argc, char *argv[], char *envp[])
    {
    cma_t_exit_status   exit;
    cma_t_address       result;
    cma_t_attr          attr;
    cma_t_thread        self;
    cma_t_integer       prio_fifo_medium;


    cts_test ("CRVB_THD_010", "Tests set_priority preemption characteristics.");
    cma_init ();

    /*
     * Set the default thread to fifo/max
     */

    cma_thread_get_self (&self);
    cma_thread_set_sched (&self, cma_c_sched_fifo, cma_c_prio_fifo_max);

    /*
     * Set up attributes object with fifo/medium
     */

    cma_attr_create (&attr, &cma_c_null);
    prio_fifo_medium = (cma_c_prio_fifo_max + cma_c_prio_fifo_min) /2;
    cma_attr_set_sched (&attr, cma_c_sched_fifo, prio_fifo_medium);

    /*
     * Create thread using fifo/medium attributes object.
     */

    cma_thread_create (
                &thread1,
                &attr,
                thread_routine,
                cma_c_null_ptr);

    /*
     * Set flag to LOW and changed default threads priority to fifo/low.
     * This should allow the created thread to run and return LOW.  Then
     * return default thread to fifo/high and change the flag value.
     */

    flag = LOW;
    cma_thread_set_priority (&self, cma_c_prio_fifo_min);
    /* Created thread should sneak in here and return LOW */
    cma_thread_set_priority (&self, cma_c_prio_fifo_max);
    flag = END;

    /*
     * Join with the created thread expecting a result value of LOW.
     */

    cma_thread_join (&thread1, &exit, &result);

    if (result == HIGH)
      cts_failed ("Created thread allowed to run, despite lower priority\n");
    else if (result == END)
      cts_failed ("Created thread did not run during priority change\n");
    else if (result != LOW)
      cts_failed ("Unknown failure, did not get expected return value\n");
                           
    cts_result ();

    }


static cma_t_integer
thread_routine        (void)
    {
    return (flag);
    }
                     
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_THD_010.C */
/*  *1    28-MAY-1991 16:17:54 CURTIN "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_THD_010.C */
