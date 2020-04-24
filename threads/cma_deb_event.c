/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_deb_event.c,v $
 * Revision 1.1.6.2  1996/02/17  23:33:48  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:50:00  marty]
 *
 * Revision 1.1.6.1  1995/12/07  22:01:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:05:09  root]
 * 
 * Revision 1.1.4.2  1994/06/09  13:37:40  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:38  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:15:53  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:16:45  jd]
 * 
 * Revision 1.1.2.2  1992/12/15  22:27:04  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:37:18  alan]
 * 
 * Revision 1.1  1992/01/19  14:38:59  devrcs
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
 *	This module implements debugging events
 *
 *  AUTHORS:
 *
 *	Bob Conti
 *
 *  CREATION DATE:
 *
 *	9 September 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Bob Conti	9 September 1990
 *		Initial creation of module
 *	002	Webb Scales	6 December 1990
 *		Made proto for signal_event conditional: on VMS only
 */


/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_defs.h>
#include <cma_deb_event.h>
#include <cma_deb_core.h>

/*
 * GLOBAL DATA
 */

/*
 * LOCAL DATA
 */

/*
 * LOCAL MACROS
 */

/*
 * LOCAL FUNCTIONS
 */
#if _CMA_PLATFORM_ == _CMA__VAX_VMS
extern void
cma$$dbg_signal_event ();
#endif



/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Called to declare the a debugging event of a particular kind
 * 	has occurred.  
 * 
 *	If the event enable bits in the current thread indicate that the
 *	event has been enabled for the thread, or if the all-threads enable
 *	bit is set, then the event is triggered.  Triggering the event 
 *	causes program execution to stop, the event is announced, and the 
 *	debugger prompt appears.  Debugger in this context refers to
 *	the real debugger (if it has been trained to understand 
 *	events) or the CMA callable debugger.
 *
 *  FORMAL PARAMETERS:
 *
 *	event 	- The code defining which event is being declared
 *
 *  IMPLICIT INPUTS:
 *
 *	Event enable bits for all threads and for the current thread.
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
 *	Changes made while in the debugger
 */

