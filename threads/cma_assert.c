/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_assert.c,v $
 * Revision 1.1.8.2  1996/02/17  23:33:40  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:49:52  marty]
 *
 * Revision 1.1.8.1  1995/12/07  21:59:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:04:36  root]
 * 
 * Revision 1.1.6.3  1994/06/28  19:49:31  jd
 * 	Have assert message output the pid.
 * 	[1994/06/28  17:28:30  jd]
 * 
 * Revision 1.1.6.2  1994/06/09  13:37:19  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:20  devsrc]
 * 
 * Revision 1.1.6.1  1994/04/01  20:15:26  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:15:48  jd]
 * 
 * Revision 1.1.4.2  1993/08/20  20:45:29  rhw
 * 	Added return 0 to quell compiler warning; for function returning an int,
 * 	"cma__assert_message", for CR 8418.
 * 	[1993/08/20  15:36:58  rhw]
 * 
 * Revision 1.1.2.3  1992/12/15  22:25:58  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:35:58  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:34:37  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:01:31  bolinger]
 * 
 * Revision 1.1.2.3  1992/08/14  20:20:50  bolinger
 * 	Merge up to BL10+ internal release from DEC.
 * 	[1992/08/11  20:50:49  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  22:35:37  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  22:35:23  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:38:50  devrcs
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
 *	001	Webb Scales	3 November 1989
 *		Changed type of cma__assert_message to cma_t_integer so it
 *		could be used in macros which expand to expressions instead of
 *		blocks.
 *	002	Paul Curtin	6 August 1990
 *		Replace printf w/ internal cma__put* functions.
 *		Add include for cma_util.h
 *	003	Dave Butenhof	14 December 1990
 *		Change cma_assertions.h to cma_assert.h (shorten length)
 *	004	Dave Butenhof	11 June 1991
 *		Change cma__assert_message() to call formatting functions for
 *		kernel and semaphore tracing if the appropriate conditionals
 *		are set.
 *	005	Dave Butenhof	30 July 1991
 *		Restore *printf functions, now that we emulate them on VMS to
 *		keep the VAX C rtl safely out of the picture.
 *	006	Paul Curtin	20 August 1991
 *		Conditionalized out the include of stdio.h on VMS.
 *	007	Dave Butenhof	24 March 1992
 *		Report assertion failure by a bugcheck, rather than by
 *		raising an exception that just obscures the error state.
 *	008	Webb Scales	20 May 1992
 *		Put assertion message text in the bugcheck message so that
 *		it is available in the log file as well.
 */

/*
 *  INCLUDE FILES
 */

#include <cma_assert.h>
#include <cma_errors.h>
#include <cma_util.h>
#if _CMA_OS_ == _CMA__UNIX
# include <stdio.h>
# include <unistd.h>
#endif

/*
 * LOCAL DATA
 */

static int 
    failure_count = 0,  /* Initially there are no failures */
    failure_limit = 1,  /* Limit on failure messages before program is 
			   terminated */
    warning_count = 0,  /* Initially there are no warnings */
    warning_limit = 50; /* Limit on warning messages before program is
			   terminated */

/*
 * LOCAL FUNCTIONS
 */



/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine is called when a warning assertion or failure
 * 	assertion triggers.
 *
 * 	For either kind, a count is incremented and if the count
 * 	does not exceed, or meets, a limit for that kind of assertion, an
 * 	error message is printed.  Then, if the limit on the number
 * 	of messages is met, or exceeded, a hard error is reported that
 * 	should result in program termination.
 *
 *  FORMAL PARAMETERS:
 *
 *	is_failure,	
 * 		TRUE if the assertion that triggered is a failure assertion
 * 		FALSE if the assertion that triggered is a warning assertion
 *
 *      description
 * 		Address of a string describing the problem in english.
 *
 *      file
 *		Name of the file where the assertion is located.
 *
 *      line	
 *		Line number where the assertion is located.
 *
 *  IMPLICIT INPUTS:
 *
 *	NONE
 *
 *  IMPLICIT OUTPUTS:
 *
 *	Output is generated that describes the assertion that failed.
 *
 *  FUNCTION VALUE:
 *
 *	NONE
 *
 *  SIDE EFFECTS:
 *
 *	The program is terminated if the assertion limit is reached.
 */

cma_t_integer
cma__assert_message(
	cma_t_boolean	is_failure,	/* Was it a fail or a warn? */
	cma__t_string	description,	/* English description of problem */
	cma__t_string	file,		/* File name */
	cma_t_integer	line)		/* Line number */
    {
    char    output[cma__c_buffer_size];
    extern pid_t getpid(void);

    output[0] = '\0';

    if (is_failure) {

	/* 
	 * Process a failure assertion.
	 *
	 * Increment the failure count.  Print out the error message
	 * up to the limit on the number of messages to be displayed.  
	 * When the limit is met or exceeded, then attempt to blow away 
	 * the program by reporting a hard error to the current
	 * thread.  
	 */
	failure_count++;

	if (failure_count <= failure_limit) {
	    cma__int_printf ("**** Assertion failure. %s\n",description);
	    cma__int_printf ("     at line %d in %s pid = %d\n", line, file,getpid());
	    };

	if (failure_count >= failure_limit)
	    cma__bugcheck (
		    "assertion failure:  %s\n\tat line %d in %s, pid = %d",
		    description,
		    line, 
		    file,
		    getpid());

	}
    else {

	/* 
	 * Process a warning assertion.
	 *
	 * Increment the warning count.  Print out the error message
	 * up to the limit on the number of messages to be displayed.  
	 * When the limit is met or exceeded, then simply stop 
	 * displaying warning messages.
	 */
	warning_count++;

	if (warning_count <= warning_limit) {
	    cma__int_printf ("**** Assertion warning. %s\n",description);
	    cma__int_printf ("     at line %d in %s pid = %d\n", line, file,getpid());
	    };

	if (warning_count >= warning_limit)
	    cma__bugcheck ("assertion warning overflow (%d) pid = %d", warning_limit,getpid());

	}    
    
    return 0;
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ASSERT.C */
/*  *8    20-MAY-1992 16:52:08 SCALES "Put assertion info in the bugcheck message" */
/*  *7    24-MAR-1992 14:47:05 BUTENHOF "Report assertion failure with bugcheck" */
/*  *6    21-AUG-1991 16:42:12 CURTIN "Removed VMS include of stdio.h" */
/*  *5    31-JUL-1991 18:40:06 BUTENHOF "Drop cma__put* functions" */
/*  *4    11-JUN-1991 17:16:30 BUTENHOF "Add & use functions to dump kernel/sem trace arrays" */
/*  *3    10-JUN-1991 18:16:51 SCALES "Add sccs headers for Ultrix" */
/*  *2    14-DEC-1990 00:54:55 BUTENHOF "Change module names" */
/*  *1    12-DEC-1990 21:41:11 BUTENHOF "Assertions" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ASSERT.C */
