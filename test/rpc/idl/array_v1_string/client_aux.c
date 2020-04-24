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
 * Revision 1.1.4.2  1996/02/17  23:12:22  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:36  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:55:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:28:05  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:03:30  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:25:35  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:30:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <test_common.h>
#include <array_v1_string.h>

/*
 * Call an RPC routine.
 *      routine_name    = name of RPC routine to call
 *      arguments       = routine argument list (enclose in parentheses)
 *      mgr_failures    = variable for count of manager failures
 *      failures        = variable for count of local failures
 */
#define CALL(routine_name, arguments, mgr_failures, failures) \
{ \
    mgr_failures = routine_name arguments; \
    if (mgr_failures != 0) \
    { \
        fprintf(stderr, "Failure in routine_name manager routine\n"); \
        failures += mgr_failures; \
    } \
}

/*
 * Report a failure and bump error count.
 *      error_string    = error text for fprintf
 *      failures        = variable for count of local failures
 */
#define FAIL(error_string, failures) \
{ \
    fprintf(stderr, error_string); \
    failures++; \
}

int client_aux(h, passno)
    handle_t h;
    int passno;
{
    int                 i, j, k, failures = 0, mgr_failures;
    array_v1_string_t   array_v1_string;    /* Array of strings */
    char                *alphabet = "abcdefghijklmnopqrstuvwxyz";
    char                *ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (i = 0 ; i < ARRAY_SIZE ; i++)
    {
        /* Put a variation of the alphabet into this string. */
        for (j = 0, k = i%STRING_SIZE ; j < STRING_SIZE ; j++, k++)
            array_v1_string[i][j] = alphabet[k%STRING_SIZE];
        array_v1_string[i][STRING_SIZE] = '\0';
    }
    CALL(op8, (h, array_v1_string), mgr_failures, failures);
    for (i = 0 ; i < ARRAY_SIZE ; i++)
    {
        /* Each return string should be the original in upper case. */
        for (j = 0, k = i%STRING_SIZE ; j < STRING_SIZE ; j++, k++)
            if (array_v1_string[i][j] != ALPHABET[k%STRING_SIZE])
                FAIL("Return values from op8 are incorrect\n", failures);
        if (array_v1_string[i][STRING_SIZE] != '\0')
            FAIL("Return string from op8 does not have null terminator\n",
                failures);
    }

    if (failures != 0)
        fprintf(stderr, "%d failures.\n", failures);

    return failures;
}
