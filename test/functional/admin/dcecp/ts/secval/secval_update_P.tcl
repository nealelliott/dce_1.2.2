#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: secval_update_P.tcl,v $
# Revision 1.1.2.2  1996/03/11  02:27:34  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:19  marty]
#
# Revision 1.1.2.1  1995/12/11  15:16:08  root
# 	Submit
# 	[1995/12/11  14:32:19  root]
# 
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/05/31  19:31 UTC  truitt
# 	Merge.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts33/1  1995/05/31  19:24 UTC  truitt
# 	Fix incorrect test numbers.
# 	[1995/12/08  23:17:18  root]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup and common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_setup.tcl
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

## Test purposes
proc secval_update_5_P {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline secval_update_5_P: check to see that we can update pesite every hour.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {
    	return;
    }
    catch {secval deactivate} donotcare
    catch {secval activate} donotcare

    # Run the test
    test {
    	list [catch {secval update -pesite 3600} msg] $msg
    } compare_strings {0 {}}

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {
    	return;
    }

}

proc secval_update_6_P {} {
    global DCP_CLIENT DCP_CLIENT_PW DCP_HOSTNAME_THREE

    # Identify the test
    tet_infoline secval_update_6_P: check to see that we can update pesite file within secval by hostname.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {
    	return;
    }
    catch {secval deactivate $DCP_HOSTNAME_THREE} donotcare
    catch {secval activate $DCP_HOSTNAME_THREE} donotcare

    # Run the test
    test {
    	list [catch {secval update -pesite 3600 $DCP_HOSTNAME_THREE} msg] $msg
    } compare_strings {0 {}}

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {
    	return;
    }
}

proc secval_update_7_P {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline secval_update_7_P: check to see that we can force an update of the pesite from within secval.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {
    	return;
    }
    catch {secval deactivate} donotcare
    catch {secval activate} donotcare

    # Run the test
    test {
    	list [catch {secval update -pesite 0} msg] $msg
    } compare_strings {0 {}}

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {
    	return;
    }

}
proc secval_update_8_P {} {
    global DCP_CLIENT DCP_CLIENT_PW DCP_HOSTNAME_THREE

    # Identify the test
    tet_infoline secval_update_8_P: check to see that we can force an update of the pesite file from within secval by hostname.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {
    	return;
    }
    catch {secval deactivate $DCP_HOSTNAME_THREE} donotcare
    catch {secval activate $DCP_HOSTNAME_THREE} donotcare

    # Run the test
    test {
    	list [catch {secval update -pesite 0 $DCP_HOSTNAME_THREE} msg] $msg
    } compare_strings {0 {}}

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {
    	return;
    }
}


set iclist "ic1 ic2 ic3 ic4 "
set ic1 "secval_update_5_P"
set ic2 "secval_update_6_P"
set ic3 "secval_update_7_P"
set ic4 "secval_update_8_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
