#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: principal_create_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:21:19  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:03  marty]
#
# Revision 1.1.4.2  1995/12/13  15:28:17  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/04/05  19:30 UTC  truitt
# 	Merge test work.
# 
# 	HP revision /main/HPDCE02/truitt_mothra_chfts24/1  1995/04/05  19:29 UTC  truitt
# 	CHFts14652: Improved error message for bad schema name.
# 
# 	HP revision /main/HPDCE02/1  1995/03/09  13:27 UTC  truitt
# 	Check bogus -attr value.
# 
# 	HP revision /main/truitt_dcecp_chfts18/1  1995/03/09  13:23 UTC  truitt
# 	CHFts14579: Test that bogus values for the -attr option
# 	are properly handled.
# 	[1995/12/11  15:55:09  root]
# 
# Revision 1.1.2.1  1994/09/28  21:43:06  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:00  melman]
# 
# Revision 1.1.2.7  1994/09/28  13:27:53  kevins
# 	OT 11597 modified uid/gid/orgid error messages
# 	[1994/09/28  13:18:29  kevins]
# 
# Revision 1.1.2.6  1994/09/01  17:49:34  kevins
# 	OT Fixed messages and some tests.
# 	[1994/09/01  16:06:52  kevins]
# 
# Revision 1.1.2.5  1994/08/12  18:04:30  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:46  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:15:49  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:56:09  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:52  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:28:30  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:16:21  melman
# 	updating to new API path
# 	[1994/05/06  17:14:58  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:22:27  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:48  kevins]
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
proc principal_create_1_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_create_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {principal create foo bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'bar'.}}

    # Cleanup steps
}

proc principal_create_2_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_create_2_N: Check that missing required argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal create} msg] $msg
    } compare_strings {1 {No principal name.}}

    # Cleanup steps
}

proc principal_create_3_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_create_3_N: Check that unauthenticated users are handled correctly.

    # Setup steps

    # Run the test
    test {
        list [catch {principal create foo} msg] $msg
    } compare_strings {1 {User is not authorized to update record}}

    # Cleanup steps
}

proc principal_create_4_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_create_4_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal create foo -fullname} msg] $msg
    } compare_strings {1 {The '-fullname' option requires a character string value.}}

    # Cleanup steps
}

proc principal_create_5_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_create_5_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal create foo -quota} msg] $msg
    } compare_strings {1 {The '-quota' option requires a character string value.}}

    # Cleanup steps
}

proc principal_create_6_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_create_6_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal create foo -uid} msg] $msg
    } compare_strings {1 {The '-uid' option requires a character string value.}}

    # Cleanup steps
}

proc principal_create_7_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_create_7_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal create foo -attribute} msg] $msg
    } compare_strings {1 {The '-attribute' option requires a character string value.}}

    # Cleanup steps
}

proc principal_create_8_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_create_8_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal create foo -attribute {alias yes}} msg] $msg
    } compare_strings {1 {No 'uid' specified when attempting to create an alias.}}

    # Cleanup steps
}

proc principal_create_9_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_create_9_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal create foo -alias yes} msg] $msg
    } compare_strings {1 {No 'uid' specified when attempting to create an alias.}}

    # Cleanup steps
}


proc principal_create_10_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_create_10_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal create foo -quota days} msg] $msg
    } compare_strings {1 {Supplied 'quota' is not a positive integer or 'unlimited'.}}

    # Cleanup steps
}

proc principal_create_11_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_create_11_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal create foo -quota -5} msg] $msg
    } compare_strings {1 {Supplied 'quota' must be a positive integer.}}

    # Cleanup steps
}

proc principal_create_12_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_create_12_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal create foo -attribute {quota days}} msg] $msg
    } compare_strings {1 {Supplied 'quota' is not a positive integer or 'unlimited'.}}

    # Cleanup steps
}

proc principal_create_13_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_create_13_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal create foo -attribute {quota -5 days}} msg] $msg
    } compare_strings {1 {The 'quota' attribute requires a single value.}}

    # Cleanup steps
}

proc principal_create_14_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_create_14_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal create foo -uid days} msg] $msg
    } compare_strings {1 {Supplied 'uid' must be a positive integer.}}

    # Cleanup steps
}

proc principal_create_15_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_create_15_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal create foo -uid -1} msg] $msg
    } compare_strings {1 {Supplied 'uid' must be a positive integer.}}

    # Cleanup steps
}

proc principal_create_16_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_create_16_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal create foo -attribute {uid days}} msg] $msg
    } compare_strings {1 {Supplied 'uid' must be a positive integer.}}

    # Cleanup steps
}

proc principal_create_17_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_create_17_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal create foo -attribute {uid days}} msg] $msg
    } compare_strings {1 {Supplied 'uid' must be a positive integer.}}

    # Cleanup steps
}

proc principal_create_18_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_create_18_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal create foo -attribute {uid 1212} -alias yes} msg] $msg
    } compare_strings {1 {Only options or an attribute list are allowed.}}

    # Cleanup steps
}

proc principal_create_19_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_create_19_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal create foo -attribute {uuid asdfasdf}} msg] $msg
    } compare_strings {1 {Invalid string UUID}}

    # Cleanup steps
}

proc principal_create_20_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline principal_create_20_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {principal create foo -uuid asdfasdfs} msg] $msg
    } compare_strings {1 {Invalid string UUID}}

    # Cleanup steps
}

proc principal_create_21_N {} {
    # Identify the test
    tet_infoline principal_create_21_N: Check bogus stuff passed to -attribute.

    # Setup steps

    # Run the test
    test {
        list [catch {principal create foo -attribute \"\"} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc principal_create_22_N {} {
    # Identify the test
    tet_infoline principal_create_22_N: Check bogus stuff passed to -attribute.

    # Setup steps

    # Run the test
    test {
        list [catch {principal create foo -attribute gumby} msg] $msg
    } compare_strings {1 {Schema entry 'gumby' not found.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 "
set ic1 "principal_create_1_N"
set ic2 "principal_create_2_N"
set ic3 "principal_create_3_N"
set ic4 "principal_create_4_N"
set ic5 "principal_create_5_N"
set ic6 "principal_create_6_N"
set ic7 "principal_create_7_N"
set ic8 "principal_create_8_N"
set ic9 "principal_create_9_N"
set ic10 "principal_create_10_N"
set ic11 "principal_create_11_N"
set ic12 "principal_create_12_N"
set ic13 "principal_create_13_N"
set ic14 "principal_create_14_N"
set ic15 "principal_create_15_N"
set ic16 "principal_create_16_N"
set ic17 "principal_create_17_N"
set ic18 "principal_create_18_N"
set ic19 "principal_create_19_N"
set ic20 "principal_create_20_N"
set ic21 "principal_create_21_N"
set ic22 "principal_create_22_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
