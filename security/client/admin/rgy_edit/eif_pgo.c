/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: eif_pgo.c,v $
 * Revision 1.1.8.2  1996/03/11  13:26:39  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:38  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:58:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:29:40  root]
 * 
 * Revision 1.1.6.2  1994/09/09  19:11:51  mob
 * 	Removing the temporary, incomplete ERA support from rgy_edit (OT 11652,11262)
 * 	[1994/09/09  00:09:06  mob]
 * 
 * Revision 1.1.6.1  1994/03/02  17:54:11  hanfei
 * 	Initial ERA drop
 * 	[1994/02/24  21:07:46  hanfei]
 * 
 * Revision 1.1.4.5  1992/12/29  12:39:02  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:35:04  zeliff]
 * 
 * Revision 1.1.4.4  1992/10/25  20:14:01  burati
 * 	CR5732 Don't assume a numeric value is a Unix id
 * 	[1992/10/23  19:49:16  burati]
 * 
 * Revision 1.1.4.3  1992/10/12  19:44:59  burati
 * 	CR2828 rgy_edit coredumps deleting >1020char names
 * 	[1992/10/09  21:05:05  burati]
 * 
 * Revision 1.1.4.2  1992/10/08  22:32:14  burati
 * 	CR5510 domain_text array too small for "principal"
 * 	[1992/10/08  21:49:42  burati]
 * 
 * Revision 1.1.2.3  1992/05/08  18:19:29  burati
 * 	Use sec_rgy_name_t instead of ek_pname_buf_t
 * 	[1992/05/07  23:10:02  burati]
 * 
 * Revision 1.1.2.2  1992/03/24  18:14:59  burati
 * 	CR2106 Make sure eif_change_pgo always 'return's a value
 * 	[1992/03/23  21:10:30  burati]
 * 
 * Revision 1.1  1992/01/19  14:46:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  eif_pgo.c V=19 02/19/92 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit generic pgo interface routines
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)eif_pgo.c	19 - 02/19/92";
#endif

#include <stdio.h>
#include <ctype.h>
#include <un_strng.h>
#include <un_maloc.h>

#include <macros.h>

#include <eif_pgo.h>
#include <eif_util.h>

#include <ek_types.h>
#include <ek_error.h>

#include <tty_io.h>
#include <tty_pgo.h>

#include <args.h>

/*
** Private routines
*/

/* FUNCTION is_number
**
** tells whether or not a string is all decimal digits
*/
PRIVATE boolean32 is_number
#ifndef __STDC__
    (str)
    char *str;
#else
  (
    char *str
  )
#endif
{
    boolean32 valid;
    int     i, length;

    length = strlen(str);
    if (length == 0)
	return false;

    valid = true;
    for (i = 0; i < length && valid; i++) {
	if (isascii(*str))
	    if (isdigit(*str))
		str++;
	    else
		valid = false;
    }	
    return valid;
}	


/*
** Public routines
*/

PUBLIC boolean32 eif_write_pgo_field
#ifndef __STDC__
    ( pgo_handle, field, data, domain, check_default, stp)
    ek_op_handle_t  pgo_handle;
    ek_field_id_t   field;
    char            *data;
    eif_domain_t    domain;
    boolean32       check_default;
    error_status_t  *stp;
#else
  (
    ek_op_handle_t  pgo_handle,
    ek_field_id_t   field,
    char            *data,
    eif_domain_t    domain,
    boolean32       check_default,
    error_status_t  *stp
  )
