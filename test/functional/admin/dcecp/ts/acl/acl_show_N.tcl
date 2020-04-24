#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: acl_show_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:09:35  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:27:58  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:07  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1995/07/21  20:14 UTC  truitt
# 	Merge.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts42/1  1995/07/21  20:13 UTC  truitt
# 	CHFts15731: Fix expected output on some of the tests.
# 
# 	HP revision /main/HPDCE02/2  1995/05/03  12:57 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts27/1  1995/05/03  12:56 UTC  truitt
# 	CHFts15023: Update tests to match new functionality.
# 
# 	HP revision /main/HPDCE02/1  1994/12/12  19:32 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_msg_chk/1  1994/12/12  19:31 UTC  truitt
# 	Fix incorrect error message.
# 	[1995/12/11  15:51:04  root]
# 
# Revision 1.1.2.2  1994/10/01  01:33:08  rousseau
# 	Dynamically generate string bindings.
# 	[1994/10/01  01:28:52  rousseau]
# 
# Revision 1.1.2.1  1994/09/28  21:37:27  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:20  melman]
# 
# Revision 1.1.2.9  1994/09/08  14:00:57  salamone
# 	CR11968-Setting authentication on binding handles.
# 	[1994/09/08  13:59:28  salamone]
# 
# Revision 1.1.2.8  1994/08/25  15:33:13  salamone
# 	CR11635 - Handle 1 string binding specification for an ACL object.
# 	[1994/08/25  15:30:16  salamone]
# 
# Revision 1.1.2.7  1994/08/09  21:45:57  salamone
# 	Updated msgs from "more arguments" to "a character string value".
# 	[1994/08/09  21:10:30  salamone]
# 
# Revision 1.1.2.6  1994/07/18  20:05:07  salamone
# 	Add tests for polymorphic object support
# 	[1994/07/18  20:02:20  salamone]
# 
# Revision 1.1.2.5  1994/06/10  20:02:19  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:27  devsrc]
# 
# Revision 1.1.2.4  1994/05/11  19:34:21  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:15:28  rousseau]
# 
# Revision 1.1.2.3  1994/05/09  14:49:21  salamone
# 	Fix "Ambiguous argument -" error message.
# 	[1994/05/09  12:40:36  salamone]
# 
# Revision 1.1.2.2  1994/05/06  15:01:07  melman
# 	updating to new API path
# 	[1994/05/06  15:00:36  melman]
# 
# Revision 1.1.2.1  1994/03/24  16:28:57  salamone
# 	Error msg from TCL changed.
# 	[1994/03/24  15:04:26  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc acl_show_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}

    # Load variables for string binding tests
    dcp_acl_binding_vars
}

proc acl_show_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc acl_show_001_N {} {
    # Identify the test
    tet_infoline acl_show_001_N: Check for an insufficient number of arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {acl show} msg] $msg
    } compare_strings {1 {ACL object not specified.}}

    # Cleanup steps
}

proc acl_show_002_N {} {
    # Identify the test
    tet_infoline acl_show_002_N: Check for a bogus ACL object.

    # Setup steps

    # Run the test
    test {
        list [catch {acl show foo} msg] $msg
    } compare_strings {1 {acl object not found}}

    # Cleanup steps
}

proc acl_show_003_N {} {
    # Identify the test
    tet_infoline acl_show_003_N: Check for a bogus ACL object in a list.

    # Setup steps

    # Run the test
    test {
        list [catch {acl show {/.:/hosts /.:/foo /.:/subsys}} msg] $msg
    } compare_strings {1 {acl object not found}}

    # Cleanup steps
}

proc acl_show_004_N {} {
    # Identify the test
    tet_infoline acl_show_004_N: Check for a bogus list of ACL objects.

    # Setup steps

    # Run the test
    test {
        list [catch {acl show /.:/hosts /.:/subsys} msg] $msg
    } compare_strings {1 {Unrecognized argument '/.:/subsys'.}}

    # Cleanup steps
}

proc acl_show_005_N {} {
    # Identify the test
    tet_infoline acl_show_005_N: Check that the -entry, -ic, and -io switches are 
    tet_infoline acl_show_005_N: mutually exclusive.

    # Setup steps

    # Run the test
    test {
        list [catch {acl show /.:/hosts -io -ic -entry} msg] $msg
    } compare_strings {1 {Too many ACL types.}}

    # Cleanup steps
}

proc acl_show_006_N {} {
    # Identify the test
    tet_infoline acl_show_006_N: Check that the -ic, and -io switches are 
    tet_infoline acl_show_006_N: mutually exclusive.

    # Setup steps

    # Run the test
    test {
        list [catch {acl show /.:/hosts -io -ic} msg] $msg
    } compare_strings {1 {Too many ACL types.}}

    # Cleanup steps
}

