/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_login_deleg.c,v $
 * Revision 1.1.4.1  1996/07/09  15:00:55  arvind
 * 	DCE 1.2.2 branch
 * 	[1996/06/18  21:39 UTC  mullan_s  /main/mullan_dce122_bugs_1/1]
 *
 * 	Don't call tet_result() from tet_startup routines.
 * 	[1996/02/17  23:11:25  marty  1.1.2.2]
 *
 * Revision 1.1.2.2  1996/02/17  23:11:25  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:13  marty]
 * 
 * Revision 1.1.2.1  1995/12/11  19:52:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:26:50  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  18:18:13  root]
 * 
 * Revision 1.1.2.2  1994/10/06  20:33:34  agd
 * 	expand copyright
 * 	[1994/10/06  14:29:18  agd]
 * 
 * Revision 1.1.2.1  1994/08/04  16:15:29  mdf
 * 	Initial checkin
 * 	[1994/08/03  19:20:08  mdf]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 *
 * Provide the tests needed to verify that the local sec_login functions
 * associated with the new EPAC/Delegation work are functioning correctly.
 */

#include <stdio.h>
#include <errno.h>
#include <macros.h>
#include <tet_api.h>
#include <ts-common.h>

#include <dce/binding.h>
#include <dce/dce_error.h>
#include <dce/sec_attr_tools.h>

#include <dce/rgybase.h>
#include <dce/uuid.h>
#include <dce/sec_login.h>
#include <dce/sec_cred.h>

#include <sec_def_flags.h>
#include <dce/sec_rgy_attr_sch.h>
#include <dce/sec_rgy_attr.h>
#include <dce/rs_attr_schema.h>


static void tp1(void), tp2(void);
static void dlg_startup(void), dlg_cleanup(void);

/* Hardwired test attribute type id */
                                  
static uuid_t test_attr_id = { /* 5f439154-2af1-11cd-8ec3-080009353559 */
        0x5f439154,
        0x2af1,
        0x11cd,
        0x8e,
        0xc3,
        {0x08, 0x00, 0x09, 0x35, 0x35, 0x59}
      };
#define TEST_ATTR_INT 92065
#define DLG_USER "root"
#define DLG_PASSWD "-dce-"

unsigned char unexpected[dce_c_error_string_len];
unsigned char expected[dce_c_error_string_len];
sec_login_handle_t login_h;
unsigned_char_p_t  user = (unsigned_char_p_t)DLG_USER;
sec_passwd_str_t   passwd_buf;
int                dce_status;
sec_login_handle_t ilh = NULL;
sec_rgy_handle_t   write_context;

#undef CHECK_RESULT
#define CHECK_RESULT(T, FUNC, RETURNED_ST_P, EXPECTED_ST) \
    if (STATUS_EQUAL(RETURNED_ST_P, EXPECTED_ST)) { \
	if ((T)) mtet_infoline("PASSED %s()\n", FUNC); \
    } else { \
	error_status_t xst; \
	dce_error_inq_text(*(RETURNED_ST_P), unexpected, &dce_status); \
	dce_error_inq_text(EXPECTED_ST, expected, &dce_status); \
	mtet_infoline("FAILED %s()\nresult: \t%s\nexpected: \t%s\n\n", \
		FUNC, unexpected, expected); \
	tet_result(TET_FAIL); \
	if (ilh != NULL) sec_login_purge_context(ilh, &xst); \
	return; \
    }

#undef CHECK_STARTUP_RESULT
#define CHECK_STARTUP_RESULT(T, FUNC, RETURNED_ST_P, EXPECTED_ST) \
    if (STATUS_EQUAL(RETURNED_ST_P, EXPECTED_ST)) { \
	if ((T)) mtet_infoline("PASSED %s()\n", FUNC); \
    } else { \
	int test_no; \
	dce_error_inq_text(*(RETURNED_ST_P), unexpected, &dce_status); \
	dce_error_inq_text(EXPECTED_ST, expected, &dce_status); \
	mtet_infoline("FAILED %s()\nresult: \t%s\nexpected: \t%s\n\n", \
		FUNC, unexpected, expected); \
	for (test_no=0; tet_testlist[test_no++].testfunc != NULL;) \
	    tet_delete(test_no,  "startup() has failed"); \
	return; \
    }

#define NUM_ATTRS 1

