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
 * Revision 1.1.4.2  1996/02/17  23:17:33  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:40  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:30:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:57  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:38:10  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:44:48  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:28:22  devrcs
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
** Routine to print a depiction of the tree
*/
void st_print_tree ();
void st_print_tree (tree, indent)
    st_node_t *tree;
    int  indent;
{
    int i;
    if (tree == NULL) return;
    for (i = 0; i < indent; i++) printf("    ");
    printf("%s\n",tree->name);
    st_print_tree(tree->left, indent + 1);
    st_print_tree(tree->right, indent + 1);
}


/*	
** Create a tree with a few nodes
*/
st_node_t *st_make_tree()
{
    st_node_t *root = (st_node_t *)malloc(sizeof(st_node_t));
    strcpy(root->name,"Root Node");

    /* left subtree node */
    root->left =  (st_node_t *)malloc(sizeof(st_node_t));
    strcpy(root->left->name,"Left subtree");    

    /* left subtree children */
    root->left->right = NULL;
    root->left->left = (st_node_t *)malloc(sizeof(st_node_t));
    strcpy(root->left->left->name,"Child of left subtree");    
    root->left->left->left = NULL;
    root->left->left->right = NULL;

    /* right subtree node */
    root->right =  (st_node_t *)malloc(sizeof(st_node_t));
    strcpy(root->right->name,"Right subtree");    
    root->right->left = NULL;
    root->right->right = NULL;

    return root;
}

int client_aux(h,passes)
      handle_t h;
      int passes;
{
    st_node_t *tree;
    st_node_t *subtree;
    error_status_t st;

    /* setup and print original tree */
    tree = st_make_tree();
    printf("Original Tree:\n");
    i_h = h;
    st_print_tree(tree, 1);

    /* call the prune routine */
    subtree = st_prune_left (tree);

    /* print the resulting trees */
    printf("Pruned Tree:\n");
    st_print_tree(tree, 1);

    printf("Pruned subtree:\n");
    st_print_tree(subtree, 1);

    return 0;
}
