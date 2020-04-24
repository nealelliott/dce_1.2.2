#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: org_create_era_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:20:01  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:30  marty]
#
# Revision 1.1.4.2  1995/12/13  15:27:46  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/4  1995/07/12  12:53 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts38/1  1995/07/12  12:52 UTC  truitt
# 	CHFts15275: Add tests to support new i18ndata behavior.
# 
# 	HP revision /main/HPDCE02/3  1995/05/01  18:20 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts26/1  1995/05/01  18:18 UTC  truitt
# 	CHFts14653: Additional tests.
# 
# 	HP revision /main/HPDCE02/2  1994/12/13  18:38 UTC  truitt
# 	Merge.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_msg_chk/1  1994/12/13  18:36 UTC  truitt
# 	Fix incorrect error messages.
# 
# 	HP revision /main/HPDCE02/1  1994/12/01  16:01 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/12/01  16:00 UTC  truitt
# 	Fix reference to i18n data type.
# 	[1995/12/11  15:54:42  root]
# 
# Revision 1.1.2.2  1994/09/30  16:21:45  truitt
# 	OT12414: Update tests to reflect new show functionality.
# 	[1994/09/30  15:39:21  truitt]
# 
# Revision 1.1.2.1  1994/09/28  21:42:21  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:27  melman]
# 
# 	Initial creation.
# 	[1994/08/05  18:53:43  nav]
# 
# Revision 1.1.2.5  1994/09/23  17:01:32  truitt
# 	Updated to reflect new aclmgr names.
# 	[1994/09/23  16:56:00  truitt]
# 
# Revision 1.1.2.4  1994/09/16  18:25:18  truitt
# 	Update tests to reflect new functionality.
# 	[1994/09/16  18:21:08  truitt]
# 
# Revision 1.1.2.3  1994/08/24  19:07:14  truitt
# 	OT11448: Updated era tests
# 	[1994/08/24  18:58:06  truitt]
# 
# Revision 1.1.2.2  1994/08/16  17:30:05  coren
# 	Added tests for attributes of type "binding".
# 	[1994/08/11  18:43:32  coren]
# 
# Revision 1.1.2.1  1994/08/05  19:15:44  nav
# 	Initial creation.
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc organization_create_era_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_integer -encoding integer -aclmgr {organization r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_attr_set -encoding attrset -aclmgr {organization r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_bytes -encoding byte -aclmgr {organization r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_i18n_data -encoding i18ndata -aclmgr {organization r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_printstring -encoding printstring -aclmgr {organization r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_uuid -encoding uuid -aclmgr {organization r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_void -encoding void -aclmgr {organization r r r r} -multi no}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_printstring_array -encoding stringarray -aclmgr {organization r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_binding -encoding binding -aclmgr {organization r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_any -encoding any -aclmgr {organization r r r r} -multi yes}] != 0} {;}
}

proc organization_create_era_cleanup {} {
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
proc organization_create_era_attr_set_1_N {} {
    # Identify the test
    tet_infoline organization_create_era_attr_set_1_N: An attr_set attribute requires a value

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {pgo_test_attr_set}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc organization_create_era_attr_set_2_N {} {
    # Identify the test
    tet_infoline organization_create_era_attr_set_2_N: An attr_set attribute fails with bad UUID

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {pgo_test_attr_set 1234}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not a valid UUID.}}

    # Cleanup steps
}

proc organization_create_era_bytes_1_N {} {
    # Identify the test
    tet_infoline organization_create_era_bytes_1_N: A bytes attribute requires a value

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {pgo_test_bytes}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc organization_create_era_bytes_2_N {} {
    # Identify the test
    tet_infoline organization_create_era_bytes_2_N: A bytes attribute fails with bad string

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {pgo_test_bytes qztrzquwzqz}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not a valid byte sequence.}}

    # Cleanup steps
}

proc organization_create_era_i18n_data_1_N {} {
    # Identify the test
    tet_infoline organization_create_era_i18n_data_1_N: An i18n_data attribute requires a value

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {pgo_test_i18n_data}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc organization_create_era_i18n_data_2_N {} {
    # Identify the test
    tet_infoline organization_create_era_i18n_data_2_N: Incorrect i18n syntax

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {pgo_test_i18n_data roman8 "test string"}} msg] $msg
    } compare_strings {1 {The syntax of the specified i18ndata element is invalid.}}

    # Cleanup steps
}

