#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: acl_modify_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:09:14  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:27:51  marty]
#
# Revision 1.1.4.2  1995/12/13  15:23:58  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/06/06  18:28 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts33/1  1995/06/06  18:27 UTC  truitt
# 	CHFts15424: Change tests to reflect new dced acl behavior.
# 	[1995/12/11  15:50:50  root]
# 
# Revision 1.1.2.3  1994/10/04  21:47:36  salamone
# 	CR12480 - Created directories in wrong cell
# 	[1994/10/04  21:39:51  salamone]
# 
# Revision 1.1.2.2  1994/10/01  01:33:00  rousseau
# 	Dynamically generate string bindings.
# 	[1994/10/01  01:28:41  rousseau]
# 
# Revision 1.1.2.1  1994/09/28  21:37:13  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:10  melman]
# 
# Revision 1.1.2.13  1994/09/27  17:39:39  salamone
# 	CR12359 - Return "UNSUPPORTED" for extended ACL entry type tests and for
# 	          initial container ACLs not supporting user_obj and group_obj ACL
# 		  entry types.
# 	[1994/09/27  17:38:32  salamone]
# 
# Revision 1.1.2.12  1994/09/26  21:04:15  salamone
# 	CR9938 - Change ACL mgr names and return UNINITIATED for those tests that aren't configured.
# 	[1994/09/26  21:02:19  salamone]
# 
# Revision 1.1.2.11  1994/09/15  21:31:40  salamone
# 	Updated all "intercell" tests relating to creating and deleting
# 	principals and groups in another cell. Had to perform "logins" to
# 	the other cell and logouts.
# 	[1994/09/15  21:30:53  salamone]
# 
# Revision 1.1.2.10  1994/08/25  15:33:06  salamone
# 	CR11635 - Handle 1 string binding specification for an ACL object.
# 	[1994/08/25  15:29:31  salamone]
# 
# Revision 1.1.2.9  1994/07/26  18:58:43  salamone
# 	CR10045 - Provide string binding support for ACL objects.
# 	[1994/07/26  18:49:10  salamone]
# 
# Revision 1.1.2.8  1994/07/18  20:04:57  salamone
# 	Add tests for polymorphic object support
# 	[1994/07/18  20:00:33  salamone]
# 
# Revision 1.1.2.7  1994/06/10  20:01:41  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:19  devsrc]
# 
# Revision 1.1.2.6  1994/05/11  19:34:10  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:15:05  rousseau]
# 
# Revision 1.1.2.5  1994/05/06  17:35:47  salamone
# 	CR10211: -mask calc is ignored; Fixed test 231.
# 	[1994/05/06  17:35:01  salamone]
# 
# Revision 1.1.2.4  1994/05/06  15:01:00  melman
# 	updating to new API path
# 	[1994/05/06  15:00:26  melman]
# 
# Revision 1.1.2.3  1994/04/12  14:21:41  salamone
# 	Fixed 3 tests comparing ACL entries.
# 	[1994/04/12  13:55:21  salamone]
# 
# Revision 1.1.2.2  1994/03/25  22:31:24  salamone
# 	CR10206 - Provide support for 1 ACL entry specified in TCL syntax.
# 	[1994/03/25  21:50:40  salamone]
# 
# Revision 1.1.2.1  1994/03/24  16:51:32  salamone
# 	Initial version.
# 	[1994/03/24  16:49:11  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc acl_modify_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}

    # Load variables for string binding tests
    dcp_acl_binding_vars
}

proc acl_modify_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc acl_modify_043_P {} {
    # Identify the test
    tet_infoline acl_modify_043_P: Purge ineffective permissions for the specified ACL
    tet_infoline acl_modify_043_P: object that does not support the mask_obj ACL entry
    tet_infoline acl_modify_043_P: type. The -purge option is ignored.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -purge -a user:$name:r" {} {user} "$name" {r------}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_044_P {} {
    # Identify the test
    tet_infoline acl_modify_044_P: Check that a mask re-calculation occurs after all
    tet_infoline acl_modify_044_P: modifications are made on the specified ACL object that
    tet_infoline acl_modify_044_P: supports the mask_obj ACL entry type. The mask_obj ACL
    tet_infoline acl_modify_044_P: entry type does not exist but will be created.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_MASK1}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test {acl modify $DCP_ACL_OBJ_MASK1 -mask calc -a user:$name:rwc
        dcp_acl_find_multi_entries "$DCP_ACL_OBJ_MASK1" {} {} "[list "mask_obj rwc" "user $name rwc"]"
    } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -r user:$name}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -r mask_obj}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_045_P {} {
    # Identify the test
    tet_infoline acl_modify_045_P: Purge ineffective permissions for the specified ACL 
    tet_infoline acl_modify_045_P: object that supports the mask_obj ACL entry type
    tet_infoline acl_modify_045_P: but it doesn't exist. The -purge option is ignored.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_MASK1}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $DCP_ACL_OBJ_MASK1 -purge -a user:$name:r" {} {user} "$name" {r--}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -r user:$name}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_046_P {} {
    # Identify the test
    tet_infoline acl_modify_046_P: Deny access to all unauthenticated users to a 
    tet_infoline acl_modify_046_P: specified ACL object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -r unauthenticated}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -a unauthenticated:-" {} {unauthenticated} {} {-------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_047_P {} {
    # Identify the test
    tet_infoline acl_modify_047_P: Deny a principal all access to a specified ACL object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -a user:$name:-" {} {user} "$name" {-------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_048_P {} {
    # Identify the test
    tet_infoline acl_modify_048_P: Add an "unauthenticated" ACL entry to the specified ACL object,
    tet_infoline acl_modify_048_P: which is not enclosed in braces.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -r unauthenticated}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -a unauthenticated:r" {} {unauthenticated} {} {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_049_P {} {
    # Identify the test
    tet_infoline acl_modify_049_P: Add a "user_obj" ACL entry to the specified ACL object,
    tet_infoline acl_modify_049_P: which is not enclosed in braces.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify /.:/sec/principal/$name -r user_obj}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify /.:/sec/principal/$name -a user_obj:r---f--ug" {} {user_obj} {} {r---f--ug}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_050_P {} {
    # Identify the test
    tet_infoline acl_modify_050_P: Add a "user" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_050_P: is not enclosed in braces.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -a user:$name:r" {} {user} "$name" {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_051_P {} {
    # Identify the test
    tet_infoline acl_modify_051_P: Add a "foreign_user" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_051_P: is not enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -a foreign_user:$DCP_CELLNAME_TWO/$name:r" {} {foreign_user} "$DCP_CELLNAME_TWO/$name" {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_052_P {} {
    # Identify the test
    tet_infoline acl_modify_052_P: Add a "group_obj" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_052_P: is not enclosed in braces.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {group create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify /.:/sec/group/$name -r group_obj}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify /.:/sec/group/$name -a group_obj:r-t-----" {} {group_obj} {} {r-t-----}

    # Cleanup steps
    if {[dcp_wrap {group delete $name}] != 0} {return;}
}

proc acl_modify_053_P {} {
    # Identify the test
    tet_infoline acl_modify_053_P: Add a "group" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_053_P: is not enclosed in braces.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {group create $name}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -a group:$name:r" {} {group} "$name" {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {group delete $name}] != 0} {return;}
}

proc acl_modify_054_P {} {
    # Identify the test
    tet_infoline acl_modify_054_P: Add a "foreign_group" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_054_P: is not enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -a foreign_group:$DCP_CELLNAME_TWO/$name:r" {} {foreign_group} "$DCP_CELLNAME_TWO/$name" {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_055_P {} {
    # Identify the test
    tet_infoline acl_modify_055_P: Add a "other_obj" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_055_P: is not enclosed in braces.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -a other_obj:rw" {} {other_obj} {} {rw-----}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_056_P {} {
    # Identify the test
    tet_infoline acl_modify_056_P: Add a "foreign_other" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_056_P: is not enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -a foreign_other:$DCP_CELLNAME_TWO:r" {} {foreign_other} "$DCP_CELLNAME_TWO" {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_057_P {} {
    # Identify the test
    tet_infoline acl_modify_057_P: Add a "any_other" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_057_P: is not enclosed in braces.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -r any_other}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -a any_other:r" {} {any_other} {} {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_058_P {} {
    # Identify the test
    tet_infoline acl_modify_058_P: Add a "unauthenticated" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_058_P: is not enclosed in braces.
    tet_infoline acl_modify_058_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_058_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -ic -r unauthenticated}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -ic -a unauthenticated:r" {ic} {unauthenticated} {} {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_059_P {} {
    # Identify the test
    tet_infoline acl_modify_059_P: Add a "user_obj" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_059_P: is not enclosed in braces.
    tet_infoline acl_modify_059_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_059_P: specified object.

    # Setup steps
    tet_result UNSUPPORTED;return

    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $name -ic -a user_obj:rc" {ic} {user_obj} {} {rc-----}

    # Cleanup steps
}

proc acl_modify_060_P {} {
    # Identify the test
    tet_infoline acl_modify_060_P: Add a "user" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_060_P: is not enclosed in braces.
    tet_infoline acl_modify_060_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_060_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -ic -a user:$name:r" {ic} {user} "$name" {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_061_P {} {
    # Identify the test
    tet_infoline acl_modify_061_P: Add a "foreign_user" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_061_P: is not enclosed in braces.
    tet_infoline acl_modify_061_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_061_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -ic -a foreign_user:$DCP_CELLNAME_TWO/$name:r" {ic} {foreign_user} "$DCP_CELLNAME_TWO/$name" {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_062_P {} {
    # Identify the test
    tet_infoline acl_modify_062_P: Add a "group_obj" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_062_P: is not enclosed in braces.
    tet_infoline acl_modify_062_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_062_P: specified object.

    # Setup steps
    tet_result UNSUPPORTED;return

    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $name -ic -a group_obj:rc" {ic} {group_obj} {} {rc-----}

    # Cleanup steps
}

proc acl_modify_063_P {} {
    # Identify the test
    tet_infoline acl_modify_063_P: Add a "group" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_063_P: is not enclosed in braces.
    tet_infoline acl_modify_063_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_063_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {group create $name}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -ic -a group:$name:r" {ic} {group} "$name" {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {group delete $name}] != 0} {return;}
}

proc acl_modify_064_P {} {
    # Identify the test
    tet_infoline acl_modify_064_P: Add a "foreign_group" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_064_P: is not enclosed in braces.
    tet_infoline acl_modify_064_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_064_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -ic -a foreign_group:$DCP_CELLNAME_TWO/$name:r" {ic} {foreign_group} "$DCP_CELLNAME_TWO/$name" {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_065_P {} {
    # Identify the test
    tet_infoline acl_modify_065_P: Add a "other_obj" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_065_P: is not enclosed in braces.
    tet_infoline acl_modify_065_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_065_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -ic -a other_obj:rc" {ic} {other_obj} {} {r---c--}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_066_P {} {
    # Identify the test
    tet_infoline acl_modify_066_P: Add a "foreign_other" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_066_P: is not enclosed in braces.
    tet_infoline acl_modify_066_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_066_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -ic -a foreign_other:$DCP_CELLNAME_TWO:r" {ic} {foreign_other} "$DCP_CELLNAME_TWO" {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_067_P {} {
    # Identify the test
    tet_infoline acl_modify_067_P: Add a "any_other" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_067_P: is not enclosed in braces.
    tet_infoline acl_modify_067_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_067_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -ic -r any_other}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -ic -a any_other:r" {ic} {any_other} {} {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_068_P {} {
    # Identify the test
    tet_infoline acl_modify_068_P: Add a "unauthenticated" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_068_P: is not enclosed in braces.
    tet_infoline acl_modify_068_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_068_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -io -r unauthenticated}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -io -a unauthenticated:r" {io} {unauthenticated} {} {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_069_P {} {
    # Identify the test
    tet_infoline acl_modify_069_P: Add a "user_obj" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_069_P: is not enclosed in braces.
    tet_infoline acl_modify_069_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_069_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name/$name}] != 0} {return;}
    if {[dcp_wrap {acl modify /.:/sec/principal/$name -io -r user_obj}] != 0} {return;}

    # Run the test
    test {acl modify /.:/sec/principal/$name -io -a user_obj:rc
        dcp_acl_find_multi_entries "/.:/sec/principal/$name" {io} {} {{user_obj rc-------}}
    } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name/$name}] != 0} {return;}
}

proc acl_modify_070_P {} {
    # Identify the test
    tet_infoline acl_modify_070_P: Add a "user" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_070_P: is not enclosed in braces.
    tet_infoline acl_modify_070_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_070_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -io -a user:$name:r" {io} {user} "$name" {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_071_P {} {
    # Identify the test
    tet_infoline acl_modify_071_P: Add a "foreign_user" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_071_P: is not enclosed in braces.
    tet_infoline acl_modify_071_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_071_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -io -a foreign_user:$DCP_CELLNAME_TWO/$name:r" {io} {foreign_user} "$DCP_CELLNAME_TWO/$name" {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_072_P {} {
    # Identify the test
    tet_infoline acl_modify_072_P: Add a "group_obj" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_072_P: is not enclosed in braces.
    tet_infoline acl_modify_072_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_072_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {group create $name/$name}] != 0} {return;}
    if {[dcp_wrap {acl modify /.:/sec/group/$name -io -r group_obj}] != 0} {return;}

    # Run the test
    test {acl modify /.:/sec/group/$name -io -a group_obj:rc
        dcp_acl_find_multi_entries "/.:/sec/group/$name" {io} {} {{group_obj rc------}}
    } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {group delete $name/$name}] != 0} {return;}
}

proc acl_modify_073_P {} {
    # Identify the test
    tet_infoline acl_modify_073_P: Add a "group" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_073_P: is not enclosed in braces.
    tet_infoline acl_modify_073_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_073_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {group create $name}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -io -a group:$name:r" {io} {group} "$name" {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {group delete $name}] != 0} {return;}
}