#endif
{
    char                 error_buf[256];
    ek_op_id_t           cur_op;
    sec_rgy_name_t       default_val;
    ek_login_name_buf_t  expanded_owner;
    boolean32            is_wild;
    long                 default_len;
    eif_domain_name_t    domain_text, op_text;

    SET_STATUS(stp, error_status_ok);

    cur_op = ek_get_op_id(pgo_handle);

        /* never a default for a key value */
    if (check_default && (field != ekn_pgo_key)) {
	ek_read_field(pgo_handle, field, (char *) default_val,
                        sizeof(default_val), &default_len);
	if ( (strlen(data) == default_len) &&
	     (strncmp((char *)default_val, data, default_len) == 0) )
	    return false;
    }
    switch (field) {
    case ekn_pgo_key:
	*stp =  ek_write_field(pgo_handle, field, data, strlen(data));
	if (BAD_STATUS(stp)) {
	
	    switch (cur_op) {

	    case ekn_person_add:
	    case ekn_group_add:
	    case ekn_org_add:
		strcpy(op_text, "add");
		break;

	    case ekn_person_change:
	    case ekn_group_change:
	    case ekn_org_change:
		strcpy(op_text, "change");
		break;
	    case ekn_person_delete:
	    case ekn_group_delete:
	    case ekn_org_delete:
		strcpy(op_text, "delete");
		break;
	    case ekn_person_get_by_name:
	    case ekn_person_get_by_num:
	    case ekn_group_get_by_name:
	    case ekn_group_get_by_num:
	    case ekn_org_get_by_name:
	    case ekn_org_get_by_num:
		strcpy(op_text, "view");
		break;
	    }
	
	    switch (domain) {
	    case eif_domain_person:
		strcpy(domain_text, "principal");
		break;
	    case eif_domain_group:
		strcpy(domain_text, "group");
		break;
	    case eif_domain_org:
		strcpy(domain_text, "org");
		break;
	    }
	    sprintf(error_buf, "Unable to %s %s", op_text, domain_text);
	    tty_status_post_echo(data, error_buf, *stp);
	}
	break;

    case ekn_pgo_name:
    case ekn_pgo_unix_num:
    case ekn_pgo_fullname:
    case ekn_pgo_uuid:
    case ekn_pgo_quota:
	*stp = ek_write_field(pgo_handle, field, data, strlen(data));
	if (BAD_STATUS(stp)) {
	    tty_status_pre_echo(data, "", *stp);
	}
	break;

    case ekn_pgo_is_an_alias:
    case ekn_pgo_projlist_ok:
	*stp= ek_write_field(pgo_handle, field, data, strlen(data));
	if (BAD_STATUS(stp)) {
	    tty_print_status("", *stp);
	}
	break;
	
    default:
	SET_STATUS(stp, ek_bad_field);
	EIF_FATAL("Internal error in eif_write_pgo_field", stp);
    }

    return GOOD_STATUS(stp);
}

/* FUNCTION eif_view_pgo
 *
 *  Takes a handle and some booleans indicating the amount of info to display.
 *  The operation id in the handle indicates the kind of pgo query. If it's a
 *  name query and the ekn_pgo_key field of the handle contains a non-zero
 *  length name, then just do one lookup.  If it's a name query on a zero
 *  length name or a unix num query then invoke the operation repeatedly until
 *  a non-zero status is returned.  An ek_misc_no_more_entries is normal -
 *  anything else gets reported.
 */

PUBLIC error_status_t eif_view_pgo
#ifndef __STDC__
    ( domain, key, show_full, show_members, show_policy, list_by_num )
    eif_domain_t   domain;
    char           *key;
    boolean32      show_full;
    boolean32      show_members;
    boolean32      show_policy;
    boolean32      list_by_num;
#else
  (
    eif_domain_t   domain,
    char           *key,
    boolean32      show_full,
    boolean32      show_members,
    boolean32      show_policy,
    boolean32      list_by_num
  )
