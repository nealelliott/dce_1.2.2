/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_rgy_attr_sch.c,v $
 * Revision 1.1.10.3  1996/02/17  23:11:50  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:22  marty]
 *
 * Revision 1.1.10.2  1995/12/11  19:53:29  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/10/19  14:48 UTC  psn
 * 	Fix OSF OT 13050: uninitialized param in func tp11()
 * 	[1995/10/19  14:42 UTC  psn  /main/HPDCE02/psn_ot13050/1]
 * 
 * 	HP revision /main/HPDCE02/2  1995/06/10  10:58 UTC  sekhar
 * 	Added new functional tests for VISTA code coverage.
 * 
 * 	HP revision /main/HPDCE02/sekhar_mothra_fix/1  1995/06/09  12:20 UTC  sekhar
 * 	Added new functional tests for VISTA code coverage.
 * 	
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/05  16:54 UTC  mullan_s
 * 	.
 * 
 * 	HP revision /main/mullan_mothra_schema_fixes/2  1994/12/05  16:39 UTC  mullan_s
 * 	Removed sec_attr_sch_entry_reserved flag from schema so it can be deleted
 * 	successfully by tp8.
 * 
 * 	HP revision /main/mullan_mothra_schema_fixes/1  1994/12/05  15:44 UTC  mullan_s
 * 	tp7: changed expected result to status_ok.
 * 	[1995/12/11  19:27:18  root]
 * 
 * Revision 1.1.5.2  1994/10/27  20:07:38  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:39:59  rrizzo]
 * 
 * Revision 1.1.5.1  1994/10/25  17:17:50  sekhar
 * 	[ OT 12398 ]  Checking in Sean Mullan's changes for testing
 * 		      security replication.
 * 	[1994/10/24  20:24:59  sekhar]
 * 
 * 	HP revision /main/mullan_mothra_test_repl_cov/2  1994/09/28  19:41 UTC  mullan_s
 * 	More replica macro changes.
 * 
 * 	HP revision /main/mullan_mothra_test_repl_cov/1  1994/09/28  15:10 UTC  mullan_s
 * 	Remove CHECK_RESULT definition.
 * 
 * Revision 1.1.2.1  1994/08/04  16:16:04  mdf
 * 	Initial checkin
 * 	[1994/08/03  19:19:56  mdf]
 * 
 * $EndLog$
 */
/* 
 * Goal here is to provide the tests needed to verify that
 * the functions within sec_rgy_attr_sch.c are working correctly.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <tet_api.h>
#include <ts-common.h>

#include <rgymacro.h>

#include <dce/binding.h>
#include <dce/dce_error.h>
#include <dce/sec_attr_tools.h>

#include <rsdb_acl.h>
#include <sec_def_flags.h>

#include <dce/sec_rgy_attr_sch.h>
#include <dce/rs_attr_schema.h>

static void tp1() , tp2()  , tp3()  , tp4();
static void tp5() , tp6()  , tp7()  , tp8();
static void tp9() , tp10() , tp11() , tp12();
static void sec_rgy_attr_sch_startup(void);
static void sec_rgy_attr_sch_cleanup(void);

#define ATTR_SCH_MALLOC(P,L,STP)        { 			              	\
    if(!((P) = malloc(L))) {                            			\
        SET_STATUS(STP, sec_rgy_cant_allocate_memory);  			\
        CHECK_RESULT(FALSE, "ATTR_SCH_MALLOC()", STP, error_status_ok);		\
    } else {                                            			\
        memset((P), NULL, (L));                         			\
    } }


/* 
 * Initialize TCM data structures 
 */

void (*tet_startup)() = sec_rgy_attr_sch_startup;
void (*tet_cleanup)() = sec_rgy_attr_sch_cleanup;
struct tet_testlist tet_testlist[] = {
    {tp1 ,  1},		/* successful test of sec_rgy_attr_sch_create_entry */
    {tp2 ,  2},		/* successful test of sec_rgy_attr_sch_lookup_by_id */
    {tp3 ,  3},		/* successful test of sec_rgy_attr_sch_lookup_by_name */
    {tp4 ,  4},		/* successful test of sec_rgy_attr_sch_cursor_init */
    {tp5 ,  5},		/* successful test of sec_rgy_attr_sch_scan */
    {tp6 ,  6},		/* successful test of sec_rgy_attr_sch_cursor_reset*/
    {tp7 ,  7},		/* successful test of sec_rgy_attr_sch_cursor_release */
    {tp8 ,  8},		/* successful test of sec_rgy_attr_sch_cursor_alloc   */
    {tp9 ,  9},		/* successful test of sec_rgy_attr_sch_get_acl_mgrs   */
    {tp10, 10},		/* successful test of sec_rgy_attr_sch_aclmgr_stings   */
    {tp11, 11},		/* successful test of sec_rgy_attr_sch_update_entry */
    {tp12, 12},		/* successful test of sec_rgy_attr_sch_delete_entry */
    {NULL, 0}
};

