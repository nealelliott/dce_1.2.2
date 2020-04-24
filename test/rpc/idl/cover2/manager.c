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
 * Revision 1.1.6.2  1996/02/17  23:13:25  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:16  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:01:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:29:48  root]
 * 
 * Revision 1.1.4.2  1993/01/07  19:51:12  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:30:58  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/26  19:04:41  harrow
 * 	Remove test for unsupported [represent_as] on a conformant structure.
 * 	[1992/05/26  18:18:44  harrow]
 * 
 * Revision 1.1  1992/01/19  03:31:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <cover2.h>
#include <test_common.h>

void s_a_op0
( h, p_o, h_a )
    /* [in] */handle_t h;
    outer *p_o;
    long h_a[FDIM][FDIM];
{
    int i,j;

    p_o->o_small *= 2;
    p_o->o_inner.i1 *= 3;
    p_o->o_inner.i2 *= 4;
    p_o->o_short *= 5;

    for (i=0; i<FDIM; i++)
    {
        for (j=0; j<FDIM; j++)
        {
            h_a[i][j] += i*100 +j*1000;
        }
    }
}
#if CONF_REP_AS
void s_a_op1
( h, p_is )
    /* [in] */handle_t h;
c_string *p_is;
{
    c_string p_os;
    int i,length;

    length = strlen(p_is); 
    for (i=0; i<length; i++)
    {
        p_os[i] = (*p_is)[length - i - 1];
    }
    p_os[length] = '\0';
    strcpy(*p_is, p_os);
}
#endif
void s_a_op2
( h, s, f, ap, a )
    /* [in] */handle_t h;
    long s;
    long *f;
    r_p_c ap[][DIM2];
    char a[][DIM2];
{
    int i,j;

    for (i=(*f); i<FDIM; i++)
    {
        for (j=0; j<DIM2; j++)
        {
            a[i][j] = *(ap[i][j]);
        }
    }
}

void s_a_op3
( h, b )
    /* [in] */handle_t h;
    op3_elt b[];
{
    int i;

    for (i=0; i<FDIM; i++)
    {
        b[i].a *= 2;
        *(b[i].p_a) *= 3;
        *(b[i].p_b) *= 4;
        b[i].b *= 5;
    }
}

void s_a_op4( h, ln, a, m, f, b, s, f2, c, f3, lt, d )
       /* [in] */ handle_t h;
       /* [in] */ long ln;
       /* [in, length_is(ln), v1_array] */ long a[FDIM];
       /* [in] */ long m;
       /* [in, out] */ long *f;
       /* [in, out, max_is(m), first_is(*f)] */ long b[];
       /* [in] */ long s;
       /* [in] */ long f2;
       /* [in, size_is(s), first_is(f2)] */ long c[];
       /* [in, out] */ long *f3;
       /* [in, out] */ long *lt;
       /* [in, out, first_is(*f3), last_is(*lt)] */ long d[FDIM];
{
    int i;

    *f = m - ln + 1;
    for (i=0; i<ln; i++)
        b[*f + i] = a[i];

    *f3 = f2 - 1;
    *lt = s - 2;
    for (i=f2; i<s; i++)
        d[i - 1] = c[i];
}

void s_a_op5(h, up)
      /* [in] */ handle_t h;
      /* [in, out] */ op5_union *up;
{
    /*
     * Operation does nothing - its whole purpose in life is to check that
     * "escape characters" get mapped correctly in the generated header; if
     * not, an "invalid tag" exception will happen on the union discriminator.
     */
}

void s_a_op6(h, sp)
      /* [in] */ handle_t h;
      /* [in, out] */ op6_string *sp;
{
    int i,j;
    idl_char temp;

    /* Reverse the string. */
    for (i=0, j=25; i<13; i++, j--)
    {
        temp = sp->s[i].c;
        sp->s[i].c = sp->s[j].c;
        sp->s[j].c = temp;
    }
}

void s_a_op7(h, a)
      /* [in] */ handle_t h;
      /* [in, out] */ op7_array a;
{
    int i;
    idl_long_int *newp;

    for (i=0; i<FDIM; i++)
    {
        newp = (idl_long_int *)rpc_ss_allocate(sizeof(idl_long_int));
        *newp = *(a[i]) * 2;
        a[i] = newp;
    }
}

void s_a_op8(h, s, a)
      /* [in] */ handle_t h;
      /* [in] */ idl_long_int s;
      /* [out] */ op8_array a;
{
    int i;

    for (i=0; i<s; i++)
    {
        *(a[i].lp) = i * 100;
    }
}

globaldef cover2_v0_0_epv_t cover2_v0_0_m_epv = {s_a_op0,
#if CONF_REP_AS
                                                 s_a_op1,
#endif
                                                 s_a_op2,
                                                 s_a_op3,
                                                 s_a_op4,
                                                 s_a_op5,
                                                 s_a_op6,
                                                 s_a_op7,
                                                 s_a_op8};
