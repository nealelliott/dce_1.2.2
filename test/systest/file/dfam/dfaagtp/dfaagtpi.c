/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: dfaagtpi.c,v $
 * Revision 1.1.2.2  1996/03/11  14:11:02  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:59:22  marty]
 *
 * Revision 1.1.2.1  1996/02/28  07:39:10  takano
 * 	First Submit
 * 	[1996/02/28  07:38:37  takano]
 * 
 * $EndLog$
 */


/*
 * dfaagtpi.c
 *	testsuite agent, DCE API program.
 *	Created by Y.Sakaguchi
 *
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi,Ltd. All Rights Reserved. Derechos Reservados.
 */

/*
 * $Header: /u0/rcs_trees/dce/rcs/test/systest/file/dfam/dfaagtp/dfaagtpi.c,v 1.1.2.2 1996/03/11 14:11:02 marty Exp $
 */

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <utime.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dce/sec_login.h>
#include <dce/daclif.h>
#include <dce/dce_cf.h>
#include <dce/binding.h>
#include <dce/pgo.h>
#include <dce/secidmap.h>
#include <errno.h>
#include "dfaagtp.h"

static error_status_t	sec_rgy_site_begin(error_status_t *status);
static int		find_acl_entry_and_set(sec_acl_p_t entries,
					       sec_acl_entry_type_t entry_type,
					       sec_acl_permset_t perm);

static sec_login_handle_t	login_context = NULL;
static sec_rgy_handle_t		rgy_handle = NULL;
static char			*local_cell_name = NULL;

/*
 * DCE login routine
 * reference materials: manual and agent source agt_login.c DCElogin().
 *
 * argument:
 *	principal	DCE login principal name
 *	password	DCE login password
 *
 * return:
 *	error_status_ok	true
 *	other		false
 *
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi,Ltd. All Rights Reserved. Derechos Reservados.
 */
error_status_t
dce_login(char *principal, char *password)
{
	error_status_t		status = error_status_ok;
	sec_passwd_rec_t	prec;
	boolean32		reset_passwd;
	sec_login_auth_src_t	auth_src;

	/*
	 * setup network identity
	 * (create login_context)
	 */
	if (!sec_login_setup_identity((unsigned_char_p_t)principal,
				      sec_login_no_flags, &login_context,
				      &status)) {
		return (status == error_status_ok ? -1 : status);
	}

	/*
	 * login context valid and identify
	 */
	prec.version_number = sec_passwd_c_version_none;
	prec.pepper = NULL;
	prec.key.key_type = sec_passwd_plain;
	prec.key.tagged_union.plain = (idl_char *)password;
	if (!sec_login_valid_and_cert_ident(login_context, &prec, &reset_passwd,
					    &auth_src, &status)) {
		dce_logout();
		return (status == error_status_ok ? -1 : status);
	}

	if (auth_src != sec_login_auth_src_network) {
		/*
		 * no network credentials obtained.
		 */
		dce_logout();
		/* return (sec_login_s_no_current_context); */
		return (sec_login_s_auth_local);
	}
	if (status != error_status_ok) {
		dce_logout();
		return (status);
	}

	sec_login_set_context(login_context, &status);
	if (status != error_status_ok) {
		dce_logout();
		return (status);
	}

	/*
	 * expiration password.
	 *
	if (reset_passwd) {
		expriration password ...
	}
	*/

	return (error_status_ok);
}

/*
 * DCE logout routine.
 * reference materials: manual and agent source agt_login.c DCElogout().
 *
 * argument:
 *
 * return:
 *	error_status_ok	true
 *	other		false
 *
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi,Ltd. All Rights Reserved. Derechos Reservados.
 */
error_status_t
dce_logout(void)
{
	error_status_t		status = error_status_ok;

	if (local_cell_name != NULL) {
		/* allocate from dce_cf_get_cell_name() */
		free(local_cell_name);
		local_cell_name = NULL;
	}

	/*
	 * login context purge and free
	 */
	sec_login_purge_context(&login_context, &status);
	return (status);
}

/*
 * security registory bind (begin)
 *
 * argument:
 *	*status		error code
 *
 * return:
 *	error_status_ok	true
 *	other		false
 *
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi,Ltd. All Rights Reserved. Derechos Reservados.
 */
