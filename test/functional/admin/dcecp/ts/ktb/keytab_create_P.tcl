#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: keytab_create_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:16:45  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:09  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:28  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:30  root]
# 
# Revision 1.1.2.1  1994/09/28  21:40:26  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:38  melman]
# 
# Revision 1.1.2.6  1994/06/22  20:37:14  baum
# 	OT #11022 - Use tetexec.cfg for cell/host.  Use tet_setup
# 	[1994/06/22  20:36:07  baum]
# 
# Revision 1.1.2.5  1994/06/10  20:13:17  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:27  devsrc]
# 
# Revision 1.1.2.4  1994/05/26  02:44:12  baum
# 	Removed VERBOSE
# 	[1994/05/26  02:43:48  baum]
# 
# Revision 1.1.2.3  1994/05/25  22:42:57  baum
# 	Keytab tests now dce_login/out on their own.  Also, corrected some of the output errors
# 	[1994/05/25  22:41:36  baum]
# 
# Revision 1.1.2.2  1994/05/11  19:37:36  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:21:42  rousseau]
# 
# Revision 1.1.2.1  1994/05/10  22:19:48  baum
# 	INitial Checkin
# 	[1994/05/10  22:19:02  baum]
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

proc tet_setup {} {

  global DCP_CLIENT DCP_CLIENT_PW DO_DCE_LOGIN TET_ROOT

  source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_vars.tcl

  if { $DO_DCE_LOGIN == 1 } {
#    puts stdout "LOGIN"
    dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}
  }

  # Make sure ktb1 does and ktb2 does not exist
  set ret [create_ktb1]

  # First delete old one, if its just a storage, latch onto it.
  set cmd "keytab create $KTB2_LONG_NAME -attr {{storage $KTB2_STORAGE} {uuid $KTB2_UUID} } -entry"
  set ret [catch "$cmd" msg]

  set cmd "keytab delete $KTB2_LONG_NAME"
  set ret [catch "$cmd" msg]
}
  
proc tet_cleanup {} { ktb_std_cleanup }


## Test purposes
proc keytab_create_11_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_vars.tcl

    # Identify the test
    tet_infoline keytab_create_11_P: Check to see that we can create a keytab entry and object using -attr syntax

    # Setup steps
    tet_setup

    # Run the test
    set cmd "keytab create $KTB2_LONG_NAME -attr {{storage $KTB2_STORAGE} {data {$TEST_PRINC1 plain 1 test_princ1}} {uuid $KTB2_UUID} }"
    set ret [catch "$cmd" msg]
    if { $ret != 0 } {
      tet_infoline "Problem: keytab create FAILS"
      tet_infoline $msg
      tet_result FAIL
      tet_cleanup
      return
    }

    # Look for the file $KTB2_STORAGE, it should exists
    set it_exists [file exists $KTB2_STORAGE]
    if { $it_exists == 0 } {
       tet_infoline "Problem: create KEYTAB succeeds, yet storage file is missing!"
      tet_result FAIL
      tet_cleanup
      return
    }
    compare_and_report $cmd $ret $msg ""

    # Cleanup steps
    tet_cleanup
}

proc keytab_create_12_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_vars.tcl

    # Identify the test
    tet_infoline keytab_create_12_P: Check to see that we can create a keytab  entry specifying storage from an existing keytab.

    # Setup steps
    tet_setup

    # Run the test
    set cmd "keytab create $KTB2_LONG_NAME -attr {{storage $KTB1_STORAGE} {uuid $KTB2_UUID} } -entry"
    set ret [catch "$cmd" msg]

    compare_and_report $cmd $ret $msg ""

    # Cleanup, careful not to remove KTB1 storage!
    set cmd "keytab delete $KTB2_LONG_NAME -entry"
    set ret [catch "$cmd" msg]
    tet_cleanup
}

proc keytab_create_13_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_vars.tcl

    # Identify the test
    tet_infoline keytab_create_13_P: Check to see that we can create a keytab entry and pbject not using attr syntax

    # Setup steps
    tet_setup

    # Run the test
    set cmd "keytab create $KTB2_LONG_NAME -storage $KTB2_STORAGE -data {$TEST_PRINC1 plain 1 test_princ1} -uuid $KTB2_UUID"
    set ret [catch "$cmd" msg]
    if { $ret != 0 } {
      tet_infoline "Problem: keytab create FAILS"
      tet_infoline $msg
      tet_result FAIL
      tet_cleanup
      return
    }

    # Look for the file $KTB2_STORAGE, it should exist
    set it_exists [file exists $KTB2_STORAGE]
    if { $it_exists == 0 } {
       tet_infoline "Problem: create KEYTAB succeeds, yet storage file is missing!"
      tet_result FAIL
      tet_cleanup
      return
    }
    compare_and_report $cmd $ret $msg ""

    # Cleanup steps
    tet_cleanup
}

set iclist "ic1 ic2 ic3 "
set ic1 "keytab_create_11_P"
set ic2 "keytab_create_12_P"
set ic3 "keytab_create_13_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
