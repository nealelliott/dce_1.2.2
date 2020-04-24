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
 * Revision 1.1.4.2  1996/02/17  23:12:37  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:47  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:57:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:28:33  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:06:18  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:27:03  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:23:28  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <arrayswith.h>
#include <test_common.h>

void his_op_hyper
( h, y_h )
    /* [in] */handle_t h;
    /* [in, out] */ndr_hyper_int *y_h[3][3];
{
    int i,j;

#ifdef DEBUG
    printf("Server received\n");
#endif
    for (i=0; i<3; i++)
    {
        for (j=0; j<3; j++)
        {
#ifdef DEBUG
            printf( "%3d %3u        ", y_h[i][j]->high, y_h[i][j]->low );
#endif
            y_h[i][j]->high *= 256;
            y_h[i][j]->low *= 16;
        }
#ifdef DEBUG
        printf("\n");
#endif
    }
}

void his_op_long
( h, f_l, l_l, y_l )
    /* [in] */handle_t h;
    /* [in, out] */ndr_long_int *f_l;
    /* [in, out] */ndr_long_int *l_l;
    /* [in, out] */ndr_long_int *y_l[10];
{
    int i;

#ifdef DEBUG
    printf("Server received\n");
    for (i=(*f_l); i<=(*l_l); i++)
    {
        printf( " %2d", *(y_l[i]) );
    }
    printf("\n");
#endif
    (*f_l)--;
    y_l[*(f_l)] = (ndr_long_int *)rpc_ss_allocate(sizeof(ndr_long_int));
    (*l_l)++;
    y_l[*(l_l)] = (ndr_long_int *)rpc_ss_allocate(sizeof(ndr_long_int));
    for (i=(*f_l); i<=(*l_l); i++)
    {
        *(y_l[i]) = 2 * i;
    }

}

void his_op_short
( h, s_o, y_o )
    /* [in] */handle_t h;
    /* [in] */ndr_long_int s_o;
    /* [in, out] */ndr_short_int *y_o[];
{
    int i;

#ifdef DEBUG
    printf("Server received\n");
#endif
    for (i=0; i<s_o; i++)
    {
#ifdef DEBUG
        printf( " %2d", *(y_o[i]) );
#endif
        *(y_o[i]) *= 3;
    }
#ifdef DEBUG
    printf("\n");
#endif
}

void his_op_small
( h, s_a, f_a, l_a, y_a )
    /* [in] */handle_t h;
    /* [in] */ndr_long_int s_a;
    /* [in, out] */ndr_long_int *f_a;
    /* [in, out] */ndr_long_int *l_a;
    /* [in, out] */ndr_small_int *y_a[];
{
    int i;

#ifdef DEBUG
    printf("Server received\n");
    for (i=(*f_a); i<=(*l_a); i++)
    {
        printf( " %2d", *(y_a[i]) );
    }
    printf("\n");
#endif
    (*f_a)--;
    y_a[*(f_a)] = (ndr_small_int *)rpc_ss_allocate(sizeof(ndr_small_int));
    (*l_a)++;
    y_a[*(l_a)] = (ndr_small_int *)rpc_ss_allocate(sizeof(ndr_small_int));
    for (i=(*f_a); i<=(*l_a); i++)
    {
        *(y_a[i]) = 4 * i;
    }
}

void his_op_unsigned
( h, s_u, y_u )
    /* [in] */handle_t h;
    /* [in] */ndr_long_int s_u;
    /* [in, out] */unsigned_p_t y_u[][2];
{
    int i,j;

#ifdef DEBUG
    printf("Server received\n");
#endif
    for (i=0; i<s_u; i++)
    {
        for (j=0; j<2; j++)
        {
#ifdef DEBUG
            printf ( "%u %u %u %u %u\n",
                *(y_u[i][j].p_a),
                *(y_u[i][j].p_o),
                *(y_u[i][j].p_l),
                y_u[i][j].p_h->low,
                y_u[i][j].p_h->high ); 
#endif
            *(y_u[i][j].p_a) *= 2;
            *(y_u[i][j].p_o) *= 3;
            *(y_u[i][j].p_l) *= 4;
            y_u[i][j].p_h->low *= 5;
            y_u[i][j].p_h->high *= 6;
        }
    }
}

void his_op_compact
( h, p_compacts )
    /* [in] */handle_t h;
    /* [in, out] */compact_p_t *p_compacts;
{
    int i;

#ifdef DEBUG
    printf("Server received\n");
#endif
    for (i=0; i<2; i++ )
    {
        if ( *(p_compacts->p_bool[i]) )
        {
#ifdef DEBUG
            printf("TRUE ");
#endif
             *(p_compacts->p_bool[i]) = false;
        }
        else
        {
#ifdef DEBUG
            printf("FALSE ");
#endif
             *(p_compacts->p_bool[i]) = true;
        }
    }
    for (i=0; i<3; i++)
    {
#ifdef DEBUG
        printf("%02x ",*(p_compacts->p_byte[i]) );
#endif
        *(p_compacts->p_byte[i]) ^= 0xFF;
    }
    for (i=0; i<5; i++)
    {
#ifdef DEBUG
        printf("%c ",*(p_compacts->p_char[i]) );
#endif
        *(p_compacts->p_char[i]) += ('a' - 'A');
    }
#ifdef DEBUG
    printf("\n");
#endif
}

void his_op_real
( h, p_reals )
    /* [in] */handle_t h;
    /* [in, out] */real_p_t *p_reals;
{
    int i;

#ifdef DEBUG
    printf("Server received\n");
    for (i=p_reals->f_f; i<=p_reals->l_f; i++)
    {
        printf( "%4.1f ",*(p_reals->y_f[i]) );
    }
    printf("\n");
#endif
    (p_reals->f_f)--;
    p_reals->y_f[p_reals->f_f] = (ndr_short_float *)
                                      rpc_ss_allocate(sizeof(ndr_short_float));
    (p_reals->l_f)++;
    p_reals->y_f[p_reals->l_f] = (ndr_short_float *)
                                      rpc_ss_allocate(sizeof(ndr_short_float));
    for (i=p_reals->f_f; i<=p_reals->l_f; i++)
    {
        *(p_reals->y_f[i]) = 11.11 * i;
    }
#ifdef DEBUG
    for (i=p_reals->f_d; i<=p_reals->l_d; i++)
    {
        printf( "%6.3f ",*(p_reals->y_d[i]) );
    }
    printf("\n");
#endif
    (p_reals->f_d)--;
    p_reals->y_d[p_reals->f_d] = (ndr_long_float *)
                                      rpc_ss_allocate(sizeof(ndr_long_float));
    (p_reals->l_d)++;
    p_reals->y_d[p_reals->l_d] = (ndr_long_float *)
                                      rpc_ss_allocate(sizeof(ndr_long_float));
    for (i=p_reals->f_d; i<=p_reals->l_d; i++)
    {
        *(p_reals->y_d[i]) = 1111.1111 * i;
    }
}

globaldef arrayswith_v0_0_epv_t arrayswith_v0_0_m_epv = {
        his_op_hyper,
        his_op_long,
        his_op_short,
        his_op_small,
        his_op_unsigned,
        his_op_compact,
        his_op_real
    };
