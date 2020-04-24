/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: copy_creds.c,v $
 * Revision 1.1.8.2  1996/02/18  00:13:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:15  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:57:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:37  root]
 * 
 * Revision 1.1.4.1  1994/09/06  19:26:34  greg
 * 	Copy authdata with fresh allocation instead
 * 	of aliasing.
 * 	[1994/09/06  17:56:42  greg]
 * 
 * Revision 1.1.2.3  1993/02/16  22:35:48  sommerfeld
 * 	Add krb5_move_cred_contents for use in ccache code to avoid memory leaks.
 * 	[1993/02/12  17:58:07  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/12/29  15:54:18  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:13:23  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:52:17  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  copy_creds.c V=3 12/16/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/copy_creds.c,v $
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
 * krb5_copy_cred()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_copy_creds_c [] =
"$Id: copy_creds.c,v 1.1.8.2 1996/02/18 00:13:32 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>


/*
 * Copy credentials, allocating fresh storage where needed.
 */

krb5_error_code
krb5_copy_creds(incred, outcred)
const krb5_creds *incred;
krb5_creds **outcred;
{
    krb5_creds *tempcred;
    krb5_error_code retval;
    krb5_data *scratch;

    if (!(tempcred = (krb5_creds *)malloc(sizeof(*tempcred))))
	return ENOMEM;

    *tempcred = *incred;		/* copy everything quickly */
    if (retval = krb5_copy_principal(incred->client, &tempcred->client))
	goto cleanlast;
    if (retval = krb5_copy_principal(incred->server, &tempcred->server))
	goto cleanclient;
    if (retval = krb5_copy_keyblock_contents(&incred->keyblock,
					     &tempcred->keyblock))
	goto cleanserver;
    if (incred->addresses) 
        if (retval = krb5_copy_addresses(incred->addresses, &tempcred->addresses))
	    goto cleanblock;
    if (!(incred->ticket.length == 0 || incred->ticket.data == NULL)) {
        if (retval = krb5_copy_data(&incred->ticket, &scratch))
	    goto cleanaddrs;
	tempcred->ticket = *scratch;
	xfree(scratch);
    }
    if (!(incred->second_ticket.length == 0 
          || incred->second_ticket.data == NULL)) {
	if (retval = krb5_copy_data(&incred->second_ticket,
				&scratch))
  	    goto cleanticket;

	tempcred->second_ticket = *scratch;
	xfree(scratch);
    }

#ifdef OSF_DCE
    /* BUGFIX applied from the krb5 beta 4 sources. */

    if (incred->authdata) {
	retval = krb5_copy_authdata(incred->authdata,&tempcred->authdata);
	if (retval)
	    goto cleanticket;
    }
#endif

    *outcred = tempcred;
    return 0;

 cleanticket:
    free(tempcred->ticket.data);
 cleanaddrs:
    krb5_free_addresses(tempcred->addresses);
 cleanblock:
    xfree(tempcred->keyblock.contents);
 cleanserver:
    krb5_free_principal(tempcred->server);
 cleanclient:
    krb5_free_principal(tempcred->client);
 cleanlast:
    xfree(tempcred);
    return retval;
}

krb5_error_code krb5_move_cred_contents (tocred, fromcred)
    krb5_creds *tocred;
    krb5_creds *fromcred;
{
#define CONDMOVE(field) \
    if (tocred->field == NULL) { \
	tocred->field = fromcred->field; fromcred->field = NULL; }

#define CONDMOVEDATA(field) \
    if (tocred->field.data == NULL) { \
	tocred->field.length = fromcred->field.length; \
	tocred->field.data = fromcred->field.data; fromcred->field.data = NULL; }
    
#define CONDMOVEKEY(field) \
    if (tocred->field.contents == NULL) { \
	tocred->field.length = fromcred->field.length; \
	tocred->field.keytype = fromcred->field.keytype; \
	tocred->field.contents = fromcred->field.contents; fromcred->field.contents = NULL; }

    CONDMOVE(client);
    CONDMOVE(server);
    CONDMOVEKEY(keyblock);
    tocred->times = fromcred->times;
    tocred->is_skey = fromcred->is_skey;
    tocred->ticket_flags = fromcred->ticket_flags;
    CONDMOVE(addresses);
    CONDMOVEDATA(ticket);
    CONDMOVEDATA(second_ticket);
    CONDMOVE(authdata);
}
