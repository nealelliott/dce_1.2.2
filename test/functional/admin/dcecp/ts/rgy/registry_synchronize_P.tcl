#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: registry_synchronize_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:23:21  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:45  marty]
#
# Revision 1.1.4.1  1995/12/11  15:13:08  root
# 	Submit
# 	[1995/12/11  14:31:02  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:36  root]
# 
# Revision 1.1.2.1  1994/09/28  21:44:06  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:43  melman]
# 
# Revision 1.1.2.2  1994/08/10  19:22:16  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:35  annie]
# 
# Revision 1.1.2.1  1994/06/29  21:39:23  salamone
# 	Initial version.
# 	[1994/06/29  20:10:09  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc registry_synchronize_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_synchronize_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc registry_synchronize_011_P {} {
    # Identify the test
    tet_infoline registry_synchronize_011_P: Synchronize the security server specified as the global
    tet_infoline registry_synchronize_011_P: name of a replica.

    # Setup steps
    if {[dcp_wrap {global DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}

    # Run the test
    test {
        registry synchronize $DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_SLAVE1
        regexp "status initializing" [eval registry dump]
    } compare_strings {1}

    # Cleanup steps
}

proc registry_synchronize_012_P {} {
    # Identify the test
    tet_infoline registry_synchronize_012_P: Synchronize the security server specified as the local
    tet_infoline registry_synchronize_012_P: name of a replica.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}

    # Run the test
    test {
        registry synchronize /.:/$DCP_RGY_SA_REP_NAME_SLAVE1
        regexp "status initializing" [eval registry dump]
    } compare_strings {1}

    # Cleanup steps
}

proc registry_synchronize_013_P {} {
    # Identify the test
    tet_infoline registry_synchronize_013_P: Synchronize the security server specified as a replica
    tet_infoline registry_synchronize_013_P: name as found in the master replica list.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}

    # Run the test
    test {
        registry synchronize $DCP_RGY_SA_REP_NAME_SLAVE1
        regexp "status initializing" [eval registry dump]
    } compare_strings {1}

    # Cleanup steps
}

proc registry_synchronize_014_P {} {
    # Identify the test
    tet_infoline registry_synchronize_014_P: Synchronize the security server specified as a string 
    tet_infoline registry_synchronize_014_P: binding in string syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_PROTSEQ_SLAVE1 DCP_RGY_SA_NETADDR_SLAVE1}] != 0} {return;}

    # Run the test
    test {
        registry synchronize $DCP_RGY_SA_PROTSEQ_SLAVE1:$DCP_RGY_SA_NETADDR_SLAVE1
        regexp "status initializing" [eval registry dump]
    } compare_strings {1}

    # Cleanup steps
}

proc registry_synchronize_015_P {} {
    # Identify the test
    tet_infoline registry_synchronize_015_P: Synchronize the security server specified as a string 
    tet_infoline registry_synchronize_015_P: binding in TCL syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_PROTSEQ_SLAVE1 DCP_RGY_SA_NETADDR_SLAVE1}] != 0} {return;}

    # Run the test
    test {
        registry synchronize "$DCP_RGY_SA_PROTSEQ_SLAVE1 $DCP_RGY_SA_NETADDR_SLAVE1"
        regexp "status initializing" [eval registry dump]
    } compare_strings {1}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 "
set ic1 "registry_synchronize_011_P"
set ic2 "registry_synchronize_012_P"
set ic3 "registry_synchronize_013_P"
set ic4 "registry_synchronize_014_P"
set ic5 "registry_synchronize_015_P"
set tet_startup registry_synchronize_startup
set tet_cleanup registry_synchronize_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
