#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: server_disable_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:28:06  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:37  marty]
#
# Revision 1.1.4.1  1995/12/11  15:16:34  root
# 	Submit
# 	[1995/12/11  14:32:28  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:30  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:39  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:35  melman]
# 
# Revision 1.1.2.5  1994/09/20  22:12:06  pwang
# 	Removed tserver obj before creating another one[OT#10262]
# 	[1994/09/20  20:24:11  pwang]
# 
# Revision 1.1.2.4  1994/09/20  18:22:47  pwang
# 	Updated to latest changes [OT#10262]
# 	[1994/09/20  18:12:22  pwang]
# 
# Revision 1.1.2.3  1994/07/05  16:14:30  pwang
# 	Adjusted tests to match with the dcecp code changes [OT#11093]
# 	[1994/07/05  16:01:28  pwang]
# 
# Revision 1.1.2.2  1994/06/17  18:44:14  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:30:28  devsrc]
# 
# Revision 1.1.2.1  1994/06/10  21:39:52  pwang
# 	Intial implementation
# 	[1994/06/10  21:29:12  pwang]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc server_disable_startup {} {
    global TET_ROOT DCP_SR_ENTRYNAME DCP_SR_PROTSEQ DCP_SR_IP_ADDR

    catch {server delete tserver} dontcare
    if {[dcp_wrap "server create tserver -attr {{arguments $DCP_SR_ENTRYNAME} {program tserver} {entryname $DCP_SR_ENTRYNAME} {services {{annotation {dcecp server object test program}} {bindings $DCP_SR_PROTSEQ:$DCP_SR_IP_ADDR} {ifname {test server}} {interface 008bebed-c7c1-1ddc-9cb3-0000c0ba4944,1.0} {objects 0073f23a-2e1a-1ddd-b73a-0000c0ba4944} {entryname $DCP_SR_ENTRYNAME}}} {principals tserver} {starton explicit} {uuid 00469db2-231c-1de5-80de-0000c0ba4944} {directory $TET_ROOT/functional/admin/dcecp/ts/server}}"] != 0} {return;}
   if {[dcp_wrap {server start tserver}] != 0} {return;}
   exec sleep 5
}

proc server_disable_cleanup {} {
    if {[dcp_wrap {server stop tserver -method hard}] != 0} {return;}
    if {[dcp_wrap {server delete tserver}] != 0} {return;}
}

## Test purposes
proc server_disable_8_P {} {
    global DCP_SR_ENTRYNAME 

    # Identify the test
    tet_infoline server_disable_8_P: Check to see that we can disable server interfaces.

    # Setup steps

    # Run the test
    test {
        list [catch {server disable tserver -interface {008bebed-c7c1-1ddc-9cb3-0000c0ba4944,1.0}} msg] $msg
    } compare_strings {0 {}}
    exec sleep 5

    test {
	list [catch {server ping $DCP_SR_ENTRYNAME} msg] $msg
    } compare_strings {0 0}

    # Cleanup steps
    catch {server enable tserver -interface {008bebed-c7c1-1ddc-9cb3-0000c0ba4944,1.0}} dontcare
    exec sleep 5
}


set iclist "ic1 "
set ic1 "server_disable_8_P"
set tet_startup server_disable_startup
set tet_cleanup server_disable_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
