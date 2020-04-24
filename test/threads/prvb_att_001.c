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
 * $Log: prvb_att_001.c,v $
 * Revision 1.1.17.1  1996/06/04  22:14:53  arvind
 * 	Add _HP_CMA fo param.h
 * 	[1996/03/08  22:18 UTC  bissen  /main/DCE_1.2/1]
 *
 * Revision 1.1.15.2  1996/02/17  23:31:45  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:13  marty]
 * 
 * Revision 1.1.15.1  1995/12/11  23:03:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:28  root]
 * 
 * Revision 1.1.12.1  1993/10/12  19:37:36  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:01  rhw]
 * 
 * Revision 1.1.10.2  1993/06/14  20:18:25  sommerfeld
 * 	Define default stack/guard sizes for
 * 	PA-RISC machines; define roundup
 * 	macro so that we can do accurate
 * 	compares of stack sizes.
 * 	[1993/06/04  20:38:24  hopkins]
 * 
 * Revision 1.1.5.4  1992/12/30  16:54:10  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:12:20  zeliff]
 * 
 * Revision 1.1.5.3  1992/09/29  20:40:00  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:34:41  sekhar]
 * 
 * Revision 1.1.5.2  1992/09/03  14:32:59  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:45:51  bolinger]
 * 
 * Revision 1.1.3.2  1992/08/11  21:44:45  bolinger
 * 	Liberalize stack size checks to account for possibility that
 * 	actual size used will be rounded up (e.g.) to "chunk"
 * 	boundary.
 * 
 * Revision 1.1.2.3  1992/05/21  14:16:27  bolinger
 * 	Provide OT enhancement 3478:  Change verification of stack size
 * 	increase to make it more portable.
 * 	[1992/05/20  19:59:12  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/14  16:03:45  smith
 * 	ifdefs for system specific default_stack sizes
 * 	[1992/05/14  00:46:28  smith]
 * 
 * Revision 1.1  1992/01/19  03:19:40  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1989, 1990 by
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
 *	PTHREAD Test Programs
 *
 *  FILENAME:
 *
 *	PRVB_ATT_001.C
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
 *	002	Paul Curtin	30 May 1990
 *		Converted test to use pthread interface.
 *	003	Paul Curtin	18 June 1990
 *		Switch the way some args are recieved and
 *		got rid of any use of guardsizes.
 *	004	Webb Scales	16 July 1990
 *		Changed internal references to correct filename.
 *		"Enabled" priority and policy checks.
 *		Purged references to "cma".
 */
 
/*
 *  INCLUDE FILES
 */
 
#include <pthread.h>
#include <dce/cts_test.h>
#include <dce/cma_defs.h>
#if defined(SNI_SVR4_POSIX)
#include <sys/unistd.h>
#endif

#if (_CMA_VENDOR_ == _CMA__IBM) || (_CMA_VENDOR_ == _CMA__HP)
# include <sys/param.h>
#endif

#if defined(SNI_SVR4_POSIX)
# define page_size sysconf( _SC_PAGESIZE )
#else
# if (_CMA_VENDOR_ == _CMA__HP) || (_CMA_VENDOR_ == _CMA__IBM)
#  define page_size NBPG    /* Page size in bytes */
# else
#  define page_size 512 /* Page size in bytes */
# endif
#endif

# if (_CMA_VENDOR_ == _CMA__IBM) || (_CMA_UNIX_TYPE == _CMA__SVR4)
#  define default_stack page_size * 8   /* Default stack */
#  define default_guard page_size * 2   /* Default guard */
# elif (_CMA_VENDOR_ == _CMA__HP) && (_CMA_HARDWARE_ == _CMA__HPPA)
#  if _CMA_HARDWARE_ == _CMA__HPPA
#   define default_stack page_size * 15  /* Default stack */
#   define default_guard page_size * 4   /* Default guard */
#  else
#   define default_stack page_size * 8  /* Default stack */
#   define default_guard page_size * 2  /* Default guard */
#  endif
# else
#  define default_stack page_size * 60  /* Default stack */
#  define default_guard page_size * 8   /* Default guard */
# endif

#define default_prio	cma_c_prio_through_mid

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
    pthread_attr_t  att1;		/* test attr object */
    int		    inherit_sched;	/* value of inherit pri. att. */
    int		    priority;		/* value of thread priority att */
    int		    policy;		/* value of thread sched. policy att */
    unsigned int    stacksize;		/* value of stack size att. */
    int		    status;


    cts_test("PRVB_ATT_001", "Test create/delete/modify/get of attr objects");
 
    status = pthread_attr_create (&att1);
    check(status,"pthread_attr_create");

    /*
     * Now that we have an attr object, verify that all default attribute 
     * values are set currectly.  Note that some comparisons (eg, stacksize)
     * which alren't architecturally specified depend on constants from the 
     * internal definitions file which are separately defined above.
     */
    inherit_sched = pthread_attr_getinheritsched (att1);

    if (inherit_sched != PTHREAD_INHERIT_SCHED)
	cts_failed ("Default attr. obj. has bad \"inherit sched\" attribute.");

    priority = pthread_attr_getprio (att1);

    if (priority != default_prio)
	cts_failed ("Default attr. obj. has bad \"priority\" attribute.");

    policy = pthread_attr_getsched (att1);

    if (policy != SCHED_OTHER)
	cts_failed ("Default attr. obj. has bad \"policy\" attribute.");

    stacksize = pthread_attr_getstacksize (att1);

	/*
	 * Allow for roundup to "chunk" boundary.
	 */
    if (stacksize != roundup_chunksize(default_stack))
	cts_failed ("Default attr. obj. has bad \"stacksize\" attribute (has %d, should have %d).", stacksize, roundup_chunksize(default_stack));

    /*
     * Now modify all of the attr, and read them back to verify that we
     * get the changed values.
     */
    status = pthread_attr_setinheritsched (&att1, PTHREAD_DEFAULT_SCHED);
    check(status,"pthread_attr_setinheritsched");
    inherit_sched = pthread_attr_getinheritsched (att1);
    if (inherit_sched != PTHREAD_DEFAULT_SCHED)
	cts_failed ("Failed to change \"inherit sched\" attribute.");

    status = pthread_attr_setprio (&att1, PRI_OTHER_MAX);
    check(status,"pthread_attr_setprio");
    priority = pthread_attr_getprio (att1);
    if (priority != PRI_OTHER_MAX)
	cts_failed ("Failed to change \"priority\" attribute.");

    status = pthread_attr_setsched (&att1, SCHED_RR);
    check(status,"pthread_attr_setsched");
    policy = pthread_attr_getsched (att1);
    if (policy != SCHED_RR)
	cts_failed ("Failed to change \"policy\" attribute.");

    status = pthread_attr_setstacksize (&att1, default_stack * 2);
    check(status,"pthread_attr_setstacksize");
    stacksize = pthread_attr_getstacksize (att1);
	/*
	 * Allow for roundup to "chunk" boundary.
	 */
    if (stacksize != roundup_chunksize(default_stack * 2))
	cts_failed ("Failed to change \"stacksize\" attribute.");

    /*
     * Now, delete it and create a new one (which should reclaim the original
     * from cache), and verify that all the values were set back to the
     * defaults!
     */
    status = pthread_attr_delete(&att1);
    check(status,"pthread_attr_delete");
    status = pthread_attr_create(&att1);
    check(status,"pthread_attr_create");

    inherit_sched = pthread_attr_getinheritsched (att1);

    if (inherit_sched != PTHREAD_INHERIT_SCHED)
	cts_failed ("Reclaimed attr. obj. has bad \"inherit sched\" attribute.");

    priority = pthread_attr_getprio (att1);

    if (priority != default_prio)
	cts_failed ("Reclaimed attr. obj. has bad \"priority\" attribute.");

    policy = pthread_attr_getsched (att1);

    if (policy != SCHED_OTHER)
	cts_failed ("Reclaimed attr. obj. has bad \"policy\" attribute.");

    stacksize = pthread_attr_getstacksize (att1);

	/*
	 * Allow for roundup to "chunk" boundary.
	 */
    if (stacksize != roundup_chunksize(default_stack))
	cts_failed ("Reclaimed attr. obj. has bad \"stacksize\" attribute (has %d, should have %d).", stacksize, roundup_chunksize(default_stack));

    cts_result();
    }
