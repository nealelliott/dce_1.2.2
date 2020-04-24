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
 * $Log: recvauth.c,v $
 * Revision 1.1.2.1  1996/06/05  21:06:47  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:03:39  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/krb/recvauth.c
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
 * convenience sendauth/recvauth functions
 */

#define NEED_SOCKETS
#include "k5-int.h"
#include "auth_con.h"
#include "com_err.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

extern krb5_flags	krb5_kdc_default_options;

static char *sendauth_version = "KRB5_SENDAUTH_V1.0";

krb5_error_code
krb5_recvauth(context, auth_context,
	      /* IN */
	      fd, appl_version, server, flags, keytab,
	      /* OUT */
	      ticket)
    krb5_context 	  context;
    krb5_auth_context  ** auth_context;
    krb5_pointer	  fd;
    char		* appl_version;
    krb5_principal	  server;
    krb5_int32		  flags;
    krb5_keytab		  keytab;
    krb5_ticket	       ** ticket;
{
    krb5_auth_context	* new_auth_context;
    krb5_flags		  ap_option;
    krb5_error_code	  retval, problem;
    krb5_data		  inbuf;
    krb5_data		  outbuf;
    krb5_rcache 	  rcache;
    krb5_octet		  response;
    krb5_data		  null_server;
    extern krb5_deltat 	  krb5_clockskew;
	
	/*
	 * Zero out problem variable.  If problem is set at the end of
	 * the intial version negotiation section, it means that we
	 * need to send an error code back to the client application
	 * and exit.
	 */
	problem = 0;

	if (!(flags & KRB5_RECVAUTH_SKIP_VERSION)) {
	    /*
	     * First read the sendauth version string and check it.
	     */
	    if (retval = krb5_read_message(context, fd, &inbuf))
		return(retval);
	    if (strcmp(inbuf.data, sendauth_version)) {
		krb5_xfree(inbuf.data);
		problem = KRB5_SENDAUTH_BADAUTHVERS;
	    }
	    krb5_xfree(inbuf.data);
	}
	if (flags & KRB5_RECVAUTH_BADAUTHVERS)
	    problem = KRB5_SENDAUTH_BADAUTHVERS;
	
	/*
	 * Do the same thing for the application version string.
	 */
	if (retval = krb5_read_message(context, fd, &inbuf))
		return(retval);
	if (strcmp(inbuf.data, appl_version)) {
		krb5_xfree(inbuf.data);
		if (!problem)
			problem = KRB5_SENDAUTH_BADAPPLVERS;
	}
	krb5_xfree(inbuf.data);
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
	if ((krb5_net_write(context, *((int *)fd), (char *)&response, 1)) < 0) {
		return(problem); /* We'll return the top-level problem */
	}
	if (problem)
		return(problem);

/* Were clear here */
    /*
     * Setup the replay cache.
     */
    if (server) {
        problem = krb5_get_server_rcache(context, 
			krb5_princ_component(context, server, 0), &rcache);
    } else {
    	null_server.length = 7;
    	null_server.data = "default";
    	problem = krb5_get_server_rcache(context, &null_server, &rcache);
    }

    if (!problem) {
	if (krb5_rc_recover(context, rcache)) {
	    /*
	     * If the rc_recover() didn't work, then try
	     * initializing the replay cache.
	     */
	    if (problem = krb5_rc_initialize(context, rcache, krb5_clockskew)) {
	        krb5_rc_close(context, rcache);
		rcache = NULL;
	    }
	}
    }

    /*
     * Now, let's read the AP_REQ message and decode it
     */
    if (retval = krb5_read_message(context, fd, &inbuf)) {
	if (problem) /* Return top level problem */
	    retval = problem; 
	goto cleanup;
    }

    if (!problem) {
    	if (*auth_context == NULL) {
	    problem = krb5_auth_con_init(context, &new_auth_context);
	    *auth_context = new_auth_context;
	}
    }
    if (!problem) {
	problem = krb5_auth_con_setrcache(context, *auth_context, rcache);
    }
    if (!problem) {
	problem = krb5_rd_req(context, auth_context, &inbuf, server,
				      keytab, &ap_option, ticket);
	krb5_xfree(inbuf.data);
    }
	
    /*
     * If there was a problem, send back a krb5_error message,
     * preceeded by the length of the krb5_error message.  If
     * everything's ok, send back 0 for the length.
     */
    if (problem) {
	krb5_error	error;
	const	char *message;

	memset((char *)&error, 0, sizeof(error));
	krb5_us_timeofday(context, &error.stime, &error.susec);
	error.server = server;
	error.error = problem - ERROR_TABLE_BASE_krb5;
	if (error.error > 127)
		error.error = KRB_ERR_GENERIC;
	message = error_message(problem);
	error.text.length  = strlen(message) + 1;
	if (!(error.text.data = malloc(error.text.length))) {
	    retval = ENOMEM;
	    goto cleanup;
	}
	strcpy(error.text.data, message);
	if (retval = krb5_mk_error(context, &error, &outbuf)) {
	    free(error.text.data);
	    goto cleanup;
	}
	free(error.text.data);
    } else {
	outbuf.length = 0;
	outbuf.data = 0;
    }

    if (!problem) {
    	retval = krb5_write_message(context, fd, &outbuf);
    	if (outbuf.data)
	    krb5_xfree(outbuf.data);
    	if (retval)
	    goto cleanup;
    } else {
    	/* We sent back an error, we need cleanup then return */
    	retval = problem;
    	goto cleanup;
    }

    /* Here lies the mutual authentication stuff... */
    if ((ap_option & AP_OPTS_MUTUAL_REQUIRED)) {
	if (retval = krb5_mk_rep(context, *auth_context, &outbuf)) {
	    return(retval);
	}
	retval = krb5_write_message(context, fd, &outbuf);
	krb5_xfree(outbuf.data);
    }

cleanup:;
    if (retval) {
	if (rcache)
	    krb5_rc_close(context, rcache);
    }
    return retval;
}
