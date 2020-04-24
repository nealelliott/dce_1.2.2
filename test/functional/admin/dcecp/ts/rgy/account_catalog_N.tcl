#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: account_catalog_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:18:25  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:53  marty]
#
# Revision 1.1.4.2  1995/12/13  15:27:06  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/03/09  14:16 UTC  truitt
# 	Test cellname argument.
# 
# 	HP revision /main/truitt_dcecp_chfts18/1  1995/03/09  14:15 UTC  truitt
# 	CHFts14388: Test that the cellname argument is
# 	handled correctly.
# 	[1995/12/11  15:54:06  root]
# 
# Revision 1.1.2.1  1994/09/28  21:41:19  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:33  melman]
# 
# Revision 1.1.2.6  1994/09/14  14:34:48  kevins
# 	OT 11641 modified to with tcl list arguments
# 	[1994/09/13  19:26:09  kevins]
# 
# Revision 1.1.2.5  1994/09/01  17:49:16  kevins
# 	OT Fixed messages and some tests.
# 	[1994/09/01  16:06:27  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:13:59  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:55  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:38:45  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:22:58  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:15:26  melman
# 	updating to new API path
# 	[1994/05/06  17:13:52  melman]
# 
# Revision 1.1.2.1  1994/04/15  19:24:15  kevins
# 	CR 9938 add account functional tests
# 	[1994/04/15  15:31:15  kevins]
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
proc account_catalog_1_N {} {
    # Identify the test
    tet_infoline account_catalog_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {account catalog bar foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc account_catalog_2_N {} {
    # Identify the test
    tet_infoline account_catalog_2_N: Check that extra options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {account catalog bar -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc account_catalog_3_N {} {
    # Identify the test
    tet_infoline account_catalog_3_N: Check that an illegal cell name is trapped.

    # Setup steps

    # Run the test
    test {
        list [catch {account catalog gumby} msg] $msg
    } compare_strings {1 {The cell name 'gumby' is invalid.}}

    # Cleanup steps
}

proc account_catalog_4_N {} {
    # Identify the test
    tet_infoline account_catalog_4_N: Check that an illegal cell name is trapped.

    # Setup steps

    # Run the test
    test {
        list [catch {account catalog /gumby} msg] $msg
    } compare_strings {1 {Unable to parse cell name '/gumby'.}}

    # Cleanup steps
}

proc account_catalog_5_N {} {
    # Identify the test
    tet_infoline account_catalog_5_N: Check that an illegal cell name is trapped.

    # Setup steps

    # Run the test
    test {
        list [catch {account catalog /.../gumby} msg] $msg
    } compare_strings {1 {Registry server '/.../gumby' is unavailable.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 "
set ic1 "account_catalog_1_N"
set ic2 "account_catalog_2_N"
set ic3 "account_catalog_3_N"
set ic4 "account_catalog_4_N"
set ic5 "account_catalog_5_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
