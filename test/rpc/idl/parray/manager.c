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
 * Revision 1.1.4.2  1996/02/17  23:15:58  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:30  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:26:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:30  root]
 * 
 * Revision 1.1.2.3  1993/01/13  21:49:38  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:38:13  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:26:09  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <parray.h>
#include <stdio.h>
#include <test_common.h>

static ndr_small_int mop1
#ifdef NIDL_PROTOTYPES
(
    handle_t h,
    a_t **arr,
    ndr_long_int checksum
)
#else
(h, arr, checksum)
    handle_t h;
    a_t **arr;
    ndr_long_int checksum;
#endif
{
    ndr_long_int check2 = 0;
    int i, j;

    for (i = 0; i < A; i++)
        for (j = 0; j < B; j++)
            check2 += (**arr)[i][j];

    if (check2 == checksum) 
    {
        return 0;
    }
    else 
    {
        printf("Checksum doesn't match! (%d != %d)\n", check2, checksum);
        return 1;
    }
}

static ndr_small_int mop2
#ifdef NIDL_PROTOTYPES
(
    handle_t h,
    a_t **arr,
    ndr_long_int *checksum
)
#else
(h, arr, checksum)
    handle_t h;
    a_t **arr;
    ndr_long_int *checksum;
#endif
{
    ndr_long_int check2 = 0;
    int i, j, failure = 0;
    a_t *arr2 = *arr;

    for (i = 0; i < A; i++)
        for (j = 0; j < B; j++)
            check2 += (**arr)[i][j];

    if (check2 == *checksum) ;
    else 
    {
        printf("Checksum doesn't match! (%d != %d)\n", check2, checksum);
        failure = 1;
    }

    /*srandom(time(0));*/

    *checksum = 0;
    for (i = 0; i < A; i++)
        for (j = 0; j < B; j++)
            *checksum += (*arr2)[i][j] = random();

    *arr = arr2;

    return failure;
}

static ndr_small_int mop3
#ifdef NIDL_PROTOTYPES
(
    handle_t h,
    a_t **arr,
    ndr_long_int *checksum
)
#else
(h, arr, checksum)
    handle_t h;
    a_t **arr;
    ndr_long_int *checksum;
#endif
{
    ndr_long_int check2 = 0;
    int i, j, failure = 0;
    a_t *arr2 = malloc(A*B*sizeof(ndr_long_int));


    for (i = 0; i < A; i++)
        for (j = 0; j < B; j++)
            check2 += (**arr)[i][j];

    if (check2 == *checksum) ;
    else 
    {
        printf("Checksum doesn't match! (%d != %d)\n", check2, checksum);
        failure = 1;
    }

    /*srandom(time(0));*/

    *checksum = 0;
    for (i = 0; i < A; i++)
        for (j = 0; j < B; j++)
            *checksum += (*arr2)[i][j] = random();

    *arr = arr2;

    return failure;
}

static ndr_small_int mop4
#ifdef NIDL_PROTOTYPES
(
    handle_t h,
    a_t **arr,
    ndr_long_int *checksum
)
#else
(h, arr, checksum)
    handle_t h;
    a_t **arr;
    ndr_long_int *checksum;
#endif
{
    int i, j, failure = 0;
    a_t *arr2 = malloc(A*B*sizeof(ndr_long_int));


    if (*arr == NULL) ;
    else 
    {
        printf("*arr is not NULL!\n");
        failure = 1;
    }

    if (*checksum != 0) 
    {
        printf("Checksum doesn't match! (0 != %d)\n", *checksum);
        failure = 1;
    }

    /*srandom(time(0));*/

    *checksum = 0;
    for (i = 0; i < A; i++)
        for (j = 0; j < B; j++)
            *checksum += (*arr2)[i][j] = random();

    *arr = arr2;

    return failure;
}

static ndr_small_int mop5
#ifdef NIDL_PROTOTYPES
(
    handle_t h,
    a_t **arr,
    ndr_long_int *checksum
)
#else
(h, arr, checksum)
    handle_t h;
    a_t **arr;
    ndr_long_int *checksum;
#endif
{
    ndr_long_int check2 = 0;
    int i, j, failure = 0;
    a_t *arr2 = NULL;


    for (i = 0; i < A; i++)
        for (j = 0; j < B; j++)
            check2 += (**arr)[i][j];

    if (check2 == *checksum) ;
    else 
    {
        printf("Checksum doesn't match! (%d != %d)\n", check2, checksum);
        failure = 1;
    }

    *checksum = 0;

    *arr = arr2;

    return failure;
}

static void mop6
#ifdef NIDL_PROTOTYPES
(
    handle_t h,
    a_t **arr,
    ndr_long_int *checksum
)
#else
(h, arr, checksum)
    handle_t h;
    a_t **arr;
    ndr_long_int *checksum;
#endif
{
    int i, j;
    a_t *arr2 = malloc(A*B*sizeof(ndr_long_int));

    
    *checksum = 0;
    for (i = 0; i < A; i++)
        for (j = 0; j < B; j++)
            *checksum += (*arr2)[i][j] = random();

    *arr = arr2;

}

globaldef parray_v0_0_epv_t parray_v0_0_m_epv = {mop1, mop2, mop3, mop4, mop5, mop6};
