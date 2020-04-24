//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//

// HISTORY
// $Log: pkss_pipe_server.cxx,v $
// Revision 1.1.2.1  1996/10/04  14:20:43  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/04  13:34:14  arvind]
//
// Revision /main/DCE_1.2.2/1  1996/09/09  21:28 UTC  arvind
// 	PKSS drop from DEC (DCE1.2.2)
// 	[1996/08/30  16:03 UTC  arvind  /main/arvind_pkss/1]
// 
// $EndLog$

//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
//

#include <dce/dce.h>
#include <dce/nbase.h>
#include <dce/dce_svc.h>
#include <dce/dcepksmsg.h>
#include <dce/dcepkssvc.h>
#include <dce/dcepksmac.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "sec_pkss_util.h"
#include "sec_pkss_clt_api.h"
#include "pkss_pipe_format.h"
#include "pkss_pipe_messages.h"
#include "pkss_config_common.h"
#include <dce/sec_pk_base.h>
#include "sec_bsafe.h"

#ifdef DEBUG
#include <stdio.h>
#endif


int main(int argc, char * argv[]) {

    unsigned32 st;
    unsigned32 op_st;

    request_header_t request_header;
    reply_header_t reply_header;

    char * server_profile;
    unsigned32 message_type;

    size_t message_length;
    unsigned char * message;

    size_t reply_length;
    unsigned char * reply;


    sec_pk_gen_data_t input_message;
    sec_pk_gen_data_t output_message;
    sec_pk_gen_data_t iv;
    sec_pk_gen_data_t key;

    unsigned key_dat_len;
    unsigned iv_dat_len;

    pid_t my_pid;
    time_t my_time;

#ifdef DEBUG
    FILE * terminal = NULL;    
    int i;
#endif




#ifdef DEBUG
    terminal = fopen("/dev/tty", "w");    
#endif


// Initialize the serviceability tables

  sec_pkss_init_svc_handle ();

  st = pkss_config__init ("pkss");

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug3,
                   "status from pkss_config__init : 0x%x", st));

  if (st != 0)
  {
    return (st);
  }

// We're performing a single operation, then exiting.
// Read a message from fd0, process it, write results back to fd1, exit.    


// First the header...
    if (read_item(REQUEST_FD, &request_header, sizeof(request_header))
                      != sizeof(request_header)) {
        dce_svc_printf(PKS_S_INT_LENGTH_ERRORP_MSG, 1);
        return pks_s_int_length_error;
    };

    message_length = request_header.message_length;
    if (message_length < sizeof(request_header)
                         + request_header.server_bindings_length) {
        dce_svc_printf(PKS_S_INT_LENGTH_ERRORP_MSG, 2);
        return pks_s_int_length_error;
    };                         

    message_type = request_header.message_type;

    server_profile = (char *)malloc(request_header.server_bindings_length);
    if (server_profile == NULL) {
        dce_svc_printf(PKS_S_PIPSVR_NOMEM_MSG, 1);
        return pks_s_no_memory;
    };
    read_item(REQUEST_FD, server_profile, request_header.server_bindings_length);
    
    message_length -= sizeof(request_header);
    message_length -= request_header.server_bindings_length;
    message = (unsigned char *)malloc(message_length);
    if (message == NULL) {
        dce_svc_printf(PKS_S_PIPSVR_NOMEM_MSG, 2);
        return pks_s_no_memory;
    };
    read_item(REQUEST_FD, message, message_length);

// Now we have the operation message in <message>, length in <message_length>.

#ifndef NOENCRYPT_PIPE

    input_message.data = message;
    input_message.len = message_length;

    output_message.data = NULL;
    output_message.len = 0;

    iv.data = (unsigned char *)(&request_header.iv[0]);
    iv.len = 8;

    key.data = (unsigned char *)(&request_header.key[0]);
    key.len = 8;

    st = sec_bsafe_symmetric_decrypt(sec_bsafe_enc_des,
                                     &key,
				     &iv,
				     &input_message,
				     &output_message);
    if (st) {
        dce_svc_printf(PKS_S_PIPE_SVR_DECRYPT_FAIL_MSG, st);
        return st;
    };

    message_length = output_message.len;
    free(message);
    message = output_message.data;

