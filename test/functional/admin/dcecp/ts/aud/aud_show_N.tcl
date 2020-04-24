#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: aud_show_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:10:16  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:08  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:32  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:51:33  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:00  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:38  melman]
# 
# Revision 1.1.2.5  1994/09/02  20:28:31  rousseau
# 	Mass test cleanup.
# 	[1994/09/02  20:27:47  rousseau]
# 
# Revision 1.1.2.4  1994/06/10  20:02:31  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:36  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:34:46  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:16:02  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:26:52  rousseau
# 	Performance changes, and new API path (CR 10558).
# 	[1994/05/06  15:03:57  rousseau]
# 
# Revision 1.1.2.1  1994/04/21  20:28:37  rousseau
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


proc aud_show_1_N {} {
    global DCP_HOSTNAME_ONE

    # Identify the test
    tet_infoline aud_show_1_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {aud show /.:/hosts/$DCP_HOSTNAME_ONE/audit-server foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc aud_show_2_N {} {
    global DCP_HOSTNAME_ONE

    # Identify the test
    tet_infoline aud_show_2_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {aud show /.:/hosts/$DCP_HOSTNAME_ONE/audit-server -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "aud_show_1_N"
set ic2 "aud_show_2_N"
set tet_startup aud_show_startup
set tet_cleanup aud_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl

