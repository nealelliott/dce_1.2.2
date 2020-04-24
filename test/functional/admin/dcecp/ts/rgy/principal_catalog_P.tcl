#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: principal_catalog_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:21:16  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:02  marty]
#
# Revision 1.1.4.2  1995/12/13  15:28:16  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/03/09  13:38 UTC  truitt
# 	Test for cellname argument.
# 
# 	HP revision /main/truitt_dcecp_chfts18/1  1995/03/09  13:34 UTC  truitt
# 	CHFts14388: Test that the cellname argument is
# 	handled correctly.
# 	[1995/12/11  15:55:09  root]
# 
# Revision 1.1.2.1  1994/09/28  21:43:05  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:58  melman]
# 
# Revision 1.1.2.5  1994/08/12  18:04:29  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:44  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:15:27  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:56:09  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:51  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:28:25  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:16:20  melman
# 	updating to new API path
# 	[1994/05/06  17:14:57  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:22:24  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:47  kevins]
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

proc principal_catalog_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc principal_catalog_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc principal_catalog_1_P {} {
    # Identify the test
    tet_infoline principal_catalog_1_P: Check that we can show the principal catalog.

    # Setup steps

    # Run the test
    test { if {[dcp_find_pgo [principal catalog] nobody] == 0} {
        set a "found"} else { set a "not_found" } 
    } compare_strings found

    # Cleanup steps
}

proc principal_catalog_2_P {} {
    # Identify the test
    tet_infoline principal_catalog_2_P: Check that we can show the principal with simple pathnames.

    # Setup steps

    # Run the test
    test { if {[dcp_find_pgo [principal catalog -simplename] nobody] == 0 } {
        set a "found" } else {set a "not_found"}
    } compare_strings found
 
   # Cleanup steps
}

proc principal_catalog_3_P {} {
    # Identify the test
    tet_infoline principal_catalog_3_P: Show the principals with a shorthand cell name.

    # Setup steps

    # Run the test
    test { principal catalog /.: } dcp_find_pgo cell_admin

    # Cleanup steps
}

proc principal_catalog_4_P {} {
    global DCP_CELLNAME_ONE
    # Identify the test
    tet_infoline principal_catalog_4_P: Show the principals with a longhand cell name.

    # Setup steps

    # Run the test
    test { principal catalog $DCP_CELLNAME_ONE } dcp_find_pgo cell_admin

    # Cleanup steps
}

proc principal_catalog_5_P {} {
    global DCP_CELLNAME_ONE
    # Identify the test
    tet_infoline principal_catalog_5_P: Show the principals with both cell names.

    # Setup steps

    # Run the test
    test { principal catalog "/.: $DCP_CELLNAME_ONE" } dcp_find_pgo cell_admin

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 "
set ic1 "principal_catalog_1_P"
set ic2 "principal_catalog_2_P"
set ic3 "principal_catalog_3_P"
set ic4 "principal_catalog_4_P"
set ic5 "principal_catalog_5_P"
set tet_startup principal_catalog_startup
set tet_cleanup principal_catalog_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