/* List of tests to run that TET will be looking for */
struct tet_testlist tet_testlist[] = {
    {tp1, 1}, /* become_initiator, get_initiator, get_dlg_type, get_delegate */
    {tp2, 2}, /* local attrs: set_extended_attrs, get_extended_attrs... */
    {NULL, 0}
};

void (*tet_startup)() = dlg_startup;
void (*tet_cleanup)() = dlg_cleanup;

#define ATTR_SCH_MALLOC(P,L,STP)                                                \
{                                                                               \
    if(!((P) = malloc(L))) {                                                    \
        SET_STATUS(STP, sec_rgy_cant_allocate_memory);                          \
        CHECK_RESULT(FALSE, "ATTR_SCH_MALLOC()", STP, error_status_ok);         \
    } else {                                                                    \
        memset((P), NULL, (L));                                                 \
    }                                                                           \
}

typedef struct rgy_res_attr_sch_entry_t {
    char                        *attr_name;
    char                        *attr_id;
    sec_attr_encoding_t         attr_encoding;
    char                        *acl_mgr_type;
    sec_acl_permset_t           query_permset;
    sec_acl_permset_t           update_permset;
    sec_acl_permset_t           test_permset;
    sec_acl_permset_t           delete_permset;
    sec_attr_sch_entry_flags_t  schema_entry_flags;
    sec_attr_intercell_action_t intercell_action;
    sec_attr_trig_type_flags_t  trig_types;
    char                        *trig_binding;
    char                        *scope;
    char                        *comment;
} rgy_res_attr_sch_entry_t;

rgy_res_attr_sch_entry_t   rgy_res_attr_sch_entry[] = {
{ "test_integer",                       "5f439154-2af1-11cd-8ec3-080009353559",
  sec_attr_enc_integer,                 "06ab9320-0191-11ca-a9e8-08001e039d7d",
  sec_acl_perm_read,            sec_acl_perm_read,
  sec_acl_perm_read,            sec_acl_perm_read,
  ( sec_attr_sch_entry_multi_inst | sec_attr_sch_entry_use_defaults),
  sec_attr_intercell_act_reject,        sec_attr_trig_type_none,
  NULL,                                 "",
  "test_integer: encoding type integer" }
};


static void
Create_Schema_Entry()	
{
    sec_attr_schema_entry_t	*sch_entry_p;
    rgy_res_attr_sch_entry_t	*sp, *spe;
    error_status_t		st;

    unsigned32			length;

    CLEAR_STATUS(&st);
    if(!(sch_entry_p = (sec_attr_schema_entry_t *)malloc(
		sizeof(sec_attr_schema_entry_t)))) {
	SET_STATUS(&st, sec_rgy_cant_allocate_memory);
        CHECK_RESULT(FALSE, "malloc()", &st, error_status_ok);
    }
    /* Assuming one acl_mgr for now */
    ATTR_SCH_MALLOC(SA_ACL_MGR_SET_P(sch_entry_p), SA_ACL_MGR_SET_SIZE(1), &st);
    ATTR_SCH_MALLOC(SA_ACL_MGR_INFO_P(sch_entry_p, 0),
	sizeof(*(SA_ACL_MGR_INFO_P(sch_entry_p, 0))), &st);
    SA_ACL_MGR_NUM(sch_entry_p)	= 1;

    spe = ((rgy_res_attr_sch_entry_t *) (((char *) &rgy_res_attr_sch_entry[0]) + 
		sizeof(rgy_res_attr_sch_entry)));

    /* WARNING: this routine must be fixed to
     * free memory properly when more acl_mgr_types are
     * added to schema entries.
     */

    for (sp = &rgy_res_attr_sch_entry[0]; st == 0 && sp < spe; sp++) { 
	sch_entry_p->attr_name = (sec_attr_component_name_t)sp->attr_name;
	uuid_from_string((unsigned char *)(sp->attr_id), 
		&(sch_entry_p->attr_id), &st);
	sch_entry_p->attr_encoding	= sp->attr_encoding;

	uuid_from_string((unsigned char *)(sp->acl_mgr_type), 
	    &(SA_ACL_MGR_TYPE(sch_entry_p, 0)), &st);
	SA_ACL_MGR_PERMS_QUERY(sch_entry_p, 0)	= sp->query_permset;
	SA_ACL_MGR_PERMS_UPDATE(sch_entry_p, 0)	= sp->update_permset;
	SA_ACL_MGR_PERMS_TEST(sch_entry_p, 0)	= sp->test_permset;
	SA_ACL_MGR_PERMS_DELETE(sch_entry_p, 0)	= sp->delete_permset;

	sch_entry_p->schema_entry_flags   = sp->schema_entry_flags;
	sch_entry_p->intercell_action		= sp->intercell_action;
	sch_entry_p->trig_types			= sp->trig_types;

	/* Skip trig_binding for now */
	sch_entry_p->trig_binding	= NULL;
	sch_entry_p->scope		= (unsigned_char_p_t)(sp->scope);
	sch_entry_p->comment		= (unsigned_char_p_t)(sp->comment);

    	sec_rgy_attr_sch_create_entry(write_context, 
		sec_master_site_name, sch_entry_p, &st);
    }
    CHECK_STARTUP_RESULT(false, "sec_rgy_attr_sch_create_entry()", &st, error_status_ok);

    SEC_RGY_SITE_SYNCH(write_context);
}

