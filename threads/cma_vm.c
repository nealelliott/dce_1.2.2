/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 */
/*
 * HISTORY
 * $Log: cma_vm.c,v $
 * Revision 1.1.11.1  1996/05/10  13:44:09  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:47 UTC  bissen
 * 	Merge in exec wrapper changes for CHFts15229 and purify changes.
 * 	[1995/06/05  21:55 UTC  jss  /main/HPDCE02/5]
 *
 * 	Purify the cma__alloc_mem() and cma__free_mem() routines.
 * 	[1995/05/11  17:31 UTC  jss  /main/HPDCE02/jss_mothra_fix4/1]
 *
 * 	Purify/threads changes
 * 	[1995/03/22  17:01 UTC  rps  /main/HPDCE02/4]
 *
 * 	conditionalize sbrk for purify
 * 	[1995/03/15  17:19 UTC  rps  /main/HPDCE02/rps_hc/1]
 *
 * 	Merge from branch jss_fork_mothra to HPDCE02.
 * 	[1994/12/09  16:24 UTC  jss  /main/HPDCE02/3]
 *
 * 	Fix for CHFts13768. Remove use of cma__g_global_lock from CMA memory allocation
 * 	routines.
 * 	[1994/12/07  22:23 UTC  jss  /main/HPDCE02/jss_fork_mothra/1]
 *
 * 	Merge singlestack code.
 * 	[1994/10/27  18:06 UTC  chris_p  /main/HPDCE02/2]
 *
 * 	Change symbol names with "carevue" to "singlestack"
 * 	[1994/10/17  20:30 UTC  chris_p  /main/proven_threads_940803/2]
 *
 * 	Upgrade vm mutex to a cma__g_kernal_critical for all vm routines.
 * 	[1994/10/06  21:53 UTC  chris_p  /main/proven_threads_940803/1]
 *
 * 	Created branch so singlestack code can be merged to HPDCE02
 * 	[1994/10/27  17:48 UTC  chris_p  /main/HPDCE02/1]
 *
 * Revision 1.1.6.2  1994/06/09  13:39:08  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:52  devsrc]
 * 
 * Revision 1.1.6.1  1994/04/01  20:17:45  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:25  jd]
 * 
 * Revision 1.1.4.4  1992/12/15  22:32:39  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:44:02  alan]
 * 
 * Revision 1.1.4.3  1992/09/29  20:16:31  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  21:09:20  sekhar]
 * 
 * 	Don't assume that sbrk(2) returns memory initialized to zero.
 * 	Where pointers in such memory must be zero, reset them
 * 	explicitly.
 * 	[1992/08/30  15:39:32  bolinger]
 * 
 * 	Move latest 1.0.1-based revision into 1.0.2.
 * 	[1992/08/28  20:43:27  bolinger]
 * 
 * Revision 1.1.4.2  1992/09/03  14:42:17  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:07:48  bolinger]
 * 
 * 	Fix cma__init_object() to return the pointer it's called with.
 * 	[1992/08/28  17:18:56  bolinger]
 * 
 * 	Return correct region length to caller in get_area_nolock() routine.
 * 	[1992/08/28  14:16:01  bolinger]
 * 
 * 	Merge up to BL10+ internal release from DEC.
 * 	[1992/08/11  21:20:09  bolinger]
 * 
 * 	Make change in spirit of 1.0.1 revision 1.1.2.3 (fix for OT defect
 * 	4716).
 * 	[1992/08/10  16:40:27  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/09  00:13:35  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:13:19  bolinger]
 * 
 * Revision 1.1.2.3  1992/07/15  22:47:16  bolinger
 * 	Fix OT defect 4716:  in cma__get_area_nolock(), ensure
 * 	that we always get at least as much memory as our caller
 * 	requested from sbrk(), even if sbrk()'s return address
 * 	has to be rounded up.
 * 	[1992/07/15  22:39:02  bolinger]
 * 
 * Revision 1.1.2.2  1992/02/10  14:09:16  rec
 * 	Test
 * 	[1992/02/10  14:08:39  rec]
 * 
 * Revision 1.1  1992/01/19  14:40:48  devrcs
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
 *	Manage virtual memory (isolate O/S dependencies)
 *
 *  AUTHORS:
 *
 *	Dave Butenhof, Paul Curtin
 *
 *  CREATION DATE:
 *
 *	19 July 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	30 August 1989
 *		Fix assertion macro to proper name.
 *	002	Dave Butenhof	26 September 1989
 *		Add locking.
 *	003	Dave Butenhof	12 October 1989
 *		Use internal mutex for locking (less overhead)
 *	004	Dave Butenhof	Halloween 1989
 *		Use cma_error to report malloc failure instead of assert.
 *	005	Dave Butenhof	1 December 1989
 *		Add external malloc/free routines which use interlocks.
 *	006	Dave Butenhof	7 December 1989
 *		Add cma_cfree routine.
 *	007	Dave Butenhof	10 April 1990
 *		Name the VM mutex for debugging.
 *	008	Dave Butenhof	29 May 1990
 *		Make VM mutex extern so it can be accessed by rtl linking
 *		against libcma.a.
 *	009	Dave Butenhof	26 June 1990
 *		Replace use of private "VM mutex" with the generic "global
 *		mutex".
 *	010	Paul Curtin	14 August 1990
 *		Added internal memory allocation scheme w/ look-aside lists
 *		and memory pool.
 *	011	Webb Scales	16 August 1990
 *		Replace #ifdef's with #if's
 *	012	Dave Butenhof	21 August 1990
 *		Fix errors in function declarations detected by the MIPS C
 *		compiler.
 *	013	Webb Scales	21 August 1990
 *		Reworked get_area to remove duplicated code between locking
 *		and non-locking versions.  Broke dependency of CMA memory 
 *		management on stack chunk size.
 *	014	Paul Curtin	22 August 1990
 *		Removed cma__alloc_mem_nolock code.  Added cma___mem_lock, 
 *		cma___mem_unlock, and cma___segment_pool.  Perform run-time 
 *		check to lock or not now.  Round pool to largest mem object.
 *	015	Paul Curtin	10 September 1990
 *		Fix allocation assertion check, had a 1 byte hole.
 *	016	Dave Butenhof	3 October 1990
 *		Fix deallocation of "other" sized chunks; there's no count or
 *		lock in the pool array, and it's nice to avoid memory access
 *		violations...
 *	017	Dave Butenhof	25 October 1990
 *		Add name to mutexes created at init time.
 *	018	Dave Butenhof	29 October 1990
 *		Fix several occurrences of "cma__c_large" which ought to be
 *		"cma__c_large_alloc".  Also fix cma__free_mem to break up and
 *		use the "oversized" packet being returned, rather than the
 *		remaining memory pool.
 *	019	Paul Curtin	 7 November 1990
 *		Added some variables to track internal mem alloc.
 *	020	Paul Curtin	 8 January 1991
 *		Added try block to cma__get_area to catch exc's and
 *		unlock a mutex.
 *	022	Dave Butenhof	22 January 1991
 *		Fix exception names
 *	023	Paul Curtin	20 February 1991
 *		Round up return address from sbrk to be quadword aligned
 *	024	Webb Scales	12 March 1991
 *		Added HP-specific page sizing code.
 *	025	Dave Butenhof	25 March 1991
 *		Change from cma_exception to exc_handling
 *	026	Dave Butenhof	11 April 1991
 *		CATCH exc_e_insfmem from cma__get_area_nolock and resignal
 *		after appropriate cleanup.
 *	027	Paul Curtin	18 April 1991
 *		Only round up in __get_area_nolock if succussful.
 *	028	Dave Butenhof	01 May 1991
 *		Add arguments to cma__bugcheck() calls.
 *	029	Paul Curtin	09 May 1991
 *		Added a RERAISE to a CATCH_ALL clause.
 *	030	Paul Curtin	24 May 1991
 *		Added cma__reinit_memory.
 *	031	Paul Curtin	29 May 1991
 *		get_area for init pool uses cma__c_pool_init solely.
 *	032	Paul Curtin	 5 June 1991
 *		Rearranged flags in reinit routine.
 *	033	Dave Butenhof	19 September 1991
 *		Integrate HPUX CMA5 reverse drop: change HP page size.
 *	034	Dave Butenhof	08 October 1991
 *		With advent of dynamically allocated file descriptor related
 *		structures, we may need to allocated more than the "pool
 *		size" for the structures (for example, just any array of 4096
 *		pointers is 16,384 bytes where the current pool size limit is
 *		10,240). Add code to allocate directly instead of going to
 *		pool. Also, remove a lot of excess locking in cma__alloc_mem.
 *	035	Dave Butenhof	30 October 1991
 *		Handle exceptions on allocation calls.
 *	036	Paul Curtin	02 December 1991
 *		Added a call to getsyiw on Alpha to determine page size.
 *	037	Paul Curtin	20 December 1991
 *		Remove starlet.h include on VAX
 *	038	Dave Butenhof	10 February 1992
 *		Add cma__alloc_zero to allocate pre-cleared memory. Otherwise
 *		identical to cma__alloc_mem. Also, change allocation
 *		functions to return NULL rather than raising cma_e_insfmem;
 *		this will remove a lot of TRY overhead within DECthreads.
 *		It's too bad we have to compromise our clean exception based
 *		design: but the fact is that performance is more important
 *		within the library.
 *	039	Dave Butenhof	12 February 1992
 *		Rework VM to merge freed general pool packets, and provide
 *		logic to reclaim unused lookaside list packets when memory
 *		gets tight. Remove some entry points (like cma__get_area) and
 *		merge others. Generally clean up some. Also, provide more
 *		pool statistics in !NDEBUG builds.
 *	040	Dave Butenhof	18 February 1992
 *		Fix a typo (missing ';' in UNIX code).
 *	041	Webb Scales	18 February 1992
 *		Fixed a compilation error in memory packet counting.
 *	042	Dave Butenhof	19 February 1992
 *		Yet another bug... UNIX cma___get_area detects unaligned sbrk
 *		result incorrectly.
 *	043	Webb Scales	11 May 1992
 *		"Clean" queue pointers in freed memory block before enqueuing.
 *		Added routine for debugging use which clean's queue pointers
 *		in a object's header.
 */

