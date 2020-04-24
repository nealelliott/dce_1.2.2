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
 * Revision 1.1.4.2  1996/02/17  23:16:46  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:07  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:28:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:16  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:33:26  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:41:25  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:26:57  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <refptrs.h>
#include <test_common.h>


#define LEAF_DEPTH 8

int client_aux(h,passes)
      handle_t h;
      int passes;
{
    error_status_t st;
    refnode_t refnode;
    mutnode_t notchanged;
    mutnode_t changed;


    refnode.rn_char = 'X';
    refnode.rn_ptr = (refleaf_t *)malloc(sizeof(refleaf_t));
    refnode.rn_ptr->rl_char = 'Y';
    refnode.rn_ptr->rl_ptr = (leafchar_t *)malloc(sizeof(leafchar_t));
    *(refnode.rn_ptr->rl_ptr) = 'Z';

    notchanged.mn_char = 'X';
    notchanged.mn_ptr = (refleaf_t *)malloc(sizeof(refleaf_t));
    notchanged.mn_ptr->rl_char = 'Y';
    notchanged.mn_ptr->rl_ptr = (leafchar_t *)malloc(sizeof(leafchar_t));
    *(notchanged.mn_ptr->rl_ptr) = 'Z';

    changed.mn_char = 'X';
    changed.mn_ptr = (refleaf_t*)malloc(sizeof(refleaf_t));
    changed.mn_ptr->rl_char = 'Y';
    changed.mn_ptr->rl_ptr = (leafchar_t *)malloc(sizeof(leafchar_t));
    *(changed.mn_ptr->rl_ptr) = 'Z';


    op1( h, &refnode, &notchanged, &changed );
    printf( "Expecting L M S - received %c %c %c\n",
            refnode.rn_char, refnode.rn_ptr->rl_char, 
            *(refnode.rn_ptr->rl_ptr) );
    printf( "Expecting S & D - received %c %c %c\n",
            notchanged.mn_char, notchanged.mn_ptr->rl_char, 
            *(notchanged.mn_ptr->rl_ptr) );
    printf( "Expecting G W R - received %c %c %c\n",
            changed.mn_char, changed.mn_ptr->rl_char, 
            *(changed.mn_ptr->rl_ptr) );
    return 0;
}
