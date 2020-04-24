/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tsh_call.h,v $
 * Revision 1.1.4.2  1996/02/18  23:06:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:13  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:11:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:45:45  root]
 * 
 * Revision 1.1.2.3  1993/01/25  19:29:18  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:46:03  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/13  19:48:37  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:15:38  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:37:41  devrcs
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
*/



#include <tsh_common.h>
#include <dce/rpc.h>
#include <dce/stubbase.h>
#include <dce/idlbase.h>
#include <dce/nbase.h>

/* %*START*% Start the body of code that will get translated into the FILE.tshrc */
       
/*
#define NON_IDEMPOTENT_OPTION 0 
#define IDEMPOTENT_OPTION     1
#define MAYBE_OPTION          2
#define BRDCST_OPTION         3
#define INVALID_OPTION        4

#define START                 0
#define END                   1
#define TRANSCEIVE            2
#define TRANSMIT              3
#define RECEIVE               4
#define TRANSMIT_FAULT        5
#define RECEIVE_FAULT         6
#define BLOCK_UNTIL_FREE      7
#define CANCEL                8
#define CANCEL_NO_ACK         9
#define INVALID_BINDING     100
#define INVALID_OPNUM       101
#define INVALID_IF_SPEC     102
#define INVALID_CALL_HANDLE 103
#define MEMORY_ERROR        104
*/
/* %*STOP*%  Stop translating for FILE.tshrc  */
    
/*
#define TSH_CALL_UUID     (unsigned_char_p_t)"3fb4f09c-b000-0000-0d00-018ed9000000"
#define TSH_CALL_PROTSEQ  (unsigned_char_p_t)"ncadg_ip_udp"
#define TSH_CALL_ENDPOINT (unsigned_char_p_t)"6696"
*/

/*  The stuff from the NIDL generated header file.
 
extern void op();

typedef struct tsh_call_test_v2_0_epv_t {
void (*op)(
#ifdef IDL_PROTOTYPES
    /* [in] / handle_t h
#endif
);
} tsh_call_test_v2_0_epv_t;
                    
 static tsh_call_test_v2_0_epv_t NIDL_manager_epv = {
op
};


globalref tsh_call_test_v2_0_epv_t tsh_call_test_v2_0_epv;
globalref rpc_if_handle_t tsh_call_test_v2_0_c_ifspec;
globalref rpc_if_handle_t tsh_call_test_v2_0_s_ifspec;
*/

#ifndef NIDL_SSTUB 

/*  Generic stuff for all call clients...
 */                        
#define            tsh_c_invalid_option      10
handle_t           tsh_c_invalid_handle      = {0};
rpc_if_rep_t       tsh_c_invalid_if_spec     = {0};
rpc_call_handle_t  tsh_c_invalid_call_handle = {0};
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
  1, /* ifspec rep version */
  1, /* op count */
  1, /* if version */
  {0x42c460ba, 0xa000, 0x0000, 0x0d, 0x00, {0x0, 0xc3, 0x66, 0x0, 0x0, 0x0}},
  2, /* stub/rt if version */
  {tsh_c_number_of_protseqs, NIDL_endpoints}, /* endpoint vector */
  {1, &NIDL_ndr_id} /* syntax vector */
};




globaldef rpc_if_handle_t tsh_call_test_v2_0_c_ifspec = (rpc_if_handle_t)&NIDL_ifspec;
static rpc_iovector_t NIDL_null_iovec = {0};

#endif
