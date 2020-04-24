#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: aud_modify_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:10:02  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:05  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:29  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:51:28  root]
# 
# Revision 1.1.2.2  1994/10/01  01:33:22  rousseau
# 	Updated for auditd changes.
# 	[1994/10/01  01:24:53  rousseau]
# 
# Revision 1.1.2.1  1994/09/28  21:37:59  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:36  melman]
# 
# Revision 1.1.2.4  1994/06/10  20:02:28  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:35  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:34:46  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:15:58  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:26:51  rousseau
# 	Performance changes, and new API path (CR 10558).
# 	[1994/05/06  15:03:56  rousseau]
# 
# Revision 1.1.2.1  1994/04/21  20:28:35  rousseau
# 	Initial drop of audit tests (CR 9938).
# 	[1994/04/21  20:27:15  rousseau]
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
proc aud_modify_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc aud_modify_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}    


proc aud_modify_6_P {} {
    # Identify the test
    tet_infoline aud_modify_6_P: Check the handling of the -stostrategy stop option.

    # Setup steps
    if {[dcp_wrap {aud modify -stostrategy wrap}] != 0} {return;}

    # Run the test
    test {
	aud modify -stostrategy save
	list [dcp_get_element [aud show] stostrategy]
    } compare_strings {save}

    # Cleanup steps
}

proc aud_modify_7_P {} {
    # Identify the test
    tet_infoline aud_modify_7_P: Check the handling of the -stostrategy wrap option.

    # Setup steps

    # Run the test
    test {
	aud modify -stostrategy wrap
	list [dcp_get_element [aud show] stostrategy]
    } compare_strings {wrap}

    # Cleanup steps
}

proc aud_modify_10_P {} {
    # Identify the test
    tet_infoline aud_modify_10_P: Check the handling of the -state enabled option.

    # Setup steps
    dcp_hide {aud disable}

    # Run the test
    test {
	aud modify -state enabled
	list [dcp_get_element [aud show] state]
    } compare_strings {enabled}

    # Cleanup steps
}

proc aud_modify_11_P {} {
    # Identify the test
    tet_infoline aud_modify_11_P: Check the handling of the -state disabled option.

    # Setup steps

    # Run the test
    test {
	aud modify -state disabled
	list [catch {aud show} msg] $msg
    } compare_strings {1 {Operation denied: audit daemon is disabled}}

    # Cleanup steps
    if {[dcp_wrap {aud enable}] != 0} {return;}
}

proc aud_modify_16_P {} {
    # Identify the test
    tet_infoline aud_modify_16_P: Check the handling of the -changeattr option.

    # Setup steps
    dcp_hide {aud modify -change {{stostrategy wrap} {state disabled}}}

    # Run the test
    test {
	aud enable
	aud modify -change {{state enabled} {stostrategy save}}
	list [dcp_get_element [aud show] state] [dcp_get_element [aud show] stostrategy] 
    } compare_strings {enabled save}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 "
set ic1 "aud_modify_6_P"
set ic2 "aud_modify_7_P"
set ic3 "aud_modify_10_P"
set ic4 "aud_modify_11_P"
set ic5 "aud_modify_16_P"
set tet_startup aud_modify_startup
set tet_cleanup aud_modify_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl

