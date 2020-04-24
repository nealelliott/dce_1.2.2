/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: util_rep.c,v $
 * Revision 1.1.4.2  1996/02/17  23:18:50  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:37:30  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:32:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:17:03  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:59:07  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:48:05  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:33:40  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <xmitas_repas.h>

static void free_list_recursively();         /* auxiliary function */

void list_t_to_local(list, structure)
list_t *list;
local_t *structure;
{
    int count = 0;
    list_t *l = list;

    /* copy the values from the list to the array */
    while (l)
    {
        structure->values[count++] = l->value;
        l = (list_t *)l->next;
    }
    structure->last = (ndr_long_int)(count-1);
}

void list_t_from_local(structure, list)
local_t *structure;
list_t **list;
{
    int i = 0;
    list_t *node, *next, *root = NULL;

    /* reconstruct the linked list from the array */
    for (i = 0; i <= structure->last; i++)
    {
        node = (list_t *)malloc(sizeof(list_t));
        node->next = NULL;
        node->value = structure->values[i];
            
        if (root == NULL)
            root = next = node;
        else
        {
            next->next = node;
            next = node;
        }
    } 

    *list = root;
}


void list_t_free_local(structure)
local_t *structure;
{
    /* No content to free in the local type */
}

static void free_list_recursively(l)
list_t *l;
{
    if (l->next) free_list_recursively(l->next);
    free(l);
}

