/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_att_003.c,v $
 * Revision 1.1.38.2  1996/02/17  23:30:05  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:51  marty]
 *
 * Revision 1.1.38.1  1995/12/11  22:58:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:09  root]
 * 
 * Revision 1.1.36.1  1994/04/01  20:09:18  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:13:23  jd]
 * 
 * Revision 1.1.34.1  1993/10/12  19:34:59  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:50  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:29:52  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:06:37  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:46  devrcs
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
 *	RV_ATT_003
 *
 *  TEST TOPICS:
 *
 *	ATT  -  Attributes Objects
 *
 *  ABSTRACT:
 *
 *	Create and then delete a number of object to ensure that
 *	cached objects are eventually flushed from the cache.  There
 *	isn't any automatic way to detect whether this is working as
 *	desired.  However, this test will ensure that no coding
 *	errors result in problems, and it allows manual verification
 *	via a debugger.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	04 June 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	4 September 1990
 *		Change to new CMA interfaces.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

/*
 * We'll create 21 attributes objects, and then delete them, to trigger cache
 * flushing back to the low water mark.  Note: this depends upon the (current)
 * default high water mark of 20!
 */
#define MAX_ATTR	21
#define MAX_THREAD	21
#define MAX_MUTEX	21
#define MAX_COND	21

static cma_t_address empty_thread (cma_t_address);

main ()
    {
    cma_t_attr		attr[MAX_ATTR];
    cma_t_thread	thd[MAX_THREAD];
    cma_t_mutex		mu[MAX_MUTEX];
    cma_t_cond		cv[MAX_COND];
    cma_t_integer	i;
    cma_t_exit_status	status;
    cma_t_address	result;


    cts_test ("CRVB_ATT_003", "Test cache highwater marking");

    cma_init ();

    for (i = 0; i < MAX_ATTR; i++)
	cma_attr_create (&attr[i], &cma_c_null);

    for (i = 0; i < MAX_ATTR; i++)
	cma_attr_delete (&attr[i]);

    for (i = 0; i < MAX_THREAD; i++)
	cma_thread_create (&thd[i], &cma_c_null, empty_thread, cma_c_null_ptr);

    for (i = 0; i < MAX_THREAD; i++) {
	cma_thread_join (&thd[i], &status, &result);
	cma_thread_detach (&thd[i]);
	}

    for (i = 0; i < MAX_COND; i++)
	cma_cond_create (&cv[i], &cma_c_null);

    for (i = 0; i < MAX_COND; i++)
	cma_cond_delete (&cv[i]);

    for (i = 0; i < MAX_MUTEX; i++)
	cma_mutex_create (&mu[i], &cma_c_null);

    for (i = 0; i < MAX_MUTEX; i++)
	cma_mutex_delete (&mu[i]);

    cts_result ();
    }

static cma_t_address
empty_thread    (cma_t_address arg)
    {
    return cma_c_null_ptr;
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_ATT_003.C */
/*  *4     5-SEP-1990 09:40:27 BUTENHOF "Convert to new CMA interfaces" */
/*  *3     3-JUL-1990 12:03:35 BUTENHOF "Change copyright" */
/*  *2    21-JUN-1990 08:27:44 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *1     8-JUN-1990 15:46:34 CURTIN "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_ATT_003.C */
