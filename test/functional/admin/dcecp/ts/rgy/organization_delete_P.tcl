#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: organization_delete_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:20:42  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:48  marty]
#
# Revision 1.1.4.2  1995/12/13  15:28:05  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:54:57  root]
# 
# Revision 1.1.2.1  1994/09/28  21:42:47  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:45  melman]
# 
# Revision 1.1.2.7  1994/09/21  17:11:09  kevins
# 	OT 9938 Modified pgo functional tests
# 	[1994/09/21  17:10:49  kevins]
# 
# Revision 1.1.2.6  1994/09/01  17:49:31  kevins
# 	OT Fixed messages and some tests.
# 	[1994/09/01  16:06:47  kevins]
# 
# Revision 1.1.2.5  1994/08/12  18:04:17  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:32  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:15:16  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:55:53  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:33  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:25:31  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:16:05  melman
# 	updating to new API path
# 	[1994/05/06  17:14:41  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:22:08  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:34  kevins]
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
proc organization_delete_4_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_delete_4_P: Check to see that we can delete a organization.

    # Setup steps
        # Must be running as cell administrator. 
        # Organization needs to exist in the registry.
     if {[dcp_wrap {set name [lindex [info level [info level]] 0]
                    organization create $name}] != 0} {return;}

    # Run the test
    test { organization delete $name
           dcp_missing [organization catalog -simplename] $name
         } compare_strings {not found}

    # Cleanup steps
}

proc organization_delete_5_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_delete_5_P: Check to see that we can delete a list of organizations.

    # Setup steps
        # Must be running as cell administrator. 
        # Organizations needs to exist in the registry.
     if {[dcp_wrap {set name [lindex [info level [info level]] 0]
                    organization create "$name bar_$name"}] != 0} {return;}


    # Run the test
    test { organization delete "$name bar_$name"
           dcp_missing [organization catalog -simplename] "$name bar_$name"
         } compare_strings {not found}

}


set iclist "ic1 ic2 "
set ic1 "organization_delete_4_P"
set ic2 "organization_delete_5_P"

set tet_startup org_startup
set tet_cleanup org_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
