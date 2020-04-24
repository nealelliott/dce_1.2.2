#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: principal_modify_era_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:21:49  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:14  marty]
#
# Revision 1.1.4.1  1995/12/13  15:28:29  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1995/07/12  12:56 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts38/1  1995/07/12  12:55 UTC  truitt
# 	CHFts15275: Add tests to support new i18ndata behavior.
# 
# 	HP revision /main/HPDCE02/2  1995/05/01  18:31 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts26/1  1995/05/01  18:30 UTC  truitt
# 	CHFts14653: Add new tests.
# 
# 	HP revision /main/HPDCE02/1  1994/12/01  16:18 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/12/01  16:17 UTC  truitt
# 	Create local schema objects to operate on instead
# 	of relying on those already in the security schema.
# 	[1995/12/11  15:55:19  root]
# 
# Revision 1.1.2.2  1994/09/30  16:21:56  truitt
# 	OT12414: Update tests to reflect new show functionality.
# 	[1994/09/30  15:42:05  truitt]
# 
# Revision 1.1.2.1  1994/09/28  21:43:25  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:12  melman]
# 
# Revision 1.1.2.3  1994/09/19  17:31:42  truitt
# 	Add binding tests
# 	[1994/09/19  16:21:21  truitt]
# 
# Revision 1.1.2.2  1994/09/16  18:25:33  truitt
# 	Update tests to reflect new functionality.
# 	[1994/09/16  18:23:08  truitt]
# 
# Revision 1.1.2.1  1994/08/26  18:17:13  truitt
# 	OT11448: Add modify era tests.
# 	[1994/08/26  18:13:55  truitt]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc principal_modify_era_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_integer -encoding integer -aclmgr {principal r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_attr_set -encoding attrset -aclmgr {principal r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_bytes -encoding byte -aclmgr {principal r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_i18n_data -encoding i18ndata -aclmgr {principal r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_printstring -encoding printstring -aclmgr {principal r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_uuid -encoding uuid -aclmgr {principal r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_void -encoding void -aclmgr {principal r r r r} -multi no}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_printstring_array -encoding stringarray -aclmgr {principal r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_binding -encoding binding -aclmgr {principal r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_any -encoding any -aclmgr {principal r r r r} -multi yes}] != 0} {;}
}

proc principal_modify_era_cleanup {} {
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/pgo_test_integer}] != 0} {;}
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/pgo_test_attr_set}] != 0} {;}
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/pgo_test_bytes}] != 0} {;}
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/pgo_test_i18n_data}] != 0} {;}
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/pgo_test_printstring}] != 0} {;}
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/pgo_test_uuid}] != 0} {;}
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/pgo_test_void}] != 0} {;}
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/pgo_test_printstring_array}] != 0} {;}
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/pgo_test_binding}] != 0} {;}
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/pgo_test_any}] != 0} {;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc principal_modify_era_void_1_P {} {
    # Identify the test
    tet_infoline principal_modify_era_void_1_P: Add one void attribute with none present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_1_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_1_P -add {pgo_test_void present}
        dcp_attrlist getval [principal show -all gumby_1_P] pgo_test_void
    } compare_strings present

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_1_P}] != 0} {return;}
}

proc principal_modify_era_void_2_P {} {
    # Identify the test
    tet_infoline principal_modify_era_void_2_P: Add one void attribute with one other present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_2_P -attribute {pgo_test_integer 101}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_2_P -add {pgo_test_void present}
        attrlist getvalues [principal show -all gumby_2_P] -type pgo_test_void
    } compare_lists present

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_2_P}] != 0} {return;}
}

proc principal_modify_era_void_3_P {} {
    # Identify the test
    tet_infoline principal_modify_era_void_3_P: Add one void attribute with none present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_3_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_3_P -add {pgo_test_void present}
        attrlist getvalues [principal show -all gumby_3_P] -type pgo_test_void
    } compare_lists present

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_3_P}] != 0} {return;}
}

proc principal_modify_era_void_4_P {} {
    # Identify the test
    tet_infoline principal_modify_era_void_4_P: Add one void attributes with many others present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_4_P -attribute {pgo_test_integer 101 102}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_4_P -add {pgo_test_void present}
        attrlist getvalues [principal show -all gumby_4_P] -type pgo_test_void
    } compare_lists present

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_4_P}] != 0} {return;}
}

proc principal_modify_era_void_5_P {} {
    # Identify the test
    tet_infoline principal_modify_era_void_5_P: Add one void attribute to multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_5_P_1 gumby_5_P_2} -attribute {pgo_test_integer 101}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_5_P_1 gumby_5_P_2} -add {pgo_test_void present}
        dcp_attrlist getval [principal show -all {gumby_5_P_1 gumby_5_P_2}] pgo_test_void
    } compare_strings present

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_5_P_1 gumby_5_P_2}}] != 0} {return;}
}

proc principal_modify_era_void_6_P {} {
    # Identify the test
    tet_infoline principal_modify_era_void_6_P: Remove one of one void attribute from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_6_P -attribute {pgo_test_void present}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_6_P -remove {pgo_test_void present}
        dcp_multi_values [principal show -all gumby_6_P] {pgo_test_void present}
    } compare_strings {pgo_test_void not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_6_P}] != 0} {return;}
}

proc principal_modify_era_void_7_P {} {
    # Identify the test
    tet_infoline principal_modify_era_void_7_P: Remove all void attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_7_P -attribute {pgo_test_void present}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_7_P -remove {pgo_test_void present}
        dcp_multi_values [principal show -all gumby_7_P] {pgo_test_void present}
    } compare_strings {pgo_test_void not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_7_P}] != 0} {return;}
}

proc principal_modify_era_void_8_P {} {
    # Identify the test
    tet_infoline principal_modify_era_void_8_P: Remove all void attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_8_P -attribute {pgo_test_void present}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_8_P -remove {pgo_test_void}
        dcp_multi_values [principal show -all gumby_8_P] {pgo_test_void present}
    } compare_strings {pgo_test_void not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_8_P}] != 0} {return;}
}

proc principal_modify_era_void_9_P {} {
    # Identify the test
    tet_infoline principal_modify_era_void_9_P: Remove all void attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_9_P -attribute {pgo_test_void present}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_9_P -remove {pgo_test_void} -types
        dcp_multi_values [principal show -all gumby_9_P] {pgo_test_void present}
    } compare_strings {pgo_test_void not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_9_P}] != 0} {return;}
}

proc principal_modify_era_void_10_P {} {
    # Identify the test
    tet_infoline principal_modify_era_void_10_P: Remove one of one void attribute from multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_10_P_1 gumby_10_P_2} -attribute {pgo_test_void present}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_10_P_1 gumby_10_P_2} -remove {pgo_test_void present}
        dcp_multi_values [principal show -all {gumby_10_P_1 gumby_10_P_2}] {pgo_test_void present}
    } compare_strings {pgo_test_void not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_10_P_1 gumby_10_P_2}}] != 0} {return;}
}

proc principal_modify_era_integer_1_P {} {
    # Identify the test
    tet_infoline principal_modify_era_integer_1_P: Add one integer attribute with none present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_1_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_1_P -add {pgo_test_integer 101}
        dcp_attrlist getval [principal show -all gumby_1_P] pgo_test_integer
    } compare_strings 101

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_1_P}] != 0} {return;}
}

proc principal_modify_era_integer_2_P {} {
    # Identify the test
    tet_infoline principal_modify_era_integer_2_P: Add one integer attribute with one present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_2_P -attribute {pgo_test_integer 101}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_2_P -add {pgo_test_integer 102}
        attrlist getvalues [principal show -all gumby_2_P] -type pgo_test_integer
    } compare_lists {101 102}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_2_P}] != 0} {return;}
}

proc principal_modify_era_integer_3_P {} {
    # Identify the test
    tet_infoline principal_modify_era_integer_3_P: Add many integer attributes with none present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_3_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_3_P -add {{pgo_test_integer 101} {pgo_test_integer 102}}
        attrlist getvalues [principal show -all gumby_3_P] -type pgo_test_integer
    } compare_lists {101 102}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_3_P}] != 0} {return;}
}

proc principal_modify_era_integer_4_P {} {
    # Identify the test
    tet_infoline principal_modify_era_integer_4_P: Add many integer attributes with many present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_4_P -attribute {pgo_test_integer 101 102}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_4_P -add {pgo_test_integer 103 104}
        attrlist getvalues [principal show -all gumby_4_P] -type pgo_test_integer
    } compare_lists {101 102 103 104}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_4_P}] != 0} {return;}
}

proc principal_modify_era_integer_5_P {} {
    # Identify the test
    tet_infoline principal_modify_era_integer_5_P: Add one integer attribute to multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_5_P_1 gumby_5_P_2} -attribute {pgo_test_printstring "blech"}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_5_P_1 gumby_5_P_2} -add {pgo_test_integer 101}
        dcp_attrlist getval [principal show -all {gumby_5_P_1 gumby_5_P_2}] pgo_test_integer
    } compare_strings 101

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_5_P_1 gumby_5_P_2}}] != 0} {return;}
}

proc principal_modify_era_integer_6_P {} {
    # Identify the test
    tet_infoline principal_modify_era_integer_6_P: Change to one integer attribute from one on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_6_P -attribute {pgo_test_integer 101}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_6_P -change {pgo_test_integer 102}
        attrlist getvalues [principal show -all gumby_6_P] -type pgo_test_integer
    } compare_lists {102}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_6_P}] != 0} {return;}
}

proc principal_modify_era_integer_7_P {} {
    # Identify the test
    tet_infoline principal_modify_era_integer_7_P: Change to one integer attribute from many on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_7_P -attribute {pgo_test_integer 101 102}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_7_P -change {pgo_test_integer 103}
        attrlist getvalues [principal show -all gumby_7_P] -type pgo_test_integer
    } compare_lists {103}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_7_P}] != 0} {return;}
}

proc principal_modify_era_integer_8_P {} {
    # Identify the test
    tet_infoline principal_modify_era_integer_8_P: Change to many integer attributes from one on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_8_P -attribute {pgo_test_integer 101}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_8_P -change {pgo_test_integer 102 103}
        attrlist getvalues [principal show -all gumby_8_P] -type pgo_test_integer
    } compare_lists {102 103}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_8_P}] != 0} {return;}
}

proc principal_modify_era_integer_9_P {} {
    # Identify the test
    tet_infoline principal_modify_era_integer_9_P: Change to many integer attributes from many on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_9_P -attribute {pgo_test_integer 101 102}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_9_P -change {pgo_test_integer 103 104}
        attrlist getvalues [principal show -all gumby_9_P] -type pgo_test_integer
    } compare_lists {103 104}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_9_P}] != 0} {return;}
}

proc principal_modify_era_integer_10_P {} {
    # Identify the test
    tet_infoline principal_modify_era_integer_10_P: Change to one integer attribute from one on multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_10_P_1 gumby_10_P_2} -attribute {pgo_test_integer 101}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_10_P_1 gumby_10_P_2} -change {pgo_test_integer 102}
        attrlist getvalues [principal show -all {gumby_10_P_1 gumby_10_P_2}] -type pgo_test_integer
    } compare_lists {102 102}

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_10_P_1 gumby_10_P_2}}] != 0} {return;}
}

proc principal_modify_era_integer_11_P {} {
    # Identify the test
    tet_infoline principal_modify_era_integer_11_P: Remove one of three integer attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_11_P -attribute {pgo_test_integer 101 102 103}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_11_P -remove {pgo_test_integer 102}
        attrlist getvalues [principal show -all gumby_11_P] -type pgo_test_integer
    } compare_lists {101 103}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_11_P}] != 0} {return;}
}

proc principal_modify_era_integer_12_P {} {
    # Identify the test
    tet_infoline principal_modify_era_integer_12_P: Remove two of four integer attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_12_P -attribute {pgo_test_integer 101 102 103 104}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_12_P -remove {{pgo_test_integer 101} {pgo_test_integer 103}}
        attrlist getvalues [principal show -all gumby_12_P] -type pgo_test_integer
    } compare_lists {102 104}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_12_P}] != 0} {return;}
}

proc principal_modify_era_integer_13_P {} {
    # Identify the test
    tet_infoline principal_modify_era_integer_13_P: Remove two of four integer attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_13_P -attribute {pgo_test_integer 101 102 103 104}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_13_P -remove {pgo_test_integer 101 103}
        attrlist getvalues [principal show -all gumby_13_P] -type pgo_test_integer
    } compare_lists {102 104}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_13_P}] != 0} {return;}
}

proc principal_modify_era_integer_14_P {} {
    # Identify the test
    tet_infoline principal_modify_era_integer_14_P: Remove the only one integer attribute from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_14_P -attribute {pgo_test_integer 101}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_14_P -remove {pgo_test_integer 101}
        dcp_multi_values [principal show -all gumby_14_P] {pgo_test_integer 101}
    } compare_strings {pgo_test_integer not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_14_P}] != 0} {return;}
}

proc principal_modify_era_integer_15_P {} {
    # Identify the test
    tet_infoline principal_modify_era_integer_15_P: Remove all integer attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_15_P -attribute {pgo_test_integer 101 102}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_15_P -remove {pgo_test_integer}
        dcp_multi_values [principal show -all gumby_15_P] {{pgo_test_integer 101} {pgo_test_integer 102}}
    } compare_strings {pgo_test_integer 101 not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_15_P}] != 0} {return;}
}

proc principal_modify_era_integer_16_P {} {
    # Identify the test
    tet_infoline principal_modify_era_integer_16_P: Remove all integer attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_16_P -attribute {pgo_test_integer 101 102}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_16_P -remove {pgo_test_integer} -types
        dcp_multi_values [principal show -all gumby_16_P] {{pgo_test_integer 101} {pgo_test_integer 102}}
    } compare_strings {pgo_test_integer 101 not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_16_P}] != 0} {return;}
}

