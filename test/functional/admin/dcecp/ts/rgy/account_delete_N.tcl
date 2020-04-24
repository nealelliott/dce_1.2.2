#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: account_delete_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:18:37  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:57  marty]
#
# Revision 1.1.4.2  1995/12/13  15:27:11  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:54:10  root]
# 
# Revision 1.1.2.1  1994/09/28  21:41:26  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:40  melman]
# 
# Revision 1.1.2.5  1994/09/01  17:49:21  kevins
# 	OT Fixed messages and some tests.
# 	[1994/09/01  16:06:34  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:14:03  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:55:01  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:38:49  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:23:15  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  17:15:35  melman
# 	updating to new API path
# 	[1994/05/06  17:13:59  melman]
# 
# Revision 1.1.2.1  1994/04/15  19:24:21  kevins
# 	CR 9938 add account functional tests
# 	[1994/04/15  15:31:19  kevins]
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
proc account_delete_1_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_delete_1_N: Check that extra arguments are trapped as errors.

    # Setup steps
        
    # Run the test
    test {
        list [catch {account delete kevin bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'bar'.}}

    # Cleanup steps
}

proc account_delete_2_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_delete_2_N: Check that missing required argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {account delete} msg] $msg
    } compare_strings {1 {No account name.}}

    # Cleanup steps
}

proc account_delete_3_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline account_delete_3_N: Check that unauthenticated users are handled correctly.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set account_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $account_name}] != 0} {return;}
    if {[dcp_wrap {group create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization create $account_name}] != 0} {return;}
    if {[dcp_wrap {organization add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {group add $account_name -member $account_name}] != 0} {return;}
    if {[dcp_wrap {account create $account_name -org $account_name \
        -group $account_name -password $account_name -mypwd $DCP_CLIENT_PW}] != 0} {return;}
     if {[dcp_wrap {logout}] != 0} {return;}


    # Run the test
    test {
        list [catch {account delete $account_name} msg] $msg
    } compare_strings {1 {User is not authorized to update record}}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal delete $account_name}] != 0} {return;}
    if {[dcp_wrap {group delete $account_name}] != 0} {return;}
    if {[dcp_wrap {organization delete $account_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 "
set ic1 "account_delete_1_N"
set ic2 "account_delete_2_N"
set ic3 "account_delete_3_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
