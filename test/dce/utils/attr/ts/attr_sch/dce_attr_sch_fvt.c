/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: dce_attr_sch_fvt.c,v $
 * Revision 1.1.4.2  1996/02/18  00:23:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:16  marty]
 *
 * Revision 1.1.4.1  1995/12/08  22:08:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:43:17  root]
 * 
 * Revision 1.1.2.2  1994/10/02  21:35:16  bowe
 * 	Better error checking in test setup.  [CR 12449]
 * 	[1994/10/02  21:34:58  bowe]
 * 
 * Revision 1.1.2.1  1994/08/11  18:35:33  pwang
 * 	Initial version
 * 	[1994/08/11  18:29:29  pwang]
 * 
 * $EndLog$
 */

/* 
** Goal here is to provide the necessary functional tests needed to 
** verify that the functions spec-ed by dce_attr_sch.idl are working 
** correctly.
*/

#include <dce_attr_sch_fvt.h>

/*
** Test purposes
*/
static void tp1(void), tp2(void), tp3(void), tp4(void), tp5(void), tp6(void), 
            tp7(void), tp8(void), tp9(void), tp10(void), tp11(void), tp12(void);
/* 
** Initialize TCM data structures 
*/
void (*tet_startup)(void) = NULL;
void (*tet_cleanup)(void) = NULL;
struct tet_testlist tet_testlist[] = {
    {tp1, 1},		/* successful test of dce_attr_sch_bind */
    {tp2, 2},		/* successful test of dce_attr_sch_create_entry */
    {tp3, 3},		/* successful test of dce_attr_sch_lookup_by_id */
    {tp4, 4},		/* successful test of dce_attr_sch_lookup_by_name */
    {tp5, 5},		/* successful test of dce_attr_sch_update_entry */
    {tp6, 6},		/* successful test of dce_attr_sch_cursor_init */
    {tp7, 7},		/* successful test of dce_attr_sch_scan */
    {tp8, 8},		/* successful test of dce_attr_sch_cursor_release */
    {tp9, 9},		/* successful test of dce_attr_sch_alloc */
    {tp10, 10},		/* successful test of dce_attr_sch_reset */
    {tp11, 11},		/* successful test of dce_attr_sch_delete_entry */
    {tp12, 12},		/* successful test of dce_attr_sch_bind_free */
    {NULL, 0}
};


static dce_attr_sch_handle_t		bind_handle;
static dce_attr_cursor_t		cursor;
static dce_attr_schema_entry_t		s_null_attr_sch_entry;
static dce_attr_bind_auth_info_t	s_null_auth_info;

static boolean				init_failed=FALSE;

static void
tp1()	/* successful test for dce_attr_sch_bind */
{

    dce_attr_bind_auth_info_t	auth_info;
    sec_login_handle_t		login_context;
    error_status_t		status;
    dce_attr_component_name_t	name;

    tet_infoline("Test purpose 1 (tp1): dce_attr_sch_bind");

    CLEAR_STATUS(&status);

    auth_info = s_null_auth_info;
    sec_login_get_current_context(&login_context, &status);
    if (GOOD_STATUS(&status)) {
        auth_info.info_type = sec_attr_bind_auth_dce;
        auth_info.tagged_union.dce_info.protect_level = test_c_prot_level;
        auth_info.tagged_union.dce_info.authn_svc = test_c_authn_svc;
        auth_info.tagged_union.dce_info.authz_svc = test_c_authz_svc;
    }
    else {
	tet_infoline("sec_login_get_current_context() failed in tp1");
	tet_result(TET_FAIL);
	init_failed = TRUE;
	return;
    }
    name = (dce_attr_component_name_t)schema_srv_name;
    if (name) {
	char buf[200];
	sprintf(buf, "Using schema: %s", name);
	tet_infoline(buf);
    }
    else {
	init_failed = TRUE;
	tet_infoline("Could not find a schema name.");
	tet_result(TET_FAIL);
	return;
    }
    dce_attr_sch_bind(name, &auth_info, &bind_handle, &status);
    if (!GOOD_STATUS(&status)) {
	init_failed = TRUE;
    }
    CHECK_RESULT(TRUE, "dce_attr_sch_bind", status, error_status_ok);

    tet_result(TET_PASS);
}

