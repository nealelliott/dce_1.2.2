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
 * @(#)Module: cma_stack_int.h $Revision: 1.1.15.1 $ $Date: 1996/05/10 13:42:13 $
 */
/*
 * HISTORY
 * $Log: cma_stack_int.h,v $
 * Revision 1.1.15.1  1996/05/10  13:42:13  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:45 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:44 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  18:10  truitt  /main/HPDCE01/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:36  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:52  jd]
 *
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:45:05  hinman]
 *
 * 	Submitting 102-dme port to 103i
 * 	[1993/05/24  20:17:24  cjd]
 *
 * 	Initial 486 port.
 * 	[1993/05/12  19:31:54  jd]
 *
 * 	Change polarity of ifdef for including cma performance fixes
 * 	[1993/10/29  18:47:35  ajayb]
 * 	 *
 *
 * 	Add proper ifdefs to include DEPT changes.
 * 	[1993/10/19  19:38:04  ajayb]
 *
 * 	Merging DEPT changes
 * 	[1993/09/30  15:07:55  gaz]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  14:54:49  truitt]
 *
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:45:05  hinman]
 *
 * 	Submitting 102-dme port to 103i
 * 	[1993/05/24  20:17:24  cjd]
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:44:20  root  1.1.7.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	redefine "class" and "new" keywords
 * 	[1993/08/03  18:14:02  hopkins]
 *
 * 	Initial 486 port.
 * 	[1993/05/12  19:31:54  jd]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:49:30  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:35:07  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:41:50  alan]
 *
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:53:21  sekhar]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:06:22  bolinger]
 *
 * 	Fix another #if predicate glitch, in which OS == UNIX was substituted
 * 	for VENDOR == IBM, resulting in bad "stack reserve" sizes on non-IBM
 * 	platforms.
 * 	[1992/08/14  20:05:05  bolinger]
 *
 * 	Fix apparent error in #ifdef'ing of default stack and guard
 * 	sizes -- just put back #ifdef's and values in effect in
 * 	dce1.0.1 sources.
 * 	[1992/08/11  21:07:09  bolinger]
 *
 * 	Fix apparent typo in AIX def of cma__c_chunk_count.
 * 	[1992/08/10  16:12:53  bolinger]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:54:53  bolinger]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:35:30  mgm  1.1.1.11]
 *
 * Revision 1.1.10.2  1994/06/09  13:38:51  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:36  devsrc]
 * 
 * Revision 1.1.10.1  1994/04/01  20:17:21  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:52  jd]
 * 
 * Revision 1.1.8.3  1993/06/24  21:03:37  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:45:05  hinman]
 * 
 * Revision 1.1.8.2  1993/05/24  20:50:06  cjd
 * 	Submitting 102-dme port to 103i
 * 	[1993/05/24  20:17:24  cjd]
 * 
 * Revision 1.1.6.2  1993/05/12  19:32:16  jd
 * 	Initial 486 port.
 * 	[1993/05/12  19:31:54  jd]
 * 
 * Revision 1.1.2.4  1992/12/15  22:31:03  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:41:50  alan]
 * 
 * Revision 1.1.2.3  1992/09/29  20:15:40  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:53:21  sekhar]
 * 
 * Revision 1.1.2.2  1992/09/03  14:40:24  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:06:22  bolinger]
 * 
 * Revision 1.1.2.3  1992/08/14  20:22:37  bolinger
 * 	Fix another #if predicate glitch, in which OS == UNIX was substituted
 * 	for VENDOR == IBM, resulting in bad "stack reserve" sizes on non-IBM
 * 	platforms.
 * 	[1992/08/14  20:05:05  bolinger]
 * 
 * 	Fix apparent error in #ifdef'ing of default stack and guard
 * 	sizes -- just put back #ifdef's and values in effect in
 * 	dce1.0.1 sources.
 * 	[1992/08/11  21:07:09  bolinger]
 * 
 * 	Fix apparent typo in AIX def of cma__c_chunk_count.
 * 	[1992/08/10  16:12:53  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:55:12  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:54:53  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:40:53  devrcs
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
 *	CMA services
 *
 *  ABSTRACT:
 *
 *	Header file for stack management (internal to cma_stack.c, but
 *	separate for convenience, and unit testing).
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	30 August 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	4 December 1989
 *		Include cma_tcb_defs.h instead of cma_tcb.h
 *	002	Paul Curtin	27 June	1990
 *		Changed stack structure to reflect new indexing, Added
 *		cma___c_null_reserve_size.
 *	003	Paul Curtin	3 July 1990
 *		Added cma__c_ stack constants here from cma_defs.h
 *	004	Paul Curtin	24 July 1990
 *		Added cma__roundup_chunksize macro
 *	005	Paul Curtin	10 September 1990
 *		Changed default guard size from 10 to 8 (512 byte) pages.
 *	006	Webb Scales	 6 December 1990
 *		Added HP-specific constants.
 *	007	Paul Curtin	19 February 1991
 *		Adjusting cluster size.
 *	008	Dave Butenhof	25 February 1991
 *		Set stack chunk count for AIX.
 *	009	Paul Curtin	23 May 1991
 *		Adjust stack clusters to handle 8 threads.
 *	010	Dave Butenhof and Webb Scales	05 June 1991
 *		Conditionalize vacuous (forward) structure defs, since MIPS C
 *		V2.1 doesn't like (or, apparently, need).
 *	011	Paul Curtin	11 June 1991
 *		Added large stack support; two constants and a struct field.
 *	012	Dave Butenhof	02 October 1991
 *		Integrate changes provided by Alan Peckham to unprotect guard
 *		pages on all stacks before aborting the process; this allows
 *		the UNIX core dump code (which makes the absurd assumption
 *		that the known universe stops just because a page is
 *		protected) to work on threaded processes.
 *	013	Dave Butenhof	11 November 1991
 *		Fix type of cluster_limit and stacks fields to cma_t_address
 *		(rather than cma_t_address *, which doesn't make much sense).
 *	014	Dave Butenhof	22 November 1991
 *		Add a chunk count value for Alpha.
 *	015	Dave Butenhof	28 January 1992
 *		Add new structure type for special clusters, rather than
 *		wasting space on a "thread map" that's not relevant.
 *	016	Dave Butenhof	06 March 1992
 *		Save on swapfile space by cutting stack size (but not on VMS,
 *		since current values were selected for VAX Ada compatibility,
 *		which was considered important).
 *	017	Webb Scales	27 March 1992
 *		Fix a cut-and-paste-o from 016: reset VMS reserve size to the
 *		appropriate numbers for VAX Ada.
 */

#ifndef CMA_STACK_INT
#define CMA_STACK_INT

/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_queue.h>
#include <cma_list.h>
#include <cma_tcb_defs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

#ifdef __cplusplus
#define	protected	c_protected
#endif

#define cma___c_first_free_chunk	0
#define cma___c_min_count	2	/* Smallest number of chunks to leave */
#define cma___c_end		(-1)	/* End of free list (flag) */
#define cma__c_yellow_size	0

/* 
 * Cluster types
 */
#define cma___c_cluster  0	/* Default cluster */
#define cma___c_bigstack 1	/* Looks like a cluster, but it's a stack */


#define cma___c_null_cluster	(cma___t_cluster *)cma_c_null_ptr

/*
 * TYPEDEFS
 */

#ifndef __STDC__
struct CMA__T_INT_STACK;
#endif

typedef cma_t_natural	cma___t_index;	/* Type for chunk index */

typedef struct CMA___T_CLU_DESC {
    cma__t_list		list;		/* Queue element for cluster list */
    cma_t_integer	type;		/* Type of cluster */
    cma_t_address	stacks;
    cma_t_address	limit;
    } cma___t_clu_desc;

typedef union CMA___T_MAP_ENTRY {
    struct {
	cma__t_int_tcb	*tcb;		/* TCB associated with stack chunk */
	struct CMA__T_INT_STACK	*stack;	/* Stack desc. ass. with stack chunk */
	} mapped;
    struct {
	cma___t_index		size;	/* Number of chunks in block */
	cma___t_index		next;	/* Next free block */
	} free;
    } cma___t_map_entry;

/*
 * NOTE: It is VERY IMPORTANT that both cma___t_cluster and cma___t_bigstack
 * begin with the cma___t_clu_desc structure, as there is some code in the
 * stack manager that relies on being able to treat both as equivalent!
 */
typedef struct CMA___T_CLUSTER {
    cma___t_clu_desc	desc;		/* Describe this cluster */
    cma___t_map_entry	map[cma__c_chunk_count];	/* thread map */
    cma___t_index	free;		/* First free chunk index */
    } cma___t_cluster;

/*
 * NOTE: It is VERY IMPORTANT that both cma___t_cluster and cma___t_bigstack
 * begin with the cma___t_clu_desc structure, as there is some code in the
 * stack manager that relies on being able to treat both as equivalent!
 */
typedef struct CMA___T_BIGSTACK {
    cma___t_clu_desc	desc;		/* Describe this cluster */
    cma__t_int_tcb	*tcb;		/* TCB associated with stack */
    struct CMA__T_INT_STACK	*stack;	/* Stack desc. ass. with stack */
    cma_t_natural	size;		/* Size of big stack */
    cma_t_boolean	in_use;		/* Set if allocated */
    } cma___t_bigstack;

#if _CMA_PROTECT_MEMORY_
typedef struct CMA___T_INT_HOLE {
    cma__t_queue	link;		/* Link holes together */
    cma_t_boolean	protected;	/* Set when pages are protected */
    cma_t_address	first;		/* First protected byte */
    cma_t_address	last;		/* Last protected byte */
    } cma___t_int_hole;
#endif

typedef struct CMA__T_INT_STACK {
    cma__t_object	header;		/* Common header (sequence, type info */
    cma__t_int_attr	*attributes;	/* Backpointer to attr obj */
    cma___t_cluster	*cluster;	/* Stack's cluster */
    cma_t_address	stack_base;	/* base address of stack */
    cma_t_address	yellow_zone;	/* first address of yellow zone */
    cma_t_address	last_guard;	/* last address of guard pages */
    cma_t_natural	first_chunk;	/* First chunk allocated */
    cma_t_natural	chunk_count;	/* Count of chunks allocated */
    cma__t_int_tcb	*tcb;		/* TCB backpointer */
#if _CMA_PROTECT_MEMORY_
    cma___t_int_hole	hole;		/* Description of hole */
#endif
    } cma__t_int_stack;

/*
 *  GLOBAL DATA
 */

/*
 * INTERNAL INTERFACES
 */

#ifdef __cplusplus
#undef	protected
#endif

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_STACK_INT.H */
/*  *17   27-MAR-1992 18:05:41 SCALES "Fix VMS reserve size" */
/*  *16   19-MAR-1992 13:17:52 BUTENHOF "Cut stack size on DEC OSF/1" */
/*  *15   29-JAN-1992 14:37:30 BUTENHOF "Improve special cluster management" */
/*  *14   22-NOV-1991 13:38:33 BUTENHOF "Set a chunk count for Alpha" */
/*  *13   11-NOV-1991 11:56:49 BUTENHOF "Fix type of fields" */
/*  *12   14-OCT-1991 13:40:29 BUTENHOF "Unprotect guard pages on abort" */
/*  *11   18-JUL-1991 09:46:17 CURTIN "Added large stack support" */
/*  *10   10-JUN-1991 19:56:48 SCALES "Convert to stream format for ULTRIX build" */
/*  *9    10-JUN-1991 19:21:47 BUTENHOF "Fix the sccs headers" */
/*  *8    10-JUN-1991 18:23:55 SCALES "Add sccs headers for Ultrix" */
/*  *7     5-JUN-1991 17:31:39 BUTENHOF "Conditionalize vacuous defs" */
/*  *6    23-MAY-1991 16:39:26 CURTIN "Changed cluster sizes to handle 8 stacks" */
/*  *5    14-MAR-1991 13:45:54 SCALES "Convert to stream format for ULTRIX build" */
/*  *4    12-MAR-1991 19:36:46 SCALES "Merge Apollo changes to CD4" */
/*  *3    25-FEB-1991 18:26:02 BUTENHOF "Fix up chunk size as variable" */
/*  *2    19-FEB-1991 17:17:29 CURTIN "Made cluster size larger, also uniform on our platforms" */
/*  *1    12-DEC-1990 21:53:29 BUTENHOF "Thread stacks" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_STACK_INT.H */
