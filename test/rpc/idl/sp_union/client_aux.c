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
 * Revision 1.1.8.2  1996/02/17  23:17:14  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:26  marty]
 *
 * Revision 1.1.8.1  1995/12/11  19:29:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:42  root]
 * 
 * Revision 1.1.6.2  1993/06/15  21:07:57  sommerfeld
 * 	Changed "spu_t" to "spux_t" because of name collision on HP-UX.
 * 	[1993/06/14  21:05:21  sommerfeld]
 * 
 * Revision 1.1.2.2  1993/01/07  23:35:53  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:43:11  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:27:36  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <sp_union.h>
#include <test_common.h>


void print_depth(spu_chain)
      spux_t *spu_chain;
{
    int counted_depth;

    counted_depth = 0;
    while (spu_chain->c == 'p')
    {
        counted_depth++;
        spu_chain = spu_chain->spu_s.p_spu;
    }
    printf(" - counted depth %d, depth in node %d\n",
           counted_depth, spu_chain->spu_s.depth);
}

#define BUILD_DEPTH 10

int client_aux(h,passes)
      handle_t h;
      int passes;
{
    error_status_t st;
    spux_t *new_spu, *old_spu;
    int i;

    old_spu = (spux_t *)malloc(sizeof(spux_t));
    old_spu->c = 'x';
    old_spu->spu_s.depth = BUILD_DEPTH;
    for(i=0; i<BUILD_DEPTH; i++)
    {
        new_spu = (spux_t *)malloc(sizeof(spux_t));
        new_spu->c = 'p';
        new_spu->spu_s.p_spu = old_spu;
        old_spu = new_spu;
    }


    printf("Before call");
    print_depth(new_spu);
    my_oper(h, &new_spu);
    printf("After call");
    print_depth(new_spu);

    return 0;
}
