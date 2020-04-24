/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: manager.c,v $
 * Revision 1.1.4.2  1996/02/17  23:13:30  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:19  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:01:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:29:54  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:51:47  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:31:16  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:24:23  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <cover_array.h>
#include <test_common.h>

ndr_long_int _io_double_op(h, double_arr)
    handle_t        h;
    ndr_long_float  double_arr[NUM_DOUBLE];
{
    int i;

    for (i = NUM_DOUBLE-1 ; i > 0 ; i--)
        double_arr[i] = double_arr[i] / double_arr[i-1];

    return 0;
}

ndr_long_int _io_array_op(h, array_arr, size)
    handle_t        h;
    array_t         array_arr[];
    ndr_long_int    *size;
{
    int i,j;
    int failures = 0;

    if (*size != SIZE_ARRAY_IN)
    {
        fprintf(stderr, "Failure in _io_array_op()\n");
        fprintf(stderr, "  expected size = %d\n", SIZE_ARRAY_IN);
        fprintf(stderr, "  actual size   = %d\n", *size);
        failures++;
    }

    /*
     * Munge all the values that were sent us, but return a smaller size.
     * The values between SIZE_ARRAY_OUT and SIZE_ARRAY_IN should be
     * changed here but not reflected back to the client.
     */
    for (i = 0 ; i < *size ; i++)
        for (j = 0 ; j < NUM_BYTE ; j++)
            array_arr[i][j] = array_arr[i][j] * 3;

    *size = SIZE_ARRAY_OUT;
    return failures;
}

ndr_long_int _i_c_union_op(h, union_arr, size)
    handle_t        h;
    union_t         union_arr[];
    ndr_long_int    size;
{
    int i,j;
    int failures = 0;

    if (size != SIZE_UNION)
    {
        fprintf(stderr, "Failure in _i_c_union_op()\n");
        fprintf(stderr, "  expected size = %d\n", SIZE_UNION);
        fprintf(stderr, "  actual size   = %d\n", size);
        failures++;
    }

    for (i = 0 ; i < SIZE_UNION ; i++)
    {
        j = i % 3;
        switch (j)
        {
        case 0:
            if (union_arr[i].sw != char_k
                ||  union_arr[i].tagged_union.c != i)
            {
                fprintf(stderr, "Failure in _i_c_union_op()\n");
                fprintf(stderr, "  union_arr[%d] = %c\n", i,
                        union_arr[i].tagged_union.c);
                fprintf(stderr, "  expected value was %c\n", i);
                return ++failures;
            }
            break;

        case 1:
            if (union_arr[i].sw != long_k
                ||  union_arr[i].tagged_union.l != i * 10)
            {
                fprintf(stderr, "Failure in _i_c_union_op()\n");
                fprintf(stderr, "  union_arr[%d] = %d\n", i,
                        union_arr[i].tagged_union.l);
                fprintf(stderr, "  expected value was %d\n", i * 10);
                return ++failures;
            }
            break;

        case 2:
            if (union_arr[i].sw != double_k
                ||  union_arr[i].tagged_union.d != i * 100)
            {
                fprintf(stderr, "Failure in _i_c_union_op()\n");
                fprintf(stderr, "  union_arr[%d] = %f\n", i,
                        union_arr[i].tagged_union.d);
                fprintf(stderr, "  expected value was %f\n", i * 100);
                return ++failures;
            }
            break;
        }
    }
    return failures;
}

static int passno = -1;
ndr_long_int _o_cv_union_op(h, union_arr, size, length)
    handle_t        h;
    union_t         union_arr[];
    ndr_long_int    size;
    ndr_long_int    *length;
{
    int i,j;
    int failures = 0;

    passno++;

    if (size != SIZE_UNION)
    {
        fprintf(stderr, "Failure in _o_cv_union_op()\n");
        fprintf(stderr, "  expected size = %d\n", SIZE_UNION);
        fprintf(stderr, "  actual size   = %d\n", size);
        failures++;
    }

    /*
     * The first time called, return a bogus length to test the
     * invalid bound exception logic.
     */
    if (passno == 0)
        *length = SIZE_UNION + 1;
    else
    {
        for (i = 0 ; i < DATA_LIM_UNION ; i++)
        {
            j = (i + 1) % 3;
            switch (j)
            {
            case 0:
                union_arr[i].sw = char_k;
                union_arr[i].tagged_union.c = i;
                break;

            case 1:
                union_arr[i].sw = long_k;
                union_arr[i].tagged_union.l = i * 50;
                break;

            case 2:
                union_arr[i].sw = double_k;
                union_arr[i].tagged_union.d = i * 500;
                break;
            }
        }
        *length = DATA_LIM_UNION;
    }

    return failures;
}

globaldef cover_array_v0_0_epv_t cover_array_v0_0_m_epv = {
    _io_double_op,
    _io_array_op,
    _i_c_union_op,
    _o_cv_union_op
};
