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
 * Revision 1.1.4.2  1996/02/17  23:13:13  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:09  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:00:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:29:24  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:11:43  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:29:47  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:24:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <conftree.h>
#include <test_common.h>

#define LEAF_DEPTH 8

int client_aux(h,passes)
      handle_t h;
      int passes;
{
   int failures=0;
   error_status_t st;
   largenode *p_l, *new_l;
   smallnode *p_s, *new_s;
   int i,j;

    p_l = NULL;
    for (i=1; i<10; i++)
    {
        new_l = (largenode *)malloc(sizeof(largenode)+(i-1)*sizeof(long int));
        new_l->next = p_l;
        new_l->array_size = i;
        for (j=0; j<i; j++) new_l->array_data[j] = 100*i + j;
        p_l = new_l;
   }
    p_s = NULL;

   largetosmall ( h, p_l, &p_s );

    for( ; p_s != NULL; p_s = p_s->next)
    {
        printf( "%d array elements ", (p_s->array_max)+1 );
        for (j=0; j<=p_s->array_max; j++) printf(" %d", p_s->array_data[j] );
        printf( "\n" );
    }

    p_s = NULL;
    for (i=1; i<10; i++)
    {
        new_s = (smallnode *)malloc(sizeof(smallnode)+(i-1)*sizeof(long int));
        new_s->next = p_s;
        new_s->array_max = i - 1;
        for (j=0; j<i; j++) new_s->array_data[j] = 100*i + j;
        p_s = new_s;
   }
    p_l = NULL;

    smalltolarge ( h, p_s, &p_l );

    for( ; p_l != NULL; p_l = p_l->next)
    {
        printf( "%d array elements ", p_l->array_size);
        for (j=0; j<p_l->array_size; j++) printf(" %d", p_l->array_data[j] );
        printf( "\n" );
    }

    return failures;
}
