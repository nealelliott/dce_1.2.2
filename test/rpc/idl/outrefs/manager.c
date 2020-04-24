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
 * Revision 1.1.4.2  1996/02/17  23:15:51  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:24  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:26:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:24  root]
 * 
 * Revision 1.1.2.2  1993/01/07  20:37:02  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:37:43  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:25:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <outrefs.h>
#include <test_common.h>

void his_op0
( h, r2param )
    /* [in] */handle_t h;
    /* [out] */r2p *r2param;
{
    *(r2param->r2p_1->r3p_1->r4p_1) = 'A';
    *(r2param->r2p_1->r3p_1->r4p_2) = 'B';
    *(r2param->r2p_1->r3p_1->r4p_3) = 'C';
    *(r2param->r2p_1->r3p_1->r4p_4) = 'D';
    *(r2param->r2p_1->r3p_2->r4p_1) = 'E';
    *(r2param->r2p_1->r3p_2->r4p_2) = 'F';
    *(r2param->r2p_1->r3p_2->r4p_3) = 'G';
    *(r2param->r2p_1->r3p_2->r4p_4) = 'H';
    *(r2param->r2p_1->r3p_3->r4p_1) = 'I';
    *(r2param->r2p_1->r3p_3->r4p_2) = 'J';
    *(r2param->r2p_1->r3p_3->r4p_3) = 'K';
    *(r2param->r2p_1->r3p_3->r4p_4) = 'L';
    *(r2param->r2p_2->r3p_1->r4p_1) = 'M';
    *(r2param->r2p_2->r3p_1->r4p_2) = 'N';
    *(r2param->r2p_2->r3p_1->r4p_3) = 'O';
    *(r2param->r2p_2->r3p_1->r4p_4) = 'P';
    *(r2param->r2p_2->r3p_2->r4p_1) = 'Q';
    *(r2param->r2p_2->r3p_2->r4p_2) = 'R';
    *(r2param->r2p_2->r3p_2->r4p_3) = 'S';
    *(r2param->r2p_2->r3p_2->r4p_4) = 'T';
    *(r2param->r2p_2->r3p_3->r4p_1) = 'U';
    *(r2param->r2p_2->r3p_3->r4p_2) = 'V';
    *(r2param->r2p_2->r3p_3->r4p_3) = 'W';
    *(r2param->r2p_2->r3p_3->r4p_4) = 'X';
}

void his_op1
( h, f, l, a )
    /* [in] */handle_t h;
    /* [in, out] */ndr_long_int *f;
    /* [in, out] */ndr_long_int *l;
    /* [in, out] */r4pp a[10];
{
    (*f)--;
    *(a[*f]->r4p_1) = '1' + (*f) - 3; 
    *(a[*f]->r4p_2) = '2' + (*f) - 3; 
    *(a[*f]->r4p_3) = '3' + (*f) - 3;
    *(a[*f]->r4p_4) = '4' + (*f) - 3;
    (*l)++;
    *(a[*l]->r4p_1) = '1' + (*l) - 2; 
    *(a[*l]->r4p_2) = '2' + (*l) - 2; 
    *(a[*l]->r4p_3) = '3' + (*l) - 2; 
    *(a[*l]->r4p_4) = '4' + (*l) - 2; 
}

void his_op2 ( h, p_crp )
       /* [in] */ handle_t h;
       /* [in,out] */ crp_t *p_crp;
{
    int i;

    for (i=0; i<(*p_crp)->s; i++)
    {
        *((*p_crp)->rusps[i]) += (i + 1);
    }
}

void his_op3 ( h, s, rusps )
       /* [in] */ handle_t h;
       /* [in] */ long s;
       /* [out,size_is(s)] */ rusp_t rusps[];
{
    int i;

    for (i=0; i<s; i++)
    {
        *(rusps[i]) = (i + 1) * 100;
    }
}


globaldef outrefs_v0_0_epv_t outrefs_v0_0_m_epv = {
    his_op0,
    his_op1,
    his_op2,
    his_op3
};
