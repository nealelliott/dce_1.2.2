/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: union_mgr.c,v $
 * Revision 1.1.8.2  1996/03/09  20:51:44  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:29:27  marty]
 *
 * Revision 1.1.8.1  1995/12/13  21:56:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:29  root]
 * 
 * Revision 1.1.6.2  1993/07/09  13:44:00  root
 * 	Initial King Kong branch
 * 	[1993/07/09  13:43:24  root]
 * 
 * Revision 1.1.4.3  1993/04/23  14:43:06  giza
 * 	dce1.0.2 merge up
 * 	[1993/04/23  13:45:17  giza]
 * 
 * Revision 1.1.4.2  1993/03/31  12:40:50  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.3.2  1992/06/11  20:00:03  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:07:00  mcfarland]
 * 
 * Revision 1.1.1.2  1992/05/28  15:15:14  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1  1992/01/19  14:36:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <sp_union.h>

void my_oper( h, p_p_spu_param)
      /*[in]*/ handle_t h;
      /*[in,out]*/ sp_union_t **p_p_spu_param;

{ 
    sp_union_t *spu_head, *spu_tail;

    spu_head = *p_p_spu_param;
    spu_head = spu_head->spu_s.p_spu;
    spu_tail = spu_head;
    while (spu_tail->c == 'p')
    {
        spu_tail = spu_tail->spu_s.p_spu;
    }
    (spu_tail->spu_s.depth)--;
    *p_p_spu_param = spu_head;

}
