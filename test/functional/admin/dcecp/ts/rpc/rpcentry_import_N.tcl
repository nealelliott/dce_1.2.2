#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: rpcentry_import_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:25:05  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:23  marty]
#
# Revision 1.1.4.1  1995/12/11  15:14:11  root
# 	Submit
# 	[1995/12/11  14:31:33  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:16  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:01  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:23  melman]
# 
# Revision 1.1.2.3  1994/08/18  21:16:23  coren
# 	Fixed error messages to match improvements in dcp.sams.
# 	[1994/08/18  20:01:30  coren]
# 
# Revision 1.1.2.2  1994/06/17  18:43:40  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:27:45  devsrc]
# 
# Revision 1.1.2.1  1994/05/19  20:02:35  melman
# 	initial rpccp tests
# 	[1994/05/19  19:59:44  melman]
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
proc rpcentry_import_1_N {} {
    # Identify the test
    tet_infoline rpcentry_import_1_N: Check that no supplied arguments is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcentry import} msg] $msg
    } compare_strings {1 {No RPC entry name given.}}

    # Cleanup steps
}

proc rpcentry_import_2_N {} {
    # Identify the test
    tet_infoline rpcentry_import_2_N: Check that -interface is required.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcentry import foo} msg] $msg
    } compare_strings {1 {Interface id not entered.}}

    # Cleanup steps
}

proc rpcentry_import_3_N {} {
    # Identify the test
    tet_infoline rpcentry_import_3_N: Check that -interface doesn't take a list.

    # Setup steps
    if {[dcp_wrap {global INTERFACE1 INTERFACE2}] != 0} {return;}

    # Run the test
    test {
        list [catch {rpcentry import /.:/foo -interface {$INTERFACE1 $INTERFACE2}} msg] $msg
    } compare_strings {1 {The interface specification does not contain a valid version identifier.}}

    # Cleanup steps
}

proc rpcentry_import_4_N {} {
    # Identify the test
    tet_infoline rpcentry_import_4_N: Check that -max requires an integer

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 INTERFACE1}] != 0} {return;}

    # Run the test
    test {
        list [catch {rpcentry import $RPCENTRY1 -interface $INTERFACE1 -max} msg] $msg
    } compare_strings {1 {The '-max' option requires an integer value.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "rpcentry_import_1_N"
set ic2 "rpcentry_import_2_N"
set ic3 "rpcentry_import_3_N"
set ic4 "rpcentry_import_4_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
