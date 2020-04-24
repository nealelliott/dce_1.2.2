/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: krb5.h,v $
 * Revision 1.1.16.2  1996/10/03  14:56:51  arvind
 * 	Add pa_enc_ts
 * 	pick up more stuff from beta6: etype, SAM datatypes.
 * 	remove gratuitous extra indirections.
 * 	merge in from aha's branch.
 * 	[1996/09/16  21:25 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/2]
 *
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/10  14:58 UTC  aha  /main/DCE122_PK/2]
 *
 * 	Merge to DCE122_PK
 * 	[1996/06/07  18:07 UTC  aha  /main/DCE122_PK/aha_pk6/1]
 *
 * 	Changes for Public Key Login; change krb5_enc_pa_pk_as_rep_part
 * 	nonce from pointer to int.
 * 	[1996/06/04  18:50 UTC  psn  /main/DCE122_PK/1]
 *
 * 	Merge ASN.1 code and encoding functions for public key login
 * 	[1996/04/16  21:33 UTC  aha  /main/DCE_1.2/aha_pk2/1]
 *
 * 	Additions for public key login
 * 	[1996/01/03  19:13 UTC  psn  /main/DCE_1.2/1]
 *
 * Revision 1.1.16.1  1996/07/08  18:34:59  arvind
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/10  14:58 UTC  aha  /main/DCE122_PK/2]
 * 
 * 	Merge to DCE122_PK
 * 	[1996/06/07  18:07 UTC  aha  /main/DCE122_PK/aha_pk6/1]
 * 
 * 	Changes for Public Key Login; change krb5_enc_pa_pk_as_rep_part
 * 	nonce from pointer to int.
 * 	[1996/06/04  18:50 UTC  psn  /main/DCE122_PK/1]
 * 
 * 	Merge ASN.1 code and encoding functions for public key login
 * 	[1996/04/16  21:33 UTC  aha  /main/DCE_1.2/aha_pk2/1]
 * 
 * 	Additions for public key login
 * 	[1996/01/03  19:13 UTC  psn  /main/DCE_1.2/1]
 * 
 * Revision 1.1.14.3  1996/02/18  23:02:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:08  marty]
 * 
 * Revision 1.1.14.2  1995/12/08  17:42:50  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/01  19:00 UTC  mdf
 * 	"Resolve CHFts14998"
 * 	[1995/04/26  12:07 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_3/1]
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/15  00:00 UTC  rps
 * 	copyright
 * 	[1995/04/05  17:05 UTC  rps  /main/MOTHRA_panic/2]
 * 
 * 	add preauth subtype
 * 	[1995/03/04  01:56 UTC  rps  /main/rps_hc/2]
 * 
 * 	merge some of krb5beta4patch3
 * 	[1995/01/29  21:44 UTC  rps  /main/rps_hc/1]
 * 
 * Revision 1.1.10.1  1994/10/25  22:00:41  mullan_s
 * 	[OT 12495] Plaintext third party preauth fixes : add new
 * 	data structs for error info & new preauth key data.
 * 	[1994/10/25  22:00:19  mullan_s]
 * 
 * Revision 1.1.4.2  1994/08/26  18:46:44  mullan_s
 * 	[OT11557] Subtype DCE preauth data.
 * 	[1994/08/25  18:27:38  mullan_s]
 * 
 * Revision 1.1.4.1  1994/05/11  19:17:03  ahop
 * 	hp_sec_to_osf_2 drop
 * 	Add krb5_pa_third_party struct for DCE third party preauthentication.
 * 	Add krb5_time_and_key struct for DCE third party preauthentication.
 * 	[1994/04/29  21:14:19  ahop]
 * 
 * Revision 1.1.2.2  1992/12/29  14:00:04  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:52:55  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:49:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
