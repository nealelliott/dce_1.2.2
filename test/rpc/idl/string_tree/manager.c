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
 * Revision 1.1.4.2  1996/02/17  23:17:35  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:41  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:30:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:58  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:38:15  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:44:52  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:28:23  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <string_tree.h>
#include <test_common.h>

/*	
** Prune the left subtree of the specified tree and return it as the function
** value.  
*/
st_node_t *_st_prune_left (tree)
    /* [in,out] */  st_node_t	*tree;
{
    /* Remove the left subtree */
    st_node_t *left_sub_tree = tree->left;

    /* Replace with a new subtree */ 
    tree->left = (st_node_t *)rpc_ss_allocate(sizeof(st_node_t));
    strcpy(tree->left->name,"New subtree");
    tree->left->left = (st_node_t *)NULL;
    tree->left->right = (st_node_t *)NULL;

    /* return the subtree */
    return left_sub_tree;
}

globaldef string_tree_v0_0_epv_t string_tree_v0_0_m_epv = {_st_prune_left};
