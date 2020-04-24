#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: keytab_add_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:16:33  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:05  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:23  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/06/08  14:06 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts33/1  1995/06/08  14:05 UTC  truitt
# 	CHFts15424: Change some tests to support new dced acl behavior.
# 
# 	HP revision /main/HPDCE02/1  1994/11/10  14:25 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/10  14:23 UTC  truitt
# 	Get rid of extra logout from testcase 9.
# 	[1995/12/11  15:53:26  root]
# 
# Revision 1.1.2.2  1994/09/29  22:01:32  baum
# 	Fixed negative messages (OT 12370)
# 	[1994/09/29  22:01:06  baum]
# 
# Revision 1.1.2.1  1994/09/28  21:40:19  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:32  melman]
# 
# Revision 1.1.2.7  1994/09/01  15:44:02  baum
# 	Fixed output messages in negative tests (OT 11093)
# 	[1994/09/01  15:42:35  baum]
# 
# Revision 1.1.2.6  1994/06/10  20:13:12  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:22  devsrc]
# 
# Revision 1.1.2.5  1994/06/07  23:13:47  baum
# 	Mods to handle new dcecp output (CR #10761)
# 	[1994/06/07  23:13:18  baum]
# 
# Revision 1.1.2.4  1994/05/25  22:42:50  baum
# 	Keytab tests now dce_login/out on their own.  Also, corrected some of the output errors
# 	[1994/05/25  22:41:31  baum]
# 
# Revision 1.1.2.3  1994/05/19  23:06:21  baum
# 	Resync with changes made to commands and command output
# 	[1994/05/19  23:05:49  baum]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_setup.tcl

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


## Test purposes
proc keytab_add_1_N {} {

    # Identify the test
    tet_infoline keytab_add_1_N: Check that insufficient/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab add} msg] $msg
    } compare_strings {1 {'dced' object not specified.}}

    # Cleanup steps
}

proc keytab_add_2_N {} {

    # Identify the test
    tet_infoline keytab_add_2_N: Check that insufficient/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab add name} msg] $msg
    } compare_strings {1 {Required option 'member' not entered.}}

    # Cleanup steps
}

proc keytab_add_3_N {} {

    # Identify the test
    tet_infoline keytab_add_3_N: Check that insufficient arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab add name -member} msg] $msg
    } compare_strings {1 {The '-member' option requires a character string value.}}

    # Cleanup steps
}

proc keytab_add_4_N {} {

    # Identify the test
    tet_infoline keytab_add_4_N: Check that insufficient/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab add name -member member} msg] $msg
    } compare_strings {1 {Required option 'key or random' not entered.}}

    # Cleanup steps
}

proc keytab_add_5_N {} {

    # Identify the test
    tet_infoline keytab_add_5_N: Check that insufficient/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab add name -member member -key} msg] $msg
    } compare_strings {1 {The '-key' option requires a character string value.}}

    # Cleanup steps
}

proc keytab_add_6_N {} {

    # Identify the test
    tet_infoline keytab_add_6_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab add name -member member -random key} msg] $msg
    } compare_strings {1 {Too many arguments specified in command.}}

    # Cleanup steps
}

proc keytab_add_7_N {} {

    # Identify the test
    tet_infoline keytab_add_7_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab add name -member member -random -registry foo} msg] $msg
    } compare_strings {1 {Too many arguments specified in command.}}

    # Cleanup steps
}

proc keytab_add_8_N {} {

    # Identify the test
    tet_infoline keytab_add_8_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab add name -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc keytab_add_9_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline keytab_add_9_N: Check that unauthenticated users are handled correctly.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {keytab add ktb2 -member test_princ1 -random -registry} msg] $msg
    } compare_strings {1 {permission not valid for this acl}}

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc keytab_add_10_N {} {

    # Identify the test
    tet_infoline keytab_add_10_N: Check that insufficient/invalid are returned as error. 

    # Setup steps

    # Run the test
    test {
        list [catch {keytab add foo -member member -random} msg] $msg
    } compare_strings {1 {Required option 'registry' not entered.}}

    # Cleanup steps
}

proc keytab_add_11_N {} {

    # Identify the test
    tet_infoline keytab_add_11_N: Check that wrong object name are returned as not found. 

    # Setup steps

    # Run the test
    test {
        list [catch {keytab add foo -member member -random -registry} msg] $msg
    } compare_strings {1 {Cannot find a matching object.}}

    # Cleanup steps
}

proc keytab_add_12_N {} {

    # Identify the test
    tet_infoline keytab_add_12_N: Check that insufficient/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab add name -member member -key} msg] $msg
    } compare_strings {1 {The '-key' option requires a character string value.}}

    # Cleanup steps
}

proc keytab_add_13_N {} {

    # Identify the test
    tet_infoline keytab_add_13_N: Check that wrong principal name are returned as not found. 

    # Setup steps

    # Run the test
    test {
        list [catch {keytab add name -member foo -key plain_key -version 1} msg] $msg
    } compare_strings {1 {Cannot find a matching object.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 "
set ic1 "keytab_add_1_N"
set ic2 "keytab_add_2_N"
set ic3 "keytab_add_3_N"
set ic4 "keytab_add_4_N"
set ic5 "keytab_add_5_N"
set ic6 "keytab_add_6_N"
set ic7 "keytab_add_7_N"
set ic8 "keytab_add_8_N"
set ic9 "keytab_add_9_N"
set ic10 "keytab_add_10_N"
set ic11 "keytab_add_11_N"
set ic12 "keytab_add_12_N"
set ic13 "keytab_add_13_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
