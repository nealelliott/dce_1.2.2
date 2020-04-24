/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: timop_refmon.c,v $
 * Revision 1.1.4.2  1996/02/18  18:26:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:48:38  marty]
 *
 * Revision 1.1.4.1  1995/12/08  20:55:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:50:30  root]
 * 
 * Revision 1.1.2.1  1994/06/14  20:00:08  weir
 * 	DCE 1.1 Beta check-in
 * 	[1994/06/14  16:16:51  weir]
 * 
 * $EndLog$
 */
/*
 * OLD HISTORY:
 * 
 * Revision 1.2.3.5  1993/10/28  21:07:29  weir
 * 	String handling fixes for OT 9215
 * 	[1993/10/28  21:06:48  weir]
 *
 * Revision 1.2.3.4  1993/10/28  20:37:31  weir
 * 	String handling fixes for OT 9215
 * 	[1993/10/28  20:36:36  weir]
 * 
 * Revision 1.2.3.3  1993/10/21  19:44:48  weir
 * 	timop for DCE 1.0.3, with following fixes:
 * 	CR 4804: use DCE status codes
 * 	CR 4805: use relative principal names
 * 	CR 7460: initialize remote status parameter in server manager
 * 	CR 9215: port to HP platform.
 * 	[1993/10/21  19:38:28  weir]
 * 
 * Revision 1.2.2.3  1993/02/04  20:32:34  cjd
 * 	Embedded copyright notice
 * 	[1993/02/04  20:29:58  cjd]
 * 
 * Revision 1.2.2.2  1992/12/18  18:18:42  marcyw
 * 	Defect 4998: adding disclaimer comment
 * 	[1992/12/18  18:12:28  marcyw]
 * 
 * Revision 1.2  1992/01/19  22:14:31  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * 
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
**	timop_refmon.c
**
**	Reference monitor for timop example.
*/

#include <stdio.h>
#include <dce/dce_cf.h>
#include "timop_aux.h"
#include "timop.h"
#include "timop_server.h"

extern char *malloc();
extern char *strcpy();
extern char *strcat();
extern char *strncat();

/*
 *	ref_mon()
 *
 *	Reference monitor for timop.
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


	/* Get client auth info. */
	rpc_binding_inq_auth_client(bind_handle, &privs, &server_princ_name,
	    &protect_level, &authn_svc, &authz_svc, &status);
	if (status != rpc_s_ok)
	{
		print_error("rpc_binding_inq_auth_client()", status);
		exit(1);
	}

	/* Check if selected authn service is acceptable to us. */
	if (authn_svc != rpc_c_authn_dce_secret) 
	{
		fprintf(stderr, " Server: ref_mon(): bad authentication service\n");
		return(DENY_ACCESS);
	}

	/* Check if selected protection level is acceptable to us. */
	if (protect_level != rpc_c_protect_level_pkt_integ
	&&  protect_level != rpc_c_protect_level_pkt_privacy)
	{
		fprintf(stderr, " Server: ref_mon(): bad protection level\n");
		return(DENY_ACCESS);
	}

	/* Check if selected authz service is acceptable to us. */
	if (authz_svc != rpc_c_authz_name)
	{
		fprintf(stderr, " Server: ref_mon(): bad authorization service\n");
		return(DENY_ACCESS);
	}

	/* If rpc_c_authz_dce were being used instead of rpc_c_authz_name,
	   privs would be a PAC (sec_id_pac_t *), not a name as it is here. */
	client_princ_name = (unsigned_char_t *)privs;

	/* Check if selected server principal name is supported. */

	/* Since we're getting a full name, we have to build a full name from  */
	/*  our SERVER_PRINC_NAME constant to compare with it.                 */
	/* First, we get our cell name...                                      */
	new_string = malloc(MAX_NAME);
	dce_cf_get_cell_name(&cell_string, &status);
	if (status != dce_cf_st_ok)
	{
		print_error("dce_cf_get_cell_name()", status);
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
		fprintf(stderr, " Server: ref_mon(): bad server principal name\n");
		free(new_string);
		free(cell_string);
		return(DENY_ACCESS);

	}

	/* Now that things seem generally OK, check the specifics. */
	ret = is_authorized(client_princ_name, requested_op);
	if (ret == NOT_AUTHORIZED)
	{
		fprintf(stderr, " Server: ref_mon(): not authorized\n");
		free(new_string);
		free(cell_string);
		return(DENY_ACCESS);
	}

 	free(new_string);
	free(cell_string);

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
	unsigned32			status;

	/* Check if we want to let this client do this operation. */

	/* Since we're getting a full name, we have to build a full name from  */
	/*  our CLIENT_PRINC_NAME constant to compare with it.                 */
	/* First, we get our cell name...                                      */
	new_string = malloc(MAX_NAME);
	dce_cf_get_cell_name(&cell_string, &status);
	if (status != dce_cf_st_ok)
	{
		print_error("dce_cf_get_cell_name()", status);
		exit(1);
	}

	strcpy(new_string, cell_string);

	/* Next, we add a slash to the end of it...                            */
	strcat(new_string, (unsigned_char_t *)"/");

	/* Finally, we add the simple client principal name that we're         */
	/*  expecting to see...                                                */
	strncat(new_string, CLIENT_PRINC_NAME, strlen(CLIENT_PRINC_NAME));

	/* And now we can do the actual comparison...                          */
	if (strcmp(client_princ_name, new_string) == 0 &&  requested_op == GETSPAN_OP)
	{
		/* OK, we'll let this access happen. */
		free(new_string);
		free(cell_string);
		return(IS_AUTHORIZED);
	}

	free(new_string);
	free(cell_string);

	/* Sorry, Charlie. */
	return(NOT_AUTHORIZED);
}
