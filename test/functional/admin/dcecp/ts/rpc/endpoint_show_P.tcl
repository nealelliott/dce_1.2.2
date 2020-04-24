#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: endpoint_show_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:24:43  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:14  marty]
#
# Revision 1.1.4.1  1995/12/11  15:13:59  root
# 	Submit
# 	[1995/12/11  14:31:26  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:08  root]
# 
# Revision 1.1.2.1  1994/09/28  21:44:53  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:14  melman]
# 
# Revision 1.1.2.5  1994/09/09  20:50:03  coren
# 	Added tests for -uuid and host binding argument (CR12022).
# 	[1994/09/09  20:07:31  coren]
# 
# Revision 1.1.2.4  1994/08/18  21:16:21  coren
# 	Fixed to pass correct TCL-syntax interfaces and bindings (CR 11733).
# 	[1994/08/18  20:01:11  coren]
# 
# Revision 1.1.2.3  1994/08/10  19:22:35  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:55  annie]
# 
# Revision 1.1.2.2  1994/08/03  21:10:45  coren
# 	Corrected expected output format (CR 11551)
# 	[1994/08/03  21:01:12  coren]
# 
# Revision 1.1.2.1  1994/06/20  19:52:14  coren
# 	Changed startup proc to remove remnants of previous tests
# 	[1994/06/16  19:40:42  coren]
# 
# 	Initial version.
# 	[1994/06/16  17:09:21  coren]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc endpoint_show_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW LBINDING1 LBINDING2 SBINDING1 SBINDING2 INTERFACE1 LINTERFACE1 LINTERFACEV INTERFACEV IPADDR
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set LINTERFACE1 [list $INTERFACE1]}] != 0} {return;}
    if {[dcp_wrap {set LINTERFACEV [list $INTERFACEV]}] != 0} {return;}
    if {[dcp_wrap {set IPADDR [exec getip [exec uname -n]]}] != 0} {return;}
    if {[dcp_wrap {set LBINDING1 [list "ncadg_ip_udp $IPADDR 1234"]}] != 0} {return;}
    if {[dcp_wrap {set LBINDING2 [list "ncacn_ip_tcp $IPADDR 5678"]}] != 0} {return;}

# don't care if the following commands succeed, but we need to make
# sure that any remnants of previous tests are gone

    catch "endpoint delete -interface $LINTERFACE1 $LBINDING1"
    catch "endpoint delete -interface $LINTERFACE1 $LBINDING2"
    catch "endpoint delete -interface $LINTERFACEV $LBINDING1"
    catch "endpoint delete -interface $LINTERFACEV $LBINDING2"
}

proc endpoint_show_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc endpoint_show_3_P {} {
    # Identify the test
    tet_infoline endpoint_show_3_P: Check that command succeeds with no options.

    # Setup steps

    # Run the test
    test { llength [eval {endpoint show}]
         } dcp_no_error {}

    # Cleanup steps
}

proc endpoint_show_4_P {} {
    # Identify the test
    tet_infoline endpoint_show_4_P: Check that all endpoints matching a specified interface
    tet_infoline endpoint_show_4_P: are shown if no other options given

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1 OBJECT1 LBINDING2 OBJECT2}] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $LBINDING1 -object $OBJECT1"] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $LBINDING1 -object $OBJECT2"] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $LBINDING2"] != 0} {return;}

    # Run the test
    test { llength  [eval "endpoint show -interface $LINTERFACE1"]
         } compare_strings {3}

    # Cleanup steps
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding $LBINDING2"] != 0} {return;}
}

proc endpoint_show_5_P {} {
    # Identify the test
    tet_infoline endpoint_show_5_P: Check that only endpoints matching a specified obect UUID
    tet_infoline endpoint_show_5_P: are shown.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1 OBJECT1 OBJECT2}] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $LBINDING1 -object $OBJECT1"] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $LBINDING1 -object $OBJECT2"] != 0} {return;}

    # Run the test
    test " endpoint show -interface $LINTERFACE1 -object $OBJECT1
	 " compare_lists [format "{{object %s} {interface %s} {binding %s}}" $OBJECT1 $LINTERFACE1 $LBINDING1]

    # Cleanup steps
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}
}

proc endpoint_show_6_P {} {
    # Identify the test
    tet_infoline endpoint_show_6_P: Check that obect UUID and annotation are shown if present.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1 OBJECT1}] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $LBINDING1 -object $OBJECT1 -annotation Annotation"] != 0} {return;}

    # Run the test
    test " endpoint show -interface $LINTERFACE1
         " compare_lists [format "{{object %s} {interface %s} {binding %s} {annotation Annotation}}" $OBJECT1 $LINTERFACE1 $LBINDING1]

    # Cleanup steps
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}
}

proc endpoint_show_7_P {} {
    # Identify the test
    tet_infoline endpoint_show_7_P: Check that obect UUID and annotation are not shown if null.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1}] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}

    # Run the test
    test " endpoint show -interface $LINTERFACE1
         " compare_lists [format "{{interface %s} {binding %s}}" $LINTERFACE1 $LBINDING1]

    # Cleanup steps
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}
}

proc endpoint_show_8_P {} {
    # Identify the test
    tet_infoline endpoint_show_8_P: Check that annotation is displayed as a list if it contains
    tet_infoline endpoint_show_8_P: spaces.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1}] != 0} {return;}
    if {[dcp_wrap {set annot_string [list "This is an annotation"]}] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $LBINDING1 -annotation $annot_string"] != 0} {return;}

    # Run the test
    test " endpoint show -interface $LINTERFACE1
         " compare_lists [format "{{interface %s} {binding %s} {annotation {This is an annotation}}}" $LINTERFACE1 $LBINDING1]

    # Cleanup steps
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}
}

