#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: acl_check_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:09:00  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:27:43  marty]
#
# Revision 1.1.4.2  1995/12/13  15:23:53  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/11/15  13:03 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/15  13:02 UTC  truitt
# 	Fix parameters being used for the acl tests.
# 	[1995/12/11  15:50:44  root]
# 
# Revision 1.1.2.2  1994/10/01  01:32:54  rousseau
# 	Dynamically generate string bindings.
# 	[1994/10/01  01:28:31  rousseau]
# 
# Revision 1.1.2.1  1994/09/28  21:37:04  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:03  melman]
# 
# Revision 1.1.2.10  1994/09/26  21:04:09  salamone
# 	CR9938 - Change ACL mgr names and return UNINITIATED for those tests that aren't configured.
# 	[1994/09/26  21:02:11  salamone]
# 
# Revision 1.1.2.9  1994/09/08  14:00:47  salamone
# 	CR11968-Setting authentication on binding handles.
# 	[1994/09/08  13:59:19  salamone]
# 
# Revision 1.1.2.8  1994/09/02  18:50:55  salamone
# 	Compare ACL entry with output from "acl check".
# 	[1994/09/02  18:31:20  salamone]
# 
# Revision 1.1.2.7  1994/08/25  15:33:01  salamone
# 	CR11635 - Handle 1 string binding specification for an ACL object.
# 	[1994/08/25  15:28:54  salamone]
# 
# Revision 1.1.2.6  1994/07/18  20:04:51  salamone
# 	Add tests for ploymorphic object support
# 	[1994/07/18  19:58:40  salamone]
# 
# Revision 1.1.2.5  1994/06/10  20:01:36  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:07  devsrc]
# 
# Revision 1.1.2.4  1994/05/18  20:39:08  salamone
# 	CR10621 - dcecp "acl show", "acl check", and "acl permissions" should
# 	          display the appropriate info if unauthenticated.
# 	[1994/05/13  22:30:00  salamone]
# 
# Revision 1.1.2.3  1994/05/11  19:30:59  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:14:47  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:00:54  melman
# 	updating to new API path
# 	[1994/05/06  15:00:18  melman]
# 
# Revision 1.1.2.1  1994/03/24  16:28:51  salamone
# 	Used "test" proc instead of "dcp_acl_check_test" proc
# 	[1994/03/22  18:24:21  salamone]
# 
# 	Add to Setup & Cleanup steps.
# 	Add last test case.
# 	[1994/03/11  16:34:31  salamone]
# 
# 	Initial version.
# 	[1994/03/09  21:47:32  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc acl_check_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}

    # Load variables for string binding tests
    dcp_acl_binding_vars
}

proc acl_check_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc acl_check_013_P {} {

    # Identify the test
    tet_infoline acl_check_013_P: Display ACL permissions of invoker for 1 ACL object, which
    tet_infoline acl_check_013_P: is not enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT}] != 0} {return;}

    # Run the test
    test { acl check /.:/hosts 
	   dcp_acl_find_multi_entries "/.:/hosts" {} {} [list "user $DCP_CLIENT [acl check /.:/hosts]"]
	   } compare_strings {found}

    # Cleanup steps
}

proc acl_check_014_P {} {

    # Identify the test
    tet_infoline acl_check_014_P: Display ACL permissions of invoker for 1 ACL object, which
    tet_infoline acl_check_014_P: is enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT}] != 0} {return;}

    # Run the test
    test { acl check {{/.:/hosts}}
	   dcp_acl_find_multi_entries "/.:/hosts" {} {} [list "user $DCP_CLIENT [acl check /.:/hosts]"]
	   } compare_strings {found}

    # Cleanup steps
}

proc acl_check_015_P {} {

    # Identify the test
    tet_infoline acl_check_015_P: Display ACL permissions of invoker for more than 1 ACL object,
    tet_infoline acl_check_015_P: which are not individually enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT}] != 0} {return;}

    # Run the test
    test { acl check {/.:/hosts /.:/sec/principal} 
	   dcp_acl_find_multi_entries "/.:/hosts" {} {} [list "user $DCP_CLIENT [acl check /.:/hosts]"]
	   } compare_strings {found}

    test { dcp_acl_find_multi_entries "/.:/sec/principal" {} {} [list "user $DCP_CLIENT [acl check /.:/sec/principal]"]
	   } compare_strings {found}

    # Cleanup steps
}

proc acl_check_016_P {} {

    # Identify the test
    tet_infoline acl_check_016_P: Display ACL permissions of invoker for more than 1 ACL object,
    tet_infoline acl_check_016_P: which are individually enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT}] != 0} {return;}

    # Run the test
    test { acl check {{/.:/hosts} {/.:/sec/principal}} 
	   dcp_acl_find_multi_entries "/.:/hosts" {} {} [list "user $DCP_CLIENT [acl check /.:/hosts]"]
	   } compare_strings {found}

    test { dcp_acl_find_multi_entries "/.:/sec/principal" {} {} [list "user $DCP_CLIENT [acl check /.:/sec/principal]"]
	   } compare_strings {found}

    # Cleanup steps
}

proc acl_check_017_P {} {

    # Identify the test
    tet_infoline acl_check_017_P: Display ACL permissions of invoker for 1 ACL object.
    tet_infoline acl_check_017_P: This test operates on the namespace entry ACL of the 
    tet_infoline acl_check_017_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT}] != 0} {return;}

    # Run the test
    test { acl check /.:/hosts -entry
	   dcp_acl_find_multi_entries "/.:/hosts" {entry} {} [list "user $DCP_CLIENT [acl check /.:/hosts -entry]"]
	   } compare_strings {found}

    # Cleanup steps
}

