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
 * Revision 1.1.6.2  1996/02/18  23:08:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:32:03  marty]
 *
 * Revision 1.1.6.1  1995/12/07  21:49:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:00:14  root]
 * 
 * Revision 1.1.4.2  1994/06/09  13:35:07  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:52  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/25  19:57:35  ohara
 * 	more code cleanup
 * 	[1994/04/20  20:38:02  ohara]
 * 
 * Revision 1.1.2.2  1992/12/15  22:22:26  alan
 * 	Insert copyright notices
 * 	[1992/12/07  16:01:04  alan]
 * 
 * Revision 1.1  1992/01/19  14:38:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
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
 *	Note:  This header file is used by *both* C and Assembler source code.
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
 *	008	Webb Scales	16 August 1990
 *		Rearranged things, moving a lot into the host-generic file.
 *	009	Dave Butenhof	18 December 1990
 *		Reintegrate things from old "host-generic file," since we've
 *		removed two-tiered cma_host mechanism.
 *	010	Dave Butenhof	09 April 1991
 *		Improve portability by adding a type for internal locks,
 *		rather than assuming it can be just an int.
 *	011	Dave Butenhof	25 April 1991
 *		Remove experimental locking and rely on O/S calls (blech).
 *	012	Dave Butenhof	07 May 1991
 *		Add cma__fetch_gp() function to set up child thread state.
 *	013	Dave Butenhof	14 May 1991
 *		Fix MP cma__t_atomic_bit macros.  When using the emulated TAC
 *		opcode on OSF/1, the "set" state is the address of the
 *		variable... static initialization to "1" doesn't work.
 */


#if !defined(__LANGUAGE_ASSEMBLY__) && !defined(LANGUAGE_ASSEMBLY)

/*
 *  INCLUDE FILES
 */

/*
 * CONSTANTS AND MACROS
 */

/*
 * Initialization for "atomic bit" objects
 */
#if _CMA_UNIPROCESSOR_
# define cma__c_tac_static_clear	0
# define cma__tac_clear(addr)		((*addr) = 0)
# define cma__tac_set(addr)		((*addr) = 1)
# define cma__tac_isset(addr)		((*addr) != 0)
#else
# define cma__c_tac_static_clear	0
# define cma__tac_clear(addr)		((*addr) = 0)
# define cma__tac_set(addr)		((*addr) = (cma__t_atomic_bit)(addr))
# define cma__tac_isset(addr)		((*addr) != 0)
#endif

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
#if 0 /* This macro is not currently needed, use the dummy in cma_host.h */
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
#if 0 /* This macro is not currently needed, use the dummy in cma_host.h */
# define cma__interrupt_enable(sig_mask) \
    (sigprocmask (SIG_SETMASK, &sig_mask, cma_c_null_ptr) == -1 ? \
	cma__bugcheck() : \
	0)
#else
# define cma__interrupt_enable(dummy)
#endif

/*
 * Perform an interlocked test-and-set operation... the argument is the
 * address of a variable of type cma__t_atomic_bit (normally a
 * cma_t_boolean), and the result is a boolean indicating the previous state
 * of the variable (i.e., when cma__test_and_set() is used as a lock, it
 * returns the value cma_c_false (0) when the lock is acquired).
 */
#if _CMA_UNIPROCESSOR_
/*
 * The uniprocessor operations use the kernel lock to create uniprocessor
 * atomic operations on a bit.  (Note that cma__unset doesn't actually need
 * to lock the kernel, since writing a single value is atomic anyway.)
 */
# define cma__kernel_set(address) \
    ((*(address) != 0) ? cma_c_true : (*(address) = 1, cma_c_false))
# define cma__test_and_set(address) \
    ((cma__kernel_set (&cma__g_kernel_critical), (*(address) != 0)) ? \
	(cma__kernel_unset (&cma__g_kernel_critical), cma_c_true) : \
	(*(address) = 1, cma__kernel_unset (&cma__g_kernel_critical), cma_c_false))
#else					/* Use VP lock for MP */
# define cma__test_and_set(address)	(cma__tas (address))
# define cma__kernel_set(address)	cma__test_and_set (address)
#endif

