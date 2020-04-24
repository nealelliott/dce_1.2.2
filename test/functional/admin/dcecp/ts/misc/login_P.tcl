#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: login_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:17:47  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:36  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:51  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/07/24  20:18 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts42/1  1995/07/24  20:16 UTC  truitt
# 	CHFts14292: initial test cleanup.
# 	[1995/12/11  15:53:52  root]
# 
# Revision 1.1.2.2  1994/09/30  15:29:29  kevins
# 	OT 12435 modified tests and test messages.
# 	[1994/09/30  15:27:07  kevins]
# 
# Revision 1.1.2.1  1994/09/28  21:40:54  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:12  melman]
# 
# Revision 1.1.2.2  1994/06/17  18:43:25  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:26:26  devsrc]
# 
# Revision 1.1.2.1  1994/06/02  18:49:59  kevins
# 	Initial Misc commands test drop
# 	[1994/06/02  18:43:31  kevins]
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
proc login_4_P {} {
    global DCP_CLIENT DCP_CLIENT_PW _u

    # Identify the test
    tet_infoline login_4_P: See if we can login to the dce security service.

    # Setup steps

    # Run the test
    test {login $DCP_CLIENT -password $DCP_CLIENT_PW 
        set _u } compare_strings cell_admin

   if {[dcp_wrap {logout}] != 0} {return;}
}


set iclist "ic1 "
set ic1 "login_4_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
