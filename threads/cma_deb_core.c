/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_deb_core.c,v $
 * Revision 1.1.6.2  1996/02/17  23:33:47  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:49:58  marty]
 *
 * Revision 1.1.6.1  1995/12/07  22:00:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:05:04  root]
 * 
 * Revision 1.1.4.2  1994/06/09  13:37:37  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:35  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:15:48  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:16:33  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:26:51  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:37:06  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:35:56  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:02:24  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:09:36  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:09:19  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:38:57  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *  Copyright (c) 1990, 1992 by
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
 *	CMA services
 *
 *  ABSTRACT:
 *
 *	This module implements the core of CMA debugging services.
 *	These include the services required by the CMA_DEBUG_CLIENT.h
 * 	require file.
 *
 *  AUTHORS:
 *
 *	Bob Conti
 *
 *  CREATION DATE:
 *
 *	13 September 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Bob Conti	13 September 1990
 *		Initial creation of module
 *	002	Bob Conti	22 September 1990
 *		Fix syntax error in no-prototype preventing VAX ULTRIX build
 *	003	Bob Conti	8 October 1990
 *		Major features of SHOW TASK now working.
 *	004	Bob Conti	9 October 1990
 *		Add casts and comments.
 *	005	Bob Conti	10 October 1990
 *		Improve show_task display by showing name of null or initial
 *		thread on line 1. Also make sure start address is not the
 *		entry mask, or breakpoint setting will not work if the module
 *		is not set.
 *	006	Dave Butenhof	7 February 1991
 *		Change alert control bit names.
 *	007	Paul Curtin	6 March 1991
 *		Added cma_debug_get_sp_context routine.
 *	008	Dave Butenhof	11 March 1991
 *		Handle null symbolize function to cma__deb_show_task; just
 *		print hex value.
 *	009	Dave Butenhof	11 April 1991
 *		Use cma__vp_suspend_others and cma__vp_resume_others to
 *		disable multithreading when using VP layer
 *		(non-uniprocessor).
 *	010	Dave Butenhof	01 May 1991
 *		Add arguments to cma__bugcheck() calls.
 *	011	Paul Curtin	02 May 1991
 *		Added cma__deb_set_alert code to wake threads on CVs.
 *	012	Paul Curtin	24 May 1991
 *		Added a cma__reinit_debug routine.
 *	013	Paul Curtin	29 May 1991
 *		Added code to clear the known threads list
 *		after a fork (in reinit routine).
 *	014	Paul Curtin	06 May 1991
 *		Added a few fix-ups to satisfy cma internal qar 119
 *	015	Paul Curtin	09 December 1991
 *		Added code to allow debugging while on an alternate stack 
 *		to report stack over/under flow properly.
 *	016	Paul Curtin	13 January 1992
 *		Added Alpha DEBUG support.
 *	017	Paul Curtin	19 January 1991
 *		Changed bzero, and bcopy references to cma_util routines.
 *	018	Dave Butenhof and Webb Scales	6 February 1992
 *		Changed fork-reinit to destroy tcb's instead of just freeing
 *		their storage.
 *	019	Webb Scales	13 February 1992
 *		Replaced enter/exit kernel with non-undeferring versions.
 *	020	Dave Butenhof	12 March 1992
 *		Fix up reinit function to correct locking.
 *	021	Webb Scales	13 March 1992
 *		Parameterized scheduling policies.
 */


/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_assem.h>
#include <cma_condition.h>
#include <cma_mutex.h>
#include <cma_defs.h>
#include <cma_defer.h>
#include <cma_debug_client.h>
#include <cma_deb_core.h>
#include <cma_tcb.h>
#include <cma_stack_int.h>
#include <cma_stack.h>
#include <cma_vm.h>
#include <cma_util.h>
#include <cma_sched.h>

/*
 * GLOBAL DATA
 */

/* 
 * Variable holding the global debugging state 
 */
cma__t_debug_state	cma__g_debug_state;

/*
 * Known object queues.
 * The known thread queue is a special case; because it's referenced in code
 * which has the kernel locked, it's protected by the kernel lock instead of
 * a mutex.  cma__g_known_threads.mutex is unused, and is not initialized.
 */
cma__t_known_object	cma__g_known_atts;
cma__t_known_object	cma__g_known_cvs;
cma__t_known_object	cma__g_known_mutexes;
cma__t_known_object	cma__g_known_threads;


/*
 * LOCAL DATA
 */
static char		deb_buf[cma__c_buffer_size];

/*
 * LOCAL TYPES
 */

/*FIX-ME* Need to use sizes that are platform specific */
typedef long int cma___t_saved_ctx[15];	    /* Mach. state in context switch */

/*
 * LOCAL MACROS
 */

#define cma___write_buffer(value)   \
    *((cma_t_integer *)buffer) = (cma_t_integer)(value)

/*
 * This macro calls the debugger function that symbolizes an address
 * (for a given address, display its symbolic value (if available), hex
 * if not.
 */
#define cma___putsymbol(output, the_address)			\
    {								\
    cma__t_eol_routine  prev_routine;				\
    cma__t_eol_routine	dummy_routine;				\
								\
    if (print_string_noline != (cma__t_eol_routine)cma_c_null_ptr) { \
	cma__set_eol_routine (print_string_noline, &prev_routine); \
	cma__puteol (output);					\
	cma__set_eol_routine (prev_routine, &dummy_routine);	\
	}							\
    if (print_symbol == (cma__t_print_symbol)cma_c_null_ptr)	\
	cma__puthex (output, (cma_t_integer)(the_address));	\
    else							\
	print_symbol ((cma_t_address)(the_address));		\
    }

/*
 * Set to enable debugging printouts while being called by the debugger.
 */
#define CMA_DEBUG_PRINTOUT  0

#if CMA_DEBUG_PRINTOUT
#define cma___debputhex(output, val)	    cma__puthex(output,val)
#define cma___debputstring(output, val)	    cma__putstring(output,val)
#define cma___debputint(output, val)	    cma__putint(output,val)
#define cma___debputeol(output)		    cma__puteol(output)
#else
#define cma___debputhex(output, val)			
#define cma___debputstring(output, val)	
#define cma___debputint(output, val)			
#define cma___debputeol(output)			
#endif


/*
 * LOCAL FUNCTIONS
 */

/*
 * Function to tell the CMA thread dispatcher that the debugger
 * made its database inconsistent.
 */
