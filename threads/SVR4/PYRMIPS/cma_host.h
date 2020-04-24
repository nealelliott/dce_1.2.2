/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 * All Rights Reserved
 * (c) Copyright 1990, 1991, 1992
 */
/*
 * HISTORY
 * $Log: cma_host.h,v $
 * Revision 1.1.8.2  1996/02/18  23:08:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:32:23  marty]
 *
 * Revision 1.1.8.1  1995/12/07  21:52:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:01:58  root]
 * 
 * Revision 1.1.6.1  1994/06/09  13:36:06  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:26:32  devsrc]
 * 
 * Revision 1.1.3.3  1992/12/15  22:23:51  alan
 * 	Insert copyright notices
 * 	[1992/12/07  16:07:55  alan]
 * 
 * Revision 1.1.3.2  1992/09/29  21:25:37  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:38:02  sekhar]
 * 
 * $EndLog$
 */
/*
** (c) Copyright 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
** ALL RIGHTS RESERVED
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
 *	Header file for MIPS/DCOSX host-specific functions
 *
 *	Note:  This header file is used by *both* C and Assembler source code.
 *
 *  AUTHORS:
 *
 *	David Bein and Bill Rees
 *
 *  CREATION DATE:
 *
 *	30 July 1992
 *
 *  MODIFICATION HISTORY:
 */


#ifndef __CMA_HOST_H__
#define __CMA_HOST_H__

#if !defined(__LANGUAGE_ASSEMBLY__) && !defined(LANGUAGE_ASSEMBLY)
/*
 *  INCLUDE FILES
 */
#include <ucontext.h>
/*
 * CONSTANTS AND MACROS
 */

/*
 * Initialization for "atomic bit" objects:
 *      NOTE:   
 *			On an R3000 platform there is no such
 *		thing as an "atomic" test and set.  For threads 
 *		truely parallel threads ( kernel knows about them),
 *		THIS WILL CAUSE PROBLEMS.  Some form of atomicity
 *		will have to be provided.
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
# define cma__interrupt_disable(dummy)   /* SAS */
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
# define cma__interrupt_enable(dummy)   /* SAS */
#endif

   /*
   ** redefine cma_pyrmips_syscall here, but later add the call to
   ** cma_assem.s as an assembly routine call to the syscall interface.
   */
#define cma_pyrmips_syscall(a,b,c,d,e)  syscall(a,b,c,d,e)

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

/*
 * Remove this comment if you want inline insecure ops.
 *
 * #define PYRMIPS_QUICK_TSTNSET  1
 */

#ifdef PYRMIPS_QUICK_TSTNSET     /* set and test segment start....*/
# define cma__kernel_set(address) \
    ((*(address) != 0) ? cma_c_true : (*(address) = 1, cma_c_false))
# define cma__test_and_set(address) \
    ((cma__kernel_set (&cma__g_kernel_critical), (*(address) != 0)) ? \
	(cma__kernel_unset (&cma__g_kernel_critical), cma_c_true) : \
	(*(address) = 1, cma__kernel_unset (&cma__g_kernel_critical), cma_c_false))

#else  /* old definition for testing and setting... here is the new one */

extern   int  cma__blk_tst_n_set();
extern   int  cma__pyrmips_kclear();

#define cma__kernel_set(address) (cma__blk_tst_n_set(address))
#define cma__test_and_set(address) (cma__kernel_set(address))

#endif     /* set and test segment  */

#else					/* Use VP lock for MP */
# define cma__test_and_set(address)	(cma__tas (address))
# define cma__kernel_set(address)	cma__test_and_set (address)
#endif

#define cma__set_init( set_ptr )        (*set_ptr = NODEV)
#define cma__set_insert( set_ptr, set_value ) (*set_ptr = set_value)
#define cma__set_in( set, set_value )   ( (set == set_value) ? cma_c_true : cma_c_false )

/*
 * Clear the low bit of longword at the specified address.
 */
#define cma__unset(address) (cma__tac_clear (address))
#if 0	/* For debugging */
#define cma__kernel_unset(address) cma__pyrmips_kclear(address)
#else
#define cma__kernel_unset(address) cma__unset(address)
#endif
/*
 * TYPEDEFS
 */
typedef unsigned int cma__t_set;   /* this is from SVR4/cma_host.h !@!  */

typedef cma_t_boolean	cma__t_atomic_bit;

typedef struct CMA__T_STATIC_CTX {
    long int	sp;
    } cma__t_static_ctx;

typedef ucontext_t   cma__t_async_ctx;        /* ucontext info */
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

#endif	/* !__CMA_HOST_H__ */

