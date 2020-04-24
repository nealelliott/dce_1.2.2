#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: cdsalias_create_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:12:15  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:28:31  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:58  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/11/07  15:56 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/07  15:54 UTC  truitt
# 	Upgrade the Directory Version attribute to 4.0
# 	during the setup step.
# 	[1995/12/11  15:52:08  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:27  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:07:06  melman]
# 
# Revision 1.1.2.1  1994/09/28  14:23:49  kevins
# 	OT 12378 Initial submission
# 	[1994/09/28  14:22:52  kevins]
# 
# 	OT 12378 cdsalias functional tests
# 	[1994/09/28  11:06:07  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc cdsalias_create_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc cdsalias_create_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc cdsalias_create_5_P {} {
    # Identify the test
    tet_infoline cdsalias_create_5_P: Check that an alias can be created.

    # Setup steps
    if {[dcp_wrap {Upgrade_Rootdir}] != 0} {return;}
    if {[dcp_wrap {set name [string tolower [lindex [info level [info level]] 0]]
        cdsalias delete /.../$name
        set foo not_found
        set bar "Alias /.../$name"}] != 0} {return;}

    # Run the test
    test {cdsalias create /.../$name
          foreach element [lindex [cdsalias catalog /.:] 0] {
            if { [string compare $element $bar ] == 0 } {
                set foo found } 
          }
          set foo
         } compare_strings  found
    # Cleanup steps
    if {[dcp_wrap {cdsalias delete /.../$name}] != 0} {return;}
}


set iclist "ic1 "
set ic1 "cdsalias_create_5_P"
set tet_startup cdsalias_create_startup
set tet_cleanup cdsalias_create_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
