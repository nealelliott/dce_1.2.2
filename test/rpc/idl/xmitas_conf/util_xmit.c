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
 * Revision 1.1.4.2  1996/02/17  23:18:39  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:37:25  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:32:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:54  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:58:24  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:47:28  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:33:22  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <xmitas_conf.h>

static void free_list_recursively();         /* auxiliary function */

void list_t_to_xmit(list, xmit_struct)
list_t *list;
trans_t **xmit_struct;
{
    int count = 0;
    list_t *l = list;

    /* determine size of the array */
    while (l)
    {
        count++;
        l = l->next;
    }

    /* allocate the structure */
    *xmit_struct = (trans_t *)malloc(sizeof(trans_t)+(count-1)*sizeof(long));

    /* copy the values from the list to the array */
    l = list;
    count = 0;
    while (l)
    {
        (*xmit_struct)->values[count++] = l->value;
        l = l->next;
    }
    (*xmit_struct)->last = (ndr_long_int)(count-1);
}

void list_t_from_xmit(xmit_struct, list)
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

void list_t_free_inst(list)
list_t *list;
{
    if (list->next != NULL) 
       free_list_recursively(list->next);
}

void list_t_free_xmit(xmit_struct)
trans_t *xmit_struct;
{
/*	
    free(xmit_struct);
  */
}

static void free_list_recursively(l)
list_t *l;
{
    if (l->next) free_list_recursively(l->next);
    free(l);
}

