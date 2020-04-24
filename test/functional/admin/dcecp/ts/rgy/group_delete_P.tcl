#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: group_delete_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:19:20  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:14  marty]
#
# Revision 1.1.4.2  1995/12/13  15:27:30  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:54:26  root]
# 
# Revision 1.1.2.1  1994/09/28  21:41:57  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:03  melman]
# 
# Revision 1.1.2.5  1994/08/12  18:04:00  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:08  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:14:35  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:55:23  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:08  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:24:17  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:15:47  melman
# 	updating to new API path
# 	[1994/05/06  17:14:16  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:48  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:13  kevins]
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
proc group_delete_4_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline group_delete_4_P: Check to see that we can delete a group.

    # Setup steps
        # Must be running as cell administrator. 
        # Group needs to exist in the registry.
    set gr_name [lindex [info level [info level]] 0]
    if {[dcp_wrap {group create $gr_name}] != 0} {return;}

    # Run the test
    test { group delete $gr_name
         dcp_missing [group catalog -simplename] $gr_name 
         } compare_strings {not found}

        # Expected results are:
        # Successful completion.

    # Cleanup steps
}

proc group_delete_5_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline group_delete_5_P: Check to see that we can delete a list of groups.

    # Setup steps
        # Must be running as cell administrator. 
        # Groups needs to exist in the registry.
    set gr_name [lindex [info level [info level]] 0]
    if {[dcp_wrap {group create {$gr_name foo_$gr_name}}] != 0} {return;}

    # Run the test
    test { group delete {$gr_name foo_$gr_name} 
         dcp_missing [group catalog -simplename] {$gr_name foo_$gr_name} 
         } compare_strings {not found}

    # Cleanup steps

}


set iclist "ic1 ic2 "
set ic1 "group_delete_4_P"
set ic2 "group_delete_5_P"

set tet_startup registry_show_startup
set tet_cleanup registry_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
