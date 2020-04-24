#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: server_modify_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:28:16  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:40  marty]
#
# Revision 1.1.4.1  1995/12/11  15:16:42  root
# 	Submit
# 	[1995/12/11  14:32:32  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:34  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:45  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:39  melman]
# 
# Revision 1.1.2.5  1994/09/20  22:12:11  pwang
# 	Removed tserver obj before creating another one [OT#10262]
# 	[1994/09/20  20:29:58  pwang]
# 
# Revision 1.1.2.4  1994/09/20  18:22:55  pwang
# 	Updated to latest changes [OT#10262]
# 	[1994/09/20  18:14:18  pwang]
# 
# Revision 1.1.2.3  1994/07/05  16:14:40  pwang
# 	Adjusted tests to match with the dcecp code changes [OT#11093]
# 	[1994/07/05  16:03:51  pwang]
# 
# Revision 1.1.2.2  1994/06/17  18:44:22  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:30:45  devsrc]
# 
# Revision 1.1.2.1  1994/06/10  21:40:01  pwang
# 	Initial implementation
# 	[1994/06/10  21:31:13  pwang]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc server_modify_startup {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW DCP_SR_ENTRYNAME DCP_SR_PROTSEQ DCP_SR_IP_ADDR

    catch {server delete tserver} dontcare
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap "server create tserver -attr {{arguments $DCP_SR_ENTRYNAME} {program tserver} {entryname $DCP_SR_ENTRYNAME} {services {{annotation {dcecp server object test program}} {bindings $DCP_SR_PROTSEQ:$DCP_SR_IP_ADDR} {ifname {test server}} {interface 008bebed-c7c1-1ddc-9cb3-0000c0ba4944,1.0} {objects 0073f23a-2e1a-1ddd-b73a-0000c0ba4944} {entryname $DCP_SR_ENTRYNAME}}} {principals tserver} {starton explicit} {uuid 00469db2-231c-1de5-80de-0000c0ba4944} {directory $TET_ROOT/functional/admin/dcecp/ts/server}}"] != 0} {return;}
}

proc server_modify_cleanup {} {
    if {[dcp_wrap {server delete tserver}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc server_modify_7_P {} {
    # Identify the test
    tet_infoline server_modify_7_P: Check to see that we can add era attr to server objects.

    # Setup steps

    # Run the test
    test {
	list [catch {server modify tserver -add {hostdata/data line}} msg] $msg
    } compare_strings {0 {}}

    test {
	dcp_get_element [server show tserver] hostdata/data 
    } compare_strings {line}

    # Cleanup steps
}

proc server_modify_8_P {} {
    # Identify the test
    tet_infoline server_modify_8_P: Check that we can replace era attrs with new ones on a server object.

    # Setup steps

    # Run the test
    test {
	list [catch {server modify tserver -change {hostdata/data line1 line2}} msg] $msg
    } compare_strings {0 {}}

    test {
	dcp_get_element [server show tserver] hostdata/data 
    } compare_strings {line1}

    # Cleanup steps
}

proc server_modify_9_P {} {
    # Identify the test
    tet_infoline server_modify_9_P: Check that we can remove an era attr value from server oject.

    # Setup steps

    # Run the test
    test {
	list [catch {server modify tserver -remove {hostdata/data line1}} msg] $msg
    } compare_strings {0 {}}

    test {
	dcp_get_element [server show tserver] hostdata/data 
    } compare_strings {line2}

    # Cleanup steps
}

proc server_modify_10_P {} {
    # Identify the test
    tet_infoline server_modify_10_P: Check to see that we can remove the era attr from server oject by -types.

    # Setup steps

    # Run the test
    test {
	list [catch {server modify tserver -remove hostdata/data -types} msg] $msg
    } compare_strings {0 {}}

    test {
	dcp_get_element [server show tserver] hostdata/data 
    } compare_strings {}

    # Cleanup steps
}

set iclist "ic1 ic2 ic3 ic4 "
set ic1 "server_modify_7_P"
set ic2 "server_modify_8_P"
set ic3 "server_modify_9_P"
set ic4 "server_modify_10_P"
set tet_startup server_modify_startup
set tet_cleanup server_modify_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
