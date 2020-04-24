/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rc_conv.c,v $
 * Revision 1.1.4.2  1996/02/18  00:16:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:13  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:44:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:48  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:31:56  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:19:43  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:52:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rc_conv.c V=1 06/07/91 //littl/prgy/krb5/lib/rcache
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/rcache/rc_conv.c,v $
 * $Author: marty $
 *
 * This file of the Kerberos V5 software is derived from public-domain code
 * contributed by Daniel J. Bernstein, <brnstnd@acf10.nyu.edu>.
 *
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_rc_conv_c[] =
"$Id: rc_conv.c,v 1.1.4.2 1996/02/18 00:16:00 marty Exp $";
#endif	/* !lint & !SABER */

/*
 * An implementation for the default replay cache type.
 */

#define FREE(x) ((void) free((char *) (x)))

#include "rc_base.h"

/*
Local stuff:
 krb5_auth_to_replay(krb5_tkt_authent *auth,krb5_donot_replay *rep)
  given auth, take important information and make rep; return -1 if failed
*/

krb5_error_code
krb5_auth_to_rep(auth, rep)
krb5_tkt_authent *auth;
krb5_donot_replay *rep;
{
 krb5_error_code retval;
 rep->cusec = auth->authenticator->cusec;
 rep->ctime = auth->authenticator->ctime;
 if (retval = krb5_unparse_name(auth->ticket->server,&rep->server))
   return retval; /* shouldn't happen */
 if (retval = krb5_unparse_name(auth->authenticator->client,&rep->client)) {
     FREE(rep->server);
     return retval; /* shouldn't happen. */
 }
 return 0;
}
