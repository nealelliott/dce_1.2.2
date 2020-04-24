/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: server.c,v $
 * Revision 1.1.4.2  1996/02/17  23:12:49  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:53  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:58:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:28:48  root]
 * 
 * Revision 1.1.2.1  1994/05/02  22:43:47  tom
 * 	Initial checkin.
 * 	[1994/05/02  21:06:40  tom]
 * 
 * $EndLog$
 */

#include <test_common.h>
#include "bindcall.h"
extern bindcall_v0_0_epv_t bindcall_v0_0_m_epv;

main (argc, argv)
    int argc;
    char **argv;
{
    error_status_t        status;             /* error_status_t type from nbase.h */
    rpc_binding_vector_p_t  binding_vector;   /* Binding vector pointer type from rpc.h */
    char *entry_name = getenv("RPC_DEFAULT_ENTRY");
    int i;
    unsigned_char_t *string_binding;
    char *auth_principal_template = "/.:/%s/self";
    idl_char *hostname;
    idl_char auth_principal[128];
    unsigned32 authn_protocol = rpc_c_authn_dce_secret;

    printf("I'm about to register interface\n");
    rpc_server_register_if (    
        bindcall_v0_0_s_ifspec,
        NULL,
        (rpc_mgr_epv_t)&bindcall_v0_0_m_epv,
        &status                   /* error status */
    );
    if (status != rpc_s_ok) {
        fprintf(stdout, "Can't register interface (%s)\n",error_text(status));
        exit(1);
    }

    if (argc == 2)
	rpc_server_use_protseq((unsigned_char_p_t)argv[1], rpc_c_protseq_max_reqs_default, &status);
    else
	rpc_server_use_all_protseqs(rpc_c_protseq_max_reqs_default, &status);
    if (status != rpc_s_ok) {
        fprintf(stdout, "Can't establish protocol sequences - %lx\n", status);
        exit(1);
    }

    rpc_server_inq_bindings(      /* Obtain a vector of binding handles */
        &binding_vector,
        &status
    );
    if (status != rpc_s_ok) {
        fprintf(stdout, "Can't get binding handles - %lx\n", status);
        exit(1);
    }


    for (i = 0; i < binding_vector->count; i++)
    {
        rpc_binding_to_string_binding(
                                      binding_vector->binding_h[i],
                                      &string_binding,
                                      &status);
        if (status != error_status_ok) 
        {
            printf("Can't get string binding - %s\n", error_text(status));
            exit(1);
        }
        printf("Obtained binding %s\n",string_binding);        
    }

    printf("I'm about to register endpoints\n");
#ifndef PRT
    rpc_ep_register (    
        bindcall_v0_0_s_ifspec,
        binding_vector,
        NULL,
        "Server for bindcall",
        &status                   /* error status */
    );
    if (status != rpc_s_ok) {
        fprintf(stdout, "Can't register interface (%s)\n",error_text(status));
        exit(1);
    }
#endif

    if (entry_name == NULL) entry_name = ".:/bindcall";
    printf("Exporting binding(s) into %s\n",entry_name);
#ifndef PRT
    rpc_ns_binding_export(
            rpc_c_ns_syntax_default,
            entry_name,
            bindcall_v0_0_s_ifspec,
            binding_vector,
            NULL,
            &status
        );
   if (status != rpc_s_ok) {
        fprintf(stdout, "Can't export to name service - %lx (%s)\n", status,
              error_text(status));
        exit(1);
        };
#endif

    printf("Registering auth info\n");
#ifndef PRT
    dce_cf_get_host_name(&hostname, &status);
    if (status != 0/*dce_cf_st_ok*/)
    {
        printf("Error getting host name - %s\n", error_text(status));
        exit(1);
    }
    sprintf(auth_principal, auth_principal_template, hostname);
    rpc_server_register_auth_info(
            auth_principal,
            authn_protocol,
            NULL, NULL, &status);
    if (status != rpc_s_ok)
    {
        printf("Error setting identity \"%s\" for authn protocol %lu - %s\n",
                auth_principal, authn_protocol, error_text(status));
	exit(1);
    }
#endif

    printf("Listening...\n");
    rpc_server_listen(rpc_c_listen_max_calls_default, &status);
    if (status != rpc_s_ok) {
        fprintf(stdout, "Listen failed (%s)\n",error_text(status));
        exit(1);
    }

}
