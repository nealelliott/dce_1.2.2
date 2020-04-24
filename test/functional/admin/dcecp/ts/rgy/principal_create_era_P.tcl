#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: principal_create_era_P.tcl,v $
# Revision 1.1.4.1  1995/12/08  23:19:08  root
# 	Submit OSF/DCE 1.2.1
#
# 	HP revision /main/HPDCE02/3  1995/07/12  12:59 UTC  truitt
# 	Merge test changes.
#
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts38/1  1995/07/12  12:58 UTC  truitt
# 	CHFts15275: Add tests to support new i18ndata behavior.
#
# 	HP revision /main/HPDCE02/2  1995/05/01  18:28 UTC  truitt
# 	Merge test changes.
#
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts26/1  1995/05/01  18:28 UTC  truitt
# 	CHFts14653: Add new tests.
#
# 	HP revision /main/HPDCE02/1  1994/12/01  16:15 UTC  truitt
# 	Initial HPDCE02 branch.
#
# 	HP revision /main/truitt_dcecp_01/1  1994/12/01  16:14 UTC  truitt
# 	Create local schema objects to operate on instead
# 	of relying on those already in the security schema.
# 	[1995/12/08  23:01:40  root]
#
# Revision 1.1.2.2  1994/09/30  16:21:54  truitt
# 	OT12414: Update tests to reflect new show functionality.
# 	[1994/09/30  15:41:33  truitt]
# 
# Revision 1.1.2.1  1994/09/28  21:43:12  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:03  melman]
# 
# Revision 1.1.2.4  1994/09/16  18:25:29  truitt
# 	Update tests to reflect new functionality.
# 	[1994/09/16  18:22:40  truitt]
# 
# Revision 1.1.2.3  1994/08/24  19:08:07  truitt
# 	OT11448: Updated era tests
# 	[1994/08/24  19:01:38  truitt]
# 
# Revision 1.1.2.2  1994/08/16  17:29:56  coren
# 	Shortened overlong tet_infoline output.
# 	[1994/08/10  20:53:25  coren]
# 
# 	Added tests for attributes of type "binding".
# 	[1994/08/10  19:52:57  coren]
# 
# Revision 1.1.2.1  1994/08/05  19:15:49  nav
# 	Initial creation.
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_setup.tcl
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc principal_create_era_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW LBINDING1 LBINDING2 SBINDING1 SBINDING2
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set IPADDR 127.0.0.0}] != 0} {return;}
    if {[dcp_wrap {set LBINDING1 [list "ncadg_ip_udp $IPADDR 1234"]}] != 0} {return;}
    if {[dcp_wrap {set LBINDING2 [list "ncacn_ip_tcp $IPADDR 5678"]}] != 0} {return;}
    if {[dcp_wrap {set SBINDING1 "ncadg_ip_udp:$IPADDR\\\[1234\\\]"}] != 0} {return;}
    if {[dcp_wrap {set SBINDING2 "ncacn_ip_tcp:$IPADDR\\\[5678\\\]"}] != 0} {return;}
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

proc principal_create_era_cleanup {} {
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
proc principal_create_era_attr_set_1_P {} {
    # Identify the test
    tet_infoline principal_create_era_attr_set_1_P: An attr_set accepts a single UUID format value

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_attr_set 291e5274-7887-11cd-98a5-0800092464af}
        dcp_attrlist getval [principal show -all foob] pgo_test_attr_set
    } compare_strings 291e5274-7887-11cd-98a5-0800092464af

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_attr_set_2_P {} {
    # Identify the test
    tet_infoline principal_create_era_attr_set_2_P: An attr_set accepts quoted UUID

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_attr_set "291e5274-7887-11cd-98a5-0800092464af"}
        dcp_attrlist getval [principal show -all foob] pgo_test_attr_set
    } compare_strings 291e5274-7887-11cd-98a5-0800092464af

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_attr_set_3_P {} {
    # Identify the test
    tet_infoline principal_create_era_attr_set_3_P: An attr_set accepts a Tcl listed UUID

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_attr_set {291e5274-7887-11cd-98a5-0800092464af}}
        dcp_attrlist getval [principal show -all foob] pgo_test_attr_set
    } compare_strings 291e5274-7887-11cd-98a5-0800092464af

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_attr_set_4_P {} {
    # Identify the test
    tet_infoline principal_create_era_attr_set_4_P: An attr_set accepts multiple quoted UUIDs

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_attr_set "291e5274-7887-11cd-98a5-0800092464af" "291e5274-7887-11cd-98a5-0800092464ab" }
        dcp_attrlist getval [principal show -all foob] pgo_test_attr_set
    } compare_lists {291e5274-7887-11cd-98a5-0800092464af  291e5274-7887-11cd-98a5-0800092464ab}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_attr_set_5_P {} {
    # Identify the test
    tet_infoline principal_create_era_attr_set_5_P: An attr_set accepts a multiple Tcl listed UUIDs

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_attr_set {291e5274-7887-11cd-98a5-0800092464af} {291e5274-7887-11cd-98a5-0800092464ab} }
        dcp_attrlist getval [principal show -all foob] pgo_test_attr_set
    } compare_lists {291e5274-7887-11cd-98a5-0800092464af  291e5274-7887-11cd-98a5-0800092464ab}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_attr_set_6_P {} {
    # Identify the test
    tet_infoline principal_create_era_attr_set_6_P: An attr_set accepts a quoted null value

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_attr_set ""}
        dcp_attrlist getval [principal show -all foob] pgo_test_attr_set
    } compare_strings 00000000-0000-0000-0000-000000000000 

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_attr_set_7_P {} {
    # Identify the test
    tet_infoline principal_create_era_attr_set_7_P: An attr_set accepts a Tcl list null value

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_attr_set {}}
        dcp_attrlist getval [principal show -all foob] pgo_test_attr_set
    } compare_strings 00000000-0000-0000-0000-000000000000 

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_attr_set_8_P {} {
    # Identify the test
    tet_infoline principal_create_era_attr_set_8_P: An attr_set accepts multiple quoted UUIDs and null values

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_attr_set "291e5274-7887-11cd-98a5-0800092464af" "" "291e5274-7887-11cd-98a5-0800092464ab"}
        dcp_attrlist getval [principal show -all foob] pgo_test_attr_set
    } compare_lists  {291e5274-7887-11cd-98a5-0800092464af  00000000-0000-0000-0000-000000000000  291e5274-7887-11cd-98a5-0800092464ab}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_attr_set_9_P {} {
    # Identify the test
    tet_infoline principal_create_era_attr_set_9_P: An attr_set accepts a multiple Tcl/null listed UUID

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_attr_set {291e5274-7887-11cd-98a5-0800092464af} {} {291e5274-7887-11cd-98a5-0800092464ab}}
        dcp_attrlist getval [principal show -all foob] pgo_test_attr_set
    } compare_lists {291e5274-7887-11cd-98a5-0800092464af  00000000-0000-0000-0000-000000000000  291e5274-7887-11cd-98a5-0800092464ab}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_attr_set_10_P {} {
    # Identify the test
    tet_infoline principal_create_era_attr_set_10_P: An attr_set accepts a multiple Tcl/quote/null listed UUID

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_attr_set {291e5274-7887-11cd-98a5-0800092464af} "" {291e5274-7887-11cd-98a5-0800092464ab}}
        dcp_attrlist getval [principal show -all foob] pgo_test_attr_set
    } compare_lists {291e5274-7887-11cd-98a5-0800092464af  00000000-0000-0000-0000-000000000000  291e5274-7887-11cd-98a5-0800092464ab}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_attr_set_11_P {} {
    # Identify the test
    tet_infoline principal_create_era_attr_set_11_P: An attr_set accepts quoted UUID's for 2 principals

    # Setup steps

    # Run the test
    test {principal create {foob arspam} -attribute {pgo_test_attr_set "291e5274-7887-11cd-98a5-0800092464af" "291e5274-7887-11cd-98a5-0800092464ab"}
        dcp_attrlist getval [principal show -all {foob arspam}] pgo_test_attr_set
    } compare_lists {291e5274-7887-11cd-98a5-0800092464af  291e5274-7887-11cd-98a5-0800092464ab}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob arspam}}] != 0} {return;}
}

