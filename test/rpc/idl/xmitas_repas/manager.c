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
 * Revision 1.1.8.2  1996/02/17  23:18:48  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:37:29  marty]
 *
 * Revision 1.1.8.1  1995/12/11  19:32:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:17:02  root]
 * 
 * Revision 1.1.6.2  1993/06/15  21:10:16  sommerfeld
 * 	Add #include of <test_common.h>
 * 	[1993/06/14  21:37:38  sommerfeld]
 * 
 * Revision 1.1.2.2  1993/01/07  23:59:02  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:48:02  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:33:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <xmitas_repas.h>
#include <test_common.h>

ndr_long_int _xmitas_sum(h, structure)
handle_t h;
local_t structure;
{
    int i;
    ndr_long_int sum = 0;

    for (i = 0; i <= structure.last; i++) sum += structure.values[i];
    return sum;
}

ndr_long_int _xmitas_sum_in_out(h, structure, total)
handle_t h;
local_t *structure;
idl_long_int total;
{
    int i;
    int num_elems = (random() % (MAXELEMENTS-2)) + 1;
    ndr_long_int sum = 0;

    /* Check [in] value */
    for (i = 0; i <= structure->last; i++) sum += structure->values[i];
    if (total != sum) 
        printf("***FAILURE***: Bad input to xmitas_sum_in_out (total=%d,sum=%d)\n",total,sum);

    /* Generate a new value to return */
    structure->last = num_elems;
    sum = 0;
    for (i = 0; i <= num_elems; i++)
    {
        structure->values[i] = i * 77;
        sum += structure->values[i];
    }

    return sum;
}

globaldef xmitas_repas_v0_0_epv_t xmitas_repas_v0_0_m_epv = {
    _xmitas_sum,
    _xmitas_sum_in_out
    };
