#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: registry_designate_P.tcl,v $
# Revision 1.1.2.2  1996/03/09  20:47:26  marty
# 	Add OSF copyright
# 	[1996/03/09  20:26:19  marty]
#
# Revision 1.1.2.1  1995/12/11  15:12:46  root
# 	Submit
# 	[1995/12/11  14:30:44  root]
# 
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1995/09/28  17:48 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts47/1  1995/09/28  17:47 UTC  truitt
# 	CHFts14292: Improvement in tests.
# 
# 	HP revision /main/HPDCE02/2  1995/09/11  15:55 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts46/1  1995/09/11  15:54 UTC  truitt
# 	CHFts16100: Additional tests to support new functionality.
# 
# 	HP revision /main/HPDCE02/1  1995/03/30  15:29 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts23/1  1995/03/30  15:28 UTC  truitt
# 	New test file
# 	[1995/12/08  23:15:16  root]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc registry_designate_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_designate_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}


## Test purposes
proc registry_designate_1_P {} {
    global DCP_RGY_SA_REP_NAME_MASTER

    # Identify the test
    tet_infoline registry_designate_1_P: Set the default master to be a slave.

    # Setup steps
    if {[dcp_wrap {registry checkpoint $DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}

    # Run the test
    test {
        registry designate $DCP_RGY_SA_REP_NAME_MASTER -slave
        attrlist getvalues [registry dump] -type type
    } compare_lists {slave slave}

    # Cleanup steps
}

proc registry_designate_2_P {} {
    global DCP_RGY_SA_REP_NAME_MASTER

    # Identify the test
    tet_infoline registry_designate_2_P: Set the now-slave back to being the master.

    # Setup steps
    if {[dcp_wrap {registry checkpoint $DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}

    # Run the test
    test {
        registry designate $DCP_RGY_SA_REP_NAME_MASTER -master
        attrlist getvalues [registry dump] -type type
    } compare_lists {master slave}

    # Cleanup steps
}

proc registry_designate_3_P {} {
    global DCP_RGY_SA_REP_NAME_MASTER

    # Identify the test
    tet_infoline registry_designate_3_P: Set a master using the -force option.

    # Setup steps
    if {[dcp_wrap {registry checkpoint $DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}
    if {[dcp_wrap {registry designate $DCP_RGY_SA_REP_NAME_MASTER -slave}] != 0} {return;}
    if {[dcp_wrap {registry checkpoint $DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}

    # Run the test
    test {
        registry designate $DCP_RGY_SA_REP_NAME_MASTER -master -force
        attrlist getvalues [registry dump] -type type
    } compare_lists {master slave}

    # Cleanup steps
    if {[dcp_wrap {registry checkpoint $DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 "
set ic1 "registry_designate_1_P"
set ic2 "registry_designate_2_P"
set ic3 "registry_designate_3_P"
set tet_startup registry_designate_startup
set tet_cleanup registry_designate_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
