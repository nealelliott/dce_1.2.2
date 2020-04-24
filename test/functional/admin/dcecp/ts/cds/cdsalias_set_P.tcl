#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: cdsalias_set_P.tcl,v $
# Revision 1.1.2.3  1996/03/11  02:12:36  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:44  marty]
#
# Revision 1.1.2.2  1995/12/13  15:25:04  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/06/09  13:18 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts34/1  1995/06/09  13:17 UTC  truitt
# 	New test.
# 	[1995/12/11  15:52:15  root]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc cdsalias_set_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc cdsalias_set_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc cdsalias_set_1_P {} {
    global DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline cdsalias_set_1_P: Set the primary cdsalias to a new alias name.

    # Setup steps
    if {[dcp_wrap {Upgrade_Rootdir}] != 0} {return;}
    if {[dcp_wrap {cdsalias create /.../pokey1}] != 0} {return;}
    set foo not_found

    # Run the test
    test {
        cdsalias set /.../pokey1
        foreach element [lindex [cdsalias catalog /.:] 0] {
            if {[string compare $element "Primary /.../pokey1"] == 0 } {
                set foo found
            }
        }
        set foo
    } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {cdsalias set $DCP_CELLNAME_ONE}] != 0} {return;}
    if {[dcp_wrap {cdsalias delete /.../pokey1}] != 0} {return;}
}

proc cdsalias_set_2_P {} {
    global DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline cdsalias_set_2_P: Set the primary cdsalias to the already primary name.

    # Setup steps
    if {[dcp_wrap {Upgrade_Rootdir}] != 0} {return;}
    if {[dcp_wrap {cdsalias create /.../pokey2}] != 0} {return;}
    set foo not_found

    # Run the test
    test {
        cdsalias set $DCP_CELLNAME_ONE
        foreach element [lindex [cdsalias catalog /.:] 0] {
            if {[string compare $element "Primary $DCP_CELLNAME_ONE"] == 0 } {
                set foo found
            }
        }
        set foo
    } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {cdsalias delete /.../pokey2}] != 0} {return;}
}


set iclist "ic1 ic2 "
set ic1 "cdsalias_set_1_P"
set ic2 "cdsalias_set_2_P"
set tet_startup cdsalias_set_startup
set tet_cleanup cdsalias_set_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