static error_status_t
sec_rgy_site_begin(error_status_t *status)
{
	unsigned long	rval = dce_cf_st_ok;

	if (local_cell_name == NULL) {
		/*
		 * get local cell name
		 */
		dce_cf_get_cell_name(&local_cell_name, &rval);
		if (rval != dce_cf_st_ok) {
			*status = rval;
			return (-1);
		}
	}

	/*
	 * registry site open and bind
	 */
	*status = error_status_ok;
	sec_rgy_site_open((unsigned_char_t *)local_cell_name, &rgy_handle,
			  status);
	if (*status != error_status_ok) {
		return (*status);
	}

	return (error_status_ok);
}

/*
 * user (principal) or group existence check
 *
 * argument:
 *	name		check name
 *	flag		FLAG_USER : name == user name
 *			FLAG_GROUP: name == group name
 *
 * return:
 *	error_status_ok	true
 *	other		false
 *
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi,Ltd. All Rights Reserved. Derechos Reservados.
 */
error_status_t
ug_chk(char *name, char flag)
{
	error_status_t		status = error_status_ok,
				rval;
	sec_rgy_domain_t	name_domain;
	sec_rgy_name_t		pgo_name;
	sec_rgy_cursor_t	item_cursor;
	sec_rgy_pgo_item_t	pgo_item;

	/*
	 * flag check and set domain name
	 */
	switch (flag) {
	case FLAG_USER:
		name_domain = sec_rgy_domain_person;
		break;
	case FLAG_GROUP:
		name_domain = sec_rgy_domain_group;
		break;
	default:
		return (-1);
	}
	strcpy((char *)pgo_name, name);

	/*
	 * begin sec_rgy_site
	 */
	if (sec_rgy_site_begin(&rval) != error_status_ok) {
		return (rval);
	}

	/*
	 * find acct data
	 */
	sec_rgy_cursor_reset(&item_cursor);	/* reset item_cursor */
	sec_rgy_pgo_get_by_name(rgy_handle, name_domain, pgo_name, &item_cursor,
				&pgo_item, &rval);

	/*
	 * registry site unbind and close
	 */
	sec_rgy_site_close(rgy_handle, &status);
	if (status != error_status_ok && rval == error_status_ok) {
		return (status);
	}

	return (rval);
}

/*
 * find ACL entry
 * and set permission
 *
 * argument:
 *	entries		ACL entries
 *	entry_type	find entry type
 *	perm		set permission
 *
 * return:
 *	0		found and set permission
 *	-1		not found
 *
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi,Ltd. All Rights Reserved. Derechos Reservados.
 */
static int
find_acl_entry_and_set(sec_acl_p_t entries,
			    sec_acl_entry_type_t entry_type,
			    sec_acl_permset_t perm)
{
	int	i;

	for (i = 0; i < entries->num_entries; i++) {
		if (entries->sec_acl_entries[i].entry_info.entry_type
		    == entry_type) {
			entries->sec_acl_entries[i].perms = perm;
			return (0);
		}
	}
	return (-1);
}

/*
 * ACL set
 * reference materials: manual, acl_edit source, and agent source.
 *
 * argument:
 *	path		path name
 *	acl_list	ACL list
 *
 * return:
 *	error_status_ok	true
 *	other		false
 *
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi,Ltd. All Rights Reserved. Derechos Reservados.
 */