proc principal_create_era_attr_set_12_P {} {
    # Identify the test
    tet_infoline principal_create_era_attr_set_12_P: An attr_set accepts quoted UUID for 10 principals

    # Setup steps

    # Run the test
    test {principal create {foob arspam blech ni piron alon xyzzy jabber wock y} -attribute {pgo_test_attr_set "291e5274-7887-11cd-98a5-0800092464af" "291e5274-7887-11cd-98a5-0800092464ab"}
        dcp_attrlist getval [principal show -all {foob arspam blech ni piron alon xyzzy jabber wock y}] pgo_test_attr_set
    } compare_lists {291e5274-7887-11cd-98a5-0800092464af  291e5274-7887-11cd-98a5-0800092464ab}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob arspam blech ni piron alon xyzzy jabber wock y} }] != 0} {return;}
}

proc principal_create_era_bytes_1_P {} {
    # Identify the test
    tet_infoline principal_create_era_bytes_1_P: A bytes accepts a single non-quoted value

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_bytes a0a0a0}
        dcp_attrlist getval [principal show -all foob] pgo_test_bytes
    } compare_strings {{a0 a0 a0}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_bytes_2_P {} {
    # Identify the test
    tet_infoline principal_create_era_bytes_2_P: A bytes accepts quoted string

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_bytes "b1b1b1"}
        dcp_attrlist getval [principal show -all foob] pgo_test_bytes
    } compare_strings {{b1 b1 b1}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_bytes_3_P {} {
    # Identify the test
    tet_infoline principal_create_era_bytes_3_P: A bytes accepts a single Tcl list

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_bytes {a0a0 a0a0}}
        dcp_attrlist getval [principal show -all foob] pgo_test_bytes
    } compare_strings {{a0 a0 a0 a0}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_bytes_4_P {} {
    # Identify the test
    tet_infoline principal_create_era_bytes_4_P: A bytes accepts a quoted null value

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_bytes ""}
        dcp_attrlist getval [principal show -all foob] pgo_test_bytes
    } compare_strings {{}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_bytes_5_P {} {
    # Identify the test
    tet_infoline principal_create_era_bytes_5_P: A bytes accepts a Tcl list null value

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_bytes {}}
        dcp_attrlist getval [principal show -all foob] pgo_test_bytes
    } compare_strings {{}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_bytes_6_P {} {
    # Identify the test
    tet_infoline principal_create_era_bytes_6_P: A bytes is accepted for 2 principals

    # Setup steps

    # Run the test
    test {principal create {foob arspam} -attribute {pgo_test_bytes a0b123} 
        dcp_attrlist getval [principal show -all {foob arspam}] pgo_test_bytes
    } compare_strings {{a0 b1 23}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob arspam}}] != 0} {return;}
}

proc principal_create_era_bytes_7_P {} {
    # Identify the test
    tet_infoline principal_create_era_bytes_7_P: A bytes is accepted for 10 principals

    # Setup steps

    # Run the test
    test {principal create {foob arspam blech ni piron alon xyzzy jabber wock y} -attribute {pgo_test_bytes a0b123}
        dcp_attrlist getval [principal show -all {foob arspam blech ni piron alon xyzzy jabber wock y}] pgo_test_bytes
    } compare_strings {{a0 b1 23}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob arspam blech ni piron alon xyzzy jabber wock y}}] != 0} {return;}
}

proc principal_create_era_bytes_8_P {} {
    # Identify the test
    tet_infoline principal_create_era_bytes_8_P: Multiple bytes are accepted

    # Setup steps

    # Run the test
    test {principal create bytes_8_P -attribute {pgo_test_bytes a0b123 876543}
        attrlist getvalues [principal show -all bytes_8_P] -type pgo_test_bytes
    } compare_lists {{a0 b1 23} {87 65 43}}

    # Cleanup steps
    if {[dcp_wrap {principal delete bytes_8_P}] != 0} {return;}
}