static void
Delete_Schema_Entry()	
{
    sec_attr_schema_entry_t	*sch_entry_p;
    rgy_res_attr_sch_entry_t	*sp, *spe;
    error_status_t		st;
    unsigned32			length;
    uuid_t	 		*attr_id;
    
    CLEAR_STATUS(&st);
    if(!(sch_entry_p = (sec_attr_schema_entry_t *)malloc(
		sizeof(sec_attr_schema_entry_t)))) {
	SET_STATUS(&st, sec_rgy_cant_allocate_memory);
        CHECK_RESULT(FALSE, "malloc()", &st, error_status_ok);
    }
    /* Assuming one acl_mgr for now */
    ATTR_SCH_MALLOC(SA_ACL_MGR_SET_P(sch_entry_p),
	SA_ACL_MGR_SET_SIZE(1), &st);
    ATTR_SCH_MALLOC(SA_ACL_MGR_INFO_P(sch_entry_p, 0),
	sizeof(*(SA_ACL_MGR_INFO_P(sch_entry_p, 0))), &st);
    SA_ACL_MGR_NUM(sch_entry_p)	= 1;

    spe = ((rgy_res_attr_sch_entry_t *)
	(((char *) &rgy_res_attr_sch_entry[0]) + 
		sizeof(rgy_res_attr_sch_entry)));

    /* WARNING: this routine must be fixed to
     * free memory properly when more acl_mgr_types are
     * added to schema entries.
     */

    for (sp = &rgy_res_attr_sch_entry[0]; st == 0 && sp < spe; sp++) { 
	sch_entry_p->attr_name = (sec_attr_component_name_t)sp->attr_name;
	uuid_from_string((unsigned char *)(sp->attr_id), 
		&(sch_entry_p->attr_id), &st);

        sec_rgy_attr_sch_delete_entry(write_context, sec_master_site_name, 
	    &(sch_entry_p->attr_id), &st);
    }
    CHECK_RESULT(FALSE, "sec_rgy_attr_sch_delete_entry()", &st, error_status_ok);

    SEC_RGY_SITE_SYNCH(write_context);
}


/* Initialization of data needed for each of the test cases */
static void dlg_startup(void)
{
    sec_attr_t              attrs[NUM_ATTRS], out_attrs[NUM_ATTRS];
    unsigned32              num_ret, num_left;
    signed32                failure_index;
    error_status_t          st;
    sec_passwd_rec_t        prec;
    sec_login_auth_src_t    auth_src;
    boolean32               reset_passwd;
    unsigned_char_t         *sec_master_site_name = NULL;

    tet_infoline("sec_login_deleg test startup");
    CLEAR_STATUS(&st);
    strcpy((char *)passwd_buf, DLG_PASSWD);

    /* Set up the initial login context */
    if (sec_login_setup_identity(user, sec_login_no_flags, &login_h, &st)) {
        prec.key.key_type = sec_passwd_plain;
        prec.key.tagged_union.plain = passwd_buf;
        prec.pepper = NULL;
        prec.version_number = sec_passwd_c_version_none;

        if (! sec_login_validate_identity(login_h, &prec, &reset_passwd,
            &auth_src, &st)) {
            CHECK_STARTUP_RESULT(false,
                "SEC_LOGIN_DELEG setup: sec_login_validate_identity ", &st,
                error_status_ok);
        }
    } else {
        CHECK_STARTUP_RESULT(false,
            "SEC_LOGIN_DELEG setup: sec_login_setup_identity ", &st,
            error_status_ok);
    }
 
    sec_rgy_site_open_update(sec_master_site_name, &write_context, &st);
    CHECK_STARTUP_RESULT(false, 
        "SEC_LOGIN_DELEG setup: sec_rgy_site_open_update ", &st,
        error_status_ok);

    Create_Schema_Entry();
        /* Set up attributes to look for (just one for now) */
    attrs[0].attr_id = test_attr_id;
    attrs[0].attr_value.attr_encoding = sec_attr_enc_integer;
    attrs[0].attr_value.tagged_union.signed_int = TEST_ATTR_INT;
    sec_rgy_attr_update(write_context, sec_rgy_domain_person, user,
        NUM_ATTRS, NUM_ATTRS, attrs, &num_ret, out_attrs, &num_left,
        &failure_index, &st);
    CHECK_STARTUP_RESULT(false, "SEC_LOGIN_DELEG setup: sec_rgy_attr_update",
        &st, error_status_ok);
}


