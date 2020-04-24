/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_vm.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:44:21 $
 */
/*
 * HISTORY
 * $Log: cma_vm.h,v $
 * Revision 1.1.8.1  1996/05/10  13:44:21  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:47 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:45 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:53  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:29  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/25  15:15  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  15:50:57  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:45:00  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:15:01  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  19:15:58  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:36:07  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:44:08  alan]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:07:53  bolinger]
 *
 * 	Merge up to BL10+ internal release from DEC.
 * 	[1992/08/11  21:21:00  bolinger]
 *
 * 	Add AIX-specific #ifdef missing from BL10 sources.
 * 	[1992/08/10  16:41:10  bolinger]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:14:01  bolinger]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:36:26  mgm  1.1.1.11]
 *
 * Revision 1.1.4.2  1994/06/09  13:39:10  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:53  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:17:48  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:29  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:32:43  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:44:08  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:42:24  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:07:53  bolinger]
 * 
 * Revision 1.1.2.3  1992/08/14  20:23:49  bolinger
 * 	Merge up to BL10+ internal release from DEC.
 * 	[1992/08/11  21:21:00  bolinger]
 * 
 * 	Add AIX-specific #ifdef missing from BL10 sources.
 * 	[1992/08/10  16:41:10  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/09  00:14:14  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:14:01  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:41:22  devrcs
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
 *	Header file for VM functions
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
 *	001	Dave Butenhof	26 September 1989
 *		Add defs for new routines.
 *	002	Webb Scales	 7 November 1989
 *		Added cma__alloc_object macro
 *	003	Dave Butenhof	29 May 1990
 *		Make VM mutex extern so it can be accessed by rtl linking
 *		against libcma.a.
 *	004	Dave Butenhof	7 June 1990
 *		Add cma__clear_object macro to clear object in debug build.
 *	009	Dave Butenhof	26 June 1990
 *		Replace use of private "VM mutex" with the generic "global
 *		mutex".
 *	010	Paul Curtin	2 August 1990
 *		Added memory definitions and interface definitions.
 *		Replaced memset w/ cma__memset
 *	011	Paul Curtin	22 August 1990
 *		Removed prototype for cma__alloc_mem_nolock.  Adjusted
 *		alloc sizes so as to be dynamic within classes of
 *		structures.
 *	012	Dave Butenhof	29 October 1990
 *		Round up allocation sizes to next octaword (8 byte boundary)
 *		to ensure packets are aligned.
 *	013	Paul Curtin	 7 November 1990
 *		Added 'total' to lookup list structure for mem tracking.
 *	014	Paul Curtin	22 May 1991
 *		Adjusted the number of pages in a memory pool.
 *	015	Paul Curtin	24 May 1991
 *		Added a proto for cma__reinit_memory.
 *	016	Dave Butenhof	10 October 1991
 *		Add cma__free_area to handle recycling large packages
 *		allocated via cma__get_area (such as special stack clusters).
 *		Also, export some of the VM information so cma_debug can
 *		report it.
 *	017	Paul Curtin	18 November 1991
 *		Added a prototype for cma__init_mem_locks.
 *	018	Dave Butenhof	10 February 1992
 *		Add cma__alloc_zero. Make "get_area_nolock" private to vm.
 *	019	Webb Scales	13 May 1992
 *		Added (conditional) initialization to alloc-object.  Added
 *		a conditional routine to do the initialization (expands
 *		harmlessly as a macro for production builds).
 *	020	Webb Scales	19 May 1992
 *		Fix a conditional type-o in the init-object macro
 */

#ifndef CMA_VM
#define CMA_VM

/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_defs.h>
#include <cma_mutex.h>
#include <cma_util.h>
#include <cma_condition.h>
#include <cma_attr.h>
#include <cma_defer.h>
#include <cma_dispatch.h>
#include <cma_stack.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

#define cma__c_small	    0		    /* Index for small areas	     */
#define	cma__c_med	    1		    /* Index for medium areas	     */
#define cma__c_large	    2		    /* Index for large areas	     */
#define cma__c_pool	    3		    /* Indicates size > cma__c_large */
#define cma__c_alloc_sizes  4		    /* Number of allocation sizes    */

#define cma__c_mem_tag	    8		    /* bytes used to store size	     */

/* Size of small allocation	    */
#define cma__c_small_alloc \
    ((cma__max (sizeof (cma__t_vp), sizeof(cma__t_int_alert)) + 7) & ~7)

