/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ts-common.h,v $
 * Revision 1.1.10.1  1996/07/09  15:00:20  arvind
 * 	Make globals 'extern'.
 * 	[1996/06/17  19:42 UTC  jrr  /main/jrr_122_1/2]
 *
 * 	Fix the SEC_RGY_SITE_OPEN_* macros. Initialize variables where
 * 	appropriate. Add an entra check in for REPLICATION tests.
 * 	[1996/06/14  20:54 UTC  jrr  /main/jrr_122_1/1]
 *
 * Revision 1.1.8.3  1996/02/18  23:05:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:46  marty]
 * 
 * Revision 1.1.8.2  1995/12/11  19:52:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:26:40  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  18:18:04  root]
 * 
 * Revision 1.1.3.2  1994/10/27  20:07:34  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:39:55  rrizzo]
 * 
 * Revision 1.1.3.1  1994/10/25  17:17:41  sekhar
 * 	[ OT 12398 ]  Checking in Sean Mullan's changes for testing
 * 		      security replication.
 * 	[1994/10/24  20:28:43  sekhar]
 * 
 * 	 *	HP revision /main/mullan_mothra_test_repl_cov/3  1994/09/28  19:42 UTC  mullan_s
 * 	Set sec_replica_site_name=sec_master_site_name if not testing replication.
 * 
 * 	 *	HP revision /main/mullan_mothra_test_repl_cov/2  1994/09/28  15:08 UTC  mullan_s
 * 	Ensure that query site is really a read-only replica.
 * 
 * 	 *	HP revision /main/mullan_mothra_test_repl_cov/1  1994/09/13  17:11 UTC  mullan_s
 * 	Common header file for Security TET tests.
 * 
 * $EndLog$
 */

/* multiple inclusion guard */
#ifndef __ts_common_h_included__
#define __ts_common_h_included__

#include <dce/binding.h>
#include <dce/dce.h>
#include <dce/dce_error.h>
#include <dce/macros.h>
#include <dce/misc.h>
#include <dce/repadm.h>

/*
 * checks an expected status against the actual status returned from the API and
 * reports the results. Input:
 *
 * T - If TRUE and expected status = actual status, print
 *     a PASSED message.
 * MSG - The API and info being tested
 * RETURNED_ST_P - a pointer to the actual DCE status
 * EXPECTED_ST - expected status
 *
 * XXX- There is one problem with these macros. If you need to perform some
 * local cleanup (freeing memory, purging login context) before returning from
 * the function, there is no way to do this.
 */
extern unsigned char           unexpected[dce_c_error_string_len];
extern unsigned char           expected[dce_c_error_string_len];
extern int                     dce_status;
extern error_status_t          rgy_st;
extern int                     test_no;
extern int                     test_replica;
extern unsigned_char_t         *sec_replica_site_name;
extern unsigned_char_t         *sec_master_site_name;
extern char                    *sec_test_replication;
#define CHECK_RESULT(T, MSG, RETURNED_ST_P, EXPECTED_ST)                        \
    if (STATUS_EQUAL(RETURNED_ST_P, EXPECTED_ST)) {                             \
        if ((T)) mtet_infoline("PASSED %s\n", MSG);                             \
    } else {                                                                    \
        dce_error_inq_text(*(RETURNED_ST_P), unexpected, &dce_status);          \
        dce_error_inq_text(EXPECTED_ST, expected, &dce_status);                 \
        mtet_infoline("FAILED %s\nresult: \t%s\nexpected: \t%s\n\n",            \
                MSG, unexpected, expected);                                     \
        tet_result(TET_FAIL);                                                   \
        return;                                                                 \
    }

/*
 * CHECK_CLEANUP_RESULT - similar to CHECK_RESULT, called from tet_cleanup
 */
#define CHECK_CLEANUP_RESULT(T, MSG, RETURNED_ST_P, EXPECTED_ST)                \
    if (STATUS_EQUAL(RETURNED_ST_P, EXPECTED_ST)) {                             \
        if ((T)) mtet_infoline("PASSED %s\n", MSG);                             \
    } else {                                                                    \
        dce_error_inq_text(*(RETURNED_ST_P), unexpected, &dce_status);          \
        dce_error_inq_text(EXPECTED_ST, expected, &dce_status);                 \
        mtet_infoline("WARNING %s\nresult: \t%s\nexpected: \t%s\n\n",           \
                MSG, unexpected, expected);                                     \
        return;                                                                 \
    }

/*
 * CHECK_STARTUP_RESULT - similar to CHECK_RESULT, called from tet_startup
 */
#define CHECK_STARTUP_RESULT(T, MSG, RETURNED_ST_P, EXPECTED_ST)                \
    if (STATUS_EQUAL(RETURNED_ST_P, EXPECTED_ST)) {                             \
        if ((T)) mtet_infoline("PASSED %s\n", MSG);                             \
    } else {                                                                    \
        dce_error_inq_text(*(RETURNED_ST_P), unexpected, &dce_status);          \
        dce_error_inq_text(EXPECTED_ST, expected, &dce_status);                 \
        mtet_infoline("FAILED %s\nresult: \t%s\nexpected: \t%s\n\n",            \
                MSG, unexpected, expected);                                     \
        for (test_no=0; tet_testlist[test_no++].testfunc != NULL;)              \
            tet_delete(test_no, "tet_startup() has failed");                    \
        return;                                                                 \
    }