proc endpoint_show_9_P {} {
    # Identify the test
    tet_infoline endpoint_show_9_P: Check that object UUIDs can be specified in a list.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1 LBINDING2 OBJECT1 OBJECT2}] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $LBINDING1 -object $OBJECT1"] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $LBINDING2 -object $OBJECT2"] != 0} {return;}
    if {[dcp_wrap {set objlist [list "$OBJECT1 $OBJECT2"]}] != 0} {return;}

    # Run the test
    test { llength [eval "endpoint show -interface $LINTERFACE1 -object $objlist"]
         } compare_strings {2}

    # Cleanup steps
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding $LBINDING2"] != 0} {return;}
}

proc endpoint_show_10_P {} {
    # Identify the test
    tet_infoline endpoint_show_10_P: Check that empty string is returned if no endpoints match.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LBINDING1 OBJECT1 OBJECT2}] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $LBINDING1 -object $OBJECT1"] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}

    # Run the test
    test " endpoint show -interface $LINTERFACE1 -object $OBJECT2
         " compare_strings {}

    # Cleanup steps
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}
}

proc endpoint_show_11_P {} {
    # Identify the test
    tet_infoline endpoint_show_11_P: Check that -version option is accepted,

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LINTERFACEV LBINDING1 LBINDING2}] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACEV -binding $LBINDING2"] != 0} {return;}

    # Run the test
    test " endpoint show -interface $LINTERFACEV -version exact
         " compare_lists [format "{{interface %s} {binding %s}}" $LINTERFACEV $LBINDING2]

    # Cleanup steps
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}
    if {[dcp_wrap "endpoint delete -interface $LINTERFACEV -binding $LBINDING2"] != 0} {return;}
}

proc endpoint_show_12_P {} {
    # Identify the test
    tet_infoline endpoint_show_12_P: Check that -version upto displays all qualifying versions.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LINTERFACEV LBINDING1 LBINDING2}] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACEV -binding $LBINDING2"] != 0} {return;}

    # Run the test
    test { llength [eval "endpoint show -interface $LINTERFACEV -version upto"]
         } compare_strings {2}

    # Cleanup steps
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}
    if {[dcp_wrap "endpoint delete -interface $LINTERFACEV -binding $LBINDING2"] != 0} {return;}
}

proc endpoint_show_13_P {} {
    # Identify the test
    tet_infoline endpoint_show_13_P: Check that -version upto does not display nonqualifying versions.

    # Setup steps
    if {[dcp_wrap {global LINTERFACE1 LINTERFACEV LBINDING1 LBINDING2}] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACEV -binding $LBINDING2"] != 0} {return;}

    # Run the test
    test " endpoint show -interface $LINTERFACE1 -version upto
         " compare_lists [format "{{interface %s} {binding %s}}" $LINTERFACE1 $LBINDING1]

    # Cleanup steps
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}
    if {[dcp_wrap "endpoint delete -interface $LINTERFACEV -binding $LBINDING2"] != 0} {return;}
}

proc endpoint_show_14_P {} {
    # Identify the test
    tet_infoline endpoint_show_14_P: Check that host can be specified as a binding in string syntax.

    # Setup steps
    if {[dcp_wrap {global INTERFACE1 LINTERFACE1 LBINDING1 IPADDR}] != 0} {return;}
    if {[dcp_wrap "endpoint create -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}

    # Run the test
    test " endpoint show ncadg_ip_udp:$IPADDR -interface $LINTERFACE1
         " compare_lists [format "{{interface %s} {binding %s}}" $LINTERFACE1 $LBINDING1]

    # Cleanup steps
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}
}

proc endpoint_show_15_P {} {
    # Identify the test
    tet_infoline endpoint_show_15_P: Check that host can be specified as a binding in TCL syntax.

    # Setup steps
    if {[dcp_wrap {global INTERFACE1 LINTERFACE1 LBINDING1 IPADDR HBINDING}] != 0} {return;}
    if {[dcp_wrap {endpoint create -interface $INTERFACE1 -binding $LBINDING1}] != 0} {return;}
    if {[dcp_wrap {set HBINDING [list ncadg_ip_udp $IPADDR]}] != 0} {return;}
    # Run the test
    test " endpoint show [list $HBINDING] -interface $LINTERFACE1
         " compare_lists [format "{{interface %s} {binding %s}}" $LINTERFACE1 $LBINDING1]

    # Cleanup steps
    if {[dcp_wrap "endpoint delete -interface $LINTERFACE1 -binding $LBINDING1"] != 0} {return;}
}

proc endpoint_show_16_P {} {
    # Identify the test
    tet_infoline endpoint_show_16_P: Check that UUID of endpoint map is returned if -uuid specified.

    # Setup steps

    # Run the test
    test { endpoint show -uuid
	 } dcp_no_error {0}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 "
set ic1 "endpoint_show_3_P"
set ic2 "endpoint_show_4_P"
set ic3 "endpoint_show_5_P"
set ic4 "endpoint_show_6_P"
set ic5 "endpoint_show_7_P"
set ic6 "endpoint_show_8_P"
set ic7 "endpoint_show_9_P"
set ic8 "endpoint_show_10_P"
set ic9 "endpoint_show_11_P"
set ic10 "endpoint_show_12_P"
set ic11 "endpoint_show_13_P"
set ic12 "endpoint_show_14_P"
set ic13 "endpoint_show_15_P"
set ic14 "endpoint_show_16_P"
set tet_startup endpoint_show_startup
set tet_cleanup endpoint_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
