#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: acl_replace_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:09:31  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:27:56  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:05  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:51:02  root]
# 
# Revision 1.1.2.3  1994/10/04  21:47:40  salamone
# 	CR12480 - Created directories in wrong cell
# 	[1994/10/04  21:39:56  salamone]
# 
# Revision 1.1.2.2  1994/10/01  01:33:07  rousseau
# 	Dynamically generate string bindings.
# 	[1994/10/01  01:28:50  rousseau]
# 
# Revision 1.1.2.1  1994/09/28  21:37:24  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:18  melman]
# 
# Revision 1.1.2.7  1994/09/26  21:04:19  salamone
# 	CR9938 - Change ACL mgr names and return UNINITIATED for those tests that aren't configured.
# 	[1994/09/26  21:02:24  salamone]
# 
# Revision 1.1.2.6  1994/08/25  15:33:12  salamone
# 	CR11635 - Handle 1 string binding specification for an ACL object.
# 	[1994/08/25  15:30:07  salamone]
# 
# Revision 1.1.2.5  1994/07/18  20:05:05  salamone
# 	Add tests for polymorphic object support
# 	[1994/07/18  20:01:59  salamone]
# 
# Revision 1.1.2.4  1994/06/10  20:02:18  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:26  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:34:20  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:15:24  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:01:06  melman
# 	updating to new API path
# 	[1994/05/06  15:00:34  melman]
# 
# Revision 1.1.2.1  1994/03/24  16:51:38  salamone
# 	Initial version.
# 	[1994/03/24  16:48:26  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc acl_replace_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}

    # Load variables for string binding tests
    dcp_acl_binding_vars
}

proc acl_replace_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc acl_replace_022_P {} {

    # Identify the test
    tet_infoline acl_replace_022_P: Replace all ACL entries with 1 ACL entry specified in a 
    tet_infoline acl_replace_022_P: string syntax for 1 ACL object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    test { acl replace $dir0 -acl "user:$DCP_CLIENT:rwdtcia"
	   acl show $dir0
	   } compare_strings [list "user $DCP_CLIENT rwdtcia"]

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_replace_023_P {} {

    # Identify the test
    tet_infoline acl_replace_023_P: Replace all ACL entries with 1 ACL entry specified in a TCL
    tet_infoline acl_replace_023_P: syntax for 1 ACL object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0"}] != 0} {return;}

    # Run the test
    test { acl replace $dir0 -acl [list "user $DCP_CLIENT rwdtcia"]
	   acl show $dir0
	   } compare_strings [list "user $DCP_CLIENT rwdtcia"]

    # Cleanup steps
    if {[dcp_wrap {directory delete "$dir0"}] != 0} {return;}
}

proc acl_replace_024_P {} {

    # Identify the test
    tet_infoline acl_replace_024_P: Replace all ACL entries for 1 ACL object, which
    tet_infoline acl_replace_024_P: is not enclosed in braces.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {acl mod $dir0 -r {unauthenticated any_other}}] != 0} {return;}

    # Run the test
    test { acl replace $dir0 -acl [acl sh $dir1]
	   dcp_acl_find_multi_entries "$dir0" {} {} [eval "acl sh $dir1"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {directory delete "$dir0 $dir1"}] != 0} {return;}
}

proc acl_replace_025_P {} {

    # Identify the test
    tet_infoline acl_replace_025_P: Replace all ACL entries and  the value of the default 
    tet_infoline acl_replace_025_P: cell for 1 ACL object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_ONE DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL DCP_HOSTNAME_TWO}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 $DCP_CELLNAME_TWO/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0"}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir1"}] != 0} {return;}
    if {[dcp_wrap {acl mod $dir1 -ch "unauthenticated:rwdtcia any_other:rwdtcia" -r "user:$DCP_CLIENT user:hosts/$DCP_HOSTNAME_TWO/cds-server group:subsys/dce/cds-admin group:subsys/dce/cds-server"}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test { acl replace $dir0 -acl [acl sh $dir1] -cell $DCP_CELLNAME_TWO
	   dcp_acl_find_cell "$dir0" {} "$DCP_CELLNAME_TWO"
	   } compare_strings {found}

    test { dcp_acl_find_multi_entries "$dir0" {} {} [eval "acl sh $dir1"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory delete "$dir1"}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {directory delete "$dir0"}] != 0} {return;}
}

