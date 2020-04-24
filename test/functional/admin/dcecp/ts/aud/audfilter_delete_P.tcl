#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: audfilter_delete_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:11:10  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:18  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:44  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:51:50  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:14  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:54  melman]
# 
# Revision 1.1.2.5  1994/06/10  20:03:20  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:57  devsrc]
# 
# Revision 1.1.2.4  1994/05/23  18:09:25  rousseau
# 	Various updates.
# 	[1994/05/23  18:08:59  rousseau]
# 
# Revision 1.1.2.3  1994/05/11  19:35:07  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:16:56  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:27:06  rousseau
# 	Performance changes, and new API path (CR 10558).
# 	[1994/05/06  15:24:51  rousseau]
# 
# Revision 1.1.2.1  1994/04/21  20:28:54  rousseau
# 	Initial drop of audit tests (CR 9938).
# 	[1994/04/21  20:27:32  rousseau]
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
proc audfilter_delete_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc audfilter_delete_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}


proc audfilter_delete_3_P {} {
    # Identify the test
    tet_infoline audfilter_delete_3_P: Check that we can delete a single filter.

    # Setup steps
    if {[dcp_wrap {audfilter create world -att {dce_dts_synch denial log}}] != 0} {return;}

    # Run the test
    test {
	audfilter delete world
	audfilter catalog
    } compare_strings {}

    # Cleanup steps
}

proc audfilter_delete_4_P {} {
    # Identify the test
    tet_infoline audfilter_delete_4_P: Check that we can delete a list of filters.

    # Setup steps
    if {[dcp_wrap {principal create audfilter_delete_4_P}] != 0} {return;}
    if {[dcp_wrap {group create audfilter_delete_4_P}] != 0} {return;}
    if {[dcp_wrap {audfilter create world -att {dce_dts_synch denial log}}] != 0} {return;}
    if {[dcp_wrap {audfilter create {principal audfilter_delete_4_P} -att {dce_dts_synch denial log}}] != 0} {return;}
    if {[dcp_wrap {audfilter create {group audfilter_delete_4_P} -att {dce_dts_synch denial log}}] != 0} {return;}

    # Run the test
    test {
	audfilter delete {{principal audfilter_delete_4_P} {group audfilter_delete_4_P} world}
	audfilter catalog
    } compare_strings {}

    # Cleanup steps
    if {[dcp_wrap {principal delete audfilter_delete_4_P}] != 0} {return;}
    if {[dcp_wrap {group delete audfilter_delete_4_P}] != 0} {return;}
}


set iclist "ic1 ic2 "
set ic1 "audfilter_delete_3_P"
set ic2 "audfilter_delete_4_P"
set tet_startup audfilter_delete_startup
set tet_cleanup audfilter_delete_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl

