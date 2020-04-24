#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: rpcentry_show_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:25:10  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:25  marty]
#
# Revision 1.1.4.1  1995/12/11  15:14:16  root
# 	Submit
# 	[1995/12/11  14:31:35  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:18  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:05  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:25  melman]
# 
# Revision 1.1.2.2  1994/06/17  18:43:42  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:27:53  devsrc]
# 
# Revision 1.1.2.1  1994/05/19  20:02:37  melman
# 	initial rpccp tests
# 	[1994/05/19  19:59:47  melman]
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
proc rpcentry_show_1_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_show_1_N: Check that no supplied arguments is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcentry show} msg] $msg
    } compare_strings {1 {No RPC entry name given.}}

    # Cleanup steps
}

proc rpcentry_show_2_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_show_2_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcentry show foo bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'bar'.}}

    # Cleanup steps
}

proc rpcentry_show_3_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_show_3_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcentry show foo -bar} msg] $msg
    } compare_strings {1 {Unrecognized option '-bar'.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 "
set ic1 "rpcentry_show_1_N"
set ic2 "rpcentry_show_2_N"
set ic3 "rpcentry_show_3_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
