#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: schema_create_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:23:37  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:49  marty]
#
# Revision 1.1.4.1  1995/12/11  15:13:16  root
# 	Submit
# 	[1995/12/11  14:31:06  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:42  root]
# 
# Revision 1.1.2.2  1994/09/30  16:22:00  truitt
# 	Add some new test cases.
# 	[1994/09/30  15:44:24  truitt]
# 
# Revision 1.1.2.1  1994/09/28  21:44:16  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:49  melman]
# 
# Revision 1.1.2.8  1994/09/23  17:01:42  truitt
# 	Updated to reflect new aclmgr names.
# 	[1994/09/23  16:58:08  truitt]
# 
# Revision 1.1.2.7  1994/09/21  17:53:15  truitt
# 	Change some messages to reflect new schema name parsing.
# 	[1994/09/21  17:38:41  truitt]
# 
# Revision 1.1.2.6  1994/09/21  15:26:04  truitt
# 	Update tests to reflect new aclmgr for generic schema.
# 	[1994/09/21  14:55:52  truitt]
# 
# Revision 1.1.2.5  1994/09/07  12:56:12  truitt
# 	Update schema tests to reflect new messages
# 	[1994/09/06  18:44:41  truitt]
# 
# Revision 1.1.2.4  1994/08/26  17:45:41  truitt
# 	OT11184: update tests to reflect new functionality.
# 	[1994/08/26  17:43:20  truitt]
# 
# Revision 1.1.2.3  1994/08/17  18:44:44  truitt
# 	OT11693 & others: update tests to reflect new functionality.
# 	[1994/08/17  18:42:39  truitt]
# 
# Revision 1.1.2.2  1994/08/10  19:22:20  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:40  annie]
# 
# Revision 1.1.2.1  1994/07/26  18:22:49  truitt
# 	Initial schema test drop
# 	[1994/07/26  17:32:10  truitt]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc schema_create_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc schema_create_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc schema_create_1_N {} {
    # Identify the test
    tet_infoline schema_create_1_N: The create command must have an attribute

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create} msg] $msg
    } compare_strings {1 {No schema name.}}

    # Cleanup steps
}

proc schema_create_2_N {} {
    # Identify the test
    tet_infoline schema_create_2_N: Too many arguments passed to create

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby pokey} msg] $msg
    } compare_strings {1 {Unrecognized argument 'pokey'.}}

    # Cleanup steps
}

proc schema_create_3_N {} {
    # Identify the test
    tet_infoline schema_create_3_N: A bad option with no value passed to create

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -blech} msg] $msg
    } compare_strings {1 {Unrecognized option '-blech'.}}

    # Cleanup steps
}

proc schema_create_4_N {} {
    # Identify the test
    tet_infoline schema_create_4_N: A bad option with value passed to create

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -blech 101} msg] $msg
    } compare_strings {1 {Unrecognized option '-blech'.}}

    # Cleanup steps
}

proc schema_create_5_N {} {
    # Identify the test
    tet_infoline schema_create_5_N: A badly formatted attribute switch passed to create

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute} msg] $msg
    } compare_strings {1 {The '-attribute' option requires a character string value.}}

    # Cleanup steps
}

proc schema_create_6_N {} {
    # Identify the test
    tet_infoline schema_create_6_N: A bad attribute with no value passed to create

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {blech}} msg] $msg
    } compare_strings {1 {The attribute name is not a recognized attribute.}}

    # Cleanup steps
}

proc schema_create_7_N {} {
    # Identify the test
    tet_infoline schema_create_7_N: A bad attribute with value passed to create

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {blech 101}} msg] $msg
    } compare_strings {1 {The attribute name is not a recognized attribute.}}

    # Cleanup steps
}

proc schema_create_8_N {} {
    # Identify the test
    tet_infoline schema_create_8_N: Pass no parameter to the -uuid switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -uuid} msg] $msg
    } compare_strings {1 {The '-uuid' option requires a character string value.}}

    # Cleanup steps
}

