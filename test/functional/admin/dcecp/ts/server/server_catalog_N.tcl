#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: server_catalog_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:27:39  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:23  marty]
#
# Revision 1.1.4.1  1995/12/11  15:16:12  root
# 	Submit
# 	[1995/12/11  14:32:21  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:20  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:21  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:25  melman]
# 
# Revision 1.1.2.3  1994/07/05  16:14:30  pwang
# 	Adjusted status code check [OT#11093]
# 	[1994/07/05  15:52:22  pwang]
# 
# Revision 1.1.2.2  1994/06/17  18:44:06  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:29:53  devsrc]
# 
# Revision 1.1.2.1  1994/06/10  21:39:34  pwang
# 	Initial Implementation
# 	[1994/06/10  21:24:19  pwang]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

## Test purposes
proc server_catalog_1_N {} {
    # Identify the test
    tet_infoline server_catalog_1_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {server catalog -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc server_catalog_2_N {} {
    # Identify the test
    tet_infoline server_catalog_2_N: Check that a wrong host are returned as unknown dce entry. 

    # Setup steps

    # Run the test
    test {
        list [catch {server catalog -executing foo} msg] $msg
    } compare_strings {1 {Bad or wrong arguments}}

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "server_catalog_1_N"
set ic2 "server_catalog_2_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
