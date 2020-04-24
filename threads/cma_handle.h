/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_handle.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:37:25 $
 */
/*
 * HISTORY
 * $Log: cma_handle.h,v $
 * Revision 1.1.8.1  1996/05/10  13:37:25  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:41 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:42 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:00  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:17:39  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  15:59  truitt  /main/HPDCE01/1]
 *
 * 	Change polarity of ifdef for including cma performance fixes
 * 	[1993/10/29  18:43:05  ajayb]
 * 	 *
 *
 * 	Add proper ifdefs to include DEPT changes.
 * 	[1993/10/19  19:37:36  ajayb]
 *
 * 	correcting '#if !defined' syntax
 * 	[1993/09/30  15:27:54  prepetit]
 *
 * 	Merging DEPT changes
 * 	[1993/09/30  15:11:20  prepetit]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  12:45:11  truitt]
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:34  root  1.1.7.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:12:26  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:17:05  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:33:30  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:38:54  alan]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:04:14  bolinger]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:26:03  bolinger]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:34:16  mgm  1.1.1.11]
 *
 * Revision 1.1.4.2  1994/06/09  13:38:05  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:00  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:16:27  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:17:39  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:28:20  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:38:54  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:37:40  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:04:14  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:26:26  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:26:03  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:39:08  devrcs
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
 *	Header file for handles
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	20 July 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	25 August 1989
 *		Modify cma__validate_handle to return internal object
 *		pointer; check object type.
 *	002	Dave Butenhof	15 September 1989
 *		Define symbol for number of object types.
 *	003	Dave Butenhof	19 October 1989
 *		Add macros to encapsulate validation calls, and cast the
 *		result pointer to the appropriate type.
 *	004	Dave Butenhof	19 October 1989
 *		Substitute "cma_t_address" for explicit "void *" to make
 *		porting easier.
 *	005	Bob Conti	14 September 1990
 *		Move pointer to be first in the handle so users can
 *		more easily find it to pass it to debugger commands.
 *	006	Dave Butenhof	14 December 1990
 *		Change cma_attributes.h to cma_attr.h (shorten length)
 *	007	Dave Butenhof	10 February 1992
 *		Add new macros for status-returning validation functions (so
 *		that pthread code doesn't have to use TRY).
 *	008	Dave Butenhof	19 February 1992
 *		Fix type cast errors in 007
 */

#ifndef CMA_HANDLE
#define CMA_HANDLE

/*
 *  INCLUDE FILES
 */

#include <cma_defs.h>
#include <cma_attr.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

#define cma__validate_attr(handle) \
    ((cma__t_int_attr *)cma__validate_handle ( \
	    (cma_t_handle *)(handle), \
	    cma__c_obj_attr))

#define cma__validate_cv(handle) \
    ((cma__t_int_cv *)cma__validate_handle ( \
	    (cma_t_handle *)(handle), \
	    cma__c_obj_cv))

#define cma__validate_mutex(handle) \
    ((cma__t_int_mutex *)cma__validate_handle ( \
	    (cma_t_handle *)(handle), \
	    cma__c_obj_mutex))

#define cma__validate_tcb(handle) \
    ((cma__t_int_tcb *)cma__validate_handle ( \
	    (cma_t_handle *)(handle), \
	    cma__c_obj_tcb))

#define cma__validate_stack(handle) \
    ((cma__t_int_stack *)cma__validate_handle ( \
	    (cma_t_handle *)(handle), \
	    cma__c_obj_stack))

#define cma__validate_null_attr(handle) \
    ((cma__t_int_attr *)cma__validate_handle_null ( \
	    (cma_t_handle *)(handle), \
	    cma__c_obj_attr))

#define cma__validate_null_cv(handle) \
    ((cma__t_int_cv *)cma__validate_handle_null ( \
	    (cma_t_handle *)(handle), \
	    cma__c_obj_cv))

#define cma__validate_null_mutex(handle) \
    ((cma__t_int_mutex *)cma__validate_handle_null ( \
	    (cma_t_handle *)(handle), \
	    cma__c_obj_mutex))

#define cma__validate_null_tcb(handle) \
    ((cma__t_int_tcb *)cma__validate_handle_null ( \
	    (cma_t_handle *)(handle), \
	    cma__c_obj_tcb))

#define cma__validate_null_stack(handle) \
    ((cma__t_int_stack *)cma__validate_handle_null ( \
	    (cma_t_handle *)(handle), \
	    cma__c_obj_stack))

#define cma__val_attr_stat(handle,obj) \
    cma__val_hand_stat ( \
	    (cma_t_handle *)(handle), \
	    cma__c_obj_attr, \
	    (cma__t_object **)obj)

#define cma__val_cv_stat(handle,obj) \
    cma__val_hand_stat ( \
	    (cma_t_handle *)(handle), \
	    cma__c_obj_cv, \
	    (cma__t_object **)obj)

#define cma__val_mutex_stat(handle,obj) \
    cma__val_hand_stat ( \
	    (cma_t_handle *)(handle), \
	    cma__c_obj_mutex, \
	    (cma__t_object **)obj)

#define cma__val_tcb_stat(handle) \
    cma__val_hand_stat ( \
	    (cma_t_handle *)(handle), \
	    cma__c_obj_tcb, \
	    (cma__t_object **)obj)

#define cma__val_stack_stat(handle,obj) \
    cma__val_hand_stat ( \
	    (cma_t_handle *)(handle), \
	    cma__c_obj_stack, \
	    (cma__t_object **)obj)

#define cma__val_nullattr_stat(handle,obj) \
    cma__val_handnull_stat ( \
	    (cma_t_handle *)(handle), \
	    cma__c_obj_attr, \
	    (cma__t_object **)obj)

#define cma__val_nullcv_stat(handle,obj) \
    cma__val_handnull_stat ( \
	    (cma_t_handle *)(handle), \
	    cma__c_obj_cv, \
	    (cma__t_object **)obj)

#define cma__val_nullmutex_stat(handle,obj) \
    cma__val_handnull_stat ( \
	    (cma_t_handle *)(handle), \
	    cma__c_obj_mutex, \
	    (cma__t_object **)obj)

#define cma__val_nulltcb_stat(handle,obj) \
    cma__val_handnull_stat ( \
	    (cma_t_handle *)(handle), \
	    cma__c_obj_tcb, \
	    (cma__t_object **)obj)

#define cma__val_nullstack_stat(handle) \
    cma__val_handnull_stat ( \
	    (cma_t_handle *)(handle), \
	    cma__c_obj_stack, \
	    (cma__t_object **)obj)

#ifndef _HP_CMA_PERF_
#define cma__object_to_handle(addr,handle)				\
    {									\
    cma__t_int_handle	*ihandle__ = (cma__t_int_handle *)handle;	\
    ihandle__->sequence = (addr)->sequence;				\
    ihandle__->type = (addr)->type;					\
    ihandle__->pointer = (addr);					\
    }
#endif /* _HP_CMA_PERF_ */

/*
 * TYPEDEFS
 */

/*
 * Internal format of a handle (to the outside world it's an array of two
 * addresses, but we know better).
 */
typedef struct CMA__T_INT_HANDLE {
    cma__t_object	*pointer;	/* Address of internal structure */
    cma__t_short	sequence;	/* Sequence number of object */
    cma__t_short	type;		/* Type code of object */
    } cma__t_int_handle;

/*
 *  GLOBAL DATA
 */

/*
 * INTERNAL INTERFACES
 */

extern void cma__clear_handle (cma_t_handle *);
#ifdef _HP_CMA_PERF_
extern void cma__object_to_handle (cma__t_object *,cma_t_handle *);
#endif /* _HP_CMA_PERF_ */
extern cma__t_int_attr * cma__validate_default_attr (cma_t_handle *);

extern cma_t_status cma__val_defattr_stat (cma_t_handle *,cma__t_int_attr **);

extern cma__t_object * cma__validate_handle (cma_t_handle *,cma_t_natural );

extern cma_t_status cma__val_hand_stat (cma_t_handle *,cma_t_natural,cma__t_object **);

extern 	cma__t_object	*cma__validate_handle_null (cma_t_handle *,cma_t_natural);

extern cma_t_status cma__val_handnull_stat (cma_t_handle *,cma_t_natural,cma__t_object **);

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_HANDLE.H */
/*  *7    19-FEB-1992 07:16:14 BUTENHOF "Fix type cast errors" */
/*  *6    18-FEB-1992 15:29:03 BUTENHOF "Make status-returning variants of validate functions" */
/*  *5    10-JUN-1991 19:53:22 SCALES "Convert to stream format for ULTRIX build" */
/*  *4    10-JUN-1991 19:20:53 BUTENHOF "Fix the sccs headers" */
/*  *3    10-JUN-1991 18:21:58 SCALES "Add sccs headers for Ultrix" */
/*  *2    14-DEC-1990 00:55:39 BUTENHOF "Change module names" */
/*  *1    12-DEC-1990 21:45:59 BUTENHOF "General handle support" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_HANDLE.H */