error_status_t
acl_set(char *path, struct acl_list *acl_list)
{
	int			i;
	sec_acl_handle_t	acl_h;
	unsigned32		size_used;
	unsigned32		num_types;
	uuid_t			manager_type;
	sec_acl_list_t		sec_acl_list;
	error_status_t		status = error_status_ok;
	error_status_t		rval = error_status_ok;
	int			num_print_string = 6;
	sec_acl_printstring_t	print_strings[] = {
					{ "r", "", sec_acl_perm_read   },
					{ "w", "", sec_acl_perm_write  },
					{ "x", "", sec_acl_perm_execute},
					{ "c", "", sec_acl_perm_control},
					{ "i", "", sec_acl_perm_insert },
					{ "d", "", sec_acl_perm_delete } };
	char			*cp;
	sec_acl_permset_t	perm;
	sec_rgy_name_t		global;
	sec_rgy_name_t		cell_name;
	sec_rgy_name_t		rgy_name;
	sec_rgy_domain_t	name_domain;
	long			unix_id;
	uuid_t			st_uuid;
	uuid_t			cell_id;
	uuid_t			rgy_id;
	sec_acl_entry_type_t	entry_type;
	sec_acl_entry_t		*new_acl_entry;
	sec_acl_entry_t		*add_acl_entry;
	struct stat		st;

	/*
	 * sec_rgy_site_open
	 */
	if (sec_rgy_site_begin(&status) != error_status_ok) {
		return (status);
	}

	/*
	 * bind to object
	 * (create ACL handle)
	 */
	sec_acl_bind((unsigned char *)path, 0, &acl_h, &status);
	if (status != error_status_ok) {
		sec_rgy_site_close(rgy_handle, &rval);
		return (status);
	}

	/*
	 * get manager type
	 */
	sec_acl_get_manager_types(acl_h, sec_acl_type_object, 1, &size_used,
				  &num_types, &manager_type, &status);
	if (status != error_status_ok) {
		sec_acl_release_handle(&acl_h, &rval);
		sec_rgy_site_close(rgy_handle, &rval);
		return (status);
	}

	/*
	 * get ACL list
	 */
	sec_acl_lookup(acl_h, &manager_type, sec_acl_type_object,
		       &sec_acl_list, &rval);
	if (rval != error_status_ok) {
		sec_acl_release_handle(&acl_h, &status);
		sec_rgy_site_close(rgy_handle, &status);
		return (rval);
	}

	if (stat(path, &st) != 0) {
		rval = -1;
		goto acl_set_error_end;
	}

	/*
	sec_acl_get_printstring(acl_h, &manager_type, PRINT_STRING_SIZE(32),
				&manager_type_chain, &manager_info, &tokenize,
				&num_print_string, print_strings,  &status);
	 */

	/*
	 * add sec_acl_list from acl_list
	 */
	for (;acl_list != NULL; acl_list = acl_list->next) {
		/*
		 * t_list check
		 */
		if (acl_list->t_list == NULL) {
			rval = -1;
			goto acl_set_error_end;
		}
		/*
		 * decode parmission
		 */
		perm = 0;
		for (cp = acl_list->t_list; *cp != '\0'; cp++) {
			for (i = 0; i < num_print_string; i++) {
				if (*cp == *print_strings[i].printstring) {
					if (perm&print_strings[i].permissions) {
						rval = -1;
						goto acl_set_error_end;
					}
					perm |= print_strings[i].permissions;
					break;
				}

			}
			if (i >= num_print_string) {
				rval = -1;
				goto acl_set_error_end;
			}
		}

		/*
		 * check flag and name to uuid
		 */
		switch (acl_list->flag) {
		case FLAG_USER:
		case FLAG_GROUP:
			/*
			 * name check
			 */
			if (acl_list->name == NULL) {
				rval = -1;
				goto acl_set_error_end;
			}
			/*
			 * make principal global name
			 */
			sprintf((char *)global, "%s/%s", local_cell_name,
				acl_list->name);
			/*
			 * principal global name to uuid
			 */
			if (acl_list->flag == FLAG_USER) {
				entry_type = sec_acl_e_type_user;
				name_domain = sec_rgy_domain_person;
				unix_id = st.st_uid;
				sec_id_parse_name(rgy_handle, global,
						  cell_name, &cell_id,
						  rgy_name, &rgy_id, &rval);
			}
			else {
				entry_type = sec_acl_e_type_group;
				name_domain = sec_rgy_domain_group;
				unix_id = st.st_gid;
				sec_id_parse_group(rgy_handle, global,
						   cell_name, &cell_id,
						   rgy_name, &rgy_id, &rval);
			}
			if (rval != error_status_ok) {
				goto acl_set_error_end;
			}
			/*
			 * unix_id (uid/gid) to uuid
			 */
			sec_rgy_pgo_unix_num_to_id(rgy_handle, name_domain,
						   unix_id, &st_uuid, &status);
			if (status != error_status_ok) {
				status = error_status_ok;
				break;
			}
			if (memcmp((char *)&rgy_id, (char *)&st_uuid,
				   sizeof(uuid_t)) != 0) {
				break;
			}
			entry_type = acl_list->flag == FLAG_USER ?
				     sec_acl_e_type_user_obj :
				     sec_acl_e_type_group_obj;
			if (find_acl_entry_and_set(sec_acl_list.sec_acls[0],
						   entry_type, perm) == 0) {
				continue;
			}
			break;
		case FLAG_OTHER:
			if (find_acl_entry_and_set(sec_acl_list.sec_acls[0],
						   sec_acl_e_type_other_obj,
						   perm) == 0) {
				continue;
			}
			entry_type = sec_acl_e_type_other_obj;
			break;
		case FLAG_MASK:
			if (find_acl_entry_and_set(sec_acl_list.sec_acls[0],
						   sec_acl_e_type_mask_obj,
						   perm) == 0) {
				continue;
			}
			entry_type = sec_acl_e_type_mask_obj;
			break;
		default:
			rval = -1;
			goto acl_set_error_end;
		}

		/*
		 * allocate new ACL entry
		 */
		new_acl_entry = (sec_acl_entry_t *)malloc(
				     sizeof(sec_acl_entry_t)*
				     (sec_acl_list.sec_acls[0]->num_entries+1));
		if (new_acl_entry == NULL) {
			rval = -1;
			goto acl_set_error_end;
		}
		add_acl_entry = (sec_acl_entry_t *)malloc(
						       sizeof(sec_acl_entry_t));
		if (add_acl_entry == NULL) {
			free(new_acl_entry);
			rval = -1;
			goto acl_set_error_end;
		}

		/*
		 * copy ACL entry
		 */
		for (i = 0; i < sec_acl_list.sec_acls[0]->num_entries; i++) {
			new_acl_entry[i] =
				sec_acl_list.sec_acls[0]->sec_acl_entries[i];
		}

		add_acl_entry->perms = perm;
		add_acl_entry->entry_info.entry_type = entry_type;
		if (entry_type == sec_acl_e_type_user ||
		    entry_type == sec_acl_e_type_group) {
			add_acl_entry->entry_info.tagged_union.id.uuid = rgy_id;
		}
		new_acl_entry[i] = *add_acl_entry;

		if (sec_acl_list.sec_acls[0]->num_entries > 0) {
			free((char *)sec_acl_list.sec_acls[0]->sec_acl_entries);
		}
		sec_acl_list.sec_acls[0]->sec_acl_entries = new_acl_entry;
		sec_acl_list.sec_acls[0]->num_entries++;
	}

	/*
	 * replace ACL list
	 */
	sec_acl_replace(acl_h, &manager_type, sec_acl_type_object,
			&sec_acl_list, &rval);

 acl_set_error_end:
	/*
	 * release ACL list
	 */
	sec_acl_release(acl_h, sec_acl_list.sec_acls[0], &status);
	if (status != error_status_ok && rval == error_status_ok) {
		rval = status;
	}

	/*
	 * release ACL handle
	 */
	sec_acl_release_handle(&acl_h, &status);
	if (status != error_status_ok && rval == error_status_ok) {
		rval = status;
	}

	/*
	 * registry site unbind and close
	 */
	sec_rgy_site_close(rgy_handle, &status);
	if (status != error_status_ok && rval == error_status_ok) {
		return (status);
	}

	return (rval);
}

