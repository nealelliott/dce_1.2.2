/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpriv_util.h,v $
 * Revision 1.1.15.1  1996/05/10  13:21:19  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:49 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/01/31  18:00 UTC  hanfei
 * 	multi-cell group work: add header for rpriv_util_get_foreign_groupset()
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:36 UTC  psn
 * 	fix for CHFts13993 (and OSF CR# 12770).
 * 	[1995/01/10  16:35 UTC  greg  /main/HPDCE02/1]
 *
 * Revision 1.1.9.1  1994/10/25  20:23:42  kline_s
 * 	UHYP CR 12343: Enable Delegation audit points: utilize dce_aud_start_with_uuid()
 * 	to prevent audit from causing secd to go into an infinite loop.
 * 	[1994/10/25  20:21:02  kline_s]
 * 
 * Revision 1.1.2.6  1994/09/28  20:50:24  greg
 * 	CR11910: pass delegation chain in priv_attr_trig_query calls.
 * 	[1994/09/28  18:02:27  greg]
 * 
 * Revision 1.1.5.2  94/09/21  9:45:19  greg
 * 	Add delegate info to priv_attr_trig callouts.
 * 
 * Revision 1.1.2.5  1994/08/24  21:07:09  cuti
 * 	Add audit points.
 * 	[1994/08/24  20:08:17  cuti]
 * 
 * Revision 1.1.2.4  1994/08/18  20:25:38  greg
 * 	Cosmetic changes, mostly to convenience macros so that
 * 	the difference bewteen delegation tokens with and without
 * 	"confidential bytes" is a bit more obvious (confidential
 * 	bytes are not supported in 1.1, but the protocol hooks are
 * 	there).
 * 	[1994/08/16  19:36:38  greg]
 * 
 * Revision 1.1.2.3  1994/06/10  15:07:22  greg
 * 	intercell fixes for DCE1.1 beta
 * 	[1994/06/03  20:50:12  greg]
 * 
 * Revision 1.1.2.2  1994/06/02  21:27:16  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:17:53  mdf]
 * 
 * Revision 1.1.2.1  1994/05/11  19:28:02  ahop
 * 	hp_sec_to_osf_2 drop
 * 	first cut at a real delegation token.
 * 	 add common attribute processing routines
 * 	[1994/04/29  21:40:37  ahop]
 * 
 * $EndLog$
 */

/*  rpriv_util.h
**
** Copyright (c) Hewlett-Packard Company 1994
** Unpublished work. All Rights Reserved.
**
*/
/* 
 * Useful macros
 */

#ifndef rpriv_util_h__included
#define rpriv_util_h__included

#include <krb5/krb5.h>
#include <dce/sec_base.h>
#include <dce/id_base.h>
#include <dce/sec_attr_base.h>
#include <dce/id_epac.h>
#include <dce/rgynbase.h>
#include <dce/prvnbase.h>
#include <dce/authz_base.h>

#include <dce/audit.h>
#include <sec_aud_evts.h>

extern boolean32 priv_debug;
#define  DEBUG_PRINT(args) if (priv_debug) printf args

#define PRIV_KDC_REP_PADATA -2  /* unregistered pa-data type used to
				 * return encrypted
                                 * auth info to the client
				 */
#define INIT_AUX_RETURN_PARMS(aux_attr_result_p, app_tkt_result_p) \
    memset((aux_attr_result), 0, sizeof(rpriv_attr_result_t)); \
    memset((app_tkt_result), 0, sizeof(rpriv_app_tkt_result_t)); \
    SET_STATUS(&(aux_attr_result)->status, sec_rgy_not_implemented); \
    SET_STATUS(&(app_tkt_result)->status, sec_rgy_not_implemented);
 
#define CHECK_DELEGATION_TYPE(deleg_type,stp) \
    switch(deleg_type) { \
    case sec_id_deleg_type_none: \
    case sec_id_deleg_type_traced: \
    case sec_id_deleg_type_impersonation: \
	CLEAR_STATUS(stp); \
	break; \
    default: \
	SET_STATUS(stp, sec_priv_s_bad_deleg_type); \
	break; \
    }

/*
 * CMA currently defines the pthread_cancel_e exception in the wrong
 * set of include files - work around this problem by defining it here if
 * it is missing.... For other implementations of exc_handling.h, we 
 * expect that pthread_cancel_e is defined.
 */#ifdef CMA_INCLUDE
#   ifndef pthread_cancel_e
#       define pthread_cancel_e cma_e_alerted
#   endif
#endif


