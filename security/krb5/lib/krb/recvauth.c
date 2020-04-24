/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: recvauth.c,v $
 * Revision 1.1.6.2  1996/02/18  00:14:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:11  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:01:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:12  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:58:45  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:15:48  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:52:20  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  recvauth.c V=3 12/19/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/recvauth.c,v $
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
 * convenience sendauth/recvauth functions
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_recvauth_c [] =
"$Id: recvauth.c,v 1.1.6.2 1996/02/18 00:14:44 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/osconf.h>
#include <krb5/kdb.h>
#include <krb5/kdb_dbm.h>
#include <krb5/ext-proto.h>
#include <krb5/los-proto.h>
#include <com_err.h>
#include <errno.h>

#include <stdio.h>
#include <string.h>
#include <netinet/in.h>

#define WORKING_RCACHE

extern krb5_flags	krb5_kdc_default_options;

static char *sendauth_version = "KRB5_SENDAUTH_V1.0";

krb5_error_code
krb5_recvauth(/* IN */
	      fd, appl_version, server, sender_addr, fetch_from,
	      keyproc, keyprocarg, rc_type, 
	      /* OUT */
	      seq_number, client, ticket, authent)
	krb5_pointer	fd;
	char	*appl_version;
	krb5_principal	server;
	krb5_address	*sender_addr;
	krb5_pointer	fetch_from;
	krb5_int32	*seq_number;
	char		*rc_type;
	krb5_rdreq_key_proc keyproc;
	krb5_pointer keyprocarg;
	krb5_principal	*client;
	krb5_ticket	**ticket;
	krb5_authenticator	**authent;
{
	krb5_error_code	retval, problem;
	krb5_data	inbuf;
	krb5_tkt_authent	*authdat;
	krb5_data		outbuf;
	krb5_rcache rcache;
	krb5_octet		response;
	char *cachename;
	extern krb5_deltat krb5_clockskew;
	static char		*rc_base = "rc_";
	
	/*
	 * Zero out problem variable.  If problem is set at the end of
	 * the intial version negotiation section, it means that we
	 * need to send an error code back to the client application
	 * and exit.
	 */
	problem = 0;
	/*
	 * First read the sendauth version string and check it.
	 */
	if (retval = krb5_read_message(fd, &inbuf))
		return(retval);
	if (strcmp(inbuf.data, sendauth_version)) {
		xfree(inbuf.data);
		problem = KRB5_SENDAUTH_BADAUTHVERS;
	}
	xfree(inbuf.data);
	/*
	 * Do the same thing for the application version string.
	 */
	if (retval = krb5_read_message(fd, &inbuf))
		return(retval);
	if (strcmp(inbuf.data, appl_version)) {
		xfree(inbuf.data);
		if (!problem)
			problem = KRB5_SENDAUTH_BADAPPLVERS;
	}
	xfree(inbuf.data);
	/*
	 * OK, now check the problem variable.  If it's zero, we're
	 * fine and we can continue.  Otherwise, we have to signal an
	 * error to the client side and bail out.
	 */
	switch (problem) {
	case 0:
		response = 0;
		break;
	case KRB5_SENDAUTH_BADAUTHVERS:
		response = 1;
		break;
	case KRB5_SENDAUTH_BADAPPLVERS:
		response = 2;
		break;
	default:
		/*
		 * Should never happen!
		 */
		response = 255;
#ifdef SENDAUTH_DEBUG
		fprintf(stderr, "Programming botch in recvauth!  problem = %d",
			problem);
		abort();
#endif
		break;
	}
	/*
	 * Now we actually write the response.  If the response is non-zero,
	 * exit with a return value of problem
	 */
	if ((krb5_net_write(*((int *) fd), (char *)&response, 1)) < 0) {
		return(problem); /* We'll return the top-level problem */
	}
	if (problem)
		return(problem);
	rcache = NULL;
#ifdef WORKING_RCACHE
	/*
	 * Setup the replay cache.
	 */
	if (!(rcache = (krb5_rcache) malloc(sizeof(*rcache)))) 
		problem = ENOMEM;
	if (!problem) 
		problem = krb5_rc_resolve_type(&rcache,
					       rc_type ? rc_type : "dfl");
	cachename = NULL;
	if (!problem && !(cachename = malloc(server[1]->length+1+
					     strlen(rc_base))))
		problem = ENOMEM;
	if (!problem) {
		strcpy(cachename, rc_base ? rc_base : "rc_");
		strncat(cachename, server[1]->data, server[1]->length);
		cachename[server[1]->length+strlen(rc_base)] = '\0';
		problem = krb5_rc_resolve(rcache, cachename);
	}
	if (!problem) {
		if (krb5_rc_recover(rcache))
			/*
			 * If the rc_recover didn't work, then try
			 * initializing the replay cache.
			 */
			problem = krb5_rc_initialize(rcache, krb5_clockskew);
		if (problem) {
			krb5_rc_close(rcache);
			rcache = NULL;
		}
	}
#endif
	/*
	 * Now, let's read the AP_REQ message and decode it
	 */
	if (retval = krb5_read_message(fd, &inbuf)) {
#ifdef WORKING_RCACHE		
		(void) krb5_rc_close(rcache);
		if (cachename)
			free(cachename);
#endif
		return(retval);
	}
	authdat = 0;			/* so we can tell if we need to
					   free it later... */
	if (!problem)
		problem = krb5_rd_req(&inbuf, server, sender_addr, fetch_from,
				      keyproc, keyprocarg, rcache, &authdat);
	xfree(inbuf.data);
#ifdef WORKING_RCACHE
	if (rcache)
	    retval = krb5_rc_close(rcache);
#endif
	if (!problem && retval)
		problem = retval;
#ifdef WORKING_RCACHE
	if (cachename)
		free(cachename);
#endif
	
	/*
	 * If there was a problem, send back a krb5_error message,
	 * preceeded by the length of the krb5_error message.  If
	 * everything's ok, send back 0 for the length.
	 */
	if (problem) {
		krb5_error	error;
		const	char *message;

		memset((char *)&error, 0, sizeof(error));
		krb5_us_timeofday(&error.stime, &error.susec);
		error.server = server;
		error.error = problem - ERROR_TABLE_BASE_krb5;
		if (error.error > 127)
			error.error = KRB_ERR_GENERIC;
		message = error_message(problem);
		error.text.length  = strlen(message) + 1;
		if (!(error.text.data = malloc(error.text.length)))
			return(ENOMEM);
		strcpy(error.text.data, message);
		if (retval = krb5_mk_error(&error, &outbuf)) {
			free(error.text.data);
			return(retval);
		}
		free(error.text.data);
	} else {
		outbuf.length = 0;
		outbuf.data = 0;
	}
	if (retval = krb5_write_message(fd, &outbuf)) {
		if (outbuf.data)
			xfree(outbuf.data);
		if (!problem)
			krb5_free_tkt_authent(authdat);
		return(retval);
	}
	if (problem) {
		/*
		 * We sent back an error, we need to return
		 */
		if (authdat) krb5_free_tkt_authent(authdat);
		return(problem);
	}
	/*
	 * Here lies the mutual authentication stuff...
	 *
	 * We're going to compose and send a AP_REP message.
	 */
	if ((authdat->ap_options & AP_OPTS_MUTUAL_REQUIRED)) {
		krb5_ap_rep_enc_part	repl;

		/*
		 * Generate a random sequence number
		 */
		if (seq_number &&
		    (retval = krb5_generate_seq_number(authdat->ticket->enc_part2->session,
						      seq_number))) {
		    krb5_free_tkt_authent(authdat);
		    return(retval);
		}

		repl.ctime = authdat->authenticator->ctime;
		repl.cusec = authdat->authenticator->cusec;
		repl.subkey = authdat->authenticator->subkey;
		if (seq_number)
		    repl.seq_number = *seq_number;
		else
		    repl.seq_number = 0;

		if (retval = krb5_mk_rep(&repl,
					 authdat->ticket->enc_part2->session,
					 &outbuf)) {
			krb5_free_tkt_authent(authdat);
			return(retval);
		}
		if (retval = krb5_write_message(fd, &outbuf)) {
			xfree(outbuf.data);
			krb5_free_tkt_authent(authdat);
			return(retval);
		}
		xfree(outbuf.data);
	}
	/*
	 * At this point, we've won.  We just need to copy whatever
	 * parts of the authdat structure which the user wants, clean
	 * up, and exit.
	 */
	if (client)
		if (retval =
		    krb5_copy_principal(authdat->ticket->enc_part2->client,
					client))
			return(retval);
	/*
	 * The following efficiency hack assumes knowledge about the
	 * structure of krb5_tkt_authent.  If we later add additional
	 * allocated substructures to krb5_tkt_authent, they will have
	 * to be reflected here; otherwise, we will probably have a
	 * memory leak.
	 *
	 * If the user wants that part of the authdat structure,
	 * return it; otherwise free it.
	 */
	if (ticket)
		*ticket = authdat->ticket;
	else
		krb5_free_ticket(authdat->ticket);
	if (authent)
		*authent = authdat->authenticator;
	else
		krb5_free_authenticator(authdat->authenticator);
	xfree(authdat);
	return 0;
}


