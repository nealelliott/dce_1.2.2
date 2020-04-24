#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: registry_replace_P.tcl,v $
# Revision 1.1.2.2  1996/03/09  20:47:29  marty
# 	Add OSF copyright
# 	[1996/03/09  20:26:21  marty]
#
# Revision 1.1.2.1  1995/12/11  15:13:01  root
# 	Submit
# 	[1995/12/11  14:30:55  root]
# 
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/09/11  16:13 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts46/1  1995/09/11  16:11 UTC  truitt
# 	Initial test file.
# 	[1995/12/08  23:15:30  root]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc registry_replace_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_replace_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc registry_replace_001_P {} {
    global DCP_RGY_SA_REP_NAME_SLAVE1 DCP_RGY_SA_NETADDR_SLAVE1

    # Identify the test
    tet_infoline registry_replace_1_P: Replace the address of the slave.

    # Setup steps

    # Run the test
    test {
        registry replace $DCP_RGY_SA_REP_NAME_SLAVE1 -address "ncacn_ip_tcp:$DCP_RGY_SA_NETADDR_SLAVE1 ncadg_ip_udp:$DCP_RGY_SA_NETADDR_SLAVE1"
        attrlist getvalues [registry show -replica $DCP_RGY_SA_REP_NAME_SLAVE1] -type addresses
    } compare_lists "{ncacn_ip_tcp $DCP_RGY_SA_NETADDR_SLAVE1} {ncadg_ip_udp $DCP_RGY_SA_NETADDR_SLAVE1}"

    # Cleanup steps
}

set iclist "ic1 "
set ic1 "registry_replace_001_P"
set tet_startup registry_replace_startup
set tet_cleanup registry_replace_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