/* Size of medium allocation    */
#define cma__c_med_alloc \
    ((cma__max (sizeof (cma__t_int_mutex), \
	cma__max(sizeof(cma__t_int_cv), sizeof(cma__t_int_stack))) + 7) & ~7)

/* Size of large allocation	*/
#define cma__c_large_alloc \
    ((cma__max(sizeof(cma__t_int_tcb), sizeof(cma__t_int_attr)) + 7) & ~7)

#define cma__c_pool_size	(32 * cma__c_page_size)

#define cma__alloc_object(type) \
    (type *)cma__init_object (cma__alloc_mem (sizeof (type)))

#define cma__clear_object(object) \
    cma__memset ((cma_t_address)object, 0, sizeof *object)

#ifdef NDEBUG
# define cma__init_object(obj_ptr) (obj_ptr)
#endif

/*
 * TYPEDEFS
 */

typedef struct CMA__T_ALLOC_QUEUE {
    cma_t_integer	size;		/* Size of memory objects */
    cma_t_integer	count;		/* Number of objects in list */
    cma_t_integer	total;		/* Total of objects existing */
    cma__t_int_mutex	*mutex;		/* Mutex for queue access */
    cma__t_queue	queue;		/* Singular look up list */
    } cma__t_alloc_queue;

typedef struct CMA__T_POOL_STAT {
    cma_t_integer	allocs;		/* Number of sbrks or expregs */
    cma_t_integer	bytes_allocd;	/* Number of bytes sbrkd or expregd */
    cma_t_integer	exact_allocs;	/* Number of allocs for exact size */
    cma_t_integer	failures;	/* Number of times allocation failed */
    cma_t_integer	zero_allocs;	/* Number of zeroed allocations */
    cma_t_integer	zero_bytes;	/* Number of zeroed bytes */
    cma_t_integer	extractions;	/* Number of packets extracted */
    cma_t_integer	breakups;	/* Number of packets split up */
    cma_t_integer	returns;	/* Number of packets returned */
    cma_t_integer	merge_befores;	/* Returns merged with previous */
    cma_t_integer	merge_afters;	/* Returns merged with next */
    cma_t_integer	scrounges[3];	/* Scrounges from each list */
    } cma__t_pool_stat;

/*
 *  GLOBAL DATA
 */

extern cma__t_alloc_queue	cma__g_memory[cma__c_alloc_sizes];
extern cma_t_integer		cma__g_page_size;
#if _CMA_OS_ == _CMA__UNIX
 extern cma_t_integer		cma__g_sbrk_align;
#endif
extern cma__t_pool_stat		cma__g_pool_stat;

/*
 * INTERNAL INTERFACE
 */

extern cma_t_address cma__alloc_mem  (cma_t_natural);

extern cma_t_address cma__alloc_zero  (cma_t_natural);

extern void cma__free_mem  (cma_t_address);

extern void cma__init_memory  (void);

extern void cma__init_mem_locks  (void); 

#ifndef NDEBUG
extern cma_t_address cma__init_object  (cma_t_address);
#endif

extern void cma__reinit_memory  (cma_t_integer);	
	
#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_VM.H */
/*   13   15-JUN-1992 16:45:25 KEANE "Octaword align memory allocations on Alpha OSF/1" */
/*  *12   19-MAY-1992 14:20:58 SCALES "Fix typo" */
/*  *11   15-MAY-1992 15:05:35 SCALES "Add additional queue consistency checks" */
/*  *10   18-FEB-1992 15:31:53 BUTENHOF "Add cma__alloc_zero (pre-cleared memory)" */
/*  *9    18-NOV-1991 11:19:40 CURTIN "Added a prototype for cma__init_mem_locks" */
/*  *8    14-OCT-1991 13:42:40 BUTENHOF "Add support for large allocations" */
/*  *7    10-JUN-1991 19:58:13 SCALES "Convert to stream format for ULTRIX build" */
/*  *6    10-JUN-1991 19:22:25 BUTENHOF "Fix the sccs headers" */
/*  *5    10-JUN-1991 18:25:17 SCALES "Add sccs headers for Ultrix" */
/*  *4    24-MAY-1991 16:52:30 CURTIN "Added a new reinit routine" */
/*  *3    23-MAY-1991 16:39:02 CURTIN "Changed the size of pools to accomodate all of init" */
/*  *2    17-DEC-1990 09:33:08 CURTIN "Added field for memory statistics" */
/*  *1    12-DEC-1990 21:55:59 BUTENHOF "VM management" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_VM.H */
