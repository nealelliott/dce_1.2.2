#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: principal_modify_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:21:40  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:10  marty]
#
# Revision 1.1.4.2  1995/12/13  15:28:24  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:55:15  root]
# 
# Revision 1.1.2.1  1994/09/28  21:43:18  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:07  melman]
# 
# Revision 1.1.2.6  1994/09/01  17:49:36  kevins
# 	OT Fixed messages and some tests.
# 	[1994/09/01  16:06:54  kevins]
# 
# Revision 1.1.2.5  1994/08/12  18:04:33  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:50  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:15:53  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:56:15  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:55  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:28:44  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:16:24  melman
# 	updating to new API path
# 	[1994/05/06  17:15:03  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:22:31  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:53  kevins]
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
proc principal_modify_1_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_modify_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {principal modify foo bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'bar'.}}

    # Cleanup steps
}

proc principal_modify_2_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_modify_2_N: Check that missing required argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal modify} msg] $msg
    } compare_strings {1 {No principal name.}}

    # Cleanup steps
}

proc principal_modify_3_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_modify_3_N: Check that missing required argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal modify foo} msg] $msg
    } compare_strings {1 {No modify operation has been specified.}}

    # Cleanup steps
}

proc principal_modify_4_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_modify_4_N: Check that attribute and option arguments are not allowed..

    # Setup steps

    # Run the test
    test {
        list [catch {principal modify foo -alias yes -change {alias yes}} msg] $msg
    } compare_strings {1 {Only options or an attribute list are allowed.}}

    # Cleanup steps
}

proc principal_modify_5_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline principal_modify_5_N: Check that unauthenticated users are handled correctly.

    # Setup steps
    set pr_name [lindex [info level [info level]] 0]
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create $pr_name}] != 0 } {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    
    # Run the test
    test {
        list [catch {principal modify $pr_name -quota 1241} msg] $msg
    } compare_strings {1 {User is not authorized to update record}}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal delete $pr_name}] != 0 } {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc principal_modify_6_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_modify_6_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal modify foo -fullname} msg] $msg
    } compare_strings {1 {The '-fullname' option requires a character string value.}}

    # Cleanup steps
}

proc principal_modify_7_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_modify_7_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal modify foo -quota} msg] $msg
    } compare_strings {1 {The '-quota' option requires a character string value.}}

    # Cleanup steps
}

proc principal_modify_8_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_modify_8_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal modify foo -change} msg] $msg
    } compare_strings {1 {The '-change' option requires a character string value.}}

    # Cleanup steps
}

proc principal_modify_9_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_modify_9_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal modify foo -quota days} msg] $msg
    } compare_strings {1 {Supplied 'quota' is not a positive integer or 'unlimited'.}}

    # Cleanup steps
}

proc principal_modify_10_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_modify_10_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal modify foo -quota -5} msg] $msg
    } compare_strings {1 {Supplied 'quota' must be a positive integer.}}

    # Cleanup steps
}

proc principal_modify_11_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_modify_11_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal modify foo -change {quota days}} msg] $msg
    } compare_strings {1 {Supplied 'quota' is not a positive integer or 'unlimited'.}}

    # Cleanup steps
}

proc principal_modify_12_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_modify_12_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal modify foo -change {quota 5 days}} msg] $msg
    } compare_strings {1 {The 'quota' attribute requires a single value.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 "
set ic1 "principal_modify_1_N"
set ic2 "principal_modify_2_N"
set ic3 "principal_modify_3_N"
set ic4 "principal_modify_4_N"
set ic5 "principal_modify_5_N"
set ic6 "principal_modify_6_N"
set ic7 "principal_modify_7_N"
set ic8 "principal_modify_8_N"
set ic9 "principal_modify_9_N"
set ic10 "principal_modify_10_N"
set ic11 "principal_modify_11_N"
set ic12 "principal_modify_12_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
