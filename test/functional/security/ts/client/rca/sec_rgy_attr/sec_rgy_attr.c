/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_rgy_attr.c,v $
 * Revision 1.1.12.1  1996/06/04  22:09:13  arvind
 * 	Fix compilation errors caught by AIX ansi compiler
 * 	[1995/12/13  18:40 UTC  dat  /main/HPDCE02/jrr_1.2_mothra/1]
 *
 * 	[1994/12/05  21:08 UTC  mullan_s  /main/HPDCE02/1]
 *
 * 	Remove sec_attr_sch_entry_reserved flag from schema so that it can be deleted.
 * 	[1994/12/05  21:05 UTC  mullan_s  /main/mullan_mothra_schema_fixes/1]
 *
 * Revision 1.1.10.3  1996/02/17  23:11:47  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:20  marty]
 * 
 * Revision 1.1.10.2  1995/12/11  19:53:25  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/04/29  16:02 UTC  mdf
 * 	Can't have multi_instanced void attributes.
 * 	[1995/04/29  14:22 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_3/1]
 * 
 * 	HP revision /main/HPDCE02/2  1995/04/06  16:58 UTC  mdf
 * 	Resolution of CHFts14769
 * 	[1995/04/05  14:45 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_2/1]
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/05  21:08 UTC  mullan_s
 * 
 * 	HP revision /main/mullan_mothra_schema_fixes/1  1994/12/05  21:05 UTC  mullan_s
 * 	Remove sec_attr_sch_entry_reserved flag from schema so that it can be deleted.
 * 	[1995/12/11  19:27:14  root]
 * 
 * Revision 1.1.5.2  1994/10/27  20:07:36  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:39:57  rrizzo]
 * 
 * Revision 1.1.5.1  1994/10/25  17:17:47  sekhar
 * 	[ OT 12398 ]  Checking in Sean Mullan's changes for testing
 * 		      security replication.
 * 	[1994/10/24  20:23:01  sekhar]
 * 
 * 	HP revision /main/mullan_mothra_test_repl_cov/3  1994/09/28  15:09 UTC  mullan_s
 * 	More replica macro changes.
 * 
 * 	HP revision /main/mullan_mothra_test_repl_cov/2  1994/09/13  16:54 UTC  mullan_s
 * 	Move SEC_RGY_SITE_OPEN calls to tet_startup.
 * 
 * 	HP revision /main/mullan_mothra_test_repl_cov/1  1994/09/12  18:37 UTC  mullan_s
 * 	Add replication testing.
 * 
 * Revision 1.1.2.1  1994/08/04  16:16:00  mdf
 * 	Initial checkin
 * 	[1994/08/03  19:19:42  mdf]
 * 
 * $EndLog$
 */
/*
 * @OSF_COPYRIGHT@
 */
/* 
 * Goal here is to provide the tests needed to verify that
 * the functions within sec_rgy_attr.c are working correctly.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <varargs.h>

#include <tet_api.h>
#include <ts-common.h>

#include <rgymacro.h>

#include <dce/binding.h>
#include <dce/dce_error.h>
#include <dce/sec_attr_tools.h>

#include <rsdb_acl.h>
#include <sec_def_flags.h>

#include <dce/sec_rgy_attr_sch.h>
#include <dce/sec_rgy_attr.h>
#include <dce/rs_attr_schema.h>

static void tp1(), tp2(), tp3(), tp4();
static void tp5(), tp6(), tp7(), tp8();
static void tp9(), tp10(), tp11(), tp12();
static void tp20(), tp21();
static void sec_rgy_attr_startup(void);
static void sec_rgy_attr_cleanup(void);
#define MAX_ATTRS 20

/* 
 * Initialize TCM data structures 
 */

void (*tet_startup)() = sec_rgy_attr_startup;
void (*tet_cleanup)() = sec_rgy_attr_cleanup;

struct tet_testlist tet_testlist[] = {
    {tp1, 1},	/* test_integer */
    {tp2, 2},	/* test_void */
    {tp3, 3},	/* test_any */
    {tp4, 4},	/* test_printstring */
    {tp5, 5},	/* test_printstring_array */
    {tp6, 6},	/* test_bytes */
    {tp7, 7}, 	/* test_confidential_bytes */
    {tp8, 8},	/* test_i18n_data */
    {tp9, 9},	/* test_uuid */
    {tp10, 10},	/* test_attr_set */
    {tp11, 11},	/* test_binding */
    {tp12, 12},	/* test all previous tests as one */
    {tp20, 20},	/* successful test of sec_rgy_attr_lookup_by_id */
    {tp21, 21},	
    {NULL, 0}
};

/*
 * Index into the test_uuids array with enum names.
 */
enum class { 
	test_any, 
	test_void, 
	test_printstring, 
	test_printstring_array, 
	test_integer, 
	test_bytes, 
	test_confidential_bytes, 
	test_i18n_data,
	test_uuid, 
	test_attr_set, 
	test_binding, 
	testcase1,
	testcase2
} TEST_TYPE;

char *test_uuids[] = {
    "5bbe88d6-2af1-11cd-8475-080009353559",	/* test_any 		   */
    "5d1b6546-2af1-11cd-9bf5-080009353559",	/* test_void 		   */
    "5e2321ae-2af1-11cd-a742-080009353559",	/* test_printstring 	   */
    "dc71e3dc-2af2-11cd-a8bd-080009353559",	/* test_printstring_array  */
    "5f439154-2af1-11cd-8ec3-080009353559",	/* test_integer 	   */
    "60734ee8-2af1-11cd-8a04-080009353559",	/* test_bytes 	 	   */
    "61db5bb8-2af1-11cd-8286-080009353559",	/* test_confidential_bytes */
    "633378d8-2af1-11cd-8a76-080009353559",	/* test_i18n_data 	   */
    "64768168-2af1-11cd-9b8c-080009353559",	/* test_uuid 	   	   */
    "87bbe4d2-2af2-11cd-95a4-080009353559",	/* test_attr_set 	   */
    "886f4950-2af2-11cd-baa2-080009353559",	/* test_binding 	   */
    "b6b40696-5182-11cd-a501-0800092784c3",	/* testcase1 	   	   */
    "d268c3a2-51f7-11cd-a095-0800092784c3"	/* testcase2 	 	   */
};