**
** Copyright (c) Hewlett-Packard Company 1991, 1994, 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 *
 * Copyright 1989,1990 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America is assumed
 *   to require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 * 
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 * 
 *
 * General definitions for Kerberos version 5.
 */

#ifndef KRB5_GENERAL__
#define KRB5_GENERAL__

#ifdef OSF_DCE
#define OSF_DCE_ARCHAISM
#define OSF_DCE_FEATURE
#   include <krb5/osf_dce.h>
#endif

#ifndef KRB5_SYSTYPES__
#define KRB5_SYSTYPES__
#include <sys/types.h>
#endif /* KRB5_SYSTYPES__ */

#include <krb5/config.h>

#include <krb5/base-defs.h>
#include <krb5/hostaddr.h>
#include <krb5/encryption.h>
#include <krb5/fieldbits.h>
#include <krb5/errors.h>
#include <krb5/proto.h>
#include <krb5/macros.h>
#include <krb5/error_def.h>

/* Time set */
typedef struct _krb5_ticket_times {
    krb5_timestamp authtime; /* XXX ? should ktime in KDC_REP == authtime
				in ticket? otherwise client can't get this */ 
    krb5_timestamp starttime;		/* optional in ticket, if not present,
					   use authtime */
    krb5_timestamp endtime;
    krb5_timestamp renew_till;
} krb5_ticket_times;

/* structure for auth data */
typedef struct _krb5_authdata {
    krb5_authdatatype ad_type;
    int length;
    krb5_octet *contents;
} krb5_authdata;

/* structure for transited encoding */
typedef struct _krb5_transited {
    krb5_octet tr_type;
    krb5_data tr_contents;
} krb5_transited;

typedef struct _krb5_enc_tkt_part {
    /* to-be-encrypted portion */
    krb5_flags flags;			/* flags */
    krb5_keyblock *session;		/* session key: includes keytype */
    krb5_principal client;		/* client name/realm */
    krb5_transited transited;		/* list of transited realms */
    krb5_ticket_times times;		/* auth, start, end, renew_till */
    krb5_address **caddrs;		/* array of ptrs to addresses */
    krb5_authdata **authorization_data;	/* auth data */
} krb5_enc_tkt_part;

typedef struct _krb5_ticket {
    /* cleartext portion */
    krb5_principal server;		/* server name/realm */
    krb5_enc_data enc_part;		/* encryption type, kvno, encrypted
					   encoding */
    krb5_enc_tkt_part *enc_part2;	/* ptr to decrypted version, if
					   available */
} krb5_ticket;

/* the unencrypted version */
typedef struct _krb5_authenticator {
    krb5_principal client;		/* client name/realm */
    krb5_checksum *checksum;		/* checksum, includes type, optional */
    krb5_int32 cusec;			/* client usec portion */
    krb5_timestamp ctime;		/* client sec portion */
    krb5_keyblock *subkey;		/* true session key, optional */
    krb5_int32 seq_number;		/* sequence #, optional */
} krb5_authenticator;

typedef struct _krb5_tkt_authent {
    krb5_ticket *ticket;
    krb5_authenticator *authenticator;
    krb5_flags ap_options;
} krb5_tkt_authent;

/* credentials:  Ticket, session key, etc. */
typedef struct _krb5_creds {
    krb5_principal client;		/* client's principal identifier */
    krb5_principal server;		/* server's principal identifier */
    krb5_keyblock keyblock;		/* session encryption key info */
    krb5_ticket_times times;		/* lifetime info */
    krb5_boolean is_skey;		/* true if ticket is encrypted in
					   another ticket's skey */
    krb5_flags ticket_flags;		/* flags in ticket */
    krb5_address **addresses;		/* addrs in ticket */
    krb5_data ticket;			/* ticket string itself */
    krb5_data second_ticket;		/* second ticket, if related to
					   ticket (via DUPLICATE-SKEY or
					   ENC-TKT-IN-SKEY) */
    krb5_authdata **authdata;		/* authorization data */
} krb5_creds;

/* Last request fields */
typedef struct _krb5_last_req_entry {
    krb5_octet lr_type;
    krb5_timestamp value;
} krb5_last_req_entry;

/* pre-authentication data */
typedef struct _krb5_pa_data {
    krb5_ui_2  pa_type;
    int length;
    krb5_octet *contents;
} krb5_pa_data;

typedef struct _krb5_kdc_req {
    krb5_msgtype msg_type;		/* AS_REQ or TGS_REQ? */
    krb5_pa_data **padata;		/* e.g. encoded AP_REQ */
    /* real body */
    krb5_flags kdc_options;		/* requested options */
    krb5_principal client;		/* includes realm; optional */
    krb5_principal server;		/* includes realm (only used if no
					   client) */
    krb5_timestamp from;		/* requested starttime */
    krb5_timestamp till;		/* requested endtime */
    krb5_timestamp rtime;		/* (optional) requested renew_till */
    krb5_int32 nonce;			/* nonce to match request/response */
    int netypes;			/* # of etypes, must be positive */
    krb5_enctype *etype;		/* requested encryption type(s) */
    krb5_address **addresses;		/* requested addresses, optional */
    krb5_enc_data authorization_data;	/* encrypted auth data; OPTIONAL */
    krb5_authdata **unenc_authdata;	/* unencrypted auth data,
					   if available */
    krb5_ticket **second_ticket;	/* second ticket array; OPTIONAL */
} krb5_kdc_req;

typedef struct _krb5_enc_kdc_rep_part {
    /* encrypted part: */
    krb5_keyblock *session;		/* session key */
    krb5_last_req_entry **last_req;	/* array of ptrs to entries */
    krb5_int32 nonce;			/* nonce from request */
    krb5_timestamp key_exp;		/* expiration date */
    krb5_flags flags;			/* ticket flags */
    krb5_ticket_times times;		/* lifetime info */
    krb5_principal server;		/* server's principal identifier */
    krb5_address **caddrs;		/* array of ptrs to addresses,
					   optional */
} krb5_enc_kdc_rep_part;

typedef struct _krb5_kdc_rep {
    /* cleartext part: */
    krb5_msgtype msg_type;		/* AS_REP or KDC_REP? */
    krb5_pa_data **padata;		/* preauthentication data from KDC */
    krb5_principal client;		/* client's principal identifier */
    krb5_ticket *ticket;		/* ticket */
    krb5_enc_data enc_part;		/* encryption type, kvno, encrypted
					   encoding */
    krb5_enc_kdc_rep_part *enc_part2;	/* unencrypted version, if available */
} krb5_kdc_rep;

/* error message structure */
typedef struct _krb5_error {
    /* some of these may be meaningless in certain contexts */
    krb5_timestamp ctime;		/* client sec portion; optional */
    krb5_int32 cusec;			/* client usec portion; optional */
    krb5_int32 susec;			/* server usec portion */
    krb5_timestamp stime;		/* server sec portion */
    krb5_ui_4 error;			/* error code (protocol error #'s) */
    krb5_principal client;		/* client's principal identifier;
					   optional */
    krb5_principal server;		/* server's principal identifier */
    krb5_data text;			/* descriptive text */
    krb5_data e_data;			/* additional error-describing data */
} krb5_error;

typedef struct _krb5_ap_req {
    krb5_flags ap_options;		/* requested options */
    krb5_ticket *ticket;		/* ticket */
    krb5_enc_data authenticator;	/* authenticator (already encrypted) */
} krb5_ap_req;

typedef struct _krb5_ap_rep {
    krb5_enc_data enc_part;
} krb5_ap_rep;

typedef struct _krb5_ap_rep_enc_part {
    krb5_timestamp ctime;		/* client time, seconds portion */
    krb5_int32 cusec;			/* client time, microseconds portion */
    krb5_keyblock *subkey;		/* true session key, optional */
    krb5_int32 seq_number;		/* sequence #, optional */
} krb5_ap_rep_enc_part;

typedef struct _krb5_response {
    krb5_octet message_type;
    krb5_data response;
} krb5_response;

typedef struct _krb5_safe {
    krb5_data user_data;		/* user data */
    krb5_timestamp timestamp;		/* client time, optional */
    krb5_int32 usec;			/* microsecond portion of time,
					   optional */
    krb5_int32 seq_number;		/* sequence #, optional */
    krb5_address *s_address;		/* sender address */
    krb5_address *r_address;		/* recipient address, optional */
    krb5_checksum *checksum;		/* data integrity checksum */
} krb5_safe;

typedef struct _krb5_priv {
    krb5_enc_data enc_part;		/* encrypted part */
} krb5_priv;

typedef struct _krb5_priv_enc_part {
    krb5_data user_data;		/* user data */
    krb5_timestamp timestamp;		/* client time, optional */
    krb5_int32 usec;			/* microsecond portion of time, opt. */
    krb5_int32 seq_number;		/* sequence #, optional */
    krb5_address *s_address;		/* sender address */
    krb5_address *r_address;		/* recipient address, optional */
} krb5_priv_enc_part;

#ifdef OSF_DCE_FEATURE
/*
 * Different subtypes of DCE padata
 *
 * OSF_DCE_THIRD_PARTY - protocol defined
 * in DCE RFC 26.
 */
#define OSF_DCE_THIRD_PARTY     	1
/*
 * OSF_DCE_THIRD_PARTY_PLAINTEXT - extension
 * to RFC 26 to preserve plaintext portion
 * of password.
 */
#define OSF_DCE_THIRD_PARTY_PLAINTEXT	2

/*
 * Different subtypes of DCE error data found in krb5 e_data field
 *
 * OSF_DCE_ERR_PREAUTH_SALT - used when the account's salt
 * does not match the server's storage.
 */
#define OSF_DCE_ERR_PREAUTH_SALT        1
/*
 * OSF_DCE_ERR_PREAUTH - used to indicate there is additional
 * information regarding why the preauthentication failed.
 */
#define OSF_DCE_ERR_PREAUTH		2
/*
 * _INTERCELL - used to indicate that although no key was obtained for
 * the krbtgt/fromcell principal, proceed anyway (for transitive trust)
 */
#define OSF_DCE_ERR_PREAUTH_INTERCELL	3

/*
 * Preserve error return status, since the server now checks 
 * to see if the account is valid.
 */

#define OSF_DCE_ERR_ADMIN_VALID		4

/*
 * Structure to hold preauth subtype data
 */
typedef struct _krb5_sub_pa_data {
    krb5_ui_2  pa_subtype;
    int length;
    krb5_octet *contents;
} krb5_sub_pa_data;

/*
 * Structure to hold subtypes of error data
 */
typedef struct _krb5_sub_err_data {
    krb5_ui_2  err_subtype;
    int length;
    krb5_octet *contents;
} krb5_sub_err_data;

/*
 * This structure contains various information
 * regarding why the server could not decrypt the
 * preauthentication data.
 */
typedef struct _krb5_dce_pa_err_info {
    krb5_error_code   err;            /* internal preauth error */
    krb5_preauthtype  type;           /* min. type of preauth allowed */
    krb5_preauthtype  subtype;        /* min. subtype of preauth allowed */
    krb5_data         salt;           /* required salt */
} krb5_dce_pa_err_info;

/*
 * This structure is part of the DCE third party preauthentication
 * data. It will be encrypted in the client's key.
 */
typedef struct _krb5_time_and_key {
    krb5_timestamp timestamp;           /* current time of KDC */
    krb5_keyblock key;                  /* key used in reply */
} krb5_time_and_key;

