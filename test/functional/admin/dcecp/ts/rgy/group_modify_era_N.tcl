#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: group_modify_era_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:19:32  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:18  marty]
#
# Revision 1.1.4.2  1995/12/13  15:27:34  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/5  1995/07/12  12:42 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts38/1  1995/07/12  12:41 UTC  truitt
# 	CHFts15275: Add tests to support new i18ndata behavior.
# 
# 	HP revision /main/HPDCE02/4  1995/05/01  17:51 UTC  truitt
# 	Additional test change needed to support new void behavior.
# 
# 	HP revision /main/HPDCE02/3  1995/05/01  14:42 UTC  truitt
# 	Merge new tests.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts26/1  1995/05/01  14:42 UTC  truitt
# 	CHFts14653: Additional tests.
# 
# 	HP revision /main/HPDCE02/2  1995/03/02  20:05 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts17/1  1995/03/02  20:04 UTC  truitt
# 	Correct the encoding type of the confidential bytes
# 	schema object.
# 
# 	HP revision /main/HPDCE02/1  1994/12/13  18:41 UTC  truitt
# 	Merge.
# 
# 	HP revision /main/truitt_dcecp_msg_chk/1  1994/12/13  18:39 UTC  truitt
# 	Fix incorrect error messages.
# 	[1995/12/11  15:54:30  root]
# 
# Revision 1.1.2.1  1994/09/28  21:42:04  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:09  melman]
# 
# Revision 1.1.2.2  1994/09/16  18:25:12  truitt
# 	Update tests to reflect new functionality.
# 	[1994/09/16  18:20:31  truitt]
# 
# Revision 1.1.2.1  1994/08/26  18:17:01  truitt
# 	OT11448: Add modify era tests.
# 	[1994/08/26  18:13:07  truitt]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc group_modify_era_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_integer -encoding integer -aclmgr {group r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_attr_set -encoding attrset -aclmgr {group r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_bytes -encoding byte -aclmgr {group r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_confidential_bytes -encoding confidential -aclmgr {group r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_i18n_data -encoding i18ndata -aclmgr {group r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_printstring -encoding printstring -aclmgr {group r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_uuid -encoding uuid -aclmgr {group r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_void -encoding void -aclmgr {group r r r r} -multi no}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_printstring_array -encoding stringarray -aclmgr {group r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_binding -encoding binding -aclmgr {group r r r r} -multi yes}] != 0} {;}
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/pgo_test_any -encoding any -aclmgr {group r r r r} -multi yes}] != 0} {;}
}

proc group_modify_era_cleanup {} {
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
proc group_modify_era_1_N {} {
    # Identify the test
    tet_infoline group_modify_era_1_N: The modify command must have an attribute

    # Setup steps

    # Run the test
    test {
        list [catch {group modify} msg] $msg
    } compare_strings {1 {No group name.}}

    # Cleanup steps
}

proc group_modify_era_2_N {} {
    # Identify the test
    tet_infoline group_modify_era_2_N: Too many arguments passed to modify

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby pokey} msg] $msg
    } compare_strings {1 {Unrecognized argument 'pokey'.}}

    # Cleanup steps
}

proc group_modify_era_3_N {} {
    # Identify the test
    tet_infoline group_modify_era_3_N: No modify option specified

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby} msg] $msg
    } compare_strings {1 {No modify operation has been specified.}}

    # Cleanup steps
}

proc group_modify_era_4_N {} {
    # Identify the test
    tet_infoline group_modify_era_4_N: A bad option with no value passed

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -blech} msg] $msg
    } compare_strings {1 {Unrecognized option '-blech'.}}

    # Cleanup steps
}

proc group_modify_era_5_N {} {
    # Identify the test
    tet_infoline group_modify_era_5_N: A bad option with value passed

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -blech 101} msg] $msg
    } compare_strings {1 {Unrecognized option '-blech'.}}

    # Cleanup steps
}

proc group_modify_era_6_N {} {
    # Identify the test
    tet_infoline group_modify_era_6_N: Attribute switches aren't supported.

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -attribute} msg] $msg
    } compare_strings {1 {Unrecognized option '-attribute'.}}

    # Cleanup steps
}

proc group_modify_era_7_N {} {
    # Identify the test
    tet_infoline group_modify_era_7_N: No attribute information passed

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -add} msg] $msg
    } compare_strings {1 {The '-add' option requires a character string value.}}

    # Cleanup steps
}

proc group_modify_era_8_N {} {
    # Identify the test
    tet_infoline group_modify_era_8_N: No attribute information passed

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -change} msg] $msg
    } compare_strings {1 {The '-change' option requires a character string value.}}

    # Cleanup steps
}

proc group_modify_era_9_N {} {
    # Identify the test
    tet_infoline group_modify_era_9_N: No attribute information passed

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -remove} msg] $msg
    } compare_strings {1 {The '-remove' option requires a character string value.}}

    # Cleanup steps
}

proc group_modify_era_10_N {} {
    # Identify the test
    tet_infoline group_modify_era_10_N: Bad usage of types option

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -types} msg] $msg
    } compare_strings {1 {No modify operation has been specified.}}

    # Cleanup steps
}

proc group_modify_era_11_N {} {
    # Identify the test
    tet_infoline group_modify_era_11_N: No value passed for add of pgo_test_void

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -add {pgo_test_void}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc group_modify_era_12_N {} {
    # Identify the test
    tet_infoline group_modify_era_12_N: Bad usage of types option

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -add {pgo_test_void present} -types} msg] $msg
    } compare_strings {1 {Illegal use of '-type' option.}}

    # Cleanup steps
}

