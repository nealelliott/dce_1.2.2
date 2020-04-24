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
 * Revision 1.1.4.2  1996/02/17  23:15:00  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:06  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:09:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:32:48  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:28:14  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:15:55  ganni]
 * 
 * $EndLog$
 */
/*
*/

#include <stdio.h>
#include <mtsvary.h>
#include <test_common.h>

void s_op0( h, sm, p_f1 )
    handle_t h;
    idl_small_int sm[DIM1];
    short *p_f1;
{
    int i;

    for ( i = *p_f1; i < DIM1; i++ )
        sm[i] = -sm[i];
    (*p_f1)--;
    sm[*p_f1] = -(*p_f1 + DIM1);
}

void s_op1( h, fl, p_l2)
    handle_t h;
    idl_short_float fl[DIM1][DIM2];
    unsigned short *p_l2;
{
    int i,j;

    for (i=0; i<DIM1; i++)
    {
        for (j=0; j <= *p_l2; j++)
            fl[i][j] = -fl[i][j];
    }
    (*p_l2)++;
    for (i=0; i<DIM1; i++)
    {
            fl[i][*p_l2] = -(2.0 * i * (*p_l2));
    }
}

void s_op2( h, p_t )
    handle_t h;
    three_ways_t *p_t;
{
    int i,j,k;


    for (i=p_t->f1; i<=p_t->l1; i++)
    {
        for (j=p_t->f2; j<=p_t->l2; j++)
        {
            for (k=p_t->f3; k<=p_t->l3; k++)
            {
                p_t->a[i+4][j+3][k+2] = p_t->a[i][j][k];
            }
        }
    }
    p_t->f1 += 4;
    p_t->l1 += 4;
    p_t->f2 += 3;
    p_t->l2 += 3;
    p_t->f3 += 2;
    p_t->l3 += 2;
}

void _opc1( h, v, b, hy, u, sm, p_f, p_l, a)
    handle_t h;
    level v[];
    idl_boolean b[];
    ndr_hyper_int hy[];
    u_t u[];
    idl_small_int sm[];
    idl_small_int *p_f;
    idl_usmall_int *p_l;
    colors a[];
{
    int i;

    for (i=0; i<B_SIZE; i++)
    {
        if (v[i] == low)
            v[i] = high;
        else
            printf("v[%d] != low\n", i);
        if (b[i] == idl_true)
            b[i] = idl_false;
        else
            printf("b[%d] != idl_true\n", i);
        if (hy[i].high == COOKIE1)
            hy[i].high = COOKIE3;
        else
            printf( "hy[%d].high = %08x\n", i, hy[i].high);
        if (hy[i].low == COOKIE2)
            hy[i].low = COOKIE4;
        else
            printf( "hy[%d].low = %08x\n", i, hy[i].low);
        if (u[i].a == COOKIE3)
            u[i].a = COOKIE1;
        else
            printf("u[%d].a = %08X\n", i, u[i].a);
        if (u[i].arms.d == COOKIE4)
            u[i].arms.d = COOKIE2;
        else
            printf("u[%d].arms.d = %08x\n", i, u[i].arms.d);
        if (sm[i] == i)
            sm[i] = -i;
        else
            printf("sm[%d] = %d\n", i, sm[i]);
    }
            
    for (i = *p_f; i<= *p_l; i++)
    {
        if (a[i] == red)
            a[i] = cyan;
        else
            printf("a[%d] != red\n", i);
    }
    (*p_f)--;
    a[*p_f] = yellow;
    (*p_l)++;
    a[*p_l] = magenta;
}

/*globaldef*/ mtsvary_v0_0_epv_t mtsvary_v0_0_m_epv = {
    s_op0,
    s_op1,
    s_op2, _opc1
};