proc principal_create_era_i18n_data_1_P {} {
    # Identify the test
    tet_infoline principal_create_era_i18n_data_1_P: An i18n_data accepts a single non-quoted value

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_i18n_data {roman8 abcdef}}
        dcp_attrlist getval [principal show -all foob] pgo_test_i18n_data
    } compare_strings {{roman8 {\x61\x62\x63\x64\x65\x66}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_i18n_data_2_P {} {
    # Identify the test
    tet_infoline principal_create_era_i18n_data_2_P: An i18n_data accepts single quoted string

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_i18n_data {roman8 "abc def"}}
        dcp_attrlist getval [principal show -all foob] pgo_test_i18n_data
    } compare_strings {{roman8 {\x61\x62\x63\x20\x64\x65\x66}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_i18n_data_3_P {} {
    # Identify the test
    tet_infoline principal_create_era_i18n_data_3_P: An i18n_data accepts a single Tcl list

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_i18n_data {roman8 {abc def}}}
        dcp_attrlist getval [principal show -all foob] pgo_test_i18n_data
    } compare_strings {{roman8 {\x61\x62\x63\x20\x64\x65\x66}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_i18n_data_4_P {} {
    # Identify the test
    tet_infoline principal_create_era_i18n_data_4_P: An i18n_data accepts a quoted null value

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_i18n_data {roman8 ""}}
        dcp_attrlist getval [principal show -all foob] pgo_test_i18n_data
    } compare_strings {{roman8 {}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_i18n_data_5_P {} {
    # Identify the test
    tet_infoline principal_create_era_i18n_data_5_P: An i18n_data accepts a Tcl list null value

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_i18n_data {roman8 {}}}
        dcp_attrlist getval [principal show -all foob] pgo_test_i18n_data
    } compare_strings {{roman8 {}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_i18n_data_6_P {} {
    # Identify the test
    tet_infoline principal_create_era_i18n_data_6_P: An i18n_data is accepted for 2 principals

    # Setup steps

    # Run the test
    test {principal create {foob arspam} -attribute {pgo_test_i18n_data {roman8 abc}}
        dcp_attrlist getval [principal show -all {foob arspam}] pgo_test_i18n_data
    } compare_strings {{roman8 {\x61\x62\x63}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob arspam}}] != 0} {return;}
}

proc principal_create_era_i18n_data_7_P {} {
    # Identify the test
    tet_infoline principal_create_era_i18n_data_7_P: An i18n_data is accepted for 10 principals

    # Setup steps

    # Run the test
    test {principal create {foob arspam blech ni piron alon xyzzy jabber wock y} -attribute {pgo_test_i18n_data {268500993 abc}}
        dcp_attrlist getval [principal show -all {foob arspam blech ni piron alon xyzzy jabber wock y}] pgo_test_i18n_data
    } compare_strings {{roman8 {\x61\x62\x63}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob arspam blech ni piron alon xyzzy jabber wock y}}] != 0} {return;}
}

proc principal_create_era_i18n_data_8_P {} {
    # Identify the test
    tet_infoline principal_create_era_i18n_data_8_P: Multiple i18n_data are accepted

    # Setup steps

    # Run the test
    test {principal create i18n_8_P -attribute {pgo_test_i18n_data {roman8 abc} {roman8 def}}
        attrlist getvalues [principal show -all i18n_8_P] -type pgo_test_i18n_data
    } compare_lists {{roman8 {\x61\x62\x63}} {roman8 {\x64\x65\x66}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete i18n_8_P}] != 0} {return;}
}

proc principal_create_era_integer_1_P {} {
    # Identify the test
    tet_infoline principal_create_era_integer_1_P: An integer accepts integer value

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_integer 1}
        dcp_attrlist getval [principal show -all foob] pgo_test_integer
    } compare_strings {1}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_integer_2_P {} {
    # Identify the test
    tet_infoline principal_create_era_integer_2_P: An integer accepts negative integer value

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_integer -1}
        dcp_attrlist getval [principal show -all foob] pgo_test_integer
    } compare_strings {-1}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_integer_3_P {} {
    # Identify the test
    tet_infoline principal_create_era_integer_3_P: An integer accepts quoted integer value

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_integer "1"}
        dcp_attrlist getval [principal show -all foob] pgo_test_integer
    } compare_strings {1}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_integer_4_P {} {
    # Identify the test
    tet_infoline principal_create_era_integer_4_P: An integer accepts quoted null value

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_integer ""}
        dcp_attrlist getval [principal show -all foob] pgo_test_integer
    } compare_strings {0}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_integer_5_P {} {
    # Identify the test
    tet_infoline principal_create_era_integer_5_P: An integer accepts single Tcl list integer value

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_integer {1}}
        dcp_attrlist getval [principal show -all foob] pgo_test_integer
    } compare_strings {1}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_integer_6_P {} {
    # Identify the test
    tet_infoline principal_create_era_integer_6_P: An integer accepts Tcl list null value

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_integer {}}
        dcp_attrlist getval [principal show -all foob] pgo_test_integer
    } compare_strings {0}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_integer_7_P {} {
    # Identify the test
    tet_infoline principal_create_era_integer_7_P: An integer is accepted for 2 principals

    # Setup steps

    # Run the test
    test {principal create {foob arspam} -attribute {pgo_test_integer 1}
        dcp_attrlist getval [principal show -all {foob arspam}] pgo_test_integer
    } compare_strings {1}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob arspam}}] != 0} {return;}
}

proc principal_create_era_integer_8_P {} {
    # Identify the test
    tet_infoline principal_create_era_integer_8_P: An integer is accepted for 10 principals

    # Setup steps

    # Run the test
    test {principal create {foob arspam blech ni piron alon xyzzy jabber wock y} -attribute {pgo_test_integer 1}
        dcp_attrlist getval [principal show -all {foob arspam blech ni piron alon xyzzy jabber wock y}] pgo_test_integer
    } compare_strings {1}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob arspam blech ni piron alon xyzzy jabber wock y}}] != 0} {return;}
}

proc principal_create_era_integer_9_P {} {
    # Identify the test
    tet_infoline principal_create_era_integer_9_P: Multiple integers are accepted

    # Setup steps

    # Run the test
    test {principal create integer_9_P -attribute {pgo_test_integer 101 102 103}
        attrlist getvalues [principal show -all integer_9_P] -type pgo_test_integer
    } compare_lists {101 102 103}

    # Cleanup steps
    if {[dcp_wrap {principal delete integer_9_P}] != 0} {return;}
}

proc principal_create_era_printstring_1_P {} {
    # Identify the test
    tet_infoline principal_create_era_printstring_1_P: A printstring accepts a non-quoted value

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_printstring abcdef}
        dcp_attrlist getval [principal show -all foob] pgo_test_printstring
    } compare_strings abcdef

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_printstring_2_P {} {
    # Identify the test
    tet_infoline principal_create_era_printstring_2_P: A printstring accepts a quoted string

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_printstring "abc def"}
        dcp_attrlist getval [principal show -all foob] pgo_test_printstring
    } compare_strings {{abc def}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_printstring_3_P {} {
    # Identify the test
    tet_infoline principal_create_era_printstring_3_P: A printstring accepts a single-item Tcl list

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_printstring {abc def}}
        dcp_attrlist getval [principal show -all foob] pgo_test_printstring
    } compare_strings {{abc def}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_printstring_4_P {} {
    # Identify the test
    tet_infoline principal_create_era_printstring_4_P: A printstring accepts a quoted null value

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_printstring ""}
        dcp_attrlist getval [principal show -all foob] pgo_test_printstring
    } compare_strings {{}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_printstring_5_P {} {
    # Identify the test
    tet_infoline principal_create_era_printstring_5_P: A printstring accepts a Tcl list null value

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_printstring {}}
        dcp_attrlist getval [principal show -all foob] pgo_test_printstring
    } compare_strings {{}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_printstring_6_P {} {
    # Identify the test
    tet_infoline principal_create_era_printstring_6_P: A printstring is accepted for 2 principals

    # Setup steps

    # Run the test
    test {principal create {foob arspam} -attribute {pgo_test_printstring "abc"}
        dcp_attrlist getval [principal show -all {foob arspam}] pgo_test_printstring
    } compare_strings abc

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob arspam}}] != 0} {return;}
}

