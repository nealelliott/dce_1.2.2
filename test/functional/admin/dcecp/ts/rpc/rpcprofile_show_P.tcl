#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: rpcprofile_show_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:26:24  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:51  marty]
#
# Revision 1.1.4.1  1995/12/11  15:15:15  root
# 	Submit
# 	[1995/12/11  14:31:58  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:50  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:46  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:54  melman]
# 
# Revision 1.1.2.4  1994/09/19  19:55:41  rousseau
# 	Fix test variable usage.
# 	[1994/09/19  19:55:09  rousseau]
# 
# Revision 1.1.2.3  1994/09/19  17:57:29  rousseau
# 	Fix hardcoded cell name (CR 12242).
# 	[1994/09/19  17:55:37  rousseau]
# 
# Revision 1.1.2.2  1994/06/17  18:44:03  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:29:36  devsrc]
# 
# Revision 1.1.2.1  1994/05/26  18:42:20  rousseau
# 	Initial submission.
# 	[1994/05/26  18:39:31  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc rpcprofile_show_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc rpcprofile_show_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc rpcprofile_show_6_P {} {
    global DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline rpcprofile_show_6_P: Verify that we can show an rpc profile.

    # Setup steps
    if {[dcp_wrap {rpcprofile add /.:/rpcp_show_6_P -interface {004dbc1e-5c69-1de3-9add-0000c03adf56 1.0} -member /.:/foo -priority 1}] != 0} {return;}

    # Run the test
    test {
	rpcprofile show /.:/rpcp_show_6_P
    } compare_lists "{{004dbc1e-5c69-1de3-9add-0000c03adf56 1.0} $DCP_CELLNAME_ONE/foo 1 }"

    # Cleanup steps
    if {[dcp_wrap {object delete /.:/rpcp_show_6_P}] != 0} {return;}
}

proc rpcprofile_show_7_P {} {
    global DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline rpcprofile_show_7_P: Verify that we can show a specific profile entry with -annotation.

    # Setup steps
    if {[dcp_wrap {rpcprofile add /.:/rpcp_show_7_P -member /.:/show7 -interface {004dbc1e-5c69-1de3-9add-0000c03adf56 1.0} -annotation foo}] != 0} {return;}
    if {[dcp_wrap {rpcprofile add /.:/rpcp_show_7_P -member /.:/show7 -interface {004dbc1e-5c69-1de3-9add-0000c03adf56 2.0} -annotation bar}] != 0} {return;}

    # Run the test
    test {
	rpcprofile show /.:/rpcp_show_7_P -annotation foo
    } compare_lists "{{004dbc1e-5c69-1de3-9add-0000c03adf56 1.0} $DCP_CELLNAME_ONE/show7 0 foo}"

    # Cleanup steps
    if {[dcp_wrap {object delete /.:/rpcp_show_7_P}] != 0} {return;}
}

proc rpcprofile_show_8_P {} {
    global DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline rpcprofile_show_8_P: Verify that we can show a specific profile entry with -member.

    # Setup steps
    if {[dcp_wrap {rpcprofile add /.:/rpcp_show_8_P -member /.:/foo -interface {004dbc1e-5c69-1de3-9add-0000c03adf56 1.0}}] != 0} {return;}
    if {[dcp_wrap {rpcprofile add /.:/rpcp_show_8_P -member /.:/bar -interface {004dbc1e-5c69-1de3-9add-0000c03adf56 2.0}}] != 0} {return;}

    # Run the test
    test {
	rpcprofile show /.:/rpcp_show_8_P -member /.:/foo
    } compare_lists "{{004dbc1e-5c69-1de3-9add-0000c03adf56 1.0} $DCP_CELLNAME_ONE/foo 0 }"

    # Cleanup steps
    if {[dcp_wrap {object delete /.:/rpcp_show_8_P}] != 0} {return;}
}

proc rpcprofile_show_9_P {} {
    global DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline rpcprofile_show_9_P: Verify that we can show a specific profile entry with -interface.

    # Setup steps
    if {[dcp_wrap {rpcprofile add /.:/rpcp_show_9_P -member /.:/foo -interface {004dbc1e-5c69-1de3-9add-0000c03adf56 1.0}}] != 0} {return;}
    if {[dcp_wrap {rpcprofile add /.:/rpcp_show_9_P -member /.:/bar -interface {00220434-6417-1de3-9add-0000c03adf56 1.0}}] != 0} {return;}

    # Run the test
    test {
	rpcprofile show /.:/rpcp_show_9_P -interface {00220434-6417-1de3-9add-0000c03adf56 1.0}
    } compare_lists "{{00220434-6417-1de3-9add-0000c03adf56 1.0} $DCP_CELLNAME_ONE/bar 0 }"

    # Cleanup steps
    if {[dcp_wrap {object delete /.:/rpcp_show_9_P}] != 0} {return;}
}

proc rpcprofile_show_10_P {} {
    global DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline rpcprofile_show_10_P: Verify that we can show a specific profile entry with -interface
    tet_infoline rpcprofile_show_10_P: and -version.

    # Setup steps
    if {[dcp_wrap {rpcprofile add /.:/rpcp_show_10_P -member /.:/foo -interface {004dbc1e-5c69-1de3-9add-0000c03adf56 1.0}}] != 0} {return;}
    if {[dcp_wrap {rpcprofile add /.:/rpcp_show_10_P -member /.:/bar -interface {004dbc1e-5c69-1de3-9add-0000c03adf56 2.0}}] != 0} {return;}

    # Run the test
    test {
	rpcprofile show /.:/rpcp_show_10_P -interface {004dbc1e-5c69-1de3-9add-0000c03adf56 1.0} -version exact
    } compare_lists "{{004dbc1e-5c69-1de3-9add-0000c03adf56 1.0} $DCP_CELLNAME_ONE/foo 0 }"

    # Cleanup steps
    if {[dcp_wrap {object delete /.:/rpcp_show_10_P}] != 0} {return;}
}

proc rpcprofile_show_11_P {} {
    global DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline rpcprofile_show_11_P: Verify that we can show a specific profile entry with -interface
    tet_infoline rpcprofile_show_11_P: and -version.

    # Setup steps
    if {[dcp_wrap {rpcprofile add /.:/rpcp_show_11_P_1 -member /.:/foo -interface {004dbc1e-5c69-1de3-9add-0000c03adf56 1.0}}] != 0} {return;}
    if {[dcp_wrap {rpcprofile add /.:/rpcp_show_11_P_2 -member /.:/bar -interface {004dbc1e-5c69-1de3-9add-0000c03adf56 2.0}}] != 0} {return;}

    # Run the test
    test {
	rpcprofile show {/.:/rpcp_show_11_P_1 /.:/rpcp_show_11_P_2}
    } compare_lists "{{004dbc1e-5c69-1de3-9add-0000c03adf56 1.0} $DCP_CELLNAME_ONE/foo 0 } {{004dbc1e-5c69-1de3-9add-0000c03adf56 2.0} $DCP_CELLNAME_ONE/bar 0 }"

    # Cleanup steps
    if {[dcp_wrap {object delete /.:/rpcp_show_11_P_1}] != 0} {return;}
    if {[dcp_wrap {object delete /.:/rpcp_show_11_P_2}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 "
set ic1 "rpcprofile_show_6_P"
set ic2 "rpcprofile_show_7_P"
set ic3 "rpcprofile_show_8_P"
set ic4 "rpcprofile_show_9_P"
set ic5 "rpcprofile_show_10_P"
set ic6 "rpcprofile_show_11_P"
set tet_startup rpcprofile_show_startup
set tet_cleanup rpcprofile_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
