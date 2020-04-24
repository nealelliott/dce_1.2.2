/*
 * (C) COPYRIGHT Transarc Corp. 1995
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */

#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dce/rpc.h>
#include <dce/pgo.h>
#include <dce/sec_login.h>
#include <dce/id_epac.h>
#include <dce/sec_cred.h>
#include <dce/secidmap.h>

/*
So we can look at pac for Solaris.
Not supported for AIX.
*/

#ifdef SUNOS5
#include <dce/sec_login_util.h>
#endif

#include <dce/binding.h>
#include <dce/policy.h>
#include <dce/uuid.h>
#include <dcedfs/stds.h>
#include "check_status.h"
#include "common.h"

/* Local defines */

#define UNKNOWN 9
#define PRINC 10
#define GROUP 11
#define OTHER 12

/* Print out a binding handle's string binding */

unsigned32 print_binding( IN rpc_binding_handle_t binding)
{
	unsigned32 status;
	unsigned_char_t *string_binding;
	unsigned_char_t *protseq;

	rpc_binding_to_string_binding(binding,&string_binding,&status);
	if(status)
		printf("rpc_binding_to_string_binding:status=%d\n",
			status);
	else
		printf("Binding is %s\n", string_binding);

	fflush(stdout);

	rpc_string_free(&string_binding, &status);

	return (status);
}

/* retrieve the binding for a CDS object */

unsigned32 do_import_binding( IN char *entry_name,
	IN rpc_if_handle_t if_handle,
	OUT rpc_binding_handle_t *binding_h)
{
	unsigned32 status;
	rpc_ns_handle_t  import_context;
	char prot_seq[20];
	unsigned_char_t *string_binding;
	unsigned_char_t *protseq;

	rpc_ns_binding_import_begin(rpc_c_ns_syntax_default,
				    (unsigned_char_t *)entry_name,
				    if_handle, NULL,
				    &import_context, &status);
	if(status)
	{
		printf("rpc_ns_binding_import_begin:status=%d\n",
			status);
		fflush(stdout);
		return(status);
	}

	while (1)
	{
	  	rpc_ns_binding_import_next(import_context, binding_h,
			 &status);
		if(status)
		{
			printf("rpc_ns_binding_import_next:status=%d\n",
				status);
			fflush(stdout);
			break;
		}

	  	if (getenv("PROTSEQ") == NULL)
			break;
	  	else
		{
			rpc_binding_to_string_binding(*binding_h,
				 &string_binding, &status);
			if(status)
			{
				printf("rpc_binding_to_string_binding: \
					status=%d\n",status);
				fflush(stdout);
				break;
			}

			rpc_string_binding_parse(string_binding, NULL,
				 &protseq, NULL, NULL, NULL, &status);
                        if(status)
                        {
                                printf("rpc_binding_to_string_binding: \
                                        status=%d\n",status);
                                fflush(stdout);
                                break;
                        }

			strcpy(prot_seq, (char *)protseq);

			rpc_string_free(&string_binding, &status);
			rpc_string_free(&protseq, &status);

			if (strcmp(prot_seq, getenv("PROTSEQ")) == 0)
		  		break;
			else
		  		rpc_binding_free(binding_h, &status);
	  	}
	}
	rpc_ns_binding_import_done(&import_context, &status);
	if(status)
	{
		printf("rpc_ns_binding_import_done:status=%d\n",
			status);
		fflush(stdout);
	}
	return (status);
}

/* Get fully resolved binding handle from CDS object and interface */

unsigned32 get_binding( IN char *entry_name, sec_login_handle_t lc,
	IN rpc_if_handle_t if_handle,
	OUT rpc_binding_handle_t *binding_h)
{
	int result = 0, i;
	unsigned32 status, num_groups, j, dce_status;
	signed32 *groups;
	sec_rgy_handle_t rgy_handle;
	sec_login_handle_t login_context;
	sec_rgy_name_t group_name;
	unsigned_char_t *server_princ_name;
	boolean32 is_member;
	
	status = do_import_binding(entry_name, if_handle, binding_h);
	if(status)
	{
		printf("do_import_binding:status=%d\n",status);
		fflush(stdout);
		return(status);
	}

	rpc_ep_resolve_binding(*binding_h, if_handle, &status);
        if(status)
        {
                printf("rpc_ep_resolve_binding:status=%d\n",status);
                fflush(stdout);
                return(status);
        }

	rpc_mgmt_inq_server_princ_name(*binding_h,
		 rpc_c_authn_dce_secret, &server_princ_name, &status);
        if(status)
        {
                printf("rpc_mgmt_inq_server_princ_name:status=%d\n",
			status);
                fflush(stdout);
                return(status);
        }

	rpc_binding_set_auth_info(*binding_h, NULL,
		 rpc_c_authn_level_default, rpc_c_authn_dce_secret,
		 (rpc_auth_identity_handle_t) lc, rpc_c_authz_dce,
		 &status);
        if(status)
        {
                printf("rpc_binding_set_auth_info:status=%d\n",
                        status);
                fflush(stdout);
        }

	return status;
}

/*
Set up a validated identity
 	ARGUMENTS:
 	          context - Handle to set up the context in
 	          princ - Principal name to set up the context as
 	          passwd - Password for princ
 	          status - Return status code
*/

void validate_ident( OUT sec_login_handle_t *context,
	IN unsigned_char_p_t  princ, IN unsigned_char_p_t  passwd,
	OUT error_status_t *status)
{
	sec_passwd_rec_t passwd_rec;
	sec_login_auth_src_t auth_src;
	boolean32 reset_passwd;

	sec_login_setup_identity(princ, sec_login_no_flags,
		 context, status);
	if(*status)
	{
		printf("sec_login_setup_identity:status=%d\n",
			*status);
		fflush(stdout);
		return;
	}

	passwd_rec.version_number = sec_passwd_c_version_none;
	passwd_rec.pepper = NULL;
	passwd_rec.key.key_type = sec_passwd_plain;
	passwd_rec.key.tagged_union.plain = passwd;

	sec_login_validate_identity(*context, &passwd_rec,
		 &reset_passwd, &auth_src, status);
        if(*status)
        {
                printf("sec_login_validate_identity:status=%d\n",
                        *status);
                fflush(stdout);
                return;
        }

	sec_login_set_context(*context, status);
        if(*status)
        {
                printf("sec_login_set_context:status=%d\n",
                        *status);
                fflush(stdout);
                return;
        }

	return;
}

unsigned char *dup_name(unsigned char *name)
{
	unsigned char *retval;
	int i = strlen((char *)name);

	if ((retval =
	      (unsigned char *)malloc (sizeof(unsigned char) * (i + 1)))
                        == NULL)
	{
		printf("No more memory!\n");
		fflush(stdout);
		exit(2);
	}
	memcpy(retval, name, i+1);

	return retval;
}
	