#endif


// Dispatch to the appropriate routine...

    switch (message_type) {
    case PKSS_PIPE_OP_GET_KEY_REQUEST:
        {
            pkss_get_key_request_t * req_ptr;
            pkss_get_key_reply_t * rep_ptr;

            char * principal_name;
            sec_pk_domain_t domain_id;
            sec_pk_usage_flags_t key_usages;
            unsigned32 key_vno;
            char * password;
            sec_pk_pvtkey_t pvt_key;
            sec_pk_pubkey_t pub_key;
            unsigned32 ret_key_vno;

	    char * dat_ptr;

// Overlay the message structure onto the message...
            req_ptr = (pkss_get_key_request_t *)message;

	    dat_ptr = (char *)&message[sizeof(pkss_get_key_request_t)];

            principal_name = dat_ptr;
	    dat_ptr+= req_ptr->name_length;
	    
            domain_id = req_ptr->domain_id;

            key_usages = req_ptr->key_usages;

            key_vno = req_ptr->key_vno;

            password = dat_ptr;

// Call the routine...
            op_st = sec_pkss_get_key(
                       server_profile,
                       principal_name,
                       domain_id,
                       key_usages,
                       key_vno,
                       password,
                       &pvt_key,
                       &pub_key,
                       &ret_key_vno);
            if (op_st) {
                reply_header.message_type = PKSS_PIPE_OP_FAIL_REPLY;
                reply_length = 0;
                reply = NULL;
            } else {

// Build a response message...          
                reply_header.message_type = PKSS_PIPE_OP_GET_KEY_REPLY;
                reply = (unsigned char *)malloc(
                             sizeof(pkss_get_key_reply_t) +
                             pvt_key.len + pub_key.len
                                           );
                if (reply == NULL) {
                    dce_svc_printf(PKS_S_PIPSVR_NOMEM_MSG, 3);
                    return pks_s_no_memory;
                };
                rep_ptr = (pkss_get_key_reply_t *)reply;
                rep_ptr->pvtkey_length = pvt_key.len;
                rep_ptr->pubkey_length = pub_key.len;
                rep_ptr->key_vno = ret_key_vno;
                reply_length = sizeof(pkss_get_key_reply_t);
                memcpy(&reply[reply_length],
                       pvt_key.data,
                       pvt_key.len);
                reply_length += pvt_key.len;
                memcpy(&reply[sizeof(pkss_get_key_reply_t)+pvt_key.len],
                       pub_key.data,
                       pub_key.len);
                reply_length += pub_key.len;
            }
        }
        break;

    case PKSS_PIPE_OP_KEY_CHG_CLT_REQUEST:
        {
	    pkss_key_chg_clt_request_t * req_ptr;
            pkss_key_chg_clt_reply_t * rep_ptr;

            char * principal_name;
            sec_pk_domain_t domain_id;
            sec_pk_usage_flags_t key_usages;
            unsigned32 key_vno;
            char * old_password;
            char * new_password;
            sec_pk_pvtkey_t pvt_key;
            sec_pk_pubkey_t pub_key;
            unsigned32 ret_key_vno;

	    unsigned char * dat_ptr;
	    
// Overlay the message structure onto the message...
            req_ptr = (pkss_key_chg_clt_request_t *)message;

	    dat_ptr = &message[sizeof(pkss_key_chg_clt_request_t)];

            principal_name = (char *)dat_ptr;
            dat_ptr += req_ptr->name_length;

            domain_id = req_ptr->domain_id;

            key_usages = req_ptr->key_usages;

            key_vno = req_ptr->key_vno;;

            old_password = (char *)dat_ptr;
            dat_ptr += req_ptr->old_pwd_length;

            new_password = (char *)dat_ptr;
            dat_ptr += req_ptr->new_pwd_length;

            pvt_key.len = req_ptr->new_pvtkey_length;
            pvt_key.data = dat_ptr;
	    dat_ptr += req_ptr->new_pvtkey_length;

            pub_key.len = req_ptr->new_pubkey_length;
            pub_key.data = dat_ptr;

// Call the routine...
            op_st = sec_pkss_key_chg_clt(
                       server_profile,
                       principal_name,
                       domain_id,
                       key_usages,
                       key_vno,
                       old_password,
                       new_password,
		       &pvt_key,
                       &pub_key,
                       &ret_key_vno);
            if (op_st) {
                reply_header.message_type = PKSS_PIPE_OP_FAIL_REPLY;
                reply_length = 0;
                reply = NULL;
            } else {

// Build a response message...          
                reply_header.message_type = PKSS_PIPE_OP_KEY_CHG_CLT_REPLY;
                reply = (unsigned char *)malloc(
                             sizeof(pkss_key_chg_clt_reply_t));
                if (reply == NULL) {
                    dce_svc_printf(PKS_S_PIPSVR_NOMEM_MSG, 4);
                    return pks_s_no_memory;
                };
                rep_ptr = (pkss_key_chg_clt_reply_t *)reply;
                rep_ptr->key_vno = ret_key_vno;
                reply_length = sizeof(pkss_key_chg_clt_reply_t);
            }
        }
        break;

    case PKSS_PIPE_OP_KEY_CHG_SVR_REQUEST:

        {
	    pkss_key_chg_svr_request_t * req_ptr;
            pkss_key_chg_svr_reply_t * rep_ptr;

            char * principal_name;
            sec_pk_domain_t domain_id;
            sec_pk_usage_flags_t key_usages;
            unsigned32 key_vno;
            char * old_password;
            char * new_password;
            unsigned32 key_chg_flag;
	    sec_pk_algorithm_id_t pk_alg_id;
            unsigned32 pk_len;
            sec_pk_pubkey_t pub_key;
            unsigned32 ret_key_vno;

	    unsigned char * dat_ptr;
	    
// Overlay the message structure onto the message...
            req_ptr = (pkss_key_chg_svr_request_t *)message;

	    dat_ptr = &message[sizeof(pkss_key_chg_svr_request_t)];

            principal_name = (char *)dat_ptr;
            dat_ptr += req_ptr->name_length;

            domain_id = req_ptr->domain_id;

            key_usages = req_ptr->key_usages;

            key_vno = req_ptr->key_vno;;

            old_password = (char *)dat_ptr;
            dat_ptr += req_ptr->old_pwd_length;

            new_password = (char *)dat_ptr;
            dat_ptr += req_ptr->new_pwd_length;

            key_chg_flag = req_ptr->key_chg_flag;

	    pk_alg_id.len = req_ptr->algorithm_length;
	    pk_alg_id.data = dat_ptr;
            dat_ptr += pk_alg_id.len;
	    
            pk_len = req_ptr->key_size;

// Call the routine...
            op_st = sec_pkss_key_chg_svr(
                       server_profile,
                       principal_name,
                       domain_id,
                       key_usages,
                       key_vno,
                       old_password,
                       new_password,
                       key_chg_flag,
                       &pk_alg_id,
                       pk_len,
                       &pub_key,
                       &ret_key_vno);
            if (op_st) {
                reply_header.message_type = PKSS_PIPE_OP_FAIL_REPLY;
                reply_length = 0;
                reply = NULL;
            } else {

// Build a response message...          
                reply_header.message_type = PKSS_PIPE_OP_KEY_CHG_SVR_REPLY;
                reply = (unsigned char *)malloc(
                             sizeof(pkss_key_chg_svr_reply_t) +
			     pub_key.len);
                if (reply == NULL) {
                    dce_svc_printf(PKS_S_PIPSVR_NOMEM_MSG, 5);
                    return pks_s_no_memory;
                };
                rep_ptr = (pkss_key_chg_svr_reply_t *)reply;
                rep_ptr->key_vno = ret_key_vno;
                rep_ptr->new_pubkey_length = pub_key.len;
                reply_length = sizeof(pkss_key_chg_svr_reply_t);
		memcpy(&reply[reply_length],
      	               pub_key.data,
                       pub_key.len);
                reply_length += pub_key.len;
            }
        }
        break;

    default:
        dce_svc_printf(PKS_S_PIPSVR_DECODE_ERRORP_MSG, 1);
        reply_header.message_type = PKSS_PIPE_OP_FAIL_REPLY;
        op_st = pks_s_pipsvr_decode_error;
        reply_length = 0;
        reply = NULL;
    };

    reply_header.status_code = op_st;


