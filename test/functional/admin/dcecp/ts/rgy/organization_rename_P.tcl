#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: organization_rename_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:21:06  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:58  marty]
#
# Revision 1.1.4.2  1995/12/13  15:28:12  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:55:05  root]
# 
# Revision 1.1.2.1  1994/09/28  21:43:00  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:54  melman]
# 
# Revision 1.1.2.7  1994/09/01  17:49:33  kevins
# 	OT Fixed messages and some tests.
# 	[1994/09/01  16:06:50  kevins]
# 
# Revision 1.1.2.6  1994/08/30  18:28:58  kevins
# 	OT 10617 modified messages/output for functional tests
# 	[1994/08/30  18:28:33  kevins]
# 
# Revision 1.1.2.5  1994/08/12  18:04:25  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:40  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:15:23  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:56:04  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:47  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:28:10  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:16:15  melman
# 	updating to new API path
# 	[1994/05/06  17:14:50  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:22:18  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:42  kevins]
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
proc organization_rename_5_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_rename_5_P: Check t see that we can rename the organization.

    # Setup steps
        # Must be running as cell administrator. 
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set org_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {organization create $org_name}] != 0} {return;}

    # Run the test
    test { organization rename $org_name -to foo_$org_name
           dcp_found [organization catalog -simplename] foo_$org_name
        } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {organization delete foo_$org_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}


set iclist "ic1 "
set ic1 "organization_rename_5_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
