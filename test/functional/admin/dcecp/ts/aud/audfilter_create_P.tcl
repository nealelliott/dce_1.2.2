#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: audfilter_create_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:11:02  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:17  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:42  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:51:48  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:12  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:52  melman]
# 
# Revision 1.1.2.5  1994/06/10  20:03:17  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:55  devsrc]
# 
# Revision 1.1.2.4  1994/05/23  18:09:24  rousseau
# 	Various updates.
# 	[1994/05/23  18:08:58  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:27:01  rousseau
# 	Performance changes, and new API path (CR 10558).
# 	[1994/05/06  15:24:43  rousseau]
# 
# Revision 1.1.2.1  1994/04/21  20:28:52  rousseau
# 	Initial drop of audit tests (CR 9938).
# 	[1994/04/21  20:27:31  rousseau]
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
proc audfilter_create_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc audfilter_create_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}


proc audfilter_create_13_P {} {
    # Identify the test
    tet_infoline audfilter_create_13_P: Check that we can create a single filter with a single guide.

    # Setup steps
    # Cuz the previous test keeps failing and leaving trash around.
    dcp_hide {audfilter delete [audfilter cat]}

    # Run the test
    test {
	audfilter create world -attribute {dce_audit_filter_query success alarm}
	audfilter show world
    } compare_strings {{dce_audit_filter_query success alarm}}

    # Cleanup steps
    if {[dcp_wrap {audfilter delete world}] != 0} {return;}
}

proc audfilter_create_14_P {} {
    # Identify the test
    tet_infoline audfilter_create_14_P: Check that we can create a list of filters for a list of guides.

    # Setup steps
    if {[dcp_wrap {principal create audfilter_create_14_P
                   group create audfilter_create_14_P}] != 0} {return;}

    # Run the test
        test {
	    audfilter create {{principal audfilter_create_14_P} {group audfilter_create_14_P} world_overridable} -attr {{dce_dts_synch {success denial} all} {{dce_dts_mgt_query dce_dts_time_provider} failure {log alarm}}}
	    audfilter cat
	} compare_lists {{principal audfilter_create_14_P} {group audfilter_create_14_P} world_overridable}

    # Cleanup steps
    if {[dcp_wrap {audfilter delete [audfilter catalog]
                   principal delete audfilter_create_14_P
                   group delete audfilter_create_14_P}] != 0} {return;}
}


set iclist "ic1 ic2 "
set ic1 "audfilter_create_13_P"
set ic2 "audfilter_create_14_P"
set tet_startup audfilter_create_startup
set tet_cleanup audfilter_create_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl

