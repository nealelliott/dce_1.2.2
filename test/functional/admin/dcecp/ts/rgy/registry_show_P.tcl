#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: registry_show_P.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:23:08  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:41  marty]
#
# Revision 1.1.4.1  1995/12/11  15:13:03  root
# 	Submit
# 	[1995/12/11  14:30:59  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:32  root]
# 
# Revision 1.1.2.1  1994/09/28  21:43:57  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:38  melman]
# 
# Revision 1.1.2.8  1994/09/12  19:19:02  kevins
# 	OT 12133 modified -policies show functional test
# 	[1994/09/12  19:18:47  kevins]
# 
# Revision 1.1.2.7  1994/08/31  23:04:22  salamone
# 	CR11916 - Add _b(sec) support.
# 	[1994/08/31  23:02:54  salamone]
# 
# Revision 1.1.2.6  1994/08/26  12:43:31  kevins
# 	OT 11598 added tests for registry -policies ops
# 	[1994/08/26  11:06:24  kevins]
# 
# Revision 1.1.2.5  1994/07/22  18:03:40  salamone
# 	CR11393 - Accept setting _s(sec) with a TCL syntax string binding
# 	[1994/07/22  17:55:18  salamone]
# 
# Revision 1.1.2.4  1994/06/29  21:39:25  salamone
# 	Add -replica and -master option support.
# 	[1994/06/29  20:12:19  salamone]
# 
# Revision 1.1.2.3  1994/06/10  20:16:01  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:56:24  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  19:40:11  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:29:21  rousseau]
# 
# Revision 1.1.2.1  1994/05/11  13:48:08  kevins
# 	Initial submission
# 	[1994/05/11  13:43:51  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc registry_show_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc registry_show_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc registry_show_007_P {} {
    # Identify the test
    tet_infoline registry_show_007_P: Show registry attributes for an unauthenticated user.

    # Setup steps

    # Run the test
    test {
        dcp_found [registry show] {minorgid mingid maxuid minuid}
    } compare_strings found

    # Cleanup steps
}

proc registry_show_008_P {} {
    # Identify the test
    tet_infoline registry_show_008_P: Show registry attributes for an authenticated user
    tet_infoline registry_show_008_P: with no arguments.

    # Setup steps

    # Run the test
    test {
        dcp_found [registry show /.: -policies] {pwdalpha pwdexpdate pwdlife}
    } compare_strings found

    # cleanup steps
}

proc registry_show_009_P {} {
    # Identify the test
    tet_infoline registry_show_009_P: Show registry policies.

    # Setup steps

    # Run the test
    test {
        dcp_found [registry show -policies] {pwdalpha pwdexpdate pwdlife}
    } compare_strings found

    # Cleanup steps
}

proc registry_show_010_P {} {
    # Identify the test
    tet_infoline registry_show_010_P: Show registry attributes with argument specified as 
    tet_infoline registry_show_010_P: local cell.

    # Setup steps

    # Run the test
    test {
        dcp_found [registry show /.:] {minorgid mingid maxuid minuid}
    } compare_strings found

    # Cleanup steps
}

