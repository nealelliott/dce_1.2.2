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
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dce/binding.h>
#include <dce/sec_login.h>
#include "delegate.h"
#include "check_status.h"

int main(int ac, char **av)
{
	unsigned32           status;
	rpc_binding_handle_t binding;
	sec_login_handle_t login_context,ilh = NULL,cur_context = NULL;
	sec_id_delegation_type_t delegation_type =
		 sec_id_deleg_type_impersonation;
	sec_id_compatibility_mode_t compatibility_mode =
		 sec_id_compat_mode_none;
	extern char *optarg;
	extern int optind, opterr, optopt;
	int c, retval = 0;
	long granted_perms, expected_perms = 0;
	char *user = NULL, *password = NULL;
	int bind_to_entry = FALSE;
	sec_id_restriction_set_t *deleg_restrict= NULL,
		 *target_restrict = NULL;

	while ((c = getopt(ac,av, "u:p:d:c:a:er:t:R:T:")) != EOF)
	{
		switch (c)
		{
		  	case 'u':
				user = optarg;
				break;
		  	case 'p':
				password = optarg;
				break;
		  	case 'a':
				expected_perms = acl_to_num(optarg);
				break;
		  	case 'e':
				bind_to_entry = TRUE;
				break;
		  	case 'd':
				switch (tolower(optarg[0]))
				{
			  		case 'n':
						delegation_type =
						 sec_id_deleg_type_none;
						break;
			  		case 't':
						delegation_type =
					       sec_id_deleg_type_traced;
						break;
			  		case 'i':
						delegation_type =
				        sec_id_deleg_type_impersonation;
						break;
			  		default:
						printf("Unknown \
						 delegation \
						 type.  Should be one \
						 of:\n");
						printf("\tnone,traced, \
						 impersonation\n");
						printf("Using default \
						 value: \
						 impersonation.\n");
						break;
				}
				break;
		  	case 'c':
				switch (tolower(optarg[0]))
				{
			  		case 'n':
						compatibility_mode =
						sec_id_compat_mode_none;
						break;
			  		case 'i':
						compatibility_mode =
					   sec_id_compat_mode_initiator;
						break;
			  		case 'c':
						compatibility_mode =
					      sec_id_compat_mode_caller;
						break;
			  		default:
						printf("Unknown \
						 compatibility \
						 mode.  Should be one \
						 of:\n");
						printf("\tnone, \
						 initiator, \
						 caller\n");
						printf("Using default \
						 value: \
						 none.\n");
						break;
				}
				break;
		  	case 'r':
				restrictions_from_file(optarg,
					 &deleg_restrict);
				break;
		  	case 't':
				restrictions_from_file(optarg,
					 &target_restrict);
				break;
		  	case 'R':
				restrictions_from_list(optarg,
					 &deleg_restrict);
				break;
		  	case 'T':
				restrictions_from_list(optarg,
					 &target_restrict);
				break;
		  	case '?':
				printf("Options are:\n");
				printf("\t-u username\n");
				printf("\t-p password\n");
				printf("\t-d delegation_type \
				 (none, traced, impersonation)\n");
				printf("\t-c \
				 compatibility_mode (none, initiator, \
				 caller)\n");

				retval=1;
				goto failures;
		}
	}

	/* Set-up the context of the client. */

	if (user && password)
	{
		printf("User is %s, password %s\n", user, password);
		validate_ident(&login_context, user, password, &status);
		if(status)
		{
			printf("validate_ident:status=%d\n",status);
			fflush(stdout);

			retval=1;
			goto failures;
		}
	}
	else
	{
		sec_login_get_current_context(&login_context, &status);
		if(status)
		{
			printf("sec_login_get_current_context:status= \
				%d\n",status);
			fflush(stdout);
			
			retval=1;
			goto failures;
		}
	}

	/* Get a binding handle to the first server on the list. */

	status=get_binding(av[optind++], login_context,
		 dce_delegate_v1_2_c_ifspec, &binding);
	if(status)
	{
		printf("get_binding:status=%d\n",status);
		fflush(stdout);

		retval=1;
		goto failures;
	}

	/* Shut down the server if we're supposed to. */

	if (strcmp(av[optind], "quit") == 0)
	{
		printf("Client shutting down server\n");
		fflush(stdout);
		rpc_mgmt_stop_server_listening(binding, &status);
		if(status)
		{
			printf("rpc_mgmt_stop_server_listening:status \
				=%d\n",status);
			fflush(stdout);
			exit(1);
		}
		sleep(5);
		rpc_mgmt_stop_server_listening(binding, &status);
		exit(0);
	}

	/* Tell the context that it can be delegated. */

	ilh = sec_login_become_initiator(login_context,
		delegation_type, deleg_restrict,
		target_restrict, NULL, NULL, compatibility_mode,
		&status);
	if(status)
	{
		printf("sec_login_become_initiator:status=%d\n",
			status);
		fflush(stdout);

		retval=1;
		goto failures;
	}

	/* Clean up the restrictions. */

	free_restrictions(&deleg_restrict);
	free_restrictions(&target_restrict);

	/* Set the auth info. */

	rpc_binding_set_auth_info(binding, NULL,
		 rpc_c_authn_level_default, rpc_c_authn_dce_secret,
		 (rpc_auth_identity_handle_t) ilh, rpc_c_authz_dce,
		 &status);
	if(status)
	{
		printf("rpc_binding_set_auth_info:status=%d\n",
			status);
		fflush(stdout);

		retval=1;
		goto failures;
	}

	/*
	Save the current context and use the context that has been
	designated to be delegatable.
	*/

	sec_login_get_current_context(&cur_context, &status);
	if(status)
	{
		printf("sec_login_get_current_context:status=%d\n",
			status);
		fflush(stdout);

		retval=1;
		goto failures;
	}

	sec_login_set_context(login_context, &status);
	if(status)
	{
		printf("sec_login_set_context:status=%d\n",status);
		fflush(stdout);

		retval=1;
		goto failures;
	}

	/*
	Print the client's access level to the object being checked.
	*/

	printf("Client access level: %s(C=%d)\n", av[ac - 1],
		 check_access(av[ac - 1],
		 bind_to_entry));
	fflush(stdout);

	/* Call the server */

	dce_delegate_v1_2_c_epv.delegate_to(binding, delegation_type,
		 compatibility_mode, bind_to_entry, optind,
		 ac - optind, (entry *)av, &granted_perms, &status);
	if(status)
	{
		system("echo delegate failed>>/tmp/delegate.fail.log");
		printf("dce_delegate_v1_2_c_epv.delegate_to: status \
			=%d\n",status);
		fflush(stdout);
		retval=1;
		goto errors;
	}

	/*
	Check to make sure that the values that were returned
	were correct.
	*/

	if (expected_perms &&
		(expected_perms != (granted_perms & expected_perms)))
	{
		printf("\nSubtest failed\n");
		printf("*error: Expected permissions(%d) do not \
			 match granted permissions(%d)\n\n",
			 expected_perms,
			 granted_perms & expected_perms);
		fflush(stdout);
		retval = 1;
	}
	else
	{
		printf("\nSubtest passed\n");
		printf("Permissions: %s(E=%d,G=%d)\n\n", av[ac - 1],
			 expected_perms,
			 granted_perms & expected_perms);
		fflush(stdout);
	}

errors:

	/* Reset context. */

	sec_login_set_context(cur_context, &status);

failures:

	/* signal test driver that something went wrong */

	if(retval)
		system("echo delegate failed>>/tmp/delegate.fail.log");
	exit(retval);
}
