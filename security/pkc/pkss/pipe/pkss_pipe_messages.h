//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//

// HISTORY
// $Log: pkss_pipe_messages.h,v $
// Revision 1.1.2.1  1996/10/04  14:20:29  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/04  13:34:04  arvind]
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

#ifndef PKSS_PIPE_MESSAGES_H_
#define PKSS_PIPE_MESSAGES_H_

#include <dce/dce.h>
#include <dce/nbase.h>


#define REQUEST_FD 2
#define REPLY_FD 3

#define HELPER_IMAGE_NAME "/opt/dcelocal/bin/pkss_helper"

#define PKSS_PIPE_OP_GET_KEY_REQUEST	    1
#define PKSS_PIPE_OP_KEY_CHG_CLT_REQUEST    2
#define PKSS_PIPE_OP_KEY_CHG_SVR_REQUEST    3

#define PKSS_PIPE_OP_GET_KEY_REPLY	    17
#define PKSS_PIPE_OP_KEY_CHG_CLT_REPLY	    18
#define PKSS_PIPE_OP_KEY_CHG_SVR_REPLY	    19

#define PKSS_PIPE_OP_FAIL_REPLY		    0xffffffffu


typedef struct request_header {
    size_t message_length;
    unsigned32 message_type;
    size_t server_bindings_length;
    unsigned char iv[8];
    unsigned char key[8];
} request_header_t;

typedef struct reply_header {
    size_t message_length;
    unsigned32 message_type;
    unsigned32 status_code;
    unsigned char iv[8];
    unsigned char key[8];
} reply_header_t ;




typedef struct pkss_get_key_request {
    unsigned32 name_length;
    uuid_t domain_id;
    unsigned32 key_usages;
    unsigned32 key_vno;
    unsigned32 pwd_length;
//  unsigned char [] Name
//  unsigned char [] Pwd
} pkss_get_key_request_t;

typedef struct pkss_get_key_reply {
    unsigned32 pvtkey_length;
    unsigned32 pubkey_length;
    unsigned32 key_vno;
//  unsigned char [] Pvtkey
 //  unsigned char [] Pubkey
} pkss_get_key_reply_t;



typedef struct pkss_key_chg_clt_request {
    unsigned32 name_length;
    uuid_t domain_id;
    unsigned32 key_usages;
    unsigned32 key_vno;
    unsigned32 old_pwd_length;
    unsigned32 new_pwd_length;
    unsigned32 new_pvtkey_length;
    unsigned32 new_pubkey_length;
//  unsigned char [] Name
//  unsigned char [] Old-Pwd
//  unsigned char [] New-Pwd
//  unsigned char [] New-Pvtkey
//  unsigned char [] New-Pubkey
} pkss_key_chg_clt_request_t;

typedef struct pkss_key_chg_clt_reply {
    unsigned32 key_vno;
} pkss_key_chg_clt_reply_t;



typedef struct pkss_key_chg_svr_request {
    unsigned32 name_length;
    uuid_t domain_id;
    unsigned32 key_usages;
    unsigned32 key_vno;
    unsigned32 old_pwd_length;
    unsigned32 new_pwd_length;
    unsigned32 key_chg_flag;
    unsigned32 algorithm_length;
    unsigned32 key_size;
//  unsigned char [] Name
//  unsigned char [] Old-Pwd
//  unsigned char [] New-Pwd
//  unsigned char [] Algorithm-ID
} pkss_key_chg_svr_request_t;

typedef struct pkss_key_chg_svr_reply {
    unsigned32 new_pubkey_length;
    unsigned32 key_vno;
//  unsigned char [] Public-key
} pkss_key_chg_svr_reply_t;




#endif
