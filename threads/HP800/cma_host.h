/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_host.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:26:47 $
 */
/*
 * HISTORY
 * $Log: cma_host.h,v $
 * Revision 1.1.8.1  1996/05/10  13:26:47  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:49 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:47 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  18:32  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/20  14:08:59  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:41:35  root  1.1.8.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:10:01  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  17:31:13  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:30:56  hopkins]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:30:51  mgm  1.1.1.11]
 *
 * Revision 1.1.4.3  1994/06/09  13:34:25  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:23  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/25  19:57:34  ohara
 * 	more code cleanup
 * 	[1994/04/20  20:37:59  ohara]
 * 
 * Revision 1.1.4.1  1994/04/01  20:13:37  jd
 * 	First drop for code cleanup
 * 	[1994/03/28  19:30:32  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:21:07  alan
 * 	Insert copyright notices
 * 	[1992/12/07  15:59:36  alan]
 * 
 * Revision 1.1.2.2  1992/10/03  19:46:50  sommerfeld
 * 	Protect against multiple inclusion.
 * 	Redefine cma__t_async_ctx to match other ports.
 * 	Add cma__clear_interrupt().
 * 	Add placebo for SIGWINCH.
 * 	[1992/09/09  20:31:18  sommerfeld]
 * 
 * Revision 1.1  1992/01/19  14:40:05  devrcs
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
 *	Header file for HPUX S800/OS host-specific functions
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
 *		Cma_host_m68k_apollo.h => cma_host_hp9000s800.h
 *	001	Webb Scales	3  May 1990
 *		Replace cma__lock_any and cma__unlock_any by 
 *		cma__interrupt_disable and cma__interrupt_enable.
 */

#ifndef CMA_HOST_DEFS
# define CMA_HOST_DEFS


/* Start moved here HP800 */


#define cma__c_page_size NBPG	
# if _CMA_HARDWARE_ == _CMA__HPPA
/* 
 * These constants are guesses calculated by multiplying the non-hp values
 * by the ratio of hpux hppa page size (2048) to the non-hp page size (512).
 */
#  define cma___c_reserve_size	cma__c_page_size * 6 /* Stack reserve area */
# else
/* 
 * These constants are guesses calculated by multiplying the non-hp values
 * by the ratio of hpux 68k page size (4096) to the non-hp page size (512).
 */
#  define cma___c_reserve_size	cma__c_page_size * 3 /* Stack reserve area */
/*
 * Note: 
 * Default sizes for CMA's stacks and their guard pages have been derived
 * from current values used in the ADA run time library.  These values have
 * proven to be good working values.
 *
 * WARNING: The default stack size and default guard size on CMA stacks
 * must never be made smaller!  Such a change would break many applications.
 */
#endif
# if _CMA_HARDWARE_ == _CMA__HPPA
#  define cma__c_default_stack	cma__c_page_size * 15	/* Default stack */
#  define cma__c_default_guard	cma__c_page_size * 4	/* Default guard */
# else
#  define cma__c_default_stack	cma__c_page_size * 8	/* Default stack */
#  define cma__c_default_guard	cma__c_page_size * 2	/* Default guard */
# endif

# if _CMA_HARDWARE_ == _CMA__HPPA
#  ifndef _HP_CMA_PERF_
#   define cma__c_chunk_count	100	/* This many chunks in an array */
#  else
#   define cma__c_chunk_count	96	/* This many chunks in an array */
#  endif /* _HP_CMA_PERF_ */
# else
#  define cma__c_chunk_count	48	/* This many chunks in an array */
# endif

/* End moved here */


/*
 *  INCLUDE FILES
 */

#if 0
#include <cma.h>
#endif
#include <signal.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

/*
 * NIH strikes again: consider BSD "SIGWINCH" to be equivalent to 
 * HPUX "SIGWINDOW".
 */

#ifndef SIGWINCH
#define SIGWINCH SIGWINDOW
#endif

/*
 * Initialization for "atomic bit" objects
 */
#define cma__c_tac_static_clear         0
#define cma__tac_clear(addr)            ((*addr) = 0)
#define cma__tac_set(addr)              ((*addr) = 1)
#define cma__tac_isset(addr)            ((*addr) != 0)

/* Possible actions to be taken as a result of a timer signal. */
#define cma__c_hppa_timer_no_action	0
#define cma__c_hppa_timer_yield		1
#define cma__c_hppa_timer_alert		2

/* FIX-ME: These min/max stack values were stuck in to satisfy compiler
   errors.  Need to set them to the proper values. */

/* 
 * Lowest and highest (numerical) addresses in the initial (default) thread's 
 * stack.
 */

#define cma__c_def_stack_min	0x40000000
#define cma__c_def_stack_max	0x7fffffff

/*
 * This platform provides a secondary signal code.  Raise specific exceptions
 * where possible.
 */
#define _CMA_DECODE_SIGNALS_	1

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

