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
 * Revision 1.1.4.2  1996/02/17  23:16:39  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:01  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:28:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:09  root]
 * 
 * Revision 1.1.2.2  1993/01/07  20:44:04  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:41:06  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:26:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <ptrarray.h>
#include <test_common.h>

ndr_short_int fixed_non_null_array[NELEMENTS];
ndr_short_int *conf_non_null_array;

client_aux(h)
rpc_binding_handle_t h;
{
    ndr_long_int j, length, size, checksum, check2;
    char buf[100];
    int tmp, failures = 0;

    checksum = 0;
    for (j = 0; j < NELEMENTS; j++)
        checksum += fixed_non_null_array[j] = (random()&0x7fff);

    printf("Calling null_in_array()\n");
    if (!null_in_array (h, NULL)) {
        printf("!null_in_array (h, NULL)\n");
        failures++;
    }

    printf("Calling non_null_in_array()\n");
    if (!non_null_in_array (h, fixed_non_null_array, checksum)) {
        printf("!non_null_in_array (h, fixed_non_null_array)\n");
        failures++;
    }

    printf("Calling null_in_out_array()\n");
    if (!null_in_out_array (h, NULL)) {
        printf("!null_in_out_array (h, NULL)\n");
        failures++;
    }

    printf("Calling non_null_in_out_array()\n");
    if (!non_null_in_out_array (h, fixed_non_null_array, &checksum)) {
        printf("!non_null_in_out_array (h, fixed_non_null_array)\n");
        failures++;
    }
    else {
        check2 = 0;
        for (j = 0; j < NELEMENTS; j++) check2 += fixed_non_null_array[j];
 
        if (check2 != checksum) {
            printf("after non_null_in_out_array: check2 != checksum\n");
            failures++;
        }
    }

    length = random()%NELEMENTS;

    checksum = 0;
    for (j = 0; j < length; j++) 
          checksum += fixed_non_null_array[j] = (random()&0x7fff);

#ifdef DEBUG
    printf("Calling varying_in_out_array(length = %d)\n", length);
#endif
    if (!varying_in_out_array(h, fixed_non_null_array, &length, &checksum)) {
        printf("!varying_in_out()\n");
        failures++;
    }
    else {
        check2 = 0;
        for (j = 0; j < length; j++) check2 += fixed_non_null_array[j];
        if (check2 != checksum) {
            printf("after varying_in_out_array: check2 != checksum\n");
            failures++;
        }
    }

    size = random()%(2*NELEMENTS);
    conf_non_null_array = (ndr_short_int *)malloc(size*sizeof(ndr_short_int));

    checksum = 0;
    for (j = 0; j < size; j++) checksum += conf_non_null_array[j] = (random()&0x7fff);

#ifdef DEBUG
    printf("Calling conformant_in_out_array(size = %d)\n", size);
#endif
    if (!conformant_in_out_array(h, conf_non_null_array, size, &checksum)) {
        printf("!conformant_in_out()\n");
        failures++;
    }
    else {
        check2 = 0;
        for (j = 0; j < size; j++) check2 += conf_non_null_array[j];
        if (check2 != checksum) {
            printf("after conformant_in_out_array: check2 != checksum\n");
            failures++;
        }
    }

    free(conf_non_null_array);

    size = random()%(2*NELEMENTS);
    conf_non_null_array = (ndr_short_int *)malloc(size*sizeof(ndr_short_int));
    length = random()%size;

    checksum = 0;
    for (j = 0; j < length; j++) checksum += conf_non_null_array[j] = (random()&0x7fff);

#ifdef DEBUG
    printf("Calling conformant_varying_in_out_array(length = %d, size = %d)\n", 
           length, size);
#endif
    if (!conformant_varying_in_out_array(h, conf_non_null_array, &length, size, 
        &checksum)) {
        printf("!conformant_in_out_array()\n");
        failures++;
    }

    else {
        check2 = 0;
        for (j = 0; j < length; j++) check2 += conf_non_null_array[j];
        if (check2 != checksum) {
            printf("after conformant_varying_in_out_array: check2 != checksum\n");
            failures++;
        }
    }

    free(conf_non_null_array);

    return(failures);
}
