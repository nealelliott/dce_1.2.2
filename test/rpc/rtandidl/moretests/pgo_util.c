/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pgo_util.c,v $
 * Revision 1.1.6.2  1996/02/17  23:21:41  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:51  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:08:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:24  root]
 * 
 * Revision 1.1.4.3  1993/02/25  22:03:06  cuti
 * 	Change sec_rgy_site_open() to sec_rgy_site_open_update() to work with master site.
 * 	[1993/02/25  18:32:16  cuti]
 * 
 * Revision 1.1.4.2  1993/01/11  23:38:25  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:11:26  bbelch]
 * 
 * Revision 1.1.2.5  1992/07/21  20:37:22  dceguest
 * 	remove_acct() had wrong number of args causing a bad context in
 * 	some testcases.  Changed argument list to include "context".
 * 	[1992/07/21  15:32:43  dceguest]
 * 
 * Revision 1.1.2.4  1992/07/15  02:34:22  smith
 * 	took out dead code.
 * 
 * 	removed the following routines:
 * 	      get_pgo(), reset_tstatus(), compare_pgo(), add_pgo(),
 * 	      do_add_pgo(), do_delete_pgo(),do_add_member(),
 * 	      do_delete_member(), do_add_account(), do_delete_account.
 * 
 * 	replaced the above routines with the following:
 * 	      add_account(), remove_account(), log_in(), and log_out().
 * 	[1992/07/15  02:24:02  smith]
 * 
 * Revision 1.1.2.3  1992/07/09  16:29:46  rsalz
 * 	     Fix compile problems on AIX
 * 	     [1992/07/09  13:57:05  rsalz]
 * 
 * Revision 1.1.2.2  1992/06/30  18:53:48  cuti
 * 	     Add client right to account add and ignore "object exists" on account add result.
 * 	     [1992/06/30  14:44:51  cuti]
 * 
 * Revision 1.1  1992/01/19  03:38:59  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

#include <sec_rgy.h>
#include "tsh_auth.h"
 
error_status_t log_out ();
error_status_t do_site_open ();
error_status_t add_account ();
error_status_t remove_acct ();
error_status_t log_in ();
error_status_t log_out ();

error_status_t
do_site_open (ctx, estatus, rgy_context, tstatus)
CtxBlock           *ctx;
error_status_t     estatus;
sec_rgy_handle_t   *rgy_context;
error_status_t     *tstatus;	
{
	sec_rgy_pname_t    node_name, site_name;

	sec_rgy_site_open_update ((unsigned_char_t *)"", rgy_context, tstatus);

	TSHMessage (ctx, MOTRACE,
		"sec_rgy_site_open_update status: %d\n", *tstatus);

	if (TSHStatus (ctx, SETUP, "sec_rgy_site_open_update failed",
		*tstatus, error_status_ok))
	   return *tstatus;
    
	return 0;
}
  
error_status_t
add_account (ctx, context, pgo_name, unix_num, passwd, upasswd, st)
CtxBlock           *ctx;
sec_rgy_handle_t   *context;
long               unix_num;
sec_rgy_name_t     pgo_name;
sec_passwd_str_t   passwd;
sec_passwd_str_t   upasswd;
error_status_t     *st;	
{
	sec_rgy_login_name_t    account;
	sec_rgy_pgo_item_t      pgo_item;
        sec_rgy_cursor_t        marker;
        sec_rgy_login_name_t    name_result;
        sec_rgy_sid_t           sid;
        sec_rgy_unix_sid_t      unix_sid;
        sec_rgy_acct_key_t      key_parts;
        sec_rgy_acct_user_t     user_part;
        sec_rgy_acct_admin_t    admin_part;
        sec_passwd_rec_t        caller_key;
        sec_passwd_rec_t        new_key;
        sec_passwd_type_t       new_key_type;
        sec_passwd_version_t    new_key_version;
        unsigned_char_p_t       object;
	sec_rgy_name_t		group, org;
	error_status_t		lst;

	strcpy (group, "none");
	strcpy (org, "none");
	/*
	 * Add the principal.
	 */
	uuid_create_nil (&pgo_item.id, st);
	pgo_item.unix_num = unix_num;
	pgo_item.quota = -1;
	pgo_item.quota = -1;
	pgo_item.flags = sec_rgy_pgo_projlist_ok;

	sec_rgy_pgo_add (*context,sec_rgy_domain_person,pgo_name,&pgo_item, st);

	/*
	 * If the principal is already there (sec_rgy_name_exists)
	 * set the status to ok and continue.
	 */
	if (*st == sec_rgy_name_exists || *st == sec_rgy_object_exists)
		*st = error_status_ok;

	if (TSHStatus (ctx, SETUP, "sec_rgy_pgo_add failed\n",
		*st, error_status_ok)) {
		return *st;
	}

	/*
	 * Add principal as a member of the group "none"
	 */
	if (! sec_rgy_pgo_is_member(*context, sec_rgy_domain_group, group, pgo_name, st)) {

		sec_rgy_pgo_add_member(*context, sec_rgy_domain_group, group, pgo_name, st);

		if (TSHStatus (ctx, SETUP, "sec_rgy_pgo_add_member group failed\n",
			*st, error_status_ok)) {
			remove_acct (ctx, context, pgo_name, &lst);
			return *st;
		}
	}

	/*
	 * Add principal as a member of the organization "none"
	 */
	if (! sec_rgy_pgo_is_member(*context, sec_rgy_domain_org, org, pgo_name, st)) {

		sec_rgy_pgo_add_member(*context, sec_rgy_domain_org, org, pgo_name, st);

		if (TSHStatus (ctx, SETUP, "sec_rgy_pgo_add_member org failed\n",
			*st, error_status_ok)) {
			remove_acct (ctx, context, pgo_name, &lst);
			return *st;
		}
	}

	/*
	 * Add the principal as an account.
	 */
        key_parts = (sec_rgy_acct_key_t)sec_rgy_acct_key_person;

        bzero ((char *)&user_part, sizeof (user_part));
        user_part.passwd_version_number = 1;
        user_part.passwd_dtm = time(NULL);
        user_part.flags = sec_rgy_acct_user_passwd_valid;

        bzero ((char *)&admin_part, sizeof (admin_part));
        admin_part.creation_date = time(NULL);
        admin_part.change_date = time(NULL);
        admin_part.expiration_date = time(NULL) + 24*60*60;
        admin_part.good_since_date = time(NULL);
	admin_part.flags = sec_rgy_acct_admin_valid |
		sec_rgy_acct_admin_client | sec_rgy_acct_admin_server;
        admin_part.authentication_flags = sec_rgy_acct_auth_tgt;

        bzero ((char *)&caller_key, sizeof (sec_passwd_rec_t));
        caller_key.version_number = sec_passwd_c_version_none;
        caller_key.key.key_type = sec_passwd_plain;
        caller_key.pepper=NULL;
        caller_key.key.tagged_union.plain = (idl_char *)upasswd;

        bzero ((char *)&new_key, sizeof (sec_passwd_rec_t));
        new_key.key.key_type = sec_passwd_plain;
        new_key.key.tagged_union.plain = passwd;
	strcpy (account.pname, pgo_name);
	strcpy (account.gname, "none");
	strcpy (account.oname, "none");

        sec_rgy_acct_add (*context, &account, &key_parts, &user_part,
                &admin_part, &caller_key, &new_key, sec_passwd_des,
                &new_key_version, st);

        TSHMessage (ctx, MOTRACE, "sec_rgy_acct_add (%s) status: %d\n",
                account.pname, *st);

	if (*st == sec_rgy_object_exists || *st == sec_rgy_name_exists)
		*st = error_status_ok;

        if (TSHStatus (ctx, SETUP, "sec_rgy_acct_add failed\n",
                        *st, error_status_ok)) {
		remove_acct (ctx, context, pgo_name, &lst);
                return *st;
        }

        return 0;
}

