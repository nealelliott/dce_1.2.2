#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: endpoint_delete_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:24:35  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:11  marty]
#
# Revision 1.1.4.1  1995/12/11  15:13:52  root
# 	Submit
# 	[1995/12/11  14:31:23  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:05  root]
# 
# Revision 1.1.2.1  1994/09/28  21:44:48  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:10  melman]
# 
# Revision 1.1.2.4  1994/09/01  21:13:40  rousseau
# 	Mass test cleanup
# 	[1994/09/01  21:13:08  rousseau]
# 
# Revision 1.1.2.3  1994/08/18  21:16:18  coren
# 	Fixed error messages to match improvements in dcp.sams.
# 	[1994/08/18  20:00:42  coren]
# 
# Revision 1.1.2.2  1994/08/10  19:22:33  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:51  annie]
# 
# Revision 1.1.2.1  1994/06/20  19:52:09  coren
# 	Changed startup proc to remove remnants of previous tests
# 	[1994/06/16  19:40:36  coren]
# 
# 	Initial version.
# 	[1994/06/16  17:09:10  coren]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc endpoint_delete_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW LBINDING1 LBINDING2 SBINDING1 SBINDING2 INTERFACE1
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set IPADDR [exec getip [exec uname -n]]}] != 0} {return;}
    if {[dcp_wrap {set LBINDING1 [list "ncadg_ip_udp $IPADDR 1234"]}] != 0} {return;}
    if {[dcp_wrap {set LBINDING2 [list "ncacn_ip_tcp $IPADDR 5678"]}] != 0} {return;}

# don't care if the following commands succeed, but we need to make
# sure that any remnants of previous tests are gone

    catch "endpoint delete -interface [list $INTERFACE1] $LBINDING1"
    catch "endpoint delete -interface [list $INTERFACE1] $LBINDING2"
}

proc endpoint_delete_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc endpoint_delete_1_N {} {
    # Identify the test
    tet_infoline endpoint_delete_1_N: Check that any non-option argument is trapped as an error.

    # Setup steps
    if {[dcp_wrap {global INTERFACE1 BINDING1}] != 0} {return;}

    # Run the test
    test {
        list [catch "endpoint delete foo -interface $INTERFACE1 -binding $BINDING1" msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc endpoint_delete_2_N {} {
    # Identify the test
    tet_infoline endpoint_delete_2_N: Check that any unrecognized option is trapped as an error.

    # Setup steps
    if {[dcp_wrap {global INTERFACE1 BINDING1}] != 0} {return;}

    # Run the test
    test {
        list [catch "endpoint delete -foo -interface $INTERFACE1 -binding $BINDING1" msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc endpoint_delete_3_N {} {
    # Identify the test
    tet_infoline endpoint_delete_3_N: Check that -interface option is required.

    # Setup steps
    if {[dcp_wrap {global BINDING1}] != 0} {return;}

    # Run the test
    test {
        list [catch "endpoint delete -binding [list $BINDING1]" msg] $msg
    } compare_strings {1 {Interface id not entered.}}

    # Cleanup steps
}

proc endpoint_delete_4_N {} {
    # Identify the test
    tet_infoline endpoint_delete_4_N: Check that -binding option is required.

    # Setup steps
    if {[dcp_wrap {global INTERFACE1}] != 0} {return;}

    # Run the test
    test {
        list [catch "endpoint delete -interface [list $INTERFACE1]" msg] $msg
    } compare_strings {1 {No string binding entered.}}

    # Cleanup steps
}

proc endpoint_delete_5_N {} {
    # Identify the test
    tet_infoline endpoint_delete_5_N: Check that attempt to delete nonexistent endpoint is trapped
    tet_infoline endpoint_delete_5_N: as an error.

    # Setup steps
    if {[dcp_wrap {global INTERFACE1 LBINDING1}] != 0} {return;}

    # Run the test
    test {
        list [catch "endpoint delete -interface [list $INTERFACE1] -binding $LBINDING1" msg] $msg
    } compare_strings {1 {Not registered in endpoint map}}

    # Cleanup steps
}

proc endpoint_delete_6_N {} {
    # Identify the test
    tet_infoline endpoint_delete_6_N: Check that attempt to delete endpoint with incorrect object UUID
    tet_infoline endpoint_delete_6_N: is trapped as an error.

    # Setup steps
    if {[dcp_wrap {global INTERFACE1 LBINDING1 OBJECT1 OBJECT2}] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface [list $INTERFACE1] -binding $LBINDING1 -object $OBJECT1"] != 0} {return;}

    # Run the test
    test {
        list [catch "endpoint delete -interface [list $INTERFACE1] -binding $LBINDING1 -object $OBJECT2" msg] $msg
    } compare_strings {1 {Not registered in endpoint map}}

    # Cleanup steps
    if {[dcp_wrap "endpoint delete -interface [list $INTERFACE1] -binding $LBINDING1"] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 "
set ic1 "endpoint_delete_1_N"
set ic2 "endpoint_delete_2_N"
set ic3 "endpoint_delete_3_N"
set ic4 "endpoint_delete_4_N"
set ic5 "endpoint_delete_5_N"
set ic6 "endpoint_delete_6_N"
set tet_startup endpoint_delete_startup
set tet_cleanup endpoint_delete_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
