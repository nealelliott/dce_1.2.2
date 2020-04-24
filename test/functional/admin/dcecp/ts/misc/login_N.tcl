#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: login_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:17:45  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:35  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:50  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/12/13  17:58 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_msg_chk/1  1994/12/13  17:57 UTC  truitt
# 	Fix incorrect error message.
# 	[1995/12/11  15:53:51  root]
# 
# Revision 1.1.2.1  1994/09/28  21:40:53  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:11  melman]
# 
# Revision 1.1.2.2  1994/06/17  18:43:23  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:26:22  devsrc]
# 
# Revision 1.1.2.1  1994/06/02  18:49:57  kevins
# 	Initial Misc commands test drop
# 	[1994/06/02  18:43:24  kevins]
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


## Test purposes
proc login_1_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline login_1_N: Check handling of no arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {login} msg] $msg
    } compare_strings {1 {The login command requires a username argument.}}

    # Cleanup steps
}

proc login_2_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline login_2_N: Check handling of extra/invalid arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {login $DCP_CLIENT bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'bar'.}}

    # Cleanup steps
}

proc login_3_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline login_3_N: Check handling of extra/invalid options.

    # Setup steps

    # Run the test
    test {
        list [catch {login $DCP_CLIENT -password $DCP_CLIENT_PW -bar} msg] $msg
    } compare_strings {1 {Unrecognized option '-bar'.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 "
set ic1 "login_1_N"
set ic2 "login_2_N"
set ic3 "login_3_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
