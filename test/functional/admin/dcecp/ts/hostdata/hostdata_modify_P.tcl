#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: hostdata_modify_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:16:23  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:31:59  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:19  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:23  root]
# 
# Revision 1.1.2.1  1994/09/28  21:40:07  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:24  melman]
# 
# Revision 1.1.2.4  1994/06/22  20:42:45  baum
# 	Merged with changes from 1.1.2.3
# 	[1994/06/22  20:42:28  baum]
# 
# 	OT #11022 - Use tet_setup, use tetexec.cfg for cell/host
# 	[1994/06/22  20:40:17  baum]
# 
# Revision 1.1.2.3  1994/06/17  18:43:18  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:25:55  devsrc]
# 
# Revision 1.1.2.2  1994/06/07  23:13:46  baum
# 	Mods to handle new dcecp output (CR #10761)
# 	[1994/06/07  23:13:17  baum]
# 
# Revision 1.1.2.1  1994/05/19  22:51:58  baum
# 	Initial checkin
# 	[1994/05/19  22:51:24  baum]
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

## Routine specific to hostdata tests
source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_misc.tcl

proc tet_setup {} { hd_std_setup }
proc tet_cleanup {} { hd_std_cleanup }

## Test purposes
proc hostdata_modify_7_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    source $TET_ROOT/functional/admin/dcecp/ts/hostdata/hd_vars.tcl

    # Identify the test
    tet_infoline hostdata_modify_7_P: Check to see that we can modify hostdata data.

    # Setup steps
    tet_setup

    # Run the test
    set cmd "hostdata modify $HD1_LONG_NAME -data $MODIFIED_LINE"
    set ret [catch "$cmd" msg]
    if { $ret != 0 } {
      tet_infoline "Hostdata modify fails:"
      tet_infoline $msg
      tet_result FAIL
      tet_cleanup
      return
    }

    # Verify results, hostdata show $HD1_LONG_NAME.

    set cmd "hostdata show $HD1_LONG_NAME"
    set ret [catch "$cmd" msg]

    compare_list_and_report $cmd $ret $msg $MODIFIED_LINE

    # Cleanup steps
    tet_cleanup
}


set iclist "ic1 "
set ic1 "hostdata_modify_7_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
