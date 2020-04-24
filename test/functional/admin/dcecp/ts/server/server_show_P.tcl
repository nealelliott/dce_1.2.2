#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: server_show_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:28:31  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:48  marty]
#
# Revision 1.1.4.1  1995/12/11  15:16:57  root
# 	Submit
# 	[1995/12/11  14:32:37  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:42  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:53  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:46  melman]
# 
# Revision 1.1.2.5  1994/09/20  22:12:16  pwang
# 	Remove tserver obj before creating another one [OT#10262]
# 	[1994/09/20  20:33:58  pwang]
# 
# Revision 1.1.2.4  1994/09/20  18:23:10  pwang
# 	Updated to latest changes [OT#10262]
# 	[1994/09/20  18:16:25  pwang]
# 
# Revision 1.1.2.3  1994/07/05  16:14:28  pwang
# 	Adjusted tests to match with the dcecp code changes [OT#11093]
# 	[1994/07/05  16:04:39  pwang]
# 
# Revision 1.1.2.2  1994/06/17  18:44:29  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:31:08  devsrc]
# 
# Revision 1.1.2.1  1994/06/10  21:40:16  pwang
# 	Initial implementation
# 	[1994/06/10  21:33:31  pwang]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc server_show_startup {} {
    global TET_ROOT DCP_SR_ENTRYNAME DCP_SR_PROTSEQ DCP_SR_IP_ADDR

    catch {server delete tserver} dontcare
    if {[dcp_wrap "server create tserver -attr {{arguments $DCP_SR_ENTRYNAME} {program tserver} {entryname $DCP_SR_ENTRYNAME} {services {{annotation {dcecp server object test program}} {bindings $DCP_SR_PROTSEQ:$DCP_SR_IP_ADDR} {ifname {test server}} {interface 008bebed-c7c1-1ddc-9cb3-0000c0ba4944,1.0} {objects 0073f23a-2e1a-1ddd-b73a-0000c0ba4944} {entryname $DCP_SR_ENTRYNAME}}} {principals tserver} {starton explicit} {uuid 00469db2-231c-1de5-80de-0000c0ba4944} {directory $TET_ROOT/functional/admin/dcecp/ts/server}}"] != 0} {return;}

}

proc server_show_cleanup {} {
    if {[dcp_wrap {server delete tserver}] != 0} {return;}
}

## Test purposes
proc server_show_6_P {} {
    global TET_ROOT DCP_SR_ENTRYNAME DCP_SR_PROTSEQ DCP_SR_IP_ADDR DCP_CELLNAME_THREE

    # Identify the test
    tet_infoline server_show_6_P: Check to see that we can show server configuration objects.

    # Setup steps

    # Run the test
    test {
	list [catch {server show tserver} msg] $msg
    } compare_lists "0 {{uuid 00469db2-231c-1de5-80de-0000c0ba4944}
{program tserver}
{arguments
 $DCP_SR_ENTRYNAME}
{prerequisites {}}
{keytabs {}}
{entryname $DCP_SR_ENTRYNAME}
{services
 {{ifname  {test server}}
  {annotation  {dcecp server object test program}}
  {interface {008bebed-c7c1-1ddc-9cb3-0000c0ba4944 1.0}}
  {bindings
   {$DCP_SR_PROTSEQ $DCP_SR_IP_ADDR}}
  {objects
   0073f23a-2e1a-1ddd-b73a-0000c0ba4944}
  {flags {}}
  {entryname $DCP_SR_ENTRYNAME}}}
{principals
 $DCP_CELLNAME_THREE/tserver}
{starton explicit}
{uid 0}
{gid 0}
{dir $TET_ROOT/functional/admin/dcecp/ts/server}}"

    # Cleanup steps
}

proc server_show_7_P {} {
    # Identify the test
    tet_infoline server_show_7_P: Check to see that we can show server execution objects.

    # Setup steps
    if {[dcp_wrap {server start tserver}] != 0} {return;} 

    # Run the test
    test {
	lindex [list [catch {server show tserver -executing} msg] $msg ] 1
    } dcp_find_attr {executing}

    # Cleanup steps
    if {[dcp_wrap {server stop tserver -method hard}] != 0} {return;}
}


set iclist "ic1 ic2 "
set ic1 "server_show_6_P"
set ic2 "server_show_7_P"
set tet_startup server_show_startup
set tet_cleanup server_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