/* Cleanup of data needed for each of the test cases */
static void dlg_cleanup(void)
{
    error_status_t st;
    if (login_h != NULL) sec_login_purge_context(&login_h, &st);
    Delete_Schema_Entry();
    sec_rgy_site_close(write_context, &st);
}


/* Test purpose 1: Test success of local side of delegation setup/lookup */
static void tp1()
{
    sec_id_delegation_type_t    dlg_type_ret;
    sec_cred_pa_handle_t        pa_h;
    sec_id_pa_t                 *pa_dataP;
    sec_cred_cursor_t           cursor;
    error_status_t              st;
    sec_id_restriction_set_t    *deleg_restricts;
    sec_id_opt_req_t		*opt_restricts;
    sec_id_opt_req_t		*req_restricts;
    sec_id_restriction_set_t    *tgt_restricts;

    tet_infoline("test case (tp1)");

    ilh = sec_login_become_initiator(login_h, sec_id_deleg_type_traced, NULL,
	NULL, NULL, NULL, sec_id_compat_mode_initiator, &st);
    CHECK_RESULT(true, "SEC_LOGIN_DELEG: sec_login_become_initiator", &st,
        error_status_ok);

    pa_h = sec_login_cred_get_initiator(ilh, &st);
    CHECK_RESULT(true, "SEC_LOGIN_DELEG: sec_login_cred_get_initiator", &st,
        error_status_ok);

/* check restriction calls for sec_login_cred_get_initiator */
    deleg_restricts = sec_cred_get_deleg_restrictions( pa_h, &st );
    CHECK_RESULT(true, "SEC_LOGIN_DELEG: sec_cred_get_deleg_restrictions", &st,
        error_status_ok);

    opt_restricts = sec_cred_get_opt_restrictions(pa_h, &st ); 
    CHECK_RESULT(true, "SEC_LOGIN_DELEG: sec_cred_get_opt_restrictions", &st,
        error_status_ok);

    req_restricts = sec_cred_get_req_restrictions(pa_h, &st ); 
    CHECK_RESULT(true, "SEC_LOGIN_DELEG: sec_cred_get_req_restrictions", &st,
        error_status_ok);

    tgt_restricts = sec_cred_get_tgt_restrictions(pa_h, &st ); 
    CHECK_RESULT(true, "SEC_LOGIN_DELEG: sec_cred_get_tgt_restrictions", &st,
        error_status_ok);    
    

    pa_dataP = sec_cred_get_pa_data(pa_h, &st);
    CHECK_RESULT(true, "SEC_LOGIN_DELEG: sec_cred_get_pa_data", &st,
        error_status_ok);

    dlg_type_ret = sec_cred_get_delegation_type(pa_h, &st);
    CHECK_RESULT(true, "SEC_LOGIN_DELEG: sec_cred_get_delegation_type", &st,
        error_status_ok);
    if (dlg_type_ret != sec_id_deleg_type_traced) {
	switch(dlg_type_ret) {
	case sec_id_deleg_type_none:
            mtet_infoline(
                "FAILED sec_cred_get_delegation_type returned: NONE");
	    break;
	case sec_id_deleg_type_traced:
            mtet_infoline(
                "FAILED sec_cred_get_delegation_type returned: TRACED");
	    break;
	case sec_id_deleg_type_impersonation:
            mtet_infoline(
                "FAILED sec_cred_get_delegation_type returned: IMPERSONATION");
	    break;
	default:
            mtet_infoline("FAILED sec_cred_get_delegation_type returned: %d\n",
                dlg_type_ret);
	}
        tet_result(TET_FAIL);
        sec_login_purge_context(&ilh, &st);
        return;
    }

    sec_login_cred_init_cursor(&cursor, &st);
    CHECK_RESULT(true, "SEC_LOGIN_DELEG: sec_login_cred_init_cursor", &st,
        error_status_ok);

    pa_h = sec_login_cred_get_delegate(ilh, &cursor, &st);
    CHECK_RESULT(true, "SEC_LOGIN_DELEG: sec_login_cred_get_delegate", &st,
        sec_cred_s_no_more_entries);

/* check restriction calls for sec_login_cred_get_delegate with
   expected error */
    deleg_restricts = sec_cred_get_deleg_restrictions( pa_h, &st );
    CHECK_RESULT(true, "SEC_LOGIN_DELEG: sec_cred_get_deleg_restrictions", &st,
        sec_cred_s_invalid_pa_handle );

    opt_restricts = sec_cred_get_opt_restrictions(pa_h, &st ); 
    CHECK_RESULT(true, "SEC_LOGIN_DELEG: sec_cred_get_opt_restrictions", &st,
        sec_cred_s_invalid_pa_handle );

    req_restricts = sec_cred_get_req_restrictions(pa_h, &st ); 
    CHECK_RESULT(true, "SEC_LOGIN_DELEG: sec_cred_get_req_restrictions", &st,
        sec_cred_s_invalid_pa_handle );

    tgt_restricts = sec_cred_get_tgt_restrictions(pa_h, &st ); 
    CHECK_RESULT(true, "SEC_LOGIN_DELEG: sec_cred_get_tgt_restrictions", &st,
        sec_cred_s_invalid_pa_handle );    


    tet_result(TET_PASS);
    sec_login_purge_context(&ilh, &st);
}


