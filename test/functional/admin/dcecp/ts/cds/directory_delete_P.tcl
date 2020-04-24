#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: directory_delete_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:13:27  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:29:26  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:20  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/03/03  12:52 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts17/1  1995/03/03  12:51 UTC  truitt
# 	Fix error message when using -tree option.
# 	Add tests for -force.
# 	[1995/12/11  15:52:31  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:46  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:07:42  melman]
# 
# Revision 1.1.2.6  1994/09/02  13:56:43  kevins
# 	OT 10617 modified messages and some tests.
# 	[1994/09/02  10:10:31  kevins]
# 
# Revision 1.1.2.5  1994/06/10  20:11:15  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:23  devsrc]
# 
# Revision 1.1.2.4  1994/05/19  14:10:51  coren
# 	Merged with changes from 1.1.2.3
# 	[1994/05/19  14:10:40  coren]
# 
# 	Corrected merge error.
# 	[1994/05/17  20:59:38  coren]
# 
# 	Added -tree test (directory_delete_6_P).
# 	[1994/05/10  13:55:54  coren]
# 
# Revision 1.1.2.3  1994/05/11  19:35:53  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:18:13  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:13  melman
# 	updating to new API path
# 	[1994/05/06  15:19:59  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:20:46  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:37  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl
proc dir_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
}

proc dir_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}


## Test purposes
proc directory_delete_4_P {} {
    # Identify the test
    tet_infoline directory_delete_4_P: Check to see that we can delete the directory.

    # Setup steps
    if {[dcp_wrap {directory create /.:/dirde1}] != 0} {return;}

    # Run the test
    test { directory delete /.:/dirde1 
        dcp_missing [directory list /.: -simplename] dirde1
    } compare_strings {not found}

    # Cleanup steps
}

proc directory_delete_5_P {} {
    # Identify the test
    tet_infoline directory_delete_5_P: Check to see that we can delete a list of directories.

    # Setup steps
    if {[dcp_wrap {directory create {/.:/dirde1 /.:/dirde2}}] != 0} {return;}

    # Run the test
    test { directory delete { /.:/dirde1 /.:/dirde2 }
        dcp_missing [directory list /.: -simplename] { dirde1 dirde2 }
    } compare_strings {not found}

    # Cleanup steps
}

proc directory_delete_6_P {} {
    # Identify the test
    tet_infoline directory_delete_6_P: Check to see that we can delete a directory subtree.

    # Setup steps
    if {[dcp_wrap {directory create /.:/dirde1}] != 0} {return;}
    if {[dcp_wrap {directory create {/.:/dirde1/dirde11 /.:/dirde1/dirde12}}] != 0} {return;}
    if {[dcp_wrap {object create {/.:/dirde1/objde11 /.:/dirde1/dirde11/objde111}}] != 0} {return;}
    if {[dcp_wrap {link create /.:/dirde1/dirde11/lnde111 -to /.:/dirde12}] != 0} {return;}

    # Run the test
    test { directory delete /.:/dirde1 -tree
        dcp_missing [directory list /.: -simplename] dirde1
    } compare_strings {not found}

    # Cleanup steps
}

proc directory_delete_7_P {} {
    # Identify the test
    tet_infoline directory_delete_7_P: Check to see that the -force option works.

    # Setup steps
    if {[dcp_wrap {clearinghouse create /.:/dirdel7_ch}] != 0} {return;}
    if {[dcp_wrap {directory create /.:/dirde7}] != 0} {return;}
    if {[dcp_wrap {directory synchronize /.:}] != 0} {return;}
    if {[dcp_wrap {directory create /.:/dirde7 -replica -clearinghouse /.:/dirdel7_ch}] != 0} {return;}
    if {[dcp_wrap {directory synchronize /.:}] != 0} {return;}

    # Run the test
    test { directory delete /.:/dirde7 -force
        dcp_missing [directory list /.: -simplename] dirde7
    } compare_strings {not found}

    # Cleanup steps
    if {[dcp_wrap {directory synchronize /.:}] != 0} {return;}
    if {[dcp_wrap {clearinghouse delete /.:/dirdel7_ch}] != 0} {return;}
}

set iclist "ic1 ic2 ic3 ic4 "
set ic1 "directory_delete_4_P"
set ic2 "directory_delete_5_P"
set ic3 "directory_delete_6_P"
set ic4 "directory_delete_7_P"

set tet_startup dir_startup
set tet_cleanup dir_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
