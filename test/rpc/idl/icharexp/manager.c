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
 * Revision 1.1.4.2  1996/02/17  23:14:38  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:56  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:07:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:32:25  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:27:03  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:15:00  ganni]
 * 
 * $EndLog$
 */
/*
*/

#include <stdio.h>
#include <icharexp.h>
#include <test_common.h>


/*
** Implementation of Remote Procedures for icharexp
*/

void s_op0
#ifdef IDL_PROTOTYPES
(
    /* [in] */ handle_t h,
    /* [in, out] */ /* Type must appear in user header or IDL */ ltype e0[],
    /* [in] */ idl_long_int sz
)
#else
(h, e0, sz)
#endif

#ifndef IDL_PROTOTYPES
    handle_t h;
    /* Type must appear in user header or IDL */ ltype e0[];
    idl_long_int sz;
#endif
{
    int i;

    for (i=0; i<sz; i++)
        e0[i] += 0x20;
}


void s_op1
#ifdef IDL_PROTOTYPES
(
    /* [in] */ handle_t h,
    /* [in, out] */ /* Type must appear in user header or IDL */ ltype e1[],
    /* [in] */ idl_long_int sz,
    /* [in, out] */ idl_short_int *p_ln
)
#else
(h, e1, sz, p_ln)
#endif

#ifndef IDL_PROTOTYPES
    handle_t h;
    /* Type must appear in user header or IDL */ ltype e1[];
    idl_long_int sz;
    idl_short_int *p_ln;
#endif
{
    int i;

    for (i=0; i < *p_ln; i++)
        e1[i] += 0x20;
    e1[*p_ln] = 'z';
    (*p_ln)++;
}


void s_op2
#ifdef IDL_PROTOTYPES
(
    /* [in] */ handle_t h,
    /* [in, out] */ /* Type must appear in user header or IDL */ ltype e2[54],
    /* [in, out] */ idl_short_int *p_ln
)
#else
(h, e2, p_ln)
#endif

#ifndef IDL_PROTOTYPES
    handle_t h;
    /* Type must appear in user header or IDL */ ltype e2[54];
    idl_short_int *p_ln;
#endif
{
    int i;

    for (i=0; i < *p_ln; i++)
        e2[i] += 0x20;
    e2[*p_ln] = 'z';
    (*p_ln)++;
}


void s_op3
#ifdef IDL_PROTOTYPES
(
    /* [in] */ handle_t h,
    /* [in, out] */ conf_t *p_c
)
#else
(h, p_c)
#endif

#ifndef IDL_PROTOTYPES
    handle_t h;
    conf_t *p_c;
#endif
{
    int i;

    for (i=0; i<p_c->sz; i++)
        p_c->e[i] += 0x20;
}


void s_op4
#ifdef IDL_PROTOTYPES
(
    /* [in] */ handle_t h,
    /* [in, out] */ open_t *p_o
)
#else
(h, p_o)
#endif

#ifndef IDL_PROTOTYPES
    handle_t h;
    open_t *p_o;
#endif
{
    int i;

    for (i=0; i<p_o->ln; i++)
        p_o->e[i] += 0x20;
    p_o->e[p_o->ln] = 'z';
    (p_o->ln)++;
}


void s_op5
#ifdef IDL_PROTOTYPES
(
    /* [in] */ handle_t h,
    /* [in, out] */ varying_t *p_v
)
#else
(h, p_v)
#endif

#ifndef IDL_PROTOTYPES
    handle_t h;
    varying_t *p_v;
#endif
{
    int i;

    for (i=0; i<p_v->ln; i++)
        p_v->e[i] += 0x20;
    p_v->e[p_v->ln] = 'z';
    (p_v->ln)++;
}



/*globaldef*/ icharexp_v0_0_epv_t icharexp_v0_0_m_epv = {
    s_op0,
    s_op1,
    s_op2,
    s_op3,
    s_op4,
    s_op5
};
