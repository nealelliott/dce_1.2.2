#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: dts_synchronize_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:15:46  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:31:25  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:06  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:11  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:47  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:03  melman]
# 
# Revision 1.1.2.6  1994/09/02  22:21:11  rousseau
# 	Mass test updates.
# 	[1994/09/02  22:20:47  rousseau]
# 
# Revision 1.1.2.5  1994/06/10  20:13:10  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:20  devsrc]
# 
# Revision 1.1.2.4  1994/05/23  18:24:07  rousseau
# 	General updates and performance changes (CR 9938).
# 	[1994/05/23  18:23:15  rousseau]
# 
# Revision 1.1.2.3  1994/05/11  19:37:21  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:21:13  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:36:01  melman
# 	updating to new API path
# 	[1994/05/06  15:35:32  melman]
# 
# Revision 1.1.2.1  1994/04/21  13:58:04  rousseau
# 	Initial drop of dts and clock tests (CR 9938).
# 	[1994/04/21  13:56:03  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc dts_synchronize_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc dts_synchronize_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc dts_synchronize_3_P {} {
    # Identify the test
    tet_infoline dts_synchronize_3_P: Check to see that we can synchronize a DTS entity with other 
    tet_infoline dts_synchronize_3_P: DTS servers.

    # Setup steps

    # Run the test
    test {
	while {[dcp_get_element [dts show] status] != "enabled"} {
	    exec sleep 2
        }

	dts synchronize
    } compare_strings {}

    # Cleanup steps
}

proc dts_synchronize_4_P {} {
    # Identify the test
    tet_infoline dts_synchronize_4_P: Check handling of -abruptly option.

    # Setup steps

    # NOTE: Changing the system clock causes DCE big problems.
    # Therefore this test will not confirm the functionality of the
    # -abruptly switch.

    # Run the test
    test {
	while {[dcp_get_element [dts show] status] != "enabled"} {
	    exec sleep 2
        }

	dts synchronize -abruptly
    } compare_strings {}

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "dts_synchronize_3_P"
set ic2 "dts_synchronize_4_P"
set tet_startup dts_synchronize_startup
set tet_cleanup dts_synchronize_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
