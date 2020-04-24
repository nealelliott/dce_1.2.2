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
 * Revision 1.1.4.2  1996/02/17  23:12:43  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:48  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:57:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:28:37  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:07:00  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:27:19  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:23:38  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <arrayv1.h>
#include <test_common.h>

static idl_long_int gen_array(arr, last)
idl_long_int *arr;
idl_long_int last;
{
    int i;
    idl_long_int checksum = 0;

    for (i = 0; i <= last; i++)
        checksum += arr[i] = random();

    return checksum;
}

static idl_long_int check_array(arr, last)
idl_long_int *arr;
idl_long_int last;
{
    int i;
    idl_long_int checksum = 0;

    for (i = 0; i <= last; i++)
        checksum += arr[i];

    return checksum;
}

static idl_usmall_int op0(h, arr, checksum)
    /* [in] */ handle_t h;
    /* [in, out] */ idl_long_int arr[N];
    /* [in, out] */ idl_long_int *checksum;
{
    idl_usmall_int failures = 0;
    idl_long_int check2;

    if ((check2 = check_array(arr, N-1)) != *checksum)
    {
        printf("Checksums don't match (%d != %d)\n", check2, *checksum);
        failures++;
    }

    *checksum = gen_array(arr, N-1);
    return failures;
}

static idl_usmall_int op1(h, arr, l, checksum)
    /* [in] */ handle_t h;
    /* [in, out] */ idl_long_int arr[N];
    /* [in, out] */ idl_long_int *l;
    /* [in, out] */ idl_long_int *checksum;
{
    idl_usmall_int failures = 0;
    idl_long_int check2;

    if (*l == -1)
    {
        if (*checksum != 123456789)
        {
            printf("Bad checksum(%d)!\n", *checksum);
            failures++;
        }
        *checksum = 987654321;
    }
    else 
    {
        if ((check2 = check_array(arr, *l)) != *checksum)
        {
            printf("Checksums don't match (%d != %d)\n", check2, *checksum);
            failures++;
        }
        *l = random()%(N-1);
        *checksum = gen_array(arr, *l);
    }

    return failures;
}

static idl_usmall_int op2(h, arr, l, m, checksum)
    /* [in] */ handle_t h;
    /* [in, out] */ idl_long_int arr[];
    /* [in, out] */ idl_long_int *l;
    /* [in] */ idl_long_int m;
    /* [in, out] */ idl_long_int *checksum;
{
    idl_usmall_int failures = 0;
    idl_long_int check2;

    if (*l == -1)
    {
        if (*checksum != 123456789)
        {
            printf("Bad checksum(%d)!\n", *checksum);
            failures++;
        }
        *checksum = 987654321;
    }
    else 
    {
        if ((check2 = check_array(arr, *l)) != *checksum)
        {
            printf("Checksums don't match (%d != %d)\n", check2, *checksum);
            failures++;
        }
        *l = random()%(m);
        *checksum = gen_array(arr, *l);
    }

    return failures;
}

static idl_usmall_int op3(h, arr, l , checksum)
    /* [in] */ handle_t h;
    /* [in, out] */ idl_long_int arr[];
    /* [in, out] */ idl_long_int *l;
    /* [in, out] */ idl_long_int *checksum;
{
    idl_usmall_int failures = 0;
    idl_long_int check2;

    if (*l == -1)
    {
        if (*checksum != 123456789)
        {
            printf("Bad checksum(%d)!\n", *checksum);
            failures++;
        }
        *checksum = 987654321;
    }
    else 
    {
        if ((check2 = check_array(arr, *l)) != *checksum)
        {
            printf("Checksums don't match (%d != %d)\n", check2, *checksum);
            failures++;
        }
        *l = random()%(*l);
        *checksum = gen_array(arr, *l);
    }

    return failures;
}

globaldef arrayv1_v0_0_epv_t arrayv1_v0_0_m_epv = {op0, op1, op2, op3};