#endif
{

    boolean32       list_one_by_name;
    error_status_t  st;
    char            error_buf[256];
    boolean32       found_one;
    ek_op_handle_t  op_handle;

    SET_STATUS(&st, error_status_ok);

    switch (domain) {    /* use the appropriate global handle */
	
    case eif_domain_person:
	if (list_by_num)
	    op_handle = P_get_by_num_h;
	else
	    op_handle = P_get_by_name_h;
	break;
    case eif_domain_group:
	if (list_by_num)
	    op_handle = G_get_by_num_h;
	else
	    op_handle = G_get_by_name_h;
	break;
    case eif_domain_org:
	if (list_by_num)
	    op_handle = O_get_by_num_h;
	else
	    op_handle = O_get_by_name_h;
	break;
    default:
	return st;     /* just in case */
    } /* switch (domain) */

    list_one_by_name = (!list_by_num && strlen(key) != 0);

        /* do the first lookup */
    st = ek_init_op(op_handle);
    if (!eif_write_pgo_field(op_handle, ekn_pgo_key, key, domain, false, &st))
	if (BAD_STATUS(&st))  return st;

    st = ek_do_op(op_handle);
    found_one = (GOOD_STATUS(&st));

    if (found_one) {
	tty_pgo_display(op_handle, show_full, show_members, show_policy, &st);
	if (BAD_STATUS(&st))  tty_print_status("", st);
	
	if (!list_one_by_name) {  /* look for more entries */
	    while (GOOD_STATUS(&st)) {
		if (show_members || show_policy)
		    printf("\n");    /* separate the successive member entries */
		st = ek_do_op(op_handle);
		if (GOOD_STATUS(&st)) {
		    tty_pgo_display(op_handle, show_full, show_members,
				    show_policy, &st);
		    if (BAD_STATUS(&st))  tty_print_status("", st);
		}
	    }

	    if (! (STATUS_EQUAL(&st, ek_misc_no_more_entries)) ) {
		if (list_by_num) {
		    sprintf(error_buf, "Cannot retrieve next entry for %c%s%s",
			    DQ,key, DQ);
		    tty_print_status(error_buf, st);
		} else {  /* !list_by_num */
		    tty_print_status("Cannot retrieve next entry", st);
		}
	    }  else
		SET_STATUS(&st, error_status_ok);
	
	}  /* if (!list_one_by_name) */
	
    } else {  /* !found_one */
	if (list_one_by_name) {
	    sprintf (error_buf, "Cannot retrieve entry for %s", key);
	    tty_print_status(error_buf, st);
	} else if (list_by_num) {
	    sprintf (error_buf, "Cannot retrieve any matching entries for %s",
		     key);
	    tty_print_status(error_buf, st);
	} else
	    tty_print_status ("Cannot retrieve any entries", st);
	
	if (STATUS_EQUAL(&st, ek_misc_object_not_found) ||
	    STATUS_EQUAL(&st, ek_misc_no_more_entries))
	    /* Not finding a match doesn't count as a failure */
	    SET_STATUS(&st, error_status_ok);
    }  /* if (!found_one) */

    return st;
}

PUBLIC error_status_t eif_add_pgo
#ifndef __STDC__
    ( add_handle, domain )
    ek_op_handle_t  add_handle;
    eif_domain_t    domain;
#else
  (
    ek_op_handle_t  add_handle,
    eif_domain_t    domain
  )
#endif
{
    ek_name_buf_t        cur_name;
    long int             cur_name_len;
    eif_domain_name_t    domain_text;
    char                 error_buf[256];
    error_status_t       st;

    st = ek_do_op(add_handle);

    if (BAD_STATUS(&st)) {
	ek_read_field(add_handle, ekn_pgo_key, cur_name,
		       sizeof(cur_name), &cur_name_len);
	switch (domain) {
	case eif_domain_person:
	    strcpy(domain_text, "principal");
	    break;
	case eif_domain_group:
	    strcpy(domain_text, "group");
	    break;
	case eif_domain_org:
	    strcpy(domain_text, "org");
	    break;
	}
	sprintf(error_buf, "Unable to add %s ", domain_text);
	tty_status_post_echo(cur_name, error_buf, st);
    } 

    return st;
}

PUBLIC error_status_t eif_change_pgo
#ifndef __STDC__
    ( chg_handle, domain )
    ek_op_handle_t  chg_handle;
    eif_domain_t    domain;
