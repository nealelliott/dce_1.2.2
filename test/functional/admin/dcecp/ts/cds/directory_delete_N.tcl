#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: directory_delete_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:13:25  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:29:23  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:18  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/03/03  12:54 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts17/1  1995/03/03  12:53 UTC  truitt
# 	Fix error message when using -tree option.
# 	[1995/12/11  15:52:30  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:45  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:07:40  melman]
# 
# Revision 1.1.2.6  1994/09/02  13:56:42  kevins
# 	OT 10617 modified messages and some tests.
# 	[1994/09/02  10:10:30  kevins]
# 
# Revision 1.1.2.5  1994/06/10  20:11:13  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:21  devsrc]
# 
# Revision 1.1.2.4  1994/05/19  14:10:50  coren
# 	Merged with changes from 1.1.2.3
# 	[1994/05/19  14:10:37  coren]
# 
# 	Merged up.
# 	[1994/05/17  20:59:35  coren]
# 
# 	Added -tree test (directory_delete_7_P).
# 	[1994/05/10  13:55:57  coren]
# 
# Revision 1.1.2.3  1994/05/11  19:35:52  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:18:09  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:12  melman
# 	updating to new API path
# 	[1994/05/06  15:19:58  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:20:44  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:36  kevins]
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
proc directory_delete_1_N {} {
    # Identify the test
    tet_infoline directory_delete_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {directory delete /.:/test_1 foo} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo'.}}

    # Cleanup steps
}

proc directory_delete_2_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_delete_2_N: Check that unauthenticated users are handled correctly.

    # Setup steps
    if {[dcp_wrap {set dir_name [lindex [info level [info level]] 0]}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory create /.:/$dir_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test {
        list [catch {directory delete /.:/$dir_name} msg] $msg
    } compare_strings {1 {Insufficient rights to perform requested operation}}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory delete /.:/$dir_name}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc directory_create_3_N {} {
    # Identify the test
    tet_infoline directory_delete_3_N: Check that missing arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {directory delete} msg] $msg
    } compare_strings {1 {No directory name.}}

    # Cleanup steps

}

proc directory_delete_7_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_delete_7_N: Check to see that use of -replica and -clearinghouse
    tet_infoline directory_delete_7_N: with -tree is detected as an error.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory create /.:/dirde1}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test {
        list [catch {directory delete /.:/dirde1 -tree -replica -clearinghouse /.:/test_ch} msg] $msg
    } compare_strings {1 {The '-tree' option cannot be used with the '-replica' and '-clearinghouse' options.}}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW              }] != 0} {return;}
    if {[dcp_wrap {directory delete /.:/dirde1}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc directory_delete_8_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_delete_8_N: Check to see that use of -replica and -clearinghouse
    tet_infoline directory_delete_8_N: with -force is detected as an error.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory create /.:/dirde2}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test {
        list [catch {directory delete /.:/dirde2 -force -replica -clearinghouse /.:/test_ch} msg] $msg
    } compare_strings {1 {The '-force' option cannot be used with the '-replica' and '-clearinghouse' options.}}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW              }] != 0} {return;}
    if {[dcp_wrap {directory delete /.:/dirde2}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc directory_delete_9_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_delete_9_N: A directory with children cannot be deleted.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {directory create /.:/dirde3}] != 0} {return;}
    if {[dcp_wrap {directory create /.:/dirde3/dirde3_child}] != 0} {return;}

    # Run the test
    test {
        list [catch {directory delete /.:/dirde3} msg] $msg
    } compare_strings {1 {Directory must be empty to be deleted}}

    # Cleanup steps
    if {[dcp_wrap {directory delete /.:/dirde3 -tree}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc directory_delete_10_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_delete_10_N: A directory with replicas cannot be deleted.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {clearinghouse create /.:/dirdel10_ch}] != 0} {return;}
    if {[dcp_wrap {directory create /.:/dirde4}] != 0} {return;}
    if {[dcp_wrap {directory synchronize /.:}] != 0} {return;}
    if {[dcp_wrap {directory create /.:/dirde4 -replica -clearinghouse /.:/dirdel10_ch}] != 0} {return;}
    if {[dcp_wrap {directory synchronize /.:}] != 0} {return;}

    # Run the test
    test {
        list [catch {directory delete /.:/dirde4} msg] $msg
    } compare_strings {1 {Replica set of specified directory contains more than one replica}}

    # Cleanup steps
    if {[dcp_wrap {directory delete /.:/dirde4 -force}] != 0} {return;}
    if {[dcp_wrap {directory synchronize /.:}] != 0} {return;}
    if {[dcp_wrap {clearinghouse delete /.:/dirdel10_ch}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 "
set ic1 "directory_delete_1_N"
set ic2 "directory_delete_2_N"
set ic3 "directory_create_3_N"
set ic4 "directory_delete_7_N"
set ic5 "directory_delete_8_N"
set ic6 "directory_delete_9_N"
set ic7 "directory_delete_10_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
