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
 * Revision 1.1.4.2  1996/02/17  23:18:00  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:37:02  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:31:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:22  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:40:14  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:46:25  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:28:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <tree.h>
#include <test_common.h>

static void insert();
static void free_all();
static int traverse();
static int left();
static int right();

int client_aux(h)
handle_t h;
{
    char buf[100];
    tree_t *root, *new;
    int i, n, failures = 0;
    ndr_long_int s, l, c;

    root = (tree_t *)malloc(sizeof(tree_t));
    root->key = 0;
    root->left = root->right = NULL;

    n = random()%2000;

#ifdef DEBUG
    printf("Building a binary tree with %d nodes\n", n + 1);
#endif

    for (i = 0; i < n; i++)
    {
        new = (tree_t *)malloc(sizeof(tree_t));
        new->key = random(); new->key <<= 1;
        new->left = new->right = NULL;
        insert(root, new);
    }

    s = left(root);
    l = right(root);
    c = traverse(root);
#ifdef DEBUG
    printf("Smallest is 0x%lx, largest is 0x%lx, checksum is 0x%lx\n",
            s, l, c);
#endif

    printf("Calling op1\n");
    failures += op1(h, root, s, l, c);

    free_all(root);

    return failures;
}

static void free_all(tree)
    tree_t *tree;
{
    if (tree == NULL) return;
    free_all(tree->left);
    free_all(tree->right);
    free(tree);
}

static void insert(root, node)
    tree_t *root;
    tree_t *node;
{
    if (node->key <= root->key)
    {
        if (root->left) insert(root->left, node);
        else root->left = node;
    }
    else if (root->right) insert(root->right, node);
    else root->right = node;
}

static int traverse(tree)
    tree_t *tree;
{
    if (!tree) return 0;
    else return traverse(tree->left) + tree->key + traverse(tree->right);
}

static int left(tree)
    tree_t *tree;
{
    while(tree->left) tree = tree->left;
    return tree->key;
}

static int right(tree)
    tree_t *tree;
{
    while (tree->right) tree = tree->right;
    return tree->key;
}

