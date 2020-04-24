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
 * Revision 1.1.8.2  1996/02/17  23:18:46  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:37:28  marty]
 *
 * Revision 1.1.8.1  1995/12/11  19:32:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:17:01  root]
 * 
 * Revision 1.1.6.2  1993/06/15  21:10:02  sommerfeld
 * 	Add #include of <test_common.h>
 * 	[1993/06/14  21:12:32  sommerfeld]
 * 
 * Revision 1.1.2.2  1993/01/07  23:58:58  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:47:58  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:33:48  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <xmitas_repas.h>
#include <stdio.h>
#include <dce/nbase.h>
#include <test_common.h>

int client_aux(h,passes)
      handle_t h;
      int passes;
{
    error_status_t st;
    local_t structure;
    int num_elems = (random() % (MAXELEMENTS-2)) + 1;
    int i;
    int total = 0;
    int ret_val;
    
    structure.last = num_elems;
    for (i = 0; i <= num_elems; i++)
    {
        structure.values[i] = i * 77;
        total += structure.values[i];
    }

    ret_val = xmitas_sum(h, structure);
    if (total != ret_val)
    {
        printf("****FAILURE****: %d returned from xmitas_sum, expecte %d\n",
              ret_val, total);
        return 1;
    }       

    /* [in,out] */
    num_elems = (random() % (MAXELEMENTS-2)) + 1;
    total = 0;
    structure.last = num_elems;
    for (i = 0; i <= num_elems; i++)
    {
        structure.values[i] = i * 77;
        total += structure.values[i];
    }

    ret_val = xmitas_sum_in_out(h, &structure, total);
    total = 0;
    for (i = 0; i <= structure.last; i++)
    {
        total += structure.values[i];
    }
    if (total != ret_val)
    {
        printf("****FAILURE****: %d returned from xmitas_sum, expecte %d\n",
              ret_val, total);
        return 1;
    }       

    return 0;
}