#ifndef NOENCRYPT_PIPE
    input_message.data = reply;
    input_message.len = reply_length;

    output_message.data = NULL;
    output_message.len = 0;

    key.data = &(reply_header.key[0]);
    key.len = 8;

    key_dat_len = ((sizeof(pid_t) > 8) ? 8 : sizeof(pid_t));
    my_pid = getpid();
    memcpy(key.data, &my_pid, key_dat_len);
#ifdef DEBUG
    fprintf(terminal,
            "SVR: Reply key: %2.2X %2.2X %2.2X %2.2X %2.2X %2.2X %2.2X %2.2X\n",
	    key.data[0],
	    key.data[1],
	    key.data[2],
	    key.data[3],
	    key.data[4],
	    key.data[5],
	    key.data[6],
	    key.data[7]);
    fflush(terminal);
#endif


    iv.data = &(reply_header.iv[0]);
    iv.len = 8;

    iv_dat_len = ((sizeof(time_t) > 8) ? 8 : sizeof(time_t));
    my_time = time(NULL);
    memcpy(iv.data, &my_time, iv_dat_len);

#ifdef DEBUG
    fprintf(terminal,
            "SVR: Reply IV:  %2.2X %2.2X %2.2X %2.2X %2.2X %2.2X %2.2X %2.2X\n",
	    iv.data[0],
	    iv.data[1],
	    iv.data[2],
	    iv.data[3],
	    iv.data[4],
	    iv.data[5],
	    iv.data[6],
	    iv.data[7]);
    fflush(terminal);
#endif


#ifdef DEBUG
    fprintf(terminal,
            "SVR: Encrypting %d bytes:",
	    input_message.len);
    for (i=0;i<input_message.len;i++) {
	if (i%16 == 0) fprintf(terminal, "\n");
	fprintf(terminal, "%2.2X ", input_message.data[i]);
    };
    fprintf(terminal, "\n");fflush(terminal);
#endif

    st = sec_bsafe_symmetric_encrypt(sec_bsafe_enc_des,
                                     &key,
				     &iv,
				     &input_message,
				     &output_message);
    free(reply);

    if (st) {
        dce_svc_printf(PKS_S_PIPE_SVR_ENCRYPT_FAIL_MSG, st);
        return st;
    };

#ifdef DEBUG
    fprintf(terminal,
            "SVR: Result of encryption (%d bytes):",
	    output_message.len);
    for (i=0;i<output_message.len;i++) {
	if (i%16 == 0) fprintf(terminal, "\n");
	fprintf(terminal, "%2.2X ", output_message.data[i]);
    };
    fprintf(terminal, "\n");fflush(terminal);
#endif

    reply = output_message.data;
    reply_length = output_message.len;
    
#endif

    reply_header.message_length = sizeof(reply_header) + reply_length;

    write_item(REPLY_FD, &reply_header, sizeof(reply_header));
    if (reply_length > 0) write_item(REPLY_FD, reply, reply_length);
        
    return 0;

}
