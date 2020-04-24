/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_attr.h $Revision: 1.1.9.1 $ $Date: 1996/05/10 13:33:51 $
 */
/*
vvvvvvvvvvvvvvvvvvvv
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 * All Rights Reserved
 * (c) Copyright 1990, 1991, 1992
^^^^^^^^^^^^^^^^^^^^
*/
/*
 * HISTORY
 * $Log: cma_attr.h,v $
 * Revision 1.1.9.1  1996/05/10  13:33:51  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:38 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:41 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  13:53  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/20  18:18:57  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:42:59  root  1.1.9.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:11:03  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  17:50:53  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:31:59  hopkins]
 *
 * 	Gamera merge
 * 	[1993/03/26  23:54:01  hopkins]
 *
 * 	Loading drop DCE1_0_2B18
 * 	[1993/03/11  22:25:56  osfrcs  1.1.1.8]
 *
 * 	DCE 1.0.1 Release
 * 	[1992/09/23  20:37:25  hopkins]
 *
 * Revision 1.1.5.2  1994/06/09  13:37:23  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:24  devsrc]
 * 
 * Revision 1.1.5.1  1994/04/01  20:15:32  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:16:00  jd]
 * 
 * Revision 1.1.2.4  1992/12/15  22:26:12  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:36:16  alan]
 * 
 * Revision 1.1.2.3  1992/09/29  20:14:46  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:40:48  sekhar]
 * 
 * Revision 1.1.2.2  1992/09/03  14:35:00  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:01:49  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  22:50:46  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  22:50:31  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:40:39  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *	%W%	(DEC OSF/1)	%G%
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
 *	Header file for attributes object
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
 *	001	Dave Butenhof	24 August 1989
 *		Modify attributes object to use queues instead of lists.
 *	002	Dave Butenhof	15 September 1989
 *		Add attribute revision counts for each object class;
 *		currently only TCBs and stacks have attributes, and stacks
 *		aren't cached, but we do everything, for symmetry.
 *	003	Dave Butenhof	11 October 1989
 *		Convert to use internal mutex operations.
 *	004	Dave Butenhof	19 October 1989
 *		Don't include cma_handle.h, as it now sets up circular
 *		dependencies.
 *	005	Dave Butenhof	24 October 1989
 *		Add "delete-pending" and reference count fields to attributes
 *		object to eliminate some holes in caching.
 *	006	Dave Butenhof	1 November 1989
 *		Add extern for known atts queue.
 *	007	Dave Butenhof	30 November 1989
 *		Add field for scheduling policy (not implemented yet).
 *	008	Dave Butenhof	5 December 1989
 *		Remove initial_test field (since we're not sure what it
 *		should do).
 *	009	Dave Butenhof	9 April 1990
 *		Remove definition of known attributes queue header; it's now
 *		in cma_debugger.
 *	010	Dave Butenhof	26 June 1990
 *		Add new attribute for mutex objects; to control whether lock
 *		is "friendly" (supports nested locks by same thread).
 *	011	Paul Curtin	10 May 1991
 *		Added a number of new macros.
 *	012	Paul Curtin	24 May 1991
 *		Added a prototype for cma__reinit_attr.
 *	013	Dave Butenhof and Webb Scales	05 June 1991
 *		Conditionalize vacuous (forward) structure defs, since MIPS C
 *		V2.1 doesn't like (or, apparently, need).
 *	014	Dave Butenhof	18 September 1991
 *		Integrate HP CMA5 reverse drop.
 *	015	Dave Butenhof	07 October 1991
 *		Macro-ize guardsize functions.
 *	016	Dave Butenhof	31 October 1991
 *		Move initialization of attributes object fields from the
 *		macro for cma_attr_create into the internal creation
 *		function, cma__get_attributes; that way, any (future)
 *		internal-use attr. objs. will be initialized properly.
 *	017	Dave Butenhof	30 January 1992
 *		Allow a guard size of 0 bytes, so that memory-tight
 *		applications can create smaller threads (strictly at their
 *		own risk!)
 *	018	Dave Butenhof	10 February 1992
 *		Remove attributes create macro; functions of CMA and pthread
 *		interfaces now differ slightly (CMA raises exception).
 *	019	Dave Butenhof	12 March 1992
 *		Remove attributes delete macro.
 */

#ifndef CMA_ATTR
#define CMA_ATTR

/*
 *  INCLUDE FILES
 */

