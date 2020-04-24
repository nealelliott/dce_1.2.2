/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: timop_svc_refmon.c,v $
 * Revision 1.1.4.2  1996/02/18  18:26:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:48:31  marty]
 *
 * Revision 1.1.4.1  1995/12/08  20:55:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:50:15  root]
 * 
 * Revision 1.1.2.1  1994/06/14  19:57:42  weir
 * 	DCE 1.1 Beta check-in
 * 	[1994/06/14  16:15:18  weir]
 * 
 * $EndLog$
 */
/*
 * The Example code is provided to you as "Pass-through Code".
 * Pass-through Code and its related Documentation are not warrented,
 * supported, or indemnified by OSF, and are provided "AS IS".
 * You can use, sublicense, or distribute the same at your own risk.
 */
/*
 * Copyright (c) 1990, 1991  
 * Open Software Foundation, Inc. 
 *  
 * Permission is hereby granted to use, copy, modify and freely distribute 
 * the software in this file and its documentation for any purpose without 
 * fee, provided that the above copyright notice appears in all copies and 
 * that both the copyright notice and this permission notice appear in 
 * supporting documentation.  Further, provided that the name of Open 
 * Software Foundation, Inc. ("OSF") not be used in advertising or 
 * publicity pertaining to distribution of the software without prior 
 * written permission from OSF.  OSF makes no representations about the 
 * suitability of this software for any purpose.  It is provided "as is" 
 * without express or implied warranty. 
 */
/*
 *  OSF DCE Version 1.0 
 */

/*
**	timop_svc_refmon.c
**
**	Reference monitor for timop_svc example.
*/

#define DO_SVC

#include <dce/dce.h>
#include <stdio.h>
#include <dce/dce_error.h>

#include <dce/dce_cf.h>

#include <stddef.h>

#include <dce/rpc.h>
#include <dce/utctypes.h>
#include <pthread.h>
#include <dce/sec_login.h>
#include <dce/dcesvcmsg.h>
#include <dce/svclog.h>
#include <dce/dce_msg.h>
#include <dce/dce_svc.h>
#include "dcetsvsvc.h"
#include "dcetsvmsg.h"
#include "dcetsvmac.h"

#include "timop_svc_aux.h"
#include "timop_svc.h"
#include "timop_svc_server.h"


/* extern char *malloc(); */
extern char *strcpy();
extern char *strcat();
extern char *strncat();

/*
 *	ref_mon()
 *
 *	Reference monitor for timop_svc.
 *	It checks generalities, then calls is_authorized() to check specifics.
 */

int
ref_mon(
	rpc_binding_handle_t		bind_handle,
	int				requested_op)
{
	char				*new_string;
	char				*cell_string;
	int				ret;
	rpc_authz_handle_t		privs;
	unsigned_char_t			*client_princ_name, *server_princ_name;
	unsigned32			protect_level, authn_svc, authz_svc,
	                                    status;

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_refmon, svc_c_debug5, "Entering refmon()"));

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_refmon, svc_c_debug5, "Calling rpc_binding_inq_auth_client()"));
	/* Get client auth info. */
	rpc_binding_inq_auth_client(bind_handle, &privs, &server_princ_name,
	    &protect_level, &authn_svc, &authz_svc, &status);
	if (status != rpc_s_ok)
	{
		print_refmon_error("rpc_binding_inq_auth_client()", status);
		exit(1);
	}

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_refmon, svc_c_debug5, "Checking authentication service"));
	/* Check if selected authn service is acceptable to us. */
	if (authn_svc != rpc_c_authn_dce_secret) 
	{
#ifdef DO_SVC
		dce_svc_printf(BAD_AUTHENTICATION_MSG);
#else
		fprintf(stderr, " Server: ref_mon(): bad authentication service\n");
#endif
		return(DENY_ACCESS);
	}

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_refmon, svc_c_debug5, "Checking protection level"));
	/* Check if selected protection level is acceptable to us. */
	if (protect_level != rpc_c_protect_level_pkt_integ
	&&  protect_level != rpc_c_protect_level_pkt_privacy)
	{
#ifdef DO_SVC
		dce_svc_printf(BAD_PROTECTION_MSG);
#else
		fprintf(stderr, " Server: ref_mon(): bad protection level\n");
#endif
		return(DENY_ACCESS);
	}

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_refmon, svc_c_debug5, "Checking authorization service"));
	/* Check if selected authz service is acceptable to us. */
	if (authz_svc != rpc_c_authz_name)
	{
#ifdef DO_SVC
		dce_svc_printf(BAD_AUTHORIZATION_MSG);
#else
		fprintf(stderr, " Server: ref_mon(): bad authorization service\n");
#endif
		return(DENY_ACCESS);
	}

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_refmon, svc_c_debug5, "Constructing full principal name"));
	/* If rpc_c_authz_dce were being used instead of rpc_c_authz_name,
	   privs would be a PAC (sec_id_pac_t *), not a name as it is here. */
	client_princ_name = (unsigned_char_t *)privs;

	/* Check if selected server principal name is supported. */

	/* Since we're getting a full name, we have to build a full name from  */
	/*  our SERVER_PRINC_NAME constant to compare with it.                 */
	/* First, we get our cell name...                                      */
	new_string = malloc(MAX_NAME);
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_refmon, svc_c_debug5, "Calling dce_cf_get_cell_name()"));
	dce_cf_get_cell_name(&cell_string, &status);
	if (status != dce_cf_st_ok)
	{
		print_refmon_error("dce_cf_get_cell_name()", status);
		exit(1);
	}

	strcpy(new_string, cell_string);

	/* Next, we add a slash to the end of it...                            */
	strcat(new_string, (unsigned_char_t *)"/");

	/* Finally, we add the simple server principal name that we're         */
	/*  expecting to see...                                                */
	strncat(new_string, SERVER_PRINC_NAME, strlen(SERVER_PRINC_NAME));

	/* And now we can do the actual comparison...                          */
	if (strcmp(server_princ_name, new_string) != 0)
	{
#ifdef DO_SVC
		dce_svc_printf(BAD_PRINCIPAL_MSG);
#else
		fprintf(stderr, " Server: ref_mon(): bad server principal name\n");
#endif
		free(new_string);
		free(cell_string);
		DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_refmon, svc_c_debug5, "Returning DENY_ACCESS"));
		return(DENY_ACCESS);

	}

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_refmon, svc_c_debug5, "Calling is_authorized()"));
	/* Now that things seem generally OK, check the specifics. */
	ret = is_authorized(client_princ_name, requested_op);
	if (ret == NOT_AUTHORIZED)
	{
#ifdef DO_SVC
		dce_svc_printf(NO_AUTHORIZATION_MSG);
#else
		fprintf(stderr, " Server: ref_mon(): client not authorized\n");
#endif
		free(new_string);
		free(cell_string);
		DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_refmon, svc_c_debug5, "Returning DENY_ACCESS"));
		return(DENY_ACCESS);
	}

 	free(new_string);
	free(cell_string);

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_refmon, svc_c_debug5, "Returning GRANT_ACCESS"));
	/* Cleared all the authorization hurdles -- grant access. */
	return(GRANT_ACCESS);
}