#define cma__clear_interrupt(sig_mask) \
    (sigprocmask (SIG_SETMASK, sig_mask, cma_c_null_ptr) == -1 ? \
	cma__bugcheck("cma__clear_interrupt, mask = %x", sig_mask), 0 : 0)\


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
 * Define interlocked operations to use a kernel lock.
 *
 * First do an atomic "load and clear word" on the s800 internal variable,
 * cma__g_serialize.  If cma__g_serialize is busy then pass back a "busy"
 * status (ie, cma_c_true).  Otherwise use the standard C test-and-set
 * source code and pass back cma_c_true if the semaphore is busy or 
 * cma_c_false if it is not.  In any case, set cma__g_serialize to "free"
 * when finished with the routine ("free" is the value 1).
 */
/*
 * SINCE WE ARE ON A UNIPROCESSOR, THE FOLLOWING IS OVERKILL - WE'LL
 * USE A SIMPLIFIED IMPLEMENTATION
#define cma__kernel_set(address) \ 
    ( \
    (cma__serialize ()) ? \
      ((*(address) & 1) ?   (cma__g_serialize = 1, cma_c_true) : \
         (*(address) |= 1,  (cma__g_serialize = 1, cma_c_false))) : \
      cma_c_true \
    )
*/
#define cma__kernel_set(address) \
    ((*(address) != 0) ? cma_c_true : (*(address) = 1, cma_c_false))

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
/*
 * SINCE WE KNOW THIS CANNOT BE CALLED WHILE IN THE KERNEL, WE CAN
 * MAKE THIS MORE EFFICIENT
#define cma__test_and_set(address) \
    ( \
    (cma__call_enter_kernel (), \
       (*(address) & 1)) ? \
	   (cma__exit_kernel (), cma_c_true) : \
	   (*(address) |= 1, cma__exit_kernel (), cma_c_false) \
    )
*/
#define cma__test_and_set(address) \
    ((cma__kernel_set (&cma__g_kernel_critical), (*(address) != 0)) ? \
        (cma__kernel_unset (&cma__g_kernel_critical), cma_c_true) : \
        (*(address) = 1, cma__kernel_unset (&cma__g_kernel_critical), \
		cma_c_false))

#define cma__unset(address) (cma__tac_clear (address))

#define cma__sigcode( code )	(code)

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
    int  r1,   r2,   r3,   r4,   r5,   r6,   r7,   r8,   r9,   r10, 
         r11,  r12,  r13,  r14,  r15,  r16,  r17,  r18,  r19,  r20, 
         r21,  r22,  r23,  r24 , r25 , r26,  r27,  r28,  r29,  r30, 
         r31,  cr11, fr0,  fr1,  fr2,  fr3,  fr4,  fr5,  fr6,  fr7, 
         fr8,  fr9,  fr10, fr11, fr12, fr13, fr14, fr15, fr16,  pc;
} cma__t_hppa_regs;

/*
 * Do not change this structure without understanding assembly
 * code in cma_assem_hp9000s800.s
 */
typedef struct CMA__T_STATIC_CT {
    cma__t_hppa_regs *sp;           /* stack pointer register */

    /*** A convenient place to stash info for cma__thread_startup to use: */
    cma_t_start_routine	start_routine;	/* Function to be executed */
    cma_t_address	arg;		/* Argument to start routine */
} cma__t_static_ctx;

/*
 * Do not change this structure without understanding assembly
 * code in cma_assem_hp9000s800.s
 */
/* N.B. this record is not currently used (???) */ 
typedef struct CMA__T_ASYNC_CTX {
    cma_t_boolean valid;
    cma_t_address interrupt_ctx;
} cma__t_async_ctx;


/*
 * GLOBAL DATA
 */

/*
 * (this is actually declared by cma_kernel.h; redeclare it here so that
 * cma_host.h can be independent of cma_kernel.h, to avoid circular
 * dependency!
 */
extern cma__t_atomic_bit        cma__g_kernel_critical; /* CMA in krnl */

int			cma__g_timer_action;
   
extern cma_t_integer	cma__g_serialize;	/* must be 16 byte aligned */

/*
 * INTERNAL INTERFACES
 */

/*
 * Return the present value of the stack pointer for the current thread
 */
extern cma_t_address
cma__fetch_sp (void);

/*
 * Atomically test (and return) low bit of longword at the specified address,
 * and set the bit.
 */
extern cma_t_boolean cma__serialize  (void);
 void cma__hppa_timer_base  (int, int, struct sigcontext*);

#ifdef __cplusplus
}
#endif

#endif /* CMA_HOST_DEFS */

/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST_HP9000S800.H */
/*  *5    18-JUL-1990 15:52:42 SCALES "Apollo BL3 changes" */
/*  *4    21-JUN-1990 12:51:57 BUTENHOF "Optimize mutexes" */
/*  *3    18-MAY-1990 07:10:08 BUTENHOF "Optimize mutex and cv" */
/*  *2     4-MAY-1990 15:15:20 SCALES "Add/update interrupt-blocking macros" */
/*  *1    17-APR-1990 07:18:22 BUTENHOF "Apollo's host header" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST_HP9000S800.H */
/*   DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST.H */
/*   *2    17-SEP-1991 14:53:12 BUTENHOF "Integrate CMA5 reverse drop" */
/*   *1     7-DEC-1990 16:23:18 SCALES "HP-specific routines" */
/*   DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST.H */










