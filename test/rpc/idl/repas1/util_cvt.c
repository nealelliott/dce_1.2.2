/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: util_cvt.c,v $
 * Revision 1.1.4.2  1996/02/17  23:16:56  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:12  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:28:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:22  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:34:05  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:41:52  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:27:07  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <repas1.h>

static void free_list_recursively();         /* auxiliary function */

void trans_t_from_local(list, xmit_struct)
list_t *list;
trans_t **xmit_struct;
{
    int count = 0;
    list_t *l = list;

    /* allocate the structure */
    *xmit_struct = (trans_t *)malloc(sizeof(trans_t));

    /* copy the values from the list to the array */
    while (l)
    {
        (*xmit_struct)->values[count++] = l->value;
        l = l->next;
    }
    (*xmit_struct)->last = (ndr_long_int)(count-1);
}

void trans_t_to_local(xmit_struct, list)
trans_t *xmit_struct;
list_t *list;
{
    int index = 0;

    /* reconstruct the linked list from the array */
    do
    {
        list->value = xmit_struct->values[index++];

        if (index <= xmit_struct->last)
            list->next = (list_t *)malloc(sizeof(list_t));
        else list->next = NULL;

        list = list->next;
    } while (index <= xmit_struct->last);
}

void trans_t_free_local(list)
list_t *list;
{
    if (list->next != NULL) 
       free_list_recursively(list->next);
}

void trans_t_free_inst(xmit_struct)
trans_t *xmit_struct;
{
    free(xmit_struct);
}

static void free_list_recursively(l)
list_t *l;
{
    if (l->next) free_list_recursively(l->next);
    free(l);
}

