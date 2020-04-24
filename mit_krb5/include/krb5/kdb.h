/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: kdb.h,v $
 * Revision 1.1.2.1  1996/06/05  20:27:31  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:27:17  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * include/krb5/kdb.h
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
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

/* this is the same structure as krb5_keyblock, but with a different name to
   enable compile-time catching of programmer confusion between encrypted &
   decrypted keys in the database */

typedef struct _krb5_encrypted_keyblock {
    krb5_magic magic;
    short keytype;				/* XXX this is SO ugly --- proven */
    int length;
    krb5_octet *contents;
} krb5_encrypted_keyblock;

/*
 * Note --- this structure cannot be modified without changing the
 * database version number in libkdb.a
 */
typedef struct _krb5_db_entry {
    krb5_magic magic;
    krb5_principal principal;
    krb5_encrypted_keyblock key;
    krb5_kvno kvno;
    krb5_deltat	max_life;
    krb5_deltat	max_renewable_life;
    krb5_kvno mkvno;			/* master encryption key vno */
    
    krb5_timestamp expiration;		/* This is when the client expires */
    krb5_timestamp pw_expiration; 	/* This is when its password does */
    krb5_timestamp last_pwd_change; 	/* Last time of password change  */
    krb5_timestamp last_success;	/* Last successful password */
    
    krb5_timestamp last_failed;		/* Last failed password attempt */
    krb5_kvno fail_auth_count; 		/* # of failed password attempts */
    
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
  
#ifdef SANDIA	/* SANDIA Enhancement (Pre-Auth/Blacklist) */
#define KRB5_MAX_FAIL_COUNT		5
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
#define	KRB5_KDB_DISALLOW_ALL_TIX	0x00000040
#define	KRB5_KDB_REQUIRES_PRE_AUTH	0x00000080
#define KRB5_KDB_REQUIRES_HW_AUTH	0x00000100
#define	KRB5_KDB_REQUIRES_PWCHANGE	0x00000200
#define KRB5_KDB_DISALLOW_SVR		0x00001000
#define KRB5_KDB_PWCHANGE_SERVICE	0x00002000
#define KRB5_KDB_SUPPORT_DESMD5         0x00004000

/* XXX depends on knowledge of krb5_parse_name() formats */
#define KRB5_KDB_M_NAME		"K/M"	/* Kerberos/Master */

#define KDB_CONVERT_KEY_TO_DB(context,in,out) krb5_kdb_encrypt_key(context,&master_encblock, in, out)
#define KDB_CONVERT_KEY_OUTOF_DB(context,in, out) krb5_kdb_decrypt_key(context,&master_encblock, in, out)

/* prompts used by default when reading the KDC password from the keyboard. */
#define KRB5_KDC_MKEY_1	"Enter KDC database master key:"
#define KRB5_KDC_MKEY_2	"Re-enter KDC database master key to verify:"

extern char *krb5_mkey_pwd_prompt1;
extern char *krb5_mkey_pwd_prompt2;


/* libkdb.spec */
krb5_error_code krb5_db_set_name
	PROTOTYPE((krb5_context,
		   char * ));
krb5_error_code krb5_db_init
	PROTOTYPE((krb5_context));
krb5_error_code krb5_db_fini
	PROTOTYPE((krb5_context));
krb5_error_code krb5_db_get_age
	PROTOTYPE((krb5_context,
		   char *,
		   time_t * ));
krb5_error_code krb5_db_create
	PROTOTYPE((krb5_context,
		   char * ));
krb5_error_code krb5_db_rename
	PROTOTYPE((krb5_context,
		   char *,
		   char * ));
krb5_error_code krb5_db_get_principal
	PROTOTYPE((krb5_context,
		   krb5_principal ,
		   krb5_db_entry *,
		   int *,
		   krb5_boolean * ));
void krb5_db_free_principal
	PROTOTYPE((krb5_context,
		   krb5_db_entry *,
		   int  ));
krb5_error_code krb5_db_put_principal
	PROTOTYPE((krb5_context,
		   krb5_db_entry *,
		   int * ));
krb5_error_code krb5_db_delete_principal
	PROTOTYPE((krb5_context,
		   krb5_principal,
		   int * ));
krb5_error_code krb5_db_iterate
	PROTOTYPE((krb5_context,
		   krb5_error_code (* ) PROTOTYPE((krb5_pointer,
						   krb5_db_entry *)),
		   krb5_pointer ));
krb5_error_code krb5_db_verify_master_key
	PROTOTYPE((krb5_context,
		   krb5_principal, 
		   krb5_keyblock *, 
		   krb5_encrypt_block *));
krb5_error_code krb5_db_store_mkey 
	PROTOTYPE((krb5_context,
		   char *,
		   krb5_principal,
		   krb5_keyblock *));
krb5_error_code krb5_kdb_encrypt_key
	PROTOTYPE((krb5_context,
		   krb5_encrypt_block *,
		   const krb5_keyblock *,
		   krb5_encrypted_keyblock *));
krb5_error_code krb5_kdb_decrypt_key
	PROTOTYPE((krb5_context,
		   krb5_encrypt_block *,
		   const krb5_encrypted_keyblock *,
		   krb5_keyblock *));
krb5_error_code krb5_db_setup_mkey_name
	PROTOTYPE((krb5_context,
		   const char *, 
		   const char *, 
		   char **, 
		   krb5_principal *));
krb5_error_code krb5_db_lock
	PROTOTYPE((krb5_context,
		   int ));
krb5_error_code krb5_db_unlock
	PROTOTYPE((krb5_context));
krb5_error_code krb5_db_set_nonblocking
	PROTOTYPE((krb5_context,
		   krb5_boolean,
		   krb5_boolean * ));
krb5_boolean krb5_db_set_lockmode
	PROTOTYPE((krb5_context,
		   krb5_boolean));
krb5_error_code	krb5_db_fetch_mkey
	PROTOTYPE((krb5_context,
		   krb5_principal, 
		   krb5_encrypt_block *, 
		   krb5_boolean,
		   krb5_boolean, 
		   krb5_data *, 
		   krb5_keyblock * ));

#define KRB5_KDB_DEF_FLAGS	0

#endif /* KRB5_KDB5__ */

