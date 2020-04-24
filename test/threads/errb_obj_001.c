/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: errb_obj_001.c,v $
 * Revision 1.1.42.2  1996/02/17  23:31:18  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:51  marty]
 *
 * Revision 1.1.42.1  1995/12/11  23:01:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:07  root]
 * 
 * Revision 1.1.40.1  1994/04/01  20:11:11  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:17:44  jd]
 * 
 * Revision 1.1.38.1  1993/10/12  19:36:53  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:41  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:52:24  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:10:41  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:22:37  devrcs
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
 *	ERRB_OBJ_001
 *
 *  TEST TOPICS:
 *
 *	OBJ  -  Object creation/management
 *
 *  ABSTRACT:
 *
 *	Verify that DECthreads operation proceeds correctly when
 *	creation of a new object (TCB, CV, MUTEX) exceeds the process
 *	memory limits. This means that all internal synch. objects
 *	are released correctly so that additional objects can be
 *	created and used. The test does not verify directly that
 *	there are no memory leaks as a result of the condition,
 *	although it's likely that the test will fail if there are
 *	significant leaks.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	30 October 1991
 *
 *  MODIFICATION HISTORY:
 *
 */

/*
 *  INCLUDE FILES
 */

#include <pthread_exc.h>
#include <dce/cts_test.h>

/*
 * Assuming that malloc() returns a quadword-aligned address (which it may
 * not), it should presumably allocate one quadword for the "invisible" tag
 * information prior to that address. Added to the link and count fields
 * inside the handles_t struct, that leaves 62 quadwords in a page.
 * Therefore, it seems convenient to define each structure with 62 handles.
 */
#define MAXHAND	62

typedef struct HANDLES_T {
    struct HANDLES	*link;
    int			count;
    cma_t_handle	handles[MAXHAND];
    } handles_t;

handles_t	*handle_chain, *last_chain;

handles_t *
new_block    (void)
    {
    handles_t	*new;


    new = (handles_t *)malloc (sizeof (handles_t));

    if (!new)
	RAISE (exc_insfmem_e);

    last_chain->link = new;
    last_chain = new;
    new->link = (handles_t *)0;
    new->count = 0;
    return new;
    }

pthread_addr_t
null    (pthread_addr_t	arg)
    {
    return (pthread_addr_t)0;
    }

