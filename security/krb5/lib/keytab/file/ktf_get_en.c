/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ktf_get_en.c,v $
 * Revision 1.1.8.3  1996/02/18  00:12:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:02:37  marty]
 *
 * Revision 1.1.8.2  1995/12/08  17:54:58  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/15  00:02 UTC  rps
 * 	copyright
 * 	[1995/04/05  17:19 UTC  rps  /main/MOTHRA_panic/2]
 * 
 * 	Fuzzy realm compare
 * 	[1995/02/11  00:07 UTC  sommerfeld  /main/sommerfeld_krb5_alias_1/1]
 * 
 * Revision 1.1.4.1  1994/08/26  16:02:05  sommerfeld
 * 	Krb5beta4 merge.
 * 	Add ODE header & copyright
 * 	[1994/08/26  15:56:22  sommerfeld]
 * 
 * $EndLog$
 */
/*
**
** Copyright (c) Hewlett-Packard Company 1991, 1992, 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/keytab/file/ktf_get_en.c,v $
 * $Author: marty $
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
 * This is the get_entry routine for the file based keytab implementation.
 * It opens the keytab file, and either retrieves the entry or returns
 * an error.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_krb5_ktfile_get_entry_c[] =
"$Id: ktf_get_en.c,v 1.1.8.3 1996/02/18 00:12:40 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

#include "ktfile.h"

krb5_error_code
krb5_ktfile_get_entry(DECLARG(krb5_keytab, id),
		      DECLARG(krb5_principal, principal),
		      DECLARG(krb5_kvno, kvno),
		      DECLARG(krb5_keytab_entry *, entry))
OLDDECLARG(krb5_keytab, id)
OLDDECLARG(krb5_principal, principal)
OLDDECLARG(krb5_kvno, kvno)
OLDDECLARG(krb5_keytab_entry *, entry)
{
    krb5_keytab_entry *cur_entry;
    krb5_error_code kerror = 0;

    /* Open the keyfile for reading */
    if (kerror = krb5_ktfileint_openr(id))
	return(kerror);
    
    /* 
     * For efficiency and simplicity, we'll use a while true that 
     * is exited with a break statement.
     */
    while (TRUE) {
	cur_entry = 0;
	if (kerror = krb5_ktfileint_read_entry(id, &cur_entry))
	    break;

	if (((kvno == IGNORE_VNO) || (kvno == cur_entry->vno)) &&
	    krb5_principal_equiv(principal, cur_entry->principal)) {
	    /* found a match */
	    break;
	}
	krb5_kt_free_entry(cur_entry);
	krb5_xfree(cur_entry);
    }
    if (kerror && kerror != KRB5_KT_END) {
	(void) krb5_ktfileint_close(id);
	return kerror;
    }
    if ((kerror = krb5_ktfileint_close(id)) != 0) {
	krb5_kt_free_entry(cur_entry);
	krb5_xfree(cur_entry);
	return kerror;
    }
    if (!cur_entry)
	return KRB5_KT_NOTFOUND;
    *entry = *cur_entry;
    krb5_xfree(cur_entry);
    return 0;
}