/*
 * user or group set
 *
 * argument:
 *	path		path name
 *	name		user/group name
 *	flag		FLAG_USER : name == user name
 *			FLAG_GROUP: name == group name
 *
 * return:
 *	error_status_ok	true
 *	other		false
 *
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi,Ltd. All Rights Reserved. Derechos Reservados.
 */
error_status_t
acl_owner(char *path, char *name, char flag)
{
	sec_rgy_domain_t	name_domain;
	sec_rgy_cursor_t	item_cursor;
	sec_rgy_pgo_item_t	pgo_item;
	sec_rgy_name_t		pgo_name;
	error_status_t		status, rval;
	long			uid;
	long			gid;
	struct stat		st;

	/*
	 * path check
	 * and get uid/gid
	 */
	if (stat(path, &st) != 0) {
		return (-1);
	}

	/*
	 * flag check
	 */
	switch (flag) {
	case FLAG_USER:
		name_domain = sec_rgy_domain_person;
		gid = st.st_gid;
		break;
	case FLAG_GROUP:
		name_domain = sec_rgy_domain_group;
		uid = st.st_uid;
		break;
	default:
		return (-1);
	}
	strcpy((char *)pgo_name, name);

	/*
	 * sec_rgy_site_open
	 */
	if (sec_rgy_site_begin(&status) != error_status_ok) {
		return (status);
	}

	/*
	 * find acct data
	 */
	sec_rgy_cursor_reset(&item_cursor);	/* reset item_cursor */
	sec_rgy_pgo_get_by_name(rgy_handle, name_domain, pgo_name, &item_cursor,
				&pgo_item, &status);

	if (status != error_status_ok) {
		sec_rgy_site_close(rgy_handle, &rval);
		return (status);
	}

	if (flag == FLAG_USER) {
		uid = pgo_item.unix_num;
	}
	else {
		gid = pgo_item.unix_num;
	}

	/*
	 * registry site unbind and close
	 */
	sec_rgy_site_close(rgy_handle, &status);
	if (status != error_status_ok) {
		return (status);
	}

	/*
	 * change owner/group
	 */
	if (chown(path, uid, gid) != 0) {
		return (-1);
	}

	return (error_status_ok);
}

