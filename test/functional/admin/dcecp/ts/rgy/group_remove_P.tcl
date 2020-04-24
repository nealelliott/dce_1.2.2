#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: group_remove_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:19:43  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:24  marty]
#
# Revision 1.1.4.2  1995/12/13  15:27:39  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:54:36  root]
# 
# Revision 1.1.2.1  1994/09/28  21:42:12  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:15  melman]
# 
# Revision 1.1.2.5  1994/08/12  18:04:05  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:14  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:14:41  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:55:31  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:16  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:24:40  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:15:52  melman
# 	updating to new API path
# 	[1994/05/06  17:14:25  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:54  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:20  kevins]
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
}

proc registry_show_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}
## Test purposes
proc group_remove_3_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline group_remove_3_P: Check to see that we can remove a member to a group

    # Setup steps
        # Must be running as cell administrator. 
        # The group foo must exist and the principal kevin must exist in
        # the named group.
    if {[dcp_wrap {principal create kevin}] != 0} {return;}
    if {[dcp_wrap {group create foo}] != 0} {return;}
    if {[dcp_wrap {group add foo -member kevin}] != 0} {return;}

    # Run the test
    test { group remove foo -member kevin 
         dcp_missing [group list foo -simplename] kevin 
         } compare_strings {not found}

    # Cleanup steps
        # delete the group/principal and logout
    if {[dcp_wrap {group delete foo}] != 0} {return;}
    if {[dcp_wrap {principal delete kevin}] != 0} {return;}
}

proc group_remove_4_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline group_remove_4_P: Check to see that we can remove a list of members to a group

    # Setup steps
        # Must be running as cell administrator. 
        # The groups foo and foo_bar must exist and the groups 
        # kevin and howard must exist in the named groups.
    if {[dcp_wrap {principal create {kevin howard}}] != 0} {return;}
    if {[dcp_wrap {group create {foo foo_bar}}] != 0} {return;}
    if {[dcp_wrap {group add {foo foo_bar} -member {kevin howard}}] != 0 } {return;}

    # Run the test
    test { group remove {foo foo_bar} -member {kevin howard} 
	dcp_missing [group list {foo foo_bar} -simplename] {kevin howard}
          } compare_strings {not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete {kevin howard}}] != 0} {return;}
    if {[dcp_wrap {group delete {foo foo_bar}}] != 0} {return;}
    
}


set iclist "ic1 ic2 "
set ic1 "group_remove_3_P"
set ic2 "group_remove_4_P"


set tet_startup registry_show_startup
set tet_cleanup registry_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