int restrictions_from_list(char *list, sec_id_restriction_set_t **set)
{
	char *buf, *cur, *next, *tmp, *end_cur;
	sec_rgy_handle_t context;
	uuid_t cell_uuid, obj_uuid;
	sec_rgy_name_t cell_name, obj_name;
	sec_rgy_properties_t props;
	error_status_t status;
	int type, lineno = 0;

	if (list == NULL)
		return 0;

	buf = (char *)malloc(sizeof(char) * (strlen(list) + 1));
	strcpy(buf, list);
	next = buf;

	(*set) = (sec_id_restriction_set_t *)
		malloc(sizeof(sec_id_restriction_set_t));
	(*set)->num_restrictions = 0;
	(*set)->restrictions = NULL;

	sec_rgy_site_open(NULL, &context, &status);
	if(status)
	{
		printf("sec_rgy_site_open:status=%d\n",
			status);
		fflush(stdout);
		return status;
	}

	sec_rgy_properties_get_info(context, &props, &status);
        if(status)
        {
                printf("sec_rgy_properties_get_info:status=%d\n",
                        status);
                fflush(stdout);
                return status;
        }

	while ((cur = next) && *cur)
	{
		/* Skip any leading spaces that may have been passed. */

		while (isspace(*cur))
			cur++;

		/* Locate the next entry on the list. */

		if ((next = strchr(cur, '#')) != NULL)
		{
			*next = '\0';
			end_cur = next;
			next++;
		}
		else
			end_cur = &cur[strlen(cur)];

		/*
		Strip off any trailing spaces as they will cause the
		name not to be found.
		*/

		end_cur--;
		while (isspace(*end_cur))
		{
			*end_cur = '\0';
			end_cur--;
		}

		lineno++;
		if (strlen(cur) == 0)
			continue;
		type = UNKNOWN;

		/*
		It's quite possible to have a : in the cell name (even
		though it is not needed), but it's easy enough to check
		for a cellname since they begin with a /.
		*/

		if((*cur == '/') || (((tmp = strchr(cur,':')) == NULL)))
		{
			/*
			The file didn't specify what the thing was,
			so we'll take a crack at it.  The only directive
			that does not take an argument is any_other, so
			check for it first.
			*/
	
			if (strncasecmp(cur, "any_other", 9) == 0)
			{
				cell_uuid = props.realm_uuid;
				type = OTHER;
			}
			else
			{
				/*
				Now try for principal name. If the entry
				was a cell name, sec_id_parse_name
				returns the cell information and the
				obj_name will be blank, so make sure
				that we handle that.
				*/

				sec_id_parse_name(context,
					 (unsigned char *)cur,
					 cell_name, &cell_uuid,
					 obj_name, &obj_uuid, &status);
				if (status != error_status_ok)
				{
					/*
					Not a principal,
					try for a group.
					*/

					sec_id_parse_group(context,
						 (unsigned char *)cur,
						 cell_name, &cell_uuid,
						 obj_name, &obj_uuid,
						 &status);
					if (status != error_status_ok)
					{
						/* 
						It is not an any_other
						entry, principal, group
						or cell, so we can't
						figure out what the heck
						it is.	Bail on this
						entry, but make sure to
						print a warning message,
						so that the user knows
						their file is messed up.
						*/
				 
						printf("Processing \
						 list, at item %d \
				 		 : \"%s\" not found\n",
						 lineno, cur);
						fflush(stdout);
						continue;
					}
					else
						type = GROUP;
				}
				else
				{
					if(strlen((char *)obj_name)==0)
						type = OTHER;
					else
						type = PRINC;
				}
			}
		}
		else
		{
			/* Move past the :. */

			tmp++;

			/* And past the spaces. */

			while (isspace(*tmp))
				tmp++;

			if ((strncasecmp(cur, "user", 4) == 0)
				|| (strncasecmp(cur, "foreign_user", 12)
				 == 0))
			{
				sec_id_parse_name(context,
					 (unsigned char *)tmp,
					 cell_name, &cell_uuid,
					 obj_name,
					 &obj_uuid, &status);
				if (status != error_status_ok)
				{
					printf("Processing \
						 list, at \
						 item %d : \"%s\" not \
						 found\n",
						lineno, cur);
					fflush(stdout);
					continue;
				}
				type = PRINC;
			}
			else if ((strncasecmp(cur, "group", 5) == 0)
				 || (strncasecmp(cur, "foreign_group",
				 13) == 0))
			{
				sec_id_parse_group(context,
					(unsigned char *)tmp, cell_name,
					&cell_uuid, obj_name, &obj_uuid,
					&status);
				if (status != error_status_ok)
				{
					printf("Processing \
						 list, at \
						 item %d : \"%s\" not \
						 found\n",
						 lineno, cur);
					fflush(stdout);
					continue;
				}
				type = GROUP;
			}
			else if (strncasecmp(cur, "foreign_other", 13)
				 == 0)
			{
				sec_id_parse_name(context,
					 (unsigned char *)tmp,
					 cell_name,&cell_uuid,obj_name,
					&obj_uuid, &status);
				if (status != error_status_ok)
				{
					printf("Processing \
						 list, at \
						 item %d : \"%s\" not \
						 found\n",
						 lineno, cur);
					fflush(stdout);
					continue;
				}
				type = OTHER;
			}
			else if (strncasecmp(cur, "any_other", 9) == 0)
			{
				cell_uuid = props.realm_uuid;
				type = OTHER;
			}
		}
		if (type == UNKNOWN)
		{
			printf("Type not found while processing: \
				 %s\n", cur);
			fflush(stdout);
			continue;
		}
	
		(*set)->num_restrictions++;
		(*set)->restrictions = (sec_id_restriction_t *)
			realloc((*set)->restrictions,
			sizeof(sec_id_restriction_t) * (*set)->
			num_restrictions);
	
		if(uuid_equal(&cell_uuid, &(props.realm_uuid), &status))
		{
			/* This is the local cell */
	
			switch(type)
			{
			  	case GROUP:
					(*set)->restrictions[(*set)->
						num_restrictions - 1].
						entry_info.entry_type =
						 sec_rstr_e_type_group;
					(*set)->restrictions[(*set)->
						num_restrictions - 1].
						entry_info.tagged_union.
						id.uuid = obj_uuid;
					(*set)->restrictions[(*set)->
						num_restrictions - 1].
						entry_info.tagged_union.
						id.name =
						dup_name(obj_name);
					break;
	
			  	case PRINC:
					(*set)->restrictions[(*set)->
						num_restrictions - 1].
						entry_info.entry_type =
						sec_rstr_e_type_user;
					(*set)->restrictions[(*set)->
						num_restrictions - 1].
						entry_info.tagged_union.
						id.uuid = obj_uuid;
					(*set)->restrictions[(*set)->
						num_restrictions - 1].
						entry_info.tagged_union.
						id.name =
						dup_name(obj_name);
					break;
	
			  	case OTHER:
					(*set)->restrictions[(*set)->
						num_restrictions - 1].
						entry_info.entry_type =
					      sec_rstr_e_type_any_other;
					break;
			}
		}
		else
		{
			/* Not local cell */
	
			switch (type)
			{
			  	case GROUP:
					(*set)->restrictions[(*set)->
						num_restrictions - 1].
						entry_info.entry_type =
					  sec_rstr_e_type_foreign_group;
					(*set)->restrictions[(*set)->
						num_restrictions - 1].
						entry_info.tagged_union.
						foreign_id.id.uuid =
						obj_uuid;
					(*set)->restrictions[(*set)->
						num_restrictions - 1].
						entry_info.tagged_union.
						foreign_id.id.name =
						 dup_name(obj_name);
					(*set)->restrictions[(*set)->
						num_restrictions - 1].
						entry_info.tagged_union.
						foreign_id.realm.uuid =
						 cell_uuid;
					(*set)->restrictions[(*set)->
						num_restrictions - 1].
						entry_info.tagged_union.
						foreign_id.realm.name =
						dup_name(cell_name);
					break;
	
			  	case PRINC:
					(*set)->restrictions[(*set)->
						num_restrictions - 1].
						entry_info.entry_type =
					   sec_rstr_e_type_foreign_user;
					(*set)->restrictions[(*set)->
						num_restrictions - 1].
						entry_info.tagged_union.
						foreign_id.id.uuid =
						obj_uuid;
					(*set)->restrictions[(*set)->
						num_restrictions - 1].
						entry_info.tagged_union.
						foreign_id.id.name =
						 dup_name(obj_name);
					(*set)->restrictions[(*set)->
						num_restrictions - 1].
						entry_info.tagged_union.
						foreign_id.realm.uuid =
						cell_uuid;
					(*set)->restrictions[(*set)->
						num_restrictions - 1].
						entry_info.tagged_union.
						foreign_id.realm.name =
						 dup_name(cell_name);
					break;
	
			  	case OTHER:
					(*set)->restrictions[(*set)->
						num_restrictions - 1].
						entry_info.entry_type =
					  sec_rstr_e_type_foreign_other;
					(*set)->restrictions[(*set)->
						num_restrictions - 1].
						entry_info.tagged_union.
						foreign_id.id.uuid =
						obj_uuid;
					(*set)->restrictions[(*set)->
						num_restrictions - 1].
						entry_info.tagged_union.
						foreign_id.id.name =
						dup_name(obj_name);
					(*set)->restrictions[(*set)->
						num_restrictions - 1].
						entry_info.tagged_union.
						foreign_id.realm.uuid =
						cell_uuid;
					(*set)->restrictions[(*set)->
						num_restrictions - 1].
						entry_info.tagged_union.
						foreign_id.realm.name =
						dup_name(cell_name);
					break;
	
				}
			}
	}
	sec_rgy_site_close(context, &status);

	if ((*set)->num_restrictions == 0)
	{
		free(*set);
		*set = NULL;
	}

	return 1;
}

