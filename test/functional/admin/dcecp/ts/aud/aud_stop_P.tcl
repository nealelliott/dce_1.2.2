#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: aud_stop_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:10:27  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:10  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:35  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/09/29  13:26 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts47/1  1995/09/29  13:25 UTC  truitt
# 	CHFts14292: Improvement in tests.
# 
# 	HP revision /main/HPDCE02/1  1995/06/06  18:30 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts33/1  1995/06/06  18:29 UTC  truitt
# 	CHFts15424: Change tests to reflect new dced acl behavior.
# 	[1995/12/11  15:51:37  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:04  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:42  melman]
# 
# Revision 1.1.2.6  1994/09/21  16:58:41  rousseau
# 	Test cleanup.
# 	[1994/09/21  16:58:25  rousseau]
# 
# Revision 1.1.2.5  1994/09/02  20:28:33  rousseau
# 	Mass test cleanup.
# 	[1994/09/02  20:27:50  rousseau]
# 
# Revision 1.1.2.4  1994/06/10  20:02:35  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:41  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:34:49  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:16:14  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:26:54  rousseau
# 	Performance changes, and new API path (CR 10558).
# 	[1994/05/06  15:24:27  rousseau]
# 
# Revision 1.1.2.1  1994/04/21  20:28:42  rousseau
# 	Initial drop of audit tests (CR 9938).
# 	[1994/04/21  20:27:19  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


## Test purposes
proc aud_stop_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc aud_stop_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}    

proc aud_stop_4_P {} {
    # Identify the test
    tet_infoline aud_stop_4_P: Check to see that we can stop the audit daemon.

    # Setup steps

    # Run the test
    aud stop
    exec sleep 10
    if {[catch {exec ps -ef | grep auditd | grep -v grep} msg] == 0} {
        tet_infoline AUDITD did not stop
        tet_result FAIL
    } else {
        tet_infoline Test Passed.
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {exec /opt/dcelocal/bin/auditd -a}] != 0} {
        tet_infoline Could not re-start AUDITD
        tet_result FAIL
    }
}


set iclist "ic1 "
set ic1 "aud_stop_4_P"
set tet_startup aud_stop_startup
set tet_cleanup aud_stop_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
