#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: acl_show_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:09:38  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:27:58  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:08  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/05/23  18:56 UTC  truitt
# 	Merge test addition.
# 
# 	HP revision /main/truitt_dcecp_chfts30/1  1995/05/23  18:54 UTC  truitt
# 	CHFts15282: Add test for unauthenticated work.
# 	[1995/12/11  15:51:05  root]
# 
# Revision 1.1.2.2  1994/10/01  01:33:10  rousseau
# 	Dynamically generate string bindings.
# 	[1994/10/01  01:28:54  rousseau]
# 
# Revision 1.1.2.1  1994/09/28  21:37:29  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:21  melman]
# 
# Revision 1.1.2.9  1994/09/26  21:04:20  salamone
# 	CR9938 - Change ACL mgr names and return UNINITIATED for those tests that aren't configured.
# 	[1994/09/26  21:02:26  salamone]
# 
# Revision 1.1.2.8  1994/08/25  15:33:14  salamone
# 	CR11635 - Handle 1 string binding specification for an ACL object.
# 	[1994/08/25  15:30:25  salamone]
# 
# Revision 1.1.2.7  1994/07/26  18:58:47  salamone
# 	CR10045 - Provide string binding support for ACL objects.
# 	[1994/07/26  18:49:29  salamone]
# 
# Revision 1.1.2.6  1994/07/18  20:05:08  salamone
# 	Add tests for polymorphic object support
# 	[1994/07/18  20:02:37  salamone]
# 
# Revision 1.1.2.5  1994/06/10  20:02:20  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:28  devsrc]
# 
# Revision 1.1.2.4  1994/05/18  20:39:07  salamone
# 	CR10621-dcecp "acl show", "acl check", and "acl permissions" should
# 	        display the appropriate information if unauthenticated.
# 	[1994/05/13  22:34:31  salamone]
# 
# Revision 1.1.2.3  1994/05/11  19:34:22  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:15:33  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:01:08  melman
# 	updating to new API path
# 	[1994/05/06  15:00:37  melman]
# 
# Revision 1.1.2.1  1994/03/24  16:28:59  salamone
# 	Add to Setup & Cleanup steps.
# 	Add last test case.
# 	[1994/03/11  16:34:57  salamone]
# 
# 	Initial version.
# 	[1994/03/08  22:34:20  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc acl_show_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}

    # Load variables for string binding tests
    dcp_acl_binding_vars
}

proc acl_show_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc acl_show_014_P {} {
    # Identify the test
    tet_infoline acl_show_014_P: Display all ACL entries for 1 ACL object, which
    tet_infoline acl_show_014_P: is not enclosed in braces.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl show /.:/hosts}

    # Cleanup steps
}

proc acl_show_015_P {} {
    # Identify the test
    tet_infoline acl_show_015_P: Display the value of the default cell for 1 ACL object.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl show /.:/hosts -cell}

    # Cleanup steps
}

proc acl_show_016_P {} {
    # Identify the test
    tet_infoline acl_show_016_P: Display all ACL entries for 1 ACL object, which
    tet_infoline acl_show_016_P: is enclosed in braces.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl show {/.:/hosts}}

    # Cleanup steps
}

proc acl_show_017_P {} {
    # Identify the test
    tet_infoline acl_show_017_P: Display all ACL entries for more than 1 ACL object,
    tet_infoline acl_show_017_P: which are not individually enclosed in braces.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl show {/.:/hosts /.:/sec/principal}}

    # Cleanup steps
}

proc acl_show_018_P {} {
    # Identify the test
    tet_infoline acl_show_018_P: Display the value of the default cell for more than 
    tet_infoline acl_show_018_P: 1 ACL object.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl show {/.:/hosts /.:/sec/principal} -cell}

    # Cleanup steps
}

proc acl_show_019_P {} {
    # Identify the test
    tet_infoline acl_show_019_P: Display all ACL entries for more than 1 ACL object,
    tet_infoline acl_show_019_P: which are individually enclosed in braces.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl show {{/.:/hosts} {/.:/sec/principal}}}

    # Cleanup steps
}

proc acl_show_020_P {} {
    # Identify the test
    tet_infoline acl_show_020_P: Display all ACL entries for 1 ACL object.
    tet_infoline acl_show_020_P: This test operates on the namespace entry ACL of the 
    tet_infoline acl_show_020_P: specified object.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl show /.:/hosts -entry}

    # Cleanup steps
}

proc acl_show_021_P {} {
    # Identify the test
    tet_infoline acl_show_021_P: Display the value of the default cell for 1 ACL object.
    tet_infoline acl_show_021_P: This test operates on the namespace entry ACL of the 
    tet_infoline acl_show_021_P: specified object.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl show /.:/hosts -entry -cell}

    # Cleanup steps
}

proc acl_show_022_P {} {
    # Identify the test
    tet_infoline acl_show_022_P: Display all ACL entries for more than 1 ACL object.
    tet_infoline acl_show_022_P: This test ensures that the namespace entry ACL for all the 
    tet_infoline acl_show_022_P: specified objects are operated on.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl show {/.:/hosts /.:/sec/principal} -entry}

    # Cleanup steps
}

