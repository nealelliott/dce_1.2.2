#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: aud_show_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:10:19  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:09  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:33  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:51:34  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:01  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:39  melman]
# 
# Revision 1.1.2.5  1994/09/02  20:28:32  rousseau
# 	Mass test cleanup.
# 	[1994/09/02  20:27:48  rousseau]
# 
# Revision 1.1.2.4  1994/06/10  20:02:33  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:38  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:34:47  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:16:06  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:26:53  rousseau
# 	Performance changes, and new API path (CR 10558).
# 	[1994/05/06  15:03:57  rousseau]
# 
# Revision 1.1.2.1  1994/04/21  20:28:39  rousseau
# 	Initial drop of audit tests (CR 9938).
# 	[1994/04/21  20:27:16  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_setup.tcl

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


## Test purposes
proc aud_show_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc aud_show_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}    


proc aud_show_3_P {} {
    # Identify the test
    tet_infoline aud_show_3_P: Check to see that we can show auditd attributes using no options.

    # Setup steps
    if {[dcp_wrap {aud modify -state enabled -stostrategy wrap}] != 0} {return;}

    # Run the test
    test {
	aud show
    } compare_lists {{state enabled} {stostrategy wrap}}

    # Cleanup steps
}

proc aud_show_4_P {} {
    # Identify the test
    tet_infoline aud_show_4_P: Check to see that we can show auditd attributes using the 
    tet_infoline aud_show_4_P: -attributes option.

    # Setup steps
    if {[dcp_wrap {aud modify -state enabled -stostrategy wrap}] != 0} {return;}

    # Run the test
    # NOTE: The output should be identical to the previous test.
    test {
	aud show -attributes
    } compare_lists {{state enabled} {stostrategy wrap}}

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "aud_show_3_P"
set ic2 "aud_show_4_P"
set tet_startup aud_show_startup
set tet_cleanup aud_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl

