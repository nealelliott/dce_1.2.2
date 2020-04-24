/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prvb_thd_007.c,v $
 * Revision 1.1.62.2  1996/02/17  23:32:07  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:32  marty]
 *
 * Revision 1.1.62.1  1995/12/11  23:04:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:46  root]
 * 
 * Revision 1.1.60.1  1994/04/01  20:12:32  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:20:41  jd]
 * 
 * Revision 1.1.58.1  1993/10/12  19:38:14  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:20  rhw]
 * 
 * Revision 1.1.4.2  1992/12/30  16:55:41  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:13:42  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/15  16:53:09  bolinger
 * 	Fix OT defect 3468:  add testcase to exercise pthread_getunique_np().
 * 	[1992/05/15  13:51:12  bolinger]
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
 *	PRVB_THD_007
 *
 *  TEST TOPICS:
 *
 *	THD  -  Thread Operations
 *
 *  ABSTRACT:
 *
 *	Test that pthread_getunique_np function works correctly
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

#include <pthread.h>
#include <dce/cts_test.h>

#define MAXTHD		15

#if _CMA_THREAD_IS_VP_
# define FIRSTTHD	2
#else
# define FIRSTTHD	3
#endif

any_t
thd	(any_t	numb)
    {
    int		index = (int)numb, self_id;
    pthread_t	self;


    self = pthread_self ();
    self_id = pthread_getunique_np (&self);

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
    pthread_t	thds[MAXTHD];
    int	i;


    cts_test ("PRVB_THD_007", "Test pthread_getunique_np");

    for (i = FIRSTTHD; i < MAXTHD; i++)
	pthread_create (
		&thds[i],
		pthread_attr_default,
		thd,
		(any_t)i);

    for (i = FIRSTTHD; i < MAXTHD; i++)
	pthread_join (
		thds[i],
		(any_t *)0);

    thd ((any_t)1);
    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element PRVB_THD_007.C */
/*  *2    16-MAR-1992 08:58:22 BUTENHOF "Fix compilation warnings" */
/*  *1     4-MAR-1992 13:35:19 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element PRVB_THD_007.C */

