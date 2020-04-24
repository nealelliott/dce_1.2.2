//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
//
// All Rights Reserved

//
// HISTORY
// $Log: pkss_pipe_client.cxx,v $
// Revision 1.1.2.1  1996/10/03  20:30:02  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/03  19:46:04  arvind]
//
// 	OSF DCE 1.2.2 Drop 4
//
// Revision /main/DCE_1.2.2/1  1996/09/09  21:27 UTC  arvind
// 	PKSS drop from DEC (DCE1.2.2)
// 	[1996/08/30  16:02 UTC  arvind  /main/arvind_pkss/1]
// 
// $EndLog$

//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
//


extern "C" {
#include <string.h>
#ifdef DEBUG
#include <stdio.h>
#endif
#include <limits.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <dce/dce.h>
#include <dce/nbase.h>
#include <dce/dce_svc.h>
#include <dce/dcepksmsg.h>
#include <dce/dcepkssvc.h>
#include <dce/dcepksmac.h>
#include <dce/sec_pk_base.h>
#include <dce/sec_pk.h>
#include "sec_bsafe.h"
}
#include "sec_pkss_clt_api.h"
#include "pkss_pipe_format.h"
#include "pkss_pipe_messages.h"

#ifdef DEBUG
static FILE * debug_file = NULL;
#endif

