#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: audtrail_show_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:11:38  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:23  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:50  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:51:58  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:21  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:07:01  melman]
# 
# Revision 1.1.2.5  1994/06/10  20:03:26  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:02  devsrc]
# 
# Revision 1.1.2.4  1994/05/23  18:09:27  rousseau
# 	Various updates.
# 	[1994/05/23  18:09:02  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:27:14  rousseau
# 	Performance changes, and new API path (CR 10558).
# 	[1994/05/06  15:25:10  rousseau]
# 
# Revision 1.1.2.1  1994/04/21  20:29:04  rousseau
# 	Initial drop of audit tests (CR 9938).
# 	[1994/04/21  20:27:40  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_setup.tcl

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


## Test purposes
proc audtrail_show_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc audtrail_show_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}


proc audtrail_show_4_P {} {
    global TET_ROOT DCP_AUD_TRAIL_FILE_1

    # Identify the test
    tet_infoline audtrail_show_4_P: Check that we can show an audit trail file.

    # Setup steps

    # Run the test
    if {[catch {audtrail show "$TET_ROOT/$DCP_AUD_TRAIL_FILE_1"} msg] != 0} {
	tet_infoline Test returned $msg
	tet_infoline Test Failed.
	tet_result FAIL
    } else {
	tet_infoline Test Passed.
	tet_result PASS
    }

    # Cleanup steps
}

proc audtrail_show_5_P {} {
    global TET_ROOT DCP_AUD_TRAIL_FILE_1

    # Identify the test
    tet_infoline audtrail_show_5_P: Check that we can show an audit trail file without list braces.

    # Setup steps

    # Run the test
    if {[catch {audtrail show $TET_ROOT/$DCP_AUD_TRAIL_FILE_1} msg] != 0} {
	tet_infoline Test returned $msg
	tet_infoline Test Failed.
	tet_result FAIL
    } else {
	tet_infoline Test Passed.
	tet_result PASS
    }

    # Cleanup steps
}

proc audtrail_show_6_P {} {
    global TET_ROOT DCP_AUD_TRAIL_FILE_1 DCP_AUD_TRAIL_FILE_2

    # Identify the test
    tet_infoline audtrail_show_6_P: Check that we can show a list of audit trail files.

    # Setup steps

    # Run the test
    if {[catch {audtrail show "$TET_ROOT/$DCP_AUD_TRAIL_FILE_1 $TET_ROOT/$DCP_AUD_TRAIL_FILE_2"} msg] != 0} {
	tet_infoline Test returned $msg
	tet_infoline Test Failed.
	tet_result FAIL
    } else {
	tet_infoline Test Passed.
	tet_result PASS
    }

    # Cleanup steps
}

proc audtrail_show_7_P {} {
    global TET_ROOT DCP_AUD_TRAIL_FILE_1 DCP_AUD_TRAIL_OUTFILE

    # Identify the test
    tet_infoline audtrail_show_7_P: Check that we can dump an audit trail to a file using -to.

    # Setup steps

    # Run the test
    if {([catch {audtrail show $TET_ROOT/$DCP_AUD_TRAIL_FILE_1 -to $DCP_AUD_TRAIL_OUTFILE} msg] != 0) || ([file exists $DCP_AUD_TRAIL_OUTFILE] == 0)} {
	tet_infoline Test returned $msg
	tet_infoline Test Failed.
	tet_result FAIL
    } else {
	tet_infoline Test Passed.
	tet_result PASS
    }

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "audtrail_show_4_P"
set ic2 "audtrail_show_5_P"
set ic3 "audtrail_show_6_P"
set ic4 "audtrail_show_7_P"
set tet_startup audtrail_show_startup
set tet_cleanup audtrail_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl

