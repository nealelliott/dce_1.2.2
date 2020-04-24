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
 * Revision 1.1.4.2  1996/02/17  23:13:32  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:19  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:01:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:29:56  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:52:05  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:31:22  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:32:06  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <math.h>
#include <test_common.h>
#include <cover_array_ool.h>

#define COOKIE          0x74

int client_aux(h, passno)
    handle_t h;
    int passno;
{
    int             i, j;
    int             failures = 0, mgr_failures = 0;
    long            size, length;
    double          double_arr[NUM_DOUBLE];
    double          double_bmk[NUM_DOUBLE];
    array_t         array_arr[NUM_ARRAY];
    array_t         array_bmk[NUM_ARRAY];
    union_t         union_arr[NUM_UNION];
    union_t         union_bmk[NUM_UNION];

    /*
     * Test [in,out] array of double.
     */
    double_bmk[0] = double_arr[0] = rand();
    for (i = 1 ; i < NUM_DOUBLE ; i++)
    {
        double_arr[i] = rand();
        double_bmk[i] = double_arr[i] / double_arr[i-1];
    }

    mgr_failures += io_double_op(h, double_arr);
    if (mgr_failures != 0)
    {
        fprintf(stderr, "Failure in io_double_op manager routine\n");
        failures += mgr_failures;
    }

    for (i = 0 ; i < NUM_DOUBLE ; i++)
    {
        if (double_arr[i] != double_bmk[i])
        {
            fprintf(stderr, "Failure in io_double_op()\n");
            fprintf(stderr, "  double_arr[%d] = %f\n", i, double_arr[i]);
            fprintf(stderr, "  double_bmk[%d] = %f\n", i, double_bmk[i]);
            failures++;
            break;
        }
    }

    /*
     * Test [in,out] conformant array of arrays, with the [in] size
     * different from the [out] size.
     */
    for (i = 0 ; i < SIZE_ARRAY_OUT ; i++)
        for (j = 0 ; j < NUM_BYTE ; j++)
        {
            array_arr[i][j] = j - i;
            array_bmk[i][j] = array_arr[i][j] * 3;
        }

    for (i = SIZE_ARRAY_OUT ; i < SIZE_ARRAY_IN ; i++)
        for (j = 0 ; j < NUM_BYTE ; j++)
            array_bmk[i][j] = array_arr[i][j] = j - i;

    for (i = SIZE_ARRAY_IN ; i < NUM_ARRAY ; i++)
        for (j = 0 ; j < NUM_BYTE ; j++)
            array_bmk[i][j] = array_arr[i][j] = COOKIE;

    size = SIZE_ARRAY_IN;

    mgr_failures += io_array_op(h, array_arr, &size);
    if (mgr_failures != 0)
    {
        fprintf(stderr, "Failure in io_array_op manager routine\n");
        failures += mgr_failures;
    }

    if (size != SIZE_ARRAY_OUT)
    {
        fprintf(stderr, "Failure in io_array_op()\n");
        fprintf(stderr, "  expected size = %d\n", SIZE_ARRAY_OUT);
        fprintf(stderr, "  actual size   = %d\n", size);
    }
    else
        for (i = 0 ; i < NUM_ARRAY ; i++)
            for (j = 0 ; j < NUM_BYTE ; j++)
                if (array_arr[i][j] != array_bmk[i][j])
                {
                    fprintf(stderr, "Failure in io_array_op()\n");
                    fprintf(stderr, "  array_arr[%d][%d] = %d\n", i, j,
                            array_arr[i][j]);
                    fprintf(stderr, "  array_bmk[%d][%d] = %d\n", i, j,
                            array_bmk[i][j]);
                    failures++;
                    break;
                }

    /*
     * Test [in] conformant array of unions.
     */
    for (i = 0 ; i < SIZE_UNION ; i++)
    {
        j = i % 3;
        switch (j)
        {
        case 0:
            union_arr[i].sw = char_k;
            union_arr[i].tagged_union.c = i;
            break;

        case 1:
            union_arr[i].sw = long_k;
            union_bmk[i].tagged_union.l = union_arr[i].tagged_union.l = i * 10;
            break;

        case 2:
            union_arr[i].sw = double_k;
            union_bmk[i].tagged_union.d = union_arr[i].tagged_union.d = i * 100;
            break;
        }
    }

    mgr_failures += i_c_union_op(h, union_arr, SIZE_UNION);
    if (mgr_failures != 0)
    {
        fprintf(stderr, "Failure in i_c_union_op manager routine\n");
        failures += mgr_failures;
    }

    /*
     * Test [out] conformant varying array of unions.
     */
    for (i = DATA_LIM_UNION ; i < SIZE_UNION ; i++)
    {
        union_arr[i].sw = char_k;
        union_arr[i].tagged_union.c = COOKIE;
    }

    mgr_failures += o_cv_union_op(h, union_arr, SIZE_UNION, &length);
    if (mgr_failures != 0)
    {
        fprintf(stderr, "Failure in o_cv_union_op manager routine\n");
        failures += mgr_failures;
    }

    if (length != DATA_LIM_UNION)
    {
        fprintf(stderr, "Failure in o_cv_union_op()\n");
        fprintf(stderr, "  expected length = %d\n", DATA_LIM_UNION);
        fprintf(stderr, "  actual length   = %d\n", length);
    }

    for (i = 0 ; i < DATA_LIM_UNION ; i++)
    {
        j = (i + 1) % 3;
        switch (j)
        {
        case 0:
            if (union_arr[i].sw != char_k
                ||  union_arr[i].tagged_union.c != i)
            {
                fprintf(stderr, "Failure in o_cv_union_op()\n");
                fprintf(stderr, "  union_arr[%d] = %c\n", i,
                        union_arr[i].tagged_union.c);
                fprintf(stderr, "  expected value was %c\n", i);
                failures++;
            }
            break;

        case 1:
            if (union_arr[i].sw != long_k
                ||  union_arr[i].tagged_union.l != i * 50)
            {
                fprintf(stderr, "Failure in o_cv_union_op()\n");
                fprintf(stderr, "  union_arr[%d] = %d\n", i,
                        union_arr[i].tagged_union.l);
                fprintf(stderr, "  expected value was %d\n", i * 50);
                failures++;
            }
            break;

        case 2:
            if (union_arr[i].sw != double_k
                ||  union_arr[i].tagged_union.d != i * 500)
            {
                fprintf(stderr, "Failure in o_cv_union_op()\n");
                fprintf(stderr, "  union_arr[%d] = %f\n", i,
                        union_arr[i].tagged_union.d);
                fprintf(stderr, "  expected value was %f\n", i * 500);
                failures++;
            }
            break;
        }
    }

    for (i = DATA_LIM_UNION ; i < SIZE_UNION ; i++)
        if (union_arr[i].sw != char_k
            ||  union_arr[i].tagged_union.c != COOKIE)
        {
            fprintf(stderr, "Failure in o_cv_union_op()\n");
            fprintf(stderr, "  union_arr[%d] = %c\n", i,
                    union_arr[i].tagged_union.c);
            fprintf(stderr, "  expected value was %c\n", i);
            failures++;
            break;
        }

    if (failures != 0)
        fprintf(stderr, "%d failures.\n", failures);

    return failures;
}
