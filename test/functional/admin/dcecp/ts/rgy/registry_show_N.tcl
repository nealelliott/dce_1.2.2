#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: registry_show_N.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:23:05  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:34:40  marty]
#
# Revision 1.1.4.1  1995/12/11  15:13:02  root
# 	Submit
# 	[1995/12/11  14:30:58  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:15:30  root]
# 
# Revision 1.1.2.1  1994/09/28  21:43:55  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:12:37  melman]
# 
# Revision 1.1.2.8  1994/09/28  17:54:53  kevins
# 	OT 12376 modified registry unavailable messages
# 	[1994/09/28  17:51:58  kevins]
# 
# Revision 1.1.2.7  1994/08/31  23:04:21  salamone
# 	CR11916 - Add _b(sec) support.
# 	[1994/08/31  23:02:52  salamone]
# 
# Revision 1.1.2.6  1994/07/22  18:03:39  salamone
# 	CR11393 - Accept setting _s(sec) with a TCL syntax string binding
# 	[1994/07/22  17:53:27  salamone]
# 
# Revision 1.1.2.5  1994/07/20  19:58:09  salamone
# 	Add test for validating specified replica.
# 	[1994/07/20  19:46:23  salamone]
# 
# Revision 1.1.2.4  1994/06/29  21:39:24  salamone
# 	Add -replica and -master option support.
# 	[1994/06/29  20:11:49  salamone]
# 
# Revision 1.1.2.3  1994/06/10  20:16:00  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:56:23  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  19:40:10  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:29:17  rousseau]
# 
# Revision 1.1.2.1  1994/05/11  13:48:07  kevins
# 	Initial submission
# 	[1994/05/11  13:43:50  kevins]
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
proc registry_show_001_N {} {
    # Identify the test
    tet_infoline registry_show_001_N: Check that only one argument can be specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry show {foo foo2}} msg] $msg
    } compare_strings {1 {Only one registry replica is allowed.}}

    # Cleanup steps
}

proc registry_show_002_N {} {
    # Identify the test
    tet_infoline registry_show_002_N: Check that only one argument can be specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry show foo foo2} msg] $msg
    } compare_strings {1 {Unrecognized argument 'foo2'.}}

    # Cleanup steps
}

proc registry_show_003_N {} {
    # Identify the test
    tet_infoline registry_show_003_N: Check for bogus option specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry show foo -} msg] $msg
    } compare_strings {1 {Ambiguous argument '-': -attributes -master -policies -replica}}

    # Cleanup steps
}

proc registry_show_004_N {} {
    # Identify the test
    tet_infoline registry_show_004_N: Check for bogus option specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry show foo -foo2} msg] $msg
    } compare_strings {1 {Unrecognized option '-foo2'.}}

    # Cleanup steps
}

proc registry_show_005_N {} {
    # Identify the test
    tet_infoline registry_show_005_N: Check for all options are mutually exclusive
    tet_infoline registry_show_005_N: for this command.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry show $DCP_RGY_SA_REP_NAME_SLAVE1 -replica -attributes} msg] $msg
    } compare_strings {1 {Mutually exclusive options entered.}}

    # Cleanup steps
}

proc registry_show_006_N {} {
    # Identify the test
    tet_infoline registry_show_006_N: Check that the replica specified with the -master
    tet_infoline registry_show_006_N: option must be a master replica.

    # Setup steps
    if {[dcp_wrap {global DCP_RGY_SA_REP_NAME_SLAVE1}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry show $DCP_RGY_SA_REP_NAME_SLAVE1 -master} msg] $msg
    } compare_strings {1 {Registry replica specified is not the master.}}

    # Cleanup steps
}

proc registry_show_007_N {} {
    # Identify the test
    tet_infoline registry_show_007_N: Check for bogus option with argument specified.

    # Setup steps

    # Run the test
    test {
        list [catch {registry show -foo} msg] $msg
    } compare_strings {1 {Registry server '-foo' is unavailable.}}

    # Cleanup steps
}

proc registry_show_008_N {} {
    # Identify the test
    tet_infoline registry_show_008_N: Check for bogus argument. 

    # Setup steps

    # Run the test
    test {
        list [catch {registry show foo} msg] $msg
    } compare_strings {1 {Registry server 'foo' is unavailable.}}

    # Cleanup steps
}

proc registry_show_009_N {} {
    # Identify the test
    tet_infoline registry_show_009_N: Check for bogus argument. 

    # Setup steps

    # Run the test
    test {
        list [catch {registry show /.:/foo} msg] $msg
    } compare_strings {1 {Registry server '/.:/foo' is unavailable.}}

    # Cleanup steps
}

proc registry_show_010_N {} {
    # Identify the test
    tet_infoline registry_show_010_N: Check for bogus argument. 

    # Setup steps
    if {[dcp_wrap {global DCP_CELLNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {registry show $DCP_CELLNAME_ONE/foo} msg] $msg
    } compare_strings "1 {Registry server '$DCP_CELLNAME_ONE/foo' is unavailable.}"

    # Cleanup steps
}

proc registry_show_011_N {} {
    # Identify the test
    tet_infoline registry_show_011_N: Check for bogus argument. 

    # Setup steps

    # Run the test
    test {
        list [catch {registry show ncacn_ip_tcp:111.11.11.11} msg] $msg
    } compare_strings {1 {Registry server 'ncacn_ip_tcp:111.11.11.11' is unavailable.}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 "
set ic1 "registry_show_001_N"
set ic2 "registry_show_002_N"
set ic3 "registry_show_003_N"
set ic4 "registry_show_004_N"
set ic5 "registry_show_005_N"
set ic6 "registry_show_006_N"
set ic7 "registry_show_007_N"
set ic8 "registry_show_008_N"
set ic9 "registry_show_009_N"
set ic10 "registry_show_010_N"
set ic11 "registry_show_011_N"
set tet_startup registry_show_startup
set tet_cleanup registry_show_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
