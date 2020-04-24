/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tsh_common.h,v $
 * Revision 1.2.5.2  1996/02/18  23:07:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:41  marty]
 *
 * Revision 1.2.5.1  1995/12/11  21:50:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:53:07  root]
 * 
 * Revision 1.2.2.2  1992/12/31  17:33:45  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:58:43  htf]
 * 
 * Revision 1.2  1992/01/19  22:14:40  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/*
*/
/*
 *  OSF DCE Version 1.0 
*/
#ifndef TSH_COMMON_INCLUDED
#define TSH_COMMON_INCLUDED
/*
**  Copyright (c) 1990 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass. &
**      Locus Computing Corporation, Burlington, Mass.
**
** 
**
**  $Log: tsh_common.h,v $
 * Revision 1.2.5.2  1996/02/18  23:07:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:41  marty]
 *
 * Revision 1.2.6.2  1996/02/18  22:28:41  marty
 * 	Update OSF copyright years
 *
 * Revision 1.2.5.1  1995/12/11  21:50:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:53:07  root]
 *
 * Revision 1.2.4.2  1995/12/11  20:53:07  root
 * 	Submit OSF/DCE 1.2.1
 *
 * Revision 1.2.2.2  1992/12/31  17:33:45  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:58:43  htf]
 *
 * Revision 1.2.1.2  1992/12/31  16:58:43  htf
 * 	Embedding copyright notice
 *
 * Revision 1.2  1992/01/19  22:14:40  devrcs
 * Dropping DCE1.0 OSF1_misc port archive
 *
 * Revision 1.5.2.2  92/01/06  11:43:52  zeliff
 * 	moving up to dce1.0/dce.75
 * 	[92/01/06  11:05:35  zeliff]
 * 
 * Revision 1.5.1.2  92/01/06  11:05:35  zeliff
 * 	moving up to dce1.0/dce.75
 * 
 * Revision 1.4.2.3  92/01/02  12:35:55  eperkins
 * 
 * 
 * Revision 1.4.3.2  92/01/02  12:34:52  eperkins
 * 
 * 
 * Revision 1.4.2.2  92/01/02  12:25:15  eperkins
 * 	Added preprocessor checks for TRUE and FALSE definition
 * 	[92/01/02  12:24:10  eperkins]
 * 
 * Revision 1.4.1.2  92/01/02  12:24:10  eperkins
 * 	Added preprocessor checks for TRUE and FALSE definition
 * 
 * Revision 1.4  91/12/20  17:06:30  devrcs
 * Adding dce.74d to the rcs tree
 * 
 * Revision 1.1  91/08/22  11:07:16  finger
 * Initial check-in
 * 
**
** @(#)tsh_common.h	1.1
**  TSH LIBRARY HEADER 
**	Extracted	2/15/91 17:50:00
**	from delta on	2/15/91 17:49:11
 */
#ifndef LINT
static char tsh_common_sccs_id[] = "@(#)tsh_common.h	1.1";
#endif

#include <stdio.h>
#include <dce/rpc.h>


/*  Define the mask entries. and the default.  This is used for displaying output to
 *  the screen based on user preferences.  These masks are set from the command line
 *  of TSH. The MO stands for "Message On" and has no relation to MOBetter (Spike Lee).
 */
#define MOFAILURE    0x00000001L
#define MOSUCCESS    0x00000002L
#define MOWARNING    0x00000004L
#define MOSTATUS     0x00000008L
#define MOINFO       0x00000010L
#define MOTRACE      0x00000020L

#define DEFAULTMASK  MOFAILURE


/*  the simple boolean states yet again...
 */

#ifdef TRUE
#undef TRUE
#endif
#ifdef FALSE
#undef FALSE
#endif

#define TRUE         0x00000001L
#define FALSE        0x00000000L


/*  Also define SUCCESS and FAIL...
 */
#define SUCCESS      0x00000000L
#define FAILURE      0x00000001L


/*  These are the types of functions the test programs employ.  When the status
 *  of a particular operation is checked, the type of function it is makes a
 *  difference in how the info gets logged to output.
 */
#define SETUP        0x00000000L
#define TEST         0x00000001L
#define CLNUP        0x00000002L


/*  Here are some of our TSH error states.
 */
#define TSH_SUCCESS              rpc_s_ok
#define TSH_TEST_FAILED          0x01cc0001L
#define TSH_DATA_MISSMATCH       0x01cc0002L
#define TSH_UNKNOWN_SUBCASE      0x01cc0003L
#define TSH_UNKNOWN_ARGUMENT     0x01cc0004L
#define TSH_INVALID_ARGUMENT     0x01cc0005L
#define TSH_BAD_ARG_COUNT        0x01cc0006L
#define TSH_EXCEPTION_CAUGHT     0x01cc0007L
#define TSH_INCORRECT_DATA       0x01cc0008L
#define TSH_EXCEPTION_NOT_RAISED 0x01cc0009L
#define TSH_UNIMPLEMENTED        0x01cc0010L
#define TSH_CANT_ALLOCATE_MEMORY 0x01cc0011L


#ifdef DEBUG
/*  PRINT_ARGS - a debug line for printing out argc, argv contents.
 */
#    define PRINT_ARGS(argc, argv) \
        { \
        int i; \
        for(i=0; i<argc; i++) \
            printf("argv[%d] = %s\n", i, argv[i]); \
        }
#endif


#ifndef TSH_MAIN_INCLUDED
/*  If the ctx block has not yet been declared (as in tsh_main.h) define
 *  something in here that will pass for a ctx block.  This isolates the 
 *  context block from the test programs, since they have no business in
 *  there anyway.
 */
typedef struct ctx
    {
    unsigned32 dummy;
    } CtxBlock;
#endif


/*  Give test programs a way to cjeck bit settings...
 */
#define	ISSUCCESS(x)	(x->OptionsMask & MOSUCCESS)
#define	ISFAILURE(x)	(x->OptionsMask & MOFAILURE)
#define	ISWARNING(x)	(x->OptionsMask & MOWARNING)
#define	ISSTATUS(x)	(x->OptionsMask & MOSTATUS)
#define	ISINFO(x)	(x->OptionsMask & MOINFO)
#define	ISTRACE(x)	(x->OptionsMask & MOTRACE)

#endif