proc schema_create_9_N {} {
    # Identify the test
    tet_infoline schema_create_9_N: Pass no parameter to the -encoding switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -encoding} msg] $msg
    } compare_strings {1 {The '-encoding' option requires a character string value.}}

    # Cleanup steps
}

proc schema_create_10_N {} {
    # Identify the test
    tet_infoline schema_create_10_N: Pass no parameter to the -aclmgr switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -aclmgr} msg] $msg
    } compare_strings {1 {The '-aclmgr' option requires a character string value.}}

    # Cleanup steps
}

proc schema_create_11_N {} {
    # Identify the test
    tet_infoline schema_create_11_N: Pass no parameter to the -unique switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -unique} msg] $msg
    } compare_strings {1 {The '-unique' option requires a character string value.}}

    # Cleanup steps
}

proc schema_create_12_N {} {
    # Identify the test
    tet_infoline schema_create_12_N: Pass no parameter to the -multivalued switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -multivalued} msg] $msg
    } compare_strings {1 {The '-multivalued' option requires a character string value.}}

    # Cleanup steps
}

proc schema_create_13_N {} {
    # Identify the test
    tet_infoline schema_create_13_N: Pass no parameter to the -reserved switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -reserved} msg] $msg
    } compare_strings {1 {The '-reserved' option requires a character string value.}}

    # Cleanup steps
}

proc schema_create_14_N {} {
    # Identify the test
    tet_infoline schema_create_14_N: Pass no parameter to the -applydefs switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -applydefs} msg] $msg
    } compare_strings {1 {The '-applydefs' option requires a character string value.}}

    # Cleanup steps
}

proc schema_create_15_N {} {
    # Identify the test
    tet_infoline schema_create_15_N: Pass no parameter to the -intercell switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -intercell} msg] $msg
    } compare_strings {1 {The '-intercell' option requires a character string value.}}

    # Cleanup steps
}

proc schema_create_16_N {} {
    # Identify the test
    tet_infoline schema_create_16_N: Pass no parameter to the -trigtype switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -trigtype} msg] $msg
    } compare_strings {1 {The '-trigtype' option requires a character string value.}}

    # Cleanup steps
}

proc schema_create_17_N {} {
    # Identify the test
    tet_infoline schema_create_17_N: Pass no parameter to the -trigbind switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -trigbind} msg] $msg
    } compare_strings {1 {The '-trigbind' option requires a character string value.}}

    # Cleanup steps
}

proc schema_create_18_N {} {
    # Identify the test
    tet_infoline schema_create_18_N: Pass no parameter to the -scope switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -scope} msg] $msg
    } compare_strings {1 {The '-scope' option requires a character string value.}}

    # Cleanup steps
}

proc schema_create_19_N {} {
    # Identify the test
    tet_infoline schema_create_19_N: Pass no parameter to the -annotation switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -annotation} msg] $msg
    } compare_strings {1 {The '-annotation' option requires a character string value.}}

    # Cleanup steps
}

proc schema_create_20_N {} {
    # Identify the test
    tet_infoline schema_create_20_N: Pass a bad parameter to the -uuid switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -uuid blech} msg] $msg
    } compare_strings {1 {Invalid string UUID}}

    # Cleanup steps
}

proc schema_create_21_N {} {
    # Identify the test
    tet_infoline schema_create_21_N: Pass a bad parameter to the -encoding switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -encoding blech} msg] $msg
    } compare_strings {1 {Encoding type is unknown.}}

    # Cleanup steps
}

proc schema_create_22_N {} {
    # Identify the test
    tet_infoline schema_create_22_N: Pass a bad parameter to the -aclmgr switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -aclmgr blech} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_create_23_N {} {
    # Identify the test
    tet_infoline schema_create_23_N: Pass a bad parameter to the -unique switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -unique blech} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc schema_create_24_N {} {
    # Identify the test
    tet_infoline schema_create_24_N: Pass a bad parameter to the -multivalued switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -multivalued blech} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc schema_create_25_N {} {
    # Identify the test
    tet_infoline schema_create_25_N: Pass a bad parameter to the -reserved switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -reserved blech} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc schema_create_26_N {} {
    # Identify the test
    tet_infoline schema_create_26_N: Pass a bad parameter to the -applydefs switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -applydefs blech} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc schema_create_27_N {} {
    # Identify the test
    tet_infoline schema_create_27_N: Pass a bad parameter to the -intercell switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -intercell blech} msg] $msg
    } compare_strings {1 {Invalid intercell action.}}

    # Cleanup steps
}

