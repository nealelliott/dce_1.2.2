#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: rpcentry_import_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:25:07  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:24  marty]
#
# Revision 1.1.4.1  1995/12/11  15:14:14  root
# 	Submit
# 	[1995/12/11  14:31:34  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:17  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:03  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:24  melman]
# 
# Revision 1.1.2.4  1994/08/18  21:16:24  coren
# 	Fixed to pass correct TCL-syntax interfaces and bindings (CR 11733).
# 	[1994/08/18  20:01:40  coren]
# 
# Revision 1.1.2.3  1994/06/17  18:43:41  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:27:49  devsrc]
# 
# Revision 1.1.2.2  1994/05/26  20:21:05  coren
# 	Positive tests filled in.
# 	[1994/05/26  19:11:59  coren]
# 
# Revision 1.1.2.1  1994/05/19  20:02:36  melman
# 	initial rpccp tests
# 	[1994/05/19  19:59:45  melman]
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
proc rpcentry_import_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc rpcentry_import_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}    

proc rpcentry_import_6_P {} {
    # Identify the test
    tet_infoline rpcentry_import_6_P: Verify that we can import an RPC entry with -interface.

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 INTERFACE1 BINDING1}] != 0} {return;}
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -interface [list $INTERFACE1] -binding [list $BINDING1]"] != 0} {return;}

    # Run the test
    test "rpcentry import $RPCENTRY1 -interface [list $INTERFACE1]
    " compare_lists [format {{%s}} $BINDING1]

    # Cleanup steps
    if {[dcp_wrap "rpcentry delete $RPCENTRY1"] != 0} {return;}
}

proc rpcentry_import_7_P {} {
    # Identify the test
    tet_infoline rpcentry_import_7_P: Verify that we can import an RPC entry with -interface

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 INTERFACE1 INTERFACEV BINDING1 BINDING2}] != 0} {return;}
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -interface [list $INTERFACE1] -binding [list $BINDING1]"] != 0} {return;}
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -interface [list $INTERFACEV] -binding [list $BINDING2]"] != 0} {return;}

    # Run the test
    test "rpcentry import $RPCENTRY1 -interface [list $INTERFACEV]
    " compare_lists [format {{%s}} $BINDING2]

    # Cleanup steps
    if {[dcp_wrap "rpcentry delete $RPCENTRY1"] != 0} {return;}
}

proc rpcentry_import_8_P {} {
    # Identify the test
    tet_infoline rpcentry_import_8_P: Verify that we can import an RPC entry with -interface and -object.

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 INTERFACE1 BINDING1 OBJECT1}] != 0} {return;}
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -interface [list $INTERFACE1] -binding [list $BINDING1] -object $OBJECT1"] != 0} {return;}
 
    # Run the test
    test "rpcentry import $RPCENTRY1 -interface [list $INTERFACE1] -object $OBJECT1
    " compare_strings [format {{%s}} $BINDING1]

    # Cleanup steps
    if {[dcp_wrap "rpcentry delete $RPCENTRY1"] != 0} {return;}
}

proc rpcentry_import_9_P {} {
    # Identify the test
    tet_infoline rpcentry_import_9_P: Verify that we can import an RPC entry with all options.

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 INTERFACE1 BINDING1 OBJECT1}] != 0} {return;}
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -interface [list $INTERFACE1] -binding [list $BINDING1] -object $OBJECT1"] != 0} {return;}

    # Run the test
    test "rpcentry import $RPCENTRY1 -interface [list $INTERFACE1] -object $OBJECT1
    " compare_strings [format {{%s}} $BINDING1]

    # Cleanup steps
    if {[dcp_wrap "rpcentry delete $RPCENTRY1"] != 0} {return;}
}

proc rpcentry_import_10_P {} {
    # Identify the test
    tet_infoline rpcentry_import_10_P: Verify that we can import a list of RPC entries with -interface.

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 RPCENTRY2 RPCENTRY3 INTERFACE1 BINDING1}] != 0} {return;}
    if {[dcp_wrap {set entrylist [list "$RPCENTRY1 $RPCENTRY2 $RPCENTRY3"]}] != 0} {return;}
    if {[dcp_wrap "rpcentry export $entrylist -interface [list $INTERFACE1] -binding [list $BINDING1]"] != 0} {return;}

    # Run the test
    test "rpcentry import $entrylist -interface [list $INTERFACE1]
    " compare_lists [format "{%s} {%s} {%s}" $BINDING1 $BINDING1 $BINDING1]

    # Cleanup steps
    if {[dcp_wrap "rpcentry delete $entrylist"] != 0} {return;}
}

proc rpcentry_import_11_P {} {
    # Identify the test
    tet_infoline rpcentry_import_11_P: Verify that we can import an RPC entry with -noupdate.

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 INTERFACE1 BINDING1}] != 0} {return;}
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -interface [list $INTERFACE1] -binding [list $BINDING1]"] != 0} {return;}

    # Run the test
    test "rpcentry import $RPCENTRY1 -interface [list $INTERFACE1] -noupdate
    " compare_strings [format {{%s}} $BINDING1]

    # Cleanup steps
    if {[dcp_wrap "rpcentry delete $RPCENTRY1"] != 0} {return;}
}

proc rpcentry_import_12_P {} {
    # Identify the test
    tet_infoline rpcentry_import_12_P: Verify that we can import an RPC entry specifying the maximum number of bindings with -max.

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 INTERFACE1 BINDING1 BINDING2}] != 0} {return;}
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -interface [list $INTERFACE1] -binding [list $BINDING1]"] != 0} {return;}
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -interface [list $INTERFACE1] -binding [list $BINDING2]"] != 0} {return;}

    # Run the test
    test {llength [rpcentry import $RPCENTRY1 -interface [join $INTERFACE1 " "] -max 1]
    } compare_strings {1}

    # Cleanup steps
    if {[dcp_wrap "rpcentry delete $RPCENTRY1"] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 "
set ic1 "rpcentry_import_6_P"
set ic2 "rpcentry_import_7_P"
set ic3 "rpcentry_import_8_P"
set ic4 "rpcentry_import_9_P"
set ic5 "rpcentry_import_10_P"
set ic6 "rpcentry_import_11_P"
set ic7 "rpcentry_import_12_P"
set tet_startup rpcentry_import_startup
set tet_cleanup rpcentry_import_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
