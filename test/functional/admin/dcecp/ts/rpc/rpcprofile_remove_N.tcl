#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: rpcprofile_remove_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:26:17  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:48  marty]
#
# Revision 1.1.4.1  1995/12/11  15:15:10  root
# 	Submit
# 	[1995/12/11  14:31:55  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:46  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:43  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:51  melman]
# 
# Revision 1.1.2.3  1994/09/01  21:13:52  rousseau
# 	Mass test cleanup
# 	[1994/09/01  21:13:18  rousseau]
# 
# Revision 1.1.2.2  1994/06/17  18:44:01  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:29:24  devsrc]
# 
# Revision 1.1.2.1  1994/05/26  18:42:16  rousseau
# 	Initial submission.
# 	[1994/05/26  18:39:29  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc rpcprofile_remove_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc rpcprofile_remove_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc rpcprofile_remove_1_N {} {
    # Identify the test
    tet_infoline rpcprofile_remove_1_N: Check that no supplied arguments is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile remove} msg] $msg
    } compare_strings {1 {No RPC profile name given.}}

    # Cleanup steps
}

proc rpcprofile_remove_2_N {} {
    # Identify the test
    tet_infoline rpcprofile_remove_2_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile remove foo bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'bar'.}}

    # Cleanup steps
}

proc rpcprofile_remove_3_N {} {
    # Identify the test
    tet_infoline rpcprofile_remove_3_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile remove foo -bar} msg] $msg
    } compare_strings {1 {Unrecognized option '-bar'.}}

    # Cleanup steps
}

proc rpcprofile_remove_4_N {} {
    # Identify the test
    tet_infoline rpcprofile_remove_4_N: Check that not specifying an argument is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile remove -member /.:/foo} msg] $msg
    } compare_strings {1 {No RPC profile name given.}}

    # Cleanup steps
}

proc rpcprofile_remove_5_N {} {
    # Identify the test
    tet_infoline rpcprofile_remove_5_N: Check that not specifying -member is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile remove /.:/rpcp_remove_5_N} msg] $msg
    } compare_strings {1 {No member name given.}}

    # Cleanup steps
}

proc rpcprofile_remove_6_N {} {
    # Identify the test
    tet_infoline rpcprofile_remove_6_N: Check that no -interface or -default is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile remove /.:/rpcp_remove_6_N -member /.:/foo} msg] $msg
    } compare_strings {1 {Must use '-interface' or '-default'.}}

    # Cleanup steps
}

proc rpcprofile_remove_7_N {} {
    # Identify the test
    tet_infoline rpcprofile_remove_7_N: Check that using -default with any other option is trapped as 
    tet_infoline rpcprofile_remove_7_N: an error (-interface).

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile remove /.:/rpcp_remove_7_N -default -interface [list [uuid create] 1.0]} msg] $msg
    } compare_strings {1 {The '-default' option cannot be combined with other options.}}

    # Cleanup steps
}

proc rpcprofile_remove_8_N {} {
    # Identify the test
    tet_infoline rpcprofile_remove_8_N: Check that using -default with any other option is trapped as 
    tet_infoline rpcprofile_remove_8_N: an error (-member).

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile remove /.:/rpcp_remove_8_N -default -member /.:/foo} msg] $msg
    } compare_strings {1 {The '-default' option cannot be combined with other options.}}

    # Cleanup steps
}

proc rpcprofile_remove_9_N {} {
    # Identify the test
    tet_infoline rpcprofile_remove_9_N: Check that using -default with any other option is trapped as 
    tet_infoline rpcprofile_remove_9_N: an error (-annotation).

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile remove /.:/rpcp_remove_9_N -default -annotation foo} msg] $msg
    } compare_strings {1 {The '-default' option cannot be combined with other options.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 "
set ic1 "rpcprofile_remove_1_N"
set ic2 "rpcprofile_remove_2_N"
set ic3 "rpcprofile_remove_3_N"
set ic4 "rpcprofile_remove_4_N"
set ic5 "rpcprofile_remove_5_N"
set ic6 "rpcprofile_remove_6_N"
set ic7 "rpcprofile_remove_7_N"
set ic8 "rpcprofile_remove_8_N"
set ic9 "rpcprofile_remove_9_N"
set tet_startup rpcprofile_remove_startup
set tet_cleanup rpcprofile_remove_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
