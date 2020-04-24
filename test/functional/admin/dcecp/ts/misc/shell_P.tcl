#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: shell_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:18:10  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:45  marty]
#
# Revision 1.1.4.2  1995/12/13  15:27:01  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:54:00  root]
# 
# Revision 1.1.2.1  1994/09/28  21:41:13  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:26  melman]
# 
# Revision 1.1.2.2  1994/06/17  18:43:32  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:27:00  devsrc]
# 
# Revision 1.1.2.1  1994/06/02  18:50:28  kevins
# 	Initial Misc commands test drop
# 	[1994/06/02  18:44:14  kevins]
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


## Test purposes
proc shell_1_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW 

    # Identify the test
    tet_infoline shell_1_P: Verify that we can run a command from the OS.

    # Setup steps
    if {[dcp_wrap {set HOSTNAME [exec /bin/hostname]}] != 0} {return;}

    # Run the test
    test { shell /bin/hostname } compare_strings $HOSTNAME

    # The command should return the output of the local OS's 'hostname'
    # command.

    # Cleanup steps
}


set iclist "ic1 "
set ic1 "shell_1_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
