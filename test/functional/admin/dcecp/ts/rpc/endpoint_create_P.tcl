#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: endpoint_create_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:24:33  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:10  marty]
#
# Revision 1.1.4.1  1995/12/11  15:13:50  root
# 	Submit
# 	[1995/12/11  14:31:23  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:04  root]
# 
# Revision 1.1.2.1  1994/09/28  21:44:46  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:08  melman]
# 
# Revision 1.1.2.4  1994/08/18  21:16:17  coren
# 	Fixed to pass correct TCL-syntax interfaces and bindings (CR 11733).
# 	[1994/08/18  20:00:32  coren]
# 
# Revision 1.1.2.3  1994/08/10  19:22:32  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:50  annie]
# 
# Revision 1.1.2.2  1994/08/03  21:10:41  coren
# 	Corrected expected output format (CR 11551)
# 	[1994/08/03  21:01:06  coren]
# 
# Revision 1.1.2.1  1994/06/20  19:52:07  coren
# 	Changed startup proc to remove remnants of previous tests
# 	[1994/06/16  19:40:32  coren]
# 
# 	Initial version.
# 	[1994/06/16  17:09:07  coren]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc endpoint_create_startup {} {
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

proc endpoint_create_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc endpoint_create_5_P {} {
    # Identify the test
    tet_infoline endpoint_create_5_P: Check that an endpoint can be created without an object
    tet_infoline endpoint_create_5_P: UUID specification.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1}] != 0} {return;}

    # Run the test
    test " endpoint create -interface $LINTERFACE1 -binding $LBINDING1
           endpoint show -interface $LINTERFACE1
         " compare_lists [format "{{interface %s} {binding %s}}" $LINTERFACE1 $LBINDING1]

    # Cleanup steps
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}
}

proc endpoint_create_6_P {} {
    # Identify the test
    tet_infoline endpoint_create_6_P: Check that an endpoint can be created with an object
    tet_infoline endpoint_create_6_P: UUID specification.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1 OBJECT1}] != 0} {return;}

    # Run the test
    test " endpoint create -interface $LINTERFACE1 -binding $LBINDING1 -object $OBJECT1
           endpoint show -interface $LINTERFACE1
         " compare_lists [format "{{object %s} {interface %s} {binding %s}}" $OBJECT1 $LINTERFACE1 $LBINDING1]

    # Cleanup steps
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}
}

proc endpoint_create_7_P {} {
    # Identify the test
    tet_infoline endpoint_create_7_P: Check that endpoints can be created with a list of object
    tet_infoline endpoint_create_7_P: UUID specifications.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1 OBJECT1 OBJECT2}] != 0} {return;}
    if {[dcp_wrap {set objlist [list "$OBJECT1 $OBJECT2"]}] != 0} {return;}

    # Run the test
    test " endpoint create -interface $LINTERFACE1 -binding $LBINDING1 -object $objlist
           endpoint show -interface $LINTERFACE1
         " compare_lists [format "{{object %s} {interface %s} {binding %s}} {{object %s} {interface %s} {binding %s}}" $OBJECT1 $LINTERFACE1 $LBINDING1 $OBJECT2 $LINTERFACE1 $LBINDING1]

    # Cleanup steps
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}
}

proc endpoint_create_8_P {} {
    # Identify the test
    tet_infoline endpoint_create_8_P: Check that endpoints can be created with a list of string
    tet_infoline endpoint_create_8_P: binding specifications.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1 LBINDING2}] != 0} {return;}
    if {[dcp_wrap {set bindlist [list "$LBINDING1 $LBINDING2"]}] != 0} {return;}

    # Run the test
    test " endpoint create -interface $LINTERFACE1 -binding $bindlist
           endpoint show -interface $LINTERFACE1
         " compare_lists [format "{{interface %s} {binding %s}} {{interface %s} {binding %s}}" $LINTERFACE1 $LBINDING1 $LINTERFACE1 $LBINDING2]

    # Cleanup steps
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding $bindlist"] != 0} {return;}
}