proc acl_modify_074_P {} {
    # Identify the test
    tet_infoline acl_modify_074_P: Add a "foreign_group" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_074_P: is not enclosed in braces.
    tet_infoline acl_modify_074_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_074_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -io -a foreign_group:$DCP_CELLNAME_TWO/$name:r" {io} {foreign_group} "$DCP_CELLNAME_TWO/$name" {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_075_P {} {
    # Identify the test
    tet_infoline acl_modify_075_P: Add a "other_obj" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_075_P: is not enclosed in braces.
    tet_infoline acl_modify_075_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_075_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -io -a other_obj:rc" {io} {other_obj} {} {r---c--}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_076_P {} {
    # Identify the test
    tet_infoline acl_modify_076_P: Add a "foreign_other" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_076_P: is not enclosed in braces.
    tet_infoline acl_modify_076_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_076_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -io -a foreign_other:$DCP_CELLNAME_TWO:r" {io} {foreign_other} "$DCP_CELLNAME_TWO" {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_077_P {} {
    # Identify the test
    tet_infoline acl_modify_077_P: Add a "any_other" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_077_P: is not enclosed in braces.
    tet_infoline acl_modify_077_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_077_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -io -r any_other}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -io -a any_other:r" {io} {any_other} {} {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_078_P {} {
    # Identify the test
    tet_infoline acl_modify_078_P: Add a "unauthenticated" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_078_P: is not enclosed in braces.
    tet_infoline acl_modify_078_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_078_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -entry -r unauthenticated}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -entry -a unauthenticated:r" {entry} {unauthenticated} {} {r----}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_079_P {} {
    # Identify the test
    tet_infoline acl_modify_079_P: Add a "user_obj" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_079_P: is not enclosed in braces.
    tet_infoline acl_modify_079_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_079_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify /.:/sec/principal/$name -entry -r user_obj}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify /.:/sec/principal/$name -entry -a user_obj:rc" {entry} {user_obj} {} {rc-------}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_080_P {} {
    # Identify the test
    tet_infoline acl_modify_080_P: Add a "user" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_080_P: is not enclosed in braces.
    tet_infoline acl_modify_080_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_080_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -entry -a user:$name:r" {entry} {user} "$name" {r----}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_081_P {} {
    # Identify the test
    tet_infoline acl_modify_081_P: Add a "foreign_user" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_081_P: is not enclosed in braces.
    tet_infoline acl_modify_081_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_081_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -entry -a foreign_user:$DCP_CELLNAME_TWO/$name:r" {entry} {foreign_user} "$DCP_CELLNAME_TWO/$name" {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_082_P {} {
    # Identify the test
    tet_infoline acl_modify_082_P: Add a "group_obj" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_082_P: is not enclosed in braces.
    tet_infoline acl_modify_082_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_082_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {group create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify /.:/sec/group/$name -entry -r group_obj}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify /.:/sec/group/$name -entry -a group_obj:rc" {entry} {group_obj} {} {rc------}

    # Cleanup steps
    if {[dcp_wrap {group delete $name}] != 0} {return;}
}

proc acl_modify_083_P {} {
    # Identify the test
    tet_infoline acl_modify_083_P: Add a "group" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_083_P: is not enclosed in braces.
    tet_infoline acl_modify_083_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_083_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {group create $name}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -entry -a group:$name:r" {entry} {group} "$name" {r----}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {group delete $name}] != 0} {return;}
}

proc acl_modify_084_P {} {
    # Identify the test
    tet_infoline acl_modify_084_P: Add a "foreign_group" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_084_P: is not enclosed in braces.
    tet_infoline acl_modify_084_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_084_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -entry -a foreign_group:$DCP_CELLNAME_TWO/$name:r" {entry} {foreign_group} "$DCP_CELLNAME_TWO/$name" {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_085_P {} {
    # Identify the test
    tet_infoline acl_modify_085_P: Add a "other_obj" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_085_P: is not enclosed in braces.
    tet_infoline acl_modify_085_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_085_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -entry -a other_obj:rc" {entry} {other_obj} {} {r---c}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_086_P {} {
    # Identify the test
    tet_infoline acl_modify_086_P: Add a "foreign_other" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_086_P: is not enclosed in braces.
    tet_infoline acl_modify_086_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_086_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -entry -a foreign_other:$DCP_CELLNAME_TWO:r" {entry} {foreign_other} "$DCP_CELLNAME_TWO" {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_087_P {} {
    # Identify the test
    tet_infoline acl_modify_087_P: Add a "any_other" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_087_P: is not enclosed in braces.
    tet_infoline acl_modify_087_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_087_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -entry -r any_other}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -entry -a any_other:r" {entry} {any_other} {} {r----}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_088_P {} {
    # Identify the test
    tet_infoline acl_modify_088_P: Change a "unauthenticated" ACL entry on the specified ACL object. 

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -ch unauthenticated:rw-t" {} {unauthenticated} {} {rw-t---}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_089_P {} {
    # Identify the test
    tet_infoline acl_modify_089_P: Change a "user_obj" ACL entry on the specified ACL object. 

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify /.:/sec/principal/$name -ch user_obj:rcfug" {} {user_obj} {} {rc--f--ug}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_090_P {} {
    # Identify the test
    tet_infoline acl_modify_090_P: Change a "user" ACL entry on the specified ACL object. 

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -a user:$name:r}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -ch user:$name:rw" {} {user} "$name" {rw-----}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_091_P {} {
    # Identify the test
    tet_infoline acl_modify_091_P: Change a "foreign_user" ACL entry on the specified ACL object. 

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -a foreign_user:$DCP_CELLNAME_TWO/$name:r}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -ch foreign_user:$DCP_CELLNAME_TWO/$name:rw" {} {foreign_user} "$DCP_CELLNAME_TWO/$name" {rw-----}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_092_P {} {
    # Identify the test
    tet_infoline acl_modify_092_P: Change a "group_obj" ACL entry on the specified ACL object. 

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {group create $name}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify /.:/sec/group/$name -ch group_obj:rct-----" {} {group_obj} {} {rct-----}

    # Cleanup steps
    if {[dcp_wrap {group delete $name}] != 0} {return;}
}

proc acl_modify_093_P {} {
    # Identify the test
    tet_infoline acl_modify_093_P: Change a "group" ACL entry on the specified ACL object. 

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {group create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -a group:$name:r}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -ch group:$name:rw-t" {} {group} "$name" {rw-t---}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {group delete $name}] != 0} {return;}
}

proc acl_modify_094_P {} {
    # Identify the test
    tet_infoline acl_modify_094_P: Change a "foreign_group" ACL entry on the specified ACL object. 

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -a foreign_group:$DCP_CELLNAME_TWO/$name:r}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -ch foreign_group:$DCP_CELLNAME_TWO/$name:crw" {} {foreign_group} "$DCP_CELLNAME_TWO/$name" {rw--c--}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_095_P {} {
    # Identify the test
    tet_infoline acl_modify_095_P: Change a "other_obj" ACL entry on the specified ACL object. 

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -a other_obj:r}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -ch other_obj:rt" {} {other_obj} {} {r--t---}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_096_P {} {
    # Identify the test
    tet_infoline acl_modify_096_P: Change a "foreign_other" ACL entry on the specified ACL object. 

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -a foreign_other:$DCP_CELLNAME_TWO:r}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -ch foreign_other:$DCP_CELLNAME_TWO:rt" {} {foreign_other} "$DCP_CELLNAME_TWO" {r--t---}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_097_P {} {
    # Identify the test
    tet_infoline acl_modify_097_P: Change a "any_other" ACL entry on the specified ACL object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -ch any_other:rw-t" {} {any_other} {} {rw-t---}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_098_P {} {
    # Identify the test
    tet_infoline acl_modify_098_P: Change a "unauthenticated" ACL entry to the specified ACL object.
    tet_infoline acl_modify_098_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_098_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -ic -ch unauthenticated:rw-t" {ic} {unauthenticated} {} {rw-t---}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_099_P {} {
    # Identify the test
    tet_infoline acl_modify_099_P: Change a "user_obj" ACL entry to the specified ACL object. 
    tet_infoline acl_modify_099_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_099_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_DFS_AVAIL}] != 0} {return;}
    if {$DCP_DFS_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dfs_dir0 /.:/fs/ $name}] != 0} {return;}
    if {[dcp_wrap {mkdir $dfs_dir0}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dfs_dir0 -ic -ch user_obj:rwxc--" {ic} {user_obj} {} {rwxc--}

    # Cleanup steps
    if {[dcp_wrap {rmdir $dfs_dir0}] != 0} {return;}
}

proc acl_modify_100_P {} {
    # Identify the test
    tet_infoline acl_modify_100_P: Change a "user" ACL entry to the specified ACL object.
    tet_infoline acl_modify_100_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_100_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -ic -a user:$name:r}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -ic -ch user:$name:crw" {ic} {user} "$name" {rw--c--}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_101_P {} {
    # Identify the test
    tet_infoline acl_modify_101_P: Change a "foreign_user" ACL entry to the specified ACL object.
    tet_infoline acl_modify_101_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_101_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -ic -a foreign_user:$DCP_CELLNAME_TWO/$name:r}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -ic -ch foreign_user:$DCP_CELLNAME_TWO/$name:crw" {ic} {foreign_user} "$DCP_CELLNAME_TWO/$name" {rw--c--}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_102_P {} {
    # Identify the test
    tet_infoline acl_modify_102_P: Change a "group_obj" ACL entry to the specified ACL object.
    tet_infoline acl_modify_102_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_102_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_DFS_AVAIL}] != 0} {return;}
    if {$DCP_DFS_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dfs_dir0 /.:/fs/ $name}] != 0} {return;}
    if {[dcp_wrap {mkdir $dfs_dir0}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dfs_dir0 -ic -ch group_obj:rwxci-" {ic} {group_obj} {} {rwxci-}

    # Cleanup steps
    if {[dcp_wrap {rmdir $dfs_dir0}] != 0} {return;}
}

proc acl_modify_103_P {} {
    # Identify the test
    tet_infoline acl_modify_103_P: Change a "group" ACL entry to the specified ACL object.
    tet_infoline acl_modify_103_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_103_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {group create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -ic -a group:$name:r}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -ic -ch group:$name:crw" {ic} {group} "$name" {rw--c--}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {group delete $name}] != 0} {return;}
}

proc acl_modify_104_P {} {
    # Identify the test
    tet_infoline acl_modify_104_P: Change a "foreign_group" ACL entry to the specified ACL object.
    tet_infoline acl_modify_104_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_104_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -ic -a foreign_group:$DCP_CELLNAME_TWO/$name:r}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -ic -ch foreign_group:$DCP_CELLNAME_TWO/$name:crw" {ic} {foreign_group} "$DCP_CELLNAME_TWO/$name" {rw--c--}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_105_P {} {
    # Identify the test
    tet_infoline acl_modify_105_P: Change a "other_obj" ACL entry to the specified ACL object.
    tet_infoline acl_modify_105_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_105_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -ic -a other_obj:r}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -ic -ch other_obj:crw" {ic} {other_obj} {} {rw--c--}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_106_P {} {
    # Identify the test
    tet_infoline acl_modify_106_P: Change a "foreign_other" ACL entry to the specified ACL object.
    tet_infoline acl_modify_106_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_106_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -ic -a foreign_other:$DCP_CELLNAME_TWO:r}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -ic -ch foreign_other:$DCP_CELLNAME_TWO:crw" {ic} {foreign_other} "$DCP_CELLNAME_TWO" {rw--c--}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_107_P {} {
    # Identify the test
    tet_infoline acl_modify_107_P: Change a "any_other" ACL entry to the specified ACL object.
    tet_infoline acl_modify_107_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_107_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -ic -ch any_other:r" {ic} {any_other} {} {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_108_P {} {
    # Identify the test
    tet_infoline acl_modify_108_P: Change a "unauthenticated" ACL entry to the specified ACL object.
    tet_infoline acl_modify_108_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_108_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -io -ch unauthenticated:r" {io} {unauthenticated} {} {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_109_P {} {
    # Identify the test
    tet_infoline acl_modify_109_P: Change a "user_obj" ACL entry to the specified ACL object. 
    tet_infoline acl_modify_109_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_109_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name/$name}] != 0} {return;}

    # Run the test
    test {acl modify /.:/sec/principal/$name -io -ch user_obj:rc--f--ug
        dcp_acl_find_multi_entries "/.:/sec/principal/$name" {io} {} {{user_obj rc--f--ug}}
    } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name/$name}] != 0} {return;}
}

proc acl_modify_110_P {} {
    # Identify the test
    tet_infoline acl_modify_110_P: Change a "user" ACL entry to the specified ACL object.
    tet_infoline acl_modify_110_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_110_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -io -a user:$name:r}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -io -ch user:$name:crw" {io} {user} "$name" {rw--c--}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_111_P {} {
    # Identify the test
    tet_infoline acl_modify_111_P: Change a "foreign_user" ACL entry to the specified ACL object.
    tet_infoline acl_modify_111_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_111_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -io -a foreign_user:$DCP_CELLNAME_TWO/$name:r}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -io -ch foreign_user:$DCP_CELLNAME_TWO/$name:crw" {io} {foreign_user} "$DCP_CELLNAME_TWO/$name" {rw--c--}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_112_P {} {
    # Identify the test
    tet_infoline acl_modify_112_P: Change a "group_obj" ACL entry to the specified ACL object.
    tet_infoline acl_modify_112_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_112_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {group create $name/$name}] != 0} {return;}

    # Run the test
    test {acl modify /.:/sec/group/$name -io -ch group_obj:rct-----
        dcp_acl_find_multi_entries "/.:/sec/group/$name" {io} {} {{group_obj rct-----}}
    } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {group delete $name/$name}] != 0} {return;}
}

proc acl_modify_113_P {} {
    # Identify the test
    tet_infoline acl_modify_113_P: Change a "group" ACL entry to the specified ACL object.
    tet_infoline acl_modify_113_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_113_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {group create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -io -a group:$name:crw}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -io -ch group:$name:crw" {io} {group} "$name" {rw--c--}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {group delete $name}] != 0} {return;}
}

