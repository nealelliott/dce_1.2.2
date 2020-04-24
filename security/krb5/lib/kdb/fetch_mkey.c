/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: fetch_mkey.c,v $
 * Revision 1.1.4.2  1996/02/18  00:12:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:02:29  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:53:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:09  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:50:14  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:09:59  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:49  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  fetch_mkey.c V=4 10/24/91 //littl/prgy/krb5/lib/kdb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/kdb/fetch_mkey.c,v $
 * $Author: marty $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
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
 * krb5_db_fetch_mkey():
 * Fetch a database master key from somewhere.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_fetch_mkey_c[] =
"$Id: fetch_mkey.c,v 1.1.4.2 1996/02/18 00:12:23 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/sysincl.h>		/* for MAXPATHLEN */
#include <krb5/krb5.h>
#include <krb5/kdb.h>
#include <krb5/los-proto.h>
#include <krb5/ext-proto.h>
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
krb5_db_fetch_mkey(DECLARG(krb5_principal, mname),
		   DECLARG(krb5_encrypt_block *, eblock),
		   DECLARG(krb5_boolean, fromkeyboard),
		   DECLARG(krb5_boolean, twice),
		   DECLARG(krb5_keyblock *,key))
OLDDECLARG(krb5_principal, mname)
OLDDECLARG(krb5_encrypt_block *, eblock)
OLDDECLARG(krb5_boolean, fromkeyboard)
OLDDECLARG(krb5_boolean, twice)
OLDDECLARG(krb5_keyblock *,key)
{
    krb5_error_code retval;
    char password[BUFSIZ];
    krb5_data pwd;
    int size = sizeof(password);


    if (fromkeyboard) {
	krb5_data scratch;

	if (retval = krb5_read_password(krb5_mkey_pwd_prompt1,
					twice ? krb5_mkey_pwd_prompt2 : 0,
					password,
					&size))
	    return(retval);

	pwd.data = password;
	pwd.length = size;
	if (retval = krb5_principal2salt(mname, &scratch))
	    return retval;
	retval = krb5_string_to_key(eblock, key->keytype, key, &pwd, &scratch);
	xfree(scratch.data);
	memset(password, 0, sizeof(password)); /* erase it */
	return retval;

    } else {
	/* from somewhere else */
	krb5_keytype keytype;
	char defkeyfile[MAXPATHLEN+1];
	krb5_data *realm = krb5_princ_realm(mname);
	FILE *kf;

	retval = 0;
	(void) strcpy(defkeyfile, DEFAULT_KEYFILE_STUB);
	(void) strncat(defkeyfile, realm->data,
		       min(sizeof(defkeyfile)-sizeof(DEFAULT_KEYFILE_STUB)-1,
			   realm->length));
	(void) strcat(defkeyfile, "");
	
#if defined(__STDC__)
	if (!(kf = fopen(defkeyfile, "rb")))
#else
	if (!(kf = fopen(defkeyfile, "r")))
#endif
	    return KRB5_KDB_CANTREAD_STORED;
	if (fread((krb5_pointer) &keytype, sizeof(keytype), 1, kf) != 1) {
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
		  sizeof(key->contents[0]), key->length, kf) != key->length)
	    retval = KRB5_KDB_CANTREAD_STORED;
	else
	    retval = 0;
    errout:
	(void) fclose(kf);
	return retval;
    }
}