#include <cma_defs.h>
#include <cma_queue.h>
#ifdef __hpux
# include <sys/param.h>
#endif
#if _CMA_UNIX_TYPE == _CMA__SVR4
#include <sys/unistd.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__int_attr_get_priority -  Performs the work of cma_attr_get_priority
 *
 *  FORMAL PARAMETERS:
 *
 *	cma_t_attr	    *_att_	- Attribute object to get from
 *	cma_t_priority	    *_setting_	- Current setting
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	priority
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
#define cma__int_attr_get_priority(_att_,_setting_) { \
    cma__t_int_attr     *_int_att_; \
    (_int_att_) = cma__validate_default_attr (_att_); \
    cma__int_lock ((_int_att_)->mutex); \
    (*(_setting_)) = (_int_att_)->priority; \
    cma__int_unlock ((_int_att_)->mutex); \
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__int_attr_get_sched - Performs work of cma_attr_get_sched
 *
 *  FORMAL PARAMETERS:
 *
 *	cma_t_attr	    *_att_	_ Attributes object used
 *	cma_t_sched_policy  *_setting_	- Current setting
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	scheduling policy
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
#define cma__int_attr_get_sched(_att_,_setting_) { \
    cma__t_int_attr     *_int_att_; \
    (_int_att_) = cma__validate_default_attr (_att_); \
    cma__int_lock ((_int_att_)->mutex); \
    (*(_setting_)) = (_int_att_)->policy; \
    cma__int_unlock ((_int_att_)->mutex); \
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__int_attr_get_inherit_sched - Performs work of 
 *	cma_attr_get_inherit_sched
 *
 *  FORMAL PARAMETERS:
 *
 *	cma_t_attr	    *_att_	- Attributes object to use
 *	cma_t_sched_inherit *_setting_	- Current setting
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	Inheritable scheduling policy
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
#define cma__int_attr_get_inherit_sched(_att_,_setting_) { \
    cma__t_int_attr	*_int_att_; \
    (_int_att_) = cma__validate_default_attr (_att_); \
    cma__int_lock ((_int_att_)->mutex); \
    (*(_setting_)) \
        = ((_int_att_)->inherit_sched ? cma_c_sched_inherit : cma_c_sched_use_default); \
    cma__int_unlock ((_int_att_)->mutex); \
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__int_attr_set_stacksize - Performs work for cma_attr_set_stacksize
 *
 *  FORMAL PARAMETERS:
 *
 *      cma_t_attr          *_att_      - Attributes object to use
 *	cma_t_natural	    _setting_	- Setting
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
 *	Change attribute objects stack size setting
 */
#define cma__int_attr_set_stacksize(_att_,_setting_) { \
    cma__t_int_attr     *_int_att_; \
    if ((_setting_) <= 0) \
        cma__error (cma_s_badparam); \
    _int_att_ = cma__validate_attr (_att_); \
    cma__int_lock ((_int_att_)->mutex); \
    _int_att_->stack_size = cma__roundup_chunksize(_setting_); \
    cma__free_cache (_int_att_, cma__c_obj_tcb); \
    _int_att_->cache[cma__c_obj_tcb].revision++; \
    _int_att_->cache[cma__c_obj_stack].revision++; \
    cma__int_unlock (_int_att_->mutex); \
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__int_attr_get_stacksize - Performs work of cma_attr_get_stacksize
 *
 *  FORMAL PARAMETERS:
 *
 *      cma_t_attr          *_att_      - Attributes object to use
 *	cma_t_natural	    *_setting_	- Current setting
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	Attribute objects stack size setting
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
#define cma__int_attr_get_stacksize(_att_,_setting_) { \
    cma__t_int_attr     *_int_att_; \
    (_int_att_) = cma__validate_default_attr (_att_); \
    cma__int_lock ((_int_att_)->mutex); \
    (*(_setting_)) = (_int_att_)->stack_size; \
    cma__int_unlock ((_int_att_)->mutex); \
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__int_attr_set_guardsize - Performs work for cma_attr_set_guardsize
 *
 *  FORMAL PARAMETERS:
 *
 *      cma_t_attr          *_att_      - Attributes object to use
 *	cma_t_natural	    _setting_	- Setting
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
 *	Change attribute objects guard size setting
 */