/*
 * Test Case Wide Declarations 
 */



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

#define  ATTR_NAME "_Attribute_Schema_Test_"
#define  ATTR_ID "8992aa8e-2151-11cd-882b-0800097086d2"
#define  ACL_MGR_TYPE_PERSON  "06ab9320-0191-11ca-a9e8-08001e039d7d"
         /* person acl_mgr_type from rsdb_acl.c */

rgy_res_attr_sch_entry_t   rgy_res_attr_sch_entry[] = {
{ ATTR_NAME, ATTR_ID,
  sec_attr_enc_integer,                 
  ACL_MGR_TYPE_PERSON,
  sec_acl_perm_mgmt_info,               
  sec_acl_perm_mgmt_info,
  sec_acl_perm_mgmt_info,              
  sec_acl_perm_mgmt_info,
  sec_attr_sch_entry_use_defaults,
  sec_attr_intercell_act_reject,        
  sec_attr_trig_type_none,
  NULL,                                   
  "",
  "values: {0=ENFORCE-BY-THIRD-PARTY, 1=ENFORCE-BY-REGISTRY}" }
};


sec_rgy_handle_t	read_context, write_context;
sec_attr_cursor_t	cursor;
sec_rgy_name_t      	tmp_name;
unsigned long		status;
unsigned long		*st_p = &status;
char msg[1024];			/* buffer for info lines */

static void
sec_rgy_attr_sch_startup(void)
{
    SEC_RGY_SITE_OPEN_WRITE(&write_context);
    SEC_RGY_SITE_OPEN_READ(&read_context);
}

static void
sec_rgy_attr_sch_cleanup(void)
{
    error_status_t st;

    sec_rgy_site_close(write_context, &st);
    sec_rgy_site_close(read_context, &st);
}

