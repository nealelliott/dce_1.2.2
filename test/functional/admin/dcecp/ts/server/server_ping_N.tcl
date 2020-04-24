#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: server_ping_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:28:18  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:41  marty]
#
# Revision 1.1.4.1  1995/12/11  15:16:45  root
# 	Submit
# 	[1995/12/11  14:32:33  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:36  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:46  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:41  melman]
# 
# Revision 1.1.2.3  1994/09/20  18:22:58  pwang
# 	Updated to latest changes [OT#10262]
# 	[1994/09/20  18:14:41  pwang]
# 
# Revision 1.1.2.2  1994/06/17  18:44:23  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:30:49  devsrc]
# 
# Revision 1.1.2.1  1994/06/10  21:40:04  pwang
# 	Initial implementation
# 	[1994/06/10  21:31:35  pwang]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


## Test purposes
proc server_ping_1_N {} {
    # Identify the test
    tet_infoline server_ping_1_N: Check that insufficient/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {server ping} msg] $msg
    } compare_strings {1 {'dced' object not specified.}}

    # Cleanup steps
}

proc server_ping_2_N {} {
    # Identify the test
    tet_infoline server_ping_2_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {server ping -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc server_ping_3_N {} {
    # Identify the test
    tet_infoline server_ping_3_N: Check that wrong server entry name are returned as not found. 

    # Setup steps

    # Run the test
    test {
        list [catch {server ping /.:/foo} msg] $msg
    } compare_strings {1 {Entry not found}}

    # Cleanup steps
}

proc server_ping_4_N {} {
    # Identify the test
    tet_infoline server_ping_4_N: Check that bad string binding are returned as error. 

    # Setup steps

    # Run the test
    test {
        list [catch {server ping foo_binding} msg] $msg
    } compare_strings {1 {Invalid RPC protocol sequence}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "server_ping_1_N"
set ic2 "server_ping_2_N"
set ic3 "server_ping_3_N"
set ic4 "server_ping_4_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