proc acl_modify_114_P {} {
    # Identify the test
    tet_infoline acl_modify_114_P: Change a "foreign_group" ACL entry to the specified ACL object.
    tet_infoline acl_modify_114_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_114_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -io -a foreign_group:$DCP_CELLNAME_TWO/$name:r}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -io -ch foreign_group:$DCP_CELLNAME_TWO/$name:crw" {io} {foreign_group} "$DCP_CELLNAME_TWO/$name" {rw--c--}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_115_P {} {
    # Identify the test
    tet_infoline acl_modify_115_P: Change a "other_obj" ACL entry to the specified ACL object.
    tet_infoline acl_modify_115_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_115_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -io -a other_obj:r}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -io -ch other_obj:crw" {io} {other_obj} {} {rw--c--}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_116_P {} {
    # Identify the test
    tet_infoline acl_modify_116_P: Change a "foreign_other" ACL entry to the specified ACL object.
    tet_infoline acl_modify_116_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_116_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -io -a foreign_other:$DCP_CELLNAME_TWO:r}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -io -ch foreign_other:$DCP_CELLNAME_TWO:crw" {io} {foreign_other} "$DCP_CELLNAME_TWO" {rw--c--}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_117_P {} {
    # Identify the test
    tet_infoline acl_modify_117_P: Change a "any_other" ACL entry to the specified ACL object.
    tet_infoline acl_modify_117_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_117_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -io -ch any_other:r" {io} {any_other} {} {r------}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_118_P {} {
    # Identify the test
    tet_infoline acl_modify_118_P: Change a "unauthenticated" ACL entry to the specified ACL object.
    tet_infoline acl_modify_118_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_118_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -entry -ch unauthenticated:r" {entry} {unauthenticated} {} {r----}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_119_P {} {
    # Identify the test
    tet_infoline acl_modify_119_P: Change a "user_obj" ACL entry to the specified ACL object. 
    tet_infoline acl_modify_119_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_119_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify /.:/sec/principal/$name -entry -ch user_obj:rc--f--ug" {entry} {user_obj} {} {rc--f--ug}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_120_P {} {
    # Identify the test
    tet_infoline acl_modify_120_P: Change a "user" ACL entry to the specified ACL object.
    tet_infoline acl_modify_120_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_120_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -entry -a user:$name:r}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -entry -ch user:$name:crw" {entry} {user} "$name" {rw--c}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_121_P {} {
    # Identify the test
    tet_infoline acl_modify_121_P: Change a "foreign_user" ACL entry to the specified ACL object.
    tet_infoline acl_modify_121_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_121_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -entry -a foreign_user:$DCP_CELLNAME_TWO/$name:r}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -entry -ch foreign_user:$DCP_CELLNAME_TWO/$name:crw" {entry} {foreign_user} "$DCP_CELLNAME_TWO/$name" {rw--c--}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_122_P {} {
    # Identify the test
    tet_infoline acl_modify_122_P: Change a "group_obj" ACL entry to the specified ACL object.
    tet_infoline acl_modify_122_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_122_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {group create $name}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify /.:/sec/group/$name -entry -ch group_obj:rct-----" {entry} {group_obj} {} {rct-----}

    # Cleanup steps
    if {[dcp_wrap {group delete $name}] != 0} {return;}
}

proc acl_modify_123_P {} {
    # Identify the test
    tet_infoline acl_modify_123_P: Change a "group" ACL entry to the specified ACL object.
    tet_infoline acl_modify_123_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_123_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {group create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -entry -a group:$name:r}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -entry -ch group:$name:crw" {entry} {group} "$name" {rw--c}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {group delete $name}] != 0} {return;}
}

proc acl_modify_124_P {} {
    # Identify the test
    tet_infoline acl_modify_124_P: Change a "foreign_group" ACL entry to the specified ACL object.
    tet_infoline acl_modify_124_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_124_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -entry -a foreign_group:$DCP_CELLNAME_TWO/$name:r}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -entry -ch foreign_group:$DCP_CELLNAME_TWO/$name:crw" {entry} {foreign_group} "$DCP_CELLNAME_TWO/$name" {rw--c--}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_125_P {} {
    # Identify the test
    tet_infoline acl_modify_125_P: Change a "other_obj" ACL entry to the specified ACL object.
    tet_infoline acl_modify_125_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_125_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -entry -a other_obj:r}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -entry -ch other_obj:crw" {entry} {other_obj} {} {rw--c}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_126_P {} {
    # Identify the test
    tet_infoline acl_modify_126_P: Change a "foreign_other" ACL entry to the specified ACL object.
    tet_infoline acl_modify_126_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_126_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -entry -a foreign_other:$DCP_CELLNAME_TWO:r}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -entry -ch foreign_other:$DCP_CELLNAME_TWO:crw" {entry} {foreign_other} "$DCP_CELLNAME_TWO" {rw--c--}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_127_P {} {
    # Identify the test
    tet_infoline acl_modify_127_P: Change a "any_other" ACL entry to the specified ACL object.
    tet_infoline acl_modify_127_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_127_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    dcp_acl_modify_test "acl modify $dir0 -entry -ch any_other:r" {entry} {any_other} {} {r----}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_128_P {} {
    # Identify the test
    tet_infoline acl_modify_128_P: Remove a "unauthenticated" ACL entry from the specified ACL object. 

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -r unauthenticated
        dcp_acl_find_multi_entries "$dir0" {} {} {{unauthenticated r--t---}}
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_129_P {} {
    # Identify the test
    tet_infoline acl_modify_129_P: Remove a "user_obj" ACL entry from the specified ACL object. 

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test {acl modify /.:/sec/principal/$name -r user_obj
        dcp_acl_find_multi_entries "/.:/sec/principal/$name" {} {} {{user_obj r---f--ug}}
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_130_P {} {
    # Identify the test
    tet_infoline acl_modify_130_P: Remove a "user" ACL entry from the specified ACL object. 

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -a user:$name:r}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -r user:$name
        dcp_acl_find_multi_entries "$dir0" {} {} [list "user $name r------"]
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_131_P {} {
    # Identify the test
    tet_infoline acl_modify_131_P: Remove a "foreign_user" ACL entry from the specified ACL object. 

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -a foreign_user:$DCP_CELLNAME_TWO/$name:r}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -r foreign_user:$DCP_CELLNAME_TWO/$name
        dcp_acl_find_multi_entries "$dir0" {} {} [list "foreign_user $DCP_CELLNAME_TWO/$name r------"]
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_132_P {} {
    # Identify the test
    tet_infoline acl_modify_132_P: Remove a "group_obj" ACL entry from the specified ACL object. 

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {group create $name}] != 0} {return;}

    # Run the test
    test {acl modify /.:/sec/group/$name -r group_obj
        dcp_acl_find_multi_entries "/.:/sec/group/$name" {} {} {{group_obj r-t-----}}
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {group delete $name}] != 0} {return;}
}

proc acl_modify_133_P {} {
    # Identify the test
    tet_infoline acl_modify_133_P: Remove a "group" ACL entry from the specified ACL object. 

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {group create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -a group:$name:r}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -r group:$name
        dcp_acl_find_multi_entries "$dir0" {} {} [list "group $name r------"]
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {group delete $name}] != 0} {return;}
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_134_P {} {
    # Identify the test
    tet_infoline acl_modify_134_P: Remove a "foreign_group" ACL entry from the specified ACL object. 

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -a foreign_group:$DCP_CELLNAME_TWO/$name:r}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -r foreign_group:$DCP_CELLNAME_TWO/$name
        dcp_acl_find_multi_entries "$dir0" {} {} [list "foreign_group $DCP_CELLNAME_TWO/$name r------"]
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_135_P {} {
    # Identify the test
    tet_infoline acl_modify_135_P: Remove a "other_obj" ACL entry from the specified ACL object. 

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -a other_obj:r}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -r other_obj
        dcp_acl_find_multi_entries "$dir0" {} {} {{other_obj r------}}
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_136_P {} {
    # Identify the test
    tet_infoline acl_modify_136_P: Remove a "foreign_other" ACL entry from the specified ACL object. 

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -a foreign_other:$DCP_CELLNAME_TWO:r}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -r foreign_other:$DCP_CELLNAME_TWO
        dcp_acl_find_multi_entries "$dir0" {} {} [list "foreign_other $DCP_CELLNAME_TWO r------"]
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_137_P {} {
    # Identify the test
    tet_infoline acl_modify_137_P: Remove a "any_other" ACL entry from the specified ACL object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -r any_other
        dcp_acl_find_multi_entries "$dir0" {} {} {{any_other r------}}
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_138_P {} {
    # Identify the test
    tet_infoline acl_modify_138_P: Remove a "unauthenticated" ACL entry from the specified ACL object.
    tet_infoline acl_modify_138_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_138_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -ic -r unauthenticated 
        dcp_acl_find_multi_entries "$dir0" {ic} {} {{unauthenticated r--t---}}
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_139_P {} {
    # Identify the test
    tet_infoline acl_modify_139_P: Remove a "user_obj" ACL entry from the specified ACL object. 
    tet_infoline acl_modify_139_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_139_P: specified object.

    # Setup steps
    tet_result UNSUPPORTED;return
    if {[dcp_wrap {return "No initial container ACL to support removing user_obj"}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}

    # Run the test
    test {acl modify $name -ic -r user_obj
        dcp_acl_find_multi_entries "$name" {ic} {} {{user_obj XXX}}
    } {compare_strings} {not found}

    # Cleanup steps
}

proc acl_modify_140_P {} {
    # Identify the test
    tet_infoline acl_modify_140_P: Remove a "user" ACL entry from the specified ACL object.
    tet_infoline acl_modify_140_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_140_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -ic -a user:$name:r}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -ic -r user:$name
        dcp_acl_find_multi_entries "$dir0" {ic} {} [list "user $name r------"]
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_141_P {} {
    # Identify the test
    tet_infoline acl_modify_141_P: Remove a "foreign_user" ACL entry from the specified ACL object.
    tet_infoline acl_modify_141_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_141_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -ic -a foreign_user:$DCP_CELLNAME_TWO/$name:r}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -ic -r foreign_user:$DCP_CELLNAME_TWO/$name
        dcp_acl_find_multi_entries "$dir0" {ic} {} [list "foreign_user $DCP_CELLNAME_TWO/$name r------"]
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_142_P {} {
    # Identify the test
    tet_infoline acl_modify_142_P: Remove a "group_obj" ACL entry from the specified ACL object.
    tet_infoline acl_modify_142_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_142_P: specified object.

    # Setup steps
    tet_result UNSUPPORTED;return

    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}

    # Run the test
    test {acl modify $name -ic -r group_obj
        dcp_acl_find_multi_entries "$name" {ic} {} {{group_obj XXX}}
    } {compare_strings} {not found}

    # Cleanup steps
}

proc acl_modify_143_P {} {
    # Identify the test
    tet_infoline acl_modify_143_P: Remove a "group" ACL entry from the specified ACL object.
    tet_infoline acl_modify_143_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_143_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {group create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -ic -a group:$name:r}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -ic -r group:$name
        dcp_acl_find_multi_entries "$dir0" {ic} {} [list "group $name r------"]
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {group delete $name}] != 0} {return;}
}

proc acl_modify_144_P {} {
    # Identify the test
    tet_infoline acl_modify_144_P: Remove a "foreign_group" ACL entry from the specified ACL object.
    tet_infoline acl_modify_144_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_144_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -ic -a foreign_group:$DCP_CELLNAME_TWO/$name:r}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -ic -r foreign_group:$DCP_CELLNAME_TWO/$name
        dcp_acl_find_multi_entries "$dir0" {ic} {} [list "foreign_group $DCP_CELLNAME_TWO/$name r------"]
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_145_P {} {
    # Identify the test
    tet_infoline acl_modify_145_P: Remove a "other_obj" ACL entry from the specified ACL object.
    tet_infoline acl_modify_145_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_145_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -ic -a other_obj:r}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -ic -r other_obj
        dcp_acl_find_multi_entries "$dir0" {ic} {} {{other_obj r------}}
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_146_P {} {
    # Identify the test
    tet_infoline acl_modify_146_P: Remove a "foreign_other" ACL entry from the specified ACL object.
    tet_infoline acl_modify_146_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_146_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -ic -a foreign_other:$DCP_CELLNAME_TWO:r}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -ic -r foreign_other:$DCP_CELLNAME_TWO
        dcp_acl_find_multi_entries "$dir0" {ic} {} [list "foreign_other $DCP_CELLNAME_TWO r------"]
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_147_P {} {
    # Identify the test
    tet_infoline acl_modify_147_P: Remove a "any_other" ACL entry from the specified ACL object.
    tet_infoline acl_modify_147_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_147_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -ic -r any_other
        dcp_acl_find_multi_entries "$dir0" {ic} {} {{any_other r------}}
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_148_P {} {
    # Identify the test
    tet_infoline acl_modify_148_P: Remove a "unauthenticated" ACL entry from the specified ACL object.
    tet_infoline acl_modify_148_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_148_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -io -r unauthenticated
        dcp_acl_find_multi_entries "$dir0" {io} {} {{unauthenticated r------}}
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_149_P {} {
    # Identify the test
    tet_infoline acl_modify_149_P: Remove a "user_obj" ACL entry from the specified ACL object. 
    tet_infoline acl_modify_149_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_149_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name/$name}] != 0} {return;}

    # Run the test
    test {acl modify /.:/sec/principal/$name -io -r user_obj
        dcp_acl_find_multi_entries "/.:/sec/principal/$name" {io} {} {{user_obj r---f--ug}}
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name/$name}] != 0} {return;}
}

proc acl_modify_150_P {} {
    # Identify the test
    tet_infoline acl_modify_150_P: Remove a "user" ACL entry from the specified ACL object.
    tet_infoline acl_modify_150_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_150_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -io -a user:$name:r}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -io -r user:$name
        dcp_acl_find_multi_entries "$dir0" {io} {} [list "user $name r------"]
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_151_P {} {
    # Identify the test
    tet_infoline acl_modify_151_P: Remove a "foreign_user" ACL entry from the specified ACL object.
    tet_infoline acl_modify_151_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_151_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -io -a foreign_user:$DCP_CELLNAME_TWO/$name:r}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -io -r foreign_user:$DCP_CELLNAME_TWO/$name
        dcp_acl_find_multi_entries "$dir0" {io} {} [list "foreign_user $DCP_CELLNAME_TWO/$name r------"]
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_152_P {} {
    # Identify the test
    tet_infoline acl_modify_152_P: Remove a "group_obj" ACL entry from the specified ACL object.
    tet_infoline acl_modify_152_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_152_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {group create $name/$name}] != 0} {return;}

    # Run the test
    test {acl modify /.:/sec/group/$name -io -r group_obj
        dcp_acl_find_multi_entries "/.:/sec/group/$name" {io} {} {{group_obj r-t-----}}
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {group delete $name/$name}] != 0} {return;}
}

