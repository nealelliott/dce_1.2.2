#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: keytab_create_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:16:43  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:08  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:27  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/06/08  14:03 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts33/1  1995/06/08  14:02 UTC  truitt
# 	CHFts15424: Change some tests to support new dced acl behavior.
# 	[1995/12/11  15:53:29  root]
# 
# Revision 1.1.2.2  1994/09/29  22:01:34  baum
# 	Fixed negative messages (OT 12370)
# 	[1994/09/29  22:01:07  baum]
# 
# Revision 1.1.2.1  1994/09/28  21:40:24  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:37  melman]
# 
# Revision 1.1.2.8  1994/09/01  15:44:04  baum
# 	Fixed output messages in negative tests (OT 11093)
# 	[1994/09/01  15:42:38  baum]
# 
# Revision 1.1.2.7  1994/06/22  20:37:13  baum
# 	OT #11022 - Use tetexec.cfg for cell/host.  Use tet_setup
# 	[1994/06/22  20:36:06  baum]
# 
# Revision 1.1.2.6  1994/06/10  20:13:16  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:26  devsrc]
# 
# Revision 1.1.2.5  1994/06/07  23:13:50  baum
# 	Mods to handle new dcecp output (CR #10761)
# 	[1994/06/07  23:13:21  baum]
# 
# Revision 1.1.2.4  1994/05/25  22:42:56  baum
# 	Keytab tests now dce_login/out on their own.  Also, corrected some of the output errors
# 	[1994/05/25  22:41:34  baum]
# 
# Revision 1.1.2.3  1994/05/19  23:06:23  baum
# 	Resync with changes made to commands and command output
# 	[1994/05/19  23:05:50  baum]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_setup.tcl

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


## Test purposes
proc keytab_create_1_N {} {

    # Identify the test
    tet_infoline keytab_create_1_N: Check insufficient/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab create} msg] $msg
    } compare_strings {1 {'dced' object not specified.}}

    # Cleanup steps
}

proc keytab_create_2_N {} {

    # Identify the test
    tet_infoline keytab_create_2_N: Check insufficient/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab create name} msg] $msg
    } compare_strings {1 {Required option 'attribute or storage' not entered.}}

    # Cleanup steps
}

proc keytab_create_3_N {} {

    # Identify the test
    tet_infoline keytab_create_3_N: Check insufficient/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab create name -attr} msg] $msg
    } compare_strings {1 {The '-attribute' option requires a character string value.}}

    # Cleanup steps
}

proc keytab_create_4_N {} {

    # Identify the test
    tet_infoline keytab_create_4_N: Check insufficient/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab create name -uuid uuid} msg] $msg
    } compare_strings {1 {Required option 'attribute or storage' not entered.}}

    # Cleanup steps
}

proc keytab_create_5_N {} {

    # Identify the test
    tet_infoline keytab_create_5_N: Check insufficient/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab create name -uuid uuid -storage storage} msg] $msg
    } compare_strings {1 {Required option 'data' not entered.}}

    # Cleanup steps
}

proc keytab_create_6_N {} {

    # Identify the test
    tet_infoline keytab_create_6_N: Check insufficient/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab create name -uuid uuid -storage storage -data} msg] $msg
    } compare_strings {1 {The '-data' option requires a character string value.}}

    # Cleanup steps
}

proc keytab_create_7_N {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline keytab_create_7_N: Check that exclusive options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab create name -uuid 007a1fe9-1022-1e07-a3f6-0000c0dc0d4b -storage storage -data {$DCP_CLIENT plain 1 $DCP_CLIENT_PW } -entry} msg] $msg
    } compare_strings {1 {Data attribute specified on entry only operation.}}

    # Cleanup steps
}

proc keytab_create_8_N {} {

    # Identify the test
    tet_infoline keytab_create_8_N: Check that extra/invalid arguments are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab create name -attr attribute -entry foo} msg] $msg
    } compare_strings {1 {Too many arguments specified in command.}}

    # Cleanup steps
}

proc keytab_create_9_N {} {

    # Identify the test
    tet_infoline keytab_create_9_N: Check that extra/invalid options are trapped as errors.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab create name -attr attribute -foo} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo'.}}

    # Cleanup steps
}

proc keytab_create_10_N {} {

    # Identify the test
    tet_infoline keytab_create_10_N: Check that unauthenticated users are handled correctly.

    # Setup steps

    # Run the test
    test {
        list [catch {keytab create name -storage keytab -annotation annotation -entry} msg] $msg
    } compare_strings {1 {Failed to find file on dced import attempt}}

    # Cleanup steps
}

proc keytab_create_11_N {} {
    global DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline keytab_create_11_N: Check that unauthenticated users are handled correctly.

    # Setup steps
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {keytab create name -storage keytab -annotation annotation -entry} msg] $msg
    } compare_strings {1 {permission not valid for this acl}}

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 "
set ic1 "keytab_create_1_N"
set ic2 "keytab_create_2_N"
set ic3 "keytab_create_3_N"
set ic4 "keytab_create_4_N"
set ic5 "keytab_create_5_N"
set ic6 "keytab_create_6_N"
set ic7 "keytab_create_7_N"
set ic8 "keytab_create_8_N"
set ic9 "keytab_create_9_N"
set ic10 "keytab_create_10_N"
set ic11 "keytab_create_11_N"

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
