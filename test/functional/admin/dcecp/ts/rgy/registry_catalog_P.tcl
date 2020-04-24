#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: registry_catalog_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:22:14  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:23  marty]
#
# Revision 1.1.4.1  1995/12/13  15:28:45  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/09/11  16:05 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/truitt_dcecp_chfts46/1  1995/09/11  16:04 UTC  truitt
# 	CHFts16100: Additional tests to support new functionality.
# 	[1995/12/11  15:55:28  root]
# 
# Revision 1.1.2.1  1994/09/28  21:43:38  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:22  melman]
# 
# Revision 1.1.2.3  1994/08/31  23:04:20  salamone
# 	CR11916 - Add _b(sec) support.
# 	[1994/08/31  23:02:50  salamone]
# 
# Revision 1.1.2.2  1994/08/15  17:28:42  salamone
# 	CR11636: Allow registry catalog to take an argument.
# 	[1994/08/15  17:23:14  salamone]
# 
# Revision 1.1.2.1  1994/06/29  21:38:56  salamone
# 	Initial version.
# 	[1994/06/29  20:05:50  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc registry_catalog_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_catalog_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc registry_catalog_005_P {} {
    global _s DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_MASTER DCP_RGY_SA_REP_NAME_SLAVE1

    # Identify the test
    tet_infoline registry_catalog_005_P: Show a list of fully qualified names of registries in
    tet_infoline registry_catalog_005_P: the local cell for an authenticated user. Set the
    tet_infoline registry_catalog_005_P: _s(sec) convenience variable to the slave replica.

    # Setup steps
    if {[dcp_wrap {set _s(sec) $DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}

    # Run the test
    test {
        lsort [registry catalog]
    } compare_strings [lsort "$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_SLAVE1\n$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_MASTER"]

    # Cleanup steps
}

proc registry_catalog_006_P {} {
    global _s DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_MASTER DCP_RGY_SA_REP_NAME_SLAVE1

    # Identify the test
    tet_infoline registry_catalog_006_P: Show a list of fully qualified names of registries 
    tet_infoline registry_catalog_006_P: in the local cell for an authenticated user.
    tet_infoline registry_catalog_006_P: Unset the _s(sec) convenience variable.

    # Setup steps
    if {[dcp_wrap {unset _s(sec)}] != 0} {return;}

    # Run the test
    test {
        lsort [registry catalog]
    } compare_strings [lsort "$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_SLAVE1\n$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_MASTER"]

    # Cleanup steps
}

proc registry_catalog_007_P {} {
    global _s DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_MASTER DCP_RGY_SA_REP_NAME_SLAVE1

    # Identify the test
    tet_infoline registry_catalog_007_P: Show a list of fully qualified names of registries 
    tet_infoline registry_catalog_007_P: in the local cell for an unauthenticated user.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set _s(sec) $DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test {
        lsort [registry catalog]
    } compare_strings [lsort "$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_MASTER\n$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_SLAVE1"]

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_catalog_009_P {} {
    global _s DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_MASTER DCP_RGY_SA_REP_NAME_SLAVE1

    # Identify the test
    tet_infoline registry_catalog_009_P: Show a list of fully qualified names of registries in
    tet_infoline registry_catalog_009_P: the local cell for an authenticated user with no arg.
    tet_infoline registry_catalog_009_P: Set the _s(sec) convenience variable to the master.

    # Setup steps
    if {[dcp_wrap {set _s(sec) $DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}

    # Run the test
    test {
        lsort [registry catalog]
    } compare_strings [lsort "$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_MASTER\n$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_SLAVE1"]

    # Cleanup steps
}

proc registry_catalog_010_P {} {
    global DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_MASTER DCP_RGY_SA_REP_NAME_SLAVE1

    # Identify the test
    tet_infoline registry_catalog_010_P: Show a list of fully qualified names of registries 
    tet_infoline registry_catalog_010_P: in the local cell for an authenticated user with 
    tet_infoline registry_catalog_010_P: argument specified as local cell.

    # Setup steps

    # Run the test
    test {
        lsort [registry catalog /.:]
    } compare_strings [lsort "$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_MASTER\n$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_SLAVE1"]

    # Cleanup steps
}

proc registry_catalog_011_P {} {
    global DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_MASTER DCP_RGY_SA_REP_NAME_SLAVE1

    # Identify the test
    tet_infoline registry_catalog_011_P: Show a list of fully qualified names of registries 
    tet_infoline registry_catalog_011_P: in the local cell for an authenticated user with 
    tet_infoline registry_catalog_011_P: argument specified as cell name.

    # Setup steps

    # Run the test
    test {
        lsort [registry catalog $DCP_CELLNAME_ONE]
    } compare_strings [lsort "$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_MASTER\n$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_SLAVE1"]

    # Cleanup steps
}

proc registry_catalog_012_P {} {
    global DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_MASTER DCP_RGY_SA_REP_NAME_SLAVE1

    # Identify the test
    tet_infoline registry_catalog_012_P: Show a list of fully qualified names of registries 
    tet_infoline registry_catalog_012_P: in the local cell for an authenticated user with 
    tet_infoline registry_catalog_012_P: argument specified as global name of replica. 

    # Setup steps

    # Run the test
    test {
        lsort [registry catalog $DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_MASTER]
    } compare_strings [lsort "$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_MASTER\n$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_SLAVE1"]

    # Cleanup steps
}

proc registry_catalog_013_P {} {
    global DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_MASTER DCP_RGY_SA_REP_NAME_SLAVE1

    # Identify the test
    tet_infoline registry_catalog_013_P: Show a list of fully qualified names of registries 
    tet_infoline registry_catalog_013_P: in the local cell for an authenticated user with 
    tet_infoline registry_catalog_013_P: argument specified as local name of replica. 

    # Setup steps

    # Run the test
    test {
        lsort [registry catalog /.:/$DCP_RGY_SA_REP_NAME_MASTER]
    } compare_strings [lsort "$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_MASTER\n$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_SLAVE1"]

    # Cleanup steps
}

proc registry_catalog_014_P {} {
    global DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_MASTER DCP_RGY_SA_REP_NAME_SLAVE1

    # Identify the test
    tet_infoline registry_catalog_014_P: Show a list of fully qualified names of registries in the
    tet_infoline registry_catalog_014_P: local cell for an authenticated user with the argument
    tet_infoline registry_catalog_014_P: specified as a replica name as found in the master rep list.

    # Setup steps

    # Run the test
    test {
        lsort [registry catalog $DCP_RGY_SA_REP_NAME_MASTER]
    } compare_strings [lsort "$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_MASTER\n$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_SLAVE1"]

    # Cleanup steps
}

proc registry_catalog_015_P {} {
    global DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_MASTER DCP_RGY_SA_REP_NAME_SLAVE1

    # Identify the test
    tet_infoline registry_catalog_015_P: Show a list of fully qualified names of registries 
    tet_infoline registry_catalog_015_P: in the local cell for an authenticated user with 
    tet_infoline registry_catalog_015_P: argument specified as a string binding in string syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_PROTSEQ_MASTER DCP_RGY_SA_NETADDR_MASTER}] != 0} {return;}

    # Run the test
    test {
        lsort [registry catalog $DCP_RGY_SA_PROTSEQ_MASTER:$DCP_RGY_SA_NETADDR_MASTER]
    } compare_strings [lsort "$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_MASTER\n$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_SLAVE1"]

    # Cleanup steps
}

proc registry_catalog_016_P {} {
    global DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_MASTER DCP_RGY_SA_REP_NAME_SLAVE1

    # Identify the test
    tet_infoline registry_catalog_016_P: Show a list of fully qualified names of registries 
    tet_infoline registry_catalog_016_P: in the local cell for an authenticated user with 
    tet_infoline registry_catalog_016_P: argument specified as a string binding in TCL syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_PROTSEQ_MASTER DCP_RGY_SA_NETADDR_MASTER}] != 0} {return;}

    # Run the test
    test {
        lsort [registry catalog "$DCP_RGY_SA_PROTSEQ_MASTER $DCP_RGY_SA_NETADDR_MASTER"]
    } compare_strings [lsort "$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_MASTER\n$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_SLAVE1"]

    # Cleanup steps
}

proc registry_catalog_017_P {} {
    global _s DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_MASTER DCP_RGY_SA_REP_NAME_SLAVE1

    # Identify the test
    tet_infoline registry_catalog_017_P: Show a list of fully qualified names of registries 
    tet_infoline registry_catalog_017_P: in the local cell for an authenticated user by setting 
    tet_infoline registry_catalog_017_P: _s(sec) to a string binding in TCL syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_PROTSEQ_MASTER DCP_RGY_SA_NETADDR_MASTER}] != 0} {return;}
    if {[dcp_wrap {set _s(sec) "$DCP_RGY_SA_PROTSEQ_MASTER $DCP_RGY_SA_NETADDR_MASTER"}] != 0} {return;}

    # Run the test
    test {
        lsort [registry catalog]
    } compare_strings [lsort "$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_MASTER\n$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_SLAVE1"]

    # Cleanup steps
}

proc registry_catalog_018_P {} {
    global _s DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_MASTER DCP_RGY_SA_REP_NAME_SLAVE1

    # Identify the test
    tet_infoline registry_catalog_018_P: Show a list of fully qualified names of registries 
    tet_infoline registry_catalog_018_P: in the local cell for an authenticated user by setting 
    tet_infoline registry_catalog_018_P: _s(sec) to a string binding in string syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_PROTSEQ_MASTER DCP_RGY_SA_NETADDR_MASTER}] != 0} {return;}
    if {[dcp_wrap {set _s(sec) "$DCP_RGY_SA_PROTSEQ_MASTER:$DCP_RGY_SA_NETADDR_MASTER"}] != 0} {return;}

    # Run the test
    test {
        lsort [registry catalog]
    } compare_strings [lsort "$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_MASTER\n$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_SLAVE1"]

    # Cleanup steps
}

proc registry_catalog_019_P {} {
    global _s DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_MASTER

    # Identify the test
    tet_infoline registry_catalog_019_P: Return only the master replica.

    # Setup steps
    if {[dcp_wrap {unset _s(sec)}] != 0} {return;}

    # Run the test
    test {
        lsort [registry catalog -master]
    } compare_strings "$DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_MASTER"

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 "
set ic1 "registry_catalog_005_P"
set ic2 "registry_catalog_006_P"
set ic3 "registry_catalog_007_P"
set ic4 "registry_catalog_009_P"
set ic5 "registry_catalog_010_P"
set ic6 "registry_catalog_011_P"
set ic7 "registry_catalog_012_P"
set ic8 "registry_catalog_013_P"
set ic9 "registry_catalog_014_P"
set ic10 "registry_catalog_015_P"
set ic11 "registry_catalog_016_P"
set ic12 "registry_catalog_017_P"
set ic13 "registry_catalog_018_P"
set ic14 "registry_catalog_019_P"
set tet_startup registry_catalog_startup
set tet_cleanup registry_catalog_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