extern void
cma__debevt_report(
	cma__t_debevt 	event
	)
    {
    cma__t_int_tcb	*tcb;		/* Current thread's TCB  */
    cma__t_int_tcb	*old_tcb;	/* Preempted thread's TCB  */
    cma_t_address	p1;		/* Parameters to event routine */

    tcb = cma__get_self_tcb ();		/* Get running thread */

    /*
     * Gather some additional info for certain events
     */
    if (event == cma__c_debevt_activating)
	p1 = (cma_t_address)tcb->debug.start_pc;
    else if (event == cma__c_debevt_preempting) {
	old_tcb = (cma__t_int_tcb *)tcb->debug.preempted_tcb;
	p1 = (cma_t_address)((cma_t_integer)old_tcb->header.sequence);
	}

#if _CMA_PLATFORM_ == _CMA__VAX_VMS
    /* 
     * Pass it on to the BLISS code that knows how to inform the debugger
     */
    cma$$dbg_signal_event (
	event, 
	tcb,
	p1,
	0);	
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__debevt_report_2 is the same as cma__debevt_report except
 *	that two additional parameters are passed in.
 *
 *  FORMAL PARAMETERS:
 *
 *	event 	- The code defining which event is being declared
 *	p1	- Parameter 1 (event specific meaning)
 *	p2	- Parameter 2 (event specific meaning)
 *
 *  IMPLICIT INPUTS:
 *
 *	Event enable bits for all threads and for the current thread.
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
 *	Changes made while in the debugger
 */

extern void
cma__debevt_report_2(
	cma__t_debevt 	event,
	cma_t_address	p1,
	cma_t_address	p2
	)
    {
    cma__t_int_tcb	*tcb;		/* Current thread's TCB  */

    tcb = cma__get_self_tcb ();		/* Get running thread */

#if _CMA_PLATFORM_ == _CMA__VAX_VMS
    /* 
     * Pass it on to the BLISS code that knows how to inform the debugger
     */
    cma$$dbg_signal_event (
	event, 
	tcb,
	p1,
	p2);	
#endif
    }


extern void
cma__debevt_notify(void)
    {
    cma__t_int_tcb	*tcb;		/* Current thread's TCB  */

    tcb = cma__get_self_tcb ();		/* Get running thread */

#if _CMA_PLATFORM_ == _CMA__VAX_VMS
    /* 
     * Pass it on to the BLISS code that knows how to inform the debugger
     */
    cma$$dbg_notify (tcb);
#endif
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Called to enable a debugging event of a particular kind.
 * 
 *	The event enable events in the thread or the global bits are
 *	set.
 *  
 *	NOTE: This is called from debugger context, so no locks may
 *	be taken out.
 *
 *  FORMAL PARAMETERS:
 *
 *	TCB	- The TCB for the thread whose events are to be set
 *		  If null, then the event is set globally, for all threads.
 *	Event 	- The code defining which event is being enabled
 *
 *  IMPLICIT INPUTS:
 *
 *	Event enable bits for all threads and for the current thread.
 *
 *  IMPLICIT OUTPUTS:
 *
 *	Event enable bits for all threads and for the current thread.
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	Changes made while in the debugger
 */

extern void
cma__debevt_set(
	cma__t_int_tcb	*tcb,
	cma__t_debevt 	event
	)
    {

    /*
     * When any event is set, turn on the event reporting code
     * and leave it on.
     */
    cma__g_debug_state.events_enabled = cma_c_true;
         
    /*
     * Select either the global flags or the TCB flags, and set
     */
    if (tcb == (cma__t_int_tcb *)cma_c_null_ptr)
	cma__g_debug_state.flags[(cma_t_integer)event]	= cma_c_true;
    else
	tcb->debug.flags[(cma_t_integer)event]		= cma_c_true;

    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Called to disable a debugging event of a particular kind.
 * 
 *	The event enable events in the thread or the global bits are
 *	cleared.
 *  
 *	NOTE: This is called from debugger context, so no locks may
 *	be taken out.
 *
 *  FORMAL PARAMETERS:
 *
 *	TCB	- The TCB for the thread whose event is to be cleared
 *		  If null, then the event is cleared globally, for all threads.
 *	Event 	- The code defining which event is being disabled.
 *
 *  IMPLICIT INPUTS:
 *
 *	Event enable bits for all threads and for the current thread.
 *
 *  IMPLICIT OUTPUTS:
 *
 *	Event enable bits for all threads and for the current thread.
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	Changes made while in the debugger
 */

extern void
cma__debevt_clear(
	cma__t_int_tcb	*tcb,
	cma__t_debevt 	event
	)
    {
         
    /*
     * Select either the global flags or the TCB flags, and set
     */
    if (tcb == (cma__t_int_tcb *)cma_c_null_ptr)
	cma__g_debug_state.flags[(cma_t_integer)event]	= cma_c_false;
    else
	tcb->debug.flags[(cma_t_integer)event]		= cma_c_false;

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Called to test if a debugging event of a particular kind is
 *	enabled.
 * 
 *	The event enable events in the thread or the global bits are
 *	tested.
 *  
 *	NOTE: This is called from debugger context, so no locks may
 *	be taken out.
 *
 *  FORMAL PARAMETERS:
 *
 *	TCB	- The TCB for the thread whose event is to be tested
 *		  If null, then the global flags are tested.
 *	Event 	- The code defining which event is being tested.
 *
 *  IMPLICIT INPUTS:
 *
 *	Event enable bits for all threads and for the current thread.
 *
 *  IMPLICIT OUTPUTS:
 *
 *	Event enable bits for all threads and for the current thread.
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
cma__debevt_test(
	cma__t_int_tcb	*tcb,
	cma__t_debevt 	event
	)
    {
         
    /*
     * Select either the global flags or the TCB flags, and set
     */
    if (tcb == (cma__t_int_tcb *)cma_c_null_ptr)
	return (
	    (cma_t_boolean)(cma__g_debug_state.flags[(cma_t_integer)event]));
    else
	return (
	    (cma_t_boolean)(tcb->debug.flags[(cma_t_integer)event]));

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_DEB_EVENT.C */
/*  *2    10-JUN-1991 18:19:53 SCALES "Add sccs headers for Ultrix" */
/*  *1    12-DEC-1990 21:43:58 BUTENHOF "Debug support" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_DEB_EVENT.C */
