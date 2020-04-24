#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: secval_activate_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:27:04  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:09  marty]
#
# Revision 1.1.4.1  1995/12/11  15:15:40  root
# 	Submit
# 	[1995/12/11  14:32:11  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:07  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:02  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:11  melman]
# 
# Revision 1.1.2.5  1994/06/10  20:16:04  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:56:26  devsrc]
# 
# Revision 1.1.2.4  1994/05/25  21:30:02  pwang
# 	Moved setup and cleanup to setup.tcl and cleanup.tcl
# 	[1994/05/25  21:21:44  pwang]
# 
# Revision 1.1.2.3  1994/05/11  19:40:53  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:29:29  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:21:58  melman
# 	updating to new API path
# 	[1994/05/06  17:21:41  melman]
# 
# Revision 1.1.2.1  1994/05/04  22:37:46  pwang
# 	Initial Implementation
# 	[1994/05/04  22:28:52  pwang]
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
proc secval_activate_5_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline secval_activate_5_P: check to see that we can activate secval.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {
    	return;
    }
    catch {secval deactivate} donotcare

    # Run the test
    test {
    	list [catch {secval activate} msg] $msg
    } compare_strings {0 {}}

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {
    	return;
    }

}

proc secval_activate_6_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW DCP_HOSTNAME_THREE

    # Identify the test
    tet_infoline secval_activate_6_P: check to see that we can activate secval by hostname.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {
    	return;
    }
    catch {secval deactivate $DCP_HOSTNAME_THREE} donotcare

    # Run the test
    test {
    	list [catch {secval activate $DCP_HOSTNAME_THREE} msg] $msg
    } compare_strings {0 {}}

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {
    	return;
    }
}


set iclist "ic1 ic2 "
set ic1 "secval_activate_5_P"
set ic2 "secval_activate_6_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
