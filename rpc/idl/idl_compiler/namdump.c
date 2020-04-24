/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: namdump.c,v $
 * Revision 1.1.8.2  1996/02/18  18:52:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:42  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:20:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:12:37  root]
 * 
 * Revision 1.1.4.2  1993/07/07  20:01:59  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:33:44  ganni]
 * 
 * $EndLog$
 */
/*
**
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**  Digital Equipment Corporation, Maynard, Mass.
**  All Rights Reserved.  Unpublished rights reserved
**  under the copyright laws of the United States.
**
**  The software contained on this media is proprietary
**  to and embodies the confidential technology of
**  Digital Equipment Corporation.  Possession, use,
**  duplication or dissemination of the software and
**  media is authorized only pursuant to a valid written
**  license from Digital Equipment Corporation.
**
**  RESTRICTED RIGHTS LEGEND   Use, duplication, or
**  disclosure by the U.S. Government is subject to
**  restrictions as set forth in Subparagraph (c)(1)(ii)
**  of DFARS 252.227-7013, or in FAR 52.227-19, as
**  applicable.
**
**
**  NAME:
**
**      NAMDUMP.C
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**  This is the dumper for the name table.
**
**  VERSION: DCE 1.0
**
*/


#include <nidl.h>
#include <ctype.h>

#include <errors.h>
#include <nametbl.h>
#include <namtbpvt.h>
#include <nidlmsg.h>

extern NAMETABLE_n_t_p NAMETABLE_root;
extern NAMETABLE_temp_name_t * NAMETABLE_temp_chain;

/******************************************************************************/
/*                                                                            */
/*                  N A M E T A B L E          D U M P E R S                  */
/*                                                                            */
/******************************************************************************/

/*
 * Function: Dumps the binding information for a node of the nametable tree.
 *
 * Inputs:   Node of the tree to be dumped.
 *
 * Outputs:  None.
 *
 * Functional Value: None.
 *
 * Notes:    None.
 *
 */

static void NAMETABLE_dump_bindings_4_node
#ifdef PROTO
(
    NAMETABLE_binding_n_t * pp
)
#else
(pp)
    NAMETABLE_binding_n_t * pp;
#endif

{
    NAMETABLE_binding_n_t * p;

    p = pp;

    while (p != NULL) {
        printf ("\tBinding node at: %p \n", p);
        printf ("\t    bindingLevel: %d\n", p->bindingLevel);
        printf ("\t    theBinding: %p \n", p->theBinding);
        printf ("\t    nextBindingThisLevel: %p \n", p->nextBindingThisLevel);
        printf ("\t    oldBinding: %p \n", p->oldBinding);
        printf ("\t    boundBy: \"%s\" ( %p )\n\n", p->boundBy->id, p->boundBy);
        if (p->oldBinding != NULL) {
            printf ("\n");
        };
        p = p->oldBinding;
    };
}



/*
 * Function: Dumps a node of the nametable tree.
 *
 * Inputs:   Node of the tree to be dumped.
 *
 * Outputs:  Text is output to stdout.
 *
 * Functional Value: None.
 *
 * Notes:    None.
 *
 */

static void NAMETABLE_dump_node
#ifdef PROTO
(
    NAMETABLE_n_t_p node
)
#else
(node)
    NAMETABLE_n_t_p node;
#endif

{
    printf ("\n\"%s\" ( %p ) :\n",        /* "FOO" (0023ad8C) : */
            node->id,                   /* The id string */
            (char *) node );            /* The address of the node */

    if (node->parent != NULL) {
        printf ("        Parent: ( %p ) \"%s\"\n", /*     Parent:  ( 01234abc ) "bar" */
                node->parent,             /* The address of the parent */
                node->parent->id);        /* The id string of parent */
    } else {
        printf ("        *** NAMETABLE ROOT ***\n");     /* Handle the NULL case. */
    };

    if (node->left != NULL) {
        printf ("        Left:  ( %p ) \"%s\"\n", /*     Left:  ( 01234abc ) "bar" */
                node->left,             /* The address of the left child */
                node->left->id);        /* The id string of l. child */
    } else {
        printf ("        Left:  NULL\n");     /* Handle the NULL case. */
    };

    if (node->right != NULL) {
        printf ("        Right: ( %p ) \"%s\"\n", /*     Right: (01234abc) "bar" */
                node->right,            /* The address of the right child */
                node->right->id);       /* The id string of r. child */
    } else {
        printf ("        Right: NULL\n");   /* Handle the NULL case. */
    };


    if (node->bindings != NULL) {
        printf ("    Head of binding chain : %p \n",
                node->bindings);
        printf ("    Binding information for \"%s\"\n",
                node->id);
        NAMETABLE_dump_bindings_4_node (node->bindings);
    } else {
        printf ("    No binding chain\n");
    };

    if (node->tagBinding != NULL) {
        printf ("    Structure with this tag: %p \n",
                node->tagBinding);
        printf ("    Tag binding information for \"%s\"\n",
                node->id);
        NAMETABLE_dump_bindings_4_node (node->tagBinding);
    } else {
        printf ("    No structures with this tag\n");
    };
}




/*
 * Function: Recursively dumps all the nodes of a nametable tree.
 *           First dumps the left subtree bottom up, then the root node,
 *           then the right subtree, resulting in an alphabetical dump.
 *
 * Inputs:   Root node of the tree to be dumped.
 *
 * Outputs:  None.
 *
 * Functional Value: None.
 *
 * Notes:    None.
 *
 */

static void NAMETABLE_dump_nodes
#ifdef PROTO
(
    NAMETABLE_n_t_p node
)
#else
(node)
    NAMETABLE_n_t_p node;
#endif
{
    if (node->left != NULL) {
        NAMETABLE_dump_nodes (node->left);
    };

    NAMETABLE_dump_node (node);

    if (node->right != NULL) {
        NAMETABLE_dump_nodes (node->right);
    };
}


/*
 * Function: Dump the list of temporary name table nodes.
 *
 * Inputs:   NAMETABLE_temp_chain (Implicit)
 *
 * Outputs:
 *
 * Functional Value:
 *
 * Notes:
 *
 */

void NAMETABLE_dump_temp_node_list ()
{
NAMETABLE_temp_name_t * tb;

    if (!NAMETABLE_temp_chain) {
        printf ("\n\nThere are no temporary names.\n");
        return;
    } else {
        printf ("\n\nTemporary name chain:\n");
    }

    for (tb = NAMETABLE_temp_chain; tb; tb = tb->next) {
        printf ("    Chain block: %p NT node ( %p ): \"%s\"\n",
            tb, tb->node, tb->node->id );
    }

}



/*
 * Function: Dump a name table in human-readable form.
 *
 * Inputs:   name_table - the table to be dumped. (Implicit)
 *
 * Outputs:
 *
 * Functional Value:
 *
 * Notes:
 *
 */

void NAMETABLE_dump_tab ()
{
    NAMETABLE_dump_nodes( NAMETABLE_root );
    NAMETABLE_dump_temp_node_list();
}
