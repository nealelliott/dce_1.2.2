#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: audevents_catalog_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:10:36  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:12  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:36  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:51:39  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:06  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:45  melman]
# 
# Revision 1.1.2.5  1994/09/27  19:53:51  rousseau
# 	Fix list of event classes (CR 12353).
# 	[1994/09/27  19:53:43  rousseau]
# 
# Revision 1.1.2.4  1994/06/10  20:02:37  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:43  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:34:53  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:16:22  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:26:56  rousseau
# 	Performance changes, and new API path (CR 10558).
# 	[1994/05/06  15:04:01  rousseau]
# 
# Revision 1.1.2.1  1994/04/21  20:28:44  rousseau
# 	Initial drop of audit tests (CR 9938).
# 	[1994/04/21  20:27:22  rousseau]
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
proc audevents_catalog_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc audevents_catalog_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}    


proc audevents_catalog_3_P {} {
    # Identify the test
    tet_infoline audevents_catalog_3_P: Check that we can show a list of all the event classes.

    # Setup steps

    # Run the test
    test {
	audevents catalog
    } compare_lists {dce_audit_admin_modify dce_audit_admin_query dce_audit_filter_modify dce_audit_filter_query dce_dts_mgt_modify dce_dts_mgt_query dce_dts_synch dce_dts_time_provider dce_sec_authent dce_sec_control dce_sec_modify dce_sec_query dce_sec_server}


    # Cleanup steps
}


set iclist "ic1 "
set ic1 "audevents_catalog_3_P"
set tet_startup audevents_catalog_startup
set tet_cleanup audevents_catalog_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl

