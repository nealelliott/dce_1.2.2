#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: schema_modify_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:23:52  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:55  marty]
#
# Revision 1.1.4.1  1995/12/11  15:13:23  root
# 	Submit
# 	[1995/12/11  14:31:11  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:48  root]
# 
# Revision 1.1.2.2  1994/09/30  16:22:03  truitt
# 	Add some new test cases.
# 	[1994/09/30  15:45:38  truitt]
# 
# Revision 1.1.2.1  1994/09/28  21:44:25  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:55  melman]
# 
# 	Initial creation.
# 	[1994/07/26  19:35:14  nav]
# 
# Revision 1.1.2.6  1994/09/23  17:01:46  truitt
# 	Updated to reflect new aclmgr names.
# 	[1994/09/23  16:58:37  truitt]
# 
# Revision 1.1.2.5  1994/09/21  17:53:19  truitt
# 	Change some messages to reflect new schema name parsing.
# 	[1994/09/21  17:39:13  truitt]
# 
# Revision 1.1.2.4  1994/09/07  12:56:13  truitt
# 	Update schema tests to reflect new messages
# 	[1994/09/06  18:44:48  truitt]
# 
# Revision 1.1.2.3  1994/08/17  18:44:55  truitt
# 	Fix comment leader.
# 	[1994/08/17  18:30:45  truitt]
# 
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:45  annie]
# 
# Revision 1.1.2.2  1994/08/10  19:22:26  annie
# 	expand copyright with OSF copyright text
# 
# Revision 1.1.2.1  1994/07/27  18:52:10  nav
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


proc schema_modify_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc schema_modify_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc schema_modify_1_N {} {
    # Identify the test
    tet_infoline schema_modify_1_N: The modify command must have an attribute

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify} msg] $msg
    } compare_strings {1 {No schema name.}}

    # Cleanup steps
}

proc schema_modify_2_N {} {
    # Identify the test
    tet_infoline schema_modify_2_N: Too many arguments passed to modify

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby pokey} msg] $msg
    } compare_strings {1 {Unrecognized argument 'pokey'.}}

    # Cleanup steps
}

proc schema_modify_3_N {} {
    # Identify the test
    tet_infoline schema_modify_3_N: A bad option with no value passed to modify

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -blech} msg] $msg
    } compare_strings {1 {Unrecognized option '-blech'.}}

    # Cleanup steps
}

proc schema_modify_4_N {} {
    # Identify the test
    tet_infoline schema_modify_4_N: A bad option with value passed to modify

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -blech 101} msg] $msg
    } compare_strings {1 {Unrecognized option '-blech'.}}

    # Cleanup steps
}

proc schema_modify_5_N {} {
    # Identify the test
    tet_infoline schema_modify_5_N: A badly formatted change switch passed to modify

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -change} msg] $msg
    } compare_strings {1 {The '-change' option requires a character string value.}}

    # Cleanup steps
}

proc schema_modify_6_N {} {
    # Identify the test
    tet_infoline schema_modify_6_N: A bad attribute with no value passed to modify

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -change {blech}} msg] $msg
    } compare_strings {1 {The attribute name is not a recognized attribute.}}

    # Cleanup steps
}

proc schema_modify_7_N {} {
    # Identify the test
    tet_infoline schema_modify_7_N: A bad attribute with value passed to modify

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -change {blech 101}} msg] $msg
    } compare_strings {1 {The attribute name is not a recognized attribute.}}

    # Cleanup steps
}

proc schema_modify_8_N {} {
    # Identify the test
    tet_infoline schema_modify_8_N: Pass no parameter to the -aclmgr switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -aclmgr} msg] $msg
    } compare_strings {1 {The '-aclmgr' option requires a character string value.}}

    # Cleanup steps
}

proc schema_modify_9_N {} {
    # Identify the test
    tet_infoline schema_modify_9_N: Pass no parameter to the -unique switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -unique} msg] $msg
    } compare_strings {1 {Unrecognized option '-unique'.}}

    # Cleanup steps
}

proc schema_modify_10_N {} {
    # Identify the test
    tet_infoline schema_modify_10_N: Pass no parameter to the -reserved switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -reserved} msg] $msg
    } compare_strings {1 {The '-reserved' option requires a character string value.}}

    # Cleanup steps
}

proc schema_modify_11_N {} {
    # Identify the test
    tet_infoline schema_modify_11_N: Pass no parameter to the -applydefs switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -applydefs} msg] $msg
    } compare_strings {1 {The '-applydefs' option requires a character string value.}}

    # Cleanup steps
}

proc schema_modify_12_N {} {
    # Identify the test
    tet_infoline schema_modify_12_N: Pass no parameter to the -intercell switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -intercell} msg] $msg
    } compare_strings {1 {The '-intercell' option requires a character string value.}}

    # Cleanup steps
}

proc schema_modify_13_N {} {
    # Identify the test
    tet_infoline schema_modify_13_N: Pass no parameter to the -trigtype switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -trigtype} msg] $msg
    } compare_strings {1 {Unrecognized option '-trigtype'.}}

    # Cleanup steps
}

proc schema_modify_14_N {} {
    # Identify the test
    tet_infoline schema_modify_14_N: Pass no parameter to the -trigbind switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -trigbind} msg] $msg
    } compare_strings {1 {The '-trigbind' option requires a character string value.}}

    # Cleanup steps
}

proc schema_modify_15_N {} {
    # Identify the test
    tet_infoline schema_modify_15_N: Pass no parameter to the -scope switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -scope} msg] $msg
    } compare_strings {1 {Unrecognized option '-scope'.}}

    # Cleanup steps
}

proc schema_modify_16_N {} {
    # Identify the test
    tet_infoline schema_modify_16_N: Pass no parameter to the -annotation switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -annotation} msg] $msg
    } compare_strings {1 {The '-annotation' option requires a character string value.}}

    # Cleanup steps
}

proc schema_modify_17_N {} {
    # Identify the test
    tet_infoline schema_modify_17_N: Pass a bad parameter to the -aclmgr switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -aclmgr blech} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_modify_18_N {} {
    # Identify the test
    tet_infoline schema_modify_18_N: Pass a bad parameter to the -reserved switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -reserved blech} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc schema_modify_19_N {} {
    # Identify the test
    tet_infoline schema_modify_19_N: Pass a bad parameter to the -applydefs switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -applydefs blech} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc schema_modify_20_N {} {
    # Identify the test
    tet_infoline schema_modify_20_N: Pass a bad parameter to the -intercell switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -intercell blech} msg] $msg
    } compare_strings {1 {Invalid intercell action.}}

    # Cleanup steps
}

proc schema_modify_21_N {} {
    # Identify the test
    tet_infoline schema_modify_21_N: Pass a bad parameter to the -trigbind switch

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -trigbind blech} msg] $msg
    } compare_strings {1 {Invalid binding syntax.}}

    # Cleanup steps
}

proc schema_modify_22_N {} {
    # Identify the test
    tet_infoline schema_modify_22_N: Pass no parameter to the reserved argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -change {reserved}} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc schema_modify_23_N {} {
    # Identify the test
    tet_infoline schema_modify_23_N: Pass no parameter to the applydefs argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -change {applydefs}} msg] $msg
    } compare_strings {1 {The attribute list syntax is invalid.}}

    # Cleanup steps
}

proc schema_modify_24_N {} {
    # Identify the test
    tet_infoline schema_modify_24_N: Pass a bad parameter to the aclmgr argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -change {aclmgr blech}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_modify_25_N {} {
    # Identify the test
    tet_infoline schema_modify_25_N: Pass a bad parameter to the reserved argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -change {reserved blech}} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc schema_modify_26_N {} {
    # Identify the test
    tet_infoline schema_modify_26_N: Pass a bad parameter to the applydefs argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -change {applydefs blech}} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc schema_modify_27_N {} {
    # Identify the test
    tet_infoline schema_modify_27_N: Pass a bad parameter to the intercell argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -change {intercell blech}} msg] $msg
    } compare_strings {1 {Invalid intercell action.}}

    # Cleanup steps
}

proc schema_modify_28_N {} {
    # Identify the test
    tet_infoline schema_modify_28_N: Pass a bad parameter to the trigbind argument

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -change {trigbind blech}} msg] $msg
    } compare_strings {1 {Invalid binding syntax.}}

    # Cleanup steps
}

proc schema_modify_29_N {} {
    # Identify the test
    tet_infoline schema_modify_29_N: Pass a badly formatted command line to modify

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -aclmgr {encoding integer}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_modify_30_N {} {
    # Identify the test
    tet_infoline schema_modify_30_N: Pass both switch options and attribute arguments to modify

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -aclmgr {group r r r r} -change {encoding integer}} msg] $msg
    } compare_strings {1 {Only options or an attribute list are allowed.}}

    # Cleanup steps
}

proc schema_modify_31_N {} {
    # Identify the test
    tet_infoline schema_modify_31_N: Not a fully qualified schema name passed to modify

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -aclmgr {group r r r r}} msg] $msg
    } compare_strings {1 {Incomplete name}}

    # Cleanup steps
}