proc registry_show_011_P {} {
    # Identify the test
    tet_infoline registry_show_011_P: Show registry attributes with argument specified as 
    tet_infoline registry_show_011_P: cell name.

    # Setup steps
    if {[dcp_wrap {global DCP_CELLNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        dcp_found [registry show $DCP_CELLNAME_ONE] {minorgid mingid maxuid minuid}
    } compare_strings found

    # Cleanup steps
}

proc registry_show_012_P {} {
    # Identify the test
    tet_infoline registry_show_012_P: Show registry attributes with argument specified as 
    tet_infoline registry_show_012_P: the global name of a replica.

    # Setup steps
    if {[dcp_wrap {global DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}

    # Run the test
    test {
        dcp_found [registry show $DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_SLAVE1] {minorgid mingid maxuid minuid}
    } compare_strings found

    # Cleanup steps
}

proc registry_show_013_P {} {
    # Identify the test
    tet_infoline registry_show_013_P: Show registry attributes with argument specified as 
    tet_infoline registry_show_013_P: the local name of a replica.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}

    # Run the test
    test {
        dcp_found [registry show /.:/$DCP_RGY_SA_REP_NAME_SLAVE1] {minorgid mingid maxuid minuid}
    } compare_strings found

    # Cleanup steps
}

proc registry_show_014_P {} {
    # Identify the test
    tet_infoline registry_show_014_P: Show registry attributes with argument specified as 
    tet_infoline registry_show_014_P: a replica name as found in the master replica list.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}

    # Run the test
    test {
        dcp_found [registry show $DCP_RGY_SA_REP_NAME_SLAVE1] {minorgid mingid maxuid minuid}
    } compare_strings found

    # Cleanup steps
}

proc registry_show_015_P {} {
    # Identify the test
    tet_infoline registry_show_015_P: Show registry attributes with argument specified as 
    tet_infoline registry_show_015_P: a string binding in string syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_PROTSEQ_SLAVE1 DCP_RGY_SA_NETADDR_SLAVE1}] != 0} {return;}

    # Run the test
    test {
        dcp_found [registry show $DCP_RGY_SA_PROTSEQ_SLAVE1:$DCP_RGY_SA_NETADDR_SLAVE1] {minorgid mingid maxuid minuid}
    } compare_strings found

    # Cleanup steps
}

proc registry_show_016_P {} {
    # Identify the test
    tet_infoline registry_show_016_P: Show registry attributes with argument specified as 
    tet_infoline registry_show_016_P: a string binding in TCL syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_PROTSEQ_SLAVE1 DCP_RGY_SA_NETADDR_SLAVE1}] != 0} {return;}

    # Run the test
    test {
        dcp_found [registry show "$DCP_RGY_SA_PROTSEQ_SLAVE1 $DCP_RGY_SA_NETADDR_SLAVE1"] {minorgid mingid maxuid minuid}
    } compare_strings found

    # Cleanup steps
}

proc registry_show_017_P {} {
    # Identify the test
    tet_infoline registry_show_017_P: Show registry attributes of the master security server.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}

    # Run the test
    test {
        dcp_found [registry show $DCP_RGY_SA_REP_NAME_MASTER] {minorgid mingid maxuid minuid}
    } compare_strings found

    # Cleanup steps
}

proc registry_show_018_P {} {
    # Identify the test
    tet_infoline registry_show_018_P: Show registry attributes using the -attributes option.

    # Setup steps

    # Run the test
    test {
        dcp_found [registry show -attributes] {minorgid mingid maxuid minuid version}
    } compare_strings found

    # Cleanup steps
}

proc registry_show_019_P {} {
    # Identify the test
    tet_infoline registry_show_019_P: Show registry policies using the -policies option.

    # Setup steps

    # Run the test
    test {
        dcp_found [registry show -policies] {acctlife maxtktrenew pwdlife pwdspaces pwdminlen pwdalpha}
    } compare_strings found

    # Cleanup steps
}

