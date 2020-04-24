#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: endpoint_delete_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:24:38  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:12  marty]
#
# Revision 1.1.4.1  1995/12/11  15:13:55  root
# 	Submit
# 	[1995/12/11  14:31:24  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:06  root]
# 
# Revision 1.1.2.1  1994/09/28  21:44:50  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:12  melman]
# 
# Revision 1.1.2.4  1994/08/18  21:16:19  coren
# 	Fixed to pass correct TCL-syntax interfaces and bindings (CR 11733).
# 	[1994/08/18  20:00:51  coren]
# 
# Revision 1.1.2.3  1994/08/10  19:22:34  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:52  annie]
# 
# Revision 1.1.2.2  1994/08/03  21:10:44  coren
# 	Corrected expected output format (CR 11551)
# 	[1994/08/03  21:01:09  coren]
# 
# Revision 1.1.2.1  1994/06/20  19:52:11  coren
# 	Changed startup proc to remove remnants of previous tests
# 	[1994/06/16  19:40:39  coren]
# 
# 	Initial version.
# 	[1994/06/16  17:09:14  coren]
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
    global DCP_CLIENT DCP_CLIENT_PW LBINDING1 LBINDING2 SBINDING1 SBINDING2 INTERFACE1 LINTERFACE1
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set LINTERFACE1 [list $INTERFACE1]}] != 0} {return:}
    if {[dcp_wrap {set IPADDR [exec getip [exec uname -n]]}] != 0} {return;}
    if {[dcp_wrap {set LBINDING1 [list "ncadg_ip_udp $IPADDR 1234"]}] != 0} {return;}
    if {[dcp_wrap {set LBINDING2 [list "ncacn_ip_tcp $IPADDR 5678"]}] != 0} {return;}
    if {[dcp_wrap {set SBINDING1 "ncadg_ip_udp:$IPADDR\\\[1234\\\]"}] != 0} {return;}
    if {[dcp_wrap {set SBINDING2 "ncacn_ip_tcp:$IPADDR\\\[5678\\\]"}] != 0} {return;}

# don't care if the following commands succeed, but we need to make
# sure that any remnants of previous tests are gone

    catch "endpoint delete -interface $LINTERFACE1 $LBINDING1"
    catch "endpoint delete -interface $LINTERFACE1 $LBINDING2"
}

proc endpoint_delete_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc endpoint_delete_7_P {} {
    # Identify the test
    tet_infoline endpoint_delete_7_P: Check that an endpoint can be deleted when specifying an object UUID.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1 OBJECT1 OBJECT2}] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $LBINDING1 -object $OBJECT1"] != 0} {return;}

    # Run the test
    test " endpoint delete -interface $LINTERFACE1 -binding $LBINDING1 -object $OBJECT1
           endpoint show -interface $LINTERFACE1
         " compare_strings {}

    # Cleanup steps
}

proc endpoint_delete_8_P {} {
    # Identify the test
    tet_infoline endpoint_delete_8_P: Check that an endpoint can be deleted when specifying an object
    tet_infoline endpoint_delete_8_P: UUID, without removing any non-matching endpoints.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1 OBJECT1 OBJECT2}] != 0} {return;}
    set objlist [list "$OBJECT1 $OBJECT2"]
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $LBINDING1 -object $objlist"] != 0} {return;}

    # Run the test
    test " endpoint delete -interface $LINTERFACE1 -binding $LBINDING1 -object $OBJECT1
           endpoint show -interface $LINTERFACE1
         " compare_lists [format "{{object %s} {interface %s} {binding %s}}" $OBJECT2 $LINTERFACE1 $LBINDING1]

    # Cleanup steps
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}
}

proc endpoint_delete_9_P {} {
    # Identify the test
    tet_infoline endpoint_delete_9_P: Check that endpoints can be deleted by specifying a list of object
    tet_infoline endpoint_delete_9_P: UUIDs.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1 OBJECT1 OBJECT2}] != 0} {return;}
    set objlist [list "$OBJECT1 $OBJECT2"]
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $LBINDING1 -object $objlist"] != 0} {return;}
    # Run the test
    test " endpoint delete -interface $LINTERFACE1 -binding $LBINDING1 -object $objlist
           endpoint show -interface $LINTERFACE1
         " compare_strings {}

    # Cleanup steps
}

