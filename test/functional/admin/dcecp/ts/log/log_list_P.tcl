#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: log_list_P.tcl,v $
# Revision 1.1.6.4  1996/03/11  02:17:22  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:24  marty]
#
# Revision 1.1.6.3  1995/12/13  15:26:41  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1994/12/20  20:09 UTC  truitt
# 	Merge.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chk_04/1  1994/12/20  20:08 UTC  truitt
# 	Change tests to match changes in log functionality.
# 
# 	HP revision /main/HPDCE02/1  1994/11/08  18:39 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/08  18:37 UTC  truitt
# 	Make sure output is sorted so the compare works correctly.
# 	[1995/12/11  15:53:43  root]
# 
# Revision 1.1.2.3  1994/10/05  17:22:06  salamone
# 	CR12479- DTS turned on support for audit svc component
# 	[1994/10/05  15:42:49  salamone]
# 
# Revision 1.1.2.2  1994/10/01  01:33:14  rousseau
# 	Dynamically generate string bindings.
# 	[1994/10/01  01:27:36  rousseau]
# 
# Revision 1.1.2.1  1994/09/28  21:40:44  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:58  melman]
# 
# Revision 1.1.2.8  1994/09/20  15:14:42  salamone
# 	Use new string binding constants for dts server.
# 	[1994/09/20  14:44:20  salamone]
# 
# Revision 1.1.2.7  1994/09/08  14:01:00  salamone
# 	CR11968-Setting authentication on binding handles.
# 	[1994/09/08  13:59:30  salamone]
# 
# Revision 1.1.2.6  1994/08/19  18:04:18  salamone
# 	CR10683 - Handle 1 string binding in TCL syntax.
# 	[1994/08/19  17:54:47  salamone]
# 
# Revision 1.1.2.5  1994/06/10  20:13:52  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:46  devsrc]
# 
# Revision 1.1.2.4  1994/05/18  20:41:06  salamone
# 	CR10636 - dcecp "log show" and "log list" should display the
# 		  appropriate info if unauthenticated.
# 	[1994/05/13  22:26:41  salamone]
# 
# Revision 1.1.2.3  1994/05/11  19:38:13  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:22:30  rousseau]
# 
# Revision 1.1.2.2  1994/05/06  15:41:30  melman
# 	updating to new API path
# 	[1994/05/06  15:41:12  melman]
# 
# Revision 1.1.2.1  1994/05/02  19:26:13  salamone
# 	Initial version.
# 	[1994/05/02  19:25:43  salamone]
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
proc log_list_011_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_list_011_P: Display all components for 1 server, which
    tet_infoline log_list_011_P: is not enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { lsort [log list $DCP_LOG_SRVR_ONE]
	  } compare_strings {aud cds dts rpc sec}

    # Cleanup steps
}

proc log_list_012_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_list_012_P: Display all components for 1 server, which
    tet_infoline log_list_012_P: is enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { lsort [log list "$DCP_LOG_SRVR_ONE"]
	  } compare_strings {aud cds dts rpc sec}

    # Cleanup steps
}

proc log_list_013_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_list_013_P: Display all components for more than 1 server,
    tet_infoline log_list_013_P: which are not individually enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test { log list "$DCP_LOG_SRVR_ONE $DCP_LOG_SRVR_TWO" } 

    # Cleanup steps
}

proc log_list_014_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_list_014_P: Display all components for more than 1 server,
    tet_infoline log_list_014_P: which are individually enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {log list [list "$DCP_LOG_SRVR_ONE" "$DCP_LOG_SRVR_TWO"]} 

    # Cleanup steps
}

proc log_list_015_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_list_015_P: Display all sub-components of 1 specified component 
    tet_infoline log_list_015_P: for 1 server, which is not enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {log list $DCP_LOG_SRVR_ONE -comp $DCP_LOG_COMP_ONE} 

    # Cleanup steps
}

proc log_list_016_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_list_016_P: Display all sub-components from the component output of 
    tet_infoline log_list_016_P: another server.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_ONE}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {log list $DCP_LOG_SRVR_ONE -comp [log list $DCP_LOG_SRVR_TWO]}

    # Cleanup steps
}

