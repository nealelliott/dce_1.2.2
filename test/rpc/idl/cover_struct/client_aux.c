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
 * Revision 1.1.4.2  1996/02/17  23:13:41  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:25  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:02:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:30:17  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:53:58  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:32:18  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:24:28  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <math.h>
#include <test_common.h>
#include <cover_struct.h>

#define JUNK 0x5862188

static ndr_boolean good_batch_of_cookies(a)
    array_t         a;
{
    int i;

    for (i = 0 ; i < NUM_ELEM ; i++)
        if (a[i] != (i * i))
            return ndr_false;

    return ndr_true;
}

int client_aux(h, passno)
    handle_t h;
    int passno;
{
    int             i, j, cookie, cookie1, cookie2;
    int             failures = 0, mgr_failures;
    long            size, length;
    fixed_struct_t  fixed_struct;
    cfmt_struct_t   *cfmt_struct_p;
    i_vary_struct_t i_vary_struct;
    o_vary_struct_t o_vary_struct;
    io_vary_struct_t io_vary_struct;
    i_cv_struct_t   *i_cv_struct_p;
    o_cv_struct_t   *o_cv_struct_p;
    io_cv_struct_t  *io_cv_struct_p;
    long            al[NUM_ELEM];

    cfmt_struct_p = (cfmt_struct_t *)malloc(sizeof(cfmt_struct_t)
                                        + (NUM_ELEM-1) * sizeof(union_t));
    i_cv_struct_p = (i_cv_struct_t *)malloc(sizeof(i_cv_struct_t)
                                        + (NUM_ELEM-1) * sizeof(fixed_struct_t));
    /*
     * Note that [out] cv struct is allocated in the manager and
     * materialized in the client stub.
     */
    io_cv_struct_p = (io_cv_struct_t *)malloc(sizeof(io_cv_struct_t)
                                        + (NUM_ELEM-1) * sizeof(array_t));

    /*
     * Test fixed structure:
     *  [in,out] structure containing a union.
     */
    cookie = 999;
    cookie2 = cookie * cookie;
    fixed_struct.d = cookie;
    fixed_struct.u.sw = ndr_false;

    mgr_failures = fixed_op(h, &fixed_struct);
    if (mgr_failures != 0)
    {
        fprintf(stderr, "Failure in fixed_op manager routine\n");
        failures += mgr_failures;
    }

    if (fixed_struct.d != cookie2
        ||  fixed_struct.u.sw != ndr_true
        ||  fixed_struct.u.tagged_union.l != cookie2)
    {
        fprintf(stderr, "Failure in fixed_op()\n");
        fprintf(stderr, "  Expected: %f %d\n", (double)cookie2, cookie2);
        fprintf(stderr, "  Actual:   %f %d\n", fixed_struct.d,
                fixed_struct.u.tagged_union.l);
        failures++;
    }

    /*
     * Test conformant structure:
     *  [in] struct containing conformant array of unions.
     */
    cookie = -888;
    cfmt_struct_p->s = NUM_ELEM;
    for (i = 0 ; i < NUM_ELEM ; i += 2)
        cfmt_struct_p->au[i].sw = ndr_false;
    for (i = 1 ; i < NUM_ELEM ; i += 2)
    {
        cfmt_struct_p->au[i].sw = ndr_true;
        cfmt_struct_p->au[i].tagged_union.l = cookie * i;
    }

    mgr_failures = cfmt_op(h, cfmt_struct_p);
    if (mgr_failures != 0)
    {
        fprintf(stderr, "Failure in cfmt_op manager routine\n");
        failures += mgr_failures;
    }

    /*
     * Test varying structures:
     *  [in] struct containing varying array of char.
     *  [out] struct containing varying arrays of float and struct.
     *  [in,out] struct containing varying arrays of double and pointer.
     */
    cookie = 777;
    i_vary_struct.cl = io_vary_struct.dl = io_vary_struct.pl = NUM_ELEM;
    for (i = 0 ; i < NUM_ELEM ; i++)
    {
        cookie1 = cookie * i;
        i_vary_struct.ac[i] = cookie1 % 10;
        o_vary_struct.af[i] = JUNK;
        o_vary_struct.as[i].d = JUNK;
        o_vary_struct.as[i].u.sw = (ndr_boolean)-2;
        io_vary_struct.ad[i] = cookie1;
        io_vary_struct.ap[i] = &al[i];
        al[i] = cookie1;
    }
    
    mgr_failures = vary_op(h, i_vary_struct, &o_vary_struct, &io_vary_struct);
    if (mgr_failures != 0)
    {
        fprintf(stderr, "Failure in vary_op manager routine\n");
        failures += mgr_failures;
    }

    if (i_vary_struct.cl != NUM_ELEM
        ||  o_vary_struct.fl != DATA_LIM
        ||  o_vary_struct.sl != NUM_ELEM
        ||  io_vary_struct.dl != DATA_LIM
        ||  io_vary_struct.pl != NUM_ELEM)
    {
        fprintf(stderr, "Failure: Bad array length in vary_op()\n");
        failures++;
    }
    else
    {
        for (i = 0 ; i < DATA_LIM ; i++)
        {
            cookie1 = cookie * i;
            cookie2 = cookie1 * cookie1;
            if (i_vary_struct.ac[i] != (cookie1 % 10)
                ||  o_vary_struct.af[i] != cookie2
                ||  o_vary_struct.as[i].d != cookie2
                ||  o_vary_struct.as[i].u.sw != ndr_true
                ||  o_vary_struct.as[i].u.tagged_union.l != cookie1
                ||  io_vary_struct.ad[i] != cookie2
                ||  io_vary_struct.ap[i] != &al[i]
                ||  *io_vary_struct.ap[i] != cookie2)
            {
                fprintf(stderr, "Failure: Bad array data in vary_op()\n");
                failures++;
                break;
            }
        }

        for (i = DATA_LIM ; i < NUM_ELEM ; i++)
        {
            cookie1 = cookie * i;
            cookie2 = cookie1 * cookie1;
            if (i_vary_struct.ac[i] != (cookie1 % 10)
                ||  o_vary_struct.af[i] != JUNK
                ||  o_vary_struct.as[i].d != cookie1
                ||  o_vary_struct.as[i].u.sw != ndr_false
                ||  io_vary_struct.ad[i] != cookie1
                ||  io_vary_struct.ap[i] != &al[i]
                ||  *io_vary_struct.ap[i] != cookie2)
            {
                fprintf(stderr, "Failure: Bad array data in vary_op()\n");
                failures++;
                break;
            }
        }
    }

    /*
     * Test conformant varying structures:
     *  [in] struct containing c-v array of struct.
     *  [out] pointer to struct containing c-v array of pointer.
     *  [in,out] struct containing c-v array of arrays.
     */
    cookie = -666;
    i_cv_struct_p->s = io_cv_struct_p->s = NUM_ELEM;
    i_cv_struct_p->l = io_cv_struct_p->l = DATA_LIM;
    for (i = 0 ; i < NUM_ELEM ; i++)
    {
        cookie1 = cookie * i;
        i_cv_struct_p->as[i].d = cookie1;
        i_cv_struct_p->as[i].u.sw = ndr_true;
        i_cv_struct_p->as[i].u.tagged_union.l = cookie1;
        for (j = 0 ; j < NUM_ELEM ; j++)
            io_cv_struct_p->aa[i][j] = j;
    }
    
    mgr_failures = cv_op(h, i_cv_struct_p, &o_cv_struct_p, io_cv_struct_p);
    if (mgr_failures != 0)
    {
        fprintf(stderr, "Failure in cv_op manager routine\n");
        failures += mgr_failures;
    }

    if (i_cv_struct_p->s != NUM_ELEM
        ||  o_cv_struct_p->s != NUM_ELEM
        ||  io_cv_struct_p->s != NUM_ELEM)
    {
        fprintf(stderr, "Failure: Bad array size in cv_op()\n");
        failures++;
    }
    else if
        (i_cv_struct_p->l != DATA_LIM
        ||  o_cv_struct_p->l != DATA_LIM
        ||  io_cv_struct_p->l != NUM_ELEM)
    {
        fprintf(stderr, "Failure: Bad array length in cv_op()\n");
        failures++;
    }
    else
    {
        for (i = 0 ; i < DATA_LIM ; i++)
        {
            cookie1 = cookie * i;
            cookie2 = cookie1 * cookie1;
            if (i_cv_struct_p->as[i].d != cookie1)
            {
                fprintf(stderr, "Failure: i_cv_struct_p->as[%d].d != cookie2\n",i);
                failures++;
                break;
            }
            if ( i_cv_struct_p->as[i].u.sw != ndr_true )
            {
                fprintf(stderr, "Failure: i_cv_struct_p->as[%d].u.sw != ndr_true\n",i);
                failures++;
                break;
            }
            if ( i_cv_struct_p->as[i].u.tagged_union.l != cookie1 )
            {
                fprintf(stderr, "Failure: i_cv_struct_p->as[%d].u.tagged_union.l != cookie2\n",i);
                failures++;
                break;
            }
            if ( o_cv_struct_p->ap[i] == NULL )
            {
                fprintf(stderr, "Failure: i_cv_struct_p->ap[%d] == NULL()\n",i);
                failures++;
                break;
            }
            if ( *o_cv_struct_p->ap[i] != cookie1 )
            {
                fprintf(stderr, "Failure: *o_cv_struct_p->ap[i] != cookie1()\n",i);
                failures++;
                break;
            }
            if (!good_batch_of_cookies(io_cv_struct_p->aa[i]))
            {
                fprintf(stderr, "Failure: !good_batch_of_cookies(io_cv_struct_p->aa[%d])\n",i);
                failures++;
                break;
            }
        }

        for (i = DATA_LIM ; i < NUM_ELEM ; i++)
        {
            cookie1 = cookie * i;
            if (i_cv_struct_p->as[i].d != cookie1
                ||  i_cv_struct_p->as[i].u.sw != ndr_true
                ||  i_cv_struct_p->as[i].u.tagged_union.l != cookie1
                ||  !good_batch_of_cookies(io_cv_struct_p->aa))
            {
                fprintf(stderr, "Failure: Bad array data in cv_op()\n");
                failures++;
                break;
            }
        }
    }

    if (failures != 0)
        fprintf(stderr, "%d failures.\n", failures);

    return failures;
}