#else
  (
    ek_op_handle_t  chg_handle,
    eif_domain_t    domain
  )
#endif
{
    ek_name_buf_t       cur_name;
    long int            cur_name_len;
    eif_domain_name_t   domain_text;
    char                error_buf[256];
    error_status_t      st;

    st = ek_do_op(chg_handle);

    if (BAD_STATUS(&st)) {
        ek_read_field(chg_handle, ekn_pgo_key, cur_name,
                       sizeof(cur_name), &cur_name_len);

       switch (domain) {
        case eif_domain_person:
            strcpy(domain_text, "principal");
            break;
        case eif_domain_group:
            strcpy(domain_text, "group");
            break;
        case eif_domain_org:
            strcpy(domain_text, "org");
            break;
        }
        sprintf(error_buf, "Unable to change %s ", domain_text);
        tty_status_post_echo(cur_name, error_buf, st);
    }

    return st;
}

PUBLIC error_status_t eif_adopt_pgo
#ifndef __STDC__
    ( op_handle, domain )
    ek_op_handle_t  op_handle;
    eif_domain_t    domain;
#else
  (
    ek_op_handle_t  op_handle,
    eif_domain_t    domain
  )
#endif
{
    ek_uuid_buf_t        uuid;
    long int             uuid_len;
    eif_domain_name_t    domain_text;
    char                 error_buf[256];
    error_status_t       st;

    st = ek_do_op(op_handle);

    if (BAD_STATUS(&st)) {
	ek_read_field(op_handle, ekn_pgo_uuid, uuid, sizeof(uuid), &uuid_len);
	switch (domain) {
	case eif_domain_person:
	    strcpy(domain_text, "principal");
	    break;
	case eif_domain_group:
	    strcpy(domain_text, "group");
	    break;
	case eif_domain_org:
	    strcpy(domain_text, "org");
	    break;
	}
	sprintf(error_buf, "Unable to adopt %s uuid", domain_text);
	tty_status_post_echo(uuid, error_buf, st);
    }

    return st;
}


PUBLIC error_status_t eif_delete_pgo
#ifndef __STDC__
    ( del_handle, domain, verify )
    ek_op_handle_t  del_handle;
    eif_domain_t    domain;
    boolean32       verify;   /* verify deletes? */
#else
  (
    ek_op_handle_t  del_handle,
    eif_domain_t    domain,
    boolean32       verify
  )
#endif
{
    error_status_t  st;
    sec_rgy_name_t  name;
    long int        name_len;
    ek_flag_buf_t   alias_flag;
    long            flag_len;
    char            error_msg[256],
                    prompt[sec_rgy_name_t_size+80];
    eif_domain_name_t   domain_text;
    tty_y_n_response_t  y_n_ans;

    SET_STATUS(&st, error_status_ok);

    ek_read_field(del_handle, ekn_pgo_key, (char *)name, sizeof(name), &name_len);

    switch (domain) {
    case eif_domain_person:
	sprintf(domain_text, "principal");
	break;
    case eif_domain_group:
	sprintf(domain_text, "group");
	sprintf(prompt, "WARNING: any accounts for this %s (%s) \
will also be deleted."
		,domain_text, name);
	tty_end_prompt(prompt);
	break;
    case eif_domain_org:
	sprintf(domain_text, "org");
	sprintf(prompt, "WARNING: any accounts for this %s (%s)\
 will also be deleted."
		, domain_text, name);
	tty_end_prompt(prompt);
	break;
    }
    sprintf(prompt, "Please confirm delete of name %c%s%c",
	    DQ, name, DQ);

    if (verify) {
	y_n_ans = tty_ask_y_n(prompt, tty_response_no);
    } else {
	y_n_ans = tty_response_yes;
    }

    if (y_n_ans == tty_response_yes) {
	st = ek_do_op(del_handle);
    }

    if (BAD_STATUS(&st)) {
	sprintf(error_msg, "Unable to delete %s", domain_text);
	tty_status_post_echo((char *)name, error_msg, st);
    }

    return st;
}

	
PUBLIC boolean32 eif_write_member_field
#ifndef __STDC__
    ( op_handle, field, data, stp )
    ek_op_handle_t  op_handle;
    ek_field_id_t   field;
    char            *data;
    error_status_t  *stp;