extern void cma__deb_inconsistency (void);

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 * 	cma_debug_register is called by a CMA "debug client" to register
 * 	with CMA.  Registration means that that facility has been
 *	made known to CMA, and allows it to intercept
 *	thread-related debugging actions.
 *
 *  FORMAL PARAMETERS:
 *
 *	cma_debug_entry		- Client's entry point
 *
 *	client_key		- Client's CMA context key under which
 *				  the client agrees to store their
 *				  own "task control block" that is layered 
 *				  upon our thread.
 *
 *	client_facility		- Client's CMA facility code
 *
 *	client_has_tcb_prolog	- True if Client uses our standard prolog
 *				  for their TCB
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	The table of registered debug clients is modified.
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma_debug_register (
	cma_t_address		client_debug_entry, 
	cma_t_key		client_key, 
	cma_t_integer		client_facility,
	cma_t_boolean		client_has_tcb_prolog
	)
    {
    cma_t_integer   next;


    /* Lock the debug database */
    cma__int_lock (cma__g_debug_state.mutex);

    /* 
     * Do bounds check to ensure we don't overflow the table 
     */
    next = cma__g_debug_state.client_count;
    if (next + 1 > cma__c_deb_max_clients) 
	cma__bugcheck ("debug_register:1");

    /* 
     * Update the tables holding info on the registered clients
     */
    next    = next + 1;
    cma__g_debug_state.clients[next].entry	= client_debug_entry;
    cma__g_debug_state.clients[next].key	= client_key;
    cma__g_debug_state.clients[next].fac	= client_facility;
    cma__g_debug_state.clients[next].has_prolog	= client_has_tcb_prolog;
    cma__g_debug_state.client_count		= next;

    /* Unlock the debug database */
    cma__int_unlock (cma__g_debug_state.mutex);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 * 	cma_debug_get is called by a CMA "debug client" to get 
 * 	information about a thread while they are being called
 *	from debug context to do debugging of their tasks (which are 
 * 	layered on our threads).
 *
 * 	CAVEAT: The debugger is invoked at arbitrary points in time,
 *	so no operations executed from this context can take out a lock
 *	or modify the dispatcher database.  Essentially, the debugger
 *	uses a new, different lock than all other locks: it stops all tasks
 *	as an exclusive lock on all of process memory.
 *
 *  FORMAL PARAMETERS:
 *
 *	thread		- The thread about which information is requested
 *	item_requested 	- The item code for the information desired
 *	buffer		- The buffer to receive the results (usually integer)
 *	buffer_size	- The size of the buffer (in bytes)
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma_debug_get (
	cma_t_thread		*thread, 
	cma_t_debug_get		item_requested,
	cma_t_address		buffer,
	cma_t_integer		buffer_size
	)
    {
    cma__t_int_tcb  *tcb;

    /* 
     * Validate the handle, insure that it's a thread, and get the TCB address
     */
    tcb = cma__validate_null_tcb (thread);

    /*
     * If thread is a null handle, do nothing and return
     */
    if (tcb == (cma__t_int_tcb *) cma_c_null_ptr) return;

    /*
     * Call the internal routine that expects a valid tcb pointer
     */
    cma__deb_get (tcb, item_requested, buffer, buffer_size, 0);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_debug_get_sp_context takes in an sp and returns 
 *	the requested thread specific context.
 *
 *  FORMAL PARAMETERS:
 *
 *	sp - stack pointer
 *	key - specific thread context that is desired
 *	context - desired context.
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma_debug_get_sp_context (
	cma_t_address	    sp,
	cma_t_key	    key,
	cma_t_address	   *context
	)
    {
    cma__t_int_tcb	    *tcb;

    tcb = cma__get_sp_tcb (sp);

    /*
     * If the the thread has non-null context associated with the key,
     * fetch the value, else return null.
     */
    if ((key >= 1) && (key <= tcb->context_count)) {
        *context = tcb->contexts[key];
        }
    else
        *context = cma_c_null_ptr;

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 * 	cma_debug_set is called by a CMA "debug client" to change
 * 	the state of a thread while the client is being called
 *	from debug context to do debugging of their tasks (which are 
 * 	layered on our threads).
 *
 * 	CAVEAT: The debugger is invoked at arbitrary points in time,
 *	so no operations executed from this context can take out a lock
 *	or modify the dispatcher database.  Essentially, the debugger
 *	uses a new and different lock than all other locks: by stopping
 *	all tasks, it creates an exclusive lock on all of process memory.
 *
 *  FORMAL PARAMETERS:
 *
 *	thread		- The thread whose state is to be changed
 *	item_to_be_set	- The item code for the state to be changed
 *	buffer		- The buffer holding the new value for state
 *	buffer_size	- The size of the buffer (in bytes)
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma_debug_set (
	cma_t_thread		*thread, 
	cma_t_debug_set		item_to_be_set,
	cma_t_address		buffer,
	cma_t_integer		buffer_size
	)
    {
    cma__t_int_tcb  *tcb;

    /* 
     * Validate the handle, insure that it's a thread, and get the TCB address
     */
    tcb = cma__validate_null_tcb (thread);

    /*
     * If thread is a null handle, do nothing and return
     */
    if (tcb == (cma__t_int_tcb *) cma_c_null_ptr) return;

    /*
     * Call the internal routine that expects a valid tcb pointer
     */
    cma__deb_set (tcb, item_to_be_set, buffer, buffer_size);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 * 	cma__deb_anytcb_to_tcb is called to obtain the address of the CMA
 *	TCB from any TCB for any language.  We are assuming that
 *	the language's TCB has the standard prolog we require for debugger
 *	support; and we assume this standard prolog is at a fixed offset.
 *
 *  FORMAL PARAMETERS:
 *
 *	anytcb	    in	    Pointer to any tcb (for example, an Ada task)
 *	cmatcb	    out	    Address of the pointer to be set to point to 
 *			    a CMA TCB
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma__deb_anytcb_to_tcb(
	cma_t_tcb_header    *anytcb,
	cma__t_int_tcb	    **cmatcb
	)
    {
    cma_t_tcb_sentinel  sentinel;
    cma_t_thread	*thread;

    /*
     * Return null pointer for input null pointer
     */
    if (anytcb == (cma_t_tcb_header *)cma_c_null_ptr) {
	*cmatcb = (cma__t_int_tcb *)cma_c_null_ptr;
	return;
	}

    deb_buf[0] = '\0';
    cma___debputstring (deb_buf, "ANYTCB_TO_TCB called. ");
    cma___debputstring (deb_buf, "tcb = ");
    cma___debputint (deb_buf, (cma_t_integer)anytcb);
    cma___debputeol (deb_buf);

    /* 
     * Non-null pointer. If the sentinel is valid, and the thread is valid, 
     * return a pointer to the CMA TCB. Otherwise, return a null pointer.
     */
    sentinel = anytcb->prolog.sentinel;

    cma___debputstring (deb_buf, "sentinel = ");
    cma___debputhex (deb_buf, (cma_t_integer)sentinel);
    cma___debputeol (deb_buf);

    /* Dump the first few cells of the TCB */
    cma___debputstring (deb_buf, " ");
    cma___debputhex (deb_buf, *((cma_t_integer *)anytcb));
    cma___debputstring (deb_buf, " ");
    cma___debputhex (deb_buf, *((cma_t_integer *)(anytcb+4)));
    cma___debputstring (deb_buf, " ");
    cma___debputhex (deb_buf, *((cma_t_integer *)(anytcb+8)));
    cma___debputstring (deb_buf, " ");
    cma___debputhex (deb_buf, *((cma_t_integer *)(anytcb+16)));
    cma___debputstring (deb_buf, " ");
    cma___debputhex (deb_buf, *((cma_t_integer *)(anytcb+20)));
    cma___debputstring (deb_buf, " ");
    cma___debputhex (deb_buf, *((cma_t_integer *)(anytcb+24)));
    cma___debputstring (deb_buf, " ");
    cma___debputhex (deb_buf, *((cma_t_integer *)(anytcb+28)));
    cma___debputstring (deb_buf, " ");
    cma___debputhex (deb_buf, *((cma_t_integer *)(anytcb+32)));

    if (sentinel == cma_c_tcb_sentinel) {
	thread = (cma_t_thread *)&(anytcb->prolog.client_thread);
	*cmatcb = cma__validate_null_tcb (thread);
	}    
    else
	*cmatcb = (cma__t_int_tcb *)cma_c_null_ptr;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 * 	cma__deb_fac_to_client is called by the CMA debugger to retrieve 
 *	information about a registered CMA client.  The input is the client 
 *	facility, and the output is the client context key.
 *
 *  FORMAL PARAMETERS:
 *
 *	client_facility	    in	- Client's CMA facility code
 *
 *	client_key	    out	- Client's CMA context key under which
 *				  the client agrees to store their
 *				  own "task control block" that is layered 
 *				  upon our thread.
 *
 *  IMPLICIT INPUTS:
 *
 *	The table of registered debug clients is searched.
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma__deb_fac_to_client(
	cma_t_integer		client_facility,
	cma_t_key		*client_key
	)
    {
    cma_t_integer   next;


    for (next = 1; next <= cma__g_debug_state.client_count; next++) {
	if (cma__g_debug_state.clients[next].fac == client_facility) {
	    *client_key	    = cma__g_debug_state.clients[next].key;
	    return;
	    }
	}
    *client_key	= (cma_t_key)0;
    return;

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 * 	cma__deb_get_client_info is called by the CMA debugger to retrieve 
 *	information about a registered CMA client.  The input is the client 
 *	key, and the outputs are the other registration information.
 *
 *  FORMAL PARAMETERS:
 *
 *	client_key		- Client's CMA context key under which
 *				the client has stored their
 *				own "task control block" that is layered 
 *				upon our thread.
 *
 *	client_entry	   	- Client's debug entry
 *
 *	client_has_tcb_prolog	- True if Client uses our standard prolog
 *				for their TCB
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	The table of registered debug clients is modified.
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma__deb_get_client_info(
	cma_t_key		client_key, 
	cma_t_address		*client_debug_entry, 
	cma_t_boolean		*client_has_tcb_prolog
	)
    {
    cma_t_integer   next;


    for (next = 1; next <= cma__g_debug_state.client_count; next++) {
	if (cma__g_debug_state.clients[next].key == client_key) {
	    *client_debug_entry	    
		= cma__g_debug_state.clients[next].entry;
	    *client_has_tcb_prolog  
		= cma__g_debug_state.clients[next].has_prolog;
	    return;
	    }
	}

    /*
     * We should not be called if there are no registered
     * clients
     */
    cma__bugcheck ("deb_get_client_info:1");
    return;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 * 	cma__deb_get is called to get debugger information about a thread.
 *	It is similar to cma_debug_get, except that the input is a tcb
 *	rather than a thread.
 *
 *  FORMAL PARAMETERS:
 *
 *	tcb		- TCB about which information is requested
 *	item_requested 	- The item code for the information desired
 *	buffer		- The buffer to receive the results (usually integer)
 *	buffer_size	- The size of the buffer (in bytes)
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma__deb_get(
	cma__t_int_tcb		*tcb, 
	cma_t_debug_get		item_requested,
	cma_t_address		buffer,
	cma_t_integer		buffer_size,
	cma_t_integer		current_sp
	)
    {
    cma__t_int_stack	*stack;		/* A stack for the Thread */
    cma__t_int_stack	*next_stack;	/* A stack for the Thread */
    cma_t_integer	not_found;	/* Flag */
    cma_t_integer	stack_limit;	/* Limit of a stack */
    cma_t_integer	stack_start;	/* Start of a stack */



    /* Find the active thread stack or null */

    if (cma__queue_empty (&tcb->stack)) 
	stack = (cma__t_int_stack *)cma_c_null_ptr;
    else 
	{
	stack = (cma__t_int_stack *)cma__queue_next (&tcb->stack);
	next_stack = stack;
	
	/*
	 * If the information requested is stack specific; meaning that it
	 * might vary if it is part of a stack not originally created with
	 * the thread BUT assigned to that thread at a later point than the
	 * CURRENT_SP will be passed in.  It is passed in because the value
	 * might come directly from the (VAX) debugger... so we don't assume
	 * we have access to all cases of the SP.  We take current_sp and 
	 * look amoung all stacks that belong to a thread to see which it
	 * might belong.  We use the resultant stack to get information.
	 */
	not_found = 1;
	
	if (current_sp != 0)
	    do {
#ifdef _CMA_UPSTACK_
		stack_limit = (int)next_stack->yellow_zone + cma__c_yellow_size;
		stack_start = (int)next_stack->stack_base;
		if ((current_sp >= stack_start) && (current_sp < stack_limit)) {
		    stack = next_stack;
		    not_found = 0;
		    }
#else
		stack_limit = (int)next_stack->yellow_zone - cma__c_yellow_size;
		stack_start = (int)next_stack->stack_base;
		if ((current_sp <= stack_start) && (current_sp > stack_limit)) {
		    stack = next_stack;
		    not_found = 0;
		    }
#endif
		next_stack = (cma__t_int_stack *)cma__queue_next
			((cma__t_queue *)(&next_stack->header.queue));
	    } while ((next_stack != stack) && (not_found));
	}

    switch ((int)item_requested) {

	case (int)cma_c_debget_guardsize:
	    {
	    int stack_dir;

	    if (stack == (cma__t_int_stack *)cma_c_null_ptr) {
		cma___write_buffer (0);
		}
	    else {
		if (stack->stack_base > stack->last_guard)
		    stack_dir = -1;
		else
		    stack_dir = 1;
		
		if (tcb->kind == cma__c_thkind_initial) {
		    cma___write_buffer (0);
		    }
		else {
		    cma___write_buffer (
			stack_dir*(
			((cma_t_integer)(stack->last_guard) - 
			 (cma_t_integer)(stack->yellow_zone)) + 1
			));
		    }
		}
	    break;
	    }
	case (int)cma_c_debget_is_held:
	    {
	    cma___write_buffer (tcb->debug.on_hold);
	    break;
	    }
	case (int)cma_c_debget_is_initial:
	    {
	    cma___write_buffer (tcb->kind == cma__c_thkind_initial);
	    break;
	    }
	case (int)cma_c_debget_number:
	    {
	    /* FIX-ME probably should make sequence a full int */
	    cma___write_buffer (tcb->header.sequence);
	    break;
	    }
	case (int)cma_c_debget_stack_ptr:
	    {
	    cma___write_buffer (tcb->static_ctx.sp);
	    break;
	    }
	case (int)cma_c_debget_stack_base:
	    {
	    if (stack == (cma__t_int_stack *)cma_c_null_ptr) {
		/*FIX-ME* Default stack base is platform specific */
		cma___write_buffer (0);
		}
	    else if (tcb->kind == cma__c_thkind_initial) {
		cma___write_buffer (cma__c_def_stack_max);
		}
	    else {
		cma___write_buffer (stack->stack_base);
		}
	    break;
	    }
	case (int)cma_c_debget_stack_top:
	    {
	    if (stack == (cma__t_int_stack *)cma_c_null_ptr) {
		/*FIX-ME* Default stack top is platform specific */
		cma___write_buffer (0);
		}
	    else if (tcb->kind == cma__c_thkind_initial) {
		cma___write_buffer (cma__c_def_stack_min);
		}
	    else {
		cma___write_buffer (stack->last_guard);
		}
	    break;
	    }
	case (int)cma_c_debget_sched_state:
	    {
	    cma___write_buffer (tcb->state);
	    break;
	    }
	case (int)cma_c_debget_reserve_size:
	    {
	    /* FIX-ME Move that constant to cma_stack.h */
	    cma___write_buffer (cma___c_reserve_size);
	    break;
	    }

	case (int)cma_c_debget_base_prio:
	    {
	    /* FIX-ME may need to add a tcb field for base priority */
	    cma___write_buffer (0);
	    break;
	    }
	case (int)cma_c_debget_priority:
	    {
	    cma___write_buffer (tcb->sched.priority);
	    break;
	    }
	case (int)cma_c_debget_regs:
	    {
#if _CMA_PLATFORM_ == _CMA__VAX_VMS
	    cma___t_debug_ctx	*debstate;
	    cma___t_saved_ctx	*frame;
	    int			i;

	    /* Debug ctx is r0-r11,ap,fp,sp,pc,psl */
	    /* Saved ctx is handler, maskpsw, ap,fp,pc,r2-r11 */

	    debstate	= (cma___t_debug_ctx *)buffer;
	    frame	= (cma___t_saved_ctx *)tcb->static_ctx.fp;

	    /* 
	     * Move the machine state from the dispatcher's format
	     * to the debugger's format.
	     */
	    (*debstate)[0]	= 0;		    /* R0	*/
	    (*debstate)[1]	= 0;		    /* R1	*/
	    for (i = 2; i <= 11; i++)		    /* R2-R11	*/
		(*debstate)[i] = (*frame)[i+3];
	    (*debstate)[12] = (*frame)[2];	    /* AP	 */
	    (*debstate)[13] = tcb->static_ctx.fp;   /* FP	 */
	    (*debstate)[14] = tcb->static_ctx.sp;   /* SP	 */
	    (*debstate)[15] = (*frame)[4];	    /* PC	 */
	    (*debstate)[16] = cma__c_default_ps;    /* Def. PSL	 */
	    *(unsigned short *)&((*debstate)[16]) =   /* Move PSW  */
		*(unsigned short *)&((*frame)[1]);
#else
# if _CMA_PLATFORM_ == _CMA__ALPHA_VMS
	    cma_t_address	*debstate;
	    cma_t_address	*frame;
	    int			i;

	    /*
	     *	The registers stored in a DECthreads frame need to be 
	     *	transferred to the DEBUG supplied buffer.  On Alpha, DECthreads
	     *	stores R26, R2...R15, F2...F9, and R29 in it's frame - in that 
	     *	order.  DEBUG expects all the general-purpose registers (32 
	     *	quadwords, R0 through R31), the floating registers (32 
	     *	quadwords, F0 through F31), the PC (1 quadword) and PS (1 
	     *	quadword).
	     *
	     *	Values which DECthread does not preserve are zeroed.
	     *
	     */

	    debstate	= buffer;
	    frame	= (cma_t_address *)tcb->static_ctx.sp;

	    /* 
	     * Move the machine state from the dispatcher's format
	     * to the debugger's format.
	     */

	    cma__memset ((char *)debstate, 0, (66*8));	/* Zero structure */

	    /* Copy R2 thru R15 */
	    cma__memcpy ((cma_t_address)((cma_t_integer)(frame)+(1*8)),
		   (cma_t_address)((cma_t_integer)(debstate)+(2*8)),
		   (14*8));

	    /* copy R29 to debstate */
	    cma__memcpy ((cma_t_address)((cma_t_integer)(frame)+(22*8)),
		   (cma_t_address)((cma_t_integer)(debstate)+(29*8)),
		   (1*8));

	    /* copy F2 thru F9 */
	    cma__memcpy ((cma_t_address)((cma_t_integer)(frame)+(14*8)),
		   (cma_t_address)((cma_t_integer)(debstate)+(34*8)),
		   (8*8));
	    
	    /* copy PC to debstate */
/*	    cma__memcpy ((cma_t_address)(&tcb->static_ctx.sp),
 *		   (cma_t_address)((cma_t_integer)(debstate)+(64*8)),
 *		   (1*8));
 */

	    /* copy PS to debstate */
/*	    cma__memcpy ((cma_t_address)(&tcb->static_ctx.sp),
 *		   (cma_t_address)((cma_t_integer)(debstate)+(65*8)),
 *		   (1*8));
 */

# else
	    cma___write_buffer (0);
# endif
#endif
	    /*
	     * FIX-ME* may want to add this for other platforms, possibly
	     * should move this to the assembler
	     */
	    break;
	    }
	case (int)cma_c_debget_alt_pending:
	    {
	    cma___write_buffer (tcb->alert.pending);
	    break;
	    }
	case (int)cma_c_debget_alt_a_enable:
	    {
	    cma___write_buffer (tcb->alert.a_enable);
	    break;
	    }
	case (int)cma_c_debget_alt_g_enable:
	    {
	    cma___write_buffer (tcb->alert.g_enable);
	    break;
	    }
	case (int)cma_c_debget_substate:
	    {

	    /* 
	     * For some cases (usually blocking substates), we decide the 
	     * substate by looking at hints about what the thread is waiting 
	     * for (this saves adding debug-only instructions).
	     * For the ready or run substates, we read the field in the TCB
	     * (those field assignments are MUCH less frequent).
	     */
	    if (tcb->wait_cv != (cma__t_int_cv *)cma_c_null_ptr) {
		/* 
		 * Suspended on some c.v., look for finer details.
		 */
		if (tcb->wait_cv == cma__g_delay_cv)
		    cma___write_buffer (cma__c_substa_delay);
		else
		    cma___write_buffer (cma__c_substa_cv);
		}
		/*FIX-ME* figure out how to return cma__c_substa_timed_cv */

	    else if (tcb->wait_mutex != (cma__t_int_mutex *)cma_c_null_ptr) {
		/* 
		 * Suspended on some mutex 
		 */
		cma___write_buffer (cma__c_substa_mutex);
		}

	    else
		/* 
		 * Ready or running: just read the substate
		 */
		cma___write_buffer (tcb->debug.substate);

	    break;
	    }
	case (int)cma_c_debget_object_addr:
	    {
	    cma___write_buffer (tcb->debug.object_addr);
	    break;
	    }
	case (int)cma_c_debget_thkind:
	    {
	    cma___write_buffer (tcb->kind);
	    break;
	    }
	case (int)cma_c_debget_detached:
	    {
	    cma___write_buffer (tcb->detached);
	    break;
	    }
	case (int)cma_c_debget_tcb_size:
	    {
	    /*FIX-ME* add in the size of thread context blocks */
	    cma___write_buffer (sizeof(cma__t_int_tcb));
	    break;
	    }
	case (int)cma_c_debget_start_pc:
	    {
	    cma___write_buffer (tcb->debug.start_pc);
	    break;
	    }
	case (int)cma_c_debget_next_pc:
	    {
	    if (tcb->state == cma__c_state_terminated)
		cma___write_buffer (0);
	    else {
#if _CMA_PLATFORM_ == _CMA__VAX_VMS
		cma___t_saved_ctx	*frame;

		frame	= (cma___t_saved_ctx *)tcb->static_ctx.fp;
		cma___write_buffer ((*frame)[4]);
#else
		/*FIX-ME* Platform specific */
		cma___write_buffer (0);
#endif
		}

	    break;
	    }
	case (int)cma_c_debget_policy:
	    {
	    cma___write_buffer (tcb->sched.policy);
	    break;
	    }
	case (int)cma_c_debget_stack_yellow:
	    {
	    if (stack == (cma__t_int_stack *)cma_c_null_ptr) {
		/*FIX-ME* Platform specific */
		cma___write_buffer (0);
		}
	    else if (tcb->kind == cma__c_thkind_initial) {
		cma___write_buffer (0);
		}
	    else {
		cma___write_buffer (
		(cma__roundup_chunksize((cma_t_integer)(stack->yellow_zone))));
		}
	    break;
	    }
	case (int)cma_c_debget_stack_default:
	    {
	    if (stack == (cma__t_int_stack *)cma_c_null_ptr) {
		cma___write_buffer (cma_c_true);
		}
	    else {
		cma___write_buffer (cma_c_false);
		}
	    break;
	    }

	default:
	    {
	    cma__bugcheck ("deb_get:1");
	    break;
	    }
	}
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 * 	cma__deb_get_context is called by the CMA debugger to get the context
 *	value stored for some key in a thread.  This is interpreted
 *	as the "task value" that the debugger needs.
 *
 *  FORMAL PARAMETERS:
 *
 *	tcb	    in	Pointer to CMA TCB
 *	key	    in	Client context key that the value is stored under
 *	context	    out	Address of buffer to get the value
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma__deb_get_context(
	cma__t_int_tcb	    *tcb,
	cma_t_key	    key,
	cma_t_address	    *context
	)
    {

    /*
     * If the the thread has non-null context associated with the key,
     * fetch the value, else return null.
     */
    if ((key >= 1) && (key <= tcb->context_count)) {
	*context = tcb->contexts[key];
	}
    else
	*context = cma_c_null_ptr;

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 * 	cma__deb_get_self_tcb is called by the CMA debugger to get the address
 *	of the currently active thread's TCB.  (This version of get_self
 *	exists so that we can avoid locking on this path or take advantage
 *	of the debugger's "memory lock" which results from stopping all
 *	threads).
 *
 *  FORMAL PARAMETERS:
 *
 *	None	    (yet... will need stack address as input eventually)
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	Address of current thread's CMA tcb
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern cma__t_int_tcb *
cma__deb_get_self_tcb(void)
    {

    /* 
     * FIX-ME* This needs to get the active sp from the debugger 
     * for true parallel mode, as there is no way to compute this
     * without having an idea of what stack the thread was on while
     * running in debug context.
     */
    return (cma__get_self_tcb());

    }

/*
 * FIX-ME: this is a stub
 */
extern void
cma__deb_get_time_slice(
	cma_t_interval	*current_setting
	)
    {
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 * 	cma__deb_next_tcb is called by the CMA debugger to get the 
 *	tcb of the "next" tcb in the known-threads list.
 *
 *	The next thread matching ALL the SPECIFIED selection criteria
 *	is obtained.  If no such matching thread is found, null is returned.
 *	Any thread may be specified as the "starter" thread; or, a null
 *	can be passed in as the "starter".
 *
 *  FORMAL PARAMETERS:
 *
 *	tcb		- Starter TCB or null
 *
 *	pri_mask_ptr	- Ptr to a mask with bits set for each priority
 *			  of interest; null ptr means "any priority"
 *
 *	state_mask_ptr	- Ptr to a mask with a bit set for each state
 *			  of interest; null ptr means "any state"
 *
 *	hold_mask_ptr	- Ptr to a mask of 1 but set for the HOLD/NOHOLD
 *			  of interest; null ptr means "on HOLD or NOT".
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	Address of TCB of the thread matching the selection criteria
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern	cma__t_int_tcb	    *
cma__deb_next_tcb(
	cma__t_int_tcb	    *tcb,
	cma_t_integer	    *pri_mask_ptr,
	cma_t_integer	    *state_mask_ptr,
	cma_t_boolean	    *hold_ptr
	)
    {
    cma__t_int_tcb	*candidate;
    cma__t_int_tcb	*next_tcb;
    cma__t_queue	*ptr;
    cma__t_queue	*start_ptr;

    if (tcb == (cma__t_int_tcb *)cma_c_null_ptr)
	ptr = &cma__g_known_threads.queue;	
    else
	ptr = &(tcb->threads);

    /* Set the default next thread to be null */
    next_tcb	= (cma__t_int_tcb *)cma_c_null_ptr;

    /* Return immediately if no threads currently exist */
    if (ptr->flink == (cma__t_queue *)cma_c_null_ptr) 
	return (next_tcb);

    /* 
     * One or more threads exist, so find the "next" one in the list 
     * that satisfies the selection criteria.
     */
    start_ptr	= ptr;
    do {

	/* Find next ITEM in list after current item */
	ptr = cma__queue_next (ptr);

	/* Skip over the list header */
	if (ptr != &cma__g_known_threads.queue) {

	    /* 
	     * Not the header, must be a TCB, find TCB base address of 
	     * candidate 
	     */
	    candidate = cma__base (ptr, threads, cma__t_int_tcb);

	    /*
	     * Evaluate each requested selection criteria here.
	     * The candidate must match all criteria that are specified.
	     * Check if the candidate's HOLD state matches the request,
	     * if the candidate's state is one of the states the 
	     * user is interested in, and if it's priority is of interest.
	     */
	    if (hold_ptr != (cma_t_boolean *)cma_c_null_ptr)
		if (candidate->debug.on_hold != *hold_ptr)
		    continue;

	    if (state_mask_ptr != (cma_t_integer *)cma_c_null_ptr)
		if ( !((*state_mask_ptr) & 
			(1 << (cma_t_integer)candidate->state)) )
		    continue;

	    if (pri_mask_ptr != (cma_t_integer *)cma_c_null_ptr)
		if ( !((*pri_mask_ptr) & 
			(1 << (cma_t_integer)candidate->sched.priority)) )
		    continue;

	    next_tcb    = candidate;
	    break;

	    }
	} while (ptr != start_ptr);

    return (next_tcb);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 * 	cma__deb_set is called to change the state of a thread 
 *	while in the debugger.
 *
 * 	CAVEAT: The debugger is invoked at arbitrary points in time,
 *	so no operations executed from this context can take out a lock
 *	or modify the dispatcher database.  Essentially, the debugger
 *	uses a new and different lock than all other locks: by stopping
 *	all tasks, it creates an exclusive lock on all of process memory.
 *
 *  FORMAL PARAMETERS:
 *
 *	tcb		- TCB whose state is to be changed
 *	item_to_be_set	- The item code for the state to be changed
 *	buffer		- The buffer holding the new value for state
 *	buffer_size	- The size of the buffer (in bytes)
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma__deb_set(
	cma__t_int_tcb		*tcb, 
	cma_t_debug_set		item_to_be_set,
	cma_t_address		buffer,
	cma_t_integer		buffer_size
	)
    {

    /*
     * NOTE: nolocking can be used here because this is called from
     * debugger context.  The "inconsistency mechanism" will cause
     * the dispatcher to fix up any inconsistencies in its database.
     */

    switch ((int)item_to_be_set) {

	case (int)cma_c_debset_priority:
	    {
	    /*FIX-ME  need to limit to min and max priority for policy */	
	    tcb->sched.priority = *(cma_t_priority *)buffer;
	    break;
	    }

	case (int)cma_c_debset_policy:
	    {
	    tcb->sched.policy = *(cma_t_sched_policy *)buffer;
	    cma__sched_parameterize (tcb, tcb->sched.policy);
	    /*
	     * FIX-ME:  need to set priority to reasonable value for new policy 
	     */
	    break;
	    }

	case (int)cma_c_debset_hold:
	    {
	    /* Allow hold to be manipulated for all but the null threads */
	    if (tcb->kind != cma__c_thkind_null)
		tcb->debug.on_hold = *(cma_t_boolean *)buffer;
	    break;
	    }

	case (int)cma_c_debset_regs:
	    {
	    if (tcb->state != cma__c_state_terminated) {
#if _CMA_PLATFORM_ == _CMA__VAX_VMS
		cma___t_debug_ctx	*debstate;
		cma___t_saved_ctx	*frame;
		int			i;

		/* Debug ctx is r0-r11,ap,fp,sp,pc,psl */
		/* Saved ctx is handler, maskpsw, ap,fp,pc,r2-r11 */

		debstate    = (cma___t_debug_ctx *)buffer;
		frame	    = (cma___t_saved_ctx *)tcb->static_ctx.fp;

		/* R0 and R1 are meaningless as they are dirtied */
		for (i = 2; i <= 11; i++)		    /* R2-R11	*/
		    (*frame)[i+3]	= (*debstate)[i];
		(*frame)[2] = (*debstate)[12];		    /* AP	 */
		/* SP and FP can't be changed */
		(*frame)[4] = (*debstate)[15];		    /* PC	 */
		*(unsigned short *)&((*frame)[1]) =	    /* Move PSW  */
		    *(unsigned short *)&((*debstate)[16]);
#else
# if _CMA_PLATFORM_ == _CMA__ALPHA_VMS
	    cma_t_address	*debstate;
	    cma_t_address	*frame;
	    int			i;

	    /*
	     *	Copy registers out of DEBUG buffer into our context.
	     */

	    debstate	= buffer;
	    frame	= (cma_t_address *)tcb->static_ctx.sp;

	    /* Copy R2 thru R15 */
	    cma__memcpy ((cma_t_address)((cma_t_integer)(debstate)+(2*8)),
		   (cma_t_address)((cma_t_integer)(frame)+(1*8)),
		   (14*8));

	    /* copy R29 to frame */
	    cma__memcpy ((cma_t_address)((cma_t_integer)(debstate)+(29*8)),
		   (cma_t_address)((cma_t_integer)(frame)+(22*8)),
		   (1*8));

	    /* copy F2 thru F9 */
	    cma__memcpy ((cma_t_address)((cma_t_integer)(debstate)+(34*8)),
		   (cma_t_address)((cma_t_integer)(frame)+(14*8)),
		   (8*8));
	    
	    /* copy PC to frame */
/*	    cma__memcpy ((cma_t_address)((cma_t_integer)(debstate)+(64*8)),
 *		   (cma_t_address)(&tcb->static_ctx.sp),		   
 *		   (1*8));
 */

	    /* copy PS to frame */
/*	    cma__memcpy ((cma_t_address)((cma_t_integer)(debstate)+(65*8)),
 *		   (cma_t_address)(&tcb->static_ctx.sp),
 *		   (1*8));
 */

# else
		cma___write_buffer (0);
# endif
#endif
		/*
		 * FIX-ME not implemented yet for other platforms until
		 * there is debugger support for threads there
		 */	
		 }
	    break;
	    }
	default:
	    {
	    cma__bugcheck ("deb_set:1");
	    break;
	    }
	}

    /*
     * Notify the dispatcher that something is inconsistent
     */
    cma__deb_inconsistency ();
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 * 	cma__deb_set_alert is called to alert a thread from debugger context.
 *	(This exists so that we have a way to do this without taking
 *	out a lock.)
 *
 *  FORMAL PARAMETERS:
 *
 *	tcb		- TCB address of thread to be alerted
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern cma_t_boolean
cma__deb_set_alert(
	cma__t_int_tcb	    *tcb
	)
    {

    /*
     * Set the alert_pending bit...
     * The thread will see the alert at its next alert synchronization point,
     * for example, an alert_test 
     */
    if (tcb->kind != cma__c_thkind_null) {
	tcb->alert.pending   = cma_c_true;

	/* If blocked prepare to wake all blocked on semaphore */
	if (tcb->wait_cv != (cma__t_int_cv *)cma_c_null_ptr) {
	    cma__defer (
		    cma__c_defer_wake_all,
		    (cma_t_address)&(tcb->wait_cv->semaphore),
		    cma_c_null_ptr);
	    }
	return (cma_c_true); /* Report that we were able to do it */    
	}
    else
	return (cma_c_false); /* Report that we couldn't do it */    
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 * 	cma__deb_set_next_thread is called to request a thread to be run next.
 *
 *  FORMAL PARAMETERS:
 *
 *	tcb		- TCB address of thread to be alerted
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma__deb_set_next_thread(
	cma__t_int_tcb	    *tcb
	)
    {

    cma__g_debug_state.next_to_run	= tcb;

    /*
     * Notify the dispatcher that something is inconsistent
     */
    cma__deb_inconsistency ();

    }

/*FIX-ME* Is Stub   */
extern void
cma__deb_set_force_dispatch(
	cma_t_address	    pcpsl
	)
    {
    cma_t_boolean   lock;
    cma__t_int_tcb  *cur_tcb;

    lock = cma__tryenter_kernel ();
    cur_tcb = cma__deb_get_self_tcb();

#if _CMA_HARDWARE_ == _CMA__VAX
# if _CMA_OS_ == _CMA__VMS
    if (!lock)
	cma__cause_force_dispatch (
		cur_tcb,
		(cma_t_address)cma__force_dispatch,
		&(((cma_t_integer *)pcpsl)[0]),
		&(((cma_t_integer *)pcpsl)[1])
		);
# endif
#endif

    }

/*
 * FIX-ME* Is Stub
 */
extern void
cma__deb_set_time_slice(cma_t_interval	new_setting)
    {
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 * 	cma__deb_show_thread is called to output information about a
 *	thread.
 *
 *  FORMAL PARAMETERS:
 *
 *	tcb		- TCB whose thread info is to be displayed
 *
 *	full		- True if "full display" is requested, False for 
 *			   "brief display".
 *
 *	suppress_header	- True if the column header text is to be suppressed
 *			  (this allows creation of a table with headings
 *			  only over the first thread.)
 *
 *	active_regs	- Structure of the machine state.  This is needed
 *			  on systems where the debugger creates a copy
 *			  of the machine state internally at the time of
 *			  its invocation... in other words these are the
 *			  thread's register's at the time the debugger
 *			  was invoked.
 *
 *	print_string_noline 
 *			- Debugger routine to print a null-terminated
 *			  string, but not issue a CRLF.
 *
 *	print_string_line	
 *			- Debugger routine to print a null-terminated
 *			  string, with a trailing CRLF.
 *
 *	print_symbol	
 *			- Debugger routine to print an address symbolically
 *			  with no trailing CRLF.
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma__deb_show_thread(
	cma__t_int_tcb	    *tcb,
	cma_t_boolean	    full,
	cma_t_boolean	    suppress_header,
	cma___t_debug_ctx   active_regs,
	cma__t_eol_routine  print_string_noline,
	cma__t_eol_routine  print_string_line,
	cma__t_print_symbol print_symbol
	)
    {
    cma__t_eol_routine  
	prev_routine,
	dummy_routine;

    char		
	output[cma__c_buffer_size];

/*FIX-ME* move to proper .h file */
#define cma__c_policy__dim   6

    static char
	*sched_state_name[cma__c_state__dim] = {
	"RUN  ",
	"READY",
	"SUSP ",
	"TERM ",
	},

	*substate_name[cma__c_substa__dim] = {
	"               ",
	"Mutex Wait     ",
	"Condition Wait ",
	"Timed Cond Wait",
	"Term. by alert ",
	"Term. by exc   ",
	"Delay          ",
	"Not yet started",
	},

	*thread_kind_name[cma__c_thkind__dim] = {
	"Initial thread ",
	"Normal thread  ",
	"Null thread    "
	},

	*sched_policy_name[cma__c_policy__dim] = {
	"fifo        (real-time)",
	"round-robin (real-time)",
	"throughput             ",
	"background             ",
	"ada_low                ",
	"idle                   "
	};

    cma_t_integer
	alert_g_enable,
	alert_a_enable,
	alert_pending,
	detached,
	is_held,
	next_inst_addr,
	number,
	on_default_stack,
	policy,
	pri,
	stack_base_addr,
	stack_ptr_addr,
	stack_top_addr,
	stack_yellow_addr,
	stack_bytes_available,
	stack_bytes_in_use,
	stack_direction,
	stack_guard_bytes,
	stack_reserved_bytes,
	stack_size,
	start_inst_addr,
	state,
	substate,
	tcb_size,
	terminated,
	thread_kind,
	thread_object_addr,
	total_bytes;


    cma_t_boolean 
	is_self,
	threads_exist;


    /* 
     * Specify the debugger's print routine for the duration of this.
     * (This ensures that our debugger output text will go to a
     * window that may be created and managed by the debugger, rather
     * than standard output.)
     */
    cma__set_eol_routine (print_string_line, &prev_routine);
    output[0] = '\0';

    /*
     * If getting the "next tcb after null" with no selection criteria
     * returns null, then there are NO threads.
     */
    threads_exist = 
	    ((cma__t_int_tcb *)cma_c_null_ptr != 
	    cma__deb_next_tcb (
		(cma__t_int_tcb *)cma_c_null_ptr,
		(cma_t_integer *)cma_c_null_ptr,
		(cma_t_integer *)cma_c_null_ptr,
		(cma_t_boolean *)cma_c_null_ptr
		));


    /*
     * Decide whether to print the header or not
     */
    if (threads_exist && ((!suppress_header) || full) ) {
	cma__puteol (output);	/* Skip one line before the header */
	cma__putstring (output, 
	    " task id    state hold  pri substate        thread_object");
	cma__puteol (output);
	};	

    /* 
     * If no threads exist, say so, and leave.
     */
    if (!threads_exist) {
	cma__putstring (output, " No tasks exist now");
	cma__puteol (output);
	cma__set_eol_routine (prev_routine, &dummy_routine);
	return;
	};


    /*
     * If the requested thread value is null, say so, and leave.
     */
    if (tcb == (cma__t_int_tcb *)cma_c_null_ptr) {
	cma__putstring (output, " %TASK    0       Not a task");
	cma__puteol (output);
	cma__set_eol_routine (prev_routine, &dummy_routine);
	return;
	};

    /* 
     * Output the items for a "brief" display as a single line of text
     */
    cma__deb_get (tcb, cma_c_debget_priority, (cma_t_address)&pri, 4, 0);
    cma__deb_get (tcb, cma_c_debget_policy, (cma_t_address)&policy, 4, 0);
    cma__deb_get (tcb, cma_c_debget_number, (cma_t_address)&number, 4, 0);
    cma__deb_get (tcb, cma_c_debget_is_held, (cma_t_address)&is_held, 4, 0);
    cma__deb_get (tcb, cma_c_debget_sched_state, (cma_t_address)&state, 4, 0);
    cma__deb_get (tcb, cma_c_debget_substate, (cma_t_address)&substate, 4, 0);
    cma__deb_get (tcb, cma_c_debget_object_addr, 
	(cma_t_address)&thread_object_addr, 4, 0);
    cma__deb_get (tcb, cma_c_debget_thkind, (cma_t_address)&thread_kind, 4, 0);

    is_self = (tcb == cma__deb_get_self_tcb());
    if (is_self)
	cma__putstring (output, "*");
    else
	cma__putstring (output, " ");

    cma__putstring (output, "%TASK");
    cma__putint_5 (output, number);
    cma__putstring (output, " ");
    cma__putstring (output, sched_state_name[state]);
    cma__putstring (output, " ");
    if (is_held)
	cma__putstring (output, "HOLD");
    else
	cma__putstring (output, "    ");
    cma__putint_5 (output, pri);
    cma__putstring (output, " ");
    cma__putstring (output, substate_name[substate]);
    cma__putstring (output, " ");

    if (thread_kind == (int)cma__c_thkind_normal) {
	cma___putsymbol (output, thread_object_addr);
	cma__puteol (output);
	}
    else {
	cma__putstring (output, thread_kind_name[thread_kind]);
	cma__puteol (output);
	}

    /* 
     * Output the items for a "full" display as multiple lines of text
     */
    if (full) {

#if _CMA_PLATFORM_ == _CMA__VAX_VMS
	if (is_self)
	    stack_ptr_addr = (cma_t_integer)active_regs[14];	
	else
#endif
	    cma__deb_get (tcb,  cma_c_debget_stack_ptr, 
		(cma_t_address)&stack_ptr_addr, 4, 0);


	/* 
	 * First get all the info we need on this thread from the routine that
	 * encapsulates the "secrets" of how to get it.
	 */

	cma__deb_get (tcb, cma_c_debget_stack_default, 
		(cma_t_address)&on_default_stack, 4, 0);
	if (on_default_stack) 
	    stack_size = 0;
	else {
	    /* Not on the default stack, lots if info to get... */
	    cma__deb_get (
		tcb, 
		cma_c_debget_stack_yellow, 
		(cma_t_address)&stack_yellow_addr, 
		4,
		0);
	    cma__deb_get (tcb, 
		cma_c_debget_reserve_size, 
		(cma_t_address)&stack_reserved_bytes, 
		4,
		0);
	    cma__deb_get (tcb, 
		cma_c_debget_stack_top, 
		(cma_t_address)&stack_top_addr, 
		4, 
		0);
	    cma__deb_get (tcb, 
		cma_c_debget_stack_base, 
		(cma_t_address)&stack_base_addr, 
		4, 
		0);

	    if (stack_base_addr > stack_top_addr)
		stack_direction = -1;   /* Stacks grow downward */
	    else
		stack_direction = 1;    /* Stacks grow upward */

	    stack_bytes_in_use = 
		stack_direction*(stack_ptr_addr - stack_base_addr);
	    stack_bytes_available = 
		stack_direction*(stack_yellow_addr - stack_ptr_addr);
	    stack_guard_bytes = 
		stack_direction*(stack_top_addr - stack_yellow_addr);

	    stack_size = stack_reserved_bytes + stack_guard_bytes + 
		stack_bytes_available + stack_bytes_in_use;

	}

	cma__deb_get (tcb, 
		cma_c_debget_tcb_size,   
		(cma_t_address)&tcb_size, 
		4,
		0);
	cma__deb_get (tcb, 
		cma_c_debget_start_pc,   
		(cma_t_address)&start_inst_addr, 
		4,
		0);
	cma__deb_get (tcb, 
		cma_c_debget_detached,   
		(cma_t_address)&detached, 
		4,
		0);
	cma__deb_get (tcb, 
		cma_c_debget_alt_pending,
		(cma_t_address)&alert_pending, 
		4,
		0);
	cma__deb_get (tcb, 
		cma_c_debget_alt_a_enable,
		(cma_t_address)&alert_a_enable, 
		4,
		0);
	cma__deb_get (tcb, 
		cma_c_debget_alt_g_enable,  
		(cma_t_address)&alert_g_enable, 
		4,
		0);

#if _CMA_PLATFORM_ == _CMA__VAX_VMS
	if (is_self)
	    next_inst_addr = (cma_t_integer)active_regs[15];	
	else
#endif
	    cma__deb_get (tcb, 
		cma_c_debget_next_pc, 
		(cma_t_address)&next_inst_addr, 
		4,
		0);

	terminated = state == ((cma_t_integer)cma__c_state_terminated);
	total_bytes = tcb_size + stack_size;

	/* 
	 * First emit important info related to common errors
	 */
	if (terminated && (!detached)) {
	    cma__putstring (output, "        ");
	    cma__putstring (output, "Terminated but not yet detached");
	    cma__puteol (output);
	    }

	if (alert_pending) {
	    cma__putstring (output, "        ");
	    cma__putstring (output, "Alert is pending");
	    cma__puteol (output);
	    }

	if (alert_g_enable) {
	    cma__putstring (output, "        ");
	    cma__putstring (output, "General alert delivery is enabled");
	    cma__puteol (output);
	    }

	if ((alert_a_enable) && (alert_g_enable)) {
	    cma__putstring (output, "        ");
	    cma__putstring (output, 
		"Asynchronous alert delivery is enabled");
	    cma__puteol (output);
	    }

	/* Emit the less important info */
	cma__puteol (output);

	cma__putstring (output, "        ");
	cma__putstring (output, "Next pc:           ");

	if (next_inst_addr == 0)
	    cma__putstring (output, "(unknown)");
	else
	    cma___putsymbol (output, next_inst_addr);

	cma__puteol (output);

	cma__putstring (output, "        ");
	cma__putstring (output, "Start routine:     ");
	cma___putsymbol (output, start_inst_addr);
	cma__puteol (output);

	cma__putstring (output, "        ");
	cma__putstring (output, "Scheduling policy: ");
	cma__putstring (output, sched_policy_name[policy]);
	cma__puteol (output);

	cma__puteol (output);

	cma__putstring (output, "        ");
	cma__putstring (output, "Stack storage:");
	cma__puteol (output);
	if (on_default_stack) {
	    cma__putstring (output, "        ");
	    cma__putstring (output, "  On default stack           ");
	    cma__putstring (output, "        SP:      ");
	    cma__puthex_8 (output, (cma_t_integer)stack_ptr_addr);
	    cma__puteol (output);
	    }
	else {
	    cma__putstring (output, "        ");
	    cma__putstring (output, "  Bytes in use:    ");
	    cma__putint_10 (output, stack_bytes_in_use);
	    cma__putstring (output, "        Base:    ");
	    cma__puthex_8 (output, (cma_t_integer)stack_base_addr);
	    cma__puteol (output);

	    cma__putstring (output, "        ");
	    cma__putstring (output, "  Bytes available: ");
	    cma__putint_10 (output, stack_bytes_available);
	    cma__putstring (output, "        SP:      ");
	    cma__puthex_8 (output, (cma_t_integer)stack_ptr_addr);
	    cma__puteol (output);

	    cma__putstring (output, "        ");
	    cma__putstring (output, "  Reserved Bytes:  ");
	    cma__putint_10 (output, stack_reserved_bytes);
	    cma__putstring (output, "        Top:     ");
	    cma__puthex_8 (output, (cma_t_integer)stack_top_addr);
	    cma__puteol (output);

	    cma__putstring (output, "        ");
	    cma__putstring (output, "  Guard Bytes:     ");
	    cma__putint_10 (output, stack_guard_bytes);
	    cma__puteol (output);
	    }

	cma__puteol (output);

	cma__putstring (output, "        ");
	cma__putstring (output, "Thread control block:");
	cma__puteol (output);
	cma__putstring (output, "        ");
	cma__putstring (output, "  Size:            ");
	cma__putint_10 (output, tcb_size);
	cma__putstring (output, "        Address: ");
	cma__puthex_8 (output, (cma_t_integer)tcb);
	cma__puteol (output);

	cma__puteol (output);

	cma__putstring (output, "        ");
	cma__putstring (output, "Total storage:     ");
	cma__putint_10 (output, total_bytes);
	cma__puteol (output);

	cma__puteol (output);
	};

    /* Restore any prior alternate print routine that may have been set */
    cma__set_eol_routine (prev_routine, &dummy_routine);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 * 	cma__deb_show_stats is called to output information about the
 *	overal threading system in general (not necessarily
 *	related to a particular thread).
 *
 *  FORMAL PARAMETERS:
 *
 *	tcb		- TCB whose thread info is to be displayed
 *			  NOT USED.. for possible future use.
 *
 *	full		- True if "full display" is requested, False for 
 *			   "brief display".
 *
 *	suppress_header	- True if the column header text is to be suppressed
 *			  (this allows creation of a table with headings
 *			  only over the first thread.)
 *
 *	print_string_noline 
 *			- Debugger routine to print a null-terminated
 *			  string, but not issue a CRLF.
 *
 *	print_string_line	
 *			- Debugger routine to print a null-terminated
 *			  string, with a trailing CRLF.
 *
 *	print_symbol	
 *			- Debugger routine to print an address symbolically
 *			  with no trailing CRLF.
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma__deb_show_stats(
	cma__t_int_tcb	    *tcb,
	cma_t_boolean	    full,
	cma_t_boolean	    suppress_header,
	cma__t_eol_routine  print_string_noline,
	cma__t_eol_routine  print_string_line,
	cma__t_print_symbol print_symbol
	)
    {
    cma__t_eol_routine  prev_routine;
    cma__t_eol_routine	dummy_routine;
    char		output[cma__c_buffer_size];

    /* Specify the debugger's print routine for the duration of this */
    cma__set_eol_routine (print_string_line, &prev_routine);

    output[0] = '\0';
    cma__putstring (output, "task statistics ");
    cma__puteol (output);

/*FIX-ME* */
    cma__putstring (output, "    Total context switches:     ");
    cma__putint_10 (output, 0);    
    cma__puteol (output);

/*FIX-ME* */
    cma__putstring (output, "    Number of existing threads: ");
    cma__putint_10 (output, 0);    
    cma__puteol (output);

/*FIX-ME* */
    cma__putstring (output, "    Total threads created:      ");
    cma__putint_10 (output, 0);    
    cma__puteol (output);

    cma__set_eol_routine (prev_routine, &dummy_routine);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 * 	cma__deb_inconsistency is called by the CMA debugger code to declare
 *	that an inconsistancy has been made in the CMA scheduler database.
 *
 * 	This routine ensures that an asynchronous entry into the 
 *	dispatcher occurs as soon as the debugger is finished.
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma__deb_inconsistency(void)
    {

    cma__g_debug_state.is_inconsistency = cma_c_true;

    /*FIX-ME* When/how do we generate inconsistency interrupt? */

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 * 	cma__init_debug is called to initialize the state of CMA's
 *	debugging support.
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	The global debug state data is initialized.
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma__init_debug(void)
    {

    /*
     * Caution: make sure any values needed at program startup by the
     * debugger are zero, since we won't have the opportunity to initialize
     * before then.
     */

    cma__g_debug_state.is_inconsistency = cma_c_false;
    cma__g_debug_state.events_enabled	= cma_c_false;
    cma__g_debug_state.next_to_run	= (cma__t_int_tcb *)cma_c_null_ptr;
    cma__g_debug_state.client_count	= 0;
    cma__g_debug_state.mutex		= cma__get_mutex (&cma__g_def_attr);
    cma__obj_set_name (cma__g_debug_state.mutex, "debugger client registry");
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Performs work prior to, and after a fork() - depending upon flag
 *
 *  FORMAL PARAMETERS:
 *
 *	flag
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma__reinit_debug(
	cma_t_integer	flag)
    {
    cma__t_int_tcb      *candidate;
    cma__t_queue	*ptr;
    cma__t_queue	*next_ptr;
    cma__t_int_tcb	*cur_tcb;

	
    if (flag == cma__c_reinit_prefork_lock) {
	cma__int_lock (cma__g_debug_state.mutex);
	}
    else if (flag == cma__c_reinit_postfork_unlock) {
	cma__int_unlock (cma__g_debug_state.mutex);
	}
    else if (flag == cma__c_reinit_postfork_clear) {
	    cma__g_debug_state.next_to_run = (cma__t_int_tcb *)cma_c_null_ptr;
	    cur_tcb = cma__get_self_tcb ();     /* Get running thread */

	    /*
	     * Traverse known threads list 'getting rid' of everyone
	     * but the initial thread, the null thread, and of course
	     * ourselves.  The initial thread gets pushed to the side
	     * if it is not the forking thread. Note that we're manipulating
	     * the known threads list without the kernel locked: that's OK,
	     * since we're single-threaded at this point (just starting up in
	     * the child thread).
	     */
	    ptr = cma__queue_next (&cma__g_known_threads.queue);

	    while (ptr != &cma__g_known_threads.queue) {
		next_ptr = cma__queue_next (ptr);
		candidate = cma__base (ptr, threads, cma__t_int_tcb);

		if ((candidate->kind != cma__c_thkind_null) 
			&& (candidate != cur_tcb)) {

		    if (candidate->kind == cma__c_thkind_initial) {
			cma__queue_remove (ptr);
			}
		    else {
			cma__queue_remove (ptr);
			cma__int_lock (candidate->attributes->mutex);
			cma__destroy_tcb (candidate);
			cma__int_unlock (candidate->attributes->mutex);
			}

		    }		

		ptr = next_ptr;
		}
	    }
    }

/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_DEB_CORE.C */
/*  *21   18-MAR-1992 19:00:53 SCALES "Parameterize scheduling policies" */
/*  *20   13-MAR-1992 14:08:19 BUTENHOF "Add tracing" */
/*  *19   19-FEB-1992 13:47:36 SCALES "Undefer on enter-kernel" */
/*  *18   10-FEB-1992 11:29:10 SCALES "Fix fork-reinit tcb deallocation" */
/*  *17   30-JAN-1992 10:14:18 CURTIN "Removed references to bzero and bcopy" */
/*  *16   13-JAN-1992 18:26:06 CURTIN "Added Alpha debug support" */
/*  *15   10-DEC-1991 15:16:43 CURTIN "cma__deb_get now takes into account alternate stacks" */
/*  *14   12-JUN-1991 14:26:41 CURTIN "fixed guardsize" */
/*  *13   10-JUN-1991 18:19:23 SCALES "Add sccs headers for Ultrix" */
/*  *12    6-JUN-1991 14:14:37 CURTIN "quick fix" */
/*  *11    6-JUN-1991 11:18:28 CURTIN "Added fixups for qar 119" */
/*  *10    5-JUN-1991 16:12:11 CURTIN "fork work" */
/*  *9    24-MAY-1991 16:45:55 CURTIN "Added a reinit routine" */
/*  *8    14-MAY-1991 13:43:06 BUTENHOF "Integrate changes lost in disk crash" */
/*  *7     2-MAY-1991 14:38:51 CURTIN "added code to cma__deb_set_alert" */
/*  *6     2-MAY-1991 13:57:47 BUTENHOF "Add argument to cma__bugcheck() calls" */
/*  *5    12-APR-1991 23:35:11 BUTENHOF "Implement hold for VPs" */
/*  *4    11-MAR-1991 19:07:05 BUTENHOF "Handle null symbolize function" */
/*  *3     7-MAR-1991 11:05:58 CURTIN "added cma_debug_get_sp_context" */
/*  *2    12-FEB-1991 01:28:59 BUTENHOF "Change to alert control bits" */
/*  *1    12-DEC-1990 21:43:52 BUTENHOF "Debug support" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_DEB_CORE.C */
