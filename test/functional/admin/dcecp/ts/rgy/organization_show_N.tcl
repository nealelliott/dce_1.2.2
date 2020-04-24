#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: organization_show_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:21:08  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:58  marty]
#
# Revision 1.1.4.2  1995/12/13  15:28:13  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/05/15  18:09 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts29/1  1995/05/15  18:08 UTC  truitt
# 	CHFts15220: Additional tests.
# 	[1995/12/11  15:55:06  root]
# 
# Revision 1.1.2.1  1994/09/28  21:43:01  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:55  melman]
# 
# Revision 1.1.2.5  1994/08/12  18:04:26  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:41  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:15:24  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:56:05  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:48  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:28:14  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:16:16  melman
# 	updating to new API path
# 	[1994/05/06  17:14:52  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:22:20  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:43  kevins]
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
proc organization_show_1_N {} {
    # Identify the test
    tet_infoline organization_show_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {organization show foo bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'bar'.}}

    # Cleanup steps
}

proc organization_show_2_N {} {
    # Identify the test
    tet_infoline organization_show_2_N: Check that missing required argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization show} msg] $msg
    } compare_strings {1 {No organization name.}}

    # Cleanup steps
}

proc organization_show_3_N {} {
    # Identify the test
    tet_infoline organization_show_3_N: Check that invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {organization show foo -bar} msg] $msg
    } compare_strings {1 {Unrecognized option '-bar'.}}

    # Cleanup steps
}

proc organization_show_4_N {} {
    # Identify the test
    tet_infoline organization_show_4_N: Check that unknown organizations are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {organization show gumby} msg] $msg
    } compare_strings {1 {Registry object not found}}

    # Cleanup steps
}

proc organization_show_5_N {} {
    # Identify the test
    tet_infoline organization_show_5_N: Check mutually exclusive options

    # Setup steps

    # Run the test
    test {
        list [catch {organization show none -policies -all} msg] $msg
    } compare_strings {1 {Mutually exclusive options entered.}}

    # Cleanup steps
}

proc organization_show_6_N {} {
    # Identify the test
    tet_infoline organization_show_6_N: Check mutually exclusive options

    # Setup steps

    # Run the test
    test {
        list [catch {organization show none -xattrs -all} msg] $msg
    } compare_strings {1 {Mutually exclusive options entered.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 "
set ic1 "organization_show_1_N"
set ic2 "organization_show_2_N"
set ic3 "organization_show_3_N"
set ic4 "organization_show_4_N"
set ic5 "organization_show_5_N"
set ic6 "organization_show_6_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
