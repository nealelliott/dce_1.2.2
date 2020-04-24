#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: hostdata_create_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:16:14  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:31:51  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:16  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:20  root]
# 
# Revision 1.1.2.1  1994/09/28  21:40:00  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:17  melman]
# 
# Revision 1.1.2.4  1994/06/22  20:42:43  baum
# 	Merged with changes from 1.1.2.3
# 	[1994/06/22  20:42:22  baum]
# 
# 	OT #11022 - Use tet_setup, use tetexec.cfg for cell/host
# 	[1994/06/22  20:40:15  baum]
# 
# Revision 1.1.2.3  1994/06/17  18:43:15  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:25:39  devsrc]
# 
# Revision 1.1.2.2  1994/06/07  23:13:45  baum
# 	Mods to handle new dcecp output (CR #10761)
# 	[1994/06/07  23:13:16  baum]
# 
# Revision 1.1.2.1  1994/05/19  22:51:54  baum
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

## Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

## Routine specific to hostdata tests
source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_misc.tcl

proc tet_setup {} { hd_std_setup }
proc tet_cleanup {} { hd_std_cleanup }

## Test purposes
proc hostdata_create_9_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_vars.tcl

    # Identify the test
    tet_infoline hostdata_create_9_P: Check to see that we can create a hostdata entry.

    # Setup steps, create HD1, the remove entry
    tet_setup	
    set cmd "hostdata delete $HD1_LONG_NAME -entry"
    set ret [catch "$cmd" msg]
    if { $ret != 0 } {
      tet_infoline "hostdata delete -entry fails during setup phase:"
      tet_infoline $msg
      tet_result FAIL
      tet_cleanup
      return
    }

    # Run the test
    set cmd "hostdata create $HD1_LONG_NAME -storage $HD1_STORAGE -uuid $HD1_UUID -annotation $HD1_ANNOTATION -entry"
    set ret [catch "$cmd" msg]
    compare_and_report $cmd $ret $msg ""

    # Cleanup steps, remove HD1
    tet_cleanup	
}

proc hostdata_create_10_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_vars.tcl

    # Identify the test
    tet_infoline hostdata_create_10_P: Check to see that we can create a hostdata entry and object.

    # Setup steps, remove HD1 entry and object.
    tet_setup
    set cmd "hostdata delete $HD1_LONG_NAME"
    set ret [catch "$cmd" msg]
    if { $ret != 0 } {
      tet_infoline "hostdata delete fails during setup phase:"
      tet_infoline $msg
      tet_result FAIL
      tet_cleanup
      return
    }

    # Run the test
    set cmd "hostdata create $HD1_LONG_NAME -storage $HD1_STORAGE -uuid $HD1_UUID -annotation $HD1_ANNOTATION -data {$LINE1 $LINE2}"
    set ret [catch "$cmd" msg]
    compare_and_report $cmd $ret $msg ""

    # Cleanup steps, remove HD1
    tet_cleanup	

}


set iclist "ic1 ic2 "
set ic1 "hostdata_create_9_P"
set ic2 "hostdata_create_10_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl

