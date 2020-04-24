#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: acl_modify_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:09:10  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:27:48  marty]
#
# Revision 1.1.4.2  1995/12/13  15:23:56  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/5  1995/07/21  20:19 UTC  truitt
# 	Merge.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts42/1  1995/07/21  20:18 UTC  truitt
# 	Fix expected output on some of the tests.
# 
# 	HP revision /main/HPDCE02/4  1995/07/18  19:52 UTC  jrr
# 	# Update tests to handle new output.
# 	# [1995/07/18  19:51 UTC  jrr  /main/HPDCE02/jrr_mothra_pure/1]
# 
# 	HP revision /main/HPDCE02/3  1995/05/03  13:00 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts27/1  1995/05/03  13:00 UTC  truitt
# 	CHFts15023: Update tests to match new functionality.
# 
# 	HP revision /main/HPDCE02/2  1994/12/12  19:35 UTC  truitt
# 	Fix incorrect error message.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_msg_chk/1  1994/12/12  19:35 UTC  truitt
# 	Fix incorrect error message.
# 
# 	HP revision /main/HPDCE02/1  1994/11/15  13:10 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/15  13:09 UTC  truitt
# 	Fix parameters being used for the acl tests.
# 	[1995/12/11  15:50:48  root]
# 
# Revision 1.1.2.2  1994/10/01  01:32:58  rousseau
# 	Dynamically generate string bindings.
# 	[1994/10/01  01:28:37  rousseau]
# 
# Revision 1.1.2.1  1994/09/28  21:37:10  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:07  melman]
# 
# Revision 1.1.2.12  1994/09/26  21:04:12  salamone
# 	CR9938 - Change ACL mgr names and return UNINITIATED for those tests that aren't configured.
# 	[1994/09/26  21:02:15  salamone]
# 
# Revision 1.1.2.11  1994/09/08  14:00:51  salamone
# 	CR11968-Setting authentication on binding handles.
# 	[1994/09/08  13:59:24  salamone]
# 
# Revision 1.1.2.10  1994/08/25  15:33:04  salamone
# 	CR11635 - Handle 1 string binding specification for an ACL object.
# 	[1994/08/25  15:29:21  salamone]
# 
# Revision 1.1.2.9  1994/08/09  21:45:50  salamone
# 	Updated msgs from "more arguments" to "a character string value".
# 	[1994/08/09  21:09:30  salamone]
# 
# Revision 1.1.2.8  1994/07/26  18:58:42  salamone
# 	CR10045 - Provide string binding support for ACL objects.
# 	[1994/07/26  18:48:49  salamone]
# 
# Revision 1.1.2.7  1994/07/18  20:04:55  salamone
# 	Add tests for polymorphic object support
# 	[1994/07/18  20:00:12  salamone]
# 
# Revision 1.1.2.6  1994/06/10  20:01:39  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:11  devsrc]
# 
# Revision 1.1.2.5  1994/05/18  20:39:10  salamone
# 	CR10621 - dcecp "acl show", "acl check", and "acl permissions" should
# 	          display the appropriate information if unauthenticated.
# 	[1994/05/13  22:31:50  salamone]
# 
# Revision 1.1.2.4  1994/05/11  19:34:07  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:15:01  rousseau]
# 
# Revision 1.1.2.3  1994/05/09  14:49:18  salamone
# 	Fix "Ambiguous argument -" error message.
# 	[1994/05/09  12:39:06  salamone]
# 
# Revision 1.1.2.2  1994/05/06  15:00:58  melman
# 	updating to new API path
# 	[1994/05/06  15:00:23  melman]
# 
# Revision 1.1.2.1  1994/03/24  16:42:53  salamone
# 	Initial version.
# 	[1994/03/24  16:41:49  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
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
proc acl_modify_001_N {} {
    # Identify the test
    tet_infoline acl_modify_001_N: Check for an insufficient number of arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {acl modify} msg] $msg
    } compare_strings {1 {ACL object not specified.}}

    # Cleanup steps
}