void free_restrictions(sec_id_restriction_set_t **set)
{
	unsigned16 i;

	if (*set == NULL)
		return;

	for (i = 0; i < (*set)->num_restrictions; i++)
	{
		switch ((*set)->restrictions[i].entry_info.entry_type)
		{
			case sec_rstr_e_type_any_other:
				/* Nothing to do */

				break;

			case sec_rstr_e_type_user:
			case sec_rstr_e_type_group:
				/* Type is sec_id_t */

				free((*set)->restrictions[i].entry_info.
					tagged_union.id.name);
				break;

			case sec_rstr_e_type_foreign_other:
			case sec_rstr_e_type_foreign_user:
			case sec_rstr_e_type_foreign_group:
				/* Type is sec_id_foreign_t */

				free((*set)->restrictions[i].entry_info.
					tagged_union.foreign_id.
					id.name);
				free((*set)->restrictions[i].entry_info.
					tagged_union.foreign_id.realm.
					name);
				break;

			default:
				printf("Unhandled type(%d) in \
					free_restrictions\n",
					(*set)->restrictions[i].
					entry_info.entry_type);
				fflush(stdout);
				break;

		}
	}
	free((*set)->restrictions);
	free(*set);
}
	
int restrictions_from_file(char *filename,
	 sec_id_restriction_set_t **set)
{
	FILE *fp;
	char buf[BUFSIZ], *tmp, *list = NULL;
	int retval;

	if ((fp = fopen(filename, "r")) == NULL)
		return 0;

	while (fgets(buf, BUFSIZ, fp) != NULL)
	{
		if (buf[0] == '#')
			continue;

		buf[strlen(buf) - 1] = '\0';
		if (strlen(buf) == 0)
			continue;

		/*
		Convert spaces in the file format to colons
		in the list format
		*/

		if ((tmp = strchr(buf, ' ')) != NULL)
			*tmp = ':';

		if (list)
		{
			list = (char *)realloc(list,
				 (sizeof(char) * (strlen(list) +
				 strlen(buf) + 2)));
			strcat(list, "#");
			strcat(list, buf);
		}
		else
		{
			list = (char *)malloc(sizeof(char) *
				 (strlen(buf) + 1));
			strcpy(list, buf);
		}
	}

	fclose(fp);

	retval = restrictions_from_list(list, set);

	free(list);

	return retval;
}

/*
Code to print pacs for Solaris.
Not supported for AIX.
*/

#ifdef SUNOS5
sec_rgy_handle_t *get_rgy_handle()
{
	unsigned32 status;
	sec_rgy_handle_t rgy_handle;

	sec_rgy_site_open("/.:",&rgy_handle,&status);
	if(status)
	{
		printf("sec_rgy_site_open: %d\n",status);
		fflush(stdout);

		return NULL;
	}

	return rgy_handle;
}

print_principal(pac)
sec_id_pac_t *pac;
{
	unsigned32 status;
	sec_rgy_handle_t *rgy_handle;
	sec_rgy_name_t cell_namep;
	sec_rgy_name_t princ_namep;
	sec_rgy_name_t global_name;

	printf("principal:\n");
	fflush(stdout);

	rgy_handle=get_rgy_handle();
	if(!rgy_handle)
		return;

	sec_id_gen_name(rgy_handle,(uuid_t *)&(pac->realm),
		(uuid_t *)&(pac->principal),
		global_name,cell_namep,princ_namep,&status);
	if(status)
	{
		printf("sec_id_gen_name: %d\n",status);
		fflush(stdout);
		
		return;
	}

	printf("%s\n",(char *)princ_namep);
	fflush(stdout);

	return;
}

print_group(pac,group)
sec_id_pac_t *pac;
sec_id_t *group;
{
        unsigned32 status;
        sec_rgy_handle_t *rgy_handle;
        sec_rgy_name_t cell_namep;
        sec_rgy_name_t group_namep;
        sec_rgy_name_t global_name;

	printf("group:\n");
	fflush(stdout);

        rgy_handle=get_rgy_handle();
        if(!rgy_handle)
                return;

        sec_id_gen_group(rgy_handle,(uuid_t *)&(pac->realm),
		(uuid_t *)group,
                global_name,cell_namep,group_namep,&status);
        if(status)
        {
                printf("sec_id_gen_group: %d\n",status);
                fflush(stdout);
                
                return;
        }

        printf("%s\n",(char *)group_namep);
        fflush(stdout);

        return;
}

