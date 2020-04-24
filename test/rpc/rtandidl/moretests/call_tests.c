/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: call_tests.c,v $
 * Revision 1.1.4.2  1996/02/17  23:20:19  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:38:58  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:03:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:42:57  root]
 * 
 * Revision 1.1.2.2  1993/01/11  22:12:20  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:04:25  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:34:27  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
**  Copyright (c) 1990 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass. &
**	Locus Computing Corporation, Burlington, Mass.
**
** 
**  NIDL TEST 
**	Extracted	1/31/91 16:32:30
**	from delta on	1/31/91 16:29:56
*/
#ifndef LINT
static char call_tests_sccs_id[] = "@(#)call_tests.c	1.1";
#endif


#define NIDL_GENERATED
#define NIDL_SSTUB
#include <stdio.h>
#include <tsh_call.h>
#include <dce/stubbase.h>


static void op0_ssr (h, call_h, elt, drep, transfer_syntax, mgr_epv, st_p)
handle_t             h;
rpc_call_handle_t    call_h;
rpc_iovector_elt_p_t elt;
ndr_format_p_t       drep;
rpc_syntax_id_p_t    transfer_syntax;
rpc_mgr_epv_t        mgr_epv;
error_status_t       *st_p;
{
rpc_op_t             op;
rpc_mp_t             mp;
rpc_mp_t             pmp;
volatile error_status_t st;
rpc_ss_mem_handle    mem_handle = NULL;
ndr_boolean          buffer_changed;
volatile ndr_boolean NIDL_manager_entered = ndr_false;
IoVec_t(2)           iovec;
IoVec_t(1)           iovec2;
ndr_byte             packet[14];
rpc_iovector_elt_t   fault_info;
unsigned32           SubCase;
unsigned32           flt_body;


/* local variables */
unsigned32 *j_;
unsigned32 i_;
unsigned32 x_;
unsigned32 NIDL_ptr_0;

#ifdef DEBUG
fprintf(stderr, "--------------------\ncall_tests: Into server stub...\n");
#endif


RPC_SS_INIT_SERVER

st = error_status_ok;
TRY
/* initialize pointer surrogates */
j_ = &NIDL_ptr_0;

rpc_init_mp(mp, elt->data_addr);
if (rpc_equal_drep((*drep), ndr_g_local_drep))
    {
    rpc_unmarshall_ulong_int(mp, x_);
    rpc_advance_mp(mp, 4);
    rpc_unmarshall_ulong_int(mp, i_);
    }
else
    {
    rpc_convert_ulong_int((*drep), ndr_g_local_drep, mp, x_);
    rpc_advance_mp(mp, 4);
    rpc_convert_ulong_int((*drep), ndr_g_local_drep, mp, i_);
    }

if (elt->buff_dealloc) 
    (*elt->buff_dealloc)(elt->buff_addr);

SubCase = x_;

switch(SubCase)
    {
    case START:
    case END:
    case TRANSCEIVE:
    case TRANSMIT:
    case RECEIVE:
    case BLOCK_UNTIL_FREE:
#ifdef DEBUG
        if(SubCase != TRANSMIT && SubCase != RECEIVE)
fprintf(stderr, "call_tests: into subcase TRANSCEIVE, START, END or BLOCK\n");
#endif

        /* manager call */
        NIDL_manager_entered = ndr_true;
        (*((tsh_call_test_v2_0_epv_t *)mgr_epv)->tsh_call_transceive)(h, x_, i_, j_);

#ifdef DEBUG
fprintf(stderr, "call_tests: i = %ld, j = %ld\n", i_, *j_);
#endif

        /* Start of marshalling block */
        iovec.num_elt = 1;
        rpc_init_mp(pmp, &packet[0]);
        rpc_align_mp(pmp, 8);
        iovec.elt[0].data_addr = (byte_p_t)pmp;
        iovec.elt[0].flags = rpc_c_iovector_elt_reused;
        iovec.elt[0].buff_dealloc = NULL_FREE_RTN;
        rpc_marshall_ulong_int(pmp, NIDL_ptr_0);
        rpc_advance_mp(pmp, 4);
        iovec.elt[0].data_len = (byte_p_t)pmp-iovec.elt[0].data_addr;

        rpc_call_transmit(call_h, (rpc_iovector_p_t)&iovec, &st);
        if (st != error_status_ok) 
            goto closedown;

        if(SubCase == BLOCK_UNTIL_FREE)
            {
            rpc_call_block_until_free(call_h, &st);
            if (st != rpc_s_ok)
                goto closedown;
            }

        if(SubCase != TRANSMIT && SubCase != RECEIVE)
             break;

#ifdef DEBUG
fprintf(stderr, "call_tests: into subcase TRANSMIT OR RECEIVE\n");
#endif
        if (elt->buff_dealloc) 
            (*elt->buff_dealloc)(elt->buff_addr);

        /*  Send back everything that was received, don't bother looking 
         *  inside...
         */
        iovec.num_elt              = 1;
        iovec.elt[0].buff_dealloc  = NULL;
        iovec.elt[0].flags         = rpc_c_iovector_elt_reused;
        iovec.elt[0].buff_addr     = elt->data_addr;
        iovec.elt[0].buff_len      = elt->data_len; 
        iovec.elt[0].data_addr     = elt->data_addr;
        iovec.elt[0].data_len      = elt->data_len; 

       
#ifdef DEBUG
fprintf(stderr, "call_tests: doing another transmit\n");
#endif
        rpc_call_transmit(call_h, (rpc_iovector_p_t)&iovec, &st);
        if (st != error_status_ok) 
            goto closedown;

#ifdef DEBUG
fprintf(stderr, "call_tests: finished transmit\n");
#endif
        break;

    case CANCEL:
        break;
        
    case CANCEL_NO_ACK:
        break;
        
    case TRANSMIT_FAULT:
    case RECEIVE_FAULT:
#ifdef DEBUG
fprintf(stderr, 
"call_tests: into subcase TRANSMIT_FAULT or RECEIVE_FAULT\n");
#endif
        iovec2.num_elt              = 1;
        iovec2.elt[0].buff_dealloc  = NULL;
        iovec2.elt[0].flags         = rpc_c_iovector_elt_reused;
        iovec2.elt[0].buff_addr     = (byte_p_t) &flt_body;
        iovec2.elt[0].buff_len      = sizeof(flt_body);
        iovec2.elt[0].data_addr     = (byte_p_t) &flt_body;
        iovec2.elt[0].data_len      = sizeof(flt_body);

#ifdef NOTDEF
        /* !!! - do this the correct way; cheating will do for now... */
        rpc_stcode_marshall(i_, &fault_info);
#else
        flt_body = i_;
#endif
#ifdef DEBUG
fprintf(stderr, "call_tests: setting up fault %d\n", flt_body);
#endif
        rpc_call_transmit_fault(call_h, (rpc_iovector_p_t)&iovec2, &st);
        if (st != rpc_s_ok)
            goto closedown;

        break;
    }

/* End of marshalling block */
closedown: ;
CATCH_ALL
#ifdef DEBUG
fprintf(stderr, "call_tests: got an exception\n");
#endif

if (NIDL_manager_entered) 
    rpc_ss_send_server_exception(call_h,THIS_CATCH);

else st = rpc_s_manager_not_entered;
ENDTRY
if (mem_handle) 
    rpc_ss_mem_free(&mem_handle);
if (st != error_status_ok)
    {
    if (NIDL_manager_entered)
        {
#ifdef DEBUG
fprintf(stderr, "call_tests: sending back an exception\n");
#endif
        rpc_ss_send_server_exception(call_h,&rpc_x_comm_failure);
        st = error_status_ok;
        }
    }

#ifdef DEBUG
fprintf(stderr, "call_tests: Leaving server stub, exit status = %lx...\n", st);
#endif

*st_p = st;
}

