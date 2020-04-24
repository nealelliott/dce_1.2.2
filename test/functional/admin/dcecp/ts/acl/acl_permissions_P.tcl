#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: acl_permissions_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:09:25  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:27:54  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:03  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/11/15  13:05 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/15  13:04 UTC  truitt
# 	Fix parameters being used for the acl tests.
# 	[1995/12/11  15:50:56  root]
# 
# Revision 1.1.2.2  1994/10/01  01:33:04  rousseau
# 	Dynamically generate string bindings.
# 	[1994/10/01  01:28:47  rousseau]
# 
# Revision 1.1.2.1  1994/09/28  21:37:20  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:14  melman]
# 
# Revision 1.1.2.9  1994/09/26  21:04:18  salamone
# 	CR9938 - Change ACL mgr names and return UNINITIATED for those tests that aren't configured.
# 	[1994/09/26  21:02:22  salamone]
# 
# Revision 1.1.2.8  1994/08/25  15:33:09  salamone
# 	CR11635 - Handle 1 string binding specification for an ACL object.
# 	[1994/08/25  15:29:50  salamone]
# 
# Revision 1.1.2.7  1994/07/26  18:58:48  salamone
# 	CR10045 - Provide string binding support for ACL objects.
# 	[1994/07/26  18:49:55  salamone]
# 
# Revision 1.1.2.6  1994/07/18  20:05:02  salamone
# 	Add tests for polymorphic object support
# 	[1994/07/18  20:01:16  salamone]
# 
# Revision 1.1.2.5  1994/06/10  20:01:45  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:24  devsrc]
# 
# Revision 1.1.2.4  1994/05/18  20:39:09  salamone
# 	CR10621 - dcecp "acl show", "acl check", and "acl permissions" should
# 	          display the appropriate information if unauthenticated.
# 	[1994/05/13  22:32:47  salamone]
# 
# Revision 1.1.2.3  1994/05/11  19:34:17  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:15:16  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:01:04  melman
# 	updating to new API path
# 	[1994/05/06  15:00:32  melman]
# 
# Revision 1.1.2.1  1994/03/24  16:28:55  salamone
# 	Add to Setup & Cleanup steps.
# 	Add last test case.
# 	[1994/03/11  16:35:20  salamone]
# 
# 	Initial version.
# 	[1994/03/08  22:13:24  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc acl_permissions_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}

    # Load variables for string binding tests
    dcp_acl_binding_vars
}

proc acl_permissions_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc acl_permissions_013_P {} {

    # Identify the test
    tet_infoline acl_permissions_013_P: Display ACL permissions associated with the specified ACL 
    tet_infoline acl_permissions_013_P: object, which is not enclosed in braces.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl permissions /.:/hosts}

    # Cleanup steps
}

proc acl_permissions_014_P {} {

    # Identify the test
    tet_infoline acl_permissions_014_P: Display ACL permissions associated with the specified 
    tet_infoline acl_permissions_014_P: ACL object, which is enclosed in braces.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl permissions {/.:/hosts}}

    # Cleanup steps
}

proc acl_permissions_015_P {} {

    # Identify the test
    tet_infoline acl_permissions_015_P: Display ACL permissions associated with more than 1 
    tet_infoline acl_permissions_015_P: specified ACL object, which are not individually enclosed 
    tet_infoline acl_permissions_015_P: in braces.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl permissions {/.:/hosts /.:/sec/principal}}

    # Cleanup steps
}

proc acl_permissions_016_P {} {

    # Identify the test
    tet_infoline acl_permissions_016_P: Display ACL permissions associated with more than 1 
    tet_infoline acl_permissions_016_P: specified ACL object, which are individually enclosed 
    tet_infoline acl_permissions_016_P: in braces.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl permissions {{/.:/hosts} {/.:/sec/principal}}}

    # Cleanup steps
}

proc acl_permissions_017_P {} {

    # Identify the test
    tet_infoline acl_permissions_017_P: Display ACL permissions associated with the specified 
    tet_infoline acl_permissions_017_P: ACL object. This test operates on namespace entry ACL 
    tet_infoline acl_permissions_017_P: of the specified object.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl permissions /.:/hosts -entry}

    # Cleanup steps
}

proc acl_permissions_018_P {} {

    # Identify the test
    tet_infoline acl_permissions_018_P: Display ACL permissions associated with more than 1
    tet_infoline acl_permissions_018_P: ACL object.  This test ensures that the namespace entry
    tet_infoline acl_permissions_018_P: ACL for all the specified objects are operated on.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl permissions {/.:/hosts /.:/sec/principal} -entry}

    # Cleanup steps
}

proc acl_permissions_019_P {} {

    # Identify the test
    tet_infoline acl_permissions_019_P: Display ACL permissions associated with the specified 
    tet_infoline acl_permissions_019_P: ACL object. This test operates on initial container ACL 
    tet_infoline acl_permissions_019_P: of the specified object.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl permissions /.:/hosts -ic}

    # Cleanup steps
}

