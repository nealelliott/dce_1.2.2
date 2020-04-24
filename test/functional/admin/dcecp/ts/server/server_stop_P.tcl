#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: server_stop_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:28:43  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:52  marty]
#
# Revision 1.1.4.1  1995/12/11  15:17:07  root
# 	Submit
# 	[1995/12/11  14:32:41  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:46  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:58  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:51  melman]
# 
# Revision 1.1.2.5  1994/09/20  22:12:22  pwang
# 	Used match_string.
# 	[1994/09/20  22:11:38  pwang]
# 
# Revision 1.1.2.4  1994/09/20  18:23:20  pwang
# 	Updated to latest changes [OT#10262]
# 	[1994/09/20  18:17:53  pwang]
# 
# Revision 1.1.2.3  1994/07/05  16:14:39  pwang
# 	Adjusted tests to match with the dcecp code changes [OT#11093]
# 	[1994/07/05  16:06:44  pwang]
# 
# Revision 1.1.2.2  1994/06/17  18:44:32  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:31:25  devsrc]
# 
# Revision 1.1.2.1  1994/06/10  21:40:27  pwang
# 	Initial implementation
# 	[1994/06/10  21:34:54  pwang]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc server_stop_startup {} {
    global TET_ROOT DCP_SR_ENTRYNAME DCP_SR_PROTSEQ DCP_SR_IP_ADDR

    catch {server delete tserver} dontcare
    if {[dcp_wrap "server create tserver -attr {{arguments $DCP_SR_ENTRYNAME} {program tserver} {entryname $DCP_SR_ENTRYNAME} {services {{annotation {dcecp server object test program}} {bindings $DCP_SR_PROTSEQ:$DCP_SR_IP_ADDR} {ifname {test server}} {interface 008bebed-c7c1-1ddc-9cb3-0000c0ba4944,1.0} {objects 0073f23a-2e1a-1ddd-b73a-0000c0ba4944} {entryname $DCP_SR_ENTRYNAME}}} {principals tserver} {starton explicit} {uuid 00469db2-231c-1de5-80de-0000c0ba4944} {directory $TET_ROOT/functional/admin/dcecp/ts/server}}"] != 0} {return;}
}

proc server_stop_cleanup {} {
    if {[dcp_wrap {server delete tserver}] != 0} {return;}
}

## Test purposes
proc server_stop_7_P {} {
    global DCP_SR_PROTSEQ DCP_SR_IP_ADDR

    # Identify the test
    tet_infoline server_stop_7_P: Check to see that we can stop server by method "rpc"

    # Setup steps
    catch {server stop tserver -method hard} dontcare 
    if {[dcp_wrap {server start tserver}] != 0} {return;}

    # Sleep to allow server to register its endpoints
    exec sleep 10

    # Run the test
    test {
	list [catch {server stop tserver -method rpc} msg] $msg
    } compare_strings {0 {}}

    test {
	list [catch {server ping 0073f23a-2e1a-1ddd-b73a-0000c0ba4944@$DCP_SR_PROTSEQ:$DCP_SR_IP_ADDR} msg] $msg
    } compare_strings {0 0}

    # Cleanup steps
    if {[dcp_wrap {server stop tserver -method hard}] != 0} {return;} 
}

proc server_stop_8_P {} {
    # Identify the test
    tet_infoline server_stop_8_P: Check to see that we can stop server by method "soft"

    # Setup steps
    catch {server stop tserver -method hard} dontcare 
    if {[dcp_wrap {server start tserver}] != 0} {return;}

    # Sleep to allow server to register its endpoints
    exec sleep 10

    # Run the test
    test {
	list [catch {server stop tserver -method soft} msg] $msg
    } compare_strings {0 {}}

    # Cleanup steps
    catch {server stop tserver -method hard} dontcare 
}

proc server_stop_9_P {} {
    global DCP_SR_ENTRYNAME 

    # Identify the test
    tet_infoline server_stop_9_P: Check to see that we can stop server by method "hard"

    # Setup steps
    catch {server stop tserver -method hard} dontcare 
    if {[dcp_wrap {server start tserver}] != 0} {return;}

    # Sleep to allow server to register its endpoints
    exec sleep 10

    # Run the test
    test {
	list [catch {server stop tserver -method hard} msg] $msg
    } compare_strings {0 {}}

    test {
	list [catch {server ping $DCP_SR_ENTRYNAME} msg] $msg
    } match_strings {0 1}

    # Cleanup steps
    catch {server stop tserver -method hard} dontcare 
}

proc server_stop_10_P {} {
    global TET_ROOT

    # Identify the test
    tet_infoline server_stop_10_P: Check to see that we can stop server by method "error"

    # Setup steps
    catch {server stop tserver -method hard} dontcare 
    catch {exec rm -f $TET_ROOT/functional/admin/dcecp/ts/server/core} dontcare
    if {[dcp_wrap {server start tserver}] != 0} {return;}

    # Sleep to allow server to register its endpoints
    exec sleep 10

    # Run the test
    test {
	list [catch {server stop tserver -method error} msg] $msg
    } compare_strings {0 {}}

    # Sleep to allow server to shutdown
    exec sleep 10

    test {
        list [catch {exec ls $TET_ROOT/functional/admin/dcecp/ts/server/core} msg] $msg
    } compare_strings "0 $TET_ROOT/functional/admin/dcecp/ts/server/core"

    # Cleanup steps
    catch {server stop tserver -method hard} dontcare 
    catch {exec rm -f $TET_ROOT/functional/admin/dcecp/ts/server/core} dontcare
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "server_stop_7_P"
set ic2 "server_stop_8_P"
set ic3 "server_stop_9_P"
set ic4 "server_stop_10_P"
set tet_startup server_stop_startup
set tet_cleanup server_stop_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
