/*
 * (C) COPYRIGHT Transarc Corp. 1992
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */

#include <dcedfs/stds.h>
#include <stdio.h>
#include <string.h>
#include <dce/sec_login.h>
#include <dce/misc.h>
#include <dce/binding.h>
#include <dce/uuid.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include "delegate.h"
#include "check_status.h"

sec_login_handle_t login_context;
unsigned_char_t *princ_name;

#define NS_ANNOT "Delegation Test Server Interface"

/* Dummy auth function to allow the server to be shut down */

boolean32 auth_fn(rpc_binding_handle_t binding, unsigned32 op,
	 unsigned32 *st)
{ 
   	*st = rpc_s_ok;
	return 1;
}

int main(int ac, char **av)
{
	unsigned32 status;
	rpc_binding_vector_t *binding_vector;
	rpc_ns_handle_t context;
	unsigned_char_t *princ_name, *ns_entry, *string_uuid;
	uuid_vector_t uuid_list;
	uuid_t resource_id;

	printf("Delegation server starting\n");
	fflush(stdout);
	system("date");

	princ_name = (unsigned_char_t *)av[1];

	ns_entry = (unsigned_char_t *)av[2];

	/* access keytab */

	rpc_server_register_auth_info(princ_name,
		rpc_c_authn_dce_secret, NULL,
		"/krb5/delegation", &status);
	if(status)
	{
		printf("rpc_server_register_auth_info:status=%d\n",
			status);
		fflush(stdout);
		goto bailout;
	}

        /* register interface with RPC runtime */

	rpc_server_register_if(dce_delegate_v1_2_s_ifspec, NULL, NULL,
		 &status);
	if(status)
	{
		printf("rpc_server_register_if:status=%d\n",status);
		fflush(stdout);
		goto bailout;
	}

        /* create binding info */

	rpc_server_use_all_protseqs(rpc_c_protseq_max_reqs_default,
		 &status);
	if(status)
	{
		printf("rpc_server_use_all_protseqs:status=%d\n",
			status);
		fflush(stdout);
		goto bailout;
	}

        /* obtain this server's binding info */

	rpc_server_inq_bindings(&binding_vector, &status);
        if(status)
        {
                printf("rpc_server_inq_bindings:status=%d\n",
                        status);
                fflush(stdout);
                goto bailout;
        }

	uuid_create_nil(&resource_id, &status);
        if(status)
        {
                printf("uuid_create_nil:status=%d\n",
                        status);
                fflush(stdout);
                goto bailout;
        }

	/* check environment for uuid */

	rpc_ns_entry_object_inq_begin(rpc_c_ns_syntax_default,
		 ns_entry, &context, &status);
        if(status)
        {
                printf("rpc_ns_entry_object_inq_begin:status=%d\n",
                        status);
                fflush(stdout);
                goto bailout;
        }

	rpc_ns_entry_object_inq_next(context, &resource_id, &status);
        if(status)
        {
                printf("rpc_ns_entry_object_inq_next:status=%d\n",
                        status);
                fflush(stdout);
        }

	/* need new uuid? */

	if (uuid_is_nil(&resource_id, &status))
	{
		printf("Creating new UUID.\n");
		uuid_create(&resource_id, &status);
        	if(status)
        	{
                	printf("uuid_create:status=%d\n",
                        	status);
                	fflush(stdout);
                	goto bailout;
        	}
	}

	uuid_to_string(&resource_id, &string_uuid, &status);
        if(status)
        {
                printf("uuid_to_string:status=%d\n",
                        status);
                fflush(stdout);
                goto bailout;
        }

	rpc_string_free(&string_uuid, &status);

	uuid_list.count = 1;
	uuid_list.uuid[0] = &resource_id;

        /* export entry to name service */

	rpc_ns_binding_export(rpc_c_ns_syntax_default, ns_entry,
			dce_delegate_v1_2_s_ifspec, binding_vector,
			&uuid_list, &status);
        if(status)
        {
                printf("rpc_ns_binding_export:status=%d\n",
                        status);
                fflush(stdout);
                goto bailout;
        }

        /* register endpoints */

	rpc_ep_register(dce_delegate_v1_2_s_ifspec, binding_vector,
		 &uuid_list, (unsigned_char_t *)NS_ANNOT, &status);
        if(status)
        {
                printf("rpc_ep_register:status=%d\n",
                        status);
                fflush(stdout);
                goto bailout;
        }

	rpc_mgmt_set_authorization_fn(auth_fn, &status);
        if(status)
        {
                printf("rpc_mgmt_set_authorization_fn:status=%d\n",
                        status);
                fflush(stdout);
                goto bailout;
        }

	validate_ident(&login_context, princ_name, princ_name, &status);
        if(status)
        {
                printf("validate_ident:status=%d\n",
                        status);
                fflush(stdout);
                goto bailout;
        }

	printf("Listening for remote procedure calls...\n");
	fflush(stdout);

	rpc_server_listen(rpc_c_listen_max_calls_default, &status);
        if(status)
        {
                printf("rpc_server_listen:status=%d\n",
                        status);
                fflush(stdout);
        }

bailout:

	printf("\nServer bailing out\n");
	fflush(stdout);

	/* tell client bailing out */

	system("echo delegated failed>>/:/delegated.fail.log");

	sec_login_purge_context(&login_context, &status);
	if(status)
	{
		printf("sec_login_purge_context:status=%d\n",status);
		fflush(stdout);
	}

	rpc_ep_unregister(dce_delegate_v1_2_s_ifspec, binding_vector,
		 &uuid_list, &status);
        if(status)
        {
                printf("rpc_ep_unregister:status=%d\n",status);
                fflush(stdout);
        }

	rpc_binding_vector_free(&binding_vector, &status);
        if(status)
        {
                printf("rpc_binding_vector_free:status=%d\n",status);
                fflush(stdout);
        }

	printf("SERVER DONE\n");
	fflush(stdout);
	system("date");

	exit(0);
}
