#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: acl_permissions_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:09:22  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:27:54  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:02  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/4  1995/07/21  20:17 UTC  truitt
# 	Merge.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts42/1  1995/07/21  20:16 UTC  truitt
# 	CHFts15731: Fix expected output on some of the tests.
# 
# 	HP revision /main/HPDCE02/3  1995/07/18  19:50 UTC  jrr
# 	# Update tests to handle new output.
# 	# [1995/07/18  19:48 UTC  jrr  /main/HPDCE02/jrr_mothra_pure/1]
# 
# 	HP revision /main/HPDCE02/2  1995/05/03  12:59 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts27/1  1995/05/03  12:59 UTC  truitt
# 	CHFts15023: Update tests to match new functionality.
# 
# 	HP revision /main/HPDCE02/1  1994/12/13  14:54 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_msg_chk/1  1994/12/13  14:51 UTC  truitt
# 	Fix incorrect error message.
# 	[1995/12/11  15:50:54  root]
# 
# Revision 1.1.2.2  1994/10/01  01:33:03  rousseau
# 	Dynamically generate string bindings.
# 	[1994/10/01  01:28:46  rousseau]
# 
# Revision 1.1.2.1  1994/09/28  21:37:18  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:13  melman]
# 
# Revision 1.1.2.8  1994/09/08  14:00:53  salamone
# 	CR11968-Setting authentication on binding handles.
# 	[1994/09/08  13:59:25  salamone]
# 
# Revision 1.1.2.7  1994/08/25  15:33:09  salamone
# 	CR11635 - Handle 1 string binding specification for an ACL object.
# 	[1994/08/25  15:29:42  salamone]
# 
# Revision 1.1.2.6  1994/08/09  21:45:55  salamone
# 	Updated msgs from "more arguments" to "a character string value".
# 	[1994/08/09  21:09:54  salamone]
# 
# Revision 1.1.2.5  1994/07/18  20:05:01  salamone
# 	Add tests for polymorphic object support
# 	[1994/07/18  20:00:56  salamone]
# 
# Revision 1.1.2.4  1994/06/10  20:01:44  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:23  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:34:14  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:15:12  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:01:03  melman
# 	updating to new API path
# 	[1994/05/06  15:00:30  melman]
# 
# Revision 1.1.2.1  1994/03/24  16:28:53  salamone
# 	Error msg from TCL changed.
# 	[1994/03/24  15:03:58  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc acl_permissions_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}

    # Load variables for string binding tests
    dcp_acl_binding_vars
}

proc acl_permissions_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc acl_permissions_001_N {} {
    # Identify the test
    tet_infoline acl_permissions_001_N: Check for an insufficient number of arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {acl permissions} msg] $msg
    } compare_strings {1 {ACL object not specified.}}

    # Cleanup steps
}

proc acl_permissions_002_N {} {
    # Identify the test
    tet_infoline acl_permissions_002_N: Check for a bogus ACL object.

    # Setup steps

    # Run the test
    test {
        list [catch {acl permissions foo} msg] $msg
    } compare_strings {1 {acl object not found}}

    # Cleanup steps
}

proc acl_permissions_003_N {} {
    # Identify the test
    tet_infoline acl_permissions_003_N: Check for a bogus ACL object in a list.

    # Setup steps

    # Run the test
    test {
        list [catch {acl permissions {/.:/hosts /.:/foo /.:/subsys}} msg] $msg
    } compare_strings {1 {acl object not found}}

    # Cleanup steps
}

proc acl_permissions_004_N {} {
    # Identify the test
    tet_infoline acl_permissions_004_N: Check for a bogus list of ACL objects.

    # Setup steps

    # Run the test
    test {
        list [catch {acl permissions /.:/hosts /.:/subsys} msg] $msg
    } compare_strings {1 {Unrecognized argument '/.:/subsys'.}}

    # Cleanup steps
}

proc acl_permissions_005_N {} {
    # Identify the test
    tet_infoline acl_permissions_005_N: Check that the -entry, -ic, and -io switches are 
    tet_infoline acl_permissions_005_N: mutually exclusive.

    # Setup steps

    # Run the test
    test {
        list [catch {acl permissions /.:/hosts -io -ic -entry} msg] $msg
    } compare_strings {1 {Too many ACL types.}}

    # Cleanup steps
}