/*
 * file/directory atime/mtime set
 *
 * argument:
 *	path		path name
 *	time		time string
 *
 * return:
 *	error_status_ok	true
 *	other		false
 *
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi,Ltd. All Rights Reserved. Derechos Reservados.
 */
error_status_t
time_set(char *path, char *time)
{
	int		i;
	struct tm	tm;
	time_t		t;
	extern int	errno;
	char		*bp;
	struct utimbuf	utimbuf;
	char		*wstr[] = {"sun","mon","tue","wed",
				   "thu","fri","sat","???"};
	char		*mstr[] = {"jan","feb","mar","apr","may","jun",
				   "jul","aug","sep","oct","nov","dec"};

	/*
	 * week
	 */
	bp = strchr(time, ' ');
	if (bp == NULL || (bp - time) != 3) {
		return (-1);
	}
	for (i = 0; i < 8; i++) {
		if (strncasecmp(bp-3, wstr[i], 3) == 0) {
			tm.tm_wday = i;
			break;
		}
	}
	if (i >= 8) {
		return (-1);
	}

	/*
	 * month
	 */
	bp = strchr(bp+1, ' ');
	if (bp == NULL || (bp - time) != 7) {
		return (-1);
	}
	for (i = 0; i < 12; i++) {
		if (strncasecmp(bp-3, mstr[i], 3) == 0) {
			tm.tm_mon = i;
			break;
		}
	}
	if (i >= 12) {
		return (-1);
	}

	/*
	 * day
	 */
	tm.tm_mday = atoi(bp);
	while (isspace(*bp))	bp++;
	while (isdigit(*bp))	bp++;
	if (tm.tm_mday < 1 || 31 < tm.tm_mday ||
	    (bp - time) != 10 || *bp != ' ') {
		return (-1);
	}

	/*
	 * hour
	 */
	tm.tm_hour = atoi(bp);
	while (isspace(*bp))	bp++;
	while (isdigit(*bp))	bp++;
	if (tm.tm_hour < 0 || 23 < tm.tm_hour ||
	    (bp - time) != 13 || *bp != ':') {
		return (-1);
	}

	/*
	 * min
	 */
	tm.tm_min = atoi(++bp);
	while (isspace(*bp))	bp++;
	while (isdigit(*bp))	bp++;
	if (tm.tm_min < 0 || 59 < tm.tm_min ||
	    (bp - time) != 16 || *bp != ':') {
		return (-1);
	}

	/*
	 * sec
	 */
	tm.tm_sec = atoi(++bp);
	while (isspace(*bp))	bp++;
	while (isdigit(*bp))	bp++;
	if (tm.tm_sec < 0 || 59 < tm.tm_sec ||
	    (bp - time) != 19 || *bp != ' ') {
		return (-1);
	}

	/*
	 * year
	 */
	tm.tm_year = atoi(bp) - 1900;
	while (isspace(*bp))	bp++;
	while (isdigit(*bp))	bp++;
	if ((bp - time) != 24 || !(*bp == ' ' || *bp == '\n' || *bp == '\0') ||
	    tm.tm_year < 70 || 138 <= tm.tm_year) {
		return (-1);
	}

	errno = 0;
	utimbuf.actime = utimbuf.modtime = mktime(&tm);
	if (utimbuf.actime == -1 /*&& errno == ERANGE*/) {
		return (-1);
	}

	/*
	 * mtime and atime set
	 */
	if (utime(path, &utimbuf)) {
		return (-1);
	}

	return (error_status_ok);
}
