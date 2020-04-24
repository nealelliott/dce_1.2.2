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
 * $Log: fetch_mkey.c,v $
 * Revision 1.1.2.1  1996/06/05  20:31:40  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:35:43  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/kdb/fetch_mkey.c
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
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
 * krb5_db_fetch_mkey():
 * Fetch a database master key from somewhere.
 */

#include "k5-int.h"
#include "kdbint.h"

/* these are available to other funcs, and the pointers may be reassigned */

char *krb5_mkey_pwd_prompt1 = KRB5_KDC_MKEY_1;
char *krb5_mkey_pwd_prompt2 = KRB5_KDC_MKEY_2;

/*
 * Get the KDC database master key from somewhere, filling it into *key.
 *
 * key->keytype should be set to the desired key type.
 *
 * if fromkeyboard is TRUE, then the master key is read as a password
 * from the user's terminal.  In this case,
 * eblock should point to a block with an appropriate string_to_key function.
 * if twice is TRUE, the password is read twice for verification.
 *
 * mname is the name of the key sought; this can be used by the string_to_key
 * function or by some other method to isolate the desired key.
 *
 */

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

krb5_error_code
krb5_db_fetch_mkey(context, mname, eblock, fromkeyboard, twice, salt, key)
    krb5_context context;
    krb5_principal mname;
    krb5_encrypt_block * eblock;
    krb5_boolean fromkeyboard;
    krb5_boolean twice;
    krb5_data * salt;
    krb5_keyblock * key;
{
    krb5_error_code retval;
    char password[BUFSIZ];
    krb5_data pwd;
    int size = sizeof(password);


    if (fromkeyboard) {
	krb5_data scratch;

	if (retval = krb5_read_password(context, krb5_mkey_pwd_prompt1,
					twice ? krb5_mkey_pwd_prompt2 : 0,
					password,
					&size))
	    return(retval);

	pwd.data = password;
	pwd.length = size;
	if (!salt) {
		retval = krb5_principal2salt(context, mname, &scratch);
		if (retval)
			return retval;
	}
	retval = krb5_string_to_key(context, eblock, key->keytype, key, &pwd,
				    salt ? salt : &scratch);
	if (!salt)
		krb5_xfree(scratch.data);
	memset(password, 0, sizeof(password)); /* erase it */
	return retval;

    } else {
	/* from somewhere else */
        krb5_ui_2 keytype;
	char defkeyfile[MAXPATHLEN+1];
	krb5_data *realm = krb5_princ_realm(context, mname);
	FILE *kf;

	retval = 0;
	key->magic = KV5M_KEYBLOCK;
	key->etype = eblock->crypto_entry->proto_enctype;
	(void) strcpy(defkeyfile, DEFAULT_KEYFILE_STUB);
	(void) strncat(defkeyfile, realm->data,
		       min(sizeof(defkeyfile)-sizeof(DEFAULT_KEYFILE_STUB)-1,
			   realm->length));
	(void) strcat(defkeyfile, "");
	
#ifdef ANSI_STDIO
	if (!(kf = fopen(defkeyfile, "rb")))
#else
	if (!(kf = fopen(defkeyfile, "r")))
#endif
	    return KRB5_KDB_CANTREAD_STORED;
	if (fread((krb5_pointer) &keytype, 2, 1, kf) != 1) {
	    retval = KRB5_KDB_CANTREAD_STORED;
	    goto errout;
	}
	if (keytype != key->keytype) {
	    retval = KRB5_KDB_BADSTORED_MKEY;
	    goto errout;
	}
	if (fread((krb5_pointer) &key->length,
		  sizeof(key->length), 1, kf) != 1) {
	    retval = KRB5_KDB_CANTREAD_STORED;
	    goto errout;
	}
	if (!key->length || key->length < 0) {
	    retval = KRB5_KDB_BADSTORED_MKEY;
	    goto errout;
	}
	if (!(key->contents = (krb5_octet *)malloc(key->length))) {
	    retval = ENOMEM;
	    goto errout;
	}
	if (fread((krb5_pointer) key->contents,
		  sizeof(key->contents[0]), key->length, kf) != key->length) {
	    retval = KRB5_KDB_CANTREAD_STORED;
	    memset(key->contents, 0, key->length);
	    free(key->contents);
	    key->contents = 0;
	} else
	    retval = 0;
    errout:
	(void) fclose(kf);
	return retval;
    }
}
