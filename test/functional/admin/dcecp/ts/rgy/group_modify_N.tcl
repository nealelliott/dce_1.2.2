#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: group_modify_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:19:27  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:16  marty]
#
# Revision 1.1.4.2  1995/12/13  15:27:32  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:54:29  root]
# 
# Revision 1.1.2.1  1994/09/28  21:42:01  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:06  melman]
# 
# Revision 1.1.2.6  1994/09/01  17:49:29  kevins
# 	OT Fixed messages and some tests.
# 	[1994/09/01  16:06:44  kevins]
# 
# Revision 1.1.2.5  1994/08/12  18:04:02  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:10  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:14:38  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:55:27  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:13  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:24:29  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:15:49  melman
# 	updating to new API path
# 	[1994/05/06  17:14:20  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:51  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:17  kevins]
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
proc group_modify_1_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_modify_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {group modify foo bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'bar'.}}

    # Cleanup steps
}

proc group_modify_2_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_modify_2_N: Check that missing required argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {group modify} msg] $msg
    } compare_strings {1 {No group name.}}

    # Cleanup steps
}

proc group_modify_3_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_modify_3_N: Check that attribute and option arguments are not allowed..

    # Setup steps

    # Run the test
    test {
        list [catch {group modify foo -fullname yes -change {fullname yes}} msg] $msg
    } compare_strings {1 {Only options or an attribute list are allowed.}}

    # Cleanup steps
}

proc group_modify_4_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_modify_4_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {group modify foo -fullname} msg] $msg
    } compare_strings {1 {The '-fullname' option requires a character string value.}}

    # Cleanup steps
}

proc group_modify_5_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_modify_5_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {group modify foo -change} msg] $msg
    } compare_strings {1 {The '-change' option requires a character string value.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5"
set ic1 "group_modify_1_N"
set ic2 "group_modify_2_N"
set ic3 "group_modify_3_N"
set ic4 "group_modify_4_N"
set ic5 "group_modify_5_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
