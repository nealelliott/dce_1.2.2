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
 * Revision 1.1.4.2  1996/02/17  23:14:08  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:40  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:05:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:31:16  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:56:05  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:33:18  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:32:37  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <factorial.h>
#include <test_common.h>

int lcl_factorial();
int lcl_factorial (n)
    int n;
{
    if (n == 0) return 1;
    return n * lcl_factorial(n-1);
}

int client_aux(h,passes)
    handle_t h;
    int passes;
{
    int failures = 0;
    int result;
    idl_long_int *numberp = (idl_long_int *)(&passes);

    result = (int)net_factorial(h, &numberp);
    if (result != lcl_factorial(passes))
    {
        printf("***FAILURE***: net_factorial(%d) returned: %d, expected: %d\n",
               passes, result, lcl_factorial(passes));
        return 1;
    }

    return 0;
}
