#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: logout_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:17:50  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:37  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:52  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/06/06  18:39 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts33/1  1995/06/06  18:38 UTC  truitt
# 	CHFts15424: Change tests to reflect new dced acl behavior.
# 	[1995/12/11  15:53:53  root]
# 
# Revision 1.1.2.1  1994/09/28  21:40:55  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:13  melman]
# 
# Revision 1.1.2.2  1994/06/17  18:43:26  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:26:30  devsrc]
# 
# Revision 1.1.2.1  1994/06/02  18:50:03  kevins
# 	Initial Misc commands test drop
# 	[1994/06/02  18:43:36  kevins]
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
proc logout_1_N {} {
    # Identify the test
    tet_infoline logout_1_N: Check handling of extra/invalid arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {logout foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc logout_2_N {} {
    # Identify the test
    tet_infoline logout_2_N: Check handling of extra/invalid options.

    # Setup steps

    # Run the test
    test {
        list [catch {logout -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc logout_3_N {} {
    # Identify the test
    tet_infoline logout_3_N: Check that logging out without logging in is trapped as an error.

    # Setup steps

    # Run the test
    test {
        list [catch {logout} msg] $msg
    } compare_strings {1 {You are not allowed to logout of an inherited context.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 "
set ic1 "logout_1_N"
set ic2 "logout_2_N"
set ic3 "logout_3_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