/*
 *	is_authorized()
 *
 *	Check authorization of client to the requested service.
 *	This could be arbitrarily application-specific, but we keep it simple.
 *	A normal application (i.e., one using PACs & ACLs) would be using
 *	sec_acl_mgr_is_authorized() instead of this function.
 */

int
is_authorized(
        unsigned_char_t			*client_princ_name,
	int				requested_op)
{
	char				*new_string;
	char				*cell_string;
	int				ret;
	unsigned32			status;

	/* Check if we want to let this client do this operation. */

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_refmon, svc_c_debug5, "Entering is_authorized()"));

	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_refmon, svc_c_debug5, "Constructing full client principal name"));
	/* Since we're getting a full name, we have to build a full name from  */
	/*  our CLIENT_PRINC_NAME constant to compare with it.                 */
	/* First, we get our cell name...                                      */
	new_string = malloc(MAX_NAME);
	DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_refmon, svc_c_debug5, "Calling dce_cf_get_cell_name()"));
	dce_cf_get_cell_name(&cell_string, &status);
	if (status != dce_cf_st_ok)
	{
		print_refmon_error("dce_cf_get_cell_name()", status);
		exit(1);
	}

	strcpy(new_string, cell_string);

	/* Next, we add a slash to the end of it...                            */
	strcat(new_string, (unsigned_char_t *)"/");

	/* Finally, we add the simple client principal name that we're         */
	/*  expecting to see...                                                */
	strncat(new_string, CLIENT_PRINC_NAME, strlen(CLIENT_PRINC_NAME));

	/* And now we can do the actual comparison...                          */
	if (strcmp(client_princ_name, new_string) == 0)
	{
		switch (requested_op)
		{
			case GETSPAN_OP:
			case SVC_REMOTE:
				ret = IS_AUTHORIZED;
				DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_refmon, svc_c_debug5, "Returning IS_AUTHORIZED"));
				break;

			default:
				ret = NOT_AUTHORIZED;
				DCE_SVC_DEBUG((tsv_svc_handle, tsv_s_refmon, svc_c_debug5, "Returning NOT_AUTHORIZED"));
				break;
		}
	}

	free(new_string);
	free(cell_string);

	return(ret);
}


/*****
*
* print_refmon_error-- Refmon version. Prints text associated with bad status code.
*
*****/
void
print_refmon_error(caller, status)
char *caller;		/* a string identifying the routine that received the error  */
error_status_t status;	/* the status we want to print the message for               */
{
	dce_error_string_t error_string;
	int print_status;

	dce_error_inq_text(status, error_string, &print_status);

#ifdef DO_SVC
	dce_svc_printf(REFMON_ERROR_MSG, caller, error_string);
#else
	fprintf(stderr," Refmon: %s: %s\n", caller, error_string);
#endif

/* The following shows how you can call dce_printf() to print out a message. This    */
/*  call will print out the simple error string, but not the "(dce / <comp>)" part.  */
/*  That's because those higher bits in which the latter info is contained are not   */
/*  part of the message id, I think.                                                 */
/* Thus, where dce_error_ing_text will give you back a string with something like:   */
/*  "Bad routing specification (dce / svc)", dce_printf will print only the simple   */
/*  "Bad routing specification" part...                                              */
/*                                                                                   */
/*
dce_printf(status);
*/

}
