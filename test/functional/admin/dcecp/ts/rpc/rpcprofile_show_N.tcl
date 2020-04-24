#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: rpcprofile_show_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:26:22  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:50  marty]
#
# Revision 1.1.4.1  1995/12/11  15:15:13  root
# 	Submit
# 	[1995/12/11  14:31:57  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:49  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:45  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:53  melman]
# 
# Revision 1.1.2.3  1994/09/01  21:13:55  rousseau
# 	Mass test cleanup
# 	[1994/09/01  21:13:21  rousseau]
# 
# Revision 1.1.2.2  1994/06/17  18:44:02  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:29:32  devsrc]
# 
# Revision 1.1.2.1  1994/05/26  18:42:18  rousseau
# 	Initial submission.
# 	[1994/05/26  18:39:30  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc rpcprofile_show_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc rpcprofile_show_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc rpcprofile_show_1_N {} {
    # Identify the test
    tet_infoline rpcprofile_show_1_N: Check that no supplied arguments is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile show} msg] $msg
    } compare_strings {1 {No RPC profile name given.}}

    # Cleanup steps
}

proc rpcprofile_show_2_N {} {
    # Identify the test
    tet_infoline rpcprofile_show_2_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile show foo bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'bar'.}}

    # Cleanup steps
}

proc rpcprofile_show_3_N {} {
    # Identify the test
    tet_infoline rpcprofile_show_3_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile show foo -bar} msg] $msg
    } compare_strings {1 {Unrecognized option '-bar'.}}

    # Cleanup steps
}

proc rpcprofile_show_4_N {} {
    # Identify the test
    tet_infoline rpcprofile_show_4_N: Check that a bad value for -version fails.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile show /.:/rpcp_show_4_N -interface [list [uuid create] 1.0] -version bad} msg] $msg
    } compare_strings {1 {The value 'bad' is not a valid version identifier.}}

    # Cleanup steps
}

proc rpcprofile_show_5_N {} {
    # Identify the test
    tet_infoline rpcprofile_show_5_N: Check that -version requires -interface.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile show /.:/rpcp_show_5_N -version exact} msg] $msg
    } compare_strings {1 {The '-version' option requires the '-interface' option.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 "
set ic1 "rpcprofile_show_1_N"
set ic2 "rpcprofile_show_2_N"
set ic3 "rpcprofile_show_3_N"
set ic4 "rpcprofile_show_4_N"
set ic5 "rpcprofile_show_5_N"
set tet_startup rpcprofile_show_startup
set tet_cleanup rpcprofile_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
