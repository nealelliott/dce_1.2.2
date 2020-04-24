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
 * Revision 1.1.4.2  1996/02/17  23:15:20  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:14  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:25:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:00  root]
 * 
 * Revision 1.1.2.2  1993/01/07  20:31:51  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:35:48  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:25:19  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <math.h>
#include <ool.h>
#include <test_common.h>

ndr_long_int a, b, c;

/* Non-threaded manager routines. */

static void _has_in_ptr(h, sp)
    handle_t    h;
    ptr_t       *sp;
{
    /* Save values in static storage. */
    a = *(sp->a);
    b = *(sp->b);
    c = *(sp->c);

    if (c != a+b)
        fprintf(stderr, "Bad input parameter in _has_in_ptr\n");
}

static void _has_out_ptr(h, sp)
    handle_t    h;
    ptr_t       *sp;
{
    sp->a = (ndr_long_int *)rpc_ss_allocate(sizeof(ndr_long_int));
    sp->b = (ndr_long_int *)rpc_ss_allocate(sizeof(ndr_long_int));
    sp->c = (ndr_long_int *)rpc_ss_allocate(sizeof(ndr_long_int));

    /* Copy saved values from latest call. */
    *(sp->a) = a;
    *(sp->b) = b;
    *(sp->c) = c;
}

static void _has_in_ool(h, sp)
    handle_t    h;
    ool_t       *sp;
{
    /* Save values in static storage. */
    a = sp->a;
    b = sp->b;
    c = sp->c;

    if (c != a+b)
        fprintf(stderr, "Bad input parameter in _has_in_ool\n");
}

static void _has_out_ool(h, sp)
    handle_t    h;
    ool_t       *sp;
{
    /* Copy saved values from latest call. */
    sp->a = a;
    sp->b = b;
    sp->c = c;
}

globaldef ool_v0_0_epv_t ool_v0_0_m_epv = {
    _has_in_ptr,
    _has_out_ptr,
    _has_in_ool,
    _has_out_ool
};
