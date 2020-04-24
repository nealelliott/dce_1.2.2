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
 * Revision 1.1.6.1  1996/07/09  15:14:43  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:30 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  23:07:31  marty  1.1.4.2]
 *
 * Revision 1.1.4.2  1996/02/18  23:07:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:09  marty]
 * 
 * Revision 1.1.4.1  1995/12/11  21:52:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:54:00  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:46:25  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:17:36  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/20  18:09:23  weisz
 * 	initial submission
 * 	[1994/04/20  18:04:45  weisz]
 * 
 * $EndLog$
 */
#define	APPLICATION 	   "aud_test_application_server_princ"
#define KEY_FILE	   "/tmp/aud_test_key"
#define TRAIL_FILE	   "/tmp/aud_test_trail"
#define TRAIL_INDEX_FILE   "/tmp/aud_test_trail.md_index"
#define BINDING_FILE	   "/tmp/aud_test_application_server_binding"
#define WORKING_DIR	   "/tmp"
#define FIRST_EVT_NUMBER   0
#define NUMBER_OF_EVTS    10
#define CONCURRENCY_LEVEL  8