proc organization_create_era_i18n_data_3_N {} {
    # Identify the test
    tet_infoline organization_create_era_i18n_data_3_N: Incorrect i18n syntax

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {pgo_test_i18n_data {roman8 test string}}} msg] $msg
    } compare_strings {1 {The syntax of the specified i18ndata element is invalid.}}

    # Cleanup steps
}

proc organization_create_era_i18n_data_4_N {} {
    # Identify the test
    tet_infoline organization_create_era_i18n_data_4_N: An incorrect codeset specified.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {pgo_test_i18n_data {101 "test string"}}} msg] $msg
    } compare_strings {1 {The specified codeset is invalid.}}

    # Cleanup steps
}

proc organization_create_era_i18n_data_5_N {} {
    # Identify the test
    tet_infoline organization_create_era_i18n_data_5_N: An incorrect codeset specified.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {pgo_test_i18n_data {gumby "test string"}}} msg] $msg
    } compare_strings {1 {The specified codeset is invalid.}}

    # Cleanup steps
}
proc organization_create_era_integer_1_N {} {
    # Identify the test
    tet_infoline organization_create_era_integer_1_N: An integer attribute requires a value

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {pgo_test_integer}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc organization_create_era_integer_2_N {} {
    # Identify the test
    tet_infoline organization_create_era_integer_2_N: An integer attribute fails with a bad value

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {pgo_test_integer abc}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not an integer.}}

    # Cleanup steps
}

proc organization_create_era_integer_3_N {} {
    # Identify the test
    tet_infoline organization_create_era_integer_3_N: An integer attribute fails with a too-large value

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {pgo_test_integer 99999999999999999999}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not less than 2147483647.}}

    # Cleanup steps
}

proc organization_create_era_printstring_1_N {} {
    # Identify the test
    tet_infoline organization_create_era_printstring_1_N: A printstring attribute requires a value

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {pgo_test_printstring}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc organization_create_era_printstring_array_1_N {} {
    # Identify the test
    tet_infoline organization_create_era_printstring_array_1_N: A printstring_array attribute requires a value

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {pgo_test_printstring_array}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc organization_create_era_uuid_1_N {} {
    # Identify the test
    tet_infoline organization_create_era_uuid_1_N: A uuid attribute requires a value

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {pgo_test_uuid}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc organization_create_era_uuid_2_N {} {
    # Identify the test
    tet_infoline organization_create_era_uuid_2_N: A uuid attribute fails with a bad UUID

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {pgo_test_uuid 1234}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not a valid UUID.}}

    # Cleanup steps
}

proc organization_create_era_void_1_N {} {
    # Identify the test
    tet_infoline organization_create_era_void_1_N: A type void requires a value

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {pgo_test_void}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc organization_create_era_void_2_N {} {
    # Identify the test
    tet_infoline organization_create_era_void_2_N: An attribute of type void fails with a bad value

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {pgo_test_void abc}} msg] $msg
    } compare_strings {1 {Supplied value for attribute of type void must be 'present'.}}

    # Cleanup steps
}

proc organization_create_era_void_3_N {} {
    # Identify the test
    tet_infoline organization_create_era_void_3_N: Only one void attribute is allowed on an object

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {{pgo_test_void present} {pgo_test_integer 101} {pgo_test_void present}}} msg] $msg
    } compare_strings {1 {Only one instance of the same 'void' attribute is allowed.}}

    # Cleanup steps
}

proc organization_create_era_binding_1_N {} {
    # Identify the test
    tet_infoline organization_create_era_binding_1_N: A binding attribute requires a value

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {pgo_test_binding}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc organization_create_era_binding_2_N {} {
    # Identify the test
    tet_infoline organization_create_era_binding_2_N: A binding attribute must be a list with at least two members

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {pgo_test_binding none}} msg] $msg
    } compare_strings {1 {Invalid binding syntax.}}

    # Cleanup steps
}

