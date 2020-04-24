#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: acl_replace_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:09:28  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:27:55  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:04  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/4  1995/07/21  20:16 UTC  truitt
# 	Merge.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts42/1  1995/07/21  20:15 UTC  truitt
# 	CHFts15731: Fix expected output on some of the tests.
# 
# 	HP revision /main/HPDCE02/3  1995/05/03  12:58 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts27/1  1995/05/03  12:58 UTC  truitt
# 	CHFts15023: Update tests to match new functionality.
# 
# 	HP revision /main/HPDCE02/2  1994/12/12  19:34 UTC  truitt
# 	Fix incorrect error message.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_msg_chk/1  1994/12/12  19:33 UTC  truitt
# 	Fix incorrect error message.
# 
# 	HP revision /main/HPDCE02/1  1994/11/15  13:01 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/15  13:00 UTC  truitt
# 	Fix parameters being used for the acl tests.
# 	[1995/12/11  15:50:57  root]
# 
# Revision 1.1.2.2  1994/10/01  01:33:06  rousseau
# 	Dynamically generate string bindings.
# 	[1994/10/01  01:28:49  rousseau]
# 
# Revision 1.1.2.1  1994/09/28  21:37:22  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:15  melman]
# 
# Revision 1.1.2.10  1994/09/08  14:00:55  salamone
# 	CR11968-Setting authentication on binding handles.
# 	[1994/09/08  13:59:26  salamone]
# 
# Revision 1.1.2.9  1994/08/25  15:33:10  salamone
# 	CR11635 - Handle 1 string binding specification for an ACL object.
# 	[1994/08/25  15:29:58  salamone]
# 
# Revision 1.1.2.8  1994/08/09  21:45:45  salamone
# 	Updated msgs from "more arguments" to "a character string value".
# 	[1994/08/09  21:10:12  salamone]
# 
# Revision 1.1.2.7  1994/07/18  20:05:04  salamone
# 	Add tests for polymorphic object support
# 	[1994/07/18  20:01:39  salamone]
# 
# Revision 1.1.2.6  1994/06/10  20:01:46  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:25  devsrc]
# 
# Revision 1.1.2.5  1994/05/18  20:39:11  salamone
# 	CR10621-dcecp "acl show", "acl check", and "acl permissions" should
# 	        display the appropriate information if unauthenticated.
# 	[1994/05/13  22:33:45  salamone]
# 
# Revision 1.1.2.4  1994/05/11  19:34:19  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:15:19  rousseau]
# 
# Revision 1.1.2.3  1994/05/09  14:49:20  salamone
# 	Fix "Ambiguous argument -" error message.
# 	[1994/05/09  12:40:12  salamone]
# 
# Revision 1.1.2.2  1994/05/06  15:01:05  melman
# 	updating to new API path
# 	[1994/05/06  15:00:33  melman]
# 
# Revision 1.1.2.1  1994/03/24  16:42:56  salamone
# 	Initial version.
# 	[1994/03/24  16:42:12  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
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
proc acl_replace_001_N {} {
    # Identify the test
    tet_infoline acl_replace_001_N: Check for an insufficient number of arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {acl replace} msg] $msg
    } compare_strings {1 {ACL object not specified.}}

    # Cleanup steps
}

proc acl_replace_002_N {} {
    # Identify the test
    tet_infoline acl_replace_002_N: Check for a bogus ACL object.

    # Setup steps

    # Run the test
    test {
        list [catch {acl replace foo -acl [acl sh /.:/hosts]} msg] $msg
    } compare_strings {1 {acl object not found}}

    # Cleanup steps
}

proc acl_replace_003_N {} {
    # Identify the test
    tet_infoline acl_replace_003_N: Check for a bogus ACL object in a list. The first object 
    tet_infoline acl_replace_003_N: in the list will be modified, the second object will cause
    tet_infoline acl_replace_003_N: an error, and the third object will not be operated on.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {append dir1 /.:/ $name 1}] != 0} {return;}
    if {[dcp_wrap {directory create "$dir0 $dir1"}] != 0} {return;}
    if {[dcp_wrap {acl mod $dir0 -r {unauthenticated any_other}}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl replace "$dir0 /.:/foo /.:/hosts" -acl [acl sh $dir1]} msg] $msg
    } compare_strings {1 {acl object not found}}

    # Cleanup steps
    if {[dcp_wrap {directory delete "$dir0 $dir1"}] != 0} {return;}
}

proc acl_replace_004_N {} {
    # Identify the test
    tet_infoline acl_replace_004_N: Check for a bogus list of ACL objects.

    # Setup steps

    # Run the test
    test {
        list [catch {acl replace /.:/hosts /.:/sec/principal} msg] $msg
    } compare_strings {1 {Unrecognized argument '/.:/sec/principal'.}}

    # Cleanup steps
}