proc principal_modify_era_integer_17_P {} {
    # Identify the test
    tet_infoline principal_modify_era_integer_17_P: Remove one of three integer attributes from multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_17_P_1 gumby_17_P_2} -attribute {pgo_test_integer 101 102 103}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_17_P_1 gumby_17_P_2} -remove {pgo_test_integer 102}
        attrlist getvalues [principal show -all {gumby_17_P_1 gumby_17_P_2}] -type pgo_test_integer
    } compare_lists {101 103 101 103}

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_17_P_1 gumby_17_P_2}}] != 0} {return;}
}

proc principal_modify_era_printstring_1_P {} {
    # Identify the test
    tet_infoline principal_modify_era_printstring_1_P: Add one printstring attribute with none present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_1_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_1_P -add {pgo_test_printstring "blech"}
        dcp_attrlist getval [principal show -all gumby_1_P] pgo_test_printstring
    } compare_strings blech

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_1_P}] != 0} {return;}
}

proc principal_modify_era_printstring_2_P {} {
    # Identify the test
    tet_infoline principal_modify_era_printstring_2_P: Add one printstring attribute with one present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_2_P -attribute {pgo_test_printstring "your mother"}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_2_P -add {pgo_test_printstring "was a hamster"}
        attrlist getvalues [principal show -all gumby_2_P] -type pgo_test_printstring
    } compare_lists {{your mother} {was a hamster}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_2_P}] != 0} {return;}
}

proc principal_modify_era_printstring_3_P {} {
    # Identify the test
    tet_infoline principal_modify_era_printstring_3_P: Add many printstring attributes with none present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_3_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_3_P -add {{pgo_test_printstring "your mother"} {pgo_test_printstring "was a hamster"}}
        attrlist getvalues [principal show -all gumby_3_P] -type pgo_test_printstring
    } compare_lists {{your mother} {was a hamster}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_3_P}] != 0} {return;}
}

proc principal_modify_era_printstring_4_P {} {
    # Identify the test
    tet_infoline principal_modify_era_printstring_4_P: Add many printstring attributes with many present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_4_P -attribute {pgo_test_printstring "your mother" "was a hamster"}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_4_P -add {pgo_test_printstring "and your father" "smelt of elderberries"}
        attrlist getvalues [principal show -all gumby_4_P] -type pgo_test_printstring
    } compare_lists {{your mother} {was a hamster} {and your father} {smelt of elderberries}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_4_P}] != 0} {return;}
}

proc principal_modify_era_printstring_5_P {} {
    # Identify the test
    tet_infoline principal_modify_era_printstring_5_P: Add one printstring attribute to multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_5_P_1 gumby_5_P_2} -attribute {pgo_test_integer 101}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_5_P_1 gumby_5_P_2} -add {pgo_test_printstring "your mother was a hamster"}
        dcp_attrlist getval [principal show -all {gumby_5_P_1 gumby_5_P_2}] pgo_test_printstring
    } compare_strings {{your mother was a hamster}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_5_P_1 gumby_5_P_2}}] != 0} {return;}
}

proc principal_modify_era_printstring_6_P {} {
    # Identify the test
    tet_infoline principal_modify_era_printstring_6_P: Change to one printstring attribute from one on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_6_P -attribute {pgo_test_printstring "your mother"}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_6_P -change {pgo_test_printstring "was a hamster"}
        attrlist getvalues [principal show -all gumby_6_P] -type pgo_test_printstring
    } compare_lists {{was a hamster}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_6_P}] != 0} {return;}
}

proc principal_modify_era_printstring_7_P {} {
    # Identify the test
    tet_infoline principal_modify_era_printstring_7_P: Change to one printstring attribute from many on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_7_P -attribute {pgo_test_printstring "your mother" "was a hamster"}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_7_P -change {pgo_test_printstring "and your father smelt of elderberries"}
        attrlist getvalues [principal show -all gumby_7_P] -type pgo_test_printstring
    } compare_lists {{and your father smelt of elderberries}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_7_P}] != 0} {return;}
}

proc principal_modify_era_printstring_8_P {} {
    # Identify the test
    tet_infoline principal_modify_era_printstring_8_P: Change to many printstring attributes from one on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_8_P -attribute {pgo_test_printstring "your mother was a hamster"}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_8_P -change {pgo_test_printstring "and your father" "smelt of elderberries"}
        attrlist getvalues [principal show -all gumby_8_P] -type pgo_test_printstring
    } compare_lists {{and your father} {smelt of elderberries}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_8_P}] != 0} {return;}
}

proc principal_modify_era_printstring_9_P {} {
    # Identify the test
    tet_infoline principal_modify_era_printstring_9_P: Change to many printstring attributes from many on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_9_P -attribute {pgo_test_printstring "your mother" "was a hamster"}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_9_P -change {pgo_test_printstring "and your father" "smelt of elderberries"}
        attrlist getvalues [principal show -all gumby_9_P] -type pgo_test_printstring
    } compare_lists {{and your father} {smelt of elderberries}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_9_P}] != 0} {return;}
}

proc principal_modify_era_printstring_10_P {} {
    # Identify the test
    tet_infoline principal_modify_era_printstring_10_P: Change to one printstring attribute from one on multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_10_P_1 gumby_10_P_2} -attribute {pgo_test_printstring "your mother was a hamster"}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_10_P_1 gumby_10_P_2} -change {pgo_test_printstring "and your father smelt of elderberries"}
        attrlist getvalues [principal show -all {gumby_10_P_1 gumby_10_P_2}] -type pgo_test_printstring
    } compare_lists {{and your father smelt of elderberries} {and your father smelt of elderberries}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_10_P_1 gumby_10_P_2}}] != 0} {return;}
}

proc principal_modify_era_printstring_11_P {} {
    # Identify the test
    tet_infoline principal_modify_era_printstring_11_P: Remove one of three printstring attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_11_P -attribute {pgo_test_printstring "your mother" "was" "a hamster"}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_11_P -remove {pgo_test_printstring "was"}
        attrlist getvalues [principal show -all gumby_11_P] -type pgo_test_printstring
    } compare_lists {{your mother} {a hamster}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_11_P}] != 0} {return;}
}

proc principal_modify_era_printstring_12_P {} {
    # Identify the test
    tet_infoline principal_modify_era_printstring_12_P: Remove two of four printstring attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_12_P -attribute {pgo_test_printstring {your mother} {was a hamster} {and your father} {smelt of elderberries}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_12_P -remove {{pgo_test_printstring {was a hamster}} {pgo_test_printstring {and your father}}}
        attrlist getvalues [principal show -all gumby_12_P] -type pgo_test_printstring
    } compare_lists {{your mother} {smelt of elderberries}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_12_P}] != 0} {return;}
}

proc principal_modify_era_printstring_13_P {} {
    # Identify the test
    tet_infoline principal_modify_era_printstring_13_P: Remove two of four printstring attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_13_P -attribute {pgo_test_printstring {your mother} {was a hamster} {and your father} {smelt of elderberries}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_13_P -remove {pgo_test_printstring {was a hamster} {and your father}}
        attrlist getvalues [principal show -all gumby_13_P] -type pgo_test_printstring
    } compare_lists {{your mother} {smelt of elderberries}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_13_P}] != 0} {return;}
}

proc principal_modify_era_printstring_14_P {} {
    # Identify the test
    tet_infoline principal_modify_era_printstring_14_P: Remove the only one printstring attribute from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_14_P -attribute {pgo_test_printstring "blech"}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_14_P -remove {pgo_test_printstring "blech"}
        dcp_multi_values [principal show -all gumby_14_P] {pgo_test_printstring blech}
    } compare_strings {pgo_test_printstring not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_14_P}] != 0} {return;}
}

proc principal_modify_era_printstring_15_P {} {
    # Identify the test
    tet_infoline principal_modify_era_printstring_15_P: Remove all printstring attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_15_P -attribute {pgo_test_printstring "your mother" "was a hamster"}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_15_P -remove {pgo_test_printstring}
        dcp_multi_values [principal show -all gumby_15_P] {{pgo_test_printstring {your mother}} {pgo_test_printstring {was a hamster}}}
    } compare_strings {pgo_test_printstring {your mother} not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_15_P}] != 0} {return;}
}

proc principal_modify_era_printstring_16_P {} {
    # Identify the test
    tet_infoline principal_modify_era_printstring_16_P: Remove all printstring attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_16_P -attribute {pgo_test_printstring "your mother" "was a hamster"}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_16_P -remove {pgo_test_printstring} -types
        dcp_multi_values [principal show -all gumby_16_P] {{pgo_test_printstring {your mother}} {pgo_test_printstring {was a hamster}}}
    } compare_strings {pgo_test_printstring {your mother} not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_16_P}] != 0} {return;}
}

proc principal_modify_era_printstring_17_P {} {
    # Identify the test
    tet_infoline principal_modify_era_printstring_17_P: Remove one of three printstring attributes from multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_17_P_1 gumby_17_P_2} -attribute {pgo_test_printstring "your mother" "was" "a hamster"}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_17_P_1 gumby_17_P_2} -remove {pgo_test_printstring "was"}
        attrlist getvalues [principal show -all {gumby_17_P_1 gumby_17_P_2}] -type pgo_test_printstring
    } compare_lists {{your mother} {a hamster} {your mother} {a hamster}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_17_P_1 gumby_17_P_2}}] != 0} {return;}
}

proc principal_modify_era_bytes_1_P {} {
    # Identify the test
    tet_infoline principal_modify_era_bytes_1_P: Add one bytes attribute with none present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_1_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_1_P -add {pgo_test_bytes "a0a0a0"}
        dcp_attrlist getval [principal show -all gumby_1_P] pgo_test_bytes
    } compare_strings {{a0 a0 a0}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_1_P}] != 0} {return;}
}

proc principal_modify_era_bytes_2_P {} {
    # Identify the test
    tet_infoline principal_modify_era_bytes_2_P: Add one bytes attribute with one present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_2_P -attribute {pgo_test_bytes "a0a0a0"}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_2_P -add {pgo_test_bytes "b1b1b1"}
        attrlist getvalues [principal show -all gumby_2_P] -type pgo_test_bytes
    } compare_lists {{a0 a0 a0} {b1 b1 b1}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_2_P}] != 0} {return;}
}

proc principal_modify_era_bytes_3_P {} {
    # Identify the test
    tet_infoline principal_modify_era_bytes_3_P: Add many bytes attributes with none present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_3_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_3_P -add {{pgo_test_bytes "a0a0a0"} {pgo_test_bytes "b1b1b1"}}
        attrlist getvalues [principal show -all gumby_3_P] -type pgo_test_bytes
    } compare_lists {{a0 a0 a0} {b1 b1 b1}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_3_P}] != 0} {return;}
}

proc principal_modify_era_bytes_4_P {} {
    # Identify the test
    tet_infoline principal_modify_era_bytes_4_P: Add many bytes attributes with many present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_4_P -attribute {pgo_test_bytes "a0a0a0" "b1b1b1"}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_4_P -add {pgo_test_bytes "a1a1a1" "b0b0b0"}
        attrlist getvalues [principal show -all gumby_4_P] -type pgo_test_bytes
    } compare_lists {{a0 a0 a0} {b1 b1 b1} {a1 a1 a1} {b0 b0 b0}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_4_P}] != 0} {return;}
}

proc principal_modify_era_bytes_5_P {} {
    # Identify the test
    tet_infoline principal_modify_era_bytes_5_P: Add one bytes attribute to multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_5_P_1 gumby_5_P_2} -attribute {pgo_test_printstring "blech"}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_5_P_1 gumby_5_P_2} -add {pgo_test_bytes "a0a0a0"}
        dcp_attrlist getval [principal show -all {gumby_5_P_1 gumby_5_P_2}] pgo_test_bytes
    } compare_strings {{a0 a0 a0}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_5_P_1 gumby_5_P_2}}] != 0} {return;}
}

proc principal_modify_era_bytes_6_P {} {
    # Identify the test
    tet_infoline principal_modify_era_bytes_6_P: Change to one bytes attribute from one on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_6_P -attribute {pgo_test_bytes "a0a0a0"}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_6_P -change {pgo_test_bytes "b1b1b1"}
        attrlist getvalues [principal show -all gumby_6_P] -type pgo_test_bytes
    } compare_lists {{b1 b1 b1}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_6_P}] != 0} {return;}
}

proc principal_modify_era_bytes_7_P {} {
    # Identify the test
    tet_infoline principal_modify_era_bytes_7_P: Change to one bytes attribute from many on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_7_P -attribute {pgo_test_bytes "a0a0a0" "b1b1b1"}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_7_P -change {pgo_test_bytes "a1a1a1"}
        attrlist getvalues [principal show -all gumby_7_P] -type pgo_test_bytes
    } compare_lists {{a1 a1 a1}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_7_P}] != 0} {return;}
}

proc principal_modify_era_bytes_8_P {} {
    # Identify the test
    tet_infoline principal_modify_era_bytes_8_P: Change to many bytes attributes from one on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_8_P -attribute {pgo_test_bytes "a0a0a0"}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_8_P -change {pgo_test_bytes "b0b0b0" "b1b1b1"}
        attrlist getvalues [principal show -all gumby_8_P] -type pgo_test_bytes
    } compare_lists {{b0 b0 b0} {b1 b1 b1}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_8_P}] != 0} {return;}
}

