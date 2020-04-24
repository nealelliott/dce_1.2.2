#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: directory_add_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:13:14  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:29:15  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:15  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:52:27  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:41  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:07:33  melman]
# 
# Revision 1.1.2.5  1994/09/02  13:56:38  kevins
# 	OT 10617 modified messages and some tests.
# 	[1994/09/02  10:10:25  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:03:34  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:17  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:35:48  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:17:55  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:09  melman
# 	updating to new API path
# 	[1994/05/06  15:19:53  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:20:35  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:31  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


## Test purposes
proc directory_add_1_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline directory_add_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {directory add /.: -member test_1 -clearinghouse /.:/mach_ch extra} msg] $msg
    } compare_strings {1 {Unrecognized argument 'extra'.}}

    # Cleanup steps
}

proc directory_add_2_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline directory_add_2_N: Check that missing required argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {directory add /.: -member test_1} msg] $msg
    } compare_strings {1 {Clearinghouse name not provided.}}

    # Cleanup steps
}

proc directory_add_3_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline directory_add_3_N: Check that missing required argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {directory add /.: -clearinghouse /.:/machine_ch} msg] $msg
    } compare_strings {1 {Member name not specified.}}

    # Cleanup steps
}

proc directory_add_4_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline directory_add_4_N: Check that missing directory name is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {directory add  -member test_1 -clearinghouse /.:/mach_ch } msg] $msg
    } compare_strings {1 {No directory name.}}

    # Cleanup steps
}

proc directory_add_5_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline directory_add_5_N: Check that unauthenticated users are handled correctly.
 
    # Setup steps

    # Run the test
    test {
        list [catch {directory add /.: -member test_1 -clearinghouse /.:/mach_ch -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}
    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 "
set ic1 "directory_add_1_N"
set ic2 "directory_add_2_N"
set ic3 "directory_add_3_N"
set ic4 "directory_add_4_N"
set ic5 "directory_add_5_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
