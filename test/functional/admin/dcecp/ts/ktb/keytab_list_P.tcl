#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: keytab_list_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:16:54  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:12  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:31  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:34  root]
# 
# Revision 1.1.2.1  1994/09/28  21:40:32  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:43  melman]
# 
# Revision 1.1.2.6  1994/06/22  20:37:16  baum
# 	OT #11022 - Use tetexec.cfg for cell/host.  Use tet_setup
# 	[1994/06/22  20:36:09  baum]
# 
# Revision 1.1.2.5  1994/06/10  20:13:20  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:37  devsrc]
# 
# Revision 1.1.2.4  1994/06/07  23:13:51  baum
# 	Mods to handle new dcecp output (CR #10761)
# 	[1994/06/07  23:13:22  baum]
# 
# Revision 1.1.2.3  1994/05/25  22:43:00  baum
# 	Keytab tests now dce_login/out on their own.  Also, corrected some of the output errors
# 	[1994/05/25  22:41:39  baum]
# 
# Revision 1.1.2.2  1994/05/11  19:37:43  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:22:00  rousseau]
# 
# Revision 1.1.2.1  1994/05/10  22:19:52  baum
# 	INitial Checkin
# 	[1994/05/10  22:19:07  baum]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_setup.tcl

## Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

## Keytab specific routines
source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_misc.tcl

proc tet_setup {} { ktb_std_setup }
proc tet_cleanup {} { ktb_std_cleanup }

## Test purposes
proc keytab_list_4_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    source $TET_ROOT/functional/admin/dcecp/ts/ktb/ktb_vars.tcl

    # Identify the test
    tet_infoline keytab_list_4_P: Check to see that we can list all keys in a keytab entry.

    # Setup steps
    tet_setup

    # Run the test
    set cmd "keytab list $KTB1_LONG_NAME"
    set ret [catch "$cmd" msg]
    set expected_output [list $TEST_PRINC1 $TEST_PRINC2]
    compare_list_and_report $cmd $ret $msg $expected_output

    # Cleanup steps
    tet_cleanup
}

set iclist "ic1 "
set ic1 "keytab_list_4_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
