#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: rpcentry_export_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:25:02  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:21  marty]
#
# Revision 1.1.4.1  1995/12/11  15:14:09  root
# 	Submit
# 	[1995/12/11  14:31:32  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:15  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:00  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:22  melman]
# 
# Revision 1.1.2.4  1994/08/18  21:16:22  coren
# 	Fixed to pass correct TCL-syntax interfaces and bindings (CR 11733).
# 	[1994/08/18  20:01:20  coren]
# 
# Revision 1.1.2.3  1994/06/17  18:43:40  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:27:41  devsrc]
# 
# Revision 1.1.2.2  1994/05/26  20:21:04  coren
# 	Positive tests filled in.
# 	[1994/05/26  19:11:55  coren]
# 
# Revision 1.1.2.1  1994/05/19  20:02:34  melman
# 	initial rpccp tests
# 	[1994/05/19  19:59:43  melman]
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
proc rpcentry_export_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc rpcentry_export_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}    

proc rpcentry_export_5_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_export_5_P: Verify that we can export to an RPC entry with -object.

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 OBJECT1}] != 0} {return;}
    if {[dcp_wrap "rpcentry create $RPCENTRY1"] != 0} {return;}

    # Run the test
    test { rpcentry export $RPCENTRY1 -object $OBJECT1
           rpcentry show $RPCENTRY1
         } compare_strings [format "{%s}" $OBJECT1]

    # Cleanup steps
    if {[dcp_wrap {rpcentry delete $RPCENTRY1}] != 0} {return;}
}

proc rpcentry_export_6_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_export_6_P: Verify that we can export to an RPC entry a list of objects.

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 OBJECT1 OBJECT2}] != 0} {return;}
    if {[dcp_wrap {rpcentry create $RPCENTRY1}] != 0} {return;}

    # Run the test
    test { rpcentry export $RPCENTRY1 -object "$OBJECT1 $OBJECT2"
           rpcentry show $RPCENTRY1
         } compare_lists [format "{%s %s}" $OBJECT1 $OBJECT2]

    # Cleanup steps
    if {[dcp_wrap {rpcentry delete $RPCENTRY1}] != 0} {return;}
}

proc rpcentry_export_7_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_export_7_P: Verify that we can export an RPC entry with -interface and -binding.

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 INTERFACE1 BINDING1}] != 0} {return;}
    if {[dcp_wrap {rpcentry create $RPCENTRY1}] != 0} {return;}

    # Run the test
    test " rpcentry export $RPCENTRY1 -interface [list $INTERFACE1] -binding [list $BINDING1]
           rpcentry show $RPCENTRY1
         " compare_lists [format "{%s {%s}} noobjects" [join $INTERFACE1 " "] $BINDING1]

    # Cleanup steps
    if {[dcp_wrap {rpcentry delete $RPCENTRY1}] != 0} {return;}
}

proc rpcentry_export_8_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_export_8_P: Verify that we can export with -interface and a list of bindings.

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 INTERFACE1S BINDING1 BINDING2}] != 0} {return;}
    if {[dcp_wrap {rpcentry create $RPCENTRY1}] != 0} {return;}
    if {[dcp_wrap {set bindlist "[list $BINDING1] [list $BINDING2]"}] != 0} {return;}

    # Run the test
    test { rpcentry export $RPCENTRY1 -interface $INTERFACE1S -binding $bindlist
           lsort [lrange [lindex [rpcentry show $RPCENTRY1 -interface $INTERFACE1S] 0] 2 end]
         } compare_lists "[lsort $bindlist]"

    # Cleanup steps
    if {[dcp_wrap {rpcentry delete $RPCENTRY1}] != 0} {return;}
}

proc rpcentry_export_9_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_export_9_P: Verify that we can export an RPC entry with all options.

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 OBJECT1 BINDING1 INTERFACE1}] != 0} {return;}
    if {[dcp_wrap "rpcentry create $RPCENTRY1"] != 0} {return;}

    # Run the test
    test " rpcentry export $RPCENTRY1 -object $OBJECT1 -binding [list $BINDING1] -interface [list $INTERFACE1]
           rpcentry show $RPCENTRY1
         " compare_lists [format "{%s {%s}} {%s}" [join $INTERFACE1 " "] $BINDING1 $OBJECT1]

    # Cleanup steps
    if {[dcp_wrap {rpcentry delete $RPCENTRY1}] != 0} {return;}
}

proc rpcentry_export_10_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_export_10_P: Verify that we can export a list of RPC entries with -object.

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 RPCENTRY2 RPCENTRY3 OBJECT1}] != 0} {return;}
    if {[dcp_wrap {rpcentry create "$RPCENTRY1 $RPCENTRY2 $RPCENTRY3"}] != 0} {return;}

    # Run the test
    test { rpcentry export "$RPCENTRY1 $RPCENTRY2 $RPCENTRY3" -object $OBJECT1
           rpcentry show "$RPCENTRY1 $RPCENTRY2 $RPCENTRY3"
         } compare_lists [format "{%s} {%s} {%s}" $OBJECT1 $OBJECT1 $OBJECT1]
    # Cleanup steps

    if {[dcp_wrap {rpcentry delete "$RPCENTRY1 $RPCENTRY2 $RPCENTRY3"}] != 0} {return;}
}

proc rpcentry_export_11_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_export_11_P: Verify that we can export with -interface and spaces in the interface spec.

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 INTERFACE3 BINDING1}] != 0} {return;}
    if {[dcp_wrap {rpcentry create "$RPCENTRY1"}] != 0} {return;}

    # Run the test
    test { rpcentry export $RPCENTRY1 -interface $INTERFACE3 -binding $BINDING1
           rpcentry show $RPCENTRY1
         } compare_lists [format "{%s {%s}} noobjects" $INTERFACE3 $BINDING1] 
    # Cleanup steps

    if {[dcp_wrap {rpcentry delete "$RPCENTRY1"}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7"
set ic1 "rpcentry_export_5_P"
set ic2 "rpcentry_export_6_P"
set ic3 "rpcentry_export_7_P"
set ic4 "rpcentry_export_8_P"
set ic5 "rpcentry_export_9_P"
set ic6 "rpcentry_export_10_P"
set ic7 "rpcentry_export_11_P"
set tet_startup rpcentry_export_startup
set tet_cleanup rpcentry_export_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl










