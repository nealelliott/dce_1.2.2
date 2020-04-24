/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tgtname.c,v $
 * Revision 1.1.6.2  1996/02/18  00:14:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:17  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:01:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:16  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:59:12  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:16:04  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:52:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  tgtname.c V=3 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/tgtname.c,v $
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
 * krb5_tgtname()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_tgtname_c [] =
"$Id: tgtname.c,v 1.1.6.2 1996/02/18 00:14:51 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <krb5/kdb.h>

static krb5_data tgtdata = {sizeof(TGTNAME)-1, TGTNAME};

/* This is an internal-only function, used by krb5_get_cred_from_kdc() */

krb5_error_code
krb5_tgtname(client, server, tgtprinc)
const krb5_data *client, *server;
krb5_principal *tgtprinc;
{
    krb5_principal retprinc;
    krb5_error_code retval;

    if (!(retprinc = (krb5_data **)calloc(4, sizeof(krb5_data *))))
	return ENOMEM;
    if (retval = krb5_copy_data(server, &retprinc[0])) {
	xfree(retprinc);
	return retval;
    }
    if (retval = krb5_copy_data(&tgtdata, &retprinc[1])) {
	krb5_free_data(retprinc[0]);
	xfree(retprinc);
	return retval;
    }
    if (retval = krb5_copy_data(client, &retprinc[2])) {
	krb5_free_data(retprinc[0]);
	krb5_free_data(retprinc[1]);
	xfree(retprinc);
	return retval;
    }
    *tgtprinc = retprinc;
    return 0;
}
