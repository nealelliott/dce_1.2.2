/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xmitas_aux.c,v $
 * Revision 1.1.10.2  1996/03/09  20:51:49  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:29:31  marty]
 *
 * Revision 1.1.10.1  1995/12/13  21:56:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:34  root]
 * 
 * Revision 1.1.6.2  1993/07/09  13:56:39  root
 * 	Initial King Kong branch
 * 	[1993/07/09  13:56:12  root]
 * 
 * Revision 1.1.4.2  1993/03/31  12:44:24  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.3.2  1992/06/11  20:01:15  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:08:01  mcfarland]
 * 
 * Revision 1.1.1.2  1992/05/28  15:15:36  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1  1992/01/19  14:36:14  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <xmitas.h>
#include <stdio.h>
#include <dce/nbase.h>

extern char header[512], buf[128];

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

    ret_val = xmitas$sum(h, the_list);
    if (total != ret_val)
    {
        sprintf(buf, "%s%d returned from xmitas$sum, expecte %d\n", header,
                ret_val, total);
	xx_error(buf);
        return 1;
    }       

    sprintf(buf, "%sxmitas$sum successful\n", header);
    xx_log(buf);
    return 0;
}