proc schema_create_28_N {} {
    # Identify the test
    tet_infoline schema_create_28_N: Pass a bad parameter to the -trigtype switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -trigtype blech} msg] $msg
    } compare_strings {1 {Invalid trigger type.}}

    # Cleanup steps
}

proc schema_create_29_N {} {
    # Identify the test
    tet_infoline schema_create_29_N: Pass a bad parameter to the -trigbind switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -trigbind blech} msg] $msg
    } compare_strings {1 {Invalid binding syntax.}}

    # Cleanup steps
}

proc schema_create_30_N {} {
    # Identify the test
    tet_infoline schema_create_30_N: Pass no parameter to the uuid argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {uuid}} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc schema_create_31_N {} {
    # Identify the test
    tet_infoline schema_create_31_N: Pass no parameter to the encoding argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {encoding}} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc schema_create_32_N {} {
    # Identify the test
    tet_infoline schema_create_32_N: Pass no parameter to the aclmgr argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {aclmgr}} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc schema_create_33_N {} {
    # Identify the test
    tet_infoline schema_create_33_N: Pass no parameter to the unique argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {unique}} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc schema_create_34_N {} {
    # Identify the test
    tet_infoline schema_create_34_N: Pass no parameter to the multivalued argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {multivalued}} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc schema_create_35_N {} {
    # Identify the test
    tet_infoline schema_create_35_N: Pass no parameter to the reserved argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {reserved}} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc schema_create_36_N {} {
    # Identify the test
    tet_infoline schema_create_36_N: Pass no parameter to the applydefs argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {applydefs}} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc schema_create_37_N {} {
    # Identify the test
    tet_infoline schema_create_37_N: Pass no parameter to the intercell argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {intercell}} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc schema_create_38_N {} {
    # Identify the test
    tet_infoline schema_create_38_N: Pass no parameter to the trigtype argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {trigtype}} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc schema_create_39_N {} {
    # Identify the test
    tet_infoline schema_create_39_N: Pass no parameter to the trigbind argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {trigbind}} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc schema_create_40_N {} {
    # Identify the test
    tet_infoline schema_create_40_N: Pass no parameter to the scope argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {scope}} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc schema_create_41_N {} {
    # Identify the test
    tet_infoline schema_create_41_N: Pass no parameter to the annotation argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {annotation}} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc schema_create_42_N {} {
    # Identify the test
    tet_infoline schema_create_42_N: Pass a bad parameter to the uuid argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {uuid blech}} msg] $msg
    } compare_strings {1 {Invalid string UUID}}

    # Cleanup steps
}

proc schema_create_43_N {} {
    # Identify the test
    tet_infoline schema_create_43_N: Pass a bad parameter to the encoding argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {encoding blech}} msg] $msg
    } compare_strings {1 {Encoding type is unknown.}}

    # Cleanup steps
}

proc schema_create_44_N {} {
    # Identify the test
    tet_infoline schema_create_44_N: Pass a bad parameter to the aclmgr argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {aclmgr blech}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_create_45_N {} {
    # Identify the test
    tet_infoline schema_create_45_N: Pass a bad parameter to the unique argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {unique blech}} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc schema_create_46_N {} {
    # Identify the test
    tet_infoline schema_create_46_N: Pass a bad parameter to the multivalued argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {multivalued blech}} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc schema_create_47_N {} {
    # Identify the test
    tet_infoline schema_create_47_N: Pass a bad parameter to the reserved argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {reserved blech}} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc schema_create_48_N {} {
    # Identify the test
    tet_infoline schema_create_48_N: Pass a bad parameter to the applydefs argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {applydefs blech}} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc schema_create_49_N {} {
    # Identify the test
    tet_infoline schema_create_49_N: Pass a bad parameter to the intercell argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {intercell blech}} msg] $msg
    } compare_strings {1 {Invalid intercell action.}}

    # Cleanup steps
}