proc principal_create_era_printstring_7_P {} {
    # Identify the test
    tet_infoline principal_create_era_printstring_7_P: A printstring is accepted for 10 principals

    # Setup steps

    # Run the test
    test {principal create {foob arspam blech ni piron alon xyzzy jabber wock y} -attribute {pgo_test_printstring "abc"}
        dcp_attrlist getval [principal show -all {foob arspam blech ni piron alon xyzzy jabber wock y}] pgo_test_printstring
    } compare_strings abc

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob arspam blech ni piron alon xyzzy jabber wock y}}] != 0} {return;}
}

proc principal_create_era_printstring_8_P {} {
    # Identify the test
    tet_infoline principal_create_era_printstring_8_P: Multiple printstrings are accepted

    # Setup steps

    # Run the test
    test {principal create string_8_P -attribute {pgo_test_printstring "your mother" "was" "a hamster"}
        attrlist getvalues [principal show -all string_8_P] -type pgo_test_printstring
    } compare_lists {{your mother} was {a hamster}}

    # Cleanup steps
    if {[dcp_wrap {principal delete string_8_P}] != 0} {return;}
}

proc principal_create_era_printstring_array_1_P {} {
    # Identify the test
    tet_infoline principal_create_era_printstring_array_1_P: A stringarray accepts a non-quoted value

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_printstring_array abcdef}
        dcp_attrlist getval [principal show -all foob] pgo_test_printstring_array
    } compare_strings abcdef

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_printstring_array_2_P {} {
    # Identify the test
    tet_infoline principal_create_era_printstring_array_2_P: A stringarray accepts multiple non-quoted values

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_printstring_array abc def}
        dcp_attrlist getval [principal show -all foob] pgo_test_printstring_array
    } compare_lists {abc def}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_printstring_array_3_P {} {
    # Identify the test
    tet_infoline principal_create_era_printstring_array_3_P: A stringarray accepts a quoted string

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_printstring_array "abc def"}
        dcp_attrlist getval [principal show -all foob] pgo_test_printstring_array
    } compare_lists {{abc def}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_printstring_array_4_P {} {
    # Identify the test
    tet_infoline principal_create_era_printstring_array_4_P: A stringarray accepts multiple quoted strings

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_printstring_array "abc def" "ghi jkl"}
        dcp_attrlist getval [principal show -all foob] pgo_test_printstring_array
    } compare_lists {{abc def} {ghi jkl}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_printstring_array_5_P {} {
    # Identify the test
    tet_infoline principal_create_era_printstring_array_5_P: A stringarray accepts a single Tcl list

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_printstring_array {abc def}}
        dcp_attrlist getval [principal show -all foob] pgo_test_printstring_array
    } compare_lists {{abc def}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_printstring_array_6_P {} {
    # Identify the test
    tet_infoline principal_create_era_printstring_array_6_P: A stringarray accepts multiple Tcl lists

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_printstring_array {abc def} {ghi jkl}}
        dcp_attrlist getval [principal show -all foob] pgo_test_printstring_array
    } compare_lists {{abc def} {ghi jkl}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_printstring_array_7_P {} {
    # Identify the test
    tet_infoline principal_create_era_printstring_array_7_P: A stringarray accepts a quoted null value

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_printstring_array ""}
        dcp_attrlist getval [principal show -all foob] pgo_test_printstring_array
    } compare_lists {{}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_printstring_array_8_P {} {
    # Identify the test
    tet_infoline principal_create_era_printstring_array_8_P: A stringarray accepts multiple quoted null values

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_printstring_array "" ""}
        dcp_attrlist getval [principal show -all foob] pgo_test_printstring_array
    } compare_lists {{} {}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_printstring_array_9_P {} {
    # Identify the test
    tet_infoline principal_create_era_printstring_array_9_P: A stringarray accepts a Tcl list null value

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_printstring_array {}}
        dcp_attrlist getval [principal show -all foob] pgo_test_printstring_array
    } compare_lists {{}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_printstring_array_10_P {} {
    # Identify the test
    tet_infoline principal_create_era_printstring_array_10_P: A stringarray accepts multiple Tcl list null values

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_printstring_array {} {}}
        dcp_attrlist getval [principal show -all foob] pgo_test_printstring_array
    } compare_lists {{} {}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_printstring_array_11_P {} {
    # Identify the test
    tet_infoline principal_create_era_printstring_array_11_P: A stringarray accepts mixed quoted list/null values

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_printstring_array "abc def" "" "ghi jkl"}
        dcp_attrlist getval [principal show -all foob] pgo_test_printstring_array
    } compare_lists {{abc def} {} {ghi jkl}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_printstring_array_12_P {} {
    # Identify the test
    tet_infoline principal_create_era_printstring_array_12_P: A stringarray accepts mixed Tcl list/null values

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_printstring_array {abc def} {} {ghi jkl}}
        dcp_attrlist getval [principal show -all foob] pgo_test_printstring_array
    } compare_lists {{abc def} {} {ghi jkl}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_printstring_array_13_P {} {
    # Identify the test
    tet_infoline principal_create_era_printstring_array_13_P: A string_array accepts mixed quoted/Tcl list/null values

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_printstring_array {abc def} "" "ghi jkl" {}}
        dcp_attrlist getval [principal show -all foob] pgo_test_printstring_array
    } compare_lists {{abc def} {} {ghi jkl} {}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_printstring_array_14_P {} {
    # Identify the test
    tet_infoline principal_create_era_printstring_array_14_P: A stringarray is accepted for 2 principals

    # Setup steps

    # Run the test
    test {principal create {foob arspam} -attribute {pgo_test_printstring_array "abc" "def"}
        dcp_attrlist getval [principal show -all {foob arspam}] pgo_test_printstring_array
    } compare_lists {abc def}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob arspam}}] != 0} {return;}
}

proc principal_create_era_printstring_array_15_P {} {
    # Identify the test
    tet_infoline principal_create_era_printstring_array_15_P: A stringarray is accepted for 10 principals

    # Setup steps

    # Run the test
    test {principal create {foob arspam blech ni piron alon xyzzy jabber wock y} -attribute {pgo_test_printstring_array "abc" "def"}
        dcp_attrlist getval [principal show -all {foob arspam blech ni piron alon xyzzy jabber wock y}] pgo_test_printstring_array
    } compare_lists {abc def}

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob arspam blech ni piron alon xyzzy jabber wock y}}] != 0} {return;}
}

proc principal_create_era_uuid_1_P {} {
    # Identify the test
    tet_infoline principal_create_era_uuid_1_P: A uuid accepts a single UUID format value

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_uuid 291e5274-7887-11cd-98a5-0800092464af}
        dcp_attrlist getval [principal show -all foob] pgo_test_uuid
    } compare_strings 291e5274-7887-11cd-98a5-0800092464af

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_uuid_2_P {} {
    # Identify the test
    tet_infoline principal_create_era_uuid_2_P: A uuid accepts a single quoted UUID

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_uuid "291e5274-7887-11cd-98a5-0800092464af"}
        dcp_attrlist getval [principal show -all foob] pgo_test_uuid
    } compare_strings 291e5274-7887-11cd-98a5-0800092464af

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_uuid_3_P {} {
    # Identify the test
    tet_infoline principal_create_era_uuid_3_P: A uuid accepts a single Tcl listed UUID

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_uuid {291e5274-7887-11cd-98a5-0800092464af}}
        dcp_attrlist getval [principal show -all foob] pgo_test_uuid
    } compare_strings 291e5274-7887-11cd-98a5-0800092464af

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_uuid_4_P {} {
    # Identify the test
    tet_infoline principal_create_era_uuid_4_P: A uuid accepts a quoted null value

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_uuid ""}
        dcp_attrlist getval [principal show -all foob] pgo_test_uuid
    } compare_strings 00000000-0000-0000-0000-000000000000

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_uuid_5_P {} {
    # Identify the test
    tet_infoline principal_create_era_uuid_5_P: A uuid accepts a Tcl list null value

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_uuid {}}
        dcp_attrlist getval [principal show -all foob] pgo_test_uuid
    } compare_strings 00000000-0000-0000-0000-000000000000

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_uuid_6_P {} {
    # Identify the test
    tet_infoline principal_create_era_uuid_6_P: A uuid accepts quoted UUID for 2 principals

    # Setup steps

    # Run the test
    test {principal create {foob arspam} -attribute {pgo_test_uuid "291e5274-7887-11cd-98a5-0800092464af"}
        dcp_attrlist getval [principal show -all {foob arspam}] pgo_test_uuid
    } compare_strings 291e5274-7887-11cd-98a5-0800092464af

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob arspam}}] != 0} {return;}
}

proc principal_create_era_uuid_7_P {} {
    # Identify the test
    tet_infoline principal_create_era_uuid_7_P: A uuid accepts quoted UUID for 10 principals

    # Setup steps

    # Run the test
    test {principal create {foob arspam blech ni piron alon xyzzy jabber wock y} -attribute {pgo_test_uuid "291e5274-7887-11cd-98a5-0800092464af"}
        dcp_attrlist getval [principal show -all {foob arspam blech ni piron alon xyzzy jabber wock y}] pgo_test_uuid
    } compare_strings 291e5274-7887-11cd-98a5-0800092464af

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob arspam blech ni piron alon xyzzy jabber wock y} }] != 0} {return;}
}

proc principal_create_era_uuid_8_P {} {
    # Identify the test
    tet_infoline principal_create_era_uuid_8_P: Multiple UUID's are accepted

    # Setup steps

    # Run the test
    test {principal create uuid_8_P -attribute {pgo_test_uuid 291e5274-7887-11cd-98a5-0800092464aa 291e5274-7887-11cd-98a5-0800092464ac}
        attrlist getvalues [principal show -all uuid_8_P] -type pgo_test_uuid
    } compare_lists {291e5274-7887-11cd-98a5-0800092464aa 291e5274-7887-11cd-98a5-0800092464ac}

    # Cleanup steps
    if {[dcp_wrap {principal delete uuid_8_P}] != 0} {return;}
}

proc principal_create_era_void_1_P {} {
    # Identify the test
    tet_infoline principal_create_era_void_1_P: A void is accepted with the keyword present

    # Setup steps

    # Run the test
    test {principal create foob -attribute {pgo_test_void present}
        dcp_attrlist getval [principal show -all foob] pgo_test_void
    } compare_strings present

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob}}] != 0} {return;}
}

proc principal_create_era_void_2_P {} {
    # Identify the test
    tet_infoline principal_create_era_void_2_P: A void is accepted for 2 principals

    # Setup steps

    # Run the test
    test {principal create {foob arspam} -attribute {pgo_test_void present}
        dcp_attrlist getval [principal show -all {foob arspam}] pgo_test_void
    } compare_strings present

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob arspam}}] != 0} {return;}
}

proc principal_create_era_void_3_P {} {
    # Identify the test
    tet_infoline principal_create_era_void_3_P: A void is accepted for 10 principals

    # Setup steps

    # Run the test
    test {principal create {foob arspam blech ni piron alon xyzzy jabber wock y} -attribute {pgo_test_void present}
        dcp_attrlist getval [principal show -all {foob arspam blech ni piron alon xyzzy jabber wock y}] pgo_test_void
    } compare_strings present

    # Cleanup steps
    if {[dcp_wrap {principal delete {foob arspam blech ni piron alon xyzzy jabber wock y}}] != 0} {return;}
}

proc principal_create_era_binding_1_P {} {
    # Identify the test
    tet_infoline principal_create_era_binding_1_P: A binding with complete security info

    # Setup steps
    if {[dcp_wrap {global SECMASTER}] != 0} {return;}
    if {[dcp_wrap {set fbind "{dce cell_admin call secret dce} $SECMASTER"}] != 0} {return;}
    if {[dcp_wrap {set attrib [list "pgo_test_binding [list $fbind]"]}] != 0} {return;}

    # Run the test
    test {principal create foob1 -attribute $attrib
        dcp_attrlist getval [principal show -all foob1] pgo_test_binding
    } compare_lists [format "{{dce cell_admin call secret dce} %s}" $SECMASTER]

    # Cleanup 
    if {[dcp_wrap {principal delete foob1}] != 0} {return;}
}

proc principal_create_era_binding_2_P {} {
    # Identify the test
    tet_infoline principal_create_era_binding_2_P: A binding with complete tower information

    # Setup steps

    # Run the test
    test {principal create foob2 -attribute {pgo_test_binding {none {01 02 0a 0b}}}
        dcp_attrlist getval [principal show -all foob2] pgo_test_binding
    } compare_lists {{none   {01 02 0a 0b}}}

    # Cleanup 
    if {[dcp_wrap {principal delete foob2}] != 0} {return;}
}

proc principal_create_era_binding_3_P {} {
    # Identify the test
    tet_infoline principal_create_era_binding_3_P: A binding with null security information

    # Setup steps
    if {[dcp_wrap {global SECMASTER}] != 0} {return;}
    if {[dcp_wrap {set fbind "none $SECMASTER"}] != 0} {return;}
    if {[dcp_wrap {set attrib [list "pgo_test_binding [list $fbind]"]}] != 0} {return;}

    # Run the test
    test {principal create foob3 -attribute $attrib
        dcp_attrlist getval [principal show -all foob3] pgo_test_binding
    } compare_lists [format "{none %s}" $SECMASTER]

    # Cleanup steps
    if {[dcp_wrap {principal delete foob3}] != 0} {return;}

}

