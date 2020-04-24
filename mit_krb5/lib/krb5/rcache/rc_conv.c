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
 * $Log: rc_conv.c,v $
 * Revision 1.1.2.1  1996/06/05  21:21:16  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:11:39  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/rcache/rc_conv.c
 *
 * This file of the Kerberos V5 software is derived from public-domain code
 * contributed by Daniel J. Bernstein, <brnstnd@acf10.nyu.edu>.
 *
 */


/*
 * An implementation for the default replay cache type.
 */

#define FREE(x) ((void) free((char *) (x)))

#include "rc_base.h"

/*
Local stuff:
 krb5_auth_to_replay(context, krb5_tkt_authent *auth,krb5_donot_replay *rep)
  given auth, take important information and make rep; return -1 if failed
*/

krb5_error_code
krb5_auth_to_rep(context, auth, rep)
    krb5_context context;
    krb5_tkt_authent *auth;
    krb5_donot_replay *rep;
{
 krb5_error_code retval;
 rep->cusec = auth->authenticator->cusec;
 rep->ctime = auth->authenticator->ctime;
 if (retval = krb5_unparse_name(context, auth->ticket->server,&rep->server))
   return retval; /* shouldn't happen */
 if (retval = krb5_unparse_name(context, auth->authenticator->client,&rep->client)) {
     FREE(rep->server);
     return retval; /* shouldn't happen. */
 }
 return 0;
}
