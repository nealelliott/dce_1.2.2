/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prrb_att_001.c,v $
 * Revision 1.1.35.2  1996/02/17  23:31:32  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:02  marty]
 *
 * Revision 1.1.35.1  1995/12/11  23:02:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:18  root]
 * 
 * Revision 1.1.33.1  1994/04/01  20:11:36  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:18:29  jd]
 * 
 * Revision 1.1.31.1  1993/10/12  19:37:15  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:51  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:53:13  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:11:31  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:22:00  devrcs
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
 *	PTHREAD Test Programs
 *
 *  FILENAME:
 *
 *	PRRB_ATT_001
 *
 *  TEST TOPICS:
 *
 *	ATT  -  Attributes Objects
 *
 *  ABSTRACT:
 *
 *	Test for problem reported by Harold Siegel (Ada RTL); the
 *	embedded synch. objects in TCB were created using 
 *	cma__g_def_attr, but flushing of the attributes object TCB
 *	cache during deletion assumed they were created using the
 *	same attributes object as the TCB; this resulted in freeing
 *	the synch objects without proper locking (it was detected
 *	by assertions in cma__free_mutex_nolock).  Now embedded
 *	synch objects are created using the TCB's attributes object
 *	to avoid the problem.
 *	
 *	This test verifies that the assertions are no longer
 *	triggered.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	05 April 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Paul Curtin	29 May 1990
 *		Converted test to use pthreads interface.
 *	002	Paul Curtin	15 May 1990
 *		Cast pointer to pthread_addr_t to suit Mips cc.
 *	003	Paul Curtin	05 July 1990
 *		Added an ifdef proto
 *	004	Paul Curtin	03 April 1991
 *		Added a call to pthread_attr_get_stacksize.
 *	005	Paul Curtin	09 May 1991
 *		Adjusted check on stack size to consider page limits.
 *	006	Paul Curtin	15 May 1991
 *		Added another stack size check.
 *	007	Dave Butenhof	29 May 1991
 *		Fix name printout.
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>

pthread_addr_t
thread(
	pthread_addr_t	arg)

    {
    return (pthread_addr_t)0;
    }

main ()
    {
    pthread_attr_t  att;
    pthread_t	    th;
    int		    status;
    int		    join_status;
    unsigned long   size;
    pthread_addr_t  result;


    cts_test ("PRRB_ATT_001", "Check locking during deletion of attr. objects");

    /*
     * Create a new attributes object with a small stack size.  An
     * optimization prevents flushing of the cached TCB list if the
     * attributes object being freed has the default stack and guard size
     * attributes (since the object can still be reused when and if the
     * attributes object is reused).  To provoke the bug, we need to ensure
     * that the TCB cache is flushed.
     */
    status = pthread_attr_create (&att);
    check(status,"pthread_attr_create");
    status = pthread_attr_setstacksize (&att, 200);
    check(status,"pthread_attr_setstacksize");
    size = pthread_attr_getstacksize (att);
    check(((int)size),"pthread_attr_getstacksize");
    if ((size != 200) && (size != 512) && (size != 2048) && (size != 4096))	    
	cts_failed ("Pthread_attr_getstacksize did not return correct size\n");
	

    /*
     * Create a thread using the attributes object, and wait for it to
     * complete.  Detach the handle to free the TCB onto att's cache list.
     */
    status = pthread_create (&th, att, thread, (pthread_addr_t)0);
    check(status,"pthread_create");
    status = pthread_join (th,(pthread_addr_t *)&join_status);
    check(status,"pthread_join");
    status = pthread_detach (&th);
    check(status,"pthread_detach");

    /*
     * Now, delete the attributes object to flush the cache list.  If the bug
     * is present (and this is a debug version of the CMA reference
     * implementation), several WARNING assertion messages should occur.
     * Note that this condition cannot be tested for by the program; manual
     * review is required.
     */
    status = pthread_attr_delete (&att);
    check(status,"pthread_attr_delete");
    status = pthread_detach (&att);
    check(status,"pthread_detach");

    cts_result ();

    }
