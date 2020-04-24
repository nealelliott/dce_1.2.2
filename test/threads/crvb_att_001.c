/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 * All Rights Reserved
 * (c) Copyright 1990, 1991, 1992
 */
/*
 * HISTORY
 * $Log: crvb_att_001.c,v $
 * Revision 1.1.17.1  1996/06/04  22:13:46  arvind
 * 	Add _HP_CMA fo param.h
 * 	[1996/03/08  22:19 UTC  bissen  /main/DCE_1.2/1]
 *
 * Revision 1.1.15.2  1996/02/17  23:30:02  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:49  marty]
 * 
 * Revision 1.1.15.1  1995/12/11  22:58:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:08  root]
 * 
 * Revision 1.1.12.1  1993/10/12  19:34:55  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:49  rhw]
 * 
 * Revision 1.1.10.2  1993/06/14  20:14:20  sommerfeld
 * 	Define default_stack and default_guard
 * 	for HPPA hardware.
 * 	Define a roundup_chunksize macro so that
 * 	we can do meaningful comparisons on stack/
 * 	guard sizes.
 * 	for PA-RISC machines.
 * 	[1993/05/06  20:06:46  hopkins]
 * 
 * Revision 1.1.5.4  1992/12/30  16:29:43  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:06:27  zeliff]
 * 
 * Revision 1.1.5.3  1992/09/29  20:37:42  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:31:55  sekhar]
 * 
 * Revision 1.1.5.2  1992/09/03  14:31:16  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:44:16  bolinger]
 * 
 * Revision 1.1.3.2  1992/08/12  21:00:23  bolinger
 * 	Liberalize checks for default, modified, and reclaimed stack
 * 	plus guard sizes to account for possible round-up of app-specified
 * 	values.
 * 
 * Revision 1.1.2.2  1992/05/21  14:16:22  bolinger
 * 	Provide OT enhancement 3478:  Change verification of stack size
 * 	increase to make it more portable.
 * 	[1992/05/20  19:58:49  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:22:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1989, 1991 by
 *  Digital Equipment Corporation, Maynard Massachusetts.
 *  All rights reserved.
 *
 *  This software is furnished under a license and may be used and copied
 *  only in  accordance  with  the  terms  of  such  license and with the
 *  inclusion of the above copyright notice.  This software or  any other
 *  copies thereof may not be provided or otherwise made available to any
 *  other person.  No title to and ownership of  the  software  is hereby
 *  transferred.
 *
 *  The information in this software is subject to change  without notice
 *  and should  not  be  construed  as  a commitment by DIGITAL Equipment
 *  Corporation.
 *
 *  DIGITAL assumes no responsibility for the use or  reliability  of its
 *  software on equipment which is not supplied by DIGITAL.
 */
 
/*
 *  FACILITY:
 *
 *	CMA Test Programs
 *
 *  FILENAME:
 *
 *	RV_ATT_001.C
 *
 *  TEST TOPICS:
 *
 *	ATT  -  Attr Objects
 *
 *  ABSTRACT:
 *
 *	Verify that attr objects can be created and deleted,
 *	that default attribute values are set correctly, and that
 *	non-default values can be set and read correctly.
 *	
 *	This test does not verify that attr are properly
 *	applied to other objects.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	16 August 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	28 February 1990
 *		Modify to test new priority/policy values.
 *	002	Webb Scales	28 June 1990
 *		Modify to enable new priority/policy values.
 *	003	Paul Curtin	6 July 1990
 *		Changed default stack and default guard sizes.
 *	004	Dave Butenhof	4 September 1990
 *		Change to new CMA interfaces.
 *	005	Dave Butenhof	10 September 1990
 *		Fix some typos in UNIX-only conditionals.
 *	006	Paul Curtin	13 September 1990
 *		Changed default_guard to be 8 (512 byte) pages.
 *	007	Dave Butenhof	01 April 1991
 *		Fix use of exception package
 */

 
/*
 *  INCLUDE FILES
 */
 
#include <dce/cma.h>
#include <dce/cma_defs.h>
#include <dce/cts_test.h>

