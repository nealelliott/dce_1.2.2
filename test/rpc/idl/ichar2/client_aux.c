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
 * Revision 1.1.4.2  1996/02/17  23:14:30  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:53  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:07:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:32:09  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:26:18  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:14:26  ganni]
 * 
 * $EndLog$
 */
/*
*/

#include <stdio.h>
#include <ichar2.h>
#include <test_common.h>

int client_aux(h, passno)
    handle_t h;
    int passno;
{
    int failures = 0;
    ltype y_char = 'Y';
    tbs_t tbs;
    ltype c_arr[4];
    int i,j;
    a_t a;
    u_t u;
    is2_t *p_is2;
    os2_t os2;
    ltype *p_u, *p_r, *p_c;
    nc_arptr_t ara[IASIZE];
    nc_uptr_t a2u[IASIZE][IASIZE];

    tbs.v = 'V';
    tbs.w = 'W';
    op0(h, 'X', &y_char, &tbs, 4, c_arr);
    for (i=0; i<4; i++)
    {
        if (c_arr[i] != 'V' + i)
        {
            printf("op0 - c_arr[%d] = %x\n", i, c_arr[i]);
            failures++;
        }
    }

    for (i=0; i<IASIZE; i++)
    {
        a[i].row = i;
        a[i].column = 15 - i;
    }
    u.is_array = idl_false;
    u.arms.scalar.row = i;
    u.arms.scalar.column = 15 - i;
    op1(h, a, &u);
    if ( ! u.is_array )
    {
        printf("! u.is_array\n");
        failures++;
    }
    for (i=0; i<OASIZE; i++)
    {
        if (u.arms.array[i].row != 15 - i)
        {
            printf("u.arms.array[%d].row = %x\n", i, u.arms.array[i].row);
            failures++;
        }
        if (u.arms.array[i].column != i)
        {
            printf("u.arms.array[%d].column = %x\n", i, u.arms.array[i].column);
            failures++;
        }
    }

    p_is2 = (is2_t *)malloc(sizeof(is2_t) + (A2SIZE-1)*sizeof(ltype));
    p_is2->sz = A2SIZE;
    p_is2->p_sc = (ltype *)malloc(sizeof(ltype));
    *(p_is2->p_sc) = 'A';
    p_is2->p_ua = (at_t *)malloc(sizeof(at_t));
    p_u = (ltype *)(p_is2->p_ua);
    p_is2->p_ra = (at_t *)malloc(sizeof(at_t));
    p_r = (ltype *)(p_is2->p_ra);
    os2.p_sc = (ltype *)malloc(sizeof(ltype));
    os2.p_ua = (at_t *)malloc(sizeof(at_t));
    os2.p_ra = (at_t *)malloc(sizeof(at_t));
    os2.p_ca = (at_t *)malloc(sizeof(at_t));
    for (i=0; i<A2SIZE; i++)
    {
        p_u[i] = 'B' + i;
        p_r[i] = 'I' + i;
        p_is2->ca[i] = 'P' + i;
    }
    op2(h, p_is2, &os2);
    p_u = (ltype *)os2.p_ua;
    p_r = (ltype *)os2.p_ra;
    p_c = (ltype *)os2.p_ca;
    for (i=0; i<A2SIZE; i++)
    {
        if (p_u[i] != 'B' + i)
        {
            printf("p_u[%d] = %x\n", i, p_u[i]);
            failures++;
        }
        if (p_r[i] != 'I' + i)
        {
            printf("p_r[%d] = %x\n", i, p_r[i]);
            failures++;
        }
        if (p_c[i] != 'P' + i)
        {
            printf("p_c[%d] = %x\n", i, p_c[i]);
            failures++;
        }
    }
    free(p_is2->p_sc);
    free(p_is2->p_ua);
    free(p_is2->p_ra);
    free(os2.p_sc);
    free(os2.p_ua);
    free(os2.p_ra);
    free(os2.p_ca);
    free(p_is2);

    for (i=0; i<IASIZE; i++)
    {
        ara[i] = (nc_arptr_t)malloc(IASIZE * sizeof(ntype));
        for (j=0; j<IASIZE; j++)
        {
            a2u[i][j] = NULL;
            (*ara[i])[j] = 'A' + IASIZE * i + j;
        }
    }
    op3(h, ara, a2u);
    for (i=0; i<IASIZE; i++)
    {
        free(ara[i]);
        for (j=0; j<IASIZE; j++)
        {
            if (*(a2u[i][j]) != 'A' + IASIZE * i + j)
            {
                printf("*(a2u[%d][%d]) = %c\n", i, j, *(a2u[i][j]));
                failures++;
            }
            free(a2u[i][j]);
        }
    }

    return failures;
}
