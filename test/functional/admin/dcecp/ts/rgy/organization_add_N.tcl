#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: organization_add_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:20:23  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:41  marty]
#
# Revision 1.1.4.2  1995/12/13  15:27:58  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:54:51  root]
# 
# Revision 1.1.2.1  1994/09/28  21:42:36  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:36  melman]
# 
# Revision 1.1.2.5  1994/08/12  18:04:10  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:23  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:14:45  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:55:37  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:23  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:25:01  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:15:57  melman
# 	updating to new API path
# 	[1994/05/06  17:14:31  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:22:00  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:26  kevins]
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
proc organization_add_1_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_add_1_N: Check that not enough arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {organization add foo } msg] $msg
    } compare_strings {1 {No member name was specified.}}

    # Cleanup steps
}

proc organization_add_2_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_add_2_N: Check that unauthenticated users are handled correctly.

    # Setup steps
        # Must be running as cell administrator. 
    set org_name [lindex [info level [info level]] 0]
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {organization create $org_name}] != 0} {return;}
    if {[dcp_wrap {principal create alexis}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test {
        list [catch {organization add $org_name -member alexis} msg] $msg
    } compare_strings {1 {User is not authorized to update record}}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {organization delete $org_name}] != 0} {return;}
    if {[dcp_wrap {principal delete alexis}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

}


set iclist "ic1 ic2 "
set ic1 "organization_add_1_N"
set ic2 "organization_add_2_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
