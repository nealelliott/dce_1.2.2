#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: audfilter_show_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:11:30  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:21  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:48  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:51:56  root]
# 
# Revision 1.1.2.2  1994/10/01  01:33:27  rousseau
# 	Updated for auditd changes.
# 	[1994/10/01  01:24:56  rousseau]
# 
# Revision 1.1.2.1  1994/09/28  21:38:19  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:59  melman]
# 
# Revision 1.1.2.5  1994/09/02  20:28:41  rousseau
# 	Mass test cleanup.
# 	[1994/09/02  20:27:59  rousseau]
# 
# Revision 1.1.2.4  1994/06/10  20:03:24  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:01  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:35:10  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:17:12  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:27:12  rousseau
# 	Performance changes, and new API path (CR 10558).
# 	[1994/05/06  15:25:05  rousseau]
# 
# Revision 1.1.2.1  1994/04/21  20:29:00  rousseau
# 	Initial drop of audit tests (CR 9938).
# 	[1994/04/21  20:27:38  rousseau]
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
proc audfilter_show_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc audfilter_show_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}


proc audfilter_show_4_P {} {
    # Identify the test
    tet_infoline audfilter_show_4_P: Check that we can show a list of all the filter guides on a
    tet_infoline audfilter_show_4_P: filter.

    # Setup steps
    if {[dcp_wrap {group create audfilter_show_4_P}] != 0} {return;}
    if {[dcp_wrap {audfilter create {group audfilter_show_4_P} -a {dce_dts_synch success alarm}}] != 0} {return;}
    if {[dcp_wrap {audfilter mod {group audfilter_show_4_P} -add {{dce_dts_mgt_query dce_audit_filter_modify} {success denial failure} alarm}}] != 0} {return;}

    # Run the test
    test {
	audfilter show {group audfilter_show_4_P}
        } compare_lists {{{dce_dts_mgt_query dce_audit_filter_modify} {success failure denial} alarm} {dce_dts_synch success alarm}}

    # Cleanup steps
    if {[dcp_wrap {audfilter delete {group audfilter_show_4_P}}] != 0} {return;}
    if {[dcp_wrap {group delete audfilter_show_4_P}] != 0} {return;}
}

proc audfilter_show_5_P {} {
    # Identify the test
    tet_infoline audfilter_show_5_P: Check that we can show a list of all the filter guides on a
    tet_infoline audfilter_show_5_P: list of filters.

    # Setup steps
    if {[dcp_wrap {group create audfilter_show_5_P
                   catch {audfilter delete [audfilter catalog]}
                   audfilter create {group audfilter_show_5_P} -a {dce_dts_synch success alarm}
                   audfilter mod {group audfilter_show_5_P} -add {{dce_dts_mgt_query dce_audit_filter_modify} {success denial failure} alarm}
                   audfilter create world -a {dce_dts_mgt_query success all}}] != 0} {return;}

    # Run the test
    test {
	audfilter show [audfilter cat]
} compare_lists {{{dce_dts_mgt_query dce_audit_filter_modify} {success failure denial} alarm} {dce_dts_synch success alarm} {dce_dts_mgt_query success all}}

    # Cleanup steps
    if {[dcp_wrap {audfilter delete {{group audfilter_show_5_P} world}
                   group delete audfilter_show_5_P}] != 0} {return;}
}


set iclist "ic1 ic2 "
set ic1 "audfilter_show_4_P"
set ic2 "audfilter_show_5_P"
set tet_startup audfilter_show_startup
set tet_cleanup audfilter_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl

