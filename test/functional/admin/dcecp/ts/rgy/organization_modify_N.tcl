#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: organization_modify_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:20:53  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:33:51  marty]
#
# Revision 1.1.4.2  1995/12/13  15:28:07  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:55:00  root]
# 
# Revision 1.1.2.1  1994/09/28  21:42:51  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:11:48  melman]
# 
# Revision 1.1.2.6  1994/08/30  18:28:55  kevins
# 	OT 10617 modified messages/output for functional tests
# 	[1994/08/30  18:28:27  kevins]
# 
# Revision 1.1.2.5  1994/08/12  18:04:20  kevins
# 	OT 10617 fixed some messages
# 	[1994/08/12  17:54:35  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:15:18  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:55:57  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:39:36  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:25:43  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:16:08  melman
# 	updating to new API path
# 	[1994/05/06  17:14:44  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:22:12  kevins
# 	Initial test drop.
# 	[1994/03/09  19:18:37  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

## Test purposes
proc organization_modify_1_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_modify_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {organization modify foo bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'bar'.}}

    # Cleanup steps
}

proc organization_modify_2_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_modify_2_N: Check that missing required argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization modify} msg] $msg
    } compare_strings {1 {No organization name.}}

    # Cleanup steps
}

proc organization_modify_3_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_modify_3_N: Check that attribute and option arguments are not allowed..

    # Setup steps

    # Run the test
    test {
        list [catch {organization modify foo -fullname yes -change {fullname yes}} msg] $msg
    } compare_strings {1 {Only options or an attribute list are allowed.}}

    # Cleanup steps
}

proc organization_modify_4_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_modify_4_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization modify foo -fullname} msg] $msg
    } compare_strings {1 {The '-fullname' option requires a character string value.}}

    # Cleanup steps
}

proc organization_modify_5_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_modify_5_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization modify foo -change} msg] $msg
    } compare_strings {1 {The '-change' option requires a character string value.}}

    # Cleanup steps
}

proc organization_modify_6_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_modify_6_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization create foo -bar} msg] $msg
    } compare_strings {1 {Unrecognized option '-bar'.}}

    # Cleanup steps
}

proc organization_modify_7_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline organization_modify_7_N: Check that unauthorized use is trapped as an error.

    # Setup steps
    set org_name [lindex [info level [info level]] 0]
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {organization create $org_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test {
        list [catch {organization modify $org_name -fullname "foo_bar"} msg] $msg
    } compare_strings {1 {User is not authorized to update record}}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {organization delete $org_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

}

proc organization_modify_8_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_modify_8_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization modify foo -pwdminlen asdfasdfs} msg] $msg
    } compare_strings {1 {Supplied 'pwdminlen' is not a positive integer.}}

    # Cleanup steps
}

proc organization_modify_9_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_modify_9_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization modify foo -pwdlife asdfasdfs} msg] $msg
        } compare_strings {1 {Supplied 'pwdlife' is a bad date value.}}

    # Cleanup steps
}

proc organization_modify_10_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_modify_10_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization modify foo -pwdalpha asdfasdfs} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc organization_modify_11_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_modify_11_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization modify foo -pwdspaces asdfasdfs} msg] $msg
    } compare_strings {1 {Invalid attribute or policy value.}}

    # Cleanup steps
}

proc organization_modify_12_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_modify_12_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization modify foo -pwdlife} msg] $msg
    } compare_strings {1 {The '-pwdlife' option requires a character string value.}}

}

proc organization_modify_13_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_modify_13_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization modify foo -pwdminlen} msg] $msg
    } compare_strings {1 {The '-pwdminlen' option requires a character string value.}}

    # Cleanup steps
}

proc organization_modify_14_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_modify_14_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization modify foo -pwdalpha} msg] $msg
    } compare_strings {1 {The '-pwdalpha' option requires a character string value.}}

    # Cleanup steps
}

proc organization_modify_15_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_modify_15_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization modify foo -pwdspaces} msg] $msg
    } compare_strings {1 {The '-pwdspaces' option requires a character string value.}}

    # Cleanup steps
}

proc organization_modify_16_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_modify_16_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization modify foo -acctlife} msg] $msg
    } compare_strings {1 {The '-acctlife' option requires a character string value.}}

    # Cleanup steps
}

proc organization_modify_17_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_modify_17_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization modify foo -pwdexpdate} msg] $msg
    } compare_strings {1 {The '-pwdexpdate' option requires a character string value.}}

    # Cleanup steps
}

proc organization_modify_18_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline organization_modify_18_N: Check that bad argument syntax is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {organization modify foo -change} msg] $msg
    } compare_strings {1 {The '-change' option requires a character string value.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 "
set ic1 "organization_modify_1_N"
set ic2 "organization_modify_2_N"
set ic3 "organization_modify_3_N"
set ic4 "organization_modify_4_N"
set ic5 "organization_modify_5_N"
set ic6 "organization_modify_6_N"
set ic7 "organization_modify_7_N"
set ic8 "organization_modify_8_N"
set ic9 "organization_modify_9_N"
set ic10 "organization_modify_10_N"
set ic11 "organization_modify_11_N"
set ic12 "organization_modify_12_N"
set ic13 "organization_modify_13_N"
set ic14 "organization_modify_14_N"
set ic15 "organization_modify_15_N"
set ic16 "organization_modify_16_N"
set ic17 "organization_modify_17_N"
set ic18 "organization_modify_18_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
