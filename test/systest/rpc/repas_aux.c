/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: repas_aux.c,v $
 * Revision 1.1.10.2  1996/03/09  20:51:21  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:29:09  marty]
 *
 * Revision 1.1.10.1  1995/12/13  21:55:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:17  root]
 * 
 * Revision 1.1.6.2  1993/07/09  13:04:05  root
 * 	Initial King Kong branch
 * 	[1993/07/09  13:03:37  root]
 * 
 * Revision 1.1.4.2  1993/03/31  12:33:38  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.3.2  1992/06/11  19:57:01  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:04:14  mcfarland]
 * 
 * Revision 1.1.1.2  1992/05/28  15:14:13  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1  1992/01/19  14:37:21  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <repas.h>
#include <stdio.h>
#include <dce/nbase.h>
extern char buf[128];

int client_aux(h,passes)
      handle_t h;
      int passes;
{
    error_status_t st;
    list_t the_list, *l = &the_list;
    int num_elems; 
    int i;
    int total = 0;
    int out_total = 0;
    int ret_val;
    
    num_elems = (random() % MAXELEMENTS) + 1;
    for (i = 0; i < num_elems; i++)
    {
        l->value = i * 77;
        total += l->value;
        if (i < num_elems - 1) l->next = (list_t *)malloc(sizeof(list_t));
        else l->next = NULL;

        l = l->next;
    }

    ret_val = xmitas$sum(h, the_list);
    if (total != ret_val)
    {
        sprintf(buf, 
		"****FAILURE****: %d returned from xmitas$sum, expected %d\n", 
		ret_val, total);
	xx_error(buf);
        return 1;
    }       

    /*  
     * [out] represent as type as linked list
     */
    ret_val = xmitas$sum_out(h, &the_list);

    /* Calculate total of list returned */
    for ((out_total = 0, l = &the_list); l != NULL; l = l->next)
        out_total += l->value;

    /* If total returned is not same as list contents, error */
    if (out_total != ret_val)
    {
        sprintf(buf, "****FAILURE****: %d returned from xmitas$sum_out, expecte %d\n", ret_val, out_total);
	xx_error(buf);
        return 1;
    }       


    /*  
     * [in,out] represent as type as linked list
     */
    ret_val = xmitas$sum_in_out(h, &the_list, out_total);

    /* Calculate total of list returned */
    for ((total = 0, l = &the_list); l != NULL; l = l->next)
        total += l->value;

    /* If total returned is not same as list contents, error */
    if (total != ret_val)
    {
        sprintf(buf, "****FAILURE****: %d returned from xmitas$sum_in_out, expecte %d\n", ret_val, total);
	xx_error(buf);
        return 1;
    }       


    /*  
     * [in,out,ptr] represent as type as linked list
     */
    /* Calculate total of list returned */
    for ((out_total = 0, l = &the_list); l != NULL; l = l->next)
        out_total += l->value;

    ret_val = xmitas$sum_in_out(h, &the_list, out_total);

    /* Calculate total of list returned */
    for ((total = 0, l = &the_list); l != NULL; l = l->next)
        total += l->value;

    /* If total returned is not same as list contents, error */
    if (total != ret_val)
    {
        sprintf(buf, "****FAILURE****: %d returned from xmitas$sum_in_out, expecte %d\n", ret_val, total);
	xx_error(buf);
        return 1;
    }       
	xx_log("****SUCCESS****:  All functions returned expected results");

    return 0;
}