unsigned32 pipe_client(int readfd,
                       int writefd,
                       pid_t childpid,
                       const char * server_bindings,
                       unsigned32 message_type,
                       size_t length,
                       const void * message,
                       unsigned32 & ret_type,
                       unsigned32 & ret_code,
                       size_t & ret_length,
                       void * &ret_message) {

    int i;
    unsigned32 st;

    request_header_t request_header;
    reply_header_t reply_header;
    unsigned key_dat_len = ((sizeof(childpid) > 8) ? 8 : sizeof(childpid));

    sec_pk_gen_data_t output_message;
    sec_pk_gen_data_t input_message;
    sec_pk_gen_data_t iv;
    sec_pk_gen_data_t key;
    size_t bytes_read;
    
    ret_type = PKSS_PIPE_OP_FAIL_REPLY;
    ret_code = 0;
    ret_length = 0;
    ret_message = NULL;

    request_header.message_type = message_type;
    request_header.server_bindings_length = strlen(server_bindings)+1;
    request_header.iv[0] = 0;
    request_header.iv[1] = 1;
    request_header.iv[2] = 2;
    request_header.iv[3] = 3;
    request_header.iv[4] = 4;
    request_header.iv[5] = 5;
    request_header.iv[6] = 6;
    request_header.iv[7] = 7;

    memset(&(request_header.key[0]), 0, 8);
    memcpy(&(request_header.key[0]), &childpid, key_dat_len);

#ifndef NOENCRYPT_PIPE
    input_message.len = length;
    input_message.data = (unsigned char *)message;

    iv.len = 8;
    iv.data = (unsigned char *)&(request_header.iv[0]);
    
    key.len = 8;
    key.data = (unsigned char *)&(request_header.key[0]);

#ifdef DEBUG
    if (!debug_file) debug_file = fopen("client.log", "w");
    fprintf(debug_file, "Encrypting message:");
    for (i=0;i<input_message.len;i++) {
	if (i%16 == 0) fprintf(debug_file, "\n");
	fprintf(debug_file, "%2.2X ", input_message.data[i]);
    };
    fprintf(debug_file,"\n");
    fprintf(debug_file,
	"IV:  %2.2X %2.2X %2.2X %2.2X %2.2X %2.2X %2.2X %2.2X\n",
        iv.data[0],
        iv.data[1],
        iv.data[2],
        iv.data[3],
        iv.data[4],
        iv.data[5],
        iv.data[6],
        iv.data[7]);
    fprintf(debug_file,
	"Key: %2.2X %2.2X %2.2X %2.2X %2.2X %2.2X %2.2X %2.2X\n",
        key.data[0],
        key.data[1],
        key.data[2],
        key.data[3],
        key.data[4],
        key.data[5],
        key.data[6],
        key.data[7]);
    
    fprintf(debug_file,"\n\n"); fflush(debug_file);
#endif
    
    st = sec_bsafe_symmetric_encrypt(sec_bsafe_enc_des,
                                     &key,
                                     &iv,
                                     &input_message,
                                     &output_message);
    if (st) {
	dce_svc_printf(SEC_PKSS_PIPE_ENCRYPT_FAIL_MSG, st);
	return st;
    };
#else
    output_message.len = length;
    output_message.data = (unsigned char *)message;
#endif

    request_header.message_length = output_message.len +
                                    strlen(server_bindings)+1 +
                                    sizeof(request_header);
    
#ifdef DEBUG
    if (!debug_file) debug_file = fopen("client.log", "w");
    fprintf(debug_file, "Sending...Request header:");
    for (i=0;i<sizeof(request_header);i++) {
	if (i%16 == 0) fprintf(debug_file, "\n");
	fprintf(debug_file, "%2.2X ", ((unsigned char *)&(request_header))[i]);
    };
    fprintf(debug_file,"\n\n"); fflush(debug_file);
#endif

    write_item(writefd,
               &request_header,
	       sizeof(request_header));

#ifdef DEBUG
    fprintf(debug_file, "Server bindings: \"%s\"\n", server_bindings);
    fprintf(debug_file,"\n"); fflush(debug_file);
#endif


    write_item(writefd,
               server_bindings,
	       strlen(server_bindings)+1);

#ifdef DEBUG
    fprintf(debug_file, "Message:");
    for (i=0;i<output_message.len;i++) {
	if (i%16 == 0) fprintf(debug_file, "\n");
	fprintf(debug_file, "%2.2X ", output_message.data[i]);
    };
    fprintf(debug_file,"\n"); fflush(debug_file);
#endif

    write_item(writefd,
               output_message.data,
	       output_message.len);
#ifndef NOENCRYPT_PIPE

    sec_pk_data_free(&output_message);

#endif

// Written the command, now wait for the response.
// First read the header...

    if ((bytes_read = read_item(readfd, &reply_header, sizeof(reply_header)))
	       != sizeof(reply_header)) {
#ifdef DEBUG
    fprintf(debug_file, "\nReply header length error!  Expected %d, received %d\n",
	    sizeof(reply_header),
	    bytes_read);
#endif
        return pks_s_int_length_error;
    };

#ifdef DEBUG
    fprintf(debug_file, "\nReply header (%d bytes):", sizeof(reply_header));
    for (i=0;i<sizeof(reply_header);i++) {
	if (i%16 == 0) fprintf(debug_file, "\n");
	fprintf(debug_file, "%2.2X ", ((unsigned char *)&reply_header)[i]);
    };
    fprintf(debug_file,"\n"); fflush(debug_file);
#endif

    ret_length = reply_header.message_length - sizeof(reply_header);
    ret_type = reply_header.message_type;
    ret_code = reply_header.status_code;


#ifdef DEBUG
    fprintf(debug_file, "Message type: %d\n", ret_type);
    fprintf(debug_file, "Reply code: %d\n", ret_code);
    fprintf(debug_file, "Message body length: %d\n", ret_length);
    fprintf(debug_file,"\n"); fflush(debug_file);
#endif

// Then read the message...
    if (ret_length) {
        if ((ret_message = malloc(ret_length)) == NULL) {
            return pks_s_no_memory;
        };

        if ((bytes_read = read_item(readfd, ret_message, ret_length))
             != ret_length) {
#ifdef DEBUG
            fprintf(debug_file,
                    "\nReply body length error!  Expected %d, received %d\n",
                    ret_length,
                    bytes_read);
#endif
            free(ret_message);
            ret_message = NULL;
            return pks_s_int_length_error;
        };

#ifdef DEBUG
        fprintf(debug_file, "\nReply message ((%d bytes):", ret_length);
        for (i=0;i<ret_length;i++) {
            if (i%16 == 0) fprintf(debug_file, "\n");
            fprintf(debug_file, "%2.2X ", ((unsigned char *)ret_message)[i]);
        };
        fprintf(debug_file,"\n"); fflush(debug_file);
#endif

#ifndef NOENCRYPT_PIPE
        input_message.len = ret_length;
        input_message.data = (unsigned char *)ret_message;

        iv.len = 8;
        iv.data = (unsigned char *)&(reply_header.iv[0]);
    
        key.len = 8;
        key.data = (unsigned char *)&(reply_header.key[0]);

        output_message.len = 0;
        output_message.data = NULL;

        st = sec_bsafe_symmetric_decrypt(sec_bsafe_enc_des,
                                         &key,
                                         &iv,
                                         &input_message,
                                         &output_message);
        free (ret_message);
        ret_length = 0;
        if (st) {
            dce_svc_printf(SEC_PKSS_PIPE_DECRYPT_FAIL_MSG, st);
            return st;
        };


#ifdef DEBUG
    if (!debug_file) debug_file = fopen("client.log", "w");
    fprintf(debug_file, "Decrypted message:");
    for (i=0;i<output_message.len;i++) {
	if (i%16 == 0) fprintf(debug_file, "\n");
	fprintf(debug_file, "%2.2X ", output_message.data[i]);
    };
    fprintf(debug_file,"\n");
    fprintf(debug_file,
	"IV:  %2.2X %2.2X %2.2X %2.2X %2.2X %2.2X %2.2X %2.2X\n",
        iv.data[0],
        iv.data[1],
        iv.data[2],
        iv.data[3],
        iv.data[4],
        iv.data[5],
        iv.data[6],
        iv.data[7]);
    fprintf(debug_file,
	"Key: %2.2X %2.2X %2.2X %2.2X %2.2X %2.2X %2.2X %2.2X\n",
        key.data[0],
        key.data[1],
        key.data[2],
        key.data[3],
        key.data[4],
        key.data[5],
        key.data[6],
        key.data[7]);
    
    fprintf(debug_file,"\n\n"); fflush(debug_file);
#endif



        ret_length = output_message.len;
        ret_message = output_message.data;

#endif
    } else {
	ret_message = NULL;
    };

    return 0;
}