#define CHECK_MIGRATE_RESULT(API, T, MSG, RETURNED_ST_P)			\
    if (sec_migrate_check_exp_result(API, RETURNED_ST_P)) {			\
	if ((T)) mtet_infoline("PASSED %s\n", MSG);				\
    } else {									\
	dce_error_inq_text(*(RETURNED_ST_P), unexpected, &dce_status);		\
	mtet_infoline("FAILED %s\nresult: \t%s\nexpected: \t%s\n\n",		\
            MSG, unexpected, 							\
	    "ept_s_not_registered, rpc_s_unknown_if, or sec_s_v1_1_no_support");\
        tet_result(TET_FAIL); 	                                               \
        if (!(T)) {								\
            return;                                                             \
	}									\
    }

/*
 * SEC_RGY_SITE_OPEN_READ returns a registry handle to a security
 * server which will be used to read data from. If test_replica is TRUE,
 * the binding handle points to a replica, otherwise it points to
 * the master.
 *
 * This macro assumes you are calling it from tet_startup!
 */
#define SEC_RGY_SITE_OPEN_READ(CONTEXT_P)					\
    sec_test_replication = tet_getvar("SEC_TEST_REPLICATION");			\
    if ((sec_test_replication != NULL) &&					\
        ((*sec_test_replication == 'T') ||					\
         (*sec_test_replication == 't'))) {					\
       test_replica = TRUE; 							\
       sec_replica_site_name = (unsigned_char_t *)                              \
                                 tet_getvar("SEC_REPLICA_SITE_NAME");		\
       if (sec_replica_site_name == NULL) {                                     \
           mtet_infoline("FAILED: In tet_startup: SEC_REPLICA_SITE_NAME = NULL.\n");\
           for (test_no=0; tet_testlist[test_no++].testfunc != NULL;)           \
               tet_delete(test_no, "tet_startup() has failed");                 \
           return;                                                              \
       }                                                                        \
    } else									\
       test_replica = FALSE;							\
    if (test_replica) {								\
        rs_replica_info_t  rep_info;						\
        sec_rgy_site_open_query(sec_replica_site_name, CONTEXT_P, &rgy_st);	\
        CHECK_STARTUP_RESULT(FALSE, "sec_rgy_site_open_query()",		\
                     &rgy_st, error_status_ok);					\
        sec_rgy_rep_admin_info(*(CONTEXT_P), &rep_info, &rgy_st);		\
        CHECK_STARTUP_RESULT(FALSE, "sec_rgy_rep_admin_info()",			\
                     &rgy_st, error_status_ok);					\
        if (rep_info.master) {							\
            mtet_infoline("%s %s\n", "FAILED: SEC_REPLICA_SITE_NAME is the",	\
                "master replica, it should be read-only!");			\
            for (test_no=0; tet_testlist[test_no++].testfunc != NULL;)          \
                tet_delete(test_no, "tet_startup() has failed");                \
            return;								\
        }									\
    } else {									\
        sec_rgy_site_open_update(sec_master_site_name, CONTEXT_P, &rgy_st);	\
        CHECK_STARTUP_RESULT(FALSE, "sec_rgy_site_open_update()",		\
                     &rgy_st, error_status_ok);					\
        sec_replica_site_name = sec_master_site_name;				\
    }

/*
 * SEC_RGY_SITE_OPEN_WRITE - returns a binding handle to the master
 * security server.
 *
 * This macro assumes you are calling it from tet_startup!
 */
#define SEC_RGY_SITE_OPEN_WRITE(CONTEXT_P)					\
    sec_rgy_site_open_update(sec_master_site_name, CONTEXT_P, &rgy_st);		\
    CHECK_STARTUP_RESULT(FALSE, "sec_rgy_site_open_update()",			\
                 &rgy_st, error_status_ok);

/*
 * SEC_RGY_SITE_SYNCH - blocks until prior updates have been propagated
 * to the replica. If test_replica is FALSE, this macro does nothing.
 *
 * This macro assumes you are calling it from a test purpose!!!
 */
#define SEC_RGY_SITE_SYNCH(CONTEXT)						\
    if (test_replica) {								\
        if (!(sec_rgy_wait_until_consistent(CONTEXT, &rgy_st)) ||		\
              (rgy_st != error_status_ok)) {					\
            CHECK_RESULT(FALSE, "sec_rgy_wait_until_consistent()",      	\
                         &rgy_st, error_status_ok);                             \
            mtet_infoline("FAILED: sec_rgy_wait_until_consistent() returned FALSE\n");    \
            tet_result(TET_FAIL);						\
            return;                                                             \
        } 									\
    }

typedef enum sec_migrate_api {
        Msec_10_first = 1,
        Msec_10_last,
        Msec_11_first,
	Msec_cred_get_extended_attrs,
	Msec_cred_get_initiator,
        Msec_login_become_initiator,
        Msec_login_cred_get_initiator,
        Msec_login_cred_get_delegate,
	Msec_login_set_extended_attrs,
	Msec_login_become_delegate,
	Msec_login_disable_delegation,
	Msec_login_become_impersonator,
        Msec_pwd_mgmt_gen_pwd,
        Msec_rgy_attr_update,
	Msec_rgy_attr_delete,
        Msec_rgy_attr_lookup_by_id,
	Msec_rgy_attr_cursor_init,
	Msec_rgy_attr_lookup_by_name,
	Msec_rgy_attr_lookup_no_expand,
        Msec_rgy_attr_sch_create_entry,
        Msec_rgy_attr_sch_delete_entry,
        Msec_rgy_attr_sch_lookup_by_id,
        Msec_rgy_attr_sch_lookup_by_name,
        Msec_rgy_attr_sch_cursor_init,
        Msec_rgy_attr_sch_scan,
        Msec_rgy_attr_sch_update_entry,
        Msec_11_last
} sec_api_name_t;

#define Msec_not_remote_api     -1

#endif /* __ts_common_h_included__ */