proc group_modify_era_13_N {} {
    # Identify the test
    tet_infoline group_modify_era_13_N: No value passed for add of pgo_test_void

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -add {pgo_test_void}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc group_modify_era_14_N {} {
    # Identify the test
    tet_infoline group_modify_era_14_N: No value passed for add of pgo_test_integer

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -add {pgo_test_integer}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc group_modify_era_15_N {} {
    # Identify the test
    tet_infoline group_modify_era_15_N: No value passed for add of pgo_test_printstring

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -add {pgo_test_printstring}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc group_modify_era_16_N {} {
    # Identify the test
    tet_infoline group_modify_era_16_N: No value passed for add of pgo_test_printstring_array

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -add {pgo_test_printstring_array}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc group_modify_era_17_N {} {
    # Identify the test
    tet_infoline group_modify_era_17_N: No value passed for add of pgo_test_bytes

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -add {pgo_test_bytes}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc group_modify_era_18_N {} {
    # Identify the test
    tet_infoline group_modify_era_18_N: No value passed for add of pgo_test_confidential_bytes

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -add {pgo_test_confidential_bytes}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc group_modify_era_19_N {} {
    # Identify the test
    tet_infoline group_modify_era_19_N: No value passed for add of pgo_test_i18n_data

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -add {pgo_test_i18n_data}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc group_modify_era_20_N {} {
    # Identify the test
    tet_infoline group_modify_era_20_N: No value passed for add of pgo_test_uuid

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -add {pgo_test_uuid}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc group_modify_era_21_N {} {
    # Identify the test
    tet_infoline group_modify_era_21_N: No value passed for add of pgo_test_attr_set

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -add {pgo_test_attr_set}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc group_modify_era_22_N {} {
    # Identify the test
    tet_infoline group_modify_era_22_N: No value passed for add of pgo_test_binding

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -add {pgo_test_binding}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc group_modify_era_23_N {} {
    # Identify the test
    tet_infoline group_modify_era_23_N: No value passed for change of pgo_test_void

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -change {pgo_test_void}} msg] $msg
    } compare_strings {1 {Cannot change an attribute which has an encoding type of 'void'.}}

    # Cleanup steps
}

proc group_modify_era_24_N {} {
    # Identify the test
    tet_infoline group_modify_era_24_N: No value passed for change of pgo_test_integer

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -change {pgo_test_integer}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc group_modify_era_25_N {} {
    # Identify the test
    tet_infoline group_modify_era_25_N: No value passed for change of pgo_test_printstring

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -change {pgo_test_printstring}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc group_modify_era_26_N {} {
    # Identify the test
    tet_infoline group_modify_era_26_N: No value passed for change of pgo_test_printstring_array

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -change {pgo_test_printstring_array}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc group_modify_era_27_N {} {
    # Identify the test
    tet_infoline group_modify_era_27_N: No value passed for change of pgo_test_bytes

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -change {pgo_test_bytes}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc group_modify_era_28_N {} {
    # Identify the test
    tet_infoline group_modify_era_28_N: No value passed for change of pgo_test_confidential_bytes

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -change {pgo_test_confidential_bytes}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc group_modify_era_29_N {} {
    # Identify the test
    tet_infoline group_modify_era_29_N: No value passed for change of pgo_test_i18n_data

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -change {pgo_test_i18n_data}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc group_modify_era_30_N {} {
    # Identify the test
    tet_infoline group_modify_era_30_N: No value passed for change of pgo_test_uuid

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -change {pgo_test_uuid}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc group_modify_era_31_N {} {
    # Identify the test
    tet_infoline group_modify_era_31_N: No value passed for change of pgo_test_attr_set

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -change {pgo_test_attr_set}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc group_modify_era_32_N {} {
    # Identify the test
    tet_infoline group_modify_era_32_N: No value passed for change of pgo_test_binding

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -change {pgo_test_binding}} msg] $msg
    } compare_strings {1 {Incorrect number of values for attribute type.}}

    # Cleanup steps
}

proc group_modify_era_33_N {} {
    # Identify the test
    tet_infoline group_modify_era_33_N: Remove a pgo_test_void from non-existent object

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -remove {pgo_test_void}} msg] $msg
    } compare_strings {1 {Registry object not found}}

    # Cleanup steps
}

proc group_modify_era_34_N {} {
    # Identify the test
    tet_infoline group_modify_era_34_N: Remove a pgo_test_integer from non-existent object

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -remove {pgo_test_integer}} msg] $msg
    } compare_strings {1 {Registry object not found}}

    # Cleanup steps
}

proc group_modify_era_35_N {} {
    # Identify the test
    tet_infoline group_modify_era_35_N: Remove a pgo_test_printstring from non-existent object

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -remove {pgo_test_printstring}} msg] $msg
    } compare_strings {1 {Registry object not found}}

    # Cleanup steps
}

proc group_modify_era_36_N {} {
    # Identify the test
    tet_infoline group_modify_era_36_N: Remove a pgo_test_printstring_array from non-existent object

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -remove {pgo_test_printstring_array}} msg] $msg
    } compare_strings {1 {Registry object not found}}

    # Cleanup steps
}

proc group_modify_era_37_N {} {
    # Identify the test
    tet_infoline group_modify_era_37_N: Remove a pgo_test_bytes from non-existent object

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -remove {pgo_test_bytes}} msg] $msg
    } compare_strings {1 {Registry object not found}}

    # Cleanup steps
}

proc group_modify_era_38_N {} {
    # Identify the test
    tet_infoline group_modify_era_38_N: Remove a pgo_test_confidential_bytes from non-existent object

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -remove {pgo_test_confidential_bytes}} msg] $msg
    } compare_strings {1 {Registry object not found}}

    # Cleanup steps
}

proc group_modify_era_39_N {} {
    # Identify the test
    tet_infoline group_modify_era_39_N: Remove a pgo_test_i18n_data from non-existent object

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -remove {pgo_test_i18n_data}} msg] $msg
    } compare_strings {1 {Registry object not found}}

    # Cleanup steps
}

proc group_modify_era_40_N {} {
    # Identify the test
    tet_infoline group_modify_era_40_N: Remove a pgo_test_uuid from non-existent object

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -remove {pgo_test_uuid}} msg] $msg
    } compare_strings {1 {Registry object not found}}

    # Cleanup steps
}