/*
 *  INCLUDE FILES
 */
#include <cma.h>
#include <cma_defs.h>
#include <cma_mutex.h>
#include <cma_vm.h>
#include <cma_errors.h>
#include <cma_queue.h>
#include <cma_assert.h>
#if _CMA_OS_ == _CMA__VMS
#include <stdlib.h>			/* Define UNIX VM prototypes */
# if _CMA_HARDWARE_ == _CMA__ALPHA
#  include <starlet.h>
# endif
#else
#include <sys/types.h>
#endif
#if defined(SNI_SVR4_POSIX)
#include <sys/unistd.h>
#endif


/*
 *  LOCAL MACROS
 */

/*
 * Round byte-count up to the next integral page
 */
#define cma___roundup_page_size(sz)	cma__roundup(sz, cma__g_page_size)

/*
 * Byte boundary to which to round up address of newly-allocated
 * Memory.
 */
#define	cma___c_addr_roundup	8

/*
 * Perform run time checks on cma___g_mem_init_done inorder to decide
 * whether or not to perform locking on the given mutex.  The flag
 * should be set during initialization.
 */

#ifdef HP_CMA_SINGLESTACK_OPT
#define cma___mem_lock(mutex) 			\
    if (cma___g_mem_init_done)  		\
		if (cma_singlestack_opt)			\
			cma__g_kernel_critical++;	\
		else							\
			cma__int_lock(mutex)

