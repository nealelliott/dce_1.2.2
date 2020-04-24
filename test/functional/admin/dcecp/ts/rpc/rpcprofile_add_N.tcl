#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: rpcprofile_add_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:25:52  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:40  marty]
#
# Revision 1.1.4.1  1995/12/11  15:14:49  root
# 	Submit
# 	[1995/12/11  14:31:47  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:35  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:32  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:42  melman]
# 
# Revision 1.1.2.3  1994/09/01  21:13:47  rousseau
# 	Mass test cleanup
# 	[1994/09/01  21:13:15  rousseau]
# 
# Revision 1.1.2.2  1994/06/17  18:43:53  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:28:51  devsrc]
# 
# Revision 1.1.2.1  1994/05/26  18:42:08  rousseau
# 	Initial submission.
# 	[1994/05/26  18:39:22  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc rpcprofile_add_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc rpcprofile_add_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc rpcprofile_add_1_N {} {
    # Identify the test
    tet_infoline rpcprofile_add_1_N: Check that no supplied arguments is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile add} msg] $msg
    } compare_strings {1 {No RPC profile name given.}}

    # Cleanup steps
}

proc rpcprofile_add_2_N {} {
    # Identify the test
    tet_infoline rpcprofile_add_2_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile add foo bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'bar'.}}

    # Cleanup steps
}

proc rpcprofile_add_3_N {} {
    # Identify the test
    tet_infoline rpcprofile_add_3_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile add foo -bar} msg] $msg
    } compare_strings {1 {Unrecognized option '-bar'.}}

    # Cleanup steps
}

proc rpcprofile_add_4_N {} {
    # Identify the test
    tet_infoline rpcprofile_add_4_N: Check that not specifying an argument is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile add -member /.:/foo} msg] $msg
    } compare_strings {1 {No RPC profile name given.}}

    # Cleanup steps
}

proc rpcprofile_add_5_N {} {
    # Identify the test
    tet_infoline rpcprofile_add_5_N: Check that not specifying -member is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile add /.:/rpcp_add_5_N} msg] $msg
    } compare_strings {1 {No member name given.}}

    # Cleanup steps
}

proc rpcprofile_add_6_N {} {
    # Identify the test
    tet_infoline rpcprofile_add_6_N: Check that no -interface or -default is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile add /.:/rpcp_add_6_N -member /.:/foo} msg] $msg
    } compare_strings {1 {Must use '-interface' or '-default'.}}

    # Cleanup steps
}

proc rpcprofile_add_7_N {} {
    # Identify the test
    tet_infoline rpcprofile_add_7_N: Check that using -default -and -interface is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile add /.:/rpcp_add_7_N -member /.:/foo -default -interface {00205383-5fa0-1de2-a683-0000c03adf56 1.0}} msg] $msg
    } compare_strings {1 {The '-default' option cannot be combined with other options.}}

    # Cleanup steps
}

proc rpcprofile_add_8_N {} {
    # Identify the test
    tet_infoline rpcprofile_add_8_N: Check that using -default -and -priority is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile add /.:/rpcp_add_8_N -member /.:/foo -default -priority 1} msg] $msg
    } compare_strings {1 {The '-default' option cannot be combined with other options.}}

    # Cleanup steps
}

proc rpcprofile_add_9_N {} {
    # Identify the test
    tet_infoline rpcprofile_add_9_N: Check that using -default -and -annotation is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile add /.:/rpcp_add_9_N -member /.:/foo -default -annotation foo} msg] $msg
    } compare_strings {1 {The '-default' option cannot be combined with other options.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 "
set ic1 "rpcprofile_add_1_N"
set ic2 "rpcprofile_add_2_N"
set ic3 "rpcprofile_add_3_N"
set ic4 "rpcprofile_add_4_N"
set ic5 "rpcprofile_add_5_N"
set ic6 "rpcprofile_add_6_N"
set ic7 "rpcprofile_add_7_N"
set ic8 "rpcprofile_add_8_N"
set ic9 "rpcprofile_add_9_N"
set tet_startup rpcprofile_add_startup
set tet_cleanup rpcprofile_add_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