proc acl_replace_026_P {} {

    # Identify the test
    tet_infoline acl_replace_026_P: Replace all ACL entries for 1 ACL object, which
    tet_infoline acl_replace_026_P: is enclosed in braces. Checks for an ACL entry
    tet_infoline acl_replace_026_P: format with colons instead of spaces.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl mod $dir0 -r {unauthenticated any_other}}] != 0} {return;}

    # Run the test
    test { acl replace "$dir0" -acl "user:$DCP_CLIENT:rwdtcia"
	   acl show $dir0
	   } compare_strings [list "user $DCP_CLIENT rwdtcia"]

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_replace_027_P {} {

    # Identify the test
    tet_infoline acl_replace_027_P: Replace all ACL entries for 1 ACL object, which
    tet_infoline acl_replace_027_P: is enclosed in braces. Checks for list of ACL entry
    tet_infoline acl_replace_027_P: formats with colons instead of spaces.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {acl mod $dir0 -r {unauthenticated any_other}}] != 0} {return;}

    # Run the test
    test { acl replace "$dir0" -acl "unauthenticated:r user:$DCP_CLIENT:rwdtcia"
	   dcp_acl_find_multi_entries "$dir0" {} {} [list "unauthenticated r------" "user $DCP_CLIENT rwdtcia"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_replace_028_P {} {

    # Identify the test
    tet_infoline acl_replace_028_P: Replace all ACL entries for more than 1 ACL object,
    tet_infoline acl_replace_028_P: which are not individually enclosed in braces.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {append dir2 /.:/ $name 2}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1 $dir2"}] != 0} {return;}
    if {[dcp_wrap {acl mod "$dir0 $dir1" -r {unauthenticated any_other}}] != 0} {return;}

    # Run the test
    test { acl replace "$dir0 $dir1" -acl [acl sh $dir2]
	   dcp_acl_find_multi_entries "$dir0 $dir1" {} {} [eval "acl sh $dir2"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {directory delete "$dir0 $dir1 $dir2"}] != 0} {return;}
}

proc acl_replace_029_P {} {

    # Identify the test
    tet_infoline acl_replace_029_P: Replace  all ACL entries and the value of the default cell 
    tet_infoline acl_replace_029_P: for more than 1 ACL object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_ONE DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL DCP_HOSTNAME_TWO}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {append dir2 $DCP_CELLNAME_TWO/ $name 2}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir2"}] != 0} {return;}
    if {[dcp_wrap {acl mod $dir2 -ch "unauthenticated:rwdtcia any_other:rwdtcia" -r "user:$DCP_CLIENT user:hosts/$DCP_HOSTNAME_TWO/cds-server group:subsys/dce/cds-admin group:subsys/dce/cds-server"}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test { acl replace "$dir0 $dir1" -acl [acl sh $dir2] -cell $DCP_CELLNAME_TWO
	   dcp_acl_find_cell "$dir0 $dir1" {} "$DCP_CELLNAME_TWO"
	   } compare_strings {found}

    test { dcp_acl_find_multi_entries "$dir0 $dir1" {} {} [eval "acl sh $dir2"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory delete "$dir2"}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {directory delete "$dir0 $dir1"}] != 0} {return;}
}

proc acl_replace_030_P {} {

    # Identify the test
    tet_infoline acl_replace_030_P: Replace all ACL entries for more than 1 ACL object,
    tet_infoline acl_replace_030_P: which are individually enclosed in braces.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {append dir2 /.:/ $name 2}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1 $dir2"}] != 0} {return;}
    if {[dcp_wrap {acl mod "$dir0 $dir1" -r {unauthenticated any_other}}] != 0} {return;}

    # Run the test
    test { acl replace [list "$dir0" "$dir1"] -acl [acl sh $dir2]
	   dcp_acl_find_multi_entries "$dir0 $dir1" {} {} [eval "acl sh $dir2"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {directory delete "$dir0 $dir1 $dir2"}] != 0} {return;}
}

proc acl_replace_031_P {} {

    # Identify the test
    tet_infoline acl_replace_031_P: Replace all ACL entries for 1 ACL object.
    tet_infoline acl_replace_031_P: This test operates on the namespace entry ACL of the 
    tet_infoline acl_replace_031_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {acl mod $dir0 -entry -r {unauthenticated any_other}}] != 0} {return;}

    # Run the test
    test { acl replace $dir0 -acl [acl sh $dir1 -entry ] -entry
	   dcp_acl_find_multi_entries "$dir0" {entry} {} [eval "acl sh $dir1 -entry"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {directory delete "$dir0 $dir1"}] != 0} {return;}
}

