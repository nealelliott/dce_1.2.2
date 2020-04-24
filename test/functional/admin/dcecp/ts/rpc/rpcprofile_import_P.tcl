#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: rpcprofile_import_P.tcl,v $
# Revision 1.1.2.2  1996/03/11  02:26:10  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:46  marty]
#
# Revision 1.1.2.1  1995/12/11  15:15:04  root
# 	Submit
# 	[1995/12/11  14:31:53  root]
# 
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/06/30  14:06 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts37/1  1995/06/30  14:05 UTC  truitt
# 	CHFts15604: Update tests to reflect removal of -version option
# 	from import action.
# 
# 	HP revision /main/HPDCE02/1  1995/06/12  12:47 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts34/1  1995/06/12  12:46 UTC  truitt
# 	New test.
# 	[1995/12/08  23:16:43  root]
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
proc rpcprofile_import_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc rpcprofile_import_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc rpcprofile_import_1_P {} {
    global RPCENTRY1 INTERFACE1 BINDING1

    # Identify the test
    tet_infoline rpcprofile_import_1_P: Verify we can import an RPC profile with -interface.

    # Setup steps
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -interface [list $INTERFACE1] -binding [list $BINDING1]"] != 0} {return;}
    if {[dcp_wrap {rpcprofile create /.:/rpcprofile_import1}] != 0} {return;}
    if {[dcp_wrap "rpcprofile add /.:/rpcprofile_import1 -member $RPCENTRY1 -default"] != 0} {return;}

    # Run the test
    test "rpcprofile import /.:/rpcprofile_import1 -interface [list $INTERFACE1]
    " compare_lists [format {{%s}} $BINDING1]

    # Cleanup steps
    if {[dcp_wrap "rpcentry delete $RPCENTRY1"] != 0} {return;}
    if {[dcp_wrap {object delete /.:/rpcprofile_import1}] != 0} {return;}
}

proc rpcprofile_import_2_P {} {
    global RPCENTRY1 INTERFACE1 INTERFACEV BINDING1 BINDING2

    # Identify the test
    tet_infoline rpcprofile_import_2_P: Verify we can import an RPC profile with -interface

    # Setup steps
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -interface [list $INTERFACE1] -binding [list $BINDING1]"] != 0} {return;}
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -interface [list $INTERFACEV] -binding [list $BINDING2]"] != 0} {return;}
    if {[dcp_wrap {rpcprofile create /.:/rpcprofile_import2}] != 0} {return;}
    if {[dcp_wrap "rpcprofile add /.:/rpcprofile_import2 -member $RPCENTRY1 -default"] != 0} {return;}

    # Run the test
    test "rpcprofile import /.:/rpcprofile_import2 -interface [list $INTERFACEV]
    " compare_lists [format {{%s}} $BINDING2]

    # Cleanup steps
    if {[dcp_wrap "rpcentry delete $RPCENTRY1"] != 0} {return;}
    if {[dcp_wrap {object delete /.:/rpcprofile_import2}] != 0} {return;}
}

proc rpcprofile_import_3_P {} {
    global RPCENTRY1 INTERFACE1 BINDING1 OBJECT1

    # Identify the test
    tet_infoline rpcprofile_import_3_P: Verify we can import an RPC profile with -interface and -object.

    # Setup steps
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -interface [list $INTERFACE1] -binding [list $BINDING1] -object $OBJECT1"] != 0} {return;}
    if {[dcp_wrap {rpcprofile create /.:/rpcprofile_import3}] != 0} {return;}
    if {[dcp_wrap "rpcprofile add /.:/rpcprofile_import3 -member $RPCENTRY1 -default"] != 0} {return;}
 
    # Run the test
    test "rpcprofile import /.:/rpcprofile_import3 -interface [list $INTERFACE1] -object $OBJECT1
    " compare_strings [format {{%s}} $BINDING1]

    # Cleanup steps
    if {[dcp_wrap "rpcentry delete $RPCENTRY1"] != 0} {return;}
    if {[dcp_wrap {object delete /.:/rpcprofile_import3}] != 0} {return;}
}

proc rpcprofile_import_4_P {} {
    global RPCENTRY1 INTERFACE1 BINDING1 OBJECT1

    # Identify the test
    tet_infoline rpcprofile_import_4_P: Verify we can import an RPC profile with all options.

    # Setup steps
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -interface [list $INTERFACE1] -binding [list $BINDING1] -object $OBJECT1"] != 0} {return;}
    if {[dcp_wrap {rpcprofile create /.:/rpcprofile_import4}] != 0} {return;}
    if {[dcp_wrap "rpcprofile add /.:/rpcprofile_import4 -member $RPCENTRY1 -default"] != 0} {return;}

    # Run the test
    test "rpcprofile import /.:/rpcprofile_import4 -interface [list $INTERFACE1] -object $OBJECT1
    " compare_strings [format {{%s}} $BINDING1]

    # Cleanup steps
    if {[dcp_wrap "rpcentry delete $RPCENTRY1"] != 0} {return;}
    if {[dcp_wrap {object delete /.:/rpcprofile_import4}] != 0} {return;}
}

proc rpcprofile_import_5_P {} {
    global RPCENTRY1 INTERFACE1 BINDING1

    # Identify the test
    tet_infoline rpcprofile_import_5_P: Verify we can import an RPC profile with -noupdate.

    # Setup steps
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -interface [list $INTERFACE1] -binding [list $BINDING1]"] != 0} {return;}
    if {[dcp_wrap {rpcprofile create /.:/rpcprofile_import5}] != 0} {return;}
    if {[dcp_wrap "rpcprofile add /.:/rpcprofile_import5 -member $RPCENTRY1 -default"] != 0} {return;}

    # Run the test
    test "rpcprofile import /.:/rpcprofile_import5 -interface [list $INTERFACE1] -noupdate
    " compare_strings [format {{%s}} $BINDING1]

    # Cleanup steps
    if {[dcp_wrap "rpcentry delete $RPCENTRY1"] != 0} {return;}
    if {[dcp_wrap {object delete /.:/rpcprofile_import5}] != 0} {return;}
}

proc rpcprofile_import_6_P {} {
    global RPCENTRY1 INTERFACE1 BINDING1 BINDING2

    # Identify the test
    tet_infoline rpcprofile_import_6_P: Verify we can import an RPC profile specifying the maximum number of bindings with -max.

    # Setup steps
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -interface [list $INTERFACE1] -binding [list $BINDING1]"] != 0} {return;}
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -interface [list $INTERFACE1] -binding [list $BINDING2]"] != 0} {return;}
    if {[dcp_wrap {rpcprofile create /.:/rpcprofile_import6}] != 0} {return;}
    if {[dcp_wrap "rpcprofile add /.:/rpcprofile_import6 -member $RPCENTRY1 -default "] != 0} {return;}

    # Run the test
    test {llength [rpcprofile import /.:/rpcprofile_import6 -interface [join $INTERFACE1 " "] -max 1]
    } compare_strings {1}

    # Cleanup steps
    if {[dcp_wrap "rpcentry delete $RPCENTRY1"] != 0} {return;}
    if {[dcp_wrap {object delete /.:/rpcprofile_import6}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 "
set ic1 "rpcprofile_import_1_P"
set ic2 "rpcprofile_import_2_P"
set ic3 "rpcprofile_import_3_P"
set ic4 "rpcprofile_import_4_P"
set ic5 "rpcprofile_import_5_P"
set ic6 "rpcprofile_import_6_P"
set tet_startup rpcprofile_import_startup
set tet_cleanup rpcprofile_import_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