unsigned32 pipe_server(int readfd,
                       int writefd,
                       char * target_image) {
// copy readfd to REQUEST_FD, writefd to REPLY_FD, close unwanted files
// and exec the specified image,
    int fd;
    
    dup2(readfd, REQUEST_FD);
    dup2(writefd, REPLY_FD);


    for (fd = REPLY_FD+1;fd<_POSIX_OPEN_MAX;fd++) {
        close(fd);
    };
    errno = 0;

    execl(target_image,
          "");

   
    switch (errno) {
    case E2BIG:
	return pks_s_exec_e2big;
    case EACCES:
	return pks_s_exec_eacces;
    case ELOOP:
	return pks_s_exec_eloop;
    case ENAMETOOLONG:
	return pks_s_exec_enametoolong;
    case ENOENT:
	return pks_s_exec_enoent;
    case ENOMEM:
	return pks_s_exec_enomem;
    case ENOTDIR:
	return pks_s_exec_enotdir;
    case ENOEXEC:
	return pks_s_exec_enoexec;
    default:
	return pks_s_exec_fail;
    };
}

unsigned32 pipe_send(char * target_image,
                     const char * server_bindings,
                     unsigned32 type,
                     size_t length,
                     const void * message,
                     unsigned32 & ret_type,
                     unsigned32 & ret_code,
                     size_t & ret_length,
                     void * &ret_message) {
// Create two pipes.
// Fork child process.
// In parent:
//    close unwanted ends of pipes.
//    Format message
//    send to pipe
//    wait for reply.
// In child:
//    close unwanted ends of pipes.
//    and the
//    exec target image.
//    send to parent
//
//


    unsigned32 st;
    pid_t childpid;
    int pipe1[2];
    int pipe2[2];

    if (pipe(pipe1) < 0) {
	return pks_s_pipe_error;
    };
    if (pipe(pipe2) < 0) {
	close(pipe1[0]);
	return pks_s_pipe_error;
    };

    if ( (childpid = fork()) < 0) {
	close(pipe1[0]);
	close(pipe2[0]);
	return pks_s_fork_error;
    };

    if (childpid > 0) {
// In parent    
	close(pipe1[0]);
	close(pipe2[1]);
	struct sigaction newaction;
	struct sigaction oldaction;
	sigset_t sigmask;
	(void) sigemptyset (&sigmask);
	newaction.sa_handler = SIG_IGN;
	newaction.sa_mask = sigmask;
	newaction.sa_flags = SA_NOCLDSTOP;
	(void) sigaction (SIGCLD,
			 (const struct sigaction *)&newaction,
			 &oldaction);
	st = pipe_client(pipe2[0],
                         pipe1[1],
			 childpid,
                         server_bindings,
                         type,
                         length,
                         message,
                         ret_type,
                         ret_code,
                         ret_length,
                         ret_message);
        close(pipe1[1]);
        close(pipe2[0]);
	return st;
    } else {
// In child
        close(pipe1[1]);
        close(pipe2[0]);

	st = pipe_server(pipe1[0],
                         pipe2[1],
                         target_image);


        switch (st) {
	case pks_s_exec_e2big:
	    dce_svc_printf(PKS_S_EXEC_E2BIG_MSG);
	    break;
        case pks_s_exec_eacces:
            dce_svc_printf(PKS_S_EXEC_EACCES_MSG);
            break;
        case pks_s_exec_eloop:
            dce_svc_printf(PKS_S_EXEC_ELOOP_MSG);
            break;
        case pks_s_exec_enametoolong:
            dce_svc_printf(PKS_S_EXEC_ENAMETOOLONG_MSG);
            break;
        case pks_s_exec_enoent:
            dce_svc_printf(PKS_S_EXEC_ENOENT_MSG);
            break;
        case pks_s_exec_enomem:
            dce_svc_printf(PKS_S_EXEC_ENOMEM_MSG);
            break;
        case pks_s_exec_enotdir:
            dce_svc_printf(PKS_S_EXEC_ENOTDIR_MSG);
            break;
        case pks_s_exec_enoexec:
            dce_svc_printf(PKS_S_EXEC_ENOEXEC_MSG);
            break;
	case pks_s_exec_fail:
            dce_svc_printf(PKS_S_EXEC_FAIL_MSG, errno);
            break;
        default:
	    ;
        };


	dce_svc_printf(PKS_S_UNEXPECTED_TERMINATION_MSG, (unsigned long)st);

	exit(2);  // Should never reach this point
    };
    return 0;
}

