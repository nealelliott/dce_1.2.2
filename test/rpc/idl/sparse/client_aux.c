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
 * Revision 1.1.4.2  1996/02/17  23:17:17  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:29  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:29:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:45  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:36:15  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:43:25  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:29:17  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <sparse.h>
#include <stdio.h>
#include <test_common.h>

#define SPARSENESS 100   /* about one in every SPARSENESS values will be 
                            non-zero */
int client_aux(h)
handle_t h;
{
    int i, j, k, failures = 0;
    ndr_long_int last, max, checkin, checkout;
    char buf[100];
    ndr_long_int sum;
    compress_array carray;
    nocompress_array narray;
    int checksum;

    printf("Generating %d element sparse array...\n", ARRAY_SIZE);

    checksum = 0;
    for (k=0; k < ARRAY_SIZE; k++)
    {
        if (random()%SPARSENESS) carray[k] = narray[k] = 0;
        else carray[k] = narray[k] = (ndr_long_int)random()%1000;
        checksum += carray[k];
    }

    printf("Beginning non-compressing call\n");

    sum = sparse_nocompress_sum(h, narray);
    if (sum != checksum)     
    {
        printf("%d does not equal %d!\n", sum, checksum);
        failures++;
    }

    printf("Beginning compressing call\n");

    sum = sparse_compress_sum(h, carray);
    if (sum != checksum)
    {
        printf("%d does not equal %d!\n", sum, checksum);
        failures++;
    }

    return failures;
}
