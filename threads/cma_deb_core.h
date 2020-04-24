/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_deb_core.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:34:49 $
 */
/*
 * HISTORY
 * $Log: cma_deb_core.h,v $
 * Revision 1.1.8.1  1996/05/10  13:34:49  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:39 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:41 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:36  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:16:38  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  15:06  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/20  19:24:34  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:10  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:11:33  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  17:57:16  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:32:35  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:37:12  alan]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:02:30  bolinger]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:10:05  bolinger]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:33:36  mgm  1.1.1.11]
 *
 * Revision 1.1.4.2  1994/06/09  13:37:39  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:36  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:15:51  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:16:38  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:26:56  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:37:12  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:36:03  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:02:30  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:10:18  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:10:05  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:38:58  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *	%W%	(ULTRIX/OSF)	%G%
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
 *	This file defines the internal interface to the core of CMA 
 * 	debugging services. (The client interface to debugging services
 *	is provided by cma_debug_client.h).
 *
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
 *		Create module
 *	002	Bob Conti	1 October 1990
 *		Move known object queues here
 *	003	Bob Conti	11 October 1990
 *		Add init entry 
 *	004	Paul Curtin	24 May 1991
 *		Added a prototype for cma__reinit_debug.
 *	005	Paul Curtin	10 December 1991
 *		Added current_sp as an argument to cma__deb_get.
 */

#ifndef CMA_DEB_CORE
#define CMA_DEB_CORE

/*
 *  INCLUDE FILES
 */
#include <cma.h>
#include <cma_mutex.h>
#include <cma_queue.h>
#include <cma_tcb_defs.h>
#include <cma_util.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */


/*
 * TYPEDEFS
 */

/*FIX-ME* Need to use sizes that are platform specific */
typedef long int cma___t_debug_ctx[17];

/*
 * Type defing the format of known object lists
 */
typedef struct CMA__T_KNOWN_OBJECT {
    cma__t_queue	queue;		/* Queue header for known objects */
    cma__t_int_mutex	*mutex;		/* Mutex to control access to queue */
    } cma__t_known_object;


/*
 * Type defining the registration for one debug client (e.g. Ada)
 */
typedef struct CMA__T_DEB_REGISTRY {
    cma_t_address	entry;		/* Client's debug entry point */
    cma_t_key		key;		/* Client's context key */
    cma_t_integer	fac;		/* Client's debug facility number */
    cma_t_boolean 	has_prolog;	/* Client's TCBs have std prolog */
    } cma__t_deb_registry;

#define cma__c_deb_max_clients	10

/* 
 * Type defining the global debugging state for all threads.
 */
typedef struct CMA__T_DEBUG_STATE {
    /* 
     * The following flag is set if changes were made while in the
     * debugger that may make the ready lists inconsistent.  For 
     * example, if a thread priority is changed in the debugger, the
     * thread is not moved between queues.  Making things consistent
     * is deferred to when the dispatcher is next invoked -- which we
     * try to make very soon.
     */
    cma_t_boolean 	is_inconsistency;   /* Ready lists are inconsistent */


    cma_t_boolean 	events_enabled;	    /* Set if _any_ event is enabled */
    cma_t_boolean	flags[cma__c_debevt__dim]; 	
					    /* Which events are enabled */
    cma__t_int_tcb	*next_to_run;	    /* TCB of thread to run next */ 

    cma__t_int_mutex	*mutex;		    /* Mutex for registering clients */
    cma_t_integer	client_count;	    /* Count of debug clients */
    cma__t_deb_registry	clients[cma__c_deb_max_clients+1];
					    /* Array of current debug clients */
    } cma__t_debug_state;


/* 
 * Routine that will symbolize and address and print it.
 */
typedef void	(*cma__t_print_symbol) (cma_t_address);


/*
 *  GLOBAL DATA
 */

/* 
 * Variable holding the global debugging state 
 *
 * (This is primarily written by the debugger interface and read
 * by the thread dispatcher).
 */
extern cma__t_debug_state	cma__g_debug_state;

/* 
 * Known object queues
 */
extern cma__t_known_object	cma__g_known_atts;
extern cma__t_known_object	cma__g_known_cvs;
extern cma__t_known_object	cma__g_known_mutexes;
extern cma__t_known_object	cma__g_known_threads;

/*
 * INTERNAL INTERFACES
 */

/* Get information while in debugger context */
extern void cma__deb_get 
           (cma__t_int_tcb *,cma_t_debug_get,cma_t_address,cma_t_integer,cma_t_integer);

/* Set information while in debugger context */
extern void cma__deb_set (cma__t_int_tcb *,cma_t_debug_set,cma_t_address,cma_t_integer);

extern void cma__init_debug (void);

extern void cma__reinit_debug (cma_t_integer);

extern void cma__deb_anytcb_to_tcb (cma_t_tcb_header *,cma__t_int_tcb **);

extern void cma__deb_fac_to_client (cma_t_integer,cma_t_key *);

extern void cma__deb_get_client_info (cma_t_key,cma_t_address *,cma_t_boolean *);

extern void cma__deb_get_context (cma__t_int_tcb *,cma_t_key,cma_t_address *);

extern cma__t_int_tcb *cma__deb_get_self_tcb (void);

extern void cma__deb_get_time_slice (cma_t_interval *);

extern cma__t_int_tcb *cma__deb_next_tcb 
            (cma__t_int_tcb *,cma_t_integer *,cma_t_integer *,cma_t_boolean *);

extern cma_t_boolean cma__deb_set_alert (cma__t_int_tcb *);

extern void cma__deb_set_next_thread (cma__t_int_tcb *);
	
extern void cma__deb_set_force_dispatch (cma_t_address );

extern void cma__deb_set_time_slice (cma_t_interval);

extern void cma__deb_show_thread 
     (cma__t_int_tcb *,cma_t_boolean,cma_t_boolean,cma___t_debug_ctx,cma__t_eol_routine,
                cma__t_eol_routine,cma__t_print_symbol);

extern void
cma__deb_show_stats (cma__t_int_tcb *,cma_t_boolean,cma_t_boolean,cma__t_eol_routine,cma__t_eol_routine,cma__t_print_symbol);
#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_DEB_CORE.H */
/*  *6    10-DEC-1991 15:18:47 CURTIN "cma__deb_get now takes SP as an argument" */
/*  *5    10-JUN-1991 19:51:59 SCALES "Convert to stream format for ULTRIX build" */
/*  *4    10-JUN-1991 19:20:25 BUTENHOF "Fix the sccs headers" */
/*  *3    10-JUN-1991 18:19:47 SCALES "Add sccs headers for Ultrix" */
/*  *2    24-MAY-1991 16:46:07 CURTIN "Added a reinit routine" */
/*  *1    12-DEC-1990 21:43:56 BUTENHOF "Debug support" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_DEB_CORE.H */