error_status_t sec_pkss_get_key
(
   const char *pkss_svc_profile_name_p,
   const char *name_p,
   sec_pk_domain_t domain_id,
   sec_pk_usage_flags_t key_usages,
   unsigned32 key_vno,
   const char *pwd_p,
   sec_pk_pvtkey_t *pvt_key_p,
   sec_pk_pubkey_t *pub_key_p,
   unsigned32 *ret_key_vno_p
) {
    unsigned32 st;
    pkss_get_key_request_t * message;
    size_t message_length;

    pkss_get_key_reply_t * ret_message;
    size_t ret_message_length;
    unsigned32 ret_message_type;
    unsigned32 ret_message_code;
    unsigned char * ret_ptr;
    void * ptr;
    unsigned char * dat_ptr;


   pvt_key_p->len = 0;
   pvt_key_p->data = NULL;

   pub_key_p->len = 0;
   pub_key_p->data = NULL;

   *ret_key_vno_p = 0;


    message_length = sizeof(pkss_get_key_request_t) +
                            strlen(name_p)+1 +
                            strlen(pwd_p)+1;

    message = (pkss_get_key_request_t *)malloc(message_length);
    if (message == NULL) {
	dce_svc_printf(PKS_S_PIPCLT_NOMEM_MSG, 1);
	return pks_s_no_memory;
    };

#ifdef DEBUG
    if (!debug_file) debug_file = fopen("client.log", "w");
    fprintf(debug_file,
       "Get_Key...Making message, length = %d bytes",
       message_length);
    fprintf(debug_file,"\n"); fflush(debug_file);
#endif

    message->name_length = strlen(name_p)+1;
    message->domain_id = domain_id;
    message->key_usages = key_usages;
    message->key_vno = key_vno;
    message->pwd_length = strlen(pwd_p)+1;

    dat_ptr = ((unsigned char *)message) + sizeof(pkss_get_key_request_t);

    memcpy(dat_ptr, name_p, message->name_length);
    dat_ptr+=message->name_length;

    memcpy(dat_ptr, pwd_p, message->pwd_length);



    st = pipe_send(HELPER_IMAGE_NAME,
                   pkss_svc_profile_name_p,
                   PKSS_PIPE_OP_GET_KEY_REQUEST,
                   message_length,
                   message,
                   ret_message_type,
                   ret_message_code,
                   ret_message_length,
                   ptr);
    ret_ptr = (unsigned char *)ptr;
    free(message);
    if (st) return st;
    if (ret_message_type == PKSS_PIPE_OP_FAIL_REPLY) {

#ifdef DEBUG
    fprintf(debug_file,
       "Failure resonse, code = 0x%x\n",
       ret_message_code);
    fflush(debug_file);
#endif

        return ret_message_code;
    };

#ifdef DEBUG
    fprintf(debug_file,
       "Ret_message_code = 0x%x\n",
       ret_message_code);
    fflush(debug_file);
#endif

    if (ret_message_type != PKSS_PIPE_OP_GET_KEY_REPLY) {

#ifdef DEBUG
        fprintf(debug_file,
                "Bad reply type (%d)\n",
                ret_message_type);
        fflush(debug_file);
#endif

	dce_svc_printf(PKS_S_PIPCLT_BAD_MSG_MSG, 1);
        free(ret_ptr);
	return pks_s_bad_msg_type;	
    };

#ifdef DEBUG
    fprintf(debug_file,
            "Expected message type (%d)\n",
            ret_message_type);
    fflush(debug_file);
#endif


    ret_message = (pkss_get_key_reply_t *)ret_ptr;
    dat_ptr = &ret_ptr[sizeof(pkss_get_key_reply_t)];

#ifdef DEBUG
    fprintf(debug_file,
            "Copying pvt_key (into %p)\n", pvt_key_p);
    fflush(debug_file);
#endif

    pvt_key_p->data = (unsigned char *)malloc(ret_message->pvtkey_length);
    if (pvt_key_p->data == NULL) {
        dce_svc_printf(PKS_S_PIPCLT_NOMEM_MSG, 2);
        free(ret_ptr);
        return pks_s_no_memory;
    };
    pvt_key_p->len = ret_message->pvtkey_length;
    memcpy(pvt_key_p->data, dat_ptr, pvt_key_p->len);
    dat_ptr += pvt_key_p->len;

#ifdef DEBUG
    fprintf(debug_file,
            "Copying pub_key\n");
    fflush(debug_file);
#endif

    pub_key_p->data = (unsigned char *)malloc(ret_message->pubkey_length);
    if (pub_key_p->data == NULL) {
	dce_svc_printf(PKS_S_PIPCLT_NOMEM_MSG, 3);
        free(ret_ptr);
	return pks_s_no_memory;
    };
    pub_key_p->len = ret_message->pubkey_length;
    memcpy(pub_key_p->data, dat_ptr, pub_key_p->len);

#ifdef DEBUG
    fprintf(debug_file,
            "Copying key_vno\n");
    fflush(debug_file);
#endif

    *ret_key_vno_p = ret_message->key_vno;


    free(ret_ptr);

    return ret_message_code;
}

