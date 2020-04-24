/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_vp.c,v $
 * Revision 1.1.6.2  1996/02/17  23:34:28  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:50:35  marty]
 *
 * Revision 1.1.6.1  1995/12/07  22:07:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:07:52  root]
 * 
 * Revision 1.1.4.2  1994/06/09  13:39:11  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:56  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:17:52  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:34  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:32:50  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:44:19  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:42:36  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:08:03  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/09  00:15:38  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:15:19  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:39:42  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *  Copyright (c) 1991, 1992 by
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
 *  and  should  not  be  construed  as  a commitment by Digital Equipment
 *  Corporation.
 *
 *  Digital assumes no responsibility for the use or  reliability  of  its
 *  software on equipment which is not supplied by Digital.
 */

/*
 *  FACILITY:
 *
 *	DECthreads services
 *
 *  ABSTRACT:
 *
 *	Implement generic "virtual processor" layer to take advantage of
 * 	kernel threads for parallel processing.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	03 April 1991
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	24 April 1991
 *		Add cma__trace statements to aid debugging.
 *	002	Dave Butenhof	03 May 1991
 *		Change "cma__vp_set_state" to "cma__vp_set_start" and
 *		encapsulate more machine-specific information here (like
 *		address of start routine and frame setup).
 *	003	Dave Butenhof	24 May 1991
 *		Add some traces, and change state enums into flags.
 *	004	Dave Butenhof	31 May 1991
 *		Add timeout parameter to vp_suspend.
 *	005	Dave Butenhof	02 June 1991
 *		Add new return status for trying to delete a VP that's not
 *		suspended.
 *	006	Dave Butenhof	03 June 1991
 *		New tack: a thread should delete it's own VP when
 *		terminating, rather than let another thread clean it up.
 *	007	Dave Butenhof	11 June 1991
 *		Change numeric constants in reinit function to use symbols.
 *	008	Dave Butenhof	05 September 1991
 *		Add cma__vp_yield() function to call Mach "sched_yield"
 *		syscall (which I hadn't known about when I designed the VP
 *		module).
 *	009	Dave Butenhof	11 September 1991
 *		Add support for caching VPs to improve performance.
 *	010	Dave Butenhof	07 February 1992
 *		Add cma__vp_dump() function to list all active and cached VPs
 *		for debugging.
 *	011	Dave Butenhof	08 February 1992
 *		Rework state flags for better debugging.
 *	012	Dave Butenhof	06 March 1992
 *		Modify cma__vp_get_state(): thread_get_state() doesn't work
 *		on a running thread, and we'd need extra logic to fix it. We
 *		really don't need the info it gives us (current pc and sp,
 *		basically), so just zero the fields and don't bother!
 *	013	Dave Butenhof	13 March 1992
 *		Cut down initial stack argument allocation, since we're only
 *		passing one argument into thread_base instead of 3.
 *	014	Dave Butenhof	23 March 1992
 *		Modify cma__vp_dump() to use redirectable I/O system.
 *	015	Dave Butenhof	31 March 1992
 *		Add Alpha support.
 */

/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_defs.h>
#include <cma_vp.h>
#include <cma_vm.h>
#include <cma_assem.h>

#if _CMA_KTHREADS_ == _CMA__MACH
# include <mach.h>
# include <mach_error.h>
# include <mach/machine/syscall_sw.h>
#endif

/*
 * GLOBAL DATA
 */

/*
 * LOCAL DATA
 */

#if _CMA_KTHREADS_ == _CMA__MACH
static cma__t_queue		cma___g_vp_queue;
static cma__t_queue		cma___g_vp_cache;
static cma__t_atomic_bit	cma___g_vp_lock = cma__c_tac_static_clear;
#endif

/*
 * LOCAL MACROS
 */

/*
 * LOCAL FUNCTIONS
 */

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	initialize the VP layer
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	the current kernel thread
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
 * 
 */
