#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: keytab_remove_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:16:59  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:14  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:33  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/06/08  13:50 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts33/1  1995/06/08  13:50 UTC  truitt
# 	CHFts15424: Change some tests to support new dced acl behavior.
# 	[1995/12/11  15:53:36  root]
# 
# Revision 1.1.2.1  1994/09/28  21:40:34  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:48  melman]
# 
# Revision 1.1.2.6  1994/06/22  20:37:17  baum
# 	OT #11022 - Use tetexec.cfg for cell/host.  Use tet_setup
# 	[1994/06/22  20:36:10  baum]
# 
# Revision 1.1.2.5  1994/06/10  20:13:22  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:39  devsrc]
# 
# Revision 1.1.2.4  1994/05/25  22:43:02  baum
# 	Keytab tests now dce_login/out on their own.  Also, corrected some of the output errors
# 	[1994/05/25  22:41:41  baum]
# 
# Revision 1.1.2.3  1994/05/19  16:42:23  baum
# 	Removed startup/cleanup settings
# 	[1994/05/19  16:42:03  baum]
# 
# Revision 1.1.2.2  1994/05/11  19:37:47  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:22:06  rousseau]
# 
# Revision 1.1.2.1  1994/05/10  22:19:55  baum
# 	INitial Checkin
# 	[1994/05/10  22:19:10  baum]
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
proc keytab_remove_11_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_vars.tcl

    # Identify the test
    tet_infoline keytab_remove_11_P: Check to see that we can delete a particular version key.

    # Setup steps
    tet_setup

    # Run the test
    set cmd "keytab remove $KTB2_LONG_NAME -member $TEST_PRINC2 -version 0"
    set ret [catch "$cmd" msg]
    if { $ret != 0 } {
      tet_infoline "Could not remove KEYTAB key!"
      tet_infoline "Actual Result:"
      tet_infoline $msg
      tet_result FAIL
    } 

    # Verify key is gone by doing a list
    set cmd2 "keytab list $KTB2_LONG_NAME"
    set ret [catch "$cmd2" msg]
    if { $ret != 0 } {
      tet_infoline "Problem: Keytab list to check for removed key fails!!"
      tet_result FAIL
    }
    compare_and_report $cmd $ret $msg $KTB2_LIST_AFTER_DEL_OUTPUT

    # Cleanup steps
    tet_cleanup
}

proc keytab_remove_12_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_vars.tcl

    # Identify the test
    tet_infoline keytab_remove_12_P: Check to see that we can delete all versions of a key.

    # Setup steps
    tet_setup

    # Run the test
    set cmd "keytab remove $KTB2_LONG_NAME -member $TEST_PRINC2"
    set ret [catch "$cmd" msg]
    if { $ret != 0 } {
      tet_infoline "Could not remove KEYTAB key!"
      tet_infoline "Actual Result:"
      tet_infoline $msg
      tet_result FAIL
    } 

    # Verify key is gone by doing a list
    set cmd2 "keytab list $KTB2_LONG_NAME"
    set ret [catch "$cmd2" msg]
    if { $ret != 0 } {
      tet_infoline "Problem: Keytab list to check for removed key fails!!"
      tet_result FAIL
    }
    compare_and_report $cmd $ret $msg $KTB2_LIST_AFTER_DEL_OUTPUT

    # Cleanup steps
    tet_cleanup
}

proc keytab_remove_13_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_vars.tcl

    # Identify the test
    tet_infoline keytab_remove_13_P: Check to see that we can delete a plain type key.

    # Setup steps
    tet_setup

    # Run the test
    set cmd "keytab remove $KTB2_LONG_NAME -member $TEST_PRINC2 -type plain"
    set ret [catch "$cmd" msg]
    if { $ret != 0 } {
      tet_infoline "Could not remove KEYTAB key!"
      tet_infoline "Actual Result:"
      tet_infoline $msg
      tet_result FAIL
    } 

    # Verify key is gone by doing a list
    set cmd2 "keytab list $KTB2_LONG_NAME"
    set ret [catch "$cmd2" msg]
    if { $ret != 0 } {
      tet_infoline "Problem: Keytab list to check for removed key fails!!"
      tet_result FAIL
    }
    compare_and_report $cmd $ret $msg $KTB2_LIST_AFTER_DEL_OUTPUT

    # Cleanup steps
    tet_cleanup
}

proc keytab_remove_14_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_vars.tcl

    # Identify the test
    tet_infoline keytab_remove_14_P: Check to see that we can delete a des type key.

    # Setup steps, also add a 'des' type key for TEST_PRINC2
    tet_setup
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    set ret [catch "account delete $TEST_PRINC2" msg]
    if { $ret != 0 } {
      tet_infoline "Could not remove TEST_PRINC2"
      tet_infoline $msg
    }

    set ret [catch "account create $TEST_PRINC2 -attr {{organization osf} {password $TEST_PRINC2} {group osf} {mypwd $DCP_CLIENT_PW}}" msg]
    if { $ret != 0 } {
      tet_infoline "Could not re-add TEST_PRINC2"
      tet_infoline $msg
    }
    if {[dcp_wrap {logout}] != 0} {return;}

    set cmd "keytab add $KTB2_LONG_NAME -member $TEST_PRINC2 -random -registry"
    set ret [catch "$cmd" msg]
    if { $ret != 0 } {
      tet_infoline "Could not add des key for 'remove' test setup!"
      tet_infoline "Actual Result:"
      tet_infoline $msg
      tet_result FAIL
    } 

    # Run the test
    set cmd "keytab remove $KTB2_LONG_NAME -member $TEST_PRINC2 -type des"
    set ret [catch "$cmd" msg]
    if { $ret != 0 } {
      tet_infoline "Could not remove KEYTAB key!"
      tet_infoline "Actual Result:"
      tet_infoline $msg
      tet_result FAIL
    } 

    # Verify key is gone by doing a list
    set cmd2 "keytab list $KTB2_LONG_NAME"
    set ret [catch "$cmd2" msg]
    if { $ret != 0 } {
      tet_infoline "Problem: Keytab list to check for removed key fails!!"
      tet_result FAIL
    }
    compare_and_report $cmd $ret $msg $KTB2_LIST_AFTER_DEL_OUTPUT
 
    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    set ret [catch "account delete $TEST_PRINC2" msg]
    if { $ret != 0 } {
      tet_infoline "Could not remove TEST_PRINC2"
      tet_infoline $msg
    }

    set ret [catch "account create $TEST_PRINC2 -attr {{organization osf} {password $TEST_PRINC2} {group osf} {mypwd $DCP_CLIENT_PW}}" msg]
    if { $ret != 0 } {
      tet_infoline "Could not re-add TEST_PRINC2"
      tet_infoline $msg
    }
    if {[dcp_wrap {logout}] != 0} {return;}
    tet_cleanup
}

set iclist "ic1 ic2 ic3 ic4"
set ic1 "keytab_remove_11_P"
set ic2 "keytab_remove_12_P"
set ic3 "keytab_remove_13_P"
set ic4 "keytab_remove_14_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.

source $TET_ROOT/lib/tcl/tcm.tcl
