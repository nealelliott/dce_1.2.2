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
 * Revision 1.1.4.2  1996/02/17  23:14:25  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:50  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:06:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:31:58  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:25:46  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:14:04  ganni]
 * 
 * $EndLog$
 */
/*
*/

#include <stdio.h>
#include <ichar1.h>
#include <test_common.h>

int client_aux(h, passno)
    handle_t h;
    int passno;
{
    int failures = 0;
    ltype op0_arr[FA_SIZE];
    ltype op1_arr[FA_SIZE];
    ltype op2_arr[FA_SIZE];
    ltype op3_arr[FA_SIZE];
    idl_long_int op0_ln;
    idl_usmall_int op3_ln;
    int i;
    open_t *p_o;
    conf_t *p_c;
    fixed_t f;
    varying_t v;
    idl_ulong_int rtag;

    i_h = h;

    for (i=0; i<FA_SIZE; i++)
    {
        op0_arr[i] = 'A' + i;
        op1_arr[i] = 'A' + i;
    }

    op0_ln = FA_SIZE;
    op0(FA_SIZE, &op0_ln, op0_arr);
    if (op0_ln != FA_SIZE)
    {
        printf("op0_ln = %d\n", op0_ln);
        failures++;
    }
    for (i=0; i<FA_SIZE; i++)
    {
        if (op0_arr[i] != 'a' + i)
        {
            printf("op0_arr[%d] = %x\n", i, op0_arr[i]);
            failures++;
        }
    }

    if ( !op1(FA_SIZE, op1_arr) )
    {
        printf("op1 failed\n");
        failures++;
    }

    op2(op2_arr);
    for (i=0; i<FA_SIZE; i++)
    {
        if (op2_arr[i] != 'A' + 8 + i)
        {
            printf("op2_arr[%d] = %x\n", i, op2_arr[i]);
            failures++;
        }
    }

    op3(&op3_ln, op3_arr);
    if (op3_ln != FA_SIZE)
    {
        printf("op3_ln = %d\n", op3_ln);
        failures++;
    }
    for (i=0; i<FA_SIZE; i++)
    {
        if (op3_arr[i] != 'A' + 8 + i)
        {
            printf("op3_arr[%d] = %x\n", i, op3_arr[i]);
            failures++;
        }
    }

    p_o = (open_t *)malloc(sizeof(open_t) + FA_SIZE * sizeof(ltype));
    p_o->sz = FA_SIZE + 1;
    p_o->ln = FA_SIZE;
    for (i = 0; i < p_o->ln; i++)
        p_o->barr[i] = 'A' + i;
    op4(RTN_STAG, RTN_DRTAG, &rtag, p_o);
    if (rtag != RTN_RTAG)
    {
        printf("op4 - rtag = %d\n", rtag);
        failures++;
    }
    if (p_o->sz != FA_SIZE + 1) 
    {
        printf("op4 - p_o->sz = %d\n", p_o->sz);
        failures++;
    }
    if (p_o->ln != FA_SIZE) 
    {
        printf("op4 - p_o->ln = %d\n", p_o->ln);
        failures++;
    }
    for (i=0; i<FA_SIZE; i++)
    {
        if (p_o->barr[i] != 'a' + i)
        {
            printf("p_o->barr[%d] = %x\n", i, p_o->barr[i]);
            failures++;
        }
    }
    free(p_o);

    p_c = (conf_t *)malloc(sizeof(conf_t) + (FA_SIZE - 1) * sizeof(ltype));
    p_c->sz = FA_SIZE;
    for (i = 0; i < p_c->sz; i++)
        p_c->barr[i] = 'A' + i;
    if ( ! op5(RTN_STAG, p_c) )
    {
        printf("op5 failed\n");
        failures++;
    }
    free(p_c);

    op6(&rtag, &f);
    if (rtag != RTN_RTAG)
    {
        printf("op6 - rtag = %d\n", rtag);
        failures++;
    }
    for (i=0; i<FA_SIZE; i++)
    {
        if (f.barr[i] != 'A' + 8 + i)
        {
            printf("f.barr[%d] = %x\n", i, f.barr[i]);
            failures++;
        }
    }

    op7(RTN_DRTAG, &rtag, &v);
    if (rtag != RTN_RTAG)
    {
        printf("op7 - rtag = %d\n", rtag);
        failures++;
    }
    if (v.ln != FA_SIZE/2)
    {
        printf("op7 - v.ln = %d\n", v.ln);
        failures++;
    }
    for (i=0; i<v.ln; i++)
    {
        if (v.barr[i] != 'A' + 8 + i)
        {
            printf("v.barr[%d] = %x\n", i, v.barr[i]);
            failures++;
        }
    }


    return failures;
}
