/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_host.h,v $
 * Revision 1.1.6.2  1996/02/18  23:09:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:33:32  marty]
 *
 * Revision 1.1.6.1  1995/12/07  21:58:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:04:03  root]
 * 
 * Revision 1.1.4.2  1994/06/09  13:37:01  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:06  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/25  19:57:41  ohara
 * 	more code cleanup
 * 	[1994/04/20  20:38:11  ohara]
 * 
 * Revision 1.1.2.2  1992/12/15  22:24:38  alan
 * 	Insert copyright notices
 * 	[1992/12/07  16:11:22  alan]
 * 
 * Revision 1.1  1992/01/19  14:38:43  devrcs
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
 *	Header file for VAX/ULTRIX host-specific functions
 *
 *  AUTHORS:
 *
 *	Dave Butenhof and Webb Scales
 *
 *  CREATION DATE:
 *
 *	9 January 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Webb Scales	16 April 1990
 *		Add initial (default) thread stack limits 
 *	002	Webb Scales	3 May 1990
 *		Replace cma__lock_any and cma__unlock_any by 
 *		cma__interrupt_disable and cma__interrupt_enable.
 *	003	Dave Butenhof	14 June 1990
 *		Add new cma__kernel_set and cma__kernel_unset operations.
 *	004	Webb Scales	15 August 1990
 *		Rearranged things, moving a lot into the host-generic file
 *	005	Dave Butenhof	18 December 1990
 *		Remove dependencies on generic host file.
 *	006	Dave Butenhof	09 April 1991
 *		Improve portability by adding a type for internal locks,
 *		rather than assuming it can be just an int.
 *	013	Dave Butenhof	14 May 1991
 *		Fix MP cma__t_atomic_bit macros.
 */


/*
 *  INCLUDE FILES
 */

#include <cma.h>

/*
 * CONSTANTS AND MACROS
 */

/*
 * Initialization for "atomic bit" objects
 */
#define cma__c_tac_static_clear	0
#define cma__tac_clear(addr)	(*(addr) = 0)
#define cma__tac_set(addr)	(*(addr) = 1)
#define cma__tac_isset(addr)	(*(addr) != 0)

#define cma__c_def_stack_min	0x40000000
#define cma__c_def_stack_max	0x7fffffff

/*
 * This platform provides an atomic test-and-set instruction.
 * Therefore define this switch to make cma_host.h set up the
 * macros and routine prototypes correctly.
 */
#define _CMA_HOST_ATOMIC_TAS_	1

/*
 * This platform provides a secondary signal code.  Raise specific exceptions
 * where possible.
 */
#define _CMA_DECODE_SIGNALS_	1

/*
 * Disable interrupts (signals)
 */
#if 0 /* This macro is not currently needed, so use the dummy in cma_host.h */
# define cma__interrupt_disable(sig_mask) \
    (sigprocmask (SIG_SETMASK, &cma___g_sig_block_mask, &sig_mask) == -1 ? \
	cma__bugcheck() : \
	0)
#else
# define cma__interrupt_disable(dummy)
#endif

/*
 * Enable interrupts (signals)
 */
#if 0 /* This macro is not currently needed, so use the dummy in cma_host.h */
# define cma__interrupt_enable(sig_mask) \
    (sigprocmask (SIG_SETMASK, &sig_mask, cma_c_null_ptr) == -1 ? \
	cma__bugcheck() : \
	0)
#else
# define cma__interrupt_enable(dummy)
#endif

#define cma__kernel_set(address)    cma__test_and_set ((address))
#define cma__kernel_unset(address)  cma__unset ((address))

#define cma__sigcode( code )	(code)

/*
 * TYPEDEFS
 */

typedef struct sigcontext	cma__t_sigcontext;
typedef int					cma__t_siginfo;

typedef int	cma__t_atomic_bit;

typedef struct CMA__T_STATIC_CTX {
    long int	fp;
    long int	sp;
    } cma__t_static_ctx;

typedef struct CMA__T_ASYNC_CTX {
    cma_t_address	pc;	/* Interrupted threads PC */
    cma_t_address	psl;	/* Interrupted threads PSL */
    } cma__t_async_ctx;

/*
 *  GLOBAL DATA
 */

/*
 * INTERNAL INTERFACES
 */

extern cma_t_address
cma__fetch_sp _CMA_PROTOTYPE_ ((void));

/*
 * Atomically test (and return) low bit of longword at the specified address,
 * and set the bit.
 */
extern cma_t_boolean
cma__test_and_set _CMA_PROTOTYPE_ ((
	cma_t_boolean	*address));

/*
 * Atomically clear the low bit of longword at the specified address.
 */
extern void
cma__unset _CMA_PROTOTYPE_ ((
	cma_t_boolean	*address));

/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST_VAX_ULTRIX.H */
/*  *11   17-AUG-1990 10:36:06 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *10   17-AUG-1990 02:04:20 SCALES "Apollo changes; Sun & HPPA port" */
/*  *9     3-JUL-1990 07:33:58 BUTENHOF "Change copyright header" */
/*  *8    21-JUN-1990 12:52:10 BUTENHOF "Optimize mutexes" */
/*  *7    18-MAY-1990 09:22:16 BUTENHOF "Change PROTOTYPE macro to _CMA_PROTOTYPE_" */
/*  *6    18-MAY-1990 07:10:17 BUTENHOF "Optimize mutex and cv" */
/*  *5     4-MAY-1990 15:15:47 SCALES "Add/update interrupt-blocking macros" */
/*  *4    16-APR-1990 22:52:38 SCALES "Raise exception if thread is on a ""foriegn"" stack." */
/*  *3     9-FEB-1990 11:09:21 BUTENHOF "CMA.h is now lowercase" */
/*  *2    24-JAN-1990 09:56:39 BUTENHOF "Another pass on prototypes (use ""(void)"")" */
/*  *1     9-JAN-1990 13:35:20 BUTENHOF "VAX ULTRIX host definitions" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST_VAX_ULTRIX.H */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST.H */
/*  *4    14-MAY-1991 16:19:50 BUTENHOF "Remove static TAC inits" */
/*  *3    12-APR-1991 23:40:50 BUTENHOF "Add cma__t_int_lock type" */
/*  *2    18-DEC-1990 23:40:27 BUTENHOF "Remove dependencies on generic cma_host.h" */
/*  *1    12-DEC-1990 19:01:37 BUTENHOF "VAX specific host defs" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST.H */