proc acl_modify_002_N {} {
    # Identify the test
    tet_infoline acl_modify_002_N: Check that a modification switch must be specified.

    # Setup steps

    # Run the test
    test {
        list [catch {acl modify /.:/hosts} msg] $msg
    } compare_strings {1 {Requires more options.}}

    # Cleanup steps
}

proc acl_modify_003_N {} {
    # Identify the test
    tet_infoline acl_modify_003_N: Check for a bogus ACL object.

    # Setup steps

    # Run the test
    test {
        list [catch {acl modify foo -purge} msg] $msg
    } compare_strings {1 {acl object not found}}

    # Cleanup steps
}

proc acl_modify_004_N {} {
    # Identify the test
    tet_infoline acl_modify_004_N: Check for a bogus ACL object in a list. The first object 
    tet_infoline acl_modify_004_N: in the list will be modified, the second object will cause
    tet_infoline acl_modify_004_N: an error, and the third object will not be operated on.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify {/.:/hosts /.:/foo /.:/subsys} -a user:$name:r} msg] $msg
    } compare_strings {1 {acl object not found}}

    # Cleanup steps
    if {[dcp_wrap {acl modify /.:/hosts -r user:$name}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_005_N {} {
    # Identify the test
    tet_infoline acl_modify_005_N: Check for a bogus ACL entry in a list. None of the ACL
    tet_infoline acl_modify_005_N: objects will be modified.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append object0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append object1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {directory create $object0}] != 0} {return;}
    if {[dcp_wrap {directory create $object1}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify "$object0 $object1" -a "other_obj:r bozo:$name:r"} msg] $msg
    } compare_strings {1 {Invalid ACL entry type.}}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
    if {[dcp_wrap {directory delete $object0}] != 0} {return;}
    if {[dcp_wrap {directory delete $object1}] != 0} {return;}
}

proc acl_modify_006_N {} {
    # Identify the test
    tet_infoline acl_modify_006_N: Check for a bogus list of ACL objects.

    # Setup steps

    # Run the test
    test {
        list [catch {acl modify /.:/hosts /.:/subsys -purge} msg] $msg
    } compare_strings {1 {Unrecognized argument '/.:/subsys'.}}

    # Cleanup steps
}

proc acl_modify_007_N {} {
    # Identify the test
    tet_infoline acl_modify_007_N: Check that the -entry, -ic, and -io switches are 
    tet_infoline acl_modify_007_N: mutually exclusive.

    # Setup steps

    # Run the test
    test {
        list [catch {acl modify /.:/hosts -io -ic -entry -purge} msg] $msg
    } compare_strings {1 {Too many ACL types.}}

    # Cleanup steps
}

proc acl_modify_008_N {} {
    # Identify the test
    tet_infoline acl_modify_008_N: Check that the -ic, and -io switches are 
    tet_infoline acl_modify_008_N: mutually exclusive.

    # Setup steps

    # Run the test
    test {
        list [catch {acl modify /.:/hosts -io -ic -purge} msg] $msg
    } compare_strings {1 {Too many ACL types.}}

    # Cleanup steps
}

proc acl_modify_009_N {} {
    # Identify the test
    tet_infoline acl_modify_009_N: Check for an invalid option with no name.

    # Setup steps

    # Run the test
    test {
        list [catch {acl modify /.:/hosts -} msg] $msg
    } compare_strings {1 {Ambiguous argument '-': -add -cell -change -entry -ic -io -mask -purge -remove -type}}

    # Cleanup steps
}

