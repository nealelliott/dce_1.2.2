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
 * Revision 1.1.4.2  1996/02/17  23:15:22  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:15  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:25:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:04  root]
 * 
 * Revision 1.1.2.2  1993/01/07  20:32:25  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:36:01  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:25:23  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <oolsmall.h>

int client_aux(handle)
handle_t handle;
{
    v1_vary_struct v_s;
    v1_open_struct *p_v_o_s;
    v1_fixed_array v_f_a;
    long *v_c_a;

    long i, f, l;
              
    i_h = handle;

    v_s.l = 6;
    for( i=0; i<=v_s.l; i++)
    {
        v_s.f_a[i] = 16 * (i + 1);
    }
    v1_op0( &v_s, &l, v_f_a );
    if (l != v_s.l) return(1);
    for(i=0; i<=l; i++)
    {
        if (v_f_a[i] != v_s.f_a[i]) return(1);
    }

    p_v_o_s = (v1_open_struct *)malloc(sizeof(v1_open_struct) 
                                   + ((CONF_SIZE-1)*sizeof(long)));
    v_c_a = (long *)malloc(CONF_SIZE*sizeof(long));
    p_v_o_s->s = CONF_SIZE;
    p_v_o_s->l = 7;
    for(i=0; i<=p_v_o_s->l; i++)
    {
        p_v_o_s->c_a[i] = 16 * (i + 1);
    }
    v1_op2( p_v_o_s, CONF_SIZE, &l, v_c_a );
    if ( l != p_v_o_s->l ) return(1);
    for(i=0; i<=p_v_o_s->l; i++)
    {
        if ( p_v_o_s->c_a[i] != v_c_a[i] ) return(1);
    }

    return(0);
}
