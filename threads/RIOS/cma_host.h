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
 * Revision 1.1.10.3  1996/02/18  23:08:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:32:08  marty]
 *
 * Revision 1.1.10.2  1995/12/07  21:50:17  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/DCE1_1_WP/1  1995/04/26  16:55 UTC  jrr
 * 	Changes to allow valid AIX core dumps.
 * 	[1995/12/07  21:00:42  root]
 * 
 * Revision 1.1.6.3  1994/06/09  13:35:36  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:26:07  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/25  19:57:39  ohara
 * 	more code cleanup
 * 	[1994/04/20  20:38:08  ohara]
 * 
 * Revision 1.1.6.1  1994/04/01  20:14:12  jd
 * 	First drop for code cleanup
 * 	[1994/03/28  19:30:45  jd]
 * 
 * Revision 1.1.4.4  1992/12/15  22:23:30  alan
 * 	Insert copyright notices
 * 	[1992/12/07  16:03:01  alan]
 * 
 * Revision 1.1.4.3  1992/12/07  13:21:08  zeliff
 * 	Removed duplicate OSF copyright markers
 * 	[1992/12/06  00:45:05  zeliff]
 * 
 * Revision 1.1.4.2  1992/09/03  14:34:09  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:01:09  bolinger]
 * 
 * Revision 1.1.2.2  1992/08/28  20:59:52  bolinger
 * 	Do the update for real, this time.
 * 	[1992/08/28  20:59:09  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:38:37  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *  Copyright (c) 1989, 1991 by
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
 *	Header file for MIPS/ULTRIX host-specific functions
 *
 *  AUTHORS:
 *
 *	Dave Butenhof and Webb Scales
 *
 *  CREATION DATE:
 *
 *	19 October 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	20 October 1989
 *		Add "cma__fetch_sp" function (currently not implemented) to
 *		shadow the cma_host_vms.h implementation.
 *	002	Dave Butenhof	31 October 1989
 *		Time to implement cma__fetch_sp: as prototype for assembly
 *		code routine.
 *	003	Webb Scales	18 November 1989
 *		Add host dependent thread context structures, and #define's for
 *		mips assembly code.
 *	004	Dave Butenhof and Webb Scales	9 January 1990
 *		Delete cma__add_atomic, since it's not used anywhere.
 *	005	Webb Scales	16 April 1990
 *		Add default stack limits 
 *	006	Webb Scales	3 May 1990
 *		Replace cma__lock_any and cma__unlock_any by 
 *		cma__interrupt_disable and cma__interrupt_enable.
 *	007	Dave Butenhof	14 May 1990
 *		Replace the old cma__test_and_set with the new
 *		cma__kernel_set, and cma__unset with cma__kernel_unset.
 *		These assume that the kernel is locked, or that they are
 *		being used to manipulate the kernel lock.  The
 *		cma__test_and_set and cma__unset operations now lock the
 *		kernel (using cma__kernel_set) before attempting the
 *		operation, to ensure atomicity on a uniprocessor.
 *	008	Dave Butenhof	25 February 1991
 *		Correct #includes; internal-only CMA files are no longer
 *		exported, and so should be accessed through appropriate -I
 *		switches, rather than through usr/include/dce/threads.
 *	009	Dave Mehaffy, et al	???
 *		Port to IBM RS/6000
 *	010	Dave Butenhof	12 June 1991
 *		Integrate IBM changes, and update to use new atomic bit
 *		types.
 */

#ifndef CMA_HOST_DEFS
#define CMA_HOST_DEFS

#include <sys/param.h>

/* Start moved here RIOS */


#define cma__c_page_size NBPG	
#define cma___c_reserve_size	cma__c_page_size * 3	/* Stack reserver area */

/*
 * Note: 
 * Default sizes for CMA's stacks and their guard pages have been derived
 * from current values used in the ADA run time library.  These values have
 * proven to be good working values.
 *
 * WARNING: The default stack size and default guard size on CMA stacks
 * must never be made smaller!  Such a change would break many applications.
 */
#  define cma__c_default_stack	cma__c_page_size * 8	/* Default stack */
#  define cma__c_default_guard	cma__c_page_size * 2	/* Default guard */

#  define cma__c_chunk_count	48

/* End moved here */




/*
 *  INCLUDE FILES
 */

#include <sys/signal.h>

/*
 * CONSTANTS AND MACROS
 */

/*
 * Initialization for "atomic bit" objects
 */
#define cma__c_tac_static_clear		0
#define cma__tac_clear(addr)		((*addr) = 0)
#define cma__tac_set(addr)		((*addr) = 1)
#define cma__tac_isset(addr)		((*addr) != 0)

/*
 * Lowest and highest (numerical) addresses in the initial (default) thread's 
 * stack.
 */
#define cma__c_def_stack_min	0x00000000
#define cma__c_def_stack_max	0x2fffffff

/*
 * Disable interrupts (signals)
 */
#if 0 /* This macro is not currently needed */
# define cma__interrupt_disable(sig_mask) \
    (sigprocmask (SIG_SETMASK, &cma___g_sig_block_mask, &sig_mask) == -1 ? \
	cma__bugcheck(), 0 : \
	0)
#else
# define cma__interrupt_disable(dummy)
#endif

/*
 * Enable interrupts (signals)
 */
#if 0 /* This macro is not currently needed */
# define cma__interrupt_enable(sig_mask) \
    (sigprocmask (SIG_SETMASK, &sig_mask, cma_c_null_ptr) == -1 ? \
	cma__bugcheck(), 0 : \
	0)
#else
# define cma__interrupt_enable(dummy)
#endif

/*
 * Clear interrupt context (to allow new interrupts to come in)
 */
#define cma__clear_interrupt(sig_mask) \
	(sigprocmask (SIG_SETMASK, sig_mask, cma_c_null_ptr) == -1 ? \
	 cma__bugcheck("cma__clear_interrupt, mask = %x", sig_mask), 0 : \
	 0)

/*
 * The following four operations implement "atomic test-and-set" and "atomic
 * clear" operations, on the low bit of a specified address.
 *
 * If the architecture supports interlocked atomic instructions, they must be
 * used to implement these functions.  On architectures which do not support
 * interlocked atomic instructions, and which only support uniprocessor
 * configurations, the CMA kernel lock (via cma__enter_kernel and
 * cma__exit_kernel) can be used to ensure atomicity. Locking the kernel
 * prevents a timeslice, guaranteeing serialization of access.
 *
 * "cma__test_and_set" and "cma__unset" are used for arbitrary data; if
 * necessary they may lock the kernel, and thus cannot be used while the
 * kernel is already locked.
 *
 * "cma__kernel_set" and "cma__kernel_unset" are used for locking the kernel
 * (which is atomic on uniprocessors due to the nature of the kernel lock),
 * and for locking arbitrary bits when the kernel is already locked.
 *
 * "cma__kernel_set" and "cma__test_and_set" must be identical, with the
 * exception that "cma__test_and_set" may lock and unlock the kernel if
 * necessary.  "cma__kernel_set" and "cma__unset" must also be identical,
 * with the same exception (although on many architectures it may be
 * unnecessary to lock the kernel for an unset, in which case they should be
 * completely identical).
 */

/*
 * Return low bit of longword at the specified address, and set the bit.
 */
#define cma__kernel_set(address) \
    (cs(address,0,1) ? cma_c_true : cma_c_false)

/*
 * Clear the low bit of longword at the specified address.
 */
#define cma__kernel_unset(address) (cma__tac_clear (address))

/*
 * These operations use the cma__kernel_set and cma__kernel_unset functions
 * to create uniprocessor atomic operations on a bit.  (Note that cma__unset
 * doesn't actually need to lock the kernel, since writing a single value is
 * atomic anyway.)
 */
#define cma__test_and_set(address) \
    (cs(address,0,1) ? cma_c_true : cma_c_false)

#define cma__unset(address) (cma__tac_clear (address))

#define cma__sigcode( code )	(code)

/*
 * TYPEDEFS
 */

typedef struct sigcontext	cma__t_sigcontext;
typedef int					cma__t_siginfo;

typedef int	cma__t_atomic_bit;

typedef struct CMA__T_STATIC_CTX {
    long int	sp;
    } cma__t_static_ctx;

typedef struct CMA__T_ASYNC_CTX {
	cma_t_boolean   valid;
	cma_t_address   interrupt_ctx;
#ifdef	AIX_CORE_FILE
		/* may not be used - needed to pad tcb struct for core file */
	struct sigcontext	ibmr2_scp;
#endif	/* AIX_CORE_FILE */
    } cma__t_async_ctx;

/*
 *  GLOBAL DATA
 */

/*
 * INTERNAL INTERFACES
 */

/*
 * Return the present value of the stack pointer for the current thread
 */
extern cma_t_address cma__fetch_sp (void);

#endif	/* CMA_HOST_DEFS */