static rpc_v2_server_stub_proc_t NIDL_epva[] = 
    {(rpc_v2_server_stub_proc_t)op0_ssr};

static rpc_v2_server_stub_epv_t tsh_call_test_v2_0_s_epv = 
    (rpc_v2_server_stub_epv_t)NIDL_epva;

static tsh_call_test_v2_0_epv_t NIDL_manager_epv = { tsh_call_transceive };



#ifdef tsh_c_number_of_protseqs
#    undef tsh_c_number_of_protseqs
#endif
#define tsh_c_number_of_protseqs  6


static rpc_endpoint_vector_elt_t NIDL_endpoints[tsh_c_number_of_protseqs] = 
{
 {(unsigned_char_p_t)"ncadg_ip_udp",   (unsigned_char_p_t)"6696"},
 {(unsigned_char_p_t)"ncadg_dds",      (unsigned_char_p_t)"6697"},
 {(unsigned_char_p_t)"ncacn_ip_tcp",   (unsigned_char_p_t)"6692"},
 {(unsigned_char_p_t)"ncacn_dnet_nsp", (unsigned_char_p_t)"6693"},
 {(unsigned_char_p_t)"ncacn_osi_nsp",  (unsigned_char_p_t)"6694"},
 {(unsigned_char_p_t)"ncacn_osi_tp4",  (unsigned_char_p_t)"6695"}
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
  {1, &NIDL_ndr_id}, /* syntax vector */
  NIDL_epva, /* server_epv */
  (rpc_mgr_epv_t)&NIDL_manager_epv /* manager epv */
};

globaldef rpc_if_handle_t tsh_call_test_v2_0_s_ifspec = 
    (rpc_if_handle_t)&NIDL_ifspec;


void tsh_call_transceive(h, x, i, j)
handle_t h;
unsigned32 x, i, *j;
{
*j = i;
}
