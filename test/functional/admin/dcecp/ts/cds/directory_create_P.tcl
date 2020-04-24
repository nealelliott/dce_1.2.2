#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: directory_create_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:13:22  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:29:21  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:17  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:52:29  root]
# 
# Revision 1.1.2.1  1994/09/28  21:38:44  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:07:39  melman]
# 
# Revision 1.1.2.5  1994/09/02  13:56:41  kevins
# 	OT 10617 modified messages and some tests.
# 	[1994/09/02  10:10:28  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:11:12  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:20  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:35:51  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:18:05  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:11  melman
# 	updating to new API path
# 	[1994/05/06  15:19:56  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:20:41  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:34  kevins]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


## Test purposes
proc directory_create_5_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_create_5_P: Check to see that we can create the directory.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test {  directory create /.:/dir_cr_1
	dcp_found [directory list /.: -directories -simple] dir_cr_1
         } compare_strings found

    # Cleanup steps
    if {[dcp_wrap {directory delete /.:/dir_cr_1}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}

proc directory_create_6_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_create_6_P: Check handling of -attribute option.

    # Setup steps
        # Must be running as cell administrator.
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test { directory create /.:/dir_cr_1 -attribute {CDS_Convergence high}
       attrlist getvalues [directory show /.:/dir_cr_1] -type CDS_Convergence
         } compare_strings high

    # Cleanup steps
    if {[dcp_wrap {directory delete /.:/dir_cr_1}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

}

proc directory_create_7_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline directory_create_7_P: Check to see that we can create a list of directorys.

    # Setup steps
        # Must be running as cell administrator.
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test { directory create {/.:/dir_cr_1 /.:/dir_cr_2}
         dcp_found [directory list /.: -directories -simple] {dir_cr_1 dir_cr_2}
	} compare_strings found

    # Cleanup steps
        # The directorys '/.:/dir_cr_1 and /.:/dir_cr_1' must be deleted 
        # from the cds namespace.
    if {[dcp_wrap {directory delete {/.:/dir_cr_1 /.:/dir_cr_2}}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 "
set ic1 "directory_create_5_P"
set ic2 "directory_create_6_P"
set ic3 "directory_create_7_P"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