proc acl_modify_010_N {} {
    # Identify the test
    tet_infoline acl_modify_010_N: Check for an invalid option.

    # Setup steps

    # Run the test
    test {
        list [catch {acl modify /.:/hosts -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc acl_modify_011_N {} {
    # Identify the test
    tet_infoline acl_modify_011_N: Check that the switch -entry does not accept any
    tet_infoline acl_modify_011_N: values.

    # Setup steps

    # Run the test
    test {
        list [catch {acl modify /.:/hosts -entry foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc acl_modify_012_N {} {
    # Identify the test
    tet_infoline acl_modify_012_N: Check that the switch -ic does not accept any
    tet_infoline acl_modify_012_N: values.

    # Setup steps

    # Run the test
    test {
        list [catch {acl modify /.:/hosts -ic foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc acl_modify_013_N {} {
    # Identify the test
    tet_infoline acl_modify_013_N: Check that the switch -io does not accept any
    tet_infoline acl_modify_013_N: values.

    # Setup steps

    # Run the test
    test {
        list [catch {acl modify /.:/hosts -io foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc acl_modify_014_N {} {
    # Identify the test
    tet_infoline acl_modify_014_N: Check that a value is specified for the -cell option.

    # Setup steps

    # Run the test
    test {
        list [catch {acl modify /.:/hosts -cell} msg] $msg
    } compare_strings {1 {The '-cell' option requires a character string value.}}

    # Cleanup steps
}

proc acl_modify_015_N {} {
    # Identify the test
    tet_infoline acl_modify_015_N: Check for a bogus default cell name.

    # Setup steps

    # Run the test
    test {
        list [catch {acl modify /.:/hosts -cell foo} msg] $msg
    } compare_strings {1 {Invalid default cell name.}}

    # Cleanup steps
}

proc acl_modify_016_N {} {
    # Identify the test
    tet_infoline acl_modify_016_N: Check for a default cell name not found in the Registry.

    # Setup steps

    # Run the test
    test {
        list [catch {acl modify /.:/hosts -cell /.../foo} msg] $msg
    } compare_strings {1 {Registry object not found}}

    # Cleanup steps
}

proc acl_modify_017_N {} {
    # Identify the test
    tet_infoline acl_modify_017_N: Check that the -cell option accepts only one value.

    # Setup steps
    if {[dcp_wrap {global DCP_CELLNAME_ONE DCP_CELLNAME_TWO}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify /.:/hosts -cell "$DCP_CELLNAME_ONE $DCP_CELLNAME_TWO"} msg] $msg
    } compare_strings {1 {Too many cell attributes.}}

    # Cleanup steps
}

proc acl_modify_018_N {} {
    # Identify the test
    tet_infoline acl_modify_018_N: Check that the -entry switch used with a string binding
    tet_infoline acl_modify_018_N: produces an error.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0}] != 0} {return;}
    if {[dcp_wrap {principal create $prin1}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify "$OBJECT_UUID@$PROTSEQ:$NETADDR principal/$prin0" -entry -add user:$prin1:r} msg] $msg
    } compare_strings {1 {Entry option used with a string binding.}}

    # Cleanup steps
    if {[dcp_wrap {principal delete $prin0}] != 0} {return;}
    if {[dcp_wrap {principal delete $prin1}] != 0} {return;}
}

proc acl_modify_019_N {} {
    # Identify the test
    tet_infoline acl_modify_019_N: Check that the residual name must be specified in a
    tet_infoline acl_modify_019_N: string binding ACL object specification.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify $OBJECT_UUID@$PROTSEQ:$NETADDR -add user:$name:r} msg] $msg
    } compare_strings {1 {acl object not found}}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_020_N {} {
    # Identify the test
    tet_infoline acl_modify_020_N: Check that a value is specified for the -add option.

    # Setup steps

    # Run the test
    test {
        list [catch {acl modify /.:/hosts -add } msg] $msg
    } compare_strings {1 {The '-add' option requires a character string value.}}

    # Cleanup steps
}

proc acl_modify_021_N {} {
    # Identify the test
    tet_infoline acl_modify_021_N: Check that a value is specified for the -change option.

    # Setup steps

    # Run the test
    test {
        list [catch {acl modify /.:/hosts -change } msg] $msg
    } compare_strings {1 {The '-change' option requires a character string value.}}

    # Cleanup steps
}

proc acl_modify_022_N {} {
    # Identify the test
    tet_infoline acl_modify_022_N: Check that a value is specified for the -remove option.

    # Setup steps

    # Run the test
    test {
        list [catch {acl modify /.:/hosts -remove } msg] $msg
    } compare_strings {1 {The '-remove' option requires a character string value.}}

    # Cleanup steps
}

proc acl_modify_023_N {} {
    # Identify the test
    tet_infoline acl_modify_023_N: Check that a value is specified for the -mask option.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_MASK1}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify $DCP_ACL_OBJ_MASK1 -mask } msg] $msg
    } compare_strings {1 {The '-mask' option requires a character string value.}}

    # Cleanup steps
}

proc acl_modify_024_N {} {
    # Identify the test
    tet_infoline acl_modify_024_N: Check that the -mask option must be specified with a -add
    tet_infoline acl_modify_024_N: and/or -change option.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_MASK1}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify $DCP_ACL_OBJ_MASK1 -mask calc} msg] $msg
    } compare_strings {1 {Requires more options.}}

    # Cleanup steps
}

proc acl_modify_025_N {} {
    # Identify the test
    tet_infoline acl_modify_025_N: Remove a "mask_obj" ACL entry from the specified ACL object 
    tet_infoline acl_modify_025_N: and re-calculate the mask.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_MASK1}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify $DCP_ACL_OBJ_MASK1 -mask calc -r mask_obj:rcw} msg] $msg
    } compare_strings {1 {Mask option used without '-add' or '-change' option.}}

    # Cleanup steps
}

