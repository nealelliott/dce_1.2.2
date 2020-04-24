/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: util_xmit.c,v $
 * Revision 1.1.4.2  1996/02/17  23:18:52  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:37:30  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:32:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:17:03  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:59:12  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:48:09  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:33:41  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <xmitas_repas.h>

static void free_list_recursively();         /* auxiliary function */

void list_t_to_xmit(list, structure)
list_t *list;
trans_t **structure;
{
    int count = 0;
    list_t *l = list;

    /* allocate the structure */
    *structure = (trans_t *)malloc(sizeof(trans_t));

    /* copy the values from the list to the array */
    while (l)
    {
        (*structure)->values[count++] = l->value;
        l = (list_t *)l->next;
    }
    (*structure)->last = (ndr_long_int)(count-1);
}

void list_t_from_xmit(structure, list)
trans_t *structure;
list_t *list;
{
    int index = 0;

    /* reconstruct the linked list from the array */
    do
    {
        list->value = structure->values[index++];

        if (index <= structure->last)
            list->next = (list_t *)malloc(sizeof(list_t));
        else list->next = NULL;

        list = (list_t *)list->next;
    } while (index <= structure->last);
}

void list_t_free_inst(list)
list_t *list;
{
    if (list->next != NULL) 
       free_list_recursively(list->next);
}

void list_t_free_xmit(structure)
trans_t *structure;
{
    free(structure);
}

static void free_list_recursively(l)
list_t *l;
{
    if (l->next) free_list_recursively(l->next);
    free(l);
}

