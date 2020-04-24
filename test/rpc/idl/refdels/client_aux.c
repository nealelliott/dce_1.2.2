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
 * Revision 1.1.7.2  1996/02/17  23:16:42  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:04  marty]
 *
 * Revision 1.1.7.1  1995/12/11  19:28:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:13  root]
 * 
 * Revision 1.1.4.1  1994/06/29  21:00:02  ohara
 * 	add p_node to return  statement in build_node
 * 	[1994/05/12  21:01:43  ohara]
 * 
 * Revision 1.1.2.2  1993/08/11  16:28:47  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:16:20  ganni]
 * 
 * $EndLog$
 */
/*
*/

#include <stdio.h>
#include <refdels.h>
#include <test_common.h>

#define TREE_DEPTH 13

static idl_long_int node_label;

static tree_t *build_node(depth)
    int depth;
{
    tree_t *p_node;

    p_node = (tree_t *)malloc(sizeof(tree_t));
    node_label++;
    p_node->key = node_label;
    if (depth == 1)
    {
        p_node->left = NULL;
        p_node->right = NULL;
        return p_node;
    }
    p_node->left = build_node(depth - 1);
    p_node->right = build_node(depth - 1);
    return p_node;
}

static idl_long_int check_node(p_node)
    tree_t *p_node;
{
    if (p_node->right == NULL)
        return(p_node->left->key);
    else
        return(check_node(p_node->left) + check_node(p_node->right));
}

static idl_long_int local_deletes;

static void count_frees(ptr)
    idl_void_p_t ptr;
{
    local_deletes++;
    free(ptr);
}

static free_nodes(p_node)
    tree_t *p_node;
{
    if (p_node->left != NULL)
        free_nodes(p_node->left);
    if (p_node->right != NULL)
        free_nodes(p_node->right);
    free(p_node);
}

static idl_boolean equal_leaves(p_node)
    tree_t *p_node;
{
    if (p_node->left->left == NULL)
        return(p_node->left->key == p_node->right->key);
    else
        return(equal_leaves(p_node->left) && equal_leaves(p_node->right));
}

int client_aux(h,passes)
    int passes;handle_t h;
{
    tree_t *p_root;
    int failures = 0;
    idl_long_int local_checksum, remote_checksum, remote_deletes;
    idl_long_int val;

    node_label = 0;
    local_deletes = 0;
    rpc_ss_set_client_alloc_free(malloc, count_frees);

    /* Build a node in which all the leaves are at the same depth
        and all non-leaf nodes have two children */
    p_root = build_node(TREE_DEPTH);

    prune(h, p_root, &remote_deletes, &remote_checksum);

    /* Returned tree has all leaves that were right-hand children deleted.
        Checksum is sum of keys of remaining leaves */
    local_checksum = check_node(p_root);
    if (local_checksum != remote_checksum)
    {
        printf("Local checksum %d != remote checksum %d\n",
                local_checksum, remote_checksum);
        failures++;
    }
    if (local_deletes != remote_deletes)
    {
        printf("Local deletes %d != remote deletes %d\n",
                local_deletes, remote_deletes);
        failures++;
    }
    free_nodes(p_root);

    node_label = 0;
    /* Build a node in which all the leaves are at the same depth
        and all non-leaf nodes have two children */
    p_root = build_node(TREE_DEPTH);

    ltor(h, p_root);
    /* Returned tree should have left and right leaf children equal */
    if ( ! equal_leaves(p_root) )
    {
        printf("Unequal leaves\n");
        failures++;
    }
    free_nodes(p_root);

    val = TREE_DEPTH;
    triple(h, &val);
    if ( val != 3*TREE_DEPTH )
    {
        printf("val %d != %d\n", val, TREE_DEPTH);
        failures++;
    }

    return(failures);
}
