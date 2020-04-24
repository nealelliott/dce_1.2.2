/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cvpb_cvx_001.c,v $
 * Revision 1.1.6.2  1996/02/17  23:31:11  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:43  marty]
 *
 * Revision 1.1.6.1  1995/12/11  23:01:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:01  root]
 * 
 * Revision 1.1.4.1  1994/04/01  20:11:08  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:17:36  jd]
 * 
 * Revision 1.1.2.2  1992/12/30  16:51:48  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:10:11  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:21:40  devrcs
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
 *	CVPB_CVX_001
 *
 *  TEST TOPICS:
 *
 *	CVX  -  Condition Variable Operations
 *
 *  ABSTRACT:
 *
 *	Set a timer AST, which will signal a condition variable, then block
 *	the main thread on that condition; time the latency from the
 *	signal_int operation until the main thread wakes up.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	12 February 1991
 *
 *  MODIFICATION HISTORY:
 *
 */

/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cts_test.h>
#include <lib$routines.h>
#include <libdtdef.h>

cma_t_cond	cv;
cma_t_mutex	mu;
cma_t_boolean	flag;
cma_t_date_time	signal_time, wake_time, latency;
float		seconds;

int sys$setimr (int,cma_t_date_time *,void (*astadr)(int),int,int);

int sys$gettim (cma_t_date_time *);

static void
ast_time	(int	reqidt)
    {
    flag = cma_c_true;
    cma_cond_signal_int (&cv);
    sys$gettim (&signal_time);
    }

int
main	(int argc, char *argv[], char *envp[])
    {
    cma_t_date_time	expiration, timeout;
    cma_t_status	status;


    cts_test ("CVPB_CVX_001", "Test cma_cond_signal_int");
    cma_init ();
    cma_mutex_create (&mu, &cma_c_null);
    cma_cond_create (&cv, &cma_c_null);
    flag = cma_c_false;

    /*
     * Get an absolute time 5 seconds in the future (note: we know that on
     * VAX/VMS, the cma_t_date_time type is the standard system quadword
     * time. That's not really "cheating", since that was the primary intent
     * of the platform-dependent cma_t_date_time type.
     */
    cma_time_get_expiration (&expiration, 5.0);
    cma_time_get_expiration (&timeout, 10.0);
    sys$setimr (0, &expiration, ast_time, 1, 0);

    cma_mutex_lock (&mu);

    while (!flag) {
	status = cma_cond_timed_wait (&cv, &mu, &timeout);
	sys$gettim (&wake_time);
	}

    cma_mutex_unlock (&mu);

    if (status == cma_s_timed_out)
	cts_failed ("cma_cond_signal_int didn't unblock thread");

    lib$sub_times (&wake_time, &signal_time, &latency);
    lib$cvtf_from_internal_time (&LIB$K_DELTA_SECONDS_F, &seconds, &latency);

    if (seconds < 1.0e-6)
	seconds = 0.0;

    cts_comment ("cma_cond_signal_int latency is %g seconds", seconds);

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CVPB_CVX_001.C */
/*  *1    12-FEB-1991 16:46:05 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CVPB_CVX_001.C */
