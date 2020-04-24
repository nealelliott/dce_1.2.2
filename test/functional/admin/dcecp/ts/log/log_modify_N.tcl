#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: log_modify_N.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:17:25  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:25  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:42  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1995/06/08  13:38 UTC  truitt
# 	Merge test changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts33/1  1995/06/08  13:34 UTC  truitt
# 	CHFts15424: Change tests to reflect new dced acl behavior.
# 
# 	HP revision /main/HPDCE02/2  1994/12/12  18:11 UTC  truitt
# 	Fix incorrect error message.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_msg_chk/1  1994/12/12  18:10 UTC  truitt
# 	Fix incorrect error message.
# 
# 	HP revision /main/HPDCE02/1  1994/11/08  18:41 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/08  18:40 UTC  truitt
# 	Clean up the test case which checks DCP_LOG_DCE_DEBUG.
# 	[1995/12/11  15:53:44  root]
# 
# Revision 1.1.2.2  1994/10/01  01:33:15  rousseau
# 	Dynamically generate string bindings.
# 	[1994/10/01  01:27:38  rousseau]
# 
# Revision 1.1.2.1  1994/09/28  21:40:45  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:10:00  melman]
# 
# Revision 1.1.2.8  1994/09/27  17:34:58  salamone
# 	CR12362 - Return UNINITIATED if DCP_LOG_DCE_DEBUG is 0.
# 	[1994/09/27  17:24:59  salamone]
# 
# Revision 1.1.2.7  1994/09/20  16:38:27  salamone
# 	Use new string binding constants for dts server.
# 	[1994/09/20  16:37:58  salamone]
# 
# Revision 1.1.2.6  1994/09/08  14:01:02  salamone
# 	CR11968-Setting authentication on binding handles.
# 	[1994/09/08  13:59:32  salamone]
# 
# Revision 1.1.2.5  1994/06/10  20:13:53  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:47  devsrc]
# 
# Revision 1.1.2.4  1994/05/18  20:41:07  salamone
# 	CR10636 - dcecp "log show" and "log list" should display the
# 		  appropriate info if unauthenticated.
# 	[1994/05/13  22:27:44  salamone]
# 
# Revision 1.1.2.3  1994/05/11  19:38:17  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:22:34  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:41:32  melman
# 	updating to new API path
# 	[1994/05/06  15:41:13  melman]
# 
# Revision 1.1.2.1  1994/05/02  19:26:18  salamone
# 	Initial version.
# 	[1994/04/29  20:24:31  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

# Test purpose common routines
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl


proc log_modify_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}

    # Set up variables for string binding tests
    dcp_log_binding_vars
}

proc log_modify_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc log_modify_001_N {} {
    # Identify the test
    tet_infoline log_modify_001_N: Check for an insufficient number of arguments.

    # Setup steps

    # Run the test
    test {
        list [catch {log modify} msg] $msg
    } compare_strings {1 {Server process not specified.}}

    # Cleanup steps
}

proc log_modify_002_N {} {
    # Identify the test
    tet_infoline log_modify_002_N: Check for a bogus server process.

    # Setup steps

    # Run the test
    test {
        list [catch {log modify /.:/foo -ch FATAL:DISCARD:} msg] $msg
    } compare_strings {1 {Entry not found}}

    # Cleanup steps
}

proc log_modify_003_N {} {
    # Identify the test
    tet_infoline log_modify_003_N: Check for a bogus server in a modify.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {log modify "$DCP_LOG_SRVR_ONE /.:/foo $DCP_LOG_SRVR_TWO" -ch FATAL:DISCARD:} msg] $msg
    } compare_strings {1 {Entry not found}}

    # Cleanup steps
}

proc log_modify_004_N {} {
    # Identify the test
    tet_infoline log_modify_004_N: Check for a bogus modify of ACL objects.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {log modify $DCP_LOG_SRVR_ONE $DCP_LOG_SRVR_TWO} msg] $msg
    } compare_strings {1 {Too many arguments specified in command.}}

    # Cleanup steps
}