#if (_CMA_VENDOR_ == _CMA__IBM) || (_CMA_VENDOR_ == _CMA__HP)
# include <sys/param.h>
#endif
#if _CMA_UNIX_TYPE == _CMA__SVR4
#include <sys/unistd.h>
#endif

# if (_CMA_VENDOR_ == _CMA__IBM) || (_CMA_VENDOR_ == _CMA__HP)
#  define page_size NBPG    /* Page size in bytes */
# elif _CMA_UNIX_TYPE == _CMA__SVR4
#  if defined(_CMA_BSD_CMP_)
#   define page_size getpagesize()
#  else
#   define page_size sysconf( _SC_PAGESIZE )
#  endif
# else
#  define page_size	512	/* Page size in bytes */
# endif

# if (_CMA_VENDOR_ == _CMA__IBM) || (_CMA_UNIX_TYPE == _CMA__SVR4)
#  define default_stack	page_size * 8	/* Default stack */
#  define default_guard	page_size * 2	/* Default guard */
# elif (_CMA_VENDOR_ == _CMA__HP) && (_CMA_HARDWARE_ == _CMA__HPPA)
#  if _CMA_HARDWARE_ == _CMA__HPPA
#   define default_stack	page_size * 15	/* Default stack */
#   define default_guard	page_size * 4	/* Default guard */
#  else
#   define default_stack page_size * 8  /* Default stack */
#   define default_guard page_size * 2  /* Default guard */
#  endif
# else
#  define default_stack	page_size * 60	/* Default stack */
#  define default_guard	page_size * 8	/* Default guard */
# endif

/*
 * The following requires that cma_defs.h
 * be included, but allows us to do an _exact_
 * check for stack/guard sizes ... macro must
 * track changes to stack allocation roundup
 * algorithms in cma_stack.[ch] and cma_stack_int.h.
 */
extern	cma_t_integer		cma__g_chunk_size;
#define	roundup_chunksize(x)	(cma__roundup((x),cma__g_chunk_size))

