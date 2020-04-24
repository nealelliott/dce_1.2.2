#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: keytab_catalog_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:16:38  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:07  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:25  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:28  root]
# 
# Revision 1.1.2.1  1994/09/28  21:40:22  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:34  melman]
# 
# Revision 1.1.2.5  1994/09/01  15:44:03  baum
# 	Fixed output messages in negative tests (OT 11093)
# 	[1994/09/01  15:42:36  baum]
# 
# Revision 1.1.2.4  1994/06/10  20:13:14  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:24  devsrc]
# 
# Revision 1.1.2.3  1994/05/19  23:06:22  baum
# 	Resync with changes made to commands and command output
# 	[1994/05/19  23:05:50  baum]
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
proc keytab_catalog_1_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline keytab_catalog_1_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab catalog -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc keytab_catalog_2_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline keytab_catalog_2_N: Check that a wrong host are returned as unknown entry. 

    # Setup steps

    # Run the test
    test {
        list [catch {keytab catalog foo} msg] $msg
    } compare_strings {1 {Bad or wrong arguments}}

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "keytab_catalog_1_N"
set ic2 "keytab_catalog_2_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
