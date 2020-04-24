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
 * Revision 1.1.4.2  1996/02/17  23:16:41  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:03  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:28:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:10  root]
 * 
 * Revision 1.1.2.2  1993/01/07  20:44:13  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:41:10  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:26:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <ptrarray.h>
#include <test_common.h>

static ndr_boolean null_in_array (h, a)
handle_t h;
ndr_short_int a[NELEMENTS];
{
    ndr_boolean answer;

    answer = (a == NULL);
    return answer;
}

static ndr_boolean non_null_in_array (h, a, checksum)
handle_t h;
ndr_short_int a[NELEMENTS];
ndr_long_int checksum;
{
    ndr_boolean answer;
    ndr_long_int check2 = 0;
    int i;

    if (!(answer = (a != NULL))) 
       ;
    else
    {
        for (i = 0; i < NELEMENTS; i++) check2 += a[i];
        if (answer = (check2 == checksum)) ;
        else printf("doesn't match\n");
    }

    return answer;
}

static ndr_boolean null_in_out_array (h, a)
handle_t h;
ndr_short_int a[NELEMENTS];
{
    ndr_boolean answer;

    answer = (a == NULL);
    return answer;
}

static ndr_boolean non_null_in_out_array  (h, a, checksum)
handle_t h;
ndr_short_int a[NELEMENTS];
ndr_long_int *checksum;
{
    ndr_boolean answer;
    ndr_long_int check2 = 0;
    int i;

    if (!(answer = (a != NULL))) 
        ;
    else
    {
        for (i = 0; i < NELEMENTS; i++) check2 += a[i];
        if (answer = (check2 == *checksum)) ;
        else printf("doesn't match\n");
       
        *checksum = 0;
        for (i = 0; i < NELEMENTS; i++) *checksum += a[i] = (random()&0x7fff);
    }

    return answer;
}

static ndr_boolean varying_in_out_array(h, a, length, checksum)
handle_t h;
ndr_short_int a[NELEMENTS];
ndr_long_int *length;
ndr_long_int *checksum;
{
    ndr_boolean answer;
    ndr_long_int check2 = 0;
    int i;

    if (!(answer = (a != NULL))) 
        ;
    else
    {
        for (i = 0; i < *length; i++) check2 += a[i];
        if (answer = (check2 == *checksum)) ;
        else printf("doesn't match\n");
       
        *length = random()%NELEMENTS;
        *checksum = 0;
        for (i = 0; i < *length; i++) *checksum += a[i] = (random()&0x7fff);
    }

    return answer;
}

static ndr_boolean conformant_in_out_array(h, a, size, checksum)
handle_t h;
ndr_short_int a[];
ndr_long_int size;
ndr_long_int *checksum;
{
    ndr_boolean answer;
    ndr_long_int check2 = 0;
    int i;

    if (!(answer = (a != NULL))) 
        ;
    else
    {
        for (i = 0; i < size; i++) check2 += a[i];
        if (answer = (check2 == *checksum)) ;
        else printf("doesn't match\n");
       
        *checksum = 0;
        for (i = 0; i < size; i++) *checksum += a[i] = (random()&0x7fff);
    }

    return answer;
}

static ndr_boolean conformant_varying_in_out_array(h, a, length, size, checksum)
handle_t h;
ndr_short_int a[];
ndr_long_int *length;
ndr_long_int size;
ndr_long_int *checksum;
{
    ndr_boolean answer;
    ndr_long_int check2 = 0;
    int i;

    if (!(answer = (a != NULL))) 
        ;
    else
    {
        for (i = 0; i < *length; i++) check2 += a[i];
        if (answer = (check2 == *checksum)) ;
        else printf("doesn't match\n");
       
        *length = random()%size;
        *checksum = 0;
        for (i = 0; i < *length; i++) *checksum += a[i] = (random()&0x7fff);
    }

    return answer;
}

globaldef ptrarray_v0_0_epv_t ptrarray_v0_0_m_epv = {
    null_in_array,
    non_null_in_array,
    null_in_out_array,
    non_null_in_out_array,
    varying_in_out_array,
    conformant_in_out_array,
    conformant_varying_in_out_array
};