proc acl_show_023_P {} {
    # Identify the test
    tet_infoline acl_show_023_P: Display the value of the default cell for more than 1 ACL object.
    tet_infoline acl_show_023_P: This test ensures that the namespace entry ACL for all the 
    tet_infoline acl_show_023_P: specified objects are operated on.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl show {/.:/hosts /.:/sec/principal} -entry -cell}

    # Cleanup steps
}

proc acl_show_024_P {} {
    # Identify the test
    tet_infoline acl_show_024_P: Display all ACL entries for 1 ACL object.
    tet_infoline acl_show_024_P: This test operates on initial container ACL of the 
    tet_infoline acl_show_024_P: specified object.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl show /.:/hosts -ic}

    # Cleanup steps
}

proc acl_show_025_P {} {
    # Identify the test
    tet_infoline acl_show_025_P: Display the value of the default cell for 1 ACL object.
    tet_infoline acl_show_025_P: This test operates on initial container ACL of the 
    tet_infoline acl_show_025_P: specified object.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl show /.:/hosts -ic -cell}

    # Cleanup steps
}

proc acl_show_026_P {} {
    # Identify the test
    tet_infoline acl_show_026_P: Display all ACL entries for more than 1 ACL object.
    tet_infoline acl_show_026_P: This test ensures that the initial container ACL for all the 
    tet_infoline acl_show_026_P: specified objects are operated on.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl show {/.:/hosts /.:/sec/principal} -ic}

    # Cleanup steps
}

proc acl_show_027_P {} {
    # Identify the test
    tet_infoline acl_show_027_P: Display the value of the default cell for more than 1 ACL object.
    tet_infoline acl_show_027_P: This test ensures that the initial container ACL for all the 
    tet_infoline acl_show_027_P: specified objects are operated on.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl show {/.:/hosts /.:/sec/principal} -ic -cell}

    # Cleanup steps
}

proc acl_show_028_P {} {
    # Identify the test
    tet_infoline acl_show_028_P: Display all ACL entries for 1 ACL object.
    tet_infoline acl_show_028_P: This test operates on initial object ACL of the 
    tet_infoline acl_show_028_P: specified object.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl show /.:/hosts -io}

    # Cleanup steps
}

proc acl_show_029_P {} {
    # Identify the test
    tet_infoline acl_show_029_P: Display the value of the default cell for 1 ACL object.
    tet_infoline acl_show_029_P: This test operates on initial object ACL of the 
    tet_infoline acl_show_029_P: specified object.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl show /.:/hosts -io -cell}

    # Cleanup steps
}

proc acl_show_030_P {} {
    # Identify the test
    tet_infoline acl_show_030_P: Display all ACL entries for more than 1 ACL object.
    tet_infoline acl_show_030_P: This test ensures that the initial object ACL for all the 
    tet_infoline acl_show_030_P: specified objects are operated on.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl show {/.:/hosts /.:/sec/principal} -io}

    # Cleanup steps
}

proc acl_show_031_P {} {
    # Identify the test
    tet_infoline acl_show_031_P: Display the value of the default cell for more than 1 ACL object.
    tet_infoline acl_show_031_P: This test ensures that the initial object ACL for all the 
    tet_infoline acl_show_031_P: specified objects are operated on.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl show {/.:/hosts /.:/sec/principal} -io -cell}

    # Cleanup steps
}

proc acl_show_032_P {} {
    # Identify the test
    tet_infoline acl_show_032_P: Display all ACL entries including its effective permissions 
    tet_infoline acl_show_032_P: for the file group class entries for 1 ACL object.

    # Setup steps
    if {[dcp_wrap {global DCP_ACL_HOSTNAME1 DCP_ACL_OBJ_MASK1}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -a mask_obj:rc}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {acl show $DCP_ACL_OBJ_MASK1}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -r mask_obj:rc}] != 0} {return;}
}

