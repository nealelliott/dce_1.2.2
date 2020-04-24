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
 * Revision 1.1.4.2  1996/02/17  23:12:29  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:41  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:56:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:28:19  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:04:36  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:26:15  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:23:14  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <arrayptr.h>
#include <test_common.h>

static void init_block (blockp, size, cs)
ndr_short_int **blockp;
ndr_short_int *size;
ndr_long_int *cs;
{
    int j;

    *size = random() % MAXSIZE;
    *blockp = (ndr_short_int *)malloc(*size*sizeof(ndr_short_int));

    *cs = 0;
    for (j = 0; j < *size; j++)
        *cs += *(*blockp + j) = (random()&0x7fff); /* Make sure result fits in a short*/
}

static void init_vblock (blockp, size, length, cs)
ndr_short_int **blockp;
ndr_short_int *size, *length;
ndr_long_int *cs;
{
    int j;

    *size = random() % MAXSIZE;
    *length = (*size == 0) ? 0 : random() % *size;
    *blockp = (ndr_short_int *)malloc(*size*sizeof(ndr_short_int));

    *cs = 0;
    for (j = 0; j < *length; j++)
        *cs += *(*blockp + j) = (random()&0x7fff);

}

static ndr_boolean check_block(blockp, size, cs)
ndr_short_int *blockp;
ndr_short_int size;
ndr_long_int cs;
{
    int j;

    for (j = 0; j < size; j++) cs -= *(blockp + j);
    return  (cs == 0);

}

client_aux(h)
rpc_binding_handle_t h;
{
    ndr_long_int j, checksum, check2;
    ndr_short_int length, size, *blockp;
    int failures = 0;


    printf("Calling arrayified_ref_iptr()\n");
    init_block (&blockp, &size, &checksum);
    if (!arrayified_ref_iptr (h, blockp, size, checksum)) {
        printf("!arrayified_ref_iptr ()\n");
        failures++;
    }

    printf("Calling arrayified_ref_optr()\n");
    arrayified_ref_optr (h, blockp, size, &checksum);
    check2 = 0;
    if (!check_block(blockp, size, checksum))
        failures++;
    free (blockp);

    printf("Calling arrayified_ref_ioptr()\n");
    init_block (&blockp, &size, &checksum);
    if (!arrayified_ref_ioptr (h, blockp, size, &checksum)) {
        printf("!arrayified_ref_ioptr ()\n");
        failures++;
    } else
        if (!check_block(blockp, size, checksum))
            failures++;
    free (blockp);

    printf("Calling arrayified_null_iptr()\n");
    if (!arrayified_null_iptr (h, NULL, 0, 0)) {
        printf("!arrayified_null_iptr ()\n");
        failures++;
    }

    printf("Calling arrayified_non_null_iptr()\n");
    init_block (&blockp, &size, &checksum);
    if (!arrayified_non_null_iptr (h, blockp, size, checksum)) {
        printf("!arrayified_non_null_iptr ()\n");
        failures++;
    }
    free (blockp);

    printf("Calling arrayified_null_ioptr()\n");
    if (!arrayified_null_ioptr (h, NULL, 0, &checksum)) {
        printf("!arrayified_null_ioptr ()\n");
        failures++;
    }

    printf("Calling arrayified_non_null_ioptr()\n");
    init_block (&blockp, &size, &checksum);
    if (!arrayified_non_null_ioptr (h, blockp, size, &checksum)) {
        printf("!arrayified_non_null_iptr ()\n");
        failures++;
    } else
        if (!check_block(blockp, size, checksum))
            failures++;
    free(blockp);

    printf("Calling varrayified_ref_iptr()\n");
    init_vblock (&blockp, &size, &length, &checksum);
    if (!varrayified_ref_iptr (h, blockp, length, size, checksum)) {
        printf("!varrayified_ref_iptr ()\n");
        failures++;
    }

    printf("Calling varrayified_ref_optr()\n");
    varrayified_ref_optr (h, blockp, &length, size, &checksum);
    check2 = 0;
    if (!check_block(blockp, length, checksum))
        failures++;
    free (blockp);

    printf("Calling varrayified_ref_ioptr()\n");
    init_vblock (&blockp, &size, &length, &checksum);
    if (!varrayified_ref_ioptr (h, blockp, &length, size, &checksum)) {
        printf("!varrayified_ref_ioptr ()\n");
        failures++;
    } else
        if (!check_block(blockp, length, checksum))
            failures++;
    free (blockp);

    printf("Calling varrayified_null_iptr()\n");
    if (!varrayified_null_iptr (h, NULL, 0, 0, 0)) {
        printf("!varrayified_null_iptr ()\n");
        failures++;
    }

    printf("Calling varrayified_non_null_iptr()\n");
    init_vblock (&blockp, &size, &length, &checksum);
    if (!varrayified_non_null_iptr (h, blockp, length, size, checksum)) {
        printf("!varrayified_non_null_iptr ()\n");
        failures++;
    }
    free (blockp);

    printf("Calling varrayified_null_ioptr()\n");
    if (!varrayified_null_ioptr (h, NULL, &length, 0, &checksum)) {
        printf("!varrayified_null_ioptr ()\n");
        failures++;
    }

    printf("Calling varrayified_non_null_ioptr()\n");
    init_vblock (&blockp, &size, &length, &checksum);
    if (!varrayified_non_null_ioptr (h, blockp, &length, size, &checksum)) {
        printf("!varrayified_non_null_iptr ()\n");
        failures++;
    } else
        if (!check_block(blockp, length, checksum))
            failures++;
    free(blockp);

    return(failures);
}


