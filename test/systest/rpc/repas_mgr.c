/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: repas_mgr.c,v $
 * Revision 1.1.10.2  1996/03/09  20:51:23  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:29:11  marty]
 *
 * Revision 1.1.10.1  1995/12/13  21:55:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:19  root]
 * 
 * Revision 1.1.6.2  1993/07/09  13:06:47  root
 * 	Initial King Kong branch
 * 	[1993/07/09  13:06:19  root]
 * 
 * Revision 1.1.4.2  1993/03/31  12:33:59  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.3.2  1992/06/11  19:57:15  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:04:28  mcfarland]
 * 
 * Revision 1.1.1.2  1992/05/28  15:14:18  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1  1992/01/19  14:37:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <repas.h>
#include <stdio.h>

ndr_long_int xmitas$sum(h, list)
handle_t h;
list_t list;
{
    ndr_long_int sum = list.value;    /* every list has at least one element */

    while (list.next)
    {
        list = *(list.next);
        sum += list.value;
    }

    return sum;
}

ndr_long_int xmitas$sum_out(h, list)
handle_t h;
list_t *list;
{
    int i;
    ndr_long_int total = 0;    
    int num_elems = (random() % MAXELEMENTS) + 1;
    list_t *l = list;

    for (i = 0; i < num_elems; i++)
    {
        l->value = i * 17;
        total += l->value;
        if (i < num_elems - 1) l->next = (list_t *)malloc(sizeof(list_t));
        else l->next = NULL;

        l = l->next;
    }

    return total;
}

ndr_long_int xmitas$sum_in_out(h, list, in_total)
handle_t h;
list_t *list;
ndr_long_int in_total;
{
    int i;
    ndr_long_int sum = 0;    
    ndr_long_int total = 0;    
    int num_elems = (random() % MAXELEMENTS) + 1;
    list_t *l = list;
      
    /* Check input list */
    while (l)
    {
        sum += l->value;
        l = l->next;
    }

    /* Free the in rep */
    trans_t_free_local(list);
    list->next = NULL;

    if (sum != in_total)
        printf("***FAILURE***: xmitas$sum_in_out recieved %d, expected %d\n",sum,in_total);

    /* Recreate list with a new set of elements */
    l = list; 
    for (i = 0; i < num_elems; i++)
    {
        l->value = i * 38;
        total += l->value;
        if (i < num_elems - 1) l->next = (list_t *)malloc(sizeof(list_t));
        else l->next = NULL;

        l = l->next;
    }

    return total;
}