#define cma__int_attr_set_guardsize(_att_,_setting_) { \
    cma__t_int_attr     *_int_att_; \
    _int_att_ = cma__validate_attr (_att_); \
    cma__int_lock ((_int_att_)->mutex); \
    _int_att_->guard_size = cma__roundup_chunksize(_setting_); \
    cma__free_cache (_int_att_, cma__c_obj_tcb); \
    _int_att_->cache[cma__c_obj_tcb].revision++; \
    _int_att_->cache[cma__c_obj_stack].revision++; \
    cma__int_unlock (_int_att_->mutex); \
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__int_attr_get_guardsize - Performs work of cma_attr_get_guardsize
 *
 *  FORMAL PARAMETERS:
 *
 *      cma_t_attr          *_att_      - Attributes object to use
 *	cma_t_natural	    *_setting_	- Current setting
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	Attribute objects guard size setting
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
#define cma__int_attr_get_guardsize(_att_,_setting_) { \
    cma__t_int_attr     *_int_att_; \
    (_int_att_) = cma__validate_default_attr (_att_); \
    cma__int_lock ((_int_att_)->mutex); \
    (*(_setting_)) = (_int_att_)->guard_size; \
    cma__int_unlock ((_int_att_)->mutex); \
    }

/*
 * TYPEDEFS
 */
#ifndef __STDC__
struct CMA__T_INT_MUTEX;		/* Avoid circular dependency */
#endif

typedef struct CMA__T_CACHE {
    cma_t_natural		revision;	/* Revisions */
    cma_t_natural		count;
    cma__t_queue		queue;	/* Cache headers */
    } cma__t_cache;

typedef struct CMA__T_INT_ATTR {
    cma__t_object		header;		/* Common header */
    struct CMA__T_INT_ATTR	*attributes;	/* Point to controlling attr */
    struct CMA__T_INT_MUTEX	*mutex;		/* Serialize access to object */
    cma_t_priority		priority;	/* Priority of new thread */
    cma_t_sched_policy		policy;		/* Sched policy of thread */
    cma_t_boolean		inherit_sched;	/* Is scheduling inherited? */
    cma_t_natural		stack_size;	/* Size of stack (bytes) */
    cma_t_natural		guard_size;	/* Size of guard (bytes) */
    cma_t_mutex_kind		mutex_kind;	/* Mutex kind */
    cma__t_cache		cache[cma__c_obj_num];	/* Cache information */
    cma_t_boolean		delete_pending;	/* attr. obj. is deleted */
    cma_t_natural		refcnt;	/* Number of objects using attr. obj */
    } cma__t_int_attr;

/*
 *  GLOBAL DATA
 */

extern cma__t_int_attr	cma__g_def_attr;

/*
 * INTERNAL INTERFACES
 */

extern void cma__destroy_attributes  (cma__t_int_attr *);

extern void cma__free_attributes  (cma__t_int_attr	*);

extern void cma__free_cache  (cma__t_int_attr *,cma_t_natural );

extern cma__t_int_attr *cma__get_attributes  (cma__t_int_attr	*);

extern void cma__init_attr  (void);

extern void cma__reinit_attr  (cma_t_integer);

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ATTR.H */
/*  *14   13-MAR-1992 14:07:44 BUTENHOF "Remove int_attr_delete macro" */
/*  *13   18-FEB-1992 15:27:30 BUTENHOF "Adapt to alloc_mem changes" */
/*  *12   30-JAN-1992 11:55:35 BUTENHOF "Allow guard size of 0" */
/*  *11   31-OCT-1991 12:39:16 BUTENHOF "Move attr. obj. field init to get_attributes code" */
/*  *10   14-OCT-1991 13:37:40 BUTENHOF "Macro-ize guardsize functions" */
/*  *9    24-SEP-1991 16:26:16 BUTENHOF "Merge CMA5 reverse IBM/HP/Apollo drops" */
/*  *8    10-JUN-1991 19:50:43 SCALES "Convert to stream format for ULTRIX build" */
/*  *7    10-JUN-1991 19:20:04 BUTENHOF "Fix the sccs headers" */
/*  *6    10-JUN-1991 18:17:23 SCALES "Add sccs headers for Ultrix" */
/*  *5     5-JUN-1991 17:31:03 BUTENHOF "Conditionalize vacuous defs" */
/*  *4    24-MAY-1991 16:42:16 CURTIN "Added a new reinit routine" */
/*  *3    10-MAY-1991 11:24:16 CURTIN "Added a number of new internal macros" */
/*  *2    14-DEC-1990 00:55:06 BUTENHOF "Change module names" */
/*  *1    12-DEC-1990 21:41:53 BUTENHOF "Attributes" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ATTR.H */
