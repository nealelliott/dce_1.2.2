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
 * Revision 1.1.4.2  1996/02/17  23:14:26  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:51  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:06:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:32:02  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:26:00  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:14:13  ganni]
 * 
 * $EndLog$
 */
/*
*/

#include <stdio.h>
#include <ichar1.h>
#include <test_common.h>

/*
** Implementation of Remote Procedures for ichar1
*/

void s_op0
#ifdef IDL_PROTOTYPES
(
    /* [in] */ idl_ulong_int sz,
    /* [in, out] */ idl_long_int *p_ln,
    /* [in, out] */ /* Type must appear in user header or IDL */ ltype barr[]
)
#else
(sz, p_ln, barr)
#endif

#ifndef IDL_PROTOTYPES
    idl_ulong_int sz;
    idl_long_int *p_ln;
    /* Type must appear in user header or IDL */ ltype barr[];
#endif
{
    int i;

    for (i = 0; i < *p_ln; i++)
        barr[i] += 16;
}


idl_boolean s_op1
#ifdef IDL_PROTOTYPES
(
    /* [in] */ idl_ushort_int sz,
    /* [in] */ /* Type must appear in user header or IDL */ ltype barr[]
)
#else
(sz, barr)
#endif

#ifndef IDL_PROTOTYPES
    idl_ushort_int sz;
    /* Type must appear in user header or IDL */ ltype barr[];
#endif
{
    idl_boolean result;
    int i;

    result = (sz == FA_SIZE);
    for (i = 0; i < sz; i++)
    {
        if (barr[i] != ('A' + 8 + i))
        {
            printf("s_op1 - barr[%d] = %x\n", i, barr[i]);
            result = idl_false;
        }
    }
    return result;
}

void s_op2
#ifdef IDL_PROTOTYPES
(
    /* [out] */ /* Type must appear in user header or IDL */ ltype barr[26]
)
#else
(barr)
#endif

#ifndef IDL_PROTOTYPES
    /* Type must appear in user header or IDL */ ltype barr[26];
#endif
{
    int i;

    for (i=0; i<FA_SIZE; i++)
        barr[i] = 'A' + i;
}

void s_op3
#ifdef IDL_PROTOTYPES
(
    /* [out] */ idl_usmall_int *p_ln,
    /* [out] */ /* Type must appear in user header or IDL */ ltype barr[26]
)
#else
(p_ln, barr)
#endif

#ifndef IDL_PROTOTYPES
    idl_usmall_int *p_ln;
    /* Type must appear in user header or IDL */ ltype barr[26];
#endif
{
    int i;

    *p_ln = FA_SIZE;
    for (i = 0; i < *p_ln; i++)
        barr[i] = 'A' + i;
}


void s_op4
#ifdef IDL_PROTOTYPES
(
    /* [in] */ idl_ulong_int s,
    /* [in] */ idl_ulong_int dr,
    /* [out] */ idl_ulong_int *p_r,
    /* [in, out] */ open_t *p_o
)
#else
(s, dr, p_r, p_o)
#endif

#ifndef IDL_PROTOTYPES
    idl_ulong_int s;
    idl_ulong_int dr;
    idl_ulong_int *p_r;
    open_t *p_o;
#endif
{
    int i;

    if (s != RTN_STAG)
        printf("op4 - s = %d\n", s);
    *p_r = dr + 1;

    for (i = 0; i < p_o->ln; i++)
        p_o->barr[i] += 16;
}


idl_boolean s_op5
#ifdef IDL_PROTOTYPES
(
    /* [in] */ idl_ulong_int s,
    /* [in] */ conf_t *p_c
)
#else
(s, p_c)
#endif

#ifndef IDL_PROTOTYPES
    idl_ulong_int s;
    conf_t *p_c;
#endif
{
    idl_boolean result;
    int i;

    result = (p_c->sz == FA_SIZE);
    for (i = 0; i < p_c->sz; i++)
    {
        if (p_c->barr[i] != ('A' + 8 + i))
        {
            printf("s_op5 - p_c->barr[%d] = %x\n", i, p_c->barr[i]);
            result = idl_false;
        }
    }
    return result;
}

void s_op6
#ifdef IDL_PROTOTYPES
(
    /* [out] */ idl_ulong_int *p_r,
    /* [out] */ fixed_t *p_f
)
#else
(p_r, p_f)
#endif

#ifndef IDL_PROTOTYPES
    idl_ulong_int *p_r;
    fixed_t *p_f;
#endif
{
    int i;

    *p_r = RTN_RTAG;
    for (i=0; i<FA_SIZE; i++)
        p_f->barr[i] = 'A' + i;
}


void s_op7
#ifdef IDL_PROTOTYPES
(
    /* [in] */ idl_ulong_int dr,
    /* [out] */ idl_ulong_int *p_r,
    /* [out] */ varying_t *p_v
)
#else
(dr, p_r, p_v)
#endif

#ifndef IDL_PROTOTYPES
    idl_ulong_int dr;
    idl_ulong_int *p_r;
    varying_t *p_v;
#endif
{
    int i;

    *p_r = dr + 1;
    p_v->ln = FA_SIZE/2;
    for (i=0; i<p_v->ln; i++)
        p_v->barr[i] = 'A' + i;
}


/*globaldef*/ ichar1_v0_0_epv_t ichar1_v0_0_m_epv = {
    s_op0,
    s_op1,
    s_op2,
    s_op3,
    s_op4,
    s_op5,
    s_op6,
    s_op7
};
