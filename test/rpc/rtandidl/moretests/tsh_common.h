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
 * Revision 1.1.4.2  1996/02/18  23:06:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:14  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:11:59  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/28  19:07 UTC  tatsu_s
 * 	Added the failure status message.
 * 
 * 	HP revision /main/tatsu_s.func_test_b2/1  1995/04/28  18:21 UTC  tatsu_s
 * 	Print the failure status.
 * 	[1995/12/11  19:45:47  root]
 * 
 * Revision 1.1.2.3  1993/01/25  19:29:21  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:46:08  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/13  19:48:44  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:15:42  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:38:26  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
#
#
# Copyright (c) 1991 by
#    Hewlett-Packard Company, Palo Alto, Ca.
#
# Permission to use, copy, modify, and distribute this
# software and its documentation for any purpose and without
# fee is hereby granted, provided that the above copyright
# notice appear in all copies and that both that copyright
# notice and this permission notice appear in supporting
# documentation, and that the name of Hewlett-Packard Company not
# be used in advertising or publicity pertaining to distribution
# of the software without specific, written prior permission.
# Hewlett-Packard Company makes no representations about the
# suitability of this software for any purpose.  It is provided
# "as is" without express or implied warranty.
#
#
# DCE API Test
#
*/



#ifndef TSH_COMMON_INCLUDED
#define TSH_COMMON_INCLUDED

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

#define DEFAULTMASK  (MOFAILURE|MOSTATUS)


/*  the simple boolean states yet again...
 */
#ifdef TRUE
#undef TRUE
#endif
#define TRUE         0x00000001L
#ifdef FALSE
#undef FALSE
#endif
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
#define TSH_UNIMPLEMENTED        0x01cc0099L


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
