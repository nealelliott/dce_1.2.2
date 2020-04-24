/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_once.c,v $
 * Revision 1.1.7.2  1996/02/17  23:34:07  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:50:17  marty]
 *
 * Revision 1.1.7.1  1995/12/07  22:04:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:06:27  root]
 * 
 * Revision 1.1.5.2  1994/06/09  13:38:21  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:14  devsrc]
 * 
 * Revision 1.1.5.1  1994/04/01  20:16:50  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:09  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:29:09  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:40:03  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:38:51  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:05:08  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:36:24  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:36:08  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:39:16  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *  Copyright (c) 1989, 1992 by
 *  Digital Equipment Corporation, Maynard Massachusetts.
 *  All rights reserved.
 *
 *  This software is furnished under a license and may be used and  copied
 *  only  in  accordance  with  the  terms  of  such  license and with the
 *  inclusion of the above copyright notice.  This software or  any  other
 *  copies  thereof may not be provided or otherwise made available to any
 *  other person.  No title to and ownership of  the  software  is  hereby
 *  transferred.
 *
 *  The information in this software is subject to change  without  notice
 *  and  should  not  be  construed  as  a commitment by DIGITAL Equipment
 *  Corporation.
 *
 *  DIGITAL assumes no responsibility for the use or  reliability  of  its
 *  software on equipment which is not supplied by DIGITAL.
 */

/*
 *  FACILITY:
 *
 *	Concert Multithread Architecture (CMA) services
 *
 *  ABSTRACT:
 *
 *	Support client one-time initialization
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	19 July 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	31 August 1989
 *		Fix calls which ought to pass handle addresses.
 *	002	Dave Butenhof	12 October 1989
 *		Convert to use internal mutex operations.
 *	003	Dave Butenhof	1 December 1989
 *		Modify external entries to track POSIX changes to names and
 *		argument ordering.
 *	004	Dave Butenhof	11 April 1990
 *		Catch possible errors from cma__alloc_mem while mutex is
 *		held, and unlock before reraising exception.
 *	005	Dave Butenhof	5 October 1990
 *		Add argument to client init routine!
 *	017	Dave Butenhof	25 October 1990
 *		Add name to mutexes created for one-time init.
 *	018	Dave Butenhof	14 December 1990
 *		Change cma_attributes.h to cma_attr.h (shorten length)
 *	019	Paul Curtin	21 May 1991
 *		Added a queue to hold once block mutexes.
 *	020	Paul Curtin	24 May 1991
 *		Added a cma__reinit_once routine.
 *	021	Dave Butenhof	11 February 1992
 *		A law of nature has just been changed: cma__alloc_mem now
 *		returns cma_c_null_ptr on an allocation failure, rather than
 *		raising an exception. This allows greater efficiency, since
 *		TRY is generally expensive. Anyway, apply the process of
 *		evolution: adapt or die.
 */

/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_defs.h>
#include <cma_once.h>
#include <cma_mutex.h>
#include <cma_attr.h>
#include <cma_queue.h>
#include <cma_vm.h>

/*
 *  GLOBAL DATA
 */
cma__t_queue	    cma__g_once_mutexes;

/*
 *  LOCAL DATA
 */

static cma__t_int_mutex   *cma___g_init_mutex;

/*
 * LOCAL FUNCTIONS
 */

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Initialize the control block if necessary.  Determine whether the
 *	initialization function has already been called; if not, call it.
 *
 *  FORMAL PARAMETERS:
 *
 *	init_block	control block for initialization
 *
 *	init_routine	initialization routine
 *
 *  IMPLICIT INPUTS:
 *
 *	cma___g_init_mutex	global init. control mutex
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	· initialization routine is called if necessary.
 *
 *	· a mutex is created in the control block if not already done.
 */