proc registry_show_020_P {} {
    # Identify the test
    tet_infoline registry_show_020_P: Show registry information that the master keeps.

    # Setup steps
    if {[dcp_wrap {global _s DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}
    if {[dcp_wrap {set _s(sec) $DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}

    # Run the test
    test {
        dcp_found [registry show -master] {name uuid type addresses propstatus lastupdtime lastupdseqsent numupdtogo commstate lastcommstatus}
    } compare_strings found

    # Cleanup steps
}

proc registry_show_021_P {} {
    # Identify the test
    tet_infoline registry_show_021_P: Show registry information that the replica keeps. Set
    tet_infoline registry_show_021_P: _s(sec) to the slave replica.

    # Setup steps
    if {[dcp_wrap {global _s DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}
    if {[dcp_wrap {set _s(sec) $DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}

    # Run the test
    test {
        dcp_found [registry show -replica] {name type cell uuid status lastupdtime lastupdseq addresses masteraddrs masterseqnum masteruuid supportedversions}
    } compare_strings found

    # Cleanup steps
}

proc registry_show_022_P {} {
    # Identify the test
    tet_infoline registry_show_022_P: Show registry information that the replica keeps. Set
    tet_infoline registry_show_022_P: _s(sec) to the master replica.

    # Setup steps
    if {[dcp_wrap {global _s DCP_CELLNAME_ONE DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}
    if {[dcp_wrap {set _s(sec) $DCP_CELLNAME_ONE/$DCP_RGY_SA_REP_NAME_MASTER}] != 0} {return;}

    # Run the test
    test {
        dcp_found [registry show -replica] {name type cell uuid status lastupdtime lastupdseq addresses masteraddrs masterseqnum masteruuid supportedversions updseqqueue}
    } compare_strings found

    # Cleanup steps
}

proc registry_show_023_P {} {
    # Identify the test
    tet_infoline registry_show_023_P: Show registry information that the replica keeps. Set
    tet_infoline registry_show_023_P: _s(sec) to the slave replica in TCL syntax string binding.

    # Setup steps
    if {[dcp_wrap {global _s DCP_RGY_SA_PROTSEQ_SLAVE1 DCP_RGY_SA_NETADDR_SLAVE1}] != 0} {return;}
    if {[dcp_wrap {set _s(sec) "$DCP_RGY_SA_PROTSEQ_SLAVE1 $DCP_RGY_SA_NETADDR_SLAVE1"}] != 0} {return;}

    # Run the test
    test {
        dcp_found [registry show -replica] {name type cell uuid status lastupdtime lastupdseq addresses masteraddrs masterseqnum masteruuid supportedversions}
    } compare_strings found

    # Cleanup steps
}

proc registry_show_024_P {} {
    # Identify the test
    tet_infoline registry_show_024_P: Show registry information that the replica keeps. Set
    tet_infoline registry_show_024_P: _s(sec) to the slave replica in string syntax string binding.

    # Setup steps
    if {[dcp_wrap {global _s DCP_RGY_SA_PROTSEQ_SLAVE1 DCP_RGY_SA_NETADDR_SLAVE1}] != 0} {return;}
    if {[dcp_wrap {set _s(sec) "$DCP_RGY_SA_PROTSEQ_SLAVE1:$DCP_RGY_SA_NETADDR_SLAVE1"}] != 0} {return;}

    # Run the test
    test {
        dcp_found [registry show -replica] {name type cell uuid status lastupdtime lastupdseq addresses masteraddrs masterseqnum masteruuid supportedversions}
    } compare_strings found

    # Cleanup steps
}

proc registry_show_025_P {} {
    # Identify the test
    tet_infoline registry_show_025_P: Show registry information that the master keeps
    tet_infoline registry_show_025_P: for the local cell.

    # Setup steps

    # Run the test
    test {
        dcp_found [registry show /.: -master] {name uuid type addresses propstatus lastupdtime lastupdseqsent numupdtogo commstate lastcommstatus}
    } compare_strings found

    # Cleanup steps
}

proc registry_show_026_P {} {
    # Identify the test
    tet_infoline registry_show_026_P: Show registry information that the master keeps
    tet_infoline registry_show_026_P: for the specified cell.

    # Setup steps
    if {[dcp_wrap {global DCP_CELLNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        dcp_found [registry show $DCP_CELLNAME_ONE -master] {name uuid type addresses propstatus lastupdtime lastupdseqsent numupdtogo commstate lastcommstatus}
    } compare_strings found

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 "
set ic1 "registry_show_007_P"
set ic2 "registry_show_008_P"
set ic3 "registry_show_010_P"
set ic4 "registry_show_011_P"
set ic5 "registry_show_012_P"
set ic6 "registry_show_013_P"
set ic7 "registry_show_014_P"
set ic8 "registry_show_015_P"
set ic9 "registry_show_016_P"
set ic10 "registry_show_017_P"
set ic11 "registry_show_018_P"
set ic12 "registry_show_019_P"
set ic13 "registry_show_020_P"
set ic14 "registry_show_021_P"
set ic15 "registry_show_022_P"
set ic16 "registry_show_023_P"
set ic17 "registry_show_024_P"
set ic18 "registry_show_025_P"
set ic19 "registry_show_026_P"
set tet_startup registry_show_startup
set tet_cleanup registry_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