#else
  (
    ek_op_handle_t  op_handle,
    ek_field_id_t   field,
    char            *data,
    error_status_t  *stp
  )
#endif
{
    ek_op_id_t      cur_op;
    char            error_buf[256];

    cur_op = ek_get_op_id(op_handle);

    SET_STATUS(stp, error_status_ok);

    *stp =  ek_write_field(op_handle, field, data, strlen(data));
    if (BAD_STATUS(stp)) {
	switch (field) {
	case ekn_mem_key:
	
	    switch (cur_op) {
	    case ekn_mem_get_person_groups:
		sprintf(error_buf,
			"Unable to view group memberships for principal");
		break;
	    case ekn_mem_get_group_members:
		sprintf(error_buf, "Unable to view members of group");
		break;
	    case ekn_mem_get_org_members:
		sprintf(error_buf, "Unable to view members of org");
		break;
	    case ekn_mem_add_group_member:
		sprintf(error_buf, "Unable to add member to group");
		break;
	    case ekn_mem_add_org_member:
		sprintf(error_buf, "Unable to add member to org");
		break;
	    case ekn_mem_del_group_member:
		sprintf(error_buf, "Unable to remove member from group");
		break;
	    case ekn_mem_del_org_member:
		sprintf(error_buf, "Unable to remove member from org");
		break;
	    default:
		SET_STATUS(stp, ek_bad_op_id);
		EIF_FATAL("fatal error in eif_write_member_field", stp);
		break;
	    }

	    break;
	case ekn_mem_name:

	    switch (cur_op) {
	    case ekn_mem_add_group_member:
		sprintf(error_buf, "Unable to add group member");
		break;
	    case ekn_mem_add_org_member:
		sprintf(error_buf, "Unable to add org member");
		break;
	    case ekn_mem_del_group_member:
		sprintf(error_buf, "Unable to remove group member");
		break;
	    case ekn_mem_del_org_member:
		sprintf(error_buf, "Unable to remove org member");
		break;
		
		/* the name field is readonly on view operations */
	    case ekn_mem_get_group_members:
		/*FALLTHROUGH*/
	    case ekn_mem_get_org_members:
		/*FALLTHROUGH*/
	    case ekn_mem_get_person_groups:
		/*FALLTHROUGH*/
	    default:
		SET_STATUS(stp, ek_bad_op_id);
		EIF_FATAL("fatal error in eif_write_member_field", stp);
		break;
	    }
	
	    break;
	}   /* end [switch (field)] */

	tty_status_post_echo(data, error_buf, *stp);
    }

    return GOOD_STATUS(stp);
}


PUBLIC error_status_t eif_do_member_op
#ifndef __STDC__
    ( op_handle, verify )
    ek_op_handle_t  op_handle;
    boolean32       verify;
#else
  (
    ek_op_handle_t  op_handle,
    boolean32       verify
  )
