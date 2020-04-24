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
 * Revision 1.1.8.2  1996/02/17  23:16:54  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:11  marty]
 *
 * Revision 1.1.8.1  1995/12/11  19:28:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:20  root]
 * 
 * Revision 1.1.6.2  1993/06/15  21:06:26  sommerfeld
 * 	Add #include of <test_common.h>
 * 	[1993/06/14  21:22:46  sommerfeld]
 * 
 * Revision 1.1.2.2  1993/01/07  23:33:52  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:41:42  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:27:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <repas1.h>
#include <stdio.h>
#include <test_common.h>

ndr_long_int _xmitas_sum(h, list)
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

ndr_long_int _xmitas_sum_out(h, list)
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

ndr_long_int _xmitas_sum_in_out(h, list, in_total)
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
        printf("***FAILURE***: xmitas_sum_in_out recieved %d, expected %d\n",sum,in_total);

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

globaldef repas1_v0_0_epv_t repas1_v0_0_m_epv = {
      _xmitas_sum,
      _xmitas_sum_out,
      _xmitas_sum_in_out,
      _xmitas_sum_in_out
      };
