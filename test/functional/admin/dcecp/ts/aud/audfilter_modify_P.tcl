#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: audfilter_modify_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:11:18  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:20  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:46  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/11/10  17:33 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/10  17:32 UTC  truitt
# 	Fix the expected output for the maximum complexity test.
# 	[1995/12/11  15:51:53  root]
# 
# Revision 1.1.2.2  1994/10/01  01:33:25  rousseau
# 	Updated for auditd changes.
# 	[1994/10/01  01:24:55  rousseau]
# 
# Revision 1.1.2.1  1994/09/28  21:38:17  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:56  melman]
# 
# Revision 1.1.2.6  1994/09/02  20:28:40  rousseau
# 	Mass test cleanup.
# 	[1994/09/02  20:27:57  rousseau]
# 
# Revision 1.1.2.5  1994/06/10  20:03:22  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:59  devsrc]
# 
# Revision 1.1.2.4  1994/05/23  18:09:26  rousseau
# 	Various updates.
# 	[1994/05/23  18:09:01  rousseau]
# 
# Revision 1.1.2.3  1994/05/11  19:35:09  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:17:04  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:27:08  rousseau
# 	Performance changes, and new API path (CR 10558).
# 	[1994/05/06  15:25:02  rousseau]
# 
# Revision 1.1.2.1  1994/04/21  20:28:56  rousseau
# 	Initial drop of audit tests (CR 9938).
# 	[1994/04/21  20:27:34  rousseau]
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
proc audfilter_modify_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc audfilter_modify_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}


proc audfilter_modify_13_P {} {
    # Identify the test
    tet_infoline audfilter_modify_13_P: Check to see if we can add a guide to a filter.

    # Setup steps
    if {[dcp_wrap {audfilter create world -att {dce_dts_mgt_query denial alarm}}] != 0} {return;}

    # Run the test
    test {
	audfilter modify world -add {dce_dts_synch success log}
	audfilter show world
    } compare_lists {{dce_dts_mgt_query denial alarm} {dce_dts_synch success log}}

    # Cleanup steps
    if {[dcp_wrap {audfilter delete [audfilter cat]}] != 0} {return;}
}

proc audfilter_modify_22_P {} {
    # Identify the test
    tet_infoline audfilter_modify_22_P: Check to see if we can remove a guide from a filter.

    # Setup steps
    if {[dcp_wrap {audfilter create world -att {{dce_dts_mgt_query denial alarm} {dce_dts_synch success log}}}] != 0} {return;}

    # Run the test
    test {
	audfilter modify world -remove {dce_dts_synch success log}
	audfilter show world
    } compare_strings {{dce_dts_mgt_query denial alarm}}

    # Cleanup steps
    if {[dcp_wrap {audfilter delete [audfilter cat]}] != 0} {return;}
}

proc audfilter_modify_23_P {} {
    # Identify the test
    tet_infoline audfilter_modify_23_P: Check to see if we can add and remove guides from a filter.

    # Setup steps
    if {[dcp_wrap {principal create audfilter_modify_13_P
                   audfilter create {principal audfilter_modify_13_P} -attribute {dce_dts_mgt_query denial alarm}}] != 0} {return;}

    # Run the test
    test {
	audfilter modify {principal audfilter_modify_13_P} -add {{dce_audit_filter_modify dce_dts_time_provider} {success failure} all} -remove {dce_dts_mgt_query denial alarm}
	audfilter show {principal audfilter_modify_13_P}
	} compare_lists {{{dce_audit_filter_modify dce_dts_time_provider} {success failure} all}}

    # Cleanup steps
    if {[dcp_wrap {audfilter delete {principal audfilter_modify_13_P}
                   principal delete audfilter_modify_13_P}] != 0} {return;}
}

proc audfilter_modify_24_P {} {
    # Identify the test
    tet_infoline audfilter_modify_24_P: Check to see if we can add and remove a list of guides from a list 
    tet_infoline audfilter_modify_24_P: of filters. Maximum complexity test.

    # Setup steps
    if {[dcp_wrap {principal create audfilter_modify_24_P_1 
                   principal create audfilter_modify_24_P_2
                   audfilter create {principal audfilter_modify_24_P_1} -a {dce_dts_time_provider success log}
                   audfilter create {principal audfilter_modify_24_P_2} -a {{dce_dts_time_provider {denial failure} {log alarm}} {dce_audit_filter_query success alarm}}
                   audfilter create world_overridable -a {dce_dts_time_provider failure all}
                   audfilter create world -a {dce_dts_mgt_query failure all}}] != 0} {return;}

    # Run the test
    test {
	audfilter modify {{principal audfilter_modify_24_P_1} {principal audfilter_modify_24_P_2} {world_overridable}} -add {{dce_audit_filter_query {success denial failure} alarm} {{dce_dts_mgt_modify dce_dts_time_provider dce_audit_filter_query} denial log}} -remove {dce_dts_time_provider denial log}
        lsort [audfilter show [audfilter catalog]]
    } compare_lists [lsort {{dce_audit_filter_query {success failure} alarm} {{dce_dts_mgt_modify dce_audit_filter_query} denial log} {dce_dts_time_provider success log} {dce_dts_time_provider failure all} {dce_dts_time_provider denial alarm} {dce_audit_filter_query {success failure} alarm} {{dce_dts_mgt_modify dce_audit_filter_query} denial log} {dce_dts_mgt_query failure all} {dce_dts_time_provider failure all} {dce_audit_filter_query {success failure} alarm} {{dce_dts_mgt_modify dce_audit_filter_query} denial log}}]

    # Cleanup steps
    if {[dcp_wrap {audfilter delete [audfilter catalog]
                   principal delete audfilter_modify_24_P_1
                   principal delete audfilter_modify_24_P_2}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "audfilter_modify_13_P"
set ic2 "audfilter_modify_22_P"
set ic3 "audfilter_modify_23_P"
set ic4 "audfilter_modify_24_P"
set tet_startup audfilter_modify_startup
set tet_cleanup audfilter_modify_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl

