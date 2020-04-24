/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: verify_mky.c,v $
 * Revision 1.1.4.2  1996/02/18  00:12:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:02:32  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:54:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:12  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:50:40  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:10:18  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  verify_mky.c V=4 10/24/91 //littl/prgy/krb5/lib/kdb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/kdb/verify_mky.c,v $
 * $Author: marty $
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
 * krb5_db_verify_master_key();
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_verify_mky_c[] =
"$Id: verify_mky.c,v 1.1.4.2 1996/02/18 00:12:31 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/kdb.h>
#include <krb5/ext-proto.h>

/*
 * Verify that the master key in *mkey matches the database entry
 * for mprinc.
 *
 * eblock points to an encrypt_block used for the realm in question.
 */

krb5_error_code
krb5_db_verify_master_key(mprinc, mkey, eblock)
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
    if (retval = krb5_db_get_principal(mprinc, KEYTYPE_DES, 0, 
				       &master_entry, &nprinc, &more))
	return(retval);
	
    if (nprinc != 1) {
	if (nprinc)
	    krb5_db_free_principal(&master_entry, nprinc);
	return(KRB5_KDB_NOMASTERKEY);
    } else if (more) {
	krb5_db_free_principal(&master_entry, nprinc);
	return(KRB5KDC_ERR_PRINCIPAL_NOT_UNIQUE);
    }	

    /* do any necessary key pre-processing */
    if (retval = krb5_process_key(eblock, mkey)) {
	krb5_db_free_principal(&master_entry, nprinc);
	return(retval);
    }
    if (retval = krb5_kdb_decrypt_key(eblock, &master_entry.key, &tempkey)) {
	(void) krb5_finish_key(eblock);
	krb5_db_free_principal(&master_entry, nprinc);
	return retval;
    }
    if (memcmp((char *)mkey->contents, (char *)tempkey.contents,
	       mkey->length)) {
	retval = KRB5_KDB_BADMASTERKEY;
	(void) krb5_finish_key(eblock);
    } else
	retval = krb5_finish_key(eblock);

    memset((char *)tempkey.contents, 0, tempkey.length);
    xfree(tempkey.contents);
    krb5_db_free_principal(&master_entry, nprinc);
    
    return retval;
}