proc group_modify_era_41_N {} {
    # Identify the test
    tet_infoline group_modify_era_41_N: Remove a pgo_test_attr_set from non-existent object

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -remove {pgo_test_attr_set}} msg] $msg
    } compare_strings {1 {Registry object not found}}

    # Cleanup steps
}

proc group_modify_era_42_N {} {
    # Identify the test
    tet_infoline group_modify_era_42_N: Remove a pgo_test_binding from non-existent object

    # Setup steps

    # Run the test
    test {
        list [catch {group modify gumby -remove {pgo_test_binding}} msg] $msg
    } compare_strings {1 {Registry object not found}}

    # Cleanup steps
}

proc group_modify_era_43_N {} {
    # Identify the test
    tet_infoline group_modify_era_43_N: Remove a non-existent pgo_test_void from an object

    # Setup steps
    if {[dcp_wrap {group create gumby_43_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_43_N -remove {pgo_test_void}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_43_N}] != 0} {return;}
}

proc group_modify_era_44_N {} {
    # Identify the test
    tet_infoline group_modify_era_44_N: Remove a non-existent pgo_test_integer from an object

    # Setup steps
    if {[dcp_wrap {group create gumby_44_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_44_N -remove {pgo_test_integer}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_44_N}] != 0} {return;}
}

proc group_modify_era_45_N {} {
    # Identify the test
    tet_infoline group_modify_era_45_N: Remove a non-existent pgo_test_printstring from an object

    # Setup steps
    if {[dcp_wrap {group create gumby_45_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_45_N -remove {pgo_test_printstring}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_45_N}] != 0} {return;}
}

proc group_modify_era_46_N {} {
    # Identify the test
    tet_infoline group_modify_era_46_N: Remove a non-existent pgo_test_printstring_array from an object

    # Setup steps
    if {[dcp_wrap {group create gumby_46_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_46_N -remove {pgo_test_printstring_array}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_46_N}] != 0} {return;}
}

proc group_modify_era_47_N {} {
    # Identify the test
    tet_infoline group_modify_era_47_N: Remove a non-existent pgo_test_bytes from an object

    # Setup steps
    if {[dcp_wrap {group create gumby_47_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_47_N -remove {pgo_test_bytes}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_47_N}] != 0} {return;}
}

proc group_modify_era_48_N {} {
    # Identify the test
    tet_infoline group_modify_era_48_N: Remove a non-existent pgo_test_confidential_bytes from an object

    # Setup steps
    if {[dcp_wrap {group create gumby_48_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_48_N -remove {pgo_test_confidential_bytes}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_48_N}] != 0} {return;}
}

proc group_modify_era_49_N {} {
    # Identify the test
    tet_infoline group_modify_era_49_N: Remove a non-existent pgo_test_i18n_data from an object

    # Setup steps
    if {[dcp_wrap {group create gumby_49_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_49_N -remove {pgo_test_i18n_data}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_49_N}] != 0} {return;}
}

proc group_modify_era_50_N {} {
    # Identify the test
    tet_infoline group_modify_era_50_N: Remove a non-existent pgo_test_uuid from an object

    # Setup steps
    if {[dcp_wrap {group create gumby_50_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_50_N -remove {pgo_test_uuid}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_50_N}] != 0} {return;}
}

proc group_modify_era_51_N {} {
    # Identify the test
    tet_infoline group_modify_era_51_N: Remove a non-existent pgo_test_attr_set from an object

    # Setup steps
    if {[dcp_wrap {group create gumby_51_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_51_N -remove {pgo_test_attr_set}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_51_N}] != 0} {return;}
}

proc group_modify_era_52_N {} {
    # Identify the test
    tet_infoline group_modify_era_52_N: Remove a non-existent pgo_test_binding from an object

    # Setup steps
    if {[dcp_wrap {group create gumby_52_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_52_N -remove {pgo_test_binding}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_52_N}] != 0} {return;}
}

proc group_modify_era_53_N {} {
    # Identify the test
    tet_infoline group_modify_era_53_N: Remove a non-existent pgo_test_integer from an object

    # Setup steps
    if {[dcp_wrap {group create gumby_53_N -attribute {pgo_test_integer 101}}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_53_N -remove {pgo_test_integer 102}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_53_N}] != 0} {return;}
}

proc group_modify_era_54_N {} {
    # Identify the test
    tet_infoline group_modify_era_54_N: Remove a non-existent pgo_test_printstring from an object

    # Setup steps
    if {[dcp_wrap {group create gumby_54_N -attribute {pgo_test_printstring "aaa"}}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_54_N -remove {pgo_test_printstring "bbb"}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_54_N}] != 0} {return;}
}

proc group_modify_era_55_N {} {
    # Identify the test
    tet_infoline group_modify_era_55_N: Remove a non-existent pgo_test_printstring_array from an object

    # Setup steps
    if {[dcp_wrap {group create gumby_55_N -attribute {pgo_test_printstring_array "aaa" "bbb"}}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_55_N -remove {pgo_test_printstring_array "aa" "bb" "cc"}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_55_N}] != 0} {return;}
}

proc group_modify_era_56_N {} {
    # Identify the test
    tet_infoline group_modify_era_56_N: Remove a non-existent pgo_test_bytes from an object

    # Setup steps
    if {[dcp_wrap {group create gumby_56_N -attribute {pgo_test_bytes "a0a0"}}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_56_N -remove {pgo_test_bytes "b1b1"}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_56_N}] != 0} {return;}
}

proc group_modify_era_57_N {} {
    # Identify the test
    tet_infoline group_modify_era_57_N: Remove a non-existent pgo_test_confidential_bytes from an object

    # Setup steps
    if {[dcp_wrap {group create gumby_57_N -attribute {pgo_test_confidential_bytes "a0a0"}}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_57_N -remove {pgo_test_confidential_bytes "b1b1"}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_57_N}] != 0} {return;}
}

