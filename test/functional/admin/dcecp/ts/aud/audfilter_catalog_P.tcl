#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: audfilter_catalog_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:10:53  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:15  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:40  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:51:44  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:10  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:50  melman]
# 
# Revision 1.1.2.5  1994/06/10  20:02:44  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:53  devsrc]
# 
# Revision 1.1.2.4  1994/05/23  18:09:23  rousseau
# 	Various updates.
# 	[1994/05/23  18:08:57  rousseau]
# 
# Revision 1.1.2.3  1994/05/11  19:35:01  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:16:40  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:26:59  rousseau
# 	Performance changes, and new API path (CR 10558).
# 	[1994/05/06  15:24:35  rousseau]
# 
# Revision 1.1.2.1  1994/04/21  20:28:48  rousseau
# 	Initial drop of audit tests (CR 9938).
# 	[1994/04/21  20:27:28  rousseau]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_setup.tcl

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc audfilter_catalog_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc audfilter_catalog_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc audfilter_catalog_4_P {} {
    # Identify the test
    tet_infoline audfilter_catalog_4_P: Check that we can show a list of all the filters.

    # Setup steps
    if {[dcp_wrap {principal create audfilter_catalog_4_P
                   group create audfilter_catalog_4_P
                   audfilter create {principal audfilter_catalog_4_P} \
                                    -a {dce_dts_synch denial log}
                   audfilter create {group audfilter_catalog_4_P} \
                                    -a {dce_dts_synch denial log}
                   audfilter create {world} -a {dce_dts_synch denial log}}] \
			   != 0} {return;}

    # Run the test
    test {
	audfilter catalog
    } compare_lists {{principal audfilter_catalog_4_P} {group audfilter_catalog_4_P} world}

    # Cleanup steps
    if {[dcp_wrap {audfilter delete [audfilter cat]
                   principal delete audfilter_catalog_4_P
                   group delete audfilter_catalog_4_P}] != 0} {return;}
}


set iclist "ic1 "
set ic1 "audfilter_catalog_4_P"
set tet_startup audfilter_catalog_startup
set tet_cleanup audfilter_catalog_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl

