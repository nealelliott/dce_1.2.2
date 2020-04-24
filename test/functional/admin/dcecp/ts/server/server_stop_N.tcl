#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: server_stop_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:28:41  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:51  marty]
#
# Revision 1.1.4.1  1995/12/11  15:17:04  root
# 	Submit
# 	[1995/12/11  14:32:40  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:45  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:56  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:50  melman]
# 
# Revision 1.1.2.5  1994/09/20  22:12:20  pwang
# 	Remove tserver obj before creating another one [OT#10262]
# 	[1994/09/20  20:38:14  pwang]
# 
# 	Removed "wx" perm for perm IC [OT#10262]
# 	[1994/09/20  19:59:01  pwang]
# 
# Revision 1.1.2.4  1994/09/20  18:23:18  pwang
# 	Updated to latest changes [OT#10262]
# 	[1994/09/20  18:17:29  pwang]
# 
# Revision 1.1.2.3  1994/07/05  16:14:36  pwang
# 	Adjusted tests to match with the dcecp code changes [OT#11093]
# 	[1994/07/05  16:06:07  pwang]
# 
# Revision 1.1.2.2  1994/06/17  18:44:32  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:31:21  devsrc]
# 
# Revision 1.1.2.1  1994/06/10  21:40:25  pwang
# 	Initial implementation
# 	[1994/06/10  21:34:35  pwang]
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
proc server_stop_1_N {} {
    # Identify the test
    tet_infoline server_stop_1_N: Check that insufficient/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {server stop} msg] $msg
    } compare_strings {1 {'dced' object not specified.}}

    # Cleanup steps
}

proc server_stop_2_N {} {
    # Identify the test
    tet_infoline server_stop_2_N: Check that insufficient/invalid argument is trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {server stop name -method} msg] $msg
    } compare_strings {1 {The '-method' option requires a character string value.}}

    # Cleanup steps
}

proc server_stop_3_N {} {
    # Identify the test
    tet_infoline server_stop_3_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {server stop name -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc server_stop_4_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW DCP_SR_ENTRYNAME DCP_NOTROOT DCP_NOTROOT_PW \
           DCP_SR_PROTSEQ DCP_SR_IP_ADDR DCP_CELLNAME_THREE DCP_HOSTNAME_THREE

    # Identify the test
    tet_infoline server_stop_4_N: Check that unauthenticated users are handled correctly.

    # Setup steps
    catch {server delete tserver} dontcare
    if {[dcp_wrap "server create tserver -attr {{arguments $DCP_SR_ENTRYNAME} {program tserver} {entryname $DCP_SR_ENTRYNAME} {services {{annotation {dcecp server object test program}} {bindings $DCP_SR_PROTSEQ:$DCP_SR_IP_ADDR} {ifname {test server}} {interface 008bebed-c7c1-1ddc-9cb3-0000c0ba4944,1.0} {objects 0073f23a-2e1a-1ddd-b73a-0000c0ba4944} {entryname $DCP_SR_ENTRYNAME}}} {principals tserver} {starton explicit} {uuid 00469db2-231c-1de5-80de-0000c0ba4944} {directory $TET_ROOT/functional/admin/dcecp/ts/server}}"] != 0} {return;}
    if {[dcp_wrap {server start tserver}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {group add none -member $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {account create $DCP_NOTROOT -group none -organization none -password $DCP_NOTROOT_PW -mypwd $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {login $DCP_NOTROOT -p $DCP_NOTROOT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {server stop tserver} msg] $msg
    } compare_strings {1 {permission not valid for this acl}}

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {return;}
    catch {server stop tserver -method hard} dontcare
    if {[dcp_wrap {server delete tserver}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {account delete $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc server_stop_5_N {} {
    # Identify the test
    tet_infoline server_stop_5_N: Check that wrong object name are returned as not found. 

    # Setup steps

    # Run the test
    test {
        list [catch {server stop foo} msg] $msg
    } compare_strings {1 {Cannot find a matching object.}}

    # Cleanup steps
}

proc server_stop_6_N {} {
    # Identify the test
    tet_infoline server_stop_6_N: Check that wrong method name are returned as bad method. 

    # Setup steps

    # Run the test
    test {
        list [catch {server stop name -method foo_method} msg] $msg
    } compare_strings {1 {Undefined stop method flag has been encountered.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 "
set ic1 "server_stop_1_N"
set ic2 "server_stop_2_N"
set ic3 "server_stop_3_N"
set ic4 "server_stop_4_N"
set ic5 "server_stop_5_N"
set ic6 "server_stop_6_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