proc acl_replace_032_P {} {

    # Identify the test
    tet_infoline acl_replace_032_P: Replace all ACL entries and the value of the default cell 
    tet_infoline acl_replace_032_P: for 1 ACL object.
    tet_infoline acl_replace_032_P: This test operates on the namespace entry ACL of the 
    tet_infoline acl_replace_032_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_ONE DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL DCP_HOSTNAME_TWO}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 $DCP_CELLNAME_TWO/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0"}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir1"}] != 0} {return;}
    if {[dcp_wrap {acl mod $dir1 -entry -ch "unauthenticated:rwdtc-- any_other:rwdtc--" -r "user:$DCP_CLIENT user:hosts/$DCP_HOSTNAME_TWO/cds-server group:subsys/dce/cds-admin group:subsys/dce/cds-server"}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test { acl replace $dir0 -entry -acl [acl sh $dir1 -entry] -cell $DCP_CELLNAME_TWO
	   dcp_acl_find_cell "$dir0" {entry} "$DCP_CELLNAME_TWO"
	   } compare_strings {found}

    test { dcp_acl_find_multi_entries "$dir0" {entry} {} [eval "acl sh $dir1 -entry"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory delete "$dir1"}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {directory delete "$dir0"}] != 0} {return;}
}

proc acl_replace_033_P {} {

    # Identify the test
    tet_infoline acl_replace_033_P: Replace all ACL entries for more than 1 ACL object.
    tet_infoline acl_replace_033_P: This test ensures that the namespace entry ACL for all the 
    tet_infoline acl_replace_033_P: specified objects are operated on.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {append dir2 /.:/ $name 2}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1 $dir2"}] != 0} {return;}
    if {[dcp_wrap {acl mod "$dir0 $dir1" -entry -r {unauthenticated any_other}}] != 0} {return;}

    # Run the test
    test { acl replace "$dir0 $dir1" -entry -acl [acl sh $dir2 -entry]
	   dcp_acl_find_multi_entries "$dir0 $dir1" {entry} {} [eval "acl sh $dir2 -entry"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {directory delete "$dir0 $dir1 $dir2"}] != 0} {return;}
}

proc acl_replace_034_P {} {

    # Identify the test
    tet_infoline acl_replace_034_P: Replace all ACL entries and the value of the default cell 
    tet_infoline acl_replace_034_P: for more than 1 ACL object.
    tet_infoline acl_replace_034_P: This test ensures that the namespace entry ACL for all the 
    tet_infoline acl_replace_034_P: specified objects are operated on.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_ONE DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL DCP_HOSTNAME_TWO}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {append dir2 $DCP_CELLNAME_TWO/ $name 2}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir2"}] != 0} {return;}
    if {[dcp_wrap {acl mod $dir2 -entry -ch "unauthenticated:rwdtc-- any_other:rwdtc--" -r "user:$DCP_CLIENT user:hosts/$DCP_HOSTNAME_TWO/cds-server group:subsys/dce/cds-admin group:subsys/dce/cds-server"}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test { acl replace "$dir0 $dir1" -entry -acl [acl sh $dir2 -entry] -cell $DCP_CELLNAME_TWO
	   dcp_acl_find_cell "$dir0 $dir1" {entry} "$DCP_CELLNAME_TWO"
	   } compare_strings {found}

    test { dcp_acl_find_multi_entries "$dir0 $dir1" {entry} {} [eval "acl sh $dir2 -entry"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory delete "$dir2"}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {directory delete "$dir0 $dir1"}] != 0} {return;}
}

