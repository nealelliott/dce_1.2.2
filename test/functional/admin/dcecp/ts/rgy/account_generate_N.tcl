#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: account_generate_N.tcl,v $
# Revision 1.1.6.4  1996/03/11  02:18:42  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:59  marty]
#
# Revision 1.1.6.3  1995/12/13  15:27:13  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/03/09  15:29 UTC  truitt
# 	Location of pwd_mgmt object changed.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts19/1  1995/03/09  15:28 UTC  truitt
# 	Location of pwd_mgmt handle changed.
# 
# 	HP revision /main/HPDCE02/1  1994/11/04  14:44 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/04  14:40 UTC  truitt
# 	Correct account name for test 6_N.
# 	[1995/12/11  15:54:11  root]
# 
# Revision 1.1.2.1  1994/09/28  21:41:28  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:43  melman]
# 
# Revision 1.1.2.1  1994/09/16  18:21:05  kevins
# 	OT 12199 Initial Submission
# 	[1994/09/16  18:20:17  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc account_generate_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc account_generate_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc account_generate_1_N {} {
    # Identify the test
    tet_infoline account_generate_1_N: Check that unauthenticated users are handled correctly.

    # Setup steps

    # Run the test
    test {
        list [catch {account generate cell_admin} msg] $msg
    } compare_strings {1 {Password may not be randomly generated.}}

    # Cleanup steps
}

proc account_generate_2_N {} {
    # Identify the test
    tet_infoline account_generate_2_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {account generate foo -bar} msg] $msg
    } compare_strings {1 {Unrecognized option '-bar'.}}

    # Cleanup steps
}

proc account_generate_3_N {} {
    # Identify the test
    tet_infoline account_generate_3_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {account generate foo bar} msg] $msg
    } compare_strings {1 {Unrecognized argument 'bar'.}}

    # Cleanup steps
}

proc account_generate_4_N {} {
    # Identify the test
    tet_infoline account_generate_4_N: Check that inappropriate pwd_val_types are handled correctly.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL -attribute {{pwd_val_type 0} {pwd_mgmt_binding {{dce /.:/pwd_strength pktprivacy secret name} {/.:/subsys/dce/pwd_mgmt/pwd_strength}}}}}] != 0} {return;}

    # Run the test
    test {
        list [catch {account generate $PRINCIPAL} msg] $msg
    } compare_strings {1 {Password may not be randomly generated.}}

    # Cleanup steps
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_generate_5_N {} {
    # Identify the test
    tet_infoline account_generate_5_N: Check that inappropriate pwd_val_types are handled correctly.

    # Setup steps
    if {[dcp_wrap {set PRINCIPAL [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $PRINCIPAL -attribute {{pwd_val_type 1} {pwd_mgmt_binding {{dce /.:/pwd_strength pktprivacy secret name} {/.:/subsys/dce/pwd_mgmt/pwd_strength}}}}}] != 0} {return;}

    # Run the test
    test {
        list [catch {account generate $PRINCIPAL} msg] $msg
    } compare_strings {1 {Password may not be randomly generated.}}

    # Cleanup steps
    if {[dcp_wrap {principal delete $PRINCIPAL}] != 0} {return;}
}

proc account_generate_6_N {} {
    # Identify the test
    tet_infoline account_generate_6_N: Check to see that account cannot be 
    tet_infoline account_generate_6_N: generated when password generation is required.

    # Setup steps
    global DCP_NOTROOT DCP_NOTROOT_PW DCP_CLIENT_PW
    if {[dcp_wrap {principal create $DCP_NOTROOT -attribute {{pwd_val_type 3} {pwd_mgmt_binding {{dce /.:/pwd_strength pktprivacy secret name} {/.:/subsys/dce/pwd_mgmt/pwd_strength}}}}}] != 0} {return;}
    if {[dcp_wrap {group add none -member $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $DCP_NOTROOT}] != 0} {return;}

    # Run the test
    test {
        list [catch {account create $DCP_NOTROOT -group none -org none -password $DCP_NOTROOT_PW -mypwd $DCP_CLIENT_PW} msg] $msg
    } compare_strings {1 {password failed password strength server checking policy}}

    # Cleanup steps
    if {[dcp_wrap {principal delete $DCP_NOTROOT}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 "
set ic1 "account_generate_1_N"
set ic2 "account_generate_2_N"
set ic3 "account_generate_3_N"
set ic4 "account_generate_4_N"
set ic5 "account_generate_5_N"
set ic6 "account_generate_6_N"
set tet_startup account_generate_startup
set tet_cleanup account_generate_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
