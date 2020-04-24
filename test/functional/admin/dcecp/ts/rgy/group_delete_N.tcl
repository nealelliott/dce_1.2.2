#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: group_delete_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:19:18  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:13  marty]
#
# Revision 1.1.4.2  1995/12/13  15:27:29  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:54:25  root]
# 
# Revision 1.1.2.1  1994/09/28  21:41:55  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:02  melman]
# 
# Revision 1.1.2.5  1994/08/12  18:03:59  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:07  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:14:34  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:55:22  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:06  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:24:13  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:15:46  melman
# 	updating to new API path
# 	[1994/05/06  17:14:15  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:47  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:12  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_setup.tcl
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

## Test purposes
proc group_delete_1_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_delete_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {group delete foo bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'bar'.}}

    # Cleanup steps
}

proc group_delete_2_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_delete_2_N: Check that missing required argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {group delete} msg] $msg
    } compare_strings {1 {No group name.}}

    # Cleanup steps
}

proc group_delete_3_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_delete_3_N: Check that unauthenticated users are handled correctly.

    # Setup steps
        # Must be running as unauthenticated user. 

    # Run the test
    test {
        list [catch {group delete foo} msg] $msg
    } compare_strings {1 {User is not authorized to update record}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 "
set ic1 "group_delete_1_N"
set ic2 "group_delete_2_N"
set ic3 "group_delete_3_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
