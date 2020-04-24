#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: principal_catalog_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:21:14  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:01  marty]
#
# Revision 1.1.4.2  1995/12/13  15:28:15  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/03/09  13:48 UTC  truitt
# 	Test the cellname argument.
# 
# 	HP revision /main/truitt_dcecp_chfts18/1  1995/03/09  13:45 UTC  truitt
# 	CHFts14388: Test that the cellname argument is
# 	handled correctly.
# 	[1995/12/11  15:55:08  root]
# 
# Revision 1.1.2.1  1994/09/28  21:43:03  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:57  melman]
# 
# Revision 1.1.2.7  1994/09/28  17:54:48  kevins
# 	OT 12376 modified registry unavailable messages
# 	[1994/09/28  17:51:53  kevins]
# 
# Revision 1.1.2.6  1994/09/14  14:34:51  kevins
# 	OT 11641 modified to with tcl list arguments
# 	[1994/09/13  19:26:14  kevins]
# 
# Revision 1.1.2.5  1994/08/12  18:04:28  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:43  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:15:26  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:56:07  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:50  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:28:22  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:16:19  melman
# 	updating to new API path
# 	[1994/05/06  17:14:55  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:22:23  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:46  kevins]
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
proc principal_catalog_1_N {} {
    # Identify the test
    tet_infoline principal_catalog_1_N: Check that invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {principal catalog bar -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc principal_catalog_2_N {} {
    # Identify the test
    tet_infoline principal_catalog_2_N: Check that unauthenticated users are handled correctly.

    # Setup steps
    if {[dcp_wrap {set cell_name /.../[lindex [info level [info level]] 0]
        set cell_name [string tolower $cell_name]}] != 0} {return;}

    # Run the test
    test {
        list [catch {principal catalog $cell_name} msg] $msg
    } compare_strings "1 {Registry server '$cell_name' is unavailable.}"

    # Cleanup steps
}

proc principal_catalog_3_N {} {
    # Identify the test
    tet_infoline principal_catalog_3_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {principal catalog foo foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc principal_catalog_4_N {} {
    # Identify the test
    tet_infoline principal_catalog_4_N: Check that an illegal cell name is trapped.

    # Setup steps

    # Run the test
    test {
        list [catch {principal catalog gumby} msg] $msg
    } compare_strings {1 {The cell name 'gumby' is invalid.}}

    # Cleanup steps
}

proc principal_catalog_5_N {} {
    # Identify the test
    tet_infoline principal_catalog_5_N: Check that an illegal cell name is trapped.

    # Setup steps

    # Run the test
    test {
        list [catch {principal catalog /gumby} msg] $msg
    } compare_strings {1 {Unable to parse cell name '/gumby'.}}

    # Cleanup steps
}

proc principal_catalog_6_N {} {
    # Identify the test
    tet_infoline principal_catalog_6_N: Check that an illegal cell name is trapped.

    # Setup steps

    # Run the test
    test {
        list [catch {principal catalog /.../gumby} msg] $msg
    } compare_strings {1 {Registry server '/.../gumby' is unavailable.}}

    # Cleanup steps
}
set iclist "ic1 ic2 ic3 ic4 ic5 ic6 "
set ic1 "principal_catalog_1_N"
set ic2 "principal_catalog_2_N"
set ic3 "principal_catalog_3_N"
set ic4 "principal_catalog_4_N"
set ic5 "principal_catalog_5_N"
set ic6 "principal_catalog_6_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
