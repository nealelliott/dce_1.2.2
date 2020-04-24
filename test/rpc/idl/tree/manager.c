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
 * Revision 1.1.4.2  1996/02/17  23:18:01  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:37:04  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:31:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:23  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:40:18  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:46:28  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:28:55  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <tree.h>
#include <test_common.h>

static int traverse();
static int left();
static int right();

static ndr_small_int op1
#ifdef NIDL_PROTOTYPES
(
    handle_t h,
    tree_t *tree,
    ndr_long_int smallest,
    ndr_long_int largest,
    ndr_long_int checksum
)
#else
(h, tree, smallest, largest, checksum)
    handle_t h;
    tree_t *tree;
    ndr_long_int smallest;
    ndr_long_int largest;
    ndr_long_int checksum;
#endif
{
    ndr_small_int failures = 0;
    ndr_long_int s, l, c;

    s = left(tree);
    l = right(tree);
    c = traverse(tree);

#ifdef DEBUG
    printf("In op1: smallest is 0x%lx, largest is 0x%lx, checksum is 0x%lx\n",
           s, l, c);
#endif

    if (s != smallest) failures++;
    if (l != largest) failures++;
    if (c != checksum) failures++;

    if (failures) printf("FAILURE: 0x%lx 0x%lx 0x%lx\n", smallest, largest, 
                         checksum);

    return failures;
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
    while (tree->left) tree = tree->left;
    return tree->key;
}

static int right(tree)
    tree_t *tree;
{
    while (tree->right) tree = tree->right;
    return tree->key;
}

globaldef tree_v0_0_epv_t tree_v0_0_m_epv = {op1};