/*
 * This struct is part if the DCE third party plaintext preauth
 * data. It will be encrypted in the random key (established by dced)
 */
typedef struct _krb5_dce_pa_keys {
    krb5_data   user_key;               /* user's key (plaintext) */
    krb5_keyblock reply_key;            /* random reply key */
} krb5_dce_pa_keys;

/*
 * This structure holds the three fields of DCE third party
 * preauthentication data. Each field is encoded.
 */
typedef struct _krb5_pa_third_party {
    krb5_data *machine_tgt_data;
    krb5_data *single_ebuf_data;
    krb5_data *double_ebuf_data;
} krb5_pa_third_party;

typedef struct _krb5_pa_enc_ts {
    krb5_timestamp	patimestamp;
    krb5_int32		pausec;
} krb5_pa_enc_ts;

/********************
 * Public Key Login *
 ********************/

typedef struct _krb5_pk_authenticator {
    krb5_checksum     *cksum;     /* checksum, includes type, optional */
    krb5_int32        cusec;      /* client usec portion */
    krb5_timestamp    ctime;      /* client sec portion */
    krb5_int32        nonce;      /* nonce */
    krb5_principal    kdcname;    /* KDC name/realm */
} krb5_pk_authenticator;

typedef struct _krb5_signed_pk_authenticator {
    krb5_pk_authenticator pk_authenticator;
    krb5_enc_data         authentsig;        /* signature of pk_authenticator */
} krb5_signed_pk_authenticator;

typedef struct _krb5_pa_pk_as_root {
    krb5_data                    root_cert;   /* not used in DCE Pubkey */
    krb5_signed_pk_authenticator authent;
} krb5_pa_pk_as_root;

typedef struct _krb5_enc_pa_pk_as_rep_part {
    krb5_keyblock      enc_reply_key; /* used to encrypt reply */
    krb5_int32         nonce;         /* copied from request */
} krb5_enc_pa_pk_as_rep_part;

typedef struct _krb5_enc_pa_pk_as_rep_part_shell {
    krb5_enc_pa_pk_as_rep_part reply;
    krb5_enc_data 	replysig; /* signature of reply */
} krb5_enc_pa_pk_as_rep_part_shell;

typedef struct _krb5_pa_pk_as_rep {
    krb5_data     kdc_cert;          /* not used in DCE Pubkey */
    krb5_enc_data enc_tmp_key;       /* krb5_keyblock */
    krb5_enc_data enc_shell;         /* krb5_enc_pa_pk_as_rep_part_shell */
} krb5_pa_pk_as_rep;

/***************************
 * End of Public Key Login *
 ***************************/

#endif /* OSF_DCE_FEATURE */


/*
 * This structure is returned in the e-data field of the KRB-ERROR
 * message when the error calling for an alternative form of
 * authentication is returned, KRB_AP_METHOD.
 */
typedef struct _krb5_alt_method {
#ifndef OSF_DCE_ARCHAISM
	krb5_magic	magic;
#endif
	krb5_int32	method;
	int		length;
	krb5_octet	*data;
} krb5_alt_method;

/*
 * A null-terminated array of this structure is returned by the KDC as
 * the data part of the ETYPE_INFO preauth type.  It informs the
 * client which encryption types are supported.
 */
typedef struct _krb5_etype_info_entry {
#ifndef OSF_DCE_ARCHAISM
	krb5_magic	magic;
#endif
	krb5_enctype	etype;
	int		length;
	krb5_octet	*salt;
} krb5_etype_info_entry;

typedef krb5_etype_info_entry ** krb5_etype_info;

/*
 * a sam_challenge is returned for alternate preauth 
 */
/*
          SAMFlags ::= BIT STRING {
              use-sad-as-key[0],
              send-encrypted-sad[1],
              must-pk-encrypt-sad[2]
          }
 */
