#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: rpcentry_unexport_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:25:15  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:26  marty]
#
# Revision 1.1.4.1  1995/12/11  15:14:21  root
# 	Submit
# 	[1995/12/11  14:31:36  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:21  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:10  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:28  melman]
# 
# Revision 1.1.2.3  1994/08/18  21:16:26  coren
# 	Fixed error messages to match improvements in dcp.sams.
# 	[1994/08/18  20:01:59  coren]
# 
# Revision 1.1.2.2  1994/06/17  18:43:44  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:28:01  devsrc]
# 
# Revision 1.1.2.1  1994/05/19  20:02:40  melman
# 	initial rpccp tests
# 	[1994/05/19  19:59:49  melman]
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
proc rpcentry_unexport_1_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_unexport_1_N: Check that no supplied arguments is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {rpcentry unexport} msg] $msg
    } compare_strings {1 {No RPC profile name given.}}

    # Cleanup steps
}

proc rpcentry_unexport_2_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_unexport_2_N: Check that -interface is required.

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1}] != 0} {return;}

    # Run the test
    test {
        list [catch {rpcentry unexport $RPCENTRY1} msg] $msg
    } compare_strings {1 {Must use either an object UUID or an interface id.}}

    # Cleanup steps
}

proc rpcentry_unexport_3_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_unexport_3_N: Check that a bad value for -version fails.

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 INTERFACE1}] != 0} {return;}

    # Run the test
    test {
        list [catch {rpcentry unexport $RPCENTRY1 -interface $INTERFACE1 -version bad} msg] $msg
    } compare_strings {1 {The value 'bad' is not a valid version identifier.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 "
set ic1 "rpcentry_unexport_1_N"
set ic2 "rpcentry_unexport_2_N"
set ic3 "rpcentry_unexport_3_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