static void
tp2()	/* successful test for dce_attr_sch_create_entry */
{

    dce_attr_schema_entry_t	sch_entry;
    sec_attr_acl_mgr_info_set_t	acl_mgr_set;
    sec_attr_acl_mgr_info_t	acl_mgr_info;
    error_status_t		status, temp_status;

    tet_infoline("Test purpose 2 (tp2): dce_attr_sch_create_entry");
    if (init_failed) {
	tet_result(TET_UNRESOLVED);
	return;
    }

    CLEAR_STATUS(&status);

    sch_entry = s_null_attr_sch_entry;
    sch_entry.attr_name = (sec_attr_component_name_t) 
			  test_attr_sch_entry->attr_name;
    uuid_from_string((unsigned char *) test_attr_sch_entry->attr_id, 
                     &sch_entry.attr_id, &temp_status);
    sch_entry.attr_encoding = test_attr_sch_entry->attr_encoding;

    uuid_from_string((unsigned char *) test_attr_sch_entry->acl_mgr_type,
		     &acl_mgr_info.acl_mgr_type, &temp_status);
    acl_mgr_info.query_permset = test_attr_sch_entry->query_permset;
    acl_mgr_info.update_permset = test_attr_sch_entry->update_permset;
    acl_mgr_info.test_permset = test_attr_sch_entry->test_permset;
    acl_mgr_info.delete_permset = test_attr_sch_entry->delete_permset;
    acl_mgr_set.num_acl_mgrs = 1;
    acl_mgr_set.mgr_info[0] = &acl_mgr_info;
    sch_entry.acl_mgr_set = &acl_mgr_set;
    
    sch_entry.schema_entry_flags = test_attr_sch_entry->schema_entry_flags;
    sch_entry.intercell_action   = test_attr_sch_entry->intercell_action;
    sch_entry.trig_types = test_attr_sch_entry->trig_types;
    sch_entry.scope  = (unsigned char *) test_attr_sch_entry->scope;
    sch_entry.comment  = (unsigned char *) test_attr_sch_entry->comment;

    dce_attr_sch_create_entry(bind_handle, &sch_entry, &status);
    CHECK_RESULT(TRUE, "dce_attr_sch_create_entry", status, error_status_ok);

    tet_result(TET_PASS);
}

static void
tp3()	/* successful test for dce_attr_sch_lookup_by_id */
{
    dce_attr_schema_entry_t	sch_entry;
    uuid_t	 		attr_id;
    boolean32		 	result;	
    unsigned char		*string_uuid;
    error_status_t		status, temp_status;

    tet_infoline("Test purpose 3 (tp3): dce_attr_sch_lookup_by_id");
    if (init_failed) {
	tet_result(TET_UNRESOLVED);
	return;
    }
    
    CLEAR_STATUS(&status);

    uuid_from_string((unsigned char *) test_attr_sch_entry->attr_id, 
		     &attr_id, &temp_status);
    dce_attr_sch_lookup_by_id(bind_handle, &attr_id, &sch_entry, &status);
    uuid_to_string(&sch_entry.acl_mgr_set->mgr_info[0]->acl_mgr_type,
		   &string_uuid, &temp_status);
    result = 
	((!strcmp((char *) sch_entry.attr_name, 
		  test_attr_sch_entry->attr_name)) &&
	 (sch_entry.attr_encoding == test_attr_sch_entry->attr_encoding) &&
	 (sch_entry.acl_mgr_set->num_acl_mgrs == 1) &&
	 (!strcmp((char *) string_uuid, 
		  test_attr_sch_entry->acl_mgr_type)) && 
	 (sch_entry.acl_mgr_set->mgr_info[0]->query_permset ==
	  test_attr_sch_entry->query_permset) &&
	 (sch_entry.acl_mgr_set->mgr_info[0]->update_permset ==
	  test_attr_sch_entry->update_permset) &&
	 (sch_entry.acl_mgr_set->mgr_info[0]->test_permset ==
	  test_attr_sch_entry->test_permset) &&
	 (sch_entry.acl_mgr_set->mgr_info[0]->delete_permset ==
	  test_attr_sch_entry->delete_permset) &&
	 (sch_entry.schema_entry_flags == 
	  test_attr_sch_entry->schema_entry_flags) &&	
	 (sch_entry.intercell_action == 
	  test_attr_sch_entry->intercell_action) &&
	(sch_entry.trig_types == test_attr_sch_entry->trig_types) &&
	(!strcmp((char *) sch_entry.scope, test_attr_sch_entry->scope)) &&
	(!strcmp((char *) sch_entry.comment, test_attr_sch_entry->comment)));	
    rpc_string_free(&string_uuid, &temp_status);
    sec_attr_util_sch_ent_free_ptrs(&sch_entry);

    CHECK_RESULT(result, "dce_attr_sch_lookup_by_id", status, error_status_ok);

    tet_result(TET_PASS);
}