#endif
{
    ek_op_id_t          which_op;
    eif_domain_name_t   domain_text;
    char                op_text[7],
                        article_text[5],
                        prompt[(2*sec_rgy_name_t_size)+80];

    ek_name_buf_t   go_name,
                    mem_name;
    long            go_name_len,
                    mem_name_len;
    char            error_msg[256];
    error_status_t  st;
    tty_y_n_response_t  y_n_ans;

    SET_STATUS(&st, error_status_ok);

    which_op = ek_get_op_id(op_handle);

    ek_read_field(op_handle, ekn_mem_key, go_name, sizeof(go_name),
		   &go_name_len);
    ek_read_field(op_handle, ekn_mem_name, mem_name, sizeof(mem_name),
		   &mem_name_len);
	
    switch (which_op) {
    case ekn_mem_add_group_member:
	strcpy(op_text, "add");
	strcpy(article_text, "to");
	strcpy(domain_text, "group");
	/* don't verify adds no matter what the caller says */
	verify = false;
	break;
    case ekn_mem_add_org_member:
	strcpy(op_text, "add");
	strcpy(domain_text, "org");
	strcpy(article_text, "to");
	/* don't verify adds no matter what the caller says */
	verify = false;
	break;
    case ekn_mem_del_group_member:
	strcpy(op_text, "remove");
	strcpy(article_text, "from");
	strcpy(domain_text, "group");
	sprintf(prompt,
		"WARNING: any accounts for (%s %s) will also be deleted.",
		mem_name, go_name);
	break;
    case ekn_mem_del_org_member:
	strcpy(op_text, "remove");
	strcpy(article_text, "from");
	strcpy(domain_text, "org");
	sprintf(prompt,
		"WARNING: any accounts for (%s %s) will also be deleted.",
		mem_name, go_name);
	break;
    default:
	SET_STATUS(&st, ek_bad_op_id);
	EIF_FATAL("fatal error in eif_do_member_op", &st);
	break;
    }

    if (verify) {
	tty_end_prompt(prompt);   /* display warning */
	sprintf(prompt, "Please confirm removal of %c%s%c from membership list",
		DQ, mem_name, DQ);	
	y_n_ans = tty_ask_y_n(prompt, tty_response_no);
    } else {
	y_n_ans = tty_response_yes;
    }

    if (y_n_ans == tty_response_yes) {
	st = ek_do_op(op_handle);
    }

    if (BAD_STATUS(&st)) {
	sprintf(error_msg, "Unable to %s member %s %s %s %s",
		op_text, mem_name, article_text, domain_text, go_name);
	tty_print_status(error_msg, st);
    }

    return st;

}



/* eif_member_update
 * takes an add/delete membership operation handle, a group/org name, and a
 * membership list (consisting of person names separated by commas).  Attempts to
 * add/delete each of the members in turn to/from the specified group/org.
 */
PUBLIC error_status_t eif_member_update
#ifndef __STDC__
    ( op_handle, go_name, member_list, verify )
    ek_op_handle_t  op_handle;
    char            *go_name;
    char            *member_list;
    boolean32       verify;
#else
  (
    ek_op_handle_t  op_handle,
    char            *go_name,
    char            *member_list,
    boolean32       verify
  )
#endif
{
    char            *comma_ptr,
                    **optv;             /* vector for multiple member names */
    int             optc,               /* count of multiple member names */
                    i;
    error_status_t  save_status;
    char            *list_buff;
    error_status_t  st;

        /* write the pgo name to the key field */
    eif_write_member_field(op_handle, ekn_mem_key, go_name, &st);

    if (GOOD_STATUS(&st)) {
	/* break out the individual names from the option argument.  Use the
	 * args_get from the command line processing package, but use a local
	 * copy of the member list so we don't walk over the one we got.
	 */
	list_buff = malloc(strlen(member_list) + 1);
	strcpy(list_buff, member_list);
	/* replace any commas between member names with blanks so we can use
	 * get_args to parse the option arguments
	 */
	while (comma_ptr = index(list_buff, ',')) {
	    *comma_ptr = ' ';
	}
	args_get(list_buff, &optc, &optv);

	    /* try to add/delete each member */
	SET_STATUS(&save_status, error_status_ok);
	for (i = 0; i < optc; i++) {
	    eif_write_member_field(op_handle, ekn_mem_name, optv[i], &st);
	    if (GOOD_STATUS(&st)) {
		st = eif_do_member_op(op_handle, verify);
	    }
	    if (BAD_STATUS(&st));
		save_status = st;  /* remember that last thing that went wrong */
	}
	free(list_buff);
    }

    SET_STATUS(&st, save_status);

    return st;
}
