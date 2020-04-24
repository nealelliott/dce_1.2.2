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
 * Revision 1.1.8.2  1996/02/18  23:09:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:33:03  marty]
 *
 * Revision 1.1.8.1  1995/12/07  21:55:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:03:01  root]
 * 
 * Revision 1.1.6.1  1994/06/10  20:52:41  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:49:40  devsrc]
 * 
 * Revision 1.1.4.2  1993/06/24  20:50:45  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:43:00  hinman]
 * 
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:39:50  sekhar]
 * 
 * Revision 1.1.2.2  1993/02/01  20:13:57  hinman
 * 	[hinman] - SVR4 changes
 * 	[1993/02/01  20:01:23  hinman]
 * 
 * 	[hinman] - Check in SNI revision 9.6.1.5
 * 	[1993/01/08  13:55:15  hinman]
 * 
 * Revision 9.6.1.7  92/12/21  14:25:12  raj
 * 	[raj] Roll back to pre osf merge version
 * 
 * Revision 9.6.3.2  92/12/21  14:24:07  raj
 * 	[raj] Roll back to pre osf_bug fix version.
 * 
 * Revision 9.6.1.5  92/09/30  12:39:40  root
 * 	Acceptance of OSF rev 1.1.3.2
 * 
 * Revision 9.6.1.4.1.2  92/09/30  10:31:17  hinman
 * 	Auto checkin of OSF rev 1.1.3.2
 * 
 * $EndLog$
 */


#ifndef CMA_HOST_H
#define CMA_HOST_H

/*
 *  INCLUDE FILES
 */

#define S_SP  		0	/*	offset of stack pointer in static context block */
#define S_BP  		4	/*  offset of base pointer in static context block */

#if _CMA_COMPILER_ == _CMA__SIEMENSC
# define ENTRY(x)	x proc 
# define EXIT(x)		x endp
# define IMPORT_INT(x)	extrn	x:dword
# define IMPORT_PROC(x)	extrn	x : near
# define EXPORT_PROC(x)	public x

# define TITLE	name
#else
# define	ENTRY(x)	x:
# define EXIT(x)		.align 4
#endif /* _CMA_COMPILER_ == _CMA__SIEMENSC */

/*
 * CONSTANTS AND MACROS
 */

/*
 * This platform provides an atomic test-and-set instruction.
 * Therefore define this switch to make cma_host.h set up the
 * macros and routine prototypes correctly.
 */
#define _CMA_HOST_ATOMIC_TAS_	1

/*
 * Initialization for "atomic bit" objects
 */
#define cma__c_tac_static_clear	0
#define cma__tac_clear(addr)	(*(addr) = 0)
#define cma__tac_set(addr)		(*(addr) = 1)
#define cma__tac_isset(addr)	(*(addr) != 0)

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
#define _CMA_DECODE_SIGNALS_    1


/*
 * Disable interrupts (signals)
 */
#define cma__interrupt_disable(dummy)

/*
 * Enable interrupts (signals)
 */
#define cma__interrupt_enable(dummy)


/*
 * Clear interrupt context (to allow new interrupts to come in)
 */
#  define cma__clear_interrupt(sig_mask) \
    (sigprocmask (SIG_SETMASK, sig_mask, cma_c_null_ptr) == -1 ? \
        cma__bugcheck("cma__clear_interrupt, mask = %x", sig_mask), 0 : \
        0)


/*
 * The following four operations implement "atomic test-and-set" and "atomic
 * clear" operations, on the low bit of a specified address.
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
 * Platform has an atomic test-and-set primitive
 */

#define cma__kernel_set(address)    cma__test_and_set ((address))
#define cma__kernel_unset(address)  cma__unset ((address))


/*
 * TYPEDEFS
 */

#ifndef LANGUAGE_ASSEMBLY
/*
*	Static context block for machine state.
*/

typedef int	cma__t_atomic_bit;
typedef struct CMA__T_STATIC_CTX {
	unsigned int sp;
	unsigned int bp;
    	} cma__t_static_ctx;

/*
*	Asynchronous context block for machine state.
*/

typedef struct CMA__T_ASYNC_CTX {
    cma_t_boolean   valid;
    cma_t_address   interrupt_ctx;
    } cma__t_async_ctx;



/*
*	This type holds a set of unordered data in it.  The current implementation
*	will only allow one data member in it.  If it is necessary to 
*	support more data members, then the implementation will have to
*	change.
*/

typedef	unsigned int cma__t_set;
#endif 
/*
 *  GLOBAL DATA
 */

/*
 * INTERNAL INTERFACES
 */

#ifndef LANGUAGE_ASSEMBLY

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
	cma__t_atomic_bit	*address));

/*
 * Atomically clear the low bit of longword at the specified address.
 */
extern void
cma__unset _CMA_PROTOTYPE_ ((
	cma__t_atomic_bit	*address));

#endif /* LANGUAGE_ASSEMBLY */

#endif /* CMA_HOST_H */

/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST.H */
/*  *8    17-AUG-1990 10:35:25 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *7    17-AUG-1990 02:04:09 SCALES "Apollo changes; Sun & HPPA port" */
/*  *6     3-JUL-1990 07:33:46 BUTENHOF "Change copyright header" */
/*  *5    21-JUN-1990 12:51:47 BUTENHOF "Optimize mutexes" */
/*  *4     6-MAR-1990 09:12:29 BUTENHOF "Integrate Apollo changes" */
/*  *3     9-FEB-1990 11:09:12 BUTENHOF "Convert to stream format" */
/*  *2    23-JAN-1990 14:07:12 BUTENHOF "Make function prototypes conditional" */
/*  *1    19-OCT-1989 15:56:06 SCALES "Host-specific include file" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST.H */
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

