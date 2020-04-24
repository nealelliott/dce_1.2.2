#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: secval_deactivate_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:27:10  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:11  marty]
#
# Revision 1.1.4.1  1995/12/11  15:15:46  root
# 	Submit
# 	[1995/12/11  14:32:13  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:09  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:05  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:14  melman]
# 
# Revision 1.1.2.7  1994/09/01  20:24:41  pwang
# 	Updated to new changes in code [OT#11093]
# 	[1994/09/01  20:17:11  pwang]
# 
# Revision 1.1.2.6  1994/07/01  17:25:20  pwang
# 	Adjusted for error code testing [OT#11093]
# 	[1994/07/01  17:24:35  pwang]
# 
# Revision 1.1.2.5  1994/06/10  20:16:28  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:56:28  devsrc]
# 
# Revision 1.1.2.4  1994/05/25  21:30:07  pwang
# 	Moved setup and cleanup to setup.tcl and cleanup.tcl
# 	[1994/05/25  21:23:02  pwang]
# 
# Revision 1.1.2.3  1994/05/11  19:40:54  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:29:38  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:21:59  melman
# 	updating to new API path
# 	[1994/05/06  17:21:42  melman]
# 
# Revision 1.1.2.1  1994/05/04  22:37:48  pwang
# 	Initial Implementation
# 	[1994/05/04  22:30:46  pwang]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup and common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_setup.tcl
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

## Test purposes
proc secval_deactivate_1_N {} {

    # Identify the test
    tet_infoline secval_deactivate_1_N: check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {secval deactivate -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc secval_deactivate_2_N {} {

    # Identify the test
    tet_infoline secval_deactivate_2_N: check that a wrong host is returned as invalid arg. 

    # Setup steps

    # Run the test
    test {
        list [catch {secval deactivate foo} msg] $msg
    } compare_strings {1 {Bad or wrong arguments}}

    # Cleanup steps
}

proc secval_deactivate_3_N {} {
    global DCP_CLIENT DCP_CLIENT_PW DCP_NOTROOT DCP_NOTROOT_PW

    # Identify the test
    tet_infoline secval_deactivate_3_N: check that unauthenticated users are handled correctly.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {group add none -member $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {account create $DCP_NOTROOT -group none -org none -password $DCP_NOTROOT_PW -mypwd $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {login $DCP_NOTROOT -p $DCP_NOTROOT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {secval deactivate} msg] $msg
    } compare_strings {1 {permission not valid for this acl}}

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {account delete $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc secval_deactivate_4_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline secval_deactivate_4_N: deactivate secval which is not active returns an error.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    catch {secval deactivate} donotcare

    # Run the test
    test {
        list [catch {secval deactivate} msg] $msg
    } compare_strings {1 {secval service is not enabled}}

    # Cleanup steps
    catch {secval activate} donotcare
    if {[dcp_wrap {logout}] != 0} {return;}
}

set iclist "ic1 ic2 ic3 ic4 "
set ic1 "secval_deactivate_1_N"
set ic2 "secval_deactivate_2_N"
set ic3 "secval_deactivate_3_N"
set ic4 "secval_deactivate_4_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
