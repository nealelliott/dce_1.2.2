/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: client_s.c,v $
 * Revision 1.1.4.2  1996/02/17  23:14:01  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:37  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:04:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:30:58  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:25:05  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:13:44  ganni]
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <es_arrayc.h>
#include <cstrop.h>
#include <test_common.h>

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

    for (i = 0 ; i < ARRAY_SIZE ; i++)
    {
        for (j = 0, k = i%STRING_SIZE ; j < STRING_SIZE ; j++, k++)
            if (a[i][j] != alphabet[k%STRING_SIZE])
            {
                printf("Bad Return value[%d,%d]='%c', expected '%c'\n",
                    i,j,a[i][j], alphabet[k%STRING_SIZE]);
                return 1;
            }

        if (a[i][STRING_SIZE] != '\0')
            FAIL("Return string does not have null terminator\n",
                failures);
    }
    return failures;
}

void in_fn_sub(p_es_h)
    idl_es_handle_t *p_es_h;
{
    array_v1_string_t   a[AO_ARRAY_SIZE];    /* Array of strings */
    idl_long_int m;
    int i;

    for (i = 0 ; i < AO_ARRAY_SIZE ; i++)
        setup_array(a[i]);
    m = AO_ARRAY_SIZE-1;
    fn_op(p_es_h, &m, a);
}

void out_fn_sub(p_es_h, p_failures)
    idl_es_handle_t *p_es_h;
    int *p_failures;
{
    array_v1_string_t   out_a[AO_ARRAY_SIZE];    /* Array of strings */
    idl_long_int out_m;
    int i;

    fn_op(p_es_h, &out_m, out_a);

    for (i = 0 ; i < AO_ARRAY_SIZE ; i++)
        *p_failures += check_array(out_a[i]);
}

void in_cfmt_sub(p_es_h)
    idl_es_handle_t *p_es_h;
{
    idl_char *aptr;
    idl_long_int m;
    int i;

    aptr = (idl_char *)malloc(ARRAY_SIZE * sizeof(idl_char));
    for (i=0; i<ARRAY_SIZE; i++)
        aptr[i] = 'A' + i;
    m = ARRAY_SIZE-1;
    cfmt_op(p_es_h, &m, aptr);
    free(aptr);
}

void out_cfmt_sub(p_es_h, p_failures)
    idl_es_handle_t *p_es_h;
    int *p_failures;
{
    int i;
    idl_char *aptr;
    idl_long_int m;

    aptr = (idl_char *)malloc(ARRAY_SIZE * sizeof(idl_char));

    cfmt_op(p_es_h, &m, aptr);
    if (m != ARRAY_SIZE-1)
    {
        printf("cfmt size changed to %d\n", m);
        (*p_failures)++;
    }
    for (i=0; i<ARRAY_SIZE; i++)
    {
        if (aptr[i] != 'A' + i)
        {
            printf("cfmt_op - aptr[%d] = %c\n", i, aptr[i]);
            (*p_failures)++;
        }
    }
    free(aptr);
}


void in_open_sub(p_es_h)
    idl_es_handle_t *p_es_h;
{
    array_v1_string_t   a[AO_ARRAY_SIZE];    /* Array of strings */
    idl_long_int m, last;
    int i;

    for (i = 0 ; i < AO_ARRAY_SIZE ; i++)
        setup_array(a[i]);
    last = AO_ARRAY_SIZE-1;
    m = AO_ARRAY_SIZE-1;
    open_op(p_es_h, &m, &last, a);
}

void out_open_sub(p_es_h, p_failures)
    idl_es_handle_t *p_es_h;
    int *p_failures;
{
    array_v1_string_t   out_a[AO_ARRAY_SIZE];    /* Array of strings */
    idl_long_int out_m, last;
    int i;

    open_op(p_es_h, &out_m, &last, out_a);
    if (out_m != AO_ARRAY_SIZE-1)
    {
        printf("open_op returned m = %d\n", out_m);
        (*p_failures)++;
    }
    if (last != AO_ARRAY_SIZE-1)
    {
        printf("open_op returned last = %d\n", last);
        (*p_failures)++;
    }
    for (i = 0 ; i < AO_ARRAY_SIZE ; i++)
        *p_failures += check_array(out_a[i]);
}