proc organization_create_era_binding_3_N {} {
    # Identify the test
    tet_infoline organization_create_era_binding_3_N: The security part of a binding must be none or dce

    # Setup steps
    if {[dcp_wrap {global SECMASTER}] != 0} {return;}
    if {[dcp_wrap {set fbind "non $SECMASTER"}] != 0} {return;}
    if {[dcp_wrap {set attrib [list "pgo_test_binding [list $fbind]"]}] != 0} {return;}

    # Run the test
    test {
        list [catch {organization create foob -attribute $attrib} msg] $msg
    } compare_strings {1 {Invalid binding syntax.}}

    # Cleanup steps
}

proc organization_create_era_binding_4_N {} {
    # Identify the test
    tet_infoline organization_create_era_binding_4_N: The security part of a dce binding must have five fields

    # Setup steps
    if {[dcp_wrap {global SECMASTER}] != 0} {return;}
    if {[dcp_wrap {set fbind "{dce cell_admin default default} $SECMASTER"}] != 0} {return;}
    if {[dcp_wrap {set attrib [list "pgo_test_binding [list $fbind]"]}] != 0} {return;}

    # Run the test
    test {
        list [catch {organization create foob -attribute $attrib} msg] $msg
    } compare_strings {1 {Invalid binding syntax.}}

    # Cleanup steps
}

proc organization_create_era_binding_5_N {} {
    # Identify the test
    tet_infoline organization_create_era_binding_5_N: The string-binding part of a binding must be valid

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {pgo_test_binding {none nonsense}}} msg] $msg
    } compare_strings {1 {Invalid binding syntax.}}

    # Cleanup steps
}

proc organization_create_era_binding_6_N {} {
    # Identify the test
    tet_infoline organization_create_era_binding_6_N: The tower part of a binding must be valid

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {pgo_test_binding {none {01 02 zz 0b}}}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not a valid byte sequence.}}

    # Cleanup steps
}

proc organization_create_era_binding_7_N {} {
    # Identify the test
    tet_infoline organization_create_era_binding_7_N: The tower part of a binding must be valid

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {pgo_test_binding {none {ncacn_ip_tcl:127.0.0.0[1234]}}}} msg] $msg
    } compare_strings {1 {Invalid RPC protocol sequence}}

    # Cleanup steps
}

proc organization_create_era_any_1_N {} {
    # Identify the test
    tet_infoline organization_create_era_any_1_N: An any must have a legal encoding type

    # Setup steps

    # Run the test
    test {
        list [catch {organization create gumby -attribute {pgo_test_any 101}} msg] $msg
    } compare_strings {1 {An 'any' attribute requires both a value and an encoding type.}}

    # Cleanup steps
}

proc organization_create_era_any_2_N {} {
    # Identify the test
    tet_infoline organization_create_era_any_2_N: An any must have a legal encoding type

    # Setup steps

    # Run the test
    test {
        list [catch {organization create gumby -attribute {pgo_test_any goober 101}} msg] $msg
    } compare_strings {1 {Encoding type specified for the 'any' attribute is unknown.}}

    # Cleanup steps
}

proc organization_create_era_any_3_N {} {
    # Identify the test
    tet_infoline organization_create_era_any_3_N: An any must have a legal encoding type

    # Setup steps

    # Run the test
    test {
        list [catch {organization create gumby -attribute {pgo_test_any a 101}} msg] $msg
    } compare_strings {1 {Encoding type specified for the 'any' attribute is ambiguous.}}

    # Cleanup steps
}

proc organization_create_era_any_4_N {} {
    # Identify the test
    tet_infoline organization_create_era_any_4_N: An any must have a legal encoding type

    # Setup steps

    # Run the test
    test {
        list [catch {organization create gumby -attribute {pgo_test_any any 101 102}} msg] $msg
    } compare_strings {1 {Encoding type 'any' cannot be used when specifying values for attributes.}}

    # Cleanup steps
}

