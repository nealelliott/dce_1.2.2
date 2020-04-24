#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: group_modify_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:19:30  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:17  marty]
#
# Revision 1.1.4.2  1995/12/13  15:27:33  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:54:29  root]
# 
# Revision 1.1.2.1  1994/09/28  21:42:02  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:07  melman]
# 
# Revision 1.1.2.5  1994/08/12  18:04:03  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:11  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:14:39  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:55:28  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:14  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:24:33  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:15:50  melman
# 	updating to new API path
# 	[1994/05/06  17:14:21  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:52  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:18  kevins]
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
proc group_modify_6_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline group_modify_6_P: Check to see that we can modify an alias on a group 
    tet_infoline group_modify_6_P: from a option 

    # Setup steps
        # Must be running as cell administrator. 
        # The group foo must exist.
    if {[dcp_wrap {group create foo}] != 0} {return;}

    # Run the test
    test { group modify foo -alias yes 
           dcp_attrlist getval [group show foo] alias } compare_strings yes

    # Cleanup steps
        # The group foo needs to be removed from the registry.
    if {[dcp_wrap {group delete foo}] != 0} {return;}
}

proc group_modify_7_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline group_modify_7_P: Check to see that we can modify an alias from an attribute list 
    tet_infoline group_modify_7_P: on a group

    # Setup steps
        # Must be running as cell administrator. 
        # The group foo must exist.
    if {[dcp_wrap {group create foo}] != 0} {return;}

    # Run the test
    test { group modify foo -change {alias yes}
           dcp_attrlist getval [group show foo] alias } compare_strings yes

    # Cleanup steps
        # The group foo needs to be removed from the registry.
    if {[dcp_wrap {group delete foo}] != 0} {return;}
}

proc group_modify_8_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline group_modify_8_P: Check to see that we can modify the fullname on a group 
    tet_infoline group_modify_8_P: from a option

    # Setup steps
        # Must be running as cell administrator. 
        # The group foo must exist.
    if {[dcp_wrap {group create foo}] != 0} {return;}

    # Run the test
    test { group modify foo -fullname "kevin sullivan"
	dcp_attrlist getval [group show foo] fullname 
         } compare_strings {{kevin sullivan}}

    # Cleanup steps
        # The group foo needs to be removed from the registry.
    if {[dcp_wrap {group delete foo}] != 0} {return;}
}

proc group_modify_9_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline group_modify_9_P: Check to see that we can modify the fullname on a group 
    tet_infoline group_modify_9_P: from an attribute list.

    # Setup steps
        # Must be running as cell administrator. 
        # The group foo must exist.
    if {[dcp_wrap {group create foo}] != 0} {return;}

    # Run the test
    test { group modify foo -change {fullname "kevin sullivan"}
	dcp_attrlist getval [group show foo] fullname 
         } compare_strings {{kevin sullivan}}

    # Cleanup steps
        # The group foo needs to be removed from the registry.
    if {[dcp_wrap {group delete foo}] != 0} {return;}
}

proc group_modify_10_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline group_modify_10_P: Check to see that we can modify the alias on a list of groups
    tet_infoline group_modify_10_P: from an attribute list.

    # Setup steps
        # Must be running as cell administrator. 
        # The groups foo and foo1 must exist.
    if {[dcp_wrap {group create {foo foo_bar}}] != 0} {return;}

    # Run the test
    test { group modify {foo foo_bar} -change {alias yes}
         dcp_attrlist getval [group show {foo foo_bar}] alias 
         } compare_strings yes

    # Cleanup steps
        # The groups foo and foo1 needs to be removed from the registry.
    if {[dcp_wrap {group delete {foo foo_bar}}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5"
set ic1 "group_modify_6_P"
set ic2 "group_modify_7_P"
set ic3 "group_modify_8_P"
set ic4 "group_modify_9_P"
set ic5 "group_modify_10_P"

set tet_startup registry_show_startup
set tet_cleanup registry_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
