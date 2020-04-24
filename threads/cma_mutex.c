/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_mutex.c,v $
 * Revision 1.1.11.1  1996/05/10  13:39:15  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/04/22  19:35 UTC  bissen
 * 	Backout fix for CHFts17707
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:43 UTC  bissen
 * 	Merge from Hpdce02_01
 * 	[1994/08/22  18:44 UTC  mothra  /main/HPDCE02/2]
 *
 * 	Change the check to see if the calling thread is self for a fast mutex
 * 	from a warning to a failure.
 * 	[1994/06/28  17:29:16  jd]
 * 	 *
 *
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:43 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:11  devsrc]
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:03  jd]
 *
 * 	Merge for RODAN cycle T bsu
 * 	[1994/06/16  17:25 UTC  jss  /main/HPDCE01/2]
 *
 * 	If cma_mutex() blocks, another thread could have destroyed the mutex variable
 * 	before it returns.  This change tests the mutex handle after returning from
 * 	blocking to see if it is still valid.
 * 	[1994/06/16  15:52 UTC  jss  /main/HPDCE01/jss_rodan_r/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  16:26  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  13:15:17  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:45  root  1.1.6.3]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:28:28  root]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:39:51  alan]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:04:57  bolinger]
 *
 * 	Make cma__g_global_lock get initialized at
 * 	run-time in cma_init() instead of statically
 * 	here.  This has the effect of moving it from
 * 	DATA to BSS, which is required for backwards
 * 	compatibility with binaries linked against
 * 	old versions of libdce.sl.  Note that we have
 * 	to make sure it gets initialized to zero
 * 	_before_ anyone [like cma__alloc_mem()] calls
 * 	cma___get_area().
 * 	[1993/06/18  18:29:25  hopkins]
 *
 * 	Merge up to BL10+ internal release from DEC.
 * 	[1992/08/11  20:56:28  bolinger]
 *
 * 	Bring forward 1.0.1 revision 1.1.2.2 (fix for OT defect 2101)
 * 	and 1.1.2.3 (fix for OT defect 2109).
 * 	[1992/08/10  15:24:42  bolinger]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:34:28  bolinger]
 *
 * 	Address defect 2109.  Added default clause to swithc() in cma__int_mutex_locked.
 * 	This clause bugchecks, since an unexpected mutex kind is never expected.
 * 	The default action also contains a return statement to keep the gcc compiler happy.
 * 	[1992/05/13  16:55:48  keane]
 *
 * 	Fix OT defect 2101: initialize mutex more fully in
 * 	cma__get_first_mutex().
 * 	[1992/04/24  16:59:42  bolinger]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:34:39  mgm  1.1.1.11]
 *
 * Revision 1.1.7.3  1994/06/28  19:49:28  jd
 * 	Change the check to see if the calling thread is self for a fast mutex
 * 	from a warning to a failure.
 * 	[1994/06/28  17:29:16  jd]
 * 
 * Revision 1.1.7.2  1994/06/09  13:38:18  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:11  devsrc]
 * 
 * Revision 1.1.7.1  1994/04/01  20:16:44  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:03  jd]
 * 
 * Revision 1.1.4.3  1992/12/15  22:29:01  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:39:51  alan]
 * 
 * Revision 1.1.4.2  1992/09/03  14:38:39  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:04:57  bolinger]
 * 
 * Revision 1.1.2.3  1992/08/14  20:21:46  bolinger
 * 	Merge up to BL10+ internal release from DEC.
 * 	[1992/08/11  20:56:28  bolinger]
 * 
 * 	Bring forward 1.0.1 revision 1.1.2.2 (fix for OT defect 2101)
 * 	and 1.1.2.3 (fix for OT defect 2109).
 * 	[1992/08/10  15:24:42  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:34:47  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:34:28  bolinger]
 * 
 * Revision 1.1.2.3  1992/05/13  18:58:08  keane
 * 	Address defect 2109.  Added default clause to swithc() in cma__int_mutex_locked.
 * 	This clause bugchecks, since an unexpected mutex kind is never expected.
 * 	The default action also contains a return statement to keep the gcc compiler happy.
 * 	[1992/05/13  16:55:48  keane]
 * 
 * Revision 1.1.2.2  1992/04/24  18:00:21  bolinger
 * 	Fix OT defect 2101: initialize mutex more fully in
 * 	cma__get_first_mutex().
 * 	[1992/04/24  16:59:42  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:40:42  devrcs
 * 	Initial revision
 * 
 * $EndLog$
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
 *	Operations on mutex object
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	21 August 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	18 September 1989
 *		Implement simple mutex on top of dispatcher's semaphores.
 *	002	Dave Butenhof	04 October 1989
 *		Implement internal mutex lock/unlock using object pointers
 *		instead of handles for convenient internal use.
 *	003	Dave Butenhof	9 October 1989
 *		Use cma__error to raise exceptions where necessary.
 *	004	Dave Butenhof	11 October 1989
 *		Convert to use internal mutex operations.
 *	005	Dave Butenhof	18 October 1989
 *		cma__queue_insert is now a macro, which expands to a block;
 *		this module includes a call which is the sole statement on
 *		the "then" of an if statement, and the trailing ";" (after a
 *		"}") breaks the "else" clause.  Fix the reference in such a
 *		way that some future conversion back to a routine call won't
 *		break it again.
 *	006	Dave Butenhof	19 October 1989
 *		Use new type-specific handle validation macros.
 *	007	Dave Butenhof	19 October 1989
 *		Substitute "cma_t_address" for explicit "void *" to make
 *		porting easier.
 *	008	Dave Butenhof	19 October 1989
 *		Modify use of queue operations to use appropriate casts
 *		rather than depending on ANSI C "void *" (sigh).
 *	009	Webb Scales 	20 October 1989
 *		Put in typecasts as required by MIPS/Ultrix C
 *	010	Dave Butenhof	24 October 1989
 *		Enhance free & destroy to handle attr. obj. delete-pending
 *		and refcnt.
 *	011	Dave Butenhof	25 October 1989
 *		Fix a deadlock in previous changes to free code.
 *	012	Dave Butenhof	1 November 1989
 *		- Put active mutexes onto a queue, and record waiting mutex
 *		in
 *		  TCB for debugging.
 *		- Make mutex sequence variable external so init_static can
 *		  set it up for get_first_mutex use (non-interlocked).
 *	013	Dave Butenhof	22 November 1989
 *		Add support for new "owner" field of mutex so that debug
 *		"mutex" command can report current owner of lock.
 *	014	Dave Butenhof	30 November 1989
 *		Modify external entries to track POSIX changes to names and
 *		argument ordering.
 *	015	Dave Butenhof	26 January 1990
 *		Change cma__get_self_tcb_kernel to cma__get_self_tcb (it no
 *		longer locks the kernel, so distinction is irrelevant).
 *	016	Dave Butenhof	9 April 1990
 *		Use new "known_object" structure for known mutex queue
 *		(includes mutex).
 *	017	Dave Butenhof	10 April 1990
 *		Catch memory errors over object allocation, and set names in
 *		internal objects.
 *	018	Dave Butenhof	11 April 1990
 *		Fix mutex ownership (assign after decrement_semaphore).
 *	019	Dave Butenhof	5 June 1990
 *		Support mutex cache high water mark.
 *	020	Dave Butenhof	7 June 1990
 *		Erase destroyed objects if not NDEBUG, to catch references to
 *		dead objects (can be used in conjunction with cma_maxmutex=0,
 *		which disables caching).
 *	021	Dave Butenhof	15 June 1990
 *		Streamline mutex performance by using interlocked bit;
 *		semaphores will be used only to block the thread if the mutex
 *		is already locked.
 *	022	Dave Butenhof	18 June 1990
 *		Use macros to clear object name (only defined for debug
 *		build).
 *	023	Dave Butenhof	26 June 1990
 *		Implement "friendly" mutexes which support nested locking by
 *		the same thread.  Use these to implement a new "global lock"
 *		primitive for use in synchronizing non-reentrant libraries.
 *	024	Dave Butenhof	06 July 1990
 *		Fix missing ';' in NDEBUG conditional code.
 *	025	Dave Butenhof	31 July 1990
 *		Move internal mutex operations to .h so they can be inlined
 *		everywhere within CMA.
 *	026	Paul Curtin	22 August 1990
 *		Removed reference to cma__alloc_mem_nolock.
 *	027	Dave Butenhof	27 August 1990
 *		Change interfaces to pass handles & structures by reference.
 *	028	Webb Scales	30 October 1990
 *		Removed uses of external handle fields.
 *	029	Webb Scales	30 October 1990
 *		Added casts to change #28.
 *	030	Bob Conti	5 November 1990
 *		Restore mutex debugging by setting the wait_mutex field even
 *		in production builds.
 *	031	Dave Butenhof	12 February 1991
 *		Change "friendly" to "recursive"
 *	032	Dave Butenhof	09 April 1991
 *		Use new constants to init atomic bits
 *	033	Paul Curtin	10 May 1991
 *		Use new internal macros added in cma_mutex.h
 *	034	Dave Butenhof	20 May 1991
 *		Fix a possible race condition in cma__int_mutex_block, by
 *		clearing mutex "event" flag after entering kernel.
 *	035	Paul Curtin	30 May 1991
 *		Added a reinit routine for pre/post fork() work.
 *	036	Dave Butenhof	28 June 1991
 *		Initialize the owner field of a mutex to be sure.
 *	037	Dave Butenhof	05 November 1991
 *		Fix a bug in cma__get_mutex pointed out by Peter Burgess of
 *		MCC; memory allocation may not always be zeroed, and
 *		recursive mutex setup assumes that a raw mutex's internal
 *		lock pointer will be zero.
 *	038	Dave Butenhof	11 February 1992
 *		A law of nature has just been changed: cma__alloc_mem now
 *		returns cma_c_null_ptr on an allocation failure, rather than
 *		raising an exception. This allows greater efficiency, since
 *		TRY is generally expensive. Anyway, apply the process of
 *		evolution: adapt or die.
 *	039	Dave Butenhof	19 February 1992
 *		Fix typo in alloc_object call (typing).
 *	040	Dave Butenhof	10 March 1992
 *		Changes to cma__int_[un]block_mutex to return status instead
 *		of raising exceptions. Handle new interface in lock/unlock.
 *	041	Dave Butenhof	12 March 1992
 *		Clean up 040 a little.
 *	042	Dave Butenhof	05 June 1992
 *		Fix delete_pending attr handling in free code -- it needs to
 *		free the attr. obj. instead of destroying it (which also
 *		fixes a race in attr. handling).
 */

/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_defs.h>
#include <cma_mutex.h>
#include <cma_handle.h>
#include <cma_vm.h>
#include <cma_sequence.h>
#include <cma_semaphore.h>
#include <cma_errors.h>
#include <cma_stack.h>
#include <cma_queue.h>
#include <cma_dispatch.h>
#include <cma_deb_core.h>
#include <cma_init.h>

/*
 * GLOBAL DATA
 */

cma__t_sequence	 cma__g_mutex_seq;
#ifdef __hpux
cma__t_int_mutex *cma__g_global_lock;
#else
cma__t_int_mutex *cma__g_global_lock = (cma__t_int_mutex *)cma_c_null_ptr;
#endif

/*
 * LOCAL MACROS
 */

/*
 * LOCAL DATA
 */

/*
 * LOCAL FUNCTIONS
 */


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Lock the global library mutex.
 *
 *  FORMAL PARAMETERS:
 *
 *	none
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
extern void
cma_lock_global(void)
    {
    cma__int_lock (cma__g_global_lock);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Create a new mutex object.
 *
 *  FORMAL PARAMETERS:
 *
 *	new_mutex	Output handle
 *
 *	att		Attributes object to use in creation
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
extern void
cma_mutex_create(
	cma_t_mutex	*new_mutex,	/* New handle to fill in */
	cma_t_attr	*att)		/* Old attr obj to use */
    {
    cma__t_int_mutex    *mutex;
    cma__t_int_attr     *int_att;


    int_att = cma__validate_default_attr (att);
    mutex = cma__get_mutex (int_att);

    if ((cma_t_address)mutex == cma_c_null_ptr)
	cma__error (exc_s_insfmem);
    else
	cma__object_to_handle ((cma__t_object *)mutex, new_mutex);

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Delete (free) a mutex object
 *
 *  FORMAL PARAMETERS:
 *
 *	mutex		Mutex object to free
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
extern void
cma_mutex_delete(
	cma_t_mutex	*mutex)		/* Mutex to free */
    {
    cma__int_mutex_delete (mutex);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Lock a mutex
 *
 *  FORMAL PARAMETERS:
 *
 *	mutex		Mutex object to lock
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
extern void
cma_mutex_lock(
	cma_t_mutex	*mutex)		/* Mutex to lock */
    {
    cma__t_int_tcb	*tcb;
    cma__t_int_mutex	*int_mutex;

    /*
     * Get a pointer to the mutex structure; if this is a debugging CMA,
     * we'll validate the mutex handle to be sure it's valid.  For
     * performance, if it's an NDEBUG ("production") CMA, just fetch the
     * address of the object from the handle's pointer field.
     */
#ifdef NDEBUG
    int_mutex = (cma__t_int_mutex *)((cma__t_int_handle *)mutex)->pointer;
#else
    int_mutex = cma__validate_mutex (mutex);
    tcb = cma__get_self_tcb ();
#endif

    /*
     * First, try to acquire the lock; if we get it, then we're done
     */
    if (cma__test_and_set (&int_mutex->lock)) {
	cma_t_status	res;


	res = cma__int_mutex_block (int_mutex);
        /*
	 * CHFts08244. Its possible that we returned here after the mutex
	 * object was destroyed by another thread. Check that it is still 
	 * valid
	 */
	if (((cma__t_int_handle *)mutex)->pointer == cma_c_null_ptr)
	  res = cma_s_existence;

	if (res != cma_s_normal)
	    cma__error (res);

	}

#ifndef NDEBUG
    int_mutex->owner = tcb;
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Try to lock a mutex, but return immediately with boolean status if
 *	the mutex is already locked
 *
 *  FORMAL PARAMETERS:
 *
 *	mutex		Mutex object to lock
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
extern cma_t_boolean
cma_mutex_try_lock(
	cma_t_mutex	*mutex)		/* Mutex to lock */
    {
    cma__t_int_mutex	*int_mutex;
    cma__t_int_tcb	*tcb;


#ifdef NDEBUG
    int_mutex = (cma__t_int_mutex *)((cma__t_int_handle *)mutex)->pointer;
#else
    int_mutex = cma__validate_mutex (mutex);
    tcb = cma__get_self_tcb ();
#endif

    if (cma__test_and_set (&int_mutex->lock)) {

	switch (int_mutex->mutex_kind) {
	    case cma_c_mutex_nonrecursive : {
#ifdef NDEBUG
		tcb = cma__get_self_tcb ();
#endif
		cma__int_lock (int_mutex->int_lock);

		if (int_mutex->owner == (cma__t_int_tcb *)cma_c_null_ptr) {
		    int_mutex->owner = tcb;
		    cma__int_unlock (int_mutex->int_lock);
		    return cma_c_true;
		    }
		else {
		    cma__int_unlock (int_mutex->int_lock);
		    return cma_c_false;
		    }

		break;
		}

	    case cma_c_mutex_recursive : {
#ifdef NDEBUG
		tcb = cma__get_self_tcb ();
#endif
		cma__int_lock (int_mutex->int_lock);

		if (tcb == int_mutex->owner
			|| int_mutex->owner == (cma__t_int_tcb *)cma_c_null_ptr) {
		    int_mutex->nest_count++;
		    int_mutex->owner = tcb;
		    cma__int_unlock (int_mutex->int_lock);
		    return cma_c_true;
		    }
		else {
		    cma__int_unlock (int_mutex->int_lock);
		    return cma_c_false;
		    }

		break;
		}

	    case cma_c_mutex_fast : {
		return cma_c_false;
		break;
		}

	    }

	}

#ifndef NDEBUG
    int_mutex->owner = tcb;
#endif
    return cma_c_true;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Unlock a mutex
 *
 *  FORMAL PARAMETERS:
 *
 *	mutex		Mutex object to unlock
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
extern void
cma_mutex_unlock(
	cma_t_mutex	*mutex)		/* Mutex to unlock */
    {
    cma__t_int_mutex	*int_mutex;

#ifdef NDEBUG
    int_mutex = (cma__t_int_mutex *)((cma__t_int_handle *)mutex)->pointer;
#else
    cma__t_int_tcb	*tcb;

    int_mutex = cma__validate_mutex (mutex);
    tcb = cma__get_self_tcb ();

    if (int_mutex->mutex_kind == cma_c_mutex_fast) {
	cma__assert_warn (
		(tcb == int_mutex->owner),
		"attempt to release mutex owned by another thread");
	int_mutex->owner = (cma__t_int_tcb *)cma_c_null_ptr;
	}

#endif
    cma__unset (int_mutex->unlock);

    /*
     * Check whether there might be waiters, and reset the bit (TRUE means
     * "no waiters").  If there might be waiters, increment the semaphore to
     * release one.
     */
    if (!cma__test_and_set (&int_mutex->event)) {
	cma_t_status	res;


	res = cma__int_mutex_unblock (int_mutex);

	if (res != cma_s_normal)
	    cma__error (res);

	}

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Unlock the global library mutex.
 *
 *  FORMAL PARAMETERS:
 *
 *	none
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
extern void
cma_unlock_global(void)
    {
    cma__int_unlock (cma__g_global_lock);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Deallocate a mutex structure---don't try to cache it (this is
 *	used to remove a mutex from a cache list!)
 *
 *  FORMAL PARAMETERS:
 *
 *	old_mutex	Address of the mutex object
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
extern void
cma__destroy_mutex(
	cma__t_int_mutex	*old_mutex) /* The mutex obj to delete */
    {
    /*
     * Free all the objects which are contained in the mutex obj, and
     * then deallocate the object's memory.  THIS ROUTINE ASSUMES THAT THE
     * PARENT ATTRIBUTES OBJECT IS LOCKED, AND THAT THE CALLER MANAGES THE
     * CACHE LINKS.
     */
    cma__assert_warn (
	    cma__int_mutex_locked (old_mutex->attributes->mutex),
	    "cma__destroy_mutex called without attributes object locked.");

    /*
     * Also assert that the mutex being destroyed is unlocked, just to be
     * safe...
     */
    cma__assert_warn (
	    !cma__int_mutex_locked (old_mutex),
	    "cma__destroy_mutex called for locked mutex.");

    if (old_mutex->mutex_kind == cma_c_mutex_recursive
	    || old_mutex->mutex_kind == cma_c_mutex_nonrecursive)
	cma__free_mutex (old_mutex->int_lock);

    cma__free_mem ((cma_t_address)old_mutex);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Free a mutex object to the cache list
 *
 *  FORMAL PARAMETERS:
 *
 *	old_mutex	Mutex object to free
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
extern void
cma__free_mutex(
	cma__t_int_mutex	*old_mutex) /* Mutex object to free */
    {
    cma__t_int_attr	*attrib;


    attrib = old_mutex->attributes;
    cma__int_lock (attrib->mutex);
    cma__free_mutex_nolock (old_mutex);

    if ((attrib->refcnt == 0) && (attrib->delete_pending)) {
	cma__int_unlock (attrib->mutex);
	cma__free_attributes (attrib);
	}
    else
	cma__int_unlock (attrib->mutex);

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Free a mutex object to the cache list (without locking attr. obj:
 *	used where the object is already known to be locked).
 *
 *  FORMAL PARAMETERS:
 *
 *	old_mutex	Mutex object to free
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
extern void
cma__free_mutex_nolock(
	cma__t_int_mutex	*old_mutex) /* Mutex object to free */
    {
    cma__t_int_attr	*attrib;	/* Point to controlling attr obj */


    /*
     * Add to the cached attributes list
     */
    attrib = old_mutex->attributes;

    /*
     * Assert that the attributes object is locked.
     */
    cma__assert_warn (
	    cma__int_mutex_locked (attrib->mutex),
	    "cma__free_mutex_nolock called without attributes object locked.");

    /*
     * Assert that the mutex being freed is unlocked, just to be safe...
     */
    cma__assert_warn (
	    !cma__int_mutex_locked (old_mutex),
	    "cma__free_mutex called for locked mutex.");

    old_mutex->header.sequence = 0;
    attrib->refcnt--;
    cma__int_lock (cma__g_known_mutexes.mutex);
    (void)cma__queue_remove (&old_mutex->header.queue);
    cma__int_unlock (cma__g_known_mutexes.mutex);

    if ((old_mutex->header.revision == attrib->cache[cma__c_obj_mutex].revision)
	    && (! attrib->delete_pending)) {

	if (attrib->cache[cma__c_obj_mutex].count
		< cma__g_env[cma__c_env_maxmutex].value) {
	    attrib->cache[cma__c_obj_mutex].count += 1;
	    cma__queue_insert (
		    &old_mutex->header.queue,
		    &attrib->cache[cma__c_obj_mutex].queue);
	    }
	else {
	    cma__destroy_mutex (old_mutex);

	    while ((attrib->cache[cma__c_obj_mutex].count
		    > cma__g_env[cma__c_env_minmutex].value)
		    && (! cma__queue_empty (
			    &attrib->cache[cma__c_obj_mutex].queue))) {
		cma__t_int_mutex	*item;

		item = (cma__t_int_mutex *)cma__queue_dequeue (
			&attrib->cache[cma__c_obj_mutex].queue);
		attrib->cache[cma__c_obj_mutex].count -= 1;
		cma__destroy_mutex (item);
		}

	    }

	}
    else
	cma__destroy_mutex (old_mutex);

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Allocate the first mutex objects.  This mode doesn't lock the parent
 *	attribute object's mutex, nor look for cached mutex objects.  It
 *	should ONLY be called during startup, to allow creating mutexes for
 *	the first attributes object & the attributes and mutex sequence
 *      objects (it breaks an otherwise nasty circular dependency).
 *
 *  FORMAL PARAMETERS:
 *
 *	attrib		Attributes object to use
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
 *	Address of new mutex object
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern cma__t_int_mutex *
cma__get_first_mutex(
	cma__t_int_attr	*attrib)	/* Attributes object to use */
    {
    cma__t_int_mutex	*new_mutex;	/* Pointer to new mutex */


    /*
     * We know we need to create a fresh mutex structure, do so. Initialize
     * the appropriate fields.
     */
    new_mutex = cma__alloc_object (cma__t_int_mutex);

    if ((cma_t_address)new_mutex == cma_c_null_ptr)
	cma__bugcheck ("init: can't create mutex");

    new_mutex->header.type = cma__c_obj_mutex;
	new_mutex->owner = cma_c_null_ptr;
    new_mutex->attributes = attrib;
    new_mutex->mutex_kind = attrib->mutex_kind;

    if (new_mutex->mutex_kind == cma_c_mutex_recursive
	    || new_mutex->mutex_kind == cma_c_mutex_nonrecursive) {
	cma__tac_set (&new_mutex->lock);
	new_mutex->unlock = &new_mutex->bitbucket;
	new_mutex->nest_count = 0;
	new_mutex->int_lock = cma__get_first_mutex (&cma__g_def_attr);

	if ((cma_t_address)new_mutex->int_lock == cma_c_null_ptr)
	    cma__bugcheck ("init: can't create internal mutex");

	cma__obj_set_name (new_mutex->int_lock, "inner mutex lock");
	cma__tac_clear (&new_mutex->event);
	cma__tac_set (&new_mutex->waiters);
	}
    else {
	cma__tac_clear (&new_mutex->lock);
	new_mutex->unlock = &new_mutex->lock;
	cma__tac_set (&new_mutex->event);
	new_mutex->int_lock = (cma__t_int_mutex *)cma_c_null_ptr;
	}

    new_mutex->header.sequence = cma__g_mutex_seq.seq++;
    new_mutex->header.revision = attrib->cache[cma__c_obj_mutex].revision;
    cma__obj_clear_name (new_mutex);
    attrib->refcnt++;
    cma__sem_init (&new_mutex->semaphore);

    /*
     * Normally, the known_mutex structure should be LOCKED before inserting
     * a new entry.  However, this particular routine is only called during
     * CMA "bootstrap" when there is only a single thread, and mutexes may
     * not be fully initialized (in fact, this call may be to create the lock
     * which controls access to the known_mutex queue!); so we'll do the
     * insertion without locking.
     */
    cma__queue_insert (&new_mutex->header.queue, &cma__g_known_mutexes.queue);

    return new_mutex;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Allocate a mutex object.
 *
 *  FORMAL PARAMETERS:
 *
 *	attrib		Attributes object to use
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
 *	Address of new mutex object
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern cma__t_int_mutex *
cma__get_mutex(
	cma__t_int_attr	*attrib)	/* Attributes object to use */
    {
    cma_t_boolean	list_empty;	/* true if we need to create struct */
    cma__t_int_mutex	*new_mutex;	/* Pointer to new mutex */


    cma__int_lock (attrib->mutex);

    if (attrib->delete_pending) {
	cma__int_unlock (attrib->mutex);
	return (cma__t_int_mutex *)cma_c_null_ptr;
	}

    if (cma__queue_empty (&attrib->cache[cma__c_obj_mutex].queue))
	list_empty = cma_c_true;
    else {
	new_mutex = (cma__t_int_mutex *)cma__queue_dequeue (
		&attrib->cache[cma__c_obj_mutex].queue);
	attrib->cache[cma__c_obj_mutex].count -= 1;
	list_empty = cma_c_false;
	}

    attrib->refcnt++;

    if (list_empty) {
	/*
	 * If we need to create a fresh mutex structure, do so. Initialize
	 * the appropriate fields.
	 */
	new_mutex = cma__alloc_object (cma__t_int_mutex);

	if ((cma_t_address)new_mutex == cma_c_null_ptr) {
	    cma__int_unlock (attrib->mutex);
	    return (cma__t_int_mutex *)cma_c_null_ptr;
	    }

	new_mutex->header.type = cma__c_obj_mutex;
	new_mutex->attributes = attrib;
	new_mutex->int_lock = (cma__t_int_mutex *)cma_c_null_ptr;
	}

    /*
     * Whether the mutex is old or new, we need to ensure certain field
     * initialization.
     */
    new_mutex->header.sequence = cma__assign_sequence (&cma__g_mutex_seq);
    new_mutex->header.revision = attrib->cache[cma__c_obj_mutex].revision;
    cma__obj_clear_name (new_mutex);
    new_mutex->owner = (cma__t_int_tcb *)cma_c_null_ptr;
    new_mutex->mutex_kind = attrib->mutex_kind;
    cma__int_unlock (attrib->mutex);

    if (new_mutex->mutex_kind == cma_c_mutex_recursive
	    || new_mutex->mutex_kind == cma_c_mutex_nonrecursive) {
	cma__tac_set (&new_mutex->lock);
	new_mutex->unlock = &new_mutex->bitbucket;
	new_mutex->nest_count = 0;

	if (new_mutex->int_lock == (cma__t_int_mutex *)cma_c_null_ptr) {
	    new_mutex->int_lock = cma__get_mutex (&cma__g_def_attr);

	    if ((cma_t_address)new_mutex->int_lock == cma_c_null_ptr) {
		cma__free_mem ((cma_t_address)new_mutex);
		return (cma__t_int_mutex *)cma_c_null_ptr;
		}

	    cma__obj_set_name (
		    new_mutex->int_lock,
		    "(non)recursive int mutex");
	    }

	cma__tac_clear (&new_mutex->event);
	cma__tac_set (&new_mutex->waiters);
	}
    else {
	cma__tac_clear (&new_mutex->lock);
	new_mutex->unlock = &new_mutex->lock;
	cma__tac_set (&new_mutex->event);
	new_mutex->int_lock = (cma__t_int_mutex *)cma_c_null_ptr;
	}

    cma__sem_init (&new_mutex->semaphore);
    cma__int_lock (cma__g_known_mutexes.mutex);
    cma__queue_insert (&new_mutex->header.queue, &cma__g_known_mutexes.queue);
    cma__int_unlock (cma__g_known_mutexes.mutex);

    return new_mutex;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Initialize CMA_MUTEX.C local data
 *
 *  FORMAL PARAMETERS:
 *
 *	none
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
 *	initialize static data
 */
extern void
cma__init_mutex(void)
    {
    cma__g_mutex_seq.mutex	= cma__get_first_mutex (&cma__g_def_attr);
    cma__obj_set_name (cma__g_mutex_seq.mutex, "mutex sequence object");
    cma__g_known_mutexes.mutex	= cma__get_first_mutex (&cma__g_def_attr);
    cma__obj_set_name (cma__g_known_mutexes.mutex, "known mutex list");
    cma__g_global_lock		= cma__get_first_mutex (&cma__g_def_attr);

    /*
     * Manually "fix up" the global lock mutex to behave as a recursive mutex,
     * rather than creating a second default attributes object just for it.
     */
    cma__g_global_lock->mutex_kind	= cma_c_mutex_recursive;
    cma__tac_set (&cma__g_global_lock->lock);
    cma__g_global_lock->unlock		= &cma__g_global_lock->bitbucket;
    cma__g_global_lock->nest_count	= 0;
    cma__g_global_lock->int_lock	= cma__get_first_mutex (
	    &cma__g_def_attr);
    cma__obj_set_name (cma__g_global_lock->int_lock, "global's internal lock");
    cma__tac_clear (&cma__g_global_lock->event);
    cma__tac_set (&cma__g_global_lock->waiters);
    cma__obj_set_name (cma__g_global_lock, "global lock");
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Internal routine to wait/block on a mutex
 *
 *  FORMAL PARAMETERS:
 *
 *	mutex		Mutex object to wait on
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
 *	cma_t_normal, or error status
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern cma_t_status
cma__int_mutex_block(
	cma__t_int_mutex	*mutex)
    {
    cma__t_int_tcb	*tcb = cma__get_self_tcb ();


    switch (mutex->mutex_kind) {
	case cma_c_mutex_recursive : {
	    /*
	     * Lock the internal mutex to gain control of the outer mutex
	     * structure (the mutex lock bit is no use, since it's always
	     * locked).  Using a full mutex instead of a simple lock bit
	     * allows us to block if the lock is busy, rather than spinning.
	     * Note that the inner lock is cma_c_mutex_fast, so we don't just
	     * recurse through all this stuff.
	     */
	    cma__int_lock (mutex->int_lock);

	    if ((mutex->owner == tcb)
		    || (mutex->owner == (cma__t_int_tcb *)cma_c_null_ptr)) {
		mutex->nest_count++;
		mutex->owner = tcb;	/* Say we have the lock now */
		cma__int_unlock (mutex->int_lock);
		return cma_s_normal;
		}
	    else {

		while (1) {

		    /*
		     * Mark the mutex to indicate that there "might be
		     * waiters".  It's important that this is done within a
		     * "while (1)" loop, BEFORE testing for the loop exit
		     * condition.  When there are waiters, the mark is
		     * removed and a single waiter is awakened, which loops
		     * back to here. Marking the mutex again ensures that any
		     * additional waiters will be awakened when this thread
		     * releases the mutex.  Using a "might be waiters" bit
		     * like this is less efficient than maintaining an
		     * accurate count, but has the advantage of being immune
		     * to race conditions.
		     */
		    cma__unset (&mutex->waiters);	/* About to wait */

		    if (mutex->owner == (cma__t_int_tcb *)cma_c_null_ptr)
			break;

		    cma__int_unlock (mutex->int_lock);
		    cma__enter_kernel ();
/*
 * FIX-ME evaluate if there is a better way for both debuggers to  find out
 * if a thread is waiting on a mutex other than by setting this field.
 */
		    tcb->wait_mutex = mutex;
		    cma__sem_wait (&mutex->semaphore, tcb);
		    tcb->wait_mutex = (cma__t_int_mutex *)cma_c_null_ptr;
		    cma__exit_kernel ();
		    cma__int_lock (mutex->int_lock);
		    }

		mutex->owner = tcb;
		mutex->nest_count++;
		cma__int_unlock (mutex->int_lock);
		}

	    break;
	    }

	case cma_c_mutex_nonrecursive : {
	    /*
	     * Lock the internal mutex to gain control of the outer mutex
	     * structure (the mutex lock bit is no use, since it's always
	     * locked).  Using a full mutex instead of a simple lock bit
	     * allows us to block if the lock is busy, rather than spinning.
	     * Note that the inner lock is cma_c_mutex_fast, so we don't just
	     * recurse through all this stuff.
	     */
	    cma__int_lock (mutex->int_lock);

	    if (mutex->owner == (cma__t_int_tcb *)cma_c_null_ptr) {
		mutex->owner = tcb;	/* Say we have the lock now */
		cma__int_unlock (mutex->int_lock);
		return cma_s_normal;
		}
	    else {

		/*
		 * Nonrecursive mutexes detect attempts to recursively lock a
		 * mutex (i.e., locking a mutex already owned by the thread).
		 */
		if (mutex->owner == tcb) {
		    cma__int_unlock (mutex->int_lock);
		    return (cma_s_in_use);
		    }

		while (1) {

		    /*
		     * Mark the mutex to indicate that there "might be
		     * waiters".  It's important that this is done within a
		     * "while (1)" loop, BEFORE testing for the loop exit
		     * condition.  When there are waiters, the mark is
		     * removed and a single waiter is awakened, which loops
		     * back to here. Marking the mutex again ensures that any
		     * additional waiters will be awakened when this thread
		     * releases the mutex.  Using a "might be waiters" bit
		     * like this is less efficient than maintaining an
		     * accurate count, but has the advantage of being immune
		     * to race conditions.
		     */
		    cma__unset (&mutex->waiters);	/* About to wait */

		    if (mutex->owner == (cma__t_int_tcb *)cma_c_null_ptr)
			break;

		    cma__int_unlock (mutex->int_lock);
		    cma__enter_kernel ();
/*
 * FIX-ME evaluate if there is a better way for both debuggers to  find out
 * if a thread is waiting on a mutex other than by setting this field.
 */
		    tcb->wait_mutex = mutex;
		    cma__sem_wait (&mutex->semaphore, tcb);
		    tcb->wait_mutex = (cma__t_int_mutex *)cma_c_null_ptr;
		    cma__exit_kernel ();
		    cma__int_lock (mutex->int_lock);
		    }

		mutex->owner = tcb;
		cma__int_unlock (mutex->int_lock);
		}

	    break;
	    }

	case cma_c_mutex_fast : {

	    /*
	     * Give a failure if the same thread already owns the lock (this
	     * evaporates in production builds).
	     */
	    cma__assert_fail (
		    (tcb != mutex->owner),
		    "attempt to relock a mutex");

	    /*
	     * For multiprocessor implementations, loop and retry the lock
	     * bit some number of times before attempting to block.  The
	     * count is configurable via the _CMA_SPINLOOP_ macro (see
	     * cma_config.h).  On multiprocessors, this loop can improve
	     * performance by avoiding the expensive block: on a
	     * uniprocessor, it would just waste time, since the lock can't
	     * be released until a context switch occurs.
	     *
	     * Block the thread.  First, clear the mutex bit saying that
	     * there might be a waiter (this is used to optimize the unlock
	     * path; note the "negative logic" used so the bit can be tested
	     * and reset with a test-and-set operation!). Then, to avoid a
	     * possible race condition, try the mutex again; if it's been
	     * unlocked, just return by exiting the loop.
	     *
	     * Otherwise, enter the kernel and block the thread on a
	     * semaphore. Note that we could get "spurious wakeups" because
	     * of the use of the "might be waiters" bit; that's OK, 'cause
	     * we'll just loop around and try again.
	     */
	    while (1) {

		/*
		 * Set the bit saying there "might be waiters".  It's
		 * important that this is done within a "while (1)" loop,
		 * BEFORE testing for the loop exit condition.  When there
		 * are waiters, the "waiters" bit is reset and a single
		 * waiter is awakened, which loops back to here. Setting the
		 * bit again ensures that any additional waiters will be
		 * awakened when this thread releases the mutex.  Using a
		 * "might be waiters" bit like this is less efficient than
		 * maintaining an accurate count, but has the advantage of
		 * being immune to race conditions.
		 */
		cma__unset (&mutex->event);

#if _CMA_SPINLOOP_ > 0
		{
		cma_t_integer	spincount;

		for (spincount = 0; spincount < _CMA_SPINLOOP_; spincount++) {

		    if (!cma__tac_isset (&mutex->lock)) {

			if (!cma__test_and_set (&mutex->lock))
			    return cma_s_normal;

			}

		    }

		}
#endif

		if (!cma__test_and_set (&mutex->lock))
		    return cma_s_normal;

		cma__enter_kernel ();
/*
 * FIX-ME evaluate if there is a better way for both debuggers to  find out
 * if a thread is waiting on a mutex other than by setting this field.
 */
		tcb->wait_mutex = mutex;
		cma__sem_wait (&mutex->semaphore, tcb);
		tcb->wait_mutex = (cma__t_int_mutex *)cma_c_null_ptr;
		cma__exit_kernel ();
		}

	    break;
	    }

	}

    return cma_s_normal;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Detect whether a mutex is locked.
 *
 *  FORMAL PARAMETERS:
 *
 *	mutex		Pointer to mutex object to test
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
extern cma_t_boolean
cma__int_mutex_locked(
	cma__t_int_mutex	*mutex)	/* Mutex to test */
    {

    if (cma__tac_isset (&mutex->lock)) {

	switch (mutex->mutex_kind) {
	    case cma_c_mutex_recursive :
	    case cma_c_mutex_nonrecursive : {
		cma__int_lock (mutex->int_lock);

		if (mutex->owner != (cma__t_int_tcb *)cma_c_null_ptr) {
		    cma__int_unlock (mutex->int_lock);
		    return cma_c_true;
		    }
		else {
		    cma__int_unlock (mutex->int_lock);
		    return cma_c_false;
		    }

		break;
		}
	    case cma_c_mutex_fast : {
		return cma_c_true;
		break;
		}
		default: {
		cma__bugcheck ("int_mutex_locked: bad mutex type");
		return cma_c_false;
		break;
		}

	    }

	}
    else
	return cma_c_false;

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Internal routine to unblock threads waiting on a mutex
 *
 *  FORMAL PARAMETERS:
 *
 *	mutex		Mutex object
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
 *	cma_s_normal, or error status
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern cma_t_status
cma__int_mutex_unblock(
	cma__t_int_mutex	*mutex)
    {
    /*
     * If the event flag was set because this is a friendly mutex, then we
     * need to decrement the count; and only wake other threads if the count
     * goes to 0.
     */
    switch (mutex->mutex_kind) {

	case cma_c_mutex_recursive : {
	    cma__t_int_tcb	*tcb = cma__get_self_tcb ();


	    /*
	     * Before calling this routine, the main mutex unlock code did a
	     * test-and-set on the event bit; since we want a recursive mutex
	     * to always have an "event pending", we need to clear the bit
	     * again.
	     */
	    cma__unset (&mutex->event);	/* Event is always ON */

	    /*
	     * Lock the internal mutex to gain control of the outer mutex
	     * structure (the mutex lock bit is no use, since it's always
	     * locked).  Using a full mutex instead of a simple lock bit
	     * allows us to block if the lock is busy, rather than spinning.
	     * Note that the inner lock is cma_c_mutex_fast, so we don't just
	     * recurse through all this stuff.
	     */
	    cma__int_lock (mutex->int_lock);

	    /*
	     * Recursive mutexes detect attempts to release an unowned mutex
	     * or one owned by another thread.
	     */
	    if (mutex->owner == (cma__t_int_tcb *)cma_c_null_ptr) {
		cma__int_unlock (mutex->int_lock);
		return (cma_s_use_error);
		}

	    if (tcb != mutex->owner) {
		cma__int_unlock (mutex->int_lock);
		return (cma_s_in_use);
		}

	    if (--mutex->nest_count == 0) {
		mutex->owner = (cma__t_int_tcb *)cma_c_null_ptr;

		if (!cma__test_and_set (&mutex->waiters))
		    cma__sem_wake_one (&mutex->semaphore);

		}

	    cma__int_unlock (mutex->int_lock);
	    break;
	    }

	case cma_c_mutex_nonrecursive : {
	    cma__t_int_tcb	*tcb = cma__get_self_tcb ();


	    /*
	     * Before calling this routine, the main mutex unlock code did a
	     * test-and-set on the event bit; since we want a nonrecursive
	     * mutex to always have an "event pending", we need to clear the
	     * bit again.
	     */
	    cma__unset (&mutex->event);	/* Event is always ON */

	    /*
	     * Lock the internal mutex to gain control of the outer mutex
	     * structure (the mutex lock bit is no use, since it's always
	     * locked).  Using a full mutex instead of a simple lock bit
	     * allows us to block if the lock is busy, rather than spinning.
	     * Note that the inner lock is cma_c_mutex_fast, so we don't just
	     * recurse through all this stuff.
	     */
	    cma__int_lock (mutex->int_lock);

	    /*
	     * Nonrecursive mutexes detect attempts to release an unowned
	     * mutex or one owned by another thread.
	     */
	    if (mutex->owner == (cma__t_int_tcb *)cma_c_null_ptr) {
		cma__int_unlock (mutex->int_lock);
		return (cma_s_use_error);
		}

	    if (tcb != mutex->owner) {
		cma__int_unlock (mutex->int_lock);
		return (cma_s_in_use);
		}

	    mutex->owner = (cma__t_int_tcb *)cma_c_null_ptr;

	    if (!cma__test_and_set (&mutex->waiters))
		cma__sem_wake_one (&mutex->semaphore);

	    cma__int_unlock (mutex->int_lock);
	    break;
	    }

	case cma_c_mutex_fast : {
	    cma__sem_wake_one (&mutex->semaphore);
	    break;
	    }

	}

    return cma_s_normal;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Find out whether a mutex is locked.
 *
 *  FORMAL PARAMETERS:
 *
 *	mutex		Mutex object to text
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
 *	cma_c_true if mutex is locked, cma_c_false otherwise.
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern cma_t_boolean
cma__mutex_locked(
	cma_t_mutex	mutex)		/* Mutex to test */
    {
#ifdef NDEBUG
    return cma__int_mutex_locked (
	    (cma__t_int_mutex *)((cma__t_int_handle *)&mutex)->pointer);
#else
    cma__t_int_mutex	*int_mutex;

    int_mutex = cma__validate_mutex (&mutex);
    return cma__int_mutex_locked (int_mutex);
#endif
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Try to lock a mutex, but return immediately with boolean status if
 *	the mutex is already locked (internal)
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
extern cma_t_boolean
cma__int_try_lock(
	cma__t_int_mutex	*mutex)	/* Mutex to lock */
    {
    cma__t_int_tcb	*tcb;


#ifndef NDEBUG
    tcb = cma__get_self_tcb ();
#endif

    if (cma__test_and_set (&mutex->lock)) {

	switch (mutex->mutex_kind) {
	    case cma_c_mutex_nonrecursive : {
#ifdef NDEBUG
		tcb = cma__get_self_tcb ();
#endif
		cma__int_lock (mutex->int_lock);

		if (mutex->owner == (cma__t_int_tcb *)cma_c_null_ptr) {
		    mutex->owner = tcb;
		    cma__int_unlock (mutex->int_lock);
		    return cma_c_true;
		    }
		else {
		    cma__int_unlock (mutex->int_lock);
		    return cma_c_false;
		    }

		break;
		}

	    case cma_c_mutex_recursive : {
#ifdef NDEBUG
		tcb = cma__get_self_tcb ();
#endif
		cma__int_lock (mutex->int_lock);

		if (tcb == mutex->owner
			|| mutex->owner == (cma__t_int_tcb *)cma_c_null_ptr) {
		    mutex->nest_count++;
		    mutex->owner = tcb;
		    cma__int_unlock (mutex->int_lock);
		    return cma_c_true;
		    }
		else {
		    cma__int_unlock (mutex->int_lock);
		    return cma_c_false;
		    }

		break;
		}

	    case cma_c_mutex_fast : {
		return cma_c_false;
		break;
		}

	    }

	}

#ifndef NDEBUG
    mutex->owner = tcb;
#endif
    return cma_c_true;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Performs pre (1) and post (0) fork reinitialization work.
 *
 *  FORMAL PARAMETERS:
 *
 *	flag
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
extern void
cma__reinit_mutex(
	cma_t_integer	flag)
    {
    cma__t_queue        *ptr;
    cma__t_int_mutex    *mutex;

    if (flag == cma__c_reinit_prefork_lock) {
	cma__int_lock (cma__g_mutex_seq.mutex);
	cma__int_lock (cma__g_known_mutexes.mutex);
	}
    else if (flag == cma__c_reinit_postfork_unlock) {
	cma__int_unlock (cma__g_mutex_seq.mutex);
	cma__int_unlock (cma__g_known_mutexes.mutex);
	}
    else if (flag == cma__c_reinit_postfork_clear) {
	    ptr = &cma__g_known_mutexes.queue;
	    while ((ptr = cma__queue_next (ptr)) != &cma__g_known_mutexes.queue) {
		mutex = (cma__t_int_mutex *)ptr;
		cma__queue_init (&mutex->semaphore.queue);
		}
	    }
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_MUTEX.C */
/*  *15    5-JUN-1992 13:34:53 BUTENHOF "Fix delete_pending attr handling" */
/*  *14   13-MAR-1992 14:08:57 BUTENHOF "Fix mutex lock/unlock code" */
/*  *13   10-MAR-1992 16:26:01 BUTENHOF "Eliminate need for TRY/CATCH on pthread mutex lock" */
/*  *12   19-FEB-1992 04:34:47 BUTENHOF "Fix typos" */
/*  *11   18-FEB-1992 15:29:21 BUTENHOF "Adapt to new alloc_mem protocol" */
/*  *10    5-NOV-1991 14:58:58 BUTENHOF "Fix a bug in cma__get_mutex()" */
/*  *9    28-JUN-1991 13:20:36 BUTENHOF "Initialize owner at creation" */
/*  *8    10-JUN-1991 18:22:24 SCALES "Add sccs headers for Ultrix" */
/*  *7     5-JUN-1991 16:13:04 CURTIN "fork work" */
/*  *6    29-MAY-1991 17:45:28 BUTENHOF "Fix possible race condition in block" */
/*  *5    10-MAY-1991 11:46:46 CURTIN "Use new macros in cma_mutex.h" */
/*  *4    12-APR-1991 23:36:05 BUTENHOF "Init atomic bits using new constants" */
/*  *3    13-FEB-1991 17:54:43 BUTENHOF "Change mutex attribute name symbols" */
/*  *2    12-FEB-1991 23:09:48 BUTENHOF "Recursive/nonrecursive mutexes" */
/*  *1    12-DEC-1990 21:47:28 BUTENHOF "Mutexes" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_MUTEX.C */
