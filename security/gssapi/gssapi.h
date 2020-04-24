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
 * $Log: gssapi.h,v $
 * Revision 1.1.5.2  1996/02/18  22:57:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:41  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:43:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:32  root]
 * 
 * Revision 1.1.2.6  1994/09/23  19:52:21  wray
 * 	CR 12215 - New rtn gssdce_set_cred_context_ownership
 * 	[1994/09/23  19:49:26  wray]
 * 
 * Revision 1.1.2.5  1994/08/22  13:27:05  wray
 * 	CRs 11376, 11762, 11763, 11765
 * 	[1994/08/22  13:25:01  wray]
 * 
 * Revision 1.1.2.4  1994/06/17  18:41:59  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:36  devsrc]
 * 
 * Revision 1.1.2.3  1994/03/23  16:47:31  wray
 * 	GSSAPI BL2 (OT#10179)
 * 	[1994/03/23  16:44:01  wray]
 * 
 * 	GSSAPI BL2 (OT#10179)
 * 
 * Revision 1.1.2.2  1994/03/10  21:11:05  wray
 * 	Fix for RIOS (OT#10117)
 * 	[1994/03/10  21:09:09  wray]
 * 
 * Revision 1.1.2.1  1994/03/09  15:50:57  wray
 * 	GSSAPI initial submission (OT#10019)
 * 	[1994/03/09  15:29:18  wray]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) 1994 by
 * Digital Equipment Corporation, Maynard, Mass.
 *
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 *
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digital Equipment
 * Corporation.
 *
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 *
 * FACILITY:	Generic Security Service API (GSSAPI)
 *
 * ABSTRACT:    Header file for all externally-visible GSSAPI types and
 *		routines.
 *
 * ENVIRONMENT:	OSF/1
 *
 * AUTHOR:	John Wray
 *		Distributed Processing Engineering (DPE)
 *
 * DATE: 	January 1994
 *
 */


#ifndef GSSAPI_H_
#define GSSAPI_H_

/* Include XDS files to get OM_uint32 and OM_object */
#include <xom.h>
#include <xds.h>
#include <stdlib.h>
#include <dce/sec_login.h>
#include <dce/dcegssmsg.h>
#include <dce/rpcbase.h>

#define gssdce_s_success 0

typedef OM_uint32 gss_ctx_id_t;
typedef OM_uint32 gss_cred_id_t;
typedef char * gss_name_t;

typedef OM_object_identifier gss_OID_desc, *gss_OID;

typedef struct gss_OID_set_desc_struct  {
    int     count;
    gss_OID elements;
} gss_OID_set_desc, *gss_OID_set;

typedef struct gss_buffer_desc_struct {
    size_t length;
    void *value;
} gss_buffer_desc, *gss_buffer_t;

typedef struct gss_channel_bindings_struct {
    OM_uint32 initiator_addrtype;
    gss_buffer_desc initiator_address;
    OM_uint32 acceptor_addrtype;
    gss_buffer_desc acceptor_address;
    gss_buffer_desc application_data;
} *gss_channel_bindings_t;


/*
 * Six independent flags each of which indicates that a context
 * supports a specific service option.
 */
#define GSS_C_DELEG_FLAG 1
#define GSS_C_MUTUAL_FLAG 2
#define GSS_C_REPLAY_FLAG 4
#define GSS_C_SEQUENCE_FLAG 8
#define GSS_C_CONF_FLAG 16
#define GSS_C_INTEG_FLAG 32

/*
 * Credential usage options
 */
#define GSS_C_BOTH 0
#define GSS_C_INITIATE 1
#define GSS_C_ACCEPT 2

/*
 * Status code types for gss_display_status
 */
#define GSS_C_GSS_CODE 1
#define GSS_C_MECH_CODE 2

/*
 * On a platform that provides socket.h, the following definitions
 * should equate to the AF_xxx constants defined therein:
 * #define GSS_C_AF_LOCAL AF_LOCAL
 * etc.  In the absence os a socket.h header file, the following
 * literal definitions should be used:
 */
#define GSS_C_AF_UNSPEC     0;
#define GSS_C_AF_LOCAL      1;
#define GSS_C_AF_INET       2;
#define GSS_C_AF_IMPLINK    3;
#define GSS_C_AF_PUP        4;
#define GSS_C_AF_CHAOS      5;
#define GSS_C_AF_NS         6;
#define GSS_C_AF_NBS        7;
#define GSS_C_AF_ECMA       8;
#define GSS_C_AF_DATAKIT    9;
#define GSS_C_AF_CCITT      10;
#define GSS_C_AF_SNA        11;
#define GSS_C_AF_DECnet     12;
#define GSS_C_AF_DLI        13;
#define GSS_C_AF_LAT        14;
#define GSS_C_AF_HYLINK     15;
#define GSS_C_AF_APPLETALK  16;
#define GSS_C_AF_BSC        17;
#define GSS_C_AF_DSS        18;
#define GSS_C_AF_OSI        19;
#define GSS_C_AF_X25        21;

#define GSS_C_AF_NULLADDR   255;

#define GSS_C_NO_BUFFER ((gss_buffer_t) 0)
#define GSS_C_NULL_OID ((gss_OID) 0)
#define GSS_C_NULL_OID_SET ((gss_OID_set) 0)
#define GSS_C_NO_CONTEXT ((gss_ctx_id_t) 0)
#define GSS_C_NO_CREDENTIAL ((gss_cred_id_t) 0)
#define GSS_C_NO_CHANNEL_BINDINGS ((gss_channel_bindings_t) 0)
#define GSS_C_NO_NAME ((gss_name_t) 0)
#define GSS_C_EMPTY_BUFFER {0, NULL}

/*
 * Define the default Quality of Protection for per-message
 * services.  Note that an implementation that offers multiple
 * levels of QOP may either reserve a value (for example zero,
 * as assumed here) to mean "default protection", or alternatively
 * may simply equate GSS_C_QOP_DEFAULT to a specific explicit QOP
 * value.
 */
#define GSS_C_QOP_DEFAULT 0

/* The three QOP levels offered by the DCE GSSAPI implementation apply to   */
/* the integrity protection offered by gss_sign & gss_seal.  Confdentiality */
/* protection (if available) is always provided by DES-CBC.  The three	    */
/* integrity algorithms available (in order of decreasing speed) are:	    */
/*									    */
/* i)   A truncated MD5 (sometimes referred to as MD2.5)		    */
/* ii)  A DES-MAC of an MD5 hash					    */
/* iii) A regular DES-MAC						    */
/*									    */

#define GSSDCE_C_QOP_MD5 1
#define GSSDCE_C_QOP_DES_MD5 2
#define GSSDCE_C_QOP_DES_MAC 3

/*
 * Expiration time of 2^32-1 seconds means infinite lifetime for a
 * credential or security context
 */
#define GSS_C_INDEFINITE 0xffffffffu


/* Major status codes */

#define GSS_S_COMPLETE 0

/*
 * Some "helper" definitions to make the status code macros obvious.
 */
#define GSS_C_CALLING_ERROR_OFFSET 24
#define GSS_C_ROUTINE_ERROR_OFFSET 16
#define GSS_C_SUPPLEMENTARY_OFFSET 0
#define GSS_C_CALLING_ERROR_MASK 0377ul
#define GSS_C_ROUTINE_ERROR_MASK 0377ul
#define GSS_C_SUPPLEMENTARY_MASK 0177777ul

/*
 * The macros that test status codes for error conditions
 */
#define GSS_CALLING_ERROR(x) \
        (x & (GSS_C_CALLING_ERROR_MASK << GSS_C_CALLING_ERROR_OFFSET))
#define GSS_ROUTINE_ERROR(x) \
        (x & (GSS_C_ROUTINE_ERROR_MASK << GSS_C_ROUTINE_ERROR_OFFSET))
#define GSS_SUPPLEMENTARY_INFO(x) \
        (x & (GSS_C_SUPPLEMENTARY_MASK << GSS_C_SUPPLEMENTARY_OFFSET))
#define GSS_ERROR(x) \
        ((GSS_CALLING_ERROR(x) != 0) || (GSS_ROUTINE_ERROR(x) != 0))


/*
 * Now the actual status code definitions
 */

/*
 * Calling errors:
 */
#define GSS_S_CALL_INACCESSIBLE_READ \
                                   (1 << GSS_C_CALLING_ERROR_OFFSET)
#define GSS_S_CALL_INACCESSIBLE_WRITE \
                                   (2 << GSS_C_CALLING_ERROR_OFFSET)
#define GSS_S_CALL_BAD_STRUCTURE \
                                   (3 << GSS_C_CALLING_ERROR_OFFSET)

/*
 * Routine errors:
 */
#define GSS_S_BAD_MECH (1 << GSS_C_ROUTINE_ERROR_OFFSET)
#define GSS_S_BAD_NAME (2 << GSS_C_ROUTINE_ERROR_OFFSET)
#define GSS_S_BAD_NAMETYPE (3 << GSS_C_ROUTINE_ERROR_OFFSET)
#define GSS_S_BAD_BINDINGS (4 << GSS_C_ROUTINE_ERROR_OFFSET)
#define GSS_S_BAD_STATUS (5 << GSS_C_ROUTINE_ERROR_OFFSET)
#define GSS_S_BAD_SIG (6 << GSS_C_ROUTINE_ERROR_OFFSET)
#define GSS_S_NO_CRED (7 << GSS_C_ROUTINE_ERROR_OFFSET)
#define GSS_S_NO_CONTEXT (8 << GSS_C_ROUTINE_ERROR_OFFSET)
#define GSS_S_DEFECTIVE_TOKEN (9 << GSS_C_ROUTINE_ERROR_OFFSET)
#define GSS_S_DEFECTIVE_CREDENTIAL (10 << GSS_C_ROUTINE_ERROR_OFFSET)
#define GSS_S_CREDENTIALS_EXPIRED (11 << GSS_C_ROUTINE_ERROR_OFFSET)
#define GSS_S_CONTEXT_EXPIRED (12 << GSS_C_ROUTINE_ERROR_OFFSET)
#define GSS_S_FAILURE (13 << GSS_C_ROUTINE_ERROR_OFFSET)

/*
 * Supplementary info bits:
 */
#define GSS_S_CONTINUE_NEEDED (1 << (GSS_C_SUPPLEMENTARY_OFFSET + 0))
#define GSS_S_DUPLICATE_TOKEN (1 << (GSS_C_SUPPLEMENTARY_OFFSET + 1))
#define GSS_S_OLD_TOKEN (1 << (GSS_C_SUPPLEMENTARY_OFFSET + 2))
#define GSS_S_UNSEQ_TOKEN (1 << (GSS_C_SUPPLEMENTARY_OFFSET + 3))


/*
 * Finally, function prototypes for the GSSAPI routines.
 */

OM_uint32 gss_acquire_cred
                 (OM_uint32*,       /* minor_status */
                  gss_name_t,       /* desired_name */
                  OM_uint32,        /* time_req */
                  gss_OID_set,      /* desired_mechs */
                  int,              /* cred_usage */
                  gss_cred_id_t*,   /* output_cred_handle */
                  gss_OID_set*,     /* actual_mechs */
                  OM_uint32*        /* time_rec */
                 );

OM_uint32 gss_release_cred
                 (OM_uint32*,       /* minor_status */
                  gss_cred_id_t*    /* cred_handle */
                 );

OM_uint32 gss_init_sec_context
                 (OM_uint32*,       /* minor_status */
                  gss_cred_id_t,    /* claimant_cred_handle */
                  gss_ctx_id_t*,    /* context_handle */
                  gss_name_t,       /* target_name */
                  gss_OID,          /* mech_type */
                  int,              /* req_flags */
                  OM_uint32,        /* time_req */
                  gss_channel_bindings_t,
                                    /* input_chan_bindings */
                  gss_buffer_t,     /* input_token */
                  gss_OID*,         /* actual_mech_type */
                  gss_buffer_t,     /* output_token */
                  int*,             /* ret_flags */
                  OM_uint32*        /* time_rec */
                 );

OM_uint32 gss_accept_sec_context
                 (OM_uint32*,       /* minor_status */
                  gss_ctx_id_t*,    /* context_handle */
                  gss_cred_id_t,    /* verifier_cred_handle */
                  gss_buffer_t,     /* input_token_buffer */
                  gss_channel_bindings_t,
                                    /* input_chan_bindings */
                  gss_name_t*,      /* src_name */
                  gss_OID*,         /* mech_type */
                  gss_buffer_t,     /* output_token */
                  int*,             /* ret_flags */
                  OM_uint32*,       /* time_rec */
                  gss_cred_id_t*    /* delegated_cred_handle */
                 );

OM_uint32 gss_process_context_token
                 (OM_uint32*,       /* minor_status */
                  gss_ctx_id_t,     /* context_handle */
                  gss_buffer_t      /* token_buffer */
                 );

OM_uint32 gss_delete_sec_context
                 (OM_uint32*,       /* minor_status */
                  gss_ctx_id_t*,    /* context_handle */
                  gss_buffer_t      /* output_token */
                 );

OM_uint32 gss_context_time
                 (OM_uint32*,       /* minor_status */
                  gss_ctx_id_t,     /* context_handle */
                  OM_uint32*        /* time_rec */
                 );

OM_uint32 gss_sign
                 (OM_uint32*,       /* minor_status */
                  gss_ctx_id_t,     /* context_handle */
                  int,              /* qop_req */
                  gss_buffer_t,     /* message_buffer */
                  gss_buffer_t      /* message_token */
                 );

OM_uint32 gss_verify
                 (OM_uint32*,       /* minor_status */
                  gss_ctx_id_t,     /* context_handle */
                  gss_buffer_t,     /* message_buffer */
                  gss_buffer_t,     /* token_buffer */
                  int*              /* qop_state */
                 );

OM_uint32 gss_seal
                 (OM_uint32*,       /* minor_status */
                  gss_ctx_id_t,     /* context_handle */
                  int,              /* conf_req_flag */
                  int,              /* qop_req */
                  gss_buffer_t,     /* input_message_buffer */
                  int*,             /* conf_state */
                  gss_buffer_t      /* output_message_buffer */
                 );

OM_uint32 gss_unseal
                 (OM_uint32*,       /* minor_status */
                  gss_ctx_id_t,     /* context_handle */
                  gss_buffer_t,     /* input_message_buffer */
                  gss_buffer_t,     /* output_message_buffer */
                  int*,             /* conf_state */
                  int*              /* qop_state */
                 );

OM_uint32 gss_display_status
                 (OM_uint32*,       /* minor_status */
                  OM_uint32,        /* status_value */
                  int,              /* status_type */
                  gss_OID,          /* mech_type */
                  int*,             /* message_context */
                  gss_buffer_t      /* status_string */
                 );

OM_uint32 gss_indicate_mechs
                 (OM_uint32*,       /* minor_status */
                  gss_OID_set*      /* mech_set */
                 );

OM_uint32 gss_compare_name
                 (OM_uint32*,       /* minor_status */
                  gss_name_t,       /* name1 */
                  gss_name_t,       /* name2 */
                  int*              /* name_equal */
                 );

OM_uint32 gss_display_name
                 (OM_uint32*,      /* minor_status */
                  gss_name_t,      /* input_name */
                  gss_buffer_t,     /* output_name_buffer */
                  gss_OID*         /* output_name_type */
                 );

OM_uint32 gss_import_name
                 (OM_uint32*,       /* minor_status */
                  gss_buffer_t,     /* input_name_buffer */
                  gss_OID,          /* input_name_type */
                  gss_name_t*       /* output_name */
                 );

OM_uint32 gss_release_name
                 (OM_uint32*,       /* minor_status */
                  gss_name_t*       /* input_name */
                 );

OM_uint32 gss_release_buffer
                 (OM_uint32*,       /* minor_status */
                  gss_buffer_t      /* buffer */
                 );

OM_uint32 gss_release_oid_set
                 (OM_uint32*,       /* minor_status */
                  gss_OID_set*      /* set */
                 );

OM_uint32 gss_inquire_cred
                 (OM_uint32 *,      /* minor_status */
                  gss_cred_id_t,    /* cred_handle */
                  gss_name_t *,     /* name */
                  OM_uint32 *,      /* lifetime */
                  int *,            /* cred_usage */
                  gss_OID_set *     /* mechanisms */
                 );


/* Now the DCE-specific stuff						    */

#define GSSDCE_S_SUCCESS gssdce_s_success
#define GSSDCE_S_OPERATION_INCOMPLETE  gssdce_s_operation_incomplete  
#define GSSDCE_S_INVALID_HANDLE gssdce_s_invalid_handle 
#define GSSDCE_S_NO_LOGIN_CONTEXT gssdce_s_no_login_context 
#define GSSDCE_S_UNEXPECTED_ERROR gssdce_s_unexpected_error 
#define GSSDCE_S_CREDENTIAL_CORRUPT gssdce_s_credential_corrupt 
#define GSSDCE_S_UNIMPLEMENTED_FEATURE gssdce_s_unimplemented_feature 
#define GSSDCE_S_BAD_USAGE gssdce_s_bad_usage 
#define GSSDCE_S_NAME_REQUIRED gssdce_s_name_required 
#define GSSDCE_S_BAD_NAME gssdce_s_bad_name 
#define GSSDCE_S_UNKNOWN_PRINCIPAL_NAME gssdce_s_unknown_principal_name 
#define GSSDCE_S_COULDNT_REGISTER_SERVER gssdce_s_couldnt_register_server 
#define GSSDCE_S_COULDNT_VALIDATE_CONTEXT gssdce_s_couldnt_validate_context 
#define GSSDCE_S_NO_RGY_AVAILABLE gssdce_s_no_rgy_available 
#define GSSDCE_S_COULDNT_TRANSLATE_NAME gssdce_s_couldnt_translate_name 
#define GSSDCE_S_LOCKING_ERROR gssdce_s_locking_error 
#define GSSDCE_S_OUT_OF_MEMORY gssdce_s_out_of_memory 
#define GSSDCE_S_PARAM_REQUIRED gssdce_s_param_required 
#define GSSDCE_S_UNSUPPORTED_MECHANISM gssdce_s_unsupported_mechanism 
#define GSSDCE_S_COULDNT_GET_MECH_CRED gssdce_s_couldnt_get_mech_cred 
#define GSSDCE_S_COULDNT_GET_SERVER_TICKET gssdce_s_couldnt_get_server_ticket 
#define GSSDCE_S_COULDNT_BUILD_AUTHENTICATOR gssdce_s_couldnt_build_authenticator 
#define GSSDCE_S_INVALID_CONTEXT_STATE gssdce_s_invalid_context_state 
#define GSSDCE_S_COULDNT_GENERATE_KEY gssdce_s_couldnt_generate_key 
#define GSSDCE_S_COULDNT_GENERATE_IVEC gssdce_s_couldnt_generate_ivec 
#define GSSDCE_S_COULDNT_DECODE_RESPONSE gssdce_s_couldnt_decode_response 
#define GSSDCE_S_BER_ENCODING_ERROR gssdce_s_ber_encoding_error 
#define GSSDCE_S_BER_DECODING_ERROR gssdce_s_ber_decoding_error 
#define GSSDCE_S_AUTHENTICATION_ERROR gssdce_s_authentication_error 
#define GSSDCE_S_COULDNT_BUILD_REPLY gssdce_s_couldnt_build_reply 
#define GSSDCE_S_UNSUPPORTED_NAME_TYPE gssdce_s_unsupported_name_type 
#define GSSDCE_S_UNSUPPORTED_AUTHZ_PROTOCOL gssdce_s_unsupported_authz_protocol 
#define GSSDCE_S_BINDINGS_LENGTH_ERROR gssdce_s_bindings_length_error 
#define GSSDCE_S_BINDINGS_VALUE_ERROR gssdce_s_bindings_value_error 
#define GSSDCE_S_NO_BUFFER gssdce_s_no_buffer 
#define GSSDCE_S_SHORT_SIGNATURE gssdce_s_short_signature 
#define GSSDCE_S_LONG_SIGNATURE gssdce_s_long_signature 
#define GSSDCE_S_BAD_SIGNATURE gssdce_s_bad_signature 
#define GSSDCE_S_BAD_SIG_SEQ gssdce_s_bad_sig_seq 
#define GSSDCE_S_BAD_SEQUENCE gssdce_s_bad_sequence 
#define GSSDCE_S_REFLECTED_PACKET gssdce_s_reflected_packet 
#define GSSDCE_S_MESSAGE_OMITTED gssdce_s_message_omitted 
#define GSSDCE_S_MESSAGE_ALREADY_SEEN gssdce_s_message_already_seen 
#define GSSDCE_S_MESSAGE_LATE gssdce_s_message_late 
#define GSSDCE_S_MESSAGE_OUT_OF_ORDER gssdce_s_message_out_of_order 
#define GSSDCE_S_UNSUPPORTED_SIGNATURE_ALGORITHM gssdce_s_unsupported_signature_algorithm 
#define GSSDCE_S_UNRECOGNIZED_TOKEN_TYPE gssdce_s_unrecognized_token_type 
#define GSSDCE_S_SEAL_ALGORITHM_NOT_ALLOWED gssdce_s_seal_algorithm_not_allowed 
#define GSSDCE_S_UNSUPPORTED_ENCRYPTION_ALGORITHM gssdce_s_unsupported_encryption_algorithm 
#define GSSDCE_S_SHORT_TOKEN gssdce_s_short_token 
#define GSSDCE_S_UNSUPPORTED_TOKEN_VERSION gssdce_s_unsupported_token_version 
#define GSSDCE_S_UNEXPECTED_TOKEN_TYPE gssdce_s_unexpected_token_type 
#define GSSDCE_S_COULDNT_DECRYPT_TOKEN gssdce_s_couldnt_decrypt_token 
#define GSSDCE_S_BAD_PLAINTEXT_PADDING gssdce_s_bad_plaintext_padding 
#define GSSDCE_S_INCORRECT_SERVER_PRINCIPAL gssdce_s_incorrect_server_principal 
#define GSSDCE_S_BAD_STATUS_TYPE gssdce_s_bad_status_type 
#define GSSDCE_S_BAD_STATUS_VALUE gssdce_s_bad_status_value 
#define GSSDCE_S_NEED_ACCEPTOR_CONTEXT gssdce_s_need_acceptor_context
#define GSSDCE_S_NO_PAC_AVAILABLE gssdce_s_no_pac_available

extern gss_OID GSSDCE_C_OID_DCE_KRBV5_DES;
extern gss_OID GSSDCE_C_OID_KRBV5_DES;
extern gss_OID GSSDCE_C_OID_DCENAME;

OM_uint32 gssdce_extract_creds_from_sec_context
                (OM_uint32 *,       /* minor_status */
                 gss_ctx_id_t,          /* context_handle */
                 rpc_authz_cred_handle_t *     /* output_creds */
                );

OM_uint32 gssdce_extract_PAC_from_sec_context
                (OM_uint32 *,       /* minor_status */
                 gss_ctx_id_t,          /* context_handle */
                 sec_id_pac_t *     /* output_PAC */
                );


OM_uint32 gssdce_login_context_to_cred
                (OM_uint32 *,       /* minor_status */
                 sec_login_handle_t, /* login_context */
                 OM_uint32,         /* lifetime_req */
                 gss_OID_set,       /* desired_mechs */
                 gss_cred_id_t *,   /* output_cred_handle */
                 gss_OID_set *,     /* actual_mechs */
                 OM_uint32 *        /* lifetime_rec */
                );

OM_uint32 gssdce_cred_to_login_context
                (OM_uint32 *,       /* minor_status */
                 gss_cred_id_t,         /* cred_handle */
                 sec_login_handle_t * /* output_login_context */
                );

OM_uint32 gssdce_register_acceptor_identity
		(OM_uint32 *,	    /* minor_status */
		 gss_name_t,        /* server_principal_name */
		 rpc_auth_key_retrieval_fn_t,	/* get_key_fn */
		 void *		    /* arg */
		);

/* Some convenience routines						    */
OM_uint32 gssdce_release_oid
                 (OM_uint32*,       /* minor_status */
                  gss_OID*	    /* OID */
                 );
/* gssdce_release_oid is here for completeness only.  Although some of the  */
/* GSSAPI routines return OIDs, these are only pointers to structures	    */
/* maintained in static storage.  gssdce_release_oid is smart enough to	    */
/* silently ignore attempts to free these OIDs, but it will free both the   */
/* OID data and the corresponding descriptor for other OIDs.		    */

OM_uint32 gssdce_create_empty_oid_set
		(OM_uint32*,	    /* minor_status */
		 gss_OID_set*	    /* OID_set */
		);
/* Creates an empty OID set, to which members may subsequently be added by  */
/* calling gssdce_add_oid_set_member					    */

OM_uint32 gssdce_add_oid_set_member
		(OM_uint32*,	    /* minor_status */
		 gss_OID,	    /* Member OID */
		 gss_OID_set*	    /* OID_set */
	        );
/* Adds the specified OID to the set					    */

OM_uint32 gssdce_test_oid_set_member
		(OM_uint32*,	    /* minor_status */
		 gss_OID,	    /* member */
		 gss_OID_set,	    /* set */
		 int*		    /* 1 = present, 0 = absent */
		);

OM_uint32 gssdce_set_cred_context_ownership
		(OM_uint32*,	    /* minor_status */
		 gss_cred_id_t,	    /* credential */
		 int		    /* ownership */
		);

#define GSSDCE_C_OWNERSHIP_GSSAPI 0
#define GSSDCE_C_OWNERSHIP_APPLICATION 1

/* Some definitions to ease application portability between DCE & MIT	    */
/* Kerberos								    */

#define GSS_KRB5_INTEG_C_QOP_MD5 GSSDCE_C_QOP_MD5
#define GSS_KRB5_INTEG_C_QOP_DES_MD5 GSSDCE_C_QOP_DES_MD5
#define GSS_KRB5_INTEG_C_QOP_DES_MAC GSSDCE_C_QOP_DES_MAC

#endif /* GSSAPI_H_ */
/*  DEC/CMS REPLACEMENT HISTORY, Element GSSAPI.H */
/*  *15   25-AUG-1994 10:49:58 WRAY "" */
/*   10A2 11-AUG-1994 15:20:01 WRAY "Fix constant names for new protocol" */
/*   10A1  9-AUG-1994 12:07:20 WRAY "Support new MIT protocol" */
/*  *14    8-JUL-1994 15:07:35 WRAY "GSSAPI BL3" */
/*  *13   18-MAR-1994 13:54:03 WRAY "For BL2" */
/*  *12   11-MAR-1994 14:35:52 WRAY "OSF B:L1 version" */
/*  *11   10-MAR-1994 15:01:14 WRAY "CONST tags removed from OID declarations to avoid RIOS problem" */
/*  *10    9-MAR-1994 14:33:55 WRAY "" */
/*  *9     8-MAR-1994 15:12:20 WRAY "cms replace test-gssapi.c" */
/*  *8    25-JAN-1994 12:30:58 WRAY "Add copyright" */
/*  *7    17-JAN-1994 12:35:16 WRAY "" */
/*  *6    18-NOV-1993 15:30:07 WRAY "" */
/*  *5    11-NOV-1993 14:22:18 WRAY "" */
/*  *4     9-NOV-1993 15:28:24 WRAY "Add new routines" */
/*  *3    22-SEP-1993 16:42:44 WRAY "Incorporate into ODE" */
/*  *2    19-AUG-1993 08:31:52 WRAY "Fix message downcasing" */
/*  *1     1-MAR-1993 10:32:13 WRAY "Public header for DCE/GSSAPI" */
/*  DEC/CMS REPLACEMENT HISTORY, Element GSSAPI.H */
