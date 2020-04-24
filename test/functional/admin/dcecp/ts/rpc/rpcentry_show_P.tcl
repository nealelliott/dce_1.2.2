#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: rpcentry_show_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:25:12  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:25  marty]
#
# Revision 1.1.4.1  1995/12/11  15:14:19  root
# 	Submit
# 	[1995/12/11  14:31:35  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:19  root]
# 
# Revision 1.1.2.1  1994/09/28  21:45:07  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:27  melman]
# 
# Revision 1.1.2.4  1994/08/18  21:16:25  coren
# 	Fixed to pass correct TCL-syntax interfaces and bindings (CR 11733).
# 	[1994/08/18  20:01:50  coren]
# 
# Revision 1.1.2.3  1994/06/17  18:43:43  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:27:57  devsrc]
# 
# Revision 1.1.2.2  1994/05/26  20:21:07  coren
# 	Positive tests filled in.
# 	[1994/05/26  19:12:02  coren]
# 
# Revision 1.1.2.1  1994/05/19  20:02:38  melman
# 	initial rpccp tests
# 	[1994/05/19  19:59:48  melman]
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
proc rpcentry_show_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc rpcentry_show_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}    

proc rpcentry_show_4_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_show_4_P: Verify that we can show an rpc entry.

    # Setup steps
    if {[dcp_wrap {global SECMASTER}] != 0} {return;}

    # Run the test
    test { catch {rpcentry show $SECMASTER} msg
         } compare_strings {0}

    # Cleanup steps
}

proc rpcentry_show_5_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_show_5_P: Verify that we can show a list of rpc entries.

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 RPCENTRY2 RPCENTRY3}] != 0} {return;}
    if {[dcp_wrap {rpcentry create "$RPCENTRY1 $RPCENTRY2 $RPCENTRY3"}] != 0} {return;}

    # Run the test
    test { catch {rpcentry show "$RPCENTRY1 $RPCENTRY2 $RPCENTRY3"} msg
         } compare_strings {0}

    # Cleanup steps
    if {[dcp_wrap {rpcentry delete "$RPCENTRY1 $RPCENTRY2 $RPCENTRY3"}] != 0} {return;}

}

proc rpcentry_show_6_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_show_6_P: Verify that we can show an rpc entry with -interface (single).

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 INTERFACE1 INTERFACE2 INTERFACE1S INTERFACE2S BINDING1 BINDING2}] != 0} {return;}
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -interface [list $INTERFACE1] -binding [list $BINDING1]"] != 0} {return;}
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -interface [list $INTERFACE2] -binding [list $BINDING2]"] != 0} {return;}


    # Run the test
    test { rpcentry show $RPCENTRY1 -interface $INTERFACE1S
         } compare_lists [format "{%s {%s}} noobjects" [join $INTERFACE1 " "] $BINDING1]

    # Cleanup steps
    if {[dcp_wrap {rpcentry delete $RPCENTRY1}] != 0} {return;}
}

proc rpcentry_show_7_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_show_7_P: Verify that we can show an rpc entry with -interface (list).

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 INTERFACE1 INTERFACE2 INTERFACE1S INTERFACE2S BINDING1 BINDING2}] != 0} {return;}
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -interface [list $INTERFACE1] -binding [list $BINDING1]"] != 0} {return;}
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -interface [list $INTERFACE2] -binding [list $BINDING2]"] != 0} {return;}

    # Run the test
    test { rpcentry show $RPCENTRY1 -interface "$INTERFACE1S $INTERFACE2S"
         } compare_lists [format "{%s {%s}} {%s {%s}} noobjects" [join $INTERFACE1 " "] $BINDING1 [join $INTERFACE2 " "] $BINDING2]

    # Cleanup steps
    if {[dcp_wrap {rpcentry delete $RPCENTRY1}] != 0} {return;}
}

proc rpcentry_show_8_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_show_8_P: Verify that we can show an rpc entry with -object (single).

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 OBJECT1 OBJECT2}] != 0} {return;}
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -object $OBJECT1"] != 0} {return;}
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -object $OBJECT2"] != 0} {return;}

    # Run the test
    test { rpcentry show $RPCENTRY1 -object $OBJECT1
         } compare_strings [format "{%s}" $OBJECT1]

    # Cleanup steps
    if {[dcp_wrap "rpcentry delete $RPCENTRY1"] != 0} {return;}    
}

proc rpcentry_show_9_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_show_9_P: Verify that we can show an rpc entry with -object (list).

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 OBJECT1 OBJECT2}] != 0} {return;}
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -object $OBJECT1"] != 0} {return;}
    if {[dcp_wrap "rpcentry export $RPCENTRY1 -object $OBJECT2"] != 0} {return;}

    # Run the test
    test { rpcentry show $RPCENTRY1 -object "$OBJECT1 $OBJECT2"
         } compare_lists [format "{%s %s}" $OBJECT1 $OBJECT2]

    # Cleanup steps
    if {[dcp_wrap "rpcentry delete $RPCENTRY1"] != 0} {return;}    
}

proc rpcentry_show_10_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_show_10_P: Verify the handling of the -noupdate option.

    # Setup steps
    if {[dcp_wrap {global SECMASTER}] != 0} {return;}

    # Run the test
    test {catch {rpcentry show $SECMASTER -noupdate} msg
         } compare_strings {0}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 "
set ic1 "rpcentry_show_4_P"
set ic2 "rpcentry_show_5_P"
set ic3 "rpcentry_show_6_P"
set ic4 "rpcentry_show_7_P"
set ic5 "rpcentry_show_8_P"
set ic6 "rpcentry_show_9_P"
set ic7 "rpcentry_show_10_P"
set tet_startup rpcentry_show_startup
set tet_cleanup rpcentry_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
