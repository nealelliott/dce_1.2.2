#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: dts_show_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:15:35  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:31:18  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:02  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:07  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:43  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:57  melman]
# 
# Revision 1.1.2.5  1994/06/10  20:12:43  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:16  devsrc]
# 
# Revision 1.1.2.4  1994/05/23  18:24:04  rousseau
# 	General updates and performance changes (CR 9938).
# 	[1994/05/23  18:23:12  rousseau]
# 
# Revision 1.1.2.3  1994/05/11  19:37:15  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:20:57  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:35:58  melman
# 	updating to new API path
# 	[1994/05/06  15:35:28  melman]
# 
# Revision 1.1.2.1  1994/04/21  13:57:56  rousseau
# 	Initial drop of dts and clock tests (CR 9938).
# 	[1994/04/21  13:56:00  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc dts_show_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc dts_show_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc dts_show_3_P {} {
    # Identify the test
    tet_infoline dts_show_3_P: Check to see that we can show DTS entity attributes using the 
    tet_infoline dts_show_3_P: -attributes option.

    # Setup steps
        # Must be running in a cell with multiple dtsd's. Some of the dtsd's
        # must be configured as local servers while others must be configured
        # as global servers.

    # Run the test
    if {[catch {dts show -attributes} msg] != 0} {
	tet_infoline Test Failed.
	tet_result FAIL
    } else {
	tet_infoline Test Passed.
	tet_result PASS
    }

    # Cleanup steps
}

proc dts_show_4_P {} {
    # Identify the test
    tet_infoline dts_show_4_P: Check to see that we can show DTS entity attributes using no
    tet_infoline dts_show_4_P: no options. -attributes is the default.

    # Setup steps

    # Run the test
    if {[catch {dts show} msg] != 0} {
	tet_infoline Test failed.
	tet_result FAIL
    } else {
	tet_infoline Test Passed.
	tet_result PASS
    }

    # Cleanup steps
}

proc dts_show_5_P {} {
    # Identify the test
    tet_infoline dts_show_5_P: Check to see that we can show DTS entity counters using the
    tet_infoline dts_show_5_P: -counters option.

    # Setup steps

    # Run the test
    if {[catch {dts show -counters} msg] != 0} {
	tet_infoline Test failed.
	tet_result FAIL
    } else {
	tet_infoline Test Passed.
	tet_result PASS
    }

    # Cleanup steps
}

proc dts_show_6_P {} {
    # Identify the test
    tet_infoline dts_show_6_P: Check to see that we can show the attributes and counters on a DTS 
    tet_infoline dts_show_6_P: entity.

    # Setup steps

    # Run the test
    if {[catch {dts show -all} msg] != 0} {
	tet_infoline Test failed.
	tet_result FAIL
    } else {
	tet_infoline Test Passed.
	tet_result PASS
    }

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "dts_show_3_P"
set ic2 "dts_show_4_P"
set ic3 "dts_show_5_P"
set ic4 "dts_show_6_P"
set tet_startup dts_show_startup
set tet_cleanup dts_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
