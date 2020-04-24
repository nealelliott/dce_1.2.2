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
 * Revision 1.1.4.2  1996/02/17  23:16:32  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:54  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:28:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:02  root]
 * 
 * Revision 1.1.2.2  1993/01/07  20:43:06  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:40:35  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:26:43  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <pointed.h>
#include <test_common.h>

int client_aux(h,passes)
      handle_t h;
      int passes;
{
    error_status_t st;
    this_way_t this_way;
    that_way_t that_way;

    this_way.p_boolean = (ndr_boolean *)malloc(sizeof(ndr_boolean));
    *this_way.p_boolean = true;
    this_way.p_byte = (ndr_byte *)malloc(sizeof(ndr_byte));
    *this_way.p_byte = 'A';
    this_way.p_char = (ndr_char *)malloc(sizeof(ndr_char));
    *this_way.p_char = 'Z';
    this_way.p_small = (ndr_small_int *)malloc(sizeof(ndr_small_int));
    *this_way.p_small = 0x11;
    this_way.p_short = (ndr_short_int *)malloc(sizeof(ndr_short_int));
    *this_way.p_short = 0x2222;
    this_way.p_long = (ndr_long_int *)malloc(sizeof(ndr_long_int));
    *this_way.p_long = 0x33333333;
    this_way.p_hyper = (ndr_hyper_int *)malloc(sizeof(ndr_hyper_int));
    (*this_way.p_hyper).high = 0x44444444;
    (*this_way.p_hyper).low = 0x55555555;
    this_way.p_usmall = (ndr_usmall_int *)malloc(sizeof(ndr_usmall_int));
    *this_way.p_usmall = 0x66;
    this_way.p_ushort = (ndr_ushort_int *)malloc(sizeof(ndr_ushort_int));
    *this_way.p_ushort = 0x7777;
    this_way.p_ulong = (ndr_ulong_int *)malloc(sizeof(ndr_ulong_int));
    *this_way.p_ulong = 0x88888888;
    this_way.p_uhyper = (ndr_uhyper_int *)malloc(sizeof(ndr_uhyper_int));
    (*this_way.p_uhyper).high = 0x99999999;
    (*this_way.p_uhyper).low = 0xAAAAAAAA;
    this_way.p_float = (ndr_short_float *)malloc(sizeof(ndr_short_float));
    *this_way.p_float = 1.2345;
    this_way.p_double = (ndr_long_float *)malloc(sizeof(ndr_long_float));
    *this_way.p_double = 0.9876;

    that_way.p_boolean = NULL;
    that_way.p_byte = NULL;
    that_way.p_char = NULL;
    that_way.p_small = NULL;
    that_way.p_short = NULL;
    that_way.p_long = NULL;
    that_way.p_hyper = NULL;
    that_way.p_usmall = NULL;
    that_way.p_ushort = NULL;
    that_way.p_ulong = NULL;
    that_way.p_uhyper = NULL;
    that_way.p_float = NULL;
    that_way.p_double = NULL;

   new_oper ( h, &this_way, &that_way );

    if (*this_way.p_boolean != *that_way.p_boolean)
    {
        printf("Error in p_boolean\n");
        exit(0);
    }
    if (*this_way.p_byte != *that_way.p_byte)
    {
        printf("Error in p_byte\n");
        exit(0);
    }
    if (*this_way.p_char != *that_way.p_char)
    {
        printf("Error in p_char\n");
        exit(0);
    }
    if (*this_way.p_small != *that_way.p_small)
    {
        printf("Error in p_small\n");
        exit(0);
    }
    if (*this_way.p_short != *that_way.p_short)
    {
        printf("Error in p_short\n");
        exit(0);
    }
    if (*this_way.p_long != *that_way.p_long)
    {
        printf("Error in p_long\n");
        exit(0);
    }
    if ((*this_way.p_hyper).low != (*that_way.p_hyper).low)
    {
        printf("Error in p_hyper.low\n");
        exit(0);
    }
    if ((*this_way.p_hyper).high != (*that_way.p_hyper).high)
    {
        printf("Error in p_hyper.high\n");
        exit(0);
    }
    if (*this_way.p_usmall != *that_way.p_usmall)
    {
        printf("Error in p_usmall\n");
        exit(0);
    }
    if (*this_way.p_ushort != *that_way.p_ushort)
    {
        printf("Error in p_ushort\n");
        exit(0);
    }
    if (*this_way.p_ulong != *that_way.p_ulong)
    {
        printf("Error in p_ulong\n");
        exit(0);
    }
    if ((*this_way.p_uhyper).low != (*that_way.p_uhyper).low)
    {
        printf("Error in p_uhyper.low\n");
        exit(0);
    }
    if ((*this_way.p_uhyper).high != (*that_way.p_uhyper).high)
    {
        printf("Error in p_uhyper.high\n");
        exit(0);
    }
    if (*this_way.p_float != *that_way.p_float)
    {
        printf("Error in p_float\n");
        exit(0);
    }
    if (*this_way.p_double != *that_way.p_double)
    {
        printf("Error in p_double\n");
        exit(0);
    }
 
    printf("Test successful\n");
    return 0;
}
