#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: directory_synchronize_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:13:58  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:29:54  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:32  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:52:40  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:59  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:06  melman]
# 
# Revision 1.1.2.5  1994/09/02  13:56:54  kevins
# 	OT 10617 modified messages and some tests.
# 	[1994/09/02  10:10:51  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:11:53  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:33  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:36:03  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:18:50  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:21  melman
# 	updating to new API path
# 	[1994/05/06  15:20:12  melman]
# 
# Revision 1.1.2.1  1994/04/15  19:24:12  kevins
# 	CR 9938 added directory synchronize
# 	[1994/04/15  15:32:47  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


## Test purposes
proc directory_synchronize_4_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_synchronize_4_P: Check to see that we can synchronize a directory.

    # Setup steps
        # Must be running as cell administrator.
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set bfst [attrlist getvalues [directory show /.:] \
                  -type CDS_LastSkulk]} ] != 0} {return;}
    # Run the test
    test { directory synchronize /.: 
        set afst [attrlist getvalues [directory show /.:] -type CDS_LastSkulk]
        if {[string compare bfst afst] != 0} { list skulk_worked }
	} compare_strings {skulk_worked}

        # Expected results are:
        # not_equal

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {return;}

}


set iclist "ic1 "
set ic1 "directory_synchronize_4_P"


# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
