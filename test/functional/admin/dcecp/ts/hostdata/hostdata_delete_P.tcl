#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: hostdata_delete_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:16:19  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:31:55  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:17  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:21  root]
# 
# Revision 1.1.2.1  1994/09/28  21:40:04  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:21  melman]
# 
# Revision 1.1.2.3  1994/06/22  20:42:44  baum
# 	Merged with changes from 1.1.2.2
# 	[1994/06/22  20:42:25  baum]
# 
# 	OT #11022 - Use tet_setup, use tetexec.cfg for cell/host
# 	[1994/06/22  20:40:16  baum]
# 
# Revision 1.1.2.2  1994/06/17  18:43:16  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:25:46  devsrc]
# 
# Revision 1.1.2.1  1994/05/19  22:51:56  baum
# 	Initial checkin
# 	[1994/05/19  22:51:23  baum]
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

## Routine specific to hostdata tests
source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_misc.tcl

proc tet_setup {} { hd_std_setup }
proc tet_cleanup {} { hd_std_cleanup }

## Test purposes
proc hostdata_delete_6_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_vars.tcl

    # Identify the test
    tet_infoline hostdata_delete_6_P: Check to see that we can delete hostdata entry.

    # Setup steps
    tet_setup

    # Run the test
    set cmd "hostdata delete $HD1_LONG_NAME -entry"
    set ret [catch "$cmd" msg]
    if { $ret != 0 } {
      tet_infoline "Hostdata delete -entry fails:"
      tet_infoline $msg
      tet_result FAIL
      tet_cleanup
      return
    }

    # Verify results, hostdata show $HD1_LONG_NAME should fail, storage
    # file should still be there
    set cmd "hostdata show $HD1_LONG_NAME"
    set ret [catch "$cmd" msg]
    if { $ret == 0 } {
      tet_infoline "Problem: Hostdata show of deleted entry does NOT fail!!"
      tet_result FAIL
      tet_cleanup
      return	
    }

    set ret [file exists $HD1_STORAGE]
    if { $ret == 0 } {
       tet_infoline "Problem: Hostdata delete -entry succeeds, yet storage file is missing!"
      tet_result FAIL
      tet_cleanup
      return
    }

    tet_infoline Test Passed
    tet_result PASS

    # Cleanup steps
    tet_cleanup
}

proc hostdata_delete_7_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_vars.tcl

    # Identify the test
    tet_infoline hostdata_delete_7_P: Check to see that we can delete hostdata entry and objects.

    # Setup steps
    tet_setup

    # Run the test
    set cmd "hostdata delete $HD1_LONG_NAME"
    set ret [catch "$cmd" msg]
    if { $ret != 0 } {
      tet_infoline "Hostdata delete fails:"
      tet_infoline $msg
      tet_result FAIL
      tet_cleanup
      return
    }

    # Verify results, hostdata show $HD1_LONG_NAME should fail, storage
    # file be gone
    set cmd "hostdata show $HD1_LONG_NAME"
    set ret [catch "$cmd" msg]
    if { $ret == 0 } {
      tet_infoline "Problem: Hostdata show of deleted object does NOT fail!!"
      tet_result FAIL
      tet_cleanup
      return	
    }

    set ret [file exists $HD1_STORAGE]
    if { $ret != 0 } {
       tet_infoline "Problem: Hostdata delete succeeds, yet storage file is not gone!"
      tet_result FAIL
      tet_cleanup
      return
    }

    tet_infoline Test Passed
    tet_result PASS

    # Cleanup steps
    tet_cleanup
}

set iclist "ic1 ic2 "
set ic1 "hostdata_delete_6_P"
set ic2 "hostdata_delete_7_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