proc group_modify_era_58_N {} {
    # Identify the test
    tet_infoline group_modify_era_58_N: Remove a non-existent pgo_test_i18n_data from an object

    # Setup steps
    if {[dcp_wrap {group create gumby_58_N -attribute {pgo_test_i18n_data {roman8 "aaa"}}}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_58_N -remove {pgo_test_i18n_data {roman8 "bbb"}}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_58_N}] != 0} {return;}
}

proc group_modify_era_59_N {} {
    # Identify the test
    tet_infoline group_modify_era_59_N: Remove a non-existent pgo_test_uuid from an object

    # Setup steps
    if {[dcp_wrap {group create gumby_59_N -attribute {pgo_test_uuid 442bb364-cf34-11cd-bdb6-080009247461}}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_59_N -remove {pgo_test_uuid 6e567908-cf34-11cd-9294-080009247461}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_59_N}] != 0} {return;}
}

proc group_modify_era_60_N {} {
    # Identify the test
    tet_infoline group_modify_era_60_N: Remove a non-existent pgo_test_attr_set from an object

    # Setup steps
    if {[dcp_wrap {group create gumby_60_N -attribute {pgo_test_attr_set 442bb364-cf34-11cd-bdb6-080009247461}}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_60_N -remove {pgo_test_attr_set 6e567908-cf34-11cd-9294-080009247461}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_60_N}] != 0} {return;}
}

proc group_modify_era_61_N {} {
    # Identify the test
    tet_infoline group_modify_era_61_N: Remove a non-existent pgo_test_binding from an object

    # Setup steps
    if {[dcp_wrap {group create gumby_61_N -attribute {pgo_test_binding {none /.:/gumby}}}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_61_N -remove {pgo_test_binding {none /.:/pokey}}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_61_N}] != 0} {return;}
}

proc group_modify_era_62_N {} {
    # Identify the test
    tet_infoline group_modify_era_62_N: Add a pgo_test_void with a bad value

    # Setup steps
    if {[dcp_wrap {group create gumby_62_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_62_N -add {pgo_test_void goober}} msg] $msg
    } compare_strings {1 {Supplied value for attribute of type void must be 'present'.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_62_N}] != 0} {return;}
}

proc group_modify_era_63_N {} {
    # Identify the test
    tet_infoline group_modify_era_63_N: Add a pgo_test_integer with an illegal value

    # Setup steps
    if {[dcp_wrap {group create gumby_63_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_63_N -add {pgo_test_integer goober}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not an integer.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_63_N}] != 0} {return;}
}

proc group_modify_era_64_N {} {
    # Identify the test
    tet_infoline group_modify_era_64_N: Add a pgo_test_integer with an out of bounds value

    # Setup steps
    if {[dcp_wrap {group create gumby_64_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_64_N -add {pgo_test_integer 999999999999}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not less than 2147483647.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_64_N}] != 0} {return;}
}

proc group_modify_era_65_N {} {
    # Identify the test
    tet_infoline group_modify_era_65_N: Add bad value to a pgo_test_bytes

    # Setup steps
    if {[dcp_wrap {group create gumby_65_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_65_N -add {pgo_test_bytes "zqdsdz"}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not a valid byte sequence.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_65_N}] != 0} {return;}
}

proc group_modify_era_66_N {} {
    # Identify the test
    tet_infoline group_modify_era_66_N: Add bad value to a pgo_test_confidential_bytes

    # Setup steps
    if {[dcp_wrap {group create gumby_66_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_66_N -add {pgo_test_confidential_bytes "zwqwzd"}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not a valid byte sequence.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_66_N}] != 0} {return;}
}

proc group_modify_era_67_N {} {
    # Identify the test
    tet_infoline group_modify_era_67_N: Add a bad value to a pgo_test_uuid

    # Setup steps
    if {[dcp_wrap {group create gumby_67_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_67_N -add {pgo_test_uuid goober}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not a valid UUID.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_67_N}] != 0} {return;}
}

proc group_modify_era_68_N {} {
    # Identify the test
    tet_infoline group_modify_era_68_N: Add a bad uuid to a pgo_test_attr_set

    # Setup steps
    if {[dcp_wrap {group create gumby_68_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_68_N -add {pgo_test_attr_set goober}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not a valid UUID.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_68_N}] != 0} {return;}
}

proc group_modify_era_69_N {} {
    # Identify the test
    tet_infoline group_modify_era_69_N: Add a good and a bad uuid to a pgo_test_attr_set

    # Setup steps
    if {[dcp_wrap {group create gumby_69_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_69_N -add {pgo_test_attr_set b3ad5eac-beb0-11cd-bb84-080009247461 goober}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not a valid UUID.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_69_N}] != 0} {return;}
}

proc group_modify_era_70_N {} {
    # Identify the test
    tet_infoline group_modify_era_70_N: Add a bad binding to pgo_test_binding

    # Setup steps
    if {[dcp_wrap {group create gumby_70_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_70_N -add {pgo_test_binding blech}} msg] $msg
    } compare_strings {1 {Invalid binding syntax.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_70_N}] != 0} {return;}
}

proc group_modify_era_71_N {} {
    # Identify the test
    tet_infoline group_modify_era_71_N: Add a bad binding to pgo_test_binding

    # Setup steps
    if {[dcp_wrap {group create gumby_71_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_71_N -add {pgo_test_binding blech}} msg] $msg
    } compare_strings {1 {Invalid binding syntax.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_71_N}] != 0} {return;}
}

proc group_modify_era_72_N {} {
    # Identify the test
    tet_infoline group_modify_era_72_N: Add a bad binding to pgo_test_binding

    # Setup steps
    if {[dcp_wrap {group create gumby_72_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_72_N -add {pgo_test_binding {blech {/.:/blech}}}} msg] $msg
    } compare_strings {1 {Invalid binding syntax.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_72_N}] != 0} {return;}
}

