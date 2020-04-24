#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: keytab_delete_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:16:51  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:11  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:29  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:32  root]
# 
# Revision 1.1.2.1  1994/09/28  21:40:28  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:40  melman]
# 
# Revision 1.1.2.4  1994/06/22  20:37:15  baum
# 	OT #11022 - Use tetexec.cfg for cell/host.  Use tet_setup
# 	[1994/06/22  20:36:08  baum]
# 
# Revision 1.1.2.3  1994/06/10  20:13:19  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:29  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  19:37:40  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:21:53  rousseau]
# 
# Revision 1.1.2.1  1994/05/10  22:19:50  baum
# 	INitial Checkin
# 	[1994/05/10  22:19:05  baum]
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

## Keytab specific routines
source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_misc.tcl

proc tet_setup {} { ktb_std_setup }
proc tet_cleanup {} { ktb_std_cleanup }


## Test purposes
proc keytab_delete_6_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_vars.tcl

    # Identify the test
    tet_infoline keytab_delete_6_P: Check to see that we can delete keytab entry.

    # Setup steps, create KTB2
    tet_setup
    
    # Run test
    set cmd "keytab delete $KTB2_LONG_NAME -entry"
    set ret [catch "$cmd" msg]
    if { $ret != 0 } {
      tet_infoline "Could not delete KEYTAB entry!"
      tet_infoline "Actual Result:"
      tet_infoline $msg
      tet_result FAIL
      return
    } 

    # Verify entry is gone by a LIST which should fail.
    set cmd "keytab list $KTB2_LONG_NAME"
    set ret [catch "$cmd" msg]
    if { $ret == 0 } {
      tet_infoline "Problem: Keytab list to check for deleted key fails!!"
      tet_result FAIL
      return
    }
    
    set ret [file exists $KTB2_STORAGE]
    if { $ret == 0 } {
       tet_infoline "Problem: Delete KEYTAB entry succeeds, yet storage file is missing!"
      tet_result FAIL
      return
    }

    tet_infoline Test Passed
    tet_result PASS
       
    # Cleanup steps, get rid of storage file
    tet_cleanup
    
}

proc keytab_delete_7_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_vars.tcl

    # Identify the test
    tet_infoline keytab_delete_7_P: Check to see that we can delete keytab entry and objects.

    # Setup steps
    tet_setup

    # Run test
    set cmd "keytab delete $KTB2_LONG_NAME"
    set ret [catch "$cmd" msg]
    if { $ret != 0 } {
      tet_infoline "Could not delete KEYTAB entry!"
      tet_infoline "Actual Result:"
      tet_infoline $msg
      tet_result FAIL
      return
    } 

    # Verify entry and storage is there
    set cmd "keytab list $KTB2_LONG_NAME"
    set ret [catch "$cmd" msg]
    if { $ret == 0 } {
       tet_infoline "Problem: Delete KEYTAB entry succeeds, but entry still exists!"
      tet_result FAIL
      return
    }

    set ret [file exists $KTB2_STORAGE]
    if { $ret != 0 } {
       tet_infoline "Problem: Delete KEYTAB succeeds, yet storage file still exists. ($msg)"
      tet_result FAIL
      return
    }

    tet_infoline Test Passed
    tet_result PASS

    # Cleanup steps
    tet_cleanup
}


set iclist "ic1 ic2 "
set ic1 "keytab_delete_6_P"
set ic2 "keytab_delete_7_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
