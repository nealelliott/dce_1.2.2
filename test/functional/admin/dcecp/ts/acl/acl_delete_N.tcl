#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: acl_delete_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:09:02  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:27:44  marty]
#
# Revision 1.1.4.2  1995/12/13  15:23:54  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/4  1995/07/21  20:20 UTC  truitt
# 	Merge.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts42/1  1995/07/21  20:19 UTC  truitt
# 	CHFts15731: Fix expected output on some of the tests.
# 
# 	HP revision /main/HPDCE02/3  1995/05/03  13:02 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts27/1  1995/05/03  13:01 UTC  truitt
# 	CHFts15023: Update tests to match new functionality.
# 
# 	HP revision /main/HPDCE02/2  1994/12/12  19:37 UTC  truitt
# 	Fix incorrect error message.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_msg_chk/1  1994/12/12  19:36 UTC  truitt
# 	Fix incorrect error message.
# 
# 	HP revision /main/HPDCE02/1  1994/11/15  12:59 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/15  12:57 UTC  truitt
# 	Fix parameters being used for the acl tests.
# 	[1995/12/11  15:50:45  root]
# 
# Revision 1.1.2.2  1994/10/01  01:32:56  rousseau
# 	Dynamically generate string bindings.
# 	[1994/10/01  01:28:33  rousseau]
# 
# Revision 1.1.2.1  1994/09/28  21:37:06  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:04  melman]
# 
# Revision 1.1.2.11  1994/09/08  14:00:50  salamone
# 	CR11968-Setting authentication on binding handles.
# 	[1994/09/08  13:59:22  salamone]
# 
# Revision 1.1.2.10  1994/08/25  15:33:02  salamone
# 	CR11635 - Handle 1 string binding specification for an ACL object.
# 	[1994/08/25  15:29:02  salamone]
# 
# Revision 1.1.2.9  1994/08/09  21:46:00  salamone
# 	Updated msgs from "more arguments" to "a character string value".
# 	[1994/08/09  21:42:00  salamone]
# 
# Revision 1.1.2.8  1994/07/18  20:04:53  salamone
# 	Add tests for ploymorphic object support
# 	[1994/07/18  19:59:04  salamone]
# 
# Revision 1.1.2.7  1994/06/10  20:01:37  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:08  devsrc]
# 
# Revision 1.1.2.6  1994/05/18  20:39:12  salamone
# 	CR10621 - dcecp "acl show", "acl check", and "acl permissions"
# 		  should display the appropriate info if unauthenticated.
# 	[1994/05/13  22:31:02  salamone]
# 
# Revision 1.1.2.5  1994/05/11  19:34:03  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:14:52  rousseau]
# 
# Revision 1.1.2.4  1994/05/09  14:49:17  salamone
# 	Fix "Ambiguous argument -" error message.
# 	[1994/05/09  12:36:21  salamone]
# 
# Revision 1.1.2.3  1994/05/06  15:00:56  melman
# 	updating to new API path
# 	[1994/05/06  15:00:20  melman]
# 
# Revision 1.1.2.2  1994/04/12  14:21:38  salamone
# 	Added "group add" and "organization add" commands for "account create"
# 	to do its job in the "acl delete" FVT suite.
# 	[1994/04/12  13:53:49  salamone]
# 
# Revision 1.1.2.1  1994/03/24  16:42:51  salamone
# 	    Initial version.
# 	    [1994/03/24  16:41:24  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc acl_delete_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}

    # Load variables for string binding tests
    dcp_acl_binding_vars
}

proc acl_delete_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc acl_delete_001_N {} {
    # Identify the test
    tet_infoline acl_delete_001_N: Check for an insufficient number of arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {acl delete} msg] $msg
    } compare_strings {1 {ACL object not specified.}}

    # Cleanup steps
}

proc acl_delete_002_N {} {
    # Identify the test
    tet_infoline acl_delete_002_N: Check for a bogus ACL object.

    # Setup steps

    # Run the test
    test {
        list [catch {acl delete foo} msg] $msg
    } compare_strings {1 {acl object not found}}

    # Cleanup steps
}

