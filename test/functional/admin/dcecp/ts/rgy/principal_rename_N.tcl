#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: principal_rename_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:21:55  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:17  marty]
#
# Revision 1.1.4.1  1995/12/13  15:28:33  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:55:21  root]
# 
# Revision 1.1.2.1  1994/09/28  21:43:28  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:15  melman]
# 
# Revision 1.1.2.6  1994/09/01  17:49:37  kevins
# 	OT Fixed messages and some tests.
# 	[1994/09/01  16:06:55  kevins]
# 
# Revision 1.1.2.5  1994/08/12  18:04:34  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:52  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:15:54  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:56:16  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:57  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:28:53  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:16:26  melman
# 	updating to new API path
# 	[1994/05/06  17:15:05  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:22:34  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:55  kevins]
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
proc principal_rename_1_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_rename_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {principal rename foo bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'bar'.}}

    # Cleanup steps
}

proc principal_rename_2_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_rename_2_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {principal rename foo} msg] $msg
    } compare_strings {1 {No name provided to rename to.}}

    # Cleanup steps
}

proc principal_rename_3_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline principal_rename_3_N: Check that unauthenticated users are handled correctly.

    # Setup steps
    set pr_name [lindex [info level [info level]] 0]
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create $pr_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}


    # Run the test
    test {
        list [catch {principal rename $pr_name -to foo_$pr_name} msg] $msg
    } compare_strings {1 {User is not authorized to update record}}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal delete $pr_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

}


set iclist "ic1 ic2 ic3 "
set ic1 "principal_rename_1_N"
set ic2 "principal_rename_2_N"
set ic3 "principal_rename_3_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
