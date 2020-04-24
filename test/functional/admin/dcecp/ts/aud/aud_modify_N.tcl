#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: aud_modify_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:09:58  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:04  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:28  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:51:27  root]
# 
# Revision 1.1.2.1  1994/09/28  21:37:58  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:35  melman]
# 
# Revision 1.1.2.6  1994/09/02  20:28:30  rousseau
# 	Mass test cleanup.
# 	[1994/09/02  20:27:46  rousseau]
# 
# Revision 1.1.2.5  1994/06/10  20:02:27  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:34  devsrc]
# 
# Revision 1.1.2.4  1994/05/23  18:09:22  rousseau
# 	Various updates.
# 	[1994/05/23  18:08:56  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:26:50  rousseau
# 	Performance changes, and new API path (CR 10558).
# 	[1994/05/06  15:24:21  rousseau]
# 
# Revision 1.1.2.1  1994/04/21  20:28:33  rousseau
# 	Initial drop of audit tests (CR 9938).
# 	[1994/04/21  20:27:14  rousseau]
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
proc aud_modify_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc aud_modify_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}    


proc aud_modify_1_N {} {
    # Identify the test
    tet_infoline aud_modify_1_N: Check that no supplied arguments is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {aud modify} msg] $msg
    } compare_strings {1 {The command requires one of the '-state', '-stostrategy' or '-change' options.}}

    # Cleanup steps
}

proc aud_modify_2_N {} {
    global DCP_HOSTNAME_ONE

    # Identify the test
    tet_infoline aud_modify_2_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {aud modify /.:/hosts/$DCP_HOSTNAME_ONE/audit-server foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc aud_modify_3_N {} {
    global DCP_HOSTNAME_ONE

    # Identify the test
    tet_infoline aud_modify_3_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {aud modify /.:/hosts/$DCP_HOSTNAME_ONE/audit-server -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc aud_modify_4_N {} {
    # Identify the test
    tet_infoline aud_modify_4_N: Check the handling of the -stostrategy option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {aud modify -stostrategy foo} msg] $msg
    } compare_strings {1 {The value 'foo' for the 'stostrategy' attribute is not valid.}}

    # Cleanup steps
}

proc aud_modify_5_N {} {
    # Identify the test
    tet_infoline aud_modify_5_N: Check the handling of the -stostrategy option with no data.

    # Setup steps

    # Run the test
    test {
        list [catch {aud modify -stostrategy} msg] $msg
    } compare_strings {1 {The '-stostrategy' option requires a character string value.}}

    # Cleanup steps
}

proc aud_modify_8_N {} {
    # Identify the test
    tet_infoline aud_modify_8_N: Check the handling of the -state option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {aud modify -state foo} msg] $msg
    } compare_strings {1 {The value 'foo' for the 'state' attribute is not valid.}}

    # Cleanup steps
}

proc aud_modify_9_N {} {
    # Identify the test
    tet_infoline aud_modify_9_N: Check the handling of the -state option with no data.

    # Setup steps

    # Run the test
    test {
        list [catch {aud modify -state} msg] $msg
    } compare_strings {1 {The '-state' option requires a character string value.}}

    # Cleanup steps
}

proc aud_modify_12_N {} {
    # Identify the test
    tet_infoline aud_modify_12_N: Check the handling of the -change option with invalid data.

    # Setup steps

    # Run the test
    test {
        list [catch {aud modify -change foo} msg] $msg
    } compare_strings {1 {The list {foo} contains too few elements.}}

    # Cleanup steps
}

proc aud_modify_13_N {} {
    # Identify the test
    tet_infoline aud_modify_13_N: Check the handling of the -change option with no data.

    # Setup steps

    # Run the test
    test {
        list [catch {aud modify -change} msg] $msg
    } compare_strings {1 {The '-change' option requires a character string value.}}

    # Cleanup steps
}

proc aud_modify_14_N {} {
    # Identify the test
    tet_infoline aud_modify_14_N: Check that bad (short) lists to -change are trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {aud modify -change {{wrap}}} msg] $msg
    } compare_strings {1 {The list {wrap} contains too few elements.}}

    # Cleanup steps
}

proc aud_modify_15_N {} {
    # Identify the test
    tet_infoline aud_modify_15_N: Check that bad (long) lists to -change is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {aud modify -change {{wrap foo bar}}} msg] $msg
    } compare_strings {1 {The list {wrap foo bar} contains too many elements.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 "
set ic1 "aud_modify_1_N"
set ic2 "aud_modify_2_N"
set ic3 "aud_modify_3_N"
set ic4 "aud_modify_4_N"
set ic5 "aud_modify_5_N"
set ic6 "aud_modify_8_N"
set ic7 "aud_modify_9_N"
set ic8 "aud_modify_12_N"
set ic9 "aud_modify_13_N"
set ic10 "aud_modify_14_N"
set ic11 "aud_modify_15_N"
set tet_startup aud_modify_startup
set tet_cleanup aud_modify_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl

