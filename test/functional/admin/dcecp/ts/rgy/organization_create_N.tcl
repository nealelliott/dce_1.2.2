#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: organization_create_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:20:34  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:45  marty]
#
# Revision 1.1.4.2  1995/12/13  15:28:02  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/04/05  19:26 UTC  truitt
# 	Merge test work.
# 
# 	HP revision /main/HPDCE02/truitt_mothra_chfts24/1  1995/04/05  19:25 UTC  truitt
# 	CHFts14652: Improved error message for bad schema name.
# 
# 	HP revision /main/HPDCE02/1  1995/03/09  13:51 UTC  truitt
# 	Test for bogus -attr value.
# 
# 	HP revision /main/truitt_dcecp_chfts18/1  1995/03/09  13:49 UTC  truitt
# 	CHFts14579: Test that bogus values for the -attr option
# 	are properly handled.
# 	[1995/12/11  15:54:55  root]
# 
# Revision 1.1.2.1  1994/09/28  21:42:44  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:41  melman]
# 
# Revision 1.1.2.7  1994/09/28  13:27:52  kevins
# 	OT 11597 modified uid/gid/orgid error messages
# 	[1994/09/28  13:18:28  kevins]
# 
# Revision 1.1.2.6  1994/08/30  18:28:54  kevins
# 	OT 10617 modified messages/output for functional tests
# 	[1994/08/30  18:28:25  kevins]
# 
# Revision 1.1.2.5  1994/08/12  18:04:14  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:28  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:15:12  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:55:41  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:30  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:25:18  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:16:01  melman
# 	updating to new API path
# 	[1994/05/06  17:14:36  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:22:05  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:31  kevins]
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
proc organization_create_1_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_create_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foo bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'bar'.}}

    # Cleanup steps
}

proc organization_create_2_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_create_2_N: Check that missing required argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create} msg] $msg
    } compare_strings {1 {No organization name.}}

    # Cleanup steps
}

proc organization_create_3_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_create_3_N: Check that unauthenticated users are handled correctly.

    # Setup steps
        # Must be running as unauthenticated user. 

    # Run the test
    test {
        list [catch {organization create foo} msg] $msg
    } compare_strings {1 {User is not authorized to update record}}

    # Cleanup steps
}

proc organization_create_4_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_create_4_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foo -fullname } msg] $msg
    } compare_strings {1 {The '-fullname' option requires a character string value.}}

    # Cleanup steps
}

proc organization_create_5_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_create_5_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foo -bar} msg] $msg
    } compare_strings {1 {Unrecognized option '-bar'.}}

    # Cleanup steps
}

proc organization_create_6_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_create_6_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foo -orgid} msg] $msg
    } compare_strings {1 {The '-orgid' option requires a character string value.}}

    # Cleanup steps
}

proc organization_create_7_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_create_7_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foo -attribute} msg] $msg
    } compare_strings {1 {The '-attribute' option requires a character string value.}}

    # Cleanup steps
}

proc organization_create_8_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_create_8_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foo -orgid days} msg] $msg
    } compare_strings {1 {Supplied 'orgid' must be a positive integer.}}

    # Cleanup steps
}

proc organization_create_9_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_create_9_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foo -orgid -5} msg] $msg
    } compare_strings {1 {Supplied 'orgid' must be a positive integer.}}

    # Cleanup steps
}

proc organization_create_10_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_create_10_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foo -attribute {orgid days}} msg] $msg
    } compare_strings {1 {Supplied 'orgid' must be a positive integer.}}

    # Cleanup steps
}

proc organization_create_11_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_create_11_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foo -attribute {orgid -5}} msg] $msg
    } compare_strings {1 {Supplied 'orgid' must be a positive integer.}}

    # Cleanup steps
}

proc organization_create_12_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_create_12_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foo -orgid days} msg] $msg
    } compare_strings {1 {Supplied 'orgid' must be a positive integer.}}

    # Cleanup steps
}

proc organization_create_13_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_create_13_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foo -orgid -1} msg] $msg
    } compare_strings {1 {Supplied 'orgid' must be a positive integer.}}

    # Cleanup steps
}

proc organization_create_14_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_create_14_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foo -attribute {pwdexpdate days}} msg] $msg
    } compare_strings {1 {Bad 'pwdexpdate' date string supplied.}}

    # Cleanup steps
}

proc organization_create_15_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_create_15_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foo -pwdexpdate days} msg] $msg
    } compare_strings {1 {Bad 'pwdexpdate' date string supplied.}}

    # Cleanup steps
}

proc organization_create_16_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_create_16_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foo -attribute {orgid 1212} -orgid 12} msg] $msg
    } compare_strings {1 {Only options or an attribute list are allowed.}}

    # Cleanup steps
}

proc organization_create_17_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_create_17_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foo -attribute {uuid asdfasdf}} msg] $msg
    } compare_strings {1 {Invalid string UUID}}

    # Cleanup steps
}

proc organization_create_18_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_create_18_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foo -uuid asdfasdfs} msg] $msg
    } compare_strings {1 {Invalid string UUID}}

    # Cleanup steps
}

proc organization_create_19_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_create_19_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foo -pwdminlen asdfasdfs} msg] $msg
    } compare_strings {1 {Supplied 'pwdminlen' is not a positive integer.}}

    # Cleanup steps
}

proc organization_create_20_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_create_20_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foo -pwdlife asdfasdfs} msg] $msg
    } compare_strings {1 {Supplied 'pwdlife' is a bad date value.}}

    # Cleanup steps
}

proc organization_create_21_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_create_21_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foo -pwdalpha asdfasdfs} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc organization_create_22_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_create_22_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foo -pwdspaces asdfasdfs} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc organization_create_23_N {} {
    # Identify the test
    tet_infoline organization_create_23_N: Check bogus stuff passed to -attribute.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foo -attribute \"\"} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc organization_create_24_N {} {
    # Identify the test
    tet_infoline organization_create_24_N: Check bogus stuff passed to -attribute.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foo -attribute gumby} msg] $msg
    } compare_strings {1 {Schema entry 'gumby' not found.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 "
set ic1 "organization_create_1_N"
set ic2 "organization_create_2_N"
set ic3 "organization_create_3_N"
set ic4 "organization_create_4_N"
set ic5 "organization_create_5_N"
set ic6 "organization_create_6_N"
set ic7 "organization_create_7_N"
set ic8 "organization_create_8_N"
set ic9 "organization_create_9_N"
set ic10 "organization_create_10_N"
set ic11 "organization_create_11_N"
set ic12 "organization_create_12_N"
set ic13 "organization_create_13_N"
set ic14 "organization_create_14_N"
set ic15 "organization_create_15_N"
set ic16 "organization_create_16_N"
set ic17 "organization_create_17_N"
set ic18 "organization_create_18_N"
set ic19 "organization_create_19_N"
set ic20 "organization_create_20_N"
set ic21 "organization_create_21_N"
set ic22 "organization_create_22_N"
set ic23 "organization_create_23_N"
set ic24 "organization_create_24_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
