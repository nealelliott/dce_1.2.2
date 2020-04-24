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
 * Revision 1.1.4.2  1996/02/17  23:12:19  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:34  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:55:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:28:01  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:02:57  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:25:21  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:23:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <array2.h>
#include <test_common.h>

#define MAXSIZE 25
#define EMPTINESS 10

int client_aux(h)
    handle_t h;
{
    int i, j, k, failures = 0;
    ndr_long_int size, checksum, check2;
    ndr_usmall_int tmp;
    ref_t *p;

    size = random()%MAXSIZE + 1;

    p = (ref_t *)malloc(size*sizeof(ref_t));

    checksum = 0;
    for (i = 0; i < size; i++)
    {
        p[i].s = random()%MAXSIZE + 1;
        p[i].array = (leaf_t *)malloc(p[i].s*sizeof(leaf_t));

        for (j = 0; j < p[i].s; j++)
        {
            p[i].array[j].s = random()%MAXSIZE + 1;
            p[i].array[j].l = p[i].array[j].s ? random()%p[i].array[j].s : 0;

            if (random()%EMPTINESS)
            {
                p[i].array[j].array_is_null = ndr_false;
                p[i].array[j].array = (ndr_short_int *)
                    malloc(p[i].array[j].s*sizeof(ndr_short_int));
                for (k = 0; k < p[i].array[j].l; k++)
                    checksum += p[i].array[j].array[k] = (random()&0x7fff);
            }
            else 
            {
                p[i].array[j].array_is_null = ndr_true;
                p[i].array[j].array = NULL;
            }
        }
    }

    printf("Calling op0()\n");

    if (tmp = op0(h, p, size, &checksum))
    {
        printf("op0 failed!\n");
        failures += tmp;
    }

    check2 = 0;
    for (i = 0; i < size; i++)
        for (j = 0; j < p[i].s; j++)
            if (!p[i].array[j].array_is_null)
                for (k = 0; k < p[i].array[j].l; k++)
                    check2 += p[i].array[j].array[k];

    if (check2 != checksum)
    {
        printf("Checksums don't match!\n");
        failures++;
    }

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < p[i].s; j++)
            if (!(p[i].array[j].array_is_null)) free(p[i].array[j].array);
        free(p[i].array);
    }
    free(p);

    return failures;
}