#define cma___mem_unlock(mutex) 		\
    if (cma___g_mem_init_done)  		\
		if (cma_singlestack_opt)			\
			cma__g_kernel_critical--;	\
		else							\
			cma__int_unlock(mutex)
#else
#define cma___mem_lock(mutex) \
    if (cma___g_mem_init_done)  cma__int_lock(mutex)

#define cma___mem_unlock(mutex) \
    if (cma___g_mem_init_done)  cma__int_unlock(mutex)
#endif

#if _CMA_PLATFORM_ == _CMA__ALPHA_VMS
# ifndef SYI$_PAGE_SIZE
#  define SYI$_PAGE_SIZE 4446
# endif
#endif

/*
 *  GLOBAL DATA
 */

cma__t_alloc_queue	cma__g_memory[cma__c_alloc_sizes];
cma_t_integer		cma__g_page_size;
#if _CMA_OS_ == _CMA__UNIX
 cma_t_integer		cma__g_sbrk_align = 0;
#endif
cma__t_pool_stat	cma__g_pool_stat = {0};

#ifdef HP_CMA_SINGLESTACK_OPT
extern int cma_singlestack_opt;
#endif
/*
 *  LOCAL DATA
 */

static cma_t_boolean	cma___g_mem_init_done = cma_c_false; 

/*
 * LOCAL FUNCTIONS
 */

static cma_t_address cma___alloc_from_pool  (cma_t_natural);

static void cma___free_to_pool  (cma_t_address);

static cma_t_address cma___get_area  (cma_t_natural,cma_t_natural *);

static cma_t_boolean cma___swipe_some_vm  (void);

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__alloc_mem - takes an allocation request and returns memory
 *	the size of the next closest predefined unit.  Lookup lists are
 *	checked for equivalent size units, if free memory is not found
 *	the current pool is checked to see if it large enough, otherwise 
 *	a new pool is created to handle the request.
 *
 *  FORMAL PARAMETERS:
 * 
 *	size_in		The requested size (in bytes) to be allocated
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
 *	Address of the memory allocated (or NULL)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern cma_t_address