/*
 * Test Case Wide Declarations 
 */

char 			msg[1024];		/* buffer for info lines */
sec_rgy_handle_t	read_context, write_context;
sec_attr_cursor_t	cursor;

#define ATTR_SCH_MALLOC(P,L,STP) 						\
{										\
    if(!((P) = malloc(L))) {                            			\
        SET_STATUS(STP, sec_rgy_cant_allocate_memory);  			\
        CHECK_RESULT(FALSE, "ATTR_SCH_MALLOC()", STP, error_status_ok);		\
    } else {                                            			\
        memset((P), NULL, (L));                         			\
    }										\
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

#define  ATTR_NAME "_Attribute_Schema_Test_"
#define  ATTR_ID "8992aa8e-2151-11cd-882b-0800097086d2"

rgy_res_attr_sch_entry_t   rgy_res_attr_sch_entry[] = {
{ "test_any",				"5bbe88d6-2af1-11cd-8475-080009353559",
  sec_attr_enc_any,			"06ab9320-0191-11ca-a9e8-08001e039d7d",
  sec_acl_perm_read,		sec_acl_perm_read,
  sec_acl_perm_read,		sec_acl_perm_read,
  ( sec_attr_sch_entry_multi_inst | sec_attr_sch_entry_use_defaults),
  sec_attr_intercell_act_reject,	sec_attr_trig_type_none,
  NULL,					"",
  "test_any: encoding type any" },
{ "test_void",				"5d1b6546-2af1-11cd-9bf5-080009353559",
  sec_attr_enc_void,			"06ab9320-0191-11ca-a9e8-08001e039d7d",
  sec_acl_perm_read,		sec_acl_perm_read,
  sec_acl_perm_read,		sec_acl_perm_read,
  ( sec_attr_sch_entry_use_defaults),
  sec_attr_intercell_act_reject,	sec_attr_trig_type_none,
  NULL,					"",
  "test_void: encoding type void" },
{ "test_printstring",			"5e2321ae-2af1-11cd-a742-080009353559",
  sec_attr_enc_printstring,		"06ab9320-0191-11ca-a9e8-08001e039d7d",
  sec_acl_perm_read,		sec_acl_perm_read,
  sec_acl_perm_read,		sec_acl_perm_read,
  ( sec_attr_sch_entry_multi_inst | sec_attr_sch_entry_use_defaults),
  sec_attr_intercell_act_reject,	sec_attr_trig_type_none,
  NULL,					"",
  "test_printstring: encoding type printstring" },
{ "test_printstring_array",		"dc71e3dc-2af2-11cd-a8bd-080009353559",
  sec_attr_enc_printstring_array,	"06ab9320-0191-11ca-a9e8-08001e039d7d",
  sec_acl_perm_read,		sec_acl_perm_read,
  sec_acl_perm_read,		sec_acl_perm_read,
  ( sec_attr_sch_entry_multi_inst | sec_attr_sch_entry_use_defaults),
  sec_attr_intercell_act_reject,	sec_attr_trig_type_none,
  NULL,					"",
  "test_printstring_array: encoding type printstring_array" },
{ "test_integer",			"5f439154-2af1-11cd-8ec3-080009353559",
  sec_attr_enc_integer,			"06ab9320-0191-11ca-a9e8-08001e039d7d",
  sec_acl_perm_read,		sec_acl_perm_read,
  sec_acl_perm_read,		sec_acl_perm_read,
  ( sec_attr_sch_entry_multi_inst | sec_attr_sch_entry_use_defaults),
  sec_attr_intercell_act_reject,	sec_attr_trig_type_none,
  NULL,					"",
  "test_integer: encoding type integer" },
{ "test_bytes",				"60734ee8-2af1-11cd-8a04-080009353559",
  sec_attr_enc_bytes,			"06ab9320-0191-11ca-a9e8-08001e039d7d",
  sec_acl_perm_read,		sec_acl_perm_read,
  sec_acl_perm_read,		sec_acl_perm_read,
  ( sec_attr_sch_entry_multi_inst | sec_attr_sch_entry_use_defaults),
  sec_attr_intercell_act_reject,	sec_attr_trig_type_none,
  NULL,					"",
  "test_bytes: encoding type bytes" },
{ "test_confidential_bytes",		"61db5bb8-2af1-11cd-8286-080009353559",
  sec_attr_enc_confidential_bytes,	"06ab9320-0191-11ca-a9e8-08001e039d7d",
  sec_acl_perm_read,		sec_acl_perm_read,
  sec_acl_perm_read,		sec_acl_perm_read,
  ( sec_attr_sch_entry_multi_inst | sec_attr_sch_entry_use_defaults),
  sec_attr_intercell_act_reject,	sec_attr_trig_type_none,
  NULL,					"",
  "test_confidential_bytes: encoding type confidential_bytes" },
{ "test_i18n_data",			"633378d8-2af1-11cd-8a76-080009353559",
  sec_attr_enc_i18n_data,		"06ab9320-0191-11ca-a9e8-08001e039d7d",
  sec_acl_perm_read,		sec_acl_perm_read,
  sec_acl_perm_read,		sec_acl_perm_read,
  ( sec_attr_sch_entry_multi_inst | sec_attr_sch_entry_use_defaults),
  sec_attr_intercell_act_reject,	sec_attr_trig_type_none,
  NULL,					"",
  "test_i18n_data: encoding type i18n_data" },
{ "test_uuid",				"64768168-2af1-11cd-9b8c-080009353559",
  sec_attr_enc_uuid,			"06ab9320-0191-11ca-a9e8-08001e039d7d",
  sec_acl_perm_read,		sec_acl_perm_read,
  sec_acl_perm_read,		sec_acl_perm_read,
  ( sec_attr_sch_entry_multi_inst | sec_attr_sch_entry_use_defaults),
  sec_attr_intercell_act_reject,	sec_attr_trig_type_none,
  NULL,					"",
  "test_uuid: encoding type uuid" },
{ "test_attr_set",			"87bbe4d2-2af2-11cd-95a4-080009353559",
  sec_attr_enc_attr_set,		"06ab9320-0191-11ca-a9e8-08001e039d7d",
  sec_acl_perm_read,		sec_acl_perm_read,
  sec_acl_perm_read,		sec_acl_perm_read,
  ( sec_attr_sch_entry_multi_inst | sec_attr_sch_entry_use_defaults),
  sec_attr_intercell_act_reject,	sec_attr_trig_type_none,
  NULL,					"",
  "test_attr_set: encoding type attr_set" },
{ "test_binding",			"886f4950-2af2-11cd-baa2-080009353559",
  sec_attr_enc_binding,			"06ab9320-0191-11ca-a9e8-08001e039d7d",
  sec_acl_perm_read,		sec_acl_perm_read,
  sec_acl_perm_read,		sec_acl_perm_read,
  ( sec_attr_sch_entry_multi_inst | sec_attr_sch_entry_use_defaults),
  sec_attr_intercell_act_reject,	sec_attr_trig_type_none,
  NULL,					"",
  "test_binding: encoding type binding" },
{ ATTR_NAME, ATTR_ID,
  sec_attr_enc_integer,                 
  "06ab9320-0191-11ca-a9e8-08001e039d7d",	/* person from rsdb_acl.c */
  sec_acl_perm_mgmt_info,              sec_acl_perm_mgmt_info,
  sec_acl_perm_mgmt_info,              sec_acl_perm_mgmt_info,
  sec_attr_sch_entry_use_defaults,
  sec_attr_intercell_act_reject,       sec_attr_trig_type_none,
  NULL,                                "",
  "values: {0=ENFORCE-BY-THIRD-PARTY, 1=ENFORCE-BY-REGISTRY}" }
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
    CHECK_RESULT(FALSE, "sec_rgy_attr_sch_create_entry()", &st, error_status_ok);

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

static void
attr_update
(
	char 		*testcase,
	unsigned32	num_attrs,
	sec_attr_t	*attrs,
	error_status_t	return_expected
)
{
    char	 	buf[512];
    signed32		failure_index;
    unsigned32 		num_ret, num_left;
    sec_attr_t		out_attrs[MAX_ATTRS];
    int			i;
    error_status_t		st;

    sec_rgy_attr_update(write_context, sec_rgy_domain_person, 
        (unsigned_char_p_t)"root", num_attrs, num_attrs, attrs, &num_ret,
        out_attrs, &num_left, &failure_index, &st);
    sprintf(buf, "sec_rgy_attr_update() %s", testcase);
    CHECK_RESULT(TRUE, buf, &st, return_expected);

    SEC_RGY_SITE_SYNCH(write_context);
}
static void
compare_attrs
(
    sec_attr_t	*in_attrs,
    sec_attr_t  *out_attrs,
    unsigned32  num_attrs,
    error_status_t *st_p
)
{
    unsigned32 i, j, ignore, skip=0, match=0;
    unsigned32 *matched;

    CLEAR_STATUS(st_p);
    matched = malloc(num_attrs * sizeof(unsigned32));
    memset(matched, NULL, num_attrs * sizeof(unsigned32));


/*
 * Caution: for the memcmp to work, it is assumed that all malloc'd 
 * 	    memory, is memset to NULL.  If the rsdb routines no longer
 *	    memset to NULL, a different method will be needed to compare
 *	    attributes.
 */
	for (i=0; i < num_attrs; i++) {
	    for (j=0; j < num_attrs; j++) {
		if ((memcmp(&in_attrs[i], &out_attrs[j],
		    sizeof(&in_attrs[i]))) == 0) {
		    skip = 0;
		    for (ignore=0; ignore < match; ignore++) {
			if (j == matched[ignore]) 
			    skip = 1;
		    }
		    if ( skip == 0 )
		        matched[match++] = j;
		}
	    }
	}
	if (match != num_attrs) {
	    printf("matched %d, num_attrs %d\n", match, num_attrs);
	    SET_STATUS(st_p, -1);
	}
}

static void
verify_attr_update
(
    char 		*testcase,
    unsigned32		num_attr_keys,
    sec_attr_t		*attr_keys,
    error_status_t	return_expected,	/* used for comparisons */
    error_status_t	*st_p
)
{
    char	 	buf[512];
    char	 	error_buf[512];
    sec_attr_cursor_t	cursor;

    unsigned32		num_ret, num_left;
    unsigned32		space_avail=MAX_ATTRS;
    sec_attr_t		out_attrs[MAX_ATTRS];
    int			i;
    error_status_t      st;

    sec_rgy_attr_cursor_alloc(&cursor, st_p);
    sprintf (buf, "sec_rgy_attr_cursor_alloc() ");
    CHECK_RESULT(FALSE, buf, st_p, error_status_ok);

    sec_rgy_attr_lookup_by_id(read_context, sec_rgy_domain_person,
	(unsigned_char_p_t)"root", 
	&cursor, num_attr_keys, MAX_ATTRS, attr_keys,
	&num_ret, out_attrs, &num_left, st_p);
    sprintf (buf, "sec_rgy_attr_lookup_by_id() %s.", testcase);
    CHECK_RESULT(TRUE, buf, st_p, error_status_ok);
    sprintf (buf, "verification of %s.", testcase);
    if (num_attr_keys != num_ret) {
        sprintf (error_buf, "num_attr_keys to be equal to num_returned.");
        mtet_infoline("FAILED %s\nexpected: \t%s\n\n", buf, error_buf); 
	SET_STATUS(st_p, -1); 	/* internal bad status */
	return;
    } else {
	compare_attrs(attr_keys, out_attrs, num_attr_keys, st_p);
        if (BAD_STATUS(st_p)) {
	    sprintf (error_buf, "I/O mismatch.");
            mtet_infoline("FAILED %s\nexpected: \t%s\n\n", buf, error_buf); 
	    return;
	}
    }
    mtet_infoline("PASSED %s\n", buf);
}

static void
attr_lookup 
(
	int		index,
	char		*dspmsg,
	unsigned32	number
)
{
    unsigned32 		num_left = 0;
    unsigned32 		num_attr_keys = number;
    unsigned32 		num_attrs = 0;
    error_status_t		st;
    sec_attr_t 		*attr_keys;
    sec_attr_t 		*attrs;
    sec_attr_cursor_t	cursor;
    char		buf[512];
    unsigned_char_t 	*string;


    CLEAR_STATUS(&st);

    attrs = (sec_attr_t *) malloc(MAX_ATTRS * sizeof(sec_attr_t));
    memset(attrs, NULL, MAX_ATTRS * sizeof(sec_attr_t));
    if (number) {
	attr_keys = (sec_attr_t *) malloc(number * sizeof(sec_attr_t));
    	memset(attr_keys, NULL, number * sizeof(sec_attr_t));
    }
    sec_rgy_attr_cursor_alloc(&cursor, &st);
    sprintf (buf, "sec_rgy_attr_cursor_alloc() ");
    CHECK_RESULT(FALSE, buf, &st, error_status_ok);

    if (number) {
        uuid_from_string(test_uuids[index], &(attr_keys[0].attr_id), &st);
	SA_ATTR_ENCODING(&attr_keys[0]) = sec_attr_enc_integer;
	SA_ATTR_INTEGER(&attr_keys[0]) = 042666;
    }

    sec_rgy_attr_lookup_by_id(read_context, sec_rgy_domain_person,
	(unsigned_char_p_t)"root", 
	&cursor, num_attr_keys, MAX_ATTRS, attr_keys,
	&num_attrs, attrs, &num_left, &st);
    sprintf (buf, "sec_rgy_attr_lookup_by_id() %s.", dspmsg);
    CHECK_RESULT(TRUE, buf, &st, error_status_ok);

    sec_rgy_attr_cursor_release(&cursor, &st);
    sprintf (buf, "sec_rgy_attr_cursor_release()");
    CHECK_RESULT(FALSE, "sec_rgy_attr_cursor_release()", &st, error_status_ok);
}

static void
sec_rgy_attr_startup(void)
{
    SEC_RGY_SITE_OPEN_WRITE(&write_context);
    SEC_RGY_SITE_OPEN_READ(&read_context);
    Create_Schema_Entry();
}

static void
sec_rgy_attr_cleanup(void)
{
    error_status_t st;

    Delete_Schema_Entry();
    sec_rgy_site_close(write_context, &st);
    sec_rgy_site_close(read_context, &st);
}

static void
tp1() /* test_integer */
{
    unsigned32	num_attrs=1;
    sec_attr_t 	attrs[1];
    error_status_t		st;
    uuid_from_string(test_uuids[test_integer], &(attrs[0].attr_id), &st);
    attrs[0].attr_value.attr_encoding = sec_attr_enc_integer;
    attrs[0].attr_value.tagged_union.signed_int = 92065;
    tet_infoline("test case (tp1)");
    attr_update("integer test", num_attrs, attrs, error_status_ok);
    verify_attr_update("integer test", num_attrs, attrs, error_status_ok, &st);
    tet_result(TET_PASS);
}
static void
tp2() /* test_void */
{
    unsigned32	num_attrs=1;
    sec_attr_t 	attrs[1];
    error_status_t		st;
    uuid_from_string(test_uuids[test_void], &(attrs[0].attr_id), &st);
    SA_ATTR_ENCODING(&attrs[0]) = sec_attr_enc_void;
    tet_infoline("test case (tp2)");
    attr_update("test_void", num_attrs, attrs, error_status_ok);
    verify_attr_update("test_void", num_attrs, attrs, error_status_ok, &st);
    tet_result(TET_PASS);
}
static void
tp3() /* test_any */
{
    unsigned32	num_attrs=1;
    sec_attr_t 	attrs[1];
    error_status_t		st;
    uuid_from_string(test_uuids[test_any], &(attrs[0].attr_id), &st);
    SA_ATTR_ENCODING(&attrs[0]) = sec_attr_enc_printstring;
    SA_ATTR_PRINTSTRING_P(&attrs[0]) = (unsigned_char_p_t)"Hewlett Packard";
    tet_infoline("test case (tp3)");
    attr_update("test_any", num_attrs, attrs, error_status_ok);
    verify_attr_update("test_any", num_attrs, attrs, error_status_ok, &st);
    tet_result(TET_PASS);
}

static void
tp4() /* test_printstring */
{
    unsigned32	num_attrs=1;
    sec_attr_t 	attrs[1];
    error_status_t		st;
    uuid_from_string(test_uuids[test_printstring], &(attrs[0].attr_id), &st);
    SA_ATTR_ENCODING(&attrs[0]) = sec_attr_enc_printstring;
    SA_ATTR_PRINTSTRING_P(&attrs[0]) = (unsigned_char_p_t)"Hewlett Packard";
    tet_infoline("test case (tp4)");
    attr_update("test_printstring", num_attrs, attrs, error_status_ok);
    verify_attr_update("test_printstring", num_attrs, attrs, error_status_ok, &st);
    tet_result(TET_PASS);
}

static void
tp5() /* test_printstring_array */
{
    unsigned32	num_attrs=1;
    sec_attr_t 	attrs[1];
    int i;
    error_status_t		st;
    uuid_from_string(test_uuids[test_printstring_array], 
		&(attrs[0].attr_id), &st);
    SA_ATTR_ENCODING(&attrs[0]) = sec_attr_enc_printstring_array;
    SA_ATTR_STR_ARRAY_P(&attrs[0]) =  malloc(SA_ATTR_STR_ARRAY_SIZE(4));
    memset(SA_ATTR_STR_ARRAY_P(&attrs[0]), NULL, SA_ATTR_STR_ARRAY_SIZE(4));
    for (i = 0 ; i < 4; i++) {
        SA_ATTR_STR_ARRAY_ELT_P(&attrs[0],i) =  malloc(24);
	memset(SA_ATTR_STR_ARRAY_ELT_P(&attrs[0],i), NULL, 24);
    }
    SA_ATTR_STR_ARRAY_NUM(&attrs[0]) = 4;
    SA_ATTR_STR_ARRAY_ELT_P(&attrs[0], 0)=
	(unsigned_char_p_t)"James B. Fairbrother";
    SA_ATTR_STR_ARRAY_ELT_P(&attrs[0], 1)=
	(unsigned_char_p_t)"Robert C. Fairbrother";
    SA_ATTR_STR_ARRAY_ELT_P(&attrs[0], 2)=
	(unsigned_char_p_t)"Michael D. Fairbrother";
    SA_ATTR_STR_ARRAY_ELT_P(&attrs[0], 3)=
	(unsigned_char_p_t)"Benjamin C. Fairbrother";
    tet_infoline("test case (tp5)");
    attr_update("test_printstring_array", num_attrs, attrs, error_status_ok);
    verify_attr_update("test_printstring_array", num_attrs,
	attrs, error_status_ok, &st);
    free(SA_ATTR_STR_ARRAY_P(&attrs[0]));
    tet_result(TET_PASS);
}
static void
tp6() /* test_bytes */
{
    unsigned32	num_attrs=1;
    sec_attr_t 	attrs[1];
    error_status_t		st;
    uuid_from_string(test_uuids[test_bytes], &(attrs[0].attr_id), &st);
    SA_ATTR_ENCODING(&attrs[0]) = sec_attr_enc_bytes;
    SA_ATTR_BYTES_P(&attrs[0]) = malloc(SA_ATTR_BYTES_SIZE(4));
    memset(SA_ATTR_BYTES_P(&attrs[0]), NULL, SA_ATTR_BYTES_SIZE(4));
    SA_ATTR_BYTES_LEN(&attrs[0]) = 4;
    SA_ATTR_BYTES_DATA(&attrs[0],0) = 1;
    SA_ATTR_BYTES_DATA(&attrs[0],1) = 2;
    SA_ATTR_BYTES_DATA(&attrs[0],2) = 3;
    SA_ATTR_BYTES_DATA(&attrs[0],3) = 4;
    tet_infoline("test case (tp6)");
    attr_update("test_bytes", num_attrs, attrs, error_status_ok);
    verify_attr_update("test_bytes", num_attrs, attrs, error_status_ok, &st);
    free (SA_ATTR_BYTES_P(&attrs[0]));
    tet_result(TET_PASS);
}
static void
tp7() /* test_confidential */
{
    unsigned32	num_attrs=1;
    sec_attr_t 	attrs[1];
    error_status_t		st;
    uuid_from_string(test_uuids[test_confidential_bytes],
	&(attrs[0].attr_id), &st);
    SA_ATTR_ENCODING(&attrs[0]) = sec_attr_enc_confidential_bytes;
    SA_ATTR_BYTES_P(&attrs[0]) = malloc(SA_ATTR_BYTES_SIZE(4));
    memset(SA_ATTR_BYTES_P(&attrs[0]), NULL, SA_ATTR_BYTES_SIZE(4));
    SA_ATTR_BYTES_LEN(&attrs[0]) = 4;
    SA_ATTR_BYTES_DATA(&attrs[0],0) = 1;
    SA_ATTR_BYTES_DATA(&attrs[0],1) = 2;
    SA_ATTR_BYTES_DATA(&attrs[0],2) = 3;
    SA_ATTR_BYTES_DATA(&attrs[0],3) = 4;
    tet_infoline("test case (tp7)");
    attr_update("test_confidential_bytes", num_attrs, attrs, error_status_ok);
    verify_attr_update("test_confidential_bytes", num_attrs,
	attrs, error_status_ok, &st);
    tet_result(TET_PASS);
}
static void
tp8() /* test_i18n */
{
    unsigned32	num_attrs=1;
    sec_attr_t 	attrs[1];
    error_status_t		st;
    uuid_from_string(test_uuids[test_i18n_data], &(attrs[0].attr_id), &st);
    SA_ATTR_ENCODING(&attrs[0]) = sec_attr_enc_i18n_data;
    SA_ATTR_IDATA_P(&attrs[0]) = malloc(SA_ATTR_IDATA_SIZE(1024));
    memset(SA_ATTR_IDATA_P(&attrs[0]), NULL, SA_ATTR_IDATA_SIZE(4));
    SA_ATTR_IDATA_CODESET(&attrs[0]) = 0x0011;
    SA_ATTR_IDATA_LEN(&attrs[0]) = 3;
    SA_ATTR_IDATA_DATA(&attrs[0],0) = 1;
    SA_ATTR_IDATA_DATA(&attrs[0],1) = 2;
    SA_ATTR_IDATA_DATA(&attrs[0],2) = 3;
    tet_infoline("test case (tp8)");
    attr_update("test_i18n_data", num_attrs, attrs, error_status_ok);
    verify_attr_update("test_i18n_data", num_attrs, attrs, error_status_ok, &st);
    tet_result(TET_PASS);
}

static void
tp9() /* test_uuids */
{
    unsigned32	num_attrs=1;
    sec_attr_t 	attrs[1];
    error_status_t		st;
    uuid_from_string(test_uuids[test_uuid], &(attrs[0].attr_id), &st);
    SA_ATTR_ENCODING(&attrs[0]) = sec_attr_enc_uuid;
    uuid_from_string(test_uuids[testcase1], &SA_ATTR_UUID(&attrs[0]), &st); 
    tet_infoline("test case (tp9)");
    attr_update("test_uuid", num_attrs, attrs, error_status_ok);
    verify_attr_update("test_uuid", num_attrs, attrs, error_status_ok, &st);
    tet_result(TET_PASS);
}

static void
tp10() /* test_attr_set */
{
    unsigned32	num_attrs=1;
    sec_attr_t 	attrs[1];
    int		length=2; 	/* I don't know pick a number */
    error_status_t		st;
    uuid_from_string(test_uuids[test_attr_set], &(attrs[0].attr_id), &st);
    SA_ATTR_ENCODING(&attrs[0]) = sec_attr_enc_attr_set;
    SA_ATTR_SET_P(&attrs[0]) = malloc (SA_ATTR_SET_SIZE(length));
    memset(SA_ATTR_SET_P(&attrs[0]), NULL, SA_ATTR_SET_SIZE(length));
    SA_ATTR_SET_NUM(&attrs[0]) = length;

    uuid_from_string(test_uuids[testcase1], 
		&SA_ATTR_SET_MEMBERS(&attrs[0], 0), &st);
    uuid_from_string(test_uuids[testcase2], 
		&SA_ATTR_SET_MEMBERS(&attrs[0], 1), &st);

    tet_infoline("test case (tp10)");
    attr_update("test_attr_set", num_attrs, attrs, error_status_ok);
    verify_attr_update("test_attr_set", num_attrs, attrs, error_status_ok, &st);
    tet_result(TET_PASS);
}
static void
tp11() /* test_binding */
{
    unsigned32	num_attrs=1;
    sec_attr_t 	attrs[1];
    int		length=3;
    int		name_len=32;
    int		twr_len=2;

    error_status_t		st;
    uuid_from_string(test_uuids[test_binding], &(attrs[0].attr_id), &st);
    SA_ATTR_ENCODING(&attrs[0]) = sec_attr_enc_binding;
    SA_ATTR_BND_INFO_P(&attrs[0]) = malloc(SA_BND_INFO_SIZE(length));
    memset(SA_ATTR_BND_INFO_P(&attrs[0]), NULL, SA_BND_INFO_SIZE(length));
    SA_ATTR_BND_NUM(&attrs[0]) = length;

    SA_ATTR_BND_AUTH_INFO_TYPE(&attrs[0]) = sec_attr_bind_auth_dce;
    SA_ATTR_BND_AUTH_SVR_PNAME_P(&attrs[0]) = malloc(name_len);
    memset(SA_ATTR_BND_AUTH_SVR_PNAME_P(&attrs[0]), NULL, name_len);
    SA_ATTR_BND_AUTH_SVR_PNAME_P(&attrs[0]) = (unsigned_char_p_t)"tst1";
    SA_ATTR_BND_AUTH_PROT_LEVEL(&attrs[0]) = 1001;
    SA_ATTR_BND_AUTH_AUTHN_SVC(&attrs[0]) = 11;
    SA_ATTR_BND_AUTH_AUTHZ_SVC(&attrs[0]) = 101;
    SA_ATTR_BND_TYPE(&attrs[0], 0) = sec_attr_bind_type_string;
    SA_ATTR_BND_STRING_P(&attrs[0], 0) = malloc(name_len);
    memset(SA_ATTR_BND_STRING_P(&attrs[0], 0), NULL, name_len);
    SA_ATTR_BND_STRING_P(&attrs[0], 0) = (unsigned_char_p_t)"tst2";

    SA_ATTR_BND_TYPE(&attrs[0], 1) = sec_attr_bind_type_twrs;
    SA_ATTR_BND_TWRSET_P(&attrs[0], 1) = malloc(SA_TWR_SET_SIZE(twr_len));
    memset(SA_ATTR_BND_TWRSET_P(&attrs[0], 1), NULL, SA_TWR_SET_SIZE(twr_len));
    SA_ATTR_BND_TWRSET_COUNT(&attrs[0], 1) = twr_len;
    SA_ATTR_BND_TWR_P(&attrs[0], 1, 0) = malloc(SA_TWR_SIZE(2));
    memset(SA_ATTR_BND_TWR_P(&attrs[0], 1, 0), NULL, SA_TWR_SIZE(2));
    SA_ATTR_BND_TWR_LEN(&attrs[0], 1, 0) = 2;
    SA_ATTR_BND_TWR_OCTETS(&attrs[0], 1, 0)[0] = 0;
    SA_ATTR_BND_TWR_OCTETS(&attrs[0], 1, 0)[1] = 1;

    SA_ATTR_BND_TWR_P(&attrs[0], 1, 1) = malloc(SA_TWR_SIZE(4));
    memset(SA_ATTR_BND_TWR_P(&attrs[0], 1, 1), NULL, SA_TWR_SIZE(4));
    SA_ATTR_BND_TWR_LEN(&attrs[0], 1, 1) = 4;
    SA_ATTR_BND_TWR_OCTETS(&attrs[0], 1, 1)[0] = 0;
    SA_ATTR_BND_TWR_OCTETS(&attrs[0], 1, 1)[1] = 1;
    SA_ATTR_BND_TWR_OCTETS(&attrs[0], 1, 1)[2] = 2;
    SA_ATTR_BND_TWR_OCTETS(&attrs[0], 1, 1)[3] = 3;

    SA_ATTR_BND_TYPE(&attrs[0], 2) = sec_attr_bind_type_svrname;
    SA_ATTR_BND_SVRNAME_P(&attrs[0], 2) =
	malloc(sizeof(*(SA_ATTR_BND_SVRNAME_P(&attrs[0], 2))));
    memset(SA_ATTR_BND_SVRNAME_P(&attrs[0], 2), NULL, sizeof(*(SA_ATTR_BND_SVRNAME_P(&attrs[0], 2))));
    SA_ATTR_BND_SVRNAME_SYNTAX(&attrs[0], 2) = 3;
    SA_ATTR_BND_SVRNAME_NAME_P(&attrs[0], 2) = malloc(name_len);
    memset(SA_ATTR_BND_SVRNAME_NAME_P(&attrs[0], 2), NULL, name_len);
    SA_ATTR_BND_SVRNAME_NAME_P(&attrs[0], 2) = (unsigned_char_p_t)"tst3";

    tet_infoline("test case (tp11)");
    attr_update("test_binding", num_attrs, attrs, error_status_ok);
    verify_attr_update("test_binding", num_attrs, attrs, error_status_ok, &st);
    tet_result(TET_PASS);
}

static void
tp12() /* kitchen sink */
{
    unsigned32		num_attrs=11;
    sec_attr_t 		attrs[11];
    error_status_t		st;
    int i; 

    tet_infoline("test case (tp12)");

    uuid_from_string(test_uuids[test_integer], &(attrs[0].attr_id), &st);
    attrs[0].attr_value.attr_encoding = sec_attr_enc_integer;
    attrs[0].attr_value.tagged_union.signed_int = 92065;

    uuid_from_string(test_uuids[test_void], &(attrs[1].attr_id), &st);
    SA_ATTR_ENCODING(&attrs[1]) = sec_attr_enc_void;

    uuid_from_string(test_uuids[test_any], &(attrs[2].attr_id), &st);
    SA_ATTR_ENCODING(&attrs[2]) = sec_attr_enc_printstring;
    SA_ATTR_PRINTSTRING_P(&attrs[2]) = (unsigned_char_p_t)"Hewlett Packard";

    uuid_from_string(test_uuids[test_printstring], &(attrs[3].attr_id), &st);
    SA_ATTR_ENCODING(&attrs[3]) = sec_attr_enc_printstring;
    SA_ATTR_PRINTSTRING_P(&attrs[3]) = (unsigned_char_p_t)"Hewlett Packard";

    uuid_from_string(test_uuids[test_printstring_array], 
		&(attrs[4].attr_id), &st);
    SA_ATTR_ENCODING(&attrs[4]) = sec_attr_enc_printstring_array;
    SA_ATTR_STR_ARRAY_P(&attrs[4]) =  malloc(SA_ATTR_STR_ARRAY_SIZE(4));
    memset(SA_ATTR_STR_ARRAY_P(&attrs[4]), NULL, SA_ATTR_STR_ARRAY_SIZE(4));
    for (i = 0 ; i < 4; i++) {
        SA_ATTR_STR_ARRAY_ELT_P(&attrs[4],i) =  malloc(24);
	memset(SA_ATTR_STR_ARRAY_ELT_P(&attrs[4],i), NULL, 24);
    }
    SA_ATTR_STR_ARRAY_NUM(&attrs[4]) = 4;
    SA_ATTR_STR_ARRAY_ELT_P(&attrs[4], 0)=
	(unsigned_char_p_t)"James B. Fairbrother";
    SA_ATTR_STR_ARRAY_ELT_P(&attrs[4], 1)=
	(unsigned_char_p_t)"Robert C. Fairbrother";
    SA_ATTR_STR_ARRAY_ELT_P(&attrs[4], 2)=
	(unsigned_char_p_t)"Michael D. Fairbrother";
    SA_ATTR_STR_ARRAY_ELT_P(&attrs[4], 3)=
	(unsigned_char_p_t)"Benjamin C. Fairbrother";

    uuid_from_string(test_uuids[test_bytes], &(attrs[5].attr_id), &st);
    SA_ATTR_ENCODING(&attrs[5]) = sec_attr_enc_bytes;
    SA_ATTR_BYTES_P(&attrs[5]) = malloc(SA_ATTR_BYTES_SIZE(4));
    memset(SA_ATTR_BYTES_P(&attrs[5]), NULL, SA_ATTR_BYTES_SIZE(4));
    SA_ATTR_BYTES_LEN(&attrs[5]) = 4;
    SA_ATTR_BYTES_DATA(&attrs[5],0) = 1;
    SA_ATTR_BYTES_DATA(&attrs[5],1) = 2;
    SA_ATTR_BYTES_DATA(&attrs[5],2) = 3;
    SA_ATTR_BYTES_DATA(&attrs[5],3) = 4;

    uuid_from_string(test_uuids[test_confidential_bytes],
	&(attrs[6].attr_id), &st);
    SA_ATTR_ENCODING(&attrs[6]) = sec_attr_enc_confidential_bytes;
    SA_ATTR_BYTES_P(&attrs[6]) = malloc(SA_ATTR_BYTES_SIZE(4));
    memset(SA_ATTR_BYTES_P(&attrs[6]), NULL, SA_ATTR_BYTES_SIZE(4));
    SA_ATTR_BYTES_LEN(&attrs[6]) = 4;
    SA_ATTR_BYTES_DATA(&attrs[6],0) = 1;
    SA_ATTR_BYTES_DATA(&attrs[6],1) = 2;
    SA_ATTR_BYTES_DATA(&attrs[6],2) = 3;
    SA_ATTR_BYTES_DATA(&attrs[6],3) = 4;

    uuid_from_string(test_uuids[test_i18n_data], &(attrs[7].attr_id), &st);
    SA_ATTR_ENCODING(&attrs[7]) = sec_attr_enc_i18n_data;
    SA_ATTR_IDATA_P(&attrs[7]) = malloc(SA_ATTR_IDATA_SIZE(1024));
    memset(SA_ATTR_IDATA_P(&attrs[7]), NULL, SA_ATTR_IDATA_SIZE(4));
    SA_ATTR_IDATA_CODESET(&attrs[7]) = 0x0011;
    SA_ATTR_IDATA_LEN(&attrs[7]) = 3;
    SA_ATTR_IDATA_DATA(&attrs[7],0) = 1;
    SA_ATTR_IDATA_DATA(&attrs[7],1) = 2;
    SA_ATTR_IDATA_DATA(&attrs[7],2) = 3;

    uuid_from_string(test_uuids[test_uuid], &(attrs[8].attr_id), &st);
    SA_ATTR_ENCODING(&attrs[8]) = sec_attr_enc_uuid;
    uuid_from_string(test_uuids[testcase1], &SA_ATTR_UUID(&attrs[8]), &st); 

    uuid_from_string(test_uuids[test_attr_set], &(attrs[9].attr_id), &st);
    SA_ATTR_ENCODING(&attrs[9]) = sec_attr_enc_attr_set;
    SA_ATTR_SET_P(&attrs[9]) = malloc (SA_ATTR_SET_SIZE(2));
    memset(SA_ATTR_SET_P(&attrs[9]), NULL, SA_ATTR_SET_SIZE(2));
    SA_ATTR_SET_NUM(&attrs[9]) = 2;

    uuid_from_string(test_uuids[testcase1], 
		&SA_ATTR_SET_MEMBERS(&attrs[9], 0), &st);
    uuid_from_string(test_uuids[testcase2], 
		&SA_ATTR_SET_MEMBERS(&attrs[9], 1), &st);

    uuid_from_string(test_uuids[test_binding], &(attrs[10].attr_id), &st);
    SA_ATTR_ENCODING(&attrs[10]) = sec_attr_enc_binding;
    SA_ATTR_BND_INFO_P(&attrs[10]) = malloc(SA_BND_INFO_SIZE(3));
    memset(SA_ATTR_BND_INFO_P(&attrs[10]), NULL, SA_BND_INFO_SIZE(3));
    SA_ATTR_BND_NUM(&attrs[10]) = 3;

    SA_ATTR_BND_AUTH_INFO_TYPE(&attrs[10]) = sec_attr_bind_auth_dce;
    SA_ATTR_BND_AUTH_SVR_PNAME_P(&attrs[10]) = malloc(32);
    memset(SA_ATTR_BND_AUTH_SVR_PNAME_P(&attrs[10]), NULL, 32);
    SA_ATTR_BND_AUTH_SVR_PNAME_P(&attrs[10]) = (unsigned_char_p_t)"tst1";
    SA_ATTR_BND_AUTH_PROT_LEVEL(&attrs[10]) = 1001;
    SA_ATTR_BND_AUTH_AUTHN_SVC(&attrs[10]) = 11;
    SA_ATTR_BND_AUTH_AUTHZ_SVC(&attrs[10]) = 101;
    SA_ATTR_BND_TYPE(&attrs[10], 0) = sec_attr_bind_type_string;
    SA_ATTR_BND_STRING_P(&attrs[10], 0) = malloc(32);
    memset(SA_ATTR_BND_STRING_P(&attrs[10], 0), NULL, 32);
    SA_ATTR_BND_STRING_P(&attrs[10], 0) = (unsigned_char_p_t)"tst2";

    SA_ATTR_BND_TYPE(&attrs[10], 1) = sec_attr_bind_type_twrs;
    SA_ATTR_BND_TWRSET_P(&attrs[10], 1) = malloc(SA_TWR_SET_SIZE(2));
    memset(SA_ATTR_BND_TWRSET_P(&attrs[10], 1), NULL, SA_TWR_SET_SIZE(2));
    SA_ATTR_BND_TWRSET_COUNT(&attrs[10], 1) = 2;
    SA_ATTR_BND_TWR_P(&attrs[10], 1, 0) = malloc(SA_TWR_SIZE(2));
    memset(SA_ATTR_BND_TWR_P(&attrs[10], 1, 0), NULL, SA_TWR_SIZE(2));
    SA_ATTR_BND_TWR_LEN(&attrs[10], 1, 0) = 2;
    SA_ATTR_BND_TWR_OCTETS(&attrs[10], 1, 0)[0] = 0;
    SA_ATTR_BND_TWR_OCTETS(&attrs[10], 1, 0)[1] = 1;

    SA_ATTR_BND_TWR_P(&attrs[10], 1, 1) = malloc(SA_TWR_SIZE(4));
    memset(SA_ATTR_BND_TWR_P(&attrs[10], 1, 1), NULL, SA_TWR_SIZE(4));
    SA_ATTR_BND_TWR_LEN(&attrs[10], 1, 1) = 4;
    SA_ATTR_BND_TWR_OCTETS(&attrs[10], 1, 1)[0] = 0;
    SA_ATTR_BND_TWR_OCTETS(&attrs[10], 1, 1)[1] = 1;
    SA_ATTR_BND_TWR_OCTETS(&attrs[10], 1, 1)[2] = 2;
    SA_ATTR_BND_TWR_OCTETS(&attrs[10], 1, 1)[3] = 3;

    SA_ATTR_BND_TYPE(&attrs[10], 2) = sec_attr_bind_type_svrname;
    SA_ATTR_BND_SVRNAME_P(&attrs[10], 2) =
	malloc(sizeof(*(SA_ATTR_BND_SVRNAME_P(&attrs[10], 2))));
    memset(SA_ATTR_BND_SVRNAME_P(&attrs[10], 2), NULL, 
		sizeof(*(SA_ATTR_BND_SVRNAME_P(&attrs[10], 2))));
    SA_ATTR_BND_SVRNAME_SYNTAX(&attrs[10], 2) = 3;
    SA_ATTR_BND_SVRNAME_NAME_P(&attrs[10], 2) = malloc(32);
    memset(SA_ATTR_BND_SVRNAME_NAME_P(&attrs[10], 2), NULL, 32);
    SA_ATTR_BND_SVRNAME_NAME_P(&attrs[10], 2) = (unsigned_char_p_t)"tst3";

    attr_update("test them all", num_attrs, attrs, error_status_ok);
    verify_attr_update("test them all", num_attrs, attrs, error_status_ok, &st);
    tet_result(TET_PASS);
}

static void
tp20()	/* successful test for sec_rgy_attr_lookup_by_id */
{
    tet_infoline("test case (tp20)");
    attr_lookup(test_integer, "for 1 attr id", 1);
    tet_result(TET_PASS);
}

static void
tp21()	/* successful test for sec_rgy_attr_lookup_by_id */
{
    tet_infoline("test case (tp21)");
    attr_lookup(test_integer, "for 0 attr ids", 0);
    tet_result(TET_PASS);
}