proc acl_show_033_P {} {
    # Identify the test
    tet_infoline acl_show_033_P: Display all ACL entries for 1 ACL object
    tet_infoline acl_show_033_P: specified as a string binding with an object UUID in 
    tet_infoline acl_show_033_P: string syntax.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {acl show "$OBJECT_UUID@$PROTSEQ:$NETADDR principal/$name"}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_show_034_P {} {
    # Identify the test
    tet_infoline acl_show_034_P: Display all ACL entries for 1 ACL object
    tet_infoline acl_show_034_P: specified as a string binding with an object UUID in
    tet_infoline acl_show_034_P: TCL syntax.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {acl show "$OBJECT_UUID $PROTSEQ $NETADDR principal/$name"}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_show_035_P {} {
    # Identify the test
    tet_infoline acl_show_035_P: Display all ACL entries for 1 ACL object
    tet_infoline acl_show_035_P: specified as a string binding with an endpoint in
    tet_infoline acl_show_035_P: string syntax.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR ENDPOINT}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {acl show "$OBJECT_UUID@$PROTSEQ:$NETADDR\[$ENDPOINT\] principal/$name"}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_show_036_P {} {
    # Identify the test
    tet_infoline acl_show_036_P: Display all ACL entries for 1 ACL object
    tet_infoline acl_show_036_P: specified as a string binding with an endpoint in
    tet_infoline acl_show_036_P: TCL syntax.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR ENDPOINT}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {acl show "$OBJECT_UUID $PROTSEQ $NETADDR $ENDPOINT principal/$name"}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_show_037_P {} {
    # Identify the test
    tet_infoline acl_show_037_P: Display all ACL entries for 1 ACL object with no
    tet_infoline acl_show_037_P: authentication.

    # Setup steps

    # Run the test
    dcp_acl_wrap_test {acl show /.:/hosts}

    # Cleanup steps

}

proc acl_show_038_P {} {
    # Identify the test
    tet_infoline acl_show_038_P: Display all ACL managers for 1 polymorphic object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0/$prin1}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {acl show /.:/sec/principal/$prin0 -managers}

    # Cleanup steps
    if {[dcp_wrap {principal delete $prin0/$prin1}] != 0} {return;}
    if {[dcp_wrap {principal delete $prin0}] != 0} {return;}
}

proc acl_show_039_P {} {
    # Identify the test
    tet_infoline acl_show_039_P: Display all ACL entries for 1 polymorphic object
    tet_infoline acl_show_039_P: specifying the first ACL manager.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0/$prin1}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {acl show /.:/sec/principal/$prin0 -type "principal"}

    # Cleanup steps
    if {[dcp_wrap {principal delete $prin0/$prin1}] != 0} {return;}
    if {[dcp_wrap {principal delete $prin0}] != 0} {return;}
}

proc acl_show_040_P {} {
    # Identify the test
    tet_infoline acl_show_040_P: Display all ACL entries for 1 polymorphic object
    tet_infoline acl_show_040_P: specifying the second ACL manager.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0/$prin1}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {acl show /.:/sec/principal/$prin0 -type "secdirectory"}

    # Cleanup steps
    if {[dcp_wrap {principal delete $prin0/$prin1}] != 0} {return;}
    if {[dcp_wrap {principal delete $prin0}] != 0} {return;}
}

proc acl_show_041_P {} {
    # Identify the test
    tet_infoline acl_show_041_P: Display the default cell for 1 polymorphic object
    tet_infoline acl_show_041_P: specifying the first ACL manager.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0/$prin1}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {acl show /.:/sec/principal/$prin0 -cell -type "principal"}

    # Cleanup steps
    if {[dcp_wrap {principal delete $prin0/$prin1}] != 0} {return;}
    if {[dcp_wrap {principal delete $prin0}] != 0} {return;}
}

proc acl_show_042_P {} {
    # Identify the test
    tet_infoline acl_show_042_P: Display the default cell for 1 polymorphic object
    tet_infoline acl_show_042_P: specifying the second ACL manager.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0/$prin1}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {acl show /.:/sec/principal/$prin0 -cell -type "secdirectory"}

    # Cleanup steps
    if {[dcp_wrap {principal delete $prin0/$prin1}] != 0} {return;}
    if {[dcp_wrap {principal delete $prin0}] != 0} {return;}
}

proc acl_show_043_P {} {
    global DCP_CLIENT DCP_CLIENT_PW
    # Identify the test
    tet_infoline acl_show_043_P: Display entry ACLs for one object unauthenticated.

    # Setup steps
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {acl show /.:/hosts -entry}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 ic25 ic26 ic27 ic28 ic29 ic30 "
set ic1 "acl_show_014_P"
set ic2 "acl_show_015_P"
set ic3 "acl_show_016_P"
set ic4 "acl_show_017_P"
set ic5 "acl_show_018_P"
set ic6 "acl_show_019_P"
set ic7 "acl_show_020_P"
set ic8 "acl_show_021_P"
set ic9 "acl_show_022_P"
set ic10 "acl_show_023_P"
set ic11 "acl_show_024_P"
set ic12 "acl_show_025_P"
set ic13 "acl_show_026_P"
set ic14 "acl_show_027_P"
set ic15 "acl_show_028_P"
set ic16 "acl_show_029_P"
set ic17 "acl_show_030_P"
set ic18 "acl_show_031_P"
set ic19 "acl_show_032_P"
set ic20 "acl_show_033_P"
set ic21 "acl_show_034_P"
set ic22 "acl_show_035_P"
set ic23 "acl_show_036_P"
set ic24 "acl_show_037_P"
set ic25 "acl_show_038_P"
set ic26 "acl_show_039_P"
set ic27 "acl_show_040_P"
set ic28 "acl_show_041_P"
set ic29 "acl_show_042_P"
set ic30 "acl_show_043_P"
set tet_startup acl_show_startup
set tet_cleanup acl_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