proc schema_create_50_N {} {
    # Identify the test
    tet_infoline schema_create_50_N: Pass a bad parameter to the trigtype argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {trigtype blech}} msg] $msg
    } compare_strings {1 {Invalid trigger type.}}

    # Cleanup steps
}

proc schema_create_51_N {} {
    # Identify the test
    tet_infoline schema_create_51_N: Pass a bad parameter to the trigbind argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {trigbind blech}} msg] $msg
    } compare_strings {1 {Invalid binding syntax.}}

    # Cleanup steps
}

proc schema_create_52_N {} {
    # Identify the test
    tet_infoline schema_create_52_N: Pass a badly formatted command line to create

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -aclmgr {encoding integer}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_create_53_N {} {
    # Identify the test
    tet_infoline schema_create_53_N: Pass both switch options and attribute arguments to create

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -aclmgr {group r r r r} -attribute {encoding integer}} msg] $msg
    } compare_strings {1 {Only options or an attribute list are allowed.}}

    # Cleanup steps
}

proc schema_create_54_N {} {
    # Identify the test
    tet_infoline schema_create_54_N: Not enough information passed to create

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby} msg] $msg
    } compare_strings {1 {Encoding type is missing.}}

    # Cleanup steps
}

proc schema_create_55_N {} {
    # Identify the test
    tet_infoline schema_create_55_N: Not enough information passed to create

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -encoding integer} msg] $msg
    } compare_strings {1 {The ACL manager set is missing.}}

    # Cleanup steps
}

proc schema_create_56_N {} {
    # Identify the test
    tet_infoline schema_create_56_N: Not enough information passed to create

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {encoding integer}} msg] $msg
    } compare_strings {1 {The ACL manager set is missing.}}

    # Cleanup steps
}

proc schema_create_57_N {} {
    # Identify the test
    tet_infoline schema_create_57_N: Not a fully qualified schema name passed to create

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -encoding integer -aclmgr {group r r r r}} msg] $msg
    } compare_strings {1 {Incomplete name}}

    # Cleanup steps
}

proc schema_create_58_N {} {
    # Identify the test
    tet_infoline schema_create_58_N: Not a fully qualified schema name passed to create

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create gumby -attribute {{encoding integer} {aclmgr {group r r r r}}}} msg] $msg
    } compare_strings {1 {Incomplete name}}

    # Cleanup steps
}

proc schema_create_59_N {} {
    # Identify the test
    tet_infoline schema_create_59_N: Missing trigger information when intercell evaluate specified

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_59_N -encoding integer -aclmgr {organization r r r r} -intercell evaluate} msg] $msg
    } compare_strings {1 {trigger binding info must be specified}}

    # Cleanup steps
}

proc schema_create_60_N {} {
    # Identify the test
    tet_infoline schema_create_60_N: Missing trigger information when intercell evaluate specified

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_60_N -attribute {{encoding integer} {aclmgr {organization r r r r}} {intercell evaluate}}} msg] $msg
    } compare_strings {1 {trigger binding info must be specified}}

    # Cleanup steps
}

proc schema_create_61_N {} {
    # Identify the test
    tet_infoline schema_create_61_N: Missing trigger information when trigtype query specified

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_61_N -encoding integer -aclmgr {organization r r r r} -trigtype query} msg] $msg
    } compare_strings {1 {trigger binding info must be specified}}

    # Cleanup steps
}

proc schema_create_62_N {} {
    # Identify the test
    tet_infoline schema_create_62_N: Missing trigger information when trigtype query specified

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_62_N -attribute {{encoding integer} {aclmgr {organization r r r r}} {trigtype query}}} msg] $msg
    } compare_strings {1 {trigger binding info must be specified}}

    # Cleanup steps
}

proc schema_create_63_N {} {
    # Identify the test
    tet_infoline schema_create_63_N: Missing trigger information when trigtype update specified.
    tet_infoline schema_create_63_N: Note: at this time security does not support trig type update. 

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_63_N -encoding integer -aclmgr {organization r r r r} -trigtype update} msg] $msg
    } compare_strings {1 {invalid trigger type specified}}

    # Cleanup steps
}