void
cma__init_vp(void)
    {
#if _CMA_KTHREADS_ == _CMA__MACH
    kern_return_t	status;
    thread_t		virtual_processor;
    port_t		synch_port;
    cma__t_vstruct	*vp_struct;
    task_t		task;


    task = task_self ();
    cma__trace ((
	    cma__c_trc_init | cma__c_trc_vp,
	    "(init_vp) running in task %d",
	    task));

    /*
     * Set up default VP
     */
    cma__queue_init (&cma___g_vp_queue);
    cma__queue_init (&cma___g_vp_cache);
    virtual_processor = thread_self ();
    status = port_allocate (task, &synch_port);

    cma__trace ((
	    cma__c_trc_init | cma__c_trc_vp,
	    "(init_vp) running in vp %d, synch port is %d",
	    virtual_processor,
	    synch_port));

    if (status != KERN_SUCCESS) {
	cma__trace ((
		cma__c_trc_init | cma__c_trc_vp | cma__c_trc_bug,
		"(init_vp) error \"%s\" (%d) allocating def. VP synch port",
		mach_error_string (status),
		status));
	cma__bugcheck ("init_vp: port_allocate");
	}

    if ((status = port_disable (task, synch_port)) != KERN_SUCCESS) {
	cma__trace ((
		cma__c_trc_init | cma__c_trc_vp | cma__c_trc_bug,
		"(init_vp) error \"%s\" (%d) disabling port %d",
		mach_error_string (status),
		status,
		synch_port));
	cma__bugcheck ("init_vp: port_disable");
	}

    if ((status = port_set_backlog (task, synch_port, 1)) != KERN_SUCCESS) {
	cma__trace ((
		cma__c_trc_init | cma__c_trc_vp | cma__c_trc_bug,
		"(init_vp) error \"%s\" (%d) setting port %d backlog",
		mach_error_string (status),
		status,
		synch_port));
	cma__bugcheck ("init_vp: port_set_backlog");
	}

    vp_struct = cma__alloc_object (cma__t_vstruct);
    vp_struct->vp = virtual_processor;
    vp_struct->synch = synch_port;
    vp_struct->flags = cma__c_vp_running | cma__c_vp_default;
    cma__queue_insert (&vp_struct->queue, &cma___g_vp_queue);
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__reinit_vp:  Do pre/post re-Initialize 
 *
 *  FORMAL PARAMETERS:
 *
 *	flag
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma__reinit_vp(
	cma_t_integer	    flag)
    {
#if _CMA_KTHREADS_ == _CMA__MACH

    switch (flag) {
	case cma__c_reinit_prefork_lock : {
	    cma__spinlock (&cma___g_vp_lock);
	    break;
	    }
	case cma__c_reinit_postfork_unlock : {
	    cma__spinunlock (&cma___g_vp_lock);
	    break;
	    }
	case cma__c_reinit_postfork_clear : {
	    thread_t		self;
	    cma__t_queue	*qptr;


	    /*
	     * Remove all active VP structures, except for the VP that's
	     * currently running.
	     */
	    self = thread_self ();
	    qptr = cma__queue_next (&cma___g_vp_queue);

	    while (qptr != &cma___g_vp_queue) {
		cma__t_vpid	vptr = (cma__t_vpid)qptr;
		cma__t_queue	*qnext = cma__queue_next (qptr);


		if (vptr->vp != self) {
		    cma__queue_remove (qptr);
		    /*
		     * FIX-ME
		     *
		     * Need to determine whether ports are copied to a forked
		     * task... if so, we need to port_deallocate() here, and
		     * we don't need to allocate a new port for the current
		     * VP (else clause).
		     */
		    cma__free_mem ((cma_t_address)vptr);
		    }
		else {
		    kern_return_t	status;
		    port_t		synch_port;
		    task_t		task;
		    thread_t		self;


		    task = task_self ();
		    self = thread_self ();
		    status = port_allocate (task, &synch_port);

		    cma__trace ((
			    cma__c_trc_init | cma__c_trc_vp,
			    "(reinit_vp) running in vp %d, synch port is %d",
			    self,
			    synch_port));

		    if (status != KERN_SUCCESS) {
			cma__trace ((
				cma__c_trc_init | cma__c_trc_vp | cma__c_trc_bug,
				"(reinit_vp) error \"%s\" (%d) allocating def. VP synch port",
				mach_error_string (status),
				status));
			cma__bugcheck ("reinit_vp: port_allocate");
			}

		    if ((status = port_disable (task, synch_port)) != KERN_SUCCESS) {
			cma__trace ((
				cma__c_trc_init | cma__c_trc_vp | cma__c_trc_bug,
				"(reinit_vp) error \"%s\" (%d) disabling port %d",
				mach_error_string (status),
				status,
				synch_port));
			cma__bugcheck ("reinit_vp: port_disable");
			}

		    if ((status = port_set_backlog (task, synch_port, 1)) != KERN_SUCCESS) {
			cma__trace ((
				cma__c_trc_init | cma__c_trc_vp | cma__c_trc_bug,
				"(reinit_vp) error \"%s\" (%d) setting port %d backlog",
				mach_error_string (status),
				status,
				synch_port));
			cma__bugcheck ("reinit_vp: port_set_backlog");
			}

		    vptr->vp = self;
		    vptr->synch = synch_port;
		    vptr->flags = cma__c_vp_running | cma__c_vp_default;
		    }

		qptr = qnext;
		}

	    /*
	     * Clear out any cached VPs, also
	     */
	    qptr = cma__queue_next (&cma___g_vp_cache);

	    while (qptr != &cma___g_vp_cache) {
		cma__t_vpid	vptr = (cma__t_vpid)qptr;
		cma__t_queue	*qnext = cma__queue_next (qptr);


		cma__queue_remove (qptr);
		/*
		 * FIX-ME
		 *
		 * Need to determine whether ports are copied to a forked
		 * task... if so, we need to port_deallocate() here.
		 */
		cma__free_mem ((cma_t_address)vptr);
		qptr = qnext;
		}

	    cma__spinunlock (&cma___g_vp_lock);
	    break;
	    }

	}

#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Create a suspended vp
 *
 *  FORMAL PARAMETERS:
 *
 *	vpid		return the id of the new virtual processor
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
 *	status of operation
 *
 *  SIDE EFFECTS:
 *
 *	none
 * 
 */
cma__t_vp_status
cma__vp_create(cma__t_vpid		*vpid)
    {
#if _CMA_KTHREADS_ == _CMA__MACH
    kern_return_t	status;
    thread_t		virtual_processor;
    port_t		synch_port;
    cma__t_vstruct	*vp_struct;
    task_t		task;


    /*
     * Lock the VP structures and check for a cached VP to save the trouble
     * of creating a new one.
     */
    cma__spinlock (&cma___g_vp_lock);

    if (!cma__queue_empty (&cma___g_vp_cache)) {
	vp_struct = (cma__t_vstruct *)cma__queue_dequeue (&cma___g_vp_cache);

	/*
	 * NOTE: the "if" branch leaves the VP database locked, and drops
	 * into the queue insertion after the "else". The "else" branch will
	 * unlock the database for the possibly slow Mach operations, and
	 * then relock before dropping through.
	 */
	}
    else {
	cma__spinunlock (&cma___g_vp_lock);
	task = task_self ();
	status = thread_create (task, &virtual_processor);

	if (status != KERN_SUCCESS)
	    return (cma__t_vp_status)status;

	status = port_allocate (task, &synch_port);

	if (status != KERN_SUCCESS) {

	    if (thread_terminate (virtual_processor) != KERN_SUCCESS)
		cma__bugcheck ("vp_create: thread_terminate");

	    return (cma__t_vp_status)status;
	    }

	if (port_disable (task, synch_port) != KERN_SUCCESS)
	    cma__bugcheck ("vp_create: port_disable");

	if (port_set_backlog (task, synch_port, 1) != KERN_SUCCESS)
	    cma__bugcheck ("vp_create: port_set_backlog");

	vp_struct = cma__alloc_object (cma__t_vstruct);
	vp_struct->vp = virtual_processor;
	vp_struct->synch = synch_port;
	cma__spinlock (&cma___g_vp_lock);
	}

    vp_struct->flags = cma__c_vp_new;
    cma__queue_insert (&vp_struct->queue, &cma___g_vp_queue);
    cma__spinunlock (&cma___g_vp_lock);
    *vpid = vp_struct;
    return cma__c_vp_normal;
#else
    cma__bugcheck ("vp_create: no VPs");	/* no VP creation on uniprocessor */
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	delete a VP (should be the current vp).
 *
 *  FORMAL PARAMETERS:
 *
 *	vpid		Which VP to delete
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
 *	status of operation
 *
 *  SIDE EFFECTS:
 *
 *	none
 * 
 */
void
cma__vp_delete(cma__t_vpid		vpid)
    {
#if _CMA_KTHREADS_ == _CMA__MACH
    kern_return_t	status;
    thread_t		vp;


    cma__spinlock (&cma___g_vp_lock);
    cma__queue_remove (&vpid->queue);
    cma__queue_insert (&vpid->queue, &cma___g_vp_cache);

    /*
     * Mark thread as cached for reuse (not running, suspended, etc.)
     */
    vpid->flags = cma__c_vp_cached;
    cma__spinunlock (&cma___g_vp_lock);

    /*
     * Suspend the thread, so that when it's pulled off the cache queue it'll
     * be OK to yank it to a new PC/SP with thread_set_state().
     */
    if ((status = thread_suspend (vpid->vp)) != KERN_SUCCESS) {
	cma__trace ((
		cma__c_trc_vp | cma__c_trc_bug,
		"(vp_delete) error \"%s\" (%d) suspending vp %d",
		mach_error_string (status),
		status,
		vpid->vp));
	cma__bugcheck ("vp_delete: thread_suspend");
	}

    cma__bugcheck ("vp_delete: resumed without yank");
#else
    cma__bugcheck ("vp_delete: no VPs");
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Dump the queues of active and cached VPs for debugging. (This is
 *	compiled unconditionally because there is no cost unless it is
 *	called).
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	the VP queues
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
 *	print information to stdout
 * 
 */
void
cma__vp_dump(void)
    {
#if _CMA_KTHREADS_ == _CMA__MACH
    cma__t_queue	*qptr;
    char		buffer[256];


    buffer[0] = '\0';
    cma__spinlock (&cma___g_vp_lock);

    /*
     * Report active VPs
     */
    qptr = cma__queue_next (&cma___g_vp_queue);

    while (qptr != &cma___g_vp_queue) {
	cma__t_vpid	vptr = (cma__t_vpid)qptr;


	cma__putformat (
		buffer,
		"VP %08x: port %d, synch %d, flags-%s%s%s%s%s\n",
		vptr,
		vptr->vp,
		vptr->synch,
		(vptr->flags & cma__c_vp_new ? ":new" : ""),
		(vptr->flags & cma__c_vp_running ? ":running" : ""),
		(vptr->flags & cma__c_vp_susp ? ":suspend" : ""),
		(vptr->flags & cma__c_vp_cached ? ":cached" : ""),
		(vptr->flags & cma__c_vp_default ? ":default" : ""),
		(vptr->flags & cma__c_vp_hold ? ":hold" : ""));
	cma__puteol (buffer);
	qptr = cma__queue_next (qptr);
	}

    /*
     * Report cached VPs, also
     */
    qptr = cma__queue_next (&cma___g_vp_cache);

    while (qptr != &cma___g_vp_cache) {
	cma__t_vpid	vptr = (cma__t_vpid)qptr;


	cma__putformat (
		buffer,
		"VP %08x: port %d, synch %d, flags-%s%s%s%s%s\n",
		vptr,
		vptr->vp,
		vptr->synch,
		(vptr->flags & cma__c_vp_new ? ":new" : ""),
		(vptr->flags & cma__c_vp_running ? ":running" : ""),
		(vptr->flags & cma__c_vp_cached ? ":cached" : ""),
		(vptr->flags & cma__c_vp_default ? ":default" : ""),
		(vptr->flags & cma__c_vp_hold ? ":hold" : ""));
	cma__puteol (buffer);
	qptr = cma__queue_next (qptr);
	}

    cma__spinunlock (&cma___g_vp_lock);
#else
    return;
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	return ID of current VP
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	the current kernel thread
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	kernel thread ID
 *
 *  SIDE EFFECTS:
 *
 *	none
 * 
 */
cma__t_vpid
cma__vp_get_id(void)
    {
#if _CMA_KTHREADS_ == _CMA__MACH
    thread_t		virtual_processor;
    cma__t_queue	*qptr;


    virtual_processor = thread_self ();
    cma__spinlock (&cma___g_vp_lock);
    qptr = cma__queue_next (&cma___g_vp_queue);

    while (qptr != &cma___g_vp_queue) {
	cma__t_vpid	vptr = (cma__t_vpid)qptr;

	if (vptr->vp == virtual_processor) {
	    cma__spinunlock (&cma___g_vp_lock);
	    return vptr;
	    }

	qptr = cma__queue_next (qptr);
	}

    cma__spinunlock (&cma___g_vp_lock);
    cma__bugcheck ("vp_get_id: unknown VP");	/* Not running in known VP */
#else
    return (cma__t_vpid)0;
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	get state of VP
 *
 *  FORMAL PARAMETERS:
 *
 *	vpid		ID of kernel thread
 *	state		state array
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
 *	status
 *
 *  SIDE EFFECTS:
 *
 *	none
 * 
 */
cma__t_vp_status
cma__vp_get_state(cma__t_vpid	vpid,
	cma__t_vp_state	*state)
    {
#if _CMA_KTHREADS_ == _CMA__MACH
    kern_return_t		status;
    struct thread_basic_info	info_array;
    unsigned int		info_count = THREAD_BASIC_INFO_COUNT;


    status = thread_info (
	    vpid->vp,
	    THREAD_BASIC_INFO,
	    (thread_info_t)&info_array,
	    &info_count);

    if (status != KERN_SUCCESS)
	return (cma__t_vp_status)status;

    /*
     * The stack, start_routine, and start_arg fields in the state array
     * really aren't recoverable. We could return current PC and SP, except
     * that requires an extra Mach call (thread_get_state) as well as some
     * precautionary logic since it won't work on a thread that's running.
     * The simplest solution is to simply not worry about it, and just zero
     * the fields.
     */
    state->stack = 0;
    state->start_routine = 0;
    state->start_arg = 0;

    state->priority = info_array.cur_priority;
    state->mach_state = info_array.run_state;

    if (info_array.suspend_count > 0) {

	if (vpid->flags & cma__c_vp_cached)
	    state->run_state = cma__c_vp_st_stop;
	else if (vpid->flags & cma__c_vp_hold)
	    state->run_state = cma__c_vp_st_hold;
	else
	    state->run_state = cma__c_vp_st_unks;

	}
    else if (info_array.run_state == TH_STATE_WAITING) {

	if (vpid->flags & cma__c_vp_susp)
	    state->run_state = cma__c_vp_st_susp;
	else
	    state->run_state = cma__c_vp_st_unkw;

	}
    else if (info_array.run_state = TH_STATE_RUNNING)
	state->run_state = cma__c_vp_st_run;
    else
	state->run_state = cma__c_vp_st_unk;

    state->suspend_count = info_array.suspend_count;

    return cma__c_vp_normal;
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	interrupt a VP
 *
 *  FORMAL PARAMETERS:
 *
 *	vpid		Which VP to interrupt
 *	handler		Address of routine to call in target VP
 *	arg		Argument to pass to interrupt handler
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
 *	status of operation
 *
 *  SIDE EFFECTS:
 *
 *	none
 * 
 */
cma__t_vp_status
cma__vp_interrupt(cma__t_vpid		vpid,
	cma__t_vp_handler	handler,
	cma_t_address		arg)
    {
#if _CMA_KTHREADS_ == _CMA__MACH
# if _CMA_HARDWARE_ == _CMA__MIPS
    struct mips_thread_state	state_array;
    unsigned int		state_count = MIPS_THREAD_STATE_COUNT;
# endif
# if _CMA_HARDWARE_ == _CMA__ALPHA
    struct alpha_thread_state	state_array;
    unsigned int		state_count = ALPHA_THREAD_STATE_COUNT;
# endif
    int				*stack_array;
    kern_return_t		status;


    cma__trace ((
	    cma__c_trc_vp,
	    "(vp_interrupt) interrupting vp %d to %08x(%08x)",
	    vpid->vp,
	    handler,
	    arg));

    /*
     * First, suspend the target thread
     */
    if ((status = thread_suspend (vpid->vp)) != KERN_SUCCESS) {
	cma__trace ((
		cma__c_trc_vp | cma__c_trc_bug,
		"(vp_interrupt) error \"%s\" (%d) suspending vp %d",
		mach_error_string (status),
		status,
		vpid->vp));
	cma__bugcheck ("vp_interrupt: thread_suspend");
	}

    /*
     * Now, abort any kernel wait in the target so it won't be restarted
     * when the thread wakes up.
     */
    if ((status = thread_abort (vpid->vp)) != KERN_SUCCESS) {
	cma__trace ((
		cma__c_trc_vp | cma__c_trc_bug,
		"(vp_interrupt) error \"%s\" (%d) aborting operation on vp %d",
		mach_error_string (status),
		status,
		vpid->vp));
	cma__bugcheck ("vp_interrupt: thread_abort");
	}

    /*
     * Set up for the async call
     */

    if ((status = thread_get_state (
	    vpid->vp,
# if _CMA_HARDWARE_ == _CMA__MIPS
	    MIPS_THREAD_STATE,
# endif
# if _CMA_HARDWARE_ == _CMA__ALPHA
	    ALPHA_THREAD_STATE,
# endif
	    (thread_state_t)&state_array,
	    &state_count)) != KERN_SUCCESS) {
	cma__trace ((
		cma__c_trc_vp | cma__c_trc_bug,
		"(vp_interrupt) error \"%s\" (%d) getting state on vp %d",
		mach_error_string (status),
		status,
		vpid->vp));
	cma__bugcheck ("vp_interrupt: thread_get_state");
	}

    stack_array = (int *)(state_array.r29 - (4 * sizeof (int)));
    stack_array[0] = state_array.pc;	/* Save pc */
    stack_array[1] = state_array.r16;	/* Save s0 */
    stack_array[2] = state_array.r4;	/* Save a0 */

    state_array.pc = (int)cma__do_interrupt;
    state_array.r4 = (int)arg;
    state_array.r16 = (int)handler;
    state_array.r29 = (int)stack_array;

    if ((status = thread_set_state (
	    vpid->vp,
# if _CMA_HARDWARE_ == _CMA__MIPS
	    MIPS_THREAD_STATE,
# endif
# if _CMA_HARDWARE_ == _CMA__ALPHA
	    ALPHA_THREAD_STATE,
# endif
	    (thread_state_t)&state_array,
# if _CMA_HARDWARE_ == _CMA__MIPS
	    MIPS_THREAD_STATE_COUNT)) != KERN_SUCCESS) {
# endif
# if _CMA_HARDWARE_ == _CMA__ALPHA
	    ALPHA_THREAD_STATE_COUNT)) != KERN_SUCCESS) {
# endif
	cma__trace ((
		cma__c_trc_vp | cma__c_trc_bug,
		"(vp_interrupt) error \"%s\" (%d) setting state on vp %d",
		mach_error_string (status),
		status,
		vpid->vp));
	cma__bugcheck ("vp_interrupt: thread_set_state");
	}

    /*
     * Now let it run the new code
     */
    if ((status = thread_resume (vpid->vp)) != KERN_SUCCESS) {
	cma__trace ((
		cma__c_trc_vp | cma__c_trc_bug,
		"(vp_interrupt) error \"%s\" (%d) resuming vp %d",
		mach_error_string (status),
		status,
		vpid->vp));
	cma__bugcheck ("vp_interrupt: thread_resume");
	}

#else
    cma__bugcheck ("vp_interrupt: no VPs");
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	resume a suspended VP
 *
 *  FORMAL PARAMETERS:
 *
 *	vpid		Which VP to resume
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
 *	status of operation
 *
 *  SIDE EFFECTS:
 *
 *	none
 * 
 */
cma__t_vp_status
cma__vp_resume(cma__t_vpid		vpid)
    {
#if _CMA_KTHREADS_ == _CMA__MACH
    kern_return_t	status;


    if (!(vpid->flags & cma__c_vp_new)) {
	msg_header_t	mhdr;


	mhdr.msg_simple = cma_c_true;
	mhdr.msg_size = sizeof (mhdr);
	mhdr.msg_type = MSG_TYPE_NORMAL;
	mhdr.msg_local_port = PORT_NULL;
	mhdr.msg_remote_port = vpid->synch;
	mhdr.msg_id = cma__c_vp_msg_resume;

	if ((status = msg_send (&mhdr, MSG_OPTION_NONE, 0)) != SEND_SUCCESS) {
	    cma__trace ((
		    cma__c_trc_vp | cma__c_trc_bug,
		    "(vp_resume) vp %d, port %d; error \"%s\" (%d) on send",
		    vpid->vp,
		    vpid->synch,
		    mach_error_string (status),
		    status));
	    cma__bugcheck ("vp_resume: msg_send");
	    }

	}
    else {
	vpid->flags &= ~cma__c_vp_new;
	vpid->flags |= cma__c_vp_running;

	if ((status = thread_resume (vpid->vp)) != KERN_SUCCESS) {
	    cma__trace ((
		    cma__c_trc_vp | cma__c_trc_bug,
		    "(vp_resume) vp %d; error \"%s\" (%d) on resume",
		    vpid->vp,
		    mach_error_string (status),
		    status));
	    cma__bugcheck ("vp_resume: thread_resume");
	    }

	}
#else
    cma__bugcheck ("vp_resume: no VPs");
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	resume all VPs held for debugging
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	queue of VPs
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	status of operation
 *
 *  SIDE EFFECTS:
 *
 *	none
 * 
 */
cma__t_vp_status
cma__vp_resume_others(void)
    {
#if _CMA_KTHREADS_ == _CMA__MACH
    cma__t_queue	*qptr;


    cma__spinlock (&cma___g_vp_lock);
    qptr = cma__queue_next (&cma___g_vp_queue);

    while (qptr != &cma___g_vp_queue) {
	cma__t_vpid	vptr = (cma__t_vpid)qptr;

	if (vptr->flags & cma__c_vp_hold) {

	    if (thread_resume (vptr->vp) != KERN_SUCCESS) {
		cma__spinunlock (&cma___g_vp_lock);
		cma__bugcheck ("vp_resume_others: thread_resume");
		}

	    vptr->flags &= ~cma__c_vp_hold;
	    }

	qptr = cma__queue_next (qptr);
	}

    cma__spinunlock (&cma___g_vp_lock);
    return cma__c_vp_normal;
#else
    cma__bugcheck ("vp_resume_others: no VPs");
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	set start function and argument for VP
 *
 *  FORMAL PARAMETERS:
 *
 *	vpid		ID of kernel thread
 *	state		state array
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
 *	status
 *
 *  SIDE EFFECTS:
 *
 *	none
 * 
 */
cma__t_vp_status
cma__vp_set_start(cma__t_vpid	vpid,
	cma__t_vp_state	*state)
    {
#if _CMA_KTHREADS_ == _CMA__MACH
    kern_return_t		status;
# if _CMA_HARDWARE_ == _CMA__MIPS
    struct mips_thread_state	thd_state;
# endif
# if _CMA_HARDWARE_ == _CMA__ALPHA
    struct alpha_thread_state	thd_state;
# endif
    int				*stack_array;
# if _CMA_HARDWARE_ == _CMA__MIPS
    int				state_count = MIPS_THREAD_STATE_COUNT;
# endif
# if _CMA_HARDWARE_ == _CMA__ALPHA
    int				state_count = ALPHA_THREAD_STATE_COUNT;
# endif


    stack_array = (int *)(state->stack - (2 * sizeof (int)));
    stack_array[0] = state->tcb;	/* a0 */
    stack_array[1] = 0;			/* Clear out a null a1 */
    thd_state.r28 = (int)(cma__fetch_gp ());	/* Get our gp */
    thd_state.r29 = (int)stack_array;
    thd_state.pc = (int)cma__execute_thread;
    status = thread_set_state (
	    vpid->vp,
# if _CMA_HARDWARE_ == _CMA__MIPS
	    MIPS_THREAD_STATE,
	    (thread_state_t)&thd_state,
	    MIPS_THREAD_STATE_COUNT);
# endif
# if _CMA_HARDWARE_ == _CMA__ALPHA
	    ALPHA_THREAD_STATE,
	    (thread_state_t)&thd_state,
	    ALPHA_THREAD_STATE_COUNT);
# endif

    return status;
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	suspend the current VP
 *
 *  FORMAL PARAMETERS:
 *
 *	vpid		Which VP to suspend (must be caller!)
 *	milliseconds	Timeout value (0 means no timeout)
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
 *	status of operation
 *
 *  SIDE EFFECTS:
 *
 *	none
 * 
 */
cma__t_vp_status
cma__vp_suspend(cma__t_vpid		vpid,
	cma_t_integer		milliseconds)
    {
#if _CMA_KTHREADS_ == _CMA__MACH
    kern_return_t	status;
    msg_header_t	mhdr;
    msg_option_t	mopt;


    vpid->flags &= ~cma__c_vp_running;
    vpid->flags |= cma__c_vp_susp;
    mhdr.msg_simple = cma_c_true;
    mhdr.msg_size = sizeof (mhdr);
    mhdr.msg_local_port = vpid->synch;

    if (milliseconds == 0)
	mopt = MSG_OPTION_NONE;
    else
	mopt = RCV_TIMEOUT;

    status = msg_receive (&mhdr, mopt, milliseconds);

    if (status == RCV_TIMED_OUT)
	return (cma__t_vp_status)status;

    if (status != RCV_SUCCESS) {
	cma__trace ((
		cma__c_trc_vp | cma__c_trc_bug,
		"(vp_suspend) vp %d, port %d; error \"%s\" (%d) on receive",
		vpid->vp,
		vpid->synch,
		mach_error_string (status),
		status));
	cma__bugcheck ("vp_suspend: msg_receive");
	}

    if (mhdr.msg_id != cma__c_vp_msg_resume) {
	cma__trace ((
		cma__c_trc_vp | cma__c_trc_bug,
		"(vp_suspend) vp %d, port %d; received message code of %d",
		vpid->vp,
		vpid->synch,
		mhdr.msg_id));
	cma__bugcheck ("vp_suspend: msg id");
	}

    vpid->flags &= ~cma__c_vp_susp;
    vpid->flags |= cma__c_vp_running;    
    return cma__c_vp_normal;
#else
    cma__bugcheck ("vp_suspend: no VPs");
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	suspend all other VPs for debugging
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	current VP, queue of VPs
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	status of operation
 *
 *  SIDE EFFECTS:
 *
 *	none
 * 
 */
cma__t_vp_status
cma__vp_suspend_others(void)
    {
#if _CMA_KTHREADS_ == _CMA__MACH
    cma__t_vpid		self;
    cma__t_queue	*qptr;


    self = cma__vp_get_id ();
    cma__spinlock (&cma___g_vp_lock);
    qptr = cma__queue_next (&cma___g_vp_queue);

    while (qptr != &cma___g_vp_queue) {
	cma__t_vpid	vptr = (cma__t_vpid)qptr;

	if (self->vp != vptr->vp && !(vptr->flags & cma__c_vp_hold)) {
	    cma__t_vp_status	status;


	    vptr->flags |= cma__c_vp_hold;

	    if ((status = thread_suspend (vptr->vp)) != KERN_SUCCESS) {
		cma__trace ((
			cma__c_trc_vp | cma__c_trc_bug,
			"(vp_suspend_others) error \"%s\" (%d) suspending vp %d",
			mach_error_string (status),
			status,
			vptr->vp));
		}

	    }

	qptr = cma__queue_next (qptr);
	}

    cma__spinunlock (&cma___g_vp_lock);
    return cma__c_vp_normal;
#else
    cma__bugcheck ("vp_suspend_others: no VPs");
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Yield the processor to another kernel thread
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	current VP
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
 *	may result in suspension of this VP while another runnable VP
 *	executes.
 * 
 */
extern void
cma__vp_yield(void)
    {
#if _CMA_KTHREADS_ == _CMA__MACH
    swtch (0);
#else
    cma__bugcheck ("vp_yield: no VPs");
#endif
    }