proc acl_modify_026_N {} {
    # Identify the test
    tet_infoline acl_modify_026_N: Add a "mask_obj" ACL entry to the specified ACL object 
    tet_infoline acl_modify_026_N: that supports the mask_obj entry and re-calculate the mask.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_MASK1}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify $DCP_ACL_OBJ_MASK1 -mask calc -a mask_obj:rcw} msg] $msg
    } compare_strings {1 {A 'mask_obj' entry and '-mask' option were both intentionally specified.}}

    # Cleanup steps
}

proc acl_modify_027_N {} {
    # Identify the test
    tet_infoline acl_modify_027_N: Change a "mask_obj" ACL entry to the specified ACL object 
    tet_infoline acl_modify_027_N: that supports the mask_obj entry and re-calculate the mask.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_MASK1}] != 0} {return;}
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -a mask_obj:rcw}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify $DCP_ACL_OBJ_MASK1 -mask calc -ch mask_obj:c} msg] $msg
    } compare_strings {1 {A 'mask_obj' entry and '-mask' option were both intentionally specified.}}

    # Cleanup steps
    if {[dcp_wrap {acl modify $DCP_ACL_OBJ_MASK1 -r mask_obj}] != 0} {return;}
}

proc acl_modify_028_N {} {
    # Identify the test
    tet_infoline acl_modify_028_N: Check that only "calc" and "nocalc" are valid values for
    tet_infoline acl_modify_028_N: the -mask option.

    # Setup steps
    if {[dcp_wrap {global DCP_HOSTNAME_ONE DCP_ACL_OBJ_MASK1}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify $DCP_ACL_OBJ_MASK1 -mask foo -a user:$name:r} msg] $msg
    } compare_strings {1 {Invalid '-mask' option value.}}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_029_N {} {
    # Identify the test
    tet_infoline acl_modify_029_N: Add a "mask_obj" ACL entry to the specified ACL object that
    tet_infoline acl_modify_029_N: does not support the mask_obj ACL entry type.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify $dir0 -a mask_obj:rcw} msg] $msg
    } compare_strings {1 {acl entry type not valid}}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_030_N {} {
    # Identify the test
    tet_infoline acl_modify_030_N: Check for an invalid ACL entry type.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify $dir0 -a bozo:$name:r} msg] $msg
    } compare_strings {1 {Invalid ACL entry type.}}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_031_N {} {
    # Identify the test
    tet_infoline acl_modify_031_N: Check for an invalid ACL entry key.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify $dir0 -a user:bozo:r} msg] $msg
    } compare_strings {1 {Invalid ACL entry key.}}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_032_N {} {
    # Identify the test
    tet_infoline acl_modify_032_N: Check for an invalid ACL entry permissions set.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify $dir0 -a user:$name:z} msg] $msg
    } compare_strings {1 {Invalid ACL entry.}}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_033_N {} {
    # Identify the test
    tet_infoline acl_modify_033_N: Check for an ACL entry type that does not require a key
    tet_infoline acl_modify_033_N: but a key was specified.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify $dir0 -a unauthenticated:$name:rt} msg] $msg
    } compare_strings {1 {Invalid ACL entry.}}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_034_N {} {
    # Identify the test
    tet_infoline acl_modify_034_N: Check for adding an ACL entry type but the permissions set
    tet_infoline acl_modify_034_N: was not specified.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify $dir0 -a unauthenticated} msg] $msg
    } compare_strings {1 {Invalid ACL entry.}}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_035_N {} {
    # Identify the test
    tet_infoline acl_modify_035_N: Check for adding an ACL entry type but the key 
    tet_infoline acl_modify_035_N: was not specified.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify $dir0 -a user::r} msg] $msg
    } compare_strings {1 {Invalid ACL entry key.}}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_036_N {} {
    # Identify the test
    tet_infoline acl_modify_036_N: Check for adding an ACL entry but the entry type 
    tet_infoline acl_modify_036_N: was not specified.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify $dir0 -a :$name:r} msg] $msg
    } compare_strings {1 {Invalid ACL entry type.}}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_037_N {} {
    # Identify the test
    tet_infoline acl_modify_037_N: Check for adding an ACL entry but the entry type 
    tet_infoline acl_modify_037_N: and key were not specified.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify $dir0 -a ::r} msg] $msg
    } compare_strings {1 {Invalid ACL entry type.}}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_038_N {} {
    # Identify the test
    tet_infoline acl_modify_038_N: Check for adding an ACL entry that already exists.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify $dir0 -a user:cell_admin:rwc} msg] $msg
    } compare_strings {1 {ACL entry exists.}}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_modify_039_N {} {
    # Identify the test
    tet_infoline acl_modify_039_N: Check for changing an ACL entry that does not exist.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify $dir0 -ch user:$name:r} msg] $msg
    } compare_strings {1 {ACL entry not found.}}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_040_N {} {
    # Identify the test
    tet_infoline acl_modify_040_N: Check for removing an ACL entry that does not exist.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify $dir0 -r user:$name:r} msg] $msg
    } compare_strings {1 {ACL entry not found.}}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_041_N {} {
    # Identify the test
    tet_infoline acl_modify_041_N: Add a new ACL entry with all permissions set on an ACL
    tet_infoline acl_modify_041_N: object that accepts the mask_obj entry and supports 
    tet_infoline acl_modify_041_N: POSIX semantics.

    # Setup steps
    if {[dcp_wrap {global DCP_DFS_AVAIL}] != 0} {return;}
    if {$DCP_DFS_AVAIL == 0} {tet_result UNINITIATED;return}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dfs_dir0 /.:/fs/ $name}] != 0} {return;}
    if {[dcp_wrap {mkdir $dfs_dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {acl modify $dfs_dir0 -a mask_obj:rwc}] != 0} {return;}
    if {[dcp_wrap {acl modify $dfs_dir0 -purge}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify $dfs_dir0 -a user:$name:rwxcid} msg] $msg
    } compare_strings {1 {Unintended permissions not granted.}}

    # Cleanup steps
    if {[dcp_wrap {rmdir $dfs_dir0}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_modify_042_N {} {
    # Identify the test
    tet_infoline acl_modify_042_N: Check that the user needs to be authenticated
    tet_infoline acl_modify_042_N: to modify the object's ACL.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify /.:/hosts -entry -r "user:$DCP_CLIENT"} msg] $msg
    } compare_strings {1 {operation on acl not authorized}}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
}

proc acl_modify_043_N {} {
    # Identify the test
    tet_infoline acl_modify_043_N: Check that a value must be specified for the
    tet_infoline acl_modify_043_N: -type option.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify /.:/hosts -r "user:$DCP_CLIENT" -type} msg] $msg
    } compare_strings {1 {The '-type' option requires a character string value.}}

    # Cleanup steps
}

