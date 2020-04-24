#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: rpcentry_delete_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:24:53  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:18  marty]
#
# Revision 1.1.4.1  1995/12/11  15:14:04  root
# 	Submit
# 	[1995/12/11  14:31:30  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:11  root]
# 
# Revision 1.1.2.1  1994/09/28  21:44:56  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:18  melman]
# 
# Revision 1.1.2.3  1994/09/01  21:13:45  rousseau
# 	Mass test cleanup
# 	[1994/09/01  21:13:13  rousseau]
# 
# Revision 1.1.2.2  1994/06/17  18:43:37  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:27:28  devsrc]
# 
# Revision 1.1.2.1  1994/05/19  20:02:28  melman
# 	initial rpccp tests
# 	[1994/05/19  19:59:39  melman]
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
proc rpcentry_delete_1_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_delete_1_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcentry delete foo} msg] $msg
    } compare_strings {1 {Incomplete name}}

    # Cleanup steps
}

proc rpcentry_delete_2_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_delete_2_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcentry delete foo -bar} msg] $msg
    } compare_strings {1 {Unrecognized option '-bar'.}}

    # Cleanup steps
}

proc rpcentry_delete_3_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_delete_3_N: Check that insufficient arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcentry delete} msg] $msg
    } compare_strings {1 {No RPC entry name given.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 "
set ic1 "rpcentry_delete_1_N"
set ic2 "rpcentry_delete_2_N"
set ic3 "rpcentry_delete_3_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
