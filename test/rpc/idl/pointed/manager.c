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
 * Revision 1.1.4.2  1996/02/17  23:16:34  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:56  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:28:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:03  root]
 * 
 * Revision 1.1.2.2  1993/01/07  20:43:12  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:40:38  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:26:46  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <pointed.h>
#include <test_common.h>

void his_new_oper (h, p_this_way, p_that_way)
    /* [in] */handle_t h;
    /* [in] */this_way_t *p_this_way;
    /* [out] */that_way_t *p_that_way;
{ 
    p_that_way->p_boolean = (ndr_boolean *)rpc_ss_allocate(sizeof(ndr_boolean));
    *p_that_way->p_boolean = *p_this_way->p_boolean;
    p_that_way->p_byte = (ndr_byte *)rpc_ss_allocate(sizeof(ndr_byte));
    *p_that_way->p_byte = *p_this_way->p_byte;
    p_that_way->p_char = (ndr_char *)rpc_ss_allocate(sizeof(ndr_char));
    *p_that_way->p_char = *p_this_way->p_char;
    p_that_way->p_small = (ndr_small_int *)rpc_ss_allocate(sizeof(ndr_small_int));
    *p_that_way->p_small = *p_this_way->p_small;
    p_that_way->p_short = (ndr_short_int *)rpc_ss_allocate(sizeof(ndr_short_int));
    *p_that_way->p_short = *p_this_way->p_short;
    p_that_way->p_long = (ndr_long_int *)rpc_ss_allocate(sizeof(ndr_long_int));
    *p_that_way->p_long = *p_this_way->p_long;
    p_that_way->p_hyper = (ndr_hyper_int *)rpc_ss_allocate(sizeof(ndr_hyper_int));
    (*p_that_way->p_hyper).high = (*p_this_way->p_hyper).high;
    (*p_that_way->p_hyper).low = (*p_this_way->p_hyper).low;
    p_that_way->p_usmall = (ndr_usmall_int *)rpc_ss_allocate(sizeof(ndr_usmall_int));
    *p_that_way->p_usmall = *p_this_way->p_usmall;
    p_that_way->p_ushort = (ndr_ushort_int *)rpc_ss_allocate(sizeof(ndr_ushort_int));
    *p_that_way->p_ushort = *p_this_way->p_ushort;
    p_that_way->p_ulong = (ndr_ulong_int *)rpc_ss_allocate(sizeof(ndr_ulong_int));
    *p_that_way->p_ulong = *p_this_way->p_ulong;
    p_that_way->p_uhyper = (ndr_uhyper_int *)rpc_ss_allocate(sizeof(ndr_uhyper_int));
    (*p_that_way->p_uhyper).high = (*p_this_way->p_uhyper).high;
    (*p_that_way->p_uhyper).low = (*p_this_way->p_uhyper).low;
    p_that_way->p_float = (ndr_short_float *)rpc_ss_allocate(sizeof(ndr_short_float));
    *p_that_way->p_float = *p_this_way->p_float;
    p_that_way->p_double = (ndr_long_float *)rpc_ss_allocate(sizeof(ndr_long_float));
    *p_that_way->p_double = *p_this_way->p_double;
}

globaldef pointed_v0_0_epv_t pointed_v0_0_m_epv = {his_new_oper};
