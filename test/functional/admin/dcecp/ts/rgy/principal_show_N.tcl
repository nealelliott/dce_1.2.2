#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: principal_show_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:21:59  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:18  marty]
#
# Revision 1.1.4.1  1995/12/13  15:28:36  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:55:24  root]
# 
# Revision 1.1.2.1  1994/09/28  21:43:30  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:17  melman]
# 
# Revision 1.1.2.5  1994/08/12  18:04:36  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:57  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:15:56  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:56:19  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:40:02  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:29:02  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:16:28  melman
# 	updating to new API path
# 	[1994/05/06  17:15:07  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:22:36  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:57  kevins]
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
proc principal_show_1_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_show_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {principal show foo foo_bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo_bar'.}}

    # Cleanup steps
}

proc principal_show_2_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_show_2_N: Check that not enough arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {principal show } msg] $msg
    } compare_strings {1 {No principal name.}}

    # Cleanup steps
}

proc principal_show_3_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_show_3_N: Check that unrecognized options are trapped.

    # Setup steps
    
    # Run the test
    test {
        list [catch {principal show foo -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 "
set ic1 "principal_show_1_N"
set ic2 "principal_show_2_N"
set ic3 "principal_show_3_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
