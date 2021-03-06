#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: server_enable_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:28:09  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:38  marty]
#
# Revision 1.1.4.1  1995/12/11  15:16:36  root
# 	Submit
# 	[1995/12/11  14:32:30  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:31  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:41  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:36  melman]
# 
# Revision 1.1.2.5  1994/09/20  22:12:07  pwang
# 	Removed tserver obj before creating another one [OT#10262]
# 	[1994/09/20  20:25:36  pwang]
# 
# 	Removed "wx" perm for perm IC [OT#10262]
# 	[1994/09/20  19:57:33  pwang]
# 
# Revision 1.1.2.4  1994/09/20  18:22:49  pwang
# 	Updated to latest changes [OT#10262]
# 	[1994/09/20  18:12:46  pwang]
# 
# Revision 1.1.2.3  1994/07/05  16:14:33  pwang
# 	Adjusted tests to match with the dcecp code changes [OT#11093]
# 	[1994/07/05  16:02:17  pwang]
# 
# Revision 1.1.2.2  1994/06/17  18:44:16  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:30:32  devsrc]
# 
# Revision 1.1.2.1  1994/06/10  21:39:54  pwang
# 	Initial implementation
# 	[1994/06/10  21:29:54  pwang]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc server_enable_startup {} {
    global TET_ROOT DCP_SR_ENTRYNAME DCP_SR_PROTSEQ DCP_SR_IP_ADDR

    catch {server delete tserver} dontcare
    if {[dcp_wrap "server create tserver -attr {{arguments $DCP_SR_ENTRYNAME} {program tserver} {entryname $DCP_SR_ENTRYNAME} {services {{annotation {dcecp server object test program}} {bindings $DCP_SR_PROTSEQ:$DCP_SR_IP_ADDR} {ifname {test server}} {interface 008bebed-c7c1-1ddc-9cb3-0000c0ba4944,1.0} {objects 0073f23a-2e1a-1ddd-b73a-0000c0ba4944} {entryname $DCP_SR_ENTRYNAME}}} {principals tserver} {starton explicit} {uuid 00469db2-231c-1de5-80de-0000c0ba4944} {directory $TET_ROOT/functional/admin/dcecp/ts/server}}"] != 0} {return;}
   if {[dcp_wrap {server start tserver}] != 0} {return;} 
}

proc server_enable_cleanup {} {
    if {[dcp_wrap {server stop tserver -method hard}] != 0} {return;} 
    if {[dcp_wrap {server delete tserver}] != 0} {return;} 
}

## Test purposes
proc server_enable_1_N {} {
    # Identify the test
    tet_infoline server_enable_1_N: Check that insufficient/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {server enable} msg] $msg
    } compare_strings {1 {'dced' object not specified.}}

    # Cleanup steps
}

proc server_enable_2_N {} {
    # Identify the test
    tet_infoline server_enable_2_N: Check that insufficient/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {server enable tserver} msg] $msg
    } compare_strings {1 {Required option 'interface' not entered.}}

    # Cleanup steps
}

proc server_enable_3_N {} {
    # Identify the test
    tet_infoline server_enable_3_N: Check that insufficient/invalid argument is trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {server enable tserver -interface} msg] $msg
    } compare_strings {1 {The '-interface' option requires a character string value.}}

    # Cleanup steps
}

proc server_enable_4_N {} {
    # Identify the test
    tet_infoline server_enable_4_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {server enable -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc server_enable_5_N {} {
    global DCP_CLIENT DCP_CLIENT_PW DCP_CELLNAME_THREE DCP_HOSTNAME_THREE \
           DCP_NOTROOT DCP_NOTROOT_PW

    # Identify the test
    tet_infoline server_enable_5_N: Check that unauthenticated users are handled correctly.

    # Setup steps
    catch {server disable tserver -interface {008bebed-c7c1-1ddc-9cb3-0000c0ba4944,1.0}} donotcare
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {principal create $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {group add none -member $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {account create $DCP_NOTROOT -group none -organization none -password $DCP_NOTROOT_PW -mypwd $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {login $DCP_NOTROOT -p $DCP_NOTROOT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {server enable tserver -interface {008bebed-c7c1-1ddc-9cb3-0000c0ba4944,1.0}} msg] $msg
    } compare_strings {1 {permission not valid for this acl}}

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {account delete $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc server_enable_6_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline server_enable_6_N: Check that wrong object name are returned as not found. 

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test {
	list [catch {server disable foo -interface {008bebed-c7c1-1ddc-9cb3-0000c0ba4944,1.0}} msg] $msg
    } compare_strings {1 {Cannot find a matching object.}}

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc server_enable_7_N {} {
    # Identify the test
    tet_infoline server_enable_7_N: Check that wrong interface id are returned as not found. 

    # Setup steps

    # Run the test
    test {
        list [catch {server enable tserver -interface {002972f0-44f7-1de6-817f-0000c0ba4944,1.0}} msg] $msg
    } compare_strings {1 {Cannot find a matching object.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 "
set ic1 "server_enable_1_N"
set ic2 "server_enable_2_N"
set ic3 "server_enable_3_N"
set ic4 "server_enable_4_N"
set ic5 "server_enable_5_N"
set ic6 "server_enable_6_N"
set ic7 "server_enable_7_N"
set tet_startup server_enable_startup
set tet_cleanup server_enable_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
