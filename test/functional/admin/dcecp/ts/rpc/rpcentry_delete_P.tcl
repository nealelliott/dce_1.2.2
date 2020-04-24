#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: rpcentry_delete_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:24:56  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:19  marty]
#
# Revision 1.1.4.1  1995/12/11  15:14:05  root
# 	Submit
# 	[1995/12/11  14:31:30  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:12  root]
# 
# Revision 1.1.2.1  1994/09/28  21:44:57  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:19  melman]
# 
# Revision 1.1.2.4  1994/09/02  18:43:43  coren
# 	Corrected expected error message (CR12010). Changed magic comment to
# 	specify /usr/bin/dcecp.
# 	[1994/09/02  17:36:10  coren]
# 
# Revision 1.1.2.3  1994/06/17  18:43:38  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:27:33  devsrc]
# 
# Revision 1.1.2.2  1994/05/26  20:21:03  coren
# 	Positive tests filled in.
# 	[1994/05/26  19:11:53  coren]
# 
# Revision 1.1.2.1  1994/05/19  20:02:30  melman
# 	initial rpccp tests
# 	[1994/05/19  19:59:40  melman]
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
proc rpcentry_delete_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc rpcentry_delete_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}    

proc rpcentry_delete_4_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_delete_4_P: Check to see if we can delete an RPC entry.

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1}] != 0} {return;}
    if {[dcp_wrap {rpcentry create $RPCENTRY1}] != 0} {return;}

    # Run the test
    test { rpcentry delete $RPCENTRY1
         list [catch {rpcentry show $RPCENTRY1} msg] $msg
    } compare_strings {1 {Entry not found}}

    # Cleanup steps
}

proc rpcentry_delete_5_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline rpcentry_delete_5_P: Check to see if we can delete multiple RPC entrys.

    # Setup steps
    if {[dcp_wrap {global RPCENTRY1 RPCENTRY2 RPCENTRY3}] != 0} {return;}
    if {[dcp_wrap {rpcentry create "$RPCENTRY1 $RPCENTRY2 $RPCENTRY3"}] != 0} {return;}

    # Run the test
    test { rpcentry delete "$RPCENTRY1 $RPCENTRY2 $RPCENTRY3"
         catch {rpcentry show $RPCENTRY1} msg1
         catch {rpcentry show $RPCENTRY2} msg2
         catch {rpcentry show $RPCENTRY3} msg3
         list $msg1 $msg2 $msg3
    } compare_strings {{Entry not found} {Entry not found} {Entry not found}}
        # Expected results are:
        # Successful completion.
        # The RPC entrys XXXXXX, XXXXXX and XXXXXX should be deleted as 
        # verified by rpcentry show.

    # Cleanup steps
}


set iclist "ic1 ic2 "
set ic1 "rpcentry_delete_4_P"
set ic2 "rpcentry_delete_5_P"
set tet_startup rpcentry_delete_startup
set tet_cleanup rpcentry_delete_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
