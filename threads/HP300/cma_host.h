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
 * Revision 1.1.6.2  1996/02/18  23:08:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:10  marty]
 *
 * Revision 1.1.6.1  1995/12/07  21:45:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  20:58:57  root]
 * 
 * Revision 1.1.4.2  1994/06/09  13:34:01  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:12  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/25  19:57:32  ohara
 * 	more code cleanup
 * 	[1994/04/20  20:37:55  ohara]
 * 
 * Revision 1.1.2.2  1992/12/15  22:20:12  alan
 * 	Insert copyright notices
 * 	[1992/12/07  15:58:47  alan]
 * 
 * Revision 1.1  1992/01/19  14:39:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  FACILITY:
 *
 *	CMA services
 *
 *  ABSTRACT:
 *
 *	Header file for HPUX S300/OS host-specific functions
 *
 *  AUTHORS:
 *
 *	Kevin Ackley (as told to Howard Morris)
 *
 *  CREATION DATE:
 *
 *	16 August 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	002	Howard Morris   21 Aug 1990
 *		Cma_host_m68k_apollo.h => cma_host_hp9000s300.h
 *	001	Webb Scales	3  May 1990
 *		Replace cma__lock_any and cma__unlock_any by 
 *		cma__interrupt_disable and cma__interrupt_enable.
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
#define cma__c_tac_static_clear         0
#define cma__tac_clear(addr)            ((*addr) = 0)
#define cma__tac_set(addr)              ((*addr) = 1)
#define cma__tac_isset(addr)            ((*addr) != 0)

/* FIX-ME: These min/max stack values were stuck in to satisfy compiler
   errors.  Need to set them to the proper values. */

/* 
 * Lowest and highest (numerical) addresses in the initial (default) thread's 
 * stack.
 */

#define cma__c_def_stack_min	0x40000000
#define cma__c_def_stack_max	0x7fffffff

/*
 * Note: disable interrupts/enable interrupts taken from
 * cma_host_mips_ultrix.h file.
 * 
 * Disable interrupts (signals)
 */
#if 0 /* This macro is not currently needed */
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
#if 0 /* This macro is not currently needed */
# define cma__interrupt_enable(sig_mask) \
    (sigprocmask (SIG_SETMASK, &sig_mask, cma_c_null_ptr) == -1 ? \
	cma__bugcheck() : \
	0)
#else
# define cma__interrupt_enable(dummy)
#endif


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
 * Define interlocked operations to use on kernel lock (on VAX, these are the
 * same as the normal test-and-set and unset operations, and merely call the
 * assembly code routines defined in cma_assem_vax_ultrix.s).
 */
#define cma__kernel_set(address) cma__test_and_set ((address))
#define cma__kernel_unset(address) cma__unset ((address))
#define _CMA_HOST_ATOMIC_TAS_

#define cma__sigcode( code ) (code)

/*
 * TYPEDEFS
 */

typedef struct sigcontext	cma__t_sigcontext;
typedef int					cma__t_siginfo;

typedef cma_t_boolean   cma__t_atomic_bit;

/*
 * The stack format as saved by cma__transfer_regs() and friends
 */
typedef struct {
    int  d2,d3,d4,d5,d6,d7;
    int* a2,a3,a4,a5,a6;
    int* pc;
} cma__t_m68k_regs;

/*
 * Do not change this structure without understanding assembly
 * code in cma_host_hp9000s300.s
 */
typedef struct CMA__T_STATIC_CT {
    cma__t_m68k_regs *sp;           /* stack pointer register */

    /*** A convenient place to stash info for cma__thread_startup to use: */
    cma_t_start_routine	start_routine;	/* Function to be executed */
    cma_t_address	arg;		/* Argument to start routine */
} cma__t_static_ctx;

/*
 * Do not change this structure without understanding assembly
 * code in cma_host_hp9000s300.s
 */
typedef struct CMA__T_ASYNC_CTX {
    long   restart_pc;	  /* PC at point of signal */
    short  restart_ccr;   /* condition flags at point of signal */
} cma__t_async_ctx;

/*
 * INTERNAL INTERFACES
 */

/*
 * Return the present value of the stack pointer for the current thread
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

/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST_HP9000S300.H */
/*  *5    18-JUL-1990 15:52:42 SCALES "Apollo BL3 changes" */
/*  *4    21-JUN-1990 12:51:57 BUTENHOF "Optimize mutexes" */
/*  *3    18-MAY-1990 07:10:08 BUTENHOF "Optimize mutex and cv" */
/*  *2     4-MAY-1990 15:15:20 SCALES "Add/update interrupt-blocking macros" */
/*  *1    17-APR-1990 07:18:22 BUTENHOF "Apollo's host header" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST_HP9000S300.H */
/*   DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST.H */
/*   *2    17-SEP-1991 14:55:20 BUTENHOF "Integrate CMA5 reverse drop" */
/*   *1     7-DEC-1990 16:20:16 SCALES "HP-specific routines" */
/*   DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST.H */