proc principal_modify_era_bytes_9_P {} {
    # Identify the test
    tet_infoline principal_modify_era_bytes_9_P: Change to many bytes attributes from many on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_9_P -attribute {pgo_test_bytes "a0a0a0" "a1a1a1"}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_9_P -change {pgo_test_bytes "b0b0b0" "b1b1b1"}
        attrlist getvalues [principal show -all gumby_9_P] -type pgo_test_bytes
    } compare_lists {{b0 b0 b0} {b1 b1 b1}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_9_P}] != 0} {return;}
}

proc principal_modify_era_bytes_10_P {} {
    # Identify the test
    tet_infoline principal_modify_era_bytes_10_P: Change to one bytes attribute from one on multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_10_P_1 gumby_10_P_2} -attribute {pgo_test_bytes "a0a0a0"}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_10_P_1 gumby_10_P_2} -change {pgo_test_bytes "b0b0b0"}
        attrlist getvalues [principal show -all {gumby_10_P_1 gumby_10_P_2}] -type pgo_test_bytes
    } compare_lists {{b0 b0 b0} {b0 b0 b0}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_10_P_1 gumby_10_P_2}}] != 0} {return;}
}

proc principal_modify_era_bytes_11_P {} {
    # Identify the test
    tet_infoline principal_modify_era_bytes_11_P: Remove one of three bytes attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_11_P -attribute {pgo_test_bytes "a0a0a0" "b0b0b0" "a1a1a1"}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_11_P -remove {pgo_test_bytes "b0b0b0"}
        attrlist getvalues [principal show -all gumby_11_P] -type pgo_test_bytes
    } compare_lists {{a0 a0 a0} {a1 a1 a1}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_11_P}] != 0} {return;}
}

proc principal_modify_era_bytes_12_P {} {
    # Identify the test
    tet_infoline principal_modify_era_bytes_12_P: Remove two of four bytes attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_12_P -attribute {pgo_test_bytes "a0a0a0" "a1a1a1" "b0b0b0" "b1b1b1"}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_12_P -remove {{pgo_test_bytes "a1a1a1"} {pgo_test_bytes "b1b1b1"}}
        attrlist getvalues [principal show -all gumby_12_P] -type pgo_test_bytes
    } compare_lists {{a0 a0 a0} {b0 b0 b0}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_12_P}] != 0} {return;}
}

proc principal_modify_era_bytes_13_P {} {
    # Identify the test
    tet_infoline principal_modify_era_bytes_13_P: Remove two of four bytes attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_13_P -attribute {pgo_test_bytes "a0a0a0" "a1a1a1" "b0b0b0" "b1b1b1"}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_13_P -remove {pgo_test_bytes "a1a1a1" "b1b1b1"}
        attrlist getvalues [principal show -all gumby_13_P] -type pgo_test_bytes
    } compare_lists {{a0 a0 a0} {b0 b0 b0}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_13_P}] != 0} {return;}
}

proc principal_modify_era_bytes_14_P {} {
    # Identify the test
    tet_infoline principal_modify_era_bytes_14_P: Remove the only one bytes attribute from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_14_P -attribute {pgo_test_bytes "a0a0a0"}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_14_P -remove {pgo_test_bytes "a0a0a0"}
        dcp_multi_values [principal show -all gumby_14_P] {pgo_test_bytes a0a0a0}
    } compare_strings {pgo_test_bytes not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_14_P}] != 0} {return;}
}

proc principal_modify_era_bytes_15_P {} {
    # Identify the test
    tet_infoline principal_modify_era_bytes_15_P: Remove all bytes attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_15_P -attribute {pgo_test_bytes "a0a0a0" "b1b1b1"}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_15_P -remove {pgo_test_bytes}
        dcp_multi_values [principal show -all gumby_15_P] {{pgo_test_bytes a0a0a0} {pgo_test_bytes b1b1b1}}
    } compare_strings {pgo_test_bytes a0a0a0 not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_15_P}] != 0} {return;}
}

proc principal_modify_era_bytes_16_P {} {
    # Identify the test
    tet_infoline principal_modify_era_bytes_16_P: Remove all bytes attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_16_P -attribute {pgo_test_bytes "a0a0a0" "b1b1b1"}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_16_P -remove {pgo_test_bytes} -types
        dcp_multi_values [principal show -all gumby_16_P] {{pgo_test_bytes a0a0a0} {pgo_test_bytes b1b1b1}}
    } compare_strings {pgo_test_bytes a0a0a0 not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_16_P}] != 0} {return;}
}

proc principal_modify_era_bytes_17_P {} {
    # Identify the test
    tet_infoline principal_modify_era_bytes_17_P: Remove one of three bytes attributes from multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_17_P_1 gumby_17_P_2} -attribute {pgo_test_bytes "a0a0a0" "b0b0b0" "b1b1b1"}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_17_P_1 gumby_17_P_2} -remove {pgo_test_bytes "b0b0b0"}
        attrlist getvalues [principal show -all {gumby_17_P_1 gumby_17_P_2}] -type pgo_test_bytes
    } compare_lists {{a0 a0 a0} {b1 b1 b1} {a0 a0 a0} {b1 b1 b1}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_17_P_1 gumby_17_P_2}}] != 0} {return;}
}

proc principal_modify_era_uuid_1_P {} {
    # Identify the test
    tet_infoline principal_modify_era_uuid_1_P: Add one uuid attribute with none present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_1_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_1_P -add {pgo_test_uuid 47896098-d163-11cd-bfce-080009247461}
        dcp_attrlist getval [principal show -all gumby_1_P] pgo_test_uuid
    } compare_strings 47896098-d163-11cd-bfce-080009247461

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_1_P}] != 0} {return;}
}

proc principal_modify_era_uuid_2_P {} {
    # Identify the test
    tet_infoline principal_modify_era_uuid_2_P: Add one uuid attribute with one present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_2_P -attribute {pgo_test_uuid 47896098-d163-11cd-bfce-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_2_P -add {pgo_test_uuid 655a3d7c-d163-11cd-ba81-080009247461}
        attrlist getvalues [principal show -all gumby_2_P] -type pgo_test_uuid
    } compare_lists {47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_2_P}] != 0} {return;}
}

proc principal_modify_era_uuid_3_P {} {
    # Identify the test
    tet_infoline principal_modify_era_uuid_3_P: Add many uuid attributes with none present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_3_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_3_P -add {{pgo_test_uuid 47896098-d163-11cd-bfce-080009247461} {pgo_test_uuid 655a3d7c-d163-11cd-ba81-080009247461}}
        attrlist getvalues [principal show -all gumby_3_P] -type pgo_test_uuid
    } compare_lists {47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_3_P}] != 0} {return;}
}

proc principal_modify_era_uuid_4_P {} {
    # Identify the test
    tet_infoline principal_modify_era_uuid_4_P: Add many uuid attributes with many present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_4_P -attribute {pgo_test_uuid 47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_4_P -add {pgo_test_uuid 7338bdce-d163-11cd-8eb4-080009247461 7fa874a0-d163-11cd-800e-080009247461}
        attrlist getvalues [principal show -all gumby_4_P] -type pgo_test_uuid
    } compare_lists {47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461 7338bdce-d163-11cd-8eb4-080009247461 7fa874a0-d163-11cd-800e-080009247461}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_4_P}] != 0} {return;}
}

proc principal_modify_era_uuid_5_P {} {
    # Identify the test
    tet_infoline principal_modify_era_uuid_5_P: Add one uuid attribute to multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_5_P_1 gumby_5_P_2} -attribute {pgo_test_printstring "blech"}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_5_P_1 gumby_5_P_2} -add {pgo_test_uuid 47896098-d163-11cd-bfce-080009247461}
        dcp_attrlist getval [principal show -all {gumby_5_P_1 gumby_5_P_2}] pgo_test_uuid
    } compare_strings 47896098-d163-11cd-bfce-080009247461

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_5_P_1 gumby_5_P_2}}] != 0} {return;}
}

proc principal_modify_era_uuid_6_P {} {
    # Identify the test
    tet_infoline principal_modify_era_uuid_6_P: Change to one uuid attribute from one on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_6_P -attribute {pgo_test_uuid 47896098-d163-11cd-bfce-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_6_P -change {pgo_test_uuid 655a3d7c-d163-11cd-ba81-080009247461}
        attrlist getvalues [principal show -all gumby_6_P] -type pgo_test_uuid
    } compare_lists {655a3d7c-d163-11cd-ba81-080009247461}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_6_P}] != 0} {return;}
}

proc principal_modify_era_uuid_7_P {} {
    # Identify the test
    tet_infoline principal_modify_era_uuid_7_P: Change to one uuid attribute from many on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_7_P -attribute {pgo_test_uuid 47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_7_P -change {pgo_test_uuid 7338bdce-d163-11cd-8eb4-080009247461}
        attrlist getvalues [principal show -all gumby_7_P] -type pgo_test_uuid
    } compare_lists {7338bdce-d163-11cd-8eb4-080009247461}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_7_P}] != 0} {return;}
}

proc principal_modify_era_uuid_8_P {} {
    # Identify the test
    tet_infoline principal_modify_era_uuid_8_P: Change to many uuid attributes from one on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_8_P -attribute {pgo_test_uuid 47896098-d163-11cd-bfce-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_8_P -change {pgo_test_uuid 655a3d7c-d163-11cd-ba81-080009247461 7338bdce-d163-11cd-8eb4-080009247461}
        attrlist getvalues [principal show -all gumby_8_P] -type pgo_test_uuid
    } compare_lists {655a3d7c-d163-11cd-ba81-080009247461 7338bdce-d163-11cd-8eb4-080009247461}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_8_P}] != 0} {return;}
}

proc principal_modify_era_uuid_9_P {} {
    # Identify the test
    tet_infoline principal_modify_era_uuid_9_P: Change to many uuid attributes from many on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_9_P -attribute {pgo_test_uuid 47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_9_P -change {pgo_test_uuid 7338bdce-d163-11cd-8eb4-080009247461 7fa874a0-d163-11cd-800e-080009247461}
        attrlist getvalues [principal show -all gumby_9_P] -type pgo_test_uuid
    } compare_lists {7338bdce-d163-11cd-8eb4-080009247461 7fa874a0-d163-11cd-800e-080009247461}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_9_P}] != 0} {return;}
}

proc principal_modify_era_uuid_10_P {} {
    # Identify the test
    tet_infoline principal_modify_era_uuid_10_P: Change to one uuid attribute from one on multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_10_P_1 gumby_10_P_2} -attribute {pgo_test_uuid 47896098-d163-11cd-bfce-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_10_P_1 gumby_10_P_2} -change {pgo_test_uuid 655a3d7c-d163-11cd-ba81-080009247461}
        attrlist getvalues [principal show -all {gumby_10_P_1 gumby_10_P_2}] -type pgo_test_uuid
    } compare_lists {655a3d7c-d163-11cd-ba81-080009247461 655a3d7c-d163-11cd-ba81-080009247461}

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_10_P_1 gumby_10_P_2}}] != 0} {return;}
}

proc principal_modify_era_uuid_11_P {} {
    # Identify the test
    tet_infoline principal_modify_era_uuid_11_P: Remove one of three uuid attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_11_P -attribute {pgo_test_uuid 47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461 7338bdce-d163-11cd-8eb4-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_11_P -remove {pgo_test_uuid 655a3d7c-d163-11cd-ba81-080009247461}
        attrlist getvalues [principal show -all gumby_11_P] -type pgo_test_uuid
    } compare_lists {47896098-d163-11cd-bfce-080009247461 7338bdce-d163-11cd-8eb4-080009247461}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_11_P}] != 0} {return;}
}

proc principal_modify_era_uuid_12_P {} {
    # Identify the test
    tet_infoline principal_modify_era_uuid_12_P: Remove two of four uuid attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_12_P -attribute {pgo_test_uuid 47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461 7338bdce-d163-11cd-8eb4-080009247461 7fa874a0-d163-11cd-800e-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_12_P -remove {{pgo_test_uuid 47896098-d163-11cd-bfce-080009247461} {pgo_test_uuid 7338bdce-d163-11cd-8eb4-080009247461}}
        attrlist getvalues [principal show -all gumby_12_P] -type pgo_test_uuid
    } compare_lists {655a3d7c-d163-11cd-ba81-080009247461 7fa874a0-d163-11cd-800e-080009247461}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_12_P}] != 0} {return;}
}

proc principal_modify_era_uuid_13_P {} {
    # Identify the test
    tet_infoline principal_modify_era_uuid_13_P: Remove two of four uuid attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_13_P -attribute {pgo_test_uuid 47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461 7338bdce-d163-11cd-8eb4-080009247461 7fa874a0-d163-11cd-800e-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_13_P -remove {pgo_test_uuid 47896098-d163-11cd-bfce-080009247461 7338bdce-d163-11cd-8eb4-080009247461}
        attrlist getvalues [principal show -all gumby_13_P] -type pgo_test_uuid
    } compare_lists {655a3d7c-d163-11cd-ba81-080009247461 7fa874a0-d163-11cd-800e-080009247461}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_13_P}] != 0} {return;}
}

proc principal_modify_era_uuid_14_P {} {
    # Identify the test
    tet_infoline principal_modify_era_uuid_14_P: Remove the only one uuid attribute from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_14_P -attribute {pgo_test_uuid 47896098-d163-11cd-bfce-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_14_P -remove {pgo_test_uuid 47896098-d163-11cd-bfce-080009247461}
        dcp_multi_values [principal show -all gumby_14_P] {pgo_test_uuid 47896098-d163-11cd-bfce-080009247461}
    } compare_strings {pgo_test_uuid not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_14_P}] != 0} {return;}
}

proc principal_modify_era_uuid_15_P {} {
    # Identify the test
    tet_infoline principal_modify_era_uuid_15_P: Remove all uuid attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_15_P -attribute {pgo_test_uuid 47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_15_P -remove {pgo_test_uuid}
        dcp_multi_values [principal show -all gumby_15_P] {{pgo_test_uuid 47896098-d163-11cd-bfce-080009247461} {pgo_test_uuid 655a3d7c-d163-11cd-ba81-080009247461}}
    } compare_strings {pgo_test_uuid 47896098-d163-11cd-bfce-080009247461 not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_15_P}] != 0} {return;}
}

proc principal_modify_era_uuid_16_P {} {
    # Identify the test
    tet_infoline principal_modify_era_uuid_16_P: Remove all uuid attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_16_P -attribute {pgo_test_uuid 47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_16_P -remove {pgo_test_uuid} -types
        dcp_multi_values [principal show -all gumby_16_P] {{pgo_test_uuid 47896098-d163-11cd-bfce-080009247461} {pgo_test_uuid 655a3d7c-d163-11cd-ba81-080009247461}}
    } compare_strings {pgo_test_uuid 47896098-d163-11cd-bfce-080009247461 not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_16_P}] != 0} {return;}
}

proc principal_modify_era_uuid_17_P {} {
    # Identify the test
    tet_infoline principal_modify_era_uuid_17_P: Remove one of three uuid attributes from multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_17_P_1 gumby_17_P_2} -attribute {pgo_test_uuid 47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461 7338bdce-d163-11cd-8eb4-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_17_P_1 gumby_17_P_2} -remove {pgo_test_uuid 655a3d7c-d163-11cd-ba81-080009247461}
        attrlist getvalues [principal show -all {gumby_17_P_1 gumby_17_P_2}] -type pgo_test_uuid
    } compare_lists {47896098-d163-11cd-bfce-080009247461 7338bdce-d163-11cd-8eb4-080009247461 47896098-d163-11cd-bfce-080009247461 7338bdce-d163-11cd-8eb4-080009247461}

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_17_P_1 gumby_17_P_2}}] != 0} {return;}
}

proc principal_modify_era_i18n_data_1_P {} {
    # Identify the test
    tet_infoline principal_modify_era_i18n_data_1_P: Add one i18n_data attribute with none present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_1_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_1_P -add {pgo_test_i18n_data {roman8 "aaa"}}
        dcp_attrlist getval [principal show -all gumby_1_P] pgo_test_i18n_data
    } compare_strings {{roman8 {\x61\x61\x61}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_1_P}] != 0} {return;}
}

proc principal_modify_era_i18n_data_2_P {} {
    # Identify the test
    tet_infoline principal_modify_era_i18n_data_2_P: Add one i18n_data attribute with one present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_2_P -attribute {pgo_test_i18n_data {roman8 "aaa"}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_2_P -add {pgo_test_i18n_data {roman8 "bbb"}}
        attrlist getvalues [principal show -all gumby_2_P] -type pgo_test_i18n_data
    } compare_lists {{roman8 {\x61\x61\x61}} {roman8 {\x62\x62\x62}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_2_P}] != 0} {return;}
}

proc principal_modify_era_i18n_data_3_P {} {
    # Identify the test
    tet_infoline principal_modify_era_i18n_data_3_P: Add many i18n_data attributes with none present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_3_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_3_P -add {{pgo_test_i18n_data {roman8 "aaa"}} {pgo_test_i18n_data {roman8 "bbb"}}}
        attrlist getvalues [principal show -all gumby_3_P] -type pgo_test_i18n_data
    } compare_lists {{roman8 {\x61\x61\x61}} {roman8 {\x62\x62\x62}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_3_P}] != 0} {return;}
}

proc principal_modify_era_i18n_data_4_P {} {
    # Identify the test
    tet_infoline principal_modify_era_i18n_data_4_P: Add many i18n_data attributes with many present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_4_P -attribute {pgo_test_i18n_data {roman8 "aaa"} {roman8 "bbb"}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_4_P -add {pgo_test_i18n_data {roman8 "ccc"} {roman8 "ddd"}}
        attrlist getvalues [principal show -all gumby_4_P] -type pgo_test_i18n_data
    } compare_lists {{roman8 {\x61\x61\x61}} {roman8 {\x62\x62\x62}} {roman8 {\x63\x63\x63}} {roman8 {\x64\x64\x64}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_4_P}] != 0} {return;}
}

proc principal_modify_era_i18n_data_5_P {} {
    # Identify the test
    tet_infoline principal_modify_era_i18n_data_5_P: Add one i18n_data attribute to multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_5_P_1 gumby_5_P_2} -attribute {pgo_test_printstring "blech"}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_5_P_1 gumby_5_P_2} -add {pgo_test_i18n_data {roman8 "aaa"}}
        dcp_attrlist getval [principal show -all {gumby_5_P_1 gumby_5_P_2}] pgo_test_i18n_data
    } compare_strings {{roman8 {\x61\x61\x61}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_5_P_1 gumby_5_P_2}}] != 0} {return;}
}

proc principal_modify_era_i18n_data_6_P {} {
    # Identify the test
    tet_infoline principal_modify_era_i18n_data_6_P: Change to one i18n_data attribute from one on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_6_P -attribute {pgo_test_i18n_data {roman8 "aaa"}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_6_P -change {pgo_test_i18n_data {roman8 "bbb"}}
        attrlist getvalues [principal show -all gumby_6_P] -type pgo_test_i18n_data
    } compare_strings {{roman8 {\x62\x62\x62}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_6_P}] != 0} {return;}
}

proc principal_modify_era_i18n_data_7_P {} {
    # Identify the test
    tet_infoline principal_modify_era_i18n_data_7_P: Change to one i18n_data attribute from many on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_7_P -attribute {pgo_test_i18n_data {roman8 "aaa"} {roman8 "bbb"}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_7_P -change {pgo_test_i18n_data {roman8 "ccc"}}
        attrlist getvalues [principal show -all gumby_7_P] -type pgo_test_i18n_data
    } compare_strings {{roman8 {\x63\x63\x63}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_7_P}] != 0} {return;}
}

proc principal_modify_era_i18n_data_8_P {} {
    # Identify the test
    tet_infoline principal_modify_era_i18n_data_8_P: Change to many i18n_data attributes from one on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_8_P -attribute {pgo_test_i18n_data {roman8 "aaa"}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_8_P -change {pgo_test_i18n_data {roman8 "bbb"} {roman8 "ccc"}}
        attrlist getvalues [principal show -all gumby_8_P] -type pgo_test_i18n_data
    } compare_lists {{roman8 {\x62\x62\x62}} {roman8 {\x63\x63\x63}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_8_P}] != 0} {return;}
}

proc principal_modify_era_i18n_data_9_P {} {
    # Identify the test
    tet_infoline principal_modify_era_i18n_data_9_P: Change to many i18n_data attributes from many on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_9_P -attribute {pgo_test_i18n_data {roman8 "aaa"} {roman8 "bbb"}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_9_P -change {pgo_test_i18n_data {roman8 "ccc"} {roman8 "ddd"}}
        attrlist getvalues [principal show -all gumby_9_P] -type pgo_test_i18n_data
    } compare_lists {{roman8 {\x63\x63\x63}} {roman8 {\x64\x64\x64}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_9_P}] != 0} {return;}
}

proc principal_modify_era_i18n_data_10_P {} {
    # Identify the test
    tet_infoline principal_modify_era_i18n_data_10_P: Change to one i18n_data attribute from one on multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_10_P_1 gumby_10_P_2} -attribute {pgo_test_i18n_data {roman8 "aaa"}}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_10_P_1 gumby_10_P_2} -change {pgo_test_i18n_data {roman8 "bbb"}}
        attrlist getvalues [principal show -all {gumby_10_P_1 gumby_10_P_2}] -type pgo_test_i18n_data
    } compare_lists {{roman8 {\x62\x62\x62}} {roman8 {\x62\x62\x62}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_10_P_1 gumby_10_P_2}}] != 0} {return;}
}

proc principal_modify_era_i18n_data_11_P {} {
    # Identify the test
    tet_infoline principal_modify_era_i18n_data_11_P: Remove one of three i18n_data attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_11_P -attribute {pgo_test_i18n_data {roman8 "aaa"} {roman8 "bbb"} {roman8 "ccc"}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_11_P -remove {pgo_test_i18n_data {roman8 "bbb"}}
        attrlist getvalues [principal show -all gumby_11_P] -type pgo_test_i18n_data
    } compare_lists {{roman8 {\x61\x61\x61}} {roman8 {\x63\x63\x63}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_11_P}] != 0} {return;}
}

proc principal_modify_era_i18n_data_12_P {} {
    # Identify the test
    tet_infoline principal_modify_era_i18n_data_12_P: Remove two of four i18n_data attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_12_P -attribute {pgo_test_i18n_data {roman8 "aaa"} {roman8 "bbb"} {roman8 "ccc"} {roman8 "ddd"}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_12_P -remove {{pgo_test_i18n_data {roman8 "aaa"}} {pgo_test_i18n_data {roman8 "ccc"}}}
        attrlist getvalues [principal show -all gumby_12_P] -type pgo_test_i18n_data
    } compare_lists {{roman8 {\x62\x62\x62}} {roman8 {\x64\x64\x64}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_12_P}] != 0} {return;}
}

proc principal_modify_era_i18n_data_13_P {} {
    # Identify the test
    tet_infoline principal_modify_era_i18n_data_13_P: Remove two of four i18n_data attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_13_P -attribute {pgo_test_i18n_data {roman8 "aaa"} {roman8 "bbb"} {roman8 "ccc"} {roman8 "ddd"}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_13_P -remove {pgo_test_i18n_data {roman8 "aaa"} {roman8 "ccc"}}
        attrlist getvalues [principal show -all gumby_13_P] -type pgo_test_i18n_data
    } compare_lists {{roman8 {\x62\x62\x62}} {roman8 {\x64\x64\x64}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_13_P}] != 0} {return;}
}

proc principal_modify_era_i18n_data_14_P {} {
    # Identify the test
    tet_infoline principal_modify_era_i18n_data_14_P: Remove the only one i18n_data attribute from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_14_P -attribute {pgo_test_i18n_data {roman8 "aaa"}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_14_P -remove {pgo_test_i18n_data {roman8 "aaa"}}
        dcp_multi_values [principal show -all gumby_14_P] {pgo_test_i18n_data {roman8 "aaa"}}
    } compare_strings {pgo_test_i18n_data not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_14_P}] != 0} {return;}
}

proc principal_modify_era_i18n_data_15_P {} {
    # Identify the test
    tet_infoline principal_modify_era_i18n_data_15_P: Remove all i18n_data attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_15_P -attribute {pgo_test_i18n_data {roman8 "aaa"} {roman8 "bbb"}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_15_P -remove {pgo_test_i18n_data}
        dcp_multi_values [principal show -all gumby_15_P] {{pgo_test_i18n_data {roman8 "aaa"}} {pgo_test_i18n_data {roman8 "bbb"}}}
    } compare_strings {pgo_test_i18n_data {roman8 "aaa"} not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_15_P}] != 0} {return;}
}

proc principal_modify_era_i18n_data_16_P {} {
    # Identify the test
    tet_infoline principal_modify_era_i18n_data_16_P: Remove all i18n_data attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_16_P -attribute {pgo_test_i18n_data {roman8 "aaa"} {roman8 "bbb"}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_16_P -remove {pgo_test_i18n_data} -types
        dcp_multi_values [principal show -all gumby_16_P] {{pgo_test_i18n_data {roman8 "aaa"}} {pgo_test_i18n_data {roman8 "bbb"}}}
    } compare_strings {pgo_test_i18n_data {roman8 "aaa"} not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_16_P}] != 0} {return;}
}

proc principal_modify_era_i18n_data_17_P {} {
    # Identify the test
    tet_infoline principal_modify_era_i18n_data_17_P: Remove one of three i18n_data attributes from multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_17_P_1 gumby_17_P_2} -attribute {pgo_test_i18n_data {roman8 "aaa"} {roman8 "bbb"} {roman8 "ccc"}}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_17_P_1 gumby_17_P_2} -remove {pgo_test_i18n_data {roman8 "bbb"}}
        attrlist getvalues [principal show -all {gumby_17_P_1 gumby_17_P_2}] -type pgo_test_i18n_data
    } compare_lists {{roman8 {\x61\x61\x61}} {roman8 {\x63\x63\x63}} {roman8 {\x61\x61\x61}} {roman8 {\x63\x63\x63}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_17_P_1 gumby_17_P_2}}] != 0} {return;}
}

proc principal_modify_era_stringarray_1_P {} {
    # Identify the test
    tet_infoline principal_modify_era_stringarray_1_P: Add one stringarray attribute with none present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_1_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_1_P -add {pgo_test_printstring_array 101}
        dcp_attrlist getval [principal show -all gumby_1_P] pgo_test_printstring_array
    } compare_strings 101

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_1_P}] != 0} {return;}
}

proc principal_modify_era_stringarray_2_P {} {
    # Identify the test
    tet_infoline principal_modify_era_stringarray_2_P: Add one stringarray attribute with one present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_2_P -attribute {pgo_test_printstring_array 101}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_2_P -add {pgo_test_printstring_array 102}
        attrlist getvalues [principal show -all gumby_2_P] -type pgo_test_printstring_array
    } compare_lists {101 102}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_2_P}] != 0} {return;}
}

proc principal_modify_era_stringarray_3_P {} {
    # Identify the test
    tet_infoline principal_modify_era_stringarray_3_P: Add many stringarray attributes with none present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_3_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_3_P -add {pgo_test_printstring_array 101 102}
        attrlist getvalues [principal show -all gumby_3_P] -type pgo_test_printstring_array
    } compare_lists {101 102}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_3_P}] != 0} {return;}
}

proc principal_modify_era_stringarray_4_P {} {
    # Identify the test
    tet_infoline principal_modify_era_stringarray_4_P: Add many stringarray attributes with many present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_4_P -attribute {pgo_test_printstring_array 101 102}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_4_P -add {pgo_test_printstring_array 103 104}
        attrlist getvalues [principal show -all gumby_4_P] -type pgo_test_printstring_array
    } compare_lists {101 102 103 104}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_4_P}] != 0} {return;}
}

proc principal_modify_era_stringarray_5_P {} {
    # Identify the test
    tet_infoline principal_modify_era_stringarray_5_P: Add one stringarray attribute to multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_5_P_1 gumby_5_P_2} -attribute {pgo_test_printstring "blech"}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_5_P_1 gumby_5_P_2} -add {pgo_test_printstring_array 101}
        dcp_attrlist getval [principal show -all {gumby_5_P_1 gumby_5_P_2}] pgo_test_printstring_array
    } compare_strings 101

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_5_P_1 gumby_5_P_2}}] != 0} {return;}
}

proc principal_modify_era_stringarray_6_P {} {
    # Identify the test
    tet_infoline principal_modify_era_stringarray_6_P: Change to one stringarray attribute from one on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_6_P -attribute {pgo_test_printstring_array 101}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_6_P -change {pgo_test_printstring_array 102}
        attrlist getvalues [principal show -all gumby_6_P] -type pgo_test_printstring_array
    } compare_lists {102}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_6_P}] != 0} {return;}
}

proc principal_modify_era_stringarray_7_P {} {
    # Identify the test
    tet_infoline principal_modify_era_stringarray_7_P: Change to one stringarray attribute from many on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_7_P -attribute {pgo_test_printstring_array 101 102}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_7_P -change {pgo_test_printstring_array 103}
        attrlist getvalues [principal show -all gumby_7_P] -type pgo_test_printstring_array
    } compare_lists {103}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_7_P}] != 0} {return;}
}

proc principal_modify_era_stringarray_8_P {} {
    # Identify the test
    tet_infoline principal_modify_era_stringarray_8_P: Change to many stringarray attributes from one on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_8_P -attribute {pgo_test_printstring_array 101}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_8_P -change {pgo_test_printstring_array 102 103}
        attrlist getvalues [principal show -all gumby_8_P] -type pgo_test_printstring_array
    } compare_lists {102 103}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_8_P}] != 0} {return;}
}

proc principal_modify_era_stringarray_9_P {} {
    # Identify the test
    tet_infoline principal_modify_era_stringarray_9_P: Change to many stringarray attributes from many on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_9_P -attribute {pgo_test_printstring_array 101 102}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_9_P -change {pgo_test_printstring_array 103 104}
        attrlist getvalues [principal show -all gumby_9_P] -type pgo_test_printstring_array
    } compare_lists {103 104}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_9_P}] != 0} {return;}
}

proc principal_modify_era_stringarray_10_P {} {
    # Identify the test
    tet_infoline principal_modify_era_stringarray_10_P: Change to one stringarray attribute from one on multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_10_P_1 gumby_10_P_2} -attribute {pgo_test_printstring_array 101}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_10_P_1 gumby_10_P_2} -change {pgo_test_printstring_array 102}
        attrlist getvalues [principal show -all {gumby_10_P_1 gumby_10_P_2}] -type pgo_test_printstring_array
    } compare_lists {102 102}

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_10_P_1 gumby_10_P_2}}] != 0} {return;}
}

proc principal_modify_era_stringarray_11_P {} {
    # Identify the test
    tet_infoline principal_modify_era_stringarray_11_P: Remove one of three stringarray attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_11_P -attribute {pgo_test_printstring_array 101 102 103}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_11_P -remove {pgo_test_printstring_array 102}
        attrlist getvalues [principal show -all gumby_11_P] -type pgo_test_printstring_array
    } compare_lists {101 103}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_11_P}] != 0} {return;}
}

proc principal_modify_era_stringarray_12_P {} {
    # Identify the test
    tet_infoline principal_modify_era_stringarray_12_P: Remove two of four stringarray attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_12_P -attribute {pgo_test_printstring_array 101 102 103 104}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_12_P -remove {pgo_test_printstring_array 101 103}
        attrlist getvalues [principal show -all gumby_12_P] -type pgo_test_printstring_array
    } compare_lists {102 104}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_12_P}] != 0} {return;}
}

proc principal_modify_era_stringarray_13_P {} {
    # Identify the test
    tet_infoline principal_modify_era_stringarray_13_P: Remove the only one stringarray attribute from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_13_P -attribute {pgo_test_printstring_array 101}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_13_P -remove {pgo_test_printstring_array 101}
        dcp_multi_values [principal show -all gumby_13_P] {pgo_test_printstring_array 101}
    } compare_strings {pgo_test_printstring_array not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_13_P}] != 0} {return;}
}

proc principal_modify_era_stringarray_14_P {} {
    # Identify the test
    tet_infoline principal_modify_era_stringarray_14_P: Remove all stringarray attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_14_P -attribute {pgo_test_printstring_array 101 102}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_14_P -remove {pgo_test_printstring_array}
        dcp_multi_values [principal show -all gumby_14_P] {pgo_test_printstring_array 101 102}
    } compare_strings {pgo_test_printstring_array not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_14_P}] != 0} {return;}
}

proc principal_modify_era_stringarray_15_P {} {
    # Identify the test
    tet_infoline principal_modify_era_stringarray_15_P: Remove all stringarray attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_15_P -attribute {pgo_test_printstring_array 101 102}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_15_P -remove {pgo_test_printstring_array} -types
        dcp_multi_values [principal show -all gumby_15_P] {pgo_test_printstring_array 101}
    } compare_strings {pgo_test_printstring_array not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_15_P}] != 0} {return;}
}

proc principal_modify_era_stringarray_16_P {} {
    # Identify the test
    tet_infoline principal_modify_era_stringarray_16_P: Remove one of three stringarray attributes from multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_16_P_1 gumby_16_P_2} -attribute {pgo_test_printstring_array 101 102 103}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_16_P_1 gumby_16_P_2} -remove {pgo_test_printstring_array 102}
        attrlist getvalues [principal show -all {gumby_16_P_1 gumby_16_P_2}] -type pgo_test_printstring_array
    } compare_lists {101 103 101 103}

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_16_P_1 gumby_16_P_2}}] != 0} {return;}
}

proc principal_modify_era_attr_set_1_P {} {
    # Identify the test
    tet_infoline principal_modify_era_attr_set_1_P: Add one attr_set attribute with none present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_1_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_1_P -add {pgo_test_attr_set 47896098-d163-11cd-bfce-080009247461}
        dcp_attrlist getval [principal show -all gumby_1_P] pgo_test_attr_set
    } compare_strings 47896098-d163-11cd-bfce-080009247461

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_1_P}] != 0} {return;}
}

proc principal_modify_era_attr_set_2_P {} {
    # Identify the test
    tet_infoline principal_modify_era_attr_set_2_P: Add one attr_set attribute with one present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_2_P -attribute {pgo_test_attr_set 47896098-d163-11cd-bfce-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_2_P -add {pgo_test_attr_set 655a3d7c-d163-11cd-ba81-080009247461}
        attrlist getvalues [principal show -all gumby_2_P] -type pgo_test_attr_set
    } compare_lists {47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_2_P}] != 0} {return;}
}

proc principal_modify_era_attr_set_3_P {} {
    # Identify the test
    tet_infoline principal_modify_era_attr_set_3_P: Add many attr_set attributes with none present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_3_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_3_P -add {pgo_test_attr_set 47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461}
        attrlist getvalues [principal show -all gumby_3_P] -type pgo_test_attr_set
    } compare_lists {47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_3_P}] != 0} {return;}
}

proc principal_modify_era_attr_set_4_P {} {
    # Identify the test
    tet_infoline principal_modify_era_attr_set_4_P: Add many attr_set attributes with many present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_4_P -attribute {pgo_test_attr_set 47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_4_P -add {pgo_test_attr_set 7338bdce-d163-11cd-8eb4-080009247461 7fa874a0-d163-11cd-800e-080009247461}
        attrlist getvalues [principal show -all gumby_4_P] -type pgo_test_attr_set
    } compare_lists {47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461 7338bdce-d163-11cd-8eb4-080009247461 7fa874a0-d163-11cd-800e-080009247461}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_4_P}] != 0} {return;}
}

proc principal_modify_era_attr_set_5_P {} {
    # Identify the test
    tet_infoline principal_modify_era_attr_set_5_P: Add one attr_set attribute to multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_5_P_1 gumby_5_P_2} -attribute {pgo_test_printstring "blech"}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_5_P_1 gumby_5_P_2} -add {pgo_test_attr_set 47896098-d163-11cd-bfce-080009247461}
        dcp_attrlist getval [principal show -all {gumby_5_P_1 gumby_5_P_2}] pgo_test_attr_set
    } compare_strings 47896098-d163-11cd-bfce-080009247461

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_5_P_1 gumby_5_P_2}}] != 0} {return;}
}

proc principal_modify_era_attr_set_6_P {} {
    # Identify the test
    tet_infoline principal_modify_era_attr_set_6_P: Change to one attr_set attribute from one on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_6_P -attribute {pgo_test_attr_set 47896098-d163-11cd-bfce-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_6_P -change {pgo_test_attr_set 655a3d7c-d163-11cd-ba81-080009247461}
        attrlist getvalues [principal show -all gumby_6_P] -type pgo_test_attr_set
    } compare_lists {655a3d7c-d163-11cd-ba81-080009247461}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_6_P}] != 0} {return;}
}

proc principal_modify_era_attr_set_7_P {} {
    # Identify the test
    tet_infoline principal_modify_era_attr_set_7_P: Change to one attr_set attribute from many on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_7_P -attribute {pgo_test_attr_set 47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_7_P -change {pgo_test_attr_set 7338bdce-d163-11cd-8eb4-080009247461}
        attrlist getvalues [principal show -all gumby_7_P] -type pgo_test_attr_set
    } compare_lists {7338bdce-d163-11cd-8eb4-080009247461}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_7_P}] != 0} {return;}
}

proc principal_modify_era_attr_set_8_P {} {
    # Identify the test
    tet_infoline principal_modify_era_attr_set_8_P: Change to many attr_set attributes from one on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_8_P -attribute {pgo_test_attr_set 47896098-d163-11cd-bfce-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_8_P -change {pgo_test_attr_set 655a3d7c-d163-11cd-ba81-080009247461 7338bdce-d163-11cd-8eb4-080009247461}
        attrlist getvalues [principal show -all gumby_8_P] -type pgo_test_attr_set
    } compare_lists {655a3d7c-d163-11cd-ba81-080009247461 7338bdce-d163-11cd-8eb4-080009247461}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_8_P}] != 0} {return;}
}

proc principal_modify_era_attr_set_9_P {} {
    # Identify the test
    tet_infoline principal_modify_era_attr_set_9_P: Change to many attr_set attributes from many on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_9_P -attribute {pgo_test_attr_set 47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_9_P -change {pgo_test_attr_set 7338bdce-d163-11cd-8eb4-080009247461 7fa874a0-d163-11cd-800e-080009247461}
        attrlist getvalues [principal show -all gumby_9_P] -type pgo_test_attr_set
    } compare_lists {7338bdce-d163-11cd-8eb4-080009247461 7fa874a0-d163-11cd-800e-080009247461}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_9_P}] != 0} {return;}
}

proc principal_modify_era_attr_set_10_P {} {
    # Identify the test
    tet_infoline principal_modify_era_attr_set_10_P: Change to one attr_set attribute from one on multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_10_P_1 gumby_10_P_2} -attribute {pgo_test_attr_set 47896098-d163-11cd-bfce-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_10_P_1 gumby_10_P_2} -change {pgo_test_attr_set 655a3d7c-d163-11cd-ba81-080009247461}
        attrlist getvalues [principal show -all {gumby_10_P_1 gumby_10_P_2}] -type pgo_test_attr_set
    } compare_lists {655a3d7c-d163-11cd-ba81-080009247461 655a3d7c-d163-11cd-ba81-080009247461}

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_10_P_1 gumby_10_P_2}}] != 0} {return;}
}

proc principal_modify_era_attr_set_11_P {} {
    # Identify the test
    tet_infoline principal_modify_era_attr_set_11_P: Remove one of three attr_set attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_11_P -attribute {pgo_test_attr_set 47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461 7338bdce-d163-11cd-8eb4-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_11_P -remove {pgo_test_attr_set 655a3d7c-d163-11cd-ba81-080009247461}
        attrlist getvalues [principal show -all gumby_11_P] -type pgo_test_attr_set
    } compare_lists {47896098-d163-11cd-bfce-080009247461 7338bdce-d163-11cd-8eb4-080009247461}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_11_P}] != 0} {return;}
}

proc principal_modify_era_attr_set_12_P {} {
    # Identify the test
    tet_infoline principal_modify_era_attr_set_12_P: Remove two of four attr_set attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_12_P -attribute {pgo_test_attr_set 47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461 7338bdce-d163-11cd-8eb4-080009247461 7fa874a0-d163-11cd-800e-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_12_P -remove {pgo_test_attr_set 47896098-d163-11cd-bfce-080009247461 7338bdce-d163-11cd-8eb4-080009247461}
        attrlist getvalues [principal show -all gumby_12_P] -type pgo_test_attr_set
    } compare_lists {655a3d7c-d163-11cd-ba81-080009247461 7fa874a0-d163-11cd-800e-080009247461}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_12_P}] != 0} {return;}
}

proc principal_modify_era_attr_set_13_P {} {
    # Identify the test
    tet_infoline principal_modify_era_attr_set_13_P: Remove the only one attr_set attribute from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_13_P -attribute {pgo_test_attr_set 47896098-d163-11cd-bfce-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_13_P -remove {pgo_test_attr_set 47896098-d163-11cd-bfce-080009247461}
        dcp_multi_values [principal show -all gumby_13_P] {pgo_test_attr_set 47896098-d163-11cd-bfce-080009247461}
    } compare_strings {pgo_test_attr_set not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_13_P}] != 0} {return;}
}

proc principal_modify_era_attr_set_14_P {} {
    # Identify the test
    tet_infoline principal_modify_era_attr_set_14_P: Remove all attr_set attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_14_P -attribute {pgo_test_attr_set 47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_14_P -remove {pgo_test_attr_set}
        dcp_multi_values [principal show -all gumby_14_P] {pgo_test_attr_set 47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461}
    } compare_strings {pgo_test_attr_set not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_14_P}] != 0} {return;}
}

proc principal_modify_era_attr_set_15_P {} {
    # Identify the test
    tet_infoline principal_modify_era_attr_set_15_P: Remove all attr_set attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_15_P -attribute {pgo_test_attr_set 47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_15_P -remove {pgo_test_attr_set} -types
        dcp_multi_values [principal show -all gumby_15_P] {pgo_test_attr_set 47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461}
    } compare_strings {pgo_test_attr_set not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_15_P}] != 0} {return;}
}

proc principal_modify_era_attr_set_16_P {} {
    # Identify the test
    tet_infoline principal_modify_era_attr_set_16_P: Remove one of three attr_set attributes from multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_16_P_1 gumby_16_P_2} -attribute {pgo_test_attr_set 47896098-d163-11cd-bfce-080009247461 655a3d7c-d163-11cd-ba81-080009247461 7338bdce-d163-11cd-8eb4-080009247461}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_16_P_1 gumby_16_P_2} -remove {pgo_test_attr_set 655a3d7c-d163-11cd-ba81-080009247461}
        attrlist getvalues [principal show -all {gumby_16_P_1 gumby_16_P_2}] -type pgo_test_attr_set
    } compare_lists {47896098-d163-11cd-bfce-080009247461 7338bdce-d163-11cd-8eb4-080009247461 47896098-d163-11cd-bfce-080009247461 7338bdce-d163-11cd-8eb4-080009247461}

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_16_P_1 gumby_16_P_2}}] != 0} {return;}
}

proc principal_modify_era_binding_1_P {} {
    # Identify the test
    tet_infoline principal_modify_era_binding_1_P: Add one binding attribute with none present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_1_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_1_P -add {pgo_test_binding {none {/.:/gumby /.:/pokey}}}
        dcp_attrlist getval [principal show -all gumby_1_P] pgo_test_binding
    } compare_lists {{none {/.:/gumby /.:/pokey}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_1_P}] != 0} {return;}
}

proc principal_modify_era_binding_2_P {} {
    # Identify the test
    tet_infoline principal_modify_era_binding_2_P: Add one binding attribute with one present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_2_P -attribute {pgo_test_binding {none {/.:/gumby /.:/pokey}}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_2_P -add {pgo_test_binding {{dce cell_admin call secret dce} {ncadg_ip_udp:127.0.0.0[1234] ncacn_ip_tcp:127.0.0.0[5678]}}}
        attrlist getvalues [principal show -all gumby_2_P] -type pgo_test_binding
    } compare_lists {{none {/.:/gumby /.:/pokey}} {{dce cell_admin call secret dce} {{ncadg_ip_udp 127.0.0.0 1234} {ncacn_ip_tcp 127.0.0.0 5678}}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_2_P}] != 0} {return;}
}

proc principal_modify_era_binding_3_P {} {
    # Identify the test
    tet_infoline principal_modify_era_binding_3_P: Add many binding attributes with none present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_3_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_3_P -add {{pgo_test_binding {none {/.:/gumby /.:/pokey}}} {pgo_test_binding {{dce cell_admin call secret dce} {ncadg_ip_udp:127.0.0.0[1234] ncacn_ip_tcp:127.0.0.0[5678]}}}}
        attrlist getvalues [principal show -all gumby_3_P] -type pgo_test_binding
    } compare_lists {{none {/.:/gumby /.:/pokey}} {{dce cell_admin call secret dce} {{ncadg_ip_udp 127.0.0.0 1234} {ncacn_ip_tcp 127.0.0.0 5678}}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_3_P}] != 0} {return;}
}

proc principal_modify_era_binding_4_P {} {
    # Identify the test
    tet_infoline principal_modify_era_binding_4_P: Add many binding attributes with many present to one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_4_P -attribute {pgo_test_binding {none {/.:/gumby /.:/pokey}} {{dce cell_admin call secret dce} {ncadg_ip_udp:127.0.0.0[1234] ncacn_ip_tcp:127.0.0.0[5678]}}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_4_P -add {pgo_test_binding {none {ncadg_ip_udp 127.0.0.0 1234}} {{dce cell_admin call secret dce} {/.:/gumby}}}
        attrlist getvalues [principal show -all gumby_4_P] -type pgo_test_binding
    } compare_lists {{none {/.:/gumby /.:/pokey}} {{dce cell_admin call secret dce} {{ncadg_ip_udp 127.0.0.0 1234} {ncacn_ip_tcp 127.0.0.0 5678}}} {none {ncadg_ip_udp 127.0.0.0 1234}} {{dce cell_admin call secret dce} /.:/gumby}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_4_P}] != 0} {return;}
}

proc principal_modify_era_binding_5_P {} {
    # Identify the test
    tet_infoline principal_modify_era_binding_5_P: Add one binding attribute to multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_5_P_1 gumby_5_P_2} -attribute {pgo_test_printstring "blech"}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_5_P_1 gumby_5_P_2} -add {pgo_test_binding {none {/.:/gumby /.:/pokey}}}
        dcp_attrlist getval [principal show -all {gumby_5_P_1 gumby_5_P_2}] pgo_test_binding
    } compare_lists {{none {/.:/gumby /.:/pokey}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_5_P_1 gumby_5_P_2}}] != 0} {return;}
}

proc principal_modify_era_binding_6_P {} {
    # Identify the test
    tet_infoline principal_modify_era_binding_6_P: Change to one binding attribute from one on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_6_P -attribute {pgo_test_binding {none {/.:/gumby /.:/pokey}}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_6_P -change {pgo_test_binding {{dce cell_admin call secret dce} {ncadg_ip_udp:127.0.0.0[1234] ncacn_ip_tcp:127.0.0.0[5678]}}}
        attrlist getvalues [principal show -all gumby_6_P] -type pgo_test_binding
    } compare_lists {{{dce cell_admin call secret dce} {{ncadg_ip_udp 127.0.0.0 1234} {ncacn_ip_tcp 127.0.0.0 5678}}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_6_P}] != 0} {return;}
}

proc principal_modify_era_binding_7_P {} {
    # Identify the test
    tet_infoline principal_modify_era_binding_7_P: Change to one binding attribute from many on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_7_P -attribute {pgo_test_binding {none {/.:/gumby /.:/pokey}} {{dce cell_admin call secret dce} {ncadg_ip_udp:127.0.0.0[1234] ncacn_ip_tcp:127.0.0.0[5678]}}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_7_P -change {pgo_test_binding {none {ncadg_ip_udp 127.0.0.0 1234}}}
        attrlist getvalues [principal show -all gumby_7_P] -type pgo_test_binding
    } compare_lists {{none {ncadg_ip_udp 127.0.0.0 1234}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_7_P}] != 0} {return;}
}

proc principal_modify_era_binding_8_P {} {
    # Identify the test
    tet_infoline principal_modify_era_binding_8_P: Change to many binding attributes from one on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_8_P -attribute {pgo_test_binding {none {/.:/gumby /.:/pokey}}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_8_P -change {pgo_test_binding {{dce cell_admin call secret dce} {ncadg_ip_udp:127.0.0.0[1234] ncacn_ip_tcp:127.0.0.0[5678]}} {none {ncadg_ip_udp 127.0.0.0 1234}}}
        attrlist getvalues [principal show -all gumby_8_P] -type pgo_test_binding
    } compare_lists {{{dce cell_admin call secret dce} {{ncadg_ip_udp 127.0.0.0 1234} {ncacn_ip_tcp 127.0.0.0 5678}}} {none {ncadg_ip_udp 127.0.0.0 1234}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_8_P}] != 0} {return;}
}

proc principal_modify_era_binding_9_P {} {
    # Identify the test
    tet_infoline principal_modify_era_binding_9_P: Change to many binding attributes from many on a single object

    # Setup steps
    if {[dcp_wrap {principal create gumby_9_P -attribute {pgo_test_binding {none {/.:/gumby /.:/pokey}} {{dce cell_admin call secret dce} {ncadg_ip_udp:127.0.0.0[1234] ncacn_ip_tcp:127.0.0.0[5678]}}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_9_P -change {pgo_test_binding {none {ncadg_ip_udp 127.0.0.0 1234}} {{dce cell_admin call secret dce} {/.:/gumby}}}
        attrlist getvalues [principal show -all gumby_9_P] -type pgo_test_binding
    } compare_lists {{none {ncadg_ip_udp 127.0.0.0 1234}} {{dce cell_admin call secret dce} /.:/gumby}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_9_P}] != 0} {return;}
}

proc principal_modify_era_binding_10_P {} {
    # Identify the test
    tet_infoline principal_modify_era_binding_10_P: Change to one binding attribute from one on multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_10_P_1 gumby_10_P_2} -attribute {pgo_test_binding {none {/.:/gumby /.:/pokey}}}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_10_P_1 gumby_10_P_2} -change {pgo_test_binding {{dce cell_admin call secret dce} {ncadg_ip_udp:127.0.0.0[1234] ncacn_ip_tcp:127.0.0.0[5678]}}}
        attrlist getvalues [principal show -all {gumby_10_P_1 gumby_10_P_2}] -type pgo_test_binding
    } compare_lists {{{dce cell_admin call secret dce} {{ncadg_ip_udp 127.0.0.0 1234} {ncacn_ip_tcp 127.0.0.0 5678}}} {{dce cell_admin call secret dce} {{ncadg_ip_udp 127.0.0.0 1234} {ncacn_ip_tcp 127.0.0.0 5678}}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_10_P_1 gumby_10_P_2}}] != 0} {return;}
}

proc principal_modify_era_binding_11_P {} {
    # Identify the test
    tet_infoline principal_modify_era_binding_11_P: Remove one of three binding attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_11_P -attribute {pgo_test_binding {none {/.:/gumby /.:/pokey}} {{dce cell_admin call secret dce} {ncadg_ip_udp:127.0.0.0[1234] ncacn_ip_tcp:127.0.0.0[5678]}} {none {ncadg_ip_udp 127.0.0.0 1234}}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_11_P -remove {pgo_test_binding {{dce cell_admin call secret dce} {ncadg_ip_udp:127.0.0.0[1234] ncacn_ip_tcp:127.0.0.0[5678]}}}
        attrlist getvalues [principal show -all gumby_11_P] -type pgo_test_binding
    } compare_lists {{none {/.:/gumby /.:/pokey}} {none {ncadg_ip_udp 127.0.0.0 1234}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_11_P}] != 0} {return;}
}

proc principal_modify_era_binding_12_P {} {
    # Identify the test
    tet_infoline principal_modify_era_binding_12_P: Remove two of four binding attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_12_P -attribute {pgo_test_binding {none {/.:/gumby /.:/pokey}} {{dce cell_admin call secret dce} {ncadg_ip_udp:127.0.0.0[1234] ncacn_ip_tcp:127.0.0.0[5678]}} {none {ncadg_ip_udp 127.0.0.0 1234}} {{dce cell_admin call secret dce} {/.:/gumby}}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_12_P -remove {{pgo_test_binding {none {/.:/gumby /.:/pokey}}} {pgo_test_binding {none {ncadg_ip_udp 127.0.0.0 1234}}}}
        attrlist getvalues [principal show -all gumby_12_P] -type pgo_test_binding
    } compare_lists {{{dce cell_admin call secret dce} {{ncadg_ip_udp 127.0.0.0 1234} {ncacn_ip_tcp 127.0.0.0 5678}}} {{dce cell_admin call secret dce} /.:/gumby}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_12_P}] != 0} {return;}
}

proc principal_modify_era_binding_13_P {} {
    # Identify the test
    tet_infoline principal_modify_era_binding_13_P: Remove two of four binding attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_13_P -attribute {pgo_test_binding {none {/.:/gumby /.:/pokey}} {{dce cell_admin call secret dce} {ncadg_ip_udp:127.0.0.0[1234] ncacn_ip_tcp:127.0.0.0[5678]}} {none {ncadg_ip_udp 127.0.0.0 1234}} {{dce cell_admin call secret dce} {/.:/gumby}}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_13_P -remove {pgo_test_binding {none {/.:/gumby /.:/pokey}} {none {ncadg_ip_udp 127.0.0.0 1234}}}
        attrlist getvalues [principal show -all gumby_13_P] -type pgo_test_binding
    } compare_lists {{{dce cell_admin call secret dce} {{ncadg_ip_udp 127.0.0.0 1234} {ncacn_ip_tcp 127.0.0.0 5678}}} {{dce cell_admin call secret dce} /.:/gumby}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_13_P}] != 0} {return;}
}

proc principal_modify_era_binding_14_P {} {
    # Identify the test
    tet_infoline principal_modify_era_binding_14_P: Remove the only one binding attribute from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_14_P -attribute {pgo_test_binding {none {/.:/gumby /.:/pokey}}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_14_P -remove {pgo_test_binding {none {/.:/gumby /.:/pokey}}}
        dcp_multi_values [principal show -all gumby_14_P] {pgo_test_binding {none {/.:/gumby /.:/pokey}}}
    } compare_strings {pgo_test_binding not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_14_P}] != 0} {return;}
}

