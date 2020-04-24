#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: dts_stop_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:15:42  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:31:21  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:04  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/09/29  13:30 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts47/1  1995/09/29  13:29 UTC  truitt
# 	CHFts14292: Improvement in tests.
# 
# 	HP revision /main/HPDCE02/1  1995/06/06  18:32 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts33/1  1995/06/06  18:31 UTC  truitt
# 	CHFts15424: Change tests to reflect new dced acl behavior.
# 	[1995/12/11  15:53:09  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:45  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:00  melman]
# 
# Revision 1.1.2.6  1994/09/02  22:21:10  rousseau
# 	Mass test updates.
# 	[1994/09/02  22:20:45  rousseau]
# 
# Revision 1.1.2.5  1994/06/10  20:12:44  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:18  devsrc]
# 
# Revision 1.1.2.4  1994/05/23  18:24:06  rousseau
# 	General updates and performance changes (CR 9938).
# 	[1994/05/23  18:23:14  rousseau]
# 
# Revision 1.1.2.3  1994/05/11  19:37:18  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:21:05  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:36:00  melman
# 	updating to new API path
# 	[1994/05/06  15:35:30  melman]
# 
# Revision 1.1.2.1  1994/04/21  13:58:00  rousseau
# 	Initial drop of dts and clock tests (CR 9938).
# 	[1994/04/21  13:56:02  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


## Test purposes
proc dts_stop_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc dts_stop_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}    


proc dts_stop_4_P {} {
    # Identify the test
    tet_infoline dts_stop_4_P: Check to see that we can stop a DTS entity.

    # Setup steps

    # Run the test
    dts stop
    exec sleep 10

    if {[catch {exec ps -ef | grep dtsd | grep -v grep} msg] == 0} {
	tet_infoline DTSD did not stop
	tet_result FAIL
    } else {
        tet_infoline Test Passed.
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {exec /opt/dcelocal/bin/dtsd -s}] != 0} {return;}
    dcp_hide {while {[dcp_get_element [dts show] status] != "enabled"} {
        exec sleep 2}
    }
}


set iclist "ic1 "
set ic1 "dts_stop_4_P"
set tet_startup dts_stop_startup
set tet_cleanup dts_stop_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
