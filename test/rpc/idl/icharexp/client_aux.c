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
 * Revision 1.1.4.2  1996/02/17  23:14:36  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:55  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:07:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:32:21  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:26:49  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:14:50  ganni]
 * 
 * $EndLog$
 */
/*
*/

#include <stdio.h>
#include <icharexp.h>
#include <test_common.h>

int client_aux(h, passno)
    handle_t h;
    int passno;
{
    int failures = 0;
    int i;
    ltype e0[ASIZE];
    ltype e1[ASIZE];
    idl_short_int ln;
    ltype e2[VSIZE];
    conf_t *p_c;
    open_t *p_o;
    varying_t v;

    for (i=0; i<ASIZE; i++)
        e0[i] = 'A' + i;
    op0(h, e0, ASIZE);
    for (i=0; i<ASIZE; i++)
    {
        if (e0[i] != 'a' + i)
        {
            printf("e0[%d] = %x\n", i, e0[i]);
            failures++;
        }
    }

    ln = ASIZE-1;
    for (i=0; i<ln; i++)
        e1[i] = 'A' + i;
    op1(h, e1, ASIZE, &ln);
    if (ln != ASIZE)
    {
        printf("op1 - ln = %d\n", ln);
        failures++;
    }
    for (i=0; i<ASIZE; i++)
    {
        if (e1[i] != 'a' + i)
        {
            printf("e1[%d] = %x\n", i, e1[i]);
            failures++;
        }
    }

    ln = ASIZE-1;
    for (i=0; i<ln; i++)
        e2[i] = 'A' + i;
    op2(h, e2, &ln);
    if (ln != ASIZE)
    {
        printf("op2 - ln = %d\n", ln);
        failures++;
    }
    for (i=0; i<ASIZE; i++)
    {
        if (e2[i] != 'a' + i)
        {
            printf("e2[%d] = %x\n", i, e2[i]);
            failures++;
        }
    }

    p_c = malloc(sizeof(conf_t) + (ASIZE - 1) * sizeof(ltype));
    p_c->sz = ASIZE;
    for (i = 0; i < p_c->sz; i++)
        p_c->e[i] = 'A' + i;
    op3(h, p_c);
    if (p_c->sz != ASIZE)
    {
        printf("p_c->sz = %d\n", p_c->sz);
        failures++;
    }
    for (i=0; i<ASIZE; i++)
    {
        if (p_c->e[i] != 'a' + i)
        {
            printf("p_c->e[%d] = %x\n", i, p_c->e[i]);
            failures++;
        }
    }
    free(p_c);

    p_o = malloc(sizeof(open_t) + (ASIZE - 1) * sizeof(ltype));
    p_o->sz = ASIZE;
    p_o->ln = ASIZE-1;
    for (i = 0; i < p_o->ln; i++)
        p_o->e[i] = 'A' + i;
    op4(h, p_o);
    if (p_o->sz != ASIZE)
    {
        printf("p_o->sz = %d\n", p_o->sz);
        failures++;
    }
    if (p_o->ln != ASIZE)
    {
        printf("p_o->ln = %d\n", p_o->ln);
        failures++;
    }
    for (i=0; i<ASIZE; i++)
    {
        if (p_o->e[i] != 'a' + i)
        {
            printf("p_o->e[%d] = %x\n", i, p_o->e[i]);
            failures++;
        }
    }
    free(p_o);

    v.ln = ASIZE-1;
    for (i = 0; i < v.ln; i++)
        v.e[i] = 'A' + i;
    op5(h, &v);
    if (v.ln != ASIZE)
    {
        printf("v.ln = %d\n", v.ln);
        failures++;
    }
    for (i=0; i<ASIZE; i++)
    {
        if (v.e[i] != 'a' + i)
        {
            printf("v.e[%d] = %x\n", i, v.e[i]);
            failures++;
        }
    }

    return failures;
}
