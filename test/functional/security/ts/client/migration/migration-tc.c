/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1995, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: migration-tc.c,v $
 * Revision 1.1.6.1  1996/07/09  15:01:17  arvind
 * 	Remove bogus definition of sec_master_site_name.
 * 	[1996/06/17  20:41 UTC  jrr  /main/jrr_122_1/1]
 *
 * Revision 1.1.4.3  1996/02/17  23:11:28  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:14  marty]
 * 
 * Revision 1.1.4.2  1995/12/11  19:52:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:41:25  root]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1995
 * Unpublished work. All Rights Reserved.
 * 
 * Migration tests. Test that 1.1 security functionality is
 * disabled in a cell with 1.1 security servers operating in 1.02
 * mode.
 *
 * This test case makes the following assumptions :
 *
 * a) Machine is a 1.1 DCE client
 * b) User is logged in as cell_admin, password -dce-
 */

#include <tet_api.h>

#include <stdio.h>
#include <stdlib.h>

#include <dce/binding.h>
#include <dce/id_base.h>
#include <dce/rgynbase.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>
#include <dce/sec_cred.h>
#include <dce/sec_login.h>
#include <dce/sec_pwd_mgmt.h>
#include <dce/sec_rgy_attr.h>
#include <dce/sec_rgy_attr_sch.h>

#include <ts-common.h>

static void migration_startup(void);
static void pwd_mgmt(void);
static void sec_rgy_attr_sch(void);
static void sec_rgy_attr(void);
static void deleg(void);
void (*tet_startup)() = migration_startup;
void (*tet_cleanup)() = NULL;

struct tet_testlist tet_testlist[] = {
    {pwd_mgmt, 1},
    {sec_rgy_attr_sch, 2},
    {sec_rgy_attr, 3},
    {deleg, 4},
    {NULL, 0}
};

sec_login_handle_t	your_lc;
sec_rgy_handle_t	context;

static void migration_startup(void)
{
    error_status_t	st;
    sec_rgy_bind_auth_info_t    auth_info;

    mtet_infoline("Startup for migration tests\n");
    CLEAR_STATUS(&st);

    /* get current context */
    sec_login_get_current_context(&your_lc, &st);
    CHECK_STARTUP_RESULT(false, "Migration startup: sec_login_get_current_context ",
        &st, error_status_ok);

    /* get registry server handle */
    auth_info.info_type = sec_rgy_bind_auth_dce;
    auth_info.tagged_union.dce_info.authn_level = rpc_c_protect_level_pkt_integ;
    auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret;
    auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    auth_info.tagged_union.dce_info.identity  = your_lc;
    sec_rgy_site_bind_update(sec_master_site_name, &auth_info, &context, &st);
    CHECK_STARTUP_RESULT(false, "Migration startup: sec_rgy_site_bind_update ",
        &st, error_status_ok);
}

static void pwd_mgmt(void)
{

    sec_rgy_login_name_t	login_name = { "cell_admin", "none", "none" };
    unsigned32            	num_pwds = 1,
				num_returned;
    sec_passwd_rec_t      	gen_pwds[1];
    sec_pwd_mgmt_handle_t       pwd_mgmt_h;
    error_status_t		st;

    /* setup handle - this should work */
    sec_pwd_mgmt_setup(&pwd_mgmt_h, context, login_name, your_lc, NULL, &st);
    CHECK_RESULT(true, "Migration(pwd_mgmt): sec_pwd_mgmt_setup ",
        &st, error_status_ok);

    sec_pwd_mgmt_gen_pwd(pwd_mgmt_h, num_pwds, &num_returned, gen_pwds, &st);
    CHECK_MIGRATE_RESULT(Msec_pwd_mgmt_gen_pwd, true, 
			 "Migration(pwd_mgmt): sec_pwd_mgmt_gen_pwd", &st);

    sec_pwd_mgmt_free_handle(&pwd_mgmt_h, &st);
    CHECK_RESULT(true, "Migration(pwd_mgmt): sec_pwd_mgmt_free_handle ",
        &st, error_status_ok);

    mtet_infoline("PASSED Migration(pwd_mgmt)\n");
    tet_result(TET_PASS);
}