proc acl_replace_035_P {} {

    # Identify the test
    tet_infoline acl_replace_035_P: Replace all ACL entries for 1 ACL object.
    tet_infoline acl_replace_035_P: This test operates on initial container ACL of the 
    tet_infoline acl_replace_035_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {acl mod $dir0 -ic -r {unauthenticated any_other}}] != 0} {return;}

    # Run the test
    test { acl replace $dir0 -acl [acl sh $dir1 -ic ] -ic
	   dcp_acl_find_multi_entries "$dir0" {ic} {} [eval "acl sh $dir1 -ic"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {directory delete "$dir0 $dir1"}] != 0} {return;}
}

proc acl_replace_036_P {} {

    # Identify the test
    tet_infoline acl_replace_036_P: Replace all ACL entries and the value of the default cell 
    tet_infoline acl_replace_036_P: for 1 ACL object.
    tet_infoline acl_replace_036_P: This test operates on initial container ACL of the 
    tet_infoline acl_replace_036_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_ONE DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 $DCP_CELLNAME_TWO/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0"}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir1"}] != 0} {return;}
    if {[dcp_wrap {acl mod $dir1 -ic -ch "unauthenticated:rwdtcia any_other:rwdtcia" -r "group:subsys/dce/cds-admin group:subsys/dce/cds-server"}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test { acl replace $dir0 -ic -acl [acl sh $dir1 -ic] -cell $DCP_CELLNAME_TWO
	   dcp_acl_find_cell "$dir0" {ic} "$DCP_CELLNAME_TWO"
	   } compare_strings {found}

    test { dcp_acl_find_multi_entries "$dir0" {ic} {} [eval "acl sh $dir1 -ic"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory delete "$dir1"}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {directory delete "$dir0"}] != 0} {return;}
}

proc acl_replace_037_P {} {

    # Identify the test
    tet_infoline acl_replace_037_P: Replace all ACL entries for more than 1 ACL object.
    tet_infoline acl_replace_037_P: This test ensures that the initial container ACL for all the 
    tet_infoline acl_replace_037_P: specified objects are operated on.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {append dir2 /.:/ $name 2}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1 $dir2"}] != 0} {return;}
    if {[dcp_wrap {acl mod "$dir0 $dir1" -ic -r {unauthenticated any_other}}] != 0} {return;}

    # Run the test
    test { acl replace "$dir0 $dir1" -ic -acl [acl sh $dir2 -ic]
	   dcp_acl_find_multi_entries "$dir0 $dir1" {ic} {} [eval "acl sh $dir2 -ic"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {directory delete "$dir0 $dir1 $dir2"}] != 0} {return;}
}

proc acl_replace_038_P {} {

    # Identify the test
    tet_infoline acl_replace_038_P: Replace all ACL entries and the value of the default cell 
    tet_infoline acl_replace_038_P: for more than 1 ACL object.
    tet_infoline acl_replace_038_P: This test ensures that the initial container ACL for all the 
    tet_infoline acl_replace_038_P: specified objects are operated on.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_ONE DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {append dir2 $DCP_CELLNAME_TWO/ $name 2}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir2"}] != 0} {return;}
    if {[dcp_wrap {acl mod $dir2 -ic -ch "unauthenticated:rwdtcia any_other:rwdtcia" -r "group:subsys/dce/cds-admin group:subsys/dce/cds-server"}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test { acl replace "$dir0 $dir1" -ic -acl [acl sh $dir2 -ic] -cell $DCP_CELLNAME_TWO
	   dcp_acl_find_cell "$dir0 $dir1" {ic} "$DCP_CELLNAME_TWO"
	   } compare_strings {found}

    test { dcp_acl_find_multi_entries "$dir0 $dir1" {ic} {} [eval "acl sh $dir2 -ic"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory delete "$dir2"}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {directory delete "$dir0 $dir1"}] != 0} {return;}
}

proc acl_replace_039_P {} {

    # Identify the test
    tet_infoline acl_replace_039_P: Replace all ACL entries for 1 ACL object.
    tet_infoline acl_replace_039_P: This test operates on initial object ACL of the 
    tet_infoline acl_replace_039_P: specified object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {acl mod $dir0 -io -r {unauthenticated any_other}}] != 0} {return;}

    # Run the test
    test { acl replace $dir0 -acl [acl sh $dir1 -io ] -io
	   dcp_acl_find_multi_entries "$dir0" {io} {} [eval "acl sh $dir1 -io"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {directory delete "$dir0 $dir1"}] != 0} {return;}
}

