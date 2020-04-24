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
 * Revision 1.1.8.2  1996/02/17  23:18:36  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:37:23  marty]
 *
 * Revision 1.1.8.1  1995/12/11  19:32:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:53  root]
 * 
 * Revision 1.1.6.2  1993/06/15  21:09:43  sommerfeld
 * 	Add #include of <test_common.h>
 * 	[1993/06/14  21:11:57  sommerfeld]
 * 
 * Revision 1.1.2.2  1993/01/07  23:58:15  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:47:21  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:33:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <xmitas_conf.h>
#include <stdio.h>
#include <dce/nbase.h>
#include <test_common.h>

int client_aux(h,passes)
      handle_t h;
      int passes;
{
    error_status_t st;
    list_t the_list, *l = &the_list;
    int num_elems = (random() % MAXELEMENTS) + 1;
    int i;
    int total = 0;
    int ret_val;
    
    for (i = 0; i < num_elems; i++)
    {
        l->value = i * 77;
        total += l->value;
        if (i < num_elems - 1) l->next = (list_t *)malloc(sizeof(list_t));
        else l->next = NULL;

        l = l->next;
    }

    ret_val = xmitas_sum(h, &the_list);
    if (total != ret_val)
    {
        printf("****FAILURE****: %d returned from xmitas_sum, expecte %d\n",
              ret_val, total);
        return 1;
    }       

    return 0;
}
