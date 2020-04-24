#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: rpcprofile_remove_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:26:19  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:49  marty]
#
# Revision 1.1.4.1  1995/12/11  15:15:12  root
# 	Submit
# 	[1995/12/11  14:31:56  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:47  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:44  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:52  melman]
# 
# Revision 1.1.2.4  1994/09/19  19:55:40  rousseau
# 	Fix test variable usage.
# 	[1994/09/19  19:55:07  rousseau]
# 
# Revision 1.1.2.3  1994/09/19  17:57:28  rousseau
# 	Fix hardcoded cell name (CR 12242).
# 	[1994/09/19  17:55:36  rousseau]
# 
# Revision 1.1.2.2  1994/06/17  18:44:01  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:29:28  devsrc]
# 
# Revision 1.1.2.1  1994/05/26  18:42:17  rousseau
# 	Initial submission.
# 	[1994/05/26  18:39:30  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc rpcprofile_remove_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc rpcprofile_remove_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc rpcprofile_remove_10_P {} {
    # Identify the test
    tet_infoline rpcprofile_remove_10_P: Verify that we can remove a member from a profile.

    # Setup steps
    if {[dcp_wrap {rpcprofile add /.:/rpcp_remove_10_P -interface {004dbc1e-5c69-1de3-9add-0000c03adf56 1.0} -member /.:/foo}] != 0} {return;}

    # Run the test
    test {
	rpcprofile remove /.:/rpcp_remove_10_P -member /.:/foo -interface {004dbc1e-5c69-1de3-9add-0000c03adf56 1.0}
	rpcprofile show /.:/rpcp_remove_10_P
    } compare_lists {}

    # Cleanup steps
    if {[dcp_wrap {object delete /.:/rpcp_remove_10_P}] != 0} {return;}
}

proc rpcprofile_remove_11_P {} {
    global DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline rpcprofile_remove_11_P: Verify that we can remove a member from a profile with -member and 
    tet_infoline rpcprofile_remove_11_P: -interface.

    # Setup steps
    if {[dcp_wrap {rpcprofile add /.:/rpcp_remove_11_P -interface {004dbc1e-5c69-1de3-9add-0000c03adf56 1.0} -member /.:/foo}] != 0} {return;}
    if {[dcp_wrap {rpcprofile add /.:/rpcp_remove_11_P -interface {004dbc1e-5c69-1de3-9add-0000c03adf56 2.0} -member /.:/bar}] != 0} {return;}

    # Run the test
    test {
	rpcprofile remove /.:/rpcp_remove_11_P -member /.:/foo -interface {004dbc1e-5c69-1de3-9add-0000c03adf56 1.0}
	rpcprofile show /.:/rpcp_remove_11_P
    } compare_lists "{{004dbc1e-5c69-1de3-9add-0000c03adf56 2.0} $DCP_CELLNAME_ONE/bar 0 }"

    # Cleanup steps
    if {[dcp_wrap {object delete /.:/rpcp_remove_11_P}] != 0} {return;}
}

proc rpcprofile_remove_12_P {} {
    global DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline rpcprofile_remove_12_P: Verify that we can remove a member from a profile with -priority.

    # Setup steps
    if {[dcp_wrap {rpcprofile add /.:/rpcp_remove_12_P -interface {004dbc1e-5c69-1de3-9add-0000c03adf56 1.0} -member /.:/foo -priority 1}] != 0} {return;}
    if {[dcp_wrap {rpcprofile add /.:/rpcp_remove_12_P -interface {004dbc1e-5c69-1de3-9add-0000c03adf56 2.0} -member /.:/bar -priority 2}] != 0} {return;}

    # Run the test
    test {
	rpcprofile remove /.:/rpcp_remove_12_P -priority 2
	rpcprofile show /.:/rpcp_remove_12_P
    } compare_lists "{{004dbc1e-5c69-1de3-9add-0000c03adf56 1.0} $DCP_CELLNAME_ONE/foo 1 }"

    # Cleanup steps
    if {[dcp_wrap {object delete /.:/rpcp_remove_12_P}] != 0} {return;}
}

proc rpcprofile_remove_13_P {} {
    global DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline rpcprofile_remove_13_P: Verify that we can remove a member from a profile with -annotation.

    # Setup steps
    if {[dcp_wrap {rpcprofile add /.:/rpcp_remove_13_P -interface {004dbc1e-5c69-1de3-9add-0000c03adf56 1.0} -member /.:/foo -annotation {First case}}] != 0} {return;}
    if {[dcp_wrap {rpcprofile add /.:/rpcp_remove_13_P -interface {004dbc1e-5c69-1de3-9add-0000c03adf56 2.0} -member /.:/bar -annotation {Second case}}] != 0} {return;}

    # Run the test
    test {
	rpcprofile remove /.:/rpcp_remove_13_P -annotation {Second case}
	rpcprofile show /.:/rpcp_remove_13_P
    } compare_lists "{{004dbc1e-5c69-1de3-9add-0000c03adf56 1.0} $DCP_CELLNAME_ONE/foo 0 {First case}}"

    # Cleanup steps
    if {[dcp_wrap {object delete /.:/rpcp_remove_13_P}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "rpcprofile_remove_10_P"
set ic2 "rpcprofile_remove_11_P"
set ic3 "rpcprofile_remove_12_P"
set ic4 "rpcprofile_remove_13_P"
set tet_startup rpcprofile_remove_startup
set tet_cleanup rpcprofile_remove_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
