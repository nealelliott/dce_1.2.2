#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: organization_catalog_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:20:31  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:43  marty]
#
# Revision 1.1.4.2  1995/12/13  15:28:01  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/03/09  13:56 UTC  truitt
# 	Test cellname argument.
# 
# 	HP revision /main/truitt_dcecp_chfts18/1  1995/03/09  13:53 UTC  truitt
# 	CHFts14388: Test that the cellname argument is
# 	handled correctly.
# 	[1995/12/11  15:54:54  root]
# 
# Revision 1.1.2.1  1994/09/28  21:42:42  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:40  melman]
# 
# Revision 1.1.2.5  1994/08/12  18:04:13  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:27  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:15:11  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:55:40  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:28  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:25:14  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:16:00  melman
# 	updating to new API path
# 	[1994/05/06  17:14:35  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:22:03  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:29  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc organization_catalog_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc organization_catalog_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc organization_catalog_1_P {} {
    # Identify the test
    tet_infoline organization_catalog_1_P: Check that we can show the organization catalog.

    # Setup steps
    set org_name [lindex [info level [info level]] 0]        
    if {[dcp_wrap {organization create $org_name}] != 0} {return;}

    # Run the test
    test { if {[lsearch [organization catalog] */$org_name] != -1 } {
        list found} else {list not_found}
    } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {organization delete $org_name}] != 0} {return;}
}

proc organization_catalog_2_P {} {
    # Identify the test
    tet_infoline organization_catalog_2_P: Check that we can show the organization with simple pathnames.

    # Setup steps
    set org_name [lindex [info level [info level]] 0]         
    if {[dcp_wrap {organization create $org_name}] != 0} {return;}

    # Run the test
    test { if {[lsearch [organization catalog -simplename] $org_name] != -1 } {
        list "found"} else {list not_found}
    } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {organization delete $org_name}] != 0} {return;}
}

proc organization_catalog_3_P {} {
    # Identify the test
    tet_infoline organization_catalog_3_P: Show the organizations with a shorthand cell name.

    # Setup steps
    set org_name [lindex [info level [info level]] 0]         
    if {[dcp_wrap {organization create $org_name}] != 0} {return;}

    # Run the test
    test { organization catalog /.: } dcp_find_pgo $org_name

    # Cleanup steps
    if {[dcp_wrap {organization delete $org_name}] != 0} {return;}
}

proc organization_catalog_4_P {} {
    global DCP_CELLNAME_ONE
    # Identify the test
    tet_infoline organization_catalog_4_P: Show the organizations with a longhand cell name.

    # Setup steps
    set org_name [lindex [info level [info level]] 0]         
    if {[dcp_wrap {organization create $org_name}] != 0} {return;}

    # Run the test
    test { organization catalog $DCP_CELLNAME_ONE } dcp_find_pgo $org_name

    # Cleanup steps
    if {[dcp_wrap {organization delete $org_name}] != 0} {return;}
}

proc organization_catalog_5_P {} {
    global DCP_CELLNAME_ONE
    # Identify the test
    tet_infoline organization_catalog_5_P: Show the organizations with both cell names.

    # Setup steps
    set org_name [lindex [info level [info level]] 0]         
    if {[dcp_wrap {organization create $org_name}] != 0} {return;}

    # Run the test
    test { organization catalog "/.: $DCP_CELLNAME_ONE" } dcp_find_pgo $org_name

    # Cleanup steps
    if {[dcp_wrap {organization delete $org_name}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 "
set ic1 "organization_catalog_1_P"
set ic2 "organization_catalog_2_P"
set ic3 "organization_catalog_3_P"
set ic4 "organization_catalog_4_P"
set ic5 "organization_catalog_5_P"
set tet_startup organization_catalog_startup
set tet_cleanup organization_catalog_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