static void
Create_Schema_Entry()	
{

    sec_attr_schema_entry_t	*sch_entry_p;
    rgy_res_attr_sch_entry_t	*sp, *spe;
    unsigned32			length;

    CLEAR_STATUS(st_p);
    if(!(sch_entry_p = (sec_attr_schema_entry_t *)malloc(
		sizeof(sec_attr_schema_entry_t)))) {
	SET_STATUS(st_p, sec_rgy_cant_allocate_memory);
        CHECK_RESULT(FALSE, "malloc()", st_p, error_status_ok);
    }
    /* Assuming one acl_mgr for now */
    ATTR_SCH_MALLOC(SA_ACL_MGR_SET_P(sch_entry_p), SA_ACL_MGR_SET_SIZE(1), st_p);
    ATTR_SCH_MALLOC(SA_ACL_MGR_INFO_P(sch_entry_p, 0),
	sizeof(*(SA_ACL_MGR_INFO_P(sch_entry_p, 0))), st_p);
    SA_ACL_MGR_NUM(sch_entry_p)	= 1;

    spe = ((rgy_res_attr_sch_entry_t *)
	(((char *) &rgy_res_attr_sch_entry[0]) + 
		sizeof(rgy_res_attr_sch_entry)));

    /* WARNING: this routine must be fixed to
     * free memory properly when more acl_mgr_types are
     * added to schema entries.
     */

    for (sp = &rgy_res_attr_sch_entry[0]; sp < spe; sp++) { 
	sch_entry_p->attr_name = (sec_attr_component_name_t)sp->attr_name;
	uuid_from_string((unsigned char *)(sp->attr_id), 
		&(sch_entry_p->attr_id), st_p);
	sch_entry_p->attr_encoding	= sp->attr_encoding;

	uuid_from_string((unsigned char *)(sp->acl_mgr_type), 
	    &(SA_ACL_MGR_TYPE(sch_entry_p, 0)), st_p);
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
    }

    sec_rgy_attr_sch_create_entry(write_context, sec_master_site_name, sch_entry_p, st_p);
    CHECK_RESULT(TRUE, "sec_rgy_attr_sch_create_entry()", st_p, error_status_ok);

    SEC_RGY_SITE_SYNCH(write_context);
}
static void
Delete_Schema_Entry()	
{
    sec_attr_schema_entry_t	*sch_entry_p;
    rgy_res_attr_sch_entry_t	*sp, *spe;
    unsigned32			length;
    uuid_t	 		*attr_id;
    
    CLEAR_STATUS(st_p);
    if(!(sch_entry_p = (sec_attr_schema_entry_t *)malloc(
		sizeof(sec_attr_schema_entry_t)))) {
	SET_STATUS(st_p, sec_rgy_cant_allocate_memory);
        CHECK_RESULT(FALSE, "malloc()", st_p, error_status_ok);
    }
    /* Assuming one acl_mgr for now */
    ATTR_SCH_MALLOC(SA_ACL_MGR_SET_P(sch_entry_p),
	SA_ACL_MGR_SET_SIZE(1), st_p);
    ATTR_SCH_MALLOC(SA_ACL_MGR_INFO_P(sch_entry_p, 0),
	sizeof(*(SA_ACL_MGR_INFO_P(sch_entry_p, 0))), st_p);
    SA_ACL_MGR_NUM(sch_entry_p)	= 1;

    spe = ((rgy_res_attr_sch_entry_t *)
	(((char *) &rgy_res_attr_sch_entry[0]) + 
		sizeof(rgy_res_attr_sch_entry)));

    /* WARNING: this routine must be fixed to
     * free memory properly when more acl_mgr_types are
     * added to schema entries.
     */

    for (sp = &rgy_res_attr_sch_entry[0]; sp < spe; sp++) { 
	sch_entry_p->attr_name = (sec_attr_component_name_t)sp->attr_name;
	uuid_from_string((unsigned char *)(sp->attr_id), 
		&(sch_entry_p->attr_id), st_p);
    }

    sec_rgy_attr_sch_delete_entry(write_context, sec_master_site_name, 
	&(sch_entry_p->attr_id), st_p);
    CHECK_RESULT(TRUE, "sec_rgy_attr_sch_delete_entry()", st_p, error_status_ok);

    SEC_RGY_SITE_SYNCH(write_context);
}

static void
tp1()	/* successful test for sec_rgy_attr_sch_create_entry */
{

    sec_attr_schema_entry_t	*sch_entry_p;
    rgy_res_attr_sch_entry_t	*sp, *spe;
    unsigned32			length;

    tet_infoline("test case (tp1)");

    CLEAR_STATUS(st_p);
    Create_Schema_Entry();
    tet_result(TET_PASS);
}

static void
tp2()	/* successful test for sec_rgy_attr_sch_lookup_by_id */
{
    sec_attr_schema_entry_t	sch_entry;
    uuid_t	 		attr_id;
    
    CLEAR_STATUS(st_p);
    tet_infoline("test case (tp2)");
    uuid_from_string((unsigned char *)ATTR_ID, &(attr_id), st_p);
    sec_rgy_attr_sch_lookup_by_id(read_context, sec_replica_site_name, &attr_id,
	&sch_entry, st_p);
    CHECK_RESULT(TRUE, "sec_rgy_attr_sch_lookup_by_id()", st_p, error_status_ok);
    tet_result(TET_PASS);
}

static void
tp3()	/* successful test for sec_rgy_attr_sch_lookup_by_name */
{
    sec_attr_schema_entry_t	schema_entry;
    
    CLEAR_STATUS(st_p);
    tet_infoline("test case (tp3)");
    sec_rgy_attr_sch_lookup_by_name( read_context, sec_replica_site_name,
	(unsigned char *) ATTR_NAME, &schema_entry, st_p);
    CHECK_RESULT(TRUE, "sec_rgy_attr_sch_lookup_by_name()", st_p, error_status_ok);
    tet_result(TET_PASS);
}


static void
tp4()	/* successful test for sec_rgy_attr_sch_cursor_init */
{
    unsigned32				cur_num_entries;
    
    CLEAR_STATUS(st_p);
    tet_infoline("test case (tp4)");
    sec_rgy_attr_sch_cursor_init(read_context, sec_replica_site_name,
		&cur_num_entries, &cursor, st_p);
    CHECK_RESULT(TRUE, "sec_rgy_attr_sch_cursor_init()", st_p, error_status_ok);
    tet_result(TET_PASS);

}

