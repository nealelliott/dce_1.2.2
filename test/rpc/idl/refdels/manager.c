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
 * Revision 1.1.4.2  1996/02/17  23:16:44  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:05  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:28:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:14  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:28:52  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:16:24  ganni]
 * 
 * $EndLog$
 */
/*
*/

#include <stdio.h>
#include <refdels.h>
#include <test_common.h>

static void subprune(p_node, p_num_deletes, p_checksum)
    tree_t *p_node;
    idl_long_int *p_num_deletes;
    idl_long_int *p_checksum;
{
    /* If the children of this node are leaves,
        add the key of the left child into the checksum
        and delete the right child */
    if (p_node->left->left == NULL)
    {
        *p_checksum += p_node->left->key;
        rpc_ss_free(p_node->right);
        p_node->right = NULL;
        (*p_num_deletes)++;
    }
    else
    {
        subprune(p_node->left, p_num_deletes, p_checksum);
        subprune(p_node->right, p_num_deletes, p_checksum);
    }
}

void s_prune(h, p_root, p_num_deletes, p_checksum)
    handle_t h;
    tree_t *p_root;
    idl_long_int *p_num_deletes;
    idl_long_int *p_checksum;
{
    *p_num_deletes = 0;
    *p_checksum = 0;
    subprune(p_root, p_num_deletes, p_checksum);
}

static void subltor(p_node)
    tree_t *p_node;
{
    if (p_node->left->left == NULL)
        p_node->right->key = p_node->left->key;
    else
    {
        subltor(p_node->left);
        subltor(p_node->right);
    }
}

void s_ltor(h, p_root)
    handle_t h;
    tree_t *p_root;
{
    subltor(p_root);
}

void s_triple(h, p_val)
    handle_t h;
    idl_long_int *p_val;
{
    *p_val *= 3;
}

/*globaldef*/ refdels_v0_0_epv_t refdels_v0_0_m_epv = {s_prune,
                                                        s_ltor,
                                                        s_triple};
