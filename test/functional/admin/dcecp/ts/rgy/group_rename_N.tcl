#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: group_rename_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:19:45  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:25  marty]
#
# Revision 1.1.4.2  1995/12/13  15:27:40  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:54:36  root]
# 
# Revision 1.1.2.1  1994/09/28  21:42:13  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:16  melman]
# 
# Revision 1.1.2.6  1994/09/01  17:49:30  kevins
# 	OT Fixed messages and some tests.
# 	[1994/09/01  16:06:46  kevins]
# 
# Revision 1.1.2.5  1994/08/12  18:04:06  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:15  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:14:42  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:55:32  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:17  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:24:44  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:15:53  melman
# 	updating to new API path
# 	[1994/05/06  17:14:26  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:56  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:22  kevins]
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
proc group_rename_1_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_rename_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {group rename foo bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'bar'.}}

    # Cleanup steps
}

proc group_rename_2_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline group_rename_2_N: Check that unauthenticated users are handled correctly.

    # Setup steps
    set gr_name [lindex [info level [info level]] 0]
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group create $gr_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}


    # Run the test
    test {
        list [catch {group rename $gr_name -to new_$gr_name} msg] $msg
    } compare_strings {1 {User is not authorized to update record}}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group delete $gr_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

}

proc group_rename_3_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_rename_3_N: Check that failure to include -to results is trapped as error.

    # Setup steps


    # Run the test
    test {
        list [catch {group rename foo } msg] $msg
    } compare_strings {1 {No name provided to rename to.}}

    # Cleanup steps
}

set iclist "ic1 ic2 ic3"
set ic1 "group_rename_1_N"
set ic2 "group_rename_2_N"
set ic3 "group_rename_3_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
