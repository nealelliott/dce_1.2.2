/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: union_aux.c,v $
 * Revision 1.1.8.2  1996/03/09  20:51:42  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:29:25  marty]
 *
 * Revision 1.1.8.1  1995/12/13  21:56:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:28  root]
 * 
 * Revision 1.1.6.2  1993/07/09  13:40:55  root
 * 	Initial King Kong branch
 * 	[1993/07/09  13:40:25  root]
 * 
 * Revision 1.1.4.3  1993/04/23  14:43:03  giza
 * 	dce1.0.2 merge up
 * 	[1993/04/23  13:45:02  giza]
 * 
 * Revision 1.1.4.2  1993/03/31  12:40:26  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.3.2  1992/06/11  19:59:50  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:06:43  mcfarland]
 * 
 * Revision 1.1.1.2  1992/05/28  15:15:09  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1  1992/01/19  14:36:23  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <sp_union.h>


void print_depth(spu_chain)
      sp_union_t *spu_chain;
{
    int counted_depth;

    counted_depth = 0;
    while (spu_chain->c == 'p')
    {
        counted_depth++;
        spu_chain = spu_chain->spu_s.p_spu;
    }
    printf("- counted depth %d, depth in node %d\n",
           counted_depth, spu_chain->spu_s.depth);
}

#define BUILD_DEPTH 10

int client_aux(h,passes)
      handle_t h;
      int passes;
{
    error_status_t st;
    sp_union_t *new_spu, *old_spu;
    int i;

    old_spu = (sp_union_t *)malloc(sizeof(sp_union_t));
    old_spu->c = 'x';
    old_spu->spu_s.depth = BUILD_DEPTH;
    for(i=0; i<BUILD_DEPTH; i++)
    {
        new_spu = (sp_union_t *)malloc(sizeof(sp_union_t));
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