proc schema_create_64_N {} {
    # Identify the test
    tet_infoline schema_create_64_N: Missing trigger information when trigtype update specified
    tet_infoline schema_create_64_N: Note: at this time security does not support trig type update. 

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_64_N -attribute {{encoding integer} {aclmgr {organization r r r r}} {trigtype update}}} msg] $msg
    } compare_strings {1 {invalid trigger type specified}}

    # Cleanup steps
}

proc schema_create_65_N {} {
    # Identify the test
    tet_infoline schema_create_65_N: Invalid permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_65_N -encoding integer -aclmgr {group rc id tD nf}} msg] $msg
    } compare_strings {1 {invalid permission set}}

    # Cleanup steps
}

proc schema_create_66_N {} {
    # Identify the test
    tet_infoline schema_create_66_N: Invalid permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_66_N -attribute {{encoding integer} {aclmgr {group rc id tD nf}}}} msg] $msg
    } compare_strings {1 {invalid permission set}}

    # Cleanup steps
}

proc schema_create_67_N {} {
    # Identify the test
    tet_infoline schema_create_67_N: Invalid permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_67_N -encoding integer -aclmgr {group rc id tD z}} msg] $msg
    } compare_strings {1 {Invalid permission string.}}

    # Cleanup steps
}

proc schema_create_68_N {} {
    # Identify the test
    tet_infoline schema_create_68_N: Invalid permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_68_N -attribute {{encoding integer} {aclmgr {group rc id tD z}}}} msg] $msg
    } compare_strings {1 {Invalid permission string.}}

    # Cleanup steps
}

proc schema_create_69_N {} {
    # Identify the test
    tet_infoline schema_create_69_N: Invalid permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_69_N -encoding integer -aclmgr {group r r z r}} msg] $msg
    } compare_strings {1 {Invalid permission string.}}

    # Cleanup steps
}

proc schema_create_70_N {} {
    # Identify the test
    tet_infoline schema_create_70_N: Invalid permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_70_N -attribute {{encoding integer} {aclmgr {group r r z r}}}} msg] $msg
    } compare_strings {1 {Invalid permission string.}}

    # Cleanup steps
}

proc schema_create_71_N {} {
    # Identify the test
    tet_infoline schema_create_71_N: Invalid permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_71_N -encoding integer -aclmgr {group r z r r}} msg] $msg
    } compare_strings {1 {Invalid permission string.}}

    # Cleanup steps
}

proc schema_create_72_N {} {
    # Identify the test
    tet_infoline schema_create_72_N: Invalid permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_72_N -attribute {{encoding integer} {aclmgr {group r z r r}}}} msg] $msg
    } compare_strings {1 {Invalid permission string.}}

    # Cleanup steps
}

proc schema_create_73_N {} {
    # Identify the test
    tet_infoline schema_create_73_N: Invalid permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_73_N -encoding integer -aclmgr {group z r r r}} msg] $msg
    } compare_strings {1 {Invalid permission string.}}

    # Cleanup steps
}

proc schema_create_74_N {} {
    # Identify the test
    tet_infoline schema_create_74_N: Invalid permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_74_N -attribute {{encoding integer} {aclmgr {group z r r r}}}} msg] $msg
    } compare_strings {1 {Invalid permission string.}}

    # Cleanup steps
}

proc schema_create_75_N {} {
    # Identify the test
    tet_infoline schema_create_75_N: Badly formatted permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_75_N -encoding integer -aclmgr {group -r r r r}} msg] $msg
    } compare_strings {1 {Invalid permission string.}}

    # Cleanup steps
}

proc schema_create_76_N {} {
    # Identify the test
    tet_infoline schema_create_76_N: Badly formatted permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_76_N -attribute {{encoding integer} {aclmgr {group -r r r r}}}} msg] $msg
    } compare_strings {1 {Invalid permission string.}}

    # Cleanup steps
}

