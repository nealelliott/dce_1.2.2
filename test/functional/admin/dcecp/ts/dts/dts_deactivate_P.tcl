#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: dts_deactivate_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:15:25  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:31:07  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:58  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:04  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:39  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:52  melman]
# 
# Revision 1.1.2.6  1994/09/02  22:21:03  rousseau
# 	Mass test updates.
# 	[1994/09/02  22:20:37  rousseau]
# 
# Revision 1.1.2.5  1994/06/10  20:12:39  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:12  devsrc]
# 
# Revision 1.1.2.4  1994/05/23  18:24:01  rousseau
# 	General updates and performance changes (CR 9938).
# 	[1994/05/23  18:23:08  rousseau]
# 
# Revision 1.1.2.3  1994/05/11  19:37:11  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:20:40  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:35:54  melman
# 	updating to new API path
# 	[1994/05/06  15:35:23  melman]
# 
# Revision 1.1.2.1  1994/04/21  13:57:48  rousseau
# 	Initial drop of dts and clock tests (CR 9938).
# 	[1994/04/21  13:55:56  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc dts_deactivate_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc dts_deactivate_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc dts_deactivate_3_P {} {
    # Identify the test
    tet_infoline dts_deactivate_3_P: Check to see that we can deactivate a DTS entity.

    # Setup steps
    dcp_hide {dts activate}

    # Run the test
    test {
	dts deactivate
	list [dcp_get_element [dts show] status]
    } compare_strings {disabled}

    # Cleanup steps
    if {[dcp_wrap {dts activate}] != 0} {return;}
}


set iclist "ic1 "
set ic1 "dts_deactivate_3_P"
set tet_startup dts_deactivate_startup
set tet_cleanup dts_deactivate_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