proc acl_modify_153_P {} {
    # Identify the test
    tet_infoline acl_modify_153_P: Remove a "group" ACL entry from the specified ACL object.
    tet_infoline acl_modify_153_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_153_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {group create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -io -a group:$name:r}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -io -r group:$name
        dcp_acl_find_multi_entries "$dir0" {io} {} [list "group $name r------"]
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {group delete $name}] != 0} {return;}
}

proc acl_modify_154_P {} {
    # Identify the test
    tet_infoline acl_modify_154_P: Remove a "foreign_group" ACL entry from the specified ACL object.
    tet_infoline acl_modify_154_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_154_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -io -a foreign_group:$DCP_CELLNAME_TWO/$name:r}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -io -r foreign_group:$DCP_CELLNAME_TWO/$name
        dcp_acl_find_multi_entries "$dir0" {io} {} [list "foreign_group $DCP_CELLNAME_TWO/$name r------"]
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_155_P {} {
    # Identify the test
    tet_infoline acl_modify_155_P: Remove a "other_obj" ACL entry from the specified ACL object.
    tet_infoline acl_modify_155_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_155_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -io -a other_obj:r}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -io -r other_obj
        dcp_acl_find_multi_entries "$dir0" {io} {} {{other_obj r------}}
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_156_P {} {
    # Identify the test
    tet_infoline acl_modify_156_P: Remove a "foreign_other" ACL entry from the specified ACL object.
    tet_infoline acl_modify_156_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_156_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -io -a foreign_other:$DCP_CELLNAME_TWO:r}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -io -r foreign_other:$DCP_CELLNAME_TWO
        dcp_acl_find_multi_entries "$dir0" {io} {} [list "foreign_other $DCP_CELLNAME_TWO r------"]
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_157_P {} {
    # Identify the test
    tet_infoline acl_modify_157_P: Remove a "any_other" ACL entry from the specified ACL object.
    tet_infoline acl_modify_157_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_157_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -io -r any_other
        dcp_acl_find_multi_entries "$dir0" {io} {} {{any_other r--t---}}
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_158_P {} {
    # Identify the test
    tet_infoline acl_modify_158_P: Remove a "unauthenticated" ACL entry from the specified ACL object.
    tet_infoline acl_modify_158_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_158_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -entry -r unauthenticated
        dcp_acl_find_multi_entries "$dir0" {entry} {} {{unauthenticated r--t-}}
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_159_P {} {
    # Identify the test
    tet_infoline acl_modify_159_P: Remove a "user_obj" ACL entry from the specified ACL object. 
    tet_infoline acl_modify_159_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_159_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test {acl modify /.:/sec/principal/$name -entry -r user_obj
        dcp_acl_find_multi_entries "/.:/sec/principal/$name" {entry} {} {{user_obj r---f--ug}}
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_160_P {} {
    # Identify the test
    tet_infoline acl_modify_160_P: Remove a "user" ACL entry from the specified ACL object.
    tet_infoline acl_modify_160_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_160_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -entry -a user:$name:r}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -entry -r user:$name
        dcp_acl_find_multi_entries "$dir0" {entry} {} [list "user $name r----"]
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_161_P {} {

    # Identify the test
    tet_infoline acl_modify_161_P: Remove a "foreign_user" ACL entry from the specified ACL object.
    tet_infoline acl_modify_161_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_161_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -entry -a foreign_user:$DCP_CELLNAME_TWO/$name:r}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -entry -r foreign_user:$DCP_CELLNAME_TWO/$name
        dcp_acl_find_multi_entries "$dir0" {entry} {} [list "foreign_user $DCP_CELLNAME_TWO/$name r----"]
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_162_P {} {
    # Identify the test
    tet_infoline acl_modify_162_P: Remove a "group_obj" ACL entry from the specified ACL object.
    tet_infoline acl_modify_162_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_162_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {group create $name}] != 0} {return;}

    # Run the test
    test { acl modify /.:/sec/group/$name -entry -r group_obj
        dcp_acl_find_multi_entries "/.:/sec/group/$name" {entry} {} {{group_obj r-t-----}}
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {group delete $name}] != 0} {return;}
}

proc acl_modify_163_P {} {
    # Identify the test
    tet_infoline acl_modify_163_P: Remove a "group" ACL entry from the specified ACL object.
    tet_infoline acl_modify_163_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_163_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {group create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -entry -a group:$name:r}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -entry -r group:$name
        dcp_acl_find_multi_entries "$dir0" {entry} {} [list "group $name r----"]
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {group delete $name}] != 0} {return;}
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_164_P {} {
    # Identify the test
    tet_infoline acl_modify_164_P: Remove a "foreign_group" ACL entry from the specified ACL object.
    tet_infoline acl_modify_164_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_164_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -entry -a foreign_group:$DCP_CELLNAME_TWO/$name:r}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -entry -r foreign_group:$DCP_CELLNAME_TWO/$name
        dcp_acl_find_multi_entries "$dir0" {entry} {} [list "foreign_group $DCP_CELLNAME_TWO/$name r----"]
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_165_P {} {
    # Identify the test
    tet_infoline acl_modify_165_P: Remove a "other_obj" ACL entry from the specified ACL object.
    tet_infoline acl_modify_165_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_165_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -entry -a other_obj:r}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -entry -r other_obj
        dcp_acl_find_multi_entries "$dir0" {entry} {} {{other_obj r----}}
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_166_P {} {
    # Identify the test
    tet_infoline acl_modify_166_P: Remove a "foreign_other" ACL entry from the specified ACL object.
    tet_infoline acl_modify_166_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_166_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -entry -a foreign_other:$DCP_CELLNAME_TWO:r}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -entry -r foreign_other:$DCP_CELLNAME_TWO
        dcp_acl_find_multi_entries "$dir0" {entry} {} [list "foreign_other $DCP_CELLNAME_TWO r----"]
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_167_P {} {
    # Identify the test
    tet_infoline acl_modify_167_P: Remove a "any_other" ACL entry from the specified ACL object.
    tet_infoline acl_modify_167_P: This test operates on namespace entry ACL of the 
    tet_infoline acl_modify_167_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -entry -r any_other
        dcp_acl_find_multi_entries "$dir0" {entry} {} {{any_other r----}}
    } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_168_P {} {
    # Identify the test
    tet_infoline acl_modify_168_P: Add a "user" ACL entry in TCL syntax to the 
    tet_infoline acl_modify_168_P: specified ACL object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -a [list "user $name r"]
        dcp_acl_find_multi_entries "$dir0" {} {} [list "user $name r------"]
    } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_169_P {} {
    # Identify the test
    tet_infoline acl_modify_169_P: Add a "other_obj" ACL entry in TCL syntax to the 
    tet_infoline acl_modify_169_P: specified ACL object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -a [list "other_obj r"]
        dcp_acl_find_multi_entries "$dir0" {} {} [list "other_obj r------"]
    } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_170_P {} {
    # Identify the test
    tet_infoline acl_modify_170_P: Change a "user" ACL entry in TCL syntax to the 
    tet_infoline acl_modify_170_P: specified ACL object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -a [list "user $name r"]}] != 0} {return;}

    # Run the test
    test {acl modify $dir0 -ch [list "user $name rc"] 
        dcp_acl_find_multi_entries "$dir0" {} {} [list "user $name r---c--"]
    } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_171_P {} {

    # Identify the test
    tet_infoline acl_modify_171_P: Change a "other_obj" ACL entry in TCL syntax to the 
    tet_infoline acl_modify_171_P: specified ACL object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -a other_obj:r}] != 0} {return;}

    # Run the test
    test { acl modify $dir0 -ch [list "other_obj rt"]
	 dcp_acl_find_multi_entries "$dir0" {} {} [list "other_obj r--t---"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_172_P {} {

    # Identify the test
    tet_infoline acl_modify_172_P: Remove a "user" ACL entry in TCL syntax to the 
    tet_infoline acl_modify_172_P: specified ACL object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -a user:$name:r}] != 0} {return;}

    # Run the test
    test { acl modify $dir0 -r "user $name"
	 dcp_acl_find_multi_entries "$dir0" {} {} [list "user $name r-------"]
	   } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_173_P {} {

    # Identify the test
    tet_infoline acl_modify_173_P: Remove a "other_obj" ACL entry in TCL syntax to the 
    tet_infoline acl_modify_173_P: specified ACL object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -a other_obj:r}] != 0} {return;}

    # Run the test
    test { acl modify $dir0 -r {other_obj}
	 dcp_acl_find_multi_entries "$dir0" {} {} {{other_obj r-------}}
	   } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_174_P {} {

    # Identify the test
    tet_infoline acl_modify_174_P: Perform multiple modifications on one ACL object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {append prin2 $name 2}] != 0} {return;}
    if {[dcp_wrap {append prin3 $name 3}] != 0} {return;}
    if {[dcp_wrap {principal create "$prin0 $prin1 $prin2 $prin3"}] != 0} {return;}
    if {[dcp_wrap {acl modify $dir0 -a "user:$prin1:r user:$prin2:r user:$prin3:r"}] != 0} {return;}

    # Run the test
    test { acl modify $dir0 -a "OTHER_OBJ:r USER:$prin0:r" -ch "user:$prin1:rt user:$prin2:rctw" -r user:$prin3
	 dcp_acl_find_multi_entries "$dir0" {} {} [list "other_obj r------" "user $prin0 r------" "user $prin1 r--t---" "user $prin2 rw-tc--"]
	   } {compare_strings} {found}

    test { dcp_acl_find_multi_entries "$dir0" {} {} [list "user $prin3 r------"]
	   } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete "$prin0 $prin1 $prin2 $prin3"}] != 0} {return;}
}

proc acl_modify_175_P {} {

    # Identify the test
    tet_infoline acl_modify_175_P: Perform multiple modifications on more than one ACL object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {append prin2 $name 2}] != 0} {return;}
    if {[dcp_wrap {append prin3 $name 3}] != 0} {return;}
    if {[dcp_wrap {principal create "$prin0 $prin1 $prin2 $prin3"}] != 0} {return;}
    if {[dcp_wrap {acl modify "$dir0 $dir1" -a "user:$prin1:r user:$prin2:r user:$prin3:r"}] != 0} {return;}

    # Run the test
    test { acl modify "$dir0 $dir1" -a "OTHER_OBJ:r USER:$prin0:r" -ch "user:$prin1:rt user:$prin2:rctw" -r user:$prin3
	 dcp_acl_find_multi_entries "$dir0 $dir1" {} {} [list "other_obj r------" "user $prin0 r------" "user $prin1 r--t---" "user $prin2 rw-tc--"]
	   } {compare_strings} {found}

    test { dcp_acl_find_multi_entries "$dir0 $dir1" {} {} [list "user $prin3 r------"]
	   } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal delete "$prin0 $prin1 $prin2 $prin3"}] != 0} {return;}
}

