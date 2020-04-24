/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_assert.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:33:30 $
 */
/*
 * HISTORY
 * $Log: cma_assert.h,v $
 * Revision 1.1.8.1  1996/05/10  13:33:30  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:38 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:41 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:21  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:15:52  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  13:51  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/20  18:08:24  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:42:56  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:10:57  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  17:49:35  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:31:54  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:36:03  alan]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:01:38  bolinger]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  22:48:52  bolinger]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:33:11  mgm  1.1.1.11]
 *
 * Revision 1.1.4.2  1994/06/09  13:37:20  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:21  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:15:28  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:15:52  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:26:02  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:36:03  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:34:43  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:01:38  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  22:49:07  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  22:48:52  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:38:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *	%W%	(ULTRIX/OSF)	%G%
 */
/*
 *  Copyright (c) 1989, 1990 by
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
 *	Header file for CMA Assertions package
 *
 *  AUTHORS:
 *
 *	R. Conti
 *
 *  CREATION DATE:
 *
 *	3 August 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Webb Scales	17 October 1989
 *		Corrected case of #include filenames
 *	002	Webb Scales	 3 November 1989
 *		Changed assertion macros from blocks to expressions
 *	003	Webb Scales	15 November 1989
 *		Changed NDEBUG version of assertion macros to expand to zero
 *		so that they remain valid expressions.
 */

/* 
 * Overview of Assertions
 * ----------------------
 *
 * An assertion is a claim that some relationship among program
 * variables will always be true. Put another way, an assertion is
 * a claim that an "invariant" holds true.
 * 
 * This module defines macros that allow you to insert assertions into
 * your code.  (The assertions inserted using this module are only present
 * in a 'debugging version' of CMA.  In a 'production version' of CMA,
 * while assertions remain present in the source code, no object code
 * is generated -- the assertions "evaporate".
 *
 * The purpose of assertions is to facilitate early detection of unexpected
 * situations, internal inconsistencies, and bugs in the CMA implementation.  
 * Since they do not generate any code in a production version of CMA,
 * and do not use messages that can be easily internationalized, they should
 * not be used for reporting client-generated errors (a separate 
 * error-message-reporting package is provided for that).
 *
 * As a general rule, when your code makes important assumptions
 * about the state of variables, it is a good idea to code an assertion
 * for each such assumption.  When you document such assumptions in
 * assertions, you will be immediately notified when the assertion is
 * violated at run-time (during testing or development). Without assertions, 
 * such assumptions would not get checked and might not even get documented.  
 * Assertions help to ensure that future maintenance will not accidentally 
 * violate some important built-in assumption.
 * 
 * Appropriate usage of this package saves many hours spent tracking
 * down the obscure errors that typically occur during development.
 * (It is a lot easier to locate a failing assertion than to work back
 * from an "access violation" to the offending source line.)
 *
 * Kinds of Assertions
 * -------------------
 * This module recognizes two kinds of assertions -- warnings and failures.  
 * 
 * You should insert a warning assertion when some assumption is violated
 * that is not necessarily a 'fatal' error.  When a warning assertion
 * is violated, the warning message is displayed and program execution
 * resumes.  You can also use warning assumptions to document things that 
 * you think *ought* to be true, even if you aren't absolutely sure -- 
 * if you are wrong, the consequence is a visible warning message and
 * you will will be able to investigate the inconsistency. An example of a 
 * warning assertion, stated in English, is "The foo list shouldn't have more 
 * than 100 elements". (This assumption might be the basis of your deciding 
 * to use sequential search rather than a hash algorithm, and you'd like to 
 * know when your assumption is wrong.) This assertion would be coded as:
 *  
 *   cma__assert_warn (foo_list_size <= 100, 
 *       "Size of foo list exceeds 100 elements");
 *
 * You should insert a failure assertion when some assumption is violated
 * that *must* be true for the subsequent code to execute correctly.  An
 * example of a failure assertion, is that a particular mutex *must* be locked.
 * This assertion would be coded as:
 *
 *   cma__assert_fail (cma_is_locked(att_mutex), 
 *       "Attributes object mutex is not locked");
 *
 * When Are Assertions Present?
 * ----------------------------
 * Assertions can be enabled in any build of CMA.  Typically, we do so
 * only when building a 'debugging version'.  Currently assertions are
 * activated by default whenever you compile a module, and can be made
 * to evaporate by compiling all modules with symbol NDEBUG defined.
 * (There might be some reason in the future to use a different scheme
 * since NDEBUG has widespread effects on various C macros.  But, this
 * seems reasonable for now.)
 * 
 * The test system will be run against a debugging version.
 * Also, while we are developing code, we will link against debugging
 * versions of the object modules.  So, in general, we'll all know
 * immediately when any assertion is violated.
 */

#ifndef CMA_ASSERTIONS
#define CMA_ASSERTIONS

/*
 *  INCLUDE FILES
 */
#include <cma.h>
#include <cma_defs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

/* 
 * Macro for warning assertion.  Generates no code in productions builds.
 * 
 *      cma__assert_warn (<C_boolean_expression>, <description of violation>);
 *      NOTE: a C_boolean_expression is an integer-valued expression that
 *            is interpreted as false if 0 and is true otherwise.
 *     
 */

#ifndef NDEBUG
#define cma__assert_warn(condition, description)			    \
    ( (!(condition)) ?							    \
	cma__assert_message (						    \
		cma_c_false,						    \
		description, 						    \
		__FILE__, 						    \
		__LINE__) :						    \
	0 )
#else
#define cma__assert_warn(condition, description)    0
#endif

/* 
 * Macro for failure assertions.  Generates no code in production builds.
 * 
 *      cma__assert_fail (<C_boolean_expression>, <description of violation>);
 *      NOTE: a C_boolean_expression is an integer valued expression that
 *            is interpreted as false if 0 and is true otherwise.
 */
#ifndef NDEBUG
#define cma__assert_fail(condition, description)			    \
    ( (!(condition)) ?							    \
	cma__assert_message ( 			    			    \
		cma_c_true, 			    			    \
		description, 			    			    \
		__FILE__,						    \
		__LINE__) :						    \
	0 )
#else
#define cma__assert_fail(condition, description)    0
#endif

/*
 * TYPEDEFS
 */

/*
 *  GLOBAL DATA
 */

/*
 * INTERNAL INTERFACES
 */

/* 
 * Worker routine needed by above macros.  
 * Reports the error when an assertion is violated.
 */
cma_t_integer cma__assert_message (cma_t_boolean,cma__t_string,cma__t_string,cma_t_integer);

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ASSERT.H */
/*  *4    10-JUN-1991 19:50:31 SCALES "Convert to stream format for ULTRIX build" */
/*  *3    10-JUN-1991 19:20:01 BUTENHOF "Fix the sccs headers" */
/*  *2    10-JUN-1991 18:16:59 SCALES "Add sccs headers for Ultrix" */
/*  *1    12-DEC-1990 21:41:13 BUTENHOF "Assertions" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ASSERT.H */