static void
tp5()	/* successful test for sec_rgy_attr_sch_scan */
{
    unsigned32			num_to_read=1;
    unsigned32			num_read;
    sec_attr_schema_entry_t	schema_entries[1];
    
    CLEAR_STATUS(st_p);
    tet_infoline("test case (tp5)");
    sec_rgy_attr_sch_scan( read_context, sec_replica_site_name, &cursor,
		 num_to_read, &num_read, schema_entries, st_p);
    CHECK_RESULT(TRUE, "sec_rgy_attr_sch_scan()", st_p, error_status_ok);
    tet_result(TET_PASS);
}

static void
tp6()	/* successful test for sec_rgy_attr_sch_cursor_reset */
{
    CLEAR_STATUS(st_p);
    tet_infoline("test case (tp6)");
    sec_rgy_attr_sch_cursor_reset( &cursor, st_p);
    CHECK_RESULT(TRUE, "sec_rgy_attr_sch_cursor_reset()", st_p, error_status_ok);
    tet_result(TET_PASS);
}

static void
tp7()	/* successful test for sec_rgy_attr_sch_cursor_release */
{
    
    CLEAR_STATUS(st_p);
    tet_infoline("test case (tp7)");
    sec_rgy_attr_sch_cursor_release(&cursor, st_p);
    CHECK_RESULT(TRUE, "sec_rgy_attr_sch_cursor_release()", st_p, error_status_ok);
    tet_result(TET_PASS);
}

static void
tp8()	/* successful test for sec_rgy_attr_sch_cursor_alloc */
{
    CLEAR_STATUS(st_p);
    tet_infoline("test case (tp8)");
    sec_rgy_attr_sch_cursor_alloc( &cursor, st_p);
    CHECK_RESULT(TRUE, "sec_rgy_attr_sch_cursor_alloc()", st_p, error_status_ok);
    tet_result(TET_PASS);
    /* Don't bother to free the resources ( resources allocated are local since
     * no call is made to a server ). And the local resources will be freed when 
     * the test exits.
     */
}

static void
tp9()	/* successful test for sec_rgy_attr_sch_get_acl_mgrs */
{
    unsigned32              size_avail=1;
    unsigned32              size_used;
    unsigned32              num_acl_mgr_types;
    uuid_t                  *acl_mgr_types_p;

    CLEAR_STATUS(st_p);
    tet_infoline("test case (tp9)");
    if(!(acl_mgr_types_p = (uuid_t *) malloc(sizeof(uuid_t) * 1))) {
	SET_STATUS(st_p, sec_rgy_cant_allocate_memory);
        CHECK_RESULT(TRUE, "malloc()", st_p, error_status_ok);
    }

    sec_rgy_attr_sch_get_acl_mgrs(read_context, sec_replica_site_name,
		size_avail, &size_used, &num_acl_mgr_types, acl_mgr_types_p, st_p);
    CHECK_RESULT(TRUE, "sec_rgy_attr_sch_get_acl_mgrs()", st_p, sec_attr_not_implemented);

    tet_result(TET_PASS);
}

static void
tp10()	/* successful test for sec_rgy_attr_sch_get_aclmgr_strings */
{
    unsigned32                  size_avail=1;
    unsigned32                  size_used;
    unsigned32                  num_acl_mgr_types;
    uuid_t                      acl_mgr_type_chain ;
    sec_acl_printstring_t       acl_mgr_info ;
    sec_acl_printstring_t       *permstrings_p ;
    boolean32                   tokenize ;
    unsigned32                  num_printstrings ;
    uuid_t                      acl_mgr_types;

    CLEAR_STATUS(st_p);
    tet_infoline("test case (tp10)");
    if(!(permstrings_p = (sec_acl_printstring_t *) malloc(sizeof(sec_acl_printstring_t) * size_avail))) {
	SET_STATUS(st_p, sec_rgy_cant_allocate_memory);
        CHECK_RESULT(TRUE, "malloc()", st_p, error_status_ok);
    }

    uuid_from_string(ACL_MGR_TYPE_PERSON, &acl_mgr_types, st_p);
 
    sec_rgy_attr_sch_aclmgr_strings(read_context, sec_replica_site_name,
		&acl_mgr_types,
                size_avail,
                &acl_mgr_type_chain,
                &acl_mgr_info,
                &tokenize,
                &num_printstrings,
                &size_used,
                permstrings_p,
                st_p);
    CHECK_RESULT(TRUE, "sec_rgy_attr_sch_aclmgr_strings()", st_p, sec_attr_not_implemented);

    tet_result(TET_PASS);
}


