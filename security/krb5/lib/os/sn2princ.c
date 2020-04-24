/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sn2princ.c,v $
 * Revision 1.1.4.2  1996/02/18  00:15:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:06  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:44:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:41  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:31:23  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:18:56  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:11  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  sn2princ.c V=2 09/19/91 //littl/prgy/krb5/lib/os
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/sn2princ.c,v $
 * $Author: marty $
 *
 * Copyright 1991 by the Massachusetts Institute of Technology.
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
 * Convert a hostname and service name to a principal in the "standard"
 * form.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_sn2princ_c[] =
"$Id: sn2princ.c,v 1.1.4.2 1996/02/18 00:15:49 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/los-proto.h>
#include <netdb.h>
#include <ctype.h>
#include <krb5/ext-proto.h>

krb5_error_code
krb5_sname_to_principal(DECLARG(const char *,hostname),
			DECLARG(const char *,sname),
			DECLARG(krb5_boolean,canonicalize),
			DECLARG(krb5_principal *,ret_princ))
OLDDECLARG(const char *,hostname)
OLDDECLARG(const char *,sname)
OLDDECLARG(krb5_boolean,canonicalize)
OLDDECLARG(krb5_principal *,ret_princ)
{
    krb5_principal lprinc;
    struct hostent *hp;
    char **hrealms, *remote_host;
    krb5_error_code retval;
    register char **cpp, *cp;

    /* copy the hostname into non-volatile storage */

    if (canonicalize) {
	if (!(hp = gethostbyname(hostname)))
	    return KRB5_ERR_BAD_HOSTNAME;
	remote_host = strdup(hp->h_name);
    } else {
	remote_host = strdup(hostname);
    }
    if (!remote_host)
	return ENOMEM;

    for (cp = remote_host; *cp; cp++)
	if (isupper(*cp))
	    *cp = tolower(*cp);

    if (retval = krb5_get_host_realm(remote_host, &hrealms)) {
	free(remote_host);
	return retval;
    }
    if (!hrealms[0]) {
	free(remote_host);
	xfree(hrealms);
	return KRB5_ERR_HOST_REALM_UNKNOWN;
    }
    if (!(lprinc = (krb5_principal) calloc(4, sizeof(*lprinc)))) {
	free(remote_host);
	krb5_free_host_realm(hrealms);
	return ENOMEM;
    }	
    if (!(lprinc[0] = (krb5_data *)malloc(sizeof(*lprinc[0])))) {
	krb5_free_host_realm(hrealms);
    nomem:
	free(remote_host);
	krb5_free_principal(lprinc);
	return ENOMEM;
    }
    lprinc[0]->data = hrealms[0];
    lprinc[0]->length = strlen(hrealms[0]);

    /* they're allocated; leave the first one alone, however */
    for (cpp = &hrealms[1]; *cpp; cpp++)
	xfree(*cpp);
    xfree(hrealms);

    if (!(lprinc[1] = (krb5_data *)malloc(sizeof(*lprinc[1])))) {
	goto nomem;
    }
    lprinc[1]->length = strlen(sname);
    lprinc[1]->data = strdup(sname);
    if (!(lprinc[2] = (krb5_data *)malloc(sizeof(*lprinc[2])))) {
	goto nomem;
    }
    lprinc[2]->length = strlen(remote_host);
    lprinc[2]->data = remote_host;

    *ret_princ = lprinc;
    return 0;
}

