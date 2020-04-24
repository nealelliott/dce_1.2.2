#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: rpcprofile_add_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:25:54  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:41  marty]
#
# Revision 1.1.4.1  1995/12/11  15:14:52  root
# 	Submit
# 	[1995/12/11  14:31:48  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:37  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:34  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:43  melman]
# 
# Revision 1.1.2.4  1994/09/19  19:55:37  rousseau
# 	Fix test variable usage.
# 	[1994/09/19  19:55:02  rousseau]
# 
# Revision 1.1.2.3  1994/09/19  17:57:27  rousseau
# 	Fix hardcoded cell name (CR 12242).
# 	[1994/09/19  17:55:34  rousseau]
# 
# Revision 1.1.2.2  1994/06/17  18:43:54  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:28:55  devsrc]
# 
# Revision 1.1.2.1  1994/05/26  18:42:09  rousseau
# 	Initial submission.
# 	[1994/05/26  18:39:23  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc rpcprofile_add_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc rpcprofile_add_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc rpcprofile_add_10_P {} {
    global DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline rpcprofile_add_10_P: Verify that we can add a member to an rpc profile with -default.

    # Setup steps

    # Run the test
    test {
	rpcprofile add /.:/rpcp_add_10_P -member /.:/foo -default
	rpcprofile show /.:/rpcp_add_10_P
    } compare_strings "{{00000000-0000-0000-0000-000000000000 0.0} $DCP_CELLNAME_ONE/foo 0 }"

    # Cleanup steps
    if {[dcp_wrap {object delete /.:/rpcp_add_10_P}] != 0} {return;}
}

proc rpcprofile_add_11_P {} {
    global DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline rpcprofile_add_11_P: Verify that we can add a member to an rpc profile with -interface.

    # Setup steps

    # Run the test
    test {
	rpcprofile add /.:/rpcp_add_11_P -member /.:/foo -interface {00205383-5fa0-1de2-a683-0000c03adf56 1.0}
	rpcprofile show /.:/rpcp_add_11_P
    } compare_strings "{{00205383-5fa0-1de2-a683-0000c03adf56 1.0} $DCP_CELLNAME_ONE/foo 0 }"

    # Cleanup steps
    if {[dcp_wrap {object delete /.:/rpcp_add_11_P}] != 0} {return;}
}

proc rpcprofile_add_12_P {} {
    global DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline rpcprofile_add_12_P: Verify that we can add a member to an rpc profile with -interface,
    tet_infoline rpcprofile_add_12_P: -priority and -annotation.

    # Setup steps

    # Run the test
    test {
	rpcprofile add /.:/rpcp_add_12_P -member /.:/foo -interface {00205383-5fa0-1de2-a683-0000c03adf56 1.0} -priority 2 -annotation {Test entry}
	rpcprofile show /.:/rpcp_add_12_P
    } compare_strings "{{00205383-5fa0-1de2-a683-0000c03adf56 1.0} $DCP_CELLNAME_ONE/foo 2 {Test entry}}"

    # Cleanup steps
    if {[dcp_wrap {object delete /.:/rpcp_add_12_P}] != 0} {return;}
}

proc rpcprofile_add_13_P {} {
    global DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline rpcprofile_add_13_P: Verify that we can add a member to a list of rpc profiles with 
    tet_infoline rpcprofile_add_13_P: -interface, -priority and -annotation.

    # Setup steps

    # Run the test
    test {
	rpcprofile add {/.:/rpcp_add_13_P_1 /.:/rpcp_add_13_P_2} -member /.:/foo -interface {00205383-5fa0-1de2-a683-0000c03adf56 1.0} -priority 2 -annotation {Test entry}
	rpcprofile show {/.:/rpcp_add_13_P_1 /.:/rpcp_add_13_P_2}
    } compare_lists "{{00205383-5fa0-1de2-a683-0000c03adf56 1.0} $DCP_CELLNAME_ONE/foo 2 {Test entry}} {{00205383-5fa0-1de2-a683-0000c03adf56 1.0} $DCP_CELLNAME_ONE/foo 2 {Test entry}}"

    # Cleanup steps
    if {[dcp_wrap {object delete {/.:/rpcp_add_13_P_1 /.:/rpcp_add_13_P_2}}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "rpcprofile_add_10_P"
set ic2 "rpcprofile_add_11_P"
set ic3 "rpcprofile_add_12_P"
set ic4 "rpcprofile_add_13_P"
set tet_startup rpcprofile_add_startup
set tet_cleanup rpcprofile_add_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