proc schema_modify_32_N {} {
    # Identify the test
    tet_infoline schema_modify_32_N: Not a fully qualified schema name passed to modify

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify gumby -change {aclmgr {group r r r r}}} msg] $msg
    } compare_strings {1 {Incomplete name}}

    # Cleanup steps
}

proc schema_modify_33_N {} {
    # Identify the test
    tet_infoline schema_modify_33_N: Invalid permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/sec/xattrschema/schema_modify_33_N  -aclmgr {group rc id tD z}} msg] $msg
    } compare_strings {1 {Invalid permission string.}}

    # Cleanup steps
}

proc schema_modify_34_N {} {
    # Identify the test
    tet_infoline schema_modify_34_N: Invalid permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/sec/xattrschema/schema_modify_34_N -change {aclmgr {group rc id tD z}}} msg] $msg
    } compare_strings {1 {Invalid permission string.}}

    # Cleanup steps
}

proc schema_modify_35_N {} {
    # Identify the test
    tet_infoline schema_modify_35_N: Invalid permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/sec/xattrschema/schema_modify_35_N -aclmgr {group r r z r}} msg] $msg
    } compare_strings {1 {Invalid permission string.}}

    # Cleanup steps
}

proc schema_modify_36_N {} {
    # Identify the test
    tet_infoline schema_modify_36_N: Invalid permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/sec/xattrschema/schema_modify_36_N -change {aclmgr {group r r z r}}} msg] $msg
    } compare_strings {1 {Invalid permission string.}}

    # Cleanup steps
}

proc schema_modify_37_N {} {
    # Identify the test
    tet_infoline schema_modify_37_N: Invalid permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/sec/xattrschema/schema_modify_37_N  -aclmgr {group r z r r}} msg] $msg
    } compare_strings {1 {Invalid permission string.}}

    # Cleanup steps
}

proc schema_modify_38_N {} {
    # Identify the test
    tet_infoline schema_modify_38_N: Invalid permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/sec/xattrschema/schema_modify_38_N -change {aclmgr {group r z r r}}} msg] $msg
    } compare_strings {1 {Invalid permission string.}}

    # Cleanup steps
}

proc schema_modify_39_N {} {
    # Identify the test
    tet_infoline schema_modify_39_N: Invalid permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/sec/xattrschema/schema_modify_39_N -aclmgr {group z r r r}} msg] $msg
    } compare_strings {1 {Invalid permission string.}}

    # Cleanup steps
}

proc schema_modify_40_N {} {
    # Identify the test
    tet_infoline schema_modify_40_N: Invalid permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/sec/xattrschema/schema_modify_40_N -change {aclmgr {group z r r r}}} msg] $msg
    } compare_strings {1 {Invalid permission string.}}

    # Cleanup steps
}

proc schema_modify_41_N {} {
    # Identify the test
    tet_infoline schema_modify_41_N: badly formatted permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/sec/xattrschema/schema_modify_41_N -aclmgr {group -r r r r}} msg] $msg
    } compare_strings {1 {Invalid permission string.}}

    # Cleanup steps
}

proc schema_modify_42_N {} {
    # Identify the test
    tet_infoline schema_modify_42_N: badly formatted permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/sec/xattrschema/schema_modify_42_N -change {{aclmgr {group -r r r r}}}} msg] $msg
    } compare_strings {1 {Invalid permission string.}}

    # Cleanup steps
}

proc schema_modify_43_N {} {
    # Identify the test
    tet_infoline schema_modify_43_N: badly formatted permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/sec/xattrschema/schema_modify_43_N -aclmgr {group r r r r r}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_modify_44_N {} {
    # Identify the test
    tet_infoline schema_modify_44_N: badly formatted permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/sec/xattrschema/schema_modify_44_N -change {{aclmgr {group r r r r r}}}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_modify_45_N {} {
    # Identify the test
    tet_infoline schema_modify_45_N: badly formatted permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/sec/xattrschema/schema_modify_45_N -aclmgr {{group r r r r} {organization r r r r r}}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_modify_46_N {} {
    # Identify the test
    tet_infoline schema_modify_46_N: badly formatted permission set information

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/sec/xattrschema/schema_modify_46_N -change {{aclmgr {{group r r r r} {organization r r r r r}}}}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_modify_47_N {} {
    # Identify the test
    tet_infoline schema_modify_47_N: Incorrect permission set name

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/sec/xattrschema/schema_modify_47_N -aclmgr {blech r r r r}} msg] $msg
    } compare_strings {1 {Invalid ACL manager name.}}

    # Cleanup steps
}

proc schema_modify_48_N {} {
    # Identify the test
    tet_infoline schema_modify_48_N: Incorrect permission set name

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/sec/xattrschema/schema_modify_48_N -change {{aclmgr {blech r r r r}}}} msg] $msg
    } compare_strings {1 {Invalid ACL manager name.}}

    # Cleanup steps
}