error_status_t
remove_acct (ctx, context, pname, st)
CtxBlock                *ctx;
sec_rgy_handle_t        *context;
sec_rgy_name_t          pname;
error_status_t          *st;
{
	/*
	 * Remove the principal.
	 * This will also remove the account.
	 */
        sec_rgy_pgo_delete (*context, sec_rgy_domain_person, pname, st);

        TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_delete (%d:%s) status: %d\n",
                sec_rgy_domain_person, pname, *st);

        if (*st == sec_rgy_object_not_found)
                return *st = 0;

        else if (TSHStatus (ctx, SETUP, "sec_rgy_pgo_delete failed\n",
                   *st, error_status_ok)) {
                return *st;
        }

        return 0;
}


error_status_t
log_in (ctx, auth_id, acct, passwd, st)
CtxBlock		*ctx;
sec_login_handle_t      *auth_id;
unsigned_char_p_t	acct;
unsigned_char_p_t	passwd;
error_status_t		*st;
{
	sec_passwd_rec_t	prec;
	boolean32	        reset;
	sec_login_auth_src_t	auth_src;
	error_status_t		lst;
	idl_char		*p;

	/*
	 * Log this client in by setting up the user's network identity.
	 * This operation will create any local context necessary
	 * to perform authenticated network operations.
	 */

	sec_login_setup_identity (acct, (sec_login_flags_t)0, auth_id, st);

	if (TSHStatus (ctx, SETUP, "sec_login_setup_identity failed\n",
			*st, rpc_s_ok)) {
		return *st;
	}
         
	/*
	 * Re-copy the passwd because it gets destroyed 
	 * in the sec_login_validate_identity() call.
	 */
	p = (idl_char *)malloc(strlen(passwd) + 1);
	if (p == (idl_char *)NULL)
		return (*st = rpc_s_no_memory);
	strcpy (p,passwd);

	/* Set the password information for validate_identity */
	prec.key.key_type = sec_passwd_plain;  
	prec.key.tagged_union.plain = p;
	prec.pepper = NULL;
	prec.version_number = sec_passwd_c_version_none;

	sec_login_validate_identity (*auth_id, &prec, &reset, &auth_src, st);
        free ((void *)p);

	if (TSHStatus (ctx,SETUP,"sec_login_validate_identity failed\n",
			*st, rpc_s_ok)) {
		return *st;
	}

	sec_login_set_context (*auth_id, st);

   	if (TSHStatus (ctx, SETUP, "sec_login_set_context failed\n",
			*st, rpc_s_ok)) {

		sec_login_purge_context (auth_id, &lst);

		TSHMessage(ctx,MOTRACE,"sec_login_purge_context status: %d\n",
			lst);

		TSHStatus (ctx, CLNUP, "sec_login_purge_context failed\n",
				lst, rpc_s_ok);
		return *st;
	}

	TSHMessage (ctx, MOTRACE, "sec_login... as (%s) status: %d.\n",
		acct, *st);

	return *st;
}

error_status_t
log_out (ctx, auth_id, st)
CtxBlock		*ctx;
sec_login_handle_t      *auth_id;
error_status_t		*st;
{
	sec_login_purge_context (auth_id, st);

	TSHMessage (ctx, MOTRACE,
		"sec_login_purge_context status: %d\n", *st);

	return 0;
}