proc acl_permissions_020_P {} {

    # Identify the test
    tet_infoline acl_permissions_020_P: Display ACL permissions associated with more than 1
    tet_infoline acl_permissions_020_P: ACL object.  This test ensures that the initial container
    tet_infoline acl_permissions_020_P: ACL for all the specified objects are operated on.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl permissions {/.:/hosts /.:/sec/principal} -ic}

    # Cleanup steps
}

proc acl_permissions_021_P {} {

    # Identify the test
    tet_infoline acl_permissions_021_P: Display ACL permissions associated with the specified 
    tet_infoline acl_permissions_021_P: ACL object. This test operates on initial object ACL 
    tet_infoline acl_permissions_021_P: of the specified object.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl permissions /.:/hosts -io}

    # Cleanup steps
}

proc acl_permissions_022_P {} {

    # Identify the test
    tet_infoline acl_permissions_022_P: Display ACL permissions associated with more than 1
    tet_infoline acl_permissions_022_P: ACL object. This test ensures that the initial object ACL
    tet_infoline acl_permissions_022_P: for all the specified objects are operated on.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl permissions {/.:/hosts /.:/sec/principal} -io}

    # Cleanup steps
}

proc acl_permissions_023_P {} {

    # Identify the test
    tet_infoline acl_permissions_023_P: Display ACL permissions associated with the 
    tet_infoline acl_permissions_023_P: ACL object specified as a string binding with 
    tet_infoline acl_permissions_023_P: an object UUID in string syntax.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {acl permissions "$OBJECT_UUID@$PROTSEQ:$NETADDR principal/$name"}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_permissions_024_P {} {

    # Identify the test
    tet_infoline acl_permissions_024_P: Display ACL permissions associated with 1 ACL object
    tet_infoline acl_permissions_024_P: specified as a string binding with an object UUID in
    tet_infoline acl_permissions_024_P: TCL syntax.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {acl permissions "$OBJECT_UUID $PROTSEQ $NETADDR principal/$name"}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_permissions_025_P {} {

    # Identify the test
    tet_infoline acl_permissions_025_P: Display ACL permissions associated with 1 ACL object
    tet_infoline acl_permissions_025_P: specified as a string binding with an endpoint in
    tet_infoline acl_permissions_025_P: string syntax.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR ENDPOINT}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {acl permissions "$OBJECT_UUID@$PROTSEQ:$NETADDR\[$ENDPOINT\] principal/$name"}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_permissions_026_P {} {

    # Identify the test
    tet_infoline acl_permissions_026_P: Display ACL permissions associated with 1 ACL object
    tet_infoline acl_permissions_026_P: specified as a string binding with an endpoint in
    tet_infoline acl_permissions_026_P: TCL syntax.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR ENDPOINT}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {acl permissions "$OBJECT_UUID $PROTSEQ $NETADDR $ENDPOINT principal/$name"}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_permissions_027_P {} {

    # Identify the test
    tet_infoline acl_permissions_027_P: Display ACL permissions associated with 1 ACL object with
    tet_infoline acl_permissions_027_P: no authentication.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {acl permissions /.:/hosts -entry}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}

}

proc acl_permissions_028_P {} {
    # Identify the test
    tet_infoline acl_permissions_028_P: Display ACL permissions associated with 1 polymorphic
    tet_infoline acl_permissions_028_P: object specifying first ACL manager.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0/$prin1}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {acl permissions /.:/sec/principal/$prin0 -type "principal"}

    # Cleanup steps
    if {[dcp_wrap {principal delete $prin0/$prin1}] != 0} {return;}
    if {[dcp_wrap {principal delete $prin0}] != 0} {return;}
}

proc acl_permissions_029_P {} {
    # Identify the test
    tet_infoline acl_permissions_029_P: Display ACL permissions associated with 1 polymorphic
    tet_infoline acl_permissions_029_P: object specifying second ACL manager.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0/$prin1}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test { acl permissions /.:/sec/principal/$prin0 -type "secdirectory"}

    # Cleanup steps
    if {[dcp_wrap {principal delete $prin0/$prin1}] != 0} {return;}
    if {[dcp_wrap {principal delete $prin0}] != 0} {return;}
}

set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 "
set ic1 "acl_permissions_013_P"
set ic2 "acl_permissions_014_P"
set ic3 "acl_permissions_015_P"
set ic4 "acl_permissions_016_P"
set ic5 "acl_permissions_017_P"
set ic6 "acl_permissions_018_P"
set ic7 "acl_permissions_019_P"
set ic8 "acl_permissions_020_P"
set ic9 "acl_permissions_021_P"
set ic10 "acl_permissions_022_P"
set ic11 "acl_permissions_023_P"
set ic12 "acl_permissions_024_P"
set ic13 "acl_permissions_025_P"
set ic14 "acl_permissions_026_P"
set ic15 "acl_permissions_027_P"
set ic16 "acl_permissions_028_P"
set ic17 "acl_permissions_029_P"
set tet_startup acl_permissions_startup
set tet_cleanup acl_permissions_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
