#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: directory_modify_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:13:42  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:29:41  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:26  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/10/10  14:31 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts49/1  1995/10/10  14:30 UTC  truitt
# 	CHFts16537: Update the tests for new functionality.
# 	[1995/12/11  15:52:36  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:52  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:07:53  melman]
# 
# Revision 1.1.2.5  1994/09/02  13:56:49  kevins
# 	OT 10617 modified messages and some tests.
# 	[1994/09/02  10:10:44  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:11:19  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:27  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:35:56  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:18:27  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:16  melman
# 	updating to new API path
# 	[1994/05/06  15:20:04  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:20:56  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:40  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc dir_mod_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {clearinghouse create /.:/cdsmisc_set1_ch}] != 0} {return;}
    if {[dcp_wrap {clearinghouse create /.:/cdsmisc_set2_ch}] != 0} {return;}
}

proc dir_mod_cleanup {} {
    if {[dcp_wrap {clearinghouse delete /.:/cdsmisc_set1_ch}] != 0} {return;}
    if {[dcp_wrap {clearinghouse delete /.:/cdsmisc_set2_ch}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc directory_modify_10_P {} {
    # Identify the test
    tet_infoline directory_modify_10_P: Check that we can add an attribute to the directory.

    # Setup steps
    if {[dcp_wrap {directory create /.:/dirmod}] != 0} {return;}

    # Run the test
    test {
        directory modify /.:/dirmod -add {CDS_UpgradeTo 1.0}
        attrlist getvalues [directory show /.:/dirmod] -type CDS_UpgradeTo
    } compare_strings 1.0

    # Cleanup steps
    if {[dcp_wrap {directory delete /.:/dirmod}] != 0} {return;}
}

proc directory_modify_11_P {} {
    # Identify the test
    tet_infoline directory_modify_11_P: Check that we can change an attribute in the directory.

    # Setup steps
    if {[dcp_wrap {directory create /.:/dirmod -attribute {CDS_UpgradeTo 2.0}}] != 0} {return;}

    # Run the test
    test {
        directory modify /.:/dirmod -change {CDS_UpgradeTo 2.0}
        attrlist getvalues [directory show /.:/dirmod] -type CDS_UpgradeTo
    } compare_strings 2.0

    # Cleanup steps
    if {[dcp_wrap {directory delete /.:/dirmod}] != 0} {return;}
}

proc directory_modify_12_P {} {
    # Identify the test
    tet_infoline directory_modify_12_P: Add a list of attributes to the directory.

    # Setup steps
    if {[dcp_wrap {directory create /.:/dirmod}] != 0} {return;}

    # Run the test
    test {
        directory modify /.:/dirmod -add {{CDS_UpgradeTo 1.0} {CDS_ClassVersion 1.0}}
        dcp_multi_values [directory show /.:/dirmod] {{CDS_UpgradeTo 1.0} {CDS_ClassVersion 1.0}} 
    } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {directory delete /.:/dirmod}] != 0} {return;}
}

proc directory_modify_13_P {} {
    # Identify the test
    tet_infoline directory_modify_13_P: Change a list of attributes in the directory.

    # Setup steps
    if {[dcp_wrap {directory create /.:/dirmod}] != 0} {return;}

    # Run the test
    test {
        directory modify /.:/dirmod -change {{CDS_UpgradeTo 1.0} {CDS_ClassVersion 1.0}}
        dcp_multi_values [directory show /.:/dirmod] {{CDS_UpgradeTo 1.0} {CDS_ClassVersion 1.0}}
    } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {directory delete /.:/dirmod}] != 0} {return;}
}

proc directory_modify_14_P {} {
    # Identify the test
    tet_infoline directory_modify_14_P: Add attribites to a list of directories.

    # Setup steps
    if {[dcp_wrap {directory create {/.:/dirmod /.:/test_2}}] != 0} {return;}

    # Run the test
    test {
        directory modify {/.:/dirmod /.:/test_2} -add {CDS_UpgradeTo 1.0}
        attrlist getvalues [directory show {/.:/dirmod /.:/test_2}] -type CDS_UpgradeTo
    } compare_strings "1.0\n1.0"

    # Cleanup steps
    if {[dcp_wrap {directory delete {/.:/dirmod /.:/test_2}}] != 0} {return;}
}

proc directory_modify_15_P {} {
    global DCP_ROOT_CH

    # Identify the test
    tet_infoline directory_modify_15_P: Swap the master and readonly of a directory.

    # Setup steps
    if {[dcp_wrap {directory create /.:/cdsmisc_set_4_P_dir -clearinghouse /.:/$DCP_ROOT_CH}] != 0} {return;}
    if {[dcp_wrap {directory synch /.:}] != 0} {return;}
    if {[dcp_wrap {directory create /.:/cdsmisc_set_4_P_dir -replica -clearinghouse /.:/cdsmisc_set1_ch}] != 0} {return;}
    if {[dcp_wrap {directory synch /.:}] != 0} {return;}

    # Run the test
    test {
        directory modify /.:/cdsmisc_set_4_P_dir -master /.:/cdsmisc_set1_ch -readonly /.:/$DCP_ROOT_CH
        attrlist getvalues [directory show /.:/cdsmisc_set_4_P_dir -clearinghouse /.:/cdsmisc_set1_ch] -type CDS_ReplicaType
    } compare_strings Master

    # Cleanup steps
    if {[dcp_wrap {directory delete /.:/cdsmisc_set_4_P_dir -force}] != 0} {return;}
    if {[dcp_wrap {directory synch /.:}] != 0} {return;}
}