proc acl_modify_176_P {} {

    # Identify the test
    tet_infoline acl_modify_176_P: Add more than one ACL entry to more than one ACL object.
    tet_infoline acl_modify_176_P: Also test with uppercase ACL entries types.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test { acl modify "$dir0 $dir1" -a "OTHER_OBJ:r USER:$name:r"
	 dcp_acl_find_multi_entries "$dir0 $dir1" {} {} [list "other_obj r------" "user $name r------"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {directory delete "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_177_P {} {

    # Identify the test
    tet_infoline acl_modify_177_P: Add more than one ACL entry to more than one ACL object.
    tet_infoline acl_modify_177_P: Also test with uppercase ACL entries types and spaces
    tet_infoline acl_modify_177_P: instead of colons for ACL entry formats.
    tet_infoline acl_modify_177_P: This test operates on initial container ACL of the
    tet_infoline acl_modify_177_P: specified objects.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test { acl modify "$dir0 $dir1" -ic -a [list "Other_obj r" "uSer:$name:r"]
	 dcp_acl_find_multi_entries "$dir0 $dir1" {ic} {} [list "other_obj r------" "user $name r------"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {directory delete "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_178_P {} {

    # Identify the test
    tet_infoline acl_modify_178_P: Add more than one ACL entry to more than one ACL object.
    tet_infoline acl_modify_178_P: Also test with uppercase ACL entries types and spaces
    tet_infoline acl_modify_178_P: instead of colons for ACL entry formats.
    tet_infoline acl_modify_178_P: This test operates on initial object ACL of the
    tet_infoline acl_modify_178_P: specified objects.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test { acl modify "$dir0 $dir1" -io -a [list "other_obj r" "USer $name r"]
	 dcp_acl_find_multi_entries "$dir0 $dir1" {io} {} [list "other_obj r------" "user $name r------"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {directory delete "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_179_P {} {

    # Identify the test
    tet_infoline acl_modify_179_P: Add more than one ACL entry to more than one ACL object.
    tet_infoline acl_modify_179_P: Also test with spaces instead of colons for ACL entry formats.
    tet_infoline acl_modify_179_P: This test operates on the namespace entry ACL of the
    tet_infoline acl_modify_179_P: specified objects.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test { acl modify "$dir0 $dir1" -entry -a [list "other_obj r" "user $name r"]
	 dcp_acl_find_multi_entries "$dir0 $dir1" {entry} {} [list "other_obj r----" "user $name r----"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {directory delete "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_180_P {} {

    # Identify the test
    tet_infoline acl_modify_180_P: Change more than one ACL entry on more than one ACL object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify "$dir0 $dir1" -a "other_obj:r user:$name:r"}] != 0} {return;}

    # Run the test
    test { acl modify "$dir0 $dir1" -ch "other_obj:rw user:$name:crw"
	 dcp_acl_find_multi_entries "$dir0 $dir1" {} {} [list "other_obj rw-----" "user $name rw--c--"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {directory delete "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_181_P {} {

    # Identify the test
    tet_infoline acl_modify_181_P: Change more than one ACL entry on more than one ACL object.
    tet_infoline acl_modify_181_P: This test operates on initial container ACL of the
    tet_infoline acl_modify_181_P: specified objects.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify "$dir0 $dir1" -ic -a "other_obj:r user:$name:r"}] != 0} {return;}

    # Run the test
    test { acl modify "$dir0 $dir1" -ic -ch "other_obj:rw user:$name:crw"
	 dcp_acl_find_multi_entries "$dir0 $dir1" {ic} {} [list "other_obj rw-----" "user $name rw--c--"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {directory delete "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_182_P {} {

    # Identify the test
    tet_infoline acl_modify_182_P: Change more than one ACL entry on more than one ACL object.
    tet_infoline acl_modify_182_P: This test operates on initial object ACL of the
    tet_infoline acl_modify_182_P: specified objects.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify "$dir0 $dir1" -io -a "other_obj:r user:$name:r"}] != 0} {return;}

    # Run the test
    test { acl modify "$dir0 $dir1" -io -ch "other_obj:rw user:$name:crw"
	 dcp_acl_find_multi_entries "$dir0 $dir1" {io} {} [list "other_obj rw-----" "user $name rw--c--"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {directory delete "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_183_P {} {

    # Identify the test
    tet_infoline acl_modify_183_P: Change more than one ACL entry on more than one ACL object.
    tet_infoline acl_modify_183_P: This test operates on the namespace entry ACL of the

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify "$dir0 $dir1" -entry -a "other_obj:r user:$name:r"}] != 0} {return;}

    # Run the test
    test { acl modify "$dir0 $dir1" -entry -ch "other_obj:rw user:$name:crw"
	 dcp_acl_find_multi_entries "$dir0 $dir1" {entry} {} [list "other_obj rw---" "user $name rw--c"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {directory delete "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_184_P {} {

    # Identify the test
    tet_infoline acl_modify_184_P: Remove more than one ACL entry from more than one ACL object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify "$dir0 $dir1" -a "other_obj:r user:$name:r"}] != 0} {return;}

    # Run the test
    test { acl modify "$dir0 $dir1" -r "other_obj user:$name"
	 dcp_acl_find_multi_entries "$dir0" {} {} [list "other_obj r------"]
	   } {compare_strings} {not found}

    test { dcp_acl_find_multi_entries "$dir0" {} {} [list "user $name r------"]
	   } {compare_strings} {not found}

    test { dcp_acl_find_multi_entries "$dir1" {} {} [list "other_obj r------"]
	   } {compare_strings} {not found}

    test { dcp_acl_find_multi_entries "$dir1" {} {} [list "user $name r------"]
	   } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_185_P {} {

    # Identify the test
    tet_infoline acl_modify_185_P: Remove more than one ACL entry from more than one ACL object.
    tet_infoline acl_modify_185_P: This test operates on initial container ACL of the
    tet_infoline acl_modify_185_P: specified objects.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify "$dir0 $dir1" -ic -a "other_obj:r user:$name:r"}] != 0} {return;}

    # Run the test
    test { acl modify "$dir0 $dir1" -ic -r "other_obj user:$name"
	 dcp_acl_find_multi_entries "$dir0" {} {} [list "other_obj r------"]
	   } {compare_strings} {not found}

    test { dcp_acl_find_multi_entries "$dir0" {} {} [list "user $name r------"]
	   } {compare_strings} {not found}

    test { dcp_acl_find_multi_entries "$dir1" {} {} [list "other_obj r------"]
	   } {compare_strings} {not found}

    test { dcp_acl_find_multi_entries "$dir1" {} {} [list "user $name r------"]
	   } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_186_P {} {

    # Identify the test
    tet_infoline acl_modify_186_P: Remove more than one ACL entry from more than one ACL object.
    tet_infoline acl_modify_186_P: This test operates on initial object ACL of the
    tet_infoline acl_modify_186_P: specified objects.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify "$dir0 $dir1" -io -a "other_obj:r user:$name:r"}] != 0} {return;}

    # Run the test
    test { acl modify "$dir0 $dir1" -io -r "other_obj user:$name"
	 dcp_acl_find_multi_entries "$dir0" {} {} [list "other_obj r------"]
	   } {compare_strings} {not found}

    test { dcp_acl_find_multi_entries "$dir0" {} {} [list "user $name r------"]
	   } {compare_strings} {not found}

    test { dcp_acl_find_multi_entries "$dir1" {} {} [list "other_obj r------"]
	   } {compare_strings} {not found}

    test { dcp_acl_find_multi_entries "$dir1" {} {} [list "user $name r------"]
	   } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_187_P {} {

    # Identify the test
    tet_infoline acl_modify_187_P: Remove more than one ACL entry from more than one ACL object.
    tet_infoline acl_modify_187_P: This test operates on the namespace entry ACL of the
    tet_infoline acl_modify_187_P: specified objects.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify "$dir0 $dir1" -entry -a "other_obj:r user:$name:r"}] != 0} {return;}

    # Run the test
    test { acl modify "$dir0 $dir1" -entry -r "other_obj user:$name"
	 dcp_acl_find_multi_entries "$dir0" {} {} [list "other_obj r------"]
	   } {compare_strings} {not found}

    test { dcp_acl_find_multi_entries "$dir0" {} {} [list "user $name r------"]
	   } {compare_strings} {not found}

    test { dcp_acl_find_multi_entries "$dir1" {} {} [list "other_obj r------"]
	   } {compare_strings} {not found}

    test { dcp_acl_find_multi_entries "$dir1" {} {} [list "user $name r------"]
	   } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {directory delete "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_188_P {} {

    # Identify the test
    tet_infoline acl_modify_188_P: Add a "user_obj_delegate" ACL entry to the specified ACL object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test { acl modify /.:/sec/principal/$name -a user_obj_delegate:rc
	 dcp_acl_find_multi_entries "/.:/sec/principal/$name" {} {} {{user_obj_delegate rc-------}}
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_189_P {} {

    # Identify the test
    tet_infoline acl_modify_189_P: Add a "user_delegate" ACL entry to the specified ACL object.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_DELEGATION}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_DELEGATION -a user_delegate:$name:r
	 dcp_acl_find_multi_entries "$DCP_ACL_OBJ_DELEGATION" {} {} [list "user_delegate $name r--"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -r user_delegate:$name}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_190_P {} {

    # Identify the test
    tet_infoline acl_modify_190_P: Add a "foreign_user_delegate" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_190_P: is not enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_HOSTNAME_ONE DCP_ACL_OBJ_DELEGATION DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_DELEGATION -a foreign_user_delegate:$DCP_CELLNAME_TWO/$name:r
	 dcp_acl_find_multi_entries "$DCP_ACL_OBJ_DELEGATION" {} {} [list "foreign_user_delegate $DCP_CELLNAME_TWO/$name r--"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -r foreign_user_delegate:$DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_191_P {} {

    # Identify the test
    tet_infoline acl_modify_191_P: Add a "group_obj_delegate" ACL entry to the specified ACL object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {group create $name}] != 0} {return;}

    # Run the test
    test { acl modify /.:/sec/group/$name -a group_obj_delegate:rc
	 dcp_acl_find_multi_entries "/.:/sec/group/$name" {} {} {{group_obj_delegate rc------}}
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {group delete $name}] != 0} {return;}
}

proc acl_modify_192_P {} {

    # Identify the test
    tet_infoline acl_modify_192_P: Add a "group_delegate" ACL entry to the specified ACL object.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_DELEGATION}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {group create $name}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_DELEGATION -a group_delegate:$name:r
	 dcp_acl_find_multi_entries "$DCP_ACL_OBJ_DELEGATION" {} {} [list "group_delegate $name r--"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -r group_delegate:$name}] != 0} {return;}
    if {[dcp_wrap {group delete $name}] != 0} {return;}
}

proc acl_modify_193_P {} {

    # Identify the test
    tet_infoline acl_modify_193_P: Add a "foreign_group_delegate" ACL entry to the specified ACL 
    tet_infoline acl_modify_193_P: object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_HOSTNAME_ONE DCP_ACL_OBJ_DELEGATION DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_DELEGATION -a foreign_group_delegate:$DCP_CELLNAME_TWO/$name:r
	 dcp_acl_find_multi_entries "$DCP_ACL_OBJ_DELEGATION" {} {} [list "foreign_group_delegate $DCP_CELLNAME_TWO/$name r--"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -r foreign_group_delegate:$DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_194_P {} {

    # Identify the test
    tet_infoline acl_modify_194_P: Add a "other_obj_delegate" ACL entry to the specified ACL object.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_DELEGATION}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_DELEGATION -a other_obj_delegate:rc
	 dcp_acl_find_multi_entries "$DCP_ACL_OBJ_DELEGATION" {} {} {{other_obj_delegate r-c}}
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -r other_obj_delegate}] != 0} {return;}
}

proc acl_modify_195_P {} {

    # Identify the test
    tet_infoline acl_modify_195_P: Add a "foreign_other_delegate" ACL entry to the specified 
    tet_infoline acl_modify_195_P: ACL object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_HOSTNAME_ONE DCP_ACL_OBJ_DELEGATION DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_DELEGATION -a foreign_other_delegate:$DCP_CELLNAME_TWO:r
	 dcp_acl_find_multi_entries "$DCP_ACL_OBJ_DELEGATION" {} {} [list "foreign_other_delegate $DCP_CELLNAME_TWO r--"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -r foreign_other_delegate:$DCP_CELLNAME_TWO}] != 0} {return;}
}

proc acl_modify_196_P {} {

    # Identify the test
    tet_infoline acl_modify_196_P: Add a "any_other_delegate" ACL entry to the specified ACL object.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_DELEGATION}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_DELEGATION -a any_other_delegate:r
	 dcp_acl_find_multi_entries "$DCP_ACL_OBJ_DELEGATION" {} {} {{any_other_delegate r--}}
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -r any_other_delegate}] != 0} {return;}
}

proc acl_modify_197_P {} {

    # Identify the test
    tet_infoline acl_modify_197_P: Change a "user_obj_delegate" ACL entry to the specified ACL object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify /.:/sec/principal/$name -a user_obj_delegate:r}] != 0} {return;}

    # Run the test
    test { acl modify /.:/sec/principal/$name -ch user_obj_delegate:rc
	 dcp_acl_find_multi_entries "/.:/sec/principal/$name" {} {} {{user_obj_delegate rc-------}}
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_198_P {} {

    # Identify the test
    tet_infoline acl_modify_198_P: Change a "user_delegate" ACL entry to the specified ACL object.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_DELEGATION}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -a user_delegate:$name:r}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_DELEGATION -ch user_delegate:$name:rwc
	 dcp_acl_find_multi_entries "$DCP_ACL_OBJ_DELEGATION" {} {} [list "user_delegate $name rwc"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -r user_delegate:$name}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_199_P {} {

    # Identify the test
    tet_infoline acl_modify_199_P: Change a "foreign_user_delegate" ACL entry to the specified ACL object, which
    tet_infoline acl_modify_199_P: is not enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_HOSTNAME_ONE DCP_ACL_OBJ_DELEGATION DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -a foreign_user_delegate:$DCP_CELLNAME_TWO/$name:r}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_DELEGATION -ch foreign_user_delegate:$DCP_CELLNAME_TWO/$name:rwc
	 dcp_acl_find_multi_entries "$DCP_ACL_OBJ_DELEGATION" {} {} [list "foreign_user_delegate $DCP_CELLNAME_TWO/$name rwc"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -r foreign_user_delegate:$DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_200_P {} {

    # Identify the test
    tet_infoline acl_modify_200_P: Change a "group_obj_delegate" ACL entry to the specified ACL object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {group create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify /.:/sec/group/$name -a group_obj_delegate:r}] != 0} {return;}

    # Run the test
    test { acl modify /.:/sec/group/$name -ch group_obj_delegate:rc
	 dcp_acl_find_multi_entries "/.:/sec/group/$name" {} {} {{group_obj_delegate rc------}}
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {group delete $name}] != 0} {return;}
}

proc acl_modify_201_P {} {

    # Identify the test
    tet_infoline acl_modify_201_P: Change a "group_delegate" ACL entry to the specified ACL object.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_DELEGATION}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {group create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -a group_delegate:$name:r}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_DELEGATION -ch group_delegate:$name:rwc
	 dcp_acl_find_multi_entries "$DCP_ACL_OBJ_DELEGATION" {} {} [list "group_delegate $name rwc"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -r group_delegate:$name}] != 0} {return;}
    if {[dcp_wrap {group delete $name}] != 0} {return;}
}

proc acl_modify_202_P {} {

    # Identify the test
    tet_infoline acl_modify_202_P: Change a "foreign_group_delegate" ACL entry to the specified ACL 
    tet_infoline acl_modify_202_P: object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_HOSTNAME_ONE DCP_ACL_OBJ_DELEGATION DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -a foreign_group_delegate:$DCP_CELLNAME_TWO/$name:r}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_DELEGATION -ch foreign_group_delegate:$DCP_CELLNAME_TWO/$name:rwc
	 dcp_acl_find_multi_entries "$DCP_ACL_OBJ_DELEGATION" {} {} [list "foreign_group_delegate $DCP_CELLNAME_TWO/$name rwc"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -r foreign_group_delegate:$DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_203_P {} {

    # Identify the test
    tet_infoline acl_modify_203_P: Change a "other_obj_delegate" ACL entry to the specified ACL object.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_DELEGATION}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -a other_obj_delegate:r}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_DELEGATION -ch other_obj_delegate:rwc
	 dcp_acl_find_multi_entries "$DCP_ACL_OBJ_DELEGATION" {} {} {{other_obj_delegate rwc}}
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -r other_obj_delegate}] != 0} {return;}
}

proc acl_modify_204_P {} {

    # Identify the test
    tet_infoline acl_modify_204_P: Change a "foreign_other_delegate" ACL entry to the specified 
    tet_infoline acl_modify_204_P: ACL object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_HOSTNAME_ONE DCP_ACL_OBJ_DELEGATION DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -a foreign_other_delegate:$DCP_CELLNAME_TWO:r}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_DELEGATION -ch foreign_other_delegate:$DCP_CELLNAME_TWO:rwc
	 dcp_acl_find_multi_entries "$DCP_ACL_OBJ_DELEGATION" {} {} [list "foreign_other_delegate $DCP_CELLNAME_TWO rwc"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -r foreign_other_delegate:$DCP_CELLNAME_TWO}] != 0} {return;}
}

proc acl_modify_205_P {} {

    # Identify the test
    tet_infoline acl_modify_205_P: Change a "any_other_delegate" ACL entry to the 
    tet_infoline acl_modify_205_P: specified ACL object.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_DELEGATION}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -a any_other_delegate:r}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_DELEGATION -ch any_other_delegate:rwc
	 dcp_acl_find_multi_entries "$DCP_ACL_OBJ_DELEGATION" {} {} {{any_other_delegate rwc}}
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -r any_other_delegate}] != 0} {return;}
}

proc acl_modify_206_P {} {

    # Identify the test
    tet_infoline acl_modify_206_P: Remove a "user_obj_delegate" ACL entry to the specified ACL object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify /.:/sec/principal/$name -a user_obj_delegate:r}] != 0} {return;}

    # Run the test
    test { acl modify /.:/sec/principal/$name -r user_obj_delegate
	 dcp_acl_find_multi_entries "/.:/sec/principal/$name" {} {} {{user_obj_delegate r--------}}
	   } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_207_P {} {

    # Identify the test
    tet_infoline acl_modify_207_P: Remove a "user_delegate" ACL entry to the specified ACL object.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_DELEGATION}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -a user_delegate:$name:r}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_DELEGATION -r user_delegate:$name
	 dcp_acl_find_multi_entries "$DCP_ACL_OBJ_DELEGATION" {} {} [list "user_delegate $name r--"]
	   } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_208_P {} {

    # Identify the test
    tet_infoline acl_modify_208_P: Remove a "foreign_user_delegate" ACL entry to the specified 
    tet_infoline acl_modify_208_P: ACL object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_HOSTNAME_ONE DCP_ACL_OBJ_DELEGATION DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -a foreign_user_delegate:$DCP_CELLNAME_TWO/$name:r}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_DELEGATION -r foreign_user_delegate:$DCP_CELLNAME_TWO/$name
	 dcp_acl_find_multi_entries "$DCP_ACL_OBJ_DELEGATION" {} {} [list "foreign_user_delegate $DCP_CELLNAME_TWO/$name r--"]
	   } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_209_P {} {

    # Identify the test
    tet_infoline acl_modify_209_P: Remove a "group_obj_delegate" ACL entry to the specified ACL object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {group create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify /.:/sec/group/$name -a group_obj_delegate:r}] != 0} {return;}

    # Run the test
    test { acl modify /.:/sec/group/$name -r group_obj_delegate
	 dcp_acl_find_multi_entries "/.:/sec/group/$name" {} {} {{group_obj_delegate r--}}
	   } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {group delete $name}] != 0} {return;}
}

proc acl_modify_210_P {} {

    # Identify the test
    tet_infoline acl_modify_210_P: Remove a "group_delegate" ACL entry to the specified ACL object.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_DELEGATION}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {group create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -a group_delegate:$name:r}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_DELEGATION -r group_delegate:$name
	 dcp_acl_find_multi_entries "$DCP_ACL_OBJ_DELEGATION" {} {} [list "group_delegate $name r--"]
	   } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {group delete $name}] != 0} {return;}
}

proc acl_modify_211_P {} {

    # Identify the test
    tet_infoline acl_modify_211_P: Remove a "foreign_group_delegate" ACL entry to the specified ACL 
    tet_infoline acl_modify_211_P: object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_HOSTNAME_ONE DCP_ACL_OBJ_DELEGATION DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group create $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -a foreign_group_delegate:$DCP_CELLNAME_TWO/$name:r}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_DELEGATION -r foreign_group_delegate:$DCP_CELLNAME_TWO/$name
	 dcp_acl_find_multi_entries "$DCP_ACL_OBJ_DELEGATION" {} {} [list "foreign_group_delegate $DCP_CELLNAME_TWO/$name r--"]
	   } {compare_strings} {not found}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {group delete $DCP_CELLNAME_TWO/$name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_212_P {} {

    # Identify the test
    tet_infoline acl_modify_212_P: Remove a "other_obj_delegate" ACL entry to the specified ACL object.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_DELEGATION}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -a other_obj_delegate:r}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_DELEGATION -r other_obj_delegate
	 dcp_acl_find_multi_entries "$DCP_ACL_OBJ_DELEGATION" {} {} {{other_obj_delegate r--}}
	   } {compare_strings} {not found}

    # Cleanup steps
}

proc acl_modify_213_P {} {

    # Identify the test
    tet_infoline acl_modify_213_P: Remove a "foreign_other_delegate" ACL entry to the specified 
    tet_infoline acl_modify_213_P: ACL object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_HOSTNAME_ONE DCP_ACL_OBJ_DELEGATION DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -a foreign_other_delegate:$DCP_CELLNAME_TWO:r}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_DELEGATION -r foreign_other_delegate:$DCP_CELLNAME_TWO
	 dcp_acl_find_multi_entries "$DCP_ACL_OBJ_DELEGATION" {} {} [list "foreign_other_delegate $DCP_CELLNAME_TWO r--"]
	   } {compare_strings} {not found}

    # Cleanup steps
}

proc acl_modify_214_P {} {

    # Identify the test
    tet_infoline acl_modify_214_P: Remove a "any_other_delegate" ACL entry to the 
    tet_infoline acl_modify_214_P: specified ACL object.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_DELEGATION}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_DELEGATION -a any_other_delegate:r}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_DELEGATION -r any_other_delegate
	 dcp_acl_find_multi_entries "$DCP_ACL_OBJ_DELEGATION" {} {} {{any_other_delegate r--}}
	   } {compare_strings} {not found}

    # Cleanup steps
}

proc acl_modify_215_P {} {

    # Identify the test
    tet_infoline acl_modify_215_P: Change the value of the default cell for 1 ACL object.

    # Setup steps
    if {[dcp_wrap {global DCP_CELLNAME_ONE DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 5}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create "$prin0 $prin1"}] != 0} {return;}
    if {[dcp_wrap {acl mod $dir0 -a "user:$prin0:r user:$prin1:r"}] != 0} {return;}
    if {[dcp_wrap {acl mod $dir0 -ch any_other:crw}] != 0} {return;}

    # Run the test
    test {  acl modify $dir0 -cell $DCP_CELLNAME_TWO
	    acl show $dir0 -cell
	   } {compare_strings} "$DCP_CELLNAME_TWO"

        # Expected results are:
        # Successful completion.
        # This will also test that the local cell registry and 
        # the default cell registry are binded to properly.
        # Note that the principals and groups are not registered 
        # in the default cell registry as a result, UUID strings 
        # will be displayed.
        # [acl show $dir0 ]
        # [acl show $dir0 -cell ]

    # Cleanup steps
    if {[dcp_wrap {acl modify $dir0 -cell $DCP_CELLNAME_ONE}] != 0} {return;}
    if {[dcp_wrap {principal delete "$prin0 $prin1"}] != 0} {return;}
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_216_P {} {

    # Identify the test
    tet_infoline acl_modify_216_P: Show all ACL entries from the previous test.
    tet_infoline acl_modify_216_P: This will also test that the local cell registry and 
    tet_infoline acl_modify_216_P: the default cell registry are binded to properly.
    tet_infoline acl_modify_216_P: Note that the principal, a_p_cell2, and group, a_g_cell2,
    tet_infoline acl_modify_216_P: are registered in the default cell registry. All other
    tet_infoline acl_modify_216_P: principals and groups are not registered 
    tet_infoline acl_modify_216_P: in the default cell registry.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_ONE DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create "$DCP_CELLNAME_TWO/$prin0 $DCP_CELLNAME_TWO/$prin1"}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {acl mod $dir0 -ch any_other:crw}] != 0} {return;}

    # Run the test
    test {  acl modify $dir0 -cell $DCP_CELLNAME_TWO
            acl mod $dir0 -a "user:$prin0:r user:$prin1:r"
	    acl show $dir0 -cell
	   } {compare_strings} "$DCP_CELLNAME_TWO"

    # Cleanup steps
    if {[dcp_wrap {acl mod $dir0 -cell $DCP_CELLNAME_ONE}] != 0} {return;}
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal delete "$DCP_CELLNAME_TWO/$prin0 $DCP_CELLNAME_TWO/$prin1"}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_217_P {} {

    # Identify the test
    tet_infoline acl_modify_217_P: This will test that the cell information will be processed
    tet_infoline acl_modify_217_P: first and the new default cell registry is binded to properly.
    tet_infoline acl_modify_217_P: Then the new principal, a_p_cell2, and group, a_g_cell2,
    tet_infoline acl_modify_217_P: which are registered in the default cell registry, will be
    tet_infoline acl_modify_217_P: added. All other principals and groups 
    tet_infoline acl_modify_217_P: are not registered in the default cell registry.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_ONE DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create "$DCP_CELLNAME_TWO/$prin0 $DCP_CELLNAME_TWO/$prin1"}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {acl mod $dir0 -ch any_other:crw}] != 0} {return;}

    # Run the test
    test {  acl mod $dir0 -a "user:$prin0:r user:$prin1:r" -cell $DCP_CELLNAME_TWO
	    acl show $dir0 -cell
	   } {compare_strings} "$DCP_CELLNAME_TWO"

    # Cleanup steps
    if {[dcp_wrap {acl mod $dir0 -cell $DCP_CELLNAME_ONE}] != 0} {return;}
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal delete "$DCP_CELLNAME_TWO/$prin0 $DCP_CELLNAME_TWO/$prin1"}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc acl_modify_218_P {} {

    # Identify the test
    tet_infoline acl_modify_218_P: Change the value of the default cell for more than 
    tet_infoline acl_modify_218_P: 1 ACL object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_ONE DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {directory create $dir1}] != 0} {return;}
    if {[dcp_wrap {acl mod "$dir0 $dir1" -ch any_other:crw}] != 0} {return;}

    # Run the test
    test { acl modify "$dir0 $dir1" -cell $DCP_CELLNAME_TWO
	    dcp_acl_find_cell "$dir0 $dir1" {} "$DCP_CELLNAME_TWO"
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl mod "$dir0 $dir1" -cell $DCP_CELLNAME_ONE}] != 0} {return;}
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {directory delete $dir1}] != 0} {return;}
}

proc acl_modify_219_P {} {

    # Identify the test
    tet_infoline acl_modify_219_P: Change the value of the default cell for 1 ACL object.
    tet_infoline acl_modify_219_P: This test operates on the namespace entry ACL of the 
    tet_infoline acl_modify_219_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_ONE DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl mod $dir0 -entry -ch any_other:crw}] != 0} {return;}

    # Run the test
    test {  acl modify $dir0 -entry -cell "$DCP_CELLNAME_TWO"
	    acl show $dir0 -entry -cell
	   } {compare_strings} "$DCP_CELLNAME_TWO"

    # Cleanup steps
    if {[dcp_wrap {acl mod $dir0 -entry -cell $DCP_CELLNAME_ONE}] != 0} {return;}
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_220_P {} {

    # Identify the test
    tet_infoline acl_modify_220_P: Change the value of the default cell for more than 1 ACL object.
    tet_infoline acl_modify_220_P: This test ensures that the namespace entry ACL for all the 
    tet_infoline acl_modify_220_P: specified objects are operated on.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_ONE DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {directory create $dir1}] != 0} {return;}
    if {[dcp_wrap {acl mod "$dir0 $dir1" -entry -ch any_other:crw}] != 0} {return;}

    # Run the test
    test { acl modify "$dir0 $dir1" -entry -cell "$DCP_CELLNAME_TWO"
	    dcp_acl_find_cell "$dir0 $dir1" {entry} "$DCP_CELLNAME_TWO"
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl mod "$dir0 $dir1" -entry -cell $DCP_CELLNAME_ONE}] != 0} {return;}
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {directory delete $dir1}] != 0} {return;}
}

proc acl_modify_221_P {} {

    # Identify the test
    tet_infoline acl_modify_221_P: Change the value of the default cell for 1 ACL object.
    tet_infoline acl_modify_221_P: This test operates on initial container ACL of the 
    tet_infoline acl_modify_221_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_ONE DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl mod $dir0 -ic -ch any_other:crw}] != 0} {return;}

    # Run the test
    test {  acl modify $dir0 -ic -cell "$DCP_CELLNAME_TWO"
	    acl show $dir0 -ic -cell
	   } {compare_strings} "$DCP_CELLNAME_TWO"

    # Cleanup steps
    if {[dcp_wrap {acl mod $dir0 -ic -cell $DCP_CELLNAME_ONE}] != 0} {return;}
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_222_P {} {

    # Identify the test
    tet_infoline acl_modify_222_P: Change the value of the default cell for more than 1 ACL object.
    tet_infoline acl_modify_222_P: This test ensures that the initial container ACL for all the 
    tet_infoline acl_modify_222_P: specified objects are operated on.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_ONE DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {directory create $dir1}] != 0} {return;}
    if {[dcp_wrap {acl mod "$dir0 $dir1" -ic -ch any_other:crw}] != 0} {return;}

    # Run the test
    test { acl modify "$dir0 $dir1" -ic -cell "$DCP_CELLNAME_TWO"
	    dcp_acl_find_cell "$dir0 $dir1" {ic} "$DCP_CELLNAME_TWO"
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl mod "$dir0 $dir1" -ic -cell $DCP_CELLNAME_ONE}] != 0} {return;}
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {directory delete $dir1}] != 0} {return;}
}

proc acl_modify_223_P {} {

    # Identify the test
    tet_infoline acl_modify_223_P: Change the value of the default cell for 1 ACL object.
    tet_infoline acl_modify_223_P: This test operates on initial object ACL of the 
    tet_infoline acl_modify_223_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_ONE DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl mod $dir0 -io -ch any_other:crw}] != 0} {return;}

    # Run the test
    test {  acl modify $dir0 -io -cell "$DCP_CELLNAME_TWO"
	    acl show $dir0 -io -cell
	   } {compare_strings} "$DCP_CELLNAME_TWO"

    # Cleanup steps
    if {[dcp_wrap {acl mod $dir0 -io -cell $DCP_CELLNAME_ONE}] != 0} {return;}
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_224_P {} {

    # Identify the test
    tet_infoline acl_modify_224_P: Change the value of the default cell for more than 1 ACL object.
    tet_infoline acl_modify_224_P: This test ensures that the initial object ACL for all the 
    tet_infoline acl_modify_224_P: specified objects are operated on.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_ONE DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {directory create $dir1}] != 0} {return;}
    if {[dcp_wrap {acl mod "$dir0 $dir1" -io -ch any_other:crw}] != 0} {return;}

    # Run the test
    test { acl modify "$dir0 $dir1" -io -cell "$DCP_CELLNAME_TWO"
	    dcp_acl_find_cell "$dir0 $dir1" {io} "$DCP_CELLNAME_TWO"
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl mod "$dir0 $dir1" -io -cell $DCP_CELLNAME_ONE}] != 0} {return;}
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {directory delete $dir1}] != 0} {return;}
}