proc organization_create_era_any_5_N {} {
    # Identify the test
    tet_infoline organization_create_era_any_5_N: An any integer attribute fails with a bad value

    # Setup steps

    # Run the test
    test {
        list [catch {organization create gumby -attribute {pgo_test_any integer abc}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not an integer.}}

    # Cleanup steps
}

proc organization_create_era_any_6_N {} {
    # Identify the test
    tet_infoline organization_create_era_any_6_N: An any uuid attribute fails with a bad value

    # Setup steps

    # Run the test
    test {
        list [catch {organization create gumby -attribute {pgo_test_any uuid 1234}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not a valid UUID.}}

    # Cleanup steps
}

proc organization_create_era_any_7_N {} {
    # Identify the test
    tet_infoline organization_create_era_any_7_N: An any attrset attribute fails with a bad value

    # Setup steps

    # Run the test
    test {
        list [catch {organization create gumby -attribute {pgo_test_any attrset 1234}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not a valid UUID.}}

    # Cleanup steps
}

proc organization_create_era_any_8_N {} {
    # Identify the test
    tet_infoline organization_create_era_any_8_N: An any byte attribute fails with a bad value

    # Setup steps

    # Run the test
    test {
        list [catch {organization create gumby -attribute {pgo_test_any byte qrtzqzvxzz}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not a valid byte sequence.}}

    # Cleanup steps
}

proc organization_create_era_any_9_N {} {
    # Identify the test
    tet_infoline organization_create_era_any_9_N: An any can have only one void instance

    # Setup steps

    # Run the test
    test {
        list [catch {organization create gumby -attribute {{pgo_test_any void present} {pgo_test_any void present}}} msg] $msg
    } compare_strings {1 {Only one instance of the same 'void' attribute is allowed.}}

    # Cleanup steps
}

proc organization_create_era_any_10_N {} {
    # Identify the test
    tet_infoline organization_create_era_any_10_N: An any can have only one void instance

    # Setup steps

    # Run the test
    test {
        list [catch {organization create gumby -attribute {pgo_test_any void present present}} msg] $msg
    } compare_strings {1 {Only one instance of the same 'void' attribute is allowed.}}

    # Cleanup steps
}

proc organization_create_era_any_11_N {} {
    # Identify the test
    tet_infoline organization_create_era_any_11_N: Only one record per stringarray

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foob -attribute {{pgo_test_any stringarray "gumby"} {pgo_test_any stringarray "pokey"}}} msg] $msg
    } compare_strings {1 {Illegal combination of attributes specified on command line.}}

    # Cleanup steps
}

set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 ic25 ic26 ic27 ic28 ic29 ic30 ic31 ic32 ic33 ic34 ic35 ic36 ic37 "
set ic1 "organization_create_era_attr_set_1_N"
set ic2 "organization_create_era_attr_set_2_N"
set ic3 "organization_create_era_bytes_1_N"
set ic4 "organization_create_era_bytes_2_N"
set ic5 "organization_create_era_i18n_data_1_N"
set ic6 "organization_create_era_i18n_data_2_N"
set ic7 "organization_create_era_i18n_data_3_N"
set ic8 "organization_create_era_i18n_data_4_N"
set ic9 "organization_create_era_i18n_data_5_N"
set ic10 "organization_create_era_integer_1_N"
set ic11 "organization_create_era_integer_2_N"
set ic12 "organization_create_era_integer_3_N"
set ic13 "organization_create_era_printstring_1_N"
set ic14 "organization_create_era_printstring_array_1_N"
set ic15 "organization_create_era_uuid_1_N"
set ic16 "organization_create_era_uuid_2_N"
set ic17 "organization_create_era_void_1_N"
set ic18 "organization_create_era_void_2_N"
set ic19 "organization_create_era_void_3_N"
set ic20 "organization_create_era_binding_1_N"
set ic21 "organization_create_era_binding_2_N"
set ic22 "organization_create_era_binding_3_N"
set ic23 "organization_create_era_binding_4_N"
set ic24 "organization_create_era_binding_5_N"
set ic25 "organization_create_era_binding_6_N"
set ic26 "organization_create_era_binding_7_N"
set ic27 "organization_create_era_any_1_N"
set ic28 "organization_create_era_any_2_N"
set ic29 "organization_create_era_any_3_N"
set ic30 "organization_create_era_any_4_N"
set ic31 "organization_create_era_any_5_N"
set ic32 "organization_create_era_any_6_N"
set ic33 "organization_create_era_any_7_N"
set ic34 "organization_create_era_any_8_N"
set ic35 "organization_create_era_any_9_N"
set ic36 "organization_create_era_any_10_N"
set ic37 "organization_create_era_any_11_N"
set tet_startup organization_create_era_startup
set tet_cleanup organization_create_era_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
