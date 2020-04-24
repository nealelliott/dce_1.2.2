#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: hostdata_show_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:16:28  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:03  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:21  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/06/27  13:33 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts36/1  1995/06/27  13:32 UTC  truitt
# 	CHFts15407: Add tests for new hostdata behavior.
# 	[1995/12/11  15:53:25  root]
# 
# Revision 1.1.2.1  1994/09/28  21:40:14  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:28  melman]
# 
# Revision 1.1.2.4  1994/06/22  20:42:46  baum
# 	Merged with changes from 1.1.2.3
# 	[1994/06/22  20:42:31  baum]
# 
# 	OT #11022 - Use tet_setup, use tetexec.cfg for cell/host
# 	[1994/06/22  20:40:18  baum]
# 
# Revision 1.1.2.3  1994/06/17  18:43:20  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:26:05  devsrc]
# 
# Revision 1.1.2.2  1994/06/07  23:13:47  baum
# 	Mods to handle new dcecp output (CR #10761)
# 	[1994/06/07  23:13:17  baum]
# 
# Revision 1.1.2.1  1994/05/19  22:52:00  baum
# 	Initial checkin
# 	[1994/05/19  22:51:25  baum]
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
proc hostdata_show_5_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_vars.tcl

    # Identify the test
    tet_infoline hostdata_show_5_P: Check that we can show hostdata entry.

    # Setup steps
    tet_setup

    # Run the test
    set cmd "hostdata show $HD1_LONG_NAME -entry"
    set ret [catch "$cmd" msg]
    set expected_output [list $HD1_UUID $HD1_STORAGE $HD1_ANNOTATION]
    compare_list_and_report $cmd $ret $msg $expected_output

    # Cleanup steps
    tet_cleanup
}

proc hostdata_show_6_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_vars.tcl

    # Identify the test
    tet_infoline hostdata_show_6_P: Check that we can show hostdata objects.

    # Setup steps
    tet_setup

    # Run the test
    set cmd "hostdata show $HD1_LONG_NAME"
    set ret [catch "$cmd" msg]
 
    set expected_output [list $HD1_UUID $HD1_STORAGE $HD1_ANNOTATION $LINE1 $LINE2]
    compare_list_and_report $cmd $ret $msg $expected_output

    # Cleanup steps
    tet_cleanup
}

proc hostdata_show_7_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_vars.tcl

    # Identify the test
    tet_infoline hostdata_show_7_P: Check that we can show hostdata objects using binary option.

    # Setup steps
    tet_setup

    # Run the test
    set cmd "hostdata show $HD1_LONG_NAME -binary"
    set ret [catch "$cmd" msg]
 
    set expected_output [list $HD1_UUID $HD1_STORAGE $HD1_ANNOTATION]
    compare_list_and_report $cmd $ret $msg $expected_output

    # Cleanup steps
    tet_cleanup
}

proc hostdata_show_8_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW DCP_RGY_SA_PROTSEQ_MASTER DCP_RGY_SA_NETADDR_MASTER

    source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_vars.tcl

    # Identify the test
    tet_infoline hostdata_show_8_P: Check that we can show hostdata objects using string binding.

    # Setup steps
    tet_setup

    # Run the test
    set cmd "hostdata show $DCP_RGY_SA_PROTSEQ_MASTER:$DCP_RGY_SA_NETADDR_MASTER -ifname $HD1_SHORT_NAME"
    set ret [catch "$cmd" msg]
 
    set expected_output [list $HD1_UUID $HD1_STORAGE $HD1_ANNOTATION $LINE1 $LINE2]
    compare_list_and_report $cmd $ret $msg $expected_output

    # Cleanup steps
    tet_cleanup
}

proc hostdata_show_9_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW DCP_RGY_SA_PROTSEQ_MASTER DCP_RGY_SA_NETADDR_MASTER

    source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_vars.tcl

    # Identify the test
    tet_infoline hostdata_show_9_P: Check that we can show hostdata objects using string binding.

    # Setup steps
    tet_setup

    # Run the test
    set cmd "hostdata show $DCP_RGY_SA_PROTSEQ_MASTER:$DCP_RGY_SA_NETADDR_MASTER -ifname $HD1_SHORT_NAME -entry"
    set ret [catch "$cmd" msg]
 
    set expected_output [list $HD1_UUID $HD1_STORAGE $HD1_ANNOTATION]
    compare_list_and_report $cmd $ret $msg $expected_output

    # Cleanup steps
    tet_cleanup
}

proc hostdata_show_10_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW DCP_RGY_SA_PROTSEQ_MASTER DCP_RGY_SA_NETADDR_MASTER

    source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_vars.tcl

    # Identify the test
    tet_infoline hostdata_show_10_P: Check that we can show hostdata objects using string binding.

    # Setup steps
    tet_setup

    # Run the test
    set cmd "hostdata show {$DCP_RGY_SA_PROTSEQ_MASTER $DCP_RGY_SA_NETADDR_MASTER} -ifname $HD1_SHORT_NAME -entry"
    set ret [catch "$cmd" msg]
 
    set expected_output [list $HD1_UUID $HD1_STORAGE $HD1_ANNOTATION]
    compare_list_and_report $cmd $ret $msg $expected_output

    # Cleanup steps
    tet_cleanup
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 "
set ic1 "hostdata_show_5_P"
set ic2 "hostdata_show_6_P"
set ic3 "hostdata_show_7_P"
set ic4 "hostdata_show_8_P"
set ic5 "hostdata_show_9_P"
set ic6 "hostdata_show_10_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