proc acl_modify_225_P {} {

    # Identify the test
    tet_infoline acl_modify_225_P: Purges all masked permissions.  All ACL entries in the file
    tet_infoline acl_modify_225_P: group class should not display any ineffective permissions.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_MASK1}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -a mask_obj:rc}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_MASK1 -purge
	    dcp_acl_find_multi_entries "$DCP_ACL_OBJ_MASK1" {} {} [list "user hosts/$DCP_HOSTNAME_ONE/self r-c" "group subsys/dce/dts-admin r-c"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -r mask_obj}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -ch "user:hosts/$DCP_HOSTNAME_ONE/self:crw group:subsys/dce/dts-admin:crw"}] != 0} {return;}
}

proc acl_modify_226_P {} {

    # Identify the test
    tet_infoline acl_modify_226_P: Purges all masked permissions before any ACL entry
    tet_infoline acl_modify_226_P: modifications. The user:name entry should display 
    tet_infoline acl_modify_226_P: ineffective permissions. All ACL entries in the file
    tet_infoline acl_modify_226_P: group class should not display any ineffective permissions.

    # Setup steps
        # Save original ACL entries for $DCP_ACL_OBJ_MASK1.
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_MASK1}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -a mask_obj:rc}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_MASK1 -purge -a user:$name:rwc
	    dcp_acl_find_multi_entries "$DCP_ACL_OBJ_MASK1" {} {} [list "user hosts/$DCP_HOSTNAME_ONE/self r-c" "group subsys/dce/dts-admin r-c" "user $name rwc effective r-c"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -r "mask_obj user:$name"}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -ch "user:hosts/$DCP_HOSTNAME_ONE/self:crw group:subsys/dce/dts-admin:crw"}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_227_P {} {

    # Identify the test
    tet_infoline acl_modify_227_P: Check that no mask re-calculation occurs on the specified
    tet_infoline acl_modify_227_P: ACL object that supports the mask_obj ACL entry type.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_MASK1}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_MASK1 -mask nocalc -a user:$name:r
	    dcp_acl_find_multi_entries "$DCP_ACL_OBJ_MASK1" {} {} [list "user $name r--"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -r user:$name}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_228_P {} {

    # Identify the test
    tet_infoline acl_modify_228_P: Add a "mask_obj" ACL entry to the specified ACL object that
    tet_infoline acl_modify_228_P: supports the mask_obj ACL entry type.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_MASK1}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_MASK1 -a mask_obj:rwc
	    dcp_acl_find_multi_entries "$DCP_ACL_OBJ_MASK1" {} {} {{mask_obj rwc}}
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -r mask_obj}] != 0} {return;}
}

proc acl_modify_229_P {} {

    # Identify the test
    tet_infoline acl_modify_229_P: Check that a POSIX mask re-calculation occurs on the specified
    tet_infoline acl_modify_229_P: ACL object that supports the mask_obj ACL entry type but it
    tet_infoline acl_modify_229_P: currently does exist. The current mask_obj entry will be
    tet_infoline acl_modify_229_P: changed.

    # Setup steps
    if {[dcp_wrap {global DCP_DFS_AVAIL}] != 0} {return;}
    if {$DCP_DFS_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dfs_dir0 /.:/fs/ $name}] != 0} {return;}
    if {[dcp_wrap {mkdir $dfs_dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $dfs_dir0 -a mask_obj:r}] != 0} {return;}
    if {[dcp_wrap {acl modify $dfs_dir0 -ch {user_obj:c group_obj:r other_obj:r}}] != 0} {return;}

    # Run the test
    test { acl modify $dfs_dir0 -a user:$name:rwxcid
	    dcp_acl_find_multi_entries "$dfs_dir0" {} {} "[list "mask_obj rwxcid" "user $name rwxcid"]"
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {rmdir $dfs_dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_230_P {} {

    # Identify the test
    tet_infoline acl_modify_230_P: Check that a POSIX mask re-calculation occurs on the specified
    tet_infoline acl_modify_230_P: ACL object that supports the mask_obj ACL entry type but it
    tet_infoline acl_modify_230_P: currently does not exist. A new mask_obj entry should be
    tet_infoline acl_modify_230_P: created.

    # Setup steps
    if {[dcp_wrap {global DCP_DFS_AVAIL}] != 0} {return;}
    if {$DCP_DFS_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dfs_dir0 /.:/fs/ $name}] != 0} {return;}
    if {[dcp_wrap {mkdir $dfs_dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test { acl modify $dfs_dir0 -a user:$name:rwc
	    dcp_acl_find_multi_entries "$dfs_dir0" {} {} [list "mask_obj rwxcid" "user $name rw-c--"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {rmdir $dfs_dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_231_P {} {

    # Identify the test
    tet_infoline acl_modify_231_P: Check that a mask re-calculation occurs after all
    tet_infoline acl_modify_231_P: modifications are made on the specified ACL object that 
    tet_infoline acl_modify_231_P: supports the mask_obj ACL entry type. The mask_obj ACL
    tet_infoline acl_modify_231_P: entry type does not exist but will be created.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_MASK1}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_MASK1 -mask calc -a user:$name:rwc
	    dcp_acl_find_multi_entries "$DCP_ACL_OBJ_MASK1" {} {} "[list "mask_obj rwc" "user $name rwc"]"
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -r mask_obj}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -r user:$name}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_232_P {} {

    # Identify the test
    tet_infoline acl_modify_232_P: Add an ACL entry to 1 ACL object specified as a string binding 
    tet_infoline acl_modify_232_P: with an object UUID in string syntax.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR ENDPOINT}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create "$prin0 $prin1"}] != 0} {return;}

    # Run the test
    test { acl modify "$OBJECT_UUID@$PROTSEQ:$NETADDR principal/$prin0" -a user:$prin1:r
	    dcp_acl_find_multi_entries "/.:/sec/principal/$prin0" {} {} [list "user $prin1 r--------"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {principal delete "$prin0 $prin1"}] != 0} {return;}
}

proc acl_modify_233_P {} {

    # Identify the test
    tet_infoline acl_modify_233_P: Add an ACL entry to 1 ACL object specified as a string binding 
    tet_infoline acl_modify_233_P: with an object UUID in TCL syntax.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR ENDPOINT}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create "$prin0 $prin1"}] != 0} {return;}

    # Run the test
    test { acl modify "$OBJECT_UUID $PROTSEQ $NETADDR principal/$prin0" -a user:$prin1:r
	    dcp_acl_find_multi_entries "/.:/sec/principal/$prin0" {} {} [list "user $prin1 r--------"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {principal delete "$prin0 $prin1"}] != 0} {return;}
}

proc acl_modify_234_P {} {

    # Identify the test
    tet_infoline acl_modify_234_P: Add an ACL entry to 1 ACL object specified as a string binding 
    tet_infoline acl_modify_234_P: with an endpoint in string syntax.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR ENDPOINT}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create "$prin0 $prin1"}] != 0} {return;}

    # Run the test
    test { acl modify "$OBJECT_UUID@$PROTSEQ:$NETADDR\[$ENDPOINT\] principal/$prin0" -a user:$prin1:r
	    dcp_acl_find_multi_entries "/.:/sec/principal/$prin0" {} {} [list "user $prin1 r--------"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {principal delete "$prin0 $prin1"}] != 0} {return;}
}

proc acl_modify_235_P {} {

    # Identify the test
    tet_infoline acl_modify_235_P: Add an ACL entry to 1 ACL object specified as a string binding 
    tet_infoline acl_modify_235_P: with an endpoint in TCL syntax.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR ENDPOINT}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create "$prin0 $prin1"}] != 0} {return;}

    # Run the test
    test { acl modify "$OBJECT_UUID $PROTSEQ $NETADDR $ENDPOINT principal/$prin0" -a user:$prin1:r
	    dcp_acl_find_multi_entries "/.:/sec/principal/$prin0" {} {} [list "user $prin1 r--------"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {principal delete "$prin0 $prin1"}] != 0} {return;}
}

proc acl_modify_236_P {} {

    # Identify the test
    tet_infoline acl_modify_236_P: Add a "extended" ACL entry in string syntax to the 
    tet_infoline acl_modify_236_P: specified ACL object.

    # Setup steps
    if {[dcp_wrap {global DCP_ACL_OBJ_EXTENDED EXTENDED_UUID EXTENDED_NDR_F1 EXTENDED_NDR_F2 EXTENDED_NDR_F3 EXTENDED_NDR_F4 EXTENDED_NBR_BYTES EXTENDED_DATA}] != 0} {return;}

    # Run the test
    tet_result UNSUPPORTED;return
    test { acl modify $DCP_ACL_OBJ_EXTENDED -a extended:$EXTENDED_UUID.$EXTENDED_NDR_F1.$EXTENDED_NDR_F2.$EXTENDED_NDR_F3.$EXTENDED_NDR_F4.$EXTENDED_NBR_BYTES.$EXTENDED_DATA:r
	    dcp_acl_find_multi_entries "$DCP_ACL_OBJ_EXTENDED" {} {} [list "extended $EXTENDED_UUID.$EXTENDED_NDR_F1.$EXTENDED_NDR_F2.$EXTENDED_NDR_F3.$EXTENDED_NDR_F4.$EXTENDED_NBR_BYTES.$EXTENDED_DATA r-----"]
	   } {compare_strings} {found}

    # Cleanup steps
}

proc acl_modify_237_P {} {

    # Identify the test
    tet_infoline acl_modify_237_P: Change a "extended" ACL entry in TCL syntax to the specified 
    tet_infoline acl_modify_237_P: ACL object.

    # Setup steps
    tet_result UNSUPPORTED;return
    if {[dcp_wrap {global DCP_ACL_OBJ_EXTENDED EXTENDED_UUID EXTENDED_NDR_F1 EXTENDED_NDR_F2 EXTENDED_NDR_F3 EXTENDED_NDR_F4 EXTENDED_NBR_BYTES EXTENDED_DATA}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_EXTENDED -a extended:$EXTENDED_UUID.$EXTENDED_NDR_F1.$EXTENDED_NDR_F2.$EXTENDED_NDR_F3.$EXTENDED_NDR_F4.$EXTENDED_NBR_BYTES.$EXTENDED_DATA:r}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_EXTENDED -ch [list "extended $EXTENDED_UUID.$EXTENDED_NDR_F1.$EXTENDED_NDR_F2.$EXTENDED_NDR_F3.$EXTENDED_NDR_F4.$EXTENDED_NBR_BYTES.$EXTENDED_DATA rt"]
	    dcp_acl_find_multi_entries "$DCP_ACL_OBJ_EXTENDED" {} {} [list "extended $EXTENDED_UUID.$EXTENDED_NDR_F1.$EXTENDED_NDR_F2.$EXTENDED_NDR_F3.$EXTENDED_NDR_F4.$EXTENDED_NBR_BYTES.$EXTENDED_DATA r-t---"]
	   } {compare_strings} {found}

    # Cleanup steps
}

proc acl_modify_238_P {} {

    # Identify the test
    tet_infoline acl_modify_238_P: Remove a "extended" ACL entry in TCL syntax 
    tet_infoline acl_modify_238_P: to the specified ACL object.

    # Setup steps
    tet_result UNSUPPORTED;return
    if {[dcp_wrap {global DCP_ACL_OBJ_EXTENDED EXTENDED_UUID EXTENDED_NDR_F1 EXTENDED_NDR_F2 EXTENDED_NDR_F3 EXTENDED_NDR_F4 EXTENDED_NBR_BYTES EXTENDED_DATA}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_EXTENDED -a extended:$EXTENDED_UUID.$EXTENDED_NDR_F1.$EXTENDED_NDR_F2.$EXTENDED_NDR_F3.$EXTENDED_NDR_F4.$EXTENDED_NBR_BYTES.$EXTENDED_DATA:r}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_EXTENDED -r [list "extended $EXTENDED_UUID.$EXTENDED_NDR_F1.$EXTENDED_NDR_F2.$EXTENDED_NDR_F3.$EXTENDED_NDR_F4.$EXTENDED_NBR_BYTES.$EXTENDED_DATA"]
	    dcp_acl_find_multi_entries "$DCP_ACL_OBJ_EXTENDED" {} {} [list "extended $EXTENDED_UUID.$EXTENDED_NDR_F1.$EXTENDED_NDR_F2.$EXTENDED_NDR_F3.$EXTENDED_NDR_F4.$EXTENDED_NBR_BYTES.$EXTENDED_DATA r-----"]
	   } {compare_strings} {not found}

    # Cleanup steps
}

proc acl_modify_239_P {} {

    # Identify the test
    tet_infoline acl_modify_239_P: Check that a mask re-calculation occurs after all
    tet_infoline acl_modify_239_P: modifications are made on the specified ACL object that 
    tet_infoline acl_modify_239_P: supports the mask_obj ACL entry type. The mask_obj ACL
    tet_infoline acl_modify_239_P: entry type does exist but will be updated.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_MASK1}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -a mask_obj:rc}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -purge}] != 0} {return;}

    # Run the test
    test { acl modify $DCP_ACL_OBJ_MASK1 -mask calc -a user:$name:rwc
	    dcp_acl_find_multi_entries "$DCP_ACL_OBJ_MASK1" {} {} "[list "mask_obj rwc" "user $name rwc"]"
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -r mask_obj}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -r user:$name}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -ch "user:hosts/$DCP_HOSTNAME_ONE/self:crw group:subsys/dce/dts-admin:crw"}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_240_P {} {

    # Identify the test
    tet_infoline acl_modify_240_P: If due to masking there are ineffective bits in an ACL
    tet_infoline acl_modify_240_P: entry, then on input these two elements are ignored.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0 }] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1 }] != 0} {return;}
    if {[dcp_wrap {principal create "$prin0 $prin1"}] != 0} {return;}

    # Run the test
    test { acl modify /.:/sec/principal/$prin0 -a [list "user $prin1 rc effective r"]
	    dcp_acl_find_multi_entries "/.:/sec/principal/$prin0" {} {} [list "user $prin1 rc-------"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {principal delete "$prin0 $prin1"}] != 0} {return;}
}