proc principal_create_era_binding_4_P {} {
    # Identify the test
    tet_infoline principal_create_era_binding_4_P: A binding with a single string binding

    # Setup steps
    if {[dcp_wrap {global LBINDING1 SBINDING1}] != 0} {return;}
    if {[dcp_wrap {set fbind "none $SBINDING1"}] != 0} {return;}
    if {[dcp_wrap {set attrib [list "pgo_test_binding [list $fbind]"]}] != 0} {return;}

    # Run the test
    test {principal create foob4 -attribute $attrib
        dcp_attrlist getval [principal show -all foob4] pgo_test_binding
    } compare_lists [format "{none %s}" $LBINDING1]

    # Cleanup steps
    if {[dcp_wrap {principal delete foob4}] != 0} {return;}
}

proc principal_create_era_binding_5_P {} {
    # Identify the test
    tet_infoline principal_create_era_binding_5_P: A binding with a single Tcl syntax string binding

    # Setup steps
    if {[dcp_wrap {global LBINDING1}] != 0} {return;}
    if {[dcp_wrap {set fbind "none $LBINDING1"}] != 0} {return;}
    if {[dcp_wrap {set attrib [list "pgo_test_binding [list $fbind]"]}] != 0} {return;}

    # Run the test
    test {principal create foob5 -attribute $attrib
        dcp_attrlist getval [principal show -all foob5] pgo_test_binding
    } compare_lists [format "{none %s}" $LBINDING1]

    # Cleanup steps
    if {[dcp_wrap {principal delete foob5}] != 0} {return;}
}

proc principal_create_era_binding_6_P {} {
    # Identify the test
    tet_infoline principal_create_era_binding_6_P: A binding with a list of string bindings

    # Setup steps
    if {[dcp_wrap {global LBINDING1 LBINDING2 SBINDING1 SBINDING2}] != 0} {return;}
    if {[dcp_wrap {set blist "$SBINDING1 $SBINDING2"}] != 0} {return;}
    if {[dcp_wrap {set fbind "none [list $blist]"}] != 0} {return;}
    if {[dcp_wrap {set attrib [list "pgo_test_binding [list $fbind]"]}] != 0} {return;}

    # Run the test
    test {principal create foob6 -attribute $attrib
        dcp_attrlist getval [principal show -all foob6] pgo_test_binding
    } compare_lists [format "{none {%s %s}}" $LBINDING1 $LBINDING2]

    # Cleanup steps
    if {[dcp_wrap {principal delete foob6}] != 0} {return;}
}

proc principal_create_era_binding_7_P {} {
    # Identify the test
    tet_infoline principal_create_era_binding_7_P: A binding with multiple Tcl syntax string bindings

    # Setup steps
    if {[dcp_wrap {global LBINDING1 LBINDING2}] != 0} {return;}
    if {[dcp_wrap {set blist "$LBINDING1 $LBINDING2"}] != 0} {return;}
    if {[dcp_wrap {set fbind "none [list $blist]"}] != 0} {return;}
    if {[dcp_wrap {set attrib [list "pgo_test_binding [list $fbind]"]}] != 0} {return;}

    # Run the test
    test {principal create foob7 -attribute $attrib
        dcp_attrlist getval [principal show -all foob7] pgo_test_binding
    } compare_lists [format "{none {%s %s}}" $LBINDING1 $LBINDING2]

    # Cleanup steps
    if {[dcp_wrap {principal delete foob7}] != 0} {return;}
}

proc principal_create_era_binding_8_P {} {
    # Identify the test
    tet_infoline principal_create_era_binding_8_P: A binding with a list of server names

    # Setup steps
    if {[dcp_wrap {global SECMASTER DCP_SR_ENTRYNAME}] != 0} {return;}
    if {[dcp_wrap {set blist "$SECMASTER $DCP_SR_ENTRYNAME"}] != 0} {return;}
    if {[dcp_wrap {set fbind "none [list $blist]"}] != 0} {return;}
    if {[dcp_wrap {set attrib [list "pgo_test_binding [list $fbind]"]}] != 0} {return;}

    # Run the test
    test {principal create foob8 -attribute $attrib
        dcp_attrlist getval [principal show -all foob8] pgo_test_binding
    } compare_lists [format "{none {%s %s}}" $SECMASTER $DCP_SR_ENTRYNAME]

    # Cleanup steps
    if {[dcp_wrap {principal delete foob8}] != 0} {return;}
}

proc principal_create_era_binding_9_P {} {
    # Identify the test
    tet_infoline principal_create_era_binding_9_P: A binding with both server and string bindings

    # Setup steps
    if {[dcp_wrap {global SECMASTER LBINDING1}] != 0} {return;}
    if {[dcp_wrap {set blist "$SECMASTER $LBINDING1"}] != 0} {return;}
    if {[dcp_wrap {set fbind "none [list $blist]"}] != 0} {return;}
    if {[dcp_wrap {set attrib [list "pgo_test_binding [list $fbind]"]}] != 0} {return;}

    # Run the test
    test {principal create foob9 -attribute $attrib
        dcp_attrlist getval [principal show -all foob9] pgo_test_binding
    } compare_lists [format "{none {%s %s}}" $SECMASTER $LBINDING1]

    # Cleanup steps
    if {[dcp_wrap {principal delete foob9}] != 0} {return;}
}

proc principal_create_era_binding_10_P {} {
    # Identify the test
    tet_infoline principal_create_era_binding_10_P: A binding with a list of bindings

    # Setup steps
    if {[dcp_wrap {global SBINDING1 LBINDING1 LBINDING2}] != 0} {return;}
    if {[dcp_wrap {set blist "$SBINDING1 $LBINDING2"}] != 0} {return;}
    if {[dcp_wrap {set fbind "none [list $blist]"}] != 0} {return;}
    if {[dcp_wrap {set attrib [list "pgo_test_binding [list $fbind]"]}] != 0} {return;}
    
    # Run the test
    test {principal create foob10 -attribute $attrib
        dcp_attrlist getval [principal show -all foob10] pgo_test_binding
    } compare_lists [format "{none {%s %s}}" $LBINDING1 $LBINDING2]

    # Cleanup steps
    if {[dcp_wrap {principal delete foob10}] != 0} {return;}
}

proc principal_create_era_any_1_P {} {
    # Identify the test
    tet_infoline principal_create_era_any_1_P: An any is accepted with a legal encoding type

    # Setup steps

    # Run the test
    test {principal create goober -attribute {pgo_test_any void present}
        dcp_attrlist getval [principal show goober -all] pgo_test_any
    } compare_strings present

    # Cleanup steps
    if {[dcp_wrap {principal delete {goober}}] != 0} {return;}
}

proc principal_create_era_any_2_P {} {
    # Identify the test
    tet_infoline principal_create_era_any_2_P: An any is accepted with a legal encoding type

    # Setup steps

    # Run the test
    test {principal create goober -attribute {pgo_test_any integer 101}
        dcp_attrlist getval [principal show goober -all] pgo_test_any
    } compare_strings {101}

    # Cleanup steps
    if {[dcp_wrap {principal delete {goober}}] != 0} {return;}
}

