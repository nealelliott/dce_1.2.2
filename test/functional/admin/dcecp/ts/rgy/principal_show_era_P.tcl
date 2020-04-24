#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: principal_show_era_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:22:08  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:21  marty]
#
# Revision 1.1.4.1  1995/12/13  15:28:42  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1995/07/12  12:54 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts38/1  1995/07/12  12:53 UTC  truitt
# 	CHFts15275: Add tests to support new i18ndata behavior.
# 
# 	HP revision /main/HPDCE02/2  1995/05/01  18:33 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts26/1  1995/05/01  18:32 UTC  truitt
# 	CHFts14653: Add new tests.
# 
# 	HP revision /main/HPDCE02/1  1994/12/01  16:20 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/12/01  16:19 UTC  truitt
# 	Create local schema objects to operate on instead
# 	of relying on those already in the security schema.
# 	[1995/12/11  15:55:26  root]
# 
# Revision 1.1.2.2  1994/09/30  16:21:59  truitt
# 	OT12414: Update tests to reflect new show functionality.
# 	[1994/09/30  15:43:12  truitt]
# 
# Revision 1.1.2.1  1994/09/28  21:43:34  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:20  melman]
# 
# Revision 1.1.2.2  1994/09/16  18:25:36  truitt
# 	Update tests to reflect new functionality.
# 	[1994/09/16  18:23:51  truitt]
# 
# Revision 1.1.2.1  1994/08/24  19:08:09  truitt
# 	OT11448: Updated era tests
# 	[1994/08/24  19:02:31  truitt]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc principal_show_era_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_integer -encoding integer -aclmgr {principal r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_attr_set -encoding attrset -aclmgr {principal r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_bytes -encoding byte -aclmgr {principal r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_confidential_bytes -encoding confidential -aclmgr {principal r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_i18n_data -encoding i18ndata -aclmgr {principal r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_printstring -encoding printstring -aclmgr {principal r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_uuid -encoding uuid -aclmgr {principal r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_void -encoding void -aclmgr {principal r r r r} -multi no}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_printstring_array -encoding stringarray -aclmgr {principal r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_binding -encoding binding -aclmgr {principal r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_any -encoding any -aclmgr {principal r r r r} -multi yes}] != 0} {;}
}

proc principal_show_era_cleanup {} {
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/pgo_test_integer}] != 0} {;}
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/pgo_test_attr_set}] != 0} {;}
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/pgo_test_bytes}] != 0} {;}
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/pgo_test_confidential_bytes}] != 0} {;}
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
proc principal_show_era_1_P {} {
    # Identify the test
    tet_infoline principal_show_era_1_P: Show a principal name with a few basic era's.

    # Setup steps
    if {[dcp_wrap {principal create principal_show_era_1_P -attribute {{pgo_test_integer 101} {pgo_test_void present} {pgo_test_printstring "your mother was a hamster"}}}] != 0} {return;}

    # Run the test
    test {catch {principal show -all principal_show_era_1_P} attr_list_1
        dcp_found $attr_list_1 {fullname uid uuid alias quota pgo_test_integer pgo_test_void pgo_test_printstring}
    } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {principal delete principal_show_era_1_P}] != 0} {return;}
}

proc principal_show_era_2_P {} {
    # Identify the test
    tet_infoline principal_show_era_2_P: A void type will have the value 'present'.

    # Setup steps

    # Run the test
    test {principal create principal_show_era_2_P -attribute {pgo_test_void present}
        dcp_attrlist getval [principal show -all principal_show_era_2_P] pgo_test_void
    } compare_strings present

    # Cleanup steps
    if {[dcp_wrap {principal delete principal_show_era_2_P}] != 0} {return;}
}

proc principal_show_era_3_P {} {
    # Identify the test
    tet_infoline principal_show_era_3_P: An integer displays the value you entered.

    # Setup steps

    # Run the test
    test {principal create principal_show_era_3_P -attribute {pgo_test_integer 10101}
        dcp_attrlist getval [principal show -all principal_show_era_3_P] pgo_test_integer
    } compare_strings 10101

    # Cleanup steps
    if {[dcp_wrap {principal delete principal_show_era_3_P}] != 0} {return;}
}