/* r p r i v _ k r b _ a u t h i n f o _ t
 *
 * A "convenience" type containing info needed to build a kerberos-based 
 * privilege ticket-granting ticket.  This structure is filled in and
 * validated in rpriv_krb_get_req_info.
 *
 * The krb_info field contains the kerberos authentication header info 
 * retrieved from authentication header in the following format (from krb5.h):
 * 
 * typedef struct _krb5_authenticator {
 *    krb5_principal client;        * client name/realm *
 *    krb5_checksum *checksum;      * checksum, includes type, optional *
 *    krb5_int32 cusec;         * client usec portion *
 *    krb5_timestamp ctime;     * client sec portion *
 *    krb5_keyblock *subkey;        * true session key, optional *
 *    krb5_int32 seq_number;        * sequence #, optional *
 * } krb5_authenticator;
 *
 * typedef struct _krb5_tkt_authent {
 *    krb5_ticket *ticket;
 *    krb5_authenticator *authenticator;
 *    krb5_flags ap_options;
 * } krb5_tkt_authent;
 *
 */
typedef struct _rpriv_krb_req_info_t {
    krb5_tkt_authent    *authent;        /* decoded/decrypted AP_REQ */
    krb5_enctype        etype;
    krb5_encrypt_block  eblock;          /* eblock for client's request subkey */
    krb5_authdata       **req_authdata;  /* auth data in krb5_tgs_req */
    krb5_int32          nonce;           /* from request */
} rpriv_krb_req_info_t;

/*
 * A convenience type for holding and passing 
 * DCE authorization info.  Constants used in 
 * the data_parts field are defined in priv_krb_util.h
 */
typedef struct _rpriv_dce_authdata_t {
    sec_rgy_name_t     client_princ;
    sec_rgy_name_t     client_cell;
    sec_rgy_name_t     client_tgt_princ;
    boolean32          local;
    unsigned32         data_parts;  /* which of the following are present? */
    sec_id_pac_t       v1_pac;
    sec_v1_1_authz_data_t v1_1_ad;
} rpriv_dce_authz_data_t;

void  rpriv_util_alloc_krb_req_info (
    rpriv_krb_req_info_t  **auth_info   /* [out] */
);

void  rpriv_util_free_krb_req_info (
    rpriv_krb_req_info_t  **auth_info   /* [in, out] */
);

void  rpriv_util_get_krb_req_info (
    krb5_data             *krb_req,
    rpriv_krb_req_info_t  **req_info,
    error_status_t        *stp
);

void rpriv_util_free_authz_data (
    rpriv_dce_authz_data_t  *authz_data
);

void  rpriv_util_get_dce_authz_info (
    unsigned32              aud_event,           /* [in] */ 
    handle_t                handle,              /* [in] */   
    rpriv_krb_req_info_t    *krb_req_info,       /* [in] */
    rpriv_dce_authz_data_t  *authz_data,         /* [out] */
    sec_id_pac_t            **req_authz_data,    /* [out] local pre-1.1 only */
    dce_aud_rec_t           *ard,                /* [out] */
    error_status_t           *stp                 /* [out] */
);

void  rpriv_util_get_ps_eblock (
    krb5_enctype        etype,         /* [in]  */
    krb5_kvno           req_kvno,      /* [in] */
    krb5_encrypt_block  *ps_eblock,    /* [out] */
    krb5_kvno           *ps_kvno,      /* [out] */
    error_status_t      *stp           /* [out] */
);

void  rpriv_util_finish_ps_eblock (
    krb5_encrypt_block  *ps_eblock    /* [in/out] */
);

void  rpriv_util_gen_seal (
    sec_bytes_t   *delegation_chain,  /* [in] */
    sec_id_seal_t *seal               /* [out] */
);

void  rpriv_util_verify_local_princ (
    dce_aud_rec_t  ard,            /* [in] */
    sec_rgy_name_t princ_name,  
    sec_rgy_name_t cell_name,
    sec_id_t       *req_princ_id,
    sec_id_t       *req_group_id,
    sec_id_t       *princ_id,
    sec_id_t       *group_id,
    sec_id_t       *cell_id,
    error_status_t *stp
);

void  rpriv_util_verify_local_groups (
    dce_aud_rec_t  ard,            /* [in] */
    unsigned char  *principal,         /* [in] - principal whose groupset needs 
				                   verification */
    unsigned16     req_num_groups,     /* [in] - num requested local groups */
    sec_id_t       *req_groups,        /* [in] - requested local groupset */
    unsigned16	   *num_groups,        /* [out] - num local groups verified */
    sec_id_t       **verified_groups,  /* [out] - verifed local groupset */
    error_status_t *stp                /* [out] */
);

void  rpriv_util_verify_local_attrs(
    unsigned_char_p_t   client_princ,    /* [in] - client principal name */
    sec_id_epac_data_t  *granted_epac,   /* [in] - certified principal epac */
    unsigned32          num_upstream_delegates, /* [in] */ 
    sec_id_epac_data_t  upstream_delegates[],   /* [in] - certified delegates */
    unsigned32          num_req_attrs,  /* [in] - num requested attrs */
    sec_attr_t          req_attrs[],    /* [in] - requested attr set */
    unsigned32	        *num_attrs,     /* [out] - num local attrs verified */
    sec_attr_t          **attrs,        /* [out] - verifed attr set */
    error_status_t      *stp            /* [out] - return status */
);

void  rpriv_util_parse_request (
    unsigned32              aud_event,         /* [in] */
    handle_t                handle,            /* [in] */
    rpriv_pickle_t          *ptgt_req,         /* [in] */
    rpriv_dce_authz_data_t  *authz_data,       /* [out] */
    rpriv_krb_req_info_t    **req_info,        /* [out] */
    dce_aud_rec_t           *ard,              /* [out] */
    error_status_t          *stp               /* [out] */
);

void  rpriv_util_parse_req_privs (
    sec_bytes_t         *requested_privs,  /* [in] */
    sec_id_epac_data_t  *epac_data,        /* [out] */
    error_status_t      *stp               /* [out] */
);

void  rpriv_util_verify_req_privs (
    dce_aud_rec_t       ard,
    sec_rgy_name_t      client_princ,       
    sec_rgy_name_t      client_cell,
    sec_id_epac_data_t  *req_epac_data,
    sec_id_epac_data_t  *granted_epac_data,
    error_status_t      *stp
 
);

void  rpriv_util_gen_v1_1_privs (
    sec_id_epac_data_t  *privs,             /* [in] */
    unsigned32          num_upstream_privs, /* [in] */
    sec_id_epac_data_t  *upstream_privs,    /* [in] */
    unsigned32          expiration,         /* [in] */
    sec_bytes_t         *granted_privs,     /* [out] */
    krb5_authdata       ***authdata,        /* [out] */
    error_status_t      *stp                /* [out] */

);

void  rpriv_util_build_ptgt (
    rpriv_krb_req_info_t   *req_info,        /* [in] */
    unsigned32             expiration,       /* [in] */
    krb5_authdata          **authdata,       /* [in] */
    krb5_ticket            **priv_tkt,       /* [out] */
    error_status_t         *stp              /* [out] */
);

void  rpriv_util_build_ptgt_rep
(
    rpriv_krb_req_info_t    *req_info,       /* [in] */
    krb5_pa_data            **padata_vec,    /* [in] */
    krb5_ticket             *ptgt,           /* [in] */
    rpriv_pickle_t          **ptgt_rep,      /* [out] */
    error_status_t          *stp             /* [out] */
);

void print_id ( sec_id_t  *id, char *preamble );

boolean32  rpriv_util_get_foreign_groupset
(
    uuid_t                    *realm_uuid,          /* [in] */
    uuid_t                    *princ_uuid,          /* [in] */
    sec_id_foreign_groupset_t   *foreign_groupset,  /* [out] */
    error_status_t              *stp                /* [out] */
);


/* Token version number is in byte 0 */
#define TOKEN_VNO(token_bytes) (*((unsigned8 *)&((token_bytes)[0])))

/* V0 token flag bits are in bytes 1 and 2 */
#define V0_TOKEN_FLAGS1(token_bytes) (*((unsigned8 *)&((token_bytes)[1])))
#define V0_TOKEN_FLAGS2(token_bytes) (*((unsigned8 *)&((token_bytes)[2])))

/* 
 * The priv server key version used to encrypt the ciphertext is in
 * byte 3 of a version 0 token.
 */
#define V0_TOKEN_KVNO(token_bytes)  (*((unsigned8 *)&((token_bytes)[3])))

/*
 * token ciphertext begins at byte 5
 */
#define V0_TOKEN_CIPHER(token_bytes) (&((token_bytes)[4]))

/* size of the required portion of a token */
#define V0_REQ_TOKENSIZE  36

/* size of complete token: required and optional */
#define V0_TOKENSIZE 52   

/* size of required cleartext: expiration + md5 digest */
#define V0_TOKEN_REQ_CLEAR_LEN 20 

/*
 * size of complete cleartext, required  + optional:
 *   expiration + md5 digest + conf bytes DES key + conf bytes IVEC
 */
#define V0_TOKEN_CLEAR_LEN  36

/*
 * size of required ciphertext:
 *   confounder + crc32 + expiration + md5 digest
 */
#define V0_TOKEN_REQ_CIPHER_LEN 32 

/* 
 * size of complete ciphertext, required + optional
 *   confounder + crc32 + expiration + md5 digest 
 *   + conf bytes DES key + conf bytes DES ivec
 */
#define V0_TOKEN_CIPHER_LEN 48

#endif /* rpriv_util_h__included */
