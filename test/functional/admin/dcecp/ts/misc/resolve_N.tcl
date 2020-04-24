#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: resolve_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:18:03  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:42  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:58  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/09/29  13:23 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts47/1  1995/09/29  13:22 UTC  truitt
# 	CHFts14292: Improvement in tests.
# 	[1995/12/11  15:53:58  root]
# 
# Revision 1.1.2.1  1994/09/28  21:41:10  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:23  melman]
# 
# Revision 1.1.2.3  1994/09/27  18:27:57  kevins
# 	OT 12356 modified error messages in negative tests.
# 	[1994/09/27  18:27:48  kevins]
# 
# Revision 1.1.2.2  1994/06/17  18:43:31  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:26:56  devsrc]
# 
# Revision 1.1.2.1  1994/06/02  18:50:24  kevins
# 	Initial Misc commands test drop
# 	[1994/06/02  18:44:09  kevins]
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

proc resolve_startup {} {
    global RESOLVE_STRING_BINDING1
    if {[dcp_wrap {set RESOLVE_STRING_BINDING1 128.0.0.1:ncadg_ip_udp}] != 0} {return;}
}

## Test purposes
proc resolve_1_N {} {

    # Identify the test
    tet_infoline resolve_1_N: Check handling of no arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {resolve} msg] $msg
    } compare_strings {1 {The 'resolve' command requires a binding as an argument.}}

    # Cleanup steps
}

proc resolve_2_N {} {
    global RESOLVE_STRING_BINDING1

    # Identify the test
    tet_infoline resolve_2_N: Check handling of extra/invalid arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {resolve $RESOLVE_STRING_BINDING1 foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc resolve_3_N {} {
    global RESOLVE_STRING_BINDING1

    # Identify the test
    tet_infoline resolve_3_N: Check handling of extra/invalid options.

    # Setup steps

    # Run the test
    test {
        list [catch {resolve $RESOLVE_STRING_BINDING1 -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc resolve_4_N {} {
    global RESOLVE_STRING_BINDING1

    # Identify the test
    tet_infoline resolve_4_N: Check handling of no -interface option.

    # Setup steps

    # Run the test
    test {
        list [catch {resolve $RESOLVE_STRING_BINDING1} msg] $msg
    } compare_strings {1 {The resolve command requires the '-interface' option with an interface.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "resolve_1_N"
set ic2 "resolve_2_N"
set ic3 "resolve_3_N"
set ic4 "resolve_4_N"
set tet_startup resolve_startup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