proc directory_modify_16_P {} {
    global DCP_ROOT_CH

    # Identify the test
    tet_infoline directory_modify_16_P: Use three clearinghouses to play around.

    # Setup steps
    if {[dcp_wrap {directory create /.:/cdsmisc_set_5_P_dir -clearinghouse /.:/$DCP_ROOT_CH}] != 0} {return;}
    if {[dcp_wrap {directory synch /.:}] != 0} {return;}
    if {[dcp_wrap {directory create /.:/cdsmisc_set_5_P_dir -replica -clearinghouse /.:/cdsmisc_set1_ch}] != 0} {return;}
    if {[dcp_wrap {directory synch /.:}] != 0} {return;}
    if {[dcp_wrap {directory create /.:/cdsmisc_set_5_P_dir -replica -clearinghouse /.:/cdsmisc_set2_ch}] != 0} {return;}
    if {[dcp_wrap {directory synch /.:}] != 0} {return;}

    # Run the test
    test {
        directory modify /.:/cdsmisc_set_5_P_dir -master /.:/cdsmisc_set1_ch -readonly "/.:/$DCP_ROOT_CH /.:/cdsmisc_set2_ch"
        attrlist getvalues [directory show /.:/cdsmisc_set_5_P_dir -clearinghouse /.:/cdsmisc_set1_ch] -type CDS_ReplicaType
    } compare_strings Master

    # Cleanup steps
    if {[dcp_wrap {directory delete /.:/cdsmisc_set_5_P_dir -force}] != 0} {return;}
    if {[dcp_wrap {directory synch /.:}] != 0} {return;}
}

proc directory_modify_17_P {} {
    global DCP_ROOT_CH

    # Identify the test
    tet_infoline directory_modify_17_P: Use three clearinghouses to play around.

    # Setup steps
    if {[dcp_wrap {directory create /.:/cdsmisc_set_6_P_dir -clearinghouse /.:/$DCP_ROOT_CH}] != 0} {return;}
    if {[dcp_wrap {directory synch /.:}] != 0} {return;}
    if {[dcp_wrap {directory create /.:/cdsmisc_set_6_P_dir -replica -clearinghouse /.:/cdsmisc_set1_ch}] != 0} {return;}
    if {[dcp_wrap {directory synch /.:}] != 0} {return;}
    if {[dcp_wrap {directory create /.:/cdsmisc_set_6_P_dir -replica -clearinghouse /.:/cdsmisc_set2_ch}] != 0} {return;}
    if {[dcp_wrap {directory synch /.:}] != 0} {return;}

    # Run the test
    test {
        directory modify /.:/cdsmisc_set_6_P_dir -master /.:/cdsmisc_set2_ch -readonly /.:/$DCP_ROOT_CH -exclude /.:/cdsmisc_set1_ch
        attrlist getvalues [directory show /.:/cdsmisc_set_6_P_dir -clearinghouse /.:/cdsmisc_set2_ch] -type CDS_ReplicaType
    } compare_strings Master

    # Cleanup steps
    if {[dcp_wrap {directory delete /.:/cdsmisc_set_6_P_dir -force}] != 0} {return;}
    if {[dcp_wrap {directory synch /.:}] != 0} {return;}
}

proc directory_modify_18_P {} {
    global DCP_ROOT_CH

    # Identify the test
    tet_infoline directory_modify_18_P: Use three clearinghouses to play around.

    # Setup steps
    if {[dcp_wrap {directory create /.:/cdsmisc_set_7_P_dir -clearinghouse /.:/cdsmisc_set1_ch}] != 0} {return;}
    if {[dcp_wrap {directory synch /.:}] != 0} {return;}
    if {[dcp_wrap {directory create /.:/cdsmisc_set_7_P_dir -replica -clearinghouse /.:/$DCP_ROOT_CH}] != 0} {return;}
    if {[dcp_wrap {directory synch /.:}] != 0} {return;}
    if {[dcp_wrap {directory create /.:/cdsmisc_set_7_P_dir -replica -clearinghouse /.:/cdsmisc_set2_ch}] != 0} {return;}
    if {[dcp_wrap {directory synch /.:}] != 0} {return;}

    # Run the test
    test {
        directory modify /.:/cdsmisc_set_7_P_dir -master /.:/$DCP_ROOT_CH -exclude {/.:/cdsmisc_set1_ch /.:/cdsmisc_set2_ch}
        attrlist getvalues [directory show /.:/cdsmisc_set_7_P_dir -clearinghouse /.:/$DCP_ROOT_CH] -type CDS_ReplicaType
    } compare_strings Master

    # Cleanup steps
    if {[dcp_wrap {directory delete /.:/cdsmisc_set_7_P_dir -force}] != 0} {return;}
    if {[dcp_wrap {directory synch /.:}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 "
set ic1 "directory_modify_10_P"
set ic2 "directory_modify_11_P"
set ic3 "directory_modify_12_P"
set ic4 "directory_modify_13_P"
set ic5 "directory_modify_14_P"
set ic6 "directory_modify_15_P"
set ic7 "directory_modify_16_P"
set ic8 "directory_modify_17_P"
set ic9 "directory_modify_18_P"
set tet_startup dir_mod_startup
set tet_cleanup dir_mod_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
