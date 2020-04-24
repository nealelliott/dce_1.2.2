#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: keytab_show_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:17:04  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:16  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:35  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:38  root]
# 
# Revision 1.1.2.1  1994/09/28  21:40:37  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:52  melman]
# 
# Revision 1.1.2.6  1994/06/22  20:37:18  baum
# 	OT #11022 - Use tetexec.cfg for cell/host.  Use tet_setup
# 	[1994/06/22  20:36:11  baum]
# 
# Revision 1.1.2.5  1994/06/10  20:13:24  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:41  devsrc]
# 
# Revision 1.1.2.4  1994/06/07  23:13:53  baum
# 	Mods to handle new dcecp output (CR #10761)
# 	[1994/06/07  23:13:24  baum]
# 
# Revision 1.1.2.3  1994/05/19  16:42:24  baum
# 	Removed startup/cleanup settings
# 	[1994/05/19  16:42:04  baum]
# 
# Revision 1.1.2.2  1994/05/11  19:37:50  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:22:14  rousseau]
# 
# Revision 1.1.2.1  1994/05/10  22:19:57  baum
# 	INitial Checkin
# 	[1994/05/10  22:19:13  baum]
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
proc keytab_show_6_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW 

    source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_vars.tcl

    # Identify the test
    tet_infoline keytab_show_6_P: Check to see that we can show keytab entry only.

    # Setup steps
    tet_setup

    # Run the test
    set cmd "keytab show $KTB1_LONG_NAME -entry"
    set ret [catch "$cmd" msg]
    set expected_results [list $KTB1_UUID $KTB1_STORAGE $KTB1_ANNOTATION]
    compare_list_and_report $cmd $ret $msg $expected_results

    # Cleanup steps
    tet_cleanup
}

proc keytab_show_7_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_vars.tcl

    # Identify the test
    tet_infoline keytab_show_7_P: Check to see that we can show keytab objects attribute only.

    # Setup steps
    tet_setup

    # Run the test
    set cmd "keytab show $KTB1_LONG_NAME -members"
    set ret [catch "$cmd" msg]
    set expected_results [list $TEST_PRINC1 $TEST_PRINC2]
    compare_list_and_report $cmd $ret $msg $expected_results

    # Cleanup steps
    tet_cleanup
}

proc keytab_show_8_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_vars.tcl

    # Identify the test
    tet_infoline keytab_show_8_P: Check to see that we can show keytab objects.

    # Setup steps
    tet_setup

    # Run the test
    set cmd "keytab show $KTB1_LONG_NAME"
    set ret [catch "$cmd" msg]
    set expected_results [list $KTB1_UUID $KTB1_STORAGE $KTB1_ANNOTATION $TEST_PRINC1 $TEST_PRINC2]
    compare_list_and_report $cmd $ret $msg $expected_results

    # Cleanup steps
    tet_cleanup
}


set iclist "ic1 ic2 ic3 "
set ic1 "keytab_show_6_P"
set ic2 "keytab_show_7_P"
set ic3 "keytab_show_8_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
