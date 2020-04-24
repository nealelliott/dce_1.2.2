#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: registry_enable_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:22:53  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:36  marty]
#
# Revision 1.1.4.1  1995/12/11  15:12:55  root
# 	Submit
# 	[1995/12/11  14:30:51  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:24  root]
# 
# Revision 1.1.2.1  1994/09/28  21:43:48  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:31  melman]
# 
# Revision 1.1.2.3  1994/08/10  19:22:14  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:34  annie]
# 
# Revision 1.1.2.2  1994/07/22  18:03:41  salamone
# 	CR11393 - Accept setting _s(sec) with a TCL syntax string binding
# 	[1994/07/22  17:53:03  salamone]
# 
# Revision 1.1.2.1  1994/06/29  21:39:12  salamone
# 	Initial version.
# 	[1994/06/29  20:08:35  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc registry_enable_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_enable_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc registry_enable_009_P {} {
    # Identify the test
    tet_infoline registry_enable_009_P: Enable the master replica with no arguments specified.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {registry disable}] != 0} {return;}

    # Run the test
    test {
        registry enable
        list [catch "principal delete $name" msg] $msg
    } compare_strings {0 {}}

    # Cleanup steps
}

proc registry_enable_010_P {} {
    # Identify the test
    tet_infoline registry_enable_010_P: Enable the master replica specified as the local cell.

    # Setup steps
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {registry disable}] != 0} {return;}

    # Run the test
    test {
        registry enable /.:
        list [catch "principal delete $name" msg] $msg
    } compare_strings {0 {}}

    # Cleanup steps
}

proc registry_enable_011_P {} {
    # Identify the test
    tet_infoline registry_enable_011_P: Enable the master replica specified as cell name.

    # Setup steps
    if {[dcp_wrap {global DCP_CELLNAME_ONE}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {registry disable}] != 0} {return;}

    # Run the test
    test {
        registry enable $DCP_CELLNAME_ONE
        list [catch "principal delete $name" msg] $msg
    } compare_strings {0 {}}

    # Cleanup steps
}

proc registry_enable_012_P {} {
    # Identify the test
    tet_infoline registry_enable_012_P: Enable the security server specified as the global
    tet_infoline registry_enable_012_P: name of a replica.

    # Setup steps
    if {[dcp_wrap {global DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {registry disable}] != 0} {return;}

    # Run the test
    test {
        registry enable $DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_MASTER
        list [catch "principal delete $name" msg] $msg
    } compare_strings {0 {}}

    # Cleanup steps
}

proc registry_enable_013_P {} {
    # Identify the test
    tet_infoline registry_enable_013_P: Enable the security server specified as the local
    tet_infoline registry_enable_013_P: name of a replica.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {registry disable}] != 0} {return;}

    # Run the test
    test {
        registry enable /.:/$DCP_RGY_SA_REP_NAME_MASTER
        list [catch "principal delete $name" msg] $msg
    } compare_strings {0 {}}

    # Cleanup steps
}

proc registry_enable_014_P {} {
    # Identify the test
    tet_infoline registry_enable_014_P: Enable the security server specified as a replica
    tet_infoline registry_enable_014_P: name as found in the master replica list.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {registry disable}] != 0} {return;}

    # Run the test
    test {
        registry enable $DCP_RGY_SA_REP_NAME_MASTER
        list [catch "principal delete $name" msg] $msg
    } compare_strings {0 {}}

    # Cleanup steps
}

proc registry_enable_015_P {} {
    # Identify the test
    tet_infoline registry_enable_015_P: Enable the security server specified as a string 
    tet_infoline registry_enable_015_P: binding in string syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_PROTSEQ_MASTER DCP_RGY_SA_NETADDR_MASTER}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {registry disable}] != 0} {return;}

    # Run the test
    test {
        registry enable $DCP_RGY_SA_PROTSEQ_MASTER:$DCP_RGY_SA_NETADDR_MASTER
        list [catch "principal delete $name" msg] $msg
    } compare_strings {0 {}}

    # Cleanup steps
}

proc registry_enable_016_P {} {
    # Identify the test
    tet_infoline registry_enable_016_P: Enable the security server specified as a string 
    tet_infoline registry_enable_016_P: binding in TCL syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_PROTSEQ_MASTER DCP_RGY_SA_NETADDR_MASTER}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {registry disable}] != 0} {return;}

    # Run the test
    test {
        registry enable "$DCP_RGY_SA_PROTSEQ_MASTER $DCP_RGY_SA_NETADDR_MASTER"
        list [catch "principal delete $name" msg] $msg
    } compare_strings {0 {}}

    # Cleanup steps
}

proc registry_enable_017_P {} {
    # Identify the test
    tet_infoline registry_enable_017_P: Enable the master replica. Set _s(sec) to the master 
    tet_infoline registry_enable_017_P: specified in string syntax string binding.

    # Setup steps
    if {[dcp_wrap {global _s DCP_RGY_SA_PROTSEQ_MASTER DCP_RGY_SA_NETADDR_MASTER}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {registry disable}] != 0} {return;}
    if {[dcp_wrap {set _s(sec) "$DCP_RGY_SA_PROTSEQ_MASTER:$DCP_RGY_SA_NETADDR_MASTER"}] != 0} {return;}

    # Run the test
    test {
        registry enable 
        list [catch "principal delete $name" msg] $msg
    } compare_strings {0 {}}

    # Cleanup steps
}

proc registry_enable_018_P {} {
    # Identify the test
    tet_infoline registry_enable_018_P: Enable the master replica. Set _s(sec) to the master 
    tet_infoline registry_enable_018_P: specified in TCL syntax string binding.

    # Setup steps
    if {[dcp_wrap {global _s DCP_RGY_SA_PROTSEQ_MASTER DCP_RGY_SA_NETADDR_MASTER}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {registry disable}] != 0} {return;}
    if {[dcp_wrap {set _s(sec) "$DCP_RGY_SA_PROTSEQ_MASTER $DCP_RGY_SA_NETADDR_MASTER"}] != 0} {return;}

    # Run the test
    test {
        registry enable 
        list [catch "principal delete $name" msg] $msg
    } compare_strings {0 {}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 "
set ic1 "registry_enable_009_P"
set ic2 "registry_enable_010_P"
set ic3 "registry_enable_011_P"
set ic4 "registry_enable_012_P"
set ic5 "registry_enable_013_P"
set ic6 "registry_enable_014_P"
set ic7 "registry_enable_015_P"
set ic8 "registry_enable_016_P"
set ic9 "registry_enable_017_P"
set ic10 "registry_enable_018_P"
set tet_startup registry_enable_startup
set tet_cleanup registry_enable_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