proc schema_modify_49_N {} {
    # Identify the test
    tet_infoline schema_modify_49_N: Bogus ACL manager set syntax

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/sec/xattrschema/schema_modify_49_N -aclmgr {principal {gumby r} {update r} {test r} {delete r}}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_modify_50_N {} {
    # Identify the test
    tet_infoline schema_modify_50_N: Bogus ACL manager set syntax

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/sec/xattrschema/schema_modify_50_N -aclmgr {principal {query r} {update r} {gumby r} {delete r}}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_modify_51_N {} {
    # Identify the test
    tet_infoline schema_modify_51_N: Bogus ACL manager set syntax

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/sec/xattrschema/schema_modify_51_N -aclmgr {principal {query r} {update r} {test r} {delete r r r}}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_modify_52_N {} {
    # Identify the test
    tet_infoline schema_modify_52_N: Bogus ACL manager set syntax

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/sec/xattrschema/schema_modify_52_N -change {aclmgr {principal {gumby r} {update r} {test r} {delete r}}}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_modify_53_N {} {
    # Identify the test
    tet_infoline schema_modify_53_N: Bogus ACL manager set syntax

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/sec/xattrschema/schema_modify_53_N -change {aclmgr {principal {query r} {update r} {gumby r} {delete r}}}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

proc schema_modify_54_N {} {
    # Identify the test
    tet_infoline schema_modify_54_N: Bogus ACL manager set syntax

    # Setup steps

    # Run the test
    test {
        list [catch {xattrschema modify /.:/sec/xattrschema/schema_modify_54_N -change {aclmgr {principal {query r} {update r} {test r} {delete r r r}}}} msg] $msg
    } compare_strings {1 {Invalid ACL manager syntax.}}

    # Cleanup steps
}

set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 ic25 ic26 ic27 ic28 ic29 ic30 ic31 ic32 ic33 ic34 ic35 ic36 ic37 ic38 ic39 ic40 ic41 ic42 ic43 ic44 ic45 ic46 ic47 ic48 ic49 ic50 ic51 ic52 ic53 ic54 "
set ic1 "schema_modify_1_N"
set ic2 "schema_modify_2_N"
set ic3 "schema_modify_3_N"
set ic4 "schema_modify_4_N"
set ic5 "schema_modify_5_N"
set ic6 "schema_modify_6_N"
set ic7 "schema_modify_7_N"
set ic8 "schema_modify_8_N"
set ic9 "schema_modify_9_N"
set ic10 "schema_modify_10_N"
set ic11 "schema_modify_11_N"
set ic12 "schema_modify_12_N"
set ic13 "schema_modify_13_N"
set ic14 "schema_modify_14_N"
set ic15 "schema_modify_15_N"
set ic16 "schema_modify_16_N"
set ic17 "schema_modify_17_N"
set ic18 "schema_modify_18_N"
set ic19 "schema_modify_19_N"
set ic20 "schema_modify_20_N"
set ic21 "schema_modify_21_N"
set ic22 "schema_modify_22_N"
set ic23 "schema_modify_23_N"
set ic24 "schema_modify_24_N"
set ic25 "schema_modify_25_N"
set ic26 "schema_modify_26_N"
set ic27 "schema_modify_27_N"
set ic28 "schema_modify_28_N"
set ic29 "schema_modify_29_N"
set ic30 "schema_modify_30_N"
set ic31 "schema_modify_31_N"
set ic32 "schema_modify_32_N"
set ic33 "schema_modify_33_N"
set ic34 "schema_modify_34_N"
set ic35 "schema_modify_35_N"
set ic36 "schema_modify_36_N"
set ic37 "schema_modify_37_N"
set ic38 "schema_modify_38_N"
set ic39 "schema_modify_39_N"
set ic40 "schema_modify_40_N"
set ic41 "schema_modify_41_N"
set ic42 "schema_modify_42_N"
set ic43 "schema_modify_43_N"
set ic44 "schema_modify_44_N"
set ic45 "schema_modify_45_N"
set ic46 "schema_modify_46_N"
set ic47 "schema_modify_47_N"
set ic48 "schema_modify_48_N"
set ic49 "schema_modify_49_N"
set ic50 "schema_modify_50_N"
set ic51 "schema_modify_51_N"
set ic52 "schema_modify_52_N"
set ic53 "schema_modify_53_N"
set ic54 "schema_modify_54_N"
set tet_startup schema_modify_startup
set tet_cleanup schema_modify_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