proc acl_delete_003_N {} {
    # Identify the test
    tet_infoline acl_delete_003_N: Check for a bogus ACL object in a list. The first object
    tet_infoline acl_delete_003_N: in the list will be modified, the second object will cause
    tet_infoline acl_delete_003_N: an error, and the third object will not be operated on.
    tet_infoline acl_delete_003_N: Here the dcp_acl_bind_named_object() failed in
    tet_infoline acl_delete_003_N: the acl delete operation.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {group add none -member $name}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $name}] != 0} {return;}
    if {[dcp_wrap {account create $name -group none -organization none -mypwd $DCP_CLIENT_PW -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$name -ch user_obj:rcfug}] != 0} {return;}
    if {[dcp_wrap {login $name -p $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl delete "/.:/sec/principal/$name /.:/foo /.:/hosts"} msg] $msg
    } compare_strings {1 {acl object not found}}

    # Cleanup steps
    if {[dcp_wrap {acl mod /.:/sec/principal/$name -a user:$DCP_CLIENT:rcDnfmaug}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_delete_004_N {} {
    # Identify the test
    tet_infoline acl_delete_004_N: Check for a bogus list of ACL objects.

    # Setup steps

    # Run the test
    test {
        list [catch {acl delete /.:/sec/principal /.:/hosts} msg] $msg
    } compare_strings {1 {Unrecognized argument '/.:/hosts'.}}

    # Cleanup steps
}

proc acl_delete_005_N {} {
    # Identify the test
    tet_infoline acl_delete_005_N: Check that the -entry, -ic, and -io switches are 
    tet_infoline acl_delete_005_N: mutually exclusive.

    # Setup steps

    # Run the test
    test {
        list [catch {acl delete /.:/hosts -io -ic -entry} msg] $msg
    } compare_strings {1 {Too many ACL types.}}

    # Cleanup steps
}

proc acl_delete_006_N {} {
    # Identify the test
    tet_infoline acl_delete_006_N: Check that the -ic, and -io switches are 
    tet_infoline acl_delete_006_N: mutually exclusive.

    # Setup steps

    # Run the test
    test {
        list [catch {acl delete /.:/hosts -io -ic} msg] $msg
    } compare_strings {1 {Too many ACL types.}}

    # Cleanup steps
}

proc acl_delete_007_N {} {
    # Identify the test
    tet_infoline acl_delete_007_N: Check for an invalid option with no name.

    # Setup steps

    # Run the test
    test {
        list [catch {acl delete /.:/hosts -} msg] $msg
    } compare_strings {1 {Ambiguous argument '-': -entry -ic -io -type}}

    # Cleanup steps
}

proc acl_delete_008_N {} {
    # Identify the test
    tet_infoline acl_delete_008_N: Check for an invalid option.

    # Setup steps

    # Run the test
    test {
        list [catch {acl delete /.:/hosts -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc acl_delete_009_N {} {
    # Identify the test
    tet_infoline acl_delete_009_N: Check that a value is specified for the -type
    tet_infoline acl_delete_009_N: option.

    # Setup steps

    # Run the test
    test {
        list [catch {acl delete /.:/hosts -type} msg] $msg
    } compare_strings {1 {The '-type' option requires a character string value.}}

    # Cleanup steps
}

proc acl_delete_010_N {} {
    # Identify the test
    tet_infoline acl_delete_010_N: Check for an invalid ACL manager type.

    # Setup steps

    # Run the test
    test {
        list [catch {acl delete /.:/hosts -type bozo} msg] $msg
    } compare_strings {1 {ACL manager name specified not found.}}

    # Cleanup steps
}

proc acl_delete_011_N {} {
    # Identify the test
    tet_infoline acl_delete_011_N: Check that the switch -entry does not accept any
    tet_infoline acl_delete_011_N: values.

    # Setup steps

    # Run the test
    test {
        list [catch {acl delete /.:/hosts -entry foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc acl_delete_012_N {} {
    # Identify the test
    tet_infoline acl_delete_012_N: Check that the switch -ic does not accept any
    tet_infoline acl_delete_012_N: values.

    # Setup steps

    # Run the test
    test {
        list [catch {acl delete /.:/hosts -ic foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc acl_delete_013_N {} {
    # Identify the test
    tet_infoline acl_delete_013_N: Check that the switch -io does not accept any
    tet_infoline acl_delete_013_N: values.

    # Setup steps

    # Run the test
    test {
        list [catch {acl delete /.:/hosts -io foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc acl_delete_014_N {} {
    # Identify the test
    tet_infoline acl_delete_014_N: Check that the dcp_acl_do_save_sec_acl() failed in
    tet_infoline acl_delete_014_N: the acl delete operation. The first object in the list 
    tet_infoline acl_delete_014_N: will be modified, the second object will cause an error, 
    tet_infoline acl_delete_014_N: and the third object will not be operated on.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create "$prin0 $prin1"}] != 0} {return;}
    if {[dcp_wrap {group add none -member "$prin0 $prin1"}] != 0} {return;}
    if {[dcp_wrap {organization add none -member "$prin0 $prin1"}] != 0} {return;}
    if {[dcp_wrap {account create "$prin0 $prin1" -group none -organization none -mypwd $DCP_CLIENT_PW -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$prin0 -ch user_obj:rcfug}] != 0} {return;}
    if {[dcp_wrap {login $prin0 -p $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl delete "/.:/sec/principal/$prin0 /.:/sec/principal/$prin1 /.:/hosts"} msg] $msg
    } compare_strings {1 {operation on acl not authorized}}

    # Cleanup steps
    if {[dcp_wrap {acl mod /.:/sec/principal/$prin0 -a user:$DCP_CLIENT:rcDnfmaug}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {principal delete "$prin0 $prin1"}] != 0} {return;}
}

proc acl_delete_015_N {} {
    # Identify the test
    tet_infoline acl_delete_015_N: Check that the user must be authenticated to
    tet_infoline acl_delete_015_N: delete an object's ACL.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {group add none -member $name}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $name}] != 0} {return;}
    if {[dcp_wrap {account create $name -group none -organization none -mypwd $DCP_CLIENT_PW -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$name -ch user_obj:rcfug}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl delete /.:/hosts -entry} msg] $msg
    } compare_strings {1 {operation on acl not authorized}}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_delete_016_N {} {
    # Identify the test
    tet_infoline acl_delete_016_N: Check that the -entry switch used with a string binding
    tet_infoline acl_delete_016_N: produces an error.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW OBJECT_UUID PROTSEQ NETADDR}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {group add none -member $name}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $name}] != 0} {return;}
    if {[dcp_wrap {account create $name -group none -organization none -mypwd $DCP_CLIENT_PW -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$name -ch user_obj:rcfug}] != 0} {return;}
    if {[dcp_wrap {login $name -p $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {acl delete "$OBJECT_UUID@$PROTSEQ:$NETADDR principal/$name" -entry} msg] $msg
    } compare_strings {1 {Entry option used with a string binding.}}

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 "
set ic1 "acl_delete_001_N"
set ic2 "acl_delete_002_N"
set ic3 "acl_delete_003_N"
set ic4 "acl_delete_004_N"
set ic5 "acl_delete_005_N"
set ic6 "acl_delete_006_N"
set ic7 "acl_delete_007_N"
set ic8 "acl_delete_008_N"
set ic9 "acl_delete_009_N"
set ic10 "acl_delete_010_N"
set ic11 "acl_delete_011_N"
set ic12 "acl_delete_012_N"
set ic13 "acl_delete_013_N"
set ic14 "acl_delete_014_N"
set ic15 "acl_delete_015_N"
set ic16 "acl_delete_016_N"
set tet_startup acl_delete_startup
set tet_cleanup acl_delete_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