static void
tp4()	/* successful test for dce_attr_sch_lookup_by_name */
{
    dce_attr_schema_entry_t	sch_entry;
    char	 		*attr_name;
    unsigned char		*string_uuid, *string_uuid_mgr;
    boolean32		 	result;	
    error_status_t		status, temp_status;
    
    tet_infoline("Test purpose 4 (tp4): dce_attr_sch_lookup_by_name");
    if (init_failed) {
	tet_result(TET_UNRESOLVED);
	return;
    }

    CLEAR_STATUS(&status);

    attr_name = test_attr_sch_entry->attr_name;
    dce_attr_sch_lookup_by_name(bind_handle, 
				(sec_attr_component_name_t) attr_name, 
				&sch_entry, &status);
    uuid_to_string(&sch_entry.attr_id, &string_uuid, &temp_status);
    uuid_to_string(&sch_entry.acl_mgr_set->mgr_info[0]->acl_mgr_type,
		   &string_uuid_mgr, &temp_status);
    result = 
	((!strcmp((char *) string_uuid, test_attr_sch_entry->attr_id)) &&
	 (sch_entry.attr_encoding == test_attr_sch_entry->attr_encoding) &&
	 (sch_entry.acl_mgr_set->num_acl_mgrs == 1) &&
	 (!strcmp((char *) string_uuid_mgr, 
		  test_attr_sch_entry->acl_mgr_type)) && 
	 (sch_entry.acl_mgr_set->mgr_info[0]->query_permset ==
	  test_attr_sch_entry->query_permset) &&
	 (sch_entry.acl_mgr_set->mgr_info[0]->update_permset ==
	  test_attr_sch_entry->update_permset) &&
	 (sch_entry.acl_mgr_set->mgr_info[0]->test_permset ==
	  test_attr_sch_entry->test_permset) &&
	 (sch_entry.acl_mgr_set->mgr_info[0]->delete_permset ==
	  test_attr_sch_entry->delete_permset) &&
	 (sch_entry.schema_entry_flags == 
	  test_attr_sch_entry->schema_entry_flags) &&	
	 (sch_entry.intercell_action == 
	  test_attr_sch_entry->intercell_action) &&
	 (sch_entry.trig_types == test_attr_sch_entry->trig_types) &&
	 (!strcmp((char *) sch_entry.scope, test_attr_sch_entry->scope)) &&
	(!strcmp((char *) sch_entry.comment, test_attr_sch_entry->comment)));	
    rpc_string_free(&string_uuid, &temp_status);
    rpc_string_free(&string_uuid_mgr, &temp_status);
    sec_attr_util_sch_ent_free_ptrs(&sch_entry);

    CHECK_RESULT(result, "dce_attr_sch_lookup_by_name", status, 
		 error_status_ok);

    tet_result(TET_PASS);
}