proc acl_permissions_006_N {} {
    # Identify the test
    tet_infoline acl_permissions_006_N: Check that the -ic, and -io switches are 
    tet_infoline acl_permissions_006_N: mutually exclusive.

    # Setup steps

    # Run the test
    test {
        list [catch {acl permissions /.:/hosts -io -ic} msg] $msg
    } compare_strings {1 {Too many ACL types.}}

    # Cleanup steps
}

proc acl_permissions_007_N {} {
    # Identify the test
    tet_infoline acl_permissions_007_N: Check for an invalid option with no name.

    # Setup steps

    # Run the test
    test {
        list [catch {acl permissions /.:/hosts -} msg] $msg
    } compare_strings {1 {Ambiguous argument '-': -entry -ic -io -type}}

    # Cleanup steps
}

proc acl_permissions_008_N {} {
    # Identify the test
    tet_infoline acl_permissions_008_N: Check for an invalid option.

    # Setup steps

    # Run the test
    test {
        list [catch {acl permissions /.:/hosts -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc acl_permissions_009_N {} {
    # Identify the test
    tet_infoline acl_permissions_009_N: Check that the switch -entry does not accept any
    tet_infoline acl_permissions_009_N: values.

    # Setup steps

    # Run the test
    test {
        list [catch {acl permissions /.:/hosts -entry foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc acl_permissions_010_N {} {
    # Identify the test
    tet_infoline acl_permissions_010_N: Check that the switch -ic does not accept any
    tet_infoline acl_permissions_010_N: values.

    # Setup steps

    # Run the test
    test {
        list [catch {acl permissions /.:/hosts -ic foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc acl_permissions_011_N {} {
    # Identify the test
    tet_infoline acl_permissions_011_N: Check that the switch -io does not accept any
    tet_infoline acl_permissions_011_N: values.

    # Setup steps

    # Run the test
    test {
        list [catch {acl permissions /.:/hosts -io foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc acl_permissions_012_N {} {
    # Identify the test
    tet_infoline acl_permissions_012_N: Check that a value is specified for the -type
    tet_infoline acl_permissions_012_N: option.

    # Setup steps

    # Run the test
    test {
        list [catch {acl permissions /.:/hosts -type} msg] $msg
    } compare_strings {1 {The '-type' option requires a character string value.}}

    # Cleanup steps
}

proc acl_permissions_013_N {} {
    # Identify the test
    tet_infoline acl_permissions_013_N: Check for an invalid ACL manager type.

    # Setup steps

    # Run the test
    test {
        list [catch {acl permissions /.:/hosts -type bozo} msg] $msg
    } compare_strings {1 {ACL manager name specified not found.}}

    # Cleanup steps
}

proc acl_permissions_014_N {} {
    # Identify the test
    tet_infoline acl_permissions_014_N: Check that the -entry switch used with a string binding
    tet_infoline acl_permissions_014_N: produces an error.

    # Setup steps
    if {[dcp_wrap {global OBJECT_UUID PROTSEQ NETADDR}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl permissions "$OBJECT_UUID@$PROTSEQ:$NETADDR principal/$name" -entry} msg] $msg
    } compare_strings {1 {Entry option used with a string binding.}}

    # Cleanup steps
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 "
set ic1 "acl_permissions_001_N"
set ic2 "acl_permissions_002_N"
set ic3 "acl_permissions_003_N"
set ic4 "acl_permissions_004_N"
set ic5 "acl_permissions_005_N"
set ic6 "acl_permissions_006_N"
set ic7 "acl_permissions_007_N"
set ic8 "acl_permissions_008_N"
set ic9 "acl_permissions_009_N"
set ic10 "acl_permissions_010_N"
set ic11 "acl_permissions_011_N"
set ic12 "acl_permissions_012_N"
set ic13 "acl_permissions_013_N"
set ic14 "acl_permissions_014_N"
set tet_startup acl_permissions_startup
set tet_cleanup acl_permissions_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