proc schema_create_77_N {} {
    # Identify the test
    tet_infoline schema_create_77_N: Badly formatted permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_77_N -encoding integer -aclmgr {group r r r r r}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_create_78_N {} {
    # Identify the test
    tet_infoline schema_create_78_N: Badly formatted permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_78_N -attribute {{encoding integer} {aclmgr {group r r r r r}}}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_create_79_N {} {
    # Identify the test
    tet_infoline schema_create_79_N: Badly formatted permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_79_N -encoding integer -aclmgr {{group r r r r} {organization r r r r r}}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_create_80_N {} {
    # Identify the test
    tet_infoline schema_create_80_N: Badly formatted permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_80_N -attribute {{encoding integer} {aclmgr {{group r r r r} {organization r r r r r}}}}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_create_81_N {} {
    # Identify the test
    tet_infoline schema_create_81_N: Incorrect permission set name

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_81_N -encoding integer -aclmgr {blech r r r r}} msg] $msg
    } compare_strings {1 {Invalid ACL manager name.}}

    # Cleanup steps
}

proc schema_create_82_N {} {
    # Identify the test
    tet_infoline schema_create_82_N: Incorrect permission set name

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_82_N -attribute {{encoding integer} {aclmgr {blech r r r r}}}} msg] $msg
    } compare_strings {1 {Invalid ACL manager name.}}

    # Cleanup steps
}

proc schema_create_83_N {} {
    # Identify the test
    tet_infoline schema_create_83_N: Incorrect ACL manager set name specified by uuid

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_83_N -encoding integer -aclmgr {06ab8f10-0191-11ca-a9e8-08001e039d7a r r r r}} msg] $msg
    } compare_strings {1 {invalid acl manager type}}

    # Cleanup steps
}

proc schema_create_84_N {} {
    # Identify the test
    tet_infoline schema_create_84_N: Incorrect ACL manager set name specified by uuid

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_84_N -attribute {{encoding integer} {aclmgr {06ab8f10-0191-11ca-a9e8-08001e039d7a r r r r}}}} msg] $msg
    } compare_strings {1 {invalid acl manager type}}

    # Cleanup steps
}

proc schema_create_85_N {} {
    # Identify the test
    tet_infoline schema_create_85_N: Try to create a schema name that already exists in the registry schema

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_create_85_N -encoding integer -aclmgr {group r r r r}}] != 0} {return;}

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_85_N -encoding integer -aclmgr {group r r r r}} msg] $msg
    } compare_strings {1 {attribute name already exists}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_85_N}] != 0} {return;}
}

proc schema_create_86_N {} {
    # Identify the test
    tet_infoline schema_create_86_N: Try to create a schema name that already exists in the registry schema

    # Setup steps
    if {[dcp_wrap {xattrschema create /.:/sec/xattrschema/schema_create_86_N -attribute {{encoding integer} {aclmgr {group r r r r}}}}] != 0} {return;}

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_86_N -attribute {{encoding integer} {aclmgr {group r r r r}}}} msg] $msg
    } compare_strings {1 {attribute name already exists}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_86_N}] != 0} {return;}
}

proc schema_create_87_N {} {
    global DCP_HOSTNAME_ONE DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline schema_create_87_N: Try to create a schema name that already exists in the hosts schema

    # Setup steps
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {xattrschema create "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_create_87_N" -encoding integer -aclmgr {srvrconf r r r r}}] != 0} {return;}

    # Run the test
    test {
        list [catch {xattrschema create "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_create_87_N" -encoding integer -aclmgr {srvrconf r r r r}} msg] $msg
    } compare_strings {1 {Cannot create object; already exists}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_create_87_N"}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc schema_create_88_N {} {
    global DCP_HOSTNAME_ONE DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline schema_create_88_N: Try to create a schema name that already exists in the hosts schema

    # Setup steps
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {xattrschema create "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_create_88_N" -attribute {{encoding integer} {aclmgr {srvrconf r r r r}}}}] != 0} {return;}

    # Run the test
    test {
        list [catch {xattrschema create "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_create_88_N" -attribute {{encoding integer} {aclmgr {srvrconf r r r r}}}} msg] $msg
    } compare_strings {1 {Cannot create object; already exists}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete "/.:/hosts/$DCP_HOSTNAME_ONE/config/xattrschema/schema_create_88_N"}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc schema_create_89_N {} {
    # Identify the test
    tet_infoline schema_create_89_N: The second of two schema names passed to create is bad

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create {/.:/sec/xattrschema/schema_create_89_N gumby} -encoding integer -aclmgr {group r r r r}} msg] $msg
    } compare_strings {1 {Incomplete name}}

    # Cleanup steps
    if {[dcp_wrap {xattrschema delete /.:/sec/xattrschema/schema_create_89_N}] != 0} {return;}
}