cma__alloc_mem(
	cma_t_natural	 size)		/* Requested allocation, bytes */
    {
    cma_t_integer	allocation;	/* Actual area allocated	*/
    cma_t_integer	alloc_type;	/* Type of allocation (s,m,l..) */
    cma_t_address	ret_addr;	/* Address of allocated memory  */
    cma_t_address	temp_ptr;	/* Used in managing pool	*/


#ifdef _HP_CMA_
    if (purify_is_running())
	return ((cma_t_address)malloc(size));
#endif

    /* 
     *	Round up for CMA memory management (classification) purposes.
     */
    if (size <= cma__c_small_alloc) {
	allocation = cma__roundup (size, cma__c_small_alloc);
	alloc_type = cma__c_small;
	}
    else if (size <= cma__c_med_alloc) {
	allocation = cma__roundup (size, cma__c_med_alloc);
	alloc_type = cma__c_med;
	}
    else if (size <= cma__c_large_alloc) {
	allocation = cma__roundup (size, cma__c_large_alloc);
	alloc_type = cma__c_large;
	}
    else {				/* request larger than lookasides */
	/*
	 * Round up to next multiple of alignment size
	 */
	allocation = cma__roundup (size, cma__c_mem_tag);
	alloc_type = cma__c_pool;
	}
    
    /*
     * Check the appropriate lookup list to see if there is available memory
     * already there. If something is available we will get back a pointer
     * to useable memory. If the requested size is larger than the "large"
     * lookaside packets, it will come out of general pool instead.
     */

    if (alloc_type != cma__c_pool) {
	cma___mem_lock (cma__g_memory[alloc_type].mutex);

	if (!cma__queue_empty (&cma__g_memory[alloc_type].queue)) {
	    ret_addr = (cma_t_address)cma__queue_dequeue (
		    &cma__g_memory[alloc_type].queue);
	    cma__g_memory[alloc_type].count--;
	    cma___mem_unlock (cma__g_memory[alloc_type].mutex);
	    return ret_addr;
	    }
	else
	    /*
	     * Otherwise, we need to create a new one: count it now while we
	     * have the correct structure locked.
	     */
	    cma__g_memory[alloc_type].total++;

	cma___mem_unlock (cma__g_memory[alloc_type].mutex);
	}

    /*
     * If we get here, we can't pull the packet off a lookaside list: either
     * the correct list is empty, or we want something too big.
     */

    temp_ptr = cma___alloc_from_pool (allocation);

    while (temp_ptr == cma_c_null_ptr) {

	if (!cma___swipe_some_vm ())
	    return cma_c_null_ptr;

	temp_ptr = cma___alloc_from_pool (allocation);
	}

    return temp_ptr;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__alloc_zero - Allocate memory and set it to zero.
 *
 *  FORMAL PARAMETERS:
 * 
 *	size_in		The requested size (in bytes) to be allocated
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
 *	Address of the memory allocated (or NULL)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern cma_t_address
cma__alloc_zero(
	cma_t_natural	 size)		/* Requested allocation, bytes */
    {
    cma_t_address	allocation;
    long int		*memlong;
    char		*memchar;
    int			longs, bytes;

    allocation = cma__alloc_mem (size);

    if (allocation == cma_c_null_ptr)
	return allocation;

#ifndef NDEBUG
    /*
     * We need to interlock the statistics stuff, even though this routine
     * doesn't really need to take out any locks. So, for convenience, use
     * the general pool lock.
     */
    cma___mem_lock (cma__g_memory[cma__c_pool].mutex);
    cma__g_pool_stat.zero_allocs++;
    cma__g_pool_stat.zero_bytes += size;
    cma___mem_unlock (cma__g_memory[cma__c_pool].mutex);
#endif
    memlong = (long int *)allocation;

    for (longs = size / sizeof (long int); longs > 0; longs--) {
	*memlong = 0;
	memlong++;
	}

    memchar = (char *)memlong;

    for (bytes = size % sizeof (long int); bytes > 0; bytes--) {
	*memchar = 0;
	memchar++;
	}

    return allocation;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__free_mem - takes in the address of a memory area
 *	allocated useing cma__alloc_mem and places it on the 
 *	appropriate lookup list.
 *
 *
 *  FORMAL PARAMETERS:
 * 
 *	mem area	address of area to be freed.
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
cma__free_mem(
	cma_t_address	mem_area)	/* Address of memory to be freed */
    {
    cma_t_integer	allocation;	/* Size of area - for lookup list */
    cma_t_integer	alloc_type;	/* Type of area - for lookup list */

#ifdef _HP_CMA_
    if (purify_is_running()) {
	free((char *)mem_area);
	return;
    }
#endif

    allocation = *((cma_t_integer *)
	(((cma_t_integer)mem_area) - cma__c_mem_tag));

    /*
     * Get the allocation type.
     */
    if (allocation < cma__c_small_alloc)
	alloc_type = cma__c_pool;
    else if (allocation < cma__c_med_alloc)
	alloc_type = cma__c_small;
    else if (allocation < cma__c_large_alloc)
	alloc_type = cma__c_med;
    else if (allocation == cma__c_large_alloc)
	alloc_type = cma__c_large;
    else
	alloc_type = cma__c_pool;
	
    cma___mem_lock (cma__g_memory[alloc_type].mutex);

    cma__queue_zero((cma__t_queue *)mem_area);

    if (alloc_type == cma__c_pool) {
	cma___free_to_pool (mem_area);
	}
    else {
	cma__queue_insert_after (
		((cma__t_queue *)mem_area),
		&cma__g_memory[alloc_type].queue);
	cma__g_memory[alloc_type].count++;
	}

    cma___mem_unlock (cma__g_memory[alloc_type].mutex);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Performs initialization for internal CMA memory management
 *	locks.
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
cma__init_mem_locks(void)
    {


    cma__g_memory[cma__c_small].mutex = 
	cma__get_first_mutex (&cma__g_def_attr);
    cma__obj_set_name (cma__g_memory[cma__c_small].mutex, "VM, small");
    cma__g_memory[cma__c_med].mutex = 
	cma__get_first_mutex (&cma__g_def_attr);
    cma__obj_set_name (cma__g_memory[cma__c_med].mutex, "VM, medium");
    cma__g_memory[cma__c_large].mutex = 
	cma__get_first_mutex (&cma__g_def_attr);
    cma__obj_set_name (cma__g_memory[cma__c_large].mutex, "VM, large");
    cma__g_memory[cma__c_pool].mutex = 
	cma__get_first_mutex (&cma__g_def_attr);
    cma__obj_set_name (cma__g_memory[cma__c_pool].mutex, "VM, pool");
    cma___g_mem_init_done = cma_c_true;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Performs initialization work for internal CMA memory management 
 *	constants and queues.
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
cma__init_memory(void)
    {

#if _CMA_PLATFORM_ == _CMA__ALPHA_VMS
    struct {
        unsigned short  buffer_length;
        unsigned short  item_code;
        char            *buffer_address;
        short           *return_length_address;
        int             end;
        } itmlst = {
	    (sizeof (cma__g_page_size)),
	    SYI$_PAGE_SIZE, 
	    ((char *)&cma__g_page_size), 
	    0, 
	    0};

    sys$getsyiw ( 0, 0, 0, &itmlst, 0, 0, 0 );
#endif

#if _CMA_PLATFORM_ == _CMA__VAX_VMS
    cma__g_page_size = cma__c_page_size;
#endif    

#if _CMA_OS_ == _CMA__UNIX
# if _CMA_VENDOR_ == _CMA__HP
    cma__g_page_size = NBPG;
# else
#  if defined(SNI_SVR4_POSIX)
	if ( (cma__g_page_size = sysconf( _SC_PAGESIZE )) == -1 )
		{
		cma__bugcheck("sysconf error:1");
		}
#   else /*  defined(SNI_SVR4_POSIX) */
    cma__g_page_size = getpagesize(); 
#   endif	/* defined(SNI_SVR4_POSIX) */
# endif 
#endif 

    cma__g_memory[cma__c_small].size = cma__c_small_alloc;
    cma__g_memory[cma__c_small].count = 0;
    cma__g_memory[cma__c_small].total = 0;
    cma__queue_init (&cma__g_memory[cma__c_small].queue);
    
    cma__g_memory[cma__c_med].size = cma__c_med_alloc;
    cma__g_memory[cma__c_med].count = 0;
    cma__g_memory[cma__c_med].total = 0;
    cma__queue_init (&cma__g_memory[cma__c_med].queue);

    cma__g_memory[cma__c_large].size = cma__c_large_alloc;
    cma__g_memory[cma__c_large].count = 0;
    cma__g_memory[cma__c_large].total = 0;
    cma__queue_init (&cma__g_memory[cma__c_large].queue);

    cma__g_memory[cma__c_pool].size = 0;
    cma__g_memory[cma__c_pool].count = 0;
    cma__g_memory[cma__c_pool].total = 0;
    cma__queue_init (&cma__g_memory[cma__c_pool].queue);
    }

#ifndef NDEBUG
/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Initializes an object header.
 *
 *  FORMAL PARAMETERS:
 *
 *	obj_ptr
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
 *	Returns the address passed to it.
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern cma_t_address
cma__init_object(
	cma_t_address	obj_ptr)
    {
    if (obj_ptr != cma_c_null_ptr) {
	cma__queue_zero (&(((cma__t_object *)obj_ptr)->queue));
	}
	return (obj_ptr);
    }
#endif

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Performs pre- or post- `fork() reinitialization' work.
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
cma__reinit_memory(
	cma_t_integer	flag)
    {

    if (flag == cma__c_reinit_prefork_lock) {
	cma__int_lock(cma__g_memory[cma__c_small].mutex);
	cma__int_lock(cma__g_memory[cma__c_med].mutex);
	cma__int_lock(cma__g_memory[cma__c_large].mutex);
	cma__int_lock(cma__g_memory[cma__c_pool].mutex);
	}
    else if (flag == cma__c_reinit_postfork_unlock) {
	cma__int_unlock(cma__g_memory[cma__c_pool].mutex);
	cma__int_unlock(cma__g_memory[cma__c_large].mutex);
	cma__int_unlock(cma__g_memory[cma__c_med].mutex);
	cma__int_unlock(cma__g_memory[cma__c_small].mutex);
	}

    }

/*
 *
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma___alloc_from_pool -
 *		Try to find a large enough chunk in the pool to satisfy the
 *		request. If there are none, allocate some fresh VM.
 *
 *  FORMAL PARAMETERS:
 *
 *	size	requested length.
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
 *	Returns pointer to newly allocated area, upon successful
 *	completion, or NULL if unsuccessful.
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
static cma_t_address
cma___alloc_from_pool(
	cma_t_natural	 size)
    {
    cma__t_queue	*qp, *bp, *np;	/* Queue pointers */
    cma_t_natural	*ip, *nip;	/* Integer pointer */
    cma_t_natural	asz, rsz, bsz = (cma_t_natural)-1;


    cma___mem_lock (cma__g_memory[cma__c_pool].mutex);
    qp = cma__queue_next (&cma__g_memory[cma__c_pool].queue);
    bp = (cma__t_queue *)cma_c_null_ptr;

    while (qp != &cma__g_memory[cma__c_pool].queue) {
	ip = (cma_t_natural *)qp;

	if (ip[-2] >= size) {		/* If this one's big enough */

	    if (ip[-2] < bsz) {
		bsz = ip[-2];
		bp = qp;
		}

	    }

	qp = cma__queue_next (qp);
	}

    if ((cma_t_address)bp != cma_c_null_ptr) {
	np = cma__queue_next (bp);
	qp = cma__queue_remove (bp);

	/*
	 * If what we found is at least enough larger than what we need that
	 * we can split off another small-sized package from it, do so;
	 * otherwise just leave it in one piece.
	 */
	if (bsz >= (size + cma__c_small_alloc + cma__c_mem_tag)) {
	    ip = (cma_t_natural *)qp;
	    nip = (cma_t_natural *)((cma_t_natural)qp + size + cma__c_mem_tag);
	    nip[-2] = bsz - cma__c_mem_tag - size;
	    ip[-2] = size;
	    cma__queue_zero((cma__t_queue *)nip);
	    cma__queue_insert ((cma__t_queue *)nip, np);
#ifndef NDEBUG
	    cma__g_pool_stat.breakups++;
#endif
	    }
	else
	    cma__g_memory[cma__c_pool].count--;

	}
    else {
	/*
	 * There aren't any pieces in pool that fit. So allocate some fresh
	 * VM large enough to hold it. If it's bigger than the "standard pool
	 * size" just allocate enough to fit -- otherwise allocate the
	 * standard pool size and leave the remainder on the pool queue for
	 * later use.
	 */
	if (size > cma__c_pool_size)
	    asz = size + cma__c_mem_tag;
	else
	    asz = cma__c_pool_size;

	nip = (cma_t_natural *)cma___get_area (asz, &rsz);
#ifndef NDEBUG
	cma__g_pool_stat.allocs++;
	cma__g_pool_stat.bytes_allocd += rsz;
#endif

	if ((cma_t_address)nip == cma_c_null_ptr) {
	    /*
	     * Uh oh! We're out of memory. Let higher level code scrounge
	     */
#ifndef NDEBUG
	    cma__g_pool_stat.failures++;
#endif
	    cma___mem_unlock (cma__g_memory[cma__c_pool].mutex);
	    return cma_c_null_ptr;
	    }

	if (rsz == size + cma__c_mem_tag) {
	    nip[0] = rsz - cma__c_mem_tag;
	    qp = (cma__t_queue *)&nip[2];
		cma__queue_zero(qp);
#ifndef NDEBUG
	    cma__g_pool_stat.exact_allocs++;
#endif
	    }
	else {
	    nip[0] = rsz - size - (cma__c_mem_tag * 2);
	    np = (cma__t_queue *)&nip[2];
	    ip = (cma_t_natural *)((cma_t_natural)np + nip[0]);
	    qp = (cma__t_queue *)&ip[2];
		cma__queue_zero(qp);
		cma__queue_zero(np);
	    ip[0] = size;
	    cma___free_to_pool ((cma_t_address)np);
	    }

	}

#ifndef NDEBUG
    cma__g_pool_stat.extractions++;
#endif
    cma___mem_unlock (cma__g_memory[cma__c_pool].mutex);
    return (cma_t_address)qp;
    }

/*
 *
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma___free_to_pool -
 *		Fit a packet into the pool list: merge with adjacent packets if
 *		possible.
 *
 *  FORMAL PARAMETERS:
 * 
 *	pack	Address of packet.
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
 *	none
 */
static void
cma___free_to_pool(cma_t_address	pack)
    {
    cma_t_natural	*ip, *bi, *ni, size;
    cma__t_queue	*qp, *qb, *qn, *qh, *qt;
    cma_t_boolean	found = cma_c_false, insert = cma_c_true;
    cma_t_boolean	merged = cma_c_false;


#ifndef NDEBUG
    cma__g_pool_stat.returns++;
#endif
    ip = (cma_t_natural *)((cma_t_natural)pack - cma__c_mem_tag);
    qp = (cma__t_queue *)pack;
    size = ip[0];
    qh = &cma__g_memory[cma__c_pool].queue;

    qb = cma__queue_previous (qh);	/* Point to last entry */

    while (qb != qh) {

	if (qp > qb) {
	    found = cma_c_true;
	    break;
	    }

	qb = cma__queue_previous (qb);
	}

    /*
     * As long as we found a packet with a lower address, check whether we
     * can just tack this onto the end of it.
     */
    if (found) {
	bi = (cma_t_natural *)((cma_t_natural)qb - cma__c_mem_tag);

	if ((cma_t_natural *)((cma_t_natural)qb + bi[0]) == ip) {
	    bi[0] = bi[0] + cma__c_mem_tag + size;
	    merged = cma_c_true;
#ifndef NDEBUG
	    cma__g_pool_stat.merge_befores++;
#endif

	    /*
	     * Use the new combined packet to see if we can merge with the
	     * next packet.
	     */
	    ip = bi;
	    qp = qb;
	    size = ip[0];
	    insert = cma_c_false;
	    }

	}

    qn = cma__queue_next (qb);

    if (qn != qh) {
	/*
	 * If there's a following packet, see if we can merge with it
	 */
	ni = (cma_t_natural *)((cma_t_natural)qn - cma__c_mem_tag);

	if ((cma_t_natural *)((cma_t_natural)qp + size) == ni) {
	    ip[0] = ip[0] + ni[0] + cma__c_mem_tag;
	    qt = cma__queue_next (qn);
	    cma__queue_remove (qn);

	    /*
	     * If we already merged with the previous packet, then this merge
	     * decreases the packet count by one.
	     */
	    if (merged)
		cma__g_memory[cma__c_pool].count--;

#ifndef NDEBUG
	    cma__g_pool_stat.merge_afters++;
#endif
	    merged = cma_c_true;
	    qn = qt;
	    }

	}

    /*
     * If the packet needs to be inserted, do so (before qn -- which is
     * either the queue header or the packet with the next highest address).
     */
    if (insert) {
	cma__queue_insert (qp, qn);

	if (!merged)
	    cma__g_memory[cma__c_pool].count++;

	return;
	}

    }

/*
 *
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma___get_area -
 *	    Unsynchronized jacket for basic system-specific memory allocation 
 *	    service.  This is intended to be used internally for allocation 
 *	    of large areas of memory to be managed by CMA.
 *
 *  FORMAL PARAMETERS:
 *	Inputs:
 *	    size_in - requested length.
 *	    size_out - (by reference) actual acquired length.
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
 *	Returns pointer to newly allocated area, upon successful
 *	completion, or NULL if unsuccessful.
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
static cma_t_address
cma___get_area	(
	cma_t_natural	 size_in,
	cma_t_natural	*size_out)
    {
#if _CMA_OS_ == _CMA__VMS
    cma_t_natural	pagcnt;
    cma_t_natural	retadr[2];
    cma_t_natural	status;


    pagcnt = (size_in + cma__c_page_size - 1) / cma__c_page_size;

    if (cma__g_global_lock) cma__int_lock (cma__g_global_lock);
    status = sys$expreg (pagcnt, retadr, 0, 0);
    if (cma__g_global_lock) cma__int_unlock (cma__g_global_lock);

    if (status != SS$_NORMAL) {
	*size_out = 0;
	return cma_c_null_ptr;
	}

    *size_out = retadr[1] - retadr[0] + 1;
    return ((cma_t_address)retadr[0]);
#endif
#if _CMA_OS_ == _CMA__UNIX
    cma_t_integer	incr;
    cma_t_integer	ret_addr;


    incr = cma___roundup_page_size ((cma_t_integer)size_in);

    /*
     * We need to allocate a chunk of memory at least as large as the caller
     * requested. We also need to make sure that it's aligned on at least an
     * 8-byte boundary (that's how we've defined cma__alloc_mem). Since
     * someone else could do sbrk() calls for random sizes, the address we
     * get may need to be adjusted... So, if we didn't just introduce
	 * enough slack into the allocation to take care of this possibility,
	 * do so now.
     */
	if (incr - size_in < cma___c_addr_roundup)
		incr += cma___c_addr_roundup;
#ifndef _HP_LIBC_R
    if (cma__g_global_lock)
		cma__int_lock (cma__g_global_lock);
#endif
#ifdef _HP_CMA_
    if (purify_is_running())
	ret_addr = (cma_t_integer)(malloc (incr));
    else
#endif
	ret_addr = (cma_t_integer)(sbrk (incr));

    if (ret_addr == -1) {
	*size_out = 0;
#ifndef _HP_LIBC_R
	if (cma__g_global_lock)
		cma__int_unlock (cma__g_global_lock);
#endif
	return cma_c_null_ptr;
	}

	ret_addr = cma__roundup(ret_addr, cma___c_addr_roundup);
#ifndef _HP_LIBC_R
    if (cma__g_global_lock)
		cma__int_unlock (cma__g_global_lock);
#endif
	/*
	 * Don't try to account for every byte we got from sbrk() --
	 * can't give it back anyhow.
	 */
    *size_out = (cma_t_natural)size_in;
    return ((cma_t_address)ret_addr);
#endif
    }

/*
 *
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma___swipe_some_vm -
 *		Take one element from each lookaside list and free it to the
 *		general pool.
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
 *	cma_c_true if any memory was moved; cma_c_false if lookasides were
 *	already empty.
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
static cma_t_boolean
cma___swipe_some_vm(void)
    {
    cma__t_queue	*tmp[cma__c_large + 1];
    cma_t_boolean	found = cma_c_false;
    cma_t_integer	i;


    for (i = 0; i <= cma__c_large; i++) {
	cma___mem_lock (cma__g_memory[i].mutex);

	if (!cma__queue_empty (&cma__g_memory[i].queue)) {
	    found = cma_c_true;
	    tmp[i] = cma__queue_dequeue (&cma__g_memory[i].queue);
#ifndef NDEBUG
	    cma__g_pool_stat.scrounges[i]++;
#endif
	    cma__g_memory[i].count--;
	    cma__g_memory[i].total--;
	    }
	else
	    tmp[i] = (cma__t_queue *)cma_c_null_ptr;

	cma___mem_unlock (cma__g_memory[i].mutex);
	}

    if (!found)
	return cma_c_false;

    cma___mem_lock (cma__g_memory[cma__c_pool].mutex);

    for (i = 0; i <= cma__c_large; i++) {

	if ((cma_t_address)tmp[i] != cma_c_null_ptr)
	    cma___free_to_pool ((cma_t_address)tmp[i]);

	}

    cma___mem_unlock (cma__g_memory[cma__c_pool].mutex);
    return cma_c_true;
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_VM.C */
/*   29   26-JUN-1992 10:53:13 CURTIN "Fix a hard coded constant for EVMS" */
/*  *28   15-MAY-1992 15:05:06 SCALES "Add queue consistency checks" */
/*  *27   19-FEB-1992 07:16:33 BUTENHOF "Fix bug in UNIX code (sigh)" */
/*  *26   19-FEB-1992 04:34:57 BUTENHOF "Fix typos" */
/*  *25   18-FEB-1992 16:30:47 BUTENHOF "Fix stuff" */
/*  *24   18-FEB-1992 16:01:50 BUTENHOF "Fix a typo" */
/*  *23   18-FEB-1992 15:31:35 BUTENHOF "Add cma__alloc_zero (pre-cleared memory)" */
/*  *22   20-DEC-1991 11:31:34 CURTIN "removed starlet.h on VAX/VMS" */
/*  *21    2-DEC-1991 16:55:23 CURTIN "Added a call to getsyi on Alpha to get system page size" */
/*  *20   31-OCT-1991 12:40:17 BUTENHOF "Fix handling of exceptions" */
/*  *19   14-OCT-1991 14:44:36 BUTENHOF "Fix best_fit routine" */
/*  *18   14-OCT-1991 13:42:32 BUTENHOF "Add support for large allocations" */
/*  *17   24-SEP-1991 16:30:06 BUTENHOF "Merge CMA5 reverse IBM/HP/Apollo drops" */
/*  *16   10-JUN-1991 18:25:13 SCALES "Add sccs headers for Ultrix" */
/*  *15    5-JUN-1991 16:16:22 CURTIN "fork work" */
/*  *14   29-MAY-1991 13:18:21 CURTIN "Adjust init pool size allocation" */
/*  *13   24-MAY-1991 16:52:09 CURTIN "Added a new reinit routine" */
/*  *12    9-MAY-1991 14:55:45 CURTIN "Added a RERAISE to a CATCH_ALL clause" */
/*  *11    2-MAY-1991 14:00:01 BUTENHOF "Add argument to cma__bugcheck() calls" */
/*  *10   18-APR-1991 16:42:46 CURTIN "changed position of roundup in __get_area_nolock" */
/*  *9    12-APR-1991 23:37:35 BUTENHOF "Fix bug in cma__get_area()" */
/*  *8     1-APR-1991 18:10:00 BUTENHOF "QAR 93, exception text" */
/*  *7    14-MAR-1991 13:46:08 SCALES "Convert to stream format for ULTRIX build" */
/*  *6    12-MAR-1991 19:40:51 SCALES "Merge in HP change to CD4" */
/*  *5    20-FEB-1991 14:26:43 CURTIN "Now round to quadword, that returned by sbrk" */
/*  *4    24-JAN-1991 00:35:25 BUTENHOF "Fix exception name references" */
/*  *3     8-JAN-1991 14:51:42 CURTIN "fixed internal cma qar 063" */
/*  *2    17-DEC-1990 09:32:24 CURTIN "Added statistics taking for memory" */
/*  *1    12-DEC-1990 21:55:56 BUTENHOF "VM management" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_VM.C */
