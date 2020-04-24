#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: keytab_add_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:16:35  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:06  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:24  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/06/08  14:05 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts33/1  1995/06/08  14:04 UTC  truitt
# 	CHFts15424: Change some tests to support new dced acl behavior.
# 	[1995/12/11  15:53:27  root]
# 
# Revision 1.1.2.1  1994/09/28  21:40:21  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:33  melman]
# 
# Revision 1.1.2.6  1994/06/22  20:37:11  baum
# 	OT #11022 - Use tetexec.cfg for cell/host.  Use tet_setup
# 	[1994/06/22  20:36:04  baum]
# 
# Revision 1.1.2.5  1994/06/10  20:13:13  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:23  devsrc]
# 
# Revision 1.1.2.4  1994/06/07  23:13:48  baum
# 	Mods to handle new dcecp output (CR #10761)
# 	[1994/06/07  23:13:19  baum]
# 
# Revision 1.1.2.3  1994/05/25  22:42:54  baum
# 	Keytab tests now dce_login/out on their own.  Also, corrected some of the output errors
# 	[1994/05/25  22:41:32  baum]
# 
# Revision 1.1.2.2  1994/05/11  19:37:32  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:21:21  rousseau]
# 
# Revision 1.1.2.1  1994/05/10  22:19:44  baum
# 	INitial Checkin
# 	[1994/05/10  22:18:56  baum]
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
proc keytab_add_14_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_vars.tcl

    # Identify the test
    tet_infoline keytab_add_14_P: Check to see that we can add random key.

    # Setup steps, remove and re-add principal
    tet_setup
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    set ret [catch "account delete $TEST_PRINC1" msg]
    if { $ret != 0 } {
      tet_infoline "Could not remove TEST_PRINC1"
      tet_infoline $msg
    }

    set ret [catch "account create $TEST_PRINC1 -attr {{organization osf} {password $TEST_PRINC1} {group osf} {mypwd $DCP_CLIENT_PW}}" msg]
    if { $ret != 0 } {
      tet_infoline "Could not re-add TEST_PRINC1"
      tet_infoline $msg
    }
    if {[dcp_wrap {logout}] != 0} {return;}
    
    # Run the test
    set cmd "keytab add $KTB2_LONG_NAME -member $TEST_PRINC1 -random -registry"
    set ret [catch "$cmd" msg]
    if { $ret != 0 } {
       tet_infoline "Could not add key ($msg)"
    }

    # Check that the key is in there
    set cmd2 "keytab list $KTB2_LONG_NAME"
    set ret2 [catch "$cmd2" msg]
    set expected_output [list $TEST_PRINC1 $TEST_PRINC2]
    compare_list_and_report $cmd $ret $msg $expected_output
    
    # Cleanup steps, remove and re-add registry principals affected

    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    set ret [catch "account delete $TEST_PRINC1" msg]
    if { $ret != 0 } {
      tet_infoline "Could not remove TEST_PRINC1"
      tet_infoline $msg
    }

    set ret [catch "account create $TEST_PRINC1 -attr {{organization osf} {password $TEST_PRINC1} {group osf} {mypwd $DCP_CLIENT_PW}}" msg]
    if { $ret != 0 } {
      tet_infoline "Could not re-add TEST_PRINC1"
      tet_infoline $msg
    }
    if {[dcp_wrap {logout}] != 0} {return;}
    tet_cleanup
}

proc keytab_add_15_P {} {

    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_vars.tcl

    # Identify the test
    tet_infoline keytab_add_15_P: Check to see that we can add a plain key.

    # Setup steps, create a key tab
    tet_setup

    # Run the test
    set cmd "keytab add $KTB2_LONG_NAME -member $DCP_CLIENT -key anything -version 1"
    set ret [catch "$cmd" msg]
    if { $ret != 0 } {
       tet_infoline "Could not add key ($msg)"
    }

    # Check that the key is in there
    set cmd2 "keytab list $KTB2_LONG_NAME"
    set ret [catch "$cmd2" msg]
    set expected_results [list $DCP_CLIENT]
    compare_list_and_report $cmd $ret $msg $expected_results
    
    # Cleanup steps
    tet_cleanup

}


set iclist "ic1 ic2 "
set ic1 "keytab_add_14_P"
set ic2 "keytab_add_15_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.

source $TET_ROOT/lib/tcl/tcm.tcl
