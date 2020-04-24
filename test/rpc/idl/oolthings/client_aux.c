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
 * Revision 1.1.4.2  1996/02/17  23:15:26  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:17  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:26:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:08  root]
 * 
 * Revision 1.1.2.2  1993/01/07  20:33:03  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:36:18  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:25:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <oolthings.h>

int client_aux(handle)
handle_t handle;
{
    conf_struct *p_c_s;
    open_struct *p_o_s;
    fixed_array f_a;
    long *c_a;
    long i, f, l;
              
    i_h = handle;
    p_c_s = (conf_struct *)malloc(sizeof(conf_struct) 
                                   + ((CONF_SIZE-1)*sizeof(long)));
    p_c_s->s = CONF_SIZE;
    for( i=0; i<CONF_SIZE; i++)
    {
        p_c_s->c_a[i] = 16 * (i + 1);
    }
    op0( p_c_s, f_a );
    for(i=0; i<FIXED_SIZE; i++)
    {
        if (f_a[i] != 16 * p_c_s->c_a[i % CONF_SIZE]) return(1);
    }

    c_a = (long *)malloc(CONF_SIZE*sizeof(long));
    for( i=0; i<CONF_SIZE; i++)
    {
        c_a[i] = 16 * (i + 1);
    }
    op1( CONF_SIZE, c_a, &f, &l, f_a );
    if ( l - f + 1 != CONF_SIZE ) return(1);
    for( i=0; i<CONF_SIZE; i++)
    {
        if (c_a[i] != f_a[f+i]) return(1);
    }

    p_o_s = (open_struct *)malloc(sizeof(open_struct) 
                                   + ((CONF_SIZE-1)*sizeof(long)));
    c_a = (long *)malloc(CONF_SIZE*sizeof(long));
    p_o_s->s = CONF_SIZE;
    p_o_s->f = 3;
    p_o_s->l = 7;
    for(i=p_o_s->f; i<=p_o_s->l; i++)
    {
        p_o_s->c_a[i] = 16 * (i + 1);
    }
    op2( p_o_s, CONF_SIZE, &f, &l, c_a );
    if ( f != p_o_s->f ) return(1);
    if ( l != p_o_s->l ) return(1);
    for(i=p_o_s->f; i<=p_o_s->l; i++)
    {
        if ( p_o_s->c_a[i] != c_a[i] ) return(1);
    }

    return(0);
}
