#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: dts_stop_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:15:39  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:31:20  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:03  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:08  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:44  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:58  melman]
# 
# Revision 1.1.2.6  1994/09/02  22:21:09  rousseau
# 	Mass test updates.
# 	[1994/09/02  22:20:43  rousseau]
# 
# Revision 1.1.2.5  1994/06/10  20:12:43  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:17  devsrc]
# 
# Revision 1.1.2.4  1994/05/23  18:24:05  rousseau
# 	General updates and performance changes (CR 9938).
# 	[1994/05/23  18:23:13  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc dts_stop_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc dts_stop_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc dts_stop_1_N {} {
    global DCP_HOSTNAME_ONE

    # Identify the test
    tet_infoline dts_stop_1_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {dts stop /.:/hosts/$DCP_HOSTNAME_ONE/dts-entity foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc dts_stop_2_N {} {
    global DCP_HOSTNAME_ONE

    # Identify the test
    tet_infoline dts_stop_2_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {dts stop /.:/hosts/$DCP_HOSTNAME_ONE/dts-entity -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "dts_stop_1_N"
set ic2 "dts_stop_2_N"
set tet_startup dts_stop_startup
set tet_cleanup dts_stop_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
