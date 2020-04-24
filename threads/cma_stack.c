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
 * HISTORY
 * $Log: cma_stack.c,v $
 * Revision 1.1.13.1  1996/05/10  13:41:48  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:45 UTC  bissen
 * 	Merge singlestack code.
 * 	[1994/10/27  21:05 UTC  chris_p  /main/HPDCE02/2]
 *
 * 	Changes for growing and shrinking stacks.
 * 	[1994/10/20  20:41 UTC  chris_p  /main/HPDCE02/proven_threads_940803/5]
 *
 * 	Change symbol names with "carevue" to "singlestack"
 * 	[1994/10/17  20:29 UTC  chris_p  /main/HPDCE02/proven_threads_940803/4]
 *
 * 	Fix a few minor problems
 * 	[1994/10/13  20:13 UTC  chris_p  /main/HPDCE02/proven_threads_940803/3]
 *
 * 	Actually do stack growth
 * 	[1994/10/06  21:51 UTC  chris_p  /main/HPDCE02/proven_threads_940803/2]
 *
 * 	Adjust the default stack size.
 * 	[1994/09/08  19:29 UTC  chris_p  /main/HPDCE02/proven_threads_940803/1]
 *
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:44 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  18:18  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  14:43:50  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:44:16  root  1.1.9.3]
 *
 * 	OT8391: Added check of in_use flag before returning a bigstack.
 * 	[1993/08/19  17:23:27  truitt]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:32  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:46  jd]
 *
 * 	added check for in_use flag before returning a bigstack:
 * 	if (big->in_use != cma_c_true &&
 * 	for CR8391
 * 	[1993/08/12  20:51:59  rhw]
 *
 * 	Loading drop DCE1_0_2FNL
 *
 * 	[1993/05/04  22:35:23  mgm  1.1.1.11]
 *
 * Revision 1.1.9.2  1994/06/09  13:38:47  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:32  devsrc]
 * 
 * Revision 1.1.9.1  1994/04/01  20:17:16  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:46  jd]
 * 
 * Revision 1.1.6.2  1993/08/13  14:43:41  rhw
 * 	added check for in_use flag before returning a bigstack:
 * 	  if (big->in_use != cma_c_true &&
 * 	for CR8391
 * 	[1993/08/12  20:51:59  rhw]
 * 
 * Revision 1.1.4.4  1992/12/15  22:30:53  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:41:38  alan]
 * 
 * Revision 1.1.4.3  1992/09/29  20:15:34  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:51:36  sekhar]
 * 
 * Revision 1.1.4.2  1992/09/03  14:40:12  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:06:11  bolinger]
 * 
 * Revision 1.1.2.3  1992/08/14  20:22:31  bolinger
 * 	Merge up to BL10+ internal release from DEC.
 * 	[1992/08/11  21:05:51  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:53:49  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:53:31  bolinger]
 * 
 * Revision 1.1.2.3  1992/07/15  22:47:07  bolinger
 * 	Fix OT defect 4716:  add assertions after two calls to
 * 	cma__get_area() that we got as much memory as we asked for.
 * 	[1992/07/15  22:36:42  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/13  19:56:52  bolinger
 * 	Fix OT defect 2100:  Enable the re-use of the special clusters
 * 	allocated for large thread stacks.  Keep the clusters around
 * 	on cma__g_stack_clusters, and mark them as free for re-use when
 * 	their current owner terminates.
 * 	[1992/05/07  19:53:39  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:40:45  devrcs
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
 *	Stack management for reference implementation
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
 *	001	Webb Scales	25 August 1989
 *		Implement cma_call_on_stack, provide header skeletons for
 *		other public functions.
 *	002	Dave Butenhof	26 August 1989
 *		Implement all public functions.
 *	003	Dave Butenhof	15 September 1989
 *		Fix get_self_tcb to return default TCB if stack isn't in list
 *		(it must be the default user stack, which is the default
 *		thread).
 *	004	Dave Butenhof	29 September 1989
 *		Queue stacks onto owner TCB so they can be cleaned up.  Also,
 *		remove the debugging function calls (conditionalize under
 *		STACK_TRACE).  Also change cma___assign_chunks to external
 *		cma__assign_stack, and simplify argument list.
 *	005	Dave Butenhof	9 October 1989
 *		Make use of cma__error to raise exceptions where necessary.
 *	006	Dave Butenhof	12 October 1989
 *		Use internal mutex operations on tcb->mutex.
 *	007	Dave Butenhof	19 October 1989
 *		Use new type-specific handle validation macros.
 *	008	Dave Butenhof	19 October 1989
 *		Substitute "cma_t_address" for explicit "void *" to make
 *		porting easier.
 *	009	Dave Butenhof	19 October 1989
 *		Modify use of queue operations to use appropriate casts
 *		rather than depending on ANSI C "void *" (sigh).
 *	010	Webb Scales	19 October 1989
 *		Moved comment outside of cma___compute_chunk_index macro, 
 *		and deleted function prototype for it;  created cma__fetch_SP
 *		macro to move the #pragmas out of this file
 *	011	Dave Butenhof	20 October 1989
 *		Lowercase cma__fetch_SP to cma__fetch_sp to fit APA naming
 *		conventions.
 *	012	Webb Scales	20 October 1989
 *		Changed "cma_assem_vms.h" to "cma_assem.h"
 *	013	Dave Butenhof	Halloween 1989
 *		Fix error in delete_stack; unlock stack DB before returning.
 *	014	Dave Butenhof	1 November 1989
 *		Change locking of stack info.
 *	015	Bob Conti	4 November 1989
 *		Removed superfluous include
 *	016	Dave Butenhof	27 November 1989
 *		Implement cma_allocate_stack_np.
 *	017	Dave Butenhof	1 December 1989
 *		Modify external entries to track POSIX changes to names and
 *		argument ordering.
 *	018	Dave Butenhof	8 January 1990
 *		Include cma_vm.h so allocation calls will be right.
 *	019	Dave Butenhof	24 January 1990
 *		Remove internal mutex for stack control: use kernel critical.
 *	020	Dave Butenhof	26 January 1990
 *		Convert stack cluster chain from queue to list, so we can
 *		traverse without locking.  This should increase efficiency in
 *		cma__get_self_tcb and also resolve fix BL1 locking protocol
 *		error.
 *	021	Dave Butenhof	9 February 1990
 *		Add conditional code to handle upward-growing stacks as well
 *		as downward-growing stacks.
 *	022	Dave Butenhof	10 April 1990
 *		Conditionalize the function cma__get_self_tcb; if
 *		_CMA_UNIPROCESSOR_ is defined, this is a macro which will
 *		return the dispatcher's current thread pointer.
 *	023	Webb Scales	16 April 1990
 *		Added a test to get_self_tcb which raises an exception if the
 *		caller is not on a CMA-allocated stack.
 *	024	Webb Scales	30 April 1990
 *		Made initialization of cma__g_current_thread static.
 *	025	Dave Butenhof	18 June 1990
 *		Use macros to clear object name (only defined for debug
 *		build).
 *	026	Paul Curtin	27 June	1990
 *		Reworked stacks; now calculate address. Use chunk boundaries
 *		Add to check_stack_limit, with Bob Conti's help.
 *	027	Paul Curtin	 2 July 1990
 *		Fixed get TCB code, and cleaned up a little
 *	028	Paul Curtin	17 July 1990
 *		Removed an elif.
 *	029	Paul Curtin	24 July 1990
 *		Added round up to stack size equations; using new macro
 *	030	Paul Curtin	 7 August 1990
 *		Replace printf w/ cma__put* functions.
 *		Replace cma__alloc_mem w/ cma__get_area
 *	031	Dave Butenhof	27 August 1990
 *		Change interfaces to pass handles & structures by reference.
 *	032	Paul Curtin	10 September 1990
 *		Added explicit  guardsize roundup to cma__get_stack.
 *	033	Paul Curtin	11 September 1990
 *		Fixed check stack limit.  Go off of sp if necessary.
 *	034	Paul Curtin	11 September 1990
 *		Check stack limit nolonger does DEADBEEF, now read/write.
 *	035	Dave Butenhof	29 November 1990
 *		Implement new versions of 'get-self-tcb' and 'stack-info' to
 *		allow passing in an SP from outside. This supports the Ada
 *		RTL's need for getting per-thread context while operating off
 *		the assigned thread stack (for stack overflow handling).
 *		Eventually, Ada's overflow handling will be incorporated into
 *		CMA.
 *	036	Webb Scales	 6 December 1990
 *		Put in an HP-specific definition for chunk_size.
 *	037	Dave Butenhof	14 December 1990
 *		Change cma_attributes.h to cma_attr.h (shorten length)
 *	038	Dave Butenhof	04 April 1991
 *		Change _CMA_UNIPROCESSOR_ to 0/1 instead of ifdef/ifndef
 *	039	Dave Butenhof	30 April 1991
 *		Add cma__trace calls to provide debug information.
 *	040	Paul Curtin	24 May 1991
 *		Added cma__reinit_stack routine.
 *	041	Paul Curtin	 5 June 1991
 *		Rearranged flags in reinit routine.
 *	042	Paul Curtin	10 June 1991
 *		Added large stack support (> default cluster)
 *		Create cma___stack_last_yellow, added cma___create
 *		_special_cluster, Added cma___delete_special_cluster.
 *	043	Dave Butenhof	19 September 1991
 *		Integrate HPUX CMA5 reverse drop: change size of HP stack
 *		chunk.
 *	044	Dave Butenhof	02 October 1991
 *		Integrate changes provided by Alan Peckham to unprotect guard
 *		pages on all stacks before aborting the process; this allows
 *		the UNIX core dump code (which makes the absurd assumption
 *		that the known universe stops just because a page is
 *		protected) to work on threaded processes.
 *	045	Dave Butenhof	07 November 1991
 *		Fix a bug in large stack creation (cma__get_stack): it fell
 *		through into common code that used "index" variable, which
 *		hadn't been initialized.
 *	046	Dave Butenhof	11 November 1991
 *		Fix compilation warnings in MIPS C.
 *	047	Dave Butenhof	22 November 1991
 *		Add typecast to cma__fetch_sp to simplify cma_host.h and
 *		remove its dependency on cma.h and cma_defs.h types.
 *	048	Paul Curtin	02 December 1991
 *		Removed an erroneous '$'
 *	049	Dave Butenhof	21 January 1992
 *		Fix bug in destroying special stack cluster; it was being
 *		deallocated to pool but left on the cluster list. Since items
 *		can't be removed from the list (it's traversed without
 *		synchronization by cma__get_self_tcb() in non-uniprocessor
 *		variants), leave the first "normal cluster" chunk on the list
 *		and deallocate the rest.
 *	050	Dave Butenhof	28 January 1992
 *		Resolve some remaining issues in "special cluster"
 *		management. First, improve memory usage a little by making
 *		them less like clusters; they really don't need the large
 *		thread map. Second, instead of arbitrarily breaking them up
 *		on deletion, leave them on the queue (but marked "free") so
 *		they can be reused. When a new stack is created and no normal
 *		clusters are available, create_cluster knows how to find a
 *		"big stack" cluster on the cluster list and break it up at
 *		that time.
 *	051	Dave Butenhof	10 February 1992
 *		Move cma__get_self_tcb() to the end of the module; it #undefs
 *		the quick-access uniprocessor cma__get_self_tcb macro, and
 *		(although there seems to be no bad effects now), that could
 *		potentially impact code added later to this module.
 *	052	Dave Butenhof	11 February 1992
 *		A law of nature has just been changed: cma__alloc_mem now
 *		returns cma_c_null_ptr on an allocation failure, rather than
 *		raising an exception. This allows greater efficiency, since
 *		TRY is generally expensive. Anyway, apply the process of
 *		evolution: adapt or die.
 *	053	Dave Butenhof	06 March 1992
 *		Currently, due to the way "picie" (ld's processor for making
 *		RISC code position-independent) modifies the DECthreads
 *		assembly code, we are unable to make cma__do_call_on_stack
 *		function on MIPS DEC OSF/1. Because the failure modes can be
 *		obscure, avoid it by disabling the cma_stack_call_routine_np
 *		function.
 *	054	Webb Scales	13 May 1992
 *		(Conditionally) initialized hole queue links in stack header.
 */	


/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_defs.h>
#include <cma_handle.h>
#include <cma_stack.h>
#include <cma_attr.h>
#include <cma_tcb.h>
#include <cma_queue.h>
#include <cma_list.h>
#include <cma_sequence.h>
#include <cma_mutex.h>
#include <cma_errors.h>
#include <cma_assem.h>
#include <cma_dispatch.h>
#include <cma_vm.h>
#if _CMA_UNIX_TYPE == _CMA__SVR4
#include <sys/unistd.h>
#endif
#if _CMA_PROTECT_MEMORY_
# include <cma_vmprot.h>
#endif

#if (_CMA_OS_ == _CMA__UNIX) && (_CMA_VENDOR_ != _CMA__HP)
extern cma_t_integer getpagesize  (void);
#endif

/*
 * GLOBAL DATA
 */
cma__t_list	cma__g_stack_clusters;
cma__t_int_tcb	*cma__g_current_thread = &cma__g_def_tcb;
cma_t_integer	cma__g_chunk_size;

#ifdef HP_CMA_SINGLESTACK_OPT
extern int cma_singlestack_opt;
#endif

/*
 * LOCAL DATA
 */

static cma__t_int_mutex	*cma___g_stack_mutex;
static cma__t_sequence	cma___g_stack_seq;
static cma_t_address	cma___g_def_stack_limit;
static cma_t_natural	cma___g_def_cluster_size;
#if _CMA_PROTECT_MEMORY_
static cma__t_queue	cma___g_holes;
#endif

/*
 * LOCAL MACROS
 */

/*
 * Convert an address (presumed to be within the specified stack cluster!)
 * into a chunk index.
 *
 * NOTE: If the computed chunk_index is greater than the specified
 * cma__c_chunk_count than we probably have a special, X-tra large
 * cluster here.  Inorder to prevent pointing off the end of the 
 * map[cma__g_chunk_count] entry, which isn't meant to handle special
 * stacks/clusters (it's not needed, there's 1 special stack per special
 * cluster) we will return the greatest valid index.
 *
 * 	cma___t_cluster	*cluster,
 *	cma_t_address	address
 */
#define cma___compute_chunk_index(cluster,address) \
    (((((cma_t_integer)address - (cma_t_integer)((cluster)->desc.stacks)) \
	    / cma__g_chunk_size) >= cma__c_chunk_count) ? \
	(cma__c_chunk_count - 1) : \
	(((cma_t_integer)address - (cma_t_integer)((cluster)->desc.stacks)) \
	    / cma__g_chunk_size))

/*
 * Given cluster address, and known cluster header size, calculate the 
 * address of first chunk in cluster.
 *
 *	cma___t_cluster	*cluster
 *	cma_t_int	header_size
 *
 */
#define cma___compute_first_chunk(cluster,header_size) \
    (((cma_t_integer)(cluster) + (header_size) \
	+ (cma__g_chunk_size)) & ~(cma__g_chunk_size - 1))

/*
 * Find which cluster and chunk index contains the specified SP value.
 *
 *	cma_t_address	sp		SP location to find
 *	cma___t_cluster	**cluster	Address to return cluster
 *	cma___t_index	*index		Address to return chunk index
 *
 */
#define cma___find_sp_cluster(sp,cluster,index) { \
    cma__t_list		*__fsc_clust_q__; \
    cma___t_clu_desc	*__fsc_clust__; \
    cma_t_boolean	__fsc_break__ = cma_c_false; \
    __fsc_clust_q__ = cma__list_next (&cma__g_stack_clusters); \
    while (__fsc_clust_q__ != cma__c_null_list) { \
	__fsc_clust__ = (cma___t_clu_desc *)__fsc_clust_q__; \
        if (((sp) >= __fsc_clust__->stacks) \
                && ((sp) <= __fsc_clust__->limit)) { \
	    if (__fsc_clust__->type == cma___c_bigstack) \
		*(index) = 0; \
	    else \
		*(index) = cma___compute_chunk_index ( \
			(cma___t_cluster *)__fsc_clust__, (sp)); \
	    *(cluster) = (cma___t_cluster *)__fsc_clust__; \
	    __fsc_break__ = cma_c_true; \
	    break; \
	    } \
	__fsc_clust_q__ = cma__list_next (&__fsc_clust__->list); \
	} \
    if (! __fsc_break__) { \
	*(cluster) = cma___c_null_cluster; \
	*(index) = 0; \
	} \
    }

