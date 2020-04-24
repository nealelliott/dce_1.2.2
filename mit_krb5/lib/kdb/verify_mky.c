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
 * $Log: verify_mky.c,v $
 * Revision 1.1.2.1  1996/06/05  20:32:49  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:36:43  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/kdb/verify_mky.c
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
 * krb5_db_verify_master_key();
 */

#include "k5-int.h"

/*
 * Verify that the master key in *mkey matches the database entry
 * for mprinc.
 *
 * eblock points to an encrypt_block used for the realm in question.
 */

krb5_error_code
krb5_db_verify_master_key(context, mprinc, mkey, eblock)
    krb5_context context;
    krb5_principal mprinc;
    krb5_keyblock *mkey;
    krb5_encrypt_block *eblock;
{
    krb5_error_code retval;
    krb5_db_entry master_entry;
    int nprinc;
    krb5_boolean more;
    krb5_keyblock tempkey;

    nprinc = 1;
    if (retval = krb5_db_get_principal(context, mprinc, &master_entry, &nprinc, &more))
	return(retval);
	
    if (nprinc != 1) {
	if (nprinc)
	    krb5_db_free_principal(context, &master_entry, nprinc);
	return(KRB5_KDB_NOMASTERKEY);
    } else if (more) {
	krb5_db_free_principal(context, &master_entry, nprinc);
	return(KRB5KDC_ERR_PRINCIPAL_NOT_UNIQUE);
    }	

    /* do any necessary key pre-processing */
    if (retval = krb5_process_key(context, eblock, mkey)) {
	krb5_db_free_principal(context, &master_entry, nprinc);
	return(retval);
    }
    if (retval = krb5_kdb_decrypt_key(context, eblock, &master_entry.key, &tempkey)) {
	(void) krb5_finish_key(context, eblock);
	krb5_db_free_principal(context, &master_entry, nprinc);
	return retval;
    }
    if (mkey->length != tempkey.length ||
	memcmp((char *)mkey->contents, (char *)tempkey.contents,mkey->length)) {
	retval = KRB5_KDB_BADMASTERKEY;
	(void) krb5_finish_key(context, eblock);
    } else
	retval = krb5_finish_key(context, eblock);

    memset((char *)tempkey.contents, 0, tempkey.length);
    krb5_xfree(tempkey.contents);
    krb5_db_free_principal(context, &master_entry, nprinc);
    
    return retval;
}