proc acl_modify_044_N {} {
    # Identify the test
    tet_infoline acl_modify_044_N: Check for an invalid ACL manager name.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify /.:/hosts -r "user:$DCP_CLIENT" -type bozo} msg] $msg
    } compare_strings {1 {ACL manager name specified not found.}}

    # Cleanup steps
}

proc acl_modify_045_N {} {
    # Identify the test
    tet_infoline acl_modify_045_N: Re-calculate the mask for the specified ACL object that
    tet_infoline acl_modify_045_N: does not support the mask_obj ACL entry type. mask_obj
    tet_infoline acl_modify_045_N: entry is not valid for this ACL object.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl modify $dir0 -mask calc -a user:$name:r} msg] $msg
    } compare_strings {1 {acl entry type not valid}}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 ic25 ic26 ic27 ic28 ic29 ic30 ic31 ic32 ic33 ic34 ic35 ic36 ic37 ic38 ic39 ic40 ic41 ic42 ic43 ic44 ic45 "
set ic1 "acl_modify_001_N"
set ic2 "acl_modify_002_N"
set ic3 "acl_modify_003_N"
set ic4 "acl_modify_004_N"
set ic5 "acl_modify_005_N"
set ic6 "acl_modify_006_N"
set ic7 "acl_modify_007_N"
set ic8 "acl_modify_008_N"
set ic9 "acl_modify_009_N"
set ic10 "acl_modify_010_N"
set ic11 "acl_modify_011_N"
set ic12 "acl_modify_012_N"
set ic13 "acl_modify_013_N"
set ic14 "acl_modify_014_N"
set ic15 "acl_modify_015_N"
set ic16 "acl_modify_016_N"
set ic17 "acl_modify_017_N"
set ic18 "acl_modify_018_N"
set ic19 "acl_modify_019_N"
set ic20 "acl_modify_020_N"
set ic21 "acl_modify_021_N"
set ic22 "acl_modify_022_N"
set ic23 "acl_modify_023_N"
set ic24 "acl_modify_024_N"
set ic25 "acl_modify_025_N"
set ic26 "acl_modify_026_N"
set ic27 "acl_modify_027_N"
set ic28 "acl_modify_028_N"
set ic29 "acl_modify_029_N"
set ic30 "acl_modify_030_N"
set ic31 "acl_modify_031_N"
set ic32 "acl_modify_032_N"
set ic33 "acl_modify_033_N"
set ic34 "acl_modify_034_N"
set ic35 "acl_modify_035_N"
set ic36 "acl_modify_036_N"
set ic37 "acl_modify_037_N"
set ic38 "acl_modify_038_N"
set ic39 "acl_modify_039_N"
set ic40 "acl_modify_040_N"
set ic41 "acl_modify_041_N"
set ic42 "acl_modify_042_N"
set ic43 "acl_modify_043_N"
set ic44 "acl_modify_044_N"
set ic45 "acl_modify_045_N"
set tet_startup acl_modify_startup
set tet_cleanup acl_modify_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
