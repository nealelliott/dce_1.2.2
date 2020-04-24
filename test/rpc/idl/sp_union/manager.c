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
 * Revision 1.1.8.2  1996/02/17  23:17:15  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:27  marty]
 *
 * Revision 1.1.8.1  1995/12/11  19:29:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:43  root]
 * 
 * Revision 1.1.6.2  1993/06/15  21:08:15  sommerfeld
 * 	Rename spu_t -> spux_t because of name clash on HP-UX
 * 	[1993/06/14  21:41:55  sommerfeld]
 * 
 * Revision 1.1.2.2  1993/01/07  23:35:58  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:43:15  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:27:37  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <sp_union.h>
#include <test_common.h>

void his_oper( h, p_p_spu_param)
      /*[in]*/ handle_t h;
      /*[in,out]*/ spux_t **p_p_spu_param;

{ 
    spux_t *spu_head, *spu_tail;

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

globaldef sp_union_v0_0_epv_t sp_union_v0_0_m_epv = {his_oper};