proc group_modify_era_73_N {} {
    # Identify the test
    tet_infoline group_modify_era_73_N: Add a bad binding to pgo_test_binding

    # Setup steps
    if {[dcp_wrap {group create gumby_73_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_73_N -add {pgo_test_binding {none {blech}}}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not a valid byte sequence.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_73_N}] != 0} {return;}
}

proc group_modify_era_74_N {} {
    # Identify the test
    tet_infoline group_modify_era_74_N: Add a bad binding to pgo_test_binding

    # Setup steps
    if {[dcp_wrap {group create gumby_74_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_74_N -add {pgo_test_binding {none blech blech}}} msg] $msg
    } compare_strings {1 {Invalid binding syntax.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_74_N}] != 0} {return;}
}

proc group_modify_era_75_N {} {
    # Identify the test
    tet_infoline group_modify_era_75_N: Add a bad binding to pgo_test_binding

    # Setup steps
    if {[dcp_wrap {group create gumby_75_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_75_N -add {pgo_test_binding {{dce} {/.:/blech}}}} msg] $msg
    } compare_strings {1 {Invalid binding syntax.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_75_N}] != 0} {return;}
}

proc group_modify_era_76_N {} {
    # Identify the test
    tet_infoline group_modify_era_76_N: Add a bad binding to pgo_test_binding

    # Setup steps
    if {[dcp_wrap {group create gumby_76_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_76_N -add {pgo_test_binding {{dce goober blech blech blech} {/.:/blech}}}} msg] $msg
    } compare_strings {1 {Invalid protection level.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_76_N}] != 0} {return;}
}

proc group_modify_era_77_N {} {
    # Identify the test
    tet_infoline group_modify_era_77_N: Add a bad binding to pgo_test_binding

    # Setup steps
    if {[dcp_wrap {group create gumby_77_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_77_N -add {pgo_test_binding {{dce goober default blech blech} {/.:/blech}}}} msg] $msg
    } compare_strings {1 {Invalid authentication service.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_77_N}] != 0} {return;}
}

proc group_modify_era_78_N {} {
    # Identify the test
    tet_infoline group_modify_era_78_N: Add a bad binding to pgo_test_binding

    # Setup steps
    if {[dcp_wrap {group create gumby_78_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_78_N -add {pgo_test_binding {{dce goober none none blech} {/.:/blech}}}} msg] $msg
    } compare_strings {1 {Invalid authorization service.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_78_N}] != 0} {return;}
}

proc group_modify_era_79_N {} {
    # Identify the test
    tet_infoline group_modify_era_79_N: Add a bad binding to pgo_test_binding

    # Setup steps
    if {[dcp_wrap {group create gumby_79_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_79_N -add {pgo_test_binding {{dce goober connect secret name blech} {/.:/blech}}}} msg] $msg
    } compare_strings {1 {Invalid binding syntax.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_79_N}] != 0} {return;}
}

proc group_modify_era_80_N {} {
    # Identify the test
    tet_infoline group_modify_era_80_N: Add a bad binding to pgo_test_binding

    # Setup steps
    if {[dcp_wrap {group create gumby_80_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_80_N -add {pgo_test_binding {{blech goober call public name} {/.:/blech}}}} msg] $msg
    } compare_strings {1 {Invalid binding syntax.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_80_N}] != 0} {return;}
}

proc group_modify_era_81_N {} {
    # Identify the test
    tet_infoline group_modify_era_81_N: Add a bad binding to pgo_test_binding

    # Setup steps
    if {[dcp_wrap {group create gumby_81_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_81_N -add {pgo_test_binding {{dce goober call public name} {blech}}}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not a valid byte sequence.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_81_N}] != 0} {return;}
}

proc group_modify_era_82_N {} {
    # Identify the test
    tet_infoline group_modify_era_82_N: Add a bad binding to pgo_test_binding

    # Setup steps
    if {[dcp_wrap {group create gumby_82_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_82_N -add {pgo_test_binding {none {a0 a1 z0 z1}}}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not a valid byte sequence.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_82_N}] != 0} {return;}
}

proc group_modify_era_83_N {} {
    # Identify the test
    tet_infoline group_modify_era_83_N: Try to change a non-existent attribute on an existing object.

    # Setup steps
    if {[dcp_wrap {group create gumby_83_N -attribute {pgo_test_void present}}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_83_N -change {pgo_test_integer 101}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_83_N}] != 0} {return;}
}

proc group_modify_era_84_N {} {
    # Identify the test
    tet_infoline group_modify_era_84_N: Try to change a non-existent attribute on an existing object.

    # Setup steps
    if {[dcp_wrap {group create gumby_84_N -attribute {pgo_test_void present}}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_84_N -change {pgo_test_printstring "blech"}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_84_N}] != 0} {return;}
}

proc group_modify_era_85_N {} {
    # Identify the test
    tet_infoline group_modify_era_85_N: Try to change a non-existent attribute on an existing object.

    # Setup steps
    if {[dcp_wrap {group create gumby_85_N -attribute {pgo_test_void present}}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_85_N -change {pgo_test_printstring_array "aaa" "bbb"}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_85_N}] != 0} {return;}
}

proc group_modify_era_86_N {} {
    # Identify the test
    tet_infoline group_modify_era_86_N: Try to change a non-existent attribute on an existing object.

    # Setup steps
    if {[dcp_wrap {group create gumby_86_N -attribute {pgo_test_void present}}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_86_N -change {pgo_test_bytes "blech"}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not a valid byte sequence.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_86_N}] != 0} {return;}
}

proc group_modify_era_87_N {} {
    # Identify the test
    tet_infoline group_modify_era_87_N: Try to change a non-existent attribute on an existing object.

    # Setup steps
    if {[dcp_wrap {group create gumby_87_N -attribute {pgo_test_void present}}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_87_N -change {pgo_test_confidential_bytes "blech"}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not a valid byte sequence.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_87_N}] != 0} {return;}
}

proc group_modify_era_88_N {} {
    # Identify the test
    tet_infoline group_modify_era_88_N: Try to change a non-existent attribute on an existing object.

    # Setup steps
    if {[dcp_wrap {group create gumby_88_N -attribute {pgo_test_void present}}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_88_N -change {pgo_test_i18n_data {roman8 "blech"}}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_88_N}] != 0} {return;}
}

proc group_modify_era_89_N {} {
    # Identify the test
    tet_infoline group_modify_era_89_N: Try to change a non-existent attribute on an existing object.

    # Setup steps
    if {[dcp_wrap {group create gumby_89_N -attribute {pgo_test_void present}}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_89_N -change {pgo_test_uuid d52d11f6-bf66-11cd-b03f-080009247461}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_89_N}] != 0} {return;}
}

proc group_modify_era_90_N {} {
    # Identify the test
    tet_infoline group_modify_era_90_N: Try to change a non-existent attribute on an existing object.

    # Setup steps
    if {[dcp_wrap {group create gumby_90_N -attribute {pgo_test_void present}}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_90_N -change {pgo_test_attr_set d52d11f6-bf66-11cd-b03f-080009247461 d79de74e-bf66-11cd-89a4-080009247461}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_90_N}] != 0} {return;}
}

proc group_modify_era_91_N {} {
    # Identify the test
    tet_infoline group_modify_era_91_N: Try to change a non-existent attribute on an existing object.

    # Setup steps
    if {[dcp_wrap {group create gumby_91_N -attribute {pgo_test_void present}}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_91_N -change {pgo_test_binding {none {/.:/blech}}}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_91_N}] != 0} {return;}
}

proc group_modify_era_92_N {} {
    # Identify the test
    tet_infoline group_modify_era_92_N: Try to add too many voids

    # Setup steps
    if {[dcp_wrap {group create gumby_92_N -attribute {pgo_test_void present}}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_92_N -add {pgo_test_void present}} msg] $msg
    } compare_strings {1 {An instance of the specified 'void' attribute already exists.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_92_N}] != 0} {return;}
}

proc group_modify_era_93_N {} {
    # Identify the test
    tet_infoline group_modify_era_93_N: Try to add too many voids

    # Setup steps
    if {[dcp_wrap {group create gumby_93_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_93_N -add {{pgo_test_void present} {pgo_test_integer 101} {pgo_test_void present}}} msg] $msg
    } compare_strings {1 {Only one instance of the same 'void' attribute is allowed.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_93_N}] != 0} {return;}
}

proc group_modify_era_94_N {} {
    # Identify the test
    tet_infoline group_modify_era_94_N: Try to add too many voids

    # Setup steps
    if {[dcp_wrap {group create gumby_94_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_94_N -add {pgo_test_void present present}} msg] $msg
    } compare_strings {1 {Only one instance of the same 'void' attribute is allowed.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_94_N}] != 0} {return;}
}

proc group_modify_era_95_N {} {
    # Identify the test
    tet_infoline group_modify_era_95_N: Try to remove too many voids

    # Setup steps
    if {[dcp_wrap {group create gumby_95_N -attr {pgo_test_void present}}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_95_N -remove {{pgo_test_void present} {pgo_test_void present}}} msg] $msg
    } compare_strings {1 {Only one instance of the same 'void' attribute is allowed.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_95_N}] != 0} {return;}
}

proc group_modify_era_96_N {} {
    # Identify the test
    tet_infoline group_modify_era_96_N: Try to remove too many voids

    # Setup steps
    if {[dcp_wrap {group create gumby_96_N -attr {pgo_test_void present}}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_96_N -remove {pgo_test_void pgo_test_void} -types} msg] $msg
    } compare_strings {1 {Only one instance of the same 'void' attribute is allowed.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_96_N}] != 0} {return;}
}

proc group_modify_era_97_N {} {
    # Identify the test
    tet_infoline group_modify_era_97_N: Try to add a bad any attribute

    # Setup steps
    if {[dcp_wrap {group create gumby_97_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_97_N -add {pgo_test_any integer abc}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not an integer.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_97_N}] != 0} {return;}
}

proc group_modify_era_98_N {} {
    # Identify the test
    tet_infoline group_modify_era_98_N: Try to add a bad any attribute

    # Setup steps
    if {[dcp_wrap {group create gumby_98_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_98_N -add {pgo_test_any byte erwerwe}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not a valid byte sequence.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_98_N}] != 0} {return;}
}

proc group_modify_era_99_N {} {
    # Identify the test
    tet_infoline group_modify_era_99_N: Try to add a bad any attribute

    # Setup steps
    if {[dcp_wrap {group create gumby_99_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_99_N -add {pgo_test_any uuid 1234}} msg] $msg
    } compare_strings {1 {Supplied value for extended attribute is not a valid UUID.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_99_N}] != 0} {return;}
}

proc group_modify_era_100_N {} {
    # Identify the test
    tet_infoline group_modify_era_100_N: Try to add a bad any attribute

    # Setup steps
    if {[dcp_wrap {group create gumby_100_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_100_N -add {pgo_test_any void gumby}} msg] $msg
    } compare_strings {1 {Supplied value for attribute of type void must be 'present'.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_100_N}] != 0} {return;}
}

proc group_modify_era_101_N {} {
    # Identify the test
    tet_infoline group_modify_era_101_N: Try to add a bad any attribute

    # Setup steps
    if {[dcp_wrap {group create gumby_101_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_101_N -add {pgo_test_any void present present}} msg] $msg
    } compare_strings {1 {Only one instance of the same 'void' attribute is allowed.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_101_N}] != 0} {return;}
}