void
cma_once(
	cma_t_once		*init_block,
	cma_t_init_routine	init_routine,
	cma_t_address		arg)
    {
    /*
     * Cast the external init block to the internal format
     */
    cma__t_int_once *init_record = (cma__t_int_once *)init_block;
    cma__t_once_mutex	*mutex_q_element;


    /*
     * We could introduce an optimization: if the init routine has already
     * been invoked, the flag value is "cma__c_once_inited".  Since the flag
     * is set to cma__c_once_uninit by the (mandatory) static initialization
     * macro cma_dcl_once, and it can never change once it's been set to
     * cma__c_once_inited, it's safe to test this without synchronizing.
     * However, since this requires hardware read-write ordering, and
     * memory coherency, we'll avoid depending on it for the reference
     * implementation.
     */
    cma__int_lock (cma___g_init_mutex);

    /*
     * Make sure it's a valid once block... check the mbz field and the
     * flag.
     */
    if ((init_record->mbz != 0)
	    || ((int)init_record->flag < (int)cma__c_once_uninit)
	    || ((int)init_record->flag > (int)cma__c_once_inited)
	    ) {
	(void) cma__assert_warn (0, "Invalid client initialization block");
	return;
	}

    /*
     * If the control block hasn't been initialized yet (flag ==
     * cma__c_once_uninit), then lock the global init control mutex, and
     * create a mutex for this control block.  Set the flag to
     * cma__c_once_initing, which indicates the block is ready for use.
     */
    if (init_record->flag == cma__c_once_uninit) {
	init_record->mutex = cma__get_mutex (&cma__g_def_attr);

	if ((cma_t_address)init_record->mutex == cma_c_null_ptr) {
	    cma__int_unlock (cma___g_init_mutex);
	    cma__error (exc_s_insfmem);
	    }

	cma__obj_set_name (init_record->mutex, "for a one time init block");
	mutex_q_element = cma__alloc_object (cma__t_once_mutex);
	mutex_q_element->mutex = init_record->mutex;
	cma__queue_insert (&mutex_q_element->queue, &cma__g_once_mutexes);
	init_record->flag = cma__c_once_initing;
	}

    cma__int_unlock (cma___g_init_mutex);
    
    /*
     * Now lock the control block mutex, and see whether some other thread
     * has already done the initialization for us... if not, call the init
     * routine and set the flag to cma__c_once_inited (done).
     */
    cma__int_lock (init_record->mutex);
    if (init_record->flag == cma__c_once_initing) {
	/*
	 * Call the client's init routine.  If it raises an exception, unlock
	 * the control block and reraise the exception (without declaring the
	 * initialization complete).
	 */
	TRY {
	    (init_routine) (arg);
	    }
	CATCH_ALL {
	    cma__int_unlock (init_record->mutex);
	    RERAISE;
	    }
	ENDTRY
	init_record->flag = cma__c_once_inited;
	}
    cma__int_unlock (init_record->mutex);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Initialize module static data.
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	initialize static data
 */
extern void
cma__init_once(void)
    {
    cma___g_init_mutex = cma__get_mutex (&cma__g_def_attr);
    cma__obj_set_name (cma___g_init_mutex, "one time init");
    cma__queue_init (&cma__g_once_mutexes);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Perform work prior to and after fork(), depending on flag.
 *
 *  FORMAL PARAMETERS:
 *
 *	flag
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern void
cma__reinit_once(
	cma_t_integer	flag)
    {
    
    if (flag == cma__c_reinit_prefork_lock) {
	cma__int_lock(cma___g_init_mutex);
	}
    else if (flag == cma__c_reinit_postfork_unlock) {
	cma__int_unlock(cma___g_init_mutex);
	}

    }

/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ONCE.C */
/*  *5    18-FEB-1992 15:29:40 BUTENHOF "Adapt to new alloc_mem protocol" */
/*  *4    10-JUN-1991 18:22:34 SCALES "Add sccs headers for Ultrix" */
/*  *3     5-JUN-1991 16:13:40 CURTIN "fork work" */
/*  *2    14-DEC-1990 00:55:48 BUTENHOF "Change module names" */
/*  *1    12-DEC-1990 21:47:49 BUTENHOF "Client one-time init" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ONCE.C */
