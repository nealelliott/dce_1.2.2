#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: link_modify_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:14:13  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:30:09  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:36  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:52:44  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:08  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:14  melman]
# 
# Revision 1.1.2.5  1994/09/02  18:42:11  kevins
# 	OT 10617 fixed error messages
# 	[1994/09/02  18:29:18  kevins]
# 
# Revision 1.1.2.4  1994/06/10  20:11:57  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:42  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:36:13  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:19:07  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:21:27  melman
# 	updating to new API path
# 	[1994/05/06  15:20:18  melman]
# 
# Revision 1.1.2.1  1994/03/09  19:21:12  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:50  kevins]
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
proc link_modify_1_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline link_modify_1_N: Check that extra arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {link modify /.:/link_1  extra } msg] $msg
    } compare_strings {1 {Unrecognized argument 'extra'.}}

    # Cleanup steps
}

proc link_modify_2_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline link_modify_2_N: Check that missing required argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {link modify } msg] $msg
    } compare_strings {1 {No link name.}}

    # Cleanup steps
}

proc link_modify_3_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline link_modify_3_N: Check that missing required argument is trapped as error.

    # Setup steps

    # Run the test
    test {
        list [catch {link modify /.:/link_1 } msg] $msg
    } compare_strings {1 {Option not specified.}}

    # Cleanup steps
}

proc link_modify_4_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline link_modify_4_N: Check that missing attribute list is trapped as error. 

    # Setup steps
        # Must be running as cell administrator.

    # Run the test
    test {
        list [catch {link modify /.:/link_1 -add } msg] $msg
    } compare_strings {1 {The '-add' option requires a character string value.}}

    # Cleanup steps
}

proc link_modify_5_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline link_modify_5_N: Check that missing attribute list is trapped as error.

    # Setup steps
        # Must be running as cell administrator.

    # Run the test
    test {
        list [catch {link modify /.:/link_1 -change          } msg] $msg
    } compare_strings {1 {The '-change' option requires a character string value.}}

    # Cleanup steps
}

proc link_modify_6_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline link_modify_6_N: Check that missing attribute list is trapped as error.

    # Setup steps
        # Must be running as cell administrator.

    # Run the test
    test {
        list [catch {link modify /.:/link_1 -remove                  } msg] $msg
    } compare_strings {1 {The '-remove' option requires a character string value.}}

    # Cleanup steps
}

proc link_modify_7_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW 

    # Identify the test
    tet_infoline link_modify_7_N: Check that unauthenticated users are handled correctly.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {link create /.:/link_1 -to /.:/hosts}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Run the test
    test { list [catch {link modify /.:/link_1 -change \
         {CDS_LinkTimeout  {expiration 1996} {extension 10:10:10}}} msg] $msg
    } compare_strings {1 {Insufficient rights to perform requested operation}}

    # Cleanup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {link delete /.:/link_1}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

}

proc link_modify_8_N {} {
    global TET_ROOT

    # Identify the test
    tet_infoline link_modify_8_N: Check that invalid CDS_LinkTarget value is trapped as error.

    # Setup steps


    # Run the test
    test { list [catch {link modify /.:/link_1 -change {CDS_LinkTarget foo}} msg] $msg
    } compare_strings {1 {Incomplete name}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 "
set ic1 "link_modify_1_N"
set ic2 "link_modify_2_N"
set ic3 "link_modify_3_N"
set ic4 "link_modify_4_N"
set ic5 "link_modify_5_N"
set ic6 "link_modify_6_N"
set ic7 "link_modify_7_N"
set ic8 "link_modify_8_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