proc acl_modify_241_P {} {
    # Identify the test
    tet_infoline acl_modify_241_P: Add a "user" ACL entry to a polymorphic object 
    tet_infoline acl_modify_241_P: specifying first ACL manager.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {append prin2 $name 2}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0/$prin1}] != 0} {return;}
    if {[dcp_wrap {principal create $prin2}] != 0} {return;}

    # Run the test
    test { acl modify /.:/sec/principal/$prin0 -type "principal" -a user:$prin2:r
	   dcp_acl_find_multi_entries "/.:/sec/principal/$prin0" {} "principal" [list "user $prin2 r--------"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {principal delete $prin0/$prin1}] != 0} {return;}
    if {[dcp_wrap {principal delete $prin0}] != 0} {return;}
    if {[dcp_wrap {principal delete $prin2}] != 0} {return;}
}

proc acl_modify_242_P {} {
    # Identify the test
    tet_infoline acl_modify_242_P: Add a "user" ACL entry to a polymorphic object 
    tet_infoline acl_modify_242_P: specifying second ACL manager.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {append prin2 $name 2}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0/$prin1}] != 0} {return;}
    if {[dcp_wrap {principal create $prin2}] != 0} {return;}

    # Run the test
    test { acl modify /.:/sec/principal/$prin0 -type "secdirectory" -a user:$prin2:r
	   dcp_acl_find_multi_entries "/.:/sec/principal/$prin0" {} "secdirectory" [list "user $prin2 r-----"]
	   } {compare_strings} {found}

    # Cleanup steps
    if {[dcp_wrap {principal delete $prin0/$prin1}] != 0} {return;}
    if {[dcp_wrap {principal delete $prin0}] != 0} {return;}
    if {[dcp_wrap {principal delete $prin2}] != 0} {return;}
}

proc acl_modify_243_P {} {
    # Identify the test
    tet_infoline acl_modify_243_P: Change the value of the default cell on a polymorphic
    tet_infoline acl_modify_243_P: object specifying the first ACL manager.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_ONE DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0/$prin1}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$prin0 -type "principal" -ch any_other:cr}] != 0} {return;}

    # Run the test
    test { acl modify /.:/sec/principal/$prin0 -type "principal" -cell $DCP_CELLNAME_TWO
	    acl show /.:/sec/principal/$prin0 -type "principal" -cell
	   } {compare_strings} "$DCP_CELLNAME_TWO"

    # Cleanup steps
    if {[dcp_wrap {acl modify /.:/sec/principal/$prin0 -type "principal" -cell $DCP_CELLNAME_ONE}] != 0} {return;}
    if {[dcp_wrap {principal delete $prin0/$prin1}] != 0} {return;}
    if {[dcp_wrap {principal delete $prin0}] != 0} {return;}
}

proc acl_modify_244_P {} {
    # Identify the test
    tet_infoline acl_modify_244_P: Change the value of the default cell on a polymorphic
    tet_infoline acl_modify_244_P: object specifying the second ACL manager.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_ONE DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0/$prin1}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$prin0 -type "secdirectory" -ch any_other:cr}] != 0} {return;}

    # Run the test
    test { acl modify /.:/sec/principal/$prin0 -type "secdirectory" -cell $DCP_CELLNAME_TWO
	    acl show /.:/sec/principal/$prin0 -type "secdirectory" -cell
	   } {compare_strings} "$DCP_CELLNAME_TWO"

    # Cleanup steps
    if {[dcp_wrap {acl modify /.:/sec/principal/$prin0 -type "secdirectory" -cell $DCP_CELLNAME_ONE}] != 0} {return;}
    if {[dcp_wrap {principal delete $prin0/$prin1}] != 0} {return;}
    if {[dcp_wrap {principal delete $prin0}] != 0} {return;}
}


set iclist "ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 ic25 ic26 ic27 ic28 ic29 ic30 ic31 ic32 ic33 ic34 ic35 ic36 ic37 ic38 ic39 ic40 ic41 ic42 ic43 ic44 ic45 ic46 ic47 ic48 ic49 ic50 ic51 ic52 ic53 ic54 ic55 ic56 ic57 ic58 ic59 ic60 ic61 ic62 ic63 ic64 ic65 ic66 ic67 ic68 ic69 ic70 ic71 ic72 ic73 ic74 ic75 ic76 ic77 ic78 ic79 ic80 ic81 ic82 ic83 ic84 ic85 ic86 ic87 ic88 ic89 ic90 ic91 ic92 ic93 ic94 ic95 ic96 ic97 ic98 ic99 ic100 ic101 ic102 ic103 ic104 ic105 ic106 ic107 ic108 ic109 ic110 ic111 ic112 ic113 ic114 ic115 ic116 ic117 ic118 ic119 ic120 ic121 ic122 ic123 ic124 ic125 ic126 ic127 ic128 ic129 ic130 ic131 ic132 ic133 ic134 ic135 ic136 ic137 ic138 ic139 ic140 ic141 ic142 ic143 ic144 ic145 ic146 ic147 ic148 ic149 ic150 ic151 ic152 ic153 ic154 ic155 ic156 ic157 ic158 ic159 ic160 ic161 ic162 ic163 ic164 ic165 ic166 ic167 ic168 ic169 ic170 ic171 ic172 ic173 ic174 ic175 ic176 ic177 ic178 ic179 ic180 ic181 ic182 ic183 ic184 ic185 ic186 ic187 ic188 ic189 ic190 ic191 ic192 ic193 ic194 ic195 ic196 ic197 ic198 ic199 ic200 ic201 ic202 ic203 "
set ic2 "acl_modify_043_P"
set ic3 "acl_modify_044_P"
set ic4 "acl_modify_045_P"
set ic5 "acl_modify_046_P"
set ic6 "acl_modify_047_P"
set ic7 "acl_modify_048_P"
set ic8 "acl_modify_049_P"
set ic9 "acl_modify_050_P"
set ic10 "acl_modify_051_P"
set ic11 "acl_modify_052_P"
set ic12 "acl_modify_053_P"
set ic13 "acl_modify_054_P"
set ic14 "acl_modify_055_P"
set ic15 "acl_modify_056_P"
set ic16 "acl_modify_057_P"
set ic17 "acl_modify_058_P"
set ic18 "acl_modify_059_P"
set ic19 "acl_modify_060_P"
set ic20 "acl_modify_061_P"
set ic21 "acl_modify_062_P"
set ic22 "acl_modify_063_P"
set ic23 "acl_modify_064_P"
set ic24 "acl_modify_065_P"
set ic25 "acl_modify_066_P"
set ic26 "acl_modify_067_P"
set ic27 "acl_modify_068_P"
set ic28 "acl_modify_069_P"
set ic29 "acl_modify_070_P"
set ic30 "acl_modify_071_P"
set ic31 "acl_modify_072_P"
set ic32 "acl_modify_073_P"
set ic33 "acl_modify_074_P"
set ic34 "acl_modify_075_P"
set ic35 "acl_modify_076_P"
set ic36 "acl_modify_077_P"
set ic37 "acl_modify_078_P"
set ic38 "acl_modify_079_P"
set ic39 "acl_modify_080_P"
set ic40 "acl_modify_081_P"
set ic41 "acl_modify_082_P"
set ic42 "acl_modify_083_P"
set ic43 "acl_modify_084_P"
set ic44 "acl_modify_085_P"
set ic45 "acl_modify_086_P"
set ic46 "acl_modify_087_P"
set ic47 "acl_modify_088_P"
set ic48 "acl_modify_089_P"
set ic49 "acl_modify_090_P"
set ic50 "acl_modify_091_P"
set ic51 "acl_modify_092_P"
set ic52 "acl_modify_093_P"
set ic53 "acl_modify_094_P"
set ic54 "acl_modify_095_P"
set ic55 "acl_modify_096_P"
set ic56 "acl_modify_097_P"
set ic57 "acl_modify_098_P"
set ic58 "acl_modify_099_P"
set ic59 "acl_modify_100_P"
set ic60 "acl_modify_101_P"
set ic61 "acl_modify_102_P"
set ic62 "acl_modify_103_P"
set ic63 "acl_modify_104_P"
set ic64 "acl_modify_105_P"
set ic65 "acl_modify_106_P"
set ic66 "acl_modify_107_P"
set ic67 "acl_modify_108_P"
set ic68 "acl_modify_109_P"
set ic69 "acl_modify_110_P"
set ic70 "acl_modify_111_P"
set ic71 "acl_modify_112_P"
set ic72 "acl_modify_113_P"
set ic73 "acl_modify_114_P"
set ic74 "acl_modify_115_P"
set ic75 "acl_modify_116_P"
set ic76 "acl_modify_117_P"
set ic77 "acl_modify_118_P"
set ic78 "acl_modify_119_P"
set ic79 "acl_modify_120_P"
set ic80 "acl_modify_121_P"
set ic81 "acl_modify_122_P"
set ic82 "acl_modify_123_P"
set ic83 "acl_modify_124_P"
set ic84 "acl_modify_125_P"
set ic85 "acl_modify_126_P"
set ic86 "acl_modify_127_P"
set ic87 "acl_modify_128_P"
set ic88 "acl_modify_129_P"
set ic89 "acl_modify_130_P"
set ic90 "acl_modify_131_P"
set ic91 "acl_modify_132_P"
set ic92 "acl_modify_133_P"
set ic93 "acl_modify_134_P"
set ic94 "acl_modify_135_P"
set ic95 "acl_modify_136_P"
set ic96 "acl_modify_137_P"
set ic97 "acl_modify_138_P"
set ic98 "acl_modify_139_P"
set ic99 "acl_modify_140_P"
set ic100 "acl_modify_141_P"
set ic101 "acl_modify_142_P"
set ic102 "acl_modify_143_P"
set ic103 "acl_modify_144_P"
set ic104 "acl_modify_145_P"
set ic105 "acl_modify_146_P"
set ic106 "acl_modify_147_P"
set ic107 "acl_modify_148_P"
set ic108 "acl_modify_149_P"
set ic109 "acl_modify_150_P"
set ic110 "acl_modify_151_P"
set ic111 "acl_modify_152_P"
set ic112 "acl_modify_153_P"
set ic113 "acl_modify_154_P"
set ic114 "acl_modify_155_P"
set ic115 "acl_modify_156_P"
set ic116 "acl_modify_157_P"
set ic117 "acl_modify_158_P"
set ic118 "acl_modify_159_P"
set ic119 "acl_modify_160_P"
set ic120 "acl_modify_161_P"
set ic121 "acl_modify_162_P"
set ic122 "acl_modify_163_P"
set ic123 "acl_modify_164_P"
set ic124 "acl_modify_165_P"
set ic125 "acl_modify_166_P"
set ic126 "acl_modify_167_P"
set ic127 "acl_modify_168_P"
set ic128 "acl_modify_169_P"
set ic129 "acl_modify_170_P"
set ic130 "acl_modify_171_P"
set ic131 "acl_modify_172_P"
set ic132 "acl_modify_173_P"
set ic133 "acl_modify_174_P"
set ic134 "acl_modify_175_P"
set ic135 "acl_modify_176_P"
set ic136 "acl_modify_177_P"
set ic137 "acl_modify_178_P"
set ic138 "acl_modify_179_P"
set ic139 "acl_modify_180_P"
set ic140 "acl_modify_181_P"
set ic141 "acl_modify_182_P"
set ic142 "acl_modify_183_P"
set ic143 "acl_modify_184_P"
set ic144 "acl_modify_185_P"
set ic145 "acl_modify_186_P"
set ic146 "acl_modify_187_P"
set ic147 "acl_modify_188_P"
set ic148 "acl_modify_189_P"
set ic149 "acl_modify_190_P"
set ic150 "acl_modify_191_P"
set ic151 "acl_modify_192_P"
set ic152 "acl_modify_193_P"
set ic153 "acl_modify_194_P"
set ic154 "acl_modify_195_P"
set ic155 "acl_modify_196_P"
set ic156 "acl_modify_197_P"
set ic157 "acl_modify_198_P"
set ic158 "acl_modify_199_P"
set ic159 "acl_modify_200_P"
set ic160 "acl_modify_201_P"
set ic161 "acl_modify_202_P"
set ic162 "acl_modify_203_P"
set ic163 "acl_modify_204_P"
set ic164 "acl_modify_205_P"
set ic165 "acl_modify_206_P"
set ic166 "acl_modify_207_P"
set ic167 "acl_modify_208_P"
set ic168 "acl_modify_209_P"
set ic169 "acl_modify_210_P"
set ic170 "acl_modify_211_P"
set ic171 "acl_modify_212_P"
set ic172 "acl_modify_213_P"
set ic173 "acl_modify_214_P"
set ic174 "acl_modify_215_P"
set ic175 "acl_modify_216_P"
set ic176 "acl_modify_217_P"
set ic177 "acl_modify_218_P"
set ic178 "acl_modify_219_P"
set ic179 "acl_modify_220_P"
set ic180 "acl_modify_221_P"
set ic181 "acl_modify_222_P"
set ic182 "acl_modify_223_P"
set ic183 "acl_modify_224_P"
set ic184 "acl_modify_225_P"
set ic185 "acl_modify_226_P"
set ic186 "acl_modify_227_P"
set ic187 "acl_modify_228_P"
set ic188 "acl_modify_229_P"
set ic189 "acl_modify_230_P"
set ic190 "acl_modify_231_P"
set ic191 "acl_modify_232_P"
set ic192 "acl_modify_233_P"
set ic193 "acl_modify_234_P"
set ic194 "acl_modify_235_P"
set ic195 "acl_modify_236_P"
set ic196 "acl_modify_237_P"
set ic197 "acl_modify_238_P"
set ic198 "acl_modify_239_P"
set ic199 "acl_modify_240_P"
set ic200 "acl_modify_241_P"
set ic201 "acl_modify_242_P"
set ic202 "acl_modify_243_P"
set ic203 "acl_modify_244_P"
set tet_startup acl_modify_startup
set tet_cleanup acl_modify_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
