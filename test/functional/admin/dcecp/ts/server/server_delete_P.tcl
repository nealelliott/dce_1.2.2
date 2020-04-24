#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: server_delete_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:27:58  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:34  marty]
#
# Revision 1.1.4.1  1995/12/11  15:16:28  root
# 	Submit
# 	[1995/12/11  14:32:27  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:27  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:35  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:32  melman]
# 
# Revision 1.1.2.5  1994/09/20  22:12:02  pwang
# 	Removed tserver obj before and after test [OT#10262]
# 	[1994/09/20  20:22:22  pwang]
# 
# Revision 1.1.2.4  1994/09/20  18:22:43  pwang
# 	Updated to latest changes [OT#10262]
# 	[1994/09/20  18:11:25  pwang]
# 
# Revision 1.1.2.3  1994/07/05  16:14:42  pwang
# 	Adjusted tests to match with the dcecp code changes [OT#11093]
# 	[1994/07/05  15:56:40  pwang]
# 
# Revision 1.1.2.2  1994/06/17  18:44:12  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:30:20  devsrc]
# 
# Revision 1.1.2.1  1994/06/10  21:39:48  pwang
# 	Intial implementation
# 	[1994/06/10  21:28:21  pwang]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc server_delete_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    catch {server delete tserver} dontcare
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc server_delete_cleanup {} {
    catch {server delete tserver} dontcare
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc server_delete_5_P {} {
    global TET_ROOT DCP_SR_ENTRYNAME DCP_SR_PROTSEQ DCP_SR_IP_ADDR

    # Identify the test
    tet_infoline server_delete_5_P: Check to see that we can delete  server objects.

    # Setup steps
    if {[dcp_wrap "server create tserver -attr {{arguments $DCP_SR_ENTRYNAME} {program tserver} {entryname $DCP_SR_ENTRYNAME} {services {{annotation {dcecp server object test program}} {bindings $DCP_SR_PROTSEQ:$DCP_SR_IP_ADDR} {ifname {test server}} {interface 008bebed-c7c1-1ddc-9cb3-0000c0ba4944,1.0} {objects 0073f23a-2e1a-1ddd-b73a-0000c0ba4944} {entryname $DCP_SR_ENTRYNAME}}} {principals tserver} {starton explicit} {uuid 00469db2-231c-1de5-80de-0000c0ba4944} {directory $TET_ROOT/functional/admin/dcecp/ts/server}}"] != 0} {return;}

    # Run the test
    test {
	list [catch {server delete tserver} msg] $msg
    } compare_strings {0 {}}

    # Cleanup steps
}

set iclist "ic1 "
set ic1 "server_delete_5_P"
set tet_startup server_delete_startup
set tet_cleanup server_delete_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
