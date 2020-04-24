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
 * Revision 1.1.4.2  1996/02/17  23:13:14  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:10  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:00:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:29:27  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:48:57  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:29:54  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:24:04  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <conftree.h>
#include <test_common.h>

void s_largetosmall
(h, p_l, p_s)
    /* [in] */handle_t h;
    /* [in] */largenode *p_l;
    /* [out] */smallnode **p_s;
{
    smallnode *new_s, *old_s;
    int j;

    old_s = NULL;
    for( ; p_l != NULL; p_l = p_l->next)
    {
#ifdef DEBUG
        printf( "%d array elements ", p_l->array_size );
#endif
        new_s = (smallnode *)rpc_ss_allocate(sizeof(smallnode)+
                                    (p_l->array_size-1)*sizeof(long int));
        new_s->next = old_s;
        new_s->array_max = p_l->array_size - 1;
        for (j=0; j<p_l->array_size; j++)
        {
#ifdef DEBUG
            printf(" %d", p_l->array_data[j] );
#endif
            new_s->array_data[j] = p_l->array_data[j];
        }
#ifdef DEBUG
        printf( "\n" );
#endif
        old_s = new_s;
    }
    *p_s = old_s;
}

void s_smalltolarge
( h, p_s, p_l )
    /* [in] */handle_t h;
    /* [in] */smallnode *p_s;
    /* [out] */largenode **p_l;
{
    largenode *new_l, *old_l;
    int j;

    old_l = NULL;
    for( ; p_s != NULL; p_s = p_s->next)
    {
#ifdef DEBUG
        printf( "%d array elements ", p_s->array_max+1 );
#endif
        new_l = (largenode *)rpc_ss_allocate(sizeof(largenode)+
                                    (p_s->array_max)*sizeof(long int));
        new_l->next = old_l;
        new_l->array_size = p_s->array_max+1;
        for (j=0; j<p_s->array_max+1; j++)
        {
#ifdef DEBUG
            printf(" %d", p_s->array_data[j] );
#endif
            new_l->array_data[j] = p_s->array_data[j];
        }
#ifdef DEBUG
        printf( "\n" );
#endif
        old_l = new_l;
    }
    *p_l = old_l;
}

globaldef conftree_v0_0_epv_t conftree_v0_0_m_epv = {s_largetosmall,
                                                       s_smalltolarge};