proc acl_replace_040_P {} {

    # Identify the test
    tet_infoline acl_replace_040_P: Replace all ACL entries and the value of the default cell 
    tet_infoline acl_replace_040_P: for 1 ACL object.
    tet_infoline acl_replace_040_P: This test operates on initial object ACL of the 
    tet_infoline acl_replace_040_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_ONE DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 $DCP_CELLNAME_TWO/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0"}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir1"}] != 0} {return;}
    if {[dcp_wrap {acl mod $dir1 -io -ch "unauthenticated:rwdtc-- any_other:rwdtc--" -r "group:subsys/dce/cds-admin group:subsys/dce/cds-server"}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test { acl replace $dir0 -io -acl [acl sh $dir1 -io] -cell $DCP_CELLNAME_TWO
	   dcp_acl_find_cell "$dir0" {io} "$DCP_CELLNAME_TWO"
	   } compare_strings {found}

    test { dcp_acl_find_multi_entries "$dir0" {io} {} [eval "acl sh $dir1 -io"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory delete "$dir1"}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {directory delete "$dir0"}] != 0} {return;}
}

proc acl_replace_041_P {} {

    # Identify the test
    tet_infoline acl_replace_041_P: Replace all ACL entries for more than 1 ACL object.
    tet_infoline acl_replace_041_P: This test ensures that the initial object ACL for all the 
    tet_infoline acl_replace_041_P: specified objects are operated on.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {append dir2 /.:/ $name 2}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1 $dir2"}] != 0} {return;}
    if {[dcp_wrap {acl mod "$dir0 $dir1" -io -r {unauthenticated any_other}}] != 0} {return;}

    # Run the test
    test { acl replace "$dir0 $dir1" -io -acl [acl sh $dir2 -io]
	   dcp_acl_find_multi_entries "$dir0 $dir1" {io} {} [eval "acl sh $dir2 -io"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {directory delete "$dir0 $dir1 $dir2"}] != 0} {return;}
}

proc acl_replace_042_P {} {

    # Identify the test
    tet_infoline acl_replace_042_P: Replace all ACL entries and the value of the default cell 
    tet_infoline acl_replace_042_P: for more than 1 ACL object.
    tet_infoline acl_replace_042_P: This test ensures that the initial object ACL for all the 
    tet_infoline acl_replace_042_P: specified objects are operated on.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_ONE DCP_CELLNAME_TWO DCP_INTERCELL_AVAIL}] != 0} {return;}
    if {$DCP_INTERCELL_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {append dir2 $DCP_CELLNAME_TWO/ $name 2}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir2"}] != 0} {return;}
    if {[dcp_wrap {acl mod $dir2 -io -ch "unauthenticated:rwdtc-- any_other:rwdtc--" -r "group:subsys/dce/cds-admin group:subsys/dce/cds-server"}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test { acl replace "$dir0 $dir1" -io -acl [acl sh $dir2 -io] -cell $DCP_CELLNAME_TWO
	   dcp_acl_find_cell "$dir0 $dir1" {io} "$DCP_CELLNAME_TWO"
	   } compare_strings {found}

    test { dcp_acl_find_multi_entries "$dir0 $dir1" {io} {} [eval "acl sh $dir2 -io"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CELLNAME_TWO/$DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory delete "$dir2"}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {directory delete "$dir0 $dir1"}] != 0} {return;}
}

proc acl_replace_043_P {} {

    # Identify the test
    tet_infoline acl_replace_043_P: Replace all ACL entries for 1 ACL object
    tet_infoline acl_replace_043_P: specified as a string binding with an object UUID in 
    tet_infoline acl_replace_043_P: string syntax.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {append prin2 $name 2}] != 0} {return;}
    if {[dcp_wrap {principal create "$prin0 $prin1 $prin2"}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$prin1 -a user:$prin2:rc}] != 0} {return;}

    # Run the test
    test { acl replace "$OBJECT_UUID@$PROTSEQ:$NETADDR principal/$prin0" -acl [acl sh /.:/sec/principal/$prin1]
	   dcp_acl_find_multi_entries "/.:/sec/principal/$prin0" {} {} [eval "acl sh /.:/sec/principal/$prin1"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {principal delete "$prin0 $prin1 $prin2"}] != 0} {return;}
}

