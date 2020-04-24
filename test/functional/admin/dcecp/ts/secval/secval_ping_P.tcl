#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: secval_ping_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:27:19  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:14  marty]
#
# Revision 1.1.4.1  1995/12/11  15:15:54  root
# 	Submit
# 	[1995/12/11  14:32:15  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:13  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:09  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:18  melman]
# 
# Revision 1.1.2.6  1994/09/09  19:21:48  pwang
# 	Added setup and cleanup proc [OT#12119]
# 	[1994/09/09  19:21:25  pwang]
# 
# Revision 1.1.2.5  1994/09/01  20:24:36  pwang
# 	Updated to new changes in code [OT#11093]
# 	[1994/09/01  20:18:18  pwang]
# 
# Revision 1.1.2.4  1994/06/10  20:16:31  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:56:31  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:41:00  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:29:51  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:22:02  melman
# 	updating to new API path
# 	[1994/05/06  17:21:46  melman]
# 
# Revision 1.1.2.1  1994/05/04  22:37:49  pwang
# 	Initial Implementation
# 	[1994/05/04  22:34:58  pwang]
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


proc secval_ping_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW

    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    catch {secval activate} donotcare
}

proc secval_ping_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc secval_ping_4_P {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline secval_ping_4_P: check to see that we can ping secval.

    # Setup steps

    # Run the test
    test {
    	list [catch {secval ping} msg] $msg
    } compare_strings {0 1}

    # Cleanup steps
}

proc secval_ping_5_P {} {
    global DCP_CLIENT DCP_CLIENT_PW DCP_HOSTNAME_THREE

    # Identify the test
    tet_infoline secval_ping_5_P: check to see that we can ping secval by hostname.

    # Setup steps

    # Run the test
    test {
    	list [catch {secval ping $DCP_HOSTNAME_THREE} msg] $msg
    } compare_strings {0 1}

    # Cleanup steps
}

set iclist "ic1 ic2 "
set ic1 "secval_ping_4_P"
set ic2 "secval_ping_5_P"
set tet_startup secval_ping_startup
set tet_cleanup secval_ping_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
