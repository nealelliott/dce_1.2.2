#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: server_create_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:27:52  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:31  marty]
#
# Revision 1.1.4.1  1995/12/11  15:16:24  root
# 	Submit
# 	[1995/12/11  14:32:25  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:26  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:31  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:30  melman]
# 
# Revision 1.1.2.4  1994/09/20  18:22:38  pwang
# 	Updated to latest changes [OT#10262]
# 	[1994/09/20  18:10:29  pwang]
# 
# Revision 1.1.2.3  1994/07/05  16:14:37  pwang
# 	Adjusted tests to match with the dcecp code changes [OT#11093]
# 	[1994/07/05  15:55:27  pwang]
# 
# Revision 1.1.2.2  1994/06/17  18:44:10  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:30:12  devsrc]
# 
# Revision 1.1.2.1  1994/06/10  21:39:44  pwang
# 	Initial Implementation
# 	[1994/06/10  21:27:20  pwang]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

proc server_create_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    catch {server delete tserver} donotcare
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc server_create_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

## Test purposes
proc server_create_6_P {} {
    global TET_ROOT DCP_SR_ENTRYNAME DCP_SR_PROTSEQ DCP_SR_IP_ADDR \
           DCP_HOSTNAME_THREE DCP_CELLNAME_THREE

    # Identify the test
    tet_infoline server_create_6_P: Check that we can create a server object.

    # Setup steps

    # Run the test
    test {
        list [catch "server create tserver -attr {{arguments $DCP_SR_ENTRYNAME} {program tserver} {entryname $DCP_SR_ENTRYNAME} {services {{annotation {dcecp server object test program}} {bindings $DCP_SR_PROTSEQ:$DCP_SR_IP_ADDR} {ifname {test server}} {interface 008bebed-c7c1-1ddc-9cb3-0000c0ba4944,1.0} {objects 0073f23a-2e1a-1ddd-b73a-0000c0ba4944} {entryname $DCP_SR_ENTRYNAME}}} {principals tserver} {starton explicit} {uuid 00469db2-231c-1de5-80de-0000c0ba4944} {directory $TET_ROOT/functional/admin/dcecp/ts/server}}" msg] $msg
    } compare_strings {0 {}}

    test {
        list [catch {server show $DCP_CELLNAME_THREE/$DCP_HOSTNAME_THREE/config/srvrconf/tserver} msg] $msg
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
    if {[dcp_wrap {server delete tserver}] != 0} {return;}
}

proc server_create_7_P {} {
    global TET_ROOT DCP_SR_ENTRYNAME DCP_SR_PROTSEQ DCP_SR_IP_ADDR \
	   DCP_HOSTNAME_THREE DCP_CELLNAME_THREE

    # Identify the test
    tet_infoline server_create_7_P: Check that we can create a server object by atributes switches.

    # Setup steps

    # Run the test
    test {
	list [catch "server create tserver -arguments $DCP_SR_ENTRYNAME -program tserver -entryname $DCP_SR_ENTRYNAME -services {{annotation {dcecp server object test program}} {bindings $DCP_SR_PROTSEQ:$DCP_SR_IP_ADDR} {ifname {test server}} {interface 008bebed-c7c1-1ddc-9cb3-0000c0ba4944,1.0} {objects 0073f23a-2e1a-1ddd-b73a-0000c0ba4944} {entryname $DCP_SR_ENTRYNAME}} -principals tserver -starton explicit -uuid {003b24d2-a196-1df3-915f-0000c0ba4944} -directory {$TET_ROOT/functional/admin/dcecp/ts/server}" msg] $msg
    } compare_strings {0 {}}

    test {
	list [catch {server show $DCP_CELLNAME_THREE/$DCP_HOSTNAME_THREE/config/server/tserver} msg] $msg
    } compare_lists "0 {{uuid 003b24d2-a196-1df3-915f-0000c0ba4944}
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
    if {[dcp_wrap {server delete tserver}] != 0} {return;}
}


set iclist "ic1 ic2 "
set ic1 "server_create_6_P"
set ic2 "server_create_7_P"
set tet_startup server_create_startup
set tet_cleanup server_create_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
