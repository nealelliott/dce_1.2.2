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
 * $Log: dce_attr_sch_fvt.h,v $
 * Revision 1.1.4.2  1996/02/18  23:03:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:30  marty]
 *
 * Revision 1.1.4.1  1995/12/08  22:08:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:43:19  root]
 * 
 * Revision 1.1.2.2  1994/09/09  19:24:12  pwang
 * 	Matched the changes in dced schema server implementation
 * 	[OT#12096]
 * 
 * Revision 1.1.2.1  1994/08/11  18:35:32  pwang
 * 	Initial version
 * 	[1994/08/11  18:28:53  pwang]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <stdlib.h>

#include <tet_api.h>

#include <dce/macros.h>
#include <dce/dce_error.h>
#include <dce/sec_login.h>
#include <dce/sec_attr_util.h>

#include <dce/dce_attr_base.h>
#include <dce/dced_aclbits.h>
#include <dce/dce_attr_sch.h>


/*
** Test Case Wide Declarations 
*/
#define	schema_srv_name		tet_getvar("TET_ATTR_SRV_NAME")
#define	schema_comment_update	"DCE_Attr_Sch_Test_Update"

#define test_c_prot_level	rpc_c_protect_level_default
#define test_c_authn_svc	rpc_c_authn_default
#define test_c_authz_svc	rpc_c_authz_dce

typedef struct test_res_attr_sch_entry_s_t {
    char                        *attr_name;
    char                        *attr_id;
    sec_attr_encoding_t         attr_encoding;
    /*
    **sec_attr_acl_mgr_info_set_t *acl_mgr_set;
    */
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
} test_res_attr_sch_entry_t;

test_res_attr_sch_entry_t   test_attr_sch_entry[] = {
{"DCE_Attr_Sch_Test",
 "80e07b84-b2c2-11cd-9073-0000c010df56", 	/* new uuid */
 sec_attr_enc_integer,                 
 "004d3731-c9b5-1d6c-9adb-0000c09ce054",	/* sc_obj_type of dced */
 dced_perm_read,               
 dced_perm_write,
 dced_perm_control,              
 dced_perm_delete,
 (sec_attr_sch_entry_reserved | sec_attr_sch_entry_use_defaults),
 sec_attr_intercell_act_reject,        
 sec_attr_trig_type_none,
 NULL,                                   
 "",
 "dce_attr_sch fvt"}
};

/*
** Check test purpose's reuslts and report to tet_journal or stdout
*/
int		  			dce_status;
char					msg[1024];
unsigned char 				unexpected[dce_c_error_string_len];
unsigned char 				expected[dce_c_error_string_len];

#define CHECK_RESULT(T, FUNC, RETURNED_ST, EXPECTED_ST)			\
    if (STATUS_EQUAL(&RETURNED_ST, EXPECTED_ST)) {			\
	if ((T)) {							\
		sprintf(msg, "PASSED %s()\n", FUNC);			\
		tet_infoline(msg);					\
	} else {							\
		sprintf(msg, "FAILED %s()\nresult: mismatch\n", FUNC);	\
		tet_infoline(msg);					\
		tet_result(TET_FAIL);					\
		return;							\
	}								\
    } else {								\
	dce_error_inq_text(RETURNED_ST, unexpected, &dce_status);	\
	dce_error_inq_text(EXPECTED_ST, expected, &dce_status);		\
	sprintf(msg, "FAILED %s()\nresult: \t%s\nexpected: \t%s\n\n",	\
	        FUNC, unexpected, expected);				\
	tet_infoline(msg);						\
	tet_result(TET_FAIL);						\
	return;								\
    }