proc principal_create_era_any_3_P {} {
    # Identify the test
    tet_infoline principal_create_era_any_3_P: An any is accepted with a legal encoding type

    # Setup steps

    # Run the test
    test {principal create goober -attribute {pgo_test_any printstring "your mother was a hamster"}
        dcp_attrlist getval [principal show goober -all] pgo_test_any
    } compare_strings {{your mother was a hamster}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {goober}}] != 0} {return;}
}

proc principal_create_era_any_4_P {} {
    # Identify the test
    tet_infoline principal_create_era_any_4_P: An any is accepted with a legal encoding type

    # Setup steps

    # Run the test
    test {principal create goober -attribute {pgo_test_any stringarray "and your father smelt of elderberries"}
        dcp_attrlist getval [principal show goober -all] pgo_test_any
    } compare_strings {{and your father smelt of elderberries}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {goober}}] != 0} {return;}
}

proc principal_create_era_any_5_P {} {
    # Identify the test
    tet_infoline principal_create_era_any_5_P: An any is accepted with a legal encoding type

    # Setup steps

    # Run the test
    test {principal create goober -attribute {pgo_test_any byte a0a0a0}
        dcp_attrlist getval [principal show goober -all] pgo_test_any
    } compare_strings {{a0 a0 a0}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {goober}}] != 0} {return;}
}

proc principal_create_era_any_6_P {} {
    # Identify the test
    tet_infoline principal_create_era_any_6_P: An any is accepted with a legal encoding type

    # Setup steps

    # Run the test
    test {principal create goober -attribute {pgo_test_any i18ndata {roman8 "abcdef"}}
        dcp_attrlist getval [principal show goober -all] pgo_test_any
    } compare_strings {{roman8 {\x61\x62\x63\x64\x65\x66}}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {goober}}] != 0} {return;}
}

proc principal_create_era_any_7_P {} {
    # Identify the test
    tet_infoline principal_create_era_any_7_P: An any is accepted with a legal encoding type

    # Setup steps

    # Run the test
    test {principal create goober -attribute {pgo_test_any uuid 291e5274-7887-11cd-98a5-0800092464af}
        dcp_attrlist getval [principal show goober -all] pgo_test_any
    } compare_strings 291e5274-7887-11cd-98a5-0800092464af

    # Cleanup steps
    if {[dcp_wrap {principal delete {goober}}] != 0} {return;}
}

proc principal_create_era_any_8_P {} {
    # Identify the test
    tet_infoline principal_create_era_any_8_P: An any is accepted with a legal encoding type

    # Setup steps

    # Run the test
    test {principal create goober -attribute {pgo_test_any attrset 291e5274-7887-11cd-98a5-0800092464af}
        dcp_attrlist getval [principal show goober -all] pgo_test_any
    } compare_strings 291e5274-7887-11cd-98a5-0800092464af

    # Cleanup steps
    if {[dcp_wrap {principal delete {goober}}] != 0} {return;}
}

proc principal_create_era_any_9_P {} {
    # Identify the test
    tet_infoline principal_create_era_any_9_P: An any is accepted with a legal encoding type

    # Setup steps
    if {[dcp_wrap {global SECMASTER}] != 0} {return;}
    if {[dcp_wrap {set fbind "{dce cell_admin call secret dce} $SECMASTER"}] != 0} {return;}
    if {[dcp_wrap {set attrib [list "pgo_test_any binding [list $fbind]"]}] != 0} {return;}

    # Run the test
    test {principal create goober -attribute $attrib
        dcp_attrlist getval [principal show goober -all] pgo_test_any
    } compare_lists [format "{{dce cell_admin call secret dce} %s}" $SECMASTER]

    # Cleanup steps
    if {[dcp_wrap {principal delete {goober}}] != 0} {return;}
}

proc principal_create_era_any_10_P {} {
    # Identify the test
    tet_infoline principal_create_era_any_10_P: An any is accepted with multiple integers

    # Setup steps

    # Run the test
    test {principal create goober -attribute {{pgo_test_integer 101} {pgo_test_any integer 101 102 103}}
        dcp_attrlist getval [principal show goober -all] pgo_test_any
    } compare_strings {101 102 103}

    # Cleanup steps
    if {[dcp_wrap {principal delete {goober}}] != 0} {return;}
}

proc principal_create_era_any_11_P {} {
    # Identify the test
    tet_infoline principal_create_era_any_11_P: An any is accepted with multiple strings

    # Setup steps

    # Run the test
    test {principal create goober -attribute {{pgo_test_printstring "blah"} {pgo_test_any printstring "your mother" "was a hamster"}}
        dcp_attrlist getval [principal show goober -all] pgo_test_any
    } compare_lists {{your mother} {was a hamster}}

    # Cleanup steps
    if {[dcp_wrap {principal delete {goober}}] != 0} {return;}
}

proc principal_create_era_any_12_P {} {
    # Identify the test
    tet_infoline principal_create_era_any_12_P: An any is accepted with multiple uuids

    # Setup steps

    # Run the test
    test {principal create goober -attribute {{pgo_test_integer 101} {pgo_test_any uuid 291e5274-7887-11cd-98a5-0800092464af 291e5274-7887-11cd-98a5-0800092464ab}}
        dcp_attrlist getval [principal show goober -all] pgo_test_any
    } compare_lists {291e5274-7887-11cd-98a5-0800092464af 291e5274-7887-11cd-98a5-0800092464ab}

    # Cleanup steps
    if {[dcp_wrap {principal delete {goober}}] != 0} {return;}
}

proc principal_create_era_any_13_P {} {
    # Identify the test
    tet_infoline principal_create_era_any_13_P: An any is accepted with many anys

    # Setup steps

    # Run the test
    test {principal create goober -attribute {{pgo_test_integer 101} {pgo_test_any integer 101 102 103} {pgo_test_any printstring "your mother" "was a hamster" "and your father" "smelt of elderberries"}}
        dcp_attrlist getval [principal show goober -all] pgo_test_any
    } compare_strings {101 102 103}

    # Cleanup steps
    if {[dcp_wrap {principal delete {goober}}] != 0} {return;}
}

proc principal_create_era_any_14_P {} {
    # Identify the test
    tet_infoline principal_create_era_any_14_P: An any is accepted with many anys

    # Setup steps

    # Run the test
    test {principal create goober -attribute {{pgo_test_integer 101} {pgo_test_any integer 101} {pgo_test_any uuid 291e5274-7887-11cd-98a5-0800092464af 291e5274-7887-11cd-98a5-0800092464ab}}
        dcp_attrlist getval [principal show goober -all] pgo_test_any
    } compare_strings {101}

    # Cleanup steps
    if {[dcp_wrap {principal delete {goober}}] != 0} {return;}
}

