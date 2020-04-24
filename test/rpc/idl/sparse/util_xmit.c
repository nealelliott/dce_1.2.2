/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: util_xmit.c,v $
 * Revision 1.1.6.2  1996/02/17  23:17:22  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:33  marty]
 *
 * Revision 1.1.6.1  1995/12/11  19:29:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:47  root]
 * 
 * Revision 1.1.4.2  1993/01/07  23:36:29  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:43:36  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/08  20:07:16  harrow
 * 	The test was compensating for the stub passing an extra indirection for
 * 	an array parameter with the [transmit_as] attribute.  Remove the special
 * 	case code.
 * 	[1992/05/05  14:10:48  harrow]
 * 
 * Revision 1.1  1992/01/19  03:29:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <sparse.h>

void compress_array_to_xmit(array_param, xmit_struct)
compress_array *array_param; 
compress_t **xmit_struct;
{
    int rep, pos = 0;
    ndr_long_int val, index = 0;

#   define hack (*array_param)

    /* allocate the structure */
    *xmit_struct = (compress_t *)malloc(sizeof(compress_t));

    /* run-length encode the array */
    do
    {
        rep = 0;
        val = hack[pos];
        while (pos < ARRAY_SIZE && hack[pos] == val) 
        {
            pos++;
            rep++;
        }
        (*xmit_struct)->data[index] = rep;
        (*xmit_struct)->data[index+1] = val;
        index += 2;
    } while (pos < ARRAY_SIZE);

    (*xmit_struct)->last = index-1;
}

void compress_array_from_xmit(xmit_struct, array)
compress_t *xmit_struct;
compress_array *array;
{
    int index, rep, count = 0;

    for (index = 0; index < xmit_struct->last; index+=2)
        for (rep = 0; rep < xmit_struct->data[index]; rep++)
            (*array)[count++] = xmit_struct->data[index+1];
}

void compress_array_free_inst(object)
compress_array *object;
{
    /* no freeing is appropriate here */
}

void compress_array_free_xmit(xmit_struct)
compress_t *xmit_struct;
{
    free(xmit_struct);
}
