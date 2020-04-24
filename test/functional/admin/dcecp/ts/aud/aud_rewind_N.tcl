#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: aud_rewind_N.tcl,v $
# Revision 1.1.2.3  1996/03/11  02:10:08  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:06  marty]
#
# Revision 1.1.2.2  1995/12/13  15:24:30  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/03/02  19:21 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts17/1  1995/03/02  19:20 UTC  truitt
# 	New tests for audit rewind
# 	[1995/12/11  15:51:29  root]
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
proc aud_rewind_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc aud_rewind_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}


proc aud_rewind_1_N {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline aud_show_1_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {aud rewind /.:/hosts/$DCP_HOSTNAME_ONE/audit-server gumby} msg] $msg
    } compare_strings {1 {Unrecognized argument 'gumby'.}}

    # Cleanup steps
}

proc aud_rewind_2_N {} {
    global DCP_HOSTNAME_ONE
    # Identify the test
    tet_infoline aud_show_2_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {aud rewind /.:/hosts/$DCP_HOSTNAME_ONE/audit-server -gumby} msg] $msg
    } compare_strings {1 {Unrecognized option '-gumby'.}}

    # Cleanup steps
}

proc aud_rewind_3_N {} {
    # Identify the test
    tet_infoline aud_show_3_N: See that an incomplete name is trapped.

    # Setup steps

    # Run the test
    test {
        list [catch {aud rewind /opt/dcelocal/var/audit} msg] $msg
    } compare_strings {1 {Incomplete name}}

    # Cleanup steps
}

proc aud_rewind_4_N {} {
    # Identify the test
    tet_infoline aud_show_4_N: Check that a bogus name is trapped.

    # Setup steps

    # Run the test
    test {
        list [catch {aud rewind /.:/sec/audit} msg] $msg
    } compare_strings {1 {Entry not found}}

    # Cleanup steps
}

proc aud_rewind_5_N {} {
    # Identify the test
    tet_infoline aud_show_5_N: Check that a bogus name is trapped.

    # Setup steps

    # Run the test
    test {
        list [catch {aud rewind /.:/sec} msg] $msg
    } compare_strings {1 {No more bindings}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 "
set ic1 "aud_rewind_1_N"
set ic2 "aud_rewind_2_N"
set ic3 "aud_rewind_3_N"
set ic4 "aud_rewind_4_N"
set ic5 "aud_rewind_5_N"
set tet_startup aud_rewind_startup
set tet_cleanup aud_rewind_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
