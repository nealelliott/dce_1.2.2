/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tsh_mgmt.h,v $
 * Revision 1.1.4.2  1996/02/18  23:06:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:21  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:12:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:45:55  root]
 * 
 * Revision 1.1.2.3  1993/01/25  19:29:35  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:46:27  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/13  19:49:08  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:15:57  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:37:49  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
#
#
# Copyright (c) 1991 by
#    Hewlett-Packard Company, Palo Alto, Ca.
#
# Permission to use, copy, modify, and distribute this
# software and its documentation for any purpose and without
# fee is hereby granted, provided that the above copyright
# notice appear in all copies and that both that copyright
# notice and this permission notice appear in supporting
# documentation, and that the name of Hewlett-Packard Company not
# be used in advertising or publicity pertaining to distribution
# of the software without specific, written prior permission.
# Hewlett-Packard Company makes no representations about the
# suitability of this software for any purpose.  It is provided
# "as is" without express or implied warranty.
#
#
# DCE API Test
#
# Revision 1.2  91/09/20  12:32:42  marco
# SubCase codes for mgmt_inq_stats added
# 
# Revision 1.1  91/09/11  16:15:07  marco
# Initial revision
# 
*/



#include <tsh_common.h>

/* %*START*% Start the body of code that will get translated into the FILE.tshrc */

#define INQ_COM_TIMEOUT			0
#define SET_COM_TIMEOUT			1
#define MGMT_INQ_IF_IDS			2
#define ERR_NO_INTERFACES		3
#define INQ_STATS			10
#define NULL_BINDING			11
#define BINDING_MISSING_ENDPOINT	12
#define BINDING_MISSING_UUID		13
#define BINDING_INCOMPLETE		14
#define SET_STACK_SIZE			20
#define INVALID_BINDING			100
#define INVALID_TIMEOUT			101
#define MEMORY_ERROR			102

/* %*STOP*%  Stop translating for FILE.tshrc  */


#include <dce/rpc.h>
#include <dce/stubbase.h>
#include <dce/idlbase.h>
#include <dce/nbase.h>

#define TSH_CALL_UUID     (unsigned_char_p_t)"3fb4f09c-b000-0000-0d00-018ed9000000"


handle_t           tsh_c_invalid_handle      = {0};
#define            tsh_c_number_of_protseqs  6


static rpc_endpoint_vector_elt_t NIDL_endpoints[tsh_c_number_of_protseqs] = 
{
 {(unsigned_char_p_t)"ncacn_ip_tcp",   (unsigned_char_p_t)"6692"},
 {(unsigned_char_p_t)"ncacn_dnet_nsp", (unsigned_char_p_t)"6693"},
 {(unsigned_char_p_t)"ncacn_osi_nsp",  (unsigned_char_p_t)"6694"},
 {(unsigned_char_p_t)"ncacn_osi_tp4",  (unsigned_char_p_t)"6695"},
 {(unsigned_char_p_t)"ncadg_ip_udp",   (unsigned_char_p_t)"6696"},
 {(unsigned_char_p_t)"ncadg_dds",      (unsigned_char_p_t)"6697"}
};

static rpc_syntax_id_t NIDL_ndr_id = {
{0x8a885d04, 0x1ceb, 0x11c9, 0x9f, 0xe8, {0x8, 0x0, 0x2b, 0x10, 0x48, 0x60}},
2};

static rpc_if_rep_t NIDL_ifspec = {
  0, /* ifspec rep version */
  1, /* op count */
  2, /* if version */
  {0x1cc460b7, 0xa000, 0x0000, 0x0d, 0x00, {0x0, 0xc3, 0x66, 0x0, 0x0, 0x0}},
  2, /* stub/rt if version */
  {1, NIDL_endpoints}, /* endpoint vector */
  {1, &NIDL_ndr_id} /* syntax vector */
};

globaldef rpc_if_handle_t tsh_call_test_v2_0_c_ifspec = (rpc_if_handle_t)&NIDL_ifspec;
