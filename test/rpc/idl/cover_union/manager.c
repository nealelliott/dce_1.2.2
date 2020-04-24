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
 * Revision 1.1.4.2  1996/02/17  23:13:49  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:30  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:03:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:30:34  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:55:29  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:32:57  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:32:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <cover_union.h>
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

idl_long_int _il_ool_op(h, up)
      /* [in] */ handle_t h;
      /* [in, out] */ il_union_t *up;
{
    int failures = 0;
    idl_short_int temp;

    switch(up->sw)
    {
    case 0:
        if (up->tagged_union.ios.c != COOKIE % 128
            ||  up->tagged_union.ios.l != COOKIE)
        {
            fprintf(stderr, "Failure: Bad data in _il_ool_op() up->ios\n");
            failures++;
        }
        temp = up->tagged_union.ios.os.sh;
        up->sw = 1;
        up->tagged_union.os.sh = temp;
        break;
    case 1:
        temp = up->tagged_union.os.sh;
        up->sw = 0;
        up->tagged_union.ios.c = COOKIE % 128;
        up->tagged_union.ios.os.sh = temp;
        up->tagged_union.ios.l = COOKIE;
        break;
    default:
        fprintf(stderr, "Failure: Bad discriminant in _il_ool_op()\n");
        failures++;
    }

    return failures;
}

long _end_op(h, p_ou)
handle_t h;
outer_union_t *p_ou;
{
    int failures = 0;

    switch(p_ou->swo)
    {
    case 0:
        if (p_ou->tagged_union.c != 'C') failures++;
        p_ou->swo = 1;
        p_ou->tagged_union.eu.swi = 0;
        p_ou->tagged_union.eu.tagged_union.eo.c = 'D';
        p_ou->tagged_union.eu.tagged_union.eo.os1.sh = 16;
        p_ou->tagged_union.eu.tagged_union.eo.l = COOKIE;
        p_ou->tagged_union.eu.tagged_union.eo.os2.sh = 256;
        break;
    case 1:
        {
            switch(p_ou->tagged_union.eu.swi)
            {
            case 0:
                if (p_ou->tagged_union.eu.tagged_union.eo.c !=
                        p_ou->tagged_union.eu.tagged_union.eo.l % 128) failures++;
                if (p_ou->tagged_union.eu.tagged_union.eo.os1.sh !=
                        p_ou->tagged_union.eu.tagged_union.eo.l % 32768) failures++;
                if (p_ou->tagged_union.eu.tagged_union.eo.os2.sh !=
                        -(p_ou->tagged_union.eu.tagged_union.eo.l % 32768)) failures++;
                p_ou->tagged_union.eu.tagged_union.eo.c /= 2;
                p_ou->tagged_union.eu.tagged_union.eo.os1.sh /= 3;
                p_ou->tagged_union.eu.tagged_union.eo.l /= 4;
                p_ou->tagged_union.eu.tagged_union.eo.os2.sh /= 5;
                p_ou->tagged_union.eu.swi = 1;
                p_ou->tagged_union.eu.tagged_union.ei.ll
                    = -p_ou->tagged_union.eu.tagged_union.ei.l;
                break;
            case 1:
                if (p_ou->tagged_union.eu.tagged_union.ei.c !=
                        p_ou->tagged_union.eu.tagged_union.ei.l % 128) failures++;
                if (p_ou->tagged_union.eu.tagged_union.ei.os1.sh !=
                        p_ou->tagged_union.eu.tagged_union.ei.l % 32768) failures++;
                if (p_ou->tagged_union.eu.tagged_union.ei.os2.sh !=
                        -(p_ou->tagged_union.eu.tagged_union.ei.l % 32768)) failures++;
                if (p_ou->tagged_union.eu.tagged_union.ei.ll !=
                       -(p_ou->tagged_union.eu.tagged_union.ei.l)) failures++;
                p_ou->swo = 0;
                p_ou->tagged_union.c = 'E';
                break;
            default:
                fprintf(stderr, "Failure: Bad inner discriminant in _end_op()\n");
                failures++;
            }
        }
        break;            
    default:
        fprintf(stderr, "Failure: Bad outer discriminant in _end_op()\n");
        failures++;
    }

    return failures;
}

long _end_arr_op(h, ou_a)
handle_t h;
outer_union_t ou_a[3];
{
    int failures = 0;

    if (ou_a[0].swo != 0)
        failures++;
    else
    {
        if (ou_a[0].tagged_union.c != 'C') failures++;
        ou_a[0].swo = 1;
        ou_a[0].tagged_union.eu.swi = 0;
        ou_a[0].tagged_union.eu.tagged_union.eo.c = 'D';
        ou_a[0].tagged_union.eu.tagged_union.eo.os1.sh = 16;
        ou_a[0].tagged_union.eu.tagged_union.eo.l = COOKIE;
        ou_a[0].tagged_union.eu.tagged_union.eo.os2.sh = 256;
    }
    if (ou_a[1].swo != 1)
        failures++;
    else 
    {
        if (ou_a[1].tagged_union.eu.swi != 0)
            failures++;
        else
            {
                if (ou_a[1].tagged_union.eu.tagged_union.eo.c !=
                        ou_a[1].tagged_union.eu.tagged_union.eo.l % 128) failures++;
                if (ou_a[1].tagged_union.eu.tagged_union.eo.os1.sh !=
                        ou_a[1].tagged_union.eu.tagged_union.eo.l % 32768) failures++;
                if (ou_a[1].tagged_union.eu.tagged_union.eo.os2.sh !=
                        -(ou_a[1].tagged_union.eu.tagged_union.eo.l % 32768)) failures++;
                ou_a[1].tagged_union.eu.tagged_union.eo.c /= 2;
                ou_a[1].tagged_union.eu.tagged_union.eo.os1.sh /= 3;
                ou_a[1].tagged_union.eu.tagged_union.eo.l /= 4;
                ou_a[1].tagged_union.eu.tagged_union.eo.os2.sh /= 5;
                ou_a[1].tagged_union.eu.swi = 1;
                ou_a[1].tagged_union.eu.tagged_union.ei.ll
                    = -ou_a[1].tagged_union.eu.tagged_union.ei.l;
            }
    }
    if (ou_a[2].swo != 1)
        failures++;
    else 
    {
        if (ou_a[2].tagged_union.eu.swi != 1)
            failures++;
        else
            {
                if (ou_a[2].tagged_union.eu.tagged_union.ei.c !=
                        ou_a[2].tagged_union.eu.tagged_union.ei.l % 128) failures++;
                if (ou_a[2].tagged_union.eu.tagged_union.ei.os1.sh !=
                        ou_a[2].tagged_union.eu.tagged_union.ei.l % 32768) failures++;
                if (ou_a[2].tagged_union.eu.tagged_union.ei.os2.sh !=
                        -(ou_a[2].tagged_union.eu.tagged_union.ei.l % 32768)) failures++;
                if (ou_a[2].tagged_union.eu.tagged_union.ei.ll !=
                       -(ou_a[2].tagged_union.eu.tagged_union.ei.l)) failures++;
                ou_a[2].swo = 0;
                ou_a[2].tagged_union.c = 'E';
            }
    }

    return failures;
}

globaldef cover_union_v0_0_epv_t cover_union_v0_0_m_epv = {
    _fixed_op,
    _vary_op,
    _il_ool_op,
    _end_op,
    _end_arr_op
};