proc schema_create_90_N {} {
    # Identify the test
    tet_infoline schema_create_90_N: Ambiguous encoding type specified

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_90_N -encoding b -aclmgr {group r r r r}} msg] $msg
    } compare_strings {1 {Encoding type is ambiguous.}}

    # Cleanup steps
}

proc schema_create_91_N {} {
    # Identify the test
    tet_infoline schema_create_91_N: Ambiguous encoding type specified

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_91_N -attribute {{encoding b} {aclmgr {group r r r r}}}} msg] $msg
    } compare_strings {1 {Encoding type is ambiguous.}}

    # Cleanup steps
}

proc schema_create_92_N {} {
    # Identify the test
    tet_infoline schema_create_92_N: Ambiguous ACL manager set specified

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_92_N -encoding integer -aclmgr {p r r r r}} msg] $msg
    } compare_strings {1 {Ambiguous ACL manager name.}}

    # Cleanup steps
}

proc schema_create_93_N {} {
    # Identify the test
    tet_infoline schema_create_93_N: Ambiguous ACL manager set specified

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_93_N -attribute {{encoding integer} {aclmgr {p r r r r}}}} msg] $msg
    } compare_strings {1 {Ambiguous ACL manager name.}}

    # Cleanup steps
}

proc schema_create_94_N {} {
    # Identify the test
    tet_infoline schema_create_94_N: Bogus ACL manager set syntax

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_94_N -encoding int -aclmgr {principal {gumby r} {update r} {test r} {delete r}}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_create_95_N {} {
    # Identify the test
    tet_infoline schema_create_95_N: Bogus ACL manager set syntax

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_95_N -encoding int -aclmgr {principal {query r} {update r} {gumby r} {delete r}}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_create_96_N {} {
    # Identify the test
    tet_infoline schema_create_96_N: Bogus ACL manager set syntax

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_96_N -encoding int -aclmgr {principal {query r} {update r} {test r} {delete r r r}}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_create_97_N {} {
    # Identify the test
    tet_infoline schema_create_97_N: Bogus ACL manager set syntax

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_97_N -attr {{encoding int} {aclmgr {principal {gumby r} {update r} {test r} {delete r}}}}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_create_98_N {} {
    # Identify the test
    tet_infoline schema_create_98_N: Bogus ACL manager set syntax

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_98_N -attr {{encoding int} {aclmgr {principal {query r} {update r} {gumby r} {delete r}}}}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_create_99_N {} {
    # Identify the test
    tet_infoline schema_create_99_N: Bogus ACL manager set syntax

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema create /.:/sec/xattrschema/schema_create_99_N -attr {{encoding int} {aclmgr {principal {query r} {update r} {test r} {delete r r r}}}}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 ic25 ic26 ic27 ic28 ic29 ic30 ic31 ic32 ic33 ic34 ic35 ic36 ic37 ic38 ic39 ic40 ic41 ic42 ic43 ic44 ic45 ic46 ic47 ic48 ic49 ic50 ic51 ic52 ic53 ic54 ic55 ic56 ic57 ic58 ic59 ic60 ic61 ic62 ic63 ic64 ic65 ic66 ic67 ic68 ic69 ic70 ic71 ic72 ic73 ic74 ic75 ic76 ic77 ic78 ic79 ic80 ic81 ic82 ic83 ic84 ic85 ic86 ic87 ic88 ic89 ic90 ic91 ic92 ic93 ic94 ic95 ic96 ic97 ic98 ic99 "
set ic1 "schema_create_1_N"
set ic2 "schema_create_2_N"
set ic3 "schema_create_3_N"
set ic4 "schema_create_4_N"
set ic5 "schema_create_5_N"
set ic6 "schema_create_6_N"
set ic7 "schema_create_7_N"
set ic8 "schema_create_8_N"
set ic9 "schema_create_9_N"
set ic10 "schema_create_10_N"
set ic11 "schema_create_11_N"
set ic12 "schema_create_12_N"
set ic13 "schema_create_13_N"
set ic14 "schema_create_14_N"
set ic15 "schema_create_15_N"
set ic16 "schema_create_16_N"
set ic17 "schema_create_17_N"
set ic18 "schema_create_18_N"
set ic19 "schema_create_19_N"
set ic20 "schema_create_20_N"
set ic21 "schema_create_21_N"
set ic22 "schema_create_22_N"
set ic23 "schema_create_23_N"
set ic24 "schema_create_24_N"
set ic25 "schema_create_25_N"
set ic26 "schema_create_26_N"
set ic27 "schema_create_27_N"
set ic28 "schema_create_28_N"
set ic29 "schema_create_29_N"
set ic30 "schema_create_30_N"
set ic31 "schema_create_31_N"
set ic32 "schema_create_32_N"
set ic33 "schema_create_33_N"
set ic34 "schema_create_34_N"
set ic35 "schema_create_35_N"
set ic36 "schema_create_36_N"
set ic37 "schema_create_37_N"
set ic38 "schema_create_38_N"
set ic39 "schema_create_39_N"
set ic40 "schema_create_40_N"
set ic41 "schema_create_41_N"
set ic42 "schema_create_42_N"
set ic43 "schema_create_43_N"
set ic44 "schema_create_44_N"
set ic45 "schema_create_45_N"
set ic46 "schema_create_46_N"
set ic47 "schema_create_47_N"
set ic48 "schema_create_48_N"
set ic49 "schema_create_49_N"
set ic50 "schema_create_50_N"
set ic51 "schema_create_51_N"
set ic52 "schema_create_52_N"
set ic53 "schema_create_53_N"
set ic54 "schema_create_54_N"
set ic55 "schema_create_55_N"
set ic56 "schema_create_56_N"
set ic57 "schema_create_57_N"
set ic58 "schema_create_58_N"
set ic59 "schema_create_59_N"
set ic60 "schema_create_60_N"
set ic61 "schema_create_61_N"
set ic62 "schema_create_62_N"
set ic63 "schema_create_63_N"
set ic64 "schema_create_64_N"
set ic65 "schema_create_65_N"
set ic66 "schema_create_66_N"
set ic67 "schema_create_67_N"
set ic68 "schema_create_68_N"
set ic69 "schema_create_69_N"
set ic70 "schema_create_70_N"
set ic71 "schema_create_71_N"
set ic72 "schema_create_72_N"
set ic73 "schema_create_73_N"
set ic74 "schema_create_74_N"
set ic75 "schema_create_75_N"
set ic76 "schema_create_76_N"
set ic77 "schema_create_77_N"
set ic78 "schema_create_78_N"
set ic79 "schema_create_79_N"
set ic80 "schema_create_80_N"
set ic81 "schema_create_81_N"
set ic82 "schema_create_82_N"
set ic83 "schema_create_83_N"
set ic84 "schema_create_84_N"
set ic85 "schema_create_85_N"
set ic86 "schema_create_86_N"
set ic87 "schema_create_87_N"
set ic88 "schema_create_88_N"
set ic89 "schema_create_89_N"
set ic90 "schema_create_90_N"
set ic91 "schema_create_91_N"
set ic92 "schema_create_92_N"
set ic93 "schema_create_93_N"
set ic94 "schema_create_94_N"
set ic95 "schema_create_95_N"
set ic96 "schema_create_96_N"
set ic97 "schema_create_97_N"
set ic98 "schema_create_98_N"
set ic99 "schema_create_99_N"
set tet_startup schema_create_startup
set tet_cleanup schema_create_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
