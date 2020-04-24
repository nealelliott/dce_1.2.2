#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: secval_status_P.tcl,v $
# Revision 1.1.6.3  1996/03/11  02:27:27  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:17  marty]
#
# Revision 1.1.6.2  1995/12/11  15:16:03  root
# 	Submit
# 	[1995/12/11  14:32:18  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:16  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:15  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:22  melman]
# 
# Revision 1.1.2.1  1994/09/01  20:24:43  pwang
# 	Initial implementation
# 	[1994/09/01  20:19:56  pwang]
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

proc secval_status_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW

    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {
        return;
    }
    catch {secval activate} donotcare
}

proc secval_status_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc secval_status_3_P {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline secval_status_3_P: check to see that we can check status of secval.

    # Setup steps

    # Run the test
    test {
    	list [catch {secval status} msg] $msg
    } compare_strings {0 1}

    # Cleanup steps
}

proc secval_status_4_P {} {
    global DCP_CLIENT DCP_CLIENT_PW DCP_HOSTNAME_THREE

    # Identify the test
    tet_infoline secval_status_4_P: check to see that we can check status of secval by hostname.

    # Setup steps

    # Run the test
    test {
    	list [catch {secval status $DCP_HOSTNAME_THREE} msg] $msg
    } compare_strings {0 1}

    # Cleanup steps
}

proc secval_status_5_P {} {
    global DCP_CLIENT DCP_CLIENT_PW DCP_SR_PROTSEQ DCP_SR_IP_ADDR 

    # Identify the test
    tet_infoline secval_status_5_P: check to see that we can check status of secval by string binding.

    # Setup steps

    # Run the test
    test {
    	list [catch {secval status $DCP_SR_PROTSEQ:$DCP_SR_IP_ADDR} msg] $msg
    } compare_strings {0 1}

    # Cleanup steps
}

proc secval_status_6_P {} {
    global DCP_CLIENT DCP_CLIENT_PW DCP_SR_PROTSEQ DCP_SR_IP_ADDR 

    # Identify the test
    tet_infoline secval_status_6_P: check to see that we can check status of secval by tcl binding.

    # Setup steps

    # Run the test
    test {
    	list [catch {secval status "$DCP_SR_PROTSEQ $DCP_SR_IP_ADDR"} msg] $msg
    } compare_strings {0 1}

    # Cleanup steps
}

proc secval_status_7_P {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline secval_status_7_P: check to see that secval status return 0 if the secval is not up.

    # Setup steps
    catch {secval deactivate} donotcare

    # Run the test
    test {
    	list [catch {secval status} msg] $msg
    } compare_strings {0 0}

    # Cleanup steps
}

set iclist "ic1 ic2 ic3 ic4 ic5 "
set ic1 "secval_status_3_P"
set ic2 "secval_status_4_P"
set ic3 "secval_status_5_P"
set ic4 "secval_status_6_P"
set ic5 "secval_status_7_P"
set tet_startup secval_status_startup
set tet_cleanup secval_status_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
