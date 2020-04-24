#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: account_show_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:18:52  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:03  marty]
#
# Revision 1.1.4.2  1995/12/13  15:27:18  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:54:15  root]
# 
# Revision 1.1.2.1  1994/09/28  21:41:35  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:48  melman]
# 
# Revision 1.1.2.5  1994/09/01  17:49:25  kevins
# 	OT Fixed messages and some tests.
# 	[1994/09/01  16:06:39  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:14:07  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:55:05  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:38:53  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:23:38  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:15:39  melman
# 	updating to new API path
# 	[1994/05/06  17:14:05  melman]
# 
# Revision 1.1.2.1  1994/04/15  19:24:26  kevins
# 	CR 9938 add account functional tests
# 	[1994/04/15  15:31:23  kevins]
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
proc account_show_1_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_show_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {account show kevin bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'bar'.}}

    # Cleanup steps
}

proc account_show_2_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_show_2_N: Check that missing required argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account show} msg] $msg
    } compare_strings {1 {No account name.}}

    # Cleanup steps
}

proc account_show_3_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_show_3_N: Check that extra arguments are trapped as errors.

    # Setup steps
        # Must be running as unauthenticated user. 

    # Run the test
    test {
        list [catch {account show foo -bar} msg] $msg
    } compare_strings {1 {Unrecognized option '-bar'.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 "
set ic1 "account_show_1_N"
set ic2 "account_show_2_N"
set ic3 "account_show_3_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
