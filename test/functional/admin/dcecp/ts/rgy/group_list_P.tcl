#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: group_list_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:19:25  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:15  marty]
#
# Revision 1.1.4.2  1995/12/13  15:27:31  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:54:28  root]
# 
# Revision 1.1.2.1  1994/09/28  21:42:00  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:05  melman]
# 
# Revision 1.1.2.5  1994/09/01  17:49:28  kevins
# 	OT Fixed messages and some tests.
# 	[1994/09/01  16:06:43  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:14:37  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:55:25  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:09  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:24:25  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:15:48  melman
# 	updating to new API path
# 	[1994/05/06  17:14:19  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:50  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:16  kevins]
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
proc group_list_4_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline group_list_4_P: Check to see that we can list the contents of a group.

    # Setup steps
        # Must be running as cell administrator.
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create kevin}] != 0} {return;}
    if {[dcp_wrap {group create foo}] != 0} {return;}
    if {[dcp_wrap { group add foo -member kevin }] != 0} {return;}

    # Run the test
    test { group list foo } dcp_find_pgo kevin 

    # Cleanup steps
    if {[dcp_wrap {group delete foo}] != 0} {return;}
    if {[dcp_wrap {principal delete kevin}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc group_list_5_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline group_list_5_P: Check to see that we can list the contents of a list of groups.

    # Setup steps
        # Must be running as cell administrator.
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create {kevin howard}}] != 0} {return;}
    if {[dcp_wrap {group create {foo foo_bar}}] != 0} {return;}
    if {[dcp_wrap {group add foo -member kevin }] != 0} {return;}
    if {[dcp_wrap {group add foo_bar -member howard }] != 0} {return;}

    # Run the test
    test { dcp_found [group list {foo foo_bar} -simplename] {kevin howard}
         } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {group delete {foo foo_bar}}] != 0} {return;}
    if {[dcp_wrap {principal delete {kevin howard}}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}


set iclist "ic1 ic2 "
set ic1 "group_list_4_P"
set ic2 "group_list_5_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
