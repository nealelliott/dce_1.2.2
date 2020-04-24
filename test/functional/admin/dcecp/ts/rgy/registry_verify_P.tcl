#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: registry_verify_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:23:27  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:46  marty]
#
# Revision 1.1.4.1  1995/12/11  15:13:11  root
# 	Submit
# 	[1995/12/11  14:31:04  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:38  root]
# 
# Revision 1.1.2.1  1994/09/28  21:44:10  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:46  melman]
# 
# Revision 1.1.2.2  1994/08/10  19:22:18  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:37  annie]
# 
# Revision 1.1.2.1  1994/07/20  19:58:14  salamone
# 	Initial version.
# 	[1994/07/20  19:43:00  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc registry_verify_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_verify_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc registry_verify_004_P {} {
    global _s DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_MASTER DCP_RGY_SA_REP_NAME_SLAVE1

    # Identify the test
    tet_infoline registry_verify_004_P: Return replica information for each replica in
    tet_infoline registry_verify_004_P: the cell for an authenticated user.
    tet_infoline registry_verify_004_P: Set the _s(sec) convenience variable to the master.

    # Setup steps
    if {[dcp_wrap {set _s(sec) $DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {registry verify}

    # Cleanup steps
}

proc registry_verify_005_P {} {
    global _s DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_MASTER DCP_RGY_SA_REP_NAME_SLAVE1

    # Identify the test
    tet_infoline registry_verify_005_P: Return replica information for each replica in the
    tet_infoline registry_verify_005_P: cell for an authenticated user. Set the _s(sec)
    tet_infoline registry_verify_005_P: convenience variable to the slave replica.

    # Setup steps
    if {[dcp_wrap {set _s(sec) $DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {registry verify}

    # Cleanup steps
}

proc registry_verify_006_P {} {
    global _s DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_MASTER DCP_RGY_SA_REP_NAME_SLAVE1

    # Identify the test
    tet_infoline registry_verify_006_P: Return replica information for each replica in
    tet_infoline registry_verify_006_P: the cell for an authenticated user.
    tet_infoline registry_verify_006_P: Unset the _s(sec) convenience variable.

    # Setup steps
    if {[dcp_wrap {unset _s(sec)}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {registry verify}

    # Cleanup steps
}

proc registry_verify_007_P {} {
    global _s DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_MASTER DCP_RGY_SA_REP_NAME_SLAVE1

    # Identify the test
    tet_infoline registry_verify_007_P: Return replica information for each replica in
    tet_infoline registry_verify_007_P: the cell for an unauthenticated user.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set _s(sec) $DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {registry verify}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_verify_008_P {} {
    global _s DCP_CELLNAME_ONE

    # Identify the test
    tet_infoline registry_verify_008_P: Call the verify command with a legal argument.

    # Setup steps
    if {[dcp_wrap {unset _s(sec)}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {registry verify $DCP_CELLNAME_ONE}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 "
set ic1 "registry_verify_004_P"
set ic2 "registry_verify_005_P"
set ic3 "registry_verify_006_P"
set ic4 "registry_verify_007_P"
set ic5 "registry_verify_008_P"
set tet_startup registry_verify_startup
set tet_cleanup registry_verify_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
