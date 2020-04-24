#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: directory_merge_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:13:37  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:29:37  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:24  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/03/03  12:50 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts17/1  1995/03/03  12:48 UTC  truitt
# 	Synchronize the directories before the -clearinghouse
# 	option is used.  Also re-name the clearinghouse.
# 	[1995/12/11  15:52:34  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:50  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:07:49  melman]
# 
# Revision 1.1.2.3  1994/09/02  13:56:47  kevins
# 	OT 10617 modified messages and some tests.
# 	[1994/09/02  10:10:41  kevins]
# 
# Revision 1.1.2.2  1994/06/17  18:43:09  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:24:58  devsrc]
# 
# Revision 1.1.2.1  1994/05/19  14:10:54  coren
# 	Initial test drop.
# 	[1994/05/17  20:59:44  coren]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc dir_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc dir_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc directory_merge_2_P {} {
    # Identify the test
    tet_infoline directory_merge_2_P: Check that we can merge one directory into another.

    # Setup steps
    if {[dcp_wrap {dcp_dir_merge_setup}] != 0} {return;}

    # Run the test
    test { directory merge /.:/testmersrc -into /.:/testmertarg
        dcp_dir_compare_dirs /.:/testmersrc /.:/testmertarg
    } compare_strings {0}

    # Cleanup steps
    if {[dcp_wrap {directory delete /.:/testmertarg -tree}] != 0} {return;}
    if {[dcp_wrap {directory delete /.:/testmersrc -tree}] != 0} {return;}
}

proc directory_merge_4_P {} {
    # Identify the test
    tet_infoline directory_merge_4_P: Check that we can merge one subtree into another.

    # Setup steps
    if {[dcp_wrap {dcp_dir_merge_setup}] != 0} {return;}

    # Run the test
    test { directory merge /.:/testmersrc -into /.:/testmertarg -tree
        dcp_dir_compare_trees /.:/testmersrc /.:/testmertarg
    } compare_strings {0}

    # Cleanup steps
    if {[dcp_wrap {directory delete /.:/testmertarg -tree}] != 0} {return;}
    if {[dcp_wrap {directory delete /.:/testmersrc -tree}] != 0} {return;}
}

proc directory_merge_5_P {} {
    # Identify the test
    tet_infoline directory_merge_5_P: Check that attributes are set correctly on new directory.

    # Setup steps
    if {[dcp_wrap {dcp_dir_merge_setup}] != 0} {return;}
    if {[dcp_wrap {directory modify /.:/testmersrc/d1 -change {CDS_Convergence high}}] != 0} {return;}

    # Run the test
    test {directory merge /.:/testmersrc -into /.:/testmertarg
        dcp_attrlist search [directory show /.:/testmertarg/d1] CDS_Convergence
    } compare_strings {CDS_Convergence high}

    # Cleanup steps
    if {[dcp_wrap {directory delete /.:/testmertarg -tree}] != 0} {return;}
    if {[dcp_wrap {directory delete /.:/testmersrc -tree}] != 0} {return;}
}

proc directory_merge_6_P {} {
    # Identify the test
    tet_infoline directory_merge_6_P: Check that attributes are set correctly on existing directory.

    # Setup steps
    if {[dcp_wrap {dcp_dir_merge_setup}] != 0} {return;}
    if {[dcp_wrap {directory modify /.:/testmersrc/d1 -change {CDS_Convergence high}}] != 0} {return;}
    if {[dcp_wrap {directory create /.:/testmertarg/d1}] != 0} {return;}

    # Run the test
    test {directory merge /.:/testmersrc -into /.:/testmertarg
        dcp_attrlist search [directory show /.:/testmertarg/d1] CDS_Convergence
    } compare_strings {CDS_Convergence high}

    # Cleanup steps
    if {[dcp_wrap {directory delete /.:/testmertarg -tree}] != 0} {return;}
    if {[dcp_wrap {directory delete /.:/testmersrc -tree}] != 0} {return;}
}

proc directory_merge_7_P {} {
    # Identify the test
    tet_infoline directory_merge_7_P: Check that already-existing directory is not an error.

    # Setup steps
    if {[dcp_wrap {dcp_dir_merge_setup}] != 0} {return;}
    if {[dcp_wrap {directory create /.:/testmertarg/d1}] != 0} {return;}

    # Run the test
    test { directory merge /.:/testmersrc -into /.:/testmertarg
        dcp_dir_compare_dirs /.:/testmersrc /.:/testmertarg
    } compare_strings {0}

    # Cleanup steps
    if {[dcp_wrap {directory delete /.:/testmertarg -tree}] != 0} {return;}
    if {[dcp_wrap {directory delete /.:/testmersrc -tree}] != 0} {return;}
}

proc directory_merge_14_P {} {
    # Identify the test
    tet_infoline directory_merge_14_P: Check that -clearinghouse option works.

    # Setup steps
    if {[dcp_wrap {dcp_dir_merge_setup}] != 0} {return;}
    if {[dcp_wrap {clearinghouse create /.:/dirmer14_ch}] != 0} {return;}
    if {[dcp_wrap {directory synchronize /.:}] != 0} {return;}

    # Run the test
    test { directory merge /.:/testmersrc -into /.:/testmertarg -clearinghouse /.:/dirmer14_ch
        dcp_attrlist search [lindex [dcp_attrlist search [directory show /.:/testmertarg/d1] CDS_Replicas] 1] CH_Name
    } compare_strings [format "CH_Name %s" [name expand /.:/dirmer14_ch]]

    # Cleanup steps
    if {[dcp_wrap {directory delete /.:/testmertarg -tree}] != 0} {return;}
    if {[dcp_wrap {directory delete /.:/testmersrc -tree}] != 0} {return;}
    if {[dcp_wrap {directory synchronize /.:}] != 0} {return;}
    if {[dcp_wrap {clearinghouse delete /.:/dirmer14_ch}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 "
set ic1 "directory_merge_2_P"
set ic2 "directory_merge_4_P"
set ic3 "directory_merge_5_P"
set ic4 "directory_merge_6_P"
set ic5 "directory_merge_7_P"
set ic6 "directory_merge_14_P"

set tet_startup dir_startup
set tet_cleanup dir_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