main ()
    {
    cma_t_attr		att1;		/* test attr object */
    cma_t_sched_inherit	inherit_sched;	/* value of inherit pri. att. */
    cma_t_priority	priority;	/* value of thread priority att */
    cma_t_sched_policy	policy;		/* value of thread sched. policy att */
    cma_t_natural	stacksize;	/* value of stack size att. */
    cma_t_natural	guardsize;	/* value of guard size att. */

    cts_test("CRVB_ATT_001", "Test create/delete/modify/get of attr objects");
 
    cma_init ();

    cma_attr_create (&att1, &cma_c_null);

    /*
     * Now that we have an attr object, verify that all default
     * attribute values are set currectly.  Note that some comparisons (stack
     * and guard sizes, which alren't architecturally specified) depend on
     * constants from the internal definitions file (cma_defs.h).
     */
    cma_attr_get_inherit_sched (&att1, &inherit_sched);

    if (inherit_sched != cma_c_sched_inherit)
	cts_failed ("Default attr. obj. has bad \"inherit sched\" attribute.");

    cma_attr_get_priority (&att1, &priority);

    if (priority != cma_c_prio_through_mid)
	cts_failed ("Default attr. obj. has bad \"priority\" attribute.");

    cma_attr_get_sched (&att1, &policy);

    if (policy != cma_c_sched_throughput)
	cts_failed ("Default attr. obj. has bad \"policy\" attribute.");

    cma_attr_get_stacksize (&att1, &stacksize);

	/*
	 * Allow for round-up to "chunk" boundary.
	 */
    if (stacksize != roundup_chunksize(default_stack))
	cts_failed ("Default attr. obj. has bad \"stacksize\" attribute (has %d, should have %d).", stacksize, roundup_chunksize(default_stack));

    cma_attr_get_guardsize (&att1, &guardsize);

	/*
	 * Allow for round-up to "chunk" boundary.
	 */
    if (guardsize != roundup_chunksize(default_guard))
	cts_failed ("Default attr. obj. has bad \"guardsize\" attribute (has %d, shjould have %d).", guardsize, roundup_chunksize(default_guard));

    /*
     * Now modify all of the attr, and read them back to verify that we
     * get the changed values.
     */
    cma_attr_set_inherit_sched (&att1, cma_c_sched_use_default);
    cma_attr_get_inherit_sched (&att1, &inherit_sched);
    if (inherit_sched != cma_c_sched_use_default)
	cts_failed ("Failed to change \"inherit sched\" attribute.");

    cma_attr_set_priority (&att1, cma_c_prio_through_max);
    cma_attr_get_priority (&att1, &priority);
    if (priority != cma_c_prio_through_max)
	cts_failed ("Failed to change \"priority\" attribute.");

    cma_attr_set_sched (&att1, cma_c_sched_rr, cma_c_prio_rr_min);
    cma_attr_get_sched (&att1, &policy);
    if (policy != cma_c_sched_rr)
	cts_failed ("Failed to change \"priority\" attribute.");

    cma_attr_set_stacksize (&att1, default_stack * 2);
    cma_attr_get_stacksize (&att1, &stacksize);
	/*
	 * Allow for round-up to "chunk" boundary.
	 */
    if (stacksize != roundup_chunksize(default_stack * 2))
	cts_failed ("Failed to change \"stacksize\" attribute.");

    cma_attr_set_guardsize (&att1, default_guard * 2);
    cma_attr_get_guardsize (&att1, &guardsize);
	/*
	 * Allow for round-up to "chunk" boundary.
	 */
    if (guardsize != roundup_chunksize(default_guard * 2))
	cts_failed ("Failed to change \"guardsize\" attribute.");

    /*
     * Now, delete it and create a new one (which should reclaim the original
     * from cache), and verify that all the values were set back to the
     * defaults!
     */
    cma_attr_delete (&att1);
    cma_attr_create (&att1, &cma_c_null);

    cma_attr_get_inherit_sched (&att1, &inherit_sched);

    if (inherit_sched != cma_c_sched_inherit)
	cts_failed ("Reclaimed attr. obj. has bad \"inherit sched\" attribute.");

    cma_attr_get_priority (&att1, &priority);

    if (priority != cma_c_prio_through_mid)
	cts_failed ("Reclaimed attr. obj. has bad \"priority\" attribute.");

    cma_attr_get_sched (&att1, &policy);

    if (policy != cma_c_sched_throughput)
	cts_failed ("Reclaimed attr. obj. has bad \"policy\" attribute.");

    cma_attr_get_stacksize (&att1, &stacksize);

    if (stacksize != roundup_chunksize(default_stack))
	cts_failed ("Reclaimed attr. obj. has bad \"stacksize\" attribute (has %d, should have %d).", stacksize, roundup_chunksize(default_stack));

    cma_attr_get_guardsize (&att1, &guardsize);

    if (guardsize != roundup_chunksize(default_guard))
	cts_failed ("Reclaimed attr. obj. has bad \"guardsize\" attribute (has %d, shjould have %d).", guardsize, roundup_chunksize(default_guard));

    cts_result();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_ATT_001.C */
/*  *11    1-APR-1991 17:17:36 BUTENHOF "Fix use of exception package" */
/*  *10   13-SEP-1990 15:55:13 CURTIN "changed guard size" */
/*  *9    10-SEP-1990 12:58:25 BUTENHOF "Fix another typo" */
/*  *8    10-SEP-1990 12:34:57 BUTENHOF "Fix typo" */
/*  *7     5-SEP-1990 09:40:06 BUTENHOF "Convert to new CMA interfaces" */
/*  *6     6-JUL-1990 12:47:46 CURTIN "changed default stack and guard sizes" */
/*  *5     3-JUL-1990 12:03:25 BUTENHOF "Change copyright" */
/*  *4    28-JUN-1990 18:39:59 SCALES "Enable policies and priorities" */
/*  *3    21-JUN-1990 08:27:17 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:35:49 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:57:45 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_ATT_001.C */
