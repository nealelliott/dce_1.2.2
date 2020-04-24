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
 * Revision 1.1.4.2  1996/02/17  23:16:48  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:08  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:28:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:17  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:33:30  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:41:29  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:26:59  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <refptrs.h>
#include <test_common.h>

void his_op1
( h, p_refnode, p_notchanged, p_changed )
    /* [in] */handle_t h;
    /* [in, out] */refnode_t *p_refnode;
    /* [in, out] */mutnode_t *p_notchanged;
    /* [in, out] */mutnode_t *p_changed;
{
    p_refnode->rn_char = 'L';
    p_refnode->rn_ptr->rl_char = 'M';
    *(p_refnode->rn_ptr->rl_ptr) = 'S';
    
    p_notchanged->mn_char = 'S';
    p_notchanged->mn_ptr->rl_char = '&';
    *(p_notchanged->mn_ptr->rl_ptr) = 'D';

    p_changed->mn_char = 'G';
    p_changed->mn_ptr = (refleaf_t *)rpc_ss_allocate(sizeof(refleaf_t));
    p_changed->mn_ptr->rl_char = 'W';
    p_changed->mn_ptr->rl_ptr = (leafchar_t *)rpc_ss_allocate(sizeof(leafchar_t));
    *(p_changed->mn_ptr->rl_ptr) = 'R';
}

globaldef refptrs_v0_0_epv_t refptrs_v0_0_m_epv = {his_op1};
