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
 * Revision 1.1.4.2  1996/02/17  23:15:48  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:23  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:26:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:24  root]
 * 
 * Revision 1.1.2.3  1993/01/13  21:49:33  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:37:40  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:25:55  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <test_common.h>
#include <outrefs.h>

#define NUMRUSPS 15

int client_aux(h,passes)
      handle_t h;
      int passes;
{
    error_status_t st;
    r2p r2data;
    r4pp a[10];
    int i;
    long f;
    long l;
    crp_t crp;
    rusp_t rusps[NUMRUSPS];

    r2data.r2p_1 = (r3p *)malloc(sizeof(r3p));
    r2data.r2p_1->r3p_1 = (r4p *)malloc(sizeof(r4p));
    r2data.r2p_1->r3p_1->r4p_1 = (ndr_char *)malloc(sizeof(ndr_char));
    r2data.r2p_1->r3p_1->r4p_2 = (ndr_char *)malloc(sizeof(ndr_char));
    r2data.r2p_1->r3p_1->r4p_3 = (ndr_char *)malloc(sizeof(ndr_char));
    r2data.r2p_1->r3p_1->r4p_4 = (ndr_char *)malloc(sizeof(ndr_char));
    r2data.r2p_1->r3p_2 = (r4p *)malloc(sizeof(r4p));
    r2data.r2p_1->r3p_2->r4p_1 = (ndr_char *)malloc(sizeof(ndr_char));
    r2data.r2p_1->r3p_2->r4p_2 = (ndr_char *)malloc(sizeof(ndr_char));
    r2data.r2p_1->r3p_2->r4p_3 = (ndr_char *)malloc(sizeof(ndr_char));
    r2data.r2p_1->r3p_2->r4p_4 = (ndr_char *)malloc(sizeof(ndr_char));
    r2data.r2p_1->r3p_3 = (r4p *)malloc(sizeof(r4p));
    r2data.r2p_1->r3p_3->r4p_1 = (ndr_char *)malloc(sizeof(ndr_char));
    r2data.r2p_1->r3p_3->r4p_2 = (ndr_char *)malloc(sizeof(ndr_char));
    r2data.r2p_1->r3p_3->r4p_3 = (ndr_char *)malloc(sizeof(ndr_char));
    r2data.r2p_1->r3p_3->r4p_4 = (ndr_char *)malloc(sizeof(ndr_char));
    r2data.r2p_2 = (r3p *)malloc(sizeof(r3p));
    r2data.r2p_2->r3p_1 = (r4p *)malloc(sizeof(r4p));
    r2data.r2p_2->r3p_1->r4p_1 = (ndr_char *)malloc(sizeof(ndr_char));
    r2data.r2p_2->r3p_1->r4p_2 = (ndr_char *)malloc(sizeof(ndr_char));
    r2data.r2p_2->r3p_1->r4p_3 = (ndr_char *)malloc(sizeof(ndr_char));
    r2data.r2p_2->r3p_1->r4p_4 = (ndr_char *)malloc(sizeof(ndr_char));
    r2data.r2p_2->r3p_2 = (r4p *)malloc(sizeof(r4p));
    r2data.r2p_2->r3p_2->r4p_1 = (ndr_char *)malloc(sizeof(ndr_char));
    r2data.r2p_2->r3p_2->r4p_2 = (ndr_char *)malloc(sizeof(ndr_char));
    r2data.r2p_2->r3p_2->r4p_3 = (ndr_char *)malloc(sizeof(ndr_char));
    r2data.r2p_2->r3p_2->r4p_4 = (ndr_char *)malloc(sizeof(ndr_char));
    r2data.r2p_2->r3p_3 = (r4p *)malloc(sizeof(r4p));
    r2data.r2p_2->r3p_3->r4p_1 = (ndr_char *)malloc(sizeof(ndr_char));
    r2data.r2p_2->r3p_3->r4p_2 = (ndr_char *)malloc(sizeof(ndr_char));
    r2data.r2p_2->r3p_3->r4p_3 = (ndr_char *)malloc(sizeof(ndr_char));
    r2data.r2p_2->r3p_3->r4p_4 = (ndr_char *)malloc(sizeof(ndr_char));
    op0( h, &r2data );
    printf( "op0 result\n" );
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",
        *(r2data.r2p_1->r3p_1->r4p_1),
        *(r2data.r2p_1->r3p_1->r4p_2),
        *(r2data.r2p_1->r3p_1->r4p_3),
        *(r2data.r2p_1->r3p_1->r4p_4),
        *(r2data.r2p_1->r3p_2->r4p_1),
        *(r2data.r2p_1->r3p_2->r4p_2),
        *(r2data.r2p_1->r3p_2->r4p_3),
        *(r2data.r2p_1->r3p_2->r4p_4),
        *(r2data.r2p_1->r3p_3->r4p_1),
        *(r2data.r2p_1->r3p_3->r4p_2),
        *(r2data.r2p_1->r3p_3->r4p_3),
        *(r2data.r2p_1->r3p_3->r4p_4),
        *(r2data.r2p_2->r3p_1->r4p_1),
        *(r2data.r2p_2->r3p_1->r4p_2),
        *(r2data.r2p_2->r3p_1->r4p_3),
        *(r2data.r2p_2->r3p_1->r4p_4),
        *(r2data.r2p_2->r3p_2->r4p_1),
        *(r2data.r2p_2->r3p_2->r4p_2),
        *(r2data.r2p_2->r3p_2->r4p_3),
        *(r2data.r2p_2->r3p_2->r4p_4),
        *(r2data.r2p_2->r3p_3->r4p_1),
        *(r2data.r2p_2->r3p_3->r4p_2),
        *(r2data.r2p_2->r3p_3->r4p_3),
        *(r2data.r2p_2->r3p_3->r4p_4) );

    for (i=0; i<10; i++)
    {
        a[i] = (r4p *)malloc(sizeof(r4p));
        a[i]->r4p_1 = (ndr_char *)malloc(sizeof(ndr_char));
        *(a[i]->r4p_1) = 'a' + 2*i;
        a[i]->r4p_2 = (ndr_char *)malloc(sizeof(ndr_char));
        *(a[i]->r4p_2) = 'A' + 2*i;
        a[i]->r4p_3 = (ndr_char *)malloc(sizeof(ndr_char));
        *(a[i]->r4p_3) = 'a' + 2*i + 1;
        a[i]->r4p_4 = (ndr_char *)malloc(sizeof(ndr_char));
        *(a[i]->r4p_4) = 'A' + 2*i + 1;
    }
    f = 3;
    l = 6;
    op1 ( h, &f, &l, a );
    printf( "op1 result\n" );
    for (i=f; i<=l; i++)
    {
        printf( "%c%c%c%c",
        *(a[i]->r4p_1),
        *(a[i]->r4p_2),
        *(a[i]->r4p_3),
        *(a[i]->r4p_4) );
    }
    printf("\n");

    crp = (confrusp_t *)malloc(sizeof(confrusp_t) + (NUMRUSPS-1)*sizeof(rusp_t));
    crp->s = NUMRUSPS;
    for (i=0; i<NUMRUSPS; i++)
    {
        crp->rusps[i] = (ndr_ushort_int *)malloc(sizeof(ndr_ushort_int));
        *(crp->rusps[i]) = (i + 1) * 100;
    }
    op2( h, &crp );
    printf( "op2 result\n" );
    for (i=0; i<NUMRUSPS; i++)
    {
        printf( " %d", *(crp->rusps[i]) );
    }
    printf("\n");
 
    for (i=0; i<NUMRUSPS; i++)
    {
        rusps[i] = (ndr_ushort_int *)malloc(sizeof(ndr_ushort_int));
    }
    op3( h, NUMRUSPS, rusps);
    printf( "op3 result\n" );
    for (i=0; i<NUMRUSPS; i++)
    {
        printf( " %d", *(rusps[i]) );
    }
    printf("\n");


    return 0;
}