print_groups(pac)
sec_id_pac_t *pac;
{
	int i;
	sec_id_t *group;
	unsigned32 status;

	printf("GROUPS:\n");
	fflush(stdout);

	group=pac->groups;
	for(i=0;i<(int)(pac->num_groups);++i)
	{
		print_group(pac,group);
		group++;
	}
}

sec_id_pac_t *get_pac(context)
sec_login_handle_t context;
{
	return(sec_login_util_get_pac(context));
}

print_pac(context)
sec_login_handle_t context;
{
	sec_id_pac_t *pac;

        printf("\nPAC:\n");
        fflush(stdout);

	pac=get_pac(context);
	if(!pac)
		return;

	printf("authenticated: %d\n",pac->authenticated);
	fflush(stdout);
	print_principal(pac);
	print_groups(pac);

	printf("\n");
	fflush(stdout);
}

check_epac(context)
sec_login_handle_t context;
{
	sec_bytes_t *epac;

	epac=sec_login_util_get_epac(context);
	if(!epac)
	{
		printf("\ncontext has NULL epac\n");
		fflush(stdout);
	}
	else
	{
		printf("\ncontext has OK epac\n");
		fflush(stdout);
	}
}

check_context(context)
sec_login_handle_t context;
{
	boolean32 status;
	boolean32 value;

        value=sec_login_certify_identity(context,&status);
        if(value)
                printf("\ncontext is certified\n");
        else
                printf("\ncontext is not certified\n");
        fflush(stdout);
}

check_caller(caller)
rpc_authz_cred_handle_t caller;
{
	boolean32 status;
	boolean32 value;

	value=sec_cred_is_authenticated(caller,&status);
	if(value)
		printf("\ncaller is authenticated\n");
	else
		printf("\ncaller is not authenticated\n");
	fflush(stdout);
}

get_principal(context,princ,status)
sec_login_handle_t context;
unsigned_char_t *princ;
boolean32 *status;
{
        sec_id_pac_t *pac;
        sec_rgy_handle_t *rgy_handle;
        sec_rgy_name_t cell_namep;
        sec_rgy_name_t princ_namep;
        sec_rgy_name_t global_name;

        pac=get_pac(context);
        if(!pac)
	{
		*status = -1;
                return;
	}

        rgy_handle=get_rgy_handle();
        if(!rgy_handle)
	{
		*status = -1;
                return;
	}

        sec_id_gen_name(rgy_handle,(uuid_t *)&(pac->realm),
                (uuid_t *)&(pac->principal),
                global_name,cell_namep,princ_namep,status);
        if(*status)
        {
                printf("sec_id_gen_name: %d\n",*status);
                fflush(stdout);

                return;
        }

	sprintf(princ,"%s",(char *)princ_namep);
}