/*
          PA-SAM-CHALLENGE ::= SEQUENCE {
              sam-type[0]                 INTEGER,
              sam-flags[1]                SAMFlags,
              sam-type-name[2]            GeneralString OPTIONAL,
              sam-track-id[3]             GeneralString OPTIONAL,
              sam-challenge-label[4]      GeneralString OPTIONAL,
              sam-challenge[5]            GeneralString OPTIONAL,
              sam-response-prompt[6]      GeneralString OPTIONAL,
              sam-pk-for-sad[7]           EncryptionKey OPTIONAL,
              sam-nonce[8]                INTEGER OPTIONAL,
              sam-cksum[9]                Checksum OPTIONAL
          }
*/
/* sam_type values -- informational only */
#define PA_SAM_TYPE_ENIGMA     1   /*  Enigma Logic */
#define PA_SAM_TYPE_DIGI_PATH  2   /*  Digital Pathways */
#define PA_SAM_TYPE_SKEY_K0    3   /*  S/key where  KDC has key 0 */
#define PA_SAM_TYPE_SKEY       4   /*  Traditional S/Key */
#define PA_SAM_TYPE_SECURID    5   /*  Security Dynamics */
#define PA_SAM_TYPE_GRAIL    128 /* experimental */

typedef struct _krb5_predicted_sam_response {
#ifndef OSF_DCE_ARCHAISM
	krb5_magic	magic;
#endif
	krb5_keyblock	sam_key;
} krb5_predicted_sam_response;

typedef struct _krb5_sam_challenge {
#ifndef OSF_DCE_ARCHAISM
	krb5_magic	magic;
#endif
	krb5_int32	sam_type; /* information */
	krb5_flags	sam_flags; /* KRB5_SAM_* values */
	krb5_data	sam_type_name;
	krb5_data	sam_track_id;
	krb5_data	sam_challenge_label;
	krb5_data	sam_challenge;
	krb5_data	sam_response_prompt;
	krb5_data	sam_pk_for_sad;
	krb5_int32	sam_nonce;
	krb5_checksum	sam_cksum;
} krb5_sam_challenge;

typedef struct _krb5_sam_key {	/* reserved for future use */
#ifndef OSF_DCE_ARCHAISM
	krb5_magic	magic;
#endif
	krb5_keyblock	sam_key;
} krb5_sam_key;

typedef struct _krb5_enc_sam_response_enc {
#ifndef OSF_DCE_ARCHAISM
	krb5_magic	magic;
#endif
	krb5_int32	sam_nonce;
	krb5_timestamp	sam_timestamp;
	krb5_int32	sam_usec;
	krb5_data	sam_passcode;
} krb5_enc_sam_response_enc;

typedef struct _krb5_sam_response {
#ifndef OSF_DCE_ARCHAISM
	krb5_magic	magic;
#endif
	krb5_int32	sam_type; /* informational */
	krb5_flags	sam_flags; /* KRB5_SAM_* values */
	krb5_data	sam_track_id; /* copied */
	krb5_enc_data	sam_enc_key; /* krb5_sam_key - future use */
	krb5_enc_data	sam_enc_nonce_or_ts; /* krb5_enc_sam_response_enc */
	krb5_int32	sam_nonce;
	krb5_timestamp	sam_patimestamp;
} krb5_sam_response;

/* these need to be here so the typedefs are available for the prototypes */
#include <krb5/safepriv.h>
#include <krb5/ccache.h>
#include <krb5/rcache.h>
#include <krb5/keytab.h>
#include <krb5/func-proto.h>
#include <krb5/free.h>

/* The name of the Kerberos ticket granting service... and its size */
#define	KRB5_TGS_NAME		"krbtgt"
#define KRB5_TGS_NAME_SIZE	6

/* flags for recvauth */
#define KRB5_RECVAUTH_SKIP_VERSION	0x0001
#define KRB5_RECVAUTH_BADAUTHVERS	0x0002

#endif /* KRB5_GENERAL__ */
