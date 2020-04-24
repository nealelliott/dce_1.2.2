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
 * $Log: common.h,v $
 * Revision 1.1.2.2  1996/10/03  18:42:46  arvind
 * 	Add OSF copyright
 * 	[1996/09/30  15:50 UTC  arvind  /main/DCE_1.2.2/3]
 *
 * Revision 1.1.2.1  1996/08/09  20:12:06  arvind
 * 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  19:57:39  arvind]
 * 
 * $EndLog$
 */
#define PK_USER "pk_user"
#define PK_PASSWD "pk_pwd"
#define NEW_PK_PASSWD "new_pk_pwd"


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

#undef CHECK_RESULT_DONT_RETURN
#define CHECK_RESULT_DONT_RETURN(T, FUNC, RETURNED_ST_P, EXPECTED_ST) \
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

