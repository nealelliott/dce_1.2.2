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
 * Revision 1.1.10.2  1996/02/17  23:16:58  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:13  marty]
 *
 * Revision 1.1.10.1  1995/12/11  19:28:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:24  root]
 * 
 * Revision 1.1.8.2  1993/06/15  21:06:40  sommerfeld
 * 	Add #include of test_common.h
 * 	[1993/06/14  21:03:49  sommerfeld]
 * 
 * Revision 1.1.3.2  1993/01/07  23:34:13  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:41:59  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:27:19  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <repas2.h>
#include <stdio.h>
#include <dce/nbase.h>
#include <test_common.h>

int client_aux(h,passes)
      handle_t h;
      int passes;
{
    error_status_t st;
    list_t the_list, *l = &the_list;
    int num_elems = (random() % (MAXELEMENTS-1)) + 1;
    int i;
    int total = 0;
    int out_total = 0;
    int ret_val;
    
    for (i = 0; i < num_elems; i++)
    {
        l->value = i * 77;
        total += l->value;
        if (i < num_elems - 1) l->next = (list_t *)malloc(sizeof(list_t));
        else l->next = NULL;

        l = l->next;
    }

    /* [ref] pointer to [repas] type */
    ret_val = xmitas_sum(h, &the_list);
    if (total != ret_val)
    {
        printf("****FAILURE****: %d returned from xmitas_sum, expected %d\n",
              ret_val, total);
        return 1;
    }       

    /* [ptr] pointer to [repas] type */
    ret_val = xmitas_sum2(h, &the_list);
    if (total != ret_val)
    {
        printf("****FAILURE****: %d returned from xmitas_sum2, expected %d\n",
              ret_val, total);
        return 1;
    }       

    /*	
     * [out] represent as type as linked list
     */
    ret_val = xmitas_sum_out(h, &the_list);

    /* Calculate total of list returned */
    for ((out_total = 0, l = &the_list); l != NULL; l = l->next)
        out_total += l->value;

    /* If total returned is not same as list contents, error */
    if (out_total != ret_val)
    {
        printf("****FAILURE****: %d returned from xmitas_sum_out, expecte %d\n",
              ret_val, out_total);
        return 1;
    }       


    /*	
     * [in,out] represent as type as linked list
     */
    ret_val = xmitas_sum_in_out(h, &the_list, out_total);

    /* Calculate total of list returned */
    for ((total = 0, l = &the_list); l != NULL; l = l->next)
        total += l->value;

    /* If total returned is not same as list contents, error */
    if (total != ret_val)
    {
        printf("****FAILURE****: %d returned from xmitas_sum_in_out, expecte %d\n",
              ret_val, total);
        return 1;
    }       

    return 0;
}
