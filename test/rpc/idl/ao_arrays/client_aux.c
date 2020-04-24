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
 * Revision 1.1.4.2  1996/02/17  23:12:12  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:31  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:55:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:27:53  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:01:43  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:24:51  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:30:45  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <test_common.h>
#include <ao_arrays.h>

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



void setup_array(a)
    array_v1_string_t   a; 
{
    int                 i, j, k, failures = 0;
    char                *alphabet = "abcdefghijklmnopqrstuvwxyz";
    char                *ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (i = 0 ; i < ARRAY_SIZE ; i++)
    {
        /* Put a variation of the alphabet into this string. */
        for (j = 0, k = i%STRING_SIZE ; j < STRING_SIZE ; j++, k++)
            a[i][j] = alphabet[k%STRING_SIZE];
        a[i][STRING_SIZE] = '\0';
    }
}

int check_array(a)
    array_v1_string_t   a; 
{
    int                 i, j, k, failures = 0;
    char                *alphabet = "abcdefghijklmnopqrstuvwxyz";
    char                *ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (i = 0 ; i < ARRAY_SIZE ; i++)
    {
        /* Each return string should be the original in upper case. */
        for (j = 0, k = i%STRING_SIZE ; j < STRING_SIZE ; j++, k++)
            if (a[i][j] != ALPHABET[k%STRING_SIZE])
            {
                printf("Bad Return value[%d,%d]='%c', expected '%c'\n",
                    i,j,a[i][j], ALPHABET[k%STRING_SIZE]);
                return 1;
            }

        if (a[i][STRING_SIZE] != '\0')
            FAIL("Return string does not have null terminator\n",
                failures);
    }
    return failures;
}

int client_aux(h, passno)
    handle_t h;
    int passno;
{
    int                 i, failures = 0, mgr_failures;
    array_v1_string_t   a[AO_ARRAY_SIZE];    /* Array of strings */


    printf("op1 test...\n");
    for (i = 0 ; i < AO_ARRAY_SIZE ; i++)
        setup_array(a[i]);
    CALL(op1, (h, a), mgr_failures, failures);
    for (i = 0 ; i < AO_ARRAY_SIZE ; i++)
        failures += check_array(a[i]);


    printf("op2 test...\n");
    for (i = 0 ; i < AO_ARRAY_SIZE ; i++)
        setup_array(a[i]);
    CALL(op2, (h, a), mgr_failures, failures);
    for (i = 0 ; i < AO_ARRAY_SIZE ; i++)
        failures += check_array(a[i]);


    printf("op3 test...\n");
    for (i = 0 ; i < AO_ARRAY_SIZE ; i++)
        setup_array(a[i]);
    CALL(op3, (h, a), mgr_failures, failures);
    for (i = 0 ; i < AO_ARRAY_SIZE ; i++)
        failures += check_array(a[i]);


    printf("op4 test...\n");
    for (i = 0 ; i < AO_ARRAY_SIZE ; i++)
        setup_array(a[i]);
    CALL(op4, (h, a), mgr_failures, failures);
    for (i = 0 ; i < AO_ARRAY_SIZE ; i++)
        failures += check_array(a[i]);


    printf("op5 test...\n");
    for (i = 0 ; i < AO_ARRAY_SIZE ; i++)
        setup_array(a[i]);
    CALL(op5, (h, a), mgr_failures, failures);
    for (i = 0 ; i < AO_ARRAY_SIZE ; i++)
        failures += check_array(a[i]);


    printf("op6 test...\n");
    for (i = 0 ; i < AO_ARRAY_SIZE ; i++)
        setup_array(a[i]);
    CALL(op6, (h, a), mgr_failures, failures);
    for (i = 0 ; i < AO_ARRAY_SIZE ; i++)
        failures += check_array(a[i]);


    printf("op7 test...\n");
    for (i = 0 ; i < AO_ARRAY_SIZE ; i++)
        setup_array(a[i]);
    CALL(op7, (h, a), mgr_failures, failures);
    for (i = 0 ; i < AO_ARRAY_SIZE ; i++)
        failures += check_array(a[i]);


    return failures;
}
