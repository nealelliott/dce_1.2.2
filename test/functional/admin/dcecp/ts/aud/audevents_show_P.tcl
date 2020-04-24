#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: audevents_show_P.tcl,v $
# Revision 1.1.6.4  1996/03/11  02:10:45  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:13  marty]
#
# Revision 1.1.6.3  1995/12/13  15:24:38  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1994/12/20  19:57 UTC  truitt
# 	Merge.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chk_04/1  1994/12/20  19:56 UTC  truitt
# 	Change a test to match changes in audit functionality.
# 
# 	HP revision /main/HPDCE02/1  1994/11/10  17:18 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/10  17:12 UTC  truitt
# 	Fix the index numbers for the modify filters.
# 	[1995/12/11  15:51:42  root]
# 
# Revision 1.1.2.2  1994/10/01  01:33:24  rousseau
# 	Updated for auditd changes.
# 	[1994/10/01  01:24:54  rousseau]
# 
# Revision 1.1.2.1  1994/09/28  21:38:08  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:47  melman]
# 
# Revision 1.1.2.4  1994/06/10  20:02:41  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:51  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:34:56  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:16:32  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:26:57  rousseau
# 	Performance changes, and new API path (CR 10558).
# 	[1994/05/06  15:24:31  rousseau]
# 
# Revision 1.1.2.1  1994/04/21  20:28:46  rousseau
# 	Initial drop of audit tests (CR 9938).
# 	[1994/04/21  20:27:25  rousseau]
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
proc audevents_show_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc audevents_show_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}    


proc audevents_show_3_P {} {
    # Identify the test
    tet_infoline audevents_show_3_P: Check that we can show a single event class.

    # Setup steps

    # Run the test
    test {
	audevents show {dce_audit_filter_modify}
    } compare_strings {{dce_audit_filter_modify 0x300 0x303 0x304}}

    # Cleanup steps
}

proc audevents_show_4_P {} {
    # Identify the test
    tet_infoline audevents_show_4_P: Check that we can show a list of event classes.

    # Setup steps

    # Run the test
    test {
	audevents show [audevents cat]
    } compare_lists {{dce_audit_admin_modify 0x305 0x306 0x307 0x308} {dce_audit_admin_query 0x309 0x30a} {dce_audit_filter_modify 0x300 0x303 0x304} {dce_audit_filter_query 0x301 0x302} {dce_dts_mgt_modify 0x200 0x201 0x202 0x203 0x207 0x208 0x209 0x20a} {dce_dts_mgt_query 0x20b 0x20c 0x20d 0x20e 0x20f 0x210} {dce_dts_synch 0x204 0x205 0x206} {dce_dts_time_provider 0x211 0x212} {dce_sec_authent 0x101 0x102 0x103 0x104 0x10c 0x125 0x136 0x138 0x139} {dce_sec_control 0x105 0x106 0x107 0x108 0x109 0x10a 0x10b 0x10c 0x10d 0x10e 0x10f 0x110 0x111 0x112 0x113 0x114 0x115 0x116 0x117 0x118 0x119 0x11a 0x11b 0x11c 0x11d 0x11e 0x11f 0x120 0x121 0x122 0x123 0x124 0x125 0x126 0x127 0x128 0x129 0x12a 0x12b 0x12c 0x12d 0x12e 0x12f 0x130 0x131 0x132 0x133 0x134 0x135 0x136 0x138 0x139} {dce_sec_modify 0x106 0x10d 0x10e 0x10f 0x111 0x114 0x115 0x116 0x117 0x11a 0x11b 0x11f 0x121 0x123 0x12b 0x12c 0x131 0x132 0x133} {dce_sec_query 0x105 0x107 0x108 0x109 0x10a 0x10b 0x110 0x112 0x113 0x118 0x11c 0x11d 0x11e 0x120 0x122 0x12d 0x12e 0x12f 0x130 0x134 0x135} {dce_sec_server 0x124 0x125 0x126 0x127 0x128}}

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "audevents_show_3_P"
set ic2 "audevents_show_4_P"
set tet_startup audevents_show_startup
set tet_cleanup audevents_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