proc principal_show_era_4_P {} {
    # Identify the test
    tet_infoline principal_show_era_4_P: An integer displays multiple values.

    # Setup steps

    # Run the test
    test {principal create principal_show_era_4_P -attribute {pgo_test_integer 101 102}
        attrlist getvalues [principal show -all principal_show_era_4_P] -type pgo_test_integer
    } compare_lists {101 102}

    # Cleanup steps
    if {[dcp_wrap {principal delete principal_show_era_4_P}] != 0} {return;}
}

proc principal_show_era_5_P {} {
    # Identify the test
    tet_infoline principal_show_era_5_P: A printstring puts {} for an empty string.

    # Setup steps

    # Run the test
    test {principal create principal_show_era_5_P -attribute {pgo_test_printstring {}}
        dcp_attrlist getval [principal show -all principal_show_era_5_P] pgo_test_printstring
    } compare_strings {{}}

    # Cleanup steps
    if {[dcp_wrap {principal delete principal_show_era_5_P}] != 0} {return;}
}

proc principal_show_era_6_P {} {
    # Identify the test
    tet_infoline principal_show_era_6_P: A printstring doesn't put {} around single-word strings.

    # Setup steps

    # Run the test
    test {principal create principal_show_era_6_P -attribute {pgo_test_printstring "blech"}
        dcp_attrlist getval [principal show -all principal_show_era_6_P] pgo_test_printstring
    } compare_strings blech

    # Cleanup steps
    if {[dcp_wrap {principal delete principal_show_era_6_P}] != 0} {return;}
}

proc principal_show_era_7_P {} {
    # Identify the test
    tet_infoline principal_show_era_7_P: A printstring puts {} around strings with whitespace.

    # Setup steps

    # Run the test
    test {principal create principal_show_era_7_P -attribute {pgo_test_printstring "blech blech"}
        dcp_attrlist getval [principal show -all principal_show_era_7_P] pgo_test_printstring
    } compare_strings {{blech blech}}

    # Cleanup steps
    if {[dcp_wrap {principal delete principal_show_era_7_P}] != 0} {return;}
}

proc principal_show_era_8_P {} {
    # Identify the test
    tet_infoline principal_show_era_8_P: A printstring array with the empty string.

    # Setup steps

    # Run the test
    test {principal create principal_show_era_8_P -attribute {pgo_test_printstring_array ""}
        dcp_attrlist getval [principal show -all principal_show_era_8_P] pgo_test_printstring_array
    } compare_strings {{}}

    # Cleanup steps
    if {[dcp_wrap {principal delete principal_show_era_8_P}] != 0} {return;}
}

proc principal_show_era_9_P {} {
    # Identify the test
    tet_infoline principal_show_era_9_P: A printstring array doesn't use {} around one word

    # Setup steps

    # Run the test
    test {principal create principal_show_era_9_P -attribute {pgo_test_printstring_array "blech"}
        dcp_attrlist getval [principal show -all principal_show_era_9_P] pgo_test_printstring_array
    } compare_strings blech

    # Cleanup steps
    if {[dcp_wrap {principal delete principal_show_era_9_P}] != 0} {return;}
}

proc principal_show_era_10_P {} {
    # Identify the test
    tet_infoline principal_show_era_10_P: A printstring array uses {} around strings with whitespace

    # Setup steps

    # Run the test
    test {principal create principal_show_era_10_P -attribute {pgo_test_printstring_array "gumby pokey"}
        dcp_attrlist getval [principal show -all principal_show_era_10_P] pgo_test_printstring_array
    } compare_strings {{gumby pokey}}

    # Cleanup steps
    if {[dcp_wrap {principal delete principal_show_era_10_P}] != 0} {return;}
}

proc principal_show_era_11_P {} {
    # Identify the test
    tet_infoline principal_show_era_11_P: Multiple printstring array values are displayed as expected

    # Setup steps

    # Run the test
    test {principal create principal_show_era_11_P -attribute {pgo_test_printstring_array "gumby" "pokey"}
        dcp_attrlist getval [principal show -all principal_show_era_11_P] pgo_test_printstring_array
    } compare_lists {gumby pokey}

    # Cleanup steps
    if {[dcp_wrap {principal delete principal_show_era_11_P}] != 0} {return;}
}

proc principal_show_era_12_P {} {
    # Identify the test
    tet_infoline principal_show_era_12_P: An empty bytes string just has {} for the value

    # Setup steps

    # Run the test
    test {principal create principal_show_era_12_P -attribute {pgo_test_bytes ""}
        dcp_attrlist getval [principal show -all principal_show_era_12_P] pgo_test_bytes
    } compare_strings {{}}

    # Cleanup steps
    if {[dcp_wrap {principal delete principal_show_era_12_P}] != 0} {return;}
}

proc principal_show_era_13_P {} {
    # Identify the test
    tet_infoline principal_show_era_13_P: A byte string has {} and is converted to byte format

    # Setup steps

    # Run the test
    test {principal create principal_show_era_13_P -attribute {pgo_test_bytes a0a0a0}
        dcp_attrlist getval [principal show -all principal_show_era_13_P] pgo_test_bytes
    } compare_strings {{a0 a0 a0}}

    # Cleanup steps
    if {[dcp_wrap {principal delete principal_show_era_13_P}] != 0} {return;}
}

proc principal_show_era_14_P {} {
    # Identify the test
    tet_infoline principal_show_era_14_P: An empty confidential_bytes string just has {} for the value

    # Setup steps

    # Run the test
    test {principal create principal_show_era_14_P -attribute {pgo_test_confidential_bytes ""}
        dcp_attrlist getval [principal show -all principal_show_era_14_P] pgo_test_confidential_bytes
    } compare_strings {{}}

    # Cleanup steps
    if {[dcp_wrap {principal delete principal_show_era_14_P}] != 0} {return;}
}

proc principal_show_era_15_P {} {
    # Identify the test
    tet_infoline principal_show_era_15_P: A confidential_byte string has {} and is converted to byte format

    # Setup steps

    # Run the test
    test {principal create principal_show_era_15_P -attribute {pgo_test_confidential_bytes "a0a0a0"}
        dcp_attrlist getval [principal show -all principal_show_era_15_P] pgo_test_confidential_bytes
    } compare_strings {{a0 a0 a0}}

    # Cleanup steps
    if {[dcp_wrap {principal delete principal_show_era_15_P}] != 0} {return;}
}

proc principal_show_era_16_P {} {
    # Identify the test
    tet_infoline principal_show_era_16_P: An empty i18n_data string

    # Setup steps

    # Run the test
    test {principal create principal_show_era_16_P -attribute {pgo_test_i18n_data {roman8 ""}}
        dcp_attrlist getval [principal show -all principal_show_era_16_P] pgo_test_i18n_data
    } compare_strings {{roman8 {}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete principal_show_era_16_P}] != 0} {return;}
}

proc principal_show_era_17_P {} {
    # Identify the test
    tet_infoline principal_show_era_17_P: An i18n_data string uses {} and displays the correct codeset

    # Setup steps

    # Run the test
    test {principal create principal_show_era_17_P -attribute {pgo_test_i18n_data {kana8 "abcdef"}}
        dcp_attrlist getval [principal show -all principal_show_era_17_P] pgo_test_i18n_data
    } compare_strings {{kana8 {\x61\x62\x63\x64\x65\x66}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete principal_show_era_17_P}] != 0} {return;}
}

proc principal_show_era_18_P {} {
    # Identify the test
    tet_infoline principal_show_era_18_P: A null uuid value is accepted

    # Setup steps

    # Run the test
    test {principal create principal_show_era_18_P -attribute {pgo_test_uuid ""}
        dcp_attrlist getval [principal show -all principal_show_era_18_P] pgo_test_uuid
    } compare_strings 00000000-0000-0000-0000-000000000000

    # Cleanup steps
    if {[dcp_wrap {principal delete principal_show_era_18_P}] != 0} {return;}
}

proc principal_show_era_19_P {} {
    # Identify the test
    tet_infoline principal_show_era_19_P: A single uuid value is displayed as expected

    # Setup steps

    # Run the test
    test {principal create principal_show_era_19_P -attribute {pgo_test_uuid 291e5274-7887-11cd-98a5-0800092464af}
        dcp_attrlist getval [principal show -all principal_show_era_19_P] pgo_test_uuid
    } compare_strings 291e5274-7887-11cd-98a5-0800092464af

    # Cleanup steps
    if {[dcp_wrap {principal delete principal_show_era_19_P}] != 0} {return;}
}

