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
 * Revision 1.1.4.2  1996/02/17  23:13:44  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:26  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:02:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:30:20  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:54:13  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:32:25  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:24:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <cover_struct.h>
#include <test_common.h>

#define JUNK 0x8812685

static ndr_boolean good_batch_of_cookies(a)
    array_t         a;
{
    int i;

    for (i = 0 ; i < NUM_ELEM ; i++)
        if (a[i] != i)
            return ndr_false;

    return ndr_true;
}

ndr_long_int _fixed_op(h, sp)
    handle_t        h;
    fixed_struct_t  *sp;
{
    int cookie = 999;
    int failures = 0;

    if (sp->d != cookie
        ||  sp->u.sw != ndr_false)
    {
        fprintf(stderr, "Failure in _fixed_op()\n");
        fprintf(stderr, "  Expected: %d\n", cookie);
        fprintf(stderr, "  Actual:   %d\n", sp->d);
        failures++;
    }

    sp->d *= sp->d;

    /* Flip around the union discriminator and return a value. */
    sp->u.sw = ndr_true;
    sp->u.tagged_union.l = cookie * cookie;

    return failures;
}

ndr_long_int _cfmt_op(h, sp)
    handle_t        h;
    cfmt_struct_t  *sp;
{
    int cookie = -888;
    int failures = 0;
    int i;

    if (sp->s != NUM_ELEM)
    {
        fprintf(stderr, "Failure: Bad array size in _cfmt_op()\n");
        failures++;
    }

    for (i = 0 ; i < NUM_ELEM ; i += 2)
        if (sp->au[i].sw != ndr_false)
        {
            fprintf(stderr, "Failure: Bad union discriminator in _cfmt_op()\n");
            failures++;
        }

    for (i = 1 ; i < NUM_ELEM ; i += 2)
        if (sp->au[i].sw != ndr_true
            ||  sp->au[i].tagged_union.l != cookie * i)
        {
            fprintf(stderr, "Failure: Bad union data in _cfmt_op()\n");
            failures++;
        }

    return failures;
}

ndr_long_int _vary_op(h, is, osp, iosp)
    handle_t        h;
    i_vary_struct_t is;
    o_vary_struct_t *osp;
    io_vary_struct_t *iosp;
{
    int cookie = 777, cookie1, cookie2;
    int failures = 0;
    int i;

    if (is.cl != NUM_ELEM
        ||  iosp->dl != NUM_ELEM
        ||  iosp->pl != NUM_ELEM)
    {
        fprintf(stderr, "Failure: Bad array length in _vary_op()\n");
        failures++;
    }

    for (i = 0 ; i < NUM_ELEM ; i++)
    {
        cookie1 = cookie * i;
        if (is.ac[i] != cookie1 % 10
            ||  iosp->ad[i] != cookie1
            ||  *iosp->ap[i] != cookie1)
        {
            fprintf(stderr, "Failure: Bad array data in _vary_op()\n");
            failures++;
            break;
        }
    }

    for (i = 0 ; i < DATA_LIM ; i++)
    {
        cookie1 = cookie * i;
        cookie2 = cookie1 * cookie1;
        osp->af[i] = cookie2;
        osp->as[i].d = cookie2;
        osp->as[i].u.sw = ndr_true;
        osp->as[i].u.tagged_union.l = cookie1;
        iosp->ad[i] = cookie2;
        *iosp->ap[i] = cookie2;
    }

    for (i = DATA_LIM ; i < NUM_ELEM ; i++)
    {
        cookie1 = cookie * i;
        cookie2 = cookie1 * cookie1;
        osp->af[i] = JUNK;
        osp->as[i].d = cookie1;
        osp->as[i].u.sw = ndr_false;
        iosp->ad[i] = JUNK;
        *iosp->ap[i] = cookie2;
    }

    osp->fl = DATA_LIM;
    osp->sl = NUM_ELEM;
    iosp->dl = DATA_LIM;

    return failures;
}

ndr_long_int _cv_op(h, isp, ospp, iosp)
    handle_t        h;
    i_cv_struct_t   *isp;
    o_cv_struct_t   **ospp;
    io_cv_struct_t  *iosp;
{
    int cookie = -666, cookie1, cookie2;
    int failures = 0;
    int i,j;

    /* Allocate storage for return struct (pointed-to by full pointer). */
    *ospp = (o_cv_struct_t *)rpc_ss_allocate(sizeof(o_cv_struct_t)
                                + (NUM_ELEM-1) * sizeof(long *));

    if (isp->s != NUM_ELEM
        ||  isp->l != DATA_LIM
        ||  iosp->s != NUM_ELEM
        ||  iosp->l != DATA_LIM)
    {
        fprintf(stderr, "Failure: Bad array size or length in _cv_op()\n");
        failures++;
    }

    for (i = 0 ; i < DATA_LIM ; i++)
    {
        cookie1 = cookie * i;
        if (isp->as[i].d != cookie1
            ||  isp->as[i].u.sw != ndr_true
            ||  isp->as[i].u.tagged_union.l != cookie1
            ||  !good_batch_of_cookies(iosp->aa[i]))
        {
            fprintf(stderr, "Failure: Bad array data in _cv_op()\n");
            failures++;
            break;
        }
    }

    for (i = 0 ; i < DATA_LIM ; i++)
    {
        long *lp;

        cookie1 = cookie * i;
        cookie2 = cookie1 * cookie1;
        isp->as[i].d = cookie2;
        isp->as[i].u.tagged_union.l = cookie2;
        lp = (*ospp)->ap[i] = (long *)rpc_ss_allocate(sizeof(long));
        *lp = cookie1;
        for (j = 0 ; j < NUM_ELEM ; j++)
            iosp->aa[i][j] = j * j;
    }

    for (i = DATA_LIM ; i < NUM_ELEM ; i++)
    {
        cookie1 = cookie * i;
        isp->as[i].d = JUNK;
        isp->as[i].u.tagged_union.l = JUNK;
        (*ospp)->ap[i] = NULL;
        for (j = 0 ; j < NUM_ELEM ; j++)
            iosp->aa[i][j] = j * j;
    }

    iosp->l = NUM_ELEM;
    (*ospp)->s = NUM_ELEM;
    (*ospp)->l = DATA_LIM;

    return failures;
}

globaldef cover_struct_v0_0_epv_t cover_struct_v0_0_m_epv = {
    _fixed_op,
    _cfmt_op,
    _vary_op,
    _cv_op
};