proc acl_replace_005_N {} {
    # Identify the test
    tet_infoline acl_replace_005_N: Check that the -entry, -ic, and -io switches are 
    tet_infoline acl_replace_005_N: mutually exclusive.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl replace /.:/hosts -io -ic -entry -acl user:$DCP_CLIENT:rwdtcia} msg] $msg
    } compare_strings {1 {Too many ACL types.}}

    # Cleanup steps
}

proc acl_replace_006_N {} {
    # Identify the test
    tet_infoline acl_replace_006_N: Check that the -ic, and -io switches are 
    tet_infoline acl_replace_006_N: mutually exclusive.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl replace /.:/hosts -io -ic -acl user:$DCP_CLIENT:rwdtcia} msg] $msg
    } compare_strings {1 {Too many ACL types.}}

    # Cleanup steps
}

proc acl_replace_007_N {} {
    # Identify the test
    tet_infoline acl_replace_007_N: Check for an invalid option with no name.

    # Setup steps

    # Run the test
    test {
        list [catch {acl replace /.:/hosts -} msg] $msg
    } compare_strings {1 {Ambiguous argument '-': -acl -cell -entry -ic -io -type}}

    # Cleanup steps
}

proc acl_replace_008_N {} {
    # Identify the test
    tet_infoline acl_replace_008_N: Check for an invalid option.

    # Setup steps

    # Run the test
    test {
        list [catch {acl replace /.:/hosts -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc acl_replace_009_N {} {
    # Identify the test
    tet_infoline acl_replace_009_N: Check that the switch -entry does not accept any
    tet_infoline acl_replace_009_N: values.

    # Setup steps

    # Run the test
    test {
        list [catch {acl replace /.:/hosts -entry foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc acl_replace_010_N {} {
    # Identify the test
    tet_infoline acl_replace_010_N: Check that the switch -ic does not accept any
    tet_infoline acl_replace_010_N: values.

    # Setup steps

    # Run the test
    test {
        list [catch {acl replace /.:/hosts -ic foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc acl_replace_011_N {} {
    # Identify the test
    tet_infoline acl_replace_011_N: Check that the switch -io does not accept any
    tet_infoline acl_replace_011_N: values.

    # Setup steps

    # Run the test
    test {
        list [catch {acl replace /.:/hosts -io foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc acl_replace_012_N {} {
    # Identify the test
    tet_infoline acl_replace_012_N: Check that the -acl option needs at least one ACL entry
    tet_infoline acl_replace_012_N: specified.

    # Setup steps

    # Run the test
    test {
        list [catch {acl replace /.:/hosts -acl} msg] $msg
    } compare_strings {1 {The '-acl' option requires a character string value.}}

    # Cleanup steps
}

proc acl_replace_013_N {} {
    # Identify the test
    tet_infoline acl_replace_013_N: Check that the -acl option requires a valid ACL entry
    tet_infoline acl_replace_013_N: specified.

    # Setup steps

    # Run the test
    test {
        list [catch {acl replace /.:/hosts -acl foo} msg] $msg
    } compare_strings {1 {Invalid ACL entry.}}

    # Cleanup steps
}

proc acl_replace_014_N {} {
    # Identify the test
    tet_infoline acl_replace_014_N: Check that the -acl option requires a valid ACL entry
    tet_infoline acl_replace_014_N: with permissions specified.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl replace /.:/hosts -acl user:$DCP_CLIENT} msg] $msg
    } compare_strings {1 {Invalid ACL entry.}}

    # Cleanup steps
}

proc acl_replace_015_N {} {
    # Identify the test
    tet_infoline acl_replace_015_N: Check that the -acl option requires a valid ACL entry
    tet_infoline acl_replace_015_N: key specified.

    # Setup steps

    # Run the test
    test {
        list [catch {acl replace /.:/hosts -acl user:bozo:rc} msg] $msg
    } compare_strings {1 {Invalid ACL entry key.}}

    # Cleanup steps
}

proc acl_replace_016_N {} {
    # Identify the test
    tet_infoline acl_replace_016_N: Check that the -acl option requires a valid ACL entry
    tet_infoline acl_replace_016_N: type specified.

    # Setup steps

    # Run the test
    test {
        list [catch {acl replace /.:/hosts -acl bozo:cell_admin} msg] $msg
    } compare_strings {1 {Invalid ACL entry type.}}

    # Cleanup steps
}

proc acl_replace_017_N {} {
    # Identify the test
    tet_infoline acl_replace_017_N: Check that the -acl option is required.

    # Setup steps

    # Run the test
    test {
        list [catch {acl replace /.:/hosts -cell `getcellname`} msg] $msg
    } compare_strings {1 {The '-acl' option is required.}}

    # Cleanup steps
}

proc acl_replace_018_N {} {
    # Identify the test
    tet_infoline acl_replace_018_N: Check that a value is specified for the -cell option.

    # Setup steps

    # Run the test
    test {
        list [catch {acl replace /.:/hosts -acl [acl sh /.:/sec/principal] -cell} msg] $msg
    } compare_strings {1 {The '-cell' option requires a character string value.}}

    # Cleanup steps
}

proc acl_replace_019_N {} {
    # Identify the test
    tet_infoline acl_replace_019_N: Check for a bogus default cell name.

    # Setup steps

    # Run the test
    test {
        list [catch {acl replace /.:/hosts -acl [acl sh /.:/sec/principal] -cell foo} msg] $msg
    } compare_strings {1 {Invalid default cell name.}}

    # Cleanup steps
}

proc acl_replace_020_N {} {
    # Identify the test
    tet_infoline acl_replace_020_N: Check for a default cell name not found in the Registry.

    # Setup steps

    # Run the test
    test {
        list [catch {acl replace /.:/hosts -acl [acl sh /.:/sec/principal] -cell /.../foo} msg] $msg
    } compare_strings {1 {Registry object not found}}

    # Cleanup steps
}

proc acl_replace_021_N {} {
    # Identify the test
    tet_infoline acl_replace_021_N: Check that duplicate entries are not added during
    tet_infoline acl_replace_021_N: the acl replace operation.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append dir0 /.:/ $name 0}] != 0} {return;}
    if {[dcp_wrap {directory create $dir0}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl replace $dir0 -acl "any_other:rwc any_other:rwdtcia"} msg] $msg
    } compare_strings {1 {Duplicate ACL entries are not allowed.}}

    # Cleanup steps
    if {[dcp_wrap {directory delete $dir0}] != 0} {return;}
}

proc acl_replace_022_N {} {
    # Identify the test
    tet_infoline acl_replace_022_N: Check that the -entry switch used with a string binding
    tet_infoline acl_replace_022_N: produces an error.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create "$prin0 $prin1"}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$prin0 -r {unauthenticated any_other}}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl replace "$OBJECT_UUID@$PROTSEQ:$NETADDR principal/$prin0" -entry -acl [acl sh /.:/sec/principal/$prin1]} msg] $msg
    } compare_strings {1 {Entry option used with a string binding.}}

    # Cleanup steps
    if {[dcp_wrap {principal delete "$prin0 $prin1"}] != 0} {return;}
}

proc acl_replace_023_N {} {
    # Identify the test
    tet_infoline acl_replace_023_N: Check that the user must be authenticated to
    tet_infoline acl_replace_023_N: replace an object's ACL.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl replace /.:/hosts -entry -acl "user:$DCP_CLIENT:rc"} msg] $msg
    } compare_strings {1 {operation on acl not authorized}}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
}

proc acl_replace_024_N {} {
    # Identify the test
    tet_infoline acl_replace_024_N: Check that a value is specified for the -type
    tet_infoline acl_replace_024_N: option.

    # Setup steps

    # Run the test
    test {
        list [catch {acl replace /.:/hosts -acl [acl sh /.:/sec/principal] -type} msg] $msg
    } compare_strings {1 {The '-type' option requires a character string value.}}

    # Cleanup steps
}

proc acl_replace_025_N {} {
    # Identify the test
    tet_infoline acl_replace_025_N: Check for an invalid ACL manager name.

    # Setup steps

    # Run the test
    test {
        list [catch {acl replace /.:/hosts -acl [acl sh /.:/sec/principal] -type bozo} msg] $msg
    } compare_strings {1 {ACL manager name specified not found.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 ic21 ic22 ic23 ic24 ic25 "
set ic1 "acl_replace_001_N"
set ic2 "acl_replace_002_N"
set ic3 "acl_replace_003_N"
set ic4 "acl_replace_004_N"
set ic5 "acl_replace_005_N"
set ic6 "acl_replace_006_N"
set ic7 "acl_replace_007_N"
set ic8 "acl_replace_008_N"
set ic9 "acl_replace_009_N"
set ic10 "acl_replace_010_N"
set ic11 "acl_replace_011_N"
set ic12 "acl_replace_012_N"
set ic13 "acl_replace_013_N"
set ic14 "acl_replace_014_N"
set ic15 "acl_replace_015_N"
set ic16 "acl_replace_016_N"
set ic17 "acl_replace_017_N"
set ic18 "acl_replace_018_N"
set ic19 "acl_replace_019_N"
set ic20 "acl_replace_020_N"
set ic21 "acl_replace_021_N"
set ic22 "acl_replace_022_N"
set ic23 "acl_replace_023_N"
set ic24 "acl_replace_024_N"
set ic25 "acl_replace_025_N"
set tet_startup acl_replace_startup
set tet_cleanup acl_replace_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
