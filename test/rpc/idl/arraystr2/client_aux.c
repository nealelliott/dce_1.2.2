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
 * Revision 1.1.4.2  1996/02/17  23:12:34  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:44  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:56:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:28:27  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:05:41  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:26:46  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:31:08  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <math.h>
#include <test_common.h>
#include <arraystr2.h>
#include <util_macros.h>

int client_aux(h, passno)
    handle_t h;
    int passno;
{
    int                 i, j, k, failures = 0, mgr_failures;
    array_il_string_t   array_il_string;    /* Array of struct ct. string */
    array_ool_string_t  array_ool_string;   /* Array of struct ct. string */
    char                *alphabet = "abcdefghijklmnopqrstuvwxyz";
    char                *ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    /*
     * op1 -  array of [in_line] struct containing string.
     */
    for (i = 0 ; i < ARRAY_SIZE ; i++)
    {
        /* Put a variation of the alphabet into this string. */
        for (j = 0, k = i%STRING_SIZE ; j < STRING_SIZE ; j++, k++)
            array_il_string[i].s[j] = alphabet[k%STRING_SIZE];
        array_il_string[i].s[STRING_SIZE] = '\0';
    }

    CALL(op1, (h, array_il_string), mgr_failures, failures);
    for (i = 0 ; i < ARRAY_SIZE ; i++)
    {
        /* Each return string should be the original in upper case. */
        for (j = 0, k = i%STRING_SIZE ; j < STRING_SIZE ; j++, k++)
            if (array_il_string[i].s[j] != ALPHABET[k%STRING_SIZE])
                FAIL("Return values from op1 are incorrect\n", failures);
        if (array_il_string[i].s[STRING_SIZE] != '\0')
            FAIL("Return string from op1 does not have null terminator\n",
                failures);
    }

    /*
     * op2 -  array of [out_of_line] struct containing string.
     */
    for (i = 0 ; i < ARRAY_SIZE ; i++)
    {
        /* Put a variation of the alphabet into this string. */
        for (j = 0, k = i%STRING_SIZE ; j < STRING_SIZE ; j++, k++)
            array_ool_string[i].s[j] = alphabet[k%STRING_SIZE];
        array_ool_string[i].s[STRING_SIZE] = '\0';
    }

    CALL(op2, (h, array_ool_string), mgr_failures, failures);
    for (i = 0 ; i < ARRAY_SIZE ; i++)
    {
        /* Each return string should be the original in upper case. */
        for (j = 0, k = i%STRING_SIZE ; j < STRING_SIZE ; j++, k++)
            if (array_ool_string[i].s[j] != ALPHABET[k%STRING_SIZE])
                FAIL("Return values from op2 are incorrect\n", failures);
        if (array_ool_string[i].s[STRING_SIZE] != '\0')
            FAIL("Return string from op2 does not have null terminator\n",
                failures);
    }

    if (failures != 0)
        fprintf(stderr, "%d failures.\n", failures);

    return failures;
}