proc principal_modify_era_binding_15_P {} {
    # Identify the test
    tet_infoline principal_modify_era_binding_15_P: Remove all binding attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_15_P -attribute {pgo_test_binding {none {/.:/gumby /.:/pokey}} {{dce cell_admin call secret dce} {ncadg_ip_udp:127.0.0.0[1234] ncacn_ip_tcp:127.0.0.0[5678]}}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_15_P -remove {pgo_test_binding}
        dcp_multi_values [principal show -all gumby_15_P] {{pgo_test_binding {none {/.:/gumby /.:/pokey}}} {pgo_test_binding {{dce cell_admin call secret dce} {ncadg_ip_udp:127.0.0.0[1234] ncacn_ip_tcp:127.0.0.0[5678]}}}}
    } compare_strings {pgo_test_binding {none {/.:/gumby /.:/pokey}} not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_15_P}] != 0} {return;}
}

proc principal_modify_era_binding_16_P {} {
    # Identify the test
    tet_infoline principal_modify_era_binding_16_P: Remove all binding attributes from one object

    # Setup steps
    if {[dcp_wrap {principal create gumby_16_P -attribute {pgo_test_binding {none {/.:/gumby /.:/pokey}} {{dce cell_admin call secret dce} {ncadg_ip_udp:127.0.0.0[1234] ncacn_ip_tcp:127.0.0.0[5678]}}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_16_P -remove {pgo_test_binding} -types
        dcp_multi_values [principal show -all gumby_16_P] {{pgo_test_binding {none {/.:/gumby /.:/pokey}}} {pgo_test_binding {{dce cell_admin call secret dce} {ncadg_ip_udp:127.0.0.0[1234] ncacn_ip_tcp:127.0.0.0[5678]}}}}
    } compare_strings {pgo_test_binding {none {/.:/gumby /.:/pokey}} not found}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_16_P}] != 0} {return;}
}

proc principal_modify_era_binding_17_P {} {
    # Identify the test
    tet_infoline principal_modify_era_binding_17_P: Remove one of three binding attributes from multiple objects

    # Setup steps
    if {[dcp_wrap {principal create {gumby_17_P_1 gumby_17_P_2} -attribute {pgo_test_binding {none {/.:/gumby /.:/pokey}} {{dce cell_admin call secret dce} {ncadg_ip_udp:127.0.0.0[1234] ncacn_ip_tcp:127.0.0.0[5678]}} {none {ncadg_ip_udp 127.0.0.0 1234}}}}] != 0} {return;}

    # Run the test
    test {principal modify {gumby_17_P_1 gumby_17_P_2} -remove {pgo_test_binding {{dce cell_admin call secret dce} {ncadg_ip_udp:127.0.0.0[1234] ncacn_ip_tcp:127.0.0.0[5678]}}}
        attrlist getvalues [principal show -all {gumby_17_P_1 gumby_17_P_2}] -type pgo_test_binding
    } compare_lists {{none {/.:/gumby /.:/pokey}} {none {ncadg_ip_udp 127.0.0.0 1234}} {none {/.:/gumby /.:/pokey}} {none {ncadg_ip_udp 127.0.0.0 1234}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {gumby_17_P_1 gumby_17_P_2}}] != 0} {return;}
}

proc principal_modify_era_any_1_P {} {
    # Identify the test
    tet_infoline principal_modify_era_any_1_P: Add an any attribute

    # Setup steps
    if {[dcp_wrap {principal create gumby_1_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_1_P -add {pgo_test_any integer 101 102}
        attrlist getvalues [principal show -all gumby_1_P] -type pgo_test_any
    } compare_strings {101 102}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_1_P}] != 0} {return;}
}

proc principal_modify_era_any_2_P {} {
    # Identify the test
    tet_infoline principal_modify_era_any_2_P: Add an any attribute

    # Setup steps
    if {[dcp_wrap {principal create gumby_2_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_2_P -add {pgo_test_any void present}
        attrlist getvalues [principal show -all gumby_2_P] -type pgo_test_any
    } compare_strings present

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_2_P}] != 0} {return;}
}

proc principal_modify_era_any_3_P {} {
    # Identify the test
    tet_infoline principal_modify_era_any_3_P: Add an any attribute

    # Setup steps
    if {[dcp_wrap {principal create gumby_3_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_3_P -add {pgo_test_any printstring "your mother was a hamster"}
        attrlist getvalues [principal show -all gumby_3_P] -type pgo_test_any
    } compare_strings {{your mother was a hamster}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_3_P}] != 0} {return;}
}

proc principal_modify_era_any_4_P {} {
    # Identify the test
    tet_infoline principal_modify_era_any_4_P: Add an any attribute

    # Setup steps
    if {[dcp_wrap {principal create gumby_4_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_4_P -add {pgo_test_any stringarray "and your father smelt of elderberries"}
        attrlist getvalues [principal show -all gumby_4_P] -type pgo_test_any
    } compare_strings {{and your father smelt of elderberries}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_4_P}] != 0} {return;}
}

proc principal_modify_era_any_5_P {} {
    # Identify the test
    tet_infoline principal_modify_era_any_5_P: Add an any attribute

    # Setup steps
    if {[dcp_wrap {principal create gumby_5_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_5_P -add {pgo_test_any byte a0a0a0}
        attrlist getvalues [principal show -all gumby_5_P] -type pgo_test_any
    } compare_strings {{a0 a0 a0}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_5_P}] != 0} {return;}
}

proc principal_modify_era_any_6_P {} {
    # Identify the test
    tet_infoline principal_modify_era_any_6_P: Add an any attribute

    # Setup steps
    if {[dcp_wrap {principal create gumby_6_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_6_P -add {pgo_test_any i18ndata {roman8 "abcdef"}}
        attrlist getvalues [principal show -all gumby_6_P] -type pgo_test_any
    } compare_strings {{roman8 {\x61\x62\x63\x64\x65\x66}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_6_P}] != 0} {return;}
}

proc principal_modify_era_any_7_P {} {
    # Identify the test
    tet_infoline principal_modify_era_any_7_P: Add an any attribute

    # Setup steps
    if {[dcp_wrap {principal create gumby_7_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_7_P -add {pgo_test_any uuid 291e5274-7887-11cd-98a5-0800092464af}
        attrlist getvalues [principal show -all gumby_7_P] -type pgo_test_any
    } compare_strings 291e5274-7887-11cd-98a5-0800092464af

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_7_P}] != 0} {return;}
}

proc principal_modify_era_any_8_P {} {
    # Identify the test
    tet_infoline principal_modify_era_any_8_P: Add an any attribute

    # Setup steps
    if {[dcp_wrap {principal create gumby_8_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_8_P -add {pgo_test_any attrset 291e5274-7887-11cd-98a5-0800092464af}
        attrlist getvalues [principal show -all gumby_8_P] -type pgo_test_any
    } compare_strings 291e5274-7887-11cd-98a5-0800092464af

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_8_P}] != 0} {return;}
}

proc principal_modify_era_any_9_P {} {
    # Identify the test
    tet_infoline principal_modify_era_any_9_P: Add an any attribute

    # Setup steps
    if {[dcp_wrap {global SECMASTER}] != 0} {return;}
    if {[dcp_wrap {set fbind "{dce cell_admin call secret dce} $SECMASTER"}] != 0} {return;}
    if {[dcp_wrap {set attrib [list "pgo_test_any binding [list $fbind]"]}] != 0} {return;}
    if {[dcp_wrap {principal create gumby_9_P}] != 0} {return;}

    # Run the test
    test {principal modify gumby_9_P -add $attrib
        attrlist getvalues [principal show -all gumby_9_P] -type pgo_test_any
    } compare_lists [format "{{dce cell_admin call secret dce} %s}" $SECMASTER]

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_9_P}] != 0} {return;}
}

proc principal_modify_era_any_10_P {} {
    # Identify the test
    tet_infoline principal_modify_era_any_10_P: Add an any attribute with some already present

    # Setup steps
    if {[dcp_wrap {principal create gumby_10_P -attr {pgo_test_any integer 101 102}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_10_P -add {pgo_test_any integer 103 104}
        attrlist getvalues [principal show -all gumby_10_P] -type pgo_test_any
    } compare_strings {101 102 103 104}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_10_P}] != 0} {return;}
}

proc principal_modify_era_any_11_P {} {
    # Identify the test
    tet_infoline principal_modify_era_any_11_P: Add an any attribute with some already present

    # Setup steps
    if {[dcp_wrap {principal create gumby_11_P -attr {pgo_test_any printstring "aaa" "bbb"}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_11_P -add {{pgo_test_any printstring "ccc"} {pgo_test_any printstring "ddd" "eee"}}
        attrlist getvalues [principal show -all gumby_11_P] -type pgo_test_any
    } compare_lists {aaa bbb ccc ddd eee}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_11_P}] != 0} {return;}
}

proc principal_modify_era_any_12_P {} {
    # Identify the test
    tet_infoline principal_modify_era_any_12_P: Add an any attribute with some already present

    # Setup steps
    if {[dcp_wrap {principal create gumby_12_P -attr {pgo_test_any uuid 291e5274-7887-11cd-98a5-0800092464ab}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_12_P -add {{pgo_test_any uuid 291e5274-7887-11cd-98a5-0800092464ac 291e5274-7887-11cd-98a5-0800092464ad}}
        attrlist getvalues [principal show -all gumby_12_P] -type pgo_test_any
    } compare_lists {291e5274-7887-11cd-98a5-0800092464ab 291e5274-7887-11cd-98a5-0800092464ac 291e5274-7887-11cd-98a5-0800092464ad}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_12_P}] != 0} {return;}
}

proc principal_modify_era_any_13_P {} {
    # Identify the test
    tet_infoline principal_modify_era_any_13_P: Add an any attribute with some already present

    # Setup steps
    if {[dcp_wrap {principal create gumby_13_P -attr {{pgo_test_any int 101 102} {pgo_test_any printstring "aaa"}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_13_P -add {{pgo_test_any uuid 291e5274-7887-11cd-98a5-0800092464ac} {pgo_test_any int 103} {pgo_test_printstring "aaa" "bbb" "ccc"} {pgo_test_printstring_array "blah" "blah"}}
        attrlist getvalues [principal show -all gumby_13_P] -type pgo_test_any
    } compare_lists {291e5274-7887-11cd-98a5-0800092464ac 101 102 103 aaa}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_13_P}] != 0} {return;}
}

proc principal_modify_era_any_14_P {} {
    # Identify the test
    tet_infoline principal_modify_era_any_14_P: Add an any attribute with some already present

    # Setup steps
    if {[dcp_wrap {principal create gumby_14_P -attr {pgo_test_any print "your mother" "was a hamster"}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_14_P -add {{pgo_test_any uuid 291e5274-7887-11cd-98a5-0800092464ac} {pgo_test_any int 103} {pgo_test_printstring "aaa" "bbb" "ccc"} {pgo_test_printstring_array "blah one" "blah two"}}
        attrlist getvalues [principal show -all gumby_14_P] -type pgo_test_any
    } compare_lists {291e5274-7887-11cd-98a5-0800092464ac 103 {your mother} {was a hamster}}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_14_P}] != 0} {return;}
}

proc principal_modify_era_any_15_P {} {
    # Identify the test
    tet_infoline principal_modify_era_any_15_P: Change an any attribute

    # Setup steps
    if {[dcp_wrap {principal create gumby_15_P -attr {{pgo_test_any integer 101 102} {pgo_test_any printstr "aaa" "bbb" "ccc"}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_15_P -change {pgo_test_any integer 103 104}
        attrlist getvalues [principal show -all gumby_15_P] -type pgo_test_any
    } compare_lists {103 104 aaa bbb ccc}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_15_P}] != 0} {return;}
}

proc principal_modify_era_any_16_P {} {
    # Identify the test
    tet_infoline principal_modify_era_any_16_P: Change an any attribute

    # Setup steps
    if {[dcp_wrap {principal create gumby_16_P -attr {{pgo_test_any integer 101 102} {pgo_test_any printstr "aaa" "bbb" "ccc"}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_16_P -change {{pgo_test_any integer 103 104} {pgo_test_any printstring "ccc" "ddd"}}
        attrlist getvalues [principal show -all gumby_16_P] -type pgo_test_any
    } compare_lists {103 104 ccc ddd}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_16_P}] != 0} {return;}
}

proc principal_modify_era_any_17_P {} {
    # Identify the test
    tet_infoline principal_modify_era_any_17_P: Change an any attribute

    # Setup steps
    if {[dcp_wrap {principal create gumby_17_P -attr {{pgo_test_any integer 101 102} {pgo_test_any uuid 291e5274-7887-11cd-98a5-0800092464ab}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_17_P -change {pgo_test_any uuid 291e5274-7887-11cd-98a5-0800092464bb}
        attrlist getvalues [principal show -all gumby_17_P] -type pgo_test_any
    } compare_lists {291e5274-7887-11cd-98a5-0800092464bb 101 102}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_17_P}] != 0} {return;}
}

proc principal_modify_era_any_18_P {} {
    # Identify the test
    tet_infoline principal_modify_era_any_18_P: Delete all any attributes

    # Setup steps
    if {[dcp_wrap {principal create gumby_18_P -attr {{pgo_test_any integer 101 102} {pgo_test_any printstring "aaa" "bbb"} {pgo_test_any uuid 291e5274-7887-11cd-98a5-0800092464ab}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_18_P -remove {pgo_test_any}
        attrlist getvalues [principal show -all gumby_18_P] -type pgo_test_any
    } compare_strings {}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_18_P}] != 0} {return;}
}

