#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: cdsalias_hcell_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:12:31  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:39  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:02  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/06/29  18:14 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts37/1  1995/06/29  18:13 UTC  truitt
# 	CHFts14650: Fix test to match cdsalias behavior.
# 
# 	HP revision /main/HPDCE02/1  1994/11/07  17:10 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/07  17:08 UTC  truitt
# 	Correct execution of test if gdad is not running.
# 	[1995/12/11  15:52:12  root]
# 
# Revision 1.1.2.1  1994/10/05  22:01:27  rousseau
# 	Updated to standard dcecp test format
# 	[1994/10/05  21:59:34  rousseau]
# 
# Revision 1.1.2.1  1994/10/05  18:55:26  zee
# 	Initial version.
# 	[1994/10/05  18:30:43  zee]
# 
# $EndLog$
#
# Description:
#   Positive functional tests for cdsalias functions.
#

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc cdsalias_hcell_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc cdsalias_hcell_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc cdsalias_hcell_1_P {} {
    # Identify the test
    tet_infoline cdsalias_hcell_1_P: Check to see that we can create a cdsalias
    tet_infoline cdsalias_hcell_1_P: and reference the cell using the alias.

    # Setup steps
    if {[dcp_wrap {Upgrade_Rootdir}] != 0} {return;}

    # Save CDS_CTS of local cell for later comparison
    set Cell_CTS [dcp_get_element [directory show /.:] CDS_CTS ]

    # Create alias
    if {[dcp_wrap {cdsalias create /.../create_alias}] != 0} {return;}

    # Run the comparison (same CDS_CTS using different names)
    set Alias_CTS [dcp_get_element [directory show /.../create_alias] CDS_CTS]
    
    if {[string compare $Cell_CTS $Alias_CTS] != 0} {
        tet_result FAIL
    } else {
        tet_result PASS
    }
    
    # Cleanup steps
    if {[dcp_wrap {cdsalias delete /.../create_alias
                   directory synchronize /.:}] != 0} {return;}
}

proc cdsalias_hcell_2_P {} {
    # Identify the test
    tet_infoline cdsalias_hcell_2_P: Verify that we can delete a cdsalias    

    # Setup steps
    if {[dcp_wrap {Upgrade_Rootdir}] != 0} {return;}

    # Save CDS_CTS of local cell for later comparison
    set Cell_CTS [dcp_get_element [directory show /.:] CDS_CTS ]

    # Create alias
    if {[dcp_wrap {cdsalias create /.../delete_alias}] != 0} {return;}

    # Run the comparison (same CDS_CTS using different names)
    set Alias_CTS [dcp_get_element [directory show /.../delete_alias] CDS_CTS]
    
    if {[string compare $Cell_CTS $Alias_CTS] != 0} {
        tet_result FAIL
    } else {
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap {cdsalias delete /.../delete_alias
                   directory synchronize /.:}] != 0} {return;}
}

proc cdsalias_hcell_3_P {} {
    # Identify the test
    tet_infoline cdsalias_hcell_3_P: Verify that the deleted alias from the
    tet_infoline cdsalias_hcell_3_P: previous test can not be used. This test
    tet_infoline cdsalias_hcell_3_P: is not meaningful w/o the previous test.

    # Setup steps
    if {[catch {exec ps -ef | grep gdad | grep -v grep} msg] == 0} {
        tet_infoline setup: gdad is running.
        tet_infoline setup: The first part of the test will be used.
    } else {
        tet_infoline setup: gdad is not running.
        tet_infoline setup: The second part of the test will be used.
    }

    # Test that the delete alias can no longer be used
    if {[catch {exec ps -ef | grep gdad | grep -v grep} msg] == 0} {
        test { 
            list [catch {directory show /.../delete_alias} msg] $msg
        } compare_strings {1 {Requested entry does not exist}}
    } else {
        test { 
            list [catch {directory show /.../delete_alias} msg] $msg
        } compare_strings {1 {Requested operation would result in lost connectivity to root directory}}
    }
}

proc cdsalias_hcell_4_P {} {
    # Identify the test
    tet_infoline cdsalias_hcell_4_P: Check to see that we can set a cdsalias 
    tet_infoline cdsalias_hcell_4_P: and reference the cell using the alias.

    # Setup steps
    if {[dcp_wrap {Upgrade_Rootdir}] != 0} {return;}

    # Save CDS_CTS of local cell for later comparison
    set Cell_CTS [dcp_get_element [directory show /.:] CDS_CTS]

    # Create alias and set primary name
    if {[dcp_wrap {cdsalias create /.../set_alias}] != 0} {return;}
    if {[dcp_wrap {cdsalias set /.../set_alias}] != 0} {return;}
    if {[dcp_wrap {directory synchronize /.:}] != 0} {return;}

    # Run the comparison (same CDS_CTS using different names)
    catch {set Alias_CTS [dcp_get_element [directory show /.../set_alias] CDS_CTS]}

    if {[string compare $Cell_CTS $Alias_CTS] != 0} {
        tet_result FAIL
    } else {
        tet_result PASS
    }

    # Cleanup is done in the next IC
}

proc cdsalias_hcell_5_P {} {
    global DCP_CELLNAME_ONE
    
    # Identify the test
    tet_infoline cdsalias_hcell_5_P: Check Primary name in alias list.
    tet_infoline cdsalias_hcell_5_P: Verify that we can delete a cdsalias    
    tet_infoline cdsalias_hcell_5_P: This test is not meaningful w/o the 
    tet_infoline cdsalias_hcell_5_P: previous test.
 
    # Check that the alias shows up as the Primary alias
    set Primary_Name [attrlist getvalues [attrlist getvalues [directory show /.:] -type CDS_CellAliases] -type Primary]

    if {[string compare $Primary_Name /.../set_alias] != 0} {
        tet_result FAIL
    } else {
        tet_result PASS 
    }

    # Cleanup steps
    # Reset the primary name to the cell name so we can remove it.
    if {[dcp_wrap {cdsalias set $DCP_CELLNAME_ONE}] != 0} {return;}
    if {[dcp_wrap {directory synchronize /.:}] != 0} {return;}
    if {[dcp_wrap {cdsalias delete /.../set_alias}] != 0} {return;}
    if {[dcp_wrap {directory synchronize /.:}] != 0} {return;}
}

proc cdsalias_hcell_6_P {} {
    global DCP_CELLNAME_ONE
    
    # Identify the test
    tet_infoline cdsalias_hcell_6_P: Check that basic cdsalias create works.
 
    if {[dcp_wrap "cdsalias create $DCP_CELLNAME_ONE/cdsalias_hcell_6P"] != 0} {
        tet_result FAIL
    } else {
        tet_result PASS
    }

    # Cleanup steps
    if {[dcp_wrap "cdsalias delete $DCP_CELLNAME_ONE/cdsalias_hcell_6P"] != 0} {return;}
}

set iclist "ic1 ic2 ic3 ic4 ic5 ic6 "
set ic1 "cdsalias_hcell_1_P"
set ic2 "cdsalias_hcell_2_P"
set ic3 "cdsalias_hcell_3_P"
set ic4 "cdsalias_hcell_4_P"
set ic5 "cdsalias_hcell_5_P"
set ic6 "cdsalias_hcell_6_P"

set tet_startup cdsalias_hcell_startup
set tet_cleanup cdsalias_hcell_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
