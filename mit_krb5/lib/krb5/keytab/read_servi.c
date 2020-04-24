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
 * $Log: read_servi.c,v $
 * Revision 1.1.2.1  1996/06/05  20:55:55  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:54:09  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/keytab/read_servi.c
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
 * This routine is designed to be passed to krb5_rd_req.  
 * It is a convenience function that reads a key out of a keytab.
 * It handles all of the opening and closing of the keytab 
 * internally. 
 */

#include "k5-int.h"

#define KSUCCESS 0

/*
 * effects: If keyprocarg is not NULL, it is taken to be the name of a 
 *	keytab.  Otherwise, the default keytab will be used.  This 
 *	routine opens the keytab and finds the principal associated with
 *	principal, vno, and keytype and returns the resulting key in *key 
 *	or returning an error code if it is not	found. 
 * returns: Either KSUCCESS or error code.
 * errors: error code if not found or keyprocarg is invalid.
 */
krb5_error_code
krb5_kt_read_service_key(context, keyprocarg, principal, vno, keytype, key)
    krb5_context context;
    krb5_pointer keyprocarg;
    krb5_principal principal;
    krb5_kvno vno;
    krb5_keytype keytype;
    krb5_keyblock ** key;
{
    krb5_error_code kerror = KSUCCESS;
    char keytabname[MAX_KEYTAB_NAME_LEN + 1]; /* + 1 for NULL termination */
    krb5_keytab id;
    krb5_keytab_entry entry;
        
    /*
     * Get the name of the file that we should use. 
     */
    if (!keyprocarg) {
	if ((kerror = krb5_kt_default_name(context, (char *)keytabname, 
					   sizeof(keytabname) - 1))!= KSUCCESS)
	    return (kerror);
    } else {
	memset(keytabname, 0, sizeof(keytabname));
	(void) strncpy(keytabname, (char *)keyprocarg, 
		       sizeof(keytabname) - 1);
    }

    if (kerror = krb5_kt_resolve(context, (char *)keytabname, &id))
	return (kerror);

    kerror = krb5_kt_get_entry(context, id, principal, vno, keytype, &entry);
    krb5_kt_close(context, id);

    if (kerror)
	return(kerror);

    krb5_copy_keyblock(context, &entry.key, key);

    krb5_kt_free_entry(context, &entry);

    return (KSUCCESS);
}