proc principal_modify_era_any_19_P {} {
    # Identify the test
    tet_infoline principal_modify_era_any_19_P: Delete all any attributes

    # Setup steps
    if {[dcp_wrap {principal create gumby_19_P -attr {{pgo_test_any integer 101 102} {pgo_test_any printstring "aaa" "bbb"} {pgo_test_any uuid 291e5274-7887-11cd-98a5-0800092464ab}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_19_P -remove {pgo_test_any} -types
        attrlist getvalues [principal show -all gumby_19_P] -type pgo_test_any
    } compare_strings {}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_19_P}] != 0} {return;}
}

proc principal_modify_era_any_20_P {} {
    # Identify the test
    tet_infoline principal_modify_era_any_20_P: Delete all any attributes with encoding type integer

    # Setup steps
    if {[dcp_wrap {principal create gumby_20_P -attr {{pgo_test_any integer 101 102} {pgo_test_any printstring "aaa" "bbb"} {pgo_test_any uuid 291e5274-7887-11cd-98a5-0800092464ab}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_20_P -remove {pgo_test_any integer} -types
        attrlist getvalues [principal show -all gumby_20_P] -type pgo_test_any
    } compare_lists {aaa bbb 291e5274-7887-11cd-98a5-0800092464ab}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_20_P}] != 0} {return;}
}

proc principal_modify_era_any_21_P {} {
    # Identify the test
    tet_infoline principal_modify_era_any_21_P: Delete one any attribute with encoding type integer

    # Setup steps
    if {[dcp_wrap {principal create gumby_21_P -attr {{pgo_test_any integer 101 102} {pgo_test_any printstring "aaa" "bbb"} {pgo_test_any uuid 291e5274-7887-11cd-98a5-0800092464ab}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_21_P -remove {pgo_test_any integer 102}
        attrlist getvalues [principal show -all gumby_21_P] -type pgo_test_any
    } compare_lists {101 aaa bbb 291e5274-7887-11cd-98a5-0800092464ab}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_21_P}] != 0} {return;}
}

proc principal_modify_era_any_22_P {} {
    # Identify the test
    tet_infoline principal_modify_era_any_22_P: Delete two any attributes with different encoding types

    # Setup steps
    if {[dcp_wrap {principal create gumby_22_P -attr {{pgo_test_any integer 101 102} {pgo_test_any printstring "aaa" "bbb"} {pgo_test_any uuid 291e5274-7887-11cd-98a5-0800092464ab}}}] != 0} {return;}

    # Run the test
    test {principal modify gumby_22_P -remove {{pgo_test_any integer 101} {pgo_test_any printstring "bbb"}}
        attrlist getvalues [principal show -all gumby_22_P] -type pgo_test_any
    } compare_lists {102 aaa 291e5274-7887-11cd-98a5-0800092464ab}

    # Cleanup steps
    if {[dcp_wrap {principal delete gumby_22_P}] != 0} {return;}
}

set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 ic25 ic26 ic27 ic28 ic29 ic30 ic31 ic32 ic33 ic34 ic35 ic36 ic37 ic38 ic39 ic40 ic41 ic42 ic43 ic44 ic45 ic46 ic47 ic48 ic49 ic50 ic51 ic52 ic53 ic54 ic55 ic56 ic57 ic58 ic59 ic60 ic61 ic62 ic63 ic64 ic65 ic66 ic67 ic68 ic69 ic70 ic71 ic72 ic73 ic74 ic75 ic76 ic77 ic78 ic79 ic80 ic81 ic82 ic83 ic84 ic85 ic86 ic87 ic88 ic89 ic90 ic91 ic92 ic93 ic94 ic95 ic96 ic97 ic98 ic99 ic100 ic101 ic102 ic103 ic104 ic105 ic106 ic107 ic108 ic109 ic110 ic111 ic112 ic113 ic114 ic115 ic116 ic117 ic118 ic119 ic120 ic121 ic122 ic123 ic124 c125 ic126 ic127 ic128 ic129 ic130 ic131 ic132 ic133 ic134 ic135 ic136 ic137 ic138 ic139 ic140 ic141 ic142 ic143 ic144 ic145 ic146 ic147 ic148 ic149 ic150 ic151 ic152 ic153 ic154 ic155 ic156 ic157 ic158 ic159 ic160 ic161 ic162 ic163 ic164 ic165 ic166 "
set ic1 "principal_modify_era_void_1_P"
set ic2 "principal_modify_era_void_2_P"
set ic3 "principal_modify_era_void_3_P"
set ic4 "principal_modify_era_void_4_P"
set ic5 "principal_modify_era_void_5_P"
set ic6 "principal_modify_era_void_6_P"
set ic7 "principal_modify_era_void_7_P"
set ic8 "principal_modify_era_void_8_P"
set ic9 "principal_modify_era_void_9_P"
set ic10 "principal_modify_era_void_10_P"
set ic11 "principal_modify_era_integer_1_P"
set ic12 "principal_modify_era_integer_2_P"
set ic13 "principal_modify_era_integer_3_P"
set ic14 "principal_modify_era_integer_4_P"
set ic15 "principal_modify_era_integer_5_P"
set ic16 "principal_modify_era_integer_6_P"
set ic17 "principal_modify_era_integer_7_P"
set ic18 "principal_modify_era_integer_8_P"
set ic19 "principal_modify_era_integer_9_P"
set ic20 "principal_modify_era_integer_10_P"
set ic21 "principal_modify_era_integer_11_P"
set ic22 "principal_modify_era_integer_12_P"
set ic23 "principal_modify_era_integer_13_P"
set ic24 "principal_modify_era_integer_14_P"
set ic25 "principal_modify_era_integer_15_P"
set ic26 "principal_modify_era_integer_16_P"
set ic27 "principal_modify_era_integer_17_P"
set ic28 "principal_modify_era_printstring_1_P"
set ic29 "principal_modify_era_printstring_2_P"
set ic30 "principal_modify_era_printstring_3_P"
set ic31 "principal_modify_era_printstring_4_P"
set ic32 "principal_modify_era_printstring_5_P"
set ic33 "principal_modify_era_printstring_6_P"
set ic34 "principal_modify_era_printstring_7_P"
set ic35 "principal_modify_era_printstring_8_P"
set ic36 "principal_modify_era_printstring_9_P"
set ic37 "principal_modify_era_printstring_10_P"
set ic38 "principal_modify_era_printstring_11_P"
set ic39 "principal_modify_era_printstring_12_P"
set ic40 "principal_modify_era_printstring_13_P"
set ic41 "principal_modify_era_printstring_14_P"
set ic42 "principal_modify_era_printstring_15_P"
set ic43 "principal_modify_era_printstring_16_P"
set ic44 "principal_modify_era_printstring_17_P"
set ic45 "principal_modify_era_bytes_1_P"
set ic46 "principal_modify_era_bytes_2_P"
set ic47 "principal_modify_era_bytes_3_P"
set ic48 "principal_modify_era_bytes_4_P"
set ic49 "principal_modify_era_bytes_5_P"
set ic50 "principal_modify_era_bytes_6_P"
set ic51 "principal_modify_era_bytes_7_P"
set ic52 "principal_modify_era_bytes_8_P"
set ic53 "principal_modify_era_bytes_9_P"
set ic54 "principal_modify_era_bytes_10_P"
set ic55 "principal_modify_era_bytes_11_P"
set ic56 "principal_modify_era_bytes_12_P"
set ic57 "principal_modify_era_bytes_13_P"
set ic58 "principal_modify_era_bytes_14_P"
set ic59 "principal_modify_era_bytes_15_P"
set ic60 "principal_modify_era_bytes_16_P"
set ic61 "principal_modify_era_bytes_17_P"
set ic62 "principal_modify_era_uuid_1_P"
set ic63 "principal_modify_era_uuid_2_P"
set ic64 "principal_modify_era_uuid_3_P"
set ic65 "principal_modify_era_uuid_4_P"
set ic66 "principal_modify_era_uuid_5_P"
set ic67 "principal_modify_era_uuid_6_P"
set ic68 "principal_modify_era_uuid_7_P"
set ic69 "principal_modify_era_uuid_8_P"
set ic70 "principal_modify_era_uuid_9_P"
set ic71 "principal_modify_era_uuid_10_P"
set ic72 "principal_modify_era_uuid_11_P"
set ic73 "principal_modify_era_uuid_12_P"
set ic74 "principal_modify_era_uuid_13_P"
set ic75 "principal_modify_era_uuid_14_P"
set ic76 "principal_modify_era_uuid_15_P"
set ic77 "principal_modify_era_uuid_16_P"
set ic78 "principal_modify_era_uuid_17_P"
set ic79 "principal_modify_era_i18n_data_1_P"
set ic80 "principal_modify_era_i18n_data_2_P"
set ic81 "principal_modify_era_i18n_data_3_P"
set ic82 "principal_modify_era_i18n_data_4_P"
set ic83 "principal_modify_era_i18n_data_5_P"
set ic84 "principal_modify_era_i18n_data_6_P"
set ic85 "principal_modify_era_i18n_data_7_P"
set ic86 "principal_modify_era_i18n_data_8_P"
set ic87 "principal_modify_era_i18n_data_9_P"
set ic88 "principal_modify_era_i18n_data_10_P"
set ic89 "principal_modify_era_i18n_data_11_P"
set ic90 "principal_modify_era_i18n_data_12_P"
set ic91 "principal_modify_era_i18n_data_13_P"
set ic92 "principal_modify_era_i18n_data_14_P"
set ic93 "principal_modify_era_i18n_data_15_P"
set ic94 "principal_modify_era_i18n_data_16_P"
set ic95 "principal_modify_era_i18n_data_17_P"
set ic96 "principal_modify_era_stringarray_1_P"
set ic97 "principal_modify_era_stringarray_2_P"
set ic98 "principal_modify_era_stringarray_3_P"
set ic99 "principal_modify_era_stringarray_4_P"
set ic100 "principal_modify_era_stringarray_5_P"
set ic101 "principal_modify_era_stringarray_6_P"
set ic102 "principal_modify_era_stringarray_7_P"
set ic103 "principal_modify_era_stringarray_8_P"
set ic104 "principal_modify_era_stringarray_9_P"
set ic105 "principal_modify_era_stringarray_10_P"
set ic106 "principal_modify_era_stringarray_11_P"
set ic107 "principal_modify_era_stringarray_12_P"
set ic108 "principal_modify_era_stringarray_13_P"
set ic109 "principal_modify_era_stringarray_14_P"
set ic110 "principal_modify_era_stringarray_15_P"
set ic111 "principal_modify_era_stringarray_16_P"
set ic112 "principal_modify_era_attr_set_1_P"
set ic113 "principal_modify_era_attr_set_2_P"
set ic114 "principal_modify_era_attr_set_3_P"
set ic115 "principal_modify_era_attr_set_4_P"
set ic116 "principal_modify_era_attr_set_5_P"
set ic117 "principal_modify_era_attr_set_6_P"
set ic118 "principal_modify_era_attr_set_7_P"
set ic119 "principal_modify_era_attr_set_8_P"
set ic120 "principal_modify_era_attr_set_9_P"
set ic121 "principal_modify_era_attr_set_10_P"
set ic122 "principal_modify_era_attr_set_11_P"
set ic123 "principal_modify_era_attr_set_12_P"
set ic124 "principal_modify_era_attr_set_13_P"
set ic125 "principal_modify_era_attr_set_14_P"
set ic126 "principal_modify_era_attr_set_15_P"
set ic127 "principal_modify_era_attr_set_16_P"
set ic128 "principal_modify_era_binding_1_P"
set ic129 "principal_modify_era_binding_2_P"
set ic130 "principal_modify_era_binding_3_P"
set ic131 "principal_modify_era_binding_4_P"
set ic132 "principal_modify_era_binding_5_P"
set ic133 "principal_modify_era_binding_6_P"
set ic134 "principal_modify_era_binding_7_P"
set ic135 "principal_modify_era_binding_8_P"
set ic136 "principal_modify_era_binding_9_P"
set ic137 "principal_modify_era_binding_10_P"
set ic138 "principal_modify_era_binding_11_P"
set ic139 "principal_modify_era_binding_12_P"
set ic140 "principal_modify_era_binding_13_P"
set ic141 "principal_modify_era_binding_14_P"
set ic142 "principal_modify_era_binding_15_P"
set ic143 "principal_modify_era_binding_16_P"
set ic144 "principal_modify_era_binding_17_P"
set ic145 "principal_modify_era_any_1_P"
set ic146 "principal_modify_era_any_2_P"
set ic147 "principal_modify_era_any_3_P"
set ic148 "principal_modify_era_any_4_P"
set ic149 "principal_modify_era_any_5_P"
set ic150 "principal_modify_era_any_6_P"
set ic151 "principal_modify_era_any_7_P"
set ic152 "principal_modify_era_any_8_P"
set ic153 "principal_modify_era_any_9_P"
set ic154 "principal_modify_era_any_10_P"
set ic155 "principal_modify_era_any_11_P"
set ic156 "principal_modify_era_any_12_P"
set ic157 "principal_modify_era_any_13_P"
set ic158 "principal_modify_era_any_14_P"
set ic159 "principal_modify_era_any_15_P"
set ic160 "principal_modify_era_any_16_P"
set ic161 "principal_modify_era_any_17_P"
set ic162 "principal_modify_era_any_18_P"
set ic163 "principal_modify_era_any_19_P"
set ic164 "principal_modify_era_any_20_P"
set ic165 "principal_modify_era_any_21_P"
set ic166 "principal_modify_era_any_22_P"
set tet_startup principal_modify_era_startup
set tet_cleanup principal_modify_era_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
