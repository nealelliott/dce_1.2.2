/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pthread_exc_test.c,v $
 * Revision 1.1.8.2  1996/02/17  23:32:11  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:35  marty]
 *
 * Revision 1.1.8.1  1995/12/11  23:04:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:49  root]
 * 
 * Revision 1.1.6.1  1994/04/01  20:12:38  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:21:02  jd]
 * 
 * Revision 1.1.4.2  1992/12/30  16:55:51  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:13:53  zeliff]
 * 
 * Revision 1.1.2.3  1992/06/30  13:51:20  bolinger
 * 	Fix OT defect 4009: use _CMA_PROTOTYPE_ to conditionalize use
 * 	of ANSI-style function prototype.
 * 	[1992/06/29  19:37:57  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/11  17:08:55  bolinger
 * 	Fix OT defect 2161:  provide/correct missing/bad cts_test() banners.
 * 	[1992/05/11  13:47:12  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:19:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Demonstrate pthread exception wrapper
 *
 *	Dave Butenhof, Fri Mar 23 06:53:12 1990
 */

/*
 *  INCLUDE FILES
 */

#include <dce/pthread_exc.h>
#include <stdio.h>

extern EXCEPTION enomem_e, eagain_e, esrch_e, einval_e;

static void * hello_world  (void *);

main ()
    {
    pthread_t	thread, foo;
    void 	*result;

	cts_test("pthread_exc_test", "Test Pthreads exception wrapper");

    TRY {
	pthread_exc_create (
		&thread,
		pthread_attr_default,
		hello_world,
		(void *)42);
	}
    CATCH (enomem_e) {
	printf ("Insufficient memory for thread.\n");
	}
    CATCH (eagain_e) {
	printf ("Insufficient resources for thread.\n");
	}
    ENDTRY

    /*
     * Join with an "incorrect" handle (it's not initialized).  This will
     * demonstrate that the "einval_e" exception will be raised by the
     * pthread wrapper code.
     */
    TRY {
	pthread_exc_join (
		foo,
		&result);
	}
    CATCH (esrch_e) {
	printf ("Thread does not exist.\n");
	}
    CATCH (einval_e) {
	printf ("Invalid handle.\n");
	}
    CATCH_ALL {
	printf ("Some other error.\n");
	}
    ENDTRY
	cts_result();
    }

static void *
hello_world (arg)
	void	*arg;
    {
    return (void *)printf ("Hello world (%d)\n", arg);
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element PTHREAD_EXC_TEST.C */
/*  *3     3-JUL-1990 12:10:52 BUTENHOF "Change copyright" */
/*  *2    21-JUN-1990 08:43:51 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *1    17-MAY-1990 13:56:48 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element PTHREAD_EXC_TEST.C */
