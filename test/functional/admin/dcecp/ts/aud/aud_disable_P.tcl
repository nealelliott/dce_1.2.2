#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: aud_disable_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:09:47  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:02  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:25  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:51:23  root]
# 
# Revision 1.1.2.2  1994/10/01  01:33:21  rousseau
# 	Updated for auditd changes.
# 	[1994/10/01  01:24:51  rousseau]
# 
# Revision 1.1.2.1  1994/09/28  21:37:54  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:32  melman]
# 
# Revision 1.1.2.4  1994/06/10  20:02:24  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:32  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:34:42  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:15:40  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:26:48  rousseau
# 	Performance changes, and new API path (CR 10558).
# 	[1994/05/06  15:24:17  rousseau]
# 
# Revision 1.1.2.1  1994/04/21  20:28:28  rousseau
# 	Initial drop of audit tests (CR 9938).
# 	[1994/04/21  20:27:10  rousseau]
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
proc aud_disable_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc aud_disable_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}    


proc aud_disable_3_P {} {
    global TET_ROOT

    # Identify the test
    tet_infoline aud_disable_3_P: Check to see that we can disable the audit daemon.

    # Setup steps

    # Run the test
    test {
	aud disable
	list [catch {aud show} msg] $msg
    } compare_strings {1 {Operation denied: audit daemon is disabled}}

    # Cleanup steps
    if {[dcp_wrap {aud enable}] != 0} {return;}
}


set iclist "ic1 "
set ic1 "aud_disable_3_P"
set tet_startup aud_disable_startup
set tet_cleanup aud_disable_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl

