#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: rpcprofile_list_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:26:12  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:47  marty]
#
# Revision 1.1.4.1  1995/12/11  15:15:06  root
# 	Submit
# 	[1995/12/11  14:31:54  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:44  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:40  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:49  melman]
# 
# Revision 1.1.2.2  1994/06/17  18:43:59  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:29:15  devsrc]
# 
# Revision 1.1.2.1  1994/05/26  18:42:14  rousseau
# 	Initial submission.
# 	[1994/05/26  18:39:27  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc rpcprofile_list_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc rpcprofile_list_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc rpcprofile_list_1_N {} {
    # Identify the test
    tet_infoline rpcprofile_list_1_N: Check that no supplied arguments is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile list} msg] $msg
    } compare_strings {1 {No RPC profile name given.}}

    # Cleanup steps
}

proc rpcprofile_list_2_N {} {
    # Identify the test
    tet_infoline rpcprofile_list_2_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile list foo bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'bar'.}}

    # Cleanup steps
}

proc rpcprofile_list_3_N {} {
    # Identify the test
    tet_infoline rpcprofile_list_3_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcprofile list foo -bar} msg] $msg
    } compare_strings {1 {Unrecognized option '-bar'.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 "
set ic1 "rpcprofile_list_1_N"
set ic2 "rpcprofile_list_2_N"
set ic3 "rpcprofile_list_3_N"
set tet_startup rpcprofile_list_startup
set tet_cleanup rpcprofile_list_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