proc endpoint_create_9_P {} {
    # Identify the test
    tet_infoline endpoint_create_9_P: Check that endpoints can be created with a list of string
    tet_infoline endpoint_create_9_P: bindings and a list of object UUIDs.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1 LBINDING2 OBJECT1 OBJECT2}] != 0} {return;}
    if {[dcp_wrap {set objlist "$OBJECT1 $OBJECT2"}] != 0} {return;}
    if {[dcp_wrap {set bindlist "$LBINDING1 $LBINDING2"}] != 0} {return;}
    if {[dcp_wrap "set localif $LINTERFACE1"] != 0} {return;}

    # Run the test
    test { endpoint create -interface $localif -object $objlist -binding $bindlist
           llength [endpoint show -interface $localif]
         } compare_strings {4}

    # Cleanup steps
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding [list $bindlist]"] != 0} {return;}
}

proc endpoint_create_10_P {} {
    # Identify the test
    tet_infoline endpoint_create_10_P: Check that an endpoint can be created with an annotation string.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1}] != 0} {return;}

    # Run the test
    test " endpoint create -interface $LINTERFACE1 -binding $LBINDING1 -annotation {Annotation here}
           endpoint show -interface $LINTERFACE1
         " compare_lists [format "{{interface %s} {binding %s} {annotation {Annotation here}}}" $LINTERFACE1 $LBINDING1]

    # Cleanup steps
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}
}

proc endpoint_create_11_P {} {
    # Identify the test
    tet_infoline endpoint_create_11_P: Check that multiple endpoints can be created with an annotation string.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1 OBJECT1 OBJECT2}] != 0} {return;}
    if {[dcp_wrap {set objlist [list "$OBJECT1 $OBJECT2"]}] != 0} {return;}

    # Run the test
    test " endpoint create -interface $LINTERFACE1 -binding $LBINDING1 -object $objlist -annotation {Annotation here}
           endpoint show -interface $LINTERFACE1
         " compare_lists [format "{{object %s} {interface %s} {binding %s} {annotation {Annotation here}}} {{object %s} {interface %s} {binding %s} {annotation {Annotation here}}}" $OBJECT1 $LINTERFACE1 $LBINDING1 $OBJECT2 $LINTERFACE1 $LBINDING1]

    # Cleanup steps
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}
}

proc endpoint_create_12_P {} {
    # Identify the test
    tet_infoline endpoint_create_12_P: Check that an endpoint can be created with a binding
    tet_infoline endpoint_create_12_P: specified in string syntax.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1 SBINDING1}] != 0} {return;}

    # Run the test
    test " endpoint create -interface $LINTERFACE1 -binding $SBINDING1
           endpoint show -interface $LINTERFACE1
         " compare_lists [format "{{interface %s} {binding %s}}" $LINTERFACE1 $LBINDING1]

    # Cleanup steps
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding $SBINDING1"] != 0} {return;}
}

proc endpoint_create_13_P {} {
    # Identify the test
    tet_infoline endpoint_create_13_P: Check that an endpoint can be created with a list of bindings
    tet_infoline endpoint_create_13_P: specified in string syntax.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1 LBINDING2 SBINDING1 SBINDING2}] != 0} {return;}
    if {[dcp_wrap {set bindlist [list "$SBINDING1 $SBINDING2"]}] != 0} {return;}

    # Run the test
    test " endpoint create -interface $LINTERFACE1 -binding $bindlist
           endpoint show -interface $LINTERFACE1
         " compare_lists [format "{{interface %s} {binding %s}} {{interface %s} {binding %s}}" $LINTERFACE1 $LBINDING1 $LINTERFACE1 $LBINDING2]

    # Cleanup steps
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding $bindlist"] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 "
set ic1 "endpoint_create_5_P"
set ic2 "endpoint_create_6_P"
set ic3 "endpoint_create_7_P"
set ic4 "endpoint_create_8_P"
set ic5 "endpoint_create_9_P"
set ic6 "endpoint_create_10_P"
set ic7 "endpoint_create_11_P"
set ic8 "endpoint_create_12_P"
set ic9 "endpoint_create_13_P"
set tet_startup endpoint_create_startup
set tet_cleanup endpoint_create_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