print_delegate_group(info,group)
sec_id_pa_t *info;
sec_id_t *group;
{
        unsigned32 status;
        sec_rgy_handle_t *rgy_handle;
        sec_rgy_name_t cell_namep;
        sec_rgy_name_t group_namep;
        sec_rgy_name_t global_name;

        printf("group:\n");
        fflush(stdout);

        rgy_handle=get_rgy_handle();
        if(!rgy_handle)
                return;

        sec_id_gen_group(rgy_handle,(uuid_t *)&(info->realm),
                (uuid_t *)group,
                global_name,cell_namep,group_namep,&status);
        if(status)
        {
                printf("sec_id_gen_group: %d\n",status);
                fflush(stdout);

                return;
        }

        printf("%s\n",(char *)group_namep);
        fflush(stdout);

        return;
}

print_delegate_groups(info)
sec_id_pa_t *info;
{
        int i;
        sec_id_t *group;
        unsigned32 status;

        printf("GROUPS:\n");
        fflush(stdout);

        group=info->groups;
        for(i=0;i<(int)(info->num_groups);++i)
        {
                print_delegate_group(info,group);
                group++;
        }
}

print_delegate_principal(info)
sec_id_pa_t *info;
{
        unsigned32 status;
        sec_rgy_handle_t *rgy_handle;
        sec_rgy_name_t cell_namep;
        sec_rgy_name_t princ_namep;
        sec_rgy_name_t global_name;

        printf("principal:\n");
        fflush(stdout);

        rgy_handle=get_rgy_handle();
        if(!rgy_handle)
                return;

        sec_id_gen_name(rgy_handle,(uuid_t *)&(info->realm),
                (uuid_t *)&(info->principal),
                global_name,cell_namep,princ_namep,&status);
        if(status)
        {
                printf("sec_id_gen_name: %d\n",status);
                fflush(stdout);

                return;
        }

        printf("%s\n",(char *)princ_namep);
        fflush(stdout);

        return;
}

print_delegate(context,cursor,status)
sec_login_handle_t context;
sec_cred_cursor_t *cursor;
error_status_t *status;
{
	sec_cred_pa_handle_t delegate;
	sec_id_pa_t *info;
	error_status_t error;

	delegate=sec_login_cred_get_delegate(context,cursor,
		status);
	if(*status)
	{
        	if(*status == sec_cred_s_no_more_entries)
        	{
                	printf("No more entries.\n");
                	fflush(stdout);
        	}
		else
		{
			printf("sec_login_cred_get_delegate:%d\n",
				*status);
			fflush(stdout);
		}
		goto freepahandle;
	}

	info=sec_cred_get_pa_data(delegate,&error);
	if(error)
	{
		printf("sec_cred_get_pa_data:%d\n",error);
		fflush(stdout);
		goto freepahandle;
	}

	printf("\n");
	print_delegate_principal(info);
	print_delegate_groups(info);

freepahandle:
	sec_cred_free_pa_handle(delegate,&error);
}

print_delegates(context)
sec_login_handle_t context;
{
	sec_cred_cursor_t cursor;
	error_status_t status;

	check_epac(context);

	printf("\nEPAC\n\n");
	fflush(stdout);

	sec_login_cred_init_cursor(&cursor,&status);
	if(status)
	{
		printf("sec_login_cred_init_cursor:%d\n",status);
		fflush(stdout);

		goto freecursor;
	}

	while(!status)
		print_delegate(context,&cursor,&status);

freecursor:
        printf("\nEND EPAC\n");
        fflush(stdout);

	sec_cred_free_cursor(&cursor,&status);
	return;
}
#endif
