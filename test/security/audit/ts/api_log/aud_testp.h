/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * (c) Copyright International Business Machines Corp. 1994, 1996
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: aud_testp.h,v $
 * Revision 1.1.6.1  1996/07/09  15:09:11  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:29 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  23:07:29  marty  1.1.4.2]
 *
 * Revision 1.1.4.2  1996/02/18  23:07:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:04  marty]
 * 
 * Revision 1.1.4.1  1995/12/11  21:51:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:53:43  root]
 * 
 * Revision 1.1.2.3  1994/09/27  17:32:41  weisz
 * 	CR 10703: added tests for trail full situation.
 * 	[1994/09/27  17:31:51  weisz]
 * 
 * Revision 1.1.2.2  1994/06/10  20:45:55  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:17:16  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/20  13:28:00  weisz
 * 	initial submission
 * 	[1994/04/20  13:26:27  weisz]
 * 
 * $EndLog$
 */
#define	APPLICATION 	   "aud_test_application_server_princ"
#define KEY_FILE	   "/tmp/aud_test_key"
#define TRAIL_FILE	   "/tmp/aud_test_trail"
#define TRAIL_INDEX_FILE   "/tmp/aud_test_trail.md_index"
#define TEST_TRAIL_FILE    "/tmp/test_trail"
#define TEST_INDEX_FILE    "/tmp/test_trail.md_index"
#define NEW_TRAIL_FILE     "/tmp/new_test_trail"
#define NEW_INDEX_FILE     "/tmp/new_test_trail.md_index"
#define TMP_TRAIL_FILE     "/tmp/tmp_test_trail"
#define TMP_INDEX_FILE     "/tmp/tmp_test_trail.md_index"
#define SAVE_TRAIL_FILE    "/opt/dcelocal/var/audit/adm/test_trail.old"
#define SAVE_INDEX_FILE    "/opt/dcelocal/var/audit/adm/test_trail.md_index.old"
#define TRAIL_FILE1	   "/tmp/aud_test_trail1"
#define TRAIL_INDEX_FILE1  "/tmp/aud_test_trail1.md_index"
#define TRAIL_FILE2	   "/tmp/aud_test_trail2"
#define TRAIL_INDEX_FILE2  "/tmp/aud_test_trail2.md_index"
#define TRAIL_FILE3	   "/tmp/aud_test_trail3"
#define TRAIL_INDEX_FILE3  "/tmp/aud_test_trail3.md_index"
#define BINDING_FILE	   "/tmp/aud_test_application_server_binding"
#define WORKING_DIR	   "/tmp"
#define FIRST_EVT_NUMBER   0
#define NUMBER_OF_EVTS    20
#define CONCURRENCY_LEVEL  1