proc acl_replace_044_P {} {

    # Identify the test
    tet_infoline acl_replace_044_P: Replace all ACL entries for 1 ACL object
    tet_infoline acl_replace_044_P: specified as a string binding with an object UUID in
    tet_infoline acl_replace_044_P: TCL syntax.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {append prin2 $name 2}] != 0} {return;}
    if {[dcp_wrap {principal create "$prin0 $prin1 $prin2"}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$prin1 -a user:$prin2:rc}] != 0} {return;}

    # Run the test
    test { acl replace "$OBJECT_UUID $PROTSEQ $NETADDR principal/$prin0" -acl [acl sh /.:/sec/principal/$prin1]
	   dcp_acl_find_multi_entries "/.:/sec/principal/$prin0" {} {} [eval "acl sh /.:/sec/principal/$prin1"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {principal delete "$prin0 $prin1 $prin2"}] != 0} {return;}
}

proc acl_replace_045_P {} {

    # Identify the test
    tet_infoline acl_replace_045_P: Replace all ACL entries for 1 ACL object
    tet_infoline acl_replace_045_P: specified as a string binding with an endpoint in
    tet_infoline acl_replace_045_P: string syntax.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR ENDPOINT}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {append prin2 $name 2}] != 0} {return;}
    if {[dcp_wrap {principal create "$prin0 $prin1 $prin2"}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$prin1 -a user:$prin2:rc}] != 0} {return;}

    # Run the test
    test { acl replace "$OBJECT_UUID@$PROTSEQ:$NETADDR\[$ENDPOINT\] principal/$prin0" -acl [acl sh /.:/sec/principal/$prin1]
	   dcp_acl_find_multi_entries "/.:/sec/principal/$prin0" {} {} [eval "acl sh /.:/sec/principal/$prin1"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {principal delete "$prin0 $prin1 $prin2"}] != 0} {return;}
}

proc acl_replace_046_P {} {

    # Identify the test
    tet_infoline acl_replace_046_P: Replace all ACL entries for 1 ACL object
    tet_infoline acl_replace_046_P: specified as a string binding with an endpoint in
    tet_infoline acl_replace_046_P: TCL syntax.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR ENDPOINT}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {append prin2 $name 2}] != 0} {return;}
    if {[dcp_wrap {principal create "$prin0 $prin1 $prin2"}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$prin1 -a user:$prin2:rc}] != 0} {return;}

    # Run the test
    test { acl replace "$OBJECT_UUID $PROTSEQ $NETADDR $ENDPOINT principal/$prin0" -acl [acl sh /.:/sec/principal/$prin1]
	   dcp_acl_find_multi_entries "/.:/sec/principal/$prin0" {} {} [eval "acl sh /.:/sec/principal/$prin1"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {principal delete "$prin0 $prin1 $prin2"}] != 0} {return;}
}

proc acl_replace_047_P {} {

    # Identify the test
    tet_infoline acl_replace_047_P: If due to masking there are ineffective bits in an ACL
    tet_infoline acl_replace_047_P: entry, then on input these two elements are ignored.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_MASK1}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -a mask_obj:rc}] != 0} {return;}

    # Run the test
    test { acl replace $DCP_ACL_OBJ_MASK1 -acl [acl sh $DCP_ACL_OBJ_MASK1]
	   dcp_acl_find_multi_entries "$DCP_ACL_OBJ_MASK1" {} {} [eval "acl sh $DCP_ACL_OBJ_MASK1"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -r mask_obj}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -ch "user:hosts/$DCP_HOSTNAME_ONE/self:crw group:subsys/dce/dts-admin:crw"}] != 0} {return;}
}

