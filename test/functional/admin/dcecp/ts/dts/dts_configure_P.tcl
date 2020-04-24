#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: dts_configure_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:15:21  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:31:02  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:56  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:02  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:37  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:50  melman]
# 
# Revision 1.1.2.5  1994/06/10  20:12:37  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:09  devsrc]
# 
# Revision 1.1.2.4  1994/05/23  18:23:59  rousseau
# 	General updates and performance changes (CR 9938).
# 	[1994/05/23  18:23:06  rousseau]
# 
# Revision 1.1.2.3  1994/05/11  19:37:09  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:20:31  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:35:53  melman
# 	updating to new API path
# 	[1994/05/06  15:35:21  melman]
# 
# Revision 1.1.2.1  1994/04/21  13:57:46  rousseau
# 	Initial drop of dts and clock tests (CR 9938).
# 	[1994/04/21  13:55:55  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc dts_configure_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc dts_configure_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc dts_configure_7_P {} {
    # Identify the test
    tet_infoline dts_configure_7_P: Check to see that we can configure a DTS entity as global.

    # Setup steps
    dcp_hide {dts configure -notglobal}

    # Run the test
    test {
	dts configure -global
    } compare_strings {global}

    # Cleanup steps
}

proc dts_configure_8_P {} {
    # Identify the test
    tet_infoline dts_configure_8_P: Check to see that we can configure a DTS entity as notglobal.

    # Setup steps
    dcp_hide {dts configure -global}

    # Run the test
    test {
	dts configure -notglobal
    } compare_strings {notglobal}

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "dts_configure_7_P"
set ic2 "dts_configure_8_P"
set tet_startup dts_configure_startup
set tet_cleanup dts_configure_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
