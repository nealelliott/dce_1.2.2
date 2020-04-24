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
 * Revision 1.1.4.2  1996/02/17  23:14:58  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:05  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:09:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:32:47  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:28:09  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:15:51  ganni]
 * 
 * $EndLog$
 */
/*
*/

#include <stdio.h>
#include <mtsvary.h>
#include <test_common.h>

int client_aux(h, passes)
    handle_t h;
    int passes;
{
    int failures = 0;
    idl_small_int sm[DIM1];
    short f1;
    int i,j,k;
    idl_short_float fl[DIM1][DIM2];
    unsigned short l2;
    three_ways_t t;
    level v[B_SIZE];
    idl_boolean b[B_SIZE];
    ndr_hyper_int hy[B_SIZE];
    u_t u[B_SIZE];
    idl_small_int sm2[B_SIZE];
    idl_small_int f;
    idl_usmall_int l;
    colors a[B_SIZE];

    for (j=8; j<=12; j++)
    {
        f1 = DIM1 - j;
        for (i=0; i<DIM1; i++)
            sm[i] = DIM1 + i;
        op0( h, sm, &f1 );
        if (f1 != (DIM1 -j -1))
            failures++;
        for (i=f1; i<DIM1; i++)
        {
            if (sm[i] != -(DIM1 + i))
                failures++;
        }
        printf( "op0(j=%d) completed - %d failures\n", j, failures );
    }

    l2 = 6;
    for (i=0; i<DIM1; i++)
    {
        for (j=0; j<=l2; j++)
        {
            fl[i][j] = 2.0 * i * j;
        }
    }
    op1( h, fl, &l2 );
    if ( l2 != 7 )
        failures++;
    for (i=0; i<DIM1; i++)
    {
        for (j=0; j<=l2; j++)
        {
            if (fl[i][j] != -(2.0 * i * j))
                failures++;
        }
    }
    printf( "op1 completed - %d failures\n", failures );

    t.f1 = 5;
    t.l1 = 8;
    t.f2 = 4;
    t.l2 = 6;
    t.f3 = 2;
    t.l3 = 3;

    for (i=t.f1; i<=t.l1; i++)
    {
        for (j=t.f2; j<=t.l2; j++)
        {
            for (k=t.f3; k<=t.l3; k++)
            {
                t.a[i][j][k] = i * j * k;
            }
        }
    }
    op2(h, &t);
    for (i=t.f1; i<=t.l1; i++)
    {
        for (j=t.f2; j<=t.l2; j++)
        {
            for (k=t.f3; k<=t.l3; k++)
            {
                if (t.a[i][j][k] != (i-4) * (j-3) * (k-2))
                    failures++;
            }
        }
    }
    printf( "op2 completed - %d failures\n", failures );

    t.f1 = 5;
    t.l1 = 0;    
    t.f2 = 4;
    t.l2 = 6;
    t.f3 = 2;
    t.l3 = 3;
    op2(h, &t);
    printf("Returned from op2 call with inside-out data limits\n");

    TRY
        f = -1;
        l = 10;
        opc1(h, v, b, hy, u, sm2, &f, &l, a);
        printf("**Failure**: first call to opc1 didn't get exception\n");
        failures++;
    CATCH(rpc_x_invalid_bound)
#ifndef _MSDOS
    CATCH_ALL
        printf("Unexpected exception in first call to opc1\n");
        print_exception(THIS_CATCH);
        failures++;
#endif
    ENDTRY

    for (i=0; i<B_SIZE; i++)
    {
        v[i] = low;
        b[i] = idl_true;
        a[i] = green;
        hy[i].high = COOKIE1;
        hy[i].low = COOKIE2;
        u[i].a = COOKIE3;
        u[i].arms.d = COOKIE4;
        sm2[i] = i;
    }
    f = 20;
    l = 30;
    for(i=f; i<=l; i++)
        a[i] = red;
    opc1(h, v, b, hy, u, sm2, &f, &l, a);
    for(i=0; i<B_SIZE; i++)
    {
        if (v[i] != high)
        {
            printf("opc1 - second call: v[%d] != high\n", i);
            failures++;
        }
        if (b[i] != idl_false)
        {
            printf("opc1 - second call: b[%d] != idl_false\n", i);
            failures++;
        }
        if (hy[i].high != COOKIE3)
        {
            printf("opc1 - second call: hy[%d].high = %08x\n", i, hy[i].high);
            failures++;
        }
        if (hy[i].low != COOKIE4)
        {
            printf("opc1 - second call: hy[%d].low = %08x\n", i, hy[i].low);
            failures++;
        }
        if (u[i].a != COOKIE1)
        {
            printf("opc1 - second call: u[%d].a = %08X\n", i, u[i].a);
            failures++;
        }
        if (u[i].arms.d != COOKIE2)
        {
            printf("opc1 - second call: u[%d].arms.d = %08x\n", i, u[i].arms.d);
            failures++;
        }
        if (sm2[i] != -i)
        {
            printf("opc1 - second call: sm2[%d] = %d\n", i, sm2[i]);
            failures++;
        }
    }
    if (a[f] != yellow)
    {
        printf("a[%d] != yellow\n", f);
        failures++;
    }
    for (i = f+1; i <= l-1; i++)
    {
        if (a[i] != cyan)
        {
            printf("a[%d] != cyan\n", i);
            failures++;
        }
    }
    if (a[l] != magenta)
    {
        printf("a[%d] != magenta\n", l);
        failures++;
    }

    return failures;
}