proc acl_show_007_N {} {
    # Identify the test
    tet_infoline acl_show_007_N: Check for an invalid option with no name.

    # Setup steps

    # Run the test
    test {
        list [catch {acl show /.:/hosts -} msg] $msg
    } compare_strings {1 {Ambiguous argument '-': -cell -entry -ic -io -managers -type}}

    # Cleanup steps
}

proc acl_show_008_N {} {
    # Identify the test
    tet_infoline acl_show_008_N: Check for an invalid option.

    # Setup steps

    # Run the test
    test {
        list [catch {acl show /.:/hosts -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc acl_show_009_N {} {
    # Identify the test
    tet_infoline acl_show_009_N: Check that the switch -entry does not accept any
    tet_infoline acl_show_009_N: values.

    # Setup steps

    # Run the test
    test {
        list [catch {acl show /.:/hosts -entry foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc acl_show_010_N {} {
    # Identify the test
    tet_infoline acl_show_010_N: Check that the switch -ic does not accept any
    tet_infoline acl_show_010_N: values.

    # Setup steps

    # Run the test
    test {
        list [catch {acl show /.:/hosts -ic foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc acl_show_011_N {} {
    # Identify the test
    tet_infoline acl_show_011_N: Check that the switch -io does not accept any
    tet_infoline acl_show_011_N: values.

    # Setup steps

    # Run the test
    test {
        list [catch {acl show /.:/hosts -io foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc acl_show_012_N {} {
    # Identify the test
    tet_infoline acl_show_012_N: Check that the switch -cell does not accept any
    tet_infoline acl_show_012_N: values.

    # Setup steps

    # Run the test
    test {
        list [catch {acl show /.:/hosts -cell foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc acl_show_013_N {} {
    # Identify the test
    tet_infoline acl_show_013_N: Check that a value is specified for the -type
    tet_infoline acl_show_013_N: option.

    # Setup steps

    # Run the test
    test {
        list [catch {acl show /.:/hosts -type} msg] $msg
    } compare_strings {1 {The '-type' option requires a character string value.}}

    # Cleanup steps
}

proc acl_show_014_N {} {
    # Identify the test
    tet_infoline acl_show_014_N: Check for an invalid ACL manager name.

    # Setup steps

    # Run the test
    test {
        list [catch {acl show /.:/hosts -type bozo} msg] $msg
    } compare_strings {1 {ACL manager name specified not found.}}

    # Cleanup steps
}

proc acl_show_015_N {} {
    # Identify the test
    tet_infoline acl_show_015_N: Check that the option -managers does not accept any
    tet_infoline acl_show_015_N: values.

    # Setup steps

    # Run the test
    test {
        list [catch {acl show /.:/hosts -managers foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc acl_show_016_N {} {
    # Identify the test
    tet_infoline acl_show_016_N: Check that the -cell and -managers options are 
    tet_infoline acl_show_016_N: mutually exclusive.

    # Setup steps

    # Run the test
    test {
        list [catch {acl show /.:/hosts -cell -managers} msg] $msg
    } compare_strings {1 {Mutually exclusive options entered.}}

    # Cleanup steps
}

proc acl_show_017_N {} {
    # Identify the test
    tet_infoline acl_show_017_N: Check that the -managers and -type options are 
    tet_infoline acl_show_017_N: mutually exclusive.

    # Setup steps

    # Run the test
    test {
        list [catch {acl show /.:/hosts -managers -type Directory} msg] $msg
    } compare_strings {1 {Mutually exclusive options entered.}}

    # Cleanup steps
}

proc acl_show_018_N {} {
    # Identify the test
    tet_infoline acl_show_018_N: Check that the -entry switch used with a string binding
    tet_infoline acl_show_018_N: produces an error.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl show "$OBJECT_UUID@$PROTSEQ:$NETADDR principal/$name" -entry} msg] $msg
    } compare_strings {1 {Entry option used with a string binding.}}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 "
set ic1 "acl_show_001_N"
set ic2 "acl_show_002_N"
set ic3 "acl_show_003_N"
set ic4 "acl_show_004_N"
set ic5 "acl_show_005_N"
set ic6 "acl_show_006_N"
set ic7 "acl_show_007_N"
set ic8 "acl_show_008_N"
set ic9 "acl_show_009_N"
set ic10 "acl_show_010_N"
set ic11 "acl_show_011_N"
set ic12 "acl_show_012_N"
set ic13 "acl_show_013_N"
set ic14 "acl_show_014_N"
set ic15 "acl_show_015_N"
set ic16 "acl_show_016_N"
set ic17 "acl_show_017_N"
set ic18 "acl_show_018_N"
set tet_startup acl_show_startup
set tet_cleanup acl_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
