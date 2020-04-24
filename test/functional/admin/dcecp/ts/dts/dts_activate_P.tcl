#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: dts_activate_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:15:16  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:30:57  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:54  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:00  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:35  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:46  melman]
# 
# Revision 1.1.2.6  1994/09/02  22:21:00  rousseau
# 	Mass test updates.
# 	[1994/09/02  22:20:35  rousseau]
# 
# Revision 1.1.2.5  1994/06/10  20:12:35  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:08  devsrc]
# 
# Revision 1.1.2.4  1994/05/23  18:23:57  rousseau
# 	General updates and performance changes (CR 9938).
# 	[1994/05/23  18:23:04  rousseau]
# 
# Revision 1.1.2.3  1994/05/11  19:37:08  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:20:23  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:35:51  melman
# 	updating to new API path
# 	[1994/05/06  15:35:19  melman]
# 
# Revision 1.1.2.1  1994/04/21  13:57:43  rousseau
# 	Initial drop of dts and clock tests (CR 9938).
# 	[1994/04/21  13:55:53  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_setup.tcl
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc dts_activate_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc dts_activate_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc dts_activate_3_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline dts_activate_3_P: Check to see that we can activate a DTS entity.

    # Setup steps
    dcp_hide {dts deactivate}

    # Run the test
    test {
	dts activate

	# Wait for the time servers to sync, otherwise the result of dts 
        # show will be syncing
	while {[dcp_get_element [dts show] status] != "enabled"} {
	    exec sleep 2
        }

	list [dcp_get_element [dts show] status]
    } compare_strings {enabled}

    # Cleanup steps
}

proc dts_activate_4_P {} {
    # Identify the test
    tet_infoline dts_activate_4_P: Check handling of -abruptly option.

    # Setup steps
    dcp_hide {dts deactivate}
    # NOTE: Changing the system clock causes DCE big problems.
    # Therefore this test will not confirm the functionality of the
    # -abruptly switch.

    # Run the test
    test {
        dts activate -abruptly

	# Wait for the time servers to sync, otherwise the result of dts 
        # show will be syncing
	while {[dcp_get_element [dts show] status] != "enabled"} {
	    exec sleep 2
        }

	list [dcp_get_element [dts show] status]
    } compare_strings {enabled}

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "dts_activate_3_P"
set ic2 "dts_activate_4_P"
set tet_startup dts_activate_startup
set tet_cleanup dts_activate_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
