/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: read_servi.c,v $
 * Revision 1.1.4.2  1996/02/18  00:13:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:02:59  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:56:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:30  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:53:08  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:12:33  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:53:38  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  read_servi.c V=4 12/19/91 //littl/prgy/krb5/lib/keytab
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/keytab/read_servi.c,v $
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
 * This routine is designed to be passed to krb5_rd_req.  
 * It is a convenience function that reads a key out of a keytab.
 * It handles all of the opening and closing of the keytab 
 * internally. 
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_krb5_kt_read_service_key_c[] =
"$Id: read_servi.c,v 1.1.4.2 1996/02/18 00:13:12 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>

#include <krb5/ext-proto.h>

#define KSUCCESS 0

krb5_error_code 
krb5_kt_read_service_key(DECLARG(krb5_pointer, keyprocarg),
			 DECLARG(krb5_principal, principal),
			 DECLARG(krb5_kvno, vno),
			 DECLARG(krb5_keyblock **, key))
OLDDECLARG(krb5_pointer, keyprocarg)
OLDDECLARG(krb5_principal, principal)
OLDDECLARG(krb5_kvno, vno)
OLDDECLARG(krb5_keyblock **, key)
/*
	effects: If keyprocarg is not NULL, it is taken to be 
		the name of a keytab.  Otherwise, the default
		keytab will be used.  This routine opens the
		keytab and finds the principal associated with
		principal and vno, returning the resulting key
		in *key or returning an error code if it is not
		found. 
	returns: nothing
	errors: error code if not found
*/
{
    krb5_error_code kerror = KSUCCESS;
    char keytabname[MAX_KEYTAB_NAME_LEN + 1]; /* + 1 for NULL termination */
    krb5_keytab id;
    krb5_keytab_entry entry;
        
    /*
     * Get the name of the file that we should use. 
     */
    if (!keyprocarg) {
	if ((kerror = krb5_kt_default_name((char *)keytabname, 
					   sizeof(keytabname) - 1))!= KSUCCESS)
	    return (kerror);
    } else {
	memset(keytabname, 0, sizeof(keytabname));
	(void) strncpy(keytabname, (char *)keyprocarg, 
		       sizeof(keytabname) - 1);
    }

    if (kerror = krb5_kt_resolve((char *)keytabname, &id))
	return (kerror);

    kerror = krb5_kt_get_entry(id, principal, vno, &entry);
    krb5_kt_close(id);

    if (kerror)
	return(kerror);

    krb5_copy_keyblock(&entry.key, key);

    krb5_kt_free_entry(&entry);

    return (KSUCCESS);
}
