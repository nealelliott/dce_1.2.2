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
 * Revision 1.1.4.2  1996/02/17  23:15:55  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:26  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:26:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:27  root]
 * 
 * Revision 1.1.2.3  1993/01/13  21:49:36  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:38:00  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:26:04  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <packed_byte_array.h>
#include <test_common.h>

#define COOKIE 0x8812685

ndr_long_int _fixed_op(h, ioup)
    handle_t        h;
    fixed_union_t   *ioup;
{
    int failures = 0;

    if (ioup->sw != GREEN
        ||  ioup->tagged_union.u.sw != 'l'
        ||  ioup->tagged_union.u.tagged_union.l != COOKIE)
    {
        fprintf(stderr, "Failure: Bad data in _fixed_op()\n");
        failures++;
    }

    ioup->tagged_union.u.sw = 'c';
    ioup->tagged_union.u.tagged_union.c = 'c';
    return failures;
}

ndr_long_int _vary_op(h, iup, ioup, oup)
    handle_t        h;
    i_v_union_t     *iup;
    io_v_union_t    *ioup;
    o_v_union_t     *oup;
{
    int failures = 0;
    int i, j;

    /*
     * Rather than validating the input data, just assume it is right.
     * Since the output data depends on the input data, the client
     * will catch any problems.
     */

    oup->sw = ioup->sw;

    switch (ioup->sw)
    {
    case 1:
        if (iup->sw != ndr_true
            ||  iup->tagged_union.svu.l != HALF_ELEM
            ||  ioup->tagged_union.sva.l != NUM_ELEM)
        {
            fprintf(stderr, "Failure: Bad union switch or array length in _vary_op()\n");
            failures++;
        }

        ioup->tagged_union.sva.l = HALF_ELEM;
        oup->tagged_union.svl.l = HALF_ELEM;

        for (i = 0 ; i < HALF_ELEM ; i++)
        {
            switch (i % 3)
            {
            case 0:
                oup->tagged_union.svl.al[i]
                    = iup->tagged_union.svu.au[i].tagged_union.l;
                break;

            case 1:
                oup->tagged_union.svl.al[i]
                    = iup->tagged_union.svu.au[i].tagged_union.s;
                break;

            case 2: 
                oup->tagged_union.svl.al[i]
                    = iup->tagged_union.svu.au[i].tagged_union.c;
                break;
            }

            for (j = 0 ; j < NUM_LETTERS ; j ++)
                ioup->tagged_union.sva.aa[i][j]
                    = ioup->tagged_union.sva.aa[i*2][j];
        }
        break;

    default:
        if (iup->sw != ndr_true
            ||  iup->tagged_union.svu.l != HALF_ELEM
            ||  ioup->tagged_union.svs.l != NUM_ELEM)
        {
            fprintf(stderr, "Failure: Bad union switch or array length in _vary_op()\n");
            failures++;
        }

        ioup->tagged_union.svs.l = HALF_ELEM;
        oup->tagged_union.svp.l = HALF_ELEM;

        for (i = 0 ; i < HALF_ELEM ; i++)
        {
            oup->tagged_union.svp.apl[i]
                = (long *)rpc_ss_allocate(sizeof(long));

            switch (i % 3)
            {
            case 0:
                *(oup->tagged_union.svp.apl[i])
                    = iup->tagged_union.svu.au[i].tagged_union.l;
                break;

            case 1:
                *(oup->tagged_union.svp.apl[i])
                    = iup->tagged_union.svu.au[i].tagged_union.s;
                break;

            case 2: 
                *(oup->tagged_union.svp.apl[i])
                    = iup->tagged_union.svu.au[i].tagged_union.c;
                break;
            }

            ioup->tagged_union.svs.as[i].f = ioup->tagged_union.svs.as[i*2].f;
            ioup->tagged_union.svs.as[i].l = ioup->tagged_union.svs.as[i*2].l;
        }
        break;
    }

    return failures;
}

globaldef packed_byte_array_v0_0_epv_t packed_byte_array_v0_0_m_epv = {
    _fixed_op,
    _vary_op
};
