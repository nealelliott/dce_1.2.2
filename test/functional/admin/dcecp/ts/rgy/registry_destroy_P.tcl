#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: registry_destroy_P.tcl,v $
# Revision 1.1.2.2  1996/03/11  02:22:34  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:31  marty]
#
# Revision 1.1.2.1  1995/12/11  15:12:48  root
# 	Submit
# 	[1995/12/11  14:30:46  root]
# 
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/09/11  15:51 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts46/1  1995/09/11  15:50 UTC  truitt
# 	CHFts16100: Additional tests to support new functionality.
# 
# 	HP revision /main/HPDCE02/1  1995/03/30  15:25 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts23/1  1995/03/30  15:24 UTC  truitt
# 	New test file
# 	[1995/12/08  23:15:18  root]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc registry_destroy_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_destroy_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc registry_destroy_014_P {} {
    # Identify the test
    tet_infoline registry_destroy_014_P: Delete registry copy, but not from master replica list.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}

    # Run the test
    test {
        list [catch "registry destroy $DCP_RGY_SA_REP_NAME_SLAVE1" msg] $msg
    } compare_strings {0 {}}

    # Note that registry server and database are dead
    test {
        catch "registry show $DCP_RGY_SA_REP_NAME_SLAVE1 -replica"
    } compare_strings {1}

    # Cleanup steps
    if {[dcp_wrap {registry delete $DCP_RGY_SA_REP_NAME_SLAVE1 -force}] != 0} {return;}
    if {[dcp_wrap {exit}] != 0} {return;}
}

proc registry_destroy_015_P {} {
    # Identify the test
    tet_infoline registry_destroy_015_P: Delete from master replica list, but not registry copy.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}

    # Run the test
    test {
        list [catch "registry delete $DCP_RGY_SA_REP_NAME_SLAVE1 -force" msg] $msg
    } compare_strings {0 {}}

    # Note that registry server and database are still alive
    test {
        catch "registry show $DCP_RGY_SA_REP_NAME_SLAVE1 -replica"
    } compare_strings {0}

    # Cleanup steps
    if {[dcp_wrap {registry destroy $DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}
    if {[dcp_wrap {exit}] != 0} {return;}
}

set iclist "ic1 ic2 "
set ic1 "registry_destroy_014_P"
set ic2 "registry_destroy_015_P"
set tet_startup registry_destroy_startup
set tet_cleanup registry_destroy_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
