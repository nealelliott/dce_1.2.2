#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: dts_synchronize_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:15:44  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:31:23  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:05  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:10  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:46  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:02  melman]
# 
# Revision 1.1.2.5  1994/06/10  20:13:09  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:19  devsrc]
# 
# Revision 1.1.2.4  1994/05/23  18:24:07  rousseau
# 	General updates and performance changes (CR 9938).
# 	[1994/05/23  18:23:15  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc dts_synchronize_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc dts_synchronize_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc dts_synchronize_1_N {} {
    # Identify the test
    tet_infoline dts_synchronize_1_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {dts synchronize /.:/hosts/[exec hostname]/dts-entity foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc dts_synchronize_2_N {} {
    # Identify the test
    tet_infoline dts_synchronize_2_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {dts synchronize /.:/hosts/[exec hostname]/dts-entity -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "dts_synchronize_1_N"
set ic2 "dts_synchronize_2_N"
set tet_startup dts_synchronize_startup
set tet_cleanup dts_synchronize_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