proc log_modify_005_N {} {
    # Identify the test
    tet_infoline log_modify_005_N: Check that -change option is specified.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {log modify $DCP_LOG_SRVR_ONE} msg] $msg
    } compare_strings {1 {Requires more options.}}

    # Cleanup steps
}

proc log_modify_006_N {} {
    # Identify the test
    tet_infoline log_modify_006_N: Check for an invalid option with no name.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {log modify $DCP_LOG_SRVR_ONE -} msg] $msg
    } compare_strings {1 {The '-change' option requires a character string value.}}

    # Cleanup steps
}

proc log_modify_007_N {} {
    # Identify the test
    tet_infoline log_modify_007_N: Check for an invalid option.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {log modify $DCP_LOG_SRVR_ONE -foo} msg] $msg
    } compare_strings {1 {Too many arguments specified in command.}}

    # Cleanup steps
}

proc log_modify_008_N {} {
    # Identify the test
    tet_infoline log_modify_008_N: Check that the option -change requires routing
    tet_infoline log_modify_008_N: specifications.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {log modify $DCP_LOG_SRVR_ONE -change} msg] $msg
    } compare_strings {1 {The '-change' option requires a character string value.}}

    # Cleanup steps
}

proc log_modify_009_N {} {
    # Identify the test
    tet_infoline log_modify_009_N: Check that the routing specification is valid.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {log modify $DCP_LOG_SRVR_ONE -change foo} msg] $msg
    } compare_strings {1 {Bad routing specification.}}

    # Cleanup steps
}

proc log_modify_010_N {} {
    # Identify the test
    tet_infoline log_modify_010_N: Check that the debug routing specification has a valid
    tet_infoline log_modify_010_N: sub-component name.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {log modify $DCP_LOG_SRVR_ONE -change $DCP_LOG_COMP_ONE:foo.1:DISCARD:} msg] $msg
    } compare_strings {1 {Bad routing specification}}

    # Cleanup steps
}

proc log_modify_011_N {} {
    # Identify the test
    tet_infoline log_modify_011_N: Check that the debug routing specification has a valid
    tet_infoline log_modify_011_N: level number.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {log modify $DCP_LOG_SRVR_ONE -change $DCP_LOG_COMP_ONE:general.10:DISCARD:} msg] $msg
    } compare_strings {1 {Bad routing specification}}

    # Cleanup steps
}

proc log_modify_012_N {} {
    # Identify the test
    tet_infoline log_modify_012_N: Check that the debug routing specification has a valid
    tet_infoline log_modify_012_N: "flags" field.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {log modify $DCP_LOG_SRVR_ONE -change $DCP_LOG_COMP_ONE:foo:TEXTFILE:/tmp/j} msg] $msg
    } compare_strings {1 {Bad routing specification}}

    # Cleanup steps
}

proc log_modify_013_N {} {
    # Identify the test
    tet_infoline log_modify_013_N: Check that the debug routing specification has a valid
    tet_infoline log_modify_013_N: "flags" field specified in TCL syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {log modify $DCP_LOG_SRVR_ONE -change [list "$DCP_LOG_COMP_ONE general 1 TEXTFILE /tmp/j"]} msg] $msg
    } compare_strings {1 {Bad routing specification.}}

    # Cleanup steps
}

proc log_modify_014_N {} {
    # Identify the test
    tet_infoline log_modify_014_N: Check that the debug routing specification has a valid
    tet_infoline log_modify_014_N: "flags" field.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {log modify $DCP_LOG_SRVR_ONE -change $DCP_LOG_COMP_ONE:foo,foo2:TEXTFILE:/tmp/j} msg] $msg
    } compare_strings {1 {Bad routing specification}}

    # Cleanup steps
}

proc log_modify_015_N {} {
    # Identify the test
    tet_infoline log_modify_015_N: Check that the debug routing specification has a valid
    tet_infoline log_modify_015_N: "how" field.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {log modify $DCP_LOG_SRVR_ONE -change $DCP_LOG_COMP_ONE:general.1:foo:/tmp/j} msg] $msg
    } compare_strings {1 {Bad routing specification}}

    # Cleanup steps
}