/* Test purpose 2: Test success of local side of attribute setup/lookup */
static void tp2()
{
    unsigned32              num_attrs = NUM_ATTRS;
    sec_attr_t              attrs[NUM_ATTRS], attr;
    sec_cred_attr_cursor_t  acursor;
    sec_cred_pa_handle_t    pa_h;
    error_status_t          st;
    ilh = NULL;

	/* Set up attributes to look for (just one for now) */
    attrs[0].attr_id = test_attr_id;
    attrs[0].attr_value.attr_encoding = sec_attr_enc_void;

        /* Create new login context, requesting the above attrs */
    ilh = sec_login_set_extended_attrs(login_h, num_attrs, attrs, &st);
    CHECK_RESULT(true, "SEC_LOGIN_DELEG: (attrs) sec_login_set_extended_attrs",
        &st, error_status_ok);

    pa_h = sec_login_cred_get_initiator(ilh, &st);
    CHECK_RESULT(true, "SEC_LOGIN_DELEG: (attrs) sec_login_cred_get_initiator",
        &st, error_status_ok);

    sec_cred_initialize_attr_cursor(&acursor, &st);
    CHECK_RESULT(true,
        "SEC_LOGIN_DELEG: (attrs) sec_cred_initialize_attr_cursor",
        &st, error_status_ok);

    sec_cred_get_extended_attrs(pa_h, &acursor, &attr, &st);
    CHECK_RESULT(true, "SEC_LOGIN_DELEG: (attrs) sec_cred_get_extended_attrs",
        &st, error_status_ok);

    sec_cred_free_attr_cursor( &acursor, &st );
    CHECK_RESULT(true, "SEC_LOGIN_DELEG: (attrs) sec_cred_free_attr_cursor",
        &st, error_status_ok);

    if (attr.attr_value.attr_encoding != sec_attr_enc_integer) {
        mtet_infoline(
            "FAILED SEC_LOGIN_DELEG attrs: Returned attr type %d incorrect\n",
            attr.attr_value.attr_encoding);
        tet_result(TET_FAIL);
    } else if (attr.attr_value.tagged_union.signed_int != TEST_ATTR_INT) {
        mtet_infoline(
            "FAILED SEC_LOGIN_DELEG attrs: Returned attr value %d incorrect\n",
            attr.attr_value.tagged_union.signed_int);
        tet_result(TET_FAIL);
    } else {
        tet_result(TET_PASS);
    }


    sec_login_purge_context(&ilh, &st);
}
