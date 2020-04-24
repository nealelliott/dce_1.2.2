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
 * Revision 1.1.6.2  1996/02/18  23:08:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:44  marty]
 *
 * Revision 1.1.6.1  1995/12/07  21:42:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  20:58:05  root]
 * 
 * Revision 1.1.4.2  1994/06/09  13:33:20  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:24:41  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/25  19:57:29  ohara
 * 	more code cleanup
 * 	[1994/04/20  20:37:48  ohara]
 * 
 * Revision 1.1.2.2  1992/12/15  22:19:43  alan
 * 	Insert copyright notices
 * 	[1992/12/07  15:58:28  alan]
 * 
 * Revision 1.1  1992/01/19  14:39:47  devrcs
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
 *	Header file for Apollo DOMAIN/OS host-specific functions
 *
 *  AUTHORS:
 *
 *	Kevin Ackley
 *
 *  CREATION DATE:
 *
 *	4 February 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Webb Scales	3 May 1990
 *		Replace cma__lock_any and cma__unlock_any by 
 *		cma__interrupt_disable and cma__interrupt_enable.
 */


/*
 *  INCLUDE FILES
 */

#include <cma.h>

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
 * code in APOLLO68K/cma_host_assem.asm
 */
typedef struct CMA__T_STATIC_CT {
    cma__t_m68k_regs *sp;           /* stack pointer register */

    char *clnup_p;                 /* Saved copy of pfm_level.clnup_p   */
    char *clhd_p;                  /* Saved copy of pfm_level.clhd_p  */
    short inh_count;               /* Saved copy of pm_$inh_count */
    short fault_inh_count;         /* Saved copy of pm_$fault_inh_count */
    unsigned long def_cleanup_status; /* Saved copy of pfm_$deferred_cleanup_status */
    unsigned char cleanup_pending;    /* Saved copy of pm_$cleanup_pending */

    /*** A convenient place to stash info for cma__thread_startup to use: */
    cma_t_start_routine	start_routine;	/* Function to be executed */
    cma_t_address	arg;		/* Argument to start routine */
} cma__t_static_ctx;

/*
 * Do not change this structure without understanding assembly
 * code in APOLLO68K/cma_host_assem.asm
 */
typedef struct CMA__T_ASYNC_CTX {
    long   restart_pc;	  /* PC at point of signal */
    short  restart_ccr;   /* condition flags at point of signal */
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
	cma_voiddress));

/*
 * Atomically clear the low bit of longword at the specified address.
 */
extern void
cma__unset _CMA_PROTOTYPE_ ((
	void *address));
/*   DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST.H */
/*   *2    17-SEP-1991 15:05:55 BUTENHOF "Integrate CMA5 reverse drop" */
/*   *1     7-DEC-1990 16:26:48 SCALES "HP-specific routines" */
/*   DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST.H */