proc group_modify_era_102_N {} {
    # Identify the test
    tet_infoline group_modify_era_102_N: Try to change a non-existent any attribute

    # Setup steps
    if {[dcp_wrap {group create gumby_102_N -attr {{pgo_test_any int 101} {pgo_test_any printstr "gober"}}}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_102_N -change {pgo_test_any byte "a0a0a0"}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_102_N}] != 0} {return;}
}

proc group_modify_era_103_N {} {
    # Identify the test
    tet_infoline group_modify_era_103_N: Try to change a void any attribute

    # Setup steps
    if {[dcp_wrap {group create gumby_103_N -attr {pgo_test_any void present}}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_103_N -change {pgo_test_any void present}} msg] $msg
    } compare_strings {1 {Cannot change an attribute which has an encoding type of 'void'.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_103_N}] != 0} {return;}
}

proc group_modify_era_104_N {} {
    # Identify the test
    tet_infoline group_modify_era_104_N: Try to remove non-existent any attribute

    # Setup steps
    if {[dcp_wrap {group create gumby_104_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_104_N -remove {pgo_test_any}} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_104_N}] != 0} {return;}
}

proc group_modify_era_105_N {} {
    # Identify the test
    tet_infoline group_modify_era_105_N: Try to remove non-existent any attribute

    # Setup steps
    if {[dcp_wrap {group create gumby_105_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_105_N -remove {pgo_test_any} -types} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_105_N}] != 0} {return;}
}

proc group_modify_era_106_N {} {
    # Identify the test
    tet_infoline group_modify_era_106_N: Try to remove non-existent any attribute

    # Setup steps
    if {[dcp_wrap {group create gumby_106_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_106_N -remove {pgo_test_any printstring} -types} msg] $msg
    } compare_strings {1 {The specified extended attribute does not exist.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_106_N}] != 0} {return;}
}

proc group_modify_era_107_N {} {
    # Identify the test
    tet_infoline group_modify_era_107_N: Try to remove any attribute the wrong way

    # Setup steps
    if {[dcp_wrap {group create gumby_107_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_107_N -remove {pgo_test_any printstring}} msg] $msg
    } compare_strings {1 {Improper syntax used when attempting to remove an 'any' attribute.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_107_N}] != 0} {return;}
}

proc group_modify_era_108_N {} {
    # Identify the test
    tet_infoline group_modify_era_108_N: Try to remove any attribute the wrong way

    # Setup steps
    if {[dcp_wrap {group create gumby_108_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_108_N -remove {pgo_test_any 101 102}} msg] $msg
    } compare_strings {1 {Encoding type specified for the 'any' attribute is unknown.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_108_N}] != 0} {return;}
}

proc group_modify_era_109_N {} {
    # Identify the test
    tet_infoline group_modify_era_109_N: Try to remove any attribute the wrong way

    # Setup steps
    if {[dcp_wrap {group create gumby_109_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_109_N -remove {pgo_test_integer pgo_test_any 101} -types} msg] $msg
    } compare_strings {1 {Schema entry '101' not found.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_109_N}] != 0} {return;}
}

proc group_modify_era_110_N {} {
    # Identify the test
    tet_infoline group_modify_era_110_N: Try to remove any attribute the wrong way

    # Setup steps
    if {[dcp_wrap {group create gumby_110_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_110_N -remove {pgo_test_any badenctype} -types} msg] $msg
    } compare_strings {1 {Schema entry 'badenctype' not found.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_110_N}] != 0} {return;}
}

proc group_modify_era_111_N {} {
    # Identify the test
    tet_infoline group_modify_era_111_N: Try to remove any attribute the wrong way

    # Setup steps
    if {[dcp_wrap {group create gumby_111_N -attr {pgo_test_any integer 101 102}}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_111_N -remove {pgo_test_any integer 102} -types} msg] $msg
    } compare_strings {1 {Improper syntax used when attempting to remove an 'any' attribute.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_111_N}] != 0} {return;}
}

proc group_modify_era_112_N {} {
    # Identify the test
    tet_infoline group_modify_era_112_N: Incorrect i18n syntax

    # Setup steps
    if {[dcp_wrap {group create gumby_112_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_112_N -add {pgo_test_i18n_data roman8 "testing"}} msg] $msg
    } compare_strings {1 {The syntax of the specified i18ndata element is invalid.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_112_N}] != 0} {return;}
}

proc group_modify_era_113_N {} {
    # Identify the test
    tet_infoline group_create_era_113_N: Incorrect i18n syntax

    # Setup steps
    if {[dcp_wrap {group create gumby_113_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_113_N -add {pgo_test_i18n_data {roman8 test me}}} msg] $msg
    } compare_strings {1 {The syntax of the specified i18ndata element is invalid.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_113_N}] != 0} {return;}
}

proc group_modify_era_114_N {} {
    # Identify the test
    tet_infoline group_create_era_114_N: An incorrect codeset specified.

    # Setup steps
    if {[dcp_wrap {group create gumby_114_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_114_N -add {pgo_test_i18n_data {101 "testing"}}} msg] $msg
    } compare_strings {1 {The specified codeset is invalid.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_114_N}] != 0} {return;}
}

proc group_modify_era_115_N {} {
    # Identify the test
    tet_infoline group_modify_era_115_N: An incorrect codeset specified.

    # Setup steps
    if {[dcp_wrap {group create gumby_115_N}] != 0} {return;}

    # Run the test
    test {
        list [catch {group modify gumby_115_N -add {pgo_test_i18n_data {gumby "testing"}}} msg] $msg
    } compare_strings {1 {The specified codeset is invalid.}}

    # Cleanup steps
    if {[dcp_wrap {group delete gumby_115_N}] != 0} {return;}
}