error_status_t sec_pkss_key_chg_clt
(
   const char *pkss_svc_profile_name_p,
   const char *name_p,
   sec_pk_domain_t domain_id,
   sec_pk_usage_flags_t key_usages,
   unsigned32 key_vno,
   const char *old_pwd_p,
   const char *new_pwd_p,
   sec_pk_pvtkey_t *new_pvt_key_p,
   sec_pk_pubkey_t *new_pub_key_p,
   unsigned32 *new_key_vno_p
) {
    unsigned32 st;
    pkss_key_chg_clt_request_t * message;
    size_t message_length;

    pkss_key_chg_clt_reply_t * ret_message;
    size_t ret_message_length;
    unsigned32 ret_message_type;
    unsigned32 ret_message_code;
    unsigned char * ret_ptr;
    void * ptr;
    unsigned char * dat_ptr;

    *new_key_vno_p = 0;

    message_length = sizeof(pkss_key_chg_clt_request_t) +
                            strlen(name_p)+1 +
                            strlen(old_pwd_p)+1 +
                            strlen(new_pwd_p)+1 +
                            new_pvt_key_p->len + 
                            new_pub_key_p->len;

    message = (pkss_key_chg_clt_request_t *)malloc(message_length);
    if (message == NULL) {
	dce_svc_printf(PKS_S_PIPCLT_NOMEM_MSG, 4);
	return pks_s_no_memory;
    };
    message->name_length = strlen(name_p)+1;
    message->domain_id = domain_id;
    message->key_usages = key_usages;
    message->key_vno = key_vno;

    message->old_pwd_length = strlen(old_pwd_p)+1;
    message->new_pwd_length = strlen(new_pwd_p)+1;
    message->new_pvtkey_length = new_pvt_key_p->len;
    message->new_pubkey_length = new_pub_key_p->len;


    dat_ptr = ((unsigned char *)message) + sizeof(pkss_key_chg_clt_request_t);

    memcpy(dat_ptr, name_p, message->name_length);
    dat_ptr+=message->name_length;

    memcpy(dat_ptr, old_pwd_p, message->old_pwd_length);
    dat_ptr+=message->old_pwd_length;

    memcpy(dat_ptr, new_pwd_p, message->new_pwd_length);
    dat_ptr+=message->new_pwd_length;

    memcpy(dat_ptr, new_pvt_key_p->data, message->new_pvtkey_length);
    dat_ptr+=message->new_pvtkey_length;

    memcpy(dat_ptr, new_pub_key_p->data, message->new_pubkey_length);
    dat_ptr+=message->new_pubkey_length;

    st = pipe_send(HELPER_IMAGE_NAME,
                   pkss_svc_profile_name_p,
                   PKSS_PIPE_OP_KEY_CHG_CLT_REQUEST,
                   message_length,
                   message,
                   ret_message_type,
                   ret_message_code,
                   ret_message_length,
                   ptr);
    ret_ptr = (unsigned char *)ptr;
    free(message);
    if (st) return st;
    if (ret_message_type == PKSS_PIPE_OP_FAIL_REPLY) {
        return ret_message_code;
    };
    if (ret_message_type != PKSS_PIPE_OP_KEY_CHG_CLT_REPLY) {
	dce_svc_printf(PKS_S_PIPCLT_BAD_MSG_MSG, 2);
        free(ret_ptr);
	return pks_s_bad_msg_type;	
    };
    ret_message = (pkss_key_chg_clt_reply_t *)ret_ptr;

    *new_key_vno_p = ret_message->key_vno;

    free(ret_message);
    return ret_message_code;
}

