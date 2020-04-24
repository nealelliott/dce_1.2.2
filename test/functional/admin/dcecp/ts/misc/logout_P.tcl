#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: logout_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:17:51  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:37  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:53  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/07/24  20:16 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts42/1  1995/07/24  20:13 UTC  truitt
# 	CHFts14292: initial test cleanup.
# 	[1995/12/11  15:53:54  root]
# 
# Revision 1.1.2.1  1994/09/28  21:40:56  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:15  melman]
# 
# Revision 1.1.2.2  1994/06/17  18:43:27  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:26:34  devsrc]
# 
# Revision 1.1.2.1  1994/06/02  18:50:06  kevins
# 	Initial Misc commands test drop
# 	[1994/06/02  18:43:42  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}


# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc logout_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW 
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc logout_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc logout_4_P {} {
    global DCP_CLIENT_PW _u
    tet_infoline logout_4_P: Verify that we can logout of the current context.

    # Setup steps
    if {[dcp_wrap {principal create {foo foo_bar}}] != 0} {return;}
    if {[dcp_wrap {group add none -member {foo foo_bar}}] != 0} {return;}
    if {[dcp_wrap {organization add none -member {foo foo_bar}}] != 0} {return;}
    if {[dcp_wrap {account create foo -org none -group none -pass foo -mypwd $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {account create foo_bar -org none -group none -pass foo_bar -mypwd $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {login foo -password foo}] != 0} {return;}
    if {[dcp_wrap {login foo_bar -password foo_bar}] != 0} {return;}

    # Run the test
    test { logout  
           set _u }  compare_strings {foo}

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {account delete {foo foo_bar}}] != 0} {return;}
    if {[dcp_wrap {principal delete {foo foo_bar}}] != 0} {return;}
}


set iclist "ic1 "
set ic1 "logout_4_P"
set tet_startup logout_startup
set tet_cleanup logout_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