static void
tp5()	/* successful test for dce_attr_sch_cursor_init */
{
    unsigned32			cur_num_entries;
    boolean32		 	result;	
    error_status_t		status;
    
    tet_infoline("Test purpose 5 (tp5): dce_attr_sch_cursor_init");
    if (init_failed) {
	tet_result(TET_UNRESOLVED);
	return;
    }

    CLEAR_STATUS(&status);
    
    dce_attr_sch_cursor_init(bind_handle, &cur_num_entries, &cursor, &status);
    result = ((cursor) && (cur_num_entries > 0));
    CHECK_RESULT(result, "dce_attr_sch_cursor_init", status, error_status_ok);

    tet_result(TET_PASS);

}

static void
tp6()	/* successful test for dce_attr_sch_scan */
{
    unsigned32			num_read;
    sec_attr_schema_entry_t	sch_entries;
    boolean32			result;
    error_status_t		status;
 
    tet_infoline("Test purpose 6 (tp6): dce_attr_sch_scan");
    if (init_failed) {
	tet_result(TET_UNRESOLVED);
	return;
    }

    CLEAR_STATUS(&status);

    sch_entries = s_null_attr_sch_entry;
    dce_attr_sch_scan(bind_handle, &cursor, 1, &num_read, 
		      &sch_entries, &status);
    result = ((sch_entries.attr_name) && (num_read == 1));
    CHECK_RESULT(result, "dce_attr_sch_scan", status, error_status_ok);

    tet_result(TET_PASS);
}

static void
tp7()	/* successful test for dce_attr_sch_cursor_release */
{
    boolean32		result;
    error_status_t	status;
 
    tet_infoline("Test purpose 7 (tp7): dce_attr_sch_cursor_release");
    if (init_failed) {
	tet_result(TET_UNRESOLVED);
	return;
    }

    CLEAR_STATUS(&status);

    dce_attr_sch_cursor_release(&cursor, &status);
    result = (cursor == NULL);
    CHECK_RESULT(result, "dce_attr_sch_cursor_release", status, 
		 error_status_ok);

    tet_result(TET_PASS);
}

static void
tp8()	/* successful test for dce_attr_sch_cursor_alloc */
{
    boolean32		 	result;	
    error_status_t		status;
    
    tet_infoline("Test purpose 8 (tp8): dce_attr_sch_cursor_alloc");
    if (init_failed) {
	tet_result(TET_UNRESOLVED);
	return;
    }

    CLEAR_STATUS(&status);
    
    dce_attr_sch_cursor_alloc(&cursor, &status);
    result = (cursor != NULL);
    CHECK_RESULT(result, "dce_attr_sch_cursor_alloc", status, error_status_ok);

    tet_result(TET_PASS);

}

static void
tp9()	/* successful test for dce_attr_sch_cursor_reset */
{
    error_status_t		status;
    
    tet_infoline("Test purpose 9 (tp9): dce_attr_sch_cursor_reset");
    if (init_failed) {
	tet_result(TET_UNRESOLVED);
	return;
    }

    CLEAR_STATUS(&status);
    
    dce_attr_sch_cursor_reset(&cursor, &status);
    CHECK_RESULT(TRUE, "dce_attr_sch_cursor_init", status, error_status_ok);

    tet_result(TET_PASS);

}