/*
 * Get information on the current stack (stack pointer, which cluster it's
 * in, and what the chunk index is).  If running on the default stack or a
 * non-CMA stack, the cluster is cma_c_null_ptr and the index is 0.
 *
 *	cma_t_address	*sp		Address to return SP
 *	cma___t_cluster	**cluster	Address to return cluster
 *	cma___t_index	*index		Address to return chunk index
 *
 */
#define cma___get_stack_info(sp,cluster,index) { \
    cma_t_address __gsi_stkptr__; \
    __gsi_stkptr__ = (cma_t_address)cma__fetch_sp (); \
    *(sp) = __gsi_stkptr__; \
    cma___find_sp_cluster (__gsi_stkptr__, (cluster), (index)); \
    }

#ifdef HP_CMA_SINGLESTACK_OPT
#define cma___stack_lock(mutex) 		\
    if (cma_singlestack_opt)				\
        cma__g_kernel_critical++;		\
    else								\
        cma__int_lock(mutex)			

#define cma___stack_unlock(mutex) 		\
    if (cma_singlestack_opt)				\
        cma__g_kernel_critical--;		\
    else								\
        cma__int_unlock(mutex)			

#else
#define cma___stack_lock(mutex) 	cma__int_lock(mutex)
#define cma___stack_unlock(mutex) 	cma__int_unlock(mutex)
#endif

