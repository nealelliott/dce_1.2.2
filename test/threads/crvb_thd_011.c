/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_thd_011.c,v $
 * Revision 1.1.57.2  1996/02/17  23:30:41  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:22  marty]
 *
 * Revision 1.1.57.1  1995/12/11  23:00:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:40  root]
 * 
 * Revision 1.1.55.1  1994/04/01  20:10:25  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:15:54  jd]
 * 
 * Revision 1.1.53.1  1993/10/12  19:36:07  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:19  rhw]
 * 
 * Revision 1.1.4.2  1992/12/30  16:32:46  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:22:15  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/15  16:53:14  bolinger
 * 	Fix OT defect 3468:  add testcase to exercise cma_thread_get_unique().
 * 	[1992/05/15  13:51:59  bolinger]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Digital Equipment Corporation, 1992
 * All Rights Reserved.  Unpublished rights reserved
 * under the copyright laws of the United States.
 *
 * The software contained on this media is proprietary
 * to and embodies the confidential technology of 
 * Digital Equipment Corporation.  Possession, use,
 * duplication or dissemination of the software and
 * media is authorized only pursuant to a valid written
 * license from Digital Equipment Corporation.
 *
 * RESTRICTED RIGHTS LEGEND   Use, duplication, or 
 * disclosure by the U.S. Government is subject to
 * restrictions as set forth in Subparagraph (c)(1)(ii)
 * of DFARS 252.227-7013, or in FAR 52.227-19, as
 * applicable.
 */

/*
 *  FACILITY:
 *
 *	DECthreads Test Suite
 *
 *  FILENAME:
 *
 *	CRVB_THD_011
 *
 *  TEST TOPICS:
 *
 *	THD  -  Thread Operations
 *
 *  ABSTRACT:
 *
 *	Test that cma_thread_get_unique function works correctly
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	04 March 1992
 *
 *  MODIFICATION HISTORY:
 *
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>

#define MAXTHD		15

#if _CMA_THREAD_IS_VP_
# define FIRSTTHD	2
#else
# define FIRSTTHD	3
#endif

cma_t_address
thd	(cma_t_address	numb)
    {
    cma_t_integer	index = (cma_t_integer)numb, self_id;
    cma_t_thread	self;


    cma_thread_get_self (&self);
    self_id = cma_thread_get_unique (&self);

    /*
     * This will break if DECthreads introduces additional "invisible"
     * threads beyond the null thread.
     */
    if (index != self_id)
	cts_failed ("thread %d has unique value of %d", index, self_id);
#if 0
    else
	printf ("thread %d unique value is %d\n", index, self_id);
#endif

    }

int
main	(int argc, char *argv[], char *envp[])
    {
    cma_t_thread	thds[MAXTHD];
    cma_t_integer	i;


    cts_test ("CRVB_THD_011", "Test cma_thread_get_unique");
    cma_init ();

    for (i = FIRSTTHD; i < MAXTHD; i++)
	cma_thread_create (
		&thds[i],
		&cma_c_null,
		thd,
		(cma_t_address)i);

    for (i = FIRSTTHD; i < MAXTHD; i++)
	cma_thread_join (
		&thds[i],
		(cma_t_exit_status *)cma_c_null_ptr,
		(cma_t_address *)cma_c_null_ptr);

    thd ((cma_t_address)1);
    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_THD_011.C */
/*  *3    16-MAR-1992 08:58:05 BUTENHOF "Fix compilation warnings" */
/*  *2     4-MAR-1992 13:37:11 BUTENHOF "Remove printf" */
/*  *1     4-MAR-1992 13:21:16 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_THD_011.C */

