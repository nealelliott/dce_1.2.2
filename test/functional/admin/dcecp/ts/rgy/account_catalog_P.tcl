#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: account_catalog_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:18:27  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:54  marty]
#
# Revision 1.1.4.2  1995/12/13  15:27:07  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/03/09  14:15 UTC  truitt
# 	Test cellname argument.
# 
# 	HP revision /main/truitt_dcecp_chfts18/1  1995/03/09  14:14 UTC  truitt
# 	CHFts14388: Test that the cellname argument is
# 	handled correctly.
# 	[1995/12/11  15:54:06  root]
# 
# Revision 1.1.2.1  1994/09/28  21:41:20  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:34  melman]
# 
# Revision 1.1.2.5  1994/09/01  17:49:18  kevins
# 	OT Fixed messages and some tests.
# 	[1994/09/01  16:06:29  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:14:00  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:56  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:38:46  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:23:02  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:15:31  melman
# 	updating to new API path
# 	[1994/05/06  17:13:54  melman]
# 
# Revision 1.1.2.1  1994/04/15  19:24:16  kevins
# 	CR 9938 add account functional tests
# 	[1994/04/15  15:31:16  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc account_catalog_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc account_catalog_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc account_catalog_1_P {} {
    # Identify the test
    tet_infoline account_catalog_1_P: Check to see that we can show the account catalog.

    # Setup steps

    # Run the test
    test { account catalog } dcp_find_pgo cell_admin

    # Cleanup steps
}

proc account_catalog_2_P {} {
    # Identify the test
    tet_infoline account_catalog_2_P: Show the account with simple pathnames.

    # Setup steps

    # Run the test
    test { account catalog -simplename } dcp_find_pgo cell_admin

    # Cleanup steps
}

proc account_catalog_3_P {} {
    # Identify the test
    tet_infoline account_catalog_3_P: Show the accounts with a shorthand cell name.

    # Setup steps

    # Run the test
    test { account catalog /.: } dcp_find_pgo cell_admin

    # Cleanup steps
}

proc account_catalog_4_P {} {
    global DCP_CELLNAME_ONE
    # Identify the test
    tet_infoline account_catalog_4_P: Show the accounts with a longhand cell name.

    # Setup steps

    # Run the test
    test { account catalog $DCP_CELLNAME_ONE } dcp_find_pgo cell_admin

    # Cleanup steps
}

proc account_catalog_5_P {} {
    global DCP_CELLNAME_ONE
    # Identify the test
    tet_infoline account_catalog_5_P: Show the accounts with both cell names.

    # Setup steps

    # Run the test
    test { account catalog "/.: $DCP_CELLNAME_ONE" } dcp_find_pgo cell_admin

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 "
set ic1 "account_catalog_1_P"
set ic2 "account_catalog_2_P"
set ic3 "account_catalog_3_P"
set ic4 "account_catalog_4_P"
set ic5 "account_catalog_5_P"
set tet_startup account_catalog_startup
set tet_cleanup account_catalog_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
