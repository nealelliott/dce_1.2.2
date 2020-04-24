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
 * $Log: sn2princ.c,v $
 * Revision 1.1.2.1  1996/06/05  21:19:59  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:10:12  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/os/sn2princ.c
 *
 * Copyright 1991 by the Massachusetts Institute of Technology.
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
 * Convert a hostname and service name to a principal in the "standard"
 * form.
 */

#define NEED_SOCKETS
#include "k5-int.h"
#include <ctype.h>
#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

krb5_error_code INTERFACE
krb5_sname_to_principal(context, hostname, sname, type, ret_princ)
    krb5_context context;
    const char * hostname;
    const char * sname;
    krb5_int32 type;
    krb5_principal * ret_princ;
{
    struct hostent *hp;
    char **hrealms, *realm, *remote_host;
    krb5_error_code retval;
    register char *cp;
    char localname[MAXHOSTNAMELEN];
    int  use_local_realm = 0;

    if ((type == KRB5_NT_UNKNOWN) ||
	(type == KRB5_NT_SRV_HST)) {

	if (!hostname) {
	    use_local_realm = 1;
	    /* convenience hack:  if hostname is NULL, use gethostname() */
	    if (gethostname(localname, MAXHOSTNAMELEN))
		return errno;
	    hostname = localname;
	}

	/* if sname is NULL, use "host" */

	if (! sname) {
	    sname = "host";
	}

	/* copy the hostname into non-volatile storage */

	if (type == KRB5_NT_SRV_HST) {
	    char *addr;
	    
	    if (!(hp = gethostbyname(hostname)))
		return KRB5_ERR_BAD_HOSTNAME;
	    remote_host = strdup(hp->h_name);
	    if (!remote_host)
		return ENOMEM;
	    /*
	     * Do a reverse resolution to get the full name, just in
	     * case there's some funny business going on.  If there
	     * isn't an in-addr record, give up.
	     */
	    addr = malloc(hp->h_length);
	    if (!addr)
		return ENOMEM;
	    memcpy(addr, hp->h_addr, hp->h_length);
	    hp = gethostbyaddr(addr, hp->h_length, hp->h_addrtype);
	    free(addr);
	    if (hp) {
		free(remote_host);
		remote_host = strdup(hp->h_name);
		if (!remote_host)
		    return ENOMEM;
	    }
	} else /* type == KRB5_NT_UNKNOWN */ {
	    remote_host = strdup(hostname);
	}
	if (!remote_host)
	    return ENOMEM;

	if (type == KRB5_NT_SRV_HST)
	    for (cp = remote_host; *cp; cp++)
		if (isupper(*cp))
		    *cp = tolower(*cp);

	if (use_local_realm) {
	    if (retval = krb5_get_default_realm(context, &realm)) {
		free(remote_host);
		return retval;
	    }
	} else {
	    if (retval = krb5_get_host_realm(context, remote_host, &hrealms)) {
		free(remote_host);
		return retval;
	    }
	    if (!hrealms[0]) {
		free(remote_host);
		krb5_xfree(hrealms);
		return KRB5_ERR_HOST_REALM_UNKNOWN;
	    }
	    realm = hrealms[0];
	}

	retval = krb5_build_principal(context, ret_princ, strlen(realm),
				      realm, sname, remote_host,
				      (char *)0);

	krb5_princ_type(context, *ret_princ) = type;

	free(remote_host);
	if (!use_local_realm)
	    krb5_free_host_realm(context, hrealms);
	return retval;
    } else {
	return KRB5_SNAME_UNSUPP_NAMETYPE;
    }
}

