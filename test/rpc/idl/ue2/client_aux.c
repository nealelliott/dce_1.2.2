/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: client_aux.c,v $
 * Revision 1.1.4.2  1996/02/17  23:18:09  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:37:09  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:31:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:30  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:29:34  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:16:57  ganni]
 * 
 * $EndLog$
 */
/*
*/

#include <stdio.h>
#include <ue2.h>
#include <test_common.h>

int client_aux(h)
    handle_t h;
{
    int failures = 0;

    EXCEPTION_INIT(my_exc1);
    EXCEPTION_INIT(my_exc2);
    EXCEPTION_INIT(my_exc3);
    printf("Should catch my_exc1\n");
    TRY
    {
        op1(h);
    }
    CATCH (my_exc1)
        printf("my_exc1 - caught expected exception\n");
    CATCH_ALL
        printf("my_exc1 - wrong exception ****\n");
        failures++;
    ENDTRY

    printf("Should catch rpc_x_invalid_tag\n");
    TRY
    {
        op2(h);
    }
    CATCH (rpc_x_invalid_tag)
        printf("rpc_x_invalid_tag - caught expected exception\n");
    CATCH_ALL
        printf("rpc_x_invalid_tag - wrong exception ****\n");
        failures++;
    ENDTRY

    printf("Should catch my_exc3\n");
    TRY
    {
        op3(h);
    }
    CATCH (my_exc3)
        printf("my_exc3 - caught expected exception\n");
    CATCH_ALL
        printf("my_exc3 - wrong exception ****\n");
        failures++;
    ENDTRY


    printf("Test Complete\n");
    return failures;
}
