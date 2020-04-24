/*
 * (C) COPYRIGHT Transarc Corp. 1996
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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dce/sec_login.h>
#include <dce/sec_cred.h>
#include "check_status.h"
#include "delegate.h"
#include "common.h"

/* Get the binding handle for the specified server. */

rpc_binding_handle_t resolve_binding(char *ns_entry,
	sec_login_handle_t login_handle)
{
	long i;
	rpc_binding_handle_t binding;

	if (get_binding(ns_entry, login_handle,
		 dce_delegate_v1_2_c_ifspec, &binding))
		return(NULL);

	return(binding);
}

/* Assume the caller's identity and check access to an object.

	ARGUMENTS:
		handle - client handle
		delegation_type - Indicates which type of delegation
			 we are checking
		compatibility_mode - Compatibility mode to use for
			 delegation calls
		bind_to_entry - Used for calls to check_access
		first - Index of the entry that we consider to be
			 the first
		length - Length of the entry list
		name - List of server names to contact, terminated by
			 the object who's access is to be checked.
		perms - The permissions that the delegated identity on
			 the last server was granted.
		rstatus - Status code.
*/

da_status_t delegate_to(
		IN handle_t handle,
		IN sec_id_delegation_type_t delegation_type,
		IN sec_id_compatibility_mode_t compatibility_mode,
		IN long bind_to_entry,
		IN long first,
		IN long length,
		IN entry name[MAX_ENTRIES],
		OUT long *perms,
		OUT error_status_t *rstatus)
{
	rpc_binding_handle_t   binding;
	long	svperms;
	rpc_authz_cred_handle_t	caller;
	unsigned_char_p_t  princ;
	unsigned32 protectl, ansvc, azsvc, status;
	sec_login_handle_t delegate_h, cur_context, dummy_context;
	char  deleg_char;
	unsigned_char_t princ_name[256];

	/*
	Print current test, length of chain left,
	and when we got here.
	*/

	printf("\n");
	fflush(stdout);
	system("cat /:/delegation.current_test");
	printf("SERVER: length=%d\n",length);
	fflush(stdout);
	system("date");

	/* Check the server's access to the object. */

	printf("Server's access\n");
	fflush(stdout);
	svperms = check_access(name[first + length - 1], bind_to_entry);
	printf("Perms=%d\n",svperms);
	fflush(stdout);

        /* Save current context */

        sec_login_get_current_context(&cur_context, rstatus);
        if(*rstatus)
        {
                printf("sec_login_get_current_context:status=%d\n",
                        *rstatus);
                fflush(stdout);
                return 0;
        }

#ifdef SUNOS5
	printf("Server pac\n");
	fflush(stdout);
	print_pac(cur_context);
#endif

	/* Set up the delegated identity */

	rpc_binding_inq_auth_caller(handle, &caller, &princ,
		 &protectl, &ansvc, &azsvc, rstatus);
	if(*rstatus)
	{
		printf("rpc_binding_inq_auth_caller:status=%d\n",
			*rstatus);
		fflush(stdout);
		return 0;
	}

	if (delegation_type == sec_id_deleg_type_traced)
	{
		deleg_char = 'T';

		delegate_h = sec_login_become_delegate(caller,
			cur_context, sec_id_deleg_type_traced,
			NULL, NULL, NULL, NULL, compatibility_mode,
			rstatus);
        	if(*rstatus)
        	{
                	printf("sec_login_become_delegate:status=%d\n",
                        	*rstatus);
                	fflush(stdout);
			return 0;
        	}
#ifdef SUNOS5
	printf("Delegates\n");
	fflush(stdout);
        print_delegates(delegate_h);
#endif


	}
	else if(delegation_type == sec_id_deleg_type_impersonation)
	{
		deleg_char = 'I';

		delegate_h=sec_login_become_impersonator(caller,
			cur_context,
			sec_id_deleg_type_impersonation,
			NULL, NULL, NULL, NULL, rstatus);
		if(*rstatus)
		{
			printf("sec_login_become_impersonator: \
				status=%d\n",*rstatus);
			fflush(stdout);
			return 0;
		}
	}
	else if (delegation_type == sec_id_deleg_type_none)
		deleg_char = 'N';
	else
	{
		printf("unknown delegation type\n");
		fflush(stdout);
		*rstatus = -1;
		return 0;
	}

	/* Use the delegated identity */

        sec_login_set_context(delegate_h, &status);
        if(status)
        {
                printf("sec_login_set_context:status=%d\n",
                        status);
                fflush(stdout);
        }

	if (length == 1)
	{
		/* Check delegated access to the object. */

		printf("Check delegated access: %s(T=%c,S=%d)\n",
			 name[first], deleg_char, svperms);
		fflush(stdout);
		*perms = check_access(name[first], bind_to_entry);
		printf("Permissions: D=%d\n",*perms);
		fflush(stdout);
	}
	else
	{
		/* Pass control on to the next server in the list. */

		if ((binding = resolve_binding((char *)name[first],
			 delegate_h)) == NULL)
		{
			printf("Could not import binding to \
				 server at %s.\n", name[first]);
			fflush(stdout);
			sec_login_purge_context(&delegate_h, &status);

			/* set rstatus */

			*rstatus = -1;

			return 0;
		}

		*perms=check_access(name[first + length - 1],
			 bind_to_entry);

		printf("Permissions: %s(T=%c,S=%d,D=%d) to %s\n",
			name[first + length - 1], deleg_char, svperms,
		   	*perms, name[first]);
		fflush(stdout);

		/* tell the context that it is delegatable */

		dce_delegate_v1_2_c_epv.delegate_to(binding,
			delegation_type,
			compatibility_mode, bind_to_entry,
			first + 1, length - 1, name, perms,
			rstatus);
	}

	printf("returning perms=%d\n",*perms);
	fflush(stdout);

	/* Restore our context. */

        sec_login_set_context(cur_context, &status);
        if(status)
        {
                printf("sec_login_set_context:status=%d\n",
                        status);
                fflush(stdout);
        }

	/* Destroy the delegated context. */

	sec_login_purge_context(&delegate_h, &status);
        if(status)
        {
                printf("sec_login_purge_context:status=%d\n",status);
                fflush(stdout);
        }

	return 0;
}

