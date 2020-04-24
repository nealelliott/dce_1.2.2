#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: cdsalias_hcell_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:12:26  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:37  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:01  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/06/29  18:10 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts37/1  1995/06/29  18:09 UTC  truitt
# 	CHFts14650: Fix test to match cdsalias behavior.
# 
# 	HP revision /main/HPDCE02/1  1994/11/07  17:07 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/07  17:04 UTC  truitt
# 	Pass back a result if DirectoryVersion is not
# 	at 3.0 as expected.
# 	[1995/12/11  15:52:11  root]
# 
# Revision 1.1.2.1  1994/10/05  22:01:28  rousseau
# 	Updated to standard dcecp test format
# 	[1994/10/05  21:59:35  rousseau]
# 
# Revision 1.1.2.1  1994/10/05  18:55:21  zee
# 	      Initial version.
# 	      [1994/10/05  18:30:35  zee]
# 
# $EndLog$
#
# Description:
#   Negative functional tests for cdsalias functions.
#
#   Expects to be run in a newly configured DCE 1.1 cell with
#   CDS_DirectoryVersion of /.: at 3.0. (for cdsalias_lowdirver_N test)
#   Side effect of the tests is that root directory's CDS_DirectoryVersion 
#   gets upgraded to 4.0.
#

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc cdsalias_hcell_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc cdsalias_hcell_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}


## Test purposes
proc cdsalias_hcell_1_N {} {
    # Identify the test
    tet_infoline cdsalias_hcell_1_N: Check of cdsalias error - CDS_LOWDIRVERSION
    
    # Setup steps
    if {[string compare [dcp_get_element [directory show /.:] CDS_DirectoryVersion] 4.0] == 0} {
        tet_infoline setup: CDS_DirectoryVersion is already at 4.0.  This test case is
        tet_infoline setup: only valid when the CDS_DirectoryVersion is less than 4.0.
        tet_result UNINITIATED
        return;
    }

    # Run the test
    test { 
        list [catch {cdsalias create /.../cdsalias_hcell_1_N} msg] $msg
    } compare_strings {1 {For cellname commands, the CDS_DirectoryVersion attribute is required to be 4.0 or greater.}}

    # Cleanup steps
}       

proc cdsalias_hcell_2_N {} {
    # Identify the test
    tet_infoline cdsalias_hcell_2_N: Check of cdsalias error - CDS_ALIASTOOSHORT
    
    # Setup steps
    if {[dcp_wrap {Upgrade_Rootdir}] != 0} {return;}

    # Run the test
    test { 
        list [catch {cdsalias create /...} msg] $msg
    } compare_strings {1 {The supplied cellname is required to have two simple names for an alias and three simple names for a child alias.}}

    # Cleanup steps
}       

proc cdsalias_hcell_3_N {} {
    # Identify the test
    tet_infoline cdsalias_hcell_3_N: Check of cdsalias error - CDS_CANNOT_REM_CELLNAME
    
    # Setup steps
    if {[dcp_wrap {Upgrade_Rootdir}] != 0} {return;}

    # Run the test
    test { 
        list [catch {cdsalias delete /.:} msg] $msg
    } compare_strings {1 {You cannot remove the current cellname alias.}}

    # Cleanup steps
}       


set iclist "ic1 ic2 ic3 "
set ic1 "cdsalias_hcell_1_N"
set ic2 "cdsalias_hcell_2_N"
set ic3 "cdsalias_hcell_3_N"

set tet_startup cdsalias_hcell_startup
set tet_cleanup cdsalias_hcell_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