static void
tp10()	/* successful test for dce_attr_sch_update_entry */
{
    dce_attr_schema_entry_t		sch_entry;
    sec_attr_acl_mgr_info_set_t		acl_mgr_set;
    sec_attr_acl_mgr_info_t		acl_mgr_info;
    uuid_t	 			attr_id;
    dce_attr_schema_entry_parts_t	modify_parts;
    boolean32				result;
    error_status_t			status, temp_status;
    
    tet_infoline("Test purpose 10 (tp10): dce_attr_sch_update_entry");
    if (init_failed) {
	tet_result(TET_UNRESOLVED);
	return;
    }

    CLEAR_STATUS(&status);

    sch_entry = s_null_attr_sch_entry;
    sch_entry.attr_name = (sec_attr_component_name_t) 
			  test_attr_sch_entry->attr_name;
    uuid_from_string((unsigned char *) test_attr_sch_entry->attr_id, 
                     &sch_entry.attr_id, &temp_status);
    sch_entry.attr_encoding = test_attr_sch_entry->attr_encoding;

    uuid_from_string((unsigned char *) test_attr_sch_entry->acl_mgr_type,
		     &acl_mgr_info.acl_mgr_type, &temp_status);
    acl_mgr_info.query_permset = test_attr_sch_entry->query_permset;
    acl_mgr_info.update_permset = test_attr_sch_entry->update_permset;
    acl_mgr_info.test_permset = test_attr_sch_entry->test_permset;
    acl_mgr_info.delete_permset = test_attr_sch_entry->delete_permset;
    acl_mgr_set.num_acl_mgrs = 1;
    acl_mgr_set.mgr_info[0] = &acl_mgr_info;
    sch_entry.acl_mgr_set = &acl_mgr_set;
    
    sch_entry.schema_entry_flags = test_attr_sch_entry->schema_entry_flags;
    sch_entry.intercell_action   = test_attr_sch_entry->intercell_action;
    sch_entry.trig_types = test_attr_sch_entry->trig_types;
    sch_entry.schema_entry_flags = test_attr_sch_entry->schema_entry_flags;
    sch_entry.intercell_action   = test_attr_sch_entry->intercell_action;
    sch_entry.trig_types = test_attr_sch_entry->trig_types;
    sch_entry.scope  = (unsigned char *) test_attr_sch_entry->scope;
    sch_entry.comment  = (unsigned char *) schema_comment_update;
    modify_parts = sec_attr_schema_part_comment;
    dce_attr_sch_update_entry(bind_handle, modify_parts, &sch_entry, &status);
 
    attr_id = sch_entry.attr_id;
    sch_entry = s_null_attr_sch_entry;
    dce_attr_sch_lookup_by_id(bind_handle, &attr_id, &sch_entry, &temp_status);

    result = (strcmp((char *) sch_entry.comment, schema_comment_update) == 0); 
    if (GOOD_STATUS(&temp_status)) {
    	sec_attr_util_sch_ent_free_ptrs(&sch_entry);
    }

    CHECK_RESULT(result, "dce_attr_sch_update_entry", status, error_status_ok);

    tet_result(TET_PASS);
}

static void
tp11()	/* successful test for dce_attr_sch_delete_entry */
{
    uuid_t	 		attr_id;
    dce_attr_schema_entry_t	sch_entry;
    boolean32			result;
    error_status_t		status, temp_status;
    
    tet_infoline("Test purpose 11 (tp11): dce_attr_sch_delete_entry");
    if (init_failed) {
	tet_result(TET_UNRESOLVED);
	return;
    }

    CLEAR_STATUS(&status);
    
    uuid_from_string((unsigned char *) test_attr_sch_entry->attr_id, 
		     &attr_id, &temp_status);

    dce_attr_sch_delete_entry(bind_handle, &attr_id, &status);
     
    dce_attr_sch_lookup_by_id(bind_handle, &attr_id, &sch_entry, &temp_status);
    if (!(result = BAD_STATUS(&temp_status))) {
    	sec_attr_util_sch_ent_free_ptrs(&sch_entry);
    }
    CHECK_RESULT(result, "dce_attr_sch_delete_entry", status, error_status_ok);

    tet_result(TET_PASS);
}

static void
tp12()	/* successful test for dce_attr_sch_bind_free */
{
    boolean32			result;
    error_status_t		status;
    
    tet_infoline("Test purpose 12 (tp12): dce_attr_sch_bind_free");
    if (init_failed) {
	tet_result(TET_UNRESOLVED);
	return;
    }

    CLEAR_STATUS(&status);
    
    dce_attr_sch_bind_free(&bind_handle, &status);
     
    result = (bind_handle == NULL);
    CHECK_RESULT(result, "dce_attr_sch_bind_free", status, error_status_ok);

    tet_result(TET_PASS);
}