static void sec_rgy_attr_sch(void)
{
    error_status_t	     	st;
    sec_attr_schema_entry_t  	*sch_entry_p = NULL,
				sch_entry,
				schema_entries[1];
    uuid_t			attr_id;
    unsigned32			cur_num_entries,
				num_to_read = 1,
				num_read;
    sec_attr_cursor_t		cursor;
    sec_attr_schema_entry_parts_t	modify_parts;

    memset(&sch_entry, 0, sizeof(sch_entry));
    sec_rgy_attr_sch_create_entry(context, NULL, &sch_entry, &st);
    CHECK_MIGRATE_RESULT(Msec_rgy_attr_sch_create_entry, true, 
			 "Migration(sec_rgy_attr_sch): sec_rgy_attr_sch_create_entry", &st);

    sec_rgy_attr_sch_lookup_by_id(context, sec_master_site_name, &attr_id,
				  &sch_entry, &st);
    CHECK_MIGRATE_RESULT(Msec_rgy_attr_sch_lookup_by_id, true, 
			 "Migration(sec_rgy_attr_sch): sec_rgy_attr_sch_lookup_by_id", &st);

    sec_rgy_attr_sch_lookup_by_name(context, sec_master_site_name, "_Attribute_Schema_Test_",
				    &sch_entry, &st);
    CHECK_MIGRATE_RESULT(Msec_rgy_attr_sch_lookup_by_name, true, 
			 "Migration(sec_rgy_attr_sch): sec_rgy_attr_sch_lookup_by_name", &st);

    sec_rgy_attr_sch_cursor_init(context, sec_master_site_name, &cur_num_entries, &cursor, &st);
    CHECK_MIGRATE_RESULT(Msec_rgy_attr_sch_cursor_init, true, 
			 "Migration(sec_rgy_attr_sch): sec_rgy_attr_sch_cursor_init", &st);

    sec_rgy_attr_sch_scan(context, sec_master_site_name, &cursor, num_to_read, &num_read, 
			  schema_entries, &st);
    CHECK_MIGRATE_RESULT(Msec_rgy_attr_sch_scan, true, 
			 "Migration(sec_rgy_attr_sch): sec_rgy_attr_sch_scan", &st);

    /* 
     * APIs not affected by migration - no remote calls to secd.
     *
     * sec_rgy_attr_sch_cursor_release
     * sec_rgy_attr_sch_cursor_alloc
     * sec_rgy_attr_sch_cursor_reset
     *
     * APIs not implemented yet:
     *
     * sec_rgy_attr_sch_aclmgr_strings
     * sec_rgy_attr_sch_get_acl_mgrs
     */

    sec_rgy_attr_sch_update_entry(context, sec_master_site_name, modify_parts, sch_entry_p, &st);
    CHECK_MIGRATE_RESULT(Msec_rgy_attr_sch_update_entry, true, 
			 "Migration(sec_rgy_attr_sch): sec_rgy_attr_sch_update_entry", &st);

    sec_rgy_attr_sch_delete_entry(context, sec_master_site_name, &(sch_entry_p->attr_id), &st);
    CHECK_MIGRATE_RESULT(Msec_rgy_attr_sch_delete_entry, true, 
			 "Migration(sec_rgy_attr_sch): sec_rgy_attr_sch_delete_entry", &st);

    mtet_infoline("PASSED Migration(sec_rgy_attr_sch)\n");
    tet_result(TET_PASS);
}