/*
 * LOCAL FUNCTIONS
 */
static void cma___assign_stack_nolock  (cma__t_int_stack *,cma__t_int_tcb *);

static void cma___free_chunks  (cma___t_cluster *,cma___t_index ,cma___t_index );

static void cma___free_stack_nolock  (cma__t_int_stack *);	

#if _CMA_STACK_TRACE_

static void
cma___dump_cluster(
	char *,
	int,
	cma___t_cluster *)
    {
    cma___t_index	index;
    char		output[cma__c_buffer_size];

    output[0] = '\0';
    cma__putstring  (output, type);
    cma__putstring  (output, " ");
    cma__putint	    (output, chunks);
    cma__putstring  (output, " chunks in C ");
    cma__puthex	    (output, (long)cluster);
    cma__putstring  (output, " (");
    cma__putint	    (output, cma__c_chunk_count);
    cma__putstring  (output, ",");
    cma__putint	    (output, cma__g_chunk_count * cma__g_chunk_size);
    cma__putstring  (output, "):");
    cma__puteol	    (output);

    if (cluster->desc.type = cma___c_bigstack) {
	cma__putstring (output, "  Big stack, ");
	cma__putint (output, ((cma___t_bigstack *)cluster)->size);
	cma__putstring (output, " bytes.");
	cma__puteol (output);
	}
    else
	for (
		index = cluster->free;
		index != cma___c_end;
		index = cluster->map[index].free.next) {
		    cma__putstring	(output, "  ");
		    cma__putint	(output, cluster->map[index].free.size);
		    cma__putstring	(output, " free chunks at ");
		    cma__putint	(output, index);
		    cma__puteol	(output);
		}

    }
#endif

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine returns a new SP for the caller to use in allocating a
 *	specified amount of new stack space.
 *
 *  FORMAL PARAMETERS:
 *
 *	size		The number of bytes to allocate.
 *
 *	new_stack	The address to which SP should be set to allocate.
 *
 *  IMPLICIT INPUTS:
 *
 *	None
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
 *	Raise an exception if the requested stack space would exceed the
 *	stack's size limit.
 */