error_status_t sec_pkss_key_chg_svr
(
   const char *pkss_svc_profile_name_p,
   const char *name_p,
   sec_pk_domain_t domain_id,
   sec_pk_usage_flags_t key_usages,
   unsigned32 key_vno,
   const char *old_pwd_p,
   const char *new_pwd_p,
   unsigned32 key_chg_flag,
   sec_pk_algorithm_id_t *pk_alg_id_p,
   unsigned32 pk_len,
   sec_pk_pubkey_t *new_pub_key_p,
   unsigned32 *new_key_vno_p
) {
    unsigned32 st;
    pkss_key_chg_svr_request_t * message;
    size_t message_length;

    pkss_key_chg_svr_reply_t * ret_message;
    size_t ret_message_length;
    unsigned32 ret_message_type;
    unsigned32 ret_message_code;
    unsigned char * ret_ptr;
    void * ptr;
    unsigned char * dat_ptr;


   new_pub_key_p->len = 0;
   new_pub_key_p->data = NULL;

   *new_key_vno_p = 0;

    message_length = sizeof(pkss_key_chg_svr_request_t) +
                            strlen(name_p)+1 +
                            strlen(old_pwd_p)+1 +
                            strlen(new_pwd_p)+1 +
                            pk_alg_id_p->len;
    message = (pkss_key_chg_svr_request_t *)malloc(message_length);
    if (message == NULL) {
	dce_svc_printf(PKS_S_PIPCLT_NOMEM_MSG, 5);
	return pks_s_no_memory;
    };
    message->name_length = strlen(name_p)+1;
    message->domain_id = domain_id;
    message->key_usages = key_usages;
    message->key_vno = key_vno;

    message->old_pwd_length = strlen(old_pwd_p)+1;
    message->new_pwd_length = strlen(new_pwd_p)+1;
    message->key_chg_flag = key_chg_flag;
    message->algorithm_length = pk_alg_id_p->len;
    message->key_size = pk_len;

    dat_ptr = ((unsigned char *)message) + sizeof(pkss_key_chg_svr_request_t);

    memcpy(dat_ptr, name_p, message->name_length);
    dat_ptr+=message->name_length;

    memcpy(dat_ptr, old_pwd_p, message->old_pwd_length);
    dat_ptr+=message->old_pwd_length;

    memcpy(dat_ptr, new_pwd_p, message->new_pwd_length);
    dat_ptr+=message->new_pwd_length;

    memcpy(dat_ptr, pk_alg_id_p->data, message->algorithm_length);
    dat_ptr+=message->new_pwd_length;

    st = pipe_send(HELPER_IMAGE_NAME,
                   pkss_svc_profile_name_p,
                   PKSS_PIPE_OP_KEY_CHG_SVR_REQUEST,
                   message_length,
                   message,
                   ret_message_type,
                   ret_message_code,
                   ret_message_length,
                   ptr);
    ret_ptr = (unsigned char *)ptr;
    free(message);
    if (st) return st;
    if (ret_message_type == PKSS_PIPE_OP_FAIL_REPLY) {
        return ret_message_code;
    };
    if (ret_message_type != PKSS_PIPE_OP_KEY_CHG_SVR_REPLY) {
	dce_svc_printf(PKS_S_PIPCLT_BAD_MSG_MSG, 3);
        free(ret_ptr);
	return pks_s_bad_msg_type;	
    };
    ret_message = (pkss_key_chg_svr_reply_t *)ret_ptr;
    dat_ptr = &ret_ptr[sizeof(pkss_key_chg_svr_reply_t)];

    new_pub_key_p->data =
        (unsigned char *)malloc(ret_message->new_pubkey_length);
    if (new_pub_key_p->data == NULL) {
	dce_svc_printf(PKS_S_PIPCLT_NOMEM_MSG, 3);
        free(ret_ptr);
	return pks_s_no_memory;
    };
    new_pub_key_p->len = ret_message->new_pubkey_length;
    memcpy(new_pub_key_p->data, dat_ptr, new_pub_key_p->len);

    *new_key_vno_p = ret_message->key_vno;
    
    free(ret_message);

    return ret_message_code;
}

