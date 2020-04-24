#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: organization_rename_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:21:03  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:57  marty]
#
# Revision 1.1.4.2  1995/12/13  15:28:11  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:55:04  root]
# 
# Revision 1.1.2.1  1994/09/28  21:42:58  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:53  melman]
# 
# Revision 1.1.2.6  1994/08/30  18:28:58  kevins
# 	OT 10617 modified messages/output for functional tests
# 	[1994/08/30  18:28:32  kevins]
# 
# Revision 1.1.2.5  1994/08/12  18:04:24  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:39  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:15:22  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:56:03  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:43  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:28:06  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:16:13  melman
# 	updating to new API path
# 	[1994/05/06  17:14:49  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:22:17  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:41  kevins]
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
proc organization_rename_1_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_rename_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {organization rename foo foo_bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo_bar'.}}

    # Cleanup steps
}

proc organization_rename_2_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_rename_2_N: Check that missing required argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization rename} msg] $msg
    } compare_strings {1 {No organization name.}}

    # Cleanup steps
}

proc organization_rename_3_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_rename_3_N: Check that missing required argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization rename foo} msg] $msg
    } compare_strings {1 {No name provided to rename to.}}

    # Cleanup steps
}

proc organization_rename_4_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_rename_4_N: Check that unauthenticated users are handled correctly.

    # Setup steps

    # Run the test
    test {
        list [catch {organization rename none -to foo1} msg] $msg
    } compare_strings {1 {User is not authorized to update record}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "organization_rename_1_N"
set ic2 "organization_rename_2_N"
set ic3 "organization_rename_3_N"
set ic4 "organization_rename_4_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
