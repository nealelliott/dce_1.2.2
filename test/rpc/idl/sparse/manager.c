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
 * Revision 1.1.4.2  1996/02/17  23:17:20  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:31  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:29:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:46  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:36:20  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:43:29  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:29:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <sparse.h>
#include <test_common.h>

ndr_long_int compress_sum(h, array)
handle_t h;
compress_array array;
{
    int i;
    ndr_long_int sum = 0;

    for (i = 0; i < ARRAY_SIZE; i++)
        sum += array[i];

    return sum;
}

ndr_long_int nocompress_sum(h, array)
handle_t h;
nocompress_array array;
{
    int i;
    ndr_long_int sum = 0;

    for (i = 0; i < ARRAY_SIZE; i++)
        sum += array[i];

    return sum;
}

globaldef sparse_v0_0_epv_t sparse_v0_0_m_epv = {compress_sum, nocompress_sum};