proc log_modify_016_N {} {
    # Identify the test
    tet_infoline log_modify_016_N: Check that the production routing specification has a valid
    tet_infoline log_modify_016_N: severity field.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {log modify $DCP_LOG_SRVR_ONE -change foo:DISCARD:} msg] $msg
    } compare_strings {1 {Bad routing specification}}

    # Cleanup steps
}

proc log_modify_017_N {} {
    # Identify the test
    tet_infoline log_modify_017_N: Check that the production routing specification has a valid
    tet_infoline log_modify_017_N: "how" field.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test {
        list [catch {log modify $DCP_LOG_SRVR_ONE -change FATAL:foo:} msg] $msg
    } compare_strings {1 {Bad routing specification}}

    # Cleanup steps
}

proc log_modify_018_N {} {
    # Identify the test
    tet_infoline log_modify_018_N: Check that the string binding is complete.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_OBJECT_UUID DCP_LOG_PROTSEQ DCP_LOG_NETADDR}] != 0} {return;}

    # Run the test
    test {
        list [catch {log modify $DCP_LOG_OBJECT_UUID@$DCP_LOG_PROTSEQ:$DCP_LOG_NETADDR -ch FATAL:DISCARD:} msg] $msg
    } compare_strings {1 {Binding incomplete (no object ID and no endpoint)}}

    # Cleanup steps
}

proc log_modify_019_N {} {
    # Identify the test
    tet_infoline log_modify_019_N: Check that when DCE_DEBUG is not defined and a 
    tet_infoline log_modify_019_N: debug routing was specified there is a problem.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_DCE_DEBUG DCP_LOG_SRVR_ONE DCP_LOG_COMP_ONE}] != 0} {return;}
    if {$DCP_LOG_DCE_DEBUG == 0} {
        tet_infoline The DCP_LOG_DCE_DEBUG variable (found in tetexec.cfg)
        tet_infoline is turned off, so this test has no meaning.
        tet_result UNINITIATED
        return
    }

    # Run the test
    test {
        list [catch {log modify $DCP_LOG_SRVR_ONE -ch $DCP_LOG_COMP_ONE:general.5:DISCARD:} msg] $msg
    } compare_strings {1 {Bad routing specification}}

    # Cleanup steps
}

proc log_modify_020_N {} {
    global DCP_CLIENT DCP_CLIENT_PW DCP_NOTROOT DCP_NOTROOT_PW \
           DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE

    # Identify the test
    tet_infoline log_modify_020_N: Check that an unauthenticated user can not modify any
    tet_infoline log_modify_020_N: routing specifications.

    # Setup steps
    if {[dcp_wrap {principal create $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {group add none -member $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {account create $DCP_NOTROOT -group none -organization none -password $DCP_NOTROOT_PW -mypwd $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {login $DCP_NOTROOT -password $DCP_NOTROOT_PW}] != 0} {return;}

    # Run the test
    test {
        list [catch {log modify $DCP_LOG_SRVR_ONE -ch FATAL:BINFILE:/tmp/svc} msg] $msg
    } compare_strings {1 {Must have write permission for the DTS entity}}

    # Cleanup steps
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {account delete $DCP_NOTROOT}] != 0} {return;}
    if {[dcp_wrap {principal delete $DCP_NOTROOT}] != 0} {return;}
}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 ic16 ic17 ic18 ic19 ic20 "
set ic1 "log_modify_001_N"
set ic2 "log_modify_002_N"
set ic3 "log_modify_003_N"
set ic4 "log_modify_004_N"
set ic5 "log_modify_005_N"
set ic6 "log_modify_006_N"
set ic7 "log_modify_007_N"
set ic8 "log_modify_008_N"
set ic9 "log_modify_009_N"
set ic10 "log_modify_010_N"
set ic11 "log_modify_011_N"
set ic12 "log_modify_012_N"
set ic13 "log_modify_013_N"
set ic14 "log_modify_014_N"
set ic15 "log_modify_015_N"
set ic16 "log_modify_016_N"
set ic17 "log_modify_017_N"
set ic18 "log_modify_018_N"
set ic19 "log_modify_019_N"
set ic20 "log_modify_020_N"
set tet_startup log_modify_startup
set tet_cleanup log_modify_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
