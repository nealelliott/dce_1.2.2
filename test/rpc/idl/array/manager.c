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
 * Revision 1.1.4.2  1996/02/17  23:12:17  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:34  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:55:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:27:59  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:02:29  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:25:11  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:23:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <array.h>
#include <test_common.h>

static ndr_long_int foo1
#ifdef NIDL_PROTOTYPES
(
    /* [in] */handle_t h,
    /* [in] */ndr_long_int arr[N]
)
#else
(h, arr)
    /* [in] */handle_t h;
    /* [in] */ndr_long_int arr[N];
#endif
{
    int i; 
    ndr_long_int checksum = 0;

    for (i = 0; i < N; i++)
        checksum += arr[i];
    
    return checksum;
}

static ndr_long_int foo2
#ifdef NIDL_PROTOTYPES
(
    /* [in] */handle_t h,
    /* [in] */s_t arr[N]
)
#else
(h, arr)
    /* [in] */handle_t h;
    /* [in] */s_t arr[N];
#endif
{
    int i; 
    ndr_long_int checksum = 0;

    for (i = 0; i < N; i++)
    {
        if (arr[i].b != (i & 0xff))
            printf("Byte value[%d] doesn't match in foo2()!\n", i);
        checksum += arr[i].l;
    }
    
    return checksum;
}

static ndr_long_int foo3
#ifdef NIDL_PROTOTYPES
(
    /* [in] */handle_t h,
    /* [in] */t_t arr[N]
)
#else
(h, arr)
    /* [in] */handle_t h;
    /* [in] */t_t arr[N];
#endif
{
    int i, j; 
    ndr_long_int checksum = 0;

    for (i = 0; i < N; i++)
    {
        if (arr[i].b != (i & 0xff))
            printf("Byte value[%d] doesn't match in foo3()!\n", i);
        for (j = 0; j < 7; j++)
            checksum += arr[i].a[j];
    }
    
    return checksum;
}

static ndr_long_int foo4
#ifdef NIDL_PROTOTYPES
(
    /* [in] */handle_t h,
    /* [in] */ndr_long_int arr[N],
    /* [in] */ndr_long_int last
)
#else
(h, arr, last)
    /* [in] */handle_t h;
    /* [in] */ndr_long_int arr[N];
    /* [in] */ndr_long_int last;
#endif
{
    int i; 
    ndr_long_int checksum = 0;

    for (i = 0; i <= last; i++)
        checksum += arr[i];
    
    return checksum;
}

static ndr_long_int foo5
#ifdef NIDL_PROTOTYPES
(
    /* [in] */handle_t h,
    /* [in] */my_array arr,
    /* [in] */ndr_long_int max
)
#else
(h, arr, max)
    /* [in] */handle_t h;
    /* [in] */my_array arr;
    /* [in] */ndr_long_int max;
#endif
{
    int i,j; 
    ndr_long_int checksum = 0;


    for (i = 0; i <= max; i++)
        for (j = 0; j < M; j++) 
        {
          checksum += arr[i][j];
#ifdef DEBUG
          printf("m[%d,%d]: %d,%d\n", i,j,arr[i][j], checksum);
#endif
        }    
    return checksum;
}

static void foo6
#ifdef NIDL_PROTOTYPES
(
    /* [in] */handle_t h,
    /* [in] */my_array arr,
    /* [in, out] */ndr_long_int *last,
    /* [in] */ndr_long_int max,
    /* [out] */ndr_long_int *checkin,
    /* [out] */ndr_long_int *checkout
)
#else
(h, arr, last, max, checkin, checkout)
    /* [in] */handle_t h;
    /* [in, out] */my_array arr;
    /* [in, out] */ndr_long_int *last;
    /* [in] */ndr_long_int max;
    /* [out] */ndr_long_int *checkin;
    /* [out] */ndr_long_int *checkout;
#endif
{
    int i,j; 

    *checkin = *checkout = 0;

    for (i = 0; i <= *last; i++)
        for (j = 0; j < M; j++) *checkin += arr[i][j];
    
    *last = max;

    for (i = 0; i <= *last; i++)
        for (j = 0; j < M; j++) *checkout += arr[i][j] = random();
}

globaldef array_v0_0_epv_t array_v0_0_m_epv = 
        {foo1, foo2, foo3, foo4, foo5, foo6};