int
main	(int argc, char *argv[], char *envp[])
    {
    pthread_t		thread;
    pthread_attr_t	ta;
    pthread_condattr_t	ca;
    pthread_mutexattr_t	ma;
    pthread_cond_t	cond;
    pthread_mutex_t	mu;
    handles_t		*curblk;
    int			thdcnt = 0, cvcnt = 0, mcnt = 0, i;


    cts_test ("ERRB_OBJ_001", "Test memory exhaustion handling");

    pthread_attr_create (&ta);
    pthread_condattr_create (&ca);
    pthread_mutexattr_create (&ma);
    handle_chain = (handles_t *)malloc (sizeof (handles_t));
    last_chain = handle_chain;
    curblk = handle_chain;
    curblk->link = (handles_t *)0;
    curblk->count = 0;

    cts_comment ("Part 1: test limits of thread creation");

    TRY {
	while (1) {
	    pthread_create (
		    &curblk->handles[curblk->count],
		    ta,
		    (pthread_startroutine_t)null,
		    (pthread_addr_t)0);
	    thdcnt++;

	    if (++curblk->count >= MAXHAND)
		curblk = new_block ();

	    }

	}
    CATCH (exc_insfmem_e) {
	}
    CATCH_ALL {
	cts_failed ("unexpected exception in thread creation:");
	exc_report (THIS_CATCH);
	}
    ENDTRY

    cts_comment ("created %d threads", thdcnt);
    curblk = handle_chain;

    while (curblk != (handles_t *)0) {

	for (i = 0; i < curblk->count; i++) {
	    pthread_join (curblk->handles[i], (pthread_addr_t *)0);
	    pthread_detach (&curblk->handles[i]);
	    }

	curblk->count = 0;
	curblk = curblk->link;
	}

    pthread_attr_delete (&ta);		/* Make sure threads are gone */

    /*
     * Now, create another thread, to make sure that we didn't lose all the
     * space we freed by deleting things.
     */
    TRY {
	pthread_create (
		&thread,
		pthread_attr_default,
		(pthread_startroutine_t)null,
		(pthread_addr_t)0);
	pthread_join (thread, (pthread_addr_t *)0);
	pthread_detach (&thread);
	}
    CATCH (exc_insfmem_e) {
	cts_failed ("detaching threads did not free space");
	}
    CATCH_ALL {
	cts_failed ("unexpected exception on thread create:");
	exc_report (THIS_CATCH);
	}
    ENDTRY

    cts_comment ("Part 2: test limits of condition creation");
    curblk = handle_chain;

    TRY {
	while (1) {
	    pthread_cond_init (
		    &curblk->handles[curblk->count],
		    ca);
	    cvcnt++;

	    if (++curblk->count >= MAXHAND) {

		if (curblk->link)
		    curblk = curblk->link;
		else
		    curblk = new_block ();

		}

	    }
	}
    CATCH (exc_insfmem_e) {
	}
    CATCH_ALL {
	cts_failed ("unexpected exception in thread creation:");
	exc_report (THIS_CATCH);
	}
    ENDTRY

    cts_comment ("created %d condition variables", cvcnt);
    curblk = handle_chain;

    while (curblk != (handles_t *)0) {

	for (i = 0; i < curblk->count; i++)
	    pthread_cond_destroy (&curblk->handles[i]);

	curblk->count = 0;
	curblk = curblk->link;
	}

    pthread_condattr_delete (&ca);	/* Make sure cvs are gone */

    /*
     * Now, create another condition variable, to make sure that we didn't
     * lose all the space we freed by deleting things.
     */
    TRY {
	pthread_cond_init (&cond, pthread_condattr_default);
	pthread_cond_destroy (&cond);
	}
    CATCH (exc_insfmem_e) {
	cts_failed ("destroying cvs did not free space");
	}
    CATCH_ALL {
	cts_failed ("unexpected exception on cv init:");
	exc_report (THIS_CATCH);
	}
    ENDTRY

    cts_comment ("Part 3: test limits of mutex creation");
    curblk = handle_chain;

    TRY {
	while (1) {
	    pthread_mutex_init (
		    &curblk->handles[curblk->count],
		    ma);
	    mcnt++;

	    if (++curblk->count >= MAXHAND) {

		if (curblk->link)
		    curblk = curblk->link;
		else
		    curblk = new_block ();

		}

	    }
	}
    CATCH (exc_insfmem_e) {
	}
    CATCH_ALL {
	cts_failed ("unexpected exception in thread creation:");
	exc_report (THIS_CATCH);
	}
    ENDTRY

    cts_comment ("created %d mutexes", mcnt);
    curblk = handle_chain;

    while (curblk != (handles_t *)0) {

	for (i = 0; i < curblk->count; i++)
	    pthread_mutex_destroy (&curblk->handles[i]);

	curblk->count = 0;
	curblk = curblk->link;
	}

    pthread_mutexattr_delete (&ma);	/* Make sure mutexes are gone */

    /*
     * Now, create another mutex, to make sure that we didn't lose all the
     * space we freed by deleting things.
     */
    TRY {
	pthread_mutex_init (&mu, pthread_mutexattr_default);
	pthread_mutex_lock (&mu);
	pthread_mutex_unlock (&mu);
	pthread_mutex_destroy (&mu);
	}
    CATCH (exc_insfmem_e) {
	cts_failed ("destroying mutexes did not free space");
	}
    CATCH_ALL {
	cts_failed ("unexpected exception on mutex init:");
	exc_report (THIS_CATCH);
	}
    ENDTRY

    cma_debug_cmd ("show -m", (char *)0);
    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element ERRB_OBJ_001.C */
/*  *1    31-OCT-1991 10:42:43 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element ERRB_OBJ_001.C */
