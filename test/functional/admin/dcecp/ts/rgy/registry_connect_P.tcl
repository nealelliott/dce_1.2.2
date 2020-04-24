#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: registry_connect_P.tcl,v $
# Revision 1.1.2.2  1996/03/09  20:47:24  marty
# 	Add OSF copyright
# 	[1996/03/09  20:26:17  marty]
#
# Revision 1.1.2.1  1995/12/13  15:28:52  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/09/11  16:18 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts46/1  1995/09/11  16:15 UTC  truitt
# 	Initial test file.
# 	[1995/12/11  15:55:31  root]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc registry_connect_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_connect_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc registry_connect_001_P {} {
    global DCP_CELLNAME_TWO DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline registry_connect_1_P: Connect to foreign cell.

    # Setup steps

    # Run the test
    test {
        registry connect $DCP_CELLNAME_TWO -mypwd $DCP_CLIENT_PW -facctpwd $DCP_CLIENT_PW \
            -facct $DCP_CLIENT -fgroup none -forg none -org none -group none \
            -acctvalid -facctvalid
    } compare_strings {}

    # Cleanup steps
}

set iclist "ic1 "
set ic1 "registry_connect_001_P"
set tet_startup registry_connect_startup
set tet_cleanup registry_connect_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
