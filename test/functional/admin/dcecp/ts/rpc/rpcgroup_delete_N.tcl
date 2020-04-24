#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: rpcgroup_delete_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:25:32  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:34  marty]
#
# Revision 1.1.4.1  1995/12/11  15:14:32  root
# 	Submit
# 	[1995/12/11  14:31:41  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:27  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:20  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:33  melman]
# 
# Revision 1.1.2.2  1994/06/17  18:43:48  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:28:22  devsrc]
# 
# Revision 1.1.2.1  1994/05/31  21:40:14  rousseau
# 	Initial submission.
# 	[1994/05/31  21:39:28  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc rpcgroup_delete_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc rpcgroup_delete_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc rpcgroup_delete_1_N {} {
    # Identify the test
    tet_infoline rpcgroup_delete_1_N: Check that no supplied arguments is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcgroup delete} msg] $msg
    } compare_strings {1 {No RPC group name given.}}

    # Cleanup steps
}

proc rpcgroup_delete_2_N {} {
    # Identify the test
    tet_infoline rpcgroup_delete_2_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcgroup delete foo bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'bar'.}}

    # Cleanup steps
}

proc rpcgroup_delete_3_N {} {
    # Identify the test
    tet_infoline rpcgroup_delete_3_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcgroup delete foo -bar} msg] $msg
    } compare_strings {1 {Unrecognized option '-bar'.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 "
set ic1 "rpcgroup_delete_1_N"
set ic2 "rpcgroup_delete_2_N"
set ic3 "rpcgroup_delete_3_N"
set tet_startup rpcgroup_delete_startup
set tet_cleanup rpcgroup_delete_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