proc endpoint_delete_10_P {} {
    # Identify the test
    tet_infoline endpoint_delete_10_P: Check that all matching endpoints are deleted if no object
    tet_infoline endpoint_delete_10_P: UUIDs are specified.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1 OBJECT1 OBJECT2}] != 0} {return;}
    set objlist [list "$OBJECT1 $OBJECT2"]
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $LBINDING1 -object $objlist"] != 0} {return;}

    # Run the test
    test " endpoint delete -interface $LINTERFACE1 -binding $LBINDING1
           endpoint show -interface $LINTERFACE1
         " compare_strings {}

    # Cleanup steps
}

proc endpoint_delete_11_P {} {
    # Identify the test
    tet_infoline endpoint_delete_11_P: Check that only endpoints with specified binding are deleted.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1 LBINDING2}] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $LBINDING2"] != 0} {return;}

    # Run the test
    test " endpoint delete -interface $LINTERFACE1 -binding $LBINDING1
           endpoint show -interface $LINTERFACE1
         " compare_lists [format "{{interface %s} {binding %s}}" $LINTERFACE1 $LBINDING2]

    # Cleanup steps
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding $LBINDING2"] != 0} {return;}
}

proc endpoint_delete_12_P {} {
    # Identify the test
    tet_infoline endpoint_delete_12_P: Check that an endpoint can be deleted by specifying a binding in
    tet_infoline endpoint_delete_12_P: string format.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1 SBINDING1}] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}

    # Run the test
    test " endpoint delete -interface $LINTERFACE1 -binding $SBINDING1
           endpoint show -interface $LINTERFACE1
         " compare_strings {}

    # Cleanup steps
}

proc endpoint_delete_13_P {} {
    # Identify the test
    tet_infoline endpoint_delete_13_P: Check that endpoints can be deleted by specifying a list of
    tet_infoline endpoint_delete_13_P: bindings.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1 LBINDING2}] != 0} {return;}
    if {[dcp_wrap {set bindlist [list "$LBINDING1 $LBINDING2"]}] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $bindlist"] != 0} {return;}

    # Run the test
    test " endpoint delete -interface $LINTERFACE1 -binding $bindlist
           endpoint show -interface $LINTERFACE1
         " compare_strings {}

    # Cleanup steps
}

proc endpoint_delete_14_P {} {
    # Identify the test
    tet_infoline endpoint_delete_14_P: Check that a cross-product of endpoints can be deleted by
    tet_infoline endpoint_delete_14_P: specifying a list of objects and a list of bindings.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1 LBINDING2 OBJECT1 OBJECT2}] != 0} {return;}
    if {[dcp_wrap {set objlist [list "$OBJECT1 $OBJECT2"]}] != 0} {return;}
    if {[dcp_wrap {set bindlist [list "$LBINDING1 $LBINDING2"]}] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $bindlist -object $objlist"] != 0} {return;}

    # Run the test
    test " endpoint delete -interface $LINTERFACE1 -binding $bindlist -object $objlist
           endpoint show -interface $LINTERFACE1
         " compare_strings {}

    # Cleanup steps
}

proc endpoint_delete_15_P {} {
    # Identify the test
    tet_infoline endpoint_delete_15_P: Check that endpoints can be deleted by specifying a list of
    tet_infoline endpoint_delete_15_P: string bindings.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1 LBINDING2 SBINDING1 SBINDING2}] != 0} {return;}
    if {[dcp_wrap {set bindlist [list "$SBINDING1 $SBINDING2"]}] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $bindlist"] != 0} {return;}

    # Run the test
    test " endpoint delete -interface $LINTERFACE1 -binding $bindlist
           endpoint show -interface $LINTERFACE1
         " compare_strings {}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 "
set ic1 "endpoint_delete_7_P"
set ic2 "endpoint_delete_8_P"
set ic3 "endpoint_delete_9_P"
set ic4 "endpoint_delete_10_P"
set ic5 "endpoint_delete_11_P"
set ic6 "endpoint_delete_12_P"
set ic7 "endpoint_delete_13_P"
set ic8 "endpoint_delete_14_P"
set ic9 "endpoint_delete_15_P"
set tet_startup endpoint_delete_startup
set tet_cleanup endpoint_delete_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
