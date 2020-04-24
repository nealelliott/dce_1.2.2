#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: organization_list_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:20:49  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:50  marty]
#
# Revision 1.1.4.2  1995/12/13  15:28:07  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:54:59  root]
# 
# Revision 1.1.2.1  1994/09/28  21:42:50  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:47  melman]
# 
# Revision 1.1.2.7  1994/09/21  17:11:11  kevins
# 	OT 9938 Modified pgo functional tests
# 	[1994/09/21  17:10:50  kevins]
# 
# Revision 1.1.2.6  1994/09/01  17:49:32  kevins
# 	OT Fixed messages and some tests.
# 	[1994/09/01  16:06:48  kevins]
# 
# Revision 1.1.2.5  1994/08/12  18:04:19  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:34  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:15:18  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:55:56  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:35  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:25:39  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:16:07  melman
# 	updating to new API path
# 	[1994/05/06  17:14:43  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:22:11  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:36  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup

source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl
proc org_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc org_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc organization_list_4_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_list_4_P: Check to see that we can list the contents of a organization.

    # Setup steps
        # Must be running as cell administrator.
        # The organization foo must exist in the registry.
    if {[dcp_wrap {set name [lindex [info level [info level]] 0]
                   organization create  $name
                   principal create kevin_$name
                   organization add $name -member kevin_$name}] != 0} {return;}

    # Run the test
    test { organization list $name -simplename } dcp_find_pgo kevin_$name
          
    # Cleanup steps
    if {[dcp_wrap {organization delete $name
                   principal delete kevin_$name}] != 0} {return;}
}

proc organization_list_5_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_list_5_P: Check to see that we can list the contents of a list of 
    tet_infoline organization_list_5_P: organizations.

    # Setup steps
        # Must be running as cell administrator.
    if {[dcp_wrap {set name [lindex [info level [info level]] 0]
                   organization create "$name bar_$name"
                   principal create "kevin_$name julie_$name"
                   organization add $name -member kevin_$name
                   organization add bar_$name -member julie_$name \
                    }] != 0} {return;}

    # Run the test
    test { dcp_found [organization list "$name bar_$name" -simplename ] \
           "kevin_$name julie_$name" 
         } compare_strings found
           
    # Cleanup steps
    if {[dcp_wrap {organization delete "$name bar_$name"
                   principal delete "kevin_$name julie_$name"}] != 0} {return;}
}


set iclist "ic1 ic2 "
set ic1 "organization_list_4_P"
set ic2 "organization_list_5_P"

set tet_startup org_startup
set tet_cleanup org_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