/*
 * Clear the low bit of longword at the specified address.
 */
#define cma__unset(address) (cma__tac_clear (address))
#define cma__kernel_unset(address) (cma__tac_clear (address))

#define cma__sigcode( code )	(code)

/*
 * TYPEDEFS
 */

typedef struct sigcontext	cma__t_sigcontext;
typedef int					cma__t_siginfo;

typedef cma_t_boolean	cma__t_atomic_bit;

typedef struct CMA__T_STATIC_CTX {
    long int	sp;
    } cma__t_static_ctx;

typedef struct CMA__T_ASYNC_CTX {
    long int	sig_mask;	/* Signal mask to be restored*/
    long int	restart_pc;	/* PC at point of signal */
    long int	used_fpc;	/* Whether this thread had used the FPC */
    long int	fpc_csr;	/* The FPC control/status register */
    long int	fpc_eir;	/* The FPC exception instruction register */
    long int	cp0_cause;	/* The CP0 exception cause register */
    long int	cp0_badvaddr;	/* The CP0 bad virtual address register */
    long int	cpu_badpaddr;	/* The cpu board bad physical address reg */
    } cma__t_async_ctx;
#endif /* LANGUAGE_C */

#if defined (LANGUAGE_ASSEMBLY) || defined (__LANGUAGE_ASSEMBLY__)
#define	AC_SIG_MASK	0
#define	AC_RESTART_PC	1
#define	AC_USED_FPC	2
#define	AC_FPC_CSR	3
#define	AC_FPC_EIR	4
#define	AC_CP0_CAUSE	5
#define	AC_CP0_BAD_VA	6
#define	AC_CPU_BAD_PA	7
#endif /* LANGUAGE_ASSEMBLY */

#if !defined(__LANGUAGE_ASSEMBLY__) && !defined(LANGUAGE_ASSEMBLY)
/*
 *  GLOBAL DATA
 */

/*
 * (this is actually declared by cma_kernel.h; redeclare it here so that
 * cma_host.h can be independent of cma_kernel.h, to avoid circular
 * dependency!
 */
extern cma__t_atomic_bit	cma__g_kernel_critical;	/* CMA in krnl */

/*
 * INTERNAL INTERFACES
 */

#if !_CMA_UNIPROCESSOR_
extern cma_t_boolean
cma__tas _CMA_PROTOTYPE_ ((cma__t_atomic_bit *addr));
#endif

/*
 * Return the present value of the stack pointer for the current thread
 */
extern cma_t_address
cma__fetch_sp _CMA_PROTOTYPE_ ((void));

extern cma_t_address
cma__fetch_gp _CMA_PROTOTYPE_ ((void));

#endif /* LANGUAGE_C */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST.H */
/*  *12   10-JUN-1991 18:57:23 SCALES "Fix sccs header" */
/*  *11   10-JUN-1991 18:09:13 SCALES "Add sccs headers for Ultrix" */
/*  *10    6-JUN-1991 11:18:24 BUTENHOF "Change language conditionals for Tin" */
/*  *9    15-MAY-1991 13:22:14 BUTENHOF "Fix bug introduced into kernel_set" */
/*  *8    14-MAY-1991 16:18:50 BUTENHOF "[-.mips]" */
/*  *7    14-MAY-1991 13:57:38 BUTENHOF "Define a cma__fetch_gp function" */
/*  *6     6-MAY-1991 17:46:00 BUTENHOF "Change test-and-set" */
/*  *5     3-MAY-1991 11:27:03 BUTENHOF "Change test-and-set" */
/*  *4    12-APR-1991 23:33:55 BUTENHOF "Change type of internal locks" */
/*  *3    18-DEC-1990 22:39:25 BUTENHOF "Make cma_host.h standalone" */
/*  *2    12-DEC-1990 20:33:37 BUTENHOF "Fix assem include, and clean up CMS history" */
/*  *1    12-DEC-1990 18:57:23 BUTENHOF "MIPS specific host defs" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST.H */
