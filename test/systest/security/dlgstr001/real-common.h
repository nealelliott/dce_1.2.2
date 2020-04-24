/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: real-common.h,v $
 * Revision 1.1.4.2  1996/02/18  23:07:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:18  marty]
 *
 * Revision 1.1.4.1  1995/12/11  22:45:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:21:46  root]
 * 
 * Revision 1.1.2.2  1994/10/06  20:44:52  agd
 * 	expand copyright
 * 	[1994/10/06  14:32:22  agd]
 * 
 * Revision 1.1.2.1  1994/09/19  17:55:30  bhaim
 * 	multidelegate test
 * 	[1994/09/19  17:49:33  bhaim]
 * 
 * $EndLog$
 */
#include <dce/uuid.h>

#define STATUS_OK(stp)            ((stp)==NULL || (*stp) == error_status_ok)
#define GOOD_STATUS(stp)          ((stp)==NULL || (*stp) == error_status_ok)
#define BAD_STATUS(stp)           ((*stp) != error_status_ok)
#define SET_STATUS(stp, val)      (*stp) = val
#define CLEAR_STATUS(stp)         (*stp) = error_status_ok
#define STATUS_EQUAL(stp, st_val) ((*stp) == (st_val))
#define STATUS(stp)               (*stp)
#define TRUE                      1
#define FALSE                     0

/*
 * Server principal names that must exist in the rgy server
 * database prior to execution of servers
 */
#define string_conv_SERVER_PRINCIPAL_NAME "string-conv"
#define string_conv2_SERVER_PRINCIPAL_NAME "string-conv2"
#define string_conv3_SERVER_PRINCIPAL_NAME "string-conv3"
#define string_conv4_SERVER_PRINCIPAL_NAME "string-conv4"
#define string_conv5_SERVER_PRINCIPAL_NAME "string-conv5"
#define string_conv6_SERVER_PRINCIPAL_NAME "string-conv6"

/*
 * Server authentication mode definition
 */
#define string_conv_AUTHN_SVC    rpc_c_authn_dce_secret
#define string_conv2_AUTHN_SVC   rpc_c_authn_dce_secret
#define string_conv3_AUTHN_SVC   rpc_c_authn_dce_secret
#define string_conv4_AUTHN_SVC   rpc_c_authn_dce_secret
#define string_conv5_AUTHN_SVC   rpc_c_authn_dce_secret
#define string_conv6_AUTHN_SVC   rpc_c_authn_dce_secret

/* Attribute Setting Setup */

    static uuid_t pre_auth_req_id = {
        /* pre_auth_req         6c9d0ec8-dd2d-11cc-abdd-080009353559 */
        0x6c9d0ec8, 0xdd2d, 0x11cc, 0xab, 0xdd,
                {0x08, 0x00, 0x09, 0x35, 0x35, 0x59}
    };

    static uuid_t pwd_val_type_id = {
        /* pwd_val_type     689843ce-dd2d-11cc-a3e1-080009353559 */
        0x689843ce, 0xdd2d, 0x11cc, 0xa3, 0xe1,
                {0x08, 0x00, 0x09, 0x35, 0x35, 0x59}
    };

    static uuid_t test_void_id = {
        /* test_void            5d1b6546-2af1-11cd-9bf5-080009353559 */
        0x5d1b6546, 0x2af1, 0x11cd, 0x9b, 0xf5,
                {0x08, 0x00, 0x09, 0x35, 0x35, 0x59}
    };

    static uuid_t test_printstring_id = {
        /* test_printstring     5e2321ae-2af1-11cd-a742-080009353559 */
        0x5e2321ae, 0x2af1, 0x11cd, 0xa7, 0x42,
                {0x08, 0x00, 0x09, 0x35, 0x35, 0x59}
    };

    static uuid_t test_integer_id = {
        /* test_integer         5f439154-2af1-11cd-8ec3-080009353559 */
        0x5f439154, 0x2af1, 0x11cd, 0x8e, 0xc3,
                {0x08, 0x00, 0x09, 0x35, 0x35, 0x59}
    };