proc acl_replace_048_P {} {
    # Identify the test
    tet_infoline acl_replace_048_P: Replace all ACL entries for 1 polymorphic object
    tet_infoline acl_replace_048_P: specifying the first ACL manager.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {append prin2 $name 2}] != 0} {return;}
    if {[dcp_wrap {append prin3 $name 3}] != 0} {return;}
    if {[dcp_wrap {principal create "$prin0 $prin1 $prin2"}] != 0} {return;}
    if {[dcp_wrap {principal create "$prin0/$prin3 $prin1/$prin3"}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$prin1 -type "principal" -a user:$prin2:rc}] != 0} {return;}

    # Run the test
    test { acl replace /.:/sec/principal/$prin0 -type "principal" -acl [acl sh /.:/sec/principal/$prin1 -type "principal"]
	   dcp_acl_find_multi_entries "/.:/sec/principal/$prin0" {} {principal} [eval "acl sh /.:/sec/principal/$prin1 -type {principal}"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {principal delete "$prin0/$prin3 $prin1/$prin3"}] != 0} {return;}
    if {[dcp_wrap {principal delete "$prin0 $prin1 $prin2"}] != 0} {return;}
}

proc acl_replace_049_P {} {
    # Identify the test
    tet_infoline acl_replace_049_P: Replace all ACL entries for 1 polymorphic object
    tet_infoline acl_replace_049_P: specifying the second ACL manager.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {append prin2 $name 2}] != 0} {return;}
    if {[dcp_wrap {append prin3 $name 3}] != 0} {return;}
    if {[dcp_wrap {principal create "$prin0 $prin1 $prin2"}] != 0} {return;}
    if {[dcp_wrap {principal create "$prin0/$prin3 $prin1/$prin3"}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$prin1 -type "secdirectory" -a user:$prin2:rc}] != 0} {return;}

    # Run the test
    test { acl replace /.:/sec/principal/$prin0 -type "secdirectory" -acl [acl sh /.:/sec/principal/$prin1 -type "secdirectory"]
	   dcp_acl_find_multi_entries "/.:/sec/principal/$prin0" {} {secdirectory} [eval "acl sh /.:/sec/principal/$prin1 -type {secdirectory}"]
	   } compare_strings {found}

    # Cleanup steps
    if {[dcp_wrap {principal delete "$prin0/$prin3 $prin1/$prin3"}] != 0} {return;}
    if {[dcp_wrap {principal delete "$prin0 $prin1 $prin2"}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 ic25 ic26 ic27 ic28 "
set ic1 "acl_replace_022_P"
set ic2 "acl_replace_023_P"
set ic3 "acl_replace_024_P"
set ic4 "acl_replace_025_P"
set ic5 "acl_replace_026_P"
set ic6 "acl_replace_027_P"
set ic7 "acl_replace_028_P"
set ic8 "acl_replace_029_P"
set ic9 "acl_replace_030_P"
set ic10 "acl_replace_031_P"
set ic11 "acl_replace_032_P"
set ic12 "acl_replace_033_P"
set ic13 "acl_replace_034_P"
set ic14 "acl_replace_035_P"
set ic15 "acl_replace_036_P"
set ic16 "acl_replace_037_P"
set ic17 "acl_replace_038_P"
set ic18 "acl_replace_039_P"
set ic19 "acl_replace_040_P"
set ic20 "acl_replace_041_P"
set ic21 "acl_replace_042_P"
set ic22 "acl_replace_043_P"
set ic23 "acl_replace_044_P"
set ic24 "acl_replace_045_P"
set ic25 "acl_replace_046_P"
set ic26 "acl_replace_047_P"
set ic27 "acl_replace_048_P"
set ic28 "acl_replace_049_P"
set tet_startup acl_replace_startup
set tet_cleanup acl_replace_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
