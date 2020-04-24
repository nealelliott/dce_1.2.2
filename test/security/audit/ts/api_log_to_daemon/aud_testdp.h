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
 * $Log: aud_testdp.h,v $
 * Revision 1.1.6.1  1996/07/09  15:10:28  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:29 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  23:07:30  marty  1.1.4.2]
 *
 * Revision 1.1.4.2  1996/02/18  23:07:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:06  marty]
 * 
 * Revision 1.1.4.1  1995/12/11  21:51:40  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/27  21:20 UTC  kline_s
 * 	Merge in revised audit tests.
 * 
 * 	HP revision /main/kline_s_mothra/1  1995/06/27  02:33 UTC  kline_s
 * 	Change pathname for central_trail to reflect actual location in
 * 	/opt/dcelocal/var/audit/
 * 	[1995/12/11  20:53:47  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:46:00  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:17:22  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/20  19:36:46  weisz
 * 	initial submission
 * 	[1994/04/20  19:28:12  weisz]
 * 
 * $EndLog$
 */
#define	APPLICATION 	        "aud_test_application_server_princ"
#define KEY_FILE	        "/tmp/aud_test_key"
#define DAEMON_TRAIL_FILE       "/opt/dcelocal/var/audit/central_trail"
#define DAEMON_TRAIL_INDEX_FILE "/opt/dcelocal/var/audit/central_trail.md_index"
#define BINDING_FILE	        "/tmp/aud_test_application_server_binding"
#define WORKING_DIR	        "/tmp"
#define CONCURRENCY_LEVEL       8