static void sec_rgy_attr(void)
{
    error_status_t	st;
    signed32		failure_index;
    unsigned32		num_left,
			num_ret,
                        num_attrs = 1;
    sec_attr_t		out_attrs[20],
                        *attrs = NULL;
    sec_attr_cursor_t	cursor;

    sec_rgy_attr_update(context, sec_rgy_domain_person, (unsigned_char_p_t)"root",
			num_attrs, num_attrs, attrs, &num_ret, out_attrs, &num_left,
			&failure_index, &st);
    CHECK_MIGRATE_RESULT(Msec_rgy_attr_update, true, 
			 "Migration(sec_rgy_attr): sec_rgy_attr_update", &st);

    /*
     * APIs not affected by migration - no remote calls to secd.
     *
     * sec_rgy_attr_cursor_alloc
     * sec_rgy_attr_cursor_reset
     * sec_rgy_attr_cursor_release
     *
     * APIs not implemented yet:
     *
     * sec_rgy_attr_get_effective
     * sec_rgy_attr_test_and_update 
     *
     * Developer APIs:
     * sec_attr_trig_query 
     * sec_attr_trig_update
     */

    sec_rgy_attr_lookup_by_id(context, sec_rgy_domain_person, (unsigned_char_p_t)"root",
			      &cursor, num_attrs, 20, attrs, &num_ret, out_attrs,
			      &num_left, &st);
    CHECK_MIGRATE_RESULT(Msec_rgy_attr_lookup_by_id, true, 
			 "Migration(sec_rgy_attr): sec_rgy_attr_lookup_by_id", &st);

    sec_rgy_attr_delete(context, sec_rgy_domain_person, (unsigned_char_p_t)"root", num_attrs,
			attrs, &failure_index, &st);
    CHECK_MIGRATE_RESULT(Msec_rgy_attr_delete, true, 
			 "Migration(sec_rgy_attr): sec_rgy_attr_delete", &st);

    sec_rgy_attr_cursor_init(context, sec_rgy_domain_person, (unsigned_char_p_t)"root",
			     &num_ret, &cursor, &st);
    CHECK_MIGRATE_RESULT(Msec_rgy_attr_cursor_init, true, 
			 "Migration(sec_rgy_attr): sec_rgy_attr_cursor_init", &st);

    sec_rgy_attr_lookup_by_name(context, sec_rgy_domain_person, (unsigned_char_p_t)"root",
				(char *) "TEST_ATTR", attrs, &st);
    CHECK_MIGRATE_RESULT(Msec_rgy_attr_lookup_by_name, true, 
			 "Migration(sec_rgy_attr): sec_rgy_attr_lookup_by_name", &st);

    sec_rgy_attr_lookup_no_expand(context, sec_rgy_domain_person, (unsigned_char_p_t)"root",
				  &cursor, num_attrs, 20, attrs, &num_ret, out_attrs,
				  &num_left, &st);
    CHECK_MIGRATE_RESULT(Msec_rgy_attr_lookup_no_expand, true, 
			 "Migration(sec_rgy_attr): sec_rgy_attr_lookup_no_expand", &st);

    mtet_infoline("PASSED Migration(sec_rgy_attr)\n");
    tet_result(TET_PASS);
}

static void deleg(void)
{
    sec_login_handle_t		ilh = NULL;
    sec_cred_pa_handle_t        pa_h;
    error_status_t		st;
    sec_passwd_rec_t		pwd;
    boolean32			reset_pwd;
    sec_login_auth_src_t	auth_src;
    sec_passwd_str_t	        passwd_buf = "-dce-";

    ilh = sec_login_become_initiator(your_lc, sec_id_deleg_type_traced, NULL,
				     NULL, NULL, NULL, sec_id_compat_mode_initiator, &st);
    CHECK_RESULT(true, "Migration(deleg): sec_login_become_initiator", &st, error_status_ok);

    /*pwd.key.key_type = sec_passwd_plain;
    pwd.key.tagged_union.plain = passwd_buf;
    pwd.pepper = NULL;
    pwd.version_number = sec_passwd_c_version_none;
    sec_login_valid_and_cert_ident(ilh, &pwd, &reset_pwd, &auth_src, &st);
    CHECK_MIGRATE_RESULT(Msec_login_become_initiator, true, 
			 "Migration(deleg): sec_login_become_initiator", &st);*/

    pa_h = sec_login_cred_get_initiator(ilh, &st);
    CHECK_MIGRATE_RESULT(Msec_login_cred_get_initiator, true, 
			 "Migration(deleg): sec_login_cred_get_initiator", &st);

    /*
     * APIs not affected by migration - no remote calls to secd.
     *
     * sec_cred_get_pa_data
     * sec_cred_get_delegation_type
     * sec_login_cred_init_cursor
     * sec_cred_initialize_attr_cursor
     * sec_cred_free_attr_cursor
     * sec_cred_free_cursor
     * sec_cred_free_pa_handle
     * sec_cred_get_authz_session_info
     * sec_cred_get_client_princ_name
     * sec_cred_get_deleg_restrictions
     * sec_cred_get_opt_restrictions
     * sec_cred_get_req_restrictions
     * sec_cred_get_tgt_restrictions
     * sec_cred_get_v1_pac
     * sec_cred_initialize_cursor
     * sec_cred_is_authenticated
     * sec_login_cred_get_delegate
     * sec_login_set_extended_attrs
     * sec_login_disable_delegation
     *
     * Others that don't work:
     *
     * sec_cred_get_extended_attrs - because pa_handle is invalid
     */

    mtet_infoline("PASSED Migration(deleg)\n");
    tet_result(TET_PASS);
}
