#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: registry_disable_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:22:40  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:33  marty]
#
# Revision 1.1.4.1  1995/12/11  15:12:50  root
# 	Submit
# 	[1995/12/11  14:30:47  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:20  root]
# 
# Revision 1.1.2.1  1994/09/28  21:43:43  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:27  melman]
# 
# Revision 1.1.2.3  1994/08/10  19:22:13  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:33  annie]
# 
# Revision 1.1.2.2  1994/07/22  18:03:42  salamone
# 	CR11393 - Accept setting _s(sec) with a TCL syntax string binding
# 	[1994/07/22  17:52:33  salamone]
# 
# Revision 1.1.2.1  1994/06/29  21:39:06  salamone
# 	Initial version.
# 	[1994/06/29  21:38:31  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc registry_disable_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_disable_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc registry_disable_009_P {} {
    # Identify the test
    tet_infoline registry_disable_009_P: Disable the master replica with no arguments specified.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}

    # Run the test
    test {
        registry disable
        list [catch "principal create $name" msg] $msg
    } compare_strings {1 {bad state}}

    # Cleanup steps
    if {[dcp_wrap {registry enable}] != 0} {return;}
}

proc registry_disable_010_P {} {
    # Identify the test
    tet_infoline registry_disable_010_P: Disable the master replica specified as the local cell.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}

    # Run the test
    test {
        registry disable /.:
        list [catch "principal create $name" msg] $msg
    } compare_strings {1 {bad state}}

    # Cleanup steps
    if {[dcp_wrap {registry enable}] != 0} {return;}
}

proc registry_disable_011_P {} {
    # Identify the test
    tet_infoline registry_disable_011_P: Disable the master replica specified as cell name.

    # Setup steps
    if {[dcp_wrap {global DCP_CELLNAME_ONE}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}

    # Run the test
    test {
        registry disable $DCP_CELLNAME_ONE
        list [catch "principal create $name" msg] $msg
    } compare_strings {1 {bad state}}

    # Cleanup steps
    if {[dcp_wrap {registry enable}] != 0} {return;}
}

proc registry_disable_012_P {} {
    # Identify the test
    tet_infoline registry_disable_012_P: Disable the security server specified as the global
    tet_infoline registry_disable_012_P: name of a replica.

    # Setup steps
    if {[dcp_wrap {global DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}

    # Run the test
    test {
        registry disable $DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_MASTER
        list [catch "principal create $name" msg] $msg
    } compare_strings {1 {bad state}}

    # Cleanup steps
    if {[dcp_wrap {registry enable}] != 0} {return;}
}

proc registry_disable_013_P {} {
    # Identify the test
    tet_infoline registry_disable_013_P: Disable the security server specified as the local
    tet_infoline registry_disable_013_P: name of a replica.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}

    # Run the test
    test {
        registry disable /.:/$DCP_RGY_SA_REP_NAME_MASTER
        list [catch "principal create $name" msg] $msg
    } compare_strings {1 {bad state}}

    # Cleanup steps
    if {[dcp_wrap {registry enable}] != 0} {return;}
}

proc registry_disable_014_P {} {
    # Identify the test
    tet_infoline registry_disable_014_P: Disable the security server specified as a replica
    tet_infoline registry_disable_014_P: name as found in the master replica list.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}

    # Run the test
    test {
        registry disable $DCP_RGY_SA_REP_NAME_MASTER
        list [catch "principal create $name" msg] $msg
    } compare_strings {1 {bad state}}

    # Cleanup steps
    if {[dcp_wrap {registry enable}] != 0} {return;}
}

proc registry_disable_015_P {} {
    # Identify the test
    tet_infoline registry_disable_015_P: Disable the security server specified as a string 
    tet_infoline registry_disable_015_P: binding in string syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_PROTSEQ_MASTER DCP_RGY_SA_NETADDR_MASTER}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}

    # Run the test
    test {
        registry disable $DCP_RGY_SA_PROTSEQ_MASTER:$DCP_RGY_SA_NETADDR_MASTER
        list [catch "principal create $name" msg] $msg
    } compare_strings {1 {bad state}}

    # Cleanup steps
    if {[dcp_wrap {registry enable}] != 0} {return;}
}

proc registry_disable_016_P {} {
    # Identify the test
    tet_infoline registry_disable_016_P: Disable the security server specified as a string 
    tet_infoline registry_disable_016_P: binding in TCL syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_PROTSEQ_MASTER DCP_RGY_SA_NETADDR_MASTER}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}

    # Run the test
    test {
        registry disable "$DCP_RGY_SA_PROTSEQ_MASTER $DCP_RGY_SA_NETADDR_MASTER"
        list [catch "principal create $name" msg] $msg
    } compare_strings {1 {bad state}}

    # Cleanup steps
    if {[dcp_wrap {registry enable}] != 0} {return;}
}

proc registry_disable_017_P {} {
    # Identify the test
    tet_infoline registry_disable_017_P: Disable the security server by setting _s(sec) to a string 
    tet_infoline registry_disable_017_P: binding in TCL syntax.

    # Setup steps
    if {[dcp_wrap {global _s DCP_RGY_SA_PROTSEQ_MASTER DCP_RGY_SA_NETADDR_MASTER}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set _s(sec) "$DCP_RGY_SA_PROTSEQ_MASTER $DCP_RGY_SA_NETADDR_MASTER"}] != 0} {return;}

    # Run the test
    test {
        registry disable 
        list [catch "principal create $name" msg] $msg
    } compare_strings {1 {bad state}}

    # Cleanup steps
    if {[dcp_wrap {registry enable}] != 0} {return;}
}

proc registry_disable_018_P {} {
    # Identify the test
    tet_infoline registry_disable_018_P: Disable the security server by setting _s(sec) to a string 
    tet_infoline registry_disable_018_P: binding in string syntax.

    # Setup steps
    if {[dcp_wrap {global _s DCP_RGY_SA_PROTSEQ_MASTER DCP_RGY_SA_NETADDR_MASTER}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {set _s(sec) "$DCP_RGY_SA_PROTSEQ_MASTER:$DCP_RGY_SA_NETADDR_MASTER"}] != 0} {return;}

    # Run the test
    test {
        registry disable 
        list [catch "principal create $name" msg] $msg
    } compare_strings {1 {bad state}}

    # Cleanup steps
    if {[dcp_wrap {registry enable}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 "
set ic1 "registry_disable_009_P"
set ic2 "registry_disable_010_P"
set ic3 "registry_disable_011_P"
set ic4 "registry_disable_012_P"
set ic5 "registry_disable_013_P"
set ic6 "registry_disable_014_P"
set ic7 "registry_disable_015_P"
set ic8 "registry_disable_016_P"
set ic9 "registry_disable_017_P"
set ic10 "registry_disable_018_P"
set tet_startup registry_disable_startup
set tet_cleanup registry_disable_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
