#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: hostdata_catalog_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:16:09  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:31:43  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:14  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/06/27  13:30 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts36/1  1995/06/27  13:29 UTC  truitt
# 	CHFts15407: Add tests for new hostdata behavior.
# 	[1995/12/11  15:53:18  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:57  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:14  melman]
# 
# Revision 1.1.2.4  1994/06/22  20:42:42  baum
# 	Merged with changes from 1.1.2.3
# 	[1994/06/22  20:42:19  baum]
# 
# 	OT #11022 - Use tet_setup, use tetexec.cfg for cell/host
# 	[1994/06/22  20:40:14  baum]
# 
# Revision 1.1.2.3  1994/06/17  18:43:13  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:25:30  devsrc]
# 
# Revision 1.1.2.2  1994/06/07  23:13:44  baum
# 	Mods to handle new dcecp output (CR #10761)
# 	[1994/06/07  23:13:15  baum]
# 
# Revision 1.1.2.1  1994/05/19  22:51:52  baum
# 	Initial checkin
# 	[1994/05/19  22:51:22  baum]
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

## Routine specific to hostdata tests
source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_misc.tcl

proc tet_setup {} { hd_std_setup }
proc tet_cleanup {} { hd_std_cleanup }

## Test purposes
proc hostdata_catalog_5_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_vars.tcl

    # Identify the test
    tet_infoline hostdata_catalog_5_P: Check that we can catalog hostdata objects.

    # Setup steps
    tet_setup

    # Run the test
    set cmd "hostdata catalog"
    set ret [catch "$cmd" msg2]
    set actual_output [lsort "$msg2"]
    set expected_output [list $HD1_SHORT_NAME $HD2_SHORT_NAME]
    compare_list_and_report $cmd $ret $actual_output $expected_output

    # Cleanup steps, remove HD1
    tet_cleanup	
}

proc hostdata_catalog_6_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_vars.tcl

    # Identify the test
    tet_infoline hostdata_catalog_6_P: Check that we can catalog hostdata objects by hostname.

    # Setup steps
    tet_setup

    # Run the test
    set cmd "hostdata catalog hosts/$CUR_HOST_NAME"
    set ret [catch "$cmd" msg2]
    set actual_output [lsort "$msg2"]
    set expected_output [list $HD1_SHORT_NAME $HD2_SHORT_NAME]
    compare_list_and_report $cmd $ret $actual_output $expected_output

    # Cleanup steps, remove HD1
    tet_cleanup	
}

proc hostdata_catalog_7_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW DCP_RGY_SA_PROTSEQ_MASTER DCP_RGY_SA_NETADDR_MASTER

    source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_vars.tcl

    # Identify the test
    tet_infoline hostdata_catalog_7_P: Check that we can catalog hostdata objects by address.

    # Setup steps
    tet_setup

    # Run the test
    set cmd "hostdata catalog $DCP_RGY_SA_PROTSEQ_MASTER:$DCP_RGY_SA_NETADDR_MASTER"
    set ret [catch "$cmd" msg2]
    set actual_output [lsort "$msg2"]
    set expected_output [list $HD1_SHORT_NAME $HD2_SHORT_NAME]
    compare_list_and_report $cmd $ret $actual_output $expected_output

    # Cleanup steps, remove HD1
    tet_cleanup	
}


set iclist "ic1 ic2 ic3 "
set ic1 "hostdata_catalog_5_P"
set ic2 "hostdata_catalog_6_P"
set ic3 "hostdata_catalog_7_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
