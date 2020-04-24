/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_mutex.h $Revision: 1.1.12.1 $ $Date: 1996/05/10 13:39:27 $
 */
/*
 * HISTORY
 * $Log: cma_mutex.h,v $
 * Revision 1.1.12.1  1996/05/10  13:39:27  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:43 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:43 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:13  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:06  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  16:23  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  13:18:58  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:47  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:13:00  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:30:07  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:34:07  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:39:58  alan]
 *
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:49:47  sekhar]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:05:04  bolinger]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:35:20  bolinger]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:34:43  mgm  1.1.1.11]
 *
 * Revision 1.1.7.2  1994/06/09  13:38:20  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:13  devsrc]
 * 
 * Revision 1.1.7.1  1994/04/01  20:16:47  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:06  jd]
 * 
 * Revision 1.1.2.4  1992/12/15  22:29:06  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:39:58  alan]
 * 
 * Revision 1.1.2.3  1992/09/29  20:15:10  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:49:47  sekhar]
 * 
 * Revision 1.1.2.2  1992/09/03  14:38:45  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:05:04  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:35:36  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:35:20  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:39:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *	%W%	(ULTRIX/OSF)	%G%
 */
/*
 *  Copyright (c) 1989, 1992 by
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
 *	Header file for mutex operations
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	3 August 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	14 September 1989
 *		Add semaphore to mutex.
 *	002	Dave Butenhof	04 October 1989
 *		Implement internal mutex lock/unlock using object pointers
 *		instead of handles for convenient internal use.
 *	003	Dave Butenhof	1 November 1989
 *		Define queue of active mutexes.
 *	004	Dave Butenhof	22 November 1989
 *		Add "owner" field so debug code can report current owner of
 *		mutex.
 *	005	Dave Butenhof	4 December 1989
 *		Include cma_tcb_defs.h instead of cma_tcb.h
 *	006	Dave Butenhof	9 April 1990
 *		Remove definition of known mutex queue header; it's now
 *		in cma_debugger.
 *	007	Dave Butenhof	15 June 1990
 *		Streamline mutex performance by using interlocked bit;
 *		semaphores will be used only to block the thread if the mutex
 *		is already locked.
 *	008	Dave Butenhof	26 June 1990
 *		Implement "friendly" mutexes which support nested locking by
 *		the same thread.  Use these to implement a new "global lock"
 *		primitive for use in synchronizing non-reentrant libraries.
 *	009	Dave Butenhof	31 July 1990
 *		Move internal mutex operations to .h so they can be inlined
 *		everywhere within CMA.
 *	010	Dave Butenhof	14 December 1990
 *		Change cma_attributes.h to cma_attr.h (shorten length)
 *	011	Dave Butenhof	12 December 1991
 *		Change comments to reflect change of "friendly" mutex to
 *		"recursive".
 *	012	Dave Butenhof	09 April 1991
 *		Use new type for "atomic bit" operation target
 *	013	Paul Curtin	10 May 1991
 *		Added a couple of new macros.
 *	014	Paul Curtin	30 May 1991
 *		Added a proto for cma__reinit_mutex
 *	015	Dave Butenhof	09 March 1992
 *		Modify cma__int_[un]lock to handle return status from
 *		cma__int_mutex_[un]block(), raising exception on failure.
 */

#ifndef CMA_MUTEX
#define CMA_MUTEX

/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_attr.h>
#include <cma_defs.h>
#include <cma_semaphore_defs.h>
#include <cma_sequence.h>
#include <cma_tcb_defs.h>
#include <cma_stack.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

/*
 * TYPEDEFS
 */

typedef struct CMA__T_INT_MUTEX {
    cma__t_object	header;		/* Common header (sequence, type) */
    cma__t_int_attr	*attributes;	/* Back link */
    cma__t_int_tcb	*owner;		/* Current owner (if any) */
    cma_t_integer	nest_count;	/* Nesting level for recursive mutex */
    cma__t_atomic_bit	*unlock;	/* Pointer used for unlock operation */
    cma__t_atomic_bit	lock;		/* Set if currently locked */
    struct CMA__T_INT_MUTEX *int_lock;	/* Internal protection for mutex */
    cma__t_atomic_bit	event;		/* Clear when unlock requires action */
    cma__t_atomic_bit	waiters;	/* Clear when threads are waiting */
    cma__t_atomic_bit	bitbucket;	/* Fake bit to keep friendlies locked */
    cma_t_mutex_kind	mutex_kind;	/* Kind of mutex */
    cma__t_semaphore	semaphore;	/* Semaphore for low-level wait */
    } cma__t_int_mutex;


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Lock a mutex (internal)
 *
 *  FORMAL PARAMETERS:
 *
 *	mutex		Pointer to mutex object to lock
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
#ifdef NDEBUG
# define cma__int_lock(mutex) { \
    if (cma__test_and_set (&((cma__t_int_mutex *)mutex)->lock)) { \
	cma_t_status	res;\
	res = cma__int_mutex_block ((cma__t_int_mutex *)mutex); \
	if (res != cma_s_normal) cma__error (res); \
	} \
    }
#else
# define cma__int_lock(mutex) { \
    cma__t_int_tcb *__ltcb__; \
    __ltcb__ = cma__get_self_tcb (); \
    if (cma__test_and_set (&((cma__t_int_mutex *)mutex)->lock)) { \
	cma_t_status	res;\
	res = cma__int_mutex_block ((cma__t_int_mutex *)mutex); \
	if (res != cma_s_normal) cma__error (res); \
	} \
    ((cma__t_int_mutex *)mutex)->owner = __ltcb__; \
    }
#endif

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Unlock a mutex (internal)
 *
 *  FORMAL PARAMETERS:
 *
 *	mutex		Pointer to mutex object to unlock
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
#ifdef NDEBUG
# define cma__int_unlock(mutex) { \
    cma__unset (((cma__t_int_mutex *)mutex)->unlock); \
    if (!cma__test_and_set (&((cma__t_int_mutex *)mutex)->event)) { \
	cma_t_status	res;\
	res = cma__int_mutex_unblock ((cma__t_int_mutex *)mutex); \
	if (res != cma_s_normal) cma__error (res); \
	} \
    }
#else
# define cma__int_unlock(mutex) { \
    cma__t_int_tcb	*__utcb__; \
    __utcb__ = cma__get_self_tcb (); \
    if (((cma__t_int_mutex *)mutex)->mutex_kind == cma_c_mutex_fast) { \
	cma__assert_warn ( \
		(__utcb__ == ((cma__t_int_mutex *)mutex)->owner), \
		"attempt to release mutx owned by another thread"); \
	((cma__t_int_mutex *)mutex)->owner = (cma__t_int_tcb *)cma_c_null_ptr; \
	} \
    cma__unset (((cma__t_int_mutex *)mutex)->unlock); \
    if (!cma__test_and_set (&((cma__t_int_mutex *)mutex)->event)) { \
	cma_t_status	res;\
	res = cma__int_mutex_unblock ((cma__t_int_mutex *)mutex); \
	if (res != cma_s_normal) cma__error (res); \
	} \
    }
#endif

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__int_mutex_delete - Performs work for cma_mutex_delete
 *
 *  FORMAL PARAMETERS:
 *
 *	cma__t_mutex	    _mutex_	- Mutex to be deleted
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
#define cma__int_mutex_delete(_mutex_) { \
    cma__t_int_mutex    *_int_mutex_; \
    _int_mutex_ = cma__validate_null_mutex (_mutex_); \
    if (_int_mutex_ == (cma__t_int_mutex *)cma_c_null_ptr) \
        return; \
    if (cma__int_mutex_locked (_int_mutex_)) \
        cma__error (cma_s_in_use); \
    cma__free_mutex (_int_mutex_); \
    cma__clear_handle (_mutex_); \
    }


/*
 *  GLOBAL DATA
 */

extern cma__t_sequence	cma__g_mutex_seq;
extern cma__t_int_mutex	*cma__g_global_lock;

/*
 * INTERNAL INTERFACES
 */

extern void cma__destroy_mutex  (cma__t_int_mutex *);

extern void cma__free_mutex  (cma__t_int_mutex *);

extern void cma__free_mutex_nolock  (cma__t_int_mutex *);

extern cma__t_int_mutex * cma__get_first_mutex  (cma__t_int_attr *);

extern cma__t_int_mutex * cma__get_mutex  (cma__t_int_attr *);

extern void cma__init_mutex  (void);

extern cma_t_status cma__int_mutex_block  (cma__t_int_mutex *);

extern cma_t_boolean cma__int_mutex_locked  (cma__t_int_mutex *);

extern cma_t_boolean cma__int_try_lock  (cma__t_int_mutex *);

extern cma_t_status cma__int_mutex_unblock  (cma__t_int_mutex *);

extern cma_t_boolean cma__mutex_locked  (cma_t_mutex);

extern void cma__reinit_mutex  (cma_t_integer);

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_MUTEX.H */
/*  *12   10-MAR-1992 16:26:18 BUTENHOF "Eliminate need for TRY/CATCH on pthread mutex lock" */
/*  *11   18-FEB-1992 15:29:33 BUTENHOF "Remove macros for create object" */
/*  *10   10-JUN-1991 19:54:29 SCALES "Convert to stream format for ULTRIX build" */
/*  *9    10-JUN-1991 19:21:09 BUTENHOF "Fix the sccs headers" */
/*  *8    10-JUN-1991 18:22:30 SCALES "Add sccs headers for Ultrix" */
/*  *7     5-JUN-1991 16:13:23 CURTIN "fork work" */
/*  *6    10-MAY-1991 11:46:33 CURTIN "Added a couple of new macros" */
/*  *5    12-APR-1991 23:36:12 BUTENHOF "Change type of internal locks" */
/*  *4    13-FEB-1991 17:54:51 BUTENHOF "Change name of mutex attributes" */
/*  *3    12-FEB-1991 23:09:58 BUTENHOF "Recursive/nonrecursive mutexes" */
/*  *2    14-DEC-1990 00:55:43 BUTENHOF "Change module names" */
/*  *1    12-DEC-1990 21:47:32 BUTENHOF "Mutexes" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_MUTEX.H */