proc acl_check_018_P {} {

    # Identify the test
    tet_infoline acl_check_018_P: Display ACL permissions of invoker for more than 1 ACL object.
    tet_infoline acl_check_018_P: This test ensures that the namespace entry ACL for all the 
    tet_infoline acl_check_018_P: specified objects are operated on.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT}] != 0} {return;}

    # Run the test
    test { acl check {/.:/hosts /.:/sec/principal} -entry
	   dcp_acl_find_multi_entries "/.:/hosts" {entry} {} [list "user $DCP_CLIENT [acl check /.:/hosts -entry]"]
	   } compare_strings {found}

    test { dcp_acl_find_multi_entries "/.:/sec/principal" {entry} {} [list "user $DCP_CLIENT [acl check /.:/sec/principal -entry]"]
	   } compare_strings {found}

    # Cleanup steps
}

proc acl_check_019_P {} {

    # Identify the test
    tet_infoline acl_check_019_P: Display ACL permissions of invoker for 1 ACL object
    tet_infoline acl_check_019_P: specified as a string binding with an object UUID in 
    tet_infoline acl_check_019_P: string syntax.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR DCP_CLIENT}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test { acl check "$OBJECT_UUID@$PROTSEQ:$NETADDR principal/$name"
	   dcp_acl_find_multi_entries "/.:/sec/principal/$name " {} {} [list "user $DCP_CLIENT [acl check /.:/sec/principal/$name]"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_check_020_P {} {

    # Identify the test
    tet_infoline acl_check_020_P: Display ACL permissions of invoker for 1 ACL object
    tet_infoline acl_check_020_P: specified as a string binding with an object UUID in
    tet_infoline acl_check_020_P: TCL syntax.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR DCP_CLIENT}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test { acl check "$OBJECT_UUID $PROTSEQ $NETADDR principal/$name"
	   dcp_acl_find_multi_entries "/.:/sec/principal/$name " {} {} [list "user $DCP_CLIENT [acl check /.:/sec/principal/$name]"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_check_021_P {} {

    # Identify the test
    tet_infoline acl_check_021_P: Display ACL permissions of invoker for 1 ACL object
    tet_infoline acl_check_021_P: specified as a string binding with an endpoint in
    tet_infoline acl_check_021_P: string syntax.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR ENDPOINT DCP_CLIENT}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test { acl check "$OBJECT_UUID@$PROTSEQ:$NETADDR\[$ENDPOINT\] principal/$name"
	   dcp_acl_find_multi_entries "/.:/sec/principal/$name " {} {} [list "user $DCP_CLIENT [acl check /.:/sec/principal/$name]"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_check_022_P {} {

    # Identify the test
    tet_infoline acl_check_022_P: Display ACL permissions of invoker for 1 ACL object
    tet_infoline acl_check_022_P: specified as a string binding with an endpoint in
    tet_infoline acl_check_022_P: TCL syntax.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR ENDPOINT DCP_CLIENT}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test { acl check "$OBJECT_UUID $PROTSEQ $NETADDR $ENDPOINT principal/$name"
	   dcp_acl_find_multi_entries "/.:/sec/principal/$name " {} {} [list "user $DCP_CLIENT [acl check /.:/sec/principal/$name]"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_check_023_P {} {

    # Identify the test
    tet_infoline acl_check_023_P: Display ACL permissions of invoker for 1 ACL object with
    tet_infoline acl_check_023_P: no authentication.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test { acl check /.:/subsys
	   dcp_acl_find_multi_entries "/.:/subsys" {} {} [list "unauthenticated [acl check /.:/subsys]"]
	  } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}

}

proc acl_check_024_P {} {
    # Identify the test
    tet_infoline acl_check_024_P: Display ACL permissions of invoker for 1 ACL object
    tet_infoline acl_check_024_P: using one of two ACL managers.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0/$prin1}] != 0} {return;}

    # Run the test
    test { acl check /.:/sec/principal/$prin0 -type "principal"
	   dcp_acl_find_multi_entries "/.:/sec/principal/$prin0" {} {principal} [list "user $DCP_CLIENT [acl check /.:/sec/principal/$prin0 -type "principal"]"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {principal delete $prin0/$prin1}] != 0} {return;}
    if {[dcp_wrap {principal delete $prin0}] != 0} {return;}
}

proc acl_check_025_P {} {
    # Identify the test
    tet_infoline acl_check_025_P: Display ACL permissions of invoker for 1 ACL object
    tet_infoline acl_check_025_P: using one of two ACL managers.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0/$prin1}] != 0} {return;}

    # Run the test
    test { acl check /.:/sec/principal/$prin0 -type "secdirectory"
	   dcp_acl_find_multi_entries "/.:/sec/principal/$prin0" {} {secdirectory} [list "user $DCP_CLIENT [acl check /.:/sec/principal/$prin0 -type "secdirectory"]"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {principal delete $prin0/$prin1}] != 0} {return;}
    if {[dcp_wrap {principal delete $prin0}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13"
set ic1 "acl_check_013_P"
set ic2 "acl_check_014_P"
set ic3 "acl_check_015_P"
set ic4 "acl_check_016_P"
set ic5 "acl_check_017_P"
set ic6 "acl_check_018_P"
set ic7 "acl_check_019_P"
set ic8 "acl_check_020_P"
set ic9 "acl_check_021_P"
set ic10 "acl_check_022_P"
set ic11 "acl_check_023_P"
set ic12 "acl_check_024_P"
set ic13 "acl_check_025_P"
set tet_startup acl_check_startup
set tet_cleanup acl_check_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