proc log_list_017_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_list_017_P: Display all sub-components of more than 1 specified component 
    tet_infoline log_list_017_P: for 1 server, which is not enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_COMP_ONE DCP_LOG_COMP_TWO}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test { 
	 log list $DCP_LOG_SRVR_ONE -comp "$DCP_LOG_COMP_ONE $DCP_LOG_COMP_TWO"
    }

    # Cleanup steps
}

proc log_list_018_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_list_018_P: Display all components for more than 1 server,
    tet_infoline log_list_018_P: which are not individually enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test { log list "$DCP_LOG_SRVR_ONE $DCP_LOG_SRVR_TWO"} 

    # Cleanup steps
}

proc log_list_019_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_list_019_P: Display all sub-components of 1 specified component 
    tet_infoline log_list_019_P: for more than 1 server, which is not enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE DCP_LOG_COMP_TWO}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test { 
        log list "$DCP_LOG_SRVR_ONE $DCP_LOG_SRVR_TWO" -comp $DCP_LOG_COMP_TWO
    }

    # Cleanup steps
}

proc log_list_020_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_list_020_P: Display all components for 1 server specified as a 
    tet_infoline log_list_020_P: string binding with just an endpoint in
    tet_infoline log_list_020_P: string syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_PROTSEQ DCP_LOG_NETADDR DCP_LOG_ENDPOINT}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {log list "$DCP_LOG_PROTSEQ:$DCP_LOG_NETADDR\[$DCP_LOG_ENDPOINT\]"}

    # Cleanup steps
}

proc log_list_021_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_list_021_P: Display all components for 1 server specified as a 
    tet_infoline log_list_021_P: string binding with just an endpoint in
    tet_infoline log_list_021_P: TCL syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_PROTSEQ DCP_LOG_NETADDR DCP_LOG_ENDPOINT}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {log list "$DCP_LOG_PROTSEQ $DCP_LOG_NETADDR $DCP_LOG_ENDPOINT"}

    # Cleanup steps
}

proc log_list_022_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_list_022_P: Display all components for 1 server specified as a 
    tet_infoline log_list_022_P: string binding with an endpoint and UUID in
    tet_infoline log_list_022_P: string syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_OBJECT_UUID DCP_LOG_PROTSEQ DCP_LOG_NETADDR DCP_LOG_ENDPOINT}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {log list "$DCP_LOG_OBJECT_UUID@$DCP_LOG_PROTSEQ:$DCP_LOG_NETADDR\[$DCP_LOG_ENDPOINT\]"}

    # Cleanup steps
}

proc log_list_023_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_list_023_P: Display all components for 1 server specified as a 
    tet_infoline log_list_023_P: string binding with an endpoint and UUID in
    tet_infoline log_list_023_P: TCL syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_OBJECT_UUID DCP_LOG_PROTSEQ DCP_LOG_NETADDR DCP_LOG_ENDPOINT}] != 0} {return;}

    # Run the test
    dcp_acl_wrap_test {log list "$DCP_LOG_OBJECT_UUID $DCP_LOG_PROTSEQ $DCP_LOG_NETADDR $DCP_LOG_ENDPOINT"}

    # Cleanup steps
}

proc log_list_024_P {} {
    global TET_ROOT DCP_CLIENT DCP_CLIENT_PW

    # Identify the test
    tet_infoline log_list_024_P: Display all components for 1 server for an
    tet_infoline log_list_024_P: unauthenticated user.

    # Setup steps
    if {[dcp_wrap {global DCP_LOG_SRVR_ONE DCP_LOG_SRVR_TWO DCP_HOSTNAME_ONE}] != 0} {return;}

    # Run the test
    test { lsort [log list $DCP_LOG_SRVR_ONE]
	  } compare_strings {aud cds dts rpc sec}

    # Cleanup steps

}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 "
set ic1 "log_list_011_P"
set ic2 "log_list_012_P"
set ic3 "log_list_013_P"
set ic4 "log_list_014_P"
set ic5 "log_list_015_P"
set ic6 "log_list_016_P"
set ic7 "log_list_017_P"
set ic8 "log_list_018_P"
set ic9 "log_list_019_P"
set ic10 "log_list_020_P"
set ic11 "log_list_021_P"
set ic12 "log_list_022_P"
set ic13 "log_list_023_P"
set ic14 "log_list_024_P"
set tet_startup log_list_startup
set tet_cleanup log_list_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
