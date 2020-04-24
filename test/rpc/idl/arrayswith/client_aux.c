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
 * Revision 1.1.4.2  1996/02/17  23:12:36  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:46  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:57:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:28:32  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:06:10  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:26:59  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:23:26  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <arrayswith.h>
#include <test_common.h>

#define S_O 15
#define S_A 20
#define S_U 4
#define S_D 20

int client_aux(h)
   handle_t h;
{
	error_status_t st;
    
    int i,j;

    ndr_hyper_int *y_h[3][3];
    ndr_hyper_int *R_y_h[3][3];
    ndr_long_int *y_l[10];
    long f_l = 3, l_l = 6;
    ndr_short_int *y_o[S_O];
    ndr_small_int *y_a[S_A];
    long f_a = 4, l_a = 14;
    unsigned_p_t y_u[S_U][2];
    unsigned_p_t R_y_u[S_U][2];
    compact_p_t compacts;
    real_p_t *p_reals;
    ndr_short_float ws;
    ndr_long_float wl;

    for (i=0; i<3; i++)
    {
        for (j=0; j<3; j++)
        {
            y_h[i][j] = (ndr_hyper_int *)malloc(sizeof(ndr_hyper_int));
            y_h[i][j]->high = i;
            y_h[i][j]->low = j;
            R_y_h[i][j] = (ndr_hyper_int *)malloc(sizeof(ndr_hyper_int));
            R_y_h[i][j]->high = i;
            R_y_h[i][j]->low = j;
        }
    }
    op_hyper( h, y_h );
    for (i=0; i<3; i++)
    {
        for (j=0; j<3; j++)
        {
            if (y_h[i][j]->high != 256 * R_y_h[i][j]->high) return(1);
            if (y_h[i][j]->low != 16 * R_y_h[i][j]->low) return(1);
        }
    }

    for (i=f_l; i<=l_l; i++)
    {
        y_l[i] = (ndr_long_int *)malloc(sizeof(ndr_long_int));
        *(y_l[i]) = i;
    }
    op_long( h, &f_l, &l_l, y_l);
    for (i=f_l; i<=l_l; i++)
    {
        if( *(y_l[i]) != 2 * i ) return(1);
    }

    for (i=0; i<S_O; i++)
    {
        y_o[i] = (ndr_short_int *)malloc(sizeof(ndr_short_int));
        *(y_o[i]) = i;
    }
    op_short( h, S_O, y_o);
    for (i=0; i<S_O; i++)
    {
        if ( *(y_o[i]) != 3 * i ) return(1);
    }

    for (i=f_a; i<=l_a; i++)
    {
        y_a[i] = (ndr_small_int *)malloc(sizeof(ndr_small_int));
        *(y_a[i]) = i;
    }
    op_small( h, S_A, &f_a, &l_a, y_a );
    for (i=f_a; i<=l_a; i++)
    {
        if ( *(y_a[i]) != 4 * i ) return(1);
    }

    for (i=0; i<S_U; i++)
    {
        for (j=0; j<2; j++)
        {
            y_u[i][j].p_a = (ndr_usmall_int *)malloc(sizeof(ndr_usmall_int));
            *(y_u[i][j].p_a) = i*10 + j;
            y_u[i][j].p_o = (ndr_ushort_int *)malloc(sizeof(ndr_ushort_int));
            *(y_u[i][j].p_o) = i*100 + j;
            y_u[i][j].p_l = (ndr_ulong_int *)malloc(sizeof(ndr_ulong_int));
            *(y_u[i][j].p_l) = i*1000 + j;
            y_u[i][j].p_h = (ndr_uhyper_int *)malloc(sizeof(ndr_uhyper_int));
            y_u[i][j].p_h->low = i*10000 +j;
            y_u[i][j].p_h->high = i*100000 +j;
            R_y_u[i][j].p_a = (ndr_usmall_int *)malloc(sizeof(ndr_usmall_int));
            *(R_y_u[i][j].p_a) = i*10 + j;
            R_y_u[i][j].p_o = (ndr_ushort_int *)malloc(sizeof(ndr_ushort_int));
            *(R_y_u[i][j].p_o) = i*100 + j;
            R_y_u[i][j].p_l = (ndr_ulong_int *)malloc(sizeof(ndr_ulong_int));
            *(R_y_u[i][j].p_l) = i*1000 + j;
            R_y_u[i][j].p_h = (ndr_uhyper_int *)malloc(sizeof(ndr_uhyper_int));
            R_y_u[i][j].p_h->low = i*10000 +j;
            R_y_u[i][j].p_h->high = i*100000 +j;
        }
    }
    op_unsigned( h, S_U, y_u );
    for (i=0; i<S_U; i++)
    {
        for (j=0; j<2; j++)
        {
            if ( *(y_u[i][j].p_a) != 2 * (*(R_y_u[i][j].p_a)) ) return(1);
            if ( *(y_u[i][j].p_o) != 3 * (*(R_y_u[i][j].p_o)) ) return(1);
            if ( *(y_u[i][j].p_l) != 4 * (*(R_y_u[i][j].p_l)) ) return(1);
            if ( y_u[i][j].p_h->low != 5 * R_y_u[i][j].p_h->low ) return(1);
            if ( y_u[i][j].p_h->high != 6 * R_y_u[i][j].p_h->high ) return(1);
        }
    }

    compacts.p_bool[0] = (ndr_boolean *)malloc(sizeof(ndr_boolean));
    *(compacts.p_bool[0]) = true;
    compacts.p_bool[1] = (ndr_boolean *)malloc(sizeof(ndr_boolean));
    *(compacts.p_bool[1]) = false;
    for (i=0; i<3; i++)
    {
        compacts.p_byte[i] = (ndr_byte *)malloc(sizeof(ndr_byte));
        (*compacts.p_byte[i]) = 0x80 >> i;
    }
    for (i=0; i<5; i++)
    {
        compacts.p_char[i] = (ndr_char *)malloc(sizeof(ndr_char));
        (*compacts.p_char[i]) = 'A' + i;
    }
    op_compact( h, &compacts );
    if ( *(compacts.p_bool[0]) )  return(1);
    if ( ! (*(compacts.p_bool[1])) )  return(1);
    for (i=0; i<3; i++)
    {
        if ( ( *(compacts.p_byte[i]) ^ 0xFF ) != ( 0x80 >> i ) ) return(1);
    }
    for (i=0; i<5; i++)
    {
         if ( *(compacts.p_char[i]) != 'a' + i ) return(1);
    }
 
    p_reals = (real_p_t *)malloc(
                   sizeof(real_p_t) + ((S_D - 1) * sizeof(ndr_long_float *)) );
    p_reals->f_f = 4;
    p_reals->l_f = 6;
    for (i=p_reals->f_f; i<=p_reals->l_f; i++)
    {
        p_reals->y_f[i] = (ndr_short_float *)malloc(sizeof(ndr_short_float));
        *(p_reals->y_f[i]) = 1.1 * i;
    }
    p_reals->s_d = S_D;
    p_reals->f_d = 3;
    p_reals->l_d = 8;
    for (i=p_reals->f_d; i<=p_reals->l_d; i++)
    {
        p_reals->y_d[i] = (ndr_long_float *)malloc(sizeof(ndr_long_float));
        *(p_reals->y_d[i]) = 111.111 * i;
    }
    op_real( h, p_reals );
    for (i=p_reals->f_f; i<=p_reals->l_f; i++)
    {
        ws = 11.11 * i;
        if ( *(p_reals->y_f[i]) != ws ) 
                                     return(1);
    }
    for (i=p_reals->f_d; i<=p_reals->l_d; i++)
    { 
        wl = 1111.1111 * i;
        if ( *(p_reals->y_d[i]) != wl ) 
                                     return(1);
    }

    return(0);
}

