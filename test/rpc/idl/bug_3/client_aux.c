/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * (c) Copyright 1991, 1992 
 *     Siemens-Nixdorf Information Systems, Burlington, MA, USA
 *     All Rights Reserved
 */
/*
 * HISTORY
 * $Log: client_aux.c,v $
 * Revision 1.1.5.2  1996/02/17  23:12:59  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:02  marty]
 *
 * Revision 1.1.5.1  1995/12/11  19:59:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:29:06  root]
 * 
 * Revision 1.1.3.3  1993/01/07  19:10:19  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:28:57  bbelch]
 * 
 * Revision 1.1.3.2  1992/09/29  21:20:36  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:46:01  weisman]
 * 
 * Revision 1.1  1992/01/19  03:31:21  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdlib.h>
#include <stdio.h>
#include <bug_3.h>

ndr_boolean verbose = false;

static ndr_boolean argument_values[] = {
    0, 1, -1
};

#define DIMENSION(array) (sizeof(array)/sizeof(array[0]))

#ifdef SNI_SVR4
#define TVAL 0xff
#endif

int client_aux
#ifdef NIDL_PROTOTYPES
(handle_t h, int passes)
#else
(h, passes)
handle_t h;
int passes;
#endif
{
    int index;
    long failures = 0;
    ndr_boolean argument, return_value;

    if (verbose) {
        printf("In bug_3_test().\n");
        printf("true == %#x\n", (unsigned)true);
        printf("false == %#x\n", (unsigned)false);
        printf("ndr_true == %#x\n", (unsigned)ndr_true);
        printf("ndr_false == %#x\n", (unsigned)ndr_false);
        printf("idl_true == %#x\n", (unsigned)idl_true);
        printf("idl_false == %#x\n", (unsigned)idl_false);
        printf("TRUE == %#x\n", (unsigned)bug_3_true);
        printf("FALSE == %#x\n", (unsigned)bug_3_false);
        printf("sizeof(ndr_boolean) == %d\n", sizeof(ndr_boolean));
        printf("sizeof(idl_boolean) == %d\n", sizeof(idl_boolean));
    }
#ifdef SNI_SVR4
    if (true != TVAL)
    {
        ++failures;
        printf("true != %X\n", TVAL);
    }
    if (false != 0)
    {
        ++failures;
        printf("false != 0\n");
    }
    if (bug_3_true != TVAL)
    {
        ++failures;
        printf("bug_3_true != %X\n", TVAL);
    }
    if (bug_3_false != 0)
    {
        ++failures;
        printf("bug_3_false != 0\n");
    }
    if (ndr_true != TVAL)
    {
        ++failures;
        printf("ndr_true != %X\n", TVAL);
    }
    if (ndr_false != 0)
    {
        ++failures;
        printf("ndr_false != 0\n");
    }
    if (idl_true != TVAL)
    {
        ++failures;
        printf("idl_true != %X\n", TVAL);
    }
    if (idl_false != 0)
    {
        ++failures;
        printf("idl_false != 0\n");
    }
#else
    if (true != 0xff)
    {
        ++failures;
        printf("true != 0xff\n");
    }
    if (false != 0)
    {
        ++failures;
        printf("false != 0\n");
    }
    if (bug_3_true != 0xff)
    {
        ++failures;
        printf("bug_3_true != 0xff\n");
    }
    if (bug_3_false != 0)
    {
        ++failures;
        printf("bug_3_false != 0\n");
    }
    if (ndr_true != 0xff)
    {
        ++failures;
        printf("ndr_true != 0xff\n");
    }
    if (ndr_false != 0)
    {
        ++failures;
        printf("ndr_false != 0\n");
    }
    if (idl_true != 0xff)
    {
        ++failures;
        printf("idl_true != 0xff\n");
    }
    if (idl_false != 0)
    {
        ++failures;
        printf("idl_false != 0\n");
    }
#endif
    if (sizeof(ndr_boolean) != sizeof(ndr_byte))
    {
        ++failures;
        printf("sizeof(ndr_boolean) != sizeof(ndr_byte)\n");
    }
    if (sizeof(idl_boolean) != sizeof(idl_byte))
    {
        ++failures;
        printf("sizeof(idl_boolean) != sizeof(idl_byte)\n");
    }

    for (index = 0; index < DIMENSION(argument_values); ++index) {
        argument = argument_values[index];
        if (verbose)
            printf("argument == %#x; ", (unsigned)argument);
        return_value = bug_3_op0(h, argument);
        if (verbose)
            printf(
                "%#x == bug_3_op0(h, %#x)\n",
                (unsigned)return_value, (unsigned)argument
            );
        if (return_value != !argument) {
            ++failures;
            if (verbose)
                printf("bug_3_op0() failures == %ld\n", failures);
        }
    }

    for (index = 0; index < DIMENSION(argument_values); ++index) {
        argument = argument_values[index];
        if (verbose)
            printf("argument == %#x; ", (unsigned)argument);
        return_value = bug_3_op1(h, &argument);
        if (verbose)
            printf(
                "%#x == bug_3_op1(h, &%#x)\n",
                (unsigned)return_value, (unsigned)argument
            );
        if (!!return_value != !!argument_values[index]) {
            ++failures;
            if (verbose)
                printf("bug_3_op1(return) failures == %ld\n", failures);
        }
        if (argument != !argument_values[index]) {
            ++failures;
            if (verbose)
                printf("bug_3_op1(argument) failures == %ld\n", failures);
        }
    }

    if (verbose && failures)
        printf("The %s failed %ld times.\n", bug_3_name, failures);

    return failures;
}