static void
tp11()	/* successful test for sec_rgy_attr_sch_update_entry */
{
    sec_attr_schema_entry_t	*sch_entry_p;
    rgy_res_attr_sch_entry_t	*sp, *spe;
    unsigned32			length;
    uuid_t	 		*attr_id;
    sec_attr_schema_entry_parts_t	modify_parts;
    
    CLEAR_STATUS(st_p);
    tet_infoline("test case (tp11)");
    CLEAR_STATUS(st_p);
    if(!(sch_entry_p = (sec_attr_schema_entry_t *)malloc(
		sizeof(sec_attr_schema_entry_t)))) {
	SET_STATUS(st_p, sec_rgy_cant_allocate_memory);
        CHECK_RESULT(TRUE, "malloc()", st_p, error_status_ok);
    }
    /* Assuming one acl_mgr for now */
    ATTR_SCH_MALLOC(SA_ACL_MGR_SET_P(sch_entry_p),
	SA_ACL_MGR_SET_SIZE(1), st_p);
    ATTR_SCH_MALLOC(SA_ACL_MGR_INFO_P(sch_entry_p, 0),
	sizeof(*(SA_ACL_MGR_INFO_P(sch_entry_p, 0))), st_p);
    SA_ACL_MGR_NUM(sch_entry_p)	= 1;

    spe = ((rgy_res_attr_sch_entry_t *)
	(((char *) &rgy_res_attr_sch_entry[0]) + 
		sizeof(rgy_res_attr_sch_entry)));

    /* WARNING: this routine must be fixed to
     * free memory properly when more acl_mgr_types are
     * added to schema entries.
     */

    for (sp = &rgy_res_attr_sch_entry[0]; sp < spe; sp++) { 
	sch_entry_p->attr_name = (sec_attr_component_name_t)sp->attr_name;
	uuid_from_string((unsigned char *)(sp->attr_id), 
		&(sch_entry_p->attr_id), st_p);
        /* test! */
        sch_entry_p->attr_encoding      = sp->attr_encoding;

        uuid_from_string((unsigned char *)(sp->acl_mgr_type),
            &(SA_ACL_MGR_TYPE(sch_entry_p, 0)), st_p);
        SA_ACL_MGR_PERMS_QUERY(sch_entry_p, 0)  = sp->query_permset;
        SA_ACL_MGR_PERMS_UPDATE(sch_entry_p, 0) = sp->update_permset;
        SA_ACL_MGR_PERMS_TEST(sch_entry_p, 0)   = sp->test_permset;
        SA_ACL_MGR_PERMS_DELETE(sch_entry_p, 0) = sp->delete_permset;

        sch_entry_p->schema_entry_flags   = sp->schema_entry_flags;
        sch_entry_p->intercell_action           = sp->intercell_action;
        sch_entry_p->trig_types                 = sp->trig_types;

        /* Skip trig_binding for now */
        sch_entry_p->trig_binding       = NULL;
        sch_entry_p->scope              = (unsigned_char_p_t)(sp->scope);
        sch_entry_p->comment            = (unsigned_char_p_t)(sp->comment);
    }
    modify_parts = sec_attr_schema_part_acl_mgrs |
                   sec_attr_schema_part_scope;
    sec_rgy_attr_sch_update_entry(write_context, sec_master_site_name,
		modify_parts, sch_entry_p, st_p);
    CHECK_RESULT(TRUE, "sec_rgy_attr_sch_update_entry()", st_p, error_status_ok);

    SEC_RGY_SITE_SYNCH(write_context);

    tet_result(TET_PASS);
}

static void
tp12()	/* successful test for sec_rgy_attr_sch_delete_entry */
{
    sec_attr_schema_entry_t	*sch_entry_p;
    rgy_res_attr_sch_entry_t	*sp, *spe;
    unsigned32			length;
    uuid_t	 		*attr_id;
    
    tet_infoline("test case (tp12)");
    Delete_Schema_Entry();
    tet_result(TET_PASS);
}
