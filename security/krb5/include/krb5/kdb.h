/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: kdb.h,v $
 * Revision 1.1.11.3  1996/11/13  18:04:14  arvind
 * 	Fix server side of Third-Party Preauth
 * 	[1996/10/15  17:30 UTC  aha  /main/DCE_1.2.2/aha_pk10/1]
 *
 * 	Merge pre-auth and policy work from aha_pk9_5
 * 	[1996/10/15  15:45 UTC  aha  /main/DCE_1.2.2/aha_pk9_5/2]
 *
 * 	Add krb5_kdb_decode_int16 from KRB5 Beta 6
 * 	[1996/09/30  14:15 UTC  aha  /main/DCE_1.2.2/aha_pk9_5/1]
 *
 * 	Add flag for KRB5_KDB_PADATA_ROOT_CERT
 * 	[1996/09/27  14:05 UTC  aha  /main/DCE_1.2.2/aha_pk9_4/1]
 *
 * 	Merge-up from sommerfeld_pk_kdc_1
 * 	[1996/09/16  21:25 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 *
 * 	Placebo for krb5beta6
 * 	[1996/05/24  23:15 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.11.2  1996/10/03  14:56:38  arvind
 * 	Placebo for krb5beta6
 * 	[1996/09/16  21:25 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 * 
 * Revision 1.1.11.1  1996/06/04  22:00:11  arvind
 * 	Merge KRB5_KDB_REQUIRES_USER_TO_USER flag addition from mb_u2u
 * 	[1996/05/06  19:51 UTC  burati  /main/DCE_1.2/2]
 * 
 * 	merge  u2u work
 * 	[1996/04/29  19:37 UTC  burati  /main/DCE_1.2/mb_u2u/1]
 * 
 * 	u2u work off Mothra, before 1.2 branch is available
 * 	[1996/04/29  19:32 UTC  burati  /main/HPDCE02/mb_mothra8/1]
 * 
 * 	[1994/12/14  20:20 UTC  mullan_s  /main/HPDCE02/1]
 * 
 * 	Add attribute flag for timestamps preauth.
 * 	[1994/12/14  20:12 UTC  mullan_s  /main/mullan_mothra_chfts13565/1]
 * 
 * Revision 1.1.9.3  1996/02/18  23:02:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:04  marty]
 * 
 * Revision 1.1.9.2  1995/12/08  17:42:36  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/03  19:10 UTC  mdf
 * 	Resolve CHFts14998
 * 	[1995/05/03  17:41 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_4/1]
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/14  20:20 UTC  mullan_s
 * 
 * 	HP revision /main/mullan_mothra_chfts13565/1  1994/12/14  20:12 UTC  mullan_s
 * 	Add attribute flag for timestamps preauth.
 * 	[1995/12/08  16:57:07  root]
 * 
 * Revision 1.1.4.3  1994/08/24  20:12:27  mullan_s
 * 	[OT11556] Update DCE preauth types to match Kerberos spec.
 * 	[1994/08/24  20:11:39  mullan_s]
 * 
 * Revision 1.1.4.2  1994/07/15  15:00:26  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:17:40  mdf]
 * 
 * Revision 1.1.4.1  1994/05/11  19:16:45  ahop
 * 	hp_sec_to_osf_2 drop
 * 	Start using beta3 krb5_db_entry structure.
 * 	add PADATA_ENC_THIRD_PARTY flag
 * 
 * Revision 1.1.2.2  1992/12/29  13:59:40  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:52:44  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:49:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
**
** Copyright (c) Hewlett-Packard Company 1991, 1994
** Unpublished work. All Rights Reserved.
**
*/
/*
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
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
 * KDC Database interface definitions.
 */


#ifndef KRB5_KDB5__
#define KRB5_KDB5__

/* For Kerberos V5 Beta 1 compatibility */
#ifndef KRB5_BETA1
#define KRB5_BETA1
#endif

/* this is the same structure as krb5_keyblock, but with a different name to
   enable compile-time catching of programmer confusion between encrypted &
   decrypted keys in the database */

typedef struct _krb5_encrypted_keyblock {
    krb5_keytype keytype;
    int length;
    krb5_octet *contents;
} krb5_encrypted_keyblock;

#ifdef OSF_DCE_ARCHAISM
typedef krb5_encrypted_keyblock krb5_key_data;
#endif

#ifdef KRB5_BETA1
typedef struct _krb5_db_policy {
    krb5_timestamp max_life;
    krb5_timestamp max_renewable_life;
    krb5_timestamp min_life;
    krb5_timestamp default_life;
} krb5_db_policy;
#endif /* KRB5_BETA1 */

typedef struct _krb5_db_entry {
    krb5_principal principal;
    krb5_encrypted_keyblock key;
    krb5_kvno kvno;
    krb5_deltat max_life;
    krb5_deltat max_renewable_life;
    krb5_kvno mkvno;                    /* master encryption key vno */

    krb5_timestamp expiration;          /* This is when the client expires */
    krb5_timestamp pw_expiration;       /* This is when its password does */
    krb5_timestamp last_pwd_change;     /* Last time of password change  */
    krb5_timestamp last_success;        /* Last successful password */

    krb5_timestamp last_failed;         /* Last failed password attempt */
    krb5_kvno fail_auth_count;          /* # of failed password attempts */

#ifdef OSF_DCE
    krb5_timestamp good_since_date;
    krb5_timestamp disable;             /* Disable time if max exceeded */
    krb5_int32 max_attempts;            /* Max number of attempts */
#endif

    krb5_principal mod_name;
    krb5_timestamp mod_date;
    krb5_flags attributes;
    krb5_int32 salt_type:8,
               salt_length:24;
    krb5_octet *salt;
    krb5_encrypted_keyblock alt_key;
    krb5_int32 alt_salt_type:8,
               alt_salt_length:24;
    krb5_octet *alt_salt;

    krb5_int32 expansion[8];
} krb5_db_entry;

#ifdef SANDIA   /* SANDIA Enhancement (Pre-Auth/Blacklist) */
#define KRB5_MAX_FAIL_COUNT             5
#endif

#define KRB5_KDB_SALTTYPE_NORMAL	0
#define KRB5_KDB_SALTTYPE_V4		1
#define KRB5_KDB_SALTTYPE_NOREALM	2
#define KRB5_KDB_SALTTYPE_ONLYREALM	3
#define KRB5_KDB_SALTTYPE_SPECIAL	4

#define	KRB5_KDB_DISALLOW_POSTDATED	0x00000001
#define	KRB5_KDB_DISALLOW_FORWARDABLE	0x00000002
#define	KRB5_KDB_DISALLOW_TGT_BASED	0x00000004
#define	KRB5_KDB_DISALLOW_RENEWABLE	0x00000008
#define	KRB5_KDB_DISALLOW_PROXIABLE	0x00000010
#define	KRB5_KDB_DISALLOW_DUP_SKEY	0x00000020
#ifdef KRB5_BETA1
#define	KRB5_KDB_DISALLOW_CLIENT_TIX	0x00000040
#define	KRB5_KDB_DISALLOW_SERVER_TIX	0x00000080
#endif /* KRB5_BETA1 */
#define KRB5_KDB_REQUIRES_HW_AUTH       0x00000100
#define KRB5_KDB_REQUIRES_PWCHANGE      0x00000200
/*
 * comment by mullan_s. The next two values conflict
 * with the values of KRB5_KDB_DISALLOW_CLIENT_TIX
 * & KRB5_KDB_DISALLOW_SERVER_TIX. So I gave them
 * new values until we remove the old flags.
 */
#define KRB5_KDB_DISALLOW_ALL_TIX       0x00000400
#define KRB5_KDB_REQUIRES_PRE_AUTH      0x00000800
#define KRB5_KDB_DISALLOW_SVR           0x00001000
#define KRB5_KDB_PWCHANGE_SERVICE       0x00002000

/*
 * this indicates that at least third party pre-auth is required
 * by the ERA --it is set by build_princ_entry()
 */
#define KRB5_KDB_PADATA_OSF_DCE         0x00004000
/* 
 * this indicates that at least timestamps pre-auth is
 * required by the ERA.
 */
#define KRB5_KDB_PADATA_UNIX_TIME	0x00008000

/* 
 * Used to indicate that the accout is not valid
 */
#define KRB5_KDB_DCE_ACCT_NOT_VALID	0x00010000

/*
 * Used by DCE to indicate that the server must use user-to-user
 */
#define KRB5_KDB_REQUIRES_USER_TO_USER  0x00020000


/* 
 * this indicates that public key pre-auth is
 * required by the ERA.
 */
#define KRB5_KDB_PADATA_ROOT_CERT	0x00040000

/* XXX depends on knowledge of krb5_parse_name() formats */
#define KRB5_KDB_M_NAME		"K/M"	/* Kerberos/Master */

#define KDB_CONVERT_KEY_TO_DB(in,out) krb5_kdb_encrypt_key(&master_encblock, in, out)
#define KDB_CONVERT_KEY_OUTOF_DB(in, out) krb5_kdb_decrypt_key(&master_encblock, in, out)

/* prompts used by default when reading the KDC password from the keyboard. */
#define KRB5_KDC_MKEY_1	"Enter KDC database master key:"
#define KRB5_KDC_MKEY_2	"Re-enter KDC database master key to verify:"

extern char *krb5_mkey_pwd_prompt1;
extern char *krb5_mkey_pwd_prompt2;


/* libkdb.spec */
krb5_error_code krb5_db_set_name
	PROTOTYPE((char * ));
krb5_error_code krb5_db_init
	PROTOTYPE((void ));
krb5_error_code krb5_db_fini
	PROTOTYPE((void ));
krb5_error_code krb5_db_get_age
	PROTOTYPE((char *,
		   time_t * ));
krb5_error_code krb5_db_create
	PROTOTYPE((char * ));
krb5_error_code krb5_db_rename
	PROTOTYPE((char *,
		   char * ));
void krb5_db_free_principal
	PROTOTYPE((krb5_db_entry *,
		   int  ));
krb5_error_code krb5_db_put_principal
	PROTOTYPE((krb5_db_entry *,
		   int * ));
krb5_error_code krb5_db_delete_principal
	PROTOTYPE((krb5_principal,
		   int * ));
krb5_error_code krb5_db_iterate
	PROTOTYPE((krb5_error_code (* ) PROTOTYPE((krb5_pointer,
						   krb5_db_entry *)),
		   krb5_pointer ));
krb5_error_code krb5_db_verify_master_key
	PROTOTYPE((krb5_principal, krb5_keyblock *, krb5_encrypt_block *));
krb5_error_code krb5_db_store_mkey PROTOTYPE((char *,
					      krb5_principal,
					      krb5_keyblock *));
krb5_error_code krb5_kdb_encrypt_key
	PROTOTYPE((krb5_encrypt_block *,
		   const krb5_keyblock *,
		   krb5_encrypted_keyblock *));
krb5_error_code krb5_kdb_decrypt_key
	PROTOTYPE((krb5_encrypt_block *,
		   const krb5_encrypted_keyblock *,
		   krb5_keyblock *));
krb5_error_code krb5_db_setup_mkey_name
	PROTOTYPE((const char *, const char *, char **, krb5_principal *));
krb5_error_code krb5_db_lock
	PROTOTYPE((int ));
krb5_error_code krb5_db_unlock
	PROTOTYPE ((void ));
#ifdef KRB5_BETA1
krb5_error_code krb5_db_get_policy
	PROTOTYPE((krb5_db_policy *));
#endif /* KRB5_BETA1 */

/* need to play games here, since we take a pointer and the real thing,
   and it might be narrow. */
#ifdef NARROW_PROTOTYPES
#ifdef KRB5_BETA1
krb5_error_code krb5_db_get_principal
	PROTOTYPE((krb5_principal,
		   krb5_keytype,
		   krb5_kvno,
		   krb5_db_entry *,
		   int *,
		   krb5_boolean * ));
#else
krb5_error_code krb5_db_get_principal
        PROTOTYPE((krb5_principal ,
                   krb5_db_entry *,
                   int *,
                   krb5_boolean * ));
#endif /* KRB5_BETA1 */
krb5_error_code krb5_db_set_nonblocking
	PROTOTYPE((krb5_boolean,
		   krb5_boolean * ));
krb5_boolean krb5_db_set_lockmode
	PROTOTYPE((krb5_boolean ));
#else
#ifdef KRB5_BETA1
krb5_error_code krb5_db_get_principal
	PROTOTYPE((krb5_principal,
		   int,  /* krb5_keytype */
		   int,  /* krb5_kvno */
		   krb5_db_entry *,
		   int *,
		   krb5_boolean * ));
#endif /* KRB5_BETA1 */

krb5_error_code krb5_db_set_nonblocking
	PROTOTYPE((int, /* krb5_boolean */
		   krb5_boolean * ));
krb5_boolean krb5_db_set_lockmode
	PROTOTYPE((int /* krb5_boolean */ ));
#endif /* NARROW_PROTOTYPES */
#include <krb5/widen.h>

/* Only put things which don't have pointers to the narrow types in this
   section */

#ifdef KRB5_BETA1
krb5_error_code	krb5_db_fetch_mkey
	PROTOTYPE((krb5_principal, krb5_encrypt_block *, krb5_boolean,
		   krb5_boolean, krb5_keyblock * ));
#else
krb5_error_code	krb5_db_fetch_mkey
	PROTOTYPE((krb5_principal, krb5_encrypt_block *, krb5_boolean,
		   krb5_boolean, krb5_data *, krb5_keyblock * ));
#endif /* KRB5_BETA1 */
#include <krb5/narrow.h>


#ifdef KRB5_BETA1
#define KRB5_KDB_DEF_FLAGS	(KRB5_KDB_DISALLOW_DUP_SKEY)
#else
#define KRB5_KDB_DEF_FLAGS      0
#endif /* KRB5_BETA1 */

/* XXX THIS REALLY BELONGS ELSEWHERE */
#define	TGTNAME	"krbtgt"

#endif /* KRB5_KDB5__ */
