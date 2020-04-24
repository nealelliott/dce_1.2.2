#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: directory_remove_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:13:44  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:29:43  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:27  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:52:36  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:53  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:07:54  melman]
# 
# Revision 1.1.2.5  1994/09/02  13:56:50  kevins
# 	OT 10617 modified messages and some tests.
# 	[1994/09/02  10:10:45  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:11:25  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:28  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:35:57  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:18:31  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:17  melman
# 	updating to new API path
# 	[1994/05/06  15:20:05  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:20:59  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:42  kevins]
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
proc directory_remove_1_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline directory_remove_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {directory remove /.: -member test_1 extra} msg] $msg
    } compare_strings {1 {Unrecognized argument 'extra'.}}

    # Cleanup steps
}

proc directory_remove_2_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline directory_remove_2_N: Check that missing required argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {directory remove /.:} msg] $msg
    } compare_strings {1 {Member name not specified.}}

    # Cleanup steps
}

proc directory_remove_3_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline directory_remove_3_N: Check that missing directory name is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {directory remove  -member test_1 } msg] $msg
    } compare_strings {1 {No directory name.}}

    # Cleanup steps
}

proc directory_remove_4_N {} {
    global TET_ROOT  DCP_CLIENT DCP_CLIENT_PW VERBOSE

    # Identify the test
    tet_infoline directory_remove_4_N: Check that unauthenticated users are handled correctly.

    # Setup steps
    if {[dcp_wrap {set dir_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory create /.:/$dir_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test {
        list [catch {directory remove /.: -member $dir_name} msg] $msg
    } compare_strings {1 {Server process has insufficient access to clearinghouse}}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory delete /.:/$dir_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "directory_remove_1_N"
set ic2 "directory_remove_2_N"
set ic3 "directory_remove_3_N"
set ic4 "directory_remove_4_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
