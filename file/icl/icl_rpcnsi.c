/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: icl_rpcnsi.c,v $
 * Revision 1.1.40.1  1996/10/02  17:52:23  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:41:01  damon]
 *
 * Revision 1.1.35.3  1994/07/13  22:25:43  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  21:07:40  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:59:43  mbs]
 * 
 * Revision 1.1.35.2  1994/06/09  14:11:38  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:25:19  annie]
 * 
 * Revision 1.1.35.1  1994/02/04  20:21:17  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:14:17  devsrc]
 * 
 * Revision 1.1.33.1  1993/12/07  17:27:02  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/06  13:41:16  jaffe]
 * 
 * $EndLog$
 */
/* Copyright (C) 1993 Transarc Corporation - All rights reserved */

#include <stdio.h>
#include <pthread.h>
#include <dce/dce_error.h>
#include <dce/rpc.h>
#include <dce/sec_login.h>
#include <icl_rpc.h>

#define IFSPEC	DFSTRACE_v1_0_s_ifspec
#define ANNOTATION "dfstrace interface"

static unsigned32 ConnectToServer(name, handlep)
  char *name;
  rpc_binding_handle_t *handlep;
{
    rpc_ns_lookup_handle_t lc;
    unsigned32 		st, result;
    rpc_binding_vector_t * bindingVectorp = NULL;

    result = rpc_s_entry_not_found;
    
    rpc_ns_binding_lookup_begin(rpc_c_ns_syntax_dce, (unsigned char *)name, IFSPEC, NULL,
				rpc_c_binding_max_count_default, &lc, &st);

    if (st != rpc_s_ok)
	return st;
    
    while (st == rpc_s_ok) 
    {
	rpc_ns_binding_lookup_next(lc, &bindingVectorp, &st);
	if (st == rpc_s_ok) 
	{
	    rpc_ns_binding_select(bindingVectorp, handlep, &st);
	    result = st;
	    break;
	}
    }

    if (bindingVectorp)
	rpc_binding_vector_free(&bindingVectorp, &st);
	    
    rpc_ns_binding_lookup_done(&lc, &st);
    return result;
}

unsigned32 icl_ExportIF(name)
  char *name;
{
    unsigned32 		st;
    unsigned32		dontcare;
    rpc_binding_vector_t *bindingVectorp;
    uuid_t		objectUuid;
    uuid_vector_t	uuidVector;
    handle_t		h;
    char		cdsentry[1024];
    char 		*p;
    extern DFSTRACE_v1_0_epv_t DFSTRACE_v1_0_mgr_epv;
    extern char		*strrchr();

    rpc_server_register_if(IFSPEC, NULL, (rpc_mgr_epv_t)&DFSTRACE_v1_0_mgr_epv, &st);
    if (st && st != rpc_s_type_already_registered )
	return st;
    
    rpc_server_use_all_protseqs(0, &st);
    
    /* register last component of name with dumping routines */
    if (!(p = strrchr(name, '/'))) 
	p = name;
    RegisterIclDumpFileName(p);

    if (*name != '/')
    {
	char *cellname;
	char *hostname;

	dce_cf_get_cell_name(&cellname, &st);
	if (st)
	    return st;
	dce_cf_get_host_name(&hostname, &st);
	if (st)
	{
	    free(cellname);
	    return st;
	}
	(void) sprintf(cdsentry, "%s/%s/%s", cellname, hostname, name);
	free(cellname);
	free(hostname);

	name = cdsentry;
    }

    /* see if there is already a server running */
    if (ConnectToServer(name, &h) == rpc_s_ok)
    {
	/* there's one but is it really there? */
#if 0
	/* we're getting occasional false positives here.  Let the
	 * caller worry about duplicate exports
	 */
	rpc_mgmt_set_com_timeout(h, rpc_c_binding_min_timeout, &st);
	if (rpc_mgmt_is_server_listening(h, &st))
	    return rpc_s_entry_already_exists;
#endif

	/* reuse object uuid */
	rpc_binding_inq_object(h, &objectUuid, &st);
	if (st) 
	    return st;
    }
    else
    { 
	/* create a new object ID */
	uuid_create(&objectUuid, &st);
	if (st)
	    return st;
    }
    uuidVector.count = 1;
    uuidVector.uuid[0] = &objectUuid;
	
    /* get a binding vector */
    rpc_server_inq_bindings(&bindingVectorp, &st);
    if (st && st != rpc_s_type_already_registered) 
	return st;

    /* register end point */
    rpc_ep_register(IFSPEC, bindingVectorp, &uuidVector, (unsigned char *)ANNOTATION, &st);
    if (st) 
    {
	rpc_binding_vector_free(&bindingVectorp, &st);
	return st;
    }
	
    /* export into name space */
    rpc_ns_binding_export(rpc_c_ns_syntax_dce, (unsigned char *)name, IFSPEC, bindingVectorp,
			      &uuidVector, &st);
    if (st == rpc_s_entry_already_exists)
	st = rpc_s_ok;
    if (st)
    {
	rpc_binding_vector_free(&bindingVectorp, &st);
	return st;
    }
    rpc_binding_vector_free(&bindingVectorp, &dontcare);

    return st;
}

unsigned32 icl_ConnectToServer(name, handlep)
  char *name;
  rpc_binding_handle_t *handlep;
{
    unsigned char *serverPrincipal;
    unsigned32 st;
    unsigned32 dontcare;

    /* get a handle to the server interface */
    st = ConnectToServer(name, handlep);
    if (st)
	return st;

    /* set the auth level on the connection */
    rpc_mgmt_inq_server_princ_name(*handlep, rpc_c_authn_default, 
				   &serverPrincipal, &st);
    if (st)
	return st;

    rpc_binding_set_auth_info(*handlep, serverPrincipal, rpc_c_protect_level_default,
			      rpc_c_authn_default, NULL, rpc_c_authz_dce, &st);
    if (st)
    {
	rpc_string_free(&serverPrincipal, &dontcare);
	return st;
    }

    rpc_string_free(&serverPrincipal, &dontcare);
    return st;
}
