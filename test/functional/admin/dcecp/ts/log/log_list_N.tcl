#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: log_list_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:17:19  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:23  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:40  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/12/12  18:13 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_msg_chk/1  1994/12/12  18:12 UTC  truitt
# 	Fix incorrect error message.
# 	[1995/12/11  15:53:42  root]
# 
# Revision 1.1.2.2  1994/10/01  01:33:13  rousseau
# 	Dynamically generate string bindings.
# 	[1994/10/01  01:27:34  rousseau]
# 
# Revision 1.1.2.1  1994/09/28  21:40:42  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:57  melman]
# 
# Revision 1.1.2.6  1994/09/20  16:38:26  salamone
# 	Use new string binding constants for dts server.
# 	[1994/09/20  16:37:57  salamone]
# 
# Revision 1.1.2.5  1994/09/08  14:00:59  salamone
# 	CR11968-Setting authentication on binding handles.
# 	[1994/09/08  13:59:29  salamone]
# 
# Revision 1.1.2.4  1994/06/10  20:13:51  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:45  devsrc]
# 
# Revision 1.1.2.3  1994/05/11  19:38:11  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:22:27  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:41:29  melman
# 	updating to new API path
# 	[1994/05/06  15:41:10  melman]
# 
# Revision 1.1.2.1  1994/05/02  19:26:12  salamone
# 	Initial version.
# 	[1994/04/29  20:24:10  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc log_list_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}

    # Set up variables for string binding tests
    dcp_log_binding_vars
}

proc log_list_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc log_list_001_N {} {
    # Identify the test
    tet_infoline log_list_001_N: Check for an insufficient number of arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {log list} msg] $msg
    } compare_strings {1 {Server process not specified.}}

    # Cleanup steps
}

proc log_list_002_N {} {
    # Identify the test
    tet_infoline log_list_002_N: Check for a bogus server process.

    # Setup steps

    # Run the test
    test {
        list [catch {log list /.:/foo} msg] $msg
    } compare_strings {1 {Entry not found}}

    # Cleanup steps
}

proc log_list_003_N {} {
    # Identify the test
    tet_infoline log_list_003_N: Check for a bogus server in a list.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {log list "$DCP_LOG_SRVR_ONE /.:/foo $DCP_LOG_SRVR_TWO"} msg] $msg
    } compare_strings {1 {Entry not found}}

    # Cleanup steps
}

proc log_list_004_N {} {
    # Identify the test
    tet_infoline log_list_004_N: Check for a bogus list of ACL objects.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {log list $DCP_LOG_SRVR_ONE $DCP_LOG_SRVR_TWO} msg] $msg
    } compare_strings {1 {Too many arguments specified in command.}}

    # Cleanup steps
}

proc log_list_005_N {} {
    # Identify the test
    tet_infoline log_list_005_N: Check for an invalid option with no name.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {log list $DCP_LOG_SRVR_ONE -} msg] $msg
    } compare_strings {1 {The '-comp' option requires a character string value.}}

    # Cleanup steps
}

proc log_list_006_N {} {
    # Identify the test
    tet_infoline log_list_006_N: Check for an invalid option.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {log list $DCP_LOG_SRVR_ONE -foo} msg] $msg
    } compare_strings {1 {Too many arguments specified in command.}}

    # Cleanup steps
}

proc log_list_007_N {} {
    # Identify the test
    tet_infoline log_list_007_N: Check that the option -comp requires values.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {log list $DCP_LOG_SRVR_ONE -comp} msg] $msg
    } compare_strings {1 {The '-comp' option requires a character string value.}}

    # Cleanup steps
}

proc log_list_008_N {} {
    # Identify the test
    tet_infoline log_list_008_N: Check for a bogus component name.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {log list $DCP_LOG_SRVR_ONE -comp foo} msg] $msg
    } compare_strings {1 {Unknown component}}

    # Cleanup steps
}

proc log_list_009_N {} {
    # Identify the test
    tet_infoline log_list_009_N: Check for a bogus list of components.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {log list $DCP_LOG_SRVR_ONE -comp dts cds} msg] $msg
    } compare_strings {1 {Too many arguments specified in command.}}

    # Cleanup steps
}

proc log_list_010_N {} {
    # Identify the test
    tet_infoline log_list_010_N: Check that the string binding is complete.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_OBJECT_UUID DCP_LOG_PROTSEQ DCP_LOG_NETADDR}] != 0} {return;}

    # Run the test
    test {
        list [catch {log list $DCP_LOG_OBJECT_UUID@$DCP_LOG_PROTSEQ:$DCP_LOG_NETADDR} msg] $msg
    } compare_strings {1 {Binding incomplete (no object ID and no endpoint)}}

    # Cleanup steps
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 "
set ic1 "log_list_001_N"
set ic2 "log_list_002_N"
set ic3 "log_list_003_N"
set ic4 "log_list_004_N"
set ic5 "log_list_005_N"
set ic6 "log_list_006_N"
set ic7 "log_list_007_N"
set ic8 "log_list_008_N"
set ic9 "log_list_009_N"
set ic10 "log_list_010_N"
set tet_startup log_list_startup
set tet_cleanup log_list_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