proc principal_create_era_any_15_P {} {
    # Identify the test
    tet_infoline principal_create_era_any_15_P: An any is accepted with many anys

    # Setup steps

    # Run the test
    test {principal create goober -attribute {{pgo_test_any integer 101 102 103 104} {pgo_test_any printstring "your mother" "was a hamster"} {pgo_test_any stringarray "and your father" "smelt of elderberries"} {pgo_test_any byte "a0a0a0"} {pgo_test_any uuid 291e5274-7887-11cd-98a5-0800092464af}}
        dcp_attrlist getval [principal show goober -all] pgo_test_any
    } compare_strings {101 102 103 104}

    # Cleanup steps
    if {[dcp_wrap {principal delete {goober}}] != 0} {return;}
}

set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 ic25 ic26 ic27 ic28 ic29 ic30 ic31 ic32 ic33 ic34 ic35 ic36 ic37 ic38 ic39 ic40 ic41 ic42 ic43 ic44 ic45 ic46 ic47 ic48 ic49 ic50 ic51 ic52 ic53 ic54 ic55 ic56 ic57 ic58 ic59 ic60 ic61 ic62 ic63 ic64 ic65 ic66 ic67 ic68 ic69 ic70 ic71 ic72 ic73 ic74 ic75 ic76 ic77 ic78 ic79 ic80 ic81 ic82 ic83 ic84 ic85 ic86 ic87 ic88 ic89 ic90 ic91 ic92 ic93 ic94 ic95 ic96 "
set ic1 "principal_create_era_attr_set_1_P"
set ic2 "principal_create_era_attr_set_2_P"
set ic3 "principal_create_era_attr_set_3_P"
set ic4 "principal_create_era_attr_set_4_P"
set ic5 "principal_create_era_attr_set_5_P"
set ic6 "principal_create_era_attr_set_6_P"
set ic7 "principal_create_era_attr_set_7_P"
set ic8 "principal_create_era_attr_set_8_P"
set ic9 "principal_create_era_attr_set_9_P"
set ic10 "principal_create_era_attr_set_10_P"
set ic11 "principal_create_era_attr_set_11_P"
set ic12 "principal_create_era_attr_set_12_P"
set ic13 "principal_create_era_bytes_1_P"
set ic14 "principal_create_era_bytes_2_P"
set ic15 "principal_create_era_bytes_3_P"
set ic16 "principal_create_era_bytes_4_P"
set ic17 "principal_create_era_bytes_5_P"
set ic18 "principal_create_era_bytes_6_P"
set ic19 "principal_create_era_bytes_7_P"
set ic20 "principal_create_era_bytes_8_P"
set ic21 "principal_create_era_i18n_data_1_P"
set ic22 "principal_create_era_i18n_data_2_P"
set ic23 "principal_create_era_i18n_data_3_P"
set ic24 "principal_create_era_i18n_data_4_P"
set ic25 "principal_create_era_i18n_data_5_P"
set ic26 "principal_create_era_i18n_data_6_P"
set ic27 "principal_create_era_i18n_data_7_P"
set ic28 "principal_create_era_i18n_data_8_P"
set ic29 "principal_create_era_integer_1_P"
set ic30 "principal_create_era_integer_2_P"
set ic31 "principal_create_era_integer_3_P"
set ic32 "principal_create_era_integer_4_P"
set ic33 "principal_create_era_integer_5_P"
set ic34 "principal_create_era_integer_6_P"
set ic35 "principal_create_era_integer_7_P"
set ic36 "principal_create_era_integer_8_P"
set ic37 "principal_create_era_integer_9_P"
set ic38 "principal_create_era_printstring_1_P"
set ic39 "principal_create_era_printstring_2_P"
set ic40 "principal_create_era_printstring_3_P"
set ic41 "principal_create_era_printstring_4_P"
set ic42 "principal_create_era_printstring_5_P"
set ic43 "principal_create_era_printstring_6_P"
set ic44 "principal_create_era_printstring_7_P"
set ic45 "principal_create_era_printstring_8_P"
set ic46 "principal_create_era_printstring_array_1_P"
set ic47 "principal_create_era_printstring_array_2_P"
set ic48 "principal_create_era_printstring_array_3_P"
set ic49 "principal_create_era_printstring_array_4_P"
set ic50 "principal_create_era_printstring_array_5_P"
set ic51 "principal_create_era_printstring_array_6_P"
set ic52 "principal_create_era_printstring_array_7_P"
set ic53 "principal_create_era_printstring_array_8_P"
set ic54 "principal_create_era_printstring_array_9_P"
set ic55 "principal_create_era_printstring_array_10_P"
set ic56 "principal_create_era_printstring_array_11_P"
set ic57 "principal_create_era_printstring_array_12_P"
set ic58 "principal_create_era_printstring_array_13_P"
set ic59 "principal_create_era_printstring_array_14_P"
set ic60 "principal_create_era_printstring_array_15_P"
set ic61 "principal_create_era_uuid_1_P"
set ic62 "principal_create_era_uuid_2_P"
set ic63 "principal_create_era_uuid_3_P"
set ic64 "principal_create_era_uuid_4_P"
set ic65 "principal_create_era_uuid_5_P"
set ic66 "principal_create_era_uuid_6_P"
set ic67 "principal_create_era_uuid_7_P"
set ic68 "principal_create_era_uuid_8_P"
set ic69 "principal_create_era_void_1_P"
set ic70 "principal_create_era_void_2_P"
set ic71 "principal_create_era_void_3_P"
set ic72 "principal_create_era_binding_1_P"
set ic73 "principal_create_era_binding_2_P"
set ic74 "principal_create_era_binding_3_P"
set ic75 "principal_create_era_binding_4_P"
set ic76 "principal_create_era_binding_5_P"
set ic77 "principal_create_era_binding_6_P"
set ic78 "principal_create_era_binding_7_P"
set ic79 "principal_create_era_binding_8_P"
set ic80 "principal_create_era_binding_9_P"
set ic81 "principal_create_era_binding_10_P"
set ic82 "principal_create_era_any_1_P"
set ic83 "principal_create_era_any_2_P"
set ic84 "principal_create_era_any_3_P"
set ic85 "principal_create_era_any_4_P"
set ic86 "principal_create_era_any_5_P"
set ic87 "principal_create_era_any_6_P"
set ic88 "principal_create_era_any_7_P"
set ic89 "principal_create_era_any_8_P"
set ic90 "principal_create_era_any_9_P"
set ic91 "principal_create_era_any_10_P"
set ic92 "principal_create_era_any_11_P"
set ic93 "principal_create_era_any_12_P"
set ic94 "principal_create_era_any_13_P"
set ic95 "principal_create_era_any_14_P"
set ic96 "principal_create_era_any_15_P"
set tet_startup principal_create_era_startup
set tet_cleanup principal_create_era_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
