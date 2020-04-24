#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: group_show_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:19:53  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:27  marty]
#
# Revision 1.1.4.2  1995/12/13  15:27:43  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:54:39  root]
# 
# Revision 1.1.2.1  1994/09/28  21:42:16  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:19  melman]
# 
# Revision 1.1.2.5  1994/08/12  18:04:09  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:21  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:14:44  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:55:36  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:22  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:24:57  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:15:56  melman
# 	updating to new API path
# 	[1994/05/06  17:14:30  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:59  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:25  kevins]
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
proc group_show_3_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline group_show_3_P: Check to see that we can show the group attributes.

    # Setup steps
    set gr_name [lindex [info level [info level]] 0 ]
    if {[dcp_wrap {group create $gr_name}] != 0} {return;} 

    # Run the test
     test {dcp_found [group show $gr_name] {uuid alias gid inprojlist fullname}
     } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {group delete $gr_name}] != 0} {return;} 
}

proc group_show_4_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline group_show_4_P: Check to see that we can show a list of group's attributes.

    # Setup steps
        # Must be running as cell administrator. 
    if {[dcp_wrap {group create {foo foo_bar}}] != 0} {return;} 

    # Run the test
     test {dcp_found [group show {foo foo_bar}] {uuid alias gid inprojlist fullname}
     } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {group delete {foo foo_bar}}] != 0} {return;} 
}


set iclist "ic1 ic2 "
set ic1 "group_show_3_P"
set ic2 "group_show_4_P"


set tet_startup registry_show_startup
set tet_cleanup registry_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
