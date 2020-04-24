/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_attr.c,v $
 * Revision 1.1.8.1  1996/05/10  13:33:40  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:38 UTC  bissen
 * 	Merge singlestack code.
 * 	[1994/10/27  21:04 UTC  chris_p  /main/HPDCE02/2]
 *
 * 	Change symbol names with "carevue" to "singlestack"
 * 	[1994/10/17  20:28 UTC  chris_p  /main/proven_threads_940803/2]
 *
 * 	Adjust the default stack size.
 * 	[1994/09/08  19:29 UTC  chris_p  /main/proven_threads_940803/1]
 *
 * 	Created branch so singlestack code can be merged to HPDCE02
 * 	[1994/10/27  17:47 UTC  chris_p  /main/HPDCE02/1]
 *
 * Revision 1.1.4.2  1994/06/09  13:37:22  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:22  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:15:30  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:15:56  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:26:07  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:36:10  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:34:54  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:01:43  bolinger]
 * 
 * Revision 1.1.2.3  1992/08/14  20:20:55  bolinger
 * 	Merge up to BL10+ internal release from DEC.
 * 	[1992/08/11  20:51:49  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  22:49:57  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  22:49:38  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:40:38  devrcs
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
 *	Concert Multithread Architecture (CMA) services
 *
 *  ABSTRACT:
 *
 *	Manage attributes objects
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	24 July 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	15 August 1989
 *		Add external interface routines.
 *	002	Dave Butenhof	21 August 1989
 *		Correct some comments.
 *	003	Dave Butenhof	24 August 1989
 *		Convert to use queue package instead of list traversal.
 *	004	Dave Butenhof	30 August 1989
 *		First debugging run: fix problems with cma_c_null and
 *		validation of handles.
 *	005	Dave Butenhof	14 September 1989
 *		Allow cma_c_null on cma_get_*_att routines (return the
 *		default value).
 *	006	Dave Butenhof	15 September 1989
 *		Beef up cache handling with revision counts (to handle
 *		objects which are still "loose" when a cache is flushed
 *		because of modifications to attributes).
 *	007	Dave Butenhof	21 September 1989
 *		cma_set_stacksize_att and cma_set_guardsize_att need to bump
 *		stack revision count as well as TCB count (since they will
 *		also affect stack caching, if ever implemented).
 *	008	Dave Butenhof	29 September 1989
 *		Don't flush TCB cache on priority attribute change: since
 *		cma_fork will always overwrite priority anyway.
 *	009	Bob Conti	6 October 1989
 *		Add call to raise badparam.
 *	010	Dave Butenhof	11 October 1989
 *		Convert to use internal mutex operations.
 *	011	Dave Butenhof	18 October 1989
 *		cma__queue_insert is now a macro, which expands to a block;
 *		this module includes a call which is the sole statement on
 *		the "then" of an if statement, and the trailing ";" (after a
 *		"}") breaks the "else" clause.  Fix the reference in such a
 *		way that some future conversion back to a routine call won't
 *		break it again.
 *	012	Dave Butenhof	19 October 1989
 *		Use new type-specific handle validation macros.
 *	013	Dave Butenhof	19 October 1989
 *		Substitute "cma_t_address" for explicit "void *" to make
 *		porting easier.
 *	014	Dave Butenhof	19 October 1989
 *		Modify use of queue operations to use appropriate casts
 *		rather than depending on ANSI C "void *" (sigh).
 *	015	Webb Scales	19 October 1989
 *		Add type-casts where MIPS pcc requires them.
 *	016	Webb Scales	20 October 1989
 *		Add type-casts on comparison of enum's for MIPS pcc.
 *	017	Webb Scales	20 October 1989
 *		Created a "definition" for cma__g_def_attr.
 *	018	Dave Butenhof	23 October 1989
 *		Fix locking during attributes object free.
 *	019	Dave Butenhof	24 October 1989
 *		Fix some anticipated problems with object caching coherency.
 *	029	Dave Butenhof	25 October 1989
 *		Fix some subtle problems introduced by 019 changes.
 *	030	Dave Butenhof	1 November 1989
 *		Let cma__get_first_mutex handle sequence #.
 *	031	Webb Scales	10 November 1989
 *		In cma__get_attributes, release the attrib->mutex earlier to
 *		avoid a deadlock with cma__get_mutex trying to lock it.
 *	032	Dave Butenhof	30 November 1989
 *		Modify external entries to track POSIX changes to names and
 *		argument ordering.
 *	033	Dave Butenhof	5 December 1989
 *		Fix remaining bug in conversion from "inherit_pri" to
 *		"inherit_sched" (initialization of attribute).
 *	034	Dave Butenhof	5 December 1989
 *		Remove functions dealing with initial_test.
 *	035	Dave Butenhof	8 December 1989
 *		Signal cma_s_unimp when client attempts to change priority or
 *		scheduling policy.
 *	036	Dave Butenhof	27 February 1990
 *		Make scheduling policy/priority conform to new arch. spec;
 *		cma_attr_set_sched takes both policy and priority; support
 *		new symbols for policy and priority.
 *	037	Dave Butenhof	28 February 1990
 *		Ooops--we've got two "cma_c_sched_default" symbols.  Change
 *		the inherit-sched one to "cma_c_sched_use_default".
 *	040	Dave Butenhof	28 February 1990
 *		Ooops--we've got two "cma_c_sched_default" symbols.  Change
 *		the inherit-sched one to "cma_c_sched_use_default".
 *	041	Dave Butenhof	8 March 1990
 *		Raise cma_e_badparam if client tries to set stack size to
 *		0 or less.
 *	042	Dave Butenhof	5 April 1990
 *		Create embedded objects using the same attributes object that
 *		the parent object was created with, to avoid worrying about
 *		locking order protocols during access and deletion.
 *	044	Dave Butenhof	9 April 1990
 *		Use new "known_object" structure for known attributes queue
 *		(includes mutex).
 *	045	Dave Butenhof	10 April 1990
 *		Catch memory errors over object allocation, and set names in
 *		internal objects.
 *	046	Dave Butenhof	12 April 1990
 *		Name embedded mutex in attr obj.
 *	047	Dave Butenhof	1 June 1990
 *		Implement highwater marking on cache queues.
 *	048	Dave Butenhof	7 June 1990
 *		Erase destroyed objects if not NDEBUG, to catch references to
 *		dead objects (can be used in conjunction with cma_maxattr=0,
 *		which disables caching).
 *	049	Dave Butenhof	18 June 1990
 *		Use macros to clear object name (only defined for debug
 *		build).
 *	050	Webb Scales	19 June 1990
 *		"Turn on" scheduling policies and priorities.
 *	051	Dave Butenhof	26 June 1990
 *		Add new attribute for mutex objects; to control whether lock
 *		is "friendly" (supports nested locks by same thread).
 *	052	Paul Curtin	3 July 1990
 *		Added an include for cma_stack_int.h 
 *	053	Paul Curtin	24 July 1990
 *		Putting cma__roundup_chunksize to use.
 *	054	Dave Butenhof	31 July 1990
 *		Increment mutex cache revision count when setting mutex kind.
 *	055	Dave Butenhof	27 August 1990
 *		Change interfaces to pass handles & structures by reference.
 *	056	Dave Butenhof	14 December 1990
 *		Change cma_attributes.h to cma_attr.h (shorten length)
 *	057	Dave Butenhof	12 February 1991
 *		Change "friendly" mutex kind to "recursive", and add
 *		"nonrecursive"
 *	058	Dave Butenhof	01 May 1991
 *		Add arguments to cma__bugcheck() calls.
 *	059	Paul Curtin	10 May 1991
 *		Converted a number of function to use new internal macros.
 *	060	Paul Curtin	24 May 1991
 *		Added a cma__reinit_attr routine.
 *	061	Dave Butenhof	29 May 1991
 *		Raise cma_e_unimp on attempts to modify thread
 *		priority or policy.
 *	062	Paul Curtin	 5 May 1991
 *		Rearranged flags in reinit routine.
 *	063	Dave Butenhof	07 October 1991
 *		Macro-ize guardsize functions.
 *	064	Dave Butenhof	31 October 1991
 *		Fix a bug in attributes object cache recovery, analyzed by
 *		Alan Peckham (thanks!) -- I implemented an attributes
 *		revision count to prevent accidentally caching objects on an
 *		attr obj that had been recycled (potentially with different
 *		attribute values). However, I also CLEARED the revision count
 *		when reusing a cached attr. object, which nullified the fix.
 *		The revision count should never be cleared... in fact,
 *		freeing/reusing an attr object should increment the revision
 *		count to separate it from its past life!
 *	065	Dave Butenhof	20 December 1991
 *		It'd really be nice if cma__init_attr set the mutex_kind of
 *		the default attr. object before creating a mutex using it,
 *		wouldn't it?
 *	066	Dave Butenhof	23 December 1991
 *		Stacks aren't usually cached separately, but there IS an
 *		entry for the cache, and the queue isn't being initialized
 *		in the default attributes object (cma__init_attr). Do so.
 *	067	Dave Butenhof	10 February 1992
 *		A law of nature has just been changed: cma__alloc_mem now
 *		returns cma_c_null_ptr on an allocation failure, rather than
 *		raising an exception. This allows greater efficiency, since
 *		TRY is generally expensive. Anyway, apply the process of
 *		evolution: adapt or die.
 *	068	Dave Butenhof	12 March 1992
 *		Modify fork() wrapper reinit functions to lock/unlock all
 *		known attributes objects rather than only the default:
 *		leaving any unlocked would allow some other thread a chance
 *		to acquire one during prefork processing, leaving the
 *		attributes object locked in the child.
 *	069	Webb Scales	4 June 1992
 *		Fix problem in which attr obj with pending delete set is 
 *		prematurely placed on the cache.
 *	010	Webb Scales	12 June 1992
 *		Removed no longer needed variable from free-attr
 */


/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_defs.h>
#include <cma_attr.h>
#include <cma_errors.h>
#include <cma_handle.h>
#include <cma_init.h>
#include <cma_tcb.h>
#include <cma_mutex.h>
#include <cma_condition.h>
#include <cma_vm.h>
#include <cma_sequence.h>
#include <cma_queue.h>
#include <cma_stack.h>
#include <cma_deb_core.h>
#include <cma_stack_int.h>

/*
 *  GLOBAL DATA
 */

cma__t_int_attr		cma__g_def_attr;

/*
 *  LOCAL DATA
 */

static cma__t_sequence	cma___g_attr_seq;

/*
 * LOCAL FUNCTIONS
 */


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Create a new public attributes structure.
 *
 *  FORMAL PARAMETERS:
 *
 *	new_att		Output handle
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
cma_attr_create(
	cma_t_attr	*new_att,	/* New handle to fill in */
	cma_t_attr	*att)		/* Old attr obj to use */

    {
    cma__t_int_attr     *att_obj;
    cma__t_int_attr     *int_att;


    int_att = cma__validate_default_attr (att);
    att_obj = cma__get_attributes (int_att);

    if ((cma_t_address)att_obj == cma_c_null_ptr)
	cma__error (exc_s_insfmem);
    else
	cma__object_to_handle ((cma__t_object *)att_obj, new_att);

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Delete (free) a public attributes structure.
 *
 *  FORMAL PARAMETERS:
 *
 *	att		Attributes object to free
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
cma_attr_delete(
	cma_t_attr	*att)		/* Attr obj to use */
    {
    cma__t_int_attr     *int_att;


    int_att = cma__validate_null_attr (att);

    if (int_att == (cma__t_int_attr *)cma_c_null_ptr)
	return;

    cma__free_attributes (int_att);
    cma__clear_handle (att);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Get and Set attribute routines
 *
 *  FORMAL PARAMETERS:
 *
 *	att		Attributes object to modify (set) or read (get)
 *
 *	setting		New (set) or Current (get) attribute setting
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

/*
 * Return current setting of "guard size" attribute
 */
extern void
cma_attr_get_guardsize(
	cma_t_attr	*att,		/* Attr obj to use */
	cma_t_natural	*setting)	/* Current setting */
    {
    cma__int_attr_get_guardsize (att, setting);
    }

/*
 * Return current setting of "inherit scheduling" attribute
 */
extern void
cma_attr_get_inherit_sched(
	cma_t_attr		*att,	/* Attr obj to use */
	cma_t_sched_inherit	*setting) /* Current setting */
    {
    cma__int_attr_get_inherit_sched (att, setting);
    }

/*
 * Return current setting of mutex kind attribute
 */
extern void
cma_attr_get_mutex_kind(
	cma_t_attr		*att,	/* Attr obj to use */
	cma_t_mutex_kind	*setting) /* Current setting */
    {
    cma__t_int_attr	*int_att;


    int_att = cma__validate_default_attr (att);
    cma__int_lock (int_att->mutex);
    *setting = int_att->mutex_kind;
    cma__int_unlock (int_att->mutex);
    }

/*
 * Return current setting of "priority" attribute
 */
extern void
cma_attr_get_priority(
	cma_t_attr	*att,		/* Attr obj to use */
	cma_t_priority	*setting)	/* Current setting */
    {
#if (_CMA_KTHREADS_ == _CMA__MACH)
    /*
     * FIX-ME:
     * 
     * Currently, Mach threads don't support POSIX.4 policy, so we can't
     * support this function.  Remove this restriction when possible!
     */
    cma__error (cma_s_unimp);
#else
    cma__int_attr_get_priority (att, setting);
#endif
    }

/*
 * Return current setting of "scheduling policy" attribute
 */
extern void
cma_attr_get_sched(
	cma_t_attr		*att,	/* Attr obj to use */
	cma_t_sched_policy	*setting) /* Current setting */
    {
#if (_CMA_KTHREADS_ == _CMA__MACH)
    /*
     * FIX-ME:
     * 
     * Currently, Mach threads don't support POSIX.4 policy, so we can't
     * support this function.  Remove this restriction when possible!
     */
    cma__error (cma_s_unimp);
#else
    cma__int_attr_get_sched (att, setting);
#endif
    }

/*
 * Return current setting of "stack size" attribute
 */
extern void
cma_attr_get_stacksize(
	cma_t_attr	*att,		/* Attr obj to use */
	cma_t_natural	*setting)	/* Current setting */
    {
    cma__int_attr_get_stacksize (att, setting);
    }

/*
 * Modify current setting of "guard size" attribute
 */
extern void
cma_attr_set_guardsize(
	cma_t_attr	*att,		/* Attr obj to use */
	cma_t_natural	setting)	/* New setting */
    {
    cma__int_attr_set_guardsize (att, setting);
    }

/*
 * Modify current setting of "inherit scheduling" attribute
 */
extern void
cma_attr_set_inherit_sched(
	cma_t_attr		*att,	/* Attr obj to use */
	cma_t_sched_inherit	setting) /* New setting */
    {
    cma__t_int_attr	*int_att;


    int_att = cma__validate_attr (att);

    if (((int)setting != (int)cma_c_sched_inherit)
	    && ((int)setting != (int)cma_c_sched_use_default))
	cma__error (cma_s_badparam);

    cma__int_lock (int_att->mutex);

    if (setting == cma_c_sched_inherit)
	int_att->inherit_sched = cma_c_true;
    else
	int_att->inherit_sched = cma_c_false;

    cma__int_unlock (int_att->mutex);
    }

/*
 * Modify current setting of mutex kind attribute
 */
extern void
cma_attr_set_mutex_kind(
	cma_t_attr		*att,	/* Attr obj to use */
	cma_t_mutex_kind	setting)	/* New setting */
    {
    cma__t_int_attr	*int_att;


    int_att = cma__validate_attr (att);

    if (((int)setting != (int)cma_c_mutex_fast)
	    && ((int)setting != (int)cma_c_mutex_recursive)
	    && ((int)setting != (int)cma_c_mutex_nonrecursive))
	cma__error (cma_s_badparam);

    cma__int_lock (int_att->mutex);
    int_att->mutex_kind = setting;
    int_att->cache[cma__c_obj_mutex].revision++;
    cma__int_unlock (int_att->mutex);
    }

/*
 * Modify current setting of "priority" attribute
 */
extern void
cma_attr_set_priority(
	cma_t_attr	*att,		/* Attr obj to use */
	cma_t_priority	setting)	/* New setting */
    {
#if (_CMA_KTHREADS_ == _CMA__MACH)
    /*
     * FIX-ME:
     * 
     * Currently, Mach threads don't support POSIX.4 policy, so we can't
     * support this function.  Remove this restriction when possible!
     */
    cma__error (cma_s_unimp);
#else
    cma__t_int_attr	*int_att;


    int_att = cma__validate_attr (att);
    cma__int_lock (int_att->mutex);

    switch (int_att->policy) {
	case cma_c_sched_fifo : {

	    if (setting < cma_c_prio_fifo_min
		    || setting > cma_c_prio_fifo_max) {
		cma__int_unlock (int_att->mutex);
		cma__error (cma_s_badparam);
		}

	    break;
	    }
	case cma_c_sched_rr : {

	    if (setting < cma_c_prio_rr_min
		    || setting > cma_c_prio_rr_max) {
		cma__int_unlock (int_att->mutex);
		cma__error (cma_s_badparam);
		}

	    break;
	    }
	case cma_c_sched_throughput : {

	    if (setting < cma_c_prio_through_min
		    || setting > cma_c_prio_through_max) {
		cma__int_unlock (int_att->mutex);
		cma__error (cma_s_badparam);
		}

	    break;
	    }
	case cma_c_sched_background : {

	    if (setting < cma_c_prio_back_min
		    || setting > cma_c_prio_back_max) {
		cma__int_unlock (int_att->mutex);
		cma__error (cma_s_badparam);
		}

	    break;
	    }
	case cma_c_sched_ada_low : {

	    if (setting < cma_c_prio_ada_low_min
		    || setting > cma_c_prio_ada_low_max) {
		cma__int_unlock (int_att->mutex);
		cma__error (cma_s_badparam);
		}

	    break;
	    }
	case cma_c_sched_idle : {

	    if (setting < cma_c_prio_idle_min
		    || setting > cma_c_prio_idle_max) {
		cma__int_unlock (int_att->mutex);
		cma__error (cma_s_badparam);
		}

	    break;
	    }
	default : {
	    cma__bugcheck ("attr_set_priority:1");
	    break;
	    }
	}

    int_att->priority = setting;
    cma__int_unlock (int_att->mutex);
#endif
    }

/*
 * Modify current setting of "scheduling policy" attribute
 */
extern void
cma_attr_set_sched(
	cma_t_attr		*att,	/* Attr obj to use */
	cma_t_sched_policy	setting,	/* New policy */
	cma_t_priority		priority)	/* New priority */
    {
#if (_CMA_KTHREADS_ == _CMA__MACH)
    /*
     * FIX-ME:
     * 
     * Currently, Mach threads don't support POSIX.4 policy, so we can't
     * support this function.  Remove this restriction when possible!
     */
    cma__error (cma_s_unimp);
#else
    cma__t_int_attr	*int_att;


    int_att = cma__validate_attr (att);

    switch (setting) {
	case cma_c_sched_fifo : {

	    if (priority < cma_c_prio_fifo_min
		    || priority > cma_c_prio_fifo_max)
		cma__error (cma_s_badparam);

	    break;
	    }
	case cma_c_sched_rr : {

	    if (priority < cma_c_prio_rr_min
		    || priority > cma_c_prio_rr_max)
		cma__error (cma_s_badparam);

	    break;
	    }
	case cma_c_sched_throughput : {

	    if (priority < cma_c_prio_through_min
		    || priority > cma_c_prio_through_max)
		cma__error (cma_s_badparam);

	    break;
	    }
	case cma_c_sched_background : {

	    if (priority < cma_c_prio_back_min
		    || priority > cma_c_prio_back_max)
		cma__error (cma_s_badparam);

	    break;
	    }
	case cma_c_sched_ada_low : {

	    if (priority < cma_c_prio_ada_low_min
		    || priority > cma_c_prio_ada_low_max)
		cma__error (cma_s_badparam);

	    break;
	    }
	case cma_c_sched_idle : {

	    if (priority < cma_c_prio_idle_min
		    || priority > cma_c_prio_idle_max)
		cma__error (cma_s_badparam);

	    break;
	    }
	default : {
	    cma__error (cma_s_badparam);
	    break;
	    }
	}

    cma__int_lock (int_att->mutex);
    int_att->policy = setting;
    int_att->priority = priority;
    cma__int_unlock (int_att->mutex);
#endif
    }

/*
 * Modify current setting of "stack size" attribute
 */
extern void
cma_attr_set_stacksize(
	cma_t_attr	*att,		/* Attr obj to use */
	cma_t_natural	setting)	/* Current setting */
    {
    cma__int_attr_set_stacksize (att, setting);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Deallocate an attributes structure---don't try to cache it (this is
 *	used to remove attributes from a cache list!)
 *
 *  FORMAL PARAMETERS:
 *
 *	old_attr	Address of the object
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
cma__destroy_attributes(
	cma__t_int_attr	*old_attr)	/* The attr obj to delete */
    {
    /*
     * Free all the objects which are contained in the attributes obj, and
     * then deallocate the object's memory.  THIS ROUTINE ASSUMES THAT THE
     * PARENT ATTRIBUTES OBJECT IS LOCKED, AND THAT THE CALLER MANAGES THE
     * CACHE LINKS.
     */
    cma__assert_warn (
	    cma__int_mutex_locked (old_attr->attributes->mutex),
	    "cma__destroy_attr called without attributes object locked.");

    cma__int_lock (old_attr->mutex);
    cma__free_cache (old_attr, cma__c_obj_attr);
    cma__free_cache (old_attr, cma__c_obj_cv);
    cma__free_cache (old_attr, cma__c_obj_mutex);
    cma__free_cache (old_attr, cma__c_obj_tcb);

    /*
     * If the attributes object has some objects still alive, we can't
     * arbitrarily free the memory (which could have dire consequences when
     * those objects later attempt to lock it and cache themselves).
     * Instead, just mark it as "delete pending", and wait for the objects to
     * go away (the last one will turn the light off).
     */
    if (old_attr->refcnt == 0) {
	cma__int_unlock (old_attr->mutex);
	cma__free_mutex_nolock (old_attr->mutex);
	cma__free_mem ((cma_t_address)old_attr);
	}
    else {
	old_attr->delete_pending = cma_c_true;
	cma__int_unlock (old_attr->mutex);
	}

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Free an attributes object to the cache list
 *
 *  FORMAL PARAMETERS:
 *
 *	attrib	Attributes object to free
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
cma__free_attributes(
	cma__t_int_attr	*attrib)	/* Attributes object to free */
    {
    cma__t_int_attr	*parent;


    parent = attrib->attributes;
    cma__int_lock (parent->mutex);
    cma__int_lock (attrib->mutex);

    /*
     * Check this under the mutex
     */
    (void)cma__assert_fail (
	    attrib->refcnt >= 0,
	    "cma__free_attributes: _refcnt is less than zero");

    /*
     * This will help prevent this object from being used in subsequent
     * creations -- zero indicates an invalid object.
     */
    attrib->header.sequence = 0;

    /*
     * If there are still referants to this attributes object, then
     * don't do anything more.
     */
    if (attrib->refcnt > 0) {
	attrib->delete_pending = cma_c_true;
	cma__int_unlock (attrib->mutex);
	cma__int_unlock (parent->mutex);
	return;
	}

    /*
     * Remove it from known attributes objects list
     */
    cma__int_lock (cma__g_known_atts.mutex);
    (void)cma__queue_remove (&attrib->header.queue);
    cma__int_unlock (cma__g_known_atts.mutex);

    /*
     * If the attributes settings are still compatible with this object to be
     * cached and if the parent attributes object is not being deleted, then
     * add this object to the cache.
     */
    if ((attrib->header.revision == parent->cache[cma__c_obj_attr].revision)
	    && (!parent->delete_pending)) {

	if (parent->cache[cma__c_obj_attr].count
		< cma__g_env[cma__c_env_maxattr].value) {
	    parent->cache[cma__c_obj_attr].count += 1;
	    cma__queue_insert (
		    &attrib->header.queue,
		    &parent->cache[cma__c_obj_attr].queue);

	    /*
	     * TCBs (and stacks, if we decide to cache them) which are attached
	     * to this object shouldn't be cached, since we'll be resetting the
	     * attributes when we reclaim this object from cache.  We do this by
	     * bumping the revision count for the TCB and stack attribute
	     * "classes".  However, make an exception if the stack and guard
	     * sizes are the defaults (since that's what we'll be resetting the
	     * object to).
	     */
#ifdef HP_CMA_SINGLESTACK_OPT
		if ((attrib->stack_size != cma__g_def_attr.stack_size)
			|| (attrib->guard_size != cma__g_def_attr.guard_size)) {
#else
	    if ((attrib->stack_size != cma__c_default_stack)
		    || (attrib->guard_size != cma__c_default_guard)) {
#endif
		attrib->cache[cma__c_obj_tcb].revision++;
		attrib->cache[cma__c_obj_stack].revision++;
		}

	    cma__int_unlock (attrib->mutex);
	    }
	else {
	    cma__int_unlock (attrib->mutex);
	    cma__destroy_attributes (attrib);

	    while ((parent->cache[cma__c_obj_attr].count
		    > cma__g_env[cma__c_env_minattr].value)
		    && (!cma__queue_empty (
			    &parent->cache[cma__c_obj_attr].queue))) {
		cma__t_int_attr	*item;


		item = (cma__t_int_attr *)cma__queue_dequeue (
			&parent->cache[cma__c_obj_attr].queue);
		parent->cache[cma__c_obj_attr].count -= 1;
		cma__destroy_attributes (item);
		}

	    }

	}
    /*
     * Either the parent attributes object is being deleted, or this object
     * has the wrong characteristics to be cached, so destroy it.
     */
    else {
	cma__int_unlock (attrib->mutex);
	cma__destroy_attributes (attrib);
	}

    /*
     * There is now one less reference to the parent, so decrement its 
     * reference count.
     */
    parent->refcnt--;

    /*
     * If parent attributes object is marked for deletion, and the object we
     * just deleted was the last one using it, then delete the parent also.
     */
    if ((parent->refcnt == 0) && (parent->delete_pending)) {
	cma__int_unlock (parent->mutex);
	cma__free_attributes (parent);
	}
    else
	cma__int_unlock (parent->mutex);

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Allocate an attributes object.
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
 *	Address of new attributes object
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern cma__t_int_attr *
cma__get_attributes(
	cma__t_int_attr	*attrib)	/* Attributes object to use */
    {
    cma_t_boolean	new;		    /* true if we need to create attr */
    cma__t_int_attr	*new_attributes;    /* Pointer to new attributes */


    cma__int_lock (attrib->mutex);

    if (attrib->delete_pending) {
	cma__int_unlock (attrib->mutex);
	return (cma__t_int_attr *)cma_c_null_ptr;
	}

    /*
     * Get a pre-owned attributes object if one is available, otherwise create
     * a fresh one.
     */
    if (new = cma__queue_empty (&attrib->cache[cma__c_obj_attr].queue)) {
	new_attributes = cma__alloc_object (cma__t_int_attr);

	if ((cma_t_address)new_attributes == cma_c_null_ptr) {
	    cma__int_unlock (attrib->mutex);
	    return (cma__t_int_attr *)cma_c_null_ptr;
	    }

	}
    else {
	new_attributes = (cma__t_int_attr *)cma__queue_dequeue (
		&attrib->cache[cma__c_obj_attr].queue);
	attrib->cache[cma__c_obj_attr].count -= 1;
	}

    attrib->refcnt++;			/* Count reference to "parent" */

    /*
     * Whether the attributes is old or new, we need to ensure certain field
     * initialization. The most important step is generating a unique
     * sequence number for this new incharnation of the attributes object.
     */
    new_attributes->header.sequence = cma__assign_sequence (&cma___g_attr_seq);
    new_attributes->header.revision = attrib->cache[cma__c_obj_attr].revision;
    cma__obj_clear_name (new_attributes);
    new_attributes->refcnt = 0;
    new_attributes->delete_pending = cma_c_false;
    new_attributes->priority = cma_c_prio_through_mid;
    new_attributes->policy = cma_c_sched_throughput;
    new_attributes->inherit_sched = cma_c_true;

    /*
     * Since the default attributes object can't be changed, it's a good
     * place to find the "official" stack manager's opinion on the default
     * sizes of the stack and guard area. This increases the autonomy of the
     * stack manager, and also improves efficiency (marginally) since the
     * alternative would be a couple of round-up calculations.
     */
    new_attributes->stack_size = cma__g_def_attr.stack_size;
    new_attributes->guard_size = cma__g_def_attr.guard_size;
    new_attributes->mutex_kind = cma_c_mutex_fast;
    cma__int_unlock (attrib->mutex);

    if (new) {
	cma_t_integer	i;


	/*
	 * Initialize the appropriate fields (including creating the mutex
	 * and condition variable).
	 */
	new_attributes->header.type = cma__c_obj_attr;
	new_attributes->attributes = attrib;
	new_attributes->mutex = cma__get_mutex (attrib);
	cma__obj_set_name (new_attributes->mutex, "for an attr");

	for (i = 1; i < cma__c_obj_num; i++)
	    {
	    new_attributes->cache[i].revision = 0;
	    new_attributes->cache[i].count = 0;
	    cma__queue_init (&new_attributes->cache[i].queue);
	    }

	}
    else {
	cma_t_integer	i;

	/*
	 * If we found a quality pre-owned attributes object, clean it up for
	 * resale (note that we do this when we allocate it, not when we free
	 * it: that way free_attributes pays minimal overhead for caching).
	 *
	 * Primarily, this involves deallocating all objects on the cache
	 * list (since they presumably won't be valid in the attributes
	 * object's new incarnation). Also, increment the revision count of
	 * each cache queue, to be sure that no objects created on the old
	 * attributes object can be accidentally cached on the new
	 * incarnation.
	 */
	cma__int_lock (new_attributes->mutex);

	for (i = 1; i < cma__c_obj_num; i++)
	    {
	    new_attributes->cache[i].revision++;
	    cma__free_cache (new_attributes, i);
	    }

	cma__int_unlock (new_attributes->mutex);
	}

    cma__int_lock (cma__g_known_atts.mutex);
    cma__queue_insert (&new_attributes->header.queue, &cma__g_known_atts.queue);
    cma__int_unlock (cma__g_known_atts.mutex);
    return new_attributes;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Initialize CMA_ATTRIBUTES.C local data
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
cma__init_attr(void)
    {
    cma_t_integer	i;


    cma__g_def_attr.header.type		= cma__c_obj_attr;
    cma__g_def_attr.header.sequence	= 1;
    cma__obj_set_name (&cma__g_def_attr, "default attr");
    cma__g_def_attr.priority		= cma_c_prio_through_mid;
    cma__g_def_attr.policy		= cma_c_sched_throughput;
    cma__g_def_attr.inherit_sched	= cma_c_true;
#if 0
    /*
     * The default stack & guard sizes must be adjusted (by rounding up to
     * the system memory protection granularity). The exact size isn't
     * determined until later -- and really the stack manager owns these
     * fields anyway. Therefore, defer the initialization until the stack
     * manager is initialized.
     */
    cma__g_def_attr.stack_size		= cma__c_default_stack;
    cma__g_def_attr.guard_size		= cma__c_default_guard;
#endif
    cma__g_def_attr.mutex_kind		= cma_c_mutex_fast;
    cma__g_def_attr.refcnt		= 0;
    cma__g_def_attr.delete_pending	= cma_c_false;

    for (i = 1; i < cma__c_obj_num; i++)
	{
	cma__g_def_attr.cache[i].revision = 0;
	cma__g_def_attr.cache[i].count = 0;
	cma__queue_init (&cma__g_def_attr.cache[i].queue);
	}

    cma__g_def_attr.mutex	= cma__get_first_mutex (&cma__g_def_attr);
    cma__obj_set_name (cma__g_def_attr.mutex, "default attr's mutex");
    cma___g_attr_seq.mutex	= cma__get_first_mutex (&cma__g_def_attr);
    cma__obj_set_name (cma___g_attr_seq.mutex, "attr sequence object");
    cma___g_attr_seq.seq	= 2;
    cma__g_known_atts.mutex	= cma__get_first_mutex (&cma__g_def_attr);
    cma__obj_set_name (cma__g_known_atts.mutex, "known attr list");
    cma__queue_insert (&cma__g_def_attr.header.queue, &cma__g_known_atts.queue);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Free all the objects which are hung off an attributes object's cache
 *	list.  This must be done when an attributes object structure is
 *	deleted or reused (since the cached items won't be valid).
 *
 *	THIS ROUTINE ASSUMES THAT THE ATTRIBUTES OBJECT MUTEX IS LOCKED!
 *
 *  FORMAL PARAMETERS:
 *
 *	head	Address of queue head for list
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
cma__free_cache(
	cma__t_int_attr	*att,
	cma_t_natural	type)
    {
    cma__assert_warn (
	    cma__int_mutex_locked (att->mutex),
	    "cma__free_cache called without attributes object locked.");

    if (cma__queue_empty (&att->cache[type].queue))
	return;				/* Just return if queue is empty */

    while (! cma__queue_empty (&att->cache[type].queue))
	{
	cma__t_object	*item = (cma__t_object *)cma__queue_dequeue (
		&att->cache[type].queue);

	switch (item->type) {
	    case cma__c_obj_attr : {
		cma__destroy_attributes ((cma__t_int_attr *)item);
		break;
		}
	    case cma__c_obj_cv : {
		cma__destroy_cv ((cma__t_int_cv *)item);
		break;
		}
	    case cma__c_obj_mutex : {
		cma__destroy_mutex ((cma__t_int_mutex *)item);
		break;
		}
	    case cma__c_obj_tcb : {
		cma__destroy_tcb ((cma__t_int_tcb *)item);
		break;
		}
	    default :
		(void)cma__assert_fail (
			0,
			"Bad type code in object at cma__free_cache.");
	    }

	}

    att->cache[type].count = 0;		/* No more cached items */
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Perform work both prior to, and after, a fork depending
 *	upon flag passed in.
 *
 *  FORMAL PARAMETERS:
 *
 *	flag - indicating which work to perform.
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
cma__reinit_attr(
	cma_t_integer	    flag)
    {
    cma__t_int_attr	*next;
    cma__t_queue	*ptr;


    if (flag == cma__c_reinit_prefork_lock) {
	cma__int_lock(cma__g_known_atts.mutex);
	cma__int_lock(cma___g_attr_seq.mutex);
	ptr = cma__queue_next (&cma__g_known_atts.queue);

	while (ptr != &cma__g_known_atts.queue) {
	    next = (cma__t_int_attr *)ptr;
	    cma__int_lock (next->mutex);
	    ptr = cma__queue_next (ptr);
	    }

	}
    else if (flag == cma__c_reinit_postfork_unlock) {	
	ptr = cma__queue_next (&cma__g_known_atts.queue);

	while (ptr != &cma__g_known_atts.queue) {
	    next = (cma__t_int_attr *)ptr;
	    cma__int_unlock (next->mutex);
	    ptr = cma__queue_next (ptr);
	    }

	cma__int_unlock(cma___g_attr_seq.mutex);
	cma__int_unlock(cma__g_known_atts.mutex);
	}

    }

/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ATTR.C */
/*  *18   12-JUN-1992 12:27:04 SCALES "remove unneeded variable from free-attr" */
/*  *17    5-JUN-1992 19:19:54 SCALES "Fix attr obj pending delete problem" */
/*  *16   13-MAR-1992 14:07:32 BUTENHOF "Lock all known attributes objects across fork" */
/*  *15   18-FEB-1992 15:27:20 BUTENHOF "Adapt to new alloc_mem protocol" */
/*  *14   23-DEC-1991 07:52:34 BUTENHOF "Init stack cache queue" */
/*  *13   20-DEC-1991 11:01:44 BUTENHOF "Set mutex kind default before creating mutex" */
/*  *12   31-OCT-1991 12:39:05 BUTENHOF "Fix cache consistency bug!" */
/*  *11   14-OCT-1991 13:37:33 BUTENHOF "Macro-ize guardsize functions" */
/*  *10   10-JUN-1991 18:17:07 SCALES "Add sccs headers for Ultrix" */
/*  *9     5-JUN-1991 16:11:15 CURTIN "fork work" */
/*  *8    29-MAY-1991 16:58:56 BUTENHOF "Disable setting priority/policy for Mach threads" */
/*  *7    24-MAY-1991 16:42:04 CURTIN "Added a new reinit routine" */
/*  *6    10-MAY-1991 11:23:48 CURTIN "Converted functions to use internal macros" */
/*  *5     2-MAY-1991 13:57:26 BUTENHOF "Add argument to cma__bugcheck() calls" */
/*  *4    13-FEB-1991 17:54:29 BUTENHOF "Change mutex attribute name symbols" */
/*  *3    12-FEB-1991 23:09:36 BUTENHOF "Change friendly to recursive" */
/*  *2    14-DEC-1990 00:54:59 BUTENHOF "Change module names" */
/*  *1    12-DEC-1990 21:41:50 BUTENHOF "Attributes" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ATTR.C */