extern void
cma_stack_allocate_np(
	cma_t_integer	size,
	cma_t_address	*new_stack)
    {
    cma_t_address	sp;
    cma___t_cluster	*cluster;
    cma___t_index	index;
    cma__t_int_stack	*stack;


    cma___get_stack_info (&sp, &cluster, &index);
#ifdef _CMA_UPSTACK_
    sp = (cma_t_address)((char *)sp + size);
#else
    sp = (cma_t_address)((char *)sp - size);
#endif

    if (cluster == cma___c_null_cluster)
	*new_stack = sp;
    else
	{

	if (cluster->desc.type == cma___c_cluster)
	    stack = cluster->map[index].mapped.stack;
	else
	    stack = ((cma___t_bigstack *)cluster)->stack;

#ifdef _CMA_UPSTACK_
	if (sp > stack->yellow_zone)
#else
	if (sp < stack->yellow_zone)
#endif
	    cma__error (cma_s_stackovf);
	else
	    *new_stack = sp;

	}

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine assigns a stack to thread
 *
 *  FORMAL PARAMETERS:
 *
 *	stack   -  The address of the handle of the stack to be assigned
 *
 *	thread  -  The address of the handle of the thread to be assigned to
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	Modify the appropriate thread map entries for the stack.
 */
extern void
cma_stack_assign_np(
	cma_t_stack_np	*stack,
	cma_t_thread	*thread)
    {
    cma__t_int_stack	*istack;	/* Address of internal stack object */
    cma__t_int_tcb	*tcb;


    cma___stack_lock (cma___g_stack_mutex);
    istack = cma__validate_stack (stack);
    tcb = cma__validate_tcb (thread);
    cma___assign_stack_nolock (istack, tcb);
    cma___stack_unlock (cma___g_stack_mutex);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine calls the specified routine on the specified stack passing
 *	the specified parameter.  The stack is assigned to the thread.
 *
 *  FORMAL PARAMETERS:
 *
 *	stack	The address of the handle of the stack to call on
 * 
 *	routine	The address of the routine to call
 * 
 *	arg	The parameter to pass to the routine
 *
 *	result	Return the result value of the routine
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
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma_stack_call_routine_np(
	cma_t_stack_np		*stack,
	cma_t_call_routine	routine,
	cma_t_address		arg,
	cma_t_address		*result)
    {
#ifdef _CMA_SHLIB_
    cma__error (cma_s_unimp);
#else
    cma__t_int_stack	*istack;    /* Address of internal stack object */
    cma__t_int_tcb	*tcb = cma__get_self_tcb ();


    cma___stack_lock (cma___g_stack_mutex);
    istack = cma__validate_stack (stack);
    cma___assign_stack_nolock (istack, tcb);
    cma___stack_unlock (cma___g_stack_mutex);

    *result = cma__do_call_on_stack (istack->stack_base, routine, arg);
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine performs a limit check on the current stack
 *
 *  FORMAL PARAMETERS:
 *
 *	size  -  The number of bytes by which the stack will be extended
 *
 *  IMPLICIT INPUTS:
 *
 *	The address space  ;-)
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	Returns true (1) if the requested stack extension is valid
 *	Returns false (0) if the requested stack extension is invalid
 *
 *  SIDE EFFECTS:
 *
 *	None (currently)
 */
extern cma_t_boolean
cma_stack_check_limit_np(
	cma_t_integer	size)
    {
    cma_t_address	sp;
    cma___t_cluster	*cluster;
    cma___t_index	index;
    cma__t_int_stack	*stack;
    cma_t_address	ptr;
    cma_t_integer	hold;
    cma_t_address	new_top;
    cma_t_boolean	alloc_ok = cma_c_true;

    cma___get_stack_info (&sp, &cluster, &index);

    if (cluster == cma___c_null_cluster) {

	/*
	 *  Null cluster implies default stack.  Therefore, check global
	 *  variable for top of default stack for overflow.  If overflow
	 *  has occurred then touch each intervening page and then some
	 *  (reserve) inorder to force OS to create pages.
	 *
	 *  Failure to create pages is assumed to raise an exception which
	 *  is caught and converted to a return status.
	 */

	if (cma___g_def_stack_limit == (cma_t_address)0)
	    cma___g_def_stack_limit = sp;

#ifdef _CMA_UPSTACK_
	new_top = (cma_t_address)((char *)sp + (size + cma___c_reserve_size));

	if (new_top > cma___g_def_stack_limit) {
	    TRY {
		ptr = cma___g_def_stack_limit;

		while( ptr < new_top ) {
		    ptr = (cma_t_address)
			  ((cma_t_integer)ptr + cma__g_chunk_size);
		    hold = *(cma_t_integer *)ptr;
		    *(cma_t_integer *)ptr = hold;
		    }

		hold = *(cma_t_integer *)ptr;
		*(cma_t_integer *)new_top = hold;
		cma___g_def_stack_limit = new_top;
		}
	    CATCH_ALL {		    
		alloc_ok = cma_c_false;
		}
	    ENDTRY;
	}
#else
	new_top = (cma_t_address)((char *)sp - (size + cma___c_reserve_size));

	if (new_top < cma___g_def_stack_limit) {
	    TRY {
		ptr = cma___g_def_stack_limit;

		while( ptr > new_top ) {
		    ptr = (cma_t_address)
			  ((cma_t_integer)ptr - cma__g_chunk_size);
		    hold = *(cma_t_integer *)ptr;
		    *(cma_t_integer *)ptr = hold;
		    }

		hold = *(cma_t_integer *)ptr;
		*(cma_t_integer *)new_top = hold;
		cma___g_def_stack_limit = new_top;
		}
	    CATCH_ALL {
		alloc_ok = cma_c_false;
		}
	    ENDTRY;
	    }
#endif
	}   /* close of cluster is null_cluster */
    else
	{

	if (cluster->desc.type == cma___c_cluster)
	    stack = cluster->map[index].mapped.stack;
	else
	    stack = ((cma___t_bigstack *)cluster)->stack;

#ifdef _CMA_UPSTACK_
	if (((cma_t_address)((char *)sp + (size + cma___c_reserve_size))) 
		> stack->yellow_zone)
#else
	if (((cma_t_address)((char *)sp - (size + cma___c_reserve_size))) 
		< stack->yellow_zone)
#endif
	    alloc_ok = cma_c_false;
	else
	    alloc_ok = cma_c_true;

	}

    return (alloc_ok);		    
    }    

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine creates a new stack and returns the handle of the new
 *	stack.
 *
 *  FORMAL PARAMETERS:
 *
 *	stack  -  The address of the handle to receive the address of the stack
 *
 *	att    -  The address of the handle of the attributes for the stack
 *
 *  IMPLICIT INPUTS:
 *
 *	None (currently)
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None (currently)
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma_stack_create_np(
	cma_t_stack_np	*stack,
	cma_t_attr	*att)
    {
    cma__t_int_stack	*istack;
    cma__t_int_attr	*iatt;
    cma__t_int_tcb	*tcb;


    iatt = cma__validate_default_attr (att);

#ifdef HP_CMA_SINGLESTACK_OPT
    istack = cma__get_stack (NULL, iatt);
#else
    istack = cma__get_stack (iatt);
#endif

    if ((cma_t_address)istack == cma_c_null_ptr)
	cma__error (exc_s_insfmem);
    else {
	tcb = cma__get_self_tcb ();
	cma__assign_stack (istack, tcb);	/* Assign the chunks */
	cma__object_to_handle ((cma__t_object *)istack, stack);
	}

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine deletes a stack 
 *
 *  FORMAL PARAMETERS:
 *
 *	stack  -  The address of the handle of the stack to be deleted
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None (currently)
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
cma_stack_delete_np(
	cma_t_stack_np	*stack)
    {
    cma__t_int_stack	*istack;    /* Address of internal stack object */


    cma___stack_lock (cma___g_stack_mutex);
    istack = cma__validate_null_stack (stack);

    if (istack == (cma__t_int_stack *)cma_c_null_ptr) {
	cma___stack_unlock (cma___g_stack_mutex);
	return;
	}

    (void)cma__queue_remove (&istack->header.queue);	/* Remove it from TCB queue */
    cma___free_stack_nolock (istack);
    cma___stack_unlock (cma___g_stack_mutex);
    cma__clear_handle (stack);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Reassign a stack descriptor's chunks to a new TCB (note that this is
 *	just a wrapper for cma___assign_stack_nolock, for use by external
 *	routines which don't have access to the stack database lock, or where
 *	it's not already locked for other reasons).
 *
 *  FORMAL PARAMETERS:
 *
 *	stack		the stack to be reassigned
 *
 *	tcb		the tcb to which it is assigned
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
cma__assign_stack(				/* Assign stack chunks to thread */
	cma__t_int_stack	*stack,	/*  stack to assign them to */
	cma__t_int_tcb		*tcb)	/*  tcb to assign them to */
    {
    cma___stack_lock (cma___g_stack_mutex);
    cma___assign_stack_nolock (stack, tcb);
    cma___stack_unlock (cma___g_stack_mutex);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Free a stack... deallocate the stack chunks, and the stack descriptor
 *	structure.
 *
 *  FORMAL PARAMETERS:
 *
 *	desc		Stack to be freed
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
cma__free_stack(
	cma__t_int_stack	*desc)	/* Stack to free */
    {
    cma___stack_lock (cma___g_stack_mutex);
    cma___free_stack_nolock (desc);
    cma___stack_unlock (cma___g_stack_mutex);
    }


#ifdef HP_CMA_SINGLESTACK_OPT
/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Adjust the stack save area so that it isn't too large or too small.
 *
 *  FORMAL PARAMETERS:
 *
 *	old_tcb		tcb of thread that is saving it's stack
 *	new_tcb		tcb of next thread to run 
 *				It is passed only to make the asm routines cleaner
 *	stack_size	Necessary size to copy the stack.
 */
void cma_singlestack_adjust_stack(
	cma__t_int_tcb    *old_tcb,  	/* tcb to wrk on free */
	cma__t_int_tcb    *new_tcb, 
	int stack_size)  
	{
	cma__t_int_stack *old_stack, *new_stack;
	cma__t_int_attr stack_attr;
	cma__t_int_attr * attr;
	cma_t_natural size;

	old_stack = (cma__t_int_stack *)cma__queue_previous(&old_tcb->stack);
	size = (old_stack->chunk_count * cma__c_page_size) - cma___c_reserve_size;

	if ((size < stack_size)  || (size > (stack_size + cma___c_reserve_size)))
	{
		stack_attr.guard_size = 0;
		stack_attr.stack_size = stack_size;

		attr = old_stack->attributes;

	 	cma___free_stack_nolock ((cma__t_int_stack *)
			cma__queue_remove(&old_stack->header.queue));
	
		new_stack = cma__get_stack(&stack_attr, attr);

		cma___assign_stack_nolock(new_stack, old_tcb);
	}
	}
#else
void cma_singlestack_adjust_stack()
	{
	}
#endif

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Free a list of stacks (traverses from specified stack descriptor to
 *	end of list).
 *
 *  FORMAL PARAMETERS:
 *
 *	list		Queue header of the list to be freed
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
cma__free_stack_list(
	cma__t_queue	*list)		/* Head of stack list to free */
    {
    /*
     * Lock the stack database, since we may be modifying the stack clusters
     */
    cma___stack_lock (cma___g_stack_mutex);

    while (! cma__queue_empty (list))
	cma___free_stack_nolock ((cma__t_int_stack *)cma__queue_dequeue (list));

    cma___stack_unlock (cma___g_stack_mutex);
    }

/*
 * NOTE:
 *
 * cma__get_self_tcb belongs here, by the alphabetical ordering rule.
 * However, it has been moved to the end of the module. It #undefs the macro
 * defined for quicker C access on uniprocessor configurations, and that
 * means that any code later in the module which uses the function would get
 * the call rather than the macro. Moving the definition to the end of the
 * module eliminates that risk.
 */

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Return the TCB associated with some SP value (this is intended to
 *	allow returning the CURRENT thread's TCB when the thread has to
 *	temporarily execute on a different stack; e.g., to handle stack
 *	overflow.  Note that this should be callable from interrupt level!
 *
 *  FORMAL PARAMETERS:
 *
 *	sp	Address of the target SP (may be in yellow zone or guard
 *		page)
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
 *	TCB object (or null if SP isn't in valid thread stack).
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern cma__t_int_tcb *
cma__get_sp_tcb(
	cma_t_address	sp)
    {
    cma___t_cluster	*cluster;
    cma___t_index		index;
    cma__t_int_tcb		*tcb;


    cma___find_sp_cluster (sp, &cluster, &index);

    if (cluster == cma___c_null_cluster)
	if (((cma_t_natural)sp <= cma__c_def_stack_max) 
		&& ((cma_t_natural)sp >= cma__c_def_stack_min))
	    tcb = &cma__g_def_tcb;
	else
	    tcb = (cma__t_int_tcb *)cma_c_null_ptr;
    else {

	if (cluster->desc.type == cma___c_cluster)
	    tcb = cluster->map[index].mapped.tcb;
	else
	    tcb = ((cma___t_bigstack *)cluster)->tcb;

	}

    return tcb;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Create a stack... use the attributes object to determine the proper
 *	sizes.
 *
 *  FORMAL PARAMETERS:
 *
 *	attr		Attributes object
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
 *	stack object
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern cma__t_int_stack *
cma__get_stack(				/* Create a new stack */
#ifdef HP_CMA_SINGLESTACK_OPT
	cma__t_int_attr *new_attr,	/* New attributes for sanity sake */	
#endif
	cma__t_int_attr	*attr)		/* Controlling attr. obj */
    {
    cma__t_int_stack	*new_stack;
    cma_t_natural	chunks;
    cma___t_cluster	*cluster;
    cma_t_boolean	found = cma_c_false;
    cma___t_index	*prev_index, index;

    new_stack = cma__alloc_object (cma__t_int_stack);

    if ((cma_t_address)new_stack == cma_c_null_ptr)
	return (cma__t_int_stack *)cma_c_null_ptr;

#ifdef HP_CMA_SINGLESTACK_OPT    

	if (new_attr == NULL) 
		new_attr = attr;

    new_attr->guard_size = cma__roundup_chunksize(new_attr->guard_size);
    new_attr->stack_size = cma__roundup_chunksize(new_attr->stack_size);
    chunks = (new_attr->stack_size + cma__roundup_chunksize(cma__c_yellow_size) + 
	      new_attr->guard_size + cma__roundup_chunksize(cma___c_reserve_size))
	     / cma__g_chunk_size;

    cma__trace ((
	    cma__c_trc_stack | cma__c_trc_obj,
	    "(get_stack) stack %08x; %d chunks (%d data + %d guard)",
	    new_stack,
	    chunks,
	    new_attr->stack_size,
	    new_attr->guard_size));

   	cma___stack_lock (cma___g_stack_mutex);
    new_stack->header.sequence = cma___g_stack_seq.seq++;
   	cma___stack_unlock (cma___g_stack_mutex);
#else

    attr->guard_size = cma__roundup_chunksize(attr->guard_size);
    attr->stack_size = cma__roundup_chunksize(attr->stack_size);
    chunks = (attr->stack_size + cma__roundup_chunksize(cma__c_yellow_size) + 
	      attr->guard_size + cma__roundup_chunksize(cma___c_reserve_size))
	     / cma__g_chunk_size;

    cma__trace ((
	    cma__c_trc_stack | cma__c_trc_obj,
	    "(get_stack) stack %08x; %d chunks (%d data + %d guard)",
	    new_stack,
	    chunks,
	    attr->stack_size,
	    attr->guard_size));

    new_stack->header.sequence = cma__assign_sequence (&cma___g_stack_seq);
#endif

    new_stack->header.revision = attr->cache[cma__c_obj_stack].revision;
    new_stack->header.type = cma__c_obj_stack;
    cma__obj_clear_name (new_stack);

    new_stack->attributes = attr;
    cma___stack_lock (cma___g_stack_mutex);

    /*
     * Determine if the requested stack will fit within normal cluster size
     * or if it needs a "big stack" cluster.
     */
    if (chunks <= cma__c_chunk_count) {
	cma___t_bigstack	*first_big = (cma___t_bigstack *)cma_c_null_ptr;


	/*
	 * Procede to carve the stack out of a normal cluster.
	 */
	cluster = (cma___t_cluster *)cma__list_next (&cma__g_stack_clusters);

	while ((cma__t_list *)cluster != cma__c_null_list) {
	    /*
	     * Traverse the list of stack clusters; look for 
	     * the first one with enough contiguous free chunks 
	     * to fit the new stack.
	     */
	    if (cluster->desc.type == cma___c_bigstack) {

		if (first_big == (cma___t_bigstack *)cma_c_null_ptr
			&& !((cma___t_bigstack *)cluster)->in_use)
		    first_big = (cma___t_bigstack *)cluster;

		}
	    else {

		if (cluster->free != cma___c_end) {
		    index = cluster->free;
		    prev_index = &cluster->free;

		    while (index != cma___c_end) {
			if (cluster->map[index].free.size >= chunks) {
			    found = cma_c_true;
			    new_stack->first_chunk = index;
			    new_stack->cluster = cluster;
			    break;
			    }

			prev_index = &cluster->map[index].free.next;
			index = cluster->map[index].free.next;
			}

		    }

		if (found)
		    break;

		}

	    cluster = (cma___t_cluster *)cma__list_next (
		    &cluster->desc.list);
	    }

	if (! found) {

	    if (first_big != (cma___t_bigstack *)cma_c_null_ptr) {
		/*
		 * If there's a big stack "cluster" on the list, break it up
		 * into a normal cluster and a smaller big stack (if there's
		 * any left).
		 */
		cma___t_bigstack	*rest;
		cma_t_natural		rest_size;


		cluster = (cma___t_cluster *)first_big;
		rest = (cma___t_bigstack *)(
		    (cma_t_integer)first_big + cma___g_def_cluster_size);
		rest_size = (((cma_t_integer)cluster->desc.limit
		    - (cma_t_integer)cluster) - cma___g_def_cluster_size) + 1;

		/*
		 * Convert the first part of the big stack into a
		 * normal cluster, so we can use it.
		 *
		 * Changing the fields of the cluster shouldn't hurt if any
		 * threads are currently traversing. A traverser may or may
		 * not see inconsistent address limit values as the cluster
		 * header is converted, but the limits will always be within
		 * the cluster... and no threads are running out of the
		 * cluster, or we wouldn't be deleting it!
		 */
		cluster->free = cma___c_first_free_chunk;
		cluster->map[0].free.size = cma__c_chunk_count;
		cluster->map[0].free.next = cma___c_end;
		cluster->desc.stacks =
		    (cma_t_address)(cma___compute_first_chunk (
			    cluster,
			    sizeof(cma___t_cluster)));
		cluster->desc.limit =
		    (cma_t_address)((cma_t_integer)(cluster->desc.stacks)
		    + (cma__c_chunk_count * cma__g_chunk_size) - 1);
		cluster->desc.type = cma___c_cluster;
		cma__trace ((
			cma__c_trc_stack,
			"(get_stack) split big stack: new stack cluster %08x",
			cluster));

		/*
		 * Now, if anything is left, insert the remainder as a new
		 * "big stack".
		 */
		if (rest_size >= cma___g_def_cluster_size) {
		    rest->size = rest_size;
		    rest->desc.stacks = (cma_t_address)(cma___compute_first_chunk (
			rest,
			sizeof(cma___t_clu_desc)));
		    rest->desc.limit =
			(cma_t_address)((cma_t_integer)rest + rest_size - 1);
		    rest->desc.type = cma___c_bigstack;
		    cma__trace ((
			cma__c_trc_stack,
			"(get_stack) remainder: big stack %08x (%d bytes)",
			rest,
			rest_size));
		    rest->in_use = cma_c_false;
		    cma__list_insert (&rest->desc.list, &cma__g_stack_clusters);
		    }
		else {
		    /*
		     * If we get here, "rest_size" ought to be 0, since big
		     * stacks are rounded up to a multiple of the cluster
		     * size. On the other hand, I want to check.
		     */
		    if (rest_size != 0) {
			cma__trace ((
			    cma__c_trc_stack,
			    "(get_stack) ** big stack remnant %08x (%d bytes)",
			    rest,
			    rest_size));
			}

		    }

		}
	    else {
		/*
		 * If we didn't find any space, then we need to create a new
		 * stack cluster and add it to the list.
		 */
		cluster = (cma___t_cluster *)cma__alloc_mem (
			cma___g_def_cluster_size);

		if ((cma_t_address)cluster == cma_c_null_ptr) {
		    cma___stack_unlock (cma___g_stack_mutex);
		    cma__free_mem ((cma_t_address)new_stack);
		    return (cma__t_int_stack *)cma_c_null_ptr;
		    }

		cluster->free = cma___c_first_free_chunk;
		cluster->map[0].free.size = cma__c_chunk_count;
		cluster->map[0].free.next = cma___c_end;
		cluster->desc.stacks = (cma_t_address)(
		    cma___compute_first_chunk (
			    cluster,
			    sizeof(cma___t_cluster)));
		cluster->desc.limit = (cma_t_address)(
		    (cma_t_integer)(cluster->desc.stacks)
		    + (cma__c_chunk_count * cma__g_chunk_size) - 1);
		cluster->desc.type = cma___c_cluster;
		cma__list_insert (
			&cluster->desc.list,
			&cma__g_stack_clusters);
		cma__trace ((
			cma__c_trc_stack,
			"(get_stack) new stack cluster %08x",
			cluster));
		}

	    index = cluster->free;
	    prev_index = &cluster->free;
	    new_stack->cluster = cluster;
	    new_stack->first_chunk = index;
	    }

	cma__trace ((
		cma__c_trc_stack | cma__c_trc_obj,
		"(get_stack) stack %08x in cluster %08x at chunk %d",
		new_stack,
		new_stack->cluster,
		new_stack->first_chunk));
	/*
	 * To avoid wasting space, slice off just the piece that we need; but
	 * only if the space we found is large enough (we want to minimize
	 * fragmentation, so don't leave a space that's too small to use).
	 */
	if (cluster->map[index].free.size > (chunks + cma___c_min_count)) {
	    cluster->map[index + chunks].free.next
		= cluster->map[index].free.next;
	    cluster->map[index + chunks].free.size
		= cluster->map[index].free.size - chunks;

	    *prev_index = index + chunks;
	    new_stack->chunk_count = chunks;
	    }
	else
	    {
	    new_stack->chunk_count = cluster->map[index].free.size;
	    *prev_index = cluster->map[index].free.next;
	    }

	}
    else {
	cma_t_natural 		big_size;
	cma___t_bigstack	*big;


	/*
	 * Determine big stack cluster size: round up to the next multiple of
	 * the default cluster size, so that when we are completely done the
	 * big stack cluster can be used as default clusters.
	 */
	big_size = cma__roundup (
	    ((cma_t_natural)((sizeof(cma___t_bigstack)) 
	     + ((chunks + 1) * cma__g_chunk_size))),
	    cma___g_def_cluster_size
	    );
	
	big = (cma___t_bigstack *)cma__list_next (&cma__g_stack_clusters);

	while ((cma__t_list *)big != cma__c_null_list) {

	    if (big->in_use != cma_c_true &&
		big->desc.type == cma___c_bigstack && big->size >= big_size) {
		found = cma_c_true;
		big->in_use = cma_c_true;
		cluster = (cma___t_cluster *)big;
		break;
		}

	    big = (cma___t_bigstack *)cma__list_next (&big->desc.list);
	    }

	if (! found) {
	    big = (cma___t_bigstack *)(cma__alloc_mem (big_size));

	    if ((cma_t_address)big == cma_c_null_ptr) {
		cma___stack_unlock (cma___g_stack_mutex);
		cma__free_mem ((cma_t_address)new_stack);
		return (cma__t_int_stack *)cma_c_null_ptr;
		}

	    big->desc.stacks = (cma_t_address)(cma___compute_first_chunk (
		big,
		sizeof(cma___t_clu_desc)));
	    big->desc.limit =
		(cma_t_address)((cma_t_integer)big + big_size - 1);
	    big->size = big_size;
	    big->desc.type = cma___c_bigstack;
	    cma__trace ((
		cma__c_trc_stack,
		"(get_stack) new big stack cluster %08x (%d bytes)",
		big,
		big_size));
	    big->tcb = (cma__t_int_tcb *)cma_c_null_ptr;
	    big->stack = (cma__t_int_stack *)cma_c_null_ptr;
	    big->in_use = cma_c_true;
	    cma__list_insert (&big->desc.list, &cma__g_stack_clusters);
	    cluster = (cma___t_cluster *)big;
	    }

	new_stack->first_chunk = 0;
	new_stack->chunk_count = chunks;
	new_stack->cluster = cluster;
	}
	     
    /*
     * Now compute the regions of the stack (reserved, yellow, and guard
     * pages), and set page protections for the guard pages if necessary.
     * Most of the code is conditionally divided into stacks that grow up or
     * stacks that grow down.
     */
#ifdef _CMA_UPSTACK_
    new_stack->last_guard =
	(cma_t_address)
	((cma_t_integer)(cluster->desc.stacks)
	+ ((new_stack->first_chunk + new_stack->chunk_count + 1)
	* cma__g_chunk_size) - 1);
    new_stack->yellow_zone =
	(cma_t_address)
	((cma_t_integer)new_stack->last_guard
	- (attr->guard_size + cma__c_yellow_size) + 1);

    /*
     * This sets the initial SP to the first address of the allocated stack.
     * Which assumes that a push onto SP is sp++.
     */
    new_stack->stack_base = (cma_t_address)(
	(cma_t_integer)(cluster->desc.stacks)
	+ (new_stack->first_chunk * cma__g_chunk_size));

# if _CMA_PROTECT_MEMORY_
    if (new_stack->yellow_zone != new_stack->last_guard) {
	cma__queue_zero (&(new_stack->hole.link));
	new_stack->hole.first = new_stack->yellow_zone;
	new_stack->hole.last = new_stack->last_guard;
	cma__queue_insert (&new_stack->hole.link, &cma___g_holes);
	cma__set_noaccess (new_stack->yellow_zone, new_stack->last_guard);
	new_stack->hole.protected = cma_c_true;
	}
    else
	new_stack->hole.protected = cma_c_false;
# endif
#else					/* ifndef _CMA_UPSTACK_ */
    new_stack->last_guard = 
	(cma_t_address)
	((cma_t_integer)(cluster->desc.stacks)
	+ (new_stack->first_chunk * cma__g_chunk_size));
    new_stack->yellow_zone =
	(cma_t_address)((cma_t_integer)new_stack->last_guard
	+ attr->guard_size + cma__c_yellow_size - 1);

    /*
     * This sets the initial SP to the chunk ABOVE the last chunk we
     * allocated.  Note that if we allocated the last chunk, this is actually
     * outside of the cluster.  If any C RTL does runtime array range
     * checking, this could break.  On the other hand, this is the easiest
     * and probably most portable way to do it.
     *
     * Of course, it also assumes that the correct initial SP is at the
     * address ABOVE where the first datum should be pushed (i.e., that an SP
     * push is --sp).
     */

    new_stack->stack_base =
	(cma_t_address)((cma_t_integer)(cluster->desc.stacks)
	+ (new_stack->first_chunk + new_stack->chunk_count)
	* cma__g_chunk_size);

# if _CMA_PROTECT_MEMORY_
    if (new_stack->yellow_zone != new_stack->last_guard) {
	cma__queue_zero (&(new_stack->hole.link));
	new_stack->hole.first = new_stack->last_guard;
	new_stack->hole.last = new_stack->yellow_zone;
	cma__queue_insert (&new_stack->hole.link, &cma___g_holes);
	cma__set_noaccess (new_stack->last_guard, new_stack->yellow_zone);
	new_stack->hole.protected = cma_c_true;
	}
    else
	new_stack->hole.protected = cma_c_false;
# endif
#endif

    cma__trace ((
	    cma__c_trc_stack | cma__c_trc_obj,
	    "(get_stack) stack %08x: base %08x, yellow %08x, last guard %08x",
	    new_stack,
	    new_stack->stack_base,
	    new_stack->yellow_zone,
	    new_stack->last_guard));

#if _CMA_STACK_TRACE_
    cma___dump_cluster ("Getting", chunks, cluster);
#endif
    new_stack->tcb = (cma__t_int_tcb *)cma_c_null_ptr;
    cma___stack_unlock (cma___g_stack_mutex);

    return new_stack;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Initialize CMA_STACK.C local data
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	cma___g_first_array
 *
 *  IMPLICIT OUTPUTS:
 *
 *	creates initial stack cluster
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
cma__init_stack(void)
    {


    cma___g_stack_mutex = cma__get_mutex (&cma__g_def_attr);
    cma__obj_set_name (cma___g_stack_mutex, "stack mutex");
    cma__init_sequence (&cma___g_stack_seq);
    cma__obj_set_name (cma___g_stack_seq.mutex, "stack sequence object");

    /*
     * Right now, chunk size is the same as the system "hardware" page size
     * (the memory protection granularity). Since cma__init_memory has
     * already calculated the correct value, just copy it.
     */
#	if _CMA_UNIX_TYPE == _CMA__SVR4  
	if ( (cma__g_chunk_size = sysconf( _SC_PAGESIZE )) == -1 )
		{
		cma__bugcheck("sysconf error:1");
		}
#	else	/* if _CMA_UNIX_TYPE == _CMA__SVR4  */
	
	    cma__g_chunk_size = cma__g_page_size;
# 	endif
    /*
     * Calculate the size of a default cluster at startup, since it involves
     * rounding up to the chunk size. Saves a lot of overhead later.
     */
    cma___g_def_cluster_size = cma__roundup (
	    ((cma_t_integer)((cma_t_integer)sizeof(cma___t_cluster)
		+ ((cma__c_chunk_count + 1) * cma__g_chunk_size))),
	    cma__g_chunk_size);

    /*
     * Set the default guard & stack sizes
     */
#ifdef HP_CMA_SINGLESTACK_OPT
	if (cma_singlestack_opt) {
    	cma__g_def_attr.guard_size = 
		cma__roundup_chunksize ((cma__c_page_size * 0));
    	cma__g_def_attr.stack_size = 
		cma__roundup_chunksize ((cma__c_page_size * 2));
	} else 
#endif
	{
    	cma__g_def_attr.guard_size = 
		cma__roundup_chunksize (cma__c_default_guard);
    	cma__g_def_attr.stack_size = 
		cma__roundup_chunksize (cma__c_default_stack);
	}

#if _CMA_PROTECT_MEMORY_
    cma__queue_init (&cma___g_holes);
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *      Performs pre- or post- `fork() reinitialization' work.
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
cma__reinit_stack(
	cma_t_integer	flag)
    {

    if (flag == 0) {			    /* pre fork work */
	cma___stack_lock(cma___g_stack_mutex);
	}
    else if (flag == 1) {   /* post fork work, out of kernel */
	cma___stack_unlock(cma___g_stack_mutex);
	}

    }


#if _CMA_PROTECT_MEMORY_
/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Remap all stacks so that protected guard pages won't trigger a bug
 *	in UNIX core dump code (it stops when it finds a protected page).
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	cma___g_holes - queue of holes to re-map.
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
cma__remap_stack_holes(void)
    {
    cma___t_int_hole *desc;

    /*
     * WARNING: This should really lock the stack database, but since it's
     * called from a signal handler, it really can't. Therefore, be very
     * careful not to call it anywhere else!
     */

    while (desc = (cma___t_int_hole *)cma__queue_dequeue(&cma___g_holes))
	{
	cma__set_writable (desc->first, desc->last);
	desc->protected = cma_c_false;
	}

    }
#endif /* _CMA_PROTECT_MEMORY_ */

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Assign a stack descriptor's chunks to a new TCB
 *
 *  FORMAL PARAMETERS:
 *
 *	stack		the stack to be reassigned
 *
 *	tcb		the tcb to which it is assigned
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
static void
cma___assign_stack_nolock(				/* Assign stack chunks to thread */
	cma__t_int_stack	*stack,	/*  stack to assign them to */
	cma__t_int_tcb		*tcb)	/*  tcb to assign them to */
    {
    cma___t_cluster	*cluster = stack->cluster;
    cma___t_index	index;
    cma___t_index	first = stack->first_chunk;
    cma_t_natural	count = stack->chunk_count;


    cma__trace ((
	    cma__c_trc_stack,
	    "(assign_stack_nolock) stack %08x assigned to tcb %08x",
	    stack,
	    tcb));

    /*
     * This routine assumes that the caller has locked the stack database!
     */
#ifndef HP_CMA_SINGLESTACK_OPT
    cma__assert_warn (
	cma__int_mutex_locked (cma___g_stack_mutex),
	"cma___assign_stack_nolock called without stack database locked.");
#endif

    if (cluster->desc.type == cma___c_bigstack) {
	((cma___t_bigstack *)cluster)->tcb = tcb;
	((cma___t_bigstack *)cluster)->stack = stack;
	}
    else {

	for (index = first; index < (first + count); index++) {
	    cluster->map[index].mapped.tcb = tcb;
	    cluster->map[index].mapped.stack = stack;
	    }

	}

    /*
     * If the stack is currently attached to a TCB, remove it from the queue.
     */
    if (stack->tcb != (cma__t_int_tcb *)cma_c_null_ptr) {
	cma__int_lock (stack->tcb->mutex);
	(void)cma__queue_remove (&stack->header.queue);
	cma__int_unlock (stack->tcb->mutex);
	}

#ifdef HP_CMA_SINGLESTACK_OPT
	if ((cma_singlestack_opt) && (cma__g_kernel_critical)) {
    	cma__queue_insert (&stack->header.queue, &tcb->stack);
	} else 
#endif
		{
    	cma__int_lock (tcb->mutex);
    	cma__queue_insert (&stack->header.queue, &tcb->stack);
    	cma__int_unlock (tcb->mutex);
		}
    stack->tcb = tcb;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Free a set of chunks in a stack cluster
 *
 *  FORMAL PARAMETERS:
 *
 *	cluster		Stack cluster
 *
 *	low_chunk	The lowest chunk to be freed
 *
 *	high_chunk	The highest chunk to be freed
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
static void
cma___free_chunks(				/* Deallocate chunks */
	cma___t_cluster	*cluster,	/*  stack cluster */
	cma___t_index	low_chunk,	/*  low limit */
	cma___t_index	high_chunk)	/*  high limit */
    {
    cma___t_index	index, next, first_chunk, previous;
    cma_t_natural	total_size;


    cma__trace ((
	    cma__c_trc_stack,
	    "(free_chunks) freeing cluster %08x, chunks %d to %d",
	    cluster,
	    low_chunk,
	    high_chunk));

    for (index = low_chunk; index <= high_chunk; index++) {
	cluster->map[index].mapped.tcb = (cma__t_int_tcb *)cma_c_null_ptr;
	cluster->map[index].mapped.stack = (cma__t_int_stack *)cma_c_null_ptr;
	}

    total_size = high_chunk - low_chunk + 1;

    if (cluster->free == cma___c_end) {
	/*
	 * If the free list is empty, just add the new batch and forget it.
	 */
	cluster->map[low_chunk].free.next = cma___c_end;
	cluster->map[low_chunk].free.size = total_size;
	cluster->free = low_chunk;
	}
    else {
	/*
	 * The free list isn't empty.  We want to find where the new batch
	 * should be (in index order), and attempt to merge it with the
	 * previous and/or following elements of the free list to avoid
	 * fragmentation.
	 */
	if (cluster->free > low_chunk) {
	    /*
	     * Special case if it goes before the first element on the list,
	     * since we don't have a previous element...
	     */
	    first_chunk = low_chunk;
	    next = cluster->free;
	    cluster->free = low_chunk;
	    }
	else {
	    /*
	     * Find the last element in the list with a lower index than the
	     * new block of chunks, and see if we can merge with it.
	     */
	    next = cluster->free;

	    while ((next != cma___c_end) && (next < low_chunk)) {
		previous = next;
		next = cluster->map[next].free.next;
		}

	    if (low_chunk == (cluster->map[previous].free.size + previous)) {
		/*
		 * If it rests neatly atop the previous element, merge them.
		 */
		first_chunk = previous;
		total_size += cluster->map[previous].free.size;
		}
	    else {
		/*
		 * If it can't merge, point the previous element at it and
		 * set up to check for merge with next element
		 */
		first_chunk = low_chunk;
		cluster->map[previous].free.next = low_chunk;
		}

	    }

	/*
	 * Now, see whether we can merge the element with the next one (if
	 * any).
	 */
	if (next != cma___c_end) {
	    if ((first_chunk + total_size) == next) {
		/*
		 * Merge them!
		 */
		total_size += cluster->map[next].free.size;
		next = cluster->map[next].free.next;
		}

	    cluster->map[first_chunk].free.next = next;
	    }
	else
	    cluster->map[first_chunk].free.next = cma___c_end;

	cluster->map[first_chunk].free.size = total_size;
	}

#if _CMA_STACK_TRACE_
    cma___dump_cluster ("Freeing", high_chunk - low_chunk + 1, cluster);
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Free a stack... deallocate the stack chunks, and the stack descriptor
 *	structure.  This assumes the caller has locked the stack database.
 *
 *  FORMAL PARAMETERS:
 *
 *	desc		Stack to be freed
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
static void
cma___free_stack_nolock(
	cma__t_int_stack	*desc)	/* Stack to free */
    {
    cma___t_cluster	*cluster = desc->cluster;


    cma__trace ((
	    cma__c_trc_stack | cma__c_trc_obj,
	    "(free_stack_nolock) freeing stack %08x",
	    desc));

    /*
     * This routine assumes that the caller has locked the stack database!
     */
#ifndef HP_CMA_SINGLESTACK_OPT
    cma__assert_warn (
	cma__int_mutex_locked (cma___g_stack_mutex),
	"cma___free_stack_nolock called without stack database locked.");
#endif

#if _CMA_PROTECT_MEMORY_
    if (desc->hole.protected) {
	/*
	 * Unprotect the yellow zone and guard pages before returning stack
	 * chunks to the free list.
	 */
# ifdef _CMA_UPSTACK_
	cma__set_writable (desc->yellow_zone, desc->last_guard);
# else
	cma__set_writable (desc->last_guard, desc->yellow_zone);
# endif
	(void)cma__queue_remove (&desc->hole.link);
	desc->hole.protected = cma_c_false;
	}
#endif

    if (cluster->desc.type == cma___c_cluster)
	cma___free_chunks (
		cluster,
		desc->first_chunk,
		desc->first_chunk + desc->chunk_count - 1);
    else
	((cma___t_bigstack *)cluster)->in_use = cma_c_false;

    cma__free_mem ((cma_t_address)desc);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Find the current thread's TCB.  This assumes that the SP is within a
 * 	stack controlled by the stack manager (or on the default process
 *	stack, which corresponds to the default thread).
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	SP
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Address of the TCB
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
#if !_CMA_UNIPROCESSOR_
#undef cma__get_self_tcb

extern cma__t_int_tcb *
cma__get_self_tcb(void)
    {
#if _CMA_UNIPROCESSOR_
    /*
     * This function isn't called from C code on a uniprocessor
     * configuration, however it's here for use by Assembler code.
     */
    return (cma__g_current_thread);
#else
    cma_t_address	sp;
    cma___t_cluster	*cluster;
    cma___t_index	index;
    cma__t_int_tcb	*tcb;


    cma___get_stack_info (&sp, &cluster, &index);

    if (cluster == cma___c_null_cluster)
	if (((cma_t_natural)sp <= cma__c_def_stack_max) 
		&& ((cma_t_natural)sp >= cma__c_def_stack_min))
	    tcb = &cma__g_def_tcb;
	else
	    RAISE (cma_e_notcmastack);
    else {

	if (cluster->desc.type == cma___c_cluster)
	    tcb = cluster->map[index].mapped.tcb;
	else
	    tcb = ((cma___t_bigstack *)cluster)->tcb;

	}

    return tcb;
#endif					/* _CMA_UNIPROCESSOR_ */
    }
#endif /* !_CMA_UNIPROCESSOR_ */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_STACK.C */
/*  *23   15-MAY-1992 15:04:20 SCALES "Add additional queue consistency checks" */
/*  *22   13-MAR-1992 14:09:46 BUTENHOF "Rearrange stack/guard rounding" */
/*  *21    6-MAR-1992 13:34:43 BUTENHOF "Disable call_on_stack for _CMA_SHLIB_" */
/*  *20   18-FEB-1992 15:30:15 BUTENHOF "Adapt to new alloc_mem protocol" */
/*  *19   10-FEB-1992 08:50:44 BUTENHOF "Move get_self_tcb to avoid conflicts" */
/*  *18   29-JAN-1992 14:37:10 BUTENHOF "Improve special cluster management" */
/*  *17   21-JAN-1992 11:49:59 BUTENHOF "Fix special cluster bug" */
/*  *16    2-DEC-1991 13:40:34 CURTIN "Removed an erroneous dollar sign from within a comment" */
/*  *15   22-NOV-1991 11:56:49 BUTENHOF "Cast cma__fetch_sp() to cma_t_address" */
/*  *14   11-NOV-1991 11:56:05 BUTENHOF "Fix MIPS C compilation warnings" */
/*  *13    8-NOV-1991 00:38:03 BUTENHOF "Fix bug in large stack creation" */
/*  *12   14-OCT-1991 13:40:11 BUTENHOF "Unprotect guard pages on abort" */
/*  *11   24-SEP-1991 16:28:14 BUTENHOF "Fix MIPS C compilation warning" */
/*  *10   17-SEP-1991 13:23:44 BUTENHOF "Fix MIPS C compilation warning" */
/*  *9    26-JUL-1991 15:54:11 CURTIN "Added cast to a macro for mips cc" */
/*  *8    18-JUL-1991 09:45:59 CURTIN "Added large stack support" */
/*  *7    10-JUN-1991 18:23:46 SCALES "Add sccs headers for Ultrix" */
/*  *6     5-JUN-1991 16:14:46 CURTIN "fork work" */
/*  *5    24-MAY-1991 16:47:32 CURTIN "Added a new reinit routine" */
/*  *4     2-MAY-1991 13:59:08 BUTENHOF "Add trace statements" */
/*  *3    12-APR-1991 23:36:53 BUTENHOF "Change _CMA_UNIPROCESSOR_ to 0/1" */
/*  *2    14-DEC-1990 00:55:52 BUTENHOF "Change module names" */
/*  *1    12-DEC-1990 21:53:24 BUTENHOF "Thread stacks" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_STACK.C */