set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 ic25 ic26 ic27 ic28 ic29 ic30 ic31 ic32 ic33 ic34 ic35 ic36 ic37 ic38 ic39 ic40 ic41 ic42 ic43 ic44 ic45 ic46 ic47 ic48 ic49 ic50 ic51 ic52 ic53 ic54 ic55 ic56 ic57 ic58 ic59 ic60 ic61 ic62 ic63 ic64 ic65 ic66 ic67 ic68 ic69 ic70 ic71 ic72 ic73 ic74 ic75 ic76 ic77 ic78 ic79 ic80 ic81 ic82 ic83 ic84 ic85 ic86 ic87 ic88 ic89 ic90 ic91 ic92 ic93 ic94 ic95 ic96 ic97 ic98 ic99 ic100 ic101 ic102 ic103 ic104 ic105 ic106 ic107 ic108 ic109 ic110 ic111 ic112 ic113 ic114 ic115 "
set ic1 "group_modify_era_1_N"
set ic2 "group_modify_era_2_N"
set ic3 "group_modify_era_3_N"
set ic4 "group_modify_era_4_N"
set ic5 "group_modify_era_5_N"
set ic6 "group_modify_era_6_N"
set ic7 "group_modify_era_7_N"
set ic8 "group_modify_era_8_N"
set ic9 "group_modify_era_9_N"
set ic10 "group_modify_era_10_N"
set ic11 "group_modify_era_11_N"
set ic12 "group_modify_era_12_N"
set ic13 "group_modify_era_13_N"
set ic14 "group_modify_era_14_N"
set ic15 "group_modify_era_15_N"
set ic16 "group_modify_era_16_N"
set ic17 "group_modify_era_17_N"
set ic18 "group_modify_era_18_N"
set ic19 "group_modify_era_19_N"
set ic20 "group_modify_era_20_N"
set ic21 "group_modify_era_21_N"
set ic22 "group_modify_era_22_N"
set ic23 "group_modify_era_23_N"
set ic24 "group_modify_era_24_N"
set ic25 "group_modify_era_25_N"
set ic26 "group_modify_era_26_N"
set ic27 "group_modify_era_27_N"
set ic28 "group_modify_era_28_N"
set ic29 "group_modify_era_29_N"
set ic30 "group_modify_era_30_N"
set ic31 "group_modify_era_31_N"
set ic32 "group_modify_era_32_N"
set ic33 "group_modify_era_33_N"
set ic34 "group_modify_era_34_N"
set ic35 "group_modify_era_35_N"
set ic36 "group_modify_era_36_N"
set ic37 "group_modify_era_37_N"
set ic38 "group_modify_era_38_N"
set ic39 "group_modify_era_39_N"
set ic40 "group_modify_era_40_N"
set ic41 "group_modify_era_41_N"
set ic42 "group_modify_era_42_N"
set ic43 "group_modify_era_43_N"
set ic44 "group_modify_era_44_N"
set ic45 "group_modify_era_45_N"
set ic46 "group_modify_era_46_N"
set ic47 "group_modify_era_47_N"
set ic48 "group_modify_era_48_N"
set ic49 "group_modify_era_49_N"
set ic50 "group_modify_era_50_N"
set ic51 "group_modify_era_51_N"
set ic52 "group_modify_era_52_N"
set ic53 "group_modify_era_53_N"
set ic54 "group_modify_era_54_N"
set ic55 "group_modify_era_55_N"
set ic56 "group_modify_era_56_N"
set ic57 "group_modify_era_57_N"
set ic58 "group_modify_era_58_N"
set ic59 "group_modify_era_59_N"
set ic60 "group_modify_era_60_N"
set ic61 "group_modify_era_61_N"
set ic62 "group_modify_era_62_N"
set ic63 "group_modify_era_63_N"
set ic64 "group_modify_era_64_N"
set ic65 "group_modify_era_65_N"
set ic66 "group_modify_era_66_N"
set ic67 "group_modify_era_67_N"
set ic68 "group_modify_era_68_N"
set ic69 "group_modify_era_69_N"
set ic70 "group_modify_era_70_N"
set ic71 "group_modify_era_71_N"
set ic72 "group_modify_era_72_N"
set ic73 "group_modify_era_73_N"
set ic74 "group_modify_era_74_N"
set ic75 "group_modify_era_75_N"
set ic76 "group_modify_era_76_N"
set ic77 "group_modify_era_77_N"
set ic78 "group_modify_era_78_N"
set ic79 "group_modify_era_79_N"
set ic80 "group_modify_era_80_N"
set ic81 "group_modify_era_81_N"
set ic82 "group_modify_era_82_N"
set ic83 "group_modify_era_83_N"
set ic84 "group_modify_era_84_N"
set ic85 "group_modify_era_85_N"
set ic86 "group_modify_era_86_N"
set ic87 "group_modify_era_87_N"
set ic88 "group_modify_era_88_N"
set ic89 "group_modify_era_89_N"
set ic90 "group_modify_era_90_N"
set ic91 "group_modify_era_91_N"
set ic92 "group_modify_era_92_N"
set ic93 "group_modify_era_93_N"
set ic94 "group_modify_era_94_N"
set ic95 "group_modify_era_95_N"
set ic96 "group_modify_era_96_N"
set ic97 "group_modify_era_97_N"
set ic98 "group_modify_era_98_N"
set ic99 "group_modify_era_99_N"
set ic100 "group_modify_era_100_N"
set ic101 "group_modify_era_101_N"
set ic102 "group_modify_era_102_N"
set ic103 "group_modify_era_103_N"
set ic104 "group_modify_era_104_N"
set ic105 "group_modify_era_105_N"
set ic106 "group_modify_era_106_N"
set ic107 "group_modify_era_107_N"
set ic108 "group_modify_era_108_N"
set ic109 "group_modify_era_109_N"
set ic110 "group_modify_era_110_N"
set ic111 "group_modify_era_111_N"
set ic112 "group_modify_era_112_N"
set ic113 "group_modify_era_113_N"
set ic114 "group_modify_era_114_N"
set ic115 "group_modify_era_115_N"
set tet_startup group_modify_era_startup
set tet_cleanup group_modify_era_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
