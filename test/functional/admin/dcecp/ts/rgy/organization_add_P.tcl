#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: organization_add_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:20:25  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:42  marty]
#
# Revision 1.1.4.2  1995/12/13  15:27:59  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:54:52  root]
# 
# Revision 1.1.2.1  1994/09/28  21:42:38  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:37  melman]
# 
# Revision 1.1.2.5  1994/08/12  18:04:11  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:25  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:14:46  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:55:38  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:24  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:25:05  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:15:58  melman
# 	updating to new API path
# 	[1994/05/06  17:14:33  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:22:02  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:27  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl
proc registry_show_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create {kevin julie}}] != 0} {return;}
}

proc registry_show_cleanup {} {
    if {[dcp_wrap {principal delete {kevin julie}}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}


## Test purposes
proc organization_add_3_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_add_3_P: Check to see that we can add a member to a organization.

    # Setup steps
        # Must be running as cell administrator. 
        # The organization foo must exist and the principal kevin must exist.
    set org_name [lindex [info level [info level]] 0]
    if {[dcp_wrap {organization create $org_name}] != 0} {return;}

    # Run the test
    test { organization add $org_name -member kevin
         organization list $org_name -simplename 
         } compare_strings kevin

        # Expected results are:
        # Successful completion.

    # Cleanup steps
        # The member must be removed from the organization. 
    if {[dcp_wrap {organization delete $org_name}] != 0} {return;}
}

proc organization_add_4_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_add_4_P: Check to see that we can add a list 
    tet_infoline organization_add_4_P: of members to a list of organizations.

    # Setup steps
        # Must be running as cell administrator. 
        # The organizations must exist and the principals 
        # kevin and howard must exist. 
    set org_name [lindex [info level [info level]] 0]    
    if {[dcp_wrap {organization create "$org_name 1_$org_name"}] != 0} {return;}

    # Run the test
    test { organization add "$org_name 1_$org_name" -member {kevin julie}
         dcp_found [organization list 1_$org_name -simplename] {kevin julie}
         } compare_strings found

        # Expected results are:
        # Successful completion.

    # Cleanup steps
        # The members must be removed from the organizations. 
    if {[dcp_wrap {organization delete "$org_name 1_$org_name"}] != 0} {return;}
}


set iclist "ic1 ic2 "
set ic1 "organization_add_3_P"
set ic2 "organization_add_4_P"

set tet_startup registry_show_startup
set tet_cleanup registry_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
