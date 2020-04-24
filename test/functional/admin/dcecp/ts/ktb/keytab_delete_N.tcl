#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: keytab_delete_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:16:48  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:10  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:28  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/06/08  14:02 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts33/1  1995/06/08  14:01 UTC  truitt
# 	CHFts15424: Change some tests to support new dced acl behavior.
# 
# 	HP revision /main/HPDCE02/1  1994/11/10  14:23 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/10  14:22 UTC  truitt
# 	Correct testcase 5.
# 	[1995/12/11  15:53:31  root]
# 
# Revision 1.1.2.2  1994/09/29  22:01:35  baum
# 	Fixed negative messages (OT 12370)
# 	[1994/09/29  22:01:08  baum]
# 
# Revision 1.1.2.1  1994/09/28  21:40:27  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:39  melman]
# 
# Revision 1.1.2.5  1994/06/10  20:13:17  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:28  devsrc]
# 
# Revision 1.1.2.4  1994/05/25  22:42:58  baum
# 	Keytab tests now dce_login/out on their own.  Also, corrected some of the output errors
# 	[1994/05/25  22:41:37  baum]
# 
# Revision 1.1.2.3  1994/05/19  23:06:24  baum
# 	Resync with changes made to commands and command output
# 	[1994/05/19  23:05:51  baum]
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
proc keytab_delete_1_N {} {

    # Identify the test
    tet_infoline keytab_delete_1_N: Check insufficient/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab delete} msg] $msg
    } compare_strings {1 {'dced' object not specified.}}

    # Cleanup steps
}

proc keytab_delete_2_N {} {

    # Identify the test
    tet_infoline keytab_delete_2_N: Check extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab delete name -entry foo} msg] $msg
    } compare_strings {1 {Too many arguments specified in command.}}

    # Cleanup steps
}

proc keytab_delete_3_N {} {

    # Identify the test
    tet_infoline keytab_delete_3_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab delete name -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc keytab_delete_4_N {} {

    # Identify the test
    tet_infoline keytab_delete_4_N: Check that wrong object name are returned as not found. 

    # Setup steps

    # Run the test
    test {
        list [catch {keytab delete foo} msg] $msg
    } compare_strings {1 {Cannot find a matching object.}}

    # Cleanup steps
}

proc keytab_delete_5_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline keytab_delete_5_N: Check that no permissions are handled correctly. 

    # Setup steps
    dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}

    # Run the test
    test {
        list [catch {keytab delete foo} msg] $msg
    } compare_strings {1 {permission not valid for this acl}}

    # Cleanup steps
    dcp_wrap {logout}
}

set iclist "ic1 ic2 ic3 ic4 ic5 "
set ic1 "keytab_delete_1_N"
set ic2 "keytab_delete_2_N"
set ic3 "keytab_delete_3_N"
set ic4 "keytab_delete_4_N"
set ic5 "keytab_delete_5_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
