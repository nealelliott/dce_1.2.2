#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: keytab_show_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:17:02  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:15  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:34  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/06/08  13:49 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts33/1  1995/06/08  13:48 UTC  truitt
# 	CHFts15424: Change some tests to support new dced acl behavior.
# 	[1995/12/11  15:53:37  root]
# 
# Revision 1.1.2.2  1994/09/29  22:01:38  baum
# 	Fixed negative messages (OT 12370)
# 	[1994/09/29  22:01:11  baum]
# 
# Revision 1.1.2.1  1994/09/28  21:40:36  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:50  melman]
# 
# Revision 1.1.2.5  1994/06/10  20:13:23  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:40  devsrc]
# 
# Revision 1.1.2.4  1994/05/25  22:43:03  baum
# 	Keytab tests now dce_login/out on their own.  Also, corrected some of the output errors
# 	[1994/05/25  22:41:42  baum]
# 
# Revision 1.1.2.3  1994/05/19  23:06:27  baum
# 	Resync with changes made to commands and command output
# 	[1994/05/19  23:05:54  baum]
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
proc keytab_show_1_N {} {

    # Identify the test
    tet_infoline keytab_show_1_N: Check insufficient/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab show} msg] $msg
    } compare_strings {1 {'dced' object not specified.}}

    # Cleanup steps
}

proc keytab_show_2_N {} {

    # Identify the test
    tet_infoline keytab_show_2_N: Check extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab show name -entry foo} msg] $msg
    } compare_strings {1 {Too many arguments specified in command.}}

    # Cleanup steps
}

proc keytab_show_3_N {} {

    # Identify the test
    tet_infoline keytab_show_3_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab show name -members foo} msg] $msg
    } compare_strings {1 {Too many arguments specified in command.}}

    # Cleanup steps
}

proc keytab_show_4_N {} {

    # Identify the test
    tet_infoline keytab_show_4_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab show name -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc keytab_show_5_N {} {

    # Identify the test
    tet_infoline keytab_show_5_N: Check that wrong object name are returned as not found. 

    # Setup steps

    # Run the test
    test {
        list [catch {keytab show foo} msg] $msg
    } compare_strings {1 {Cannot find a matching object.}}

    # Cleanup steps
}

proc keytab_show_6_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline keytab_show_6_N: Check that invalid permissions are trapped as errors. 

    # Setup steps
    dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}

    # Run the test
    test {
        list [catch {keytab show foo} msg] $msg
    } compare_strings {1 {permission not valid for this acl}}

    # Cleanup steps
    dcp_wrap {logout}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 "
set ic1 "keytab_show_1_N"
set ic2 "keytab_show_2_N"
set ic3 "keytab_show_3_N"
set ic4 "keytab_show_4_N"
set ic5 "keytab_show_5_N"
set ic6 "keytab_show_6_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
