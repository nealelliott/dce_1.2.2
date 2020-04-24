#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: group_create_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:19:06  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:08  marty]
#
# Revision 1.1.4.2  1995/12/13  15:27:24  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/04/05  19:24 UTC  truitt
# 	Merge message change work.
# 
# 	HP revision /main/HPDCE02/truitt_mothra_chfts24/1  1995/04/05  19:23 UTC  truitt
# 	CHFts14652: Improved error message for bad schema name.
# 
# 	HP revision /main/HPDCE02/1  1995/03/09  14:04 UTC  truitt
# 	Test bogus -attr value.
# 
# 	HP revision /main/truitt_dcecp_chfts18/1  1995/03/09  14:02 UTC  truitt
# 	CHFts14579: Test that bogus values for the -attr option
# 	are properly handled.
# 	[1995/12/11  15:54:21  root]
# 
# Revision 1.1.2.1  1994/09/28  21:41:46  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:57  melman]
# 
# Revision 1.1.2.6  1994/09/28  13:27:51  kevins
# 	OT 11597 modified uid/gid/orgid error messages
# 	[1994/09/28  13:18:26  kevins]
# 
# Revision 1.1.2.5  1994/09/01  17:49:27  kevins
# 	OT Fixed messages and some tests.
# 	[1994/09/01  16:06:42  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:14:33  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:55:19  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:01  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:24:01  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:15:44  melman
# 	updating to new API path
# 	[1994/05/06  17:14:12  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:38  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:09  kevins]
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
proc group_create_1_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_create_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {group create foo bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'bar'.}}

    # Cleanup steps
}

proc group_create_2_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_create_2_N: Check that missing required argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {group create} msg] $msg
    } compare_strings {1 {No group name.}}

    # Cleanup steps
}

proc group_create_3_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_create_3_N: Check that unauthenticated users are handled correctly.

    # Setup steps
        # Must be running as unauthenticated user. 

    # Run the test
    test {
        list [catch {group create foo} msg] $msg
    } compare_strings {1 {User is not authorized to update record}}

    # Cleanup steps
}

proc group_create_4_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_create_4_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {group create foo -fullname} msg] $msg
    } compare_strings {1 {The '-fullname' option requires a character string value.}}

    # Cleanup steps
}

proc group_create_5_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_create_5_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {group create foo -inprojlist} msg] $msg
    } compare_strings {1 {The '-inprojlist' option requires a character string value.}}

    # Cleanup steps
}

proc group_create_6_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_create_6_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {group create foo -gid} msg] $msg
    } compare_strings {1 {The '-gid' option requires a character string value.}}

    # Cleanup steps
}

proc group_create_7_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_create_7_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {group create foo -attribute} msg] $msg
    } compare_strings {1 {The '-attribute' option requires a character string value.}}

    # Cleanup steps
}

proc group_create_8_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_create_8_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {group create foo -attribute {alias yes}} msg] $msg
    } compare_strings {1 {No 'gid' specified when attempting to create an alias.}}

    # Cleanup steps
}

proc group_create_9_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_create_9_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {group create foo -alias yes} msg] $msg
    } compare_strings {1 {No 'gid' specified when attempting to create an alias.}}

    # Cleanup steps
}

proc group_create_10_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_create_10_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {group create foo -gid days} msg] $msg
    } compare_strings {1 {Supplied 'gid' must be a positive integer.}}

    # Cleanup steps
}

proc group_create_11_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_create_11_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {group create foo -gid -5} msg] $msg
    } compare_strings {1 {Supplied 'gid' must be a positive integer.}}

    # Cleanup steps
}

proc group_create_12_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_create_12_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {group create foo -attribute {gid days}} msg] $msg
    } compare_strings {1 {Supplied 'gid' must be a positive integer.}}

    # Cleanup steps
}

proc group_create_13_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_create_13_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {group create foo -attribute {gid -5 days}} msg] $msg
    } compare_strings {1 {The attribute value is a single value not a list of values.}}

    # Cleanup steps
}



proc group_create_14_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_create_14_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {group create foo -attribute {gid 1212} -alias yes} msg] $msg
    } compare_strings {1 {Only options or an attribute list are allowed.}}

    # Cleanup steps
}

proc group_create_15_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_create_15_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {group create foo -attribute {uuid asdfasdf}} msg] $msg
    } compare_strings {1 {Invalid string UUID}}

    # Cleanup steps
}

proc group_create_16_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline group_create_16_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {group create foo -uuid asdfasdfs} msg] $msg
    } compare_strings {1 {Invalid string UUID}}

    # Cleanup steps
}

proc group_create_17_N {} {
    # Identify the test
    tet_infoline group_create_17_N: Check bogus stuff passed to -attribute.

    # Setup steps

    # Run the test
    test {
        list [catch {group create foo -attribute \"\"} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc group_create_18_N {} {
    # Identify the test
    tet_infoline group_create_18_N: Check bogus stuff passed to -attribute.

    # Setup steps

    # Run the test
    test {
        list [catch {group create foo -attribute gumby} msg] $msg
    } compare_strings {1 {Schema entry 'gumby' not found.}}

    # Cleanup steps
}

set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 "
set ic1 "group_create_1_N"
set ic2 "group_create_2_N"
set ic3 "group_create_3_N"
set ic4 "group_create_4_N"
set ic5 "group_create_5_N"
set ic6 "group_create_6_N"
set ic7 "group_create_7_N"
set ic8 "group_create_8_N"
set ic9 "group_create_9_N"
set ic10 "group_create_10_N"
set ic11 "group_create_11_N"
set ic12 "group_create_12_N"
set ic13 "group_create_13_N"
set ic14 "group_create_14_N"
set ic15 "group_create_15_N"
set ic16 "group_create_16_N"
set ic17 "group_create_17_N"
set ic18 "group_create_18_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