proc principal_show_era_20_P {} {
    # Identify the test
    tet_infoline principal_show_era_20_P: A null uuid value is accepted for attr_set

    # Setup steps

    # Run the test
    test {principal create principal_show_era_20_P -attribute {pgo_test_attr_set ""}
        dcp_attrlist getval [principal show -all principal_show_era_20_P] pgo_test_attr_set
    } compare_strings 00000000-0000-0000-0000-000000000000

    # Cleanup steps
    if {[dcp_wrap {principal delete principal_show_era_20_P}] != 0} {return;}
}

proc principal_show_era_21_P {} {
    # Identify the test
    tet_infoline principal_show_era_21_P: A single uuid value in attr_set is displayed as expected

    # Setup steps

    # Run the test
    test {principal create principal_show_era_21_P -attribute {pgo_test_attr_set 291e5274-7887-11cd-98a5-0800092464af}
        dcp_attrlist getval [principal show -all principal_show_era_21_P] pgo_test_attr_set
    } compare_strings 291e5274-7887-11cd-98a5-0800092464af

    # Cleanup steps
    if {[dcp_wrap {principal delete principal_show_era_21_P}] != 0} {return;}
}

proc principal_show_era_22_P {} {
    # Identify the test
    tet_infoline principal_show_era_22_P: Multiple uuid values in attr_set are displayed as expected

    # Setup steps

    # Run the test
    test {principal create principal_show_era_22_P -attribute {pgo_test_attr_set 291e5274-7887-11cd-98a5-0800092464af 1af0c0d2-bdb1-11cd-b8e1-080009247461}
        dcp_attrlist getval [principal show -all principal_show_era_22_P] pgo_test_attr_set
    } compare_lists {291e5274-7887-11cd-98a5-0800092464af  1af0c0d2-bdb1-11cd-b8e1-080009247461}

    # Cleanup steps
    if {[dcp_wrap {principal delete principal_show_era_22_P}] != 0} {return;}
}

proc principal_show_era_23_P {} {
    # Identify the test
    tet_infoline principal_show_era_23_P: Binding values are displayed as expected

    # Setup steps

    # Run the test
    test {principal create principal_show_era_23_P -attribute {pgo_test_binding {{dce cell_admin call secret dce} {/.../gumby}}}
        dcp_attrlist getval [principal show -all principal_show_era_23_P] pgo_test_binding
    } compare_lists {{{dce cell_admin call secret dce}   /.../gumby}}

    # Cleanup steps
    if {[dcp_wrap {principal delete principal_show_era_23_P}] != 0} {return;}
}

proc principal_show_era_24_P {} {
    # Identify the test
    tet_infoline principal_show_era_24_P: Multiple any's are displayed as expected

    # Setup steps

    # Run the test
    test {principal create principal_show_era_24_P -attribute {{pgo_test_any integer 101 102} {pgo_test_any printstring "gumby1" "gumby2"}}
        dcp_attrlist getval [principal show -all principal_show_era_24_P] pgo_test_any
    } compare_lists {101 102}

    # Cleanup steps
    if {[dcp_wrap {principal delete principal_show_era_24_P}] != 0} {return;}
}

set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 "
set ic1 "principal_show_era_1_P"
set ic2 "principal_show_era_2_P"
set ic3 "principal_show_era_3_P"
set ic4 "principal_show_era_4_P"
set ic5 "principal_show_era_5_P"
set ic6 "principal_show_era_6_P"
set ic7 "principal_show_era_7_P"
set ic8 "principal_show_era_8_P"
set ic9 "principal_show_era_9_P"
set ic10 "principal_show_era_10_P"
set ic11 "principal_show_era_11_P"
set ic12 "principal_show_era_12_P"
set ic13 "principal_show_era_13_P"
set ic14 "principal_show_era_14_P"
set ic15 "principal_show_era_15_P"
set ic16 "principal_show_era_16_P"
set ic17 "principal_show_era_17_P"
set ic18 "principal_show_era_18_P"
set ic19 "principal_show_era_19_P"
set ic20 "principal_show_era_20_P"
set ic21 "principal_show_era_21_P"
set ic22 "principal_show_era_22_P"
set ic23 "principal_show_era_23_P"
set ic24 "principal_show_era_24_P"
set tet_startup principal_show_era_startup
set tet_cleanup principal_show_era_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
