#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: server_modify_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:28:13  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:39  marty]
#
# Revision 1.1.4.1  1995/12/11  15:16:40  root
# 	Submit
# 	[1995/12/11  14:32:31  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:33  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:44  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:38  melman]
# 
# Revision 1.1.2.5  1994/09/20  22:12:10  pwang
# 	Removed tserver obj before creating another one [OT#10262]
# 	[1994/09/20  20:28:44  pwang]
# 
# 	Removed "w" perm for perm IC [OT#10262]
# 	[1994/09/20  19:57:52  pwang]
# 
# Revision 1.1.2.4  1994/09/20  18:22:53  pwang
# 	Updated to latest changes [OT#10262]
# 	[1994/09/20  18:13:51  pwang]
# 
# Revision 1.1.2.3  1994/07/05  16:14:38  pwang
# 	Adjusted tests to match with the dcecp code changes [OT#11093]
# 	[1994/07/05  16:03:22  pwang]
# 
# Revision 1.1.2.2  1994/06/17  18:44:21  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:30:41  devsrc]
# 
# Revision 1.1.2.1  1994/06/10  21:39:58  pwang
# 	Initial implementation
# 	[1994/06/10  21:30:42  pwang]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

## Test purposes
proc server_modify_1_N {} {
    # Identify the test
    tet_infoline server_modify_1_N: Check that insufficient arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {server modify} msg] $msg
    } compare_strings {1 {'dced' object not specified.}}

    # Cleanup steps
}

proc server_modify_2_N {} {
    # Identify the test
    tet_infoline server_modify_2_N: Check that insufficient/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {server modify tserver} msg] $msg
    } compare_strings {1 {Required option 'add, change or remove' not entered.}}

    # Cleanup steps
}

proc server_modify_3_N {} {
    # Identify the test
    tet_infoline server_modify_3_N: Check that insufficient/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {server modify tserver  -executing} msg] $msg
    } compare_strings {1 {Unrecognized option '-executing'.}}

    # Cleanup steps
}

proc server_modify_4_N {} {
    # Identify the test
    tet_infoline server_modify_4_N: Check that insufficient/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {server modify tserver -change} msg] $msg
    } compare_strings {1 {The '-change' option requires a character string value.}}

    # Cleanup steps
}

proc server_modify_5_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW DCP_SR_ENTRYNAME DCP_NOTROOT DCP_NOTROOT_PW \
           DCP_SR_PROTSEQ DCP_SR_IP_ADDR DCP_CELLNAME_THREE DCP_HOSTNAME_THREE

    # Identify the test
    tet_infoline server_modify_5_N: Check that unauthenticated users are handled correctly.

    # Setup steps
    catch {server delete tserver} dontcare
    if {[dcp_wrap "server create tserver -attr {{arguments $DCP_SR_ENTRYNAME} {program tserver} {entryname $DCP_SR_ENTRYNAME} {services {{annotation {dcecp server object test program}} {bindings $DCP_SR_PROTSEQ:$DCP_SR_IP_ADDR} {ifname {test server}} {interface 008bebed-c7c1-1ddc-9cb3-0000c0ba4944,1.0} {objects 0073f23a-2e1a-1ddd-b73a-0000c0ba4944} {entryname $DCP_SR_ENTRYNAME}}} {principals tserver} {starton explicit} {uuid 00469db2-231c-1de5-80de-0000c0ba4944} {directory $TET_ROOT/functional/admin/dcecp/ts/server}}"] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {group add none -member $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {account create $DCP_NOTROOT -group none -organization none -password $DCP_NOTROOT_PW -mypwd $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {login $DCP_NOTROOT -p $DCP_NOTROOT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {server modify tserver -add {hostdata/data line1 line2 line3}} msg] $msg
    } compare_strings {1 {permission not valid for this acl}}

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {server delete tserver}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {account delete $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc server_modify_6_N {} {
    # Identify the test
    tet_infoline server_modify_6_N: Check that wrong object name are returned as not found. 

    # Setup steps

    # Run the test
    test {
        list [catch {server modify foo -add {hostdata/data line1 line2 line3}} msg] $msg
    } compare_strings {1 {Cannot find a matching object.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 "
set ic1 "server_modify_1_N"
set ic2 "server_modify_2_N"
set ic3 "server_modify_3_N"
set ic4 "server_modify_4_N"
set ic5 "server_modify_5_N"
set ic6 "server_modify_6_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
