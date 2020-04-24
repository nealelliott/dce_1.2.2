#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: server_catalog_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:27:42  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:24  marty]
#
# Revision 1.1.4.1  1995/12/11  15:16:14  root
# 	Submit
# 	[1995/12/11  14:32:22  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:21  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:23  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:26  melman]
# 
# Revision 1.1.2.5  1994/09/20  22:11:59  pwang
# 	Removed the tserver obj before create[OT#10262]
# 	[1994/09/20  20:12:33  pwang]
# 
# Revision 1.1.2.4  1994/09/20  18:22:29  pwang
# 	Updated to latest changes [OT#10262]
# 	[1994/09/20  18:09:09  pwang]
# 
# Revision 1.1.2.3  1994/07/05  16:14:32  pwang
# 	Adjusted tests to match with the dcecp code changes [OT#11093]
# 	[1994/07/05  15:54:02  pwang]
# 
# Revision 1.1.2.2  1994/06/17  18:44:07  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:29:57  devsrc]
# 
# Revision 1.1.2.1  1994/06/10  21:39:36  pwang
# 	Initial Implementation
# 	[1994/06/10  21:24:51  pwang]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc server_catalog_startup {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW DCP_SR_ENTRYNAME DCP_SR_PROTSEQ DCP_SR_IP_ADDR

    catch {server delete tserver} donotcare
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap "server create tserver -attr {{arguments $DCP_SR_ENTRYNAME} {program tserver} {entryname $DCP_SR_ENTRYNAME} {services {{annotation {dcecp server object test program}} {bindings $DCP_SR_PROTSEQ:$DCP_SR_IP_ADDR} {ifname {test server}} {interface 008bebed-c7c1-1ddc-9cb3-0000c0ba4944,1.0} {objects 0073f23a-2e1a-1ddd-b73a-0000c0ba4944} {entryname $DCP_SR_ENTRYNAME}}} {principals tserver} {starton explicit} {uuid 00469db2-231c-1de5-80de-0000c0ba4944} {directory $TET_ROOT/functional/admin/dcecp/ts/server}}"] != 0} {return;}
    if {[dcp_wrap {server start tserver}] != 0} {return;} 
}

proc server_catalog_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {server stop tserver -method hard}] != 0} {return;} 
    if {[dcp_wrap {server delete tserver}] != 0} {return;} 
}

## Test purposes
proc server_catalog_3_P {} {
   global DCP_CELLNAME_THREE DCP_HOSTNAME_THREE 

   # Identify the test
   tet_infoline server_catalog_3_P: Check to see that we can catalog server configuration objects.

   # Setup steps

   # Run the test
   test {
	lindex [list [catch {server catalog} msg] $msg] 1
   } dcp_search_element "$DCP_CELLNAME_THREE/$DCP_HOSTNAME_THREE/config/srvrconf/tserver" 

   # Cleanup steps
}

proc server_catalog_4_P {} {
    # Identify the test
    tet_infoline server_catalog_4_P: Check to see that we can catalog server configuration objects by -simplename.

    # Setup steps

    # Run the test
    test {
	lindex [list [catch {server catalog -simplename} msg] $msg] 1
    } dcp_search_element {tserver} 

    # Cleanup steps
}

proc server_catalog_5_P {} {
   global DCP_HOSTNAME_THREE

   # Identify the test
   tet_infoline server_catalog_5_P: Check to see that we can catalog server configuration objects by hostname.

   # Setup steps

   # Run the test
   test {
	lindex [list [catch "server catalog $DCP_HOSTNAME_THREE -simplename" msg] $msg] 1
   } dcp_search_element {tserver}

   # Cleanup steps
}

proc server_catalog_6_P {} {
   global DCP_CELLNAME_THREE DCP_HOSTNAME_THREE
 
   # Identify the test
   tet_infoline server_catalog_6_P: Check to see that we can catalog server execution objects.

   # Setup steps

   # Run the test
   test {
	lindex [list [catch {server catalog -executing} msg] $msg] 1
   } dcp_search_element "$DCP_CELLNAME_THREE/$DCP_HOSTNAME_THREE/config/srvrexec/tserver"

    # Cleanup steps
}

proc server_catalog_7_P {} {
   # Identify the test
   tet_infoline server_catalog_7_P: Check to see that we can catalog server execution objects by -simplename.

   # Setup steps

   # Run the test
   test {
	lindex [list [catch {server catalog -executing -simplename} msg] $msg] 1
   } dcp_search_element {tserver}

   # Cleanup steps
}

proc server_catalog_8_P {} {
   global DCP_HOSTNAME_THREE

   # Identify the test
   tet_infoline server_catalog_8_P: Check that we can catalog server execution objects by hostname.

   # Setup steps

   # Run the test
   test {
	lindex [list [catch "server catalog $DCP_HOSTNAME_THREE -executing -simplename" msg] $msg] 1
   } dcp_search_element {tserver}

   # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 "
set ic1 "server_catalog_3_P"
set ic2 "server_catalog_4_P"
set ic3 "server_catalog_5_P"
set ic4 "server_catalog_6_P"
set ic5 "server_catalog_7_P"
set ic6 "server_catalog_8_P"
set tet_startup server_catalog_startup
set tet_cleanup server_catalog_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
