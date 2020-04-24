#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: rpcentry_unexport_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:25:18  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:29  marty]
#
# Revision 1.1.4.1  1995/12/11  15:14:23  root
# 	Submit
# 	[1995/12/11  14:31:37  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:22  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:13  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:29  melman]
# 
# Revision 1.1.2.4  1994/08/18  21:16:27  coren
# 	Fixed to pass correct TCL-syntax interfaces and bindings (CR 11733).
# 	[1994/08/18  20:02:09  coren]
# 
# Revision 1.1.2.3  1994/06/17  18:43:44  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:28:05  devsrc]
# 
# Revision 1.1.2.2  1994/05/26  20:21:08  coren
# 	Positive tests filled in.
# 	[1994/05/26  19:12:05  coren]
# 
# Revision 1.1.2.1  1994/05/19  20:02:42  melman
# 	initial rpccp tests
# 	[1994/05/19  19:59:50  melman]
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
proc rpcentry_unexport_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc rpcentry_unexport_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}    

proc rpcentry_unexport_4_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_unexport_4_P: Verify that we can unexport an RPC entry with -interface.

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 INTERFACE1 BINDING1}] != 0} {return;}
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -interface [list $INTERFACE1] -binding [list $BINDING1]"] != 0} {return;}

    # Run the test
    test " rpcentry unexport $RPCENTRY1 -interface [list $INTERFACE1]
           rpcentry show $RPCENTRY1
         " compare_strings {}

    # Cleanup steps
    if {[dcp_wrap {rpcentry delete $RPCENTRY1}] != 0} {return;}
}

proc rpcentry_unexport_5_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_unexport_5_P: Verify that we can unexport an RPC entry with -object.

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 OBJECT1}] != 0} {return;}
    if {[dcp_wrap {rpcentry export $RPCENTRY1 -object $OBJECT1}] != 0} {return;}

    # Run the test
    test " rpcentry unexport $RPCENTRY1 -object $OBJECT1
           rpcentry show $RPCENTRY1
         " compare_strings {}

    # Cleanup steps
    if {[dcp_wrap {rpcentry delete $RPCENTRY1}] != 0} {return;}
}

proc rpcentry_unexport_6_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_unexport_6_P: Verify that we can unexport an RPC entry with -interface and -version.

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 INTERFACE1 INTERFACEV BINDING1}] != 0} {return;}
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -interface [list $INTERFACE1] -binding [list $BINDING1]"] != 0} {return;}
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -interface [list $INTERFACEV] -binding [list $BINDING1]"] != 0} {return;}

    # Run the test
    test " rpcentry unexport $RPCENTRY1 -interface [list $INTERFACE1] -version exact
           rpcentry show $RPCENTRY1
         " compare_lists [format "{%s {%s}} noobjects" [join $INTERFACEV " "] $BINDING1]

    # Cleanup steps
    if {[dcp_wrap {rpcentry delete $RPCENTRY1}] != 0} {return;}
}

proc rpcentry_unexport_7_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_unexport_7_P: Verify that we can unexport an RPC entry with -interface and -object.

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 INTERFACE1 BINDING1 OBJECT1}] != 0} {return;}
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -interface [list $INTERFACE1] -binding [list $BINDING1] -object $OBJECT1"] != 0} {return;}

    # Run the test
    test " rpcentry unexport $RPCENTRY1 -interface [list $INTERFACE1] -object $OBJECT1
           rpcentry show $RPCENTRY1
         " compare_strings {}

    # Cleanup steps
    if {[dcp_wrap {rpcentry delete $RPCENTRY1}] != 0} {return;}
}

proc rpcentry_unexport_9_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_unexport_9_P: Verify that we can unexport a list of RPC entries with -object.

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 RPCENTRY2 RPCENTRY3 OBJECT1}] != 0} {return;}
    if {[dcp_wrap {rpcentry export "$RPCENTRY1 $RPCENTRY2 $RPCENTRY3" -object $OBJECT1}] != 0} {return;}

    # Run the test
    test { rpcentry unexport "$RPCENTRY1 $RPCENTRY2 $RPCENTRY3" -object $OBJECT1
           rpcentry show $RPCENTRY1
         } compare_strings {}

    # Cleanup steps
    if {[dcp_wrap {rpcentry delete "$RPCENTRY1 $RPCENTRY2 $RPCENTRY3"}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 "
set ic1 "rpcentry_unexport_4_P"
set ic2 "rpcentry_unexport_5_P"
set ic3 "rpcentry_unexport_6_P"
set ic4 "rpcentry_unexport_7_P"
set ic5 "rpcentry_unexport_9_P"
set tet_startup rpcentry_unexport_startup
set tet_cleanup rpcentry_unexport_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
